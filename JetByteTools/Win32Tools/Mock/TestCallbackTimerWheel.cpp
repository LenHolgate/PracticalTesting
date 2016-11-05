///////////////////////////////////////////////////////////////////////////////
// File: TestCallbackTimerWheel.cpp
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

#include "TestCallbackTimerWheel.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CTestCallbackTimerWheel
///////////////////////////////////////////////////////////////////////////////

CTestCallbackTimerWheel::CTestCallbackTimerWheel()
   :  CCallbackTimerWheel(4000)
{

}

CTestCallbackTimerWheel::CTestCallbackTimerWheel(
   IMonitorCallbackTimerQueue &monitor)
   :  CCallbackTimerWheel(monitor, 4000)
{

}

CTestCallbackTimerWheel::CTestCallbackTimerWheel(
   const IProvideTickCount &tickProvider)
   :  CCallbackTimerWheel(4000, tickProvider)
{

}

CTestCallbackTimerWheel::CTestCallbackTimerWheel(
   IMonitorCallbackTimerQueue &monitor,
   const IProvideTickCount &tickProvider)
   :  CCallbackTimerWheel(monitor, 4000, tickProvider)
{

}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: TestCallbackTimerWheel.cpp
///////////////////////////////////////////////////////////////////////////////
