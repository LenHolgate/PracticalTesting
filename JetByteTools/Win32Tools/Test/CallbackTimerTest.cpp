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

#include "..\Mock\LoggingCallbackTimerHandle.h"
#include "..\Mock\MockTickCountProvider.h"
#include "..\Mock\TestCallbackTimer.h"

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

using JetByteTools::Win32::Mock::CLoggingCallbackTimerHandle;
using JetByteTools::Win32::Mock::CMockTickCountProvider;
using JetByteTools::Win32::Mock::CTestCallbackTimer;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

static DWORD s_delay = 1000;
//static DWORD s_delay = INFINITE;

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerTest
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerTest::TestAll()
{
   TestConstruct();
   TestTimer();
   TestMultipleTimers();
   TestCancelTimer();
   TestTickCountWrap();
}

void CCallbackTimerTest::TestConstruct()
{
   const _tstring functionName = _T("CCallbackTimerTest::TestConstruct");
   
   Output(functionName + _T(" - start"));

   CCallbackTimer timer;

   CMockTickCountProvider tickProvider;

   CCallbackTimer timer2(tickProvider);

   THROW_ON_FAILURE(functionName, true == tickProvider.AllowCalls(1, s_delay));

   CTestCallbackTimer timer3(1000, s_delay);

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerTest::TestTimer()
{
   const _tstring functionName = _T("CCallbackTimerTest::TestTimer");
   
   Output(functionName + _T(" - start"));

   CTestCallbackTimer timer(1000, s_delay);

   CLoggingCallbackTimerHandle handle;

   timer.SetTimerAndWait(handle, 100, 1);

   // Prove that time is standing still
   THROW_ON_FAILURE(functionName, false == handle.WaitForTimer(0));

   handle.CheckResult(_T("|"));

   timer.SetTickCountAndWait(1100, false);

   THROW_ON_FAILURE(functionName, true == handle.WaitForTimer(s_delay));

   handle.CheckResult(_T("|OnTimer: 1|"));

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerTest::TestMultipleTimers()
{
   const _tstring functionName = _T("CCallbackTimerTest::TestMultipleTimers");
   
   Output(functionName + _T(" - start"));

   CTestCallbackTimer timer(1000, s_delay);

   CLoggingCallbackTimerHandle handle1;
   CLoggingCallbackTimerHandle handle2;
   CLoggingCallbackTimerHandle handle3;
   CLoggingCallbackTimerHandle handle4;
   CLoggingCallbackTimerHandle handle5;
   CLoggingCallbackTimerHandle handle6;

   timer.SetTimerAndWait(handle1, 300, 1);
   timer.SetTimerAndWait(handle2, 100, 2);
   timer.SetTimerAndWait(handle3, 200, 3);
   timer.SetTimerAndWait(handle4, 150, 4);
   timer.SetTimerAndWait(handle5, 150, 5);
   timer.SetTimerAndWait(handle6, 160, 6);

   // Prove that time is standing still

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerNotExpired();
   handle3.CheckTimerNotExpired();
   handle4.CheckTimerNotExpired();
   handle5.CheckTimerNotExpired();
   handle6.CheckTimerNotExpired();

   timer.SetTickCountAndWait(1100, true);

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerExpired(2, s_delay);
   handle3.CheckTimerNotExpired();
   handle4.CheckTimerNotExpired();
   handle5.CheckTimerNotExpired();
   handle6.CheckTimerNotExpired();

   timer.SetTickCountAndWait(1155, true);

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerNotExpired();
   handle3.CheckTimerNotExpired();
   handle4.CheckTimerExpired(4, s_delay);
   handle5.CheckTimerExpired(5, s_delay);
   handle6.CheckTimerNotExpired();

   timer.SetTickCountAndWait(1201, true);

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerNotExpired();
   handle3.CheckTimerExpired(3, s_delay);
   handle4.CheckTimerNotExpired();
   handle5.CheckTimerNotExpired();
   handle6.CheckTimerExpired(6, s_delay);

   timer.SetTickCountAndWait(1300, false);

   handle1.CheckTimerExpired(1, s_delay);
   handle2.CheckTimerNotExpired();
   handle3.CheckTimerNotExpired();
   handle4.CheckTimerNotExpired();
   handle5.CheckTimerNotExpired();
   handle6.CheckTimerNotExpired();

   timer.SetTimerAndWait(handle1, 100, 1);
   timer.SetTimerAndWait(handle2, 200, 2);

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerNotExpired();

   timer.SetTickCountAndWait(1350, true);

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerNotExpired();

   timer.SetTickCountAndWait(1400, true);

   handle1.CheckTimerExpired(1, s_delay);
   handle2.CheckTimerNotExpired();

   timer.SetTickCountAndWait(1400, true);

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerNotExpired();

   timer.SetTickCountAndWait(1600, false);

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerExpired(2, s_delay);

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerTest::TestCancelTimer()
{
   const _tstring functionName = _T("CCallbackTimerTest::TestCancelTimer");
   
   Output(functionName + _T(" - start"));

   CTestCallbackTimer timer(1000, s_delay);

   CLoggingCallbackTimerHandle handle1;
   CLoggingCallbackTimerHandle handle2;
   CLoggingCallbackTimerHandle handle3;
   CLoggingCallbackTimerHandle handle4;

   timer.SetTimerAndWait(handle1, 100, 1);
   timer.SetTimerAndWait(handle2, 200, 2);
   timer.SetTimerAndWait(handle3, 150, 3);
   timer.SetTimerAndWait(handle4, 150, 4);

   // Prove that time is standing still

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerNotExpired();
   handle3.CheckTimerNotExpired();
   handle4.CheckTimerNotExpired();

   timer.SetTickCountAndWait(1100, true);

   handle1.CheckTimerExpired(1, s_delay);
   handle2.CheckTimerNotExpired();
   handle3.CheckTimerNotExpired();
   handle4.CheckTimerNotExpired();

   THROW_ON_FAILURE(functionName, true == timer.CancelTimer(handle4));
   THROW_ON_FAILURE(functionName, false == timer.CancelTimer(handle1));

   timer.SetTickCountAndWait(1160, true);

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerNotExpired();
   handle3.CheckTimerExpired(3, s_delay);
   handle4.CheckTimerNotExpired();

   timer.SetTickCountAndWait(1201, false);

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerExpired(2, s_delay);
   handle3.CheckTimerNotExpired();
   handle4.CheckTimerNotExpired();

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerTest::TestTickCountWrap()
{
   const _tstring functionName = _T("CCallbackTimerTest::TestTickCountWrap");
   
   Output(functionName + _T(" - start"));

   const DWORD rollOver = 0;
   const DWORD justBeforeRollOver = rollOver - 1;
   const DWORD beforeRollOver = rollOver - 1000;

   CTestCallbackTimer timer(beforeRollOver, s_delay);

   CLoggingCallbackTimerHandle handle1;
   CLoggingCallbackTimerHandle handle2;
   CLoggingCallbackTimerHandle handle3;

   timer.SetTimerAndWait(handle1, 900, 1);
   timer.SetTimerAndWait(handle2, 1000, 2);
   timer.SetTimerAndWait(handle3, 1100, 3);

   // Prove that time is standing still

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerNotExpired();
   handle3.CheckTimerNotExpired();

   timer.SetTickCountAndWait(justBeforeRollOver, true);

   handle1.CheckTimerExpired(1, s_delay);
   handle2.CheckTimerNotExpired();
   handle3.CheckTimerNotExpired();

   timer.SetTickCountAndWait(rollOver, true);

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerExpired(2, s_delay);
   handle3.CheckTimerNotExpired();

   timer.SetTickCountAndWait(2000, false);

   handle1.CheckTimerNotExpired();
   handle2.CheckTimerNotExpired();
   handle3.CheckTimerExpired(3, s_delay);

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

