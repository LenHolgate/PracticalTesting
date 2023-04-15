///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2008 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#include "JetByteTools/Admin/Admin.h"

#include "ContainingRecord.h"
#include "CallbackTimerQueue.h"
#include "TickCount64Provider.h"
#include "ToString.h"
#include "Exception.h"
#include "NullCallbackTimerQueueMonitor.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

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
// CCallbackTimerQueue::TimerData
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerQueue::TimerData : private CIntrusiveRedBlackTreeNode
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
         ULONGLONG absoluteTimeout);

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

      friend class TimerDataIntrusiveMultiMapNodeKeyAccessor;

      friend class TimerDataIntrusiveMultiMapNodeAccessor;

      CIntrusiveMultiMapNode m_timerQueueNode;

      // When processing timers we need to chain them together after removing
      // them from the timer queue. We can't reuse the timer queue node
      // so we link them in an invasive singly linked list using m_pNext

      TimerData *m_pNext;
};

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::CCallbackTimerQueue()
   :  m_tickProvider(s_tickProvider),
      m_monitor(s_monitor),
      m_maxTimeout(s_timeoutMax),
      m_handlingTimeouts(false),
      m_pTimeoutsToBeHandled(nullptr)
{

}

CCallbackTimerQueue::CCallbackTimerQueue(
   IMonitorCallbackTimerQueue &monitor)
   :  m_tickProvider(s_tickProvider),
      m_monitor(monitor),
      m_maxTimeout(s_timeoutMax),
      m_handlingTimeouts(false),
      m_pTimeoutsToBeHandled(nullptr)
{

}

CCallbackTimerQueue::CCallbackTimerQueue(
   const IProvideTickCount64 &tickProvider)
   :  m_tickProvider(tickProvider),
      m_monitor(s_monitor),
      m_maxTimeout(s_timeoutMax),
      m_handlingTimeouts(false),
      m_pTimeoutsToBeHandled(nullptr)
{

}

CCallbackTimerQueue::CCallbackTimerQueue(
   IMonitorCallbackTimerQueue &monitor,
   const IProvideTickCount64 &tickProvider)
   :  m_tickProvider(tickProvider),
      m_monitor(monitor),
      m_maxTimeout(s_timeoutMax),
      m_handlingTimeouts(false),
      m_pTimeoutsToBeHandled(nullptr)
{

}

CCallbackTimerQueue::~CCallbackTimerQueue()
{
   try
   {
      m_queue.Clear();

      ActiveHandles::Iterator it = m_activeHandles.Begin();

      const ActiveHandles::Iterator end = m_activeHandles.End();

      while (it != end)
      {
         const ActiveHandles::Iterator next = it + 1;

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

CCallbackTimerQueue::Handle CCallbackTimerQueue::CreateTimer()
{
   auto *pData = new TimerData();

   if (!m_activeHandles.Insert(pData).second)
   {
      throw CException(
         _T("CCallbackTimerQueue::CreateTimer()"),
         _T("Timer handle: ") + ToString(reinterpret_cast<Handle>(pData)) + _T(" is already in the handle map"));
   }

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerCreated();
   #endif

   return reinterpret_cast<Handle>(pData);
}

bool CCallbackTimerQueue::SetTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueue::SetTimer()"),
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

bool CCallbackTimerQueue::CancelTimer(
   const Handle &handle)
{
   const bool wasPending = CancelTimer(ValidateHandle(handle));

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerCancelled(wasPending);
   #endif

   return wasPending;
}

bool CCallbackTimerQueue::DestroyTimer(
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

bool CCallbackTimerQueue::DestroyTimer(
   const Handle &handle)
{
   Handle handle_ = handle;

   return DestroyTimer(handle_);
}

void CCallbackTimerQueue::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueue::SetTimer()"),
         _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

#pragma warning(suppress: 28197) // Possibly leaking memory - No, we're not.
   auto *pData = new TimerData(timer, userData);

   if (!m_activeHandles.Insert(pData).second)
   {
      throw CException(
         _T("CCallbackTimerQueue::SetTimer()"),
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

Milliseconds CCallbackTimerQueue::GetMaximumTimeout() const
{
   return m_maxTimeout;
}

void CCallbackTimerQueue::InsertTimer(
   TimerData * const pData,
   const Milliseconds timeout)
{
   const ULONGLONG now = m_tickProvider.GetTickCount64();

   const ULONGLONG absoluteTimeout = now + timeout;

   if (absoluteTimeout < now)
   {
      pData->ClearTimer();

      throw CException(
         _T("CCallbackTimerQueue::InsertTimer()"),
         _T("Timeout will extend beyond the wrap point of GetTickCount64(). ")
         _T("Well done at having your machine running for this long, ")
         _T("but this is outside of our specificiation..."));
   }

   InsertTimer(pData, absoluteTimeout);
}

void CCallbackTimerQueue::InsertTimer(
   TimerData * const pData,
   const ULONGLONG absoluteTimeout)
{
   pData->SetTimer(absoluteTimeout);

   m_queue.Insert(pData);
}

CCallbackTimerQueue::TimerData *CCallbackTimerQueue::ValidateHandle(
   const Handle &handle) const
{
   auto *pData = reinterpret_cast<TimerData *>(handle);

   #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES == 1)
   const ActiveHandles::Iterator it = m_activeHandles.Find(pData);

   if (it == m_activeHandles.End())
   {
      // The following warning is generated when /Wp64 is set in a 32bit build. At present I think
      // it's due to some confusion, and even if it isn't then it's not that crucial...
      #pragma warning(push, 4)
      #pragma warning(disable: 4244)
      throw CException(
         _T("CCallbackTimerQueue::ValidateHandle()"),
         _T("Invalid timer handle: ") + ToString(handle));
      #pragma warning(pop)
   }
   #endif

   return pData;
}

bool CCallbackTimerQueue::CancelTimer(
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

Milliseconds CCallbackTimerQueue::GetNextTimeout()
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

bool CCallbackTimerQueue::BeginTimeoutHandling()
{
   if (m_handlingTimeouts)
   {
      throw CException(
         _T("CCallbackTimerQueue::BeginTimeoutHandling()"),
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

      TimerData *pLastTimer = nullptr;

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
            pLastTimer->PushNext(pTimer);
         }

         pLastTimer = pTimer;

         pTimer = timers.Pop();
      }
   }

   m_handlingTimeouts = (m_pTimeoutsToBeHandled != nullptr);

   return m_handlingTimeouts;
}

void CCallbackTimerQueue::HandleTimeout()
{
   if (!m_handlingTimeouts)
   {
      throw CException(
         _T("CCallbackTimerQueue::ValidateTimeoutHandle()"),
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

void CCallbackTimerQueue::EndTimeoutHandling()
{
   if (!m_handlingTimeouts)
   {
      throw CException(
         _T("CCallbackTimerQueue::EndTimeoutHandling()"),
         _T("Not currently handling timeouts, you need to call BeginTimeoutHandling()?"));
   }

   m_handlingTimeouts = false;

   TimerData *pTimer = m_pTimeoutsToBeHandled;

   m_pTimeoutsToBeHandled = nullptr;

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
// CCallbackTimerQueue::TimerData
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::TimerData::TimerData()
   :  m_deleteAfterTimeout(false),
      m_processingTimeout(false),
      m_pNext(nullptr)
{
}

CCallbackTimerQueue::TimerData::TimerData(
   Timer &timer,
   const UserData userData)
   :  m_active(timer, userData),
      m_deleteAfterTimeout(true),
      m_processingTimeout(false),
      m_pNext(nullptr)
{
}

void CCallbackTimerQueue::TimerData::PushNext(
   TimerData *pNext)
{
   if (m_pNext)
   {
      throw CException(
         _T("CCallbackTimerQueue::TimerData::PushNext()"),
         _T("Internal Error: Next is already set"));
   }

   m_pNext = pNext;
}

CCallbackTimerQueue::TimerData *CCallbackTimerQueue::TimerData::GetNext() const
{
   return m_pNext;
}

CCallbackTimerQueue::TimerData *CCallbackTimerQueue::TimerData::PopNext()
{
   TimerData *pNext = m_pNext;

   m_pNext = nullptr;

   return pNext;
}

void CCallbackTimerQueue::TimerData::UpdateData(
   Timer &timer,
   const UserData userData)
{
   if (m_deleteAfterTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueue::TimerData::UpdateData()"),
         _T("Internal Error: Can't update one shot timers or timers pending deletion"));
   }

   m_active.pTimer = &timer;
   m_active.userData = userData;
}

void CCallbackTimerQueue::TimerData::SetTimer(
   const ULONGLONG absoluteTimeout)
{
   m_active.absoluteTimeout = absoluteTimeout;
}

ULONGLONG CCallbackTimerQueue::TimerData::GetTimeout() const
{
   return m_active.absoluteTimeout;
}

bool CCallbackTimerQueue::TimerData::IsSet() const
{
   return (m_active.pTimer != nullptr);
}

void CCallbackTimerQueue::TimerData::ClearTimer()
{
   m_active.Clear();
}

void CCallbackTimerQueue::TimerData::OnTimer()
{
   m_processingTimeout = true;

   OnTimer(m_active);

   m_active.Clear();

   m_processingTimeout = false;
}

void CCallbackTimerQueue::TimerData::OnTimer(
   const Data &data)
{
   if (!data.pTimer)
   {
      throw CException(
         _T("CCallbackTimerQueue::TimerData::OnTimer()"),
         _T("Internal Error: Timer not set"));
   }

   data.pTimer->OnTimer(data.userData);
}

void CCallbackTimerQueue::TimerData::PrepareForHandleTimeout()
{
   m_processingTimeout = true;

   m_timedout = m_active;

   m_active.Clear();
}

void CCallbackTimerQueue::TimerData::HandleTimeout()
{
   OnTimer(m_timedout);

   m_timedout.Clear();
}

void CCallbackTimerQueue::TimerData::TimeoutHandlingComplete()
{
   m_processingTimeout = false;
}

bool CCallbackTimerQueue::TimerData::DeleteAfterTimeout() const
{
   return m_deleteAfterTimeout;
}

bool CCallbackTimerQueue::TimerData::HasTimedOut() const
{
   return m_processingTimeout;
}

void CCallbackTimerQueue::TimerData::SetDeleteAfterTimeout()
{
   m_deleteAfterTimeout = true;
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::TimerData::Data
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::TimerData::Data::Data()
   :  pTimer(nullptr),
      userData(0),
      absoluteTimeout(0)
{

}

CCallbackTimerQueue::TimerData::Data::Data(
   Timer &timer,
   const UserData userData_)
   :  pTimer(&timer),
      userData(userData_),
      absoluteTimeout(0)
{

}

void CCallbackTimerQueue::TimerData::Data::Clear()
{
   pTimer = nullptr;
   userData = 0;
   absoluteTimeout = 0;
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::TimerDataIntrusiveMultiMapNodeKeyAccessor
///////////////////////////////////////////////////////////////////////////////

ULONGLONG CCallbackTimerQueue::TimerDataIntrusiveMultiMapNodeKeyAccessor::GetKeyFromT(
   const TimerData *pNode)
{
   return pNode->GetTimeout();
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::TimerDataIntrusiveMultiMapNodeAccessor
///////////////////////////////////////////////////////////////////////////////

CIntrusiveMultiMapNode *CCallbackTimerQueue::TimerDataIntrusiveMultiMapNodeAccessor::GetNodeFromT(
   const TimerData *pData)
{
   CIntrusiveMultiMapNode *pNode = nullptr;

   if (pData)
   {
      pNode = &const_cast<TimerData *>(pData)->m_timerQueueNode;
   }

   return pNode;
}

CCallbackTimerQueue::TimerData *CCallbackTimerQueue::TimerDataIntrusiveMultiMapNodeAccessor::GetTFromNode(
   const CIntrusiveMultiMapNode *pNode)
{
   TimerData *pData = nullptr;

   if (pNode)
   {
      JETBYTE_WARNING_SUPPRESS_CAST_ALIGN

      pData = CONTAINING_RECORD(const_cast<CIntrusiveMultiMapNode *>(pNode), TimerData, m_timerQueueNode);

      JETBYTE_WARNING_SUPPRESS_POP
   }

   return pData;
}

CCallbackTimerQueue::TimerData *CCallbackTimerQueue::TimerDataIntrusiveMultiMapNodeAccessor::GetTFromNode(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   TimerData *pData = nullptr;

   if (pNode)
   {
      JETBYTE_WARNING_SUPPRESS_CAST_ALIGN

      pData = CONTAINING_RECORD(const_cast<CIntrusiveMultiMapNode *>(static_cast<const CIntrusiveMultiMapNode *>(pNode)), TimerData, m_timerQueueNode);

      JETBYTE_WARNING_SUPPRESS_POP
   }

   return pData;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
