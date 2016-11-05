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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerWheelTest
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerWheelTest::TestAll(
   CTestMonitor &monitor)
{
   
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestConstruct);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestGetNextTimeoutNoTimersSet);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestGetMaximumTimeout);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestHandleTimeoutsNoTimersSetNoTimePassed);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestHandleTimeoutsNoTimersSet);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestCreateTimer);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestDestroyTimer);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestSetTimer);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestGetNextTimeoutWithTimerSet);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestSetTimerAlreadySet);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestCancelTimerNotSet);
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestCancelTimer);

   RUN_PERFORMANCE_TEST_EX(monitor, CCallbackTimerWheelTest, PerfTestCreateTimer);
   RUN_PERFORMANCE_TEST_EX(monitor, CCallbackTimerWheelTest, PerfTestSetTimer);
   RUN_PERFORMANCE_TEST_EX(monitor, CCallbackTimerWheelTest, PerfTestSetDifferentTimers);
   RUN_PERFORMANCE_TEST_EX(monitor, CCallbackTimerWheelTest, PerfTestSetDifferentTimersSameTimes);
}

void CCallbackTimerWheelTest::TestConstruct()
{
   static const Milliseconds maximumTimeout = 4000;
   
   static const Milliseconds timerGranularity = 50;

   {
      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout);
   }

   {
      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, timerGranularity);
   }

   CMockTickCountProvider tickProvider;

   {
      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);
   }

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   {
      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, timerGranularity, tickProvider);
   }

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));
}

void CCallbackTimerWheelTest::TestGetNextTimeoutNoTimersSet()
{
   static const Milliseconds maximumTimeout = 4000;

   CMockTickCountProvider tickProvider;

   CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   THROW_ON_FAILURE_EX(INFINITE == CallbackTimerWheel.GetNextTimeout());
}

void CCallbackTimerWheelTest::TestGetMaximumTimeout()
{
   CMockTickCountProvider tickProvider;

   {
      static const Milliseconds maximumTimeout = 4000;

      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_FAILURE_EX(maximumTimeout == CallbackTimerWheel.GetMaximumTimeout());

      tickProvider.CheckNoResults();
   }

   {
      static const Milliseconds maximumTimeout = 10;

      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_FAILURE_EX(maximumTimeout == CallbackTimerWheel.GetMaximumTimeout());

      tickProvider.CheckNoResults();
   }
}

void CCallbackTimerWheelTest::TestHandleTimeoutsNoTimersSetNoTimePassed()
{
   static const Milliseconds maximumTimeout = 4000;

   CMockTickCountProvider tickProvider;

   CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   CallbackTimerWheel.HandleTimeouts();

   tickProvider.CheckNoResults();
}

void CCallbackTimerWheelTest::TestHandleTimeoutsNoTimersSet()
{
   static const Milliseconds maximumTimeout = 4000;

   CMockTickCountProvider tickProvider;

   CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   tickProvider.SetTickCount(100);

   CallbackTimerWheel.HandleTimeouts();

   tickProvider.CheckNoResults();
}

void CCallbackTimerWheelTest::TestCreateTimer()
{
   static const Milliseconds maximumTimeout = 4000;

   CMockTickCountProvider tickProvider;

   CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   CCallbackTimerWheel::Handle handle = CallbackTimerWheel.CreateTimer();

   tickProvider.CheckNoResults();

   THROW_ON_FAILURE_EX(CCallbackTimerWheel::InvalidHandleValue != handle);
}

void CCallbackTimerWheelTest::TestDestroyTimer()
{
   static const Milliseconds maximumTimeout = 4000;

   CMockTickCountProvider tickProvider;

   CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   CCallbackTimerWheel::Handle handle = CallbackTimerWheel.CreateTimer();

   tickProvider.CheckNoResults();

   THROW_ON_FAILURE_EX(CCallbackTimerWheel::InvalidHandleValue != handle);

   CCallbackTimerWheel::Handle handleCopy = handle;

   THROW_ON_FAILURE_EX(false == CallbackTimerWheel.DestroyTimer(handle));

   THROW_ON_FAILURE_EX(CCallbackTimerWheel::InvalidHandleValue == handle);

   tickProvider.CheckNoResults();

   THROW_ON_NO_EXCEPTION_EX_1(CallbackTimerWheel.DestroyTimer, handleCopy);

   tickProvider.CheckNoResults();

   THROW_ON_NO_EXCEPTION_EX_1(CallbackTimerWheel.DestroyTimer, CCallbackTimerWheel::InvalidHandleValue);

   tickProvider.CheckNoResults();
}

void CCallbackTimerWheelTest::TestSetTimer()
{
   static const Milliseconds maximumTimeout = 4000;

   CMockTickCountProvider tickProvider;

   CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   CCallbackTimerWheel::Handle handle = CallbackTimerWheel.CreateTimer();

   tickProvider.CheckNoResults();

   CLoggingCallbackTimer timer;

   const CCallbackTimerWheel::UserData userData = 1;

   THROW_ON_FAILURE_EX(false == CallbackTimerWheel.SetTimer(handle, timer, 1000, userData));

   timer.CheckNoResults();
}

void CCallbackTimerWheelTest::TestGetNextTimeoutWithTimerSet()
{
   static const Milliseconds maximumTimeout = 4000;

   static const Milliseconds timerGranularity = 15;

   CMockTickCountProvider tickProvider;

   CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, timerGranularity, tickProvider);

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   CCallbackTimerWheel::Handle handle = CallbackTimerWheel.CreateTimer();

   tickProvider.CheckNoResults();

   CLoggingCallbackTimer timer;

   const CCallbackTimerWheel::UserData userData = 1;

   const Milliseconds timeout = 1000;

   THROW_ON_FAILURE_EX(false == CallbackTimerWheel.SetTimer(handle, timer, timeout, userData));

   timer.CheckNoResults();

   tickProvider.CheckNoResults();

   const Milliseconds expectedTimeout = ((timeout / timerGranularity) + 1) * timerGranularity;

   THROW_ON_FAILURE_EX(expectedTimeout == CallbackTimerWheel.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   tickProvider.SetTickCount(1);

   THROW_ON_FAILURE_EX(expectedTimeout - 1 == CallbackTimerWheel.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 1|"));

   tickProvider.SetTickCount(20);

   THROW_ON_FAILURE_EX(expectedTimeout - 20 == CallbackTimerWheel.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 20|"));

   tickProvider.SetTickCount(2000);

   THROW_ON_FAILURE_EX(2000 > expectedTimeout);

   THROW_ON_FAILURE_EX(0 == CallbackTimerWheel.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 2000|"));
}

void CCallbackTimerWheelTest::TestSetTimerAlreadySet()
{
   static const Milliseconds maximumTimeout = 4000;

   CMockTickCountProvider tickProvider;

   CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   CCallbackTimerWheel::Handle handle = CallbackTimerWheel.CreateTimer();

   tickProvider.CheckNoResults();

   CLoggingCallbackTimer timer;

   const CCallbackTimerWheel::UserData userData = 1;

   THROW_ON_FAILURE_EX(false == CallbackTimerWheel.SetTimer(handle, timer, 1000, userData));

   timer.CheckNoResults();

   tickProvider.CheckNoResults();

   THROW_ON_FAILURE_EX(true == CallbackTimerWheel.SetTimer(handle, timer, 1200, userData));

   timer.CheckNoResults();

   tickProvider.CheckNoResults();
}

void CCallbackTimerWheelTest::TestCancelTimerNotSet()
{
   static const Milliseconds maximumTimeout = 4000;

   CMockTickCountProvider tickProvider;

   CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   CCallbackTimerWheel::Handle handle = CallbackTimerWheel.CreateTimer();

   tickProvider.CheckNoResults();

   THROW_ON_FAILURE_EX(false == CallbackTimerWheel.CancelTimer(handle));

   tickProvider.CheckNoResults();
}

void CCallbackTimerWheelTest::TestCancelTimer()
{
   static const Milliseconds maximumTimeout = 4000;

   CMockTickCountProvider tickProvider;

   CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, tickProvider);

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   CCallbackTimerWheel::Handle handle = CallbackTimerWheel.CreateTimer();

   tickProvider.CheckNoResults();

   CLoggingCallbackTimer timer;

   const CCallbackTimerWheel::UserData userData = 1;

   THROW_ON_FAILURE_EX(false == CallbackTimerWheel.SetTimer(handle, timer, 1000, userData));

   timer.CheckNoResults();

   tickProvider.CheckNoResults();

   THROW_ON_FAILURE_EX(true == CallbackTimerWheel.CancelTimer(handle));

   timer.CheckNoResults();

   tickProvider.CheckNoResults();
}

void CCallbackTimerWheelTest::PerfTestCreateTimer()
{
   static const Milliseconds maximumTimeout = 4000;

   static const Milliseconds timerGranularity = 15;

   CMockTickCountProvider tickProvider;

   tickProvider.logMessages = false;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, timerGranularity, tickProvider);

      CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

      for (size_t i = 0; i < 100000; ++i)
      {
         CCallbackTimerWheel::Handle handle = CallbackTimerWheel.CreateTimer();

         (void)handle;
      }

      const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

      totalTime += timeTaken;

#if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
      OutputEx(_T("Wheel -  ") + ToString(timeTaken) + _T("ms - CreateTimer (100,000 calls)"));
#endif
   }

   OutputEx(_T("Wheel - ") + ToString(totalTime/numRuns) + _T("ms - CreateTimer (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));
}

void CCallbackTimerWheelTest::PerfTestSetTimer()
{
   static const Milliseconds maximumTimeout = 4000;

   static const Milliseconds timerGranularity = 15;

   CLoggingCallbackTimer timer;

   CMockTickCountProvider tickProvider;

   tickProvider.logMessages = false;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, timerGranularity, tickProvider);

      CCallbackTimerWheel::Handle handle = CallbackTimerWheel.CreateTimer();

      CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

      for (size_t i = 0; i < 100000; ++i)
      {
         CallbackTimerWheel.SetTimer(handle, timer, i % maximumTimeout, 0);
      }

      const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

      totalTime += timeTaken;

#if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
      OutputEx(_T("Wheel -  ") + ToString(timeTaken) + _T("ms - SetTimer (100,000 calls)"));
#endif
   }

   OutputEx(_T("Wheel - ") + ToString(totalTime/numRuns) + _T("ms - SetTimer (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));
}

void CCallbackTimerWheelTest::PerfTestSetDifferentTimers()
{
   static const Milliseconds maximumTimeout = 4000;

   static const Milliseconds timerGranularity = 15;

   CLoggingCallbackTimer timer;

   CMockTickCountProvider tickProvider;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      CCallbackTimerWheel CallbackTimerWheel(maximumTimeout, timerGranularity, tickProvider);

      typedef std::vector<CCallbackTimerWheel::Handle> Handles;

      Handles handles;

      static const size_t s_numHandles = 77;

      handles.resize(s_numHandles);

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         handles[i] = CallbackTimerWheel.CreateTimer();
      }

      {
         CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

         for (size_t i = 0; i < 100000; ++i)
         {
            CallbackTimerWheel.SetTimer(handles[i % s_numHandles], timer, i % maximumTimeout, 0);
         }

         const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

         totalTime += timeTaken;

#if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
         OutputEx(_T("Wheel -  ") + ToString(timeTaken) + _T("ms - SetTimer (Different timers) (100,000 calls)"));
#endif
      }

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         CallbackTimerWheel.DestroyTimer(handles[i]);
      }
   }

   OutputEx(_T("Wheel - ") + ToString(totalTime/numRuns) + _T("ms - SetTimer (Different timers) (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));
}

void CCallbackTimerWheelTest::PerfTestSetDifferentTimersSameTimes()
{
   static const Milliseconds maximumTimeout = 4000;

   static const Milliseconds timerGranularity = 15;

   CLoggingCallbackTimer timer;

   CMockTickCountProvider tickProvider;

   tickProvider.logMessages = false;

   Milliseconds totalTime = 0;

   const size_t numRuns = JETBYTE_PERF_NUM_TEST_RUNS;

   for (size_t y = 0; y < numRuns; ++y)
   {
      CCallbackTimerWheel timerWheel(maximumTimeout, timerGranularity, tickProvider);

      typedef std::vector<CCallbackTimerWheel::Handle> Handles;

      Handles handles;

      static const size_t s_numHandles = 1000;

      handles.resize(s_numHandles);

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         handles[i] = timerWheel.CreateTimer();
      }

      {
         CPerformanceCounter counter(CPerformanceCounter::LockThreadToOneCPU);

         for (size_t i = 0; i < 100; ++i)
         {
            for (size_t x = 0; x < s_numHandles; ++x)
            {
               timerWheel.SetTimer(handles[x], timer, ((1 + i) * 100) % 4000, 0);
            }
         }

         const DWORD timeTaken = counter.GetElapsedTimeAsDWORD();

         totalTime += timeTaken;

#if JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES
         OutputEx(_T("Wheel -  ") + ToString(timeTaken) + _T("ms - SetTimer (Different timers, same times) (100,000 calls)"));
#endif
      }

      for (size_t i = 0; i < s_numHandles; ++i)
      {
         timerWheel.DestroyTimer(handles[i]);
      }
   }

   OutputEx(_T("Wheel - ") + ToString(totalTime/numRuns) + _T("ms - SetTimer (Different timers, same times) (100,000 calls - average of ") + ToString(numRuns) + _T(" runs)"));
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

