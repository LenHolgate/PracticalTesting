///////////////////////////////////////////////////////////////////////////////
// File: LoggingCallbackTimer.cpp
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

#include "LoggingCallbackTimer.h"

#include "JetByteTools\Win32Tools\Utils.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::ToString;

using JetByteTools::Test::CTestLog;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CLoggingCallbackTimer
///////////////////////////////////////////////////////////////////////////////

CLoggingCallbackTimer::CLoggingCallbackTimer()
   :  logMessage(true),
      logUserData(true),
      m_numTimerEvents(0),
      m_pTimerQueue(nullptr),
      m_handle(IQueueTimers::InvalidHandleValue),
      m_timeout(INFINITE),
      m_userData(0)
{
}

CLoggingCallbackTimer::CLoggingCallbackTimer(
   CTestLog &linkedLog)
   :  CTestLog(&linkedLog),
      logMessage(true),
      logUserData(true),
      m_numTimerEvents(0),
      m_pTimerQueue(nullptr),
      m_handle(IQueueTimers::InvalidHandleValue),
      m_timeout(INFINITE),
      m_userData(0)
{
}

void CLoggingCallbackTimer::DestroyTimerInOnTimer(
   IQueueTimers &timerQueue,
   IQueueTimers::Handle &handle)
{
   m_pTimerQueue = &timerQueue;

   m_handle = handle;

   m_timeout = INFINITE;

   m_userData = 0;
}

void CLoggingCallbackTimer::SetTimerInOnTimer(
   IQueueTimers &timerQueue,
   IQueueTimers::Handle &handle,
   const Milliseconds timeout,
   UserData userData)
{
   m_pTimerQueue = &timerQueue;

   m_handle = handle;

   m_timeout = timeout;

   m_userData = userData;
}

bool CLoggingCallbackTimer::WaitForTimer(
   const Milliseconds timeout) const
{
   return m_timerEvent.Wait(timeout);
}

unsigned long CLoggingCallbackTimer::GetNumTimerEvents() const
{
   return m_numTimerEvents;
}

void CLoggingCallbackTimer::OnTimer(
   UserData userData)
{
   if (logMessage)
   {
      if (logUserData)
      {
         LogMessage(_T("OnTimer: ") + ToString(userData));
      }
      else
      {
         LogMessage(_T("OnTimer"));
      }
   }

   if (m_pTimerQueue)
   {
      if (m_timeout != INFINITE)
      {
         m_pTimerQueue->SetTimer(m_handle, *this, m_timeout, m_userData);

         m_timeout = INFINITE;
         m_pTimerQueue = nullptr;
         m_handle = IQueueTimers::InvalidHandleValue;
         m_userData = 0;

         LogMessage(_T("TimerSet"));
      }
      else
      {
         m_pTimerQueue->DestroyTimer(m_handle); //lint !e534 (Ignoring return value of function)

         m_pTimerQueue = nullptr;
         m_handle = IQueueTimers::InvalidHandleValue;

         LogMessage(_T("TimerDestroyed"));
      }
   }

   ::InterlockedIncrement(&m_numTimerEvents);

   m_timerEvent.Set();
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: LoggingCallbackTimer.cpp
///////////////////////////////////////////////////////////////////////////////

