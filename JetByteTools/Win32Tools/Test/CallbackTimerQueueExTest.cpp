///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueExTest.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2008 JetByte Limited.
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

#include "CallbackTimerQueueExTest.h"

#include "..\Mock\MockTickCount64Provider.h"
#include "..\Mock\LoggingCallbackTimer.h"
#include "..\Mock\MockTimerQueueMonitor.h"

#include "JetByteTools\Win32Tools\Utils.h"
#include "JetByteTools\Win32Tools\DebugTrace.h"

#include "JetByteTools\TestTools\TestException.h"
#include "JetByteTools\TestTools\RunTest.h"

#pragma hdrstop

#include "..\CallbackTimerQueueEx.h"

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
using JetByteTools::Test::CTestMonitor;

using JetByteTools::Win32::Mock::CMockTickCount64Provider;
using JetByteTools::Win32::Mock::CLoggingCallbackTimer;
using JetByteTools::Win32::Mock::CMockTimerQueueMonitor;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static CCallbackTimerQueueEx::Handle CreateAndSetTimer(
   CCallbackTimerQueueEx &timerQueue, 
   CCallbackTimerQueueEx::Timer &timer, 
   const Milliseconds timeout,
   const CCallbackTimerQueueEx::UserData userData);

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueExTest
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerQueueExTest::TestAll(
   CTestMonitor &monitor)
{
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestConstruct);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestCreateTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestDestroyTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestBeginTimeoutHandlingHandleTimeoutEndTimeoutHandling);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestBeginTimeoutHandlingEndTimeoutHandling);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestMultipleCallsToBeginTimeoutHandlingWithoutEndTimeoutHandlingFail);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestMultipleCallsToEndTimeoutHandlingFail);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestEndTimeoutHandlingWithInvalidHandleFails);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestHandleTimeoutWithInvalidHandleFails);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestMultipleCallsToHandleTimeoutFail);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestBeginTimeoutHandlingSetTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestBeginTimeoutHandlingCancelTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestBeginTimeoutHandlingDestroyTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestHandleTimeoutSetTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestHandleTimeoutCancelTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestHandleTimeoutDestroyTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestCancelTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestCancelExpiredTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestResetTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestGetMaxTimeout);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestSetTimerPastTickCount64CountWrap);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestMaxTimeout);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestMultipleTimers);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestOneShotTimer);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestActiveTimersAtDestructionTime);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestMonitoring);

   const Milliseconds timeout = INFINITE;    // Don't time out the perf tests

   RUN_TIMED_PERFORMANCE_TEST_EX(monitor, CCallbackTimerQueueExTest, PerfTestCreateTimer, timeout);
   RUN_TIMED_PERFORMANCE_TEST_EX(monitor, CCallbackTimerQueueExTest, PerfTestSetTimer, timeout);
   RUN_TIMED_PERFORMANCE_TEST_EX(monitor, CCallbackTimerQueueExTest, PerfTestSetDifferentTimers, timeout);
   RUN_TIMED_PERFORMANCE_TEST_EX(monitor, CCallbackTimerQueueExTest, PerfTestSetDifferentTimersSameTimes, timeout);
   RUN_TIMED_PERFORMANCE_TEST_EX(monitor, CCallbackTimerQueueExTest, PerfTestHandleTimeouts, timeout);
   RUN_TIMED_PERFORMANCE_TEST_EX(monitor, CCallbackTimerQueueExTest, PerfTestBeginTimeoutHandling, timeout);
}

void CCallbackTimerQueueExTest::TestConstruct()
{
   {
      CCallbackTimerQueueEx timerQueue;
   }

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(tickProvider);
   }

   tickProvider.CheckNoResults();

   CMockTimerQueueMonitor monitor;

   {
      CCallbackTimerQueueEx timerQueue(monitor);
   }

#if (JETBYTE_PERF_TIMER_QUEUE_MONITORING_DISABLED == 0)

   monitor.CheckNoResults();

#endif

   {
      CMockTickCount64Provider tickProvider;

      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);
   }

   tickProvider.CheckNoResults();

#if (JETBYTE_PERF_TIMER_QUEUE_MONITORING_DISABLED == 0)

   monitor.CheckNoResults();

#endif
}

void CCallbackTimerQueueExTest::TestCreateTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      tickProvider.CheckNoResults();

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::TestDestroyTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      tickProvider.CheckNoResults();

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);

      CCallbackTimerQueueEx::Handle handleCopy = handle;

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue == handle);

      tickProvider.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, handleCopy);

      tickProvider.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.DestroyTimer, CCallbackTimerQueueEx::InvalidHandleValue);

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::TestTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

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

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}
void CCallbackTimerQueueExTest::TestBeginTimeoutHandlingHandleTimeoutEndTimeoutHandling()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue == h);

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(100);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

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

void CCallbackTimerQueueExTest::TestBeginTimeoutHandlingEndTimeoutHandling()
{
   // You do NOT need to call HandleTimeout() on the timeoutHandle that you 
   // get from BeginTimeoutHandling()

   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

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

void CCallbackTimerQueueExTest::TestMultipleCallsToBeginTimeoutHandlingWithoutEndTimeoutHandlingFail()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

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

void CCallbackTimerQueueExTest::TestMultipleCallsToEndTimeoutHandlingFail()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(100);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      timer.CheckNoResults();

      timerQueue.EndTimeoutHandling(h);

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.EndTimeoutHandling, h);

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}
void CCallbackTimerQueueExTest::TestEndTimeoutHandlingWithInvalidHandleFails()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(100);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      timer.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.EndTimeoutHandling, IManageTimerQueue::InvalidTimeoutHandleValue);

      timerQueue.EndTimeoutHandling(h);

      timer.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::TestHandleTimeoutWithInvalidHandleFails()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(100);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

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

void CCallbackTimerQueueExTest::TestMultipleCallsToHandleTimeoutFail()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(100);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

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

void CCallbackTimerQueueExTest::TestBeginTimeoutHandlingSetTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(100);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);
   
      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      timer.CheckNoResults();

      // now call set timer to set a new timer whilst the current time is going off...
      // note that this should return false (not currently set) and the new timer and 
      // user data should not affect the timer that is in the process of going off.

      CLoggingCallbackTimer timer2;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer2, 100, 2));

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

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

void CCallbackTimerQueueExTest::TestBeginTimeoutHandlingCancelTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(100);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

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

void CCallbackTimerQueueExTest::TestBeginTimeoutHandlingDestroyTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(100);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);
   
      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

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

void CCallbackTimerQueueExTest::TestHandleTimeoutSetTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(100);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);
   
      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      timer.CheckNoResults();

      timerQueue.HandleTimeout(h);

      timer.CheckResult(_T("|OnTimer: 1|"));

      tickProvider.CheckNoResults();

      // now call set timer to set a new timer whilst the current time is going off...
      // note that this should return false (not currently set) and the new timer and 
      // user data should not affect the timer that is in the process of going off.

      CLoggingCallbackTimer timer2;

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer2, 100, 2));

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

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

void CCallbackTimerQueueExTest::TestHandleTimeoutCancelTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(100);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);
   
      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

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

void CCallbackTimerQueueExTest::TestHandleTimeoutDestroyTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(100);

      IManageTimerQueue::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

      THROW_ON_FAILURE_EX(IManageTimerQueue::InvalidTimeoutHandleValue != h);
   
      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

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

void CCallbackTimerQueueExTest::TestCancelTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));
   
      THROW_ON_FAILURE_EX(true == timerQueue.CancelTimer(handle));

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.CancelTimer(handle));

      CCallbackTimerQueueEx::Handle invalid = 0;

      THROW_ON_NO_EXCEPTION_EX_1(timerQueue.CancelTimer, invalid);

      tickProvider.SetTickCount(100);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      timerQueue.HandleTimeouts();

      tickProvider.CheckNoResults();

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::TestCancelExpiredTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

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

      THROW_ON_FAILURE_EX(false == timerQueue.CancelTimer(handle));

      timer.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::TestGetMaxTimeout()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      tickProvider.CheckNoResults();
   
      CLoggingCallbackTimer timer;

      THROW_ON_FAILURE_EX(4294967294 == timerQueue.GetMaximumTimeout());

      tickProvider.CheckNoResults();

      // GetMaximumTimeout() doesn't reduce the value that it returns as the 64bit 
      // rollover approaches; which means you WILL get an exception if you try and
      // set a timer that is less than the value returned by GetMaximumTimeout() 
      // but which crosses the rollover point. I'll fix this if I get enough 
      // repeatable bug reports.

      const ULONGLONG rollOver = 0;
      const ULONGLONG beforeRollOver = rollOver - 1000;

      tickProvider.SetTickCount(beforeRollOver);

      THROW_ON_FAILURE_EX(4294967294 == timerQueue.GetMaximumTimeout());

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::TestSetTimerPastTickCount64CountWrap()
{
   CMockTimerQueueMonitor monitor;

   const ULONGLONG rollOver = 0;
   const ULONGLONG beforeRollOver = rollOver - 1000;

   CMockTickCount64Provider tickProvider(beforeRollOver);

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      const _tstring resultBeforeRollOver = _T("|GetTickCount: ") + ToString(beforeRollOver) + _T("|");

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);

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

      timerQueue.HandleTimeouts();

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

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

      timer.CheckResult(_T("|OnTimer: 1|"));

      // Check that there are no more timers to go off.

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::TestMaxTimeout()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider(1000);

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      tickProvider.CheckNoResults();
   
      CLoggingCallbackTimer timer;

      const Milliseconds maxTimeout = timerQueue.GetMaximumTimeout();

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, maxTimeout, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

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

void CCallbackTimerQueueExTest::TestMultipleTimers()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      tickProvider.CheckNoResults();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer1;
      CLoggingCallbackTimer timer2;
      CLoggingCallbackTimer timer3;
      CLoggingCallbackTimer timer4;
      CLoggingCallbackTimer timer5;
      CLoggingCallbackTimer timer6;

      CCallbackTimerQueueEx::Handle handle1 = CreateAndSetTimer(timerQueue, timer1, 300, 1);
      CCallbackTimerQueueEx::Handle handle2 = CreateAndSetTimer(timerQueue, timer2, 100, 2);
      CCallbackTimerQueueEx::Handle handle3 = CreateAndSetTimer(timerQueue, timer3, 200, 3);
      CCallbackTimerQueueEx::Handle handle4 = CreateAndSetTimer(timerQueue, timer4, 150, 4);
      CCallbackTimerQueueEx::Handle handle5 = CreateAndSetTimer(timerQueue, timer5, 150, 5);
      CCallbackTimerQueueEx::Handle handle6 = CreateAndSetTimer(timerQueue, timer6, 160, 6);

      tickProvider.CheckResult(
         _T("|GetTickCount: 0")
         _T("|GetTickCount: 0")
         _T("|GetTickCount: 0")
         _T("|GetTickCount: 0")
         _T("|GetTickCount: 0")
         _T("|GetTickCount: 0|"));

      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timer1.CheckNoResults();
      timer2.CheckNoResults();
      timer3.CheckNoResults();
      timer4.CheckNoResults();
      timer5.CheckNoResults();
      timer6.CheckNoResults();

      tickProvider.SetTickCount(100);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 100|GetTickCount: 100|"));

      timer1.CheckNoResults();
      timer2.CheckResult(_T("|OnTimer: 2|"));
      timer3.CheckNoResults();
      timer4.CheckNoResults();
      timer5.CheckNoResults();
      timer6.CheckNoResults();

      THROW_ON_FAILURE_EX(50 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 100|"));

      tickProvider.SetTickCount(155);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 155|"));

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 155|GetTickCount: 155|GetTickCount: 155|"));

      timer1.CheckNoResults();
      timer2.CheckNoResults();
      timer3.CheckNoResults();
      timer4.CheckResult(_T("|OnTimer: 4|"));
      timer5.CheckResult(_T("|OnTimer: 5|"));
      timer6.CheckNoResults();

      THROW_ON_FAILURE_EX(5 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 155|"));

      tickProvider.SetTickCount(201);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 201|"));

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 201|GetTickCount: 201|GetTickCount: 201|"));

      timer1.CheckNoResults();
      timer2.CheckNoResults();
      timer3.CheckResult(_T("|OnTimer: 3|"));
      timer4.CheckNoResults();
      timer5.CheckNoResults();
      timer6.CheckResult(_T("|OnTimer: 6|"));

      THROW_ON_FAILURE_EX(99 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 201|"));

      tickProvider.SetTickCount(300);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 300|"));

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 300|"));

      timer1.CheckResult(_T("|OnTimer: 1|"));
      timer2.CheckNoResults();
      timer3.CheckNoResults();
      timer4.CheckNoResults();
      timer5.CheckNoResults();
      timer6.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

   //   timerQueue.SetTimer(timer1, 100, 1);
   //   timerQueue.SetTimer(timer2, 200, 2);

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle1));
      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle2));
      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle3));
      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle4));
      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle5));
      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle6));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::TestResetTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_FAILURE_EX(100 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));
   
      THROW_ON_FAILURE_EX(true == timerQueue.SetTimer(handle, timer, 90, 2));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_FAILURE_EX(90 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      tickProvider.SetTickCount(90);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 90|"));

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 90|"));

      timer.CheckResult(_T("|OnTimer: 2|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 110, 3));

      tickProvider.CheckResult(_T("|GetTickCount: 90|"));

      THROW_ON_FAILURE_EX(110 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 90|"));

      tickProvider.SetTickCount(200);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 200|"));

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 200|"));

      timer.CheckResult(_T("|OnTimer: 3|"));

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::TestOneShotTimer()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      timerQueue.SetTimer(timer, 100, 1);

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

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

void CCallbackTimerQueueExTest::TestActiveTimersAtDestructionTime()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      tickProvider.CheckNoResults();
   
      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle1 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle1);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle1, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      CCallbackTimerQueueEx::Handle handle2 = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle2);

      timerQueue.SetTimer(timer, 250, 1);

      timerQueue.SetTimer(timer, 2500, 1);

      timerQueue.HandleTimeouts();

      timer.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

#if (JETBYTE_PERF_TIMER_QUEUE_MONITORING_DISABLED == 0)
void CCallbackTimerQueueExTest::TestMonitoring()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      monitor.CheckNoResults();

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      monitor.CheckResult(_T("|OnTimerCreated|"));

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);

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

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);

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
void CCallbackTimerQueueExTest::TestMonitoring()
{
   SKIP_TEST_EX(_T("Monitoring not enabled"));
}
#endif

void CCallbackTimerQueueExTest::PerfTestCreateTimer()
{
   CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   CMockTickCount64Provider tickProvider;

   tickProvider.logMessages = false;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

      for (size_t i = 0; i < 100000; ++i)
      {
         CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

         (void)handle;
      }

      const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

      totalTime += timeTaken;

#if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
      OutputEx(_T("QueueEx -  ") + ToString(timeTaken) + _T("ms - CreateTimer (100,000 calls)"));
#endif
   }

   OutputEx(_T("QueueEx - ") + ToString(totalTime/numRuns) + _T("ms - CreateTimer (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::PerfTestSetTimer()
{
   CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   CLoggingCallbackTimer timer;

   CMockTickCount64Provider tickProvider;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

      for (size_t i = 0; i < 100000; ++i)
      {
         timerQueue.SetTimer(handle, timer, i % 4000, 0);
      }

      const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

      totalTime += timeTaken;

#if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
      OutputEx(_T("QueueEx -  ") + ToString(timeTaken) + _T("ms - SetTimer (100,000 calls)"));
#endif
   }

   OutputEx(_T("QueueEx - ") + ToString(totalTime/numRuns) + _T("ms - SetTimer (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::PerfTestSetDifferentTimers()
{
   CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   CLoggingCallbackTimer timer;

   CMockTickCount64Provider tickProvider;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      typedef std::vector<CCallbackTimerQueueEx::Handle> Handles;

      Handles handles;

      static const size_t s_numHandles = 77;

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
         OutputEx(_T("QueueEx -  ") + ToString(timeTaken) + _T("ms - SetTimer (Different timers) (100,000 calls)"));
#endif
      }

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         timerQueue.DestroyTimer(handles[i]);
      }
   }

   OutputEx(_T("QueueEx - ") + ToString(totalTime/numRuns) + _T("ms - SetTimer (Different timers) (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::PerfTestSetDifferentTimersSameTimes()
{
   CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   CLoggingCallbackTimer timer;

   CMockTickCount64Provider tickProvider;

   tickProvider.logMessages = false;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      typedef std::vector<CCallbackTimerQueueEx::Handle> Handles;

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
         OutputEx(_T("QueueEx -  ") + ToString(timeTaken) + _T("ms - SetTimer (Different timers, same times) (100,000 calls)"));
#endif
      }

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         timerQueue.DestroyTimer(handles[i]);
      }
   }

   OutputEx(_T("QueueEx - ") + ToString(totalTime/numRuns) + _T("ms - SetTimer (Different timers, same times) (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::PerfTestHandleTimeouts()
{
   CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   CMockTickCount64Provider tickProvider;

   tickProvider.logMessages = false;

   Milliseconds now = 0;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      timer.logMessage = false;

      typedef std::vector<CCallbackTimerQueueEx::Handle> Handles;

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
         OutputEx(_T("QueueEx -  ") + ToString(timeTaken) + _T("ms - HandleTimeouts (100,000 timers)"));
#endif
      }

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         timerQueue.DestroyTimer(handles[i]);
      }
   }

   OutputEx(_T("QueueEx - ") + ToString(totalTime/numRuns) + _T("ms - HandleTimeouts (100,000 timers - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::PerfTestBeginTimeoutHandling()
{
   CMockTimerQueueMonitor monitor;

   monitor.logMessages = false;

   CMockTickCount64Provider tickProvider;

   tickProvider.logMessages = false;

   Milliseconds now = 0;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      CLoggingCallbackTimer timer;

      timer.logMessage = false;

      typedef std::vector<CCallbackTimerQueueEx::Handle> Handles;

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

            CCallbackTimerQueueEx::TimeoutHandle h = timerQueue.BeginTimeoutHandling();

            timerQueue.HandleTimeout(h);

            timerQueue.EndTimeoutHandling(h);

            nextTimeout = timerQueue.GetNextTimeout();
         }

         THROW_ON_FAILURE_EX(s_numHandles == timer.GetNumTimerEvents());

         const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

         totalTime += timeTaken;

#if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
         OutputEx(_T("QueueEx -  ") + ToString(timeTaken) + _T("ms - BeginTimeoutHandling (100,000 timers)"));
#endif
      }

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         timerQueue.DestroyTimer(handles[i]);
      }
   }

   OutputEx(_T("QueueEx - ") + ToString(totalTime/numRuns) + _T("ms - BeginTimeoutHandling (100,000 timers - average of ") + ToString(numRuns) + _T(" runs)"));

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static CCallbackTimerQueueEx::Handle CreateAndSetTimer(
   CCallbackTimerQueueEx &timerQueue, 
   CCallbackTimerQueueEx::Timer &timer, 
   const Milliseconds timeout,
   const CCallbackTimerQueueEx::UserData userData)
{
   const _tstring functionName = _T("CreateAndSetTimer");

   CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

   THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);

   THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, timeout, userData));

   return handle;
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

