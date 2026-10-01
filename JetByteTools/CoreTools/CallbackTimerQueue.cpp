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
#include "ExceptionLeakPrevention.h"
#include "NullCallbackTimerQueueMonitor.h"
#include "DebugTrace.h"

#if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES_DUMP_ON_FAILURE == 1)
#include "CrashDumpGenerator.h"
#endif

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

static constexpr Milliseconds s_tickCountMax = 0xFFFFFFFF;

static constexpr Milliseconds s_timeoutMax = s_tickCountMax - 1;

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

      void PrepareForHandleTimeout();

      void HandleTimeout(
         bool shuttingDownWhenSet);

      bool DeleteAfterTimeout() const;

      bool HasTimedOut() const;

      void SetDeleteAfterTimeout();

      void TimeoutHandlingComplete();

      void PushNext(
         TimerData *pNext);

      TimerData *GetNext() const;

      TimerData *PopNext();

      void AddToEnd(
         TimerData *pTimers);

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
         const Data &data,
         bool shuttingDownWhenSet);

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
   :  CCallbackTimerQueue(
         s_monitor,
         s_tickProvider)
{
}

CCallbackTimerQueue::CCallbackTimerQueue(
   IMonitorCallbackTimerQueue &monitor)
   :  CCallbackTimerQueue(
         monitor,
         s_tickProvider)
{
}

CCallbackTimerQueue::CCallbackTimerQueue(
   const IProvideTickCount64 &tickProvider)
   :  CCallbackTimerQueue(
         s_monitor,
         tickProvider)
{
}

CCallbackTimerQueue::CCallbackTimerQueue(
   IMonitorCallbackTimerQueue &monitor,
   const IProvideTickCount64 &tickProvider)
   :  m_tickProvider(tickProvider),
      m_monitor(monitor),
      m_maxTimeout(s_timeoutMax),
      m_pTimeoutsToBeHandled(nullptr),
      m_pTimeoutsThatHaveBeenHandled(nullptr),
      m_shuttingDown(false)
{
}

CCallbackTimerQueue::~CCallbackTimerQueue()
{
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START

   WaitForShutdownToComplete();

   m_queue.Clear(TimerQueue::ClearFlags::FastAndDirty);

   #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES == 1)
   // MUST use Erase as we delete the node and Fast/FastAndDirty both require the nodes
   // to continue to exist so that the iteration can continue.

   m_activeHandles.Clear(ActiveHandles::ClearFlags::Erase, [&](const TimerData *pData) -> void {
      delete pData;

      #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
      m_monitor.OnTimerDeleted();
      #endif
      });
   #endif

   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END
}

CCallbackTimerQueue::Handle CCallbackTimerQueue::CreateTimer()
{
   auto *pData = new TimerData();

   #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES == 1)
   if (!m_activeHandles.Insert(pData).second)
   {
      const _tstring errorMessage = _T("Timer handle: ") + ToString(reinterpret_cast<Handle>(pData)) + _T(" is already in the handle map");

      #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES_NOISY_FAILURE == 1)
      OutputEx(_T("CCallbackTimerQueue::CreateTimer() - ") + errorMessage);
      #endif

      #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES_DUMP_ON_FAILURE == 1)
      CCrashDumpGenerator::GenerateDumpHere(_T("TimerQueueDuplicateHandleInsert"), CCrashDumpGenerator::PerDumpTypeMaxDumpLimits);
      #endif

      throw CException(
         _T("CCallbackTimerQueue::CreateTimer()"),
         errorMessage);
   }
   #endif

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerCreated();
   #endif

   return reinterpret_cast<Handle>(pData);
}

bool CCallbackTimerQueue::TimerIsSet(
   const Handle &handle) const
{
   if (handle != IQueueTimers::InvalidHandleValue)
   {
      const TimerData *pData = ValidateHandle(handle);

      return pData->IsSet();
   }

   return false;
}

bool CCallbackTimerQueue::SetTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   const SetTimerIf setTimerIf,
   bool *pOptionalFirstToExpireHasChanged)
{
   if (m_shuttingDown)
   {
      throw CException(
         _T("CCallbackTimerQueue::SetTimer()"),
         _T("Too late, shutting down"));
   }

   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueue::SetTimer()"),
         _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

   TimerData *pData = ValidateHandle(handle);

   const bool wasPending = pData->IsSet();

   bool firstToExpireHasChanged = false;

   if (setTimerIf == SetTimerAlways || !wasPending)
   {
      const bool atLeastOneTimerWasSet = pOptionalFirstToExpireHasChanged ? !m_queue.IsEmpty() : false;

      const ULONGLONG firstTimeoutBeforeChange = atLeastOneTimerWasSet ? m_queue.Begin()->GetTimeout() : 0;

      if (wasPending)
      {
         CancelTimer(pData);
      }

      pData->UpdateData(timer, userData);

      InsertTimer(pData, timeout);

      if (pOptionalFirstToExpireHasChanged)
      {
         firstToExpireHasChanged = true;

         if (atLeastOneTimerWasSet)
         {
            const ULONGLONG firstTimeoutNow = m_queue.Begin()->GetTimeout();

            if (firstTimeoutNow == firstTimeoutBeforeChange)
            {
               firstToExpireHasChanged = false;
            }
         }
      }

      #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
      m_monitor.OnTimerSet(wasPending);
      #endif
   }

   if (pOptionalFirstToExpireHasChanged)
   {
      *pOptionalFirstToExpireHasChanged = firstToExpireHasChanged;
   }

   return wasPending;
}

bool CCallbackTimerQueue::UpdateTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   const UpdateTimerIf updateIf,
   bool *pWasUpdated,
   bool *pOptionalFirstToExpireHasChanged)
{
   bool updated = false;

   if (m_shuttingDown)
   {
      throw CException(
         _T("CCallbackTimerQueue::UpdateTimer()"),
         _T("Too late, shutting down"));
   }

   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueue::UpdateTimer()"),
         _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

   TimerData *pData = ValidateHandle(handle);

   const bool wasPending = pData->IsSet();

   const bool atLeastOneTimerWasSet = (m_queue.Size() != 0);

   const ULONGLONG firstTimeoutBeforeChange = atLeastOneTimerWasSet ? m_queue.Begin()->GetTimeout() : 0;

   if (wasPending)
   {
      if (updateIf == UpdateAlwaysNoTimeoutChange)
      {
         updated = true;

         pData->UpdateData(timer, userData);
      }
      else
      {
         const ULONGLONG currentAbsoluteTimeout = pData->GetTimeout();

         const ULONGLONG newAbsoluteTimeout = GetAbsoluteTimeout(*pData, timeout);

         if (updateIf == UpdateAlways ||
            (updateIf == UpdateTimerIfNewTimeIsLater && newAbsoluteTimeout > currentAbsoluteTimeout) ||
            (updateIf == UpdateTimerIfNewTimeIsSooner && newAbsoluteTimeout < currentAbsoluteTimeout))
         {
            updated = true;

            pData->UpdateData(timer, userData);

            // need to cancel and set

            m_queue.Erase(pData);

            InsertTimer(pData, newAbsoluteTimeout);
         }
      }
   }
   else
   {
      pData->UpdateData(timer, userData);

      InsertTimer(pData, timeout);

      updated = true;
   }

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerUpdated(wasPending, updated);
   #endif

   if (pWasUpdated)
   {
      *pWasUpdated = updated;
   }

   if (pOptionalFirstToExpireHasChanged)
   {
      bool firstToExpireHasChanged = updated;

      if (updated)
      {
         if (atLeastOneTimerWasSet)
         {
            const ULONGLONG firstTimeoutNow = m_queue.Begin()->GetTimeout();

            if (firstTimeoutNow == firstTimeoutBeforeChange)
            {
               firstToExpireHasChanged = false;
            }
         }
      }

      *pOptionalFirstToExpireHasChanged = firstToExpireHasChanged;
   }

   return wasPending;
}

bool CCallbackTimerQueue::CancelTimer(
   const Handle &handle,
   bool *pOptionalFirstToExpireHasChanged)
{
   if (m_shuttingDown)
   {
      throw CException(
         _T("CCallbackTimerQueue::CancelTimer()"),
         _T("Too late, shutting down"));
   }

   const bool atLeastOneTimerWasSet = pOptionalFirstToExpireHasChanged ? !m_queue.IsEmpty() : false;

   const ULONGLONG firstTimeoutBeforeChange = atLeastOneTimerWasSet ? m_queue.Begin()->GetTimeout() : 0;

   const bool wasPending = CancelTimer(ValidateHandle(handle));

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerCancelled(wasPending);
   #endif

   if (pOptionalFirstToExpireHasChanged)
   {
      bool firstToExpireHasChanged = wasPending;

      if (wasPending)
      {
         if (atLeastOneTimerWasSet && !m_queue.IsEmpty())
         {
            const ULONGLONG firstTimeoutNow = m_queue.Begin()->GetTimeout();

            if (firstTimeoutNow == firstTimeoutBeforeChange)
            {
               firstToExpireHasChanged = false;
            }
         }
      }

      *pOptionalFirstToExpireHasChanged = firstToExpireHasChanged;
   }

   return wasPending;
}

bool CCallbackTimerQueue::DestroyTimer(
   Handle &handle,
   bool *pOptionalFirstToExpireHasChanged)
{
   TimerData *pData = ValidateHandle(handle);

   const bool atLeastOneTimerWasSet = pOptionalFirstToExpireHasChanged ? !m_queue.IsEmpty() : false;

   const ULONGLONG firstTimeoutBeforeChange = atLeastOneTimerWasSet ? m_queue.Begin()->GetTimeout() : 0;

   const bool wasPending = CancelTimer(pData);

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
      #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES == 1)
      if (!m_activeHandles.Erase(pData))
      {
         #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES_NOISY_FAILURE == 1)
         OutputEx(_T("CCallbackTimerQueue::DestroyTimer() - Invalid handle"));
         #endif

         #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES_DUMP_ON_FAILURE == 1)
         CCrashDumpGenerator::GenerateDumpHere(_T("TimerQueueDeleteInvalidHandle"), CCrashDumpGenerator::PerDumpTypeMaxDumpLimits);
         #endif
      }
      #endif

      delete pData;

      #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
      m_monitor.OnTimerDeleted();
      #endif
   }

   if (pOptionalFirstToExpireHasChanged)
   {
      bool firstToExpireHasChanged = wasPending;

      if (wasPending)
      {
         if (atLeastOneTimerWasSet && !m_queue.IsEmpty())
         {
            const ULONGLONG firstTimeoutNow = m_queue.Begin()->GetTimeout();

            if (firstTimeoutNow == firstTimeoutBeforeChange)
            {
               firstToExpireHasChanged = false;
            }
         }
      }

      *pOptionalFirstToExpireHasChanged = firstToExpireHasChanged;
   }

   return wasPending;
}

void CCallbackTimerQueue::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   bool *pOptionalFirstToExpireHasChanged)
{
   if (m_shuttingDown)
   {
      throw CException(
         _T("CCallbackTimerQueue::SetTimer()"),
         _T("Too late, shutting down"));
   }

   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueue::SetTimer()"),
         _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

   const bool atLeastOneTimerWasSet = pOptionalFirstToExpireHasChanged ? !m_queue.IsEmpty() : false;

   const ULONGLONG firstTimeoutBeforeChange = atLeastOneTimerWasSet ? m_queue.Begin()->GetTimeout() : 0;

#pragma warning(suppress: 28197) // Possibly leaking memory - No, we're not.
   auto *pData = new TimerData(timer, userData);

   #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES == 1)
   if (!m_activeHandles.Insert(pData).second)
   {
      const _tstring errorMessage = _T("Timer handle: ") + ToString(reinterpret_cast<Handle>(pData)) + _T(" is already in the handle map");

      #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES_NOISY_FAILURE == 1)
      OutputEx(_T("CCallbackTimerQueue::SetTimer() - ") + errorMessage);
      #endif

      #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES_DUMP_ON_FAILURE == 1)
      CCrashDumpGenerator::GenerateDumpHere(_T("TimerQueueDuplicateHandleInsert"), CCrashDumpGenerator::PerDumpTypeMaxDumpLimits);
      #endif

      throw CException(
         _T("CCallbackTimerQueue::SetTimer()"),
         errorMessage);
   }
   #endif

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerCreated();
   #endif

   InsertTimer(pData, timeout);

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnOneOffTimerSet();
   #endif

   if (pOptionalFirstToExpireHasChanged)
   {
      bool firstToExpireHasChanged = true;

      if (atLeastOneTimerWasSet)
      {
         const ULONGLONG firstTimeoutNow = m_queue.Begin()->GetTimeout();

         if (firstTimeoutNow == firstTimeoutBeforeChange)
         {
            firstToExpireHasChanged = false;
         }
      }

      *pOptionalFirstToExpireHasChanged = firstToExpireHasChanged;
   }
}

Milliseconds CCallbackTimerQueue::GetMaximumTimeout() const
{
   return m_maxTimeout;
}

ULONGLONG CCallbackTimerQueue::GetAbsoluteTimeout(
   TimerData &data,
   const Milliseconds timeout) const
{
   const ULONGLONG now = m_tickProvider.GetTickCount64();

   const ULONGLONG absoluteTimeout = now + timeout;

   if (absoluteTimeout < now)
   {
      data.ClearTimer();

      throw CException(
         _T("CCallbackTimerQueue::GetAbsoluteTimeout()"),
         _T("Timeout will extend beyond the wrap point of GetTickCount64(). ")
         _T("Well done at having your machine running for this long, ")
         _T("but this is outside of our specificiation..."));
   }

   return absoluteTimeout;
}

void CCallbackTimerQueue::InsertTimer(
   TimerData * const pData,
   const Milliseconds timeout)
{
   const ULONGLONG absoluteTimeout = GetAbsoluteTimeout(*pData, timeout);

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
   if (!handle)
   {
      throw CException(
         _T("CCallbackTimerQueue::ValidateHandle()"),
         _T("Invalid timer handle: handle is null"));
   }

   auto *pData = reinterpret_cast<TimerData *>(handle);

   #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES == 1)
   const ActiveHandles::Iterator it = m_activeHandles.Find(pData);

   if (it == m_activeHandles.End())
   {
      // The following warning is generated when /Wp64 is set in a 32bit build. At present, I think
      // it's due to some confusion, and even if it isn't then it's not that crucial...
      #pragma warning(push, 4)
      #pragma warning(disable: 4244)
      const _tstring errorMessage = _T("Invalid timer handle: ") + ToString(handle);
      #pragma warning(pop)

      #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES_NOISY_FAILURE == 1)
      OutputEx(_T("CCallbackTimerQueue::ValidateHandle() - ") + errorMessage);
      #endif

      #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES_DUMP_ON_FAILURE == 1)
      CCrashDumpGenerator::GenerateDumpHere(_T("TimerQueueInvalidHandle"), CCrashDumpGenerator::PerDumpTypeMaxDumpLimits);
      #endif

      throw CException(
         _T("CCallbackTimerQueue::ValidateHandle()"),
         errorMessage);
   }
   #endif

   if (pData->DeleteAfterTimeout())
   {
      // The following warning is generated when /Wp64 is set in a 32bit build. At present, I think
      // it's due to some confusion, and even if it isn't then it's not that crucial...
      #pragma warning(push, 4)
      #pragma warning(disable: 4244)
      throw CException(
         _T("CCallbackTimerQueue::ValidateHandle()"),
         _T("Invalid timer handle: ") + ToString(handle));
      #pragma warning(pop)
   }

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

void CCallbackTimerQueue::BeginShutdown()
{
   BeginShutdown(*this);
}

bool CCallbackTimerQueue::WaitForShutdownToComplete(
   const Milliseconds timeout)
{
   return WaitForShutdownToComplete(*this, timeout);
}

void CCallbackTimerQueue::BeginShutdown(
   IHandleTimerQueueTimeouts &timeoutHandler)
{
   if (m_shuttingDown.ToggleIfFalse())
   {
      timeoutHandler.BeginTimeoutHandling();
   }
}

bool CCallbackTimerQueue::WaitForShutdownToComplete(
   IHandleTimerQueueTimeouts &timeoutHandler,
   const Milliseconds timeout)
{
   (void)timeout;

   BeginShutdown(timeoutHandler);

   HandleTimeouts();

   timeoutHandler.EndTimeoutHandling();

   return true;
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
   bool newTimeouts = false;

   CLockableObject::Owner lock(m_lock);

   if (0 == GetNextTimeout() || m_shuttingDown)
   {
      // Scan to the end of any existing timers...

      TimerData *pLastTimer = nullptr;

      TimerData *pTimer = m_pTimeoutsToBeHandled;

      while (pTimer)
      {
         pLastTimer = pTimer;

         pTimer = pTimer->GetNext();
      }

      // collect any new timers that we should collect...

      TimerQueue::NodeCollection timers;

      while (!m_queue.IsEmpty())
      {
         m_queue.RemoveAll(m_queue.Begin(), timers);

         // Need to duplicate the timer data so that a call to SetTimer that occurs after
         // this call returns but before a call to HandleTimeout with this timer doesn't
         // cause the timer that is about to happen to be changed before it actually
         // "goes off"...

         // Need to remove all of these timers from the NodeCollection before traversing them and firing them
         // as they cannot be set again if they are still in the collection...

         pTimer = timers.Pop();

         if (pTimer)
         {
            newTimeouts = true;
         }

         while (pTimer)
         {
            pTimer->PrepareForHandleTimeout();

            // Store the timeouts that we need to handle in an invasive singly
            // linked list of timers...

            if (pLastTimer)
            {
               pLastTimer->PushNext(pTimer);
            }
            else
            {
               m_pTimeoutsToBeHandled = pTimer;
            }

            pLastTimer = pTimer;

            pTimer = timers.Pop();
         }

         if (!m_shuttingDown)
         {
            // only take the current timers if we're not shutting down...

            break;
         }
      }
   }

   return newTimeouts;
}

size_t CCallbackTimerQueue::HandleTimeouts()
{
   TimerData *pTimersToHandle = nullptr;

   {
      CLockableObject::Owner lock(m_lock);

      std::swap(pTimersToHandle, m_pTimeoutsToBeHandled);
   }

   size_t timersHandled = 0;

   auto pTimers = pTimersToHandle;

   while (pTimers)
   {
      pTimers->HandleTimeout(m_shuttingDown);

      #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
      m_monitor.OnTimer();
      #endif

      timersHandled++;

      pTimers = pTimers->GetNext();
   }

   {
      CLockableObject::Owner lock(m_lock);

      if (m_pTimeoutsThatHaveBeenHandled)
      {
         m_pTimeoutsThatHaveBeenHandled->AddToEnd(pTimersToHandle);
      }
      else
      {
         m_pTimeoutsThatHaveBeenHandled = pTimersToHandle;
      }
   }

   return timersHandled;
}

void CCallbackTimerQueue::EndTimeoutHandling()
{
   CLockableObject::Owner lock(m_lock);

   TimerData *pTimer = m_pTimeoutsThatHaveBeenHandled;

   m_pTimeoutsThatHaveBeenHandled = nullptr;

   while (pTimer)
   {
      TimerData *pNextTimer = pTimer->PopNext();

      if (pTimer->DeleteAfterTimeout())
      {
         #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES == 1)
         if (pTimer->DeleteAfterTimeout())
         {
            if (!m_activeHandles.Erase(pTimer))
            {
               #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES_NOISY_FAILURE == 1)
               OutputEx(_T("CCallbackTimerQueue::EndTimeoutHandling() - Invalid handle"));
               #endif

               #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES_DUMP_ON_FAILURE == 1)
               CCrashDumpGenerator::GenerateDumpHere(_T("TimerQueueDeleteInvalidHandle"), CCrashDumpGenerator::PerDumpTypeMaxDumpLimits);
               #endif
            }
         }
         #endif

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

void CCallbackTimerQueue::TimerData::OnTimer(
   const Data &data,
   const bool shuttingDown)
{
   if (!data.pTimer)
   {
      throw CException(
         _T("CCallbackTimerQueue::TimerData::OnTimer()"),
         _T("Internal Error: Timer not set"));
   }

   data.pTimer->OnTimerEx(
      reinterpret_cast<Handle>(this),
      data.userData,
      shuttingDown);
}

void CCallbackTimerQueue::TimerData::PrepareForHandleTimeout()
{
   m_processingTimeout = true;

   m_timedout = m_active;

   m_active.Clear();
}

void CCallbackTimerQueue::TimerData::HandleTimeout(
   const bool shuttingDownWhenSet)
{
   OnTimer(m_timedout, shuttingDownWhenSet);

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

void CCallbackTimerQueue::TimerData::AddToEnd(
   TimerData *pTimers)
{
   if (pTimers)
   {
      TimerData *pLast = nullptr;

      TimerData *pThisTimer = this;

      while (pThisTimer)
      {
         pLast = pThisTimer;

         pThisTimer = pThisTimer->GetNext();
      }

      if (pLast)
      {
         pLast->PushNext(pTimers);
      }
   }
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
