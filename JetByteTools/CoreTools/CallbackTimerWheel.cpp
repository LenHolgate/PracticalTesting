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
#include "ToString.h"
#include "NullCallbackTimerQueueMonitor.h"
#include "IntrusiveSetNode.h"
#include "DebugTrace.h"

#pragma hdrstop

#include <algorithm>          // for min
#include <cstring>            // for memset

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

static const Milliseconds s_defaultTimerGranularity = 15;

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

      TimerData *OnTimer();

      void Unlink();

      void AddTimedOutTimers(
         TimerData *pTimers);

      TimerData *PrepareForHandleTimeout();

      TimerData *HandleTimeout();

      TimerData *TimeoutHandlingComplete();

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

      TimerData * OnTimer(
         const Data &data);

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
      m_handlingTimeouts(false),
      m_pTimeoutsToBeHandled(nullptr)
{
}

CCallbackTimerWheel::~CCallbackTimerWheel()
{
   try
   {
      ActiveHandles::Iterator it = m_activeHandles.Begin();

      const ActiveHandles::Iterator end = m_activeHandles.End();

      while (it != end)
      {
         const ActiveHandles::Iterator next = it + 1;

         TimerData *pData = *it;

         m_activeHandles.Erase(it);

         delete pData;

         #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
         m_monitor.OnTimerDeleted();
         #endif

         it = next;
      }

      delete [] m_pTimersStart;
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
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
   if (m_handlingTimeouts)
   {
      throw CException(
         _T("CCallbackTimerWheel::BeginTimeoutHandling()"),
         _T("Already handling timeouts, you need to call EndTimeoutHandling()?"));
   }

   m_pTimeoutsToBeHandled = GetAllTimersToProcess(m_tickCountProvider.GetTickCount());

   m_handlingTimeouts = (m_pTimeoutsToBeHandled != nullptr);

   return m_handlingTimeouts;
}

void CCallbackTimerWheel::HandleTimeout()
{
   if (!m_handlingTimeouts)
   {
      throw CException(
         _T("CCallbackTimerWheel::HandleTimeout()"),
         _T("Not currently handling timeouts, you need to call BeginTimeoutHandling()?"));
   }

   TimerData *pTimers = m_pTimeoutsToBeHandled;

   while (pTimers)
   {
      pTimers = pTimers->HandleTimeout();

      #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
      m_monitor.OnTimer();
      #endif
   }
}

void CCallbackTimerWheel::EndTimeoutHandling()
{
   if (!m_handlingTimeouts)
   {
      throw CException(
         _T("CCallbackTimerWheel::EndTimeoutHandling()"),
         _T("Not currently handling timeouts, you need to call BeginTimeoutHandling()?"));
   }

   m_handlingTimeouts = false;

   TimerData *pTimers = m_pTimeoutsToBeHandled;

   m_pTimeoutsToBeHandled = nullptr;

   TimerData *pDeadTimer = nullptr;

   while (pTimers)
   {
      if (pTimers->DeleteAfterTimeout())
      {
         pDeadTimer = pTimers;
      }

      pTimers = pTimers->TimeoutHandlingComplete();

      if (pDeadTimer)
      {
         m_activeHandles.Erase(pDeadTimer);

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
   auto *pData = new TimerData();

   return OnTimerCreated(pData);
}

CCallbackTimerWheel::Handle CCallbackTimerWheel::OnTimerCreated(
   const TimerData *pData)
{
   m_activeHandles.Insert(pData);

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
   TimerData &data = ValidateHandle(handle);

   return data.TimerIsSet();
}

bool CCallbackTimerWheel::SetTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   const SetTimerIf setTimerIf)
{
   TimerData &data = ValidateHandle(handle);

   const bool wasPending = data.TimerIsSet();

   if (setTimerIf == SetTimerAlways || !wasPending)
   {
      // Note that we need to calculate the new actual timeout NOW
      // before we cancel the existing timer in case the new timeout
      // is out of range and we're about to throw an exception and not
      // change anything...

      const Milliseconds actualTimeout = CalculateTimeout(timeout);

      if (wasPending)
      {
         data.CancelTimer();
      }

      data.UpdateData(timer, userData);

      InsertTimer(actualTimeout, data, wasPending);

      #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
      m_monitor.OnTimerSet(wasPending);
      #endif
   }

   return wasPending;
}

void CCallbackTimerWheel::SetTimer(
   IQueueTimers::Timer &timer,
   const Milliseconds timeout,
   const IQueueTimers::UserData userData)
{
   const Milliseconds actualTimeout = CalculateTimeout(timeout);

   auto *pData = new TimerData(actualTimeout, timer, userData);

   OnTimerCreated(pData);

   InsertTimer(actualTimeout, *pData);

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
   bool *pWasUpdated)
{
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

            data.CancelTimer();

            data.UpdateData(timer, userData);

            InsertTimer(actualTimeout, data, wasPending);
         }
      }
   }
   else
   {
      const Milliseconds actualTimeout = CalculateTimeout(timeout);

      data.UpdateData(timer, userData);

      InsertTimer(actualTimeout, data, wasPending);

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
   const bool wasPending)
{
   const size_t timerOffset = timeout / m_timerGranularity;

   TimerData **ppTimer = GetTimerAtOffset(timerOffset);

   data.SetTimer(timeout, ppTimer, *ppTimer);

   m_pFirstTimerSetHint = nullptr;

   if (!wasPending)
   {
      if (0 == ++m_numTimersSet)
      {
         // number of timers set counter has wrapped!

         throw CException(_T("CCallbackTimerWheel::InsertTimer()"), _T("Too many timers set!"));
      }
   }
}

bool CCallbackTimerWheel::CancelTimer(
   const Handle &handle)
{
   TimerData &data = ValidateHandle(handle);

   const bool wasPending = data.CancelTimer();

   if (wasPending)
   {
      OnTimerCancelled();
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
   Handle &handle)
{
   TimerData &data = ValidateHandle(handle);

   const bool wasPending = data.CancelTimer();

   m_activeHandles.Erase(&data);

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

bool CCallbackTimerWheel::DestroyTimer(
   const Handle &handle)
{
   Handle handle_ = handle;

   return DestroyTimer(handle_);
}

Milliseconds CCallbackTimerWheel::GetMaximumTimeout() const
{
   return m_maximumTimeout;
}

CCallbackTimerWheel::TimerData &CCallbackTimerWheel::ValidateHandle(
   const Handle &handle) const
{
   auto *pData = reinterpret_cast<TimerData *>(handle);

   #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES == 1)
   const ActiveHandles::Iterator it = m_activeHandles.Find(pData);

   if (it == m_activeHandles.End())
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

   const DWORD difference = CTickCountCompare::TickCountDifference(m_currentTime, now);

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

   const DWORD difference = CTickCountCompare::TickCountDifference(m_currentTime, now);

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

   memset(ppTimers, 0, sizeof(TimerData*) * numTimers);

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

CCallbackTimerWheel::TimerData *CCallbackTimerWheel::TimerData::OnTimer()
{
   m_ppPrevious = nullptr;

   m_processingTimeout = true;

   OnTimer(m_active);

   m_processingTimeout = false;

   return m_pNext;
}

CCallbackTimerWheel::TimerData *CCallbackTimerWheel::TimerData::OnTimer(
   const Data &data)
{
   if (!data.pTimer)
   {
      throw CException(
         _T("CCallbackTimerWheel::TimerData::OnTimer()"),
         _T("Internal Error: Timer not set"));
   }

   data.pTimer->OnTimer(data.userData);

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

CCallbackTimerWheel::TimerData *CCallbackTimerWheel::TimerData::HandleTimeout()
{
   TimerData *pNextTimedout = OnTimer(m_timedout);

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
   return (maximumTimeout / timerGranularity) + 1;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerWheel.cpp
///////////////////////////////////////////////////////////////////////////////
