///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerWheel.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2010 JetByte Limited.
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

#include "CallbackTimerWheel.h"
#include "TickCountProvider.h"
#include "Exception.h"
#include "ToString.h"
#include "NullCallbackTimerQueueMonitor.h"
#include "TickCountProvider.h"
#include "IntrusiveSetNode.h"

#pragma hdrstop

#include <algorithm>          // for min

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#include "DebugTrace.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::min;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

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
   const Milliseconds maximumTimeout,
   const Milliseconds timerGranularity);

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerWheel::TimerData
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerWheel::TimerData : private  CIntrusiveSetNode
{
   public :

      template <class TimerData> friend class TIntrusiveRedBlackTreeNodeIsBaseClass;

      TimerData();

      TimerData(
         CCallbackTimerWheel::Timer &timer,
         const CCallbackTimerWheel::UserData userData);

      bool DeleteAfterTimeout() const;

      bool CancelTimer();

      void UpdateData(
         CCallbackTimerWheel::Timer &timer,
         const CCallbackTimerWheel::UserData userData);

      void SetTimer(
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
            CCallbackTimerWheel::Timer &timer,
            CCallbackTimerWheel::UserData userData);

         void Clear();

         CCallbackTimerWheel::Timer *pTimer;

         CCallbackTimerWheel::UserData userData;
      };

      TimerData * OnTimer(
         const Data &data);

      Data m_active;

      Data m_timedout;

      bool m_processingTimeout;

      bool m_deleteAfterTimeout;

      /// No copies do not implement
      TimerData(const TimerData &rhs);
      /// No copies do not implement
      TimerData &operator=(const TimerData &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerWheel
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerWheel::CCallbackTimerWheel(
   const Milliseconds maximumTimeout)
   :  m_monitor(s_monitor),
      m_maximumTimeout(maximumTimeout),
      m_timerGranularity(s_defaultTimerGranularity),
      m_numTimers(CalculateNumberOfTimers(m_maximumTimeout, m_timerGranularity)),
      m_tickCountProvider(s_tickProvider),
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

CCallbackTimerWheel::CCallbackTimerWheel(
   IMonitorCallbackTimerQueue &monitor,
   const Milliseconds maximumTimeout)
   :  m_monitor(monitor),
      m_maximumTimeout(maximumTimeout),
      m_timerGranularity(s_defaultTimerGranularity),
      m_numTimers(CalculateNumberOfTimers(m_maximumTimeout, m_timerGranularity)),
      m_tickCountProvider(s_tickProvider),
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

CCallbackTimerWheel::CCallbackTimerWheel(
   const Milliseconds maximumTimeout,
   const Milliseconds timerGranularity)
   :  m_monitor(s_monitor),
      m_maximumTimeout(maximumTimeout),
      m_timerGranularity(timerGranularity),
      m_numTimers(CalculateNumberOfTimers(m_maximumTimeout, m_timerGranularity)),
      m_tickCountProvider(s_tickProvider),
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

CCallbackTimerWheel::CCallbackTimerWheel(
   IMonitorCallbackTimerQueue &monitor,
   const Milliseconds maximumTimeout,
   const Milliseconds timerGranularity)
   :  m_monitor(monitor),
      m_maximumTimeout(maximumTimeout),
      m_timerGranularity(timerGranularity),
      m_numTimers(CalculateNumberOfTimers(m_maximumTimeout, m_timerGranularity)),
      m_tickCountProvider(s_tickProvider),
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

CCallbackTimerWheel::CCallbackTimerWheel(
   const Milliseconds maximumTimeout,
   const IProvideTickCount &tickCountProvider)
   :  m_monitor(s_monitor),
      m_maximumTimeout(maximumTimeout),
      m_timerGranularity(s_defaultTimerGranularity),
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

CCallbackTimerWheel::CCallbackTimerWheel(
   IMonitorCallbackTimerQueue &monitor,
   const Milliseconds maximumTimeout,
   const IProvideTickCount &tickCountProvider)
   :  m_monitor(monitor),
      m_maximumTimeout(maximumTimeout),
      m_timerGranularity(s_defaultTimerGranularity),
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

CCallbackTimerWheel::CCallbackTimerWheel(
   const Milliseconds maximumTimeout,
   const Milliseconds timerGranularity,
   const IProvideTickCount &tickCountProvider)
   :  m_monitor(s_monitor),
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
         ActiveHandles::Iterator next = it + 1;

         TimerData *pData = *it;

         m_activeHandles.Erase(it);

         delete pData;

         #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
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
   TimerData *pData = new TimerData();

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

bool CCallbackTimerWheel::SetTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   Milliseconds actualTimeout = CalculateTimeout(timeout);

   TimerData &data = ValidateHandle(handle);

   const bool wasPending = data.CancelTimer();

   data.UpdateData(timer, userData);

   InsertTimer(actualTimeout, data, wasPending);

   #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
   m_monitor.OnTimerSet(wasPending);
   #endif

   return wasPending;
}

void CCallbackTimerWheel::SetTimer(
   IQueueTimers::Timer &timer,
   const Milliseconds timeout,
   const IQueueTimers::UserData userData)
{
   Milliseconds actualTimeout = CalculateTimeout(timeout);

   #pragma warning(suppress: 28197) // Possibly leaking memory. No, we're not.
   TimerData *pData = new TimerData(timer, userData);

   OnTimerCreated(pData);

   InsertTimer(actualTimeout, *pData);

   #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
   m_monitor.OnOneOffTimerSet();
   #endif
}

void CCallbackTimerWheel::InsertTimer(
   const Milliseconds timeout,
   TimerData &data,
   const bool wasPending)
{
   const size_t timerOffset = timeout / m_timerGranularity;

   TimerData **ppTimer = GetTimerAtOffset(timerOffset);

   data.SetTimer(ppTimer, *ppTimer);

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
   TimerData *pData = reinterpret_cast<TimerData *>(handle);

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

   // Round 'now' down to the timer granularity

   const Milliseconds thisTime = ((now / m_timerGranularity) * m_timerGranularity);

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

   // Round 'now' down to the timer granularity

   const Milliseconds thisTime = ((now / m_timerGranularity) * m_timerGranularity);

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
   CCallbackTimerWheel::TimerData **ppTimers = new CCallbackTimerWheel::TimerData*[numTimers];

   memset(ppTimers, 0, sizeof(CCallbackTimerWheel::TimerData*) * numTimers);

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
   CCallbackTimerWheel::Timer &timer,
   CCallbackTimerWheel::UserData userData)
   :  m_ppPrevious(nullptr),
      m_pNext(nullptr),
      m_pNextTimedout(nullptr),
      m_active(timer, userData),
      m_processingTimeout(false),
      m_deleteAfterTimeout(true)
{

}

bool CCallbackTimerWheel::TimerData::DeleteAfterTimeout() const
{
   return m_deleteAfterTimeout;
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
   CCallbackTimerWheel::Timer &timer,
   CCallbackTimerWheel::UserData userData)
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
   TimerData **ppPrevious,
   TimerData *pNext)
{
   if (m_ppPrevious)
   {
      throw CException(
         _T("CCallbackTimerWheel::TimerData::SetTimer()"),
         _T("Internal Error: Timer is already set"));
   }

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
   :  pTimer(nullptr),
      userData(0)
{

}

CCallbackTimerWheel::TimerData::Data::Data(
   Timer &timer,
   UserData userData_)
   :  pTimer(&timer),
      userData(userData_)
{

}

void CCallbackTimerWheel::TimerData::Data::Clear()
{
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
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerWheel.cpp
///////////////////////////////////////////////////////////////////////////////
