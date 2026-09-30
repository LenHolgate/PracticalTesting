///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerWheel.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2010 JetByte Limited.
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

#include "CallbackTimerWheel.h"
#include "TickCountProvider.h"
#include "TickCountCompare.h"
#include "Exception.h"
#include "ExceptionLeakPrevention.h"
#include "ToString.h"
#include "NullCallbackTimerQueueMonitor.h"
#include "IntrusiveSetNode.h"
#include "DebugTrace.h"
#include "CheckedMemset.h"

#if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES_DUMP_ON_FAILURE == 1)
#include "CrashDumpGenerator.h"
#endif

#pragma hdrstop

#include <algorithm>          // for min

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::min;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

static constexpr Milliseconds s_defaultTimerGranularity = 15;

///////////////////////////////////////////////////////////////////////////////
// File level statics
///////////////////////////////////////////////////////////////////////////////

static const CTickCountProvider s_tickProvider;

static CNullCallbackTimerQueueMonitor s_monitor;

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static size_t CalculateNumberOfTimers(
   Milliseconds maximumTimeout,
   Milliseconds timerGranularity);

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerWheel::TimerData
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerWheel::TimerData : private  CIntrusiveSetNode
{
   public :

      template <class TimerData> friend class TIntrusiveRedBlackTreeNodeIsBaseClass;

      TimerData();

      TimerData(
         Milliseconds absoluteTimeout,
         Timer &timer,
         UserData userData);

      TimerData(
         const TimerData &rhs) = delete;

      TimerData &operator=(
         const TimerData &rhs) = delete;

      bool DeleteAfterTimeout() const;

      bool TimerIsSet() const;

      Milliseconds GetAbsoluteTimeout() const;

      bool CancelTimer();

      void UpdateData(
         Timer &timer,
         UserData userData);

      void SetTimer(
         Milliseconds timeout,
         TimerData **ppPrevious,
         TimerData *pNext);

      bool HasTimedOut() const;

      void SetDeleteAfterTimeout();

      TimerData *OnTimer(
         bool shuttingDownWhenSet);

      void Unlink();

      void AddTimedOutTimers(
         TimerData *pTimers);

      TimerData *PrepareForHandleTimeout();

      TimerData *HandleTimeout(
         bool shuttingDownWhenSet);

      TimerData *TimeoutHandlingComplete();

      void AddToEnd(
         TimerData *pTimers);

   private :

      TimerData **m_ppPrevious;

      TimerData *m_pNext;

      TimerData *m_pNextTimedout;

      struct Data
      {
         Data();

         Data(
            Milliseconds absoluteTimeout,
            Timer &timer,
            UserData userData);

         void Clear();

         Milliseconds absoluteTimeout;

         Timer *pTimer;

         UserData userData;
      };

      TimerData *OnTimer(
         const Data &data,
         bool shuttingDownWhenSet);

      Data m_active;

      Data m_timedout;

      bool m_processingTimeout;

      bool m_deleteAfterTimeout;
};

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerWheel
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerWheel::CCallbackTimerWheel(
   const Milliseconds maximumTimeout)
   :  CCallbackTimerWheel(s_monitor, maximumTimeout, s_defaultTimerGranularity, s_tickProvider)
{
}

CCallbackTimerWheel::CCallbackTimerWheel(
   IMonitorCallbackTimerQueue &monitor,
   const Milliseconds maximumTimeout)
   :  CCallbackTimerWheel(monitor, maximumTimeout, s_defaultTimerGranularity, s_tickProvider)
{
}

CCallbackTimerWheel::CCallbackTimerWheel(
   const Milliseconds maximumTimeout,
   const Milliseconds timerGranularity)
   :  CCallbackTimerWheel(s_monitor, maximumTimeout, timerGranularity, s_tickProvider)
{
}

CCallbackTimerWheel::CCallbackTimerWheel(
   IMonitorCallbackTimerQueue &monitor,
   const Milliseconds maximumTimeout,
   const Milliseconds timerGranularity)
   :  CCallbackTimerWheel(monitor, maximumTimeout, timerGranularity, s_tickProvider)
{
}

CCallbackTimerWheel::CCallbackTimerWheel(
   const Milliseconds maximumTimeout,
   const IProvideTickCount &tickCountProvider)
   :  CCallbackTimerWheel(s_monitor, maximumTimeout, s_defaultTimerGranularity, tickCountProvider)
{
}

CCallbackTimerWheel::CCallbackTimerWheel(
   IMonitorCallbackTimerQueue &monitor,
   const Milliseconds maximumTimeout,
   const IProvideTickCount &tickCountProvider)
   :  CCallbackTimerWheel(monitor, maximumTimeout, s_defaultTimerGranularity, tickCountProvider)
{
}

CCallbackTimerWheel::CCallbackTimerWheel(
   const Milliseconds maximumTimeout,
   const Milliseconds timerGranularity,
   const IProvideTickCount &tickCountProvider)
   :  CCallbackTimerWheel(s_monitor, maximumTimeout, timerGranularity, tickCountProvider)
{
}

CCallbackTimerWheel::CCallbackTimerWheel(
   IMonitorCallbackTimerQueue &monitor,
   const Milliseconds maximumTimeout,
   const Milliseconds timerGranularity,
   const IProvideTickCount &tickCountProvider)
   :  m_monitor(monitor),
      m_maximumTimeout(maximumTimeout),
      m_timerGranularity(timerGranularity),
      m_numTimers(CalculateNumberOfTimers(m_maximumTimeout, m_timerGranularity)),
      m_tickCountProvider(tickCountProvider),
      m_currentTime(m_tickCountProvider.GetTickCount()),
      m_pTimersStart(CreateTimerWheel(m_numTimers)),
      m_pTimersEnd(m_pTimersStart + m_numTimers),
      m_pNow(m_pTimersStart),
      m_pFirstTimerSetHint(nullptr),
      m_numTimersSet(0),
      m_pTimeoutsToBeHandled(nullptr),
      m_pTimeoutsThatHaveBeenHandled(nullptr),
      m_shuttingDown(false)
{
}

CCallbackTimerWheel::~CCallbackTimerWheel()
{
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START

   #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES == 1)
   // MUST use Erase as we delete the node and Fast/FastAndDirty both require the nodes
   // to continue to exist so that the iteration can continue.

   m_activeHandles.Clear(ActiveHandles::ClearFlags::Erase, [&](TimerData *pData) -> void {
      delete pData;

      #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
      m_monitor.OnTimerDeleted();
      #endif
      });
   #endif

   delete [] m_pTimersStart;

   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END
}

void CCallbackTimerWheel::BeginShutdown()
{
   BeginShutdown(*this);
}

bool CCallbackTimerWheel::WaitForShutdownToComplete(
   const Milliseconds timeout)
{
   return WaitForShutdownToComplete(*this, timeout);
}

void CCallbackTimerWheel::BeginShutdown(
   IHandleTimerQueueTimeouts &timeoutHandler)
{
   if (m_shuttingDown.ToggleIfFalse())
   {
      // what happens if we start to shut down whilst we are handling timeouts
      // will this result in an exception and failure? Ideally it should result
      // in success...

      timeoutHandler.BeginTimeoutHandling();
   }
}

bool CCallbackTimerWheel::WaitForShutdownToComplete(
   IHandleTimerQueueTimeouts &timeoutHandler,
   const Milliseconds timeout)
{
   (void)timeout;

   BeginShutdown(timeoutHandler);

   HandleTimeouts();

   timeoutHandler.EndTimeoutHandling();

   return true;
}

Milliseconds CCallbackTimerWheel::GetNextTimeout()
{
   Milliseconds nextTimeout = INFINITE;

   // We need to work out the time difference between now and the first timer that is set.

   if (!m_pFirstTimerSetHint)
   {
      m_pFirstTimerSetHint = GetFirstTimerSet();
   }

   if (m_pFirstTimerSetHint)
   {
      // A timer is set! Calculate the timeout in ms

      nextTimeout = static_cast<Milliseconds>(((m_pFirstTimerSetHint >= m_pNow ? (m_pFirstTimerSetHint - m_pNow) : ((m_pTimersEnd - m_pNow) + m_pFirstTimerSetHint - m_pTimersStart)) + 1) * m_timerGranularity);

      const Milliseconds now = m_tickCountProvider.GetTickCount();

      if (now != m_currentTime)
      {
         // Time has moved on, adjust the next timeout to take into account the difference between now and
         // the timer wheel's view of the current time...

         const Milliseconds timeDiff = (now > m_currentTime ? now - m_currentTime : m_currentTime - now);

         if (timeDiff > nextTimeout)
         {
            nextTimeout = 0;
         }
         else
         {
            nextTimeout -= timeDiff;
         }
      }

      if (nextTimeout > m_maximumTimeout)
      {
         throw CException(
            _T("CCallbackTimerWheel::GetNextTimeout()"),
            _T("Next timeout: ") + ToString(nextTimeout) + _T(" is larger than max: ") + ToString(m_maximumTimeout));
      }
   }

   return nextTimeout;
}

CCallbackTimerWheel::TimerData **CCallbackTimerWheel::GetFirstTimerSet() const
{
   TimerData **pFirstTimer = nullptr;

   if (m_numTimersSet != 0)
   {
      // Scan forwards from now to the end of the array...

      for (TimerData **p = m_pNow; !pFirstTimer && p < m_pTimersEnd; ++p)
      {
         if (*p)
         {
            pFirstTimer = p;
         }
      }

      if (!pFirstTimer)
      {
         // We havent yet found our first timer, now scan from the start of the array to
         // now...

         for (TimerData **p = m_pTimersStart; !pFirstTimer && p < m_pNow; ++p)
         {
            if (*p)
            {
               pFirstTimer = p;
            }
         }
      }

      if (!pFirstTimer)
      {
         throw CException(_T("CCallbackTimerWheel::GetFirstTimerSet()"), _T("Unexpected, no timer set but count = ") + ToString(m_numTimersSet));
      }
   }

   return pFirstTimer;
}

bool CCallbackTimerWheel::BeginTimeoutHandling()
{
   bool newTimeouts = false;

   CLockableObject::Owner lock(m_lock);

   if (m_numTimersSet)
   {
      const auto now = m_tickCountProvider.GetTickCount() + (m_shuttingDown ? m_maximumTimeout : 0);

      #if (JETBYTE_PERF_TIMER_WHEEL_HANDLE_ALL_TIMERS_IN_BEGIN_TIMEOUT_HANDLING == 1)

      auto *pNewTimeouts = GetAllTimersToProcess(now);

      #else

      auto *pNewTimeouts = GetTimersToProcess(now);

      #endif

      if (pNewTimeouts)
      {
         newTimeouts = true;

         if (m_pTimeoutsToBeHandled)
         {
            m_pTimeoutsToBeHandled->AddToEnd(pNewTimeouts);
         }
         else
         {
            m_pTimeoutsToBeHandled = pNewTimeouts;
         }
      }
   }

   return newTimeouts;
}

size_t CCallbackTimerWheel::HandleTimeouts()
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
      pTimers = pTimers->HandleTimeout(m_shuttingDown);

      #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
      m_monitor.OnTimer();
      #endif

      timersHandled++;
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

void CCallbackTimerWheel::EndTimeoutHandling()
{
   CLockableObject::Owner lock(m_lock);

   TimerData *pTimers = m_pTimeoutsThatHaveBeenHandled;

   m_pTimeoutsThatHaveBeenHandled = nullptr;

   const TimerData *pDeadTimer = nullptr;

   while (pTimers)
   {
      if (pTimers->DeleteAfterTimeout())
      {
         pDeadTimer = pTimers;
      }

      pTimers = pTimers->TimeoutHandlingComplete();

      if (pDeadTimer)
      {
         #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES == 1)
         if (!m_activeHandles.Erase(pDeadTimer))
         {
            #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES_NOISY_FAILURE == 1)
            OutputEx(_T("CCallbackTimerWheel::EndTimeoutHandling() - Invalid handle"));
            #endif

            #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES_DUMP_ON_FAILURE == 1)
            CCrashDumpGenerator::GenerateDumpHere(_T("TimerWheelDeleteInvalidHandle"), CCrashDumpGenerator::PerDumpTypeMaxDumpLimits);
            #endif

            throw CException(
               _T("CCallbackTimerWheel::EndTimeoutHandling()"),
               _T("Invalid handle"));
         }
         #endif

         delete pDeadTimer;

         #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
         m_monitor.OnTimerDeleted();
         #endif

         pDeadTimer = nullptr;
      }
   }
}

CCallbackTimerWheel::Handle CCallbackTimerWheel::CreateTimer()
{
#pragma warning(suppress: 28197) // Possibly leaking memory. No, we're not.
   const auto *pData = new TimerData();

   return OnTimerCreated(pData);
}

CCallbackTimerWheel::Handle CCallbackTimerWheel::OnTimerCreated(
   const TimerData *pData)
{
   #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES == 1)
   if (!m_activeHandles.Insert(pData).second)
   {
      const _tstring errorMessage = _T("Timer handle: ") + ToString(reinterpret_cast<Handle>(pData)) + _T(" is already in the handle map");

      #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES_NOISY_FAILURE == 1)
      OutputEx(_T("CCallbackTimerWheel::OnTimerCreated() - ") + errorMessage);
      #endif

      #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES_DUMP_ON_FAILURE == 1)
      CCrashDumpGenerator::GenerateDumpHere(_T("TimerWheelDuplicateHandleInsert"), CCrashDumpGenerator::PerDumpTypeMaxDumpLimits);
      #endif

      throw CException(
         _T("CCallbackTimerWheel::OnTimerCreated()"),
         errorMessage);
   }
   #endif

   #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
   m_monitor.OnTimerCreated();
   #endif

   return reinterpret_cast<Handle>(pData);
}

Milliseconds CCallbackTimerWheel::CalculateTimeout(
   const Milliseconds timeout)
{
   const Milliseconds now = m_tickCountProvider.GetTickCount();

   if (m_numTimersSet == 0)
   {
      m_currentTime = now;

      m_pNow = m_pTimersStart;
   }

   const Milliseconds actualTimeout = timeout + (now - m_currentTime);

   if (actualTimeout > m_maximumTimeout)
   {
      throw CException(
         _T("CCallbackTimerWheel::CalculateTimeout()"),
         _T("Timeout is too long. Max is: ") + ToString(m_maximumTimeout) +
         _T(" tried to set: ") + ToString(actualTimeout) +
         _T(" (") + ToString(timeout) +
         _T(") - now = ") + ToString(now) +
         _T(" m_currentTime = ") + ToString(m_currentTime));
   }

   return actualTimeout;
}

bool CCallbackTimerWheel::TimerIsSet(
   const Handle &handle) const
{
   if (handle != IQueueTimers::InvalidHandleValue)
   {
      const TimerData &data = ValidateHandle(handle);

      return data.TimerIsSet();
   }

   return false;
}

bool CCallbackTimerWheel::SetTimer(
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
         _T("CCallbackTimerWheel::SetTimer()"),
         _T("Too late, shutting down"));
   }

   TimerData &data = ValidateHandle(handle);

   const bool wasPending = data.TimerIsSet();

   if (setTimerIf == SetTimerAlways || !wasPending)
   {
      const Milliseconds actualTimeout = CalculateTimeout(timeout);

      size_t previousOffset = 0;
   
      if (wasPending)
      {
         if (pOptionalFirstToExpireHasChanged)
         {
            previousOffset = data.GetAbsoluteTimeout() / m_timerGranularity;
         }

         data.CancelTimer();
      }

      data.UpdateData(timer, userData);

      InsertTimer(actualTimeout, data, wasPending, previousOffset, pOptionalFirstToExpireHasChanged);

      #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
      m_monitor.OnTimerSet(wasPending);
      #endif
   }
   else if (pOptionalFirstToExpireHasChanged)
   {
      *pOptionalFirstToExpireHasChanged = false;
   }

   return wasPending;
}

void CCallbackTimerWheel::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   bool *pOptionalFirstToExpireHasChanged)
{
   if (m_shuttingDown)
   {
      throw CException(
         _T("CCallbackTimerWheel::SetTimer()"),
         _T("Too late, shutting down"));
   }

   const Milliseconds actualTimeout = CalculateTimeout(timeout);

   #pragma warning(suppress: 28197) // Possibly leaking memory. No, we're not.
   auto *pData = new TimerData(actualTimeout, timer, userData);

   OnTimerCreated(pData);

   InsertTimer(actualTimeout, *pData, false, 0, pOptionalFirstToExpireHasChanged);

   #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
   m_monitor.OnOneOffTimerSet();
   #endif
}

bool CCallbackTimerWheel::UpdateTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   const UpdateTimerIf updateIf,
   bool *pWasUpdated,
   bool *pOptionalFirstToExpireHasChanged)
{
   if (m_shuttingDown)
   {
      throw CException(
         _T("CCallbackTimerWheel::UpdateTimer()"),
         _T("Too late, shutting down"));
   }

   bool updated = false;

   TimerData &data = ValidateHandle(handle);

   const bool wasPending = data.TimerIsSet();

   if (wasPending)
   {
      if (updateIf == UpdateAlwaysNoTimeoutChange)
      {
         updated = true;

         data.UpdateData(timer, userData);
      }
      else
      {
         const Milliseconds currentTimeout = data.GetAbsoluteTimeout();

         const Milliseconds actualTimeout = CalculateTimeout(timeout);

         if (updateIf == UpdateAlways ||
            (updateIf == UpdateTimerIfNewTimeIsLater && actualTimeout > currentTimeout) ||
            (updateIf == UpdateTimerIfNewTimeIsSooner && actualTimeout < currentTimeout))
         {
            updated = true;

            const size_t previousOffset = data.GetAbsoluteTimeout() / m_timerGranularity;

            data.CancelTimer();

            data.UpdateData(timer, userData);

            InsertTimer(actualTimeout, data, wasPending, previousOffset, pOptionalFirstToExpireHasChanged);
         }
      }
   }
   else
   {
      const Milliseconds actualTimeout = CalculateTimeout(timeout);

      data.UpdateData(timer, userData);

      InsertTimer(actualTimeout, data, false, 0, pOptionalFirstToExpireHasChanged);

      updated = true;
   }

   #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
   m_monitor.OnTimerUpdated(wasPending, updated);
   #endif

   if (pWasUpdated)
   {
      *pWasUpdated = updated;
   }

   return wasPending;
}

void CCallbackTimerWheel::InsertTimer(
   const Milliseconds timeout,
   TimerData &data,
   const bool wasPending,
   const size_t previousOffset,
   bool *pOptionalFirstToExpireHasChanged)
{
   const size_t timerOffset = timeout / m_timerGranularity;

   TimerData **ppTimer = GetTimerAtOffset(timerOffset);

   const bool timerWasSetAtThisOffset = pOptionalFirstToExpireHasChanged ? (wasPending && (previousOffset == timerOffset)) || (*ppTimer != nullptr) : false;

   data.SetTimer(timeout, ppTimer, *ppTimer);

   if (!wasPending)
   {
      if (0 == ++m_numTimersSet)
      {
         // number of timers set counter has wrapped!

         throw CException(_T("CCallbackTimerWheel::InsertTimer()"), _T("Too many timers set!"));
      }
   }

   m_pFirstTimerSetHint = nullptr;

   if (pOptionalFirstToExpireHasChanged)
   {
      m_pFirstTimerSetHint = GetFirstTimerSet();

      *pOptionalFirstToExpireHasChanged = (m_pFirstTimerSetHint == ppTimer) && !timerWasSetAtThisOffset;
   }
}

bool CCallbackTimerWheel::CancelTimer(
   const Handle &handle,
   bool *pOptionalFirstToExpireHasChanged)
{
   if (m_shuttingDown)
   {
      throw CException(
         _T("CCallbackTimerWheel::CancelTimer()"),
         _T("Too late, shutting down"));
   }

   TimerData &data = ValidateHandle(handle);

   bool wasSetAtThisOffset = false;

   TimerData **pPreviouslyFirstSetTimer = nullptr;

   if (pOptionalFirstToExpireHasChanged)
   {
      const size_t ourOffset = data.GetAbsoluteTimeout() / m_timerGranularity;

      pPreviouslyFirstSetTimer = m_pFirstTimerSetHint ? m_pFirstTimerSetHint : GetFirstTimerSet();

      if (pPreviouslyFirstSetTimer)
      {
         const size_t firstSetOffset = ((*pPreviouslyFirstSetTimer)->GetAbsoluteTimeout() / m_timerGranularity);

         wasSetAtThisOffset = (ourOffset == firstSetOffset);
      }
   }

   const bool wasPending = data.CancelTimer();

   if (wasPending)
   {
      OnTimerCancelled();
   }

   if (m_numTimersSet != 0)
   {
      if (pOptionalFirstToExpireHasChanged)
      {
         // this may cost too much

         m_pFirstTimerSetHint = GetFirstTimerSet();
      }
   }

   if (pOptionalFirstToExpireHasChanged)
   {
      *pOptionalFirstToExpireHasChanged = wasPending && wasSetAtThisOffset && (m_pFirstTimerSetHint != pPreviouslyFirstSetTimer);
   }

   #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
   m_monitor.OnTimerCancelled(wasPending);
   #endif

   return wasPending;
}

void CCallbackTimerWheel::OnTimerCancelled()
{
   --m_numTimersSet;

   m_pFirstTimerSetHint = nullptr;
}

bool CCallbackTimerWheel::DestroyTimer(
   Handle &handle,
   bool *pOptionalFirstToExpireHasChanged)
{
   TimerData &data = ValidateHandle(handle);

   const TimerData * const *pPreviouslyFirstSetTimer = m_pFirstTimerSetHint;

   bool wasSetAtThisOffset = false;

   if (pOptionalFirstToExpireHasChanged)
   {
      const size_t ourOffset = data.GetAbsoluteTimeout() / m_timerGranularity;

      if (m_pFirstTimerSetHint)
      {
         const size_t firstSetOffset = ((*m_pFirstTimerSetHint)->GetAbsoluteTimeout() / m_timerGranularity);

         wasSetAtThisOffset = (ourOffset == firstSetOffset);
      }
   }

   const bool wasPending = data.CancelTimer();

   if (pOptionalFirstToExpireHasChanged)
   {
      *pOptionalFirstToExpireHasChanged = wasPending && wasSetAtThisOffset && (m_pFirstTimerSetHint != pPreviouslyFirstSetTimer);
   }

   handle = InvalidHandleValue;

   #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
   m_monitor.OnTimerDestroyed(wasPending);
   #endif

   if (data.HasTimedOut())
   {
      data.SetDeleteAfterTimeout();
   }
   else
   {
      #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES == 1)
      if (!m_activeHandles.Erase(&data))
      {
         #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES_NOISY_FAILURE == 1)
         OutputEx(_T("CCallbackTimerWheel::DestroyTimer()- Invalid handle"));
         #endif

         #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES_DUMP_ON_FAILURE == 1)
         CCrashDumpGenerator::GenerateDumpHere(_T("TimerWheelDeleteInvalidHandle"), CCrashDumpGenerator::PerDumpTypeMaxDumpLimits);
         #endif
      }
      #endif

      delete &data;

      #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
      m_monitor.OnTimerDeleted();
      #endif
   }

   if (wasPending)
   {
      OnTimerCancelled();
   }

   return wasPending;
}

Milliseconds CCallbackTimerWheel::GetMaximumTimeout() const
{
   return m_maximumTimeout;
}

CCallbackTimerWheel::TimerData &CCallbackTimerWheel::ValidateHandle(
   const Handle &handle) const
{
   if (!handle)
   {
      throw CException(
         _T("CCallbackTimerWheel::ValidateHandle()"),
         _T("Invalid timer handle: handle is null"));
   }

   auto *pData = reinterpret_cast<TimerData *>(handle);

   #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES == 1)
   const ActiveHandles::Iterator it = m_activeHandles.Find(pData);

   if (it == m_activeHandles.End())
   {
      // The following warning is generated when /Wp64 is set in a 32bit build. At present I think
      // it's due to some confusion, and even if it isn't then it's not that crucial...
      #pragma warning(push, 4)
      #pragma warning(disable: 4244)
      const _tstring errorMessage = _T("Invalid timer handle: ") + ToString(handle);
      #pragma warning(pop)

      #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES_NOISY_FAILURE == 1)
      OutputEx(_T("CCallbackTimerWheel::ValidateHandle() - ") + errorMessage);
      #endif

      #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES_DUMP_ON_FAILURE == 1)
      CCrashDumpGenerator::GenerateDumpHere(_T("TimerWheelInvalidHandle"), CCrashDumpGenerator::PerDumpTypeMaxDumpLimits);
      #endif

      throw CException(
         _T("CCallbackTimerWheel::ValidateHandle()"),
         errorMessage);
   }
   #endif

   if (pData->DeleteAfterTimeout())
   {
      // The following warning is generated when /Wp64 is set in a 32bit build. At present I think
      // it's due to some confusion, and even if it isn't then it's not that crucial...
      #pragma warning(push, 4)
      #pragma warning(disable: 4244)
      throw CException(
         _T("CCallbackTimerWheel::ValidateHandle()"),
         _T("Invalid timer handle: ") + ToString(handle));
      #pragma warning(pop)
   }

   return *pData;
}

CCallbackTimerWheel::TimerData *CCallbackTimerWheel::GetAllTimersToProcess(
   const Milliseconds now)
{
   TimerData *pTimers = nullptr;

   TimerData *pLastTimer = nullptr;

   // Round 'now' down to the timer granularity of the wheel...

   // The wheel steps in granularity sized steps from the initial m_currentTime that is set
   // when the wheel is created. The tick provider may be providing ticks at a different
   // granularity and it's important that we only ever step the current time of the wheel
   // forward in wheel granularity sized steps, otherwise the timeouts in the wheel will
   // change.

   const DWORD difference = CTickCountCompare::Difference(m_currentTime, now);

   const DWORD differenceInGranularity = (difference / m_timerGranularity) * m_timerGranularity;

   const Milliseconds thisTime = m_currentTime + differenceInGranularity;

   while (m_numTimersSet && m_currentTime != thisTime)
   {
      TimerData **ppTimers = GetTimerAtOffset(0);

      TimerData *pTheseTimers = *ppTimers;

      if (pTheseTimers)
      {
         pTheseTimers->Unlink();

         if (!pTimers)
         {
            pTimers = pTheseTimers;
         }

         if (pLastTimer)
         {
            pLastTimer->AddTimedOutTimers(pTheseTimers);
         }

         pLastTimer = PrepareTimersForHandleTimeout(pTheseTimers);
      }

      // Step along the wheel...

      m_pNow++;

      if (m_pNow >= m_pTimersEnd)
      {
         m_pNow = m_pTimersStart + (m_pNow - m_pTimersEnd);
      }

      m_currentTime += m_timerGranularity;
   }

   m_currentTime = thisTime;

   if (pTimers)
   {
      m_pFirstTimerSetHint = nullptr;
   }

   return pTimers;
}

CCallbackTimerWheel::TimerData *CCallbackTimerWheel::PrepareTimersForHandleTimeout(
   TimerData *pTimers)
{
   TimerData *pLastTimer = pTimers;

   while (pTimers)
   {
      pLastTimer = pTimers;

      pTimers = pTimers->PrepareForHandleTimeout();

      m_numTimersSet--;
   }

   return pLastTimer;
}

CCallbackTimerWheel::TimerData *CCallbackTimerWheel::GetTimersToProcess(
   const Milliseconds now)
{
   TimerData *pTimers = nullptr;

   // Round 'now' down to the timer granularity of the wheel...

   // The wheel steps in granularity sized steps from the initial m_currentTime that is set
   // when the wheel is created. The tick provider may be providing ticks at a different
   // granularity and it's important that we only ever step the current time of the wheel
   // forward in wheel granularity sized steps, otherwise the timeouts in the wheel will
   // change.

   const DWORD difference = CTickCountCompare::Difference(m_currentTime, now);

   const DWORD differenceInGranularity = (difference / m_timerGranularity) * m_timerGranularity;

   const Milliseconds thisTime = m_currentTime + differenceInGranularity;

   while (!pTimers && m_currentTime != thisTime)
   {
      TimerData **ppTimers = GetTimerAtOffset(0);

      pTimers = *ppTimers;

      if (pTimers)
      {
         pTimers->Unlink();
      }

      // Step along the wheel...

      m_pNow++;

      if (m_pNow >= m_pTimersEnd)
      {
         m_pNow = m_pTimersStart + (m_pNow - m_pTimersEnd);
      }

      m_currentTime += m_timerGranularity;
   }

   if (pTimers)
   {
      m_pFirstTimerSetHint = nullptr;
   }

   return pTimers;
}

CCallbackTimerWheel::TimerData **CCallbackTimerWheel::GetTimerAtOffset(
   const size_t offset) const
{
   TimerData **pNext = m_pNow + offset;

   if (pNext >= m_pTimersEnd)
   {
      pNext = m_pTimersStart + (pNext - m_pTimersEnd);
   }

   return pNext;
}

CCallbackTimerWheel::TimerData **CCallbackTimerWheel::CreateTimerWheel(
   const size_t numTimers)
{
   auto **ppTimers = new TimerData*[numTimers];

   CheckedByteSet(ppTimers, sizeof(TimerData*) * numTimers, 0);

   return ppTimers;
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerWheel::TimerData
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerWheel::TimerData::TimerData()
   :  m_ppPrevious(nullptr),
      m_pNext(nullptr),
      m_pNextTimedout(nullptr),
      m_processingTimeout(false),
      m_deleteAfterTimeout(false)
{

}

CCallbackTimerWheel::TimerData::TimerData(
   const Milliseconds absoluteTimeout,
   Timer &timer,
   const UserData userData)
   :  m_ppPrevious(nullptr),
      m_pNext(nullptr),
      m_pNextTimedout(nullptr),
      m_active(absoluteTimeout, timer, userData),
      m_processingTimeout(false),
      m_deleteAfterTimeout(true)
{

}

bool CCallbackTimerWheel::TimerData::DeleteAfterTimeout() const
{
   return m_deleteAfterTimeout;
}

bool CCallbackTimerWheel::TimerData::TimerIsSet() const
{
   return m_ppPrevious != nullptr;
}

Milliseconds CCallbackTimerWheel::TimerData::GetAbsoluteTimeout() const
{
   // We could do it without extra data per timer, by working out where we are in the
   // wheel, we would have to walk back along the list of timers set for this time
   // until we get to the front and then work out where the front was in relation to
   // m_pNow and then use the granularity...

   return m_active.absoluteTimeout;
}

bool CCallbackTimerWheel::TimerData::CancelTimer()
{
   bool timerWasSet = false;

   if (m_ppPrevious)
   {
      if (m_pNext)
      {
         m_pNext->m_ppPrevious = m_ppPrevious;
      }

      *m_ppPrevious = m_pNext;

      m_ppPrevious = nullptr;
      m_pNext = nullptr;

      m_active.Clear();

      timerWasSet = true;
   }

   return timerWasSet;
}

bool CCallbackTimerWheel::TimerData::HasTimedOut() const
{
   return m_processingTimeout;
}

void CCallbackTimerWheel::TimerData::SetDeleteAfterTimeout()
{
   m_deleteAfterTimeout = true;
}

void CCallbackTimerWheel::TimerData::UpdateData(
   Timer &timer,
   const UserData userData)
{
   if (m_deleteAfterTimeout)
   {
      throw CException(
         _T("CCallbackTimerWheel::TimerData::UpdateData()"),
         _T("Internal Error: Can't update one shot timers or timers pending deletion"));
   }

   m_active.pTimer = &timer;

   m_active.userData = userData;
}

void CCallbackTimerWheel::TimerData::SetTimer(
   const Milliseconds timeout,
   TimerData **ppPrevious,
   TimerData *pNext)
{
   if (m_ppPrevious)
   {
      throw CException(
         _T("CCallbackTimerWheel::TimerData::SetTimer()"),
         _T("Internal Error: Timer is already set"));
   }

   m_active.absoluteTimeout = timeout;

   m_ppPrevious = ppPrevious;

   m_pNext = pNext;

   if (m_pNext)
   {
      m_pNext->m_ppPrevious = &m_pNext;
   }

   *ppPrevious = this;
}

CCallbackTimerWheel::TimerData *CCallbackTimerWheel::TimerData::OnTimer(
   bool shuttingDownWhenSet)
{
   m_ppPrevious = nullptr;

   m_processingTimeout = true;

   OnTimer(m_active, shuttingDownWhenSet);

   m_processingTimeout = false;

   return m_pNext;
}

CCallbackTimerWheel::TimerData *CCallbackTimerWheel::TimerData::OnTimer(
   const Data &data,
   const bool shuttingDownWhenSet)
{
   if (!data.pTimer)
   {
      throw CException(
         _T("CCallbackTimerWheel::TimerData::OnTimer()"),
         _T("Internal Error: Timer not set"));
   }

   data.pTimer->OnTimerEx(
      reinterpret_cast<Handle>(this),
      data.userData,
      shuttingDownWhenSet);

   return m_processingTimeout ? m_pNextTimedout : m_pNext;
}

void CCallbackTimerWheel::TimerData::Unlink()
{
   if (m_ppPrevious)
   {
      *m_ppPrevious = nullptr;

      m_ppPrevious = nullptr;
   }
}

void CCallbackTimerWheel::TimerData::AddTimedOutTimers(
   TimerData *pTimers)
{
   m_pNextTimedout = pTimers;
}

CCallbackTimerWheel::TimerData *CCallbackTimerWheel::TimerData::PrepareForHandleTimeout()
{
   m_processingTimeout = true;

   m_timedout = m_active;

   m_active.Clear();

   m_pNextTimedout = m_pNext;

   m_pNext = nullptr;

   m_ppPrevious = nullptr;

   return m_pNextTimedout;
}

CCallbackTimerWheel::TimerData *CCallbackTimerWheel::TimerData::HandleTimeout(
   const bool shuttingDownWhenSet)
{
   TimerData *pNextTimedout = OnTimer(m_timedout, shuttingDownWhenSet);

   m_timedout.Clear();

   return pNextTimedout;
}

CCallbackTimerWheel::TimerData *CCallbackTimerWheel::TimerData::TimeoutHandlingComplete()
{
   m_processingTimeout = false;

   TimerData *pNext = m_pNextTimedout;

   m_pNextTimedout = nullptr;

   return pNext;
}

void CCallbackTimerWheel::TimerData::AddToEnd(
   TimerData *pTimers)
{
   if (pTimers)
   {
      TimerData *pLast = nullptr;

      TimerData *pThisTimer = this;

      while (pThisTimer)
      {
         pLast = pThisTimer;

         pThisTimer = pThisTimer->m_pNextTimedout;
      }

      if (pLast)
      {
         pLast->AddTimedOutTimers(pTimers);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerWheel::TimerData::Data
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerWheel::TimerData::Data::Data()
   :  absoluteTimeout(0),
      pTimer(nullptr),
      userData(0)
{
}

CCallbackTimerWheel::TimerData::Data::Data(
   const Milliseconds absoluteTimeout_,
   Timer &timer,
   const UserData userData_)
   :  absoluteTimeout(absoluteTimeout_),
      pTimer(&timer),
      userData(userData_)
{
}

void CCallbackTimerWheel::TimerData::Data::Clear()
{
   absoluteTimeout = 0;
   pTimer = nullptr;
   userData = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static size_t CalculateNumberOfTimers(
   const Milliseconds maximumTimeout,
   const Milliseconds timerGranularity)
{
   return (static_cast<size_t>(maximumTimeout) / timerGranularity) + 1;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerWheel.cpp
///////////////////////////////////////////////////////////////////////////////
