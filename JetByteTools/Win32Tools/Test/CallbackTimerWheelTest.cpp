///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerWheelTest.cpp
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

#include "CallbackTimerWheelTest.h"

#include "..\Mock\MockTickCountProvider.h"
#include "..\Mock\LoggingCallbackTimer.h"
#include "..\Mock\MockTimerQueueMonitor.h"

#include "JetByteTools\Win32Tools\Utils.h"
#include "JetByteTools\Win32Tools\DebugTrace.h"

#include "JetByteTools\TestTools\TestException.h"
#include "JetByteTools\TestTools\RunTest.h"

#pragma hdrstop

#include "..\CallbackTimerWheel.h"

#ifndef JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
#define JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES 0
#endif

#ifndef JETBYTE_TEST_SHOW_NUM_TIMER_EVENTS
#define JETBYTE_TEST_SHOW_NUM_TIMER_EVENTS 0
#endif

#ifndef JETBYTE_PERF_NUM_TEST_RUNS
#define JETBYTE_PERF_NUM_TEST_RUNS 10
#endif

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

const _tstring CCallbackTimerWheelTest::shortName = _T("  W - ");

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerWheelTest
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerWheelTest::TestAll(
   CTestMonitor &monitor)
{
   Base::TestAll(monitor, _T("CCallbackTimerWheel"));

   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestConstruct);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestGetMaximumTimeout);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestGetNextTimeout);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestSetTimerWhenNowNotEqualToCurrent);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestSetTimerWhenNowMoreThanMaxTimeoutLargerThanCurrent);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestSetTimerWhenNowMoreThanMaxTimeoutLargerThanCurrentNoOtherTimersSet);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestOnShotTimerSetTimerWhenNowNotEqualToCurrent);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestOnShotTimerSetTimerWhenNowMoreThanMaxTimeoutLargerThanCurrent);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestOnShotTimerSetTimerWhenNowMoreThanMaxTimeoutLargerThanCurrentNoOtherTimersSet);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestGetNextTimeoutWhenWheelWraps);
}

void CCallbackTimerWheelTest::TestConstruct()
{
   static const Milliseconds maximumTimeout = 4000;

   static const Milliseconds timerGranularity = 50;

   {
      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout);
   }

   CMockTimerQueueMonitor monitor;

   {
      CCallbackTimerWheel CallbackTimerWheel(monitor, maximumTimeout);

      CheckConstructionResults(monitor);
   }

   CheckDestructionResults(monitor);

   {
      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, timerGranularity);
   }

   {
      CCallbackTimerWheel CallbackTimerWheel(monitor, maximumTimeout, timerGranularity);

      CheckConstructionResults(monitor);
   }

   CheckDestructionResults(monitor);

   CMockTickCountProvider tickProvider;

   {
      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);

      CheckConstructionResults(tickProvider);
   }

   {
      CCallbackTimerWheel CallbackTimerWheel(monitor, maximumTimeout, tickProvider);

      CheckConstructionResults(monitor, tickProvider);
   }

   CheckDestructionResults(monitor);

   {
      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, timerGranularity, tickProvider);

      CheckConstructionResults(tickProvider);
   }

   {
      CCallbackTimerWheel CallbackTimerWheel(monitor, maximumTimeout, timerGranularity, tickProvider);

      CheckConstructionResults(monitor, tickProvider);
   }

   CheckDestructionResults(monitor);
}

void CCallbackTimerWheelTest::TestGetMaximumTimeout()
{
   CMockTickCountProvider tickProvider;

   tickProvider.logTickCount = false;

   CMockTimerQueueMonitor monitor;

   {
      static const Milliseconds maximumTimeout = 4000;

      CCallbackTimerWheel CallbackTimerWheel(monitor, maximumTimeout, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      THROW_ON_FAILURE_EX(maximumTimeout == CallbackTimerWheel.GetMaximumTimeout());

      tickProvider.CheckNoResults();
   }

   CheckDestructionResults(monitor);

   {
      static const Milliseconds maximumTimeout = 10;

      CCallbackTimerWheel CallbackTimerWheel(monitor, maximumTimeout, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      THROW_ON_FAILURE_EX(maximumTimeout == CallbackTimerWheel.GetMaximumTimeout());

      tickProvider.CheckNoResults();
   }

   CheckDestructionResults(monitor);

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerWheelTest::TestGetNextTimeout()
{
   CMockTickCountProvider tickProvider;

   tickProvider.logTickCount = false;

   CMockTimerQueueMonitor monitor;

   {
      static const Milliseconds maximumTimeout = 4000;

      static const Milliseconds timerGranularity = 15;

      CCallbackTimerWheel timerWheel(monitor, maximumTimeout, timerGranularity, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerWheel.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer1;

      IQueueTimers::Handle handle1 = CreateAndSetTimer(tickProvider, timerWheel, timer1, 20, 1);

      THROW_ON_FAILURE_EX(30 == timerWheel.GetNextTimeout());     // Timers actual expiry is rounded up to next granularity

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(1);

      THROW_ON_FAILURE_EX(29 == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(15);

      THROW_ON_FAILURE_EX(15 == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(16);

      THROW_ON_FAILURE_EX(14 == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(29);

      THROW_ON_FAILURE_EX(1 == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(30);

      THROW_ON_FAILURE_EX(0 == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      (void)handle1;
   }

   CheckDestructionResults(monitor, _T("|OnTimerCreated|OnTimerSet|OnTimerDeleted|"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerWheelTest::TestSetTimerWhenNowNotEqualToCurrent()
{
   CMockTickCountProvider tickProvider;

   tickProvider.logTickCount = false;

   CMockTimerQueueMonitor monitor;

   {
      static const Milliseconds maximumTimeout = 4000;

      static const Milliseconds timerGranularity = 15;

      CCallbackTimerWheel timerWheel(monitor, maximumTimeout, timerGranularity, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerWheel.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      const Milliseconds timeout1 = 1000;

      Milliseconds now = 0;

      IQueueTimers::Handle handle1 = CreateAndSetTimer(tickProvider, timerWheel, timer, timeout1, 1, now);

      Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout1);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      now = 200;

      tickProvider.SetTickCount(now);        // time moves on

      expectedTimeout = CalculateExpectedTimeout(timeout1, now, 0);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      // Now set another timer for the timeout, this should be set for now + timeout NOT 0 + timeout even
      // though the timer wheel thinks that the current time is 0.

      const Milliseconds timeout2 = 500;

      IQueueTimers::Handle handle2 = CreateAndSetTimer(tickProvider, timerWheel, timer, timeout2, 2, now);

      expectedTimeout = CalculateExpectedTimeout(timeout2, now, now);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      (void)handle1;
      (void)handle2;
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerWheelTest::TestSetTimerWhenNowMoreThanMaxTimeoutLargerThanCurrent()
{
   CMockTickCountProvider tickProvider;

   tickProvider.logTickCount = false;

   CMockTimerQueueMonitor monitor;

   {
      static const Milliseconds maximumTimeout = 4000;

      static const Milliseconds timerGranularity = 15;

      CCallbackTimerWheel timerWheel(monitor, maximumTimeout, timerGranularity, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerWheel.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      const Milliseconds timeout1 = 1000;

      Milliseconds now = 0;

      IQueueTimers::Handle handle1 = CreateAndSetTimer(tickProvider, timerWheel, timer, timeout1, 1, now);

      Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout1);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      now = 2 * maximumTimeout;

      tickProvider.SetTickCount(now);        // time moves on

      expectedTimeout = CalculateExpectedTimeout(timeout1, now, 0);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      // Now try and set another timer. This will fail as the wheel is too far behind
      // real time...

      const Milliseconds timeout2 = 500;

      IQueueTimers::Handle handle2 = timerWheel.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle2);

      THROW_ON_NO_EXCEPTION_EX_4(timerWheel.SetTimer, handle2, timer, timeout2, 2);

      CheckTickProviderFailedSetTimerResults(tickProvider, now);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      (void)handle1;
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerWheelTest::TestSetTimerWhenNowMoreThanMaxTimeoutLargerThanCurrentNoOtherTimersSet()
{
   CMockTickCountProvider tickProvider;

   tickProvider.logTickCount = false;

   CMockTimerQueueMonitor monitor;

   {
      static const Milliseconds maximumTimeout = 4000;

      static const Milliseconds timerGranularity = 15;

      CCallbackTimerWheel timerWheel(monitor, maximumTimeout, timerGranularity, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerWheel.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      const Milliseconds timeout1 = 1000;

      Milliseconds now = 2 * maximumTimeout;

      tickProvider.SetTickCount(now);

      // the wheel's 'current' is zero, now is more than the size of the wheel but since no
      // timers are set the wheel can be adjusted to 'now' before setting the timer

      IQueueTimers::Handle handle1 = CreateAndSetTimer(tickProvider, timerWheel, timer, timeout1, 1, now);

      Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout1);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      (void)handle1;
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerWheelTest::TestOnShotTimerSetTimerWhenNowNotEqualToCurrent()
{
   CMockTickCountProvider tickProvider;

   tickProvider.logTickCount = false;

   CMockTimerQueueMonitor monitor;

   {
      static const Milliseconds maximumTimeout = 4000;

      static const Milliseconds timerGranularity = 15;

      CCallbackTimerWheel timerWheel(monitor, maximumTimeout, timerGranularity, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerWheel.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      const Milliseconds timeout1 = 1000;

      Milliseconds now = 0;

      timerWheel.SetTimer(timer, timeout1, 1);

      CheckTickProviderSetTimerResults(tickProvider);

      Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout1);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      now = 200;

      tickProvider.SetTickCount(now);        // time moves on

      expectedTimeout = CalculateExpectedTimeout(timeout1, now, 0);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      // Now set another timer for the timeout, this should be set for now + timeout NOT 0 + timeout even
      // though the timer wheel thinks that the current time is 0.

      const Milliseconds timeout2 = 500;

      timerWheel.SetTimer(timer, timeout2, 2);

      CheckTickProviderSetTimerResults(tickProvider, now);

      expectedTimeout = CalculateExpectedTimeout(timeout2, now, now);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerWheelTest::TestOnShotTimerSetTimerWhenNowMoreThanMaxTimeoutLargerThanCurrent()
{
   CMockTickCountProvider tickProvider;

   tickProvider.logTickCount = false;

   CMockTimerQueueMonitor monitor;

   {
      static const Milliseconds maximumTimeout = 4000;

      static const Milliseconds timerGranularity = 15;

      CCallbackTimerWheel timerWheel(monitor, maximumTimeout, timerGranularity, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerWheel.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      const Milliseconds timeout1 = 1000;

      Milliseconds now = 0;

      timerWheel.SetTimer(timer, timeout1, 1);

      CheckTickProviderSetTimerResults(tickProvider);

      Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout1);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      now = 2 * maximumTimeout;

      tickProvider.SetTickCount(now);        // time moves on

      expectedTimeout = CalculateExpectedTimeout(timeout1, now, 0);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      // Now try and set another timer. This will fail as the wheel is too far behind
      // real time...

      const Milliseconds timeout2 = 500;

      THROW_ON_NO_EXCEPTION_EX_3(timerWheel.SetTimer, timer, timeout2, 2);

      CheckTickProviderFailedSetTimerResults(tickProvider, now);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerWheelTest::TestOnShotTimerSetTimerWhenNowMoreThanMaxTimeoutLargerThanCurrentNoOtherTimersSet()
{
   CMockTickCountProvider tickProvider;

   tickProvider.logTickCount = false;

   CMockTimerQueueMonitor monitor;

   {
      static const Milliseconds maximumTimeout = 4000;

      static const Milliseconds timerGranularity = 15;

      CCallbackTimerWheel timerWheel(monitor, maximumTimeout, timerGranularity, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerWheel.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      const Milliseconds timeout1 = 1000;

      Milliseconds now = 2 * maximumTimeout;

      tickProvider.SetTickCount(now);

      // the wheel's 'current' is zero, now is more than the size of the wheel but since no
      // timers are set the wheel can be adjusted to 'now' before setting the timer

      timerWheel.SetTimer(timer, timeout1, 1);

      CheckTickProviderSetTimerResults(tickProvider);

      Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout1);

      THROW_ON_FAILURE_EX(expectedTimeout == timerWheel.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerWheelTest::TestGetNextTimeoutWhenWheelWraps()
{
   CMockTickCountProvider tickProvider;

   tickProvider.logTickCount = false;

   CMockTimerQueueMonitor monitor;

   {
      static const Milliseconds maximumTimeout = 4000;

      static const Milliseconds timerGranularity = 15;

      CCallbackTimerWheel timerWheel(monitor, maximumTimeout, timerGranularity, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerWheel.GetNextTimeout());

      tickProvider.CheckNoResults();

      // First step the timer wheel to a point where we can wrap a timer...

      {
         CLoggingCallbackTimer timer;

         IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerWheel, timer, 3000, 1);

         THROW_ON_FAILURE_EX(3015 == timerWheel.GetNextTimeout());     // Timers actual expiry is rounded up to next granularity

         tickProvider.CheckResult(_T("|GetTickCount|"));

         tickProvider.SetTickCount(3015);

         timerWheel.HandleTimeouts();

         tickProvider.CheckResult(_T("|GetTickCount|"));

         timer.CheckResult(_T("|OnTimer: 1|"));

         THROW_ON_FAILURE_EX(INFINITE == timerWheel.GetNextTimeout());

         tickProvider.CheckNoResults();

         (void)handle;
      }

      // Now cause a wrap

      {
         CLoggingCallbackTimer timer;

         IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerWheel, timer, 1000, 2);

         THROW_ON_FAILURE_EX(1005 == timerWheel.GetNextTimeout());     // Timers actual expiry is rounded up to next granularity

         tickProvider.CheckResult(_T("|GetTickCount|"));

         tickProvider.SetTickCount(1005);

         timerWheel.HandleTimeouts();

         tickProvider.CheckResult(_T("|GetTickCount|"));

         timer.CheckResult(_T("|OnTimer: 2|"));

         THROW_ON_FAILURE_EX(INFINITE == timerWheel.GetNextTimeout());

         tickProvider.CheckNoResults();

         (void)handle;
      }
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
// End of file: CallbackTimerWheelTest.cpp
///////////////////////////////////////////////////////////////////////////////

