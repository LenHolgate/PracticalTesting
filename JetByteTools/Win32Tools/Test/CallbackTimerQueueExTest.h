#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_TEST_CALLBACK_TIMER_QUEUE_EX_TEST_INCLUDED__
#define JETBYTE_TOOLS_WIN32_TEST_CALLBACK_TIMER_QUEUE_EX_TEST_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueExTest.h
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

#include "CallbackTimerQueueTestBase.h"

#include "..\CallbackTimerQueueEx.h"

#include "..\Mock\MockTickCount64Provider.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueExTest
///////////////////////////////////////////////////////////////////////////////

struct CCallbackTimerQueueExTestTraits
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

class CCallbackTimerQueueExTest :
   public TCallbackTimerQueueTestBase<
      CCallbackTimerQueueEx,
      CCallbackTimerQueueExTestTraits,
      JetByteTools::Win32::Mock::CMockTickCount64Provider>
{
   public :

      static void TestAll(
         JetByteTools::Test::CTestMonitor &monitor);

      static void TestGetMaxTimeout();
      static void TestMaxTimeout();
      static void TestSetTimerPastTickCount64CountWrap();
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_TEST_CALLBACK_TIMER_QUEUE_EX_TEST_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueueExTest.h
///////////////////////////////////////////////////////////////////////////////
