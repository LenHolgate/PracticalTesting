#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerWheelTest.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2010 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#include "CallbackTimerQueueTestBase.h"

#include "JetByteTools/CoreTools/Mock/TestCallbackTimerWheel.h"

#include "JetByteTools/CoreTools/Mock/MockTickCountProvider.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerWheelTest
///////////////////////////////////////////////////////////////////////////////

/// A test for CCallbackTimerWheel
/// \test
/// \ingroup Win32ToolsTests

class CCallbackTimerWheelTest :
   public TCallbackTimerQueueTestBase<
      Mock::CTestCallbackTimerWheel,
      CCallbackTimerWheelTest,
      Mock::CMockTickCountProvider>
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
         #if (JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES == 1)
         handleValidationEnabled = true,
         #else
         handleValidationEnabled = false,
         #endif
         #if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
         monitoringEnabled = true
         #else
         monitoringEnabled = false
         #endif
      };

      static const _tstring shortName;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerWheelTest.h
///////////////////////////////////////////////////////////////////////////////
