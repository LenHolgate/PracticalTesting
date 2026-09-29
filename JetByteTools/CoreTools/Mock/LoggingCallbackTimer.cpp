///////////////////////////////////////////////////////////////////////////////
// File: LoggingCallbackTimer.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2004 JetByte Limited.
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

#include "LoggingCallbackTimer.h"

#include "JetByteTools/CoreTools/ToString.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestLog;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CLoggingCallbackTimer
///////////////////////////////////////////////////////////////////////////////

CLoggingCallbackTimer::CLoggingCallbackTimer()
   :  logMessage(true),
      logUserData(true),
      supportsTimersFiringDuringShutdown(false),
      m_numTimerEvents(0),
      m_pTimerQueue(nullptr),
      m_handle(IQueueTimers::InvalidHandleValue),
      m_timeout1(INFINITE),
      m_timeout2(INFINITE),
      m_userData(0)
{
}

CLoggingCallbackTimer::CLoggingCallbackTimer(
   CTestLog &linkedLog)
   :  CTestLog(&linkedLog),
      logMessage(true),
      logUserData(true),
      supportsTimersFiringDuringShutdown(false),
      m_numTimerEvents(0),
      m_pTimerQueue(nullptr),
      m_handle(IQueueTimers::InvalidHandleValue),
      m_timeout1(INFINITE),
      m_timeout2(INFINITE),
      m_userData(0)
{
}

void CLoggingCallbackTimer::DestroyTimerInOnTimer(
   IQueueTimers &timerQueue,
   IQueueTimers::Handle &handle)
{
   m_pTimerQueue = &timerQueue;

   m_handle = handle;

   m_timeout1 = INFINITE;
   m_timeout2 = INFINITE;

   m_userData = 0;
}

void CLoggingCallbackTimer::SetTimerInOnTimer(
   IQueueTimers &timerQueue,
   IQueueTimers::Handle &handle,
   const Milliseconds timeout,
   const UserData userData)
{
   m_pTimerQueue = &timerQueue;

   m_handle = handle;

   m_timeout1 = timeout;
   m_timeout2 = INFINITE;

   m_userData = userData;
}

void CLoggingCallbackTimer::SetTimerTwiceInOnTimer(
   IQueueTimers &timerQueue,
   IQueueTimers::Handle &handle,
   const Milliseconds timeout1,
   const Milliseconds timeout2,
   const UserData userData)
{
   m_pTimerQueue = &timerQueue;

   m_handle = handle;

   m_timeout1 = timeout1;
   m_timeout2 = timeout2;

   m_userData = userData;
}

bool CLoggingCallbackTimer::WaitForTimer(
   const Milliseconds timeout) const
{
   return m_timerEvent.Wait(timeout);
}

unsigned long CLoggingCallbackTimer::GetNumTimerEvents() const
{
   return m_numTimerEvents.Value();
}

void CLoggingCallbackTimer::OnTimerEx(
   Handle handle,
   UserData userData,
   const bool shuttingDownWhenSet)
{
   (void)handle;

   if (!shuttingDownWhenSet || supportsTimersFiringDuringShutdown)
   {
      OnTimer(userData);
   }
}

void CLoggingCallbackTimer::OnTimer(
   const UserData userData)
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
      if (m_timeout1 != INFINITE)
      {
         const Milliseconds timeout1 = m_timeout1;

         IQueueTimers *pTimerQueue = m_pTimerQueue;

         m_pTimerQueue = nullptr;
         m_timeout1 = INFINITE;

         pTimerQueue->SetTimer(m_handle, *this, timeout1, m_userData);

         LogMessage(_T("TimerSet"));

         if (m_timeout2 != INFINITE)
         {
            const Milliseconds timeout2 = m_timeout2;
         
            m_timeout2 = INFINITE;

            pTimerQueue->SetTimer(m_handle, *this, timeout2, m_userData);

            LogMessage(_T("TimerSet"));
         }

         m_handle = IQueueTimers::InvalidHandleValue;
         m_userData = 0;
      }
      else
      {
         m_pTimerQueue->DestroyTimer(m_handle);

         m_pTimerQueue = nullptr;
         m_handle = IQueueTimers::InvalidHandleValue;

         LogMessage(_T("TimerDestroyed"));
      }
   }

   m_numTimerEvents.Increment();

   m_timerEvent.Set();
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: LoggingCallbackTimer.cpp
///////////////////////////////////////////////////////////////////////////////
