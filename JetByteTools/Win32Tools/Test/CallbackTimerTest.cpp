///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerTest.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
//
// JetByte Limited grants you ("Licensee") a non-exclusive, royalty free, 
// licence to use, modify and redistribute this software in source and binary 
// code form, provided that i) this copyright notice and licence appear on all 
// copies of the software; and ii) Licensee does not utilize the software in a 
// manner which is disparaging to JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All 
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors 
// shall not be liable for any damages suffered by licensee as a result of 
// using, modifying or distributing the software or its derivatives. In no
// event will JetByte Limited be liable for any lost revenue, profit or data,
// or for direct, indirect, special, consequential, incidental or punitive
// damages, however caused and regardless of the theory of liability, arising 
// out of the use of or inability to use software, even if JetByte Limited 
// has been advised of the possibility of such damages.
//
// This software is not designed or intended for use in on-line control of 
// aircraft, air traffic, aircraft navigation or aircraft communications; or in 
// the design, construction, operation or maintenance of any nuclear 
// facility. Licensee represents and warrants that it will not use or 
// redistribute the Software for such purposes. 
//
///////////////////////////////////////////////////////////////////////////////

#include "CallbackTimerTest.h"

#include "..\CallbackTimer.h"

#include "..\Mock\LoggingCallbackTimerHandleCallback.h"

#include "JetByteTools\Win32Tools\Utils.h"

#include "JetByteTools\TestTools\TestException.h"

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -save
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;

using JetByteTools::Win32::Output;
using JetByteTools::Win32::_tstring;

using JetByteTools::Win32::Mock::CLoggingCallbackTimerHandleCallback;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerTest
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerTest::TestAll()
{
   TestConstruct();

   TestTimer();
}

void CCallbackTimerTest::TestConstruct()
{
   const _tstring functionName = _T("CCallbackTimerTest::TestConstruct");
   
   Output(functionName + _T(" - start"));

   CCallbackTimer timer;

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerTest::TestTimer()
{
   const _tstring functionName = _T("CCallbackTimerTest::TestTimer");
   
   Output(functionName + _T(" - start"));

   CCallbackTimer timer;

   CLoggingCallbackTimerHandleCallback callback;

   CCallbackTimer::Handle handle(callback);

   timer.SetTimer(handle, 100, 1);

   THROW_ON_FAILURE(functionName, true == callback.WaitForTimer(200));

   callback.CheckResult(_T("|OnTimer: 1|"));

   Output(functionName + _T(" - stop"));
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -restore
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerTest.cpp
///////////////////////////////////////////////////////////////////////////////

