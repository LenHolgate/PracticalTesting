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
// CCallbackTimerWheelTest
///////////////////////////////////////////////////////////////////////////////

/// A test for CCallbackTimerWheel
/// \test
/// \ingroup Win32ToolsTests

class CCallbackTimerWheelTest
{
   public :

      static void TestAll(
         JetByteTools::Test::CTestMonitor &monitor);

      static void TestConstruct();
      static void TestGetNextTimeoutNoTimersSet();
      static void TestGetMaximumTimeout();
      static void TestHandleTimeoutsNoTimersSetNoTimePassed();
      static void TestHandleTimeoutsNoTimersSet();
      static void TestCreateTimer();
      static void TestDestroyTimer();
      static void TestSetTimer();
      static void TestGetNextTimeoutWithTimerSet();
      static void TestSetTimerAlreadySet();
      static void TestCancelTimerNotSet();
      static void TestCancelTimer();

      static void PerfTestCreateTimer();
      static void PerfTestSetTimer();
      static void PerfTestSetDifferentTimers();
      static void PerfTestSetDifferentTimersSameTimes();
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
