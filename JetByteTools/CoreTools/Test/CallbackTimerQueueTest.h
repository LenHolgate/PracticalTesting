#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueTest.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2008 JetByte Limited.
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

#include "JetByteTools/CoreTools/CallbackTimerQueue.h"

#include "JetByteTools/CoreTools/Mock/MockTickCount64Provider.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueTest
///////////////////////////////////////////////////////////////////////////////

struct CCallbackTimerQueueTestTraits
{
   enum traits
   {
      timerGranularity = 1,
      maxTimeout = 4294967294,
      creationQueriesTicks = false,
      creationCreatesTimer = false,
      setQueriesTicks = true,
      failedSetQueriesTicks = false,
      handleTimeoutQueriesTicksPerTimer = true,
      timersAtSameTimeAreExpiredInOrderSet = true,
      #if (JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES == 1)
      handleValidationEnabled = true,
      #else
      handleValidationEnabled = false,
      #endif
      #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
      monitoringEnabled = true
      #else
      monitoringEnabled = false
      #endif
   };

   static const _tstring shortName;
};

/// A test for CCallbackTimerQueue
/// \test
/// \ingroup Win32ToolsTests

class CCallbackTimerQueueTest :
   public TCallbackTimerQueueTestBase<
      CCallbackTimerQueue,
      CCallbackTimerQueueTestTraits,
      Mock::CMockTickCount64Provider>
{
   public :

      static void TestAll(
         JetByteTools::Test::CTestMonitor &monitor);

      static void TestGetMaxTimeout();
      static void TestSetTimerPastTickCount64CountWrap();
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueueTest.h
///////////////////////////////////////////////////////////////////////////////
