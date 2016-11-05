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
#include "..\Mock\MockTimerQueueMonitor.h"

#include "JetByteTools\Win32Tools\Utils.h"

#include "JetByteTools\TestTools\TestException.h"
#include "JetByteTools\TestTools\RunTest.h"

#pragma hdrstop

#include "..\CallbackTimerQueueEx.h"

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;
using JetByteTools::Test::CTestMonitor;

using JetByteTools::Win32::Mock::CMockTickCount64Provider;
using JetByteTools::Win32::Mock::CLoggingCallbackTimer;
using JetByteTools::Win32::Mock::CMockTimerQueueMonitor;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

const _tstring CCallbackTimerQueueExTestTraits::shortName = _T("QEx - ");

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueExTest
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerQueueExTest::TestAll(
   CTestMonitor &monitor)
{
   Base::TestAll(monitor, _T("CCallbackTimerQueueEx"));

   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestGetMaxTimeout);
   RUN_TEST_EX(monitor, CCallbackTimerQueueExTest, TestSetTimerPastTickCount64CountWrap);
}

void CCallbackTimerQueueExTest::TestGetMaxTimeout()
{
   CMockTimerQueueMonitor monitor;

   CMockTickCount64Provider tickProvider;

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      tickProvider.CheckNoResults();
   
      CLoggingCallbackTimer timer;

      THROW_ON_FAILURE_EX(4294967294 == timerQueue.GetMaximumTimeout());

      tickProvider.CheckNoResults();

      // GetMaximumTimeout() doesn't reduce the value that it returns as the 64bit 
      // rollover approaches; which means you WILL get an exception if you try and
      // set a timer that is less than the value returned by GetMaximumTimeout() 
      // but which crosses the rollover point. I'll fix this if I get enough 
      // repeatable bug reports.

      const ULONGLONG rollOver = 0;
      const ULONGLONG beforeRollOver = rollOver - 1000;

      tickProvider.SetTickCount(beforeRollOver);

      THROW_ON_FAILURE_EX(4294967294 == timerQueue.GetMaximumTimeout());

      tickProvider.CheckNoResults();
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

void CCallbackTimerQueueExTest::TestSetTimerPastTickCount64CountWrap()
{
   CMockTimerQueueMonitor monitor;

   const ULONGLONG rollOver = 0;
   const ULONGLONG beforeRollOver = rollOver - 1000;

   CMockTickCount64Provider tickProvider(beforeRollOver);

   {
      CCallbackTimerQueueEx timerQueue(monitor, tickProvider);

      const _tstring resultBeforeRollOver = _T("|GetTickCount: ") + ToString(beforeRollOver) + _T("|");

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      CLoggingCallbackTimer timer;

      CCallbackTimerQueueEx::Handle handle = timerQueue.CreateTimer();

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(CCallbackTimerQueueEx::InvalidHandleValue != handle);

      // Set a single timer for 100ms after the tick count rolls to 0.

      THROW_ON_NO_EXCEPTION_EX_4(timerQueue.SetTimer, handle, timer, 1100, 1);

      tickProvider.CheckResult(resultBeforeRollOver);

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      tickProvider.CheckNoResults();

      THROW_ON_FAILURE_EX(false == timerQueue.CancelTimer(handle));

      // Set a single timer for 1ms before the tick count rolls to 0.

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 999, 1));

      tickProvider.CheckResult(resultBeforeRollOver);

      THROW_ON_FAILURE_EX(999 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(resultBeforeRollOver);

      // Set the time to the point where the tick count rolls over.

      tickProvider.SetTickCount(rollOver);

      // The timer expired 1ms ago...

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));      

      // Check that the time goes off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      timer.CheckResult(_T("|OnTimer: 1|"));

      // Prove that once we HAVE wrapped then all is well again for another zillion years...

      // Set a single timer for 1000ms time.

      THROW_ON_FAILURE_EX(false == timerQueue.SetTimer(handle, timer, 1000, 1));

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      THROW_ON_FAILURE_EX(1000 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 0|"));

      // Set the tick count to the timeout time.

      tickProvider.SetTickCount(1000);

      THROW_ON_FAILURE_EX(0 == timerQueue.GetNextTimeout());

      tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

      // Check that the timer goes off.

      timerQueue.HandleTimeouts();

      tickProvider.CheckResult(_T("|GetTickCount: 1000|"));

      timer.CheckResult(_T("|OnTimer: 1|"));

      // Check that there are no more timers to go off.

      THROW_ON_FAILURE_EX(INFINITE == timerQueue.GetNextTimeout());

      THROW_ON_FAILURE_EX(false == timerQueue.DestroyTimer(handle));
   }

   THROW_ON_FAILURE_EX(true == monitor.NoTimersAreActive());   // If monitoring is enabled, make sure all timers have been cleaned up
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueueTest.cpp
///////////////////////////////////////////////////////////////////////////////

