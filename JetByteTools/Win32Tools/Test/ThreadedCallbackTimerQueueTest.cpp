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

#include "JetByteTools\Admin\Admin.h"

#include "ThreadedCallbackTimerQueueTest.h"

#include "..\Mock\MockTickCountProvider.h"
#include "..\Mock\MockTickCount64Provider.h"
#include "..\Mock\LoggingCallbackTimer.h"
#include "..\Mock\MockTimerQueue.h"

#include "JetByteTools\TestTools\TestException.h"
#include "JetByteTools\TestTools\RunTest.h"

#pragma hdrstop

#include "..\ThreadedCallbackTimerQueue.h"
#include "..\CallbackTimerWheel.h"

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;
using JetByteTools::Test::TestRequiresVistaOrLater;
using JetByteTools::Test::CTestMonitor;
using JetByteTools::Test::CTestLog;

using JetByteTools::Win32::Mock::CMockTickCountProvider;
using JetByteTools::Win32::Mock::CMockTickCount64Provider;
using JetByteTools::Win32::Mock::CLoggingCallbackTimer;
using JetByteTools::Win32::Mock::CMockTimerQueue;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static void TestTimerImpl(
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation);

static void TestTimerImpl(
   IManageTimerQueue &impl,
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation);

static void TestMultipleTimers(
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation);

static void TestMultipleTimers(
   IManageTimerQueue &impl,
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation);

///////////////////////////////////////////////////////////////////////////////
// CThreadedCallbackTimerQueueTest
///////////////////////////////////////////////////////////////////////////////

void CThreadedCallbackTimerQueueTest::TestAll(
   CTestMonitor &monitor)
{
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestConstructBestForPlatform);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestConstructTickCount64);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestConstructTickCount64);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestConstructCustomQueue);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestConstructWithTimerWheel);

   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestBeginShutdown);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestWaitForShutdownToComplete);

   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestTimer);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestTimerNoLock);

   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestTimerTickCount64);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestTimerTickCount64NoLock);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestTimerHybridTickCount64);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestTimerHybridTickCount64NoLock);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestTimerTimerWheel);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestTimerTimerWheelNoLock);

   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestMultipleTimersTickCount64);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestMultipleTimersTickCount64NoLock);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestMultipleTimersHybridTickCount64);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestMultipleTimersHybridTickCount64NoLock);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestMultipleTimersTimerWheel);
   RUN_TEST_EX(monitor, CThreadedCallbackTimerQueueTest, TestMultipleTimersTimerWheelNoLock);

}

void CThreadedCallbackTimerQueueTest::TestConstructBestForPlatform()
{
   {
      CThreadedCallbackTimerQueue timerQueue;
   }

   {
      CThreadedCallbackTimerQueue timerQueue(CThreadedCallbackTimerQueue::BestForPlatform);
   }

   {
      CThreadedCallbackTimerQueue timerQueue(CThreadedCallbackTimerQueue::BestForPlatformNoLock);
   }
}

void CThreadedCallbackTimerQueueTest::TestConstructTickCount64()
{
   TestRequiresVistaOrLater();

   {
      CThreadedCallbackTimerQueue timerQueue(CThreadedCallbackTimerQueue::TickCount64);
   }

   {
      CThreadedCallbackTimerQueue timerQueue(CThreadedCallbackTimerQueue::TickCount64NoLock);
   }

   {
      CMockTickCount64Provider tickProvider;

      CThreadedCallbackTimerQueue timerQueue(tickProvider);

      tickProvider.CheckNoResults();

      CThreadedCallbackTimerQueue timerQueue2(tickProvider, CThreadedCallbackTimerQueue::DispatchTimersNoLock);

      tickProvider.CheckNoResults();

      THROW_ON_NO_EXCEPTION_EX_2(CThreadedCallbackTimerQueue, tickProvider, CThreadedCallbackTimerQueue::BestForPlatform);
      THROW_ON_NO_EXCEPTION_EX_2(CThreadedCallbackTimerQueue, tickProvider, CThreadedCallbackTimerQueue::TickCount64);
      THROW_ON_NO_EXCEPTION_EX_2(CThreadedCallbackTimerQueue, tickProvider, CThreadedCallbackTimerQueue::HybridTickCount64);

      tickProvider.CheckNoResults();
   }
}

void CThreadedCallbackTimerQueueTest::TestConstructHybridTickCount64()
{
   {
      CThreadedCallbackTimerQueue timerQueue(CThreadedCallbackTimerQueue::HybridTickCount64);
   }

   {
      CThreadedCallbackTimerQueue timerQueue(CThreadedCallbackTimerQueue::HybridTickCount64NoLock);
   }

   {
      CMockTickCountProvider tickProvider;

      CThreadedCallbackTimerQueue timerQueue(tickProvider);

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));       // creating a queue sets an 
                                                               // internal maintenance timer

      CThreadedCallbackTimerQueue timerQueue2(tickProvider, CThreadedCallbackTimerQueue::DispatchTimersNoLock);

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_NO_EXCEPTION_EX_2(CThreadedCallbackTimerQueue, tickProvider, CThreadedCallbackTimerQueue::BestForPlatform);

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_NO_EXCEPTION_EX_2(CThreadedCallbackTimerQueue, tickProvider, CThreadedCallbackTimerQueue::TickCount64);

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_NO_EXCEPTION_EX_2(CThreadedCallbackTimerQueue, tickProvider, CThreadedCallbackTimerQueue::HybridTickCount64);

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));
   }
}

void CThreadedCallbackTimerQueueTest::TestConstructCustomQueue()
{
   CMockTimerQueue queue;

   {
      CThreadedCallbackTimerQueue timerQueue(queue);

      THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));
   }

   queue.CheckResult(_T("|GetNextTimeout|"));

   {
      CThreadedCallbackTimerQueue timerQueue(queue, CThreadedCallbackTimerQueue::DispatchTimersWithLock);

      THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));
   }

   queue.CheckResult(_T("|GetNextTimeout|"));

   {
      CThreadedCallbackTimerQueue timerQueue(queue, CThreadedCallbackTimerQueue::DispatchTimersNoLock);

      THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));
   }

   queue.CheckResult(_T("|GetNextTimeout|"));

   THROW_ON_NO_EXCEPTION_EX_2(CThreadedCallbackTimerQueue, queue, CThreadedCallbackTimerQueue::BestForPlatform);
   THROW_ON_NO_EXCEPTION_EX_2(CThreadedCallbackTimerQueue, queue, CThreadedCallbackTimerQueue::TickCount64);
   THROW_ON_NO_EXCEPTION_EX_2(CThreadedCallbackTimerQueue, queue, CThreadedCallbackTimerQueue::HybridTickCount64);

   queue.CheckNoResults();
}

void CThreadedCallbackTimerQueueTest::TestConstructWithTimerWheel()
{
   const Milliseconds maximumTimeout = 4000;

   CMockTickCountProvider tickProvider;

   CCallbackTimerWheel wheel(maximumTimeout, tickProvider);

   {
      CThreadedCallbackTimerQueue timerQueue(wheel);

   }

   {
      CThreadedCallbackTimerQueue timerQueue(wheel, CThreadedCallbackTimerQueue::DispatchTimersWithLock);
   }

   {
      CThreadedCallbackTimerQueue timerQueue(wheel, CThreadedCallbackTimerQueue::DispatchTimersNoLock);
   }

   THROW_ON_NO_EXCEPTION_EX_2(CThreadedCallbackTimerQueue, wheel, CThreadedCallbackTimerQueue::BestForPlatform);
   THROW_ON_NO_EXCEPTION_EX_2(CThreadedCallbackTimerQueue, wheel, CThreadedCallbackTimerQueue::TickCount64);
   THROW_ON_NO_EXCEPTION_EX_2(CThreadedCallbackTimerQueue, wheel, CThreadedCallbackTimerQueue::HybridTickCount64);
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
   TestTimerWithLock();
}

void CThreadedCallbackTimerQueueTest::TestTimerWithLock()
{
   CMockTimerQueue queue;

   CThreadedCallbackTimerQueue timerQueue(queue, CThreadedCallbackTimerQueue::DispatchTimersWithLock);

   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));

   CLoggingCallbackTimer timer;

   IQueueTimers::Handle handle = timerQueue.CreateTimer();

   queue.CheckResult(_T("|GetNextTimeout|CreateTimer: 1|"));

   THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

   THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 500, 1));

   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));
   THROW_ON_FAILURE_EX(true == queue.WaitForOnTimer(REASONABLE_TIME));
   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));

   queue.CheckResult(_T("|SetTimer: 1: 500|GetNextTimeout|HandleTimeouts|GetNextTimeout|"));
   timer.CheckResult(_T("|OnTimer: 1|"));
}

void CThreadedCallbackTimerQueueTest::TestTimerNoLock()
{
   CMockTimerQueue queue;

   CThreadedCallbackTimerQueue timerQueue(queue, CThreadedCallbackTimerQueue::DispatchTimersNoLock);

   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));

   CLoggingCallbackTimer timer;

   IQueueTimers::Handle handle = timerQueue.CreateTimer();

   queue.CheckResult(_T("|GetNextTimeout|CreateTimer: 1|"));

   THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

   THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 500, 1));

   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));
   THROW_ON_FAILURE_EX(true == queue.WaitForOnTimer(REASONABLE_TIME));
   THROW_ON_FAILURE_EX(true == queue.WaitForNextTimeout(REASONABLE_TIME));

   queue.CheckResult(_T("|SetTimer: 1: 500|GetNextTimeout|BeginTimeoutHandling|HandleTimeout|EndTimeoutHandling|GetNextTimeout|"));
   timer.CheckResult(_T("|OnTimer: 1|"));
}

void CThreadedCallbackTimerQueueTest::TestTimerTickCount64()
{
   TestRequiresVistaOrLater();

   TestTimerImpl(CThreadedCallbackTimerQueue::TickCount64);
}

void CThreadedCallbackTimerQueueTest::TestTimerTickCount64NoLock()
{
   TestRequiresVistaOrLater();

   TestTimerImpl(CThreadedCallbackTimerQueue::TickCount64NoLock);
}

void CThreadedCallbackTimerQueueTest::TestTimerHybridTickCount64()
{
   TestTimerImpl(CThreadedCallbackTimerQueue::HybridTickCount64);
}

void CThreadedCallbackTimerQueueTest::TestTimerHybridTickCount64NoLock()
{
   TestTimerImpl(CThreadedCallbackTimerQueue::HybridTickCount64NoLock);
}

void CThreadedCallbackTimerQueueTest::TestTimerTimerWheel()
{
   const Milliseconds maximumTimeout = 30000;

   CCallbackTimerWheel timerWheel(maximumTimeout);

   TestTimerImpl(timerWheel, CThreadedCallbackTimerQueue::DispatchTimersWithLock);
}

void CThreadedCallbackTimerQueueTest::TestTimerTimerWheelNoLock()
{
   const Milliseconds maximumTimeout = 30000;

   CCallbackTimerWheel timerWheel(maximumTimeout);

   TestTimerImpl(timerWheel, CThreadedCallbackTimerQueue::DispatchTimersWithLock);
}

void CThreadedCallbackTimerQueueTest::TestMultipleTimersTickCount64()
{
   TestRequiresVistaOrLater();

   TestMultipleTimers(CThreadedCallbackTimerQueue::TickCount64);
}

void CThreadedCallbackTimerQueueTest::TestMultipleTimersTickCount64NoLock()
{
   TestRequiresVistaOrLater();

   TestMultipleTimers(CThreadedCallbackTimerQueue::TickCount64NoLock);
}

void CThreadedCallbackTimerQueueTest::TestMultipleTimersHybridTickCount64()
{
   TestMultipleTimers(CThreadedCallbackTimerQueue::HybridTickCount64);
}

void CThreadedCallbackTimerQueueTest::TestMultipleTimersHybridTickCount64NoLock()
{
   TestMultipleTimers(CThreadedCallbackTimerQueue::HybridTickCount64NoLock);
}

void CThreadedCallbackTimerQueueTest::TestMultipleTimersTimerWheel()
{
   const Milliseconds maximumTimeout = 30000;

   CCallbackTimerWheel timerWheel(maximumTimeout);

   TestMultipleTimers(timerWheel, CThreadedCallbackTimerQueue::DispatchTimersWithLock);
}

void CThreadedCallbackTimerQueueTest::TestMultipleTimersTimerWheelNoLock()
{
   const Milliseconds maximumTimeout = 30000;

   CCallbackTimerWheel timerWheel(maximumTimeout);

   TestMultipleTimers(timerWheel, CThreadedCallbackTimerQueue::DispatchTimersWithLock);
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static void TestTimerImpl(
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation)
{
   CLoggingCallbackTimer timer;

   {
      CThreadedCallbackTimerQueue timerQueue(timerQueueImplementation);

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 500, 1));

      THROW_ON_FAILURE_EX(true == timer.WaitForTimer(REASONABLE_TIME));
   }

   timer.CheckResult(_T("|OnTimer: 1|"));
}

static void TestTimerImpl(
   IManageTimerQueue &impl,
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation)
{
   CLoggingCallbackTimer timer;

   {
      CThreadedCallbackTimerQueue timerQueue(impl, timerQueueImplementation);

      IQueueTimers::Handle handle = timerQueue.CreateTimer();

      THROW_ON_FAILURE_EX(IQueueTimers::InvalidHandleValue != handle);

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 500, 1));

      THROW_ON_FAILURE_EX(true == timer.WaitForTimer(REASONABLE_TIME));
   }

   timer.CheckResult(_T("|OnTimer: 1|"));
}

static void TestMultipleTimers(
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation)
{
   CTestLog log;

   CLoggingCallbackTimer timer1(log);
   CLoggingCallbackTimer timer2(log);
   CLoggingCallbackTimer timer3(log);
   CLoggingCallbackTimer timer4(log);
   CLoggingCallbackTimer timer5(log);
   CLoggingCallbackTimer timer6(log);

   {
      CThreadedCallbackTimerQueue timerQueue(timerQueueImplementation);

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

static void TestMultipleTimers(
   IManageTimerQueue &impl,
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation)
{
   CTestLog log;

   CLoggingCallbackTimer timer1(log);
   CLoggingCallbackTimer timer2(log);
   CLoggingCallbackTimer timer3(log);
   CLoggingCallbackTimer timer4(log);
   CLoggingCallbackTimer timer5(log);
   CLoggingCallbackTimer timer6(log);

   {
      CThreadedCallbackTimerQueue timerQueue(impl, timerQueueImplementation);

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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadedCallbackTimerQueueTest.cpp
///////////////////////////////////////////////////////////////////////////////

