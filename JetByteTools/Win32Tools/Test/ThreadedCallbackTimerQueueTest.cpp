///////////////////////////////////////////////////////////////////////////////
// File: ThreadedCallbackTimerQueueTest.cpp
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

#include "JetByteTools/Admin/Admin.h"

#include "ThreadedCallbackTimerQueueTest.h"

#include "JetByteTools/Win32Tools/Mock/MockTickCountProvider.h"
#include "JetByteTools/Win32Tools/Mock/MockTickCount64Provider.h"
#include "JetByteTools/Win32Tools/Mock/LoggingCallbackTimer.h"
#include "JetByteTools/Win32Tools/Mock/MockTimerQueue.h"
#include "JetByteTools/Win32Tools/Mock/MockThreadedCallbackTimerQueueMonitor.h"
#include "JetByteTools/Win32Tools/Mock/TestThreadedCallbackTimerQueue.h"

#include "JetByteTools/TestTools/TestException.h"
#include "JetByteTools/TestTools/RunTest.h"

#pragma hdrstop

#include "JetByteTools/Win32Tools/ThreadedCallbackTimerQueue.h"
#include "JetByteTools/Win32Tools/CallbackTimerWheel.h"

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;
using JetByteTools::Test::CTestMonitor;
using JetByteTools::Test::CTestLog;

using JetByteTools::Win32::Mock::CMockTickCountProvider;
using JetByteTools::Win32::Mock::CMockTickCount64Provider;
using JetByteTools::Win32::Mock::CLoggingCallbackTimer;
using JetByteTools::Win32::Mock::CMockTimerQueue;
using JetByteTools::Win32::Mock::CMockThreadedCallbackTimerQueueMonitor;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CThreadedCallbackTimerQueueTest
///////////////////////////////////////////////////////////////////////////////

void CThreadedCallbackTimerQueueTest::TestAll(
   CTestMonitor &monitor)
{
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestConstruct);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestConstructCustomQueue);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestConstructWithTimerWheel);

   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestBeginShutdown);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestWaitForShutdownToComplete);

   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestTimer);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestTimerTimerWheel);

   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestTimerSetTimerInOnTimer);

   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestMultipleTimers);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestMultipleTimersTimerWheel);

   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestMonitorTimer);
}

void CThreadedCallbackTimerQueueTest::TestConstruct()
{
   {
      CThreadedCallbackTimerQueue timerQueue;
   }

   CMockThreadedCallbackTimerQueueMonitor monitor;

   {
      CThreadedCallbackTimerQueue timerQueue(monitor);
   }

   monitor.CheckNoResults();

   CMockTickCount64Provider tickProvider;

   {
      CThreadedCallbackTimerQueue timerQueue(tickProvider);
   }

   tickProvider.CheckNoResults();

   {
      CThreadedCallbackTimerQueue timerQueue(monitor, tickProvider);
   }

   monitor.CheckNoResults();

   tickProvider.CheckNoResults();
}

void CThreadedCallbackTimerQueueTest::TestConstructCustomQueue()
{
   CMockTimerQueue queue;

   {
      CThreadedCallbackTimerQueue timerQueue(queue);

      THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));
   }

   queue.CheckResult(_T("|GetNextTimeout|"));
}

void CThreadedCallbackTimerQueueTest::TestConstructWithTimerWheel()
{
   const Milliseconds maximumTimeout = 4000;

   const CMockTickCountProvider tickProvider;

   CCallbackTimerWheel wheel(maximumTimeout, tickProvider);

   {
      CThreadedCallbackTimerQueue timerQueue(wheel);
   }
}

void CThreadedCallbackTimerQueueTest::TestBeginShutdown()
{
   {
      CThreadedCallbackTimerQueue timerQueue;

      timerQueue.BeginShutdown();

      THROW_ON_FAILURE_EX(true == timerQueue.WaitForShutdownToComplete(SHORT_TIME_NON_ZERO));
   }
}

void CThreadedCallbackTimerQueueTest::TestWaitForShutdownToComplete()
{
   {
      CThreadedCallbackTimerQueue timerQueue;

      THROW_ON_FAILURE_EX(true == timerQueue.WaitForShutdownToComplete(SHORT_TIME_NON_ZERO));

      THROW_ON_FAILURE_EX(true == timerQueue.WaitForShutdownToComplete(0));
   }
}

void CThreadedCallbackTimerQueueTest::TestTimer()
{
   CMockTimerQueue queue;

   // We set waitForOnTimerWaitComplete as we need to prevent NextTimeout
   // being called twice before we wait on it once as then we miss one of
   // the events and hang on the second wait...

   // By setting waitForOnTimerWaitComplete we cause the OnTimer call to wait
   // for the WaitForOnTimer call before continuing, this guarantees that both
   // calls to NextTimeout do not occur before we wait on the first one.below.

   queue.waitForOnTimerWaitComplete = true;

   CThreadedCallbackTimerQueue timerQueue(queue);

   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));

   CLoggingCallbackTimer timer;

   const IQueueTimers::Handle handle = timerQueue.CreateTimer();

   queue.CheckResult(_T("|GetNextTimeout|CreateTimer: 1|"));

   THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

   THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 500, 1));

   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));
   THROW_ON_FAILURE_EX(true == queue.WaitForOnTimer(REASONABLE_TIME));
   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));

   queue.CheckResult(_T("|SetTimer: 1: 500|GetNextTimeout|BeginTimeoutHandling|HandleTimeout|EndTimeoutHandling|GetNextTimeout|"));
   timer.CheckResult(_T("|OnTimer: 1|"));
}

void CThreadedCallbackTimerQueueTest::TestTimerSetTimerInOnTimer()
{
   CMockTimerQueue queue;

   // We set waitForOnTimerWaitComplete as we need to prevent NextTimeout
   // being called twice before we wait on it once as then we miss one of
   // the events and hang on the second wait...

   // By setting waitForOnTimerWaitComplete we cause the OnTimer call to wait
   // for the WaitForOnTimer call before continuing, this guarantees that both
   // calls to NextTimeout do not occur before we wait on the first one.below.

   queue.waitForOnTimerWaitComplete = true;

   CThreadedCallbackTimerQueue timerQueue(queue);

   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));

   CLoggingCallbackTimer timer;

   IQueueTimers::Handle handle = timerQueue.CreateTimer();

   queue.CheckResult(_T("|GetNextTimeout|CreateTimer: 1|"));

   THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

   timer.SetTimerInOnTimer(timerQueue, handle, 500, 2);

   THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 500, 1));

   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));
   THROW_ON_FAILURE_EX(true == queue.WaitForOnTimer(REASONABLE_TIME));
   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));
   THROW_ON_FAILURE_EX(true == queue.WaitForOnTimer(REASONABLE_TIME));
   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));

   queue.CheckResult(_T("|SetTimer: 1: 500|GetNextTimeout|BeginTimeoutHandling|HandleTimeout|SetTimer: 1: 500|EndTimeoutHandling|GetNextTimeout|BeginTimeoutHandling|HandleTimeout|EndTimeoutHandling|GetNextTimeout|GetNextTimeout|"));
   timer.CheckResult(_T("|OnTimer: 1|TimerSet|OnTimer: 2|"));
}


void CThreadedCallbackTimerQueueTest::TestTimerTimerWheel()
{
   const Milliseconds maximumTimeout = 30000;

   CCallbackTimerWheel timerWheel(maximumTimeout);

   CLoggingCallbackTimer timer;

   {
      CThreadedCallbackTimerQueue timerQueue(timerWheel);

      const IQueueTimers::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 500, 1));

      THROW_ON_FAILURE_EX(true == timer.WaitForTimer(REASONABLE_TIME));
   }

   timer.CheckResult(_T("|OnTimer: 1|"));
}

void CThreadedCallbackTimerQueueTest::TestMultipleTimers()
{
   CTestLog log;

   CLoggingCallbackTimer timer1(log);
   CLoggingCallbackTimer timer2(log);
   CLoggingCallbackTimer timer3(log);
   CLoggingCallbackTimer timer4(log);
   CLoggingCallbackTimer timer5(log);
   CLoggingCallbackTimer timer6(log);

   {
      CThreadedCallbackTimerQueue timerQueue;

      timerQueue.SetTimer(timer1, 400, 1);      // 2
      timerQueue.SetTimer(timer2, 800, 2);      // 4
      timerQueue.SetTimer(timer3, 200, 3);      // 1
      timerQueue.SetTimer(timer4, 1000, 4);     // 5
      timerQueue.SetTimer(timer5, 600, 5);      // 3
      timerQueue.SetTimer(timer6, 1200, 6);     // 6

      THROW_ON_FAILURE_EX(true == timer1.WaitForTimer(REASONABLE_TIME));
      THROW_ON_FAILURE_EX(true == timer2.WaitForTimer(REASONABLE_TIME));
      THROW_ON_FAILURE_EX(true == timer3.WaitForTimer(REASONABLE_TIME));
      THROW_ON_FAILURE_EX(true == timer4.WaitForTimer(REASONABLE_TIME));
      THROW_ON_FAILURE_EX(true == timer5.WaitForTimer(REASONABLE_TIME));
      THROW_ON_FAILURE_EX(true == timer6.WaitForTimer(REASONABLE_TIME));
   }

   log.CheckResult(_T("|OnTimer: 3|OnTimer: 1|OnTimer: 5|OnTimer: 2|OnTimer: 4|OnTimer: 6|"));
}

void CThreadedCallbackTimerQueueTest::TestMultipleTimersTimerWheel()
{
   const Milliseconds maximumTimeout = 30000;

   CTestLog log;

   CLoggingCallbackTimer timer1(log);
   CLoggingCallbackTimer timer2(log);
   CLoggingCallbackTimer timer3(log);
   CLoggingCallbackTimer timer4(log);
   CLoggingCallbackTimer timer5(log);
   CLoggingCallbackTimer timer6(log);

   CCallbackTimerWheel timerWheel(maximumTimeout);

   {
      CThreadedCallbackTimerQueue timerQueue(timerWheel);

      timerQueue.SetTimer(timer1, 400, 1);      // 2
      timerQueue.SetTimer(timer2, 800, 2);      // 4
      timerQueue.SetTimer(timer3, 200, 3);      // 1
      timerQueue.SetTimer(timer4, 1000, 4);     // 5
      timerQueue.SetTimer(timer5, 600, 5);      // 3
      timerQueue.SetTimer(timer6, 1200, 6);     // 6

      THROW_ON_FAILURE_EX(true == timer1.WaitForTimer(REASONABLE_TIME));
      THROW_ON_FAILURE_EX(true == timer2.WaitForTimer(REASONABLE_TIME));
      THROW_ON_FAILURE_EX(true == timer3.WaitForTimer(REASONABLE_TIME));
      THROW_ON_FAILURE_EX(true == timer4.WaitForTimer(REASONABLE_TIME));
      THROW_ON_FAILURE_EX(true == timer5.WaitForTimer(REASONABLE_TIME));
      THROW_ON_FAILURE_EX(true == timer6.WaitForTimer(REASONABLE_TIME));
   }

   log.CheckResult(_T("|OnTimer: 3|OnTimer: 1|OnTimer: 5|OnTimer: 2|OnTimer: 4|OnTimer: 6|"));
}

void CThreadedCallbackTimerQueueTest::TestMonitorTimer()
{
   CMockTimerQueue queue;

   // We set waitForOnTimerWaitComplete as we need to prevent NextTimeout
   // being called twice before we wait on it once as then we miss one of
   // the events and hang on the second wait...

   // By setting waitForOnTimerWaitComplete we cause the OnTimer call to wait
   // for the WaitForOnTimer call before continuing, this guarantees that both
   // calls to NextTimeout do not occur before we wait on the first one.below.

   queue.waitForOnTimerWaitComplete = true;

   CMockThreadedCallbackTimerQueueMonitor monitor;

   CLoggingCallbackTimer timer;

   CThreadedCallbackTimerQueue timerQueue(queue, monitor);

   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));

   const IQueueTimers::Handle handle = timerQueue.CreateTimer();

   queue.CheckResult(_T("|GetNextTimeout|CreateTimer: 1|"));

   THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

   THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 500, 1));

   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));
   THROW_ON_FAILURE_EX(true == queue.WaitForOnTimer(REASONABLE_TIME));
   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));

   queue.CheckResult(_T("|SetTimer: 1: 500|GetNextTimeout|BeginTimeoutHandling|HandleTimeout|EndTimeoutHandling|GetNextTimeout|"));
   timer.CheckResult(_T("|OnTimer: 1|"));

#if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   monitor.CheckResult(_T("|OnTimerProcessingStarted|OnTimerProcessingStopped|"));
#else
   monitor.CheckNoResults();
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadedCallbackTimerQueueTest.cpp
///////////////////////////////////////////////////////////////////////////////

