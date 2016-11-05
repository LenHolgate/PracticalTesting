///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueExTest.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "CallbackTimerQueueExTest.h"

#include "..\Mock\MockTickCount64Provider.h"
#include "..\Mock\LoggingCallbackTimer.h"

#include "JetByteTools\Win32Tools\Utils.h"
#include "JetByteTools\Win32Tools\DebugTrace.h"

#include "JetByteTools\TestTools\TestException.h"

#pragma hdrstop

#include "..\CallbackTimerQueueEx.h"

///////////////////////////////////////////////////////////////////////////////
// Requires Windows Vista or later due to use of GetTickCount64()
///////////////////////////////////////////////////////////////////////////////

#if (_WIN32_WINNT >= 0x0600) 

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;

using JetByteTools::Win32::Output;
using JetByteTools::Win32::_tstring;

using JetByteTools::Win32::Mock::CMockTickCount64Provider;
using JetByteTools::Win32::Mock::CLoggingCallbackTimer;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static CCallbackTimerQueueEx::Handle CreateAndSetTimer(
   CCallbackTimerQueueEx &timerQueue, 
   CCallbackTimerQueueEx::Timer &timer, 
   const Milliseconds timeout,
   const CCallbackTimerQueueEx::UserData userData);

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueExTest
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerQueueExTest::TestAll()
{
   TestConstruct();
   TestCreateTimer();
   TestDestroyTimer();
   TestTimer();
   TestCancelTimer();
   TestCancelExpiredTimer();
   TestResetTimer();
   TestGetMaxTimeout();
   TestSetTimerPastTickCount64CountWrap();
   TestMaxTimeout();
   TestMultipleTimers();
   TestOneShotTimer();
   TestActiveTimersAtDestructionTime();
}

void CCallbackTimerQueueExTest::TestConstruct()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestConstruct");
   
   Output(functionName + _T(" - start"));

   CCallbackTimerQueueEx timerQueue;

   CMockTickCount64Provider tickProvider;

   CCallbackTimerQueueEx timerQueue2(tickProvider);

   tickProvider.CheckNoResults();

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueExTest::TestCreateTimer()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestCreateTimer");
   
   Output(functionName + _T(" - start"));

   CMockTickCount64Provider tickProvider;

   CCallbackTimerQueueEx timerQueue(tickProvider);

   tickProvider.CheckNoResults();

   CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

   tickProvider.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, CCallbackTimerQueueEx::InvalidHandleValue != handle);

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueExTest::TestDestroyTimer()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestDestroyTimer");
   
   Output(functionName + _T(" - start"));

   CMockTickCount64Provider tickProvider;

   CCallbackTimerQueueEx timerQueue(tickProvider);

   tickProvider.CheckNoResults();

   CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

   tickProvider.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, CCallbackTimerQueueEx::InvalidHandleValue != handle);

   CCallbackTimerQueueEx::Handle handleCopy = handle;

   THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle));

   THROW_ON_FAILURE(functionName, CCallbackTimerQueueEx::InvalidHandleValue == handle);

   tickProvider.CheckResult(_T("|"));

   THROW_ON_NO_EXCEPTION_1(functionName, timerQueue.DestroyTimer, handleCopy);

   tickProvider.CheckResult(_T("|"));

   THROW_ON_NO_EXCEPTION_1(functionName, timerQueue.DestroyTimer, CCallbackTimerQueueEx::InvalidHandleValue);

   tickProvider.CheckResult(_T("|"));

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueExTest::TestTimer()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestTimer");
   
   Output(functionName + _T(" - start"));

   CMockTickCount64Provider tickProvider;

   CCallbackTimerQueueEx timerQueue(tickProvider);

   tickProvider.CheckNoResults();

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

   CLoggingCallbackTimer timer;

   CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

   tickProvider.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, CCallbackTimerQueueEx::InvalidHandleValue != handle);

   THROW_ON_FAILURE(functionName, false == timerQueue.SetTimer(handle, timer, 100, 1));

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

   THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle));

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueExTest::TestCancelTimer()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestCancelTimer");
   
   Output(functionName + _T(" - start"));

   CMockTickCount64Provider tickProvider;

   CCallbackTimerQueueEx timerQueue(tickProvider);

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   CLoggingCallbackTimer timer;

   CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

   tickProvider.CheckNoResults();

   THROW_ON_FAILURE(functionName, CCallbackTimerQueueEx::InvalidHandleValue != handle);

   THROW_ON_FAILURE(functionName, false == timerQueue.SetTimer(handle, timer, 100, 1));

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   THROW_ON_FAILURE(functionName, 100 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));
   
   THROW_ON_FAILURE(functionName, true == timerQueue.CancelTimer(handle));

   timer.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, false == timerQueue.CancelTimer(handle));

   CCallbackTimerQueueEx::Handle invalid = 0;

   THROW_ON_NO_EXCEPTION_1(functionName, timerQueue.CancelTimer, invalid);

   tickProvider.SetTickCount(100);

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|"));

   timer.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle));

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueExTest::TestCancelExpiredTimer()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestCancelExpiredTimer");
   
   Output(functionName + _T(" - start"));

   CMockTickCount64Provider tickProvider;

   CCallbackTimerQueueEx timerQueue(tickProvider);

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   CLoggingCallbackTimer timer;

   CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

   tickProvider.CheckNoResults();

   THROW_ON_FAILURE(functionName, CCallbackTimerQueueEx::InvalidHandleValue != handle);

   THROW_ON_FAILURE(functionName, false == timerQueue.SetTimer(handle, timer, 100, 1));

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

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

   THROW_ON_FAILURE(functionName, false == timerQueue.CancelTimer(handle));

   timer.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle));

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueExTest::TestGetMaxTimeout()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestMaxTimeout");
   
   Output(functionName + _T(" - start"));

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(tickProvider);

      tickProvider.CheckNoResults();
   
      CLoggingCallbackTimer timer;

      THROW_ON_FAILURE(functionName, 4294967294 == timerQueue.GetMaximumTimeout());

      tickProvider.CheckNoResults();

      // GetMaximumTimeout() doesn't reduce the value that it returns as the 64bit 
      // rollover approaches; which means you WILL get an exception if you try and
      // set a timer that is less than the value returned by GetMaximumTimeout() 
      // but which crosses the rollover point. I'll fix this if I get enough 
      // repeatable bug reports.

      const ULONGLONG rollOver = 0;
      const ULONGLONG beforeRollOver = rollOver - 1000;

      tickProvider.SetTickCount(beforeRollOver);

      THROW_ON_FAILURE(functionName, 4294967294 == timerQueue.GetMaximumTimeout());

      tickProvider.CheckNoResults();
   }

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueExTest::TestSetTimerPastTickCount64CountWrap()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestSetTimerPastTickCount64CountWrap");
   
   Output(functionName + _T(" - start"));

   const ULONGLONG rollOver = 0;
   const ULONGLONG beforeRollOver = rollOver - 1000;

   CMockTickCount64Provider tickProvider(beforeRollOver);

   CCallbackTimerQueueEx timerQueue(tickProvider);

   const _tstring resultBeforeRollOver = _T("|GetTickCount: ") + ToString(beforeRollOver) + _T("|");

   tickProvider.CheckNoResults();

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

   CLoggingCallbackTimer timer;

   CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

   tickProvider.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, CCallbackTimerQueueEx::InvalidHandleValue != handle);

   // Set a single timer for 100ms after the tick count rolls to 0.

   THROW_ON_NO_EXCEPTION_4(functionName, timerQueue.SetTimer, handle, timer, 1100, 1);

   tickProvider.CheckResult(resultBeforeRollOver);

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckNoResults();

   THROW_ON_FAILURE(functionName, false == timerQueue.CancelTimer(handle));

   // Set a single timer for 1ms before the tick count rolls to 0.

   THROW_ON_FAILURE(functionName, false == timerQueue.SetTimer(handle, timer, 999, 1));

   tickProvider.CheckResult(resultBeforeRollOver);

   THROW_ON_FAILURE(functionName, 999 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(resultBeforeRollOver);

   // Set the time to the point where the tick count rolls over.

   tickProvider.SetTickCount(rollOver);

   // The timer expired 1ms ago...

   THROW_ON_FAILURE(functionName, 0 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));      

   // Check that the time goes off.

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   timer.CheckResult(_T("|OnTimer: 1|"));

   // Prove that once we HAVE wrapped then all is well again for another zillion years...

   // Set a single timer for 1000ms time.

   THROW_ON_FAILURE(functionName, false == timerQueue.SetTimer(handle, timer, 1000, 1));

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   THROW_ON_FAILURE(functionName, 1000 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   // Set the tick count to the timeout time.

   tickProvider.SetTickCount(1000);

   THROW_ON_FAILURE(functionName, 0 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

   // Check that the timer goes off.

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

   timer.CheckResult(_T("|OnTimer: 1|"));

   // Check that there are no more timers to go off.

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle));

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueExTest::TestMaxTimeout()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestMaxTimeout");
   
   Output(functionName + _T(" - start"));

   CMockTickCount64Provider tickProvider(1000);

   {
      CCallbackTimerQueueEx timerQueue(tickProvider);

      tickProvider.CheckNoResults();
   
      CLoggingCallbackTimer timer;

      const Milliseconds maxTimeout = timerQueue.GetMaximumTimeout();

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE(functionName, CCallbackTimerQueueEx::InvalidHandleValue != handle);

      THROW_ON_FAILURE(functionName, false == timerQueue.SetTimer(handle, timer, maxTimeout, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

      const Milliseconds illegalTimeout = maxTimeout + 1;

      THROW_ON_NO_EXCEPTION_4(functionName, timerQueue.SetTimer, handle, timer, illegalTimeout, 1);

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE(functionName, true == timerQueue.CancelTimer(handle));

      THROW_ON_NO_EXCEPTION_4(functionName, timerQueue.SetTimer, handle, timer, illegalTimeout, 1);

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle));

      timer.CheckNoResults();
      tickProvider.CheckNoResults();
   }

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueExTest::TestMultipleTimers()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestMultipleTimers");
   
   Output(functionName + _T(" - start"));

   CMockTickCount64Provider tickProvider;

   CCallbackTimerQueueEx timerQueue(tickProvider);

   tickProvider.CheckNoResults();

   tickProvider.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

   CLoggingCallbackTimer timer1;
   CLoggingCallbackTimer timer2;
   CLoggingCallbackTimer timer3;
   CLoggingCallbackTimer timer4;
   CLoggingCallbackTimer timer5;
   CLoggingCallbackTimer timer6;

   CCallbackTimerQueueEx::Handle handle1 = CreateAndSetTimer(timerQueue, timer1, 300, 1);
   CCallbackTimerQueueEx::Handle handle2 = CreateAndSetTimer(timerQueue, timer2, 100, 2);
   CCallbackTimerQueueEx::Handle handle3 = CreateAndSetTimer(timerQueue, timer3, 200, 3);
   CCallbackTimerQueueEx::Handle handle4 = CreateAndSetTimer(timerQueue, timer4, 150, 4);
   CCallbackTimerQueueEx::Handle handle5 = CreateAndSetTimer(timerQueue, timer5, 150, 5);
   CCallbackTimerQueueEx::Handle handle6 = CreateAndSetTimer(timerQueue, timer6, 160, 6);

   tickProvider.CheckResult(
      _T("|GetTickCount: 0")
      _T("|GetTickCount: 0")
      _T("|GetTickCount: 0")
      _T("|GetTickCount: 0")
      _T("|GetTickCount: 0")
      _T("|GetTickCount: 0|"));

   THROW_ON_FAILURE(functionName, 100 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   timer1.CheckResult(_T("|"));
   timer2.CheckResult(_T("|"));
   timer3.CheckResult(_T("|"));
   timer4.CheckResult(_T("|"));
   timer5.CheckResult(_T("|"));
   timer6.CheckResult(_T("|"));

   tickProvider.SetTickCount(100);

   THROW_ON_FAILURE(functionName, 0 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 100|"));

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 100|GetTickCount: 100|"));

   timer1.CheckResult(_T("|"));
   timer2.CheckResult(_T("|OnTimer: 2|"));
   timer3.CheckResult(_T("|"));
   timer4.CheckResult(_T("|"));
   timer5.CheckResult(_T("|"));
   timer6.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, 50 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 100|"));

   tickProvider.SetTickCount(155);

   THROW_ON_FAILURE(functionName, 0 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 155|"));

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 155|GetTickCount: 155|GetTickCount: 155|"));

   timer1.CheckResult(_T("|"));
   timer2.CheckResult(_T("|"));
   timer3.CheckResult(_T("|"));
   timer4.CheckResult(_T("|OnTimer: 4|"));
   timer5.CheckResult(_T("|OnTimer: 5|"));
   timer6.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, 5 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 155|"));

   tickProvider.SetTickCount(201);

   THROW_ON_FAILURE(functionName, 0 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 201|"));

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 201|GetTickCount: 201|GetTickCount: 201|"));

   timer1.CheckResult(_T("|"));
   timer2.CheckResult(_T("|"));
   timer3.CheckResult(_T("|OnTimer: 3|"));
   timer4.CheckResult(_T("|"));
   timer5.CheckResult(_T("|"));
   timer6.CheckResult(_T("|OnTimer: 6|"));

   THROW_ON_FAILURE(functionName, 99 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 201|"));

   tickProvider.SetTickCount(300);

   THROW_ON_FAILURE(functionName, 0 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 300|"));

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 300|"));

   timer1.CheckResult(_T("|OnTimer: 1|"));
   timer2.CheckResult(_T("|"));
   timer3.CheckResult(_T("|"));
   timer4.CheckResult(_T("|"));
   timer5.CheckResult(_T("|"));
   timer6.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

//   timerQueue.SetTimer(timer1, 100, 1);
//   timerQueue.SetTimer(timer2, 200, 2);

   THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle1));
   THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle2));
   THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle3));
   THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle4));
   THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle5));
   THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle6));

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueExTest::TestResetTimer()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestResetTimer");
   
   Output(functionName + _T(" - start"));

   CMockTickCount64Provider tickProvider;

   CCallbackTimerQueueEx timerQueue(tickProvider);

   CLoggingCallbackTimer timer;

   CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

   tickProvider.CheckNoResults();

   THROW_ON_FAILURE(functionName, CCallbackTimerQueueEx::InvalidHandleValue != handle);

   THROW_ON_FAILURE(functionName, false == timerQueue.SetTimer(handle, timer, 100, 1));

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   THROW_ON_FAILURE(functionName, 100 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));
   
   THROW_ON_FAILURE(functionName, true == timerQueue.SetTimer(handle, timer, 90, 2));

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   THROW_ON_FAILURE(functionName, 90 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 0|"));

   tickProvider.SetTickCount(90);

   THROW_ON_FAILURE(functionName, 0 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 90|"));

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 90|"));

   timer.CheckResult(_T("|OnTimer: 2|"));

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckNoResults();

   THROW_ON_FAILURE(functionName, false == timerQueue.SetTimer(handle, timer, 110, 3));

   tickProvider.CheckResult(_T("|GetTickCount: 90|"));

   THROW_ON_FAILURE(functionName, 110 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 90|"));

   tickProvider.SetTickCount(200);

   THROW_ON_FAILURE(functionName, 0 == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|GetTickCount: 200|"));

   timerQueue.HandleTimeouts();

   tickProvider.CheckResult(_T("|GetTickCount: 200|"));

   timer.CheckResult(_T("|OnTimer: 3|"));

   THROW_ON_FAILURE(functionName, INFINITE == timerQueue.GetNextTimeout());

   tickProvider.CheckResult(_T("|"));

   THROW_ON_FAILURE(functionName, false == timerQueue.DestroyTimer(handle));

   Output(functionName + _T(" - stop"));
}

void CCallbackTimerQueueExTest::TestOneShotTimer()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestOneShotTimer");
   
   Output(functionName + _T(" - start"));

   CMockTickCount64Provider tickProvider;

   CCallbackTimerQueueEx timerQueue(tickProvider);

   tickProvider.CheckNoResults();

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

void CCallbackTimerQueueExTest::TestActiveTimersAtDestructionTime()
{
   const _tstring functionName = _T("CCallbackTimerQueueExTest::TestActiveTimersAtDestructionTime");
   
   Output(functionName + _T(" - start"));

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(tickProvider);

      tickProvider.CheckNoResults();
   
      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle1 = timerQueue.CreateTimer();

      THROW_ON_FAILURE(functionName, CCallbackTimerQueueEx::InvalidHandleValue != handle1);

      THROW_ON_FAILURE(functionName, false == timerQueue.SetTimer(handle1, timer, 100, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      CCallbackTimerQueueEx::Handle handle2 = timerQueue.CreateTimer();

      THROW_ON_FAILURE(functionName, CCallbackTimerQueueEx::InvalidHandleValue != handle2);

      timerQueue.SetTimer(timer, 250, 1);

      timerQueue.SetTimer(timer, 2500, 1);

      timerQueue.HandleTimeouts();

      timer.CheckResult(_T("|"));
   }

   Output(functionName + _T(" - stop"));
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static CCallbackTimerQueueEx::Handle CreateAndSetTimer(
   CCallbackTimerQueueEx &timerQueue, 
   CCallbackTimerQueueEx::Timer &timer, 
   const Milliseconds timeout,
   const CCallbackTimerQueueEx::UserData userData)
{
   const _tstring functionName = _T("CreateAndSetTimer");

   CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

   THROW_ON_FAILURE(functionName, CCallbackTimerQueueEx::InvalidHandleValue != handle);

   THROW_ON_FAILURE(functionName, false == timerQueue.SetTimer(handle, timer, timeout, userData));

   return handle;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// Requires Windows Vista or later due to use of GetTickCount64()
///////////////////////////////////////////////////////////////////////////////

#endif // (_WIN32_WINNT >= 0x0600)

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueueExTest.cpp
///////////////////////////////////////////////////////////////////////////////

