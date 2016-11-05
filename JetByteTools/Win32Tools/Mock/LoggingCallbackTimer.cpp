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
      m_numTimerEvents(0)
{
}

CLoggingCallbackTimer::CLoggingCallbackTimer(
   CTestLog &linkedLog)
   :  CTestLog(&linkedLog),
      logMessage(true),
      m_numTimerEvents(0)
{
}

bool CLoggingCallbackTimer::WaitForTimer(
   const Milliseconds timeout)
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
      LogMessage(_T("OnTimer: ") + ToString(userData));
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

