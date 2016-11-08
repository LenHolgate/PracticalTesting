#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_TEST_CALLBACK_TIMER_WHEEL_TEST_INCLUDED__
#define JETBYTE_TOOLS_WIN32_TEST_CALLBACK_TIMER_WHEEL_TEST_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerWheelTest.h
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

#include "CallbackTimerQueueTestBase.h"

#include "JetByteTools\Win32Tools\Mock\TestCallbackTimerWheel.h"

#include "JetByteTools\Win32Tools\Mock\MockTickCountProvider.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerWheelTest
///////////////////////////////////////////////////////////////////////////////

/// A test for CCallbackTimerWheel
/// \test
/// \ingroup Win32ToolsTests

class CCallbackTimerWheelTest :
   public TCallbackTimerQueueTestBase<
      JetByteTools::Win32::Mock::CTestCallbackTimerWheel,
      CCallbackTimerWheelTest,
      JetByteTools::Win32::Mock::CMockTickCountProvider>
{
   public :

      static void TestAll(
         JetByteTools::Test::CTestMonitor &monitor);

      static void TestConstruct();
      static void TestGetMaximumTimeout();
      static void TestGetNextTimeout();
      static void TestSetTimerWhenNowNotEqualToCurrent();
      static void TestSetTimerWhenNowMoreThanMaxTimeoutLargerThanCurrent();
      static void TestSetTimerWhenNowMoreThanMaxTimeoutLargerThanCurrentNoOtherTimersSet();
      static void TestOnShotTimerSetTimerWhenNowNotEqualToCurrent();
      static void TestOnShotTimerSetTimerWhenNowMoreThanMaxTimeoutLargerThanCurrent();
      static void TestOnShotTimerSetTimerWhenNowMoreThanMaxTimeoutLargerThanCurrentNoOtherTimersSet();
      static void TestGetNextTimeoutWhenWheelWraps();

      // Test traits...
      enum traits
      {
         timerGranularity = 15,
         maxTimeout = 4000,
         creationQueriesTicks = true,
         creationCreatesTimer = false,
         setQueriesTicks = true,
         failedSetQueriesTicks = true,
         handleTimeoutQueriesTicksPerTimer = false,
         timersAtSameTimeAreExpiredInOrderSet = false,

   #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)

         monitoringEnabled = true

   #else

         monitoringEnabled = false

   #endif

      };

      static const _tstring shortName;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_TEST_CALLBACK_TIMER_WHEEL_TEST_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerWheelTest.h
///////////////////////////////////////////////////////////////////////////////
