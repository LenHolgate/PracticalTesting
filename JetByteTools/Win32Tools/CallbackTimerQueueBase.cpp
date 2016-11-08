///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueBase.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
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

#include "CallbackTimerQueueBase.h"
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

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

static const Milliseconds s_tickCountMax = 0xFFFFFFFF;

static const Milliseconds s_timeoutMax = s_tickCountMax - 1;

///////////////////////////////////////////////////////////////////////////////
// Static members
///////////////////////////////////////////////////////////////////////////////

IQueueTimers::Handle IQueueTimers::InvalidHandleValue = 0;

IManageTimerQueue::TimeoutHandle IManageTimerQueue::InvalidTimeoutHandleValue = 0;

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueBase::TimerData
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerQueueBase::TimerData : private CIntrusiveRedBlackTreeNode
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

      bool IsInternalTimer(
         const CCallbackTimerQueueBase *pQueue) const;

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

      friend class CCallbackTimerQueueBase::TimerDataIntrusiveMultiMapNodeKeyAccessor;

      friend class CCallbackTimerQueueBase::TimerDataIntrusiveMultiMapNodeAccessor;

      CIntrusiveMultiMapNode m_timerQueueNode;

      // When processing timers we need to chain them together after removing
      // them from the timer queue. We can't reuse the timer queue node
      // so we link them in an invasive singly linked list using m_pNext

      TimerData *m_pNext;
};

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueBase
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueueBase::CCallbackTimerQueueBase()
   :  m_monitor(s_monitor),
      m_maxTimeout(s_timeoutMax),
      m_handlingTimeouts(InvalidTimeoutHandleValue),
      m_nextTimeoutHandle(reinterpret_cast<ULONG_PTR>(this)),
      m_pTimeoutsToBeHandled(0)
{

}

CCallbackTimerQueueBase::CCallbackTimerQueueBase(
   IMonitorCallbackTimerQueue &monitor)
   :  m_monitor(monitor),
      m_maxTimeout(s_timeoutMax),
      m_handlingTimeouts(InvalidTimeoutHandleValue),
      m_nextTimeoutHandle(reinterpret_cast<ULONG_PTR>(this)),
      m_pTimeoutsToBeHandled(0)
{

}

CCallbackTimerQueueBase::~CCallbackTimerQueueBase()
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

CCallbackTimerQueueBase::Handle CCallbackTimerQueueBase::CreateTimer()
{
   TimerData *pData = new TimerData();

   if (!m_activeHandles.Insert(pData).second)
   {
      throw CException(
         _T("CCallbackTimerQueueBase::CreateTimerInternal()"),
         _T("Timer handle: ") + ToString(reinterpret_cast<Handle>(pData)) + _T(" is already in the handle map"));
   }

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerCreated();
   #endif

   return reinterpret_cast<Handle>(pData);
}

bool CCallbackTimerQueueBase::SetTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueueBase::SetTimer()"),
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

bool CCallbackTimerQueueBase::CancelTimer(
   const Handle &handle)
{
   const bool wasPending = CancelTimer(ValidateHandle(handle));

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerCancelled(wasPending);
   #endif

   return wasPending;
}

bool CCallbackTimerQueueBase::DestroyTimer(
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

bool CCallbackTimerQueueBase::DestroyTimer(
   const Handle &handle)
{
   Handle handle_ = handle;

   return DestroyTimer(handle_);
}

void CCallbackTimerQueueBase::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueueBase::SetTimer()"),
         _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

#pragma warning(suppress: 28197) // Possibly leaking memory - No, we're not.
   TimerData *pData = new TimerData(timer, userData);

   if (!m_activeHandles.Insert(pData).second)
   {
      throw CException(
         _T("CCallbackTimerQueueBase::SetTimer()"),
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

Milliseconds CCallbackTimerQueueBase::GetMaximumTimeout() const
{
   return m_maxTimeout;
}

bool CCallbackTimerQueueBase::DispatchesWithoutLock() const
{
   return true;
}

void CCallbackTimerQueueBase::InsertTimer(
   TimerData * const pData,
   const Milliseconds timeout)
{
   const ULONGLONG now = GetTickCount64();

   const ULONGLONG absoluteTimeout = now + timeout;

   if (absoluteTimeout < now)
   {
      pData->ClearTimer();

      throw CException(
         _T("CCallbackTimerQueueBase::InsertTimer()"),
         _T("Timeout will extend beyond the wrap point of GetTickCount64(). ")
         _T("Well done at having your machine running for this long, ")
         _T("but this is outside of our specificiation..."));
   }

   InsertTimer(pData, absoluteTimeout);
}

void CCallbackTimerQueueBase::InsertTimer(
   TimerData * const pData,
   const ULONGLONG absoluteTimeout)
{
   pData->SetTimer(absoluteTimeout);

   m_queue.Insert(pData); //lint !e534 (Ignoring return value of function)
} //lint !e429 (Custodial pointer 'pData' has not been freed or returned)

bool CCallbackTimerQueueBase::SetInternalTimer(
   const Handle &handle,
   Timer &timer,
   const ULONGLONG absoluteTimeout)
{
   TimerData *pData = ValidateHandle(handle);

   const bool wasPending = CancelTimer(pData);

   pData->UpdateData(timer, reinterpret_cast<UserData>(this));

   InsertTimer(pData, absoluteTimeout);

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerSet(wasPending);
   #endif

   return wasPending;
}

CCallbackTimerQueueBase::TimerData *CCallbackTimerQueueBase::ValidateHandle(
   const Handle &handle)
{
   TimerData *pData = reinterpret_cast<TimerData *>(handle);

   ActiveHandles::Iterator it = m_activeHandles.Find(pData);

   if (it == m_activeHandles.End())
   {
      // The following warning is generated when /Wp64 is set in a 32bit build. At present I think
      // it's due to some confusion, and even if it isn't then it's not that crucial...
      #pragma warning(push, 4)
      #pragma warning(disable: 4244)
      throw CException(
         _T("CCallbackTimerQueueBase::ValidateHandle()"),
         _T("Invalid timer handle: ") + ToString(handle));
      #pragma warning(pop)
   }

   return pData;
}

bool CCallbackTimerQueueBase::CancelTimer(
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

Milliseconds CCallbackTimerQueueBase::GetNextTimeout()
{
   Milliseconds timeUntilTimeout = INFINITE;

   TimerQueue::Iterator it = m_queue.Begin();

   if (it != m_queue.End())
   {
      if (it->IsInternalTimer(this))
      {
         it++;
      }

      if (it != m_queue.End())
      {
         const ULONGLONG now = GetTickCount64();

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
   }

   return timeUntilTimeout;
}

void CCallbackTimerQueueBase::HandleTimeouts()
{
   while (0 == GetNextTimeout())
   {
      TimerQueue::NodeCollection timers;

      m_queue.RemoveAll(m_queue.Begin(), timers);

      TimerData *pData = timers.Pop();

      while (pData)
      {
         pData->OnTimer();

         #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
         m_monitor.OnTimer();
         #endif

         if (pData->DeleteAfterTimeout())
         {
            m_activeHandles.Erase(pData);

            delete pData;

            #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
            m_monitor.OnTimerDeleted();
            #endif
         }

         pData = timers.Pop();
      }
   }
}

IManageTimerQueue::TimeoutHandle CCallbackTimerQueueBase::BeginTimeoutHandling()
{
   if (m_handlingTimeouts != InvalidTimeoutHandleValue)
   {
      throw CException(
         _T("CCallbackTimerQueueBase::BeginTimeoutHandling()"),
         _T("Already handling timeouts, you need to call EndTimeoutHandling()?"));
   }

   if (0 == GetNextTimeout())
   {
      TimerQueue::NodeCollection timers;

      m_queue.RemoveAll(m_queue.Begin(), timers);

      m_handlingTimeouts = ::InterlockedIncrement(reinterpret_cast<unsigned long*>(&m_nextTimeoutHandle));

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

   return m_handlingTimeouts;
}

void CCallbackTimerQueueBase::HandleTimeout(
   IManageTimerQueue::TimeoutHandle &handle)
{
   if (m_handlingTimeouts != handle)
   {
      throw CException(
         _T("CCallbackTimerQueueBase::ValidateTimeoutHandle()"),
         _T("Invalid timeout handle: ") + ToString(handle));
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

void CCallbackTimerQueueBase::EndTimeoutHandling(
   IManageTimerQueue::TimeoutHandle &handle)
{
   if (m_handlingTimeouts != handle)
   {
      throw CException(
         _T("CCallbackTimerQueueBase::EndTimeoutHandling()"),
         _T("Invalid timeout handle: ") + ToString(handle));
   }

   m_handlingTimeouts = InvalidTimeoutHandleValue;

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
// CCallbackTimerQueueBase::TimerData
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueueBase::TimerData::TimerData()
   :  m_deleteAfterTimeout(false),
      m_processingTimeout(false),
      m_pNext(0)
{
}

CCallbackTimerQueueBase::TimerData::TimerData(
   Timer &timer,
   UserData userData)
   :  m_active(timer, userData),
      m_deleteAfterTimeout(true),
      m_processingTimeout(false),
      m_pNext(0)
{
}

void CCallbackTimerQueueBase::TimerData::PushNext(
   TimerData *pNext)
{
   if (m_pNext)
   {
      throw CException(
         _T("CCallbackTimerQueueBase::TimerData::PushNext()"),
         _T("Internal Error: Next is already set"));
   }

   m_pNext = pNext;
}

CCallbackTimerQueueBase::TimerData *CCallbackTimerQueueBase::TimerData::GetNext() const
{
   return m_pNext;
}

CCallbackTimerQueueBase::TimerData *CCallbackTimerQueueBase::TimerData::PopNext()
{
   TimerData *pNext = m_pNext;

   m_pNext = 0;

   return pNext;
}

void CCallbackTimerQueueBase::TimerData::UpdateData(
   Timer &timer,
   UserData userData)
{
   if (m_deleteAfterTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueueBase::TimerData::UpdateData()"),
         _T("Internal Error: Can't update one shot timers or timers pending deletion"));
   }

   m_active.pTimer = &timer;
   m_active.userData = userData;
}

void CCallbackTimerQueueBase::TimerData::SetTimer(
   ULONGLONG absoluteTimeout)
{
   m_active.absoluteTimeout = absoluteTimeout;
}

ULONGLONG CCallbackTimerQueueBase::TimerData::GetTimeout() const
{
   return m_active.absoluteTimeout;
}

bool CCallbackTimerQueueBase::TimerData::IsSet() const
{
   return (m_active.pTimer != 0);
}

void CCallbackTimerQueueBase::TimerData::ClearTimer()
{
   m_active.Clear();
}

void CCallbackTimerQueueBase::TimerData::OnTimer()
{
   m_processingTimeout = true;

   OnTimer(m_active);

   m_active.Clear();

   m_processingTimeout = false;
}

void CCallbackTimerQueueBase::TimerData::OnTimer(
   const Data &data)
{
   if (!data.pTimer)
   {
      throw CException(
         _T("CCallbackTimerQueueBase::TimerData::OnTimer()"),
         _T("Internal Error: Timer not set"));
   }

   data.pTimer->OnTimer(data.userData);
}

void CCallbackTimerQueueBase::TimerData::PrepareForHandleTimeout()
{
   m_processingTimeout = true;

   m_timedout = m_active;

   m_active.Clear();
}

void CCallbackTimerQueueBase::TimerData::HandleTimeout()
{
   OnTimer(m_timedout);

   m_timedout.Clear();
}

void CCallbackTimerQueueBase::TimerData::TimeoutHandlingComplete()
{
   m_processingTimeout = false;
}

bool CCallbackTimerQueueBase::TimerData::DeleteAfterTimeout() const
{
   return m_deleteAfterTimeout;
}

bool CCallbackTimerQueueBase::TimerData::IsInternalTimer(
   const CCallbackTimerQueueBase *pQueue) const
{
   return m_active.userData == reinterpret_cast<UserData>(pQueue);
}

bool CCallbackTimerQueueBase::TimerData::HasTimedOut() const
{
   return m_processingTimeout;
}

void CCallbackTimerQueueBase::TimerData::SetDeleteAfterTimeout()
{
   m_deleteAfterTimeout = true;
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueBase::TimerData::Data
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueueBase::TimerData::Data::Data()
   :  pTimer(0),
      userData(0),
      absoluteTimeout(0)
{

}

CCallbackTimerQueueBase::TimerData::Data::Data(
   Timer &timer,
   UserData userData_)
   :  pTimer(&timer),
      userData(userData_),
      absoluteTimeout(0)
{

}

void CCallbackTimerQueueBase::TimerData::Data::Clear()
{
   pTimer = 0;
   userData = 0;
   absoluteTimeout = 0;
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueBase::TimerDataIntrusiveMultiMapNodeKeyAccessor
///////////////////////////////////////////////////////////////////////////////

ULONGLONG CCallbackTimerQueueBase::TimerDataIntrusiveMultiMapNodeKeyAccessor::GetKeyFromT(
   const CCallbackTimerQueueBase::TimerData *pNode)
{
   return pNode->GetTimeout();
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueBase::TimerDataIntrusiveMultiMapNodeAccessor
///////////////////////////////////////////////////////////////////////////////

CIntrusiveMultiMapNode *CCallbackTimerQueueBase::TimerDataIntrusiveMultiMapNodeAccessor::GetNodeFromT(
   const TimerData *pData)
{
   CIntrusiveMultiMapNode *pNode = 0;

   if (pData)
   {
      pNode = &const_cast<TimerData *>(pData)->m_timerQueueNode;
   }

   return pNode;
}

CCallbackTimerQueueBase::TimerData *CCallbackTimerQueueBase::TimerDataIntrusiveMultiMapNodeAccessor::GetTFromNode(
   const CIntrusiveMultiMapNode *pNode)
{
   TimerData *pData = 0;

   if (pNode)
   {
      pData = CONTAINING_RECORD(const_cast<CIntrusiveMultiMapNode *>(pNode), TimerData, m_timerQueueNode); //lint !e413 (Likely use of null pointer)
   }

   return pData;
}

CCallbackTimerQueueBase::TimerData *CCallbackTimerQueueBase::TimerDataIntrusiveMultiMapNodeAccessor::GetTFromNode(
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
// End of file: CallbackTimerQueueBase.cpp
///////////////////////////////////////////////////////////////////////////////
