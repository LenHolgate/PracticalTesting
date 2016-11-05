///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueTest.cpp
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

#include "CallbackTimerQueueTest.h"

#include "..\Mock\MockTickCountProvider.h"
#include "..\Mock\LoggingCallbackTimer.h"
#include "..\Mock\MockTimerQueueMonitor.h"

#include "JetByteTools\Win32Tools\Utils.h"

#include "JetByteTools\TestTools\TestException.h"
#include "JetByteTools\TestTools\RunTest.h"

#pragma hdrstop

#include "..\CallbackTimerQueue.h"

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;
using JetByteTools::Test::CTestSkippedException;
using JetByteTools::Test::CTestMonitor;

using JetByteTools::Win32::Mock::CMockTickCountProvider;
using JetByteTools::Win32::Mock::CLoggingCallbackTimer;
using JetByteTools::Win32::Mock::CMockTimerQueueMonitor;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

const _tstring CCallbackTimerQueueTestTraits::shortName = _T("  Q - ");

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueTest
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerQueueTest::TestAll(
   CTestMonitor &monitor)
{
   Base::TestAll(monitor, _T("CCallbackTimerQueue"));

   RUN_TEST_EX(monitor, CCallbackTimerQueueTest, TestTickCountWrap);
   RUN_TEST_EX(monitor, CCallbackTimerQueueTest, TestTickCountWrap2);
   RUN_TEST_EX(monitor, CCallbackTimerQueueTest, TestTickCountWrap3);
}

void CCallbackTimerQueueTest::TestTickCountWrap()
{
   CMockTimerQueueMonitor monitor;

   const Milliseconds rollOver = 0;
   const Milliseconds beforeRollOver = rollOver - 1000;

   CMockTickCountProvider tickProvider(beforeRollOver);

   {
      CCallbackTimerQueue timerQueue(monitor, tickProvider);

      const _tstring resultBeforeRollOver = _T("|GetTickCount: ") + ToString(beforeRollOver) + _T("|");

      tickProvider.CheckResult(resultBeforeRollOver);          // creating a queue sets an
                                                               // internal maintenance timer

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      CCallbackTimerQueue::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(CCallbackTimerQueue::InvalidHandleValue != handle);

      // Set a single timer for 100ms after the tick count rolls to 0.

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 1100, 1));

      tickProvider.CheckResult(resultBeforeRollOver);

      THROW_ON_FAILURE_EX(1100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(resultBeforeRollOver);

      // Check that the timer doesn't go off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(resultBeforeRollOver);

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(1100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(resultBeforeRollOver);

      // Set the time to the point where the tick count rolls over.

      tickProvider.SetTickCount(rollOver);

      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      // Because the count has wrapped the maintenance timer is reset, so two calls to GetTickCount...

      tickProvider.CheckResult(_T("|GetTickCount: 0|GetTickCount: 0|"));

      // Check that the timer doesn't go off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timer.CheckNoResults();

      // Set the tick count to the timeout time.

      tickProvider.SetTickCount(100);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      // Check that the timer goes off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      timer.CheckResult(_T("|OnTimer: 1|"));

      // Check that there are no more timers to go off.

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      tickProvider.SetTickCount(beforeRollOver);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      const Milliseconds timeout2 = timerQueue.GetMaximumTimeout();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout2, 1));

      tickProvider.CheckResult(resultBeforeRollOver);

      THROW_ON_FAILURE_EX(timeout2 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(resultBeforeRollOver);

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(resultBeforeRollOver);

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(true == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueTest::TestTickCountWrap2()
{
   CMockTimerQueueMonitor monitor;

   const Milliseconds rollOver = 0;
   const Milliseconds beforeRollOver = rollOver - 1000;

   CMockTickCountProvider tickProvider(beforeRollOver);

   {
      CCallbackTimerQueue timerQueue(monitor, tickProvider);

      const _tstring resultBeforeRollOver = _T("|GetTickCount: ") + ToString(beforeRollOver) + _T("|");

      tickProvider.CheckResult(resultBeforeRollOver);          // creating a queue sets an
                                                               // internal maintenance timer

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CCallbackTimerQueue::Handle handle1 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(CCallbackTimerQueue::InvalidHandleValue != handle1);

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      // Set a timer for 1000ms after the tick count rolls to 0.

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle1, timer, 2000, 1));

      tickProvider.CheckResult(resultBeforeRollOver);

      THROW_ON_FAILURE_EX(2000 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(resultBeforeRollOver);

      // Check that the timer doesn't go off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(resultBeforeRollOver);

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(2000 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(resultBeforeRollOver);

      // Set the time to the point where the tick count rolls over.

      tickProvider.SetTickCount(rollOver);

      THROW_ON_FAILURE_EX(1000 == timerQueue.GetNextTimeout());

      // Because the count has wrapped the maintenance timer is reset, so two calls to GetTickCount...

      tickProvider.CheckResult(_T("|GetTickCount: 0|GetTickCount: 0|"));

      // Check that the timer doesn't go off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timer.CheckNoResults();

      // Set another timer for 10000 ms time

      CCallbackTimerQueue::Handle handle2 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(CCallbackTimerQueue::InvalidHandleValue != handle2);

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle2, timer, 10000, 2));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      // Check that the timer goes off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      // Set the tick count to the timeout time.

      tickProvider.SetTickCount(1000);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

      // Check that the timer goes off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 1000|GetTickCount: 1000|"));

      timer.CheckResult(_T("|OnTimer: 1|"));

      THROW_ON_FAILURE_EX(9000 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

      // Set the tick count to the timeout time.

      tickProvider.SetTickCount(10000);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 10000|"));

      // Check that the timer goes off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 10000|"));

      timer.CheckResult(_T("|OnTimer: 2|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle1));

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle2));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueTest::TestTickCountWrap3()
{
   // If we have a timer before the GetTickCount() wrap point that is set but hasn't been
   // handled and a timer after the wrap point and the tick count wraps and we set a new
   // timer, the three timers should all go off in the right order...

   CMockTimerQueueMonitor monitor;

   const Milliseconds rollOver = 0;
   const Milliseconds beforeRollOver = rollOver - 1000;

   CMockTickCountProvider tickProvider(beforeRollOver);

   {
      CCallbackTimerQueue timerQueue(monitor, tickProvider);

      const _tstring resultBeforeRollOver = _T("|GetTickCount: ") + ToString(beforeRollOver) + _T("|");

      tickProvider.CheckResult(resultBeforeRollOver);          // creating a queue sets an
                                                               // internal maintenance timer

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      // Set a timer for 100ms before the tick count rolls to 0.

      CCallbackTimerQueue::Handle handle1 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(CCallbackTimerQueue::InvalidHandleValue != handle1);

      tickProvider.CheckNoResults();

      const Milliseconds firstTimerTime = 900;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle1, timer, firstTimerTime, 1));

      tickProvider.CheckResult(resultBeforeRollOver);

      THROW_ON_FAILURE_EX(firstTimerTime == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(resultBeforeRollOver);

      // Check that the timer doesn't go off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(resultBeforeRollOver);

      timer.CheckNoResults();

      // Now set a timer for 1000ms after the tick count rolls to 0.

      CCallbackTimerQueue::Handle handle2 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(CCallbackTimerQueue::InvalidHandleValue != handle2);

      tickProvider.CheckNoResults();

      const Milliseconds secondTimerTime = 2000;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle2, timer, secondTimerTime, 2));

      tickProvider.CheckResult(resultBeforeRollOver);

      THROW_ON_FAILURE_EX(firstTimerTime == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(resultBeforeRollOver);

      // Check that the timer doesn't go off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(resultBeforeRollOver);

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(firstTimerTime == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(resultBeforeRollOver);

      // Set the time to the point where the tick count rolls over.

      tickProvider.SetTickCount(rollOver);

      // the first timer will go off now if we call HandleTimeouts()...

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      // Because the count has wrapped the maintenance timer is reset, so two calls to GetTickCount...

      tickProvider.CheckResult(_T("|GetTickCount: 0|GetTickCount: 0|"));

      timer.CheckNoResults();

      // Set another timer for 10000 ms time

      CCallbackTimerQueue::Handle handle3 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(CCallbackTimerQueue::InvalidHandleValue != handle3);

      tickProvider.CheckNoResults();

      const Milliseconds thirdTimerTime = 10000;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle3, timer, thirdTimerTime, 3));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 0|GetTickCount: 0|"));

      timer.CheckResult(_T("|OnTimer: 1|"));

      // Set the tick count to the second timeout time.

      tickProvider.SetTickCount(1000);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

      // Check that the timer goes off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 1000|GetTickCount: 1000|"));

      timer.CheckResult(_T("|OnTimer: 2|"));

      THROW_ON_FAILURE_EX(9000 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

      // Set the tick count to the timeout time.

      tickProvider.SetTickCount(10000);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 10000|"));

      // Check that the timer goes off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 10000|"));

      timer.CheckResult(_T("|OnTimer: 3|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle1));

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle2));

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle3));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueueTest.cpp
///////////////////////////////////////////////////////////////////////////////

