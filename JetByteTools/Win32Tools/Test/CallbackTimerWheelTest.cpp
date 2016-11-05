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

const _tstring CCallbackTimerWheelTest::shortName = _T("  W - ");

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerWheelTest
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerWheelTest::TestAll(
   CTestMonitor &monitor)
{
   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestGetNextTimeout);

   Base::TestAll(monitor, _T("CCallbackTimerWheel"));

   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestConstruct);

   RUN_TEST_EX(monitor, CCallbackTimerWheelTest, TestGetMaximumTimeout);
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

void CCallbackTimerWheelTest::TestGetNextTimeout()
{
   CMockTickCountProvider tickProvider;

   tickProvider.logTickCount = false;

   {
      static const Milliseconds maximumTimeout = 4000;

      static const Milliseconds timerGranularity = 15;

      CCallbackTimerWheel timerWheel(maximumTimeout, timerGranularity, tickProvider);

      tickProvider.CheckResult(_T("|GetTickCount|"));

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

