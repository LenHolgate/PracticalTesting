#pragma once
#ifndef JETBYTE_TOOLS_WIN32_TEST_CALLBACK_TIMER_QUEUE_TEST_BASE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_TEST_CALLBACK_TIMER_QUEUE_TEST_BASE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: TCallbackTimerQueueTestBase.h
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

#include "JetByteTools/Win32Tools/Mock/LoggingCallbackTimer.h"
#include "JetByteTools/Win32Tools/Mock/MockTimerQueueMonitor.h"
#include "JetByteTools/Win32Tools/Mock/MockTickCount64Provider.h"

#include "JetByteTools/Win32Tools/DebugTrace.h"
#include "JetByteTools/Win32Tools/IManageTimerQueue.h"

#include "JetByteTools/TestTools/RunTest.h"

///////////////////////////////////////////////////////////////////////////////
// Defines - make sure they're set even if the user doesn't set them.
///////////////////////////////////////////////////////////////////////////////

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
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools
{
   namespace Test
   {
      class CTestMonitor;
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueTestBase
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerQueueTestBase
{
   public :

      static void HandleTimeouts(
         IManageTimerQueue &timerQueue,
         const bool handleAll = false)
      {
         while (timerQueue.BeginTimeoutHandling())
         {
            timerQueue.HandleTimeout();

            timerQueue.EndTimeoutHandling();

            if (!handleAll)
            {
               break;
            }
         }
      }
};

///////////////////////////////////////////////////////////////////////////////
// TCallbackTimerQueueTestBase
///////////////////////////////////////////////////////////////////////////////

/// A test for CCallbackTimerQueue
/// \test
/// \ingroup Win32ToolsTests

template <class TimerQueue, class QueueTraits, class TickProvider>
class TCallbackTimerQueueTestBase : protected CCallbackTimerQueueTestBase
{
   public :

      typedef TCallbackTimerQueueTestBase<TimerQueue, QueueTraits, TickProvider> Base;

      static void TestConstruct();
      static void TestCreateTimer();
      static void TestDestroyTimer();
      static void TestGetNextTimeoutNoTimersSet();
      static void TestHandleTimeoutsNoTimersSetNoTimePassed();
      static void TestHandleTimeoutsNoTimersSet();
      static void TestSetTimer();
      static void TestGetNextTimeoutWithTimerSet();
      static void TestSetTimerAlreadySet();
      static void TestCancelTimerNotSet();
      static void TestCancelTimerInvalidHandle();
      static void TestCancelTimer();
      static void TestTimer();
      static void TestBeginTimeoutHandlingHandleTimeoutEndTimeoutHandling();
      static void TestBeginTimeoutHandlingEndTimeoutHandling();
      static void TestMultipleCallsToBeginTimeoutHandlingWithoutEndTimeoutHandlingFail();
      static void TestMultipleCallsToEndTimeoutHandlingFail();
      static void TestMultipleCallsToHandleTimeoutFail();
      static void TestBeginTimeoutHandlingSetTimer();
      static void TestBeginTimeoutHandlingCancelTimer();
      static void TestBeginTimeoutHandlingDestroyTimer();
      static void TestHandleTimeoutSetTimer();
      static void TestHandleTimeoutSetTimerInOnTimer();
      static void TestHandleTimeoutCancelTimer();
      static void TestHandleTimeoutDestroyTimer();
      static void TestMultipleTimers();
      static void TestCancelExpiredTimer();
      static void TestGetMaxTimeout();
      static void TestMaxTimeout();
      static void TestResetTimer();
      static void TestOneShotTimer();
      static void TestBeginTimeoutHandlingHandleTimeoutEndTimeoutHandlingOneShotTimer();
      static void TestActiveTimersAtDestructionTime();
      static void TestMonitoring();
      static void TestCancelOneOfManyTimersAndThenHandleTimeouts();
      static void TestCancelOneOfManyTimersAndThenBeginTimeoutHandling();

      static void TestDestroyTimerDuringOnTimerInHandleTimeouts();

      static void PerfTestCreateTimer();
      static void PerfTestSetTimer();
      static void PerfTestSetDifferentTimers();
      static void PerfTestSetDifferentTimersSameTimes();
      static void PerfTestHandleTimeouts();
      static void PerfTestBeginTimeoutHandling();

   protected :

      static void TestAll(
         JetByteTools::Test::CTestMonitor &monitor,
         const _tstring &className);

      static IQueueTimers::Handle CreateAndSetTimer(
         TickProvider &tickProvider,
         IQueueTimers &timerQueue,
         IQueueTimers::Timer &timer,
         const Milliseconds timeout,
         const IQueueTimers::UserData userData,
         const Milliseconds expectedTickCount = 0);

      static void CheckConstructionResults(
         TickProvider &tickProvider,
         const Milliseconds expectedTickCount = 0);

      static void CheckTickProviderResults(
         const bool callQueriesTickCount,
         TickProvider  &tickProvider,
         const Milliseconds expectedTickCount);

      static void CheckConstructionResults(
         Mock::CMockTimerQueueMonitor &monitor,
         TickProvider &tickProvider,
         const Milliseconds expectedTickCount = 0);

      static void CheckConstructionResults(
         Mock::CMockTimerQueueMonitor &monitor);

      static void CheckDestructionResults(
         Mock::CMockTimerQueueMonitor &monitor,
         const _tstring &additional = _T("|"));

      static void CheckTickProviderSetTimerResults(
         TickProvider &tickProvider,
         const Milliseconds expectedTickCount = 0);

      static void CheckTickProviderFailedSetTimerResults(
         TickProvider &tickProvider,
         const Milliseconds expectedTickCount = 0);

      static void CheckTickProviderHandleTimeoutsResults(
         const size_t numDifferentTimes,
         TickProvider &tickProvider,
         const Milliseconds expectedTickCount = 0,
         const bool moreTimersSet = true);

      static Milliseconds CalculateExpectedTimeout(
         const Milliseconds timeout,
         const Milliseconds now = 0,
         const Milliseconds timerSetTime = 0);
};

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestAll(
   JetByteTools::Test::CTestMonitor &monitor,
   const _tstring &className)
{
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestConstruct);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestCreateTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestDestroyTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestGetNextTimeoutNoTimersSet);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestHandleTimeoutsNoTimersSetNoTimePassed);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestHandleTimeoutsNoTimersSet);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestSetTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestGetNextTimeoutWithTimerSet);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestSetTimerAlreadySet);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestCancelTimerNotSet);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestCancelTimerInvalidHandle);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestCancelTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestBeginTimeoutHandlingHandleTimeoutEndTimeoutHandling);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestBeginTimeoutHandlingEndTimeoutHandling);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestMultipleCallsToBeginTimeoutHandlingWithoutEndTimeoutHandlingFail);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestMultipleCallsToEndTimeoutHandlingFail);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestMultipleCallsToHandleTimeoutFail);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestBeginTimeoutHandlingSetTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestBeginTimeoutHandlingCancelTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestBeginTimeoutHandlingDestroyTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestHandleTimeoutSetTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestHandleTimeoutSetTimerInOnTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestHandleTimeoutCancelTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestHandleTimeoutDestroyTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestCancelExpiredTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestResetTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestGetMaxTimeout);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestMaxTimeout);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestMultipleTimers);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestOneShotTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestBeginTimeoutHandlingHandleTimeoutEndTimeoutHandlingOneShotTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestActiveTimersAtDestructionTime);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestMonitoring);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestCancelOneOfManyTimersAndThenHandleTimeouts);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestCancelOneOfManyTimersAndThenBeginTimeoutHandling);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestDestroyTimerDuringOnTimerInHandleTimeouts);


   const Milliseconds timeout = INFINITE;    // Don't time out the perf tests

   RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, PerfTestCreateTimer, timeout);
   RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, PerfTestSetTimer, timeout);
   RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, PerfTestSetDifferentTimers, timeout);
   RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, PerfTestSetDifferentTimersSameTimes, timeout);
   RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, PerfTestHandleTimeouts, timeout);
   RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, PerfTestBeginTimeoutHandling, timeout);
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestConstruct()
{
   {
      Q timerQueue;
   }

   P tickProvider;

   {
      Q timerQueue(tickProvider);

      CheckConstructionResults(tickProvider);
   }

   Mock::CMockTimerQueueMonitor monitor;

   {
      Q timerQueue(monitor);

      CheckConstructionResults(monitor);
   }

   CheckDestructionResults(monitor);

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(monitor, tickProvider);
   }

   CheckDestructionResults(monitor);

}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestCreateTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      const IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestDestroyTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      IQueueTimers::Handle handleCopy = T::handleValidationEnabled ? handle : 0;

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue == handle);

      tickProvider.CheckNoResults();

      if (T::handleValidationEnabled)
      {
         THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, handleCopy); //lint !e534 (Ignoring return value of function)

         tickProvider.CheckNoResults();

         THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, IQueueTimers::InvalidHandleValue); //lint !e534 (Ignoring return value of function)

         tickProvider.CheckNoResults();
      }
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestGetNextTimeoutNoTimersSet()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestHandleTimeoutsNoTimersSetNoTimePassed()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      HandleTimeouts(timerQueue);

      CheckTickProviderHandleTimeoutsResults(0, tickProvider);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestHandleTimeoutsNoTimersSet()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      const Milliseconds now = 100;

      tickProvider.SetTickCount(now);

      HandleTimeouts(timerQueue);

      CheckTickProviderHandleTimeoutsResults(0, tickProvider, now);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestSetTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      Mock::CLoggingCallbackTimer timer;

      const IQueueTimers::UserData userData = 1;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 1000, userData));

      CheckTickProviderSetTimerResults(tickProvider);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestGetNextTimeoutWithTimerSet()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, /*timerGranularity, */tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const IQueueTimers::UserData userData = 1;

      const Milliseconds timeout = 1000;

      CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData); //lint !e534 (Ignoring return value of function)

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(1);

      THROW_ON_FAILURE_EX(expectedTimeout - 1 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(20);

      THROW_ON_FAILURE_EX(expectedTimeout - 20 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(2000);

      THROW_ON_FAILURE_EX(2000 > expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestSetTimerAlreadySet()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, 1000, userData);

      THROW_ON_FAILURE_EX(true == timerQueue.SetTimer(handle, timer, 1200, userData));

      timer.CheckNoResults();

      CheckTickProviderSetTimerResults(tickProvider);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestCancelTimerNotSet()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.CancelTimer(handle));

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestCancelTimerInvalidHandle()
{
   if (T::handleValidationEnabled)
   {
      Mock::CMockTimerQueueMonitor monitor;

      P tickProvider;

      {
         Q timerQueue(monitor, tickProvider);

         CheckConstructionResults(tickProvider);

         IQueueTimers::Handle handle = IQueueTimers::InvalidHandleValue;

         THROW_ON_NO_EXCEPTION_EX_1(timerQueue.CancelTimer, handle);

         tickProvider.CheckNoResults();

         handle = 1;    // any old value

         THROW_ON_NO_EXCEPTION_EX_1(timerQueue.CancelTimer, handle);

         tickProvider.CheckNoResults();
      }

      THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
   }
   else
   {
      SKIP_TEST_EX(_T("TestCancelTimerInvalidHandle - only availble when handle validation is enabled"));
   }
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestCancelTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, 1000, userData);

      THROW_ON_FAILURE_EX(true == timerQueue.CancelTimer(handle));

      timer.CheckNoResults();

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue);

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue);

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckResult(_T("|OnTimer: 1|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestBeginTimeoutHandlingHandleTimeoutEndTimeoutHandling()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 1000;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      THROW_ON_FAILURE_EX(false == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout();

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestBeginTimeoutHandlingEndTimeoutHandling()
{
   // You do NOT need to call HandleTimeout() on the timeoutHandle that you
   // get from BeginTimeoutHandling()

   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      timerQueue.EndTimeoutHandling();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMultipleCallsToBeginTimeoutHandlingWithoutEndTimeoutHandlingFail()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX(timerQueue.BeginTimeoutHandling);

      tickProvider.CheckNoResults();

      timer.CheckNoResults();

      timerQueue.EndTimeoutHandling();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMultipleCallsToEndTimeoutHandlingFail()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      timerQueue.EndTimeoutHandling();

      THROW_ON_NO_EXCEPTION_EX(timerQueue.EndTimeoutHandling);

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMultipleCallsToHandleTimeoutFail()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout();

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX(timerQueue.HandleTimeout);

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestBeginTimeoutHandlingSetTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      Milliseconds now = expectedTimeout;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      // now call set timer to set a new timer whilst the current time is going off...
      // note that this should return false (not currently set) and the new timer and
      // user data should not affect the timer that is in the process of going off.

      Mock::CLoggingCallbackTimer timer2;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer2, timeout, 2));

      expectedTimeout = CalculateExpectedTimeout(timeout, now, now);

      CheckTickProviderSetTimerResults(tickProvider, now);

      timerQueue.HandleTimeout();

      timer.CheckResult(_T("|OnTimer: 1|"));
      timer2.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();
      timer2.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      now += expectedTimeout;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();
      timer2.CheckNoResults();

      timerQueue.HandleTimeout();

      timer.CheckNoResults();
      timer2.CheckResult(_T("|OnTimer: 2|"));

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();
      timer2.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestBeginTimeoutHandlingCancelTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      // now call cancel timer to cancel the timer that is going off...
      // Note that this should return false (not currently set) and should not
      // affect the timer that is in the process of going off.

      THROW_ON_FAILURE_EX(false == timerQueue.CancelTimer(handle));

      tickProvider.CheckNoResults();

      timerQueue.HandleTimeout();

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestBeginTimeoutHandlingDestroyTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      // now call destroy timer to destroy the timer that is going off...
      // Note that this should return false (not currently set) and should NOT
      // affect the timer that is in the process of going off, but SHOULD result
      // in all resources being cleaned up when the time has gone off.

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));

      if (T::handleValidationEnabled)
      {
         THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, handle);
      }

      tickProvider.CheckNoResults();

      timerQueue.HandleTimeout();

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      if (T::handleValidationEnabled)
      {
         THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, handle);
      }
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestHandleTimeoutSetTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      Milliseconds now = expectedTimeout;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout();

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      // now call set timer to set a new timer whilst the current time is going off...
      // note that this should return false (not currently set) and the new timer and
      // user data should not affect the timer that is in the process of going off.

      Mock::CLoggingCallbackTimer timer2;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer2, timeout, 2));

      expectedTimeout = CalculateExpectedTimeout(timeout, now, now);

      CheckTickProviderSetTimerResults(tickProvider, now);

      timer2.CheckNoResults();

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();
      timer2.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      now += expectedTimeout;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();
      timer2.CheckNoResults();

      timerQueue.HandleTimeout();

      timer.CheckNoResults();
      timer2.CheckResult(_T("|OnTimer: 2|"));

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();
      timer2.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestHandleTimeoutSetTimerInOnTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      Milliseconds now = expectedTimeout;

      tickProvider.SetTickCount(now);

      // Set up so that set timer is called from within OnTimer and it resets the current timer
      // note that this should return false (not currently set) and the new timer and
      // user data should not affect the timer that is in the process of going off.

      timer.SetTimerInOnTimer(timerQueue, handle, 500, 2);

      expectedTimeout = CalculateExpectedTimeout(500, now, now);

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout();

      timer.CheckResult(_T("|OnTimer: 1|TimerSet|"));

      tickProvider.CheckResult(_T("|GetTickCount|"));

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      now += expectedTimeout;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout();

      timer.CheckResult(_T("|OnTimer: 2|"));

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestHandleTimeoutCancelTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout();

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      // now call cancel timer to cancel the timer that is going off...
      // Note that this should return false (not currently set) and should not
      // affect the timer that is in the process of going off.

      THROW_ON_FAILURE_EX(false == timerQueue.CancelTimer(handle));

      tickProvider.CheckNoResults();

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestHandleTimeoutDestroyTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout();

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      // now call destroy timer to destroy the timer that is going off...
      // Note that this should return false (not currently set) and should NOT
      // affect the timer that is in the process of going off, but SHOULD result
      // in all resources being cleaned up when the time has gone off.

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));

      if (T::handleValidationEnabled)
      {
         THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, handle);
      }

      tickProvider.CheckNoResults();

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      if (T::handleValidationEnabled)
      {
         THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, handle);
      }
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}


template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestCancelExpiredTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue);

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckResult(_T("|OnTimer: 1|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.CancelTimer(handle));

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestGetMaxTimeout()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      THROW_ON_FAILURE_EX(T::maxTimeout == timerQueue.GetMaximumTimeout());

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMaxTimeout()
{
   Mock::CMockTimerQueueMonitor monitor;

   const Milliseconds initialTickCount = 1000;

   P tickProvider(initialTickCount);

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider, initialTickCount);

      const Milliseconds maxTimeout = timerQueue.GetMaximumTimeout();

      Mock::CLoggingCallbackTimer timer;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, maxTimeout, userData);

//      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      const Milliseconds illegalTimeout = maxTimeout + 1;

      THROW_ON_NO_EXCEPTION_EX_4(timerQueue.SetTimer, handle, timer, illegalTimeout, 1);

      CheckTickProviderFailedSetTimerResults(tickProvider, initialTickCount);

      THROW_ON_FAILURE_EX(true == timerQueue.CancelTimer(handle));

      THROW_ON_NO_EXCEPTION_EX_4(timerQueue.SetTimer, handle, timer, illegalTimeout, 1);

      CheckTickProviderFailedSetTimerResults(tickProvider, initialTickCount);

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));

      timer.CheckNoResults();
      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMultipleTimers()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      Mock::CLoggingCallbackTimer timer1;
      Mock::CLoggingCallbackTimer timer2;
      Mock::CLoggingCallbackTimer timer3;
      Mock::CLoggingCallbackTimer timer4;
      Mock::CLoggingCallbackTimer timer5;
      Mock::CLoggingCallbackTimer timer6;

      const Milliseconds timeout1 = 100;
      const Milliseconds timeout2 = 150;
      const Milliseconds timeout3 = 175;     // must be at least timer granularity apart!
      const Milliseconds timeout4 = 200;
      const Milliseconds timeout5 = 300;

      IQueueTimers::Handle handle1 = CreateAndSetTimer(tickProvider, timerQueue, timer1, timeout5, 1);
      IQueueTimers::Handle handle2 = CreateAndSetTimer(tickProvider, timerQueue, timer2, timeout1, 2);
      IQueueTimers::Handle handle3 = CreateAndSetTimer(tickProvider, timerQueue, timer3, timeout4, 3);
      IQueueTimers::Handle handle4 = CreateAndSetTimer(tickProvider, timerQueue, timer4, timeout2, 4);
      IQueueTimers::Handle handle5 = CreateAndSetTimer(tickProvider, timerQueue, timer5, timeout2, 5);
      IQueueTimers::Handle handle6 = CreateAndSetTimer(tickProvider, timerQueue, timer6, timeout3, 6);

      Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout1);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue);

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer1.CheckNoResults();
      timer2.CheckNoResults();
      timer3.CheckNoResults();
      timer4.CheckNoResults();
      timer5.CheckNoResults();
      timer6.CheckNoResults();

      Milliseconds now = expectedTimeout;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue, T::handleTimeoutQueriesTicksPerTimer);

      CheckTickProviderHandleTimeoutsResults(1, tickProvider, now);

      timer1.CheckNoResults();
      timer2.CheckResult(_T("|OnTimer: 2|"));
      timer3.CheckNoResults();
      timer4.CheckNoResults();
      timer5.CheckNoResults();
      timer6.CheckNoResults();

      expectedTimeout = CalculateExpectedTimeout(timeout2, now);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      now += expectedTimeout + 5;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue, T::handleTimeoutQueriesTicksPerTimer);

      CheckTickProviderHandleTimeoutsResults(1, tickProvider, now);

      timer1.CheckNoResults();
      timer2.CheckNoResults();
      timer3.CheckNoResults();
      timer4.CheckResult(_T("|OnTimer: 4|"));
      timer5.CheckResult(_T("|OnTimer: 5|"));
      timer6.CheckNoResults();

      expectedTimeout = CalculateExpectedTimeout(timeout3, now);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      expectedTimeout = CalculateExpectedTimeout(timeout4, now);

      now += expectedTimeout + 1;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue, T::handleTimeoutQueriesTicksPerTimer);

      CheckTickProviderHandleTimeoutsResults(2, tickProvider, now);

      timer1.CheckNoResults();
      timer2.CheckNoResults();
      timer3.CheckResult(_T("|OnTimer: 3|"));
      timer4.CheckNoResults();
      timer5.CheckNoResults();
      timer6.CheckResult(_T("|OnTimer: 6|"));

      expectedTimeout = CalculateExpectedTimeout(timeout5, now);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      now += expectedTimeout;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue, T::handleTimeoutQueriesTicksPerTimer);

      CheckTickProviderHandleTimeoutsResults(1, tickProvider, now, false);

      timer1.CheckResult(_T("|OnTimer: 1|"));
      timer2.CheckNoResults();
      timer3.CheckNoResults();
      timer4.CheckNoResults();
      timer5.CheckNoResults();
      timer6.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle1));
      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle2));
      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle3));
      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle4));
      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle5));
      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle6));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestResetTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout1 = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout1, userData);

      const Milliseconds expectedTimeout1 = CalculateExpectedTimeout(timeout1);

      THROW_ON_FAILURE_EX(expectedTimeout1 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      const Milliseconds timeout2 = 90;

      THROW_ON_FAILURE_EX(true == timerQueue.SetTimer(handle, timer, timeout2, 2));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout2 = CalculateExpectedTimeout(timeout2);

      THROW_ON_FAILURE_EX(expectedTimeout2 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      Milliseconds now = expectedTimeout2;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue);

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckResult(_T("|OnTimer: 2|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      const Milliseconds timeout3 = 110;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout3, 3));

      CheckTickProviderSetTimerResults(tickProvider, expectedTimeout2);

      const Milliseconds expectedTimeout3 = CalculateExpectedTimeout(timeout3, now, now);

      THROW_ON_FAILURE_EX(expectedTimeout3 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      now += expectedTimeout3;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue);

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckResult(_T("|OnTimer: 3|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestOneShotTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      timerQueue.SetTimer(timer, timeout, 1);

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue);

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue);

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckResult(_T("|OnTimer: 1|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestBeginTimeoutHandlingHandleTimeoutEndTimeoutHandlingOneShotTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      timerQueue.SetTimer(timer, timeout, 1);

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      THROW_ON_FAILURE_EX(false == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout();

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      timerQueue.EndTimeoutHandling();

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestActiveTimersAtDestructionTime()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      const IQueueTimers::Handle handle2 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle2);

      timerQueue.SetTimer(timer, 250, 1);

      timerQueue.SetTimer(timer, 2500, 1);

      HandleTimeouts(timerQueue);

      timer.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMonitoring()
{
   if (!T::monitoringEnabled)
   {
      SKIP_TEST_EX(_T("Monitoring is not enabled"));
   }

   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(monitor, tickProvider);

      Mock::CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      monitor.CheckResult(_T("|OnTimerCreated|"));

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      monitor.CheckResult(_T("|OnTimerSet|"));

      THROW_ON_FAILURE_EX(true == timerQueue.SetTimer(handle, timer, 200, 1));

      monitor.CheckResult(_T("|OnTimerSet: Was Pending|"));

      THROW_ON_FAILURE_EX(true == timerQueue.CancelTimer(handle));

      monitor.CheckResult(_T("|OnTimerCancelled: Was Pending|"));

      THROW_ON_FAILURE_EX(false == timerQueue.CancelTimer(handle));

      monitor.CheckResult(_T("|OnTimerCancelled|"));

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      monitor.CheckResult(_T("|OnTimerSet|"));

      Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      Milliseconds now = expectedTimeout;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      HandleTimeouts(timerQueue);

      timer.CheckResult(_T("|OnTimer: 1|"));

      monitor.CheckResult(_T("|OnTimer|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));

      monitor.CheckResult(_T("|OnTimerDestroyed|OnTimerDeleted|"));

      handle = timerQueue.CreateTimer();

      monitor.CheckResult(_T("|OnTimerCreated|"));

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      monitor.CheckResult(_T("|OnTimerSet|"));

      THROW_ON_FAILURE_EX(true == timerQueue.DestroyTimer(handle));

      monitor.CheckResult(_T("|OnTimerDestroyed: Was Pending|OnTimerDeleted|"));

      timerQueue.SetTimer(timer, timeout, 2);

      monitor.CheckResult(_T("|OnTimerCreated|OnOneOffTimerSet|"));

      expectedTimeout = CalculateExpectedTimeout(timeout, now, now);

      now += expectedTimeout;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      HandleTimeouts(timerQueue);

      timer.CheckResult(_T("|OnTimer: 2|"));

      monitor.CheckResult(_T("|OnTimer|OnTimerDeleted|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      timerQueue.SetTimer(timer, timeout, 3);

      monitor.CheckResult(_T("|OnTimerCreated|OnOneOffTimerSet|"));
   }

   CheckDestructionResults(
      monitor,
      _T("|OnTimerDeleted|"));       // The pending 'one off' timer is deleted in the destructor

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestCancelOneOfManyTimersAndThenHandleTimeouts()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      IQueueTimers::Handle handle1 = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, 1);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      IQueueTimers::Handle handle2 = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, 2);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      IQueueTimers::Handle handle3 = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, 3);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      THROW_ON_FAILURE_EX(true == timerQueue.CancelTimer(handle2));

      timer.CheckNoResults();

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_IF_NOT_EQUAL_EX(0, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      HandleTimeouts(timerQueue);

      tickProvider.CheckResult(_T("|GetTickCount|"));

      if (T::timersAtSameTimeAreExpiredInOrderSet)
      {
         timer.CheckResult(_T("|OnTimer: 1|OnTimer: 3|"));
      }
      else
      {
         timer.CheckResult(_T("|OnTimer: 3|OnTimer: 1|"));
      }

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle1));

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle2));

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle3));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestCancelOneOfManyTimersAndThenBeginTimeoutHandling()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      IQueueTimers::Handle handle1 = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, 1);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      IQueueTimers::Handle handle2 = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, 2);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      IQueueTimers::Handle handle3 = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, 3);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      THROW_ON_FAILURE_EX(true == timerQueue.CancelTimer(handle2));

      timer.CheckNoResults();

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      THROW_ON_FAILURE_EX(true == timerQueue.BeginTimeoutHandling());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timerQueue.HandleTimeout();

      if (T::timersAtSameTimeAreExpiredInOrderSet)
      {
         timer.CheckResult(_T("|OnTimer: 1|OnTimer: 3|"));
      }
      else
      {
         timer.CheckResult(_T("|OnTimer: 3|OnTimer: 1|"));
      }

      timerQueue.EndTimeoutHandling();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle1));

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle2));

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle3));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestDestroyTimerDuringOnTimerInHandleTimeouts()
{
   Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   tickProvider.logTickCount = false;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      Mock::CLoggingCallbackTimer timer;

      const Milliseconds timeout = 100;

      const IQueueTimers::UserData userData = 1;

      IQueueTimers::Handle handle = CreateAndSetTimer(tickProvider, timerQueue, timer, timeout, userData);

      timer.DestroyTimerInOnTimer(timerQueue, handle);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_IF_NOT_EQUAL_EX(expectedTimeout, timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount|"));

      tickProvider.SetTickCount(expectedTimeout);

      HandleTimeouts(timerQueue);

      tickProvider.CheckResult(_T("|GetTickCount|"));

      timer.CheckResult(_T("|OnTimer: 1|TimerDestroyed|"));

      tickProvider.CheckNoResults();

      if (T::handleValidationEnabled)
      {
         THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, handle);
      }
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

// Performance tests

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::PerfTestCreateTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   P tickProvider;

   tickProvider.logMessages = false;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      Q timerQueue(monitor, tickProvider);

      CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

      for (size_t i = 0; i < 100000; ++i)
      {
         IQueueTimers::Handle handle = timerQueue.CreateTimer();

         (void)handle;
      }

      const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

      totalTime += timeTaken;

      #if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
      OutputEx(T::shortName + ToString(timeTaken) + _T("ms - CreateTimer (100,000 calls)"));
      #endif
   }

   OutputEx(T::shortName + ToString(totalTime/numRuns) + _T("ms - CreateTimer (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::PerfTestSetTimer()
{
   Mock::CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   Mock::CLoggingCallbackTimer timer;

   P tickProvider;

   tickProvider.logMessages = false;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      Q timerQueue(monitor, tickProvider);

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

      for (size_t i = 0; i < 100000; ++i)
      {
         timerQueue.SetTimer(handle, timer, static_cast<Milliseconds>(i % 4000), 0); //lint !e534 (Ignoring return value of function)
      }

      const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

      totalTime += timeTaken;

      #if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
      OutputEx(T::shortName + ToString(timeTaken) + _T("ms - SetTimer (100,000 calls)"));
      #endif
   }

   OutputEx(T::shortName + ToString(totalTime/numRuns) + _T("ms - SetTimer (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::PerfTestSetDifferentTimers()
{
   Mock::CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   Mock::CLoggingCallbackTimer timer;

   P tickProvider;

   tickProvider.logMessages = false;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      Q timerQueue(monitor, tickProvider);

      typedef std::vector<IQueueTimers::Handle> Handles;

      Handles handles;

      static const size_t s_numHandles = 1000;

      handles.resize(s_numHandles);

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         handles[i] = timerQueue.CreateTimer();
      }

      {
         CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

         for (size_t i = 0; i < 100000; ++i)
         {
            timerQueue.SetTimer(handles[i % s_numHandles], timer, static_cast<Milliseconds>(i % 4000), 0); //lint !e534 (Ignoring return value of function)
         }

         const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

         totalTime += timeTaken;

         #if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
         OutputEx(T::shortName + ToString(timeTaken) + _T("ms - SetTimer (Different timers) (100,000 calls)"));
         #endif
      }

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         timerQueue.DestroyTimer(handles[i]); //lint !e534 (Ignoring return value of function)
      }
   }

   OutputEx(T::shortName + ToString(totalTime/numRuns) + _T("ms - SetTimer (Different timers) (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::PerfTestSetDifferentTimersSameTimes()
{
   Mock::CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   Mock::CLoggingCallbackTimer timer;

   P tickProvider;

   tickProvider.logMessages = false;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      Q timerQueue(monitor, tickProvider);

      typedef std::vector<IQueueTimers::Handle> Handles;

      Handles handles;

      static const size_t s_numHandles = 1000;

      handles.resize(s_numHandles);

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         handles[i] = timerQueue.CreateTimer();
      }

      {
         CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

         for (size_t i = 0; i < 100; ++i)
         {
            for (size_t x = 0; x < s_numHandles; ++x)
            {
               timerQueue.SetTimer(handles[x], timer, static_cast<Milliseconds>(((1 + i) * 100) % 4000), 0);
            }
         }

         const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

         totalTime += timeTaken;

         #if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
         OutputEx(T::shortName + ToString(timeTaken) + _T("ms - SetTimer (Different timers, same times) (100,000 calls)"));
         #endif
      }

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         timerQueue.DestroyTimer(handles[i]); //lint !e534 (Ignoring return value of function)
      }
   }

   OutputEx(T::shortName + ToString(totalTime/numRuns) + _T("ms - SetTimer (Different timers, same times) (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::PerfTestHandleTimeouts()
{
   Mock::CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   P tickProvider;

   tickProvider.logMessages = false;

   Milliseconds now = 0;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      Q timerQueue(monitor, tickProvider);

      Mock::CLoggingCallbackTimer timer;

      timer.logMessage = false;

      typedef std::vector<IQueueTimers::Handle> Handles;

      Handles handles;

      static const size_t s_numHandles = 100000;

      handles.resize(s_numHandles);

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         handles[i] = timerQueue.CreateTimer();

         timerQueue.SetTimer(handles[i], timer, static_cast<Milliseconds>(i % 4000), 0); //lint !e534 (Ignoring return value of function)
      }

      {
         CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

         Milliseconds nextTimeout = timerQueue.GetNextTimeout();

         while (nextTimeout != INFINITE)
         {
            now += nextTimeout;

            tickProvider.SetTickCount(now);

            HandleTimeouts(timerQueue);

            #if JETBYTE_TEST_SHOW_NUM_TIMER_EVENTS
            OutputEx(ToString(timer.GetNumTimerEvents()));
            #endif

            nextTimeout = timerQueue.GetNextTimeout();
         }

         THROW_ON_FAILURE_EX(s_numHandles == timer.GetNumTimerEvents());

         const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

         totalTime += timeTaken;

         #if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
         OutputEx(T::shortName + ToString(timeTaken) + _T("ms - HandleTimeouts (100,000 timers)"));
         #endif
      }

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         timerQueue.DestroyTimer(handles[i]); //lint !e534 (Ignoring return value of function)
      }
   }

   OutputEx(T::shortName + ToString(totalTime/numRuns) + _T("ms - HandleTimeouts (100,000 timers - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::PerfTestBeginTimeoutHandling()
{
   Mock::CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   P tickProvider;

   tickProvider.logMessages = false;

   Milliseconds now = 0;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      Q timerQueue(monitor, tickProvider);

      Mock::CLoggingCallbackTimer timer;

      timer.logMessage = false;

      typedef std::vector<IQueueTimers::Handle> Handles;

      Handles handles;

      static const size_t s_numHandles = 100000;

      handles.resize(s_numHandles);

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         handles[i] = timerQueue.CreateTimer();

         timerQueue.SetTimer(handles[i], timer, static_cast<Milliseconds>(i % 4000), 0); //lint !e534 (Ignoring return value of function)
      }

      {
         CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

         Milliseconds nextTimeout = timerQueue.GetNextTimeout();

         while (nextTimeout != INFINITE)
         {
            now += nextTimeout;

            tickProvider.SetTickCount(now);

            if (timerQueue.BeginTimeoutHandling())
            {
               timerQueue.HandleTimeout();

               timerQueue.EndTimeoutHandling();
            }

            #if JETBYTE_TEST_SHOW_NUM_TIMER_EVENTS
            OutputEx(ToString(timer.GetNumTimerEvents()));
            #endif

            nextTimeout = timerQueue.GetNextTimeout();
         }

         THROW_ON_FAILURE_EX(s_numHandles == timer.GetNumTimerEvents());

         const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

         totalTime += timeTaken;

         #if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
         OutputEx(T::shortName + ToString(timeTaken) + _T("ms - BeginTimeoutHandling (100,000 timers)"));
         #endif
      }

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         timerQueue.DestroyTimer(handles[i]); //lint !e534 (Ignoring return value of function)
      }
   }

   OutputEx(T::shortName + ToString(totalTime/numRuns) + _T("ms - BeginTimeoutHandling (100,000 timers - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
IQueueTimers::Handle TCallbackTimerQueueTestBase<Q, T, P>::CreateAndSetTimer(
   P &tickProvider,
   IQueueTimers &timerQueue,
   IQueueTimers::Timer &timer,
   const Milliseconds timeout,
   const IQueueTimers::UserData userData,
   const Milliseconds expectedTickCount)
{
   const IQueueTimers::Handle handle = timerQueue.CreateTimer();

   THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

   THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, userData));

   CheckTickProviderSetTimerResults(tickProvider, expectedTickCount);

   return handle;
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::CheckConstructionResults(
   P &tickProvider,
   const Milliseconds expectedTickCount)
{
   CheckTickProviderResults(T::creationQueriesTicks, tickProvider, expectedTickCount);
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::CheckTickProviderResults(
   const bool callQueriesTickCount,
   P &tickProvider,
   const Milliseconds expectedTickCount)
{
   if (callQueriesTickCount)
   {
      if (tickProvider.logTickCount)
      {
         tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTickCount) + _T("|"));
      }
      else
      {
         tickProvider.CheckResult(_T("|GetTickCount|"));
      }
   }
   else
   {
      tickProvider.CheckNoResults();
   }
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::CheckConstructionResults(
   Mock::CMockTimerQueueMonitor &monitor,
   P &tickProvider,
   const Milliseconds expectedTickCount)
{
   CheckConstructionResults(monitor);

   CheckTickProviderResults(T::creationQueriesTicks, tickProvider, expectedTickCount);
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::CheckConstructionResults(
   Mock::CMockTimerQueueMonitor &monitor)
{
   if (T::monitoringEnabled && T::creationCreatesTimer)
   {
      monitor.CheckResult(_T("|OnTimerCreated|OnTimerSet|"));
   }
   else
   {
      monitor.CheckNoResults();
   }
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::CheckDestructionResults(
   Mock::CMockTimerQueueMonitor &monitor,
   const _tstring &additional)
{
   if (T::monitoringEnabled)
   {
      if (T::creationCreatesTimer)
      {
         monitor.CheckResult(_T("|OnTimerDestroyed: Was Pending|OnTimerDeleted") + additional);
      }
      else
      {
         monitor.CheckResult(additional);
      }
   }
   else
   {
      monitor.CheckNoResults();
   }
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::CheckTickProviderSetTimerResults(
   P &tickProvider,
   const Milliseconds expectedTickCount)
{
   CheckTickProviderResults(T::setQueriesTicks, tickProvider, expectedTickCount);
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::CheckTickProviderFailedSetTimerResults(
   P &tickProvider,
   const Milliseconds expectedTickCount)
{
   CheckTickProviderResults(T::failedSetQueriesTicks, tickProvider, expectedTickCount);
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::CheckTickProviderHandleTimeoutsResults(
   const size_t numDifferentTimes,
   P &tickProvider,
   const Milliseconds expectedTickCount,
   const bool moreTimersSet)
{
   if (T::handleTimeoutQueriesTicksPerTimer)
   {
      if (numDifferentTimes)
      {
         const _tstring expectedTickCountAsString = ToString(expectedTickCount);

         _tstring result;

         const size_t numCalls = numDifferentTimes + (moreTimersSet ? 1 : 0);

         for (size_t i = 0; i < numCalls; ++i)
         {
            if (tickProvider.logTickCount)
            {
               result += _T("|GetTickCount: ") + expectedTickCountAsString;
            }
            else
            {
               result += _T("|GetTickCount");
            }
         }

         result += _T("|");

         tickProvider.CheckResult(result);
      }
      else
      {
         tickProvider.CheckNoResults();
      }
   }
   else
   {
      if (tickProvider.logTickCount)
      {
         tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTickCount) + _T("|"));
      }
      else
      {
         tickProvider.CheckResult(_T("|GetTickCount|"));
      }
   }
}

template <class Q, class T, class P>
Milliseconds TCallbackTimerQueueTestBase<Q, T, P>::CalculateExpectedTimeout(
   const Milliseconds timeout,
   const Milliseconds now,
   const Milliseconds timerSetTime)
{
#pragma warning(suppress: 6326)  // Constant constant comparison
   if (T::timerGranularity == 1)
   {
      return timeout - (now - timerSetTime);
   }

   const Milliseconds absoluteTimeout = timerSetTime + timeout;

   const Milliseconds expectedTimeout = ((absoluteTimeout / T::timerGranularity) + 1) * T::timerGranularity;

   if (expectedTimeout < now)
   {
      return 0;
   }

   return expectedTimeout - now;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_TEST_CALLBACK_TIMER_QUEUE_TEST_BASE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: TCallbackTimerQueueTestBase.h
///////////////////////////////////////////////////////////////////////////////
