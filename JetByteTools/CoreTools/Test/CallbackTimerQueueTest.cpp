///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueTest.cpp
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

#include "CallbackTimerQueueTest.h"

#include "JetByteTools/CoreTools/Mock/MockTickCount64Provider.h"
#include "JetByteTools/CoreTools/Mock/LoggingCallbackTimer.h"
#include "JetByteTools/CoreTools/Mock/MockTimerQueueMonitor.h"

#include "JetByteTools/CoreTools/ToString.h"

#include "JetByteTools/TestTools/TestException.h"
#include "JetByteTools/TestTools/RunTest.h"

#pragma hdrstop

#include "JetByteTools/CoreTools/CallbackTimerQueue.h"

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;
using JetByteTools::Test::CTestMonitor;

using JetByteTools::Core::Mock::CMockTickCount64Provider;
using JetByteTools::Core::Mock::CLoggingCallbackTimer;
using JetByteTools::Core::Mock::CMockTimerQueueMonitor;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Test {

const _tstring CCallbackTimerQueueTestTraits::shortName = _T("QEx - ");

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueTest
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerQueueTest::TestAll(
   CTestMonitor &monitor)
{
   Base::TestAll(monitor, _T("CCallbackTimerQueue"));

   RUN_TEST_EX(monitor, CCallbackTimerQueueTest, TestGetMaxTimeout);
   RUN_TEST_EX(monitor, CCallbackTimerQueueTest, TestSetTimerPastTickCount64CountWrap);
}

void CCallbackTimerQueueTest::TestGetMaxTimeout()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      const CCallbackTimerQueue timerQueue(monitor, tickProvider);

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(4294967294U == timerQueue.GetMaximumTimeout());

      tickProvider.CheckNoResults();

      // GetMaximumTimeout() doesn't reduce the value that it returns as the 64bit
      // rollover approaches; which means you WILL get an exception if you try and
      // set a timer that is less than the value returned by GetMaximumTimeout()
      // but which crosses the rollover point. I'll fix this if I get enough
      // repeatable bug reports.

      const ULONGLONG rollOver = 0;
      const ULONGLONG beforeRollOver = rollOver - 1000;

      tickProvider.SetTickCount(beforeRollOver);

      THROW_ON_FAILURE_EX(4294967294U == timerQueue.GetMaximumTimeout());

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueTest::TestSetTimerPastTickCount64CountWrap()
{
   CMockTimerQueueMonitor monitor;

   const ULONGLONG rollOver = 0;
   const ULONGLONG beforeRollOver = rollOver - 1000;

   CMockTickCount64Provider tickProvider(beforeRollOver);

   {
      CLoggingCallbackTimer timer;

      CCallbackTimerQueue timerQueue(monitor, tickProvider);

      const _tstring resultBeforeRollOver = _T("|GetTickCount: ") + ToString(beforeRollOver) + _T("|");

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CCallbackTimerQueue::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(CCallbackTimerQueue::InvalidHandleValue != handle);

      // Set a single timer for 100ms after the tick count rolls to 0.

      THROW_ON_NO_EXCEPTION_EX_4(timerQueue.SetTimer, handle, timer, 1100, 1);

      tickProvider.CheckResult(resultBeforeRollOver);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.CancelTimer(handle));

      // Set a single timer for 1ms before the tick count rolls to 0.

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 999, 1));

      tickProvider.CheckResult(resultBeforeRollOver);

      THROW_ON_FAILURE_EX(999 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(resultBeforeRollOver);

      // Set the time to the point where the tick count rolls over.

      tickProvider.SetTickCount(rollOver);

      // The timer expired 1ms ago...

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      // Check that the time goes off.

      HandleTimeouts(timerQueue);

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timer.CheckResult(_T("|OnTimer: 1|"));

      // Prove that once we HAVE wrapped then all is well again for another zillion years...

      // Set a single timer for 1000ms time.

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 1000, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_FAILURE_EX(1000 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      // Set the tick count to the timeout time.

      tickProvider.SetTickCount(1000);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

      // Check that the timer goes off.

      HandleTimeouts(timerQueue);

      tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

      timer.CheckResult(_T("|OnTimer: 1|"));

      // Check that there are no more timers to go off.

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueueTest.cpp
///////////////////////////////////////////////////////////////////////////////

