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

#pragma hdrstop

#include "JetByteTools\TestTools\TestException.h"

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::min;

using JetByteTools::Test::CTestSkippedException;

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
// CCallbackTimerWheel::Handle
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerWheel::TimerData
{
   public :

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

      TimerData *OnTimer();

   private :

      TimerData **m_ppPrevious;

      TimerData *m_pNext;

      CCallbackTimerWheel::Timer *m_pTimer;

      CCallbackTimerWheel::UserData m_userData;

      const bool m_deleteAfterTimeout;

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
      m_pFirstTimerSetHint(0),
      m_numTimersSet(0)
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
      m_pFirstTimerSetHint(0),
      m_numTimersSet(0)
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
      m_pFirstTimerSetHint(0),
      m_numTimersSet(0)
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
      m_pFirstTimerSetHint(0),
      m_numTimersSet(0)
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
      m_pFirstTimerSetHint(0),
      m_numTimersSet(0)
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
      m_pFirstTimerSetHint(0),
      m_numTimersSet(0)
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
      m_pFirstTimerSetHint(0),
      m_numTimersSet(0)
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
      m_pFirstTimerSetHint(0),
      m_numTimersSet(0)
{

}

CCallbackTimerWheel::~CCallbackTimerWheel()
{
   for (Handles::iterator it = m_handles.begin(); it != m_handles.end(); ++it)
   {
      TimerData *pData = *it;

      delete pData;

#if (JETBYTE_PERF_TIMER_WHEEL_MONITORING_DISABLED == 0)

      m_monitor.OnTimerDeleted();

#endif
   }

   delete [] m_pTimersStart;
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

      nextTimeout = static_cast<Milliseconds>(((m_pFirstTimerSetHint > m_pNow ? (m_pFirstTimerSetHint - m_pNow) : (m_pNow - m_pFirstTimerSetHint)) + 1) * m_timerGranularity);

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
   }

   return nextTimeout;
}

CCallbackTimerWheel::TimerData **CCallbackTimerWheel::GetFirstTimerSet() const
{
   TimerData **pFirstTimer = 0;

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

void CCallbackTimerWheel::HandleTimeouts()
{
   const Milliseconds now = m_tickCountProvider.GetTickCount();

   while (TimerData *pTimers = GetTimersToProcess(now))
   {
      while (pTimers)
      {
         TimerData *pTimer = pTimers;

         pTimers = pTimer->OnTimer();

#if (JETBYTE_PERF_TIMER_WHEEL_MONITORING_DISABLED == 0)

         m_monitor.OnTimer();

#endif

         if (pTimer->DeleteAfterTimeout())
         {
            m_handles.erase(pTimer);

            delete pTimer;

#if (JETBYTE_PERF_TIMER_WHEEL_MONITORING_DISABLED == 0)

            m_monitor.OnTimerDeleted();

#endif
         }

         --m_numTimersSet;
      }
   }
}

IManageTimerQueue::TimeoutHandle CCallbackTimerWheel::BeginTimeoutHandling()
{
   throw CTestSkippedException(_T("CCallbackTimerWheel::BeginTimeoutHandling()"), _T("Not implemented"));

   return InvalidTimeoutHandleValue;
}

void CCallbackTimerWheel::HandleTimeout(
   IManageTimerQueue::TimeoutHandle &handle)
{
   (void)handle;

   throw CTestSkippedException(_T("CCallbackTimerWheel::HandleTimeout()"), _T("Not implemented"));
}

void CCallbackTimerWheel::EndTimeoutHandling(
   IManageTimerQueue::TimeoutHandle &handle)
{
   (void)handle;

   throw CTestSkippedException(_T("CCallbackTimerWheel::EndTimeoutHandling()"), _T("Not implemented"));
}

CCallbackTimerWheel::Handle CCallbackTimerWheel::CreateTimer()
{
   TimerData *pData = new TimerData();

   return OnTimerCreated(pData);
}

CCallbackTimerWheel::Handle CCallbackTimerWheel::OnTimerCreated(
   TimerData *pData)
{
   m_handles.insert(pData);

#if (JETBYTE_PERF_TIMER_WHEEL_MONITORING_DISABLED == 0)

   m_monitor.OnTimerCreated();

#endif

   return reinterpret_cast<Handle>(pData);
}

bool CCallbackTimerWheel::SetTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   const Milliseconds now = m_tickCountProvider.GetTickCount();

   const Milliseconds actualTimeout = timeout + (now - m_currentTime);

   if (actualTimeout > m_maximumTimeout)
   {
      throw CException(
         _T("CCallbackTimerWheel::SetTimer()"), 
         _T("Timeout is too long. Max is: ") + ToString(m_maximumTimeout) + _T(" tried to set: ") + ToString(actualTimeout) + _T(" (") + ToString(timeout) + _T(")"));
   }

   TimerData &data = ValidateHandle(handle);

   const bool wasPending = data.CancelTimer();

   data.UpdateData(timer, userData);

   InsertTimer(actualTimeout, data, wasPending);

#if (JETBYTE_PERF_TIMER_WHEEL_MONITORING_DISABLED == 0)

   m_monitor.OnTimerSet(wasPending);

#endif

   return wasPending;
}

void CCallbackTimerWheel::SetTimer(
   IQueueTimers::Timer &timer,
   const Milliseconds timeout,
   const IQueueTimers::UserData userData)
{
   const Milliseconds now = m_tickCountProvider.GetTickCount();

   const Milliseconds actualTimeout = timeout + (now - m_currentTime);

   if (actualTimeout > m_maximumTimeout)
   {
      throw CException(
         _T("CCallbackTimerWheel::SetTimer()"), 
         _T("Timeout is too long. Max is: ") + ToString(m_maximumTimeout) + _T(" tried to set: ") + ToString(actualTimeout) + _T(" (") + ToString(timeout) + _T(")"));
   }

   TimerData *pData = new TimerData(timer, userData);

   OnTimerCreated(pData);

   InsertTimer(actualTimeout, *pData);

#if (JETBYTE_PERF_TIMER_WHEEL_MONITORING_DISABLED == 0)

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

   m_pFirstTimerSetHint = 0;

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

#if (JETBYTE_PERF_TIMER_WHEEL_MONITORING_DISABLED == 0)

   m_monitor.OnTimerCancelled(wasPending);

#endif

   return wasPending;
}

void CCallbackTimerWheel::OnTimerCancelled()
{
   --m_numTimersSet;

   m_pFirstTimerSetHint = 0;
}

bool CCallbackTimerWheel::DestroyTimer(
   Handle &handle)
{
   TimerData &data = ValidateHandle(handle);

   const bool wasPending = data.CancelTimer();

   m_handles.erase(&data);

   handle = InvalidHandleValue;

   delete &data;

#if (JETBYTE_PERF_TIMER_WHEEL_MONITORING_DISABLED == 0)

   m_monitor.OnTimerDestroyed(wasPending);

   m_monitor.OnTimerDeleted();

#endif

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

bool CCallbackTimerWheel::DispatchesWithoutLock() const
{
   return true;
}

CCallbackTimerWheel::TimerData &CCallbackTimerWheel::ValidateHandle(
   const Handle &handle) const
{
   TimerData *pData = reinterpret_cast<TimerData *>(handle);

   Handles::const_iterator it = m_handles.find(pData);

   if (it == m_handles.end())
   {
      throw CException(
         _T("CCallbackTimerWheel::ValidateHandle()"), 
         _T("Invalid timer handle: ") + ToString(handle));
   }

   if (pData->DeleteAfterTimeout())
   {
      throw CException(
         _T("CCallbackTimerWheel::ValidateHandle()"), 
         _T("Invalid timer handle: ") + ToString(handle));
   }

   return *pData;
}

CCallbackTimerWheel::TimerData *CCallbackTimerWheel::GetTimersToProcess(
   const Milliseconds now)
{
   TimerData *pTimers = 0;

   // Round 'now' down to the timer granularity

   const Milliseconds thisTime = ((now / m_timerGranularity) * m_timerGranularity);

   while (!pTimers && m_currentTime != thisTime)
   {
      TimerData **ppTimers = GetTimerAtOffset(0);

      pTimers = *ppTimers;

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
      m_pFirstTimerSetHint = 0;
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
   :  m_ppPrevious(0),
      m_pNext(0),
      m_pTimer(0),
      m_userData(0),
      m_deleteAfterTimeout(false)
{

}

CCallbackTimerWheel::TimerData::TimerData(
   CCallbackTimerWheel::Timer &timer,
   CCallbackTimerWheel::UserData userData)
   :  m_ppPrevious(0),
      m_pNext(0),
      m_pTimer(&timer),
      m_userData(userData),
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

      m_ppPrevious = 0;
      m_pNext = 0;

      m_pTimer = 0;
      m_userData = 0;

      timerWasSet = true;
   }

   return timerWasSet;
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

   m_pTimer = &timer;

   m_userData = userData;
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
   if (!m_pTimer)
   {
      throw CException(
         _T("CCallbackTimerWheel::TimerData::OnTimer()"), 
         _T("Internal Error: Timer not set"));
   }

   m_ppPrevious = 0;

   TimerData *pNext = m_pNext;

   if (m_pNext)
   {
      m_pNext->m_ppPrevious = 0;
   }

   m_pNext = 0;

   m_pTimer->OnTimer(m_userData);

   return pNext;
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
