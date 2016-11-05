#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_TEST_CALLBACK_TIMER_QUEUE_TEST_INCLUDED__
#define JETBYTE_TOOLS_WIN32_TEST_CALLBACK_TIMER_QUEUE_TEST_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueTest.h 
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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueTest
///////////////////////////////////////////////////////////////////////////////

/// A test for CCallbackTimerQueue
/// \test
/// \ingroup Win32ToolsTests

class CCallbackTimerQueueTest
{
   public :

      static void TestAll();
      static void TestConstruct();
      static void TestCreateTimer();
      static void TestDestroyTimer();
      static void TestTimer();
      static void TestMultipleTimers();
      static void TestCancelTimer();
      static void TestCancelExpiredTimer();
      static void TestMaxTimeout();
      static void TestTickCountWrap();
      static void TestTickCountWrap2();
      static void TestTickCountWrap3();
      static void TestGetMaxTimeout();
      static void TestSetTimerPastTickCount64CountWrap();
      static void TestResetTimer();
      static void TestOneShotTimer();
      static void TestActiveTimersAtDestructionTime();
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_WIN32_TEST_CALLBACK_TIMER_QUEUE_TEST_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueueTest.h
///////////////////////////////////////////////////////////////////////////////
