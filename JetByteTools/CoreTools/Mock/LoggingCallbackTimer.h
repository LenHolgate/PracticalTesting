#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: LoggingCallbackTimer.h
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

#include "JetByteTools/TestTools/TestLog.h"

#include "JetByteTools/CoreTools/IQueueTimers.h"
#include "JetByteTools/CoreTools/AutoResetEvent.h"
#include "JetByteTools/CoreTools/AtomicLong.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CLoggingCallbackTimer
///////////////////////////////////////////////////////////////////////////////

/// A mock object that implements IQueueTimers::Timer so that it can be
/// used as a timer for testing implementations of IQueueTimers.
/// \ingroup Win32ToolsMocks

class CLoggingCallbackTimer :
   public IQueueTimers::Timer,
   public JetByteTools::Test::CTestLog
{
   public :

      CLoggingCallbackTimer();

      explicit CLoggingCallbackTimer(
         CTestLog &linkedLog);

      CLoggingCallbackTimer(
         const CLoggingCallbackTimer &rhs) = delete;

      CLoggingCallbackTimer &operator=(
         const CLoggingCallbackTimer &rhs) = delete;

      bool logMessage;

      bool logUserData;

      void DestroyTimerInOnTimer(
         IQueueTimers &timerQueue,
         IQueueTimers::Handle &handle);

      void SetTimerInOnTimer(
         IQueueTimers &timerQueue,
         IQueueTimers::Handle &handle,
         Milliseconds timeout,
         UserData userData /* = 0*/);

      bool WaitForTimer(
         Milliseconds timeout) const;

      unsigned long GetNumTimerEvents() const;

      // Implement IQueueTimers::Timer

      void OnTimer(
         UserData userData) override;

   private :

      CAutoResetEvent m_timerEvent;

      CAtomicLong m_numTimerEvents;

      IQueueTimers *m_pTimerQueue;

      IQueueTimers::Handle m_handle;

      Milliseconds m_timeout;

      UserData m_userData;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: LoggingCallbackTimer.h
///////////////////////////////////////////////////////////////////////////////
