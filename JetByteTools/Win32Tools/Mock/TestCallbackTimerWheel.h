#pragma once
#ifndef JETBYTE_TOOLS_WIN32_MOCK_TEST_CALLBACK_TIMER_WHEEL_INCLUDED__
#define JETBYTE_TOOLS_WIN32_MOCK_TEST_CALLBACK_TIMER_WHEEL_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: TestCallbackTimerWheel.h
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

#include "JetByteTools/Win32Tools/CallbackTimerWheel.h"

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools
{
   namespace Win32
   {
      class IProvideTickCount;
      class IMonitorCallbackTimerQueue;
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CTestCallbackTimerWheel
///////////////////////////////////////////////////////////////////////////////

/// \ingroup Win32ToolsMocks

class CTestCallbackTimerWheel : public CCallbackTimerWheel
{
   public :

      CTestCallbackTimerWheel();

      explicit CTestCallbackTimerWheel(
         IMonitorCallbackTimerQueue &monitor);

      /// Create a timer queue that uses the provdided instance of
      /// IProvideTickCount to obtain its tick counts rather than getting
      /// them directly from the system.

      explicit CTestCallbackTimerWheel(
         const IProvideTickCount &tickProvider);

      /// Create a timer queue that uses the provdided instance of
      /// IProvideTickCount to obtain its tick counts rather than getting
      /// them directly from the system. Monitor it with the supplied monitor.

      CTestCallbackTimerWheel(
         IMonitorCallbackTimerQueue &monitor,
         const IProvideTickCount &tickProvider);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_MOCK_TEST_CALLBACK_TIMER_WHEEL_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: TestCallbackTimerWheel.h
///////////////////////////////////////////////////////////////////////////////
