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
// CCallbackTimerWheel::Handle
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerWheel::TimerData
{
   public :

      TimerData();

      TimerData(
         CCallbackTimerWheel::Timer &timer,
         const CCallbackTimerWheel::UserData userData);

      bool CancelTimer();

      void UpdateData(
         CCallbackTimerWheel::Timer &timer,
         const CCallbackTimerWheel::UserData userData);

      void SetTimer(
         TimerData **ppPrevious,
         TimerData *pNext);


      TimerData **m_ppPrevious;

      TimerData *m_pNext;

   private :

      CCallbackTimerWheel::Timer *m_pTimer;

      CCallbackTimerWheel::UserData m_userData;

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
   :  m_maximumTimeout(maximumTimeout),
      m_timerGranularity(s_defaultTimerGranularity),
      m_numTimers(CalculateNumberOfTimers(m_maximumTimeout, m_timerGranularity)),
      m_tickCountProvider(s_tickProvider),
      m_currentTime(m_tickCountProvider.GetTickCount()),
      m_pTimersStart(CreateTimerWheel(m_numTimers)),
      m_pTimersEnd(m_pTimersStart + m_numTimers),
      m_pNow(m_pTimersStart)
{

}

CCallbackTimerWheel::CCallbackTimerWheel(
   const Milliseconds maximumTimeout,
   const Milliseconds timerGranularity)
   :  m_maximumTimeout(maximumTimeout),
      m_timerGranularity(timerGranularity),
      m_numTimers(CalculateNumberOfTimers(m_maximumTimeout, m_timerGranularity)),
      m_tickCountProvider(s_tickProvider),
      m_currentTime(m_tickCountProvider.GetTickCount()),
      m_pTimersStart(CreateTimerWheel(m_numTimers)),
      m_pTimersEnd(m_pTimersStart + m_numTimers),
      m_pNow(m_pTimersStart)
{

}

CCallbackTimerWheel::CCallbackTimerWheel(
   const Milliseconds maximumTimeout,
   const IProvideTickCount &tickCountProvider)
   :  m_maximumTimeout(maximumTimeout),
      m_timerGranularity(s_defaultTimerGranularity),
      m_numTimers(CalculateNumberOfTimers(m_maximumTimeout, m_timerGranularity)),
      m_tickCountProvider(tickCountProvider),
      m_currentTime(m_tickCountProvider.GetTickCount()),
      m_pTimersStart(CreateTimerWheel(m_numTimers)),
      m_pTimersEnd(m_pTimersStart + m_numTimers),
      m_pNow(m_pTimersStart)
{

}

CCallbackTimerWheel::CCallbackTimerWheel(
   const Milliseconds maximumTimeout,
   const Milliseconds timerGranularity,
   const IProvideTickCount &tickCountProvider)
   :  m_maximumTimeout(maximumTimeout),
      m_timerGranularity(timerGranularity),
      m_numTimers(CalculateNumberOfTimers(m_maximumTimeout, m_timerGranularity)),
      m_tickCountProvider(tickCountProvider),
      m_currentTime(m_tickCountProvider.GetTickCount()),
      m_pTimersStart(CreateTimerWheel(m_numTimers)),
      m_pTimersEnd(m_pTimersStart + m_numTimers),
      m_pNow(m_pTimersStart)
{

}

CCallbackTimerWheel::~CCallbackTimerWheel()
{
   for (Handles::iterator it = m_handles.begin(); it != m_handles.end(); ++it)
   {
      TimerData *pData = *it;

      delete pData;
   }

   delete [] m_pTimersStart;
}

Milliseconds CCallbackTimerWheel::GetNextTimeout()
{
   throw CException(_T("CCallbackTimerWheel::GetNextTimeout()"), _T("Not implemented"));

   return INFINITE;
}

void CCallbackTimerWheel::HandleTimeouts()
{
   throw CException(_T("CCallbackTimerWheel::HandleTimeouts()"), _T("Not implemented"));
}

IManageTimerQueue::TimeoutHandle CCallbackTimerWheel::BeginTimeoutHandling()
{
   throw CException(_T("CCallbackTimerWheel::BeginTimeoutHandling()"), _T("Not implemented"));

   return InvalidTimeoutHandleValue;
}

void CCallbackTimerWheel::HandleTimeout(
   IManageTimerQueue::TimeoutHandle &handle)
{
   (void)handle;

   throw CException(_T("CCallbackTimerWheel::HandleTimeout()"), _T("Not implemented"));
}

void CCallbackTimerWheel::EndTimeoutHandling(
   IManageTimerQueue::TimeoutHandle &handle)
{
   (void)handle;

   throw CException(_T("CCallbackTimerWheel::EndTimeoutHandling()"), _T("Not implemented"));
}

CCallbackTimerWheel::Handle CCallbackTimerWheel::CreateTimer()
{
   TimerData *pData = new TimerData();

   m_handles.insert(pData);

   return reinterpret_cast<Handle>(pData);
}

bool CCallbackTimerWheel::SetTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   if (timeout > m_maximumTimeout)
   {
      throw CException(
         _T("CCallbackTimerWheel::SetTimer()"), 
         _T("Timeout is too long. Max is: ") + ToString(m_maximumTimeout) + _T(" tried to set: ") + ToString(timeout));
   }

   TimerData &data = ValidateHandle(handle);

   const bool wasSet = data.CancelTimer();

   data.UpdateData(timer, userData);

   InsertTimer(timeout, data);

   return wasSet;
}

void CCallbackTimerWheel::SetTimer(
   IQueueTimers::Timer &timer,
   const Milliseconds timeout,
   const IQueueTimers::UserData userData)
{
   (void)timer;
   (void)timeout;
   (void)userData;

   throw CException(_T("CCallbackTimerWheel::SetTimer()"), _T("Not implemented"));
}

void CCallbackTimerWheel::InsertTimer(
   const Milliseconds timeout,
   TimerData &data)
{
   const size_t timerOffset = timeout / m_timerGranularity;

   TimerData **ppTimer = GetTimerAtOffset(timerOffset);

   data.SetTimer(ppTimer, *ppTimer);
}

bool CCallbackTimerWheel::CancelTimer(
   const Handle &handle)
{
   TimerData &data = ValidateHandle(handle);

   return data.CancelTimer();
}

bool CCallbackTimerWheel::DestroyTimer(
   Handle &handle)
{
   TimerData &data = ValidateHandle(handle);

   const bool wasSet = data.CancelTimer();

   m_handles.erase(&data);

   handle = InvalidHandleValue;

   delete &data;

   return wasSet;
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

   return *pData;
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
      m_userData(0)

{

}

CCallbackTimerWheel::TimerData::TimerData(
   CCallbackTimerWheel::Timer &timer,
   CCallbackTimerWheel::UserData userData)
   :  m_ppPrevious(0),
      m_pNext(0),
      m_pTimer(&timer),
      m_userData(userData)
{

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
