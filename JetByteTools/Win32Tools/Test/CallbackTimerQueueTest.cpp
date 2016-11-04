///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueTest.cpp
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

#include "CallbackTimerQueueTest.h"

#include "..\CallbackTimerQueue.h"

#include "..\Mock\MockTickCountProvider.h"
#include "..\Mock\LoggingCallbackTimer.h"

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

using JetByteTools::Win32::Mock::CMockTickCountProvider;
using JetByteTools::Win32::Mock::CLoggingCallbackTimer;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueTest
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerQueueTest::TestAll()
{
   TestConstruct();
   TestTimer();
//   TestMultipleTimers();
   TestCancelTimer();
   TestTickCountWrap();
}

void CCallbackTimerQueueTest::TestConstruct()
{
   const _tstring functionName = _T("CCallbackTimerQueueTest::TestConstruct");
   
   Output(functionName + _T(" - start"));

   CCallbackTimerQueue timerQueue;

   CMockTickCountProvider tickProvider;

   CCallbackTimerQueue timerQueue2(tickProvider);

   tickProvider.CheckResult(_T("|"));

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueTest::TestTimer()
{
   const _tstring functionName = _T("CCallbackTimerQueueTest::TestTimer");
   
   Output(functionName + _T(" - start"));

   CMockTickCountProvider tickProvider;

   CCallbackTimerQueue timerQueue(tickProvider);

   tickProvider.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

   CLoggingCallbackTimer timer;

   timerQueue.SetTimer(timer, 100, 1);

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   THROW_ON_FAILURE(functionName, 100 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));
   
   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   timer.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, 100 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   tickProvider.SetTickCount(100);

   THROW_ON_FAILURE(functionName, 0 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 100|"));

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 100|"));

   timer.CheckResult(_T("|OnTimer: 1|"));

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueTest::TestCancelTimer()
{
   const _tstring functionName = _T("CCallbackTimerQueueTest::TestCancelTimer");
   
   Output(functionName + _T(" - start"));

   CMockTickCountProvider tickProvider;

   CCallbackTimerQueue timerQueue(tickProvider);

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   CLoggingCallbackTimer timer;

   CCallbackTimerQueue::Handle handle = timerQueue.SetTimer(timer, 100, 1);

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   THROW_ON_FAILURE(functionName, 100 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));
   
   THROW_ON_FAILURE(functionName, true == timerQueue.CancelTimer(handle));

   timer.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, false == timerQueue.CancelTimer(handle));
   THROW_ON_FAILURE(functionName, false == timerQueue.CancelTimer(0));

   tickProvider.SetTickCount(100);

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|"));

   timer.CheckResult(_T("|"));

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueTest::TestTickCountWrap()
{
   const _tstring functionName = _T("CCallbackTimerQueueTest::TestTickCountWrap");
   
   Output(functionName + _T(" - start"));

   const DWORD rollOver = 0;
   const DWORD beforeRollOver = rollOver - 1000;

   CMockTickCountProvider tickProvider(beforeRollOver);

   CCallbackTimerQueue timerQueue(tickProvider);

   tickProvider.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

   CLoggingCallbackTimer timer;

   timerQueue.SetTimer(timer, 1100, 1);

   const _tstring resultBeforeRollOver = _T("|GetTickCount: ") + ToString(beforeRollOver) + _T("|");

   tickProvider.CheckResult(resultBeforeRollOver);

   THROW_ON_FAILURE(functionName, 1100 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(resultBeforeRollOver);
   
   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(resultBeforeRollOver);

   timer.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, 1100 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(resultBeforeRollOver);

   tickProvider.SetTickCount(rollOver);

   THROW_ON_FAILURE(functionName, 100 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   timer.CheckResult(_T("|"));

   tickProvider.SetTickCount(100);

   THROW_ON_FAILURE(functionName, 0 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 100|"));

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 100|"));

   timer.CheckResult(_T("|OnTimer: 1|"));

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

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
// End of file: CallbackTimerQueueTest.cpp
///////////////////////////////////////////////////////////////////////////////

