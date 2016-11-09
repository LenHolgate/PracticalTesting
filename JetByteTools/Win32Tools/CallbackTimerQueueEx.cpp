///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueEx.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2008 JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors
// shall not be liable for any damages suffered by licensee as a result of
// using the software. In no event will JetByte Limited be liable for any
// lost revenue, profit or data, or for direct, indirect, special,
// consequential, incidental or punitive damages, however caused and regardless
// of the theory of liability, arising out of the use of or inability to use
// software, even if JetByte Limited has been advised of the possibility of
// such damages.
//
///////////////////////////////////////////////////////////////////////////////

#include "JetByteTools\Admin\Admin.h"

#include "CallbackTimerQueueEx.h"
#include "TickCount64Provider.h"
#include "Utils.h"
#include "Exception.h"
#include "NullCallbackTimerQueueMonitor.h"
#include "IntrusiveMultiMapNode.h"

#pragma hdrstop

#pragma warning(disable: 4355) // this used in base member initialiser list

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#include "DebugTrace.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// File level statics
///////////////////////////////////////////////////////////////////////////////

static CNullCallbackTimerQueueMonitor s_monitor;

static const CTickCount64Provider s_tickProvider;

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

static const Milliseconds s_tickCountMax = 0xFFFFFFFF;

static const Milliseconds s_timeoutMax = s_tickCountMax - 1;

///////////////////////////////////////////////////////////////////////////////
// Static members
///////////////////////////////////////////////////////////////////////////////

IQueueTimers::Handle IQueueTimers::InvalidHandleValue = 0;

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueEx::TimerData
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerQueueEx::TimerData : private CIntrusiveRedBlackTreeNode
{
   public :

      template <class TimerData> friend class TIntrusiveRedBlackTreeNodeIsBaseClass;

      TimerData();

      TimerData(
         Timer &timer,
         UserData userData);

      void UpdateData(
         Timer &timer,
         UserData userData);

      void SetTimer(
         const ULONGLONG absoluteTimeout);

      ULONGLONG GetTimeout() const;

      bool IsSet() const;

      void ClearTimer();

      void OnTimer();

      void PrepareForHandleTimeout();

      void HandleTimeout();

      bool DeleteAfterTimeout() const;

      bool HasTimedOut() const;

      void SetDeleteAfterTimeout();

      void TimeoutHandlingComplete();

      void PushNext(
         TimerData *pNext);

      TimerData *GetNext() const;

      TimerData *PopNext();

   private :

      struct Data
      {
         Data();

         Data(
            Timer &timer,
            UserData userData);

         void Clear();

         Timer *pTimer;

         UserData userData;

         ULONGLONG absoluteTimeout;
      };

      void OnTimer(
         const Data &data);

      Data m_active;

      Data m_timedout;

      bool m_deleteAfterTimeout;

      bool m_processingTimeout;

      // For storing in the active handles collection we need a 
      // node.

      friend class CCallbackTimerQueueEx::TimerDataIntrusiveMultiMapNodeKeyAccessor;

     friend class CCallbackTimerQueueEx::TimerDataIntrusiveMultiMapNodeAccessor;

      CIntrusiveMultiMapNode m_timerQueueNode;

      // When processing timers we need to chain them together after removing
      // them from the timer queue. We can't reuse the timer queue node
      // so we link them in an invasive singly linked list using m_pNext

      TimerData *m_pNext;
};

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueEx
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueueEx::CCallbackTimerQueueEx()
   :  m_tickProvider(s_tickProvider),
      m_monitor(s_monitor),
      m_maxTimeout(s_timeoutMax),
      m_handlingTimeouts(false),
      m_pTimeoutsToBeHandled(0)
{

}

CCallbackTimerQueueEx::CCallbackTimerQueueEx(
   IMonitorCallbackTimerQueue &monitor)
   :  m_tickProvider(s_tickProvider),
      m_monitor(monitor),
      m_maxTimeout(s_timeoutMax),
      m_handlingTimeouts(false),
      m_pTimeoutsToBeHandled(0)
{

}

CCallbackTimerQueueEx::CCallbackTimerQueueEx(
   const IProvideTickCount64 &tickProvider)
   :  m_tickProvider(tickProvider),
      m_monitor(s_monitor),
      m_maxTimeout(s_timeoutMax),
      m_handlingTimeouts(false),
      m_pTimeoutsToBeHandled(0)
{

}

CCallbackTimerQueueEx::CCallbackTimerQueueEx(
   IMonitorCallbackTimerQueue &monitor,
   const IProvideTickCount64 &tickProvider)
   :  m_tickProvider(tickProvider),
      m_monitor(monitor),
      m_maxTimeout(s_timeoutMax),
      m_handlingTimeouts(false),
      m_pTimeoutsToBeHandled(0)
{

}

CCallbackTimerQueueEx::~CCallbackTimerQueueEx()
{
   try
   {
      m_queue.Clear();

      ActiveHandles::Iterator it = m_activeHandles.Begin();

      const ActiveHandles::Iterator end = m_activeHandles.End();

      while (it != end)
      {
         ActiveHandles::Iterator next = it + 1;

         TimerData *pData = *it;

         m_activeHandles.Erase(it);

         delete pData;

         #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
         m_monitor.OnTimerDeleted();
         #endif

         it = next;
      }
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
}

CCallbackTimerQueueEx::Handle CCallbackTimerQueueEx::CreateTimer()
{
   TimerData *pData = new TimerData();

   if (!m_activeHandles.Insert(pData).second)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::CreateTimer()"),
         _T("Timer handle: ") + ToString(reinterpret_cast<Handle>(pData)) + _T(" is already in the handle map"));
   }

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerCreated();
   #endif

   return reinterpret_cast<Handle>(pData);
}

bool CCallbackTimerQueueEx::SetTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::SetTimer()"),
         _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

   TimerData *pData = ValidateHandle(handle);

   const bool wasPending = CancelTimer(pData);

   pData->UpdateData(timer, userData);

   InsertTimer(pData, timeout);

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerSet(wasPending);
   #endif

   return wasPending;
}

bool CCallbackTimerQueueEx::CancelTimer(
   const Handle &handle)
{
   const bool wasPending = CancelTimer(ValidateHandle(handle));

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerCancelled(wasPending);
   #endif

   return wasPending;
}

bool CCallbackTimerQueueEx::DestroyTimer(
   Handle &handle)
{
   TimerData *pData = ValidateHandle(handle);

   const bool wasPending = CancelTimer(pData);

   m_activeHandles.Erase(pData);

   handle = InvalidHandleValue;

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerDestroyed(wasPending);
   #endif

   if (pData->HasTimedOut())
   {
      pData->SetDeleteAfterTimeout();
   }
   else
   {
      delete pData;

      #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
      m_monitor.OnTimerDeleted();
      #endif
   }

   return wasPending;
}

bool CCallbackTimerQueueEx::DestroyTimer(
   const Handle &handle)
{
   Handle handle_ = handle;

   return DestroyTimer(handle_);
}

void CCallbackTimerQueueEx::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::SetTimer()"),
         _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

#pragma warning(suppress: 28197) // Possibly leaking memory - No, we're not.
   TimerData *pData = new TimerData(timer, userData);

   if (!m_activeHandles.Insert(pData).second)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::SetTimer()"),
         _T("Timer handle: ") + ToString(reinterpret_cast<Handle>(pData)) + _T(" is already in the handle map"));
   }

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerCreated();
   #endif

   InsertTimer(pData, timeout);

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnOneOffTimerSet();
   #endif
}

Milliseconds CCallbackTimerQueueEx::GetMaximumTimeout() const
{
   return m_maxTimeout;
}

void CCallbackTimerQueueEx::InsertTimer(
   TimerData * const pData,
   const Milliseconds timeout)
{
   const ULONGLONG now = m_tickProvider.GetTickCount64();

   const ULONGLONG absoluteTimeout = now + timeout;

   if (absoluteTimeout < now)
   {
      pData->ClearTimer();

      throw CException(
         _T("CCallbackTimerQueueEx::InsertTimer()"),
         _T("Timeout will extend beyond the wrap point of GetTickCount64(). ")
         _T("Well done at having your machine running for this long, ")
         _T("but this is outside of our specificiation..."));
   }

   InsertTimer(pData, absoluteTimeout);
}

void CCallbackTimerQueueEx::InsertTimer(
   TimerData * const pData,
   const ULONGLONG absoluteTimeout)
{
   pData->SetTimer(absoluteTimeout);

   m_queue.Insert(pData); //lint !e534 (Ignoring return value of function)
} //lint !e429 (Custodial pointer 'pData' has not been freed or returned)

CCallbackTimerQueueEx::TimerData *CCallbackTimerQueueEx::ValidateHandle(
   const Handle &handle)
{
   TimerData *pData = reinterpret_cast<TimerData *>(handle);

   #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES == 1)
   ActiveHandles::Iterator it = m_activeHandles.Find(pData);

   if (it == m_activeHandles.End())
   {
      // The following warning is generated when /Wp64 is set in a 32bit build. At present I think
      // it's due to some confusion, and even if it isn't then it's not that crucial...
      #pragma warning(push, 4)
      #pragma warning(disable: 4244)
      throw CException(
         _T("CCallbackTimerQueueEx::ValidateHandle()"),
         _T("Invalid timer handle: ") + ToString(handle));
      #pragma warning(pop)
   }
   #endif

   return pData;
}

bool CCallbackTimerQueueEx::CancelTimer(
   TimerData *pData)
{
   bool wasPending = false;

   if (pData->IsSet())
   {
      m_queue.Erase(pData);

      pData->ClearTimer();

      wasPending = true;
   }

   return wasPending;
}

Milliseconds CCallbackTimerQueueEx::GetNextTimeout()
{
   Milliseconds timeUntilTimeout = INFINITE;

   TimerQueue::Iterator it = m_queue.Begin();

   if (it != m_queue.End())
   {
      const ULONGLONG now = m_tickProvider.GetTickCount64();

      const ULONGLONG timeout = it->GetTimeout();

      const ULONGLONG ticksUntilTimeout = timeout - now;

      if (ticksUntilTimeout > s_timeoutMax)
      {
         timeUntilTimeout = 0;
      }
      else
      {
         timeUntilTimeout = static_cast<Milliseconds>(ticksUntilTimeout);
      }
   }

   return timeUntilTimeout;
}

bool CCallbackTimerQueueEx::BeginTimeoutHandling()
{
   if (m_handlingTimeouts)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::BeginTimeoutHandling()"),
         _T("Already handling timeouts, you need to call EndTimeoutHandling()?"));
   }

   if (0 == GetNextTimeout())
   {
      TimerQueue::NodeCollection timers;

      m_queue.RemoveAll(m_queue.Begin(), timers);

      // Need to duplicate the timer data so that a call to SetTimer that occurs after
      // this call returns but before a call to HandleTimeout with this timer doesn't
      // cause the timer that is about to happen to be changed before it actually
      // "goes off"...

      // Need to remove all of these timers from the NodeCollection before traversing them and firing them
      // as they cannot be set again if they are still in the collection...

      TimerData *pLastTimer = 0;

      TimerData *pTimer = timers.Pop();

      while (pTimer)
      {
         pTimer->PrepareForHandleTimeout();

         if (pTimer->DeleteAfterTimeout())
         {
            m_activeHandles.Erase(pTimer);

            // pData will be cleaned up after we've processed the timeout
         }

         // Store the timeouts that we need to handle in an invasive singly
         // linked list of timers...

         if (!m_pTimeoutsToBeHandled)
         {
            m_pTimeoutsToBeHandled = pTimer;
         }
         else
         {
            pLastTimer->PushNext(pTimer); //lint !e613 (Possible use of null pointer 'pLastTimer')
         }

         pLastTimer = pTimer;

         pTimer = timers.Pop();
      }
   }

   m_handlingTimeouts = (m_pTimeoutsToBeHandled != 0);

   return m_handlingTimeouts;
}

void CCallbackTimerQueueEx::HandleTimeout()
{
   if (!m_handlingTimeouts)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::ValidateTimeoutHandle()"),
         _T("Not currently handling timeouts, you need to call BeginTimeoutHandling()?"));
   }

   TimerData *pTimer = m_pTimeoutsToBeHandled;

   while (pTimer)
   {
      pTimer->HandleTimeout();

      #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
      m_monitor.OnTimer();
      #endif

      pTimer = pTimer->GetNext();
   }
}

void CCallbackTimerQueueEx::EndTimeoutHandling()
{
   if (!m_handlingTimeouts)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::EndTimeoutHandling()"),
         _T("Not currently handling timeouts, you need to call BeginTimeoutHandling()?"));
   }

   m_handlingTimeouts = false;

   TimerData *pTimer = m_pTimeoutsToBeHandled;

   m_pTimeoutsToBeHandled = 0;

   while (pTimer)
   {
      TimerData *pNextTimer = pTimer->PopNext();

      if (pTimer->DeleteAfterTimeout())
      {
         delete pTimer;

         #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
         m_monitor.OnTimerDeleted();
         #endif
      }
      else
      {
         pTimer->TimeoutHandlingComplete();
      }

      pTimer = pNextTimer;
   }
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueEx::TimerData
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueueEx::TimerData::TimerData()
   :  m_deleteAfterTimeout(false),
      m_processingTimeout(false),
      m_pNext(0)
{
}

CCallbackTimerQueueEx::TimerData::TimerData(
   Timer &timer,
   UserData userData)
   :  m_active(timer, userData),
      m_deleteAfterTimeout(true),
      m_processingTimeout(false),
      m_pNext(0)
{
}

void CCallbackTimerQueueEx::TimerData::PushNext(
   TimerData *pNext)
{
   if (m_pNext)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::TimerData::PushNext()"),
         _T("Internal Error: Next is already set"));
   }

   m_pNext = pNext;
}

CCallbackTimerQueueEx::TimerData *CCallbackTimerQueueEx::TimerData::GetNext() const
{
   return m_pNext;
}

CCallbackTimerQueueEx::TimerData *CCallbackTimerQueueEx::TimerData::PopNext()
{
   TimerData *pNext = m_pNext;

   m_pNext = 0;

   return pNext;
}

void CCallbackTimerQueueEx::TimerData::UpdateData(
   Timer &timer,
   UserData userData)
{
   if (m_deleteAfterTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::TimerData::UpdateData()"),
         _T("Internal Error: Can't update one shot timers or timers pending deletion"));
   }

   m_active.pTimer = &timer;
   m_active.userData = userData;
}

void CCallbackTimerQueueEx::TimerData::SetTimer(
   ULONGLONG absoluteTimeout)
{
   m_active.absoluteTimeout = absoluteTimeout;
}

ULONGLONG CCallbackTimerQueueEx::TimerData::GetTimeout() const
{
   return m_active.absoluteTimeout;
}

bool CCallbackTimerQueueEx::TimerData::IsSet() const
{
   return (m_active.pTimer != 0);
}

void CCallbackTimerQueueEx::TimerData::ClearTimer()
{
   m_active.Clear();
}

void CCallbackTimerQueueEx::TimerData::OnTimer()
{
   m_processingTimeout = true;

   OnTimer(m_active);

   m_active.Clear();

   m_processingTimeout = false;
}

void CCallbackTimerQueueEx::TimerData::OnTimer(
   const Data &data)
{
   if (!data.pTimer)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::TimerData::OnTimer()"),
         _T("Internal Error: Timer not set"));
   }

   data.pTimer->OnTimer(data.userData);
}

void CCallbackTimerQueueEx::TimerData::PrepareForHandleTimeout()
{
   m_processingTimeout = true;

   m_timedout = m_active;

   m_active.Clear();
}

void CCallbackTimerQueueEx::TimerData::HandleTimeout()
{
   OnTimer(m_timedout);

   m_timedout.Clear();
}

void CCallbackTimerQueueEx::TimerData::TimeoutHandlingComplete()
{
   m_processingTimeout = false;
}

bool CCallbackTimerQueueEx::TimerData::DeleteAfterTimeout() const
{
   return m_deleteAfterTimeout;
}

bool CCallbackTimerQueueEx::TimerData::HasTimedOut() const
{
   return m_processingTimeout;
}

void CCallbackTimerQueueEx::TimerData::SetDeleteAfterTimeout()
{
   m_deleteAfterTimeout = true;
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueEx::TimerData::Data
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueueEx::TimerData::Data::Data()
   :  pTimer(0),
      userData(0),
      absoluteTimeout(0)
{

}

CCallbackTimerQueueEx::TimerData::Data::Data(
   Timer &timer,
   UserData userData_)
   :  pTimer(&timer),
      userData(userData_),
      absoluteTimeout(0)
{

}

void CCallbackTimerQueueEx::TimerData::Data::Clear()
{
   pTimer = 0;
   userData = 0;
   absoluteTimeout = 0;
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueEx::TimerDataIntrusiveMultiMapNodeKeyAccessor
///////////////////////////////////////////////////////////////////////////////

ULONGLONG CCallbackTimerQueueEx::TimerDataIntrusiveMultiMapNodeKeyAccessor::GetKeyFromT(
   const CCallbackTimerQueueEx::TimerData *pNode)
{
   return pNode->GetTimeout();
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueEx::TimerDataIntrusiveMultiMapNodeAccessor
///////////////////////////////////////////////////////////////////////////////

CIntrusiveMultiMapNode *CCallbackTimerQueueEx::TimerDataIntrusiveMultiMapNodeAccessor::GetNodeFromT(
   const TimerData *pData)
{
   CIntrusiveMultiMapNode *pNode = 0;

   if (pData)
   {
      pNode = &const_cast<TimerData *>(pData)->m_timerQueueNode;
   }

   return pNode;
}

CCallbackTimerQueueEx::TimerData *CCallbackTimerQueueEx::TimerDataIntrusiveMultiMapNodeAccessor::GetTFromNode(
   const CIntrusiveMultiMapNode *pNode)
{
   TimerData *pData = 0;

   if (pNode)
   {
      pData = CONTAINING_RECORD(const_cast<CIntrusiveMultiMapNode *>(pNode), TimerData, m_timerQueueNode); //lint !e413 (Likely use of null pointer)
   }

   return pData;
}

CCallbackTimerQueueEx::TimerData *CCallbackTimerQueueEx::TimerDataIntrusiveMultiMapNodeAccessor::GetTFromNode(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   TimerData *pData = 0;

   if (pNode)
   {
      pData = CONTAINING_RECORD(const_cast<CIntrusiveMultiMapNode *>(static_cast<const CIntrusiveMultiMapNode *>(pNode)), TimerData, m_timerQueueNode); //lint !e413 (Likely use of null pointer)
   }

   return pData;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
