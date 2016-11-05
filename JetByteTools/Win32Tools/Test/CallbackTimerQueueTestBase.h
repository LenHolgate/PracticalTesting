#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

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

#include "..\Mock\LoggingCallbackTimer.h"
#include "..\Mock\MockTimerQueueMonitor.h"
#include "..\Mock\MockTickCountProvider.h"
#include "..\Mock\MockTickCount64Provider.h"

#include "JetByteTools\Win32Tools\DebugTrace.h"

#include "JetByteTools\TestTools\RunTest.h"

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
// TCallbackTimerQueueTestBase
///////////////////////////////////////////////////////////////////////////////

/// A test for CCallbackTimerQueue
/// \test
/// \ingroup Win32ToolsTests

template <class TimerQueue, class QueueTraits, class TickProvider>
class TCallbackTimerQueueTestBase
{
   public :

      typedef TCallbackTimerQueueTestBase<TimerQueue, QueueTraits, TickProvider> Base;

      static void TestConstructWithMonitoring();
      static void TestConstructWithoutMonitoring();
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
      static void TestEndTimeoutHandlingWithInvalidHandleFails();
      static void TestHandleTimeoutWithInvalidHandleFails();
      static void TestMultipleCallsToHandleTimeoutFail();
      static void TestBeginTimeoutHandlingSetTimer();
      static void TestBeginTimeoutHandlingCancelTimer();
      static void TestBeginTimeoutHandlingDestroyTimer();
      static void TestHandleTimeoutSetTimer();
      static void TestHandleTimeoutCancelTimer();
      static void TestHandleTimeoutDestroyTimer();
      static void TestMultipleTimers();
      static void TestCancelExpiredTimer();
      static void TestGetMaxTimeout();
      static void TestMaxTimeout();
      static void TestResetTimer();
      static void TestOneShotTimer();
      static void TestActiveTimersAtDestructionTime();
      static void TestMonitoring();
      static void TestCancelOneOfManyTimersAndThenHandleTimeouts();
      static void TestCancelOneOfManyTimersAndThenBeginTimeoutHandling();

      static void PerfTestCreateTimer();
      static void PerfTestSetTimer();
      static void PerfTestSetDifferentTimers();
      static void PerfTestSetDifferentTimersSameTimes();
      static void PerfTestHandleTimeouts();
      static void PerfTestBeginTimeoutHandling();

   protected :

      static void TestAll(
         JetByteTools::Test::CTestMonitor &monitor,
         const JetByteTools::Win32::_tstring &className);

      static IQueueTimers::Handle CreateAndSetTimer(
         JetByteTools::Test::CTestLog &tickProvider,
         IQueueTimers &timerQueue, 
         IQueueTimers::Timer &timer, 
         const Milliseconds timeout,
         const IQueueTimers::UserData userData)
      {
         const _tstring functionName = _T("CreateAndSetTimer");

         IQueueTimers::Handle handle = timerQueue.CreateTimer();

         THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

         THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, userData));

         CheckTickProviderSetTimerResults(tickProvider);

         return handle;
      }

   private :

      static void CheckConstructionResults(
         JetByteTools::Test::CTestLog &tickProvider,
         const Milliseconds expectedTickCount = 0)
      {
         CheckTickProviderResults(QueueTraits::creationQueriesTicks, tickProvider, expectedTickCount);
      }

      static void CheckTickProviderResults(
         const bool callQueriesTickCount,
         JetByteTools::Test::CTestLog &tickProvider,
         const Milliseconds expectedTickCount)
      {
         if (callQueriesTickCount)
         {
            tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTickCount) + _T("|"));
         }
         else
         {
            tickProvider.CheckNoResults();
         }
      }

      static void CheckConstructionResults(
         JetByteTools::Test::CTestLog &tickProvider,
         JetByteTools::Win32::Mock::CMockTimerQueueMonitor &monitor,
         const Milliseconds expectedTickCount = 0)
      {
         CheckTickProviderResults(QueueTraits::creationQueriesTicks, tickProvider, expectedTickCount);

         if (QueueTraits::creationQueriesTicks)
         {
            monitor.CheckResult(_T("TODO"));
         }
         else
         {
            monitor.CheckNoResults();
         }
      }

      static void CheckTickProviderSetTimerResults(
         JetByteTools::Test::CTestLog &tickProvider,
         const Milliseconds expectedTickCount = 0)
      {
         CheckTickProviderResults(QueueTraits::setQueriesTicks, tickProvider, expectedTickCount);
      }

      static void CheckTickProviderHandleTimeoutsResults(
         const size_t numDifferentTimes,
         JetByteTools::Test::CTestLog &tickProvider,
         const Milliseconds expectedTickCount = 0,
         const bool moreTimersSet = true)
      {
         if (QueueTraits::handleTimeoutQueriesTicksPerTimer)
         {
            if (numDifferentTimes)
            {
               const _tstring expectedTickCountAsString = ToString(expectedTickCount);

               _tstring result;

               const size_t numCalls = numDifferentTimes + (moreTimersSet ? 1 : 0);

               for (size_t i = 0; i < numCalls; ++i)
               {
                  result += _T("|GetTickCount: ") + expectedTickCountAsString;
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
            tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTickCount) + _T("|"));
         }
      }

      static Milliseconds CalculateExpectedTimeout(
         const Milliseconds timeout,
         const Milliseconds now = 0,
         const Milliseconds timerSetTime = 0)
      {
         if (QueueTraits::timerGranularity == 1)
         {
            return timeout - (now - timerSetTime);
         }

         const Milliseconds expectedTimeout = ((timeout / QueueTraits::timerGranularity) + 1) * QueueTraits::timerGranularity;

         const Milliseconds expectedAbsoluteTimeout = timerSetTime + expectedTimeout;

         if (expectedAbsoluteTimeout < now)
         {
            return 0;
         }

         return expectedAbsoluteTimeout - now;
      }
};

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestAll(
   JetByteTools::Test::CTestMonitor &monitor,
   const JetByteTools::Win32::_tstring &className)
{
   if (T::monitoringEnabled)
   {
      RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestConstructWithMonitoring);
   }

   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestConstructWithoutMonitoring);

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
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestEndTimeoutHandlingWithInvalidHandleFails);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestHandleTimeoutWithInvalidHandleFails);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestMultipleCallsToHandleTimeoutFail);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestBeginTimeoutHandlingSetTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestBeginTimeoutHandlingCancelTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestBeginTimeoutHandlingDestroyTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestHandleTimeoutSetTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestHandleTimeoutCancelTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestHandleTimeoutDestroyTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestCancelExpiredTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestResetTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestGetMaxTimeout);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestMaxTimeout);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestMultipleTimers);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestOneShotTimer);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestActiveTimersAtDestructionTime);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestMonitoring);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestCancelOneOfManyTimersAndThenHandleTimeouts);
   RUN_TEMPLATE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, TestCancelOneOfManyTimersAndThenBeginTimeoutHandling);


   const Milliseconds timeout = INFINITE;    // Don't time out the perf tests

   RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, PerfTestCreateTimer, timeout);
   RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, PerfTestSetTimer, timeout);
   RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, PerfTestSetDifferentTimers, timeout);
   RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, PerfTestSetDifferentTimersSameTimes, timeout);
   RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, PerfTestHandleTimeouts, timeout);
   RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(monitor, TCallbackTimerQueueTestBase, Q, T, P, className, PerfTestBeginTimeoutHandling, timeout);
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestConstructWithMonitoring()
{
   {
      Q timerQueue;
   }

   P tickProvider;

   {
      Q timerQueue(tickProvider);
   }

   CheckConstructionResults(tickProvider);

   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   {
      Q timerQueue(monitor);
   }

#if (JETBYTE_PERF_TIMER_QUEUE_MONITORING_DISABLED == 0)

   monitor.CheckNoResults();

#endif

   {
      Q timerQueue(monitor, tickProvider);
   }

   CheckConstructionResults(tickProvider);

#if (JETBYTE_PERF_TIMER_QUEUE_MONITORING_DISABLED == 0)

   monitor.CheckNoResults();

#endif
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestConstructWithoutMonitoring()
{
   {
      Q timerQueue;
   }

   P tickProvider;

   {
      Q timerQueue(tickProvider);
   }

   CheckConstructionResults(tickProvider);

   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   {
      Q timerQueue(monitor);
   }

   {
      Q timerQueue(monitor, tickProvider);
   }

   CheckConstructionResults(tickProvider);
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestCreateTimer()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestDestroyTimer()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      IQueueTimers::Handle handleCopy = handle;

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue == handle);

      tickProvider.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, handleCopy);

      tickProvider.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, IQueueTimers::InvalidHandleValue);

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestGetNextTimeoutNoTimersSet()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

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
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      timerQueue.HandleTimeouts();

      CheckTickProviderHandleTimeoutsResults(0, tickProvider);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestHandleTimeoutsNoTimersSet()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      const Milliseconds now = 100;

      tickProvider.SetTickCount(now);

      timerQueue.HandleTimeouts();

      CheckTickProviderHandleTimeoutsResults(0, tickProvider, now);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestSetTimer()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      const IQueueTimers::UserData userData = 1;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 1000, userData));

      CheckTickProviderSetTimerResults(tickProvider);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestGetNextTimeoutWithTimerSet()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, /*timerGranularity, */tickProvider);

      CheckConstructionResults(tickProvider);

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      const IQueueTimers::UserData userData = 1;

      const Milliseconds timeout = 1000;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, userData));

      timer.CheckNoResults();

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(1);

      THROW_ON_FAILURE_EX(expectedTimeout - 1 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 1|"));

      tickProvider.SetTickCount(20);

      THROW_ON_FAILURE_EX(expectedTimeout - 20 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 20|"));

      tickProvider.SetTickCount(2000);

      THROW_ON_FAILURE_EX(2000 > expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 2000|"));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestSetTimerAlreadySet()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      const IQueueTimers::UserData userData = 1;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 1000, userData));

      timer.CheckNoResults();

      CheckTickProviderSetTimerResults(tickProvider);

      THROW_ON_FAILURE_EX(true == timerQueue.SetTimer(handle, timer, 1200, userData));

      timer.CheckNoResults();

      CheckTickProviderSetTimerResults(tickProvider);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestCancelTimerNotSet()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

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
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

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

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestCancelTimer()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      const IQueueTimers::UserData userData = 1;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 1000, userData));

      timer.CheckNoResults();

      CheckTickProviderSetTimerResults(tickProvider);

      THROW_ON_FAILURE_EX(true == timerQueue.CancelTimer(handle));

      timer.CheckNoResults();

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestTimer()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      const _tstring expectedTimeoutTickResults = _T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|");

      tickProvider.CheckResult(expectedTimeoutTickResults);

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(expectedTimeoutTickResults);

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
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      const Milliseconds timeout = 1000;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue == h);

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      const _tstring expectedTimeoutTickResults = _T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|");

      tickProvider.CheckResult(expectedTimeoutTickResults);

      h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(expectedTimeoutTickResults);

      timer.CheckNoResults();

      timerQueue.HandleTimeout(h);

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      timerQueue.EndTimeoutHandling(h);

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

   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      tickProvider.SetTickCount(100);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      timer.CheckNoResults();

      timerQueue.EndTimeoutHandling(h);

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMultipleCallsToBeginTimeoutHandlingWithoutEndTimeoutHandlingFail()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      tickProvider.SetTickCount(100);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      timer.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX(timerQueue.BeginTimeoutHandling);

      tickProvider.CheckNoResults();

      timer.CheckNoResults();

      timerQueue.EndTimeoutHandling(h);

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMultipleCallsToEndTimeoutHandlingFail()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(timeout) + _T("|"));

      timer.CheckNoResults();

      timerQueue.EndTimeoutHandling(h);

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.EndTimeoutHandling, h);

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestEndTimeoutHandlingWithInvalidHandleFails()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|"));

      timer.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.EndTimeoutHandling, IManageTimerQueue::InvalidTimeoutHandleValue);

      timerQueue.EndTimeoutHandling(h);

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestHandleTimeoutWithInvalidHandleFails()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|"));

      timer.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.HandleTimeout, IManageTimerQueue::InvalidTimeoutHandleValue);

      timerQueue.HandleTimeout(h);

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();
      
      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.HandleTimeout, h);

      timerQueue.EndTimeoutHandling(h);

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMultipleCallsToHandleTimeoutFail()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout(h);

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.HandleTimeout, h);

      timerQueue.EndTimeoutHandling(h);

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestBeginTimeoutHandlingSetTimer()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|"));

      timer.CheckNoResults();

      // now call set timer to set a new timer whilst the current time is going off...
      // note that this should return false (not currently set) and the new timer and 
      // user data should not affect the timer that is in the process of going off.

      CLoggingCallbackTimer timer2;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer2, 100, 2));

      CheckTickProviderSetTimerResults(tickProvider, 100);

      timerQueue.HandleTimeout(h);

      timer.CheckResult(_T("|OnTimer: 1|"));
      timer2.CheckNoResults();

      tickProvider.CheckNoResults();
      
      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      timerQueue.EndTimeoutHandling(h);

      timer.CheckNoResults();
      timer2.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      tickProvider.SetTickCount(200);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 200|"));

      h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: 200|"));

      timer.CheckNoResults();
      timer2.CheckNoResults();

      timerQueue.HandleTimeout(h);

      timer.CheckNoResults();
      timer2.CheckResult(_T("|OnTimer: 2|"));

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      timerQueue.EndTimeoutHandling(h);

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
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|"));

      timer.CheckNoResults();

      // now call cancel timer to cancel the timer that is going off...
      // Note that this should return false (not currently set) and should not 
      // affect the timer that is in the process of going off.

      THROW_ON_FAILURE_EX(false == timerQueue.CancelTimer(handle));

      tickProvider.CheckNoResults();

      timerQueue.HandleTimeout(h);

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      timerQueue.EndTimeoutHandling(h);

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestBeginTimeoutHandlingDestroyTimer()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|"));

      timer.CheckNoResults();

      // now call destroy timer to destroy the timer that is going off...
      // Note that this should return false (not currently set) and should NOT 
      // affect the timer that is in the process of going off, but SHOULD result
      // in all resources being cleaned up when the time has gone off.

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, handle);

      tickProvider.CheckNoResults();

      timerQueue.HandleTimeout(h);

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();
      
      timerQueue.EndTimeoutHandling(h);

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, handle);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestHandleTimeoutSetTimer()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout(h);

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      // now call set timer to set a new timer whilst the current time is going off...
      // note that this should return false (not currently set) and the new timer and 
      // user data should not affect the timer that is in the process of going off.

      CLoggingCallbackTimer timer2;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer2, 100, 2));

      CheckTickProviderSetTimerResults(tickProvider, 100);

      timer2.CheckNoResults();

      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      timerQueue.EndTimeoutHandling(h);

      timer.CheckNoResults();
      timer2.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      tickProvider.SetTickCount(200);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 200|"));

      h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: 200|"));

      timer.CheckNoResults();
      timer2.CheckNoResults();

      timerQueue.HandleTimeout(h);

      timer.CheckNoResults();
      timer2.CheckResult(_T("|OnTimer: 2|"));

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      timerQueue.EndTimeoutHandling(h);

      timer.CheckNoResults();
      timer2.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestHandleTimeoutCancelTimer()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout(h);

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      // now call cancel timer to cancel the timer that is going off...
      // Note that this should return false (not currently set) and should not 
      // affect the timer that is in the process of going off.

      THROW_ON_FAILURE_EX(false == timerQueue.CancelTimer(handle));

      tickProvider.CheckNoResults();
      
      timerQueue.EndTimeoutHandling(h);

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestHandleTimeoutDestroyTimer()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout(h);

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      // now call destroy timer to destroy the timer that is going off...
      // Note that this should return false (not currently set) and should NOT 
      // affect the timer that is in the process of going off, but SHOULD result
      // in all resources being cleaned up when the time has gone off.

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, handle);

      tickProvider.CheckNoResults();
      
      timerQueue.EndTimeoutHandling(h);

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, handle);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}


template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestCancelExpiredTimer()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      const _tstring expectedTimeoutTickResults = _T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|");

      tickProvider.CheckResult(expectedTimeoutTickResults);

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(expectedTimeoutTickResults);

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
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      THROW_ON_FAILURE_EX(T::maxTimeout == timerQueue.GetMaximumTimeout());

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMaxTimeout()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   const Milliseconds initialTickCount = 1000;

   P tickProvider(initialTickCount);

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider, initialTickCount);

      CLoggingCallbackTimer timer;

      const Milliseconds maxTimeout = timerQueue.GetMaximumTimeout();

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, maxTimeout, 1));

      CheckTickProviderSetTimerResults(tickProvider, initialTickCount);

      const Milliseconds illegalTimeout = maxTimeout + 1;

      THROW_ON_NO_EXCEPTION_EX_4(timerQueue.SetTimer, handle, timer, illegalTimeout, 1);

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(true == timerQueue.CancelTimer(handle));

      THROW_ON_NO_EXCEPTION_EX_4(timerQueue.SetTimer, handle, timer, illegalTimeout, 1);

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));

      timer.CheckNoResults();
      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMultipleTimers()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer1;
      CLoggingCallbackTimer timer2;
      CLoggingCallbackTimer timer3;
      CLoggingCallbackTimer timer4;
      CLoggingCallbackTimer timer5;
      CLoggingCallbackTimer timer6;

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

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timer1.CheckNoResults();
      timer2.CheckNoResults();
      timer3.CheckNoResults();
      timer4.CheckNoResults();
      timer5.CheckNoResults();
      timer6.CheckNoResults();

      Milliseconds now = expectedTimeout;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      _tstring currentTicksAsString = _T("|GetTickCount: ") + ToString(now) + _T("|");

      tickProvider.CheckResult(currentTicksAsString);

      timerQueue.HandleTimeouts();

      CheckTickProviderHandleTimeoutsResults(1, tickProvider, now);

      timer1.CheckNoResults();
      timer2.CheckResult(_T("|OnTimer: 2|"));
      timer3.CheckNoResults();
      timer4.CheckNoResults();
      timer5.CheckNoResults();
      timer6.CheckNoResults();

      expectedTimeout = CalculateExpectedTimeout(timeout2, now);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(currentTicksAsString);

      now += expectedTimeout + 5;

      tickProvider.SetTickCount(now);

      currentTicksAsString = _T("|GetTickCount: ") + ToString(now) + _T("|");

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(currentTicksAsString);

      timerQueue.HandleTimeouts();

      CheckTickProviderHandleTimeoutsResults(1, tickProvider, now);

      timer1.CheckNoResults();
      timer2.CheckNoResults();
      timer3.CheckNoResults();
      timer4.CheckResult(_T("|OnTimer: 4|"));
      timer5.CheckResult(_T("|OnTimer: 5|"));
      timer6.CheckNoResults();

      expectedTimeout = CalculateExpectedTimeout(timeout3, now);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(currentTicksAsString);

      expectedTimeout = CalculateExpectedTimeout(timeout4, now);

      now += expectedTimeout + 1;

      tickProvider.SetTickCount(now);

      currentTicksAsString = _T("|GetTickCount: ") + ToString(now) + _T("|");

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(currentTicksAsString);

      timerQueue.HandleTimeouts();

      CheckTickProviderHandleTimeoutsResults(2, tickProvider, now);

      timer1.CheckNoResults();
      timer2.CheckNoResults();
      timer3.CheckResult(_T("|OnTimer: 3|"));
      timer4.CheckNoResults();
      timer5.CheckNoResults();
      timer6.CheckResult(_T("|OnTimer: 6|"));

      expectedTimeout = CalculateExpectedTimeout(timeout5, now);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(currentTicksAsString);

      now += expectedTimeout;

      tickProvider.SetTickCount(now);

      currentTicksAsString = _T("|GetTickCount: ") + ToString(now) + _T("|");

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(currentTicksAsString);

      timerQueue.HandleTimeouts();

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
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      const Milliseconds timeout1 = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout1, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout1 = CalculateExpectedTimeout(timeout1);

      THROW_ON_FAILURE_EX(expectedTimeout1 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      const Milliseconds timeout2 = 90;

      THROW_ON_FAILURE_EX(true == timerQueue.SetTimer(handle, timer, timeout2, 2));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout2 = CalculateExpectedTimeout(timeout2);

      THROW_ON_FAILURE_EX(expectedTimeout2 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      Milliseconds now = expectedTimeout2;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      _tstring nowTickResults = _T("|GetTickCount: ") + ToString(now) + _T("|");

      tickProvider.CheckResult(nowTickResults);

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(nowTickResults);

      timer.CheckResult(_T("|OnTimer: 2|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      const Milliseconds timeout3 = 110;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout3, 3));

      CheckTickProviderSetTimerResults(tickProvider, expectedTimeout2);

      const Milliseconds expectedTimeout3 = CalculateExpectedTimeout(timeout3);

      THROW_ON_FAILURE_EX(expectedTimeout3 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(nowTickResults);

      now += expectedTimeout3;

      tickProvider.SetTickCount(now);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      nowTickResults = _T("|GetTickCount: ") + ToString(now) + _T("|");

      tickProvider.CheckResult(nowTickResults);

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(nowTickResults);

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
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      timerQueue.SetTimer(timer, 100, 1);

      CheckTickProviderSetTimerResults(tickProvider);

      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));
   
      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(100);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      timer.CheckResult(_T("|OnTimer: 1|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestActiveTimersAtDestructionTime()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle1 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle1);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle1, timer, 100, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      IQueueTimers::Handle handle2 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle2);

      timerQueue.SetTimer(timer, 250, 1);

      timerQueue.SetTimer(timer, 2500, 1);

      timerQueue.HandleTimeouts();

      timer.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

#if (JETBYTE_PERF_TIMER_QUEUE_MONITORING_DISABLED == 0)
template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMonitoring()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider, monitor);

      CLoggingCallbackTimer timer;

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

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      monitor.CheckResult(_T("|OnTimerSet|"));

      tickProvider.SetTickCount(100);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      timerQueue.HandleTimeouts();

      timer.CheckResult(_T("|OnTimer: 1|"));

      monitor.CheckResult(_T("|OnTimer|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));

      monitor.CheckResult(_T("|OnTimerDestroyed|OnTimerDeleted|"));

      handle = timerQueue.CreateTimer();

      monitor.CheckResult(_T("|OnTimerCreated|"));

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      monitor.CheckResult(_T("|OnTimerSet|"));

      THROW_ON_FAILURE_EX(true == timerQueue.DestroyTimer(handle));

      monitor.CheckResult(_T("|OnTimerDestroyed: Was Pending|OnTimerDeleted|"));

      timerQueue.SetTimer(timer, 100, 2);

      monitor.CheckResult(_T("|OnTimerCreated|OnOneOffTimerSet|"));

      tickProvider.SetTickCount(200);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      timerQueue.HandleTimeouts();

      timer.CheckResult(_T("|OnTimer: 2|"));

      monitor.CheckResult(_T("|OnTimer|OnTimerDeleted|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      timerQueue.SetTimer(timer, 100, 3);

      monitor.CheckResult(_T("|OnTimerCreated|OnOneOffTimerSet|"));
   }

   monitor.CheckResult(_T("|OnTimerDeleted|"));       // The pending 'one off' timer is deleted in the destructor

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());
}
#else
template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestMonitoring()
{
   SKIP_TEST_EX(_T("Monitoring not enabled"));
}
#endif

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::TestCancelOneOfManyTimersAndThenHandleTimeouts()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle1 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle1);

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle1, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      IQueueTimers::Handle handle2 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle2);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle2, timer, timeout, 2));

      CheckTickProviderSetTimerResults(tickProvider);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      IQueueTimers::Handle handle3 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle3);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle3, timer, timeout, 3));

      CheckTickProviderSetTimerResults(tickProvider);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_FAILURE_EX(true == timerQueue.CancelTimer(handle2));

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      const _tstring expectedTimeoutTickResults = _T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|");

      tickProvider.CheckResult(expectedTimeoutTickResults);

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(expectedTimeoutTickResults);

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
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   P tickProvider;

   {
      Q timerQueue(monitor, tickProvider);

      CheckConstructionResults(tickProvider);

      CLoggingCallbackTimer timer;

      IQueueTimers::Handle handle1 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle1);

      const Milliseconds timeout = 100;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle1, timer, timeout, 1));

      CheckTickProviderSetTimerResults(tickProvider);

      const Milliseconds expectedTimeout = CalculateExpectedTimeout(timeout);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      IQueueTimers::Handle handle2 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle2);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle2, timer, timeout, 2));

      CheckTickProviderSetTimerResults(tickProvider);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      IQueueTimers::Handle handle3 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle3);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle3, timer, timeout, 3));

      CheckTickProviderSetTimerResults(tickProvider);

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_FAILURE_EX(true == timerQueue.CancelTimer(handle2));

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(expectedTimeout == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(expectedTimeout);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      const _tstring expectedTimeoutTickResults = _T("|GetTickCount: ") + ToString(expectedTimeout) + _T("|");

      tickProvider.CheckResult(expectedTimeoutTickResults);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      tickProvider.CheckResult(expectedTimeoutTickResults);

      timerQueue.HandleTimeout(h);

      timer.CheckResult(_T("|OnTimer: 1|OnTimer: 3|"));

      timerQueue.EndTimeoutHandling(h);

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle1));

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle2));

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle3));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

// Performance tests

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::PerfTestCreateTimer()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

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
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   CLoggingCallbackTimer timer;

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
         timerQueue.SetTimer(handle, timer, i % 4000, 0);
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
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   CLoggingCallbackTimer timer;

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
            timerQueue.SetTimer(handles[i % s_numHandles], timer, i % 4000, 0);
         }

         const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

         totalTime += timeTaken;

#if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
         OutputEx(T::shortName + ToString(timeTaken) + _T("ms - SetTimer (Different timers) (100,000 calls)"));
#endif
      }

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         timerQueue.DestroyTimer(handles[i]);
      }
   }

   OutputEx(T::shortName + ToString(totalTime/numRuns) + _T("ms - SetTimer (Different timers) (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::PerfTestSetDifferentTimersSameTimes()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   CLoggingCallbackTimer timer;

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
               timerQueue.SetTimer(handles[x], timer, ((1 + i) * 100) % 4000, 0);
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
         timerQueue.DestroyTimer(handles[i]);
      }
   }

   OutputEx(T::shortName + ToString(totalTime/numRuns) + _T("ms - SetTimer (Different timers, same times) (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::PerfTestHandleTimeouts()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   P tickProvider;

   tickProvider.logMessages = false;

   Milliseconds now = 0;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      Q timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      timer.logMessage = false;

      typedef std::vector<IQueueTimers::Handle> Handles;

      Handles handles;

      static const size_t s_numHandles = 100000;

      handles.resize(s_numHandles);

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         handles[i] = timerQueue.CreateTimer();

         timerQueue.SetTimer(handles[i], timer, i % 4000, 0);
      }

      {
         CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

         Milliseconds nextTimeout = timerQueue.GetNextTimeout();

         while (nextTimeout != INFINITE)
         {
            now += nextTimeout;

            tickProvider.SetTickCount(now);

            timerQueue.HandleTimeouts();

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
         timerQueue.DestroyTimer(handles[i]);
      }
   }

   OutputEx(T::shortName + ToString(totalTime/numRuns) + _T("ms - HandleTimeouts (100,000 timers - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

template <class Q, class T, class P>
void TCallbackTimerQueueTestBase<Q, T, P>::PerfTestBeginTimeoutHandling()
{
   JetByteTools::Win32::Mock::CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   P tickProvider;

   tickProvider.logMessages = false;

   Milliseconds now = 0;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      Q timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      timer.logMessage = false;

      typedef std::vector<IQueueTimers::Handle> Handles;

      Handles handles;

      static const size_t s_numHandles = 100000;

      handles.resize(s_numHandles);

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         handles[i] = timerQueue.CreateTimer();

         timerQueue.SetTimer(handles[i], timer, i % 4000, 0);
      }

      {
         CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

         Milliseconds nextTimeout = timerQueue.GetNextTimeout();

         while (nextTimeout != INFINITE)
         {
            now += nextTimeout;

            tickProvider.SetTickCount(now);

            IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

            timerQueue.HandleTimeout(h);

            timerQueue.EndTimeoutHandling(h);

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
         timerQueue.DestroyTimer(handles[i]);
      }
   }

   OutputEx(T::shortName + ToString(totalTime/numRuns) + _T("ms - BeginTimeoutHandling (100,000 timers - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
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
