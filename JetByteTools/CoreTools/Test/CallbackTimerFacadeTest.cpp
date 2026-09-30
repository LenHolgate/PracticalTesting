///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerFacadeTest.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2026 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#include "JetByteTools/Admin/Admin.h"

#include "CallbackTimerFacadeTest.h"

#include "JetByteTools/CoreTools/Mock/MockTimerQueue.h"
#include "JetByteTools/CoreTools/Mock/TestTimerQueueFacade.h"
#include "JetByteTools/CoreTools/Mock/LoggingCallbackTimer.h"

#include "JetByteTools/CoreTools/ToString.h"

#include "JetByteTools/TestTools/TestException.h"
#include "JetByteTools/TestTools/RunTest.h"
#include "JetByteTools/TestTools/TestLog.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;
using JetByteTools::Test::CTestMonitor;
using JetByteTools::Test::CTestLog;

using JetByteTools::Core::Mock::CMockTimerQueue;
using JetByteTools::Core::Mock::CTestTimerQueueFacade;
using JetByteTools::Core::Mock::CLoggingCallbackTimer;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerFacadeTest
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerFacadeTest::TestAll(
   CTestMonitor &monitor)
{
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestConstruct);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestHandleTimeoutsNoTimeouts);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestHandleTimeouts);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestBeginShutdownNoTimeouts);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestWaitForShutdownToCompleteNoTimeouts);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestWaitForShutdownToCompleteAfterBeginShutdownNoTimeouts);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestBeginShutdownTimersHandledInBeginShutdown);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestWaitForShutdownToCompleteTimersHandledInBeginShutdown);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestWaitForShutdownToCompleteAfterBeginShutdownTimersHandledInBeginShutdown);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestBeginShutdownTimersHandledInWaitForShutdownToComplete);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestWaitForShutdownToCompleteTimersHandledInWaitForShutdownToComplete);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestWaitForShutdownToCompleteAfterBeginShutdownTimersHandledInWaitForShutdownToComplete);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestWaitForShutdownDuringDestructionNoTimeouts);
   RUN_TEST_EX(monitor, CCallbackTimerFacadeTest, TestWaitForShutdownDuringDestruction);
}

void CCallbackTimerFacadeTest::TestConstruct()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      log.CheckNoResults();

      {
         CTestTimerQueueFacade facade(timerQueue);

         log.CheckNoResults();
      }

      log.CheckResult(_T("|[~CTestTimerQueueFacade]|"));
   }

   log.CheckNoResults();
}

void CCallbackTimerFacadeTest::TestHandleTimeoutsNoTimeouts()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      {
         CTestTimerQueueFacade facade(timerQueue);

         facade.HandleTimeouts();

         log.CheckResult(_T("|[HandleTimeouts]|[BeginTimeoutHandling]|BeginTimeoutHandling|"));
      }

      log.CheckResult(_T("|[~CTestTimerQueueFacade]|"));
   }
}

void CCallbackTimerFacadeTest::TestHandleTimeouts()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      CLoggingCallbackTimer timer(log);

      {
         CTestTimerQueueFacade facade(timerQueue);

         facade.SetTimer(timer, 100, 0);

         log.CheckResult(_T("|[SetTimer]|SetTimer: 100|"));

         facade.HandleTimeouts();

         log.CheckResult(
            _T("|[HandleTimeouts]")                // Facade
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling")            // TimerQueue
            _T("|[HandleTimeouts]")                // Facade         Does not lock over timer handling
            _T("|HandleTimeouts")                  // TimerQueue
            _T("|OnTimer: 0")                      // Timer
            _T("|[EndTimeoutHandling]")            // Facade         Can lock before calling into queue...
            _T("|EndTimeoutHandling")              // TimerQueue
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling|"));         // TimerQueue
      }

      log.CheckResult(_T("|[~CTestTimerQueueFacade]|"));
   }
}

void CCallbackTimerFacadeTest::TestBeginShutdownNoTimeouts()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      {
         CTestTimerQueueFacade facade(timerQueue);

         facade.BeginShutdown();

         log.CheckResult(
            _T("|[BeginShutdown]")                 // Facade         Does not lock before calling into queue...
            _T("|BeginShutdown")                   // TimerQueue
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling|"));         // TimerQueue
      }

      log.CheckResult(_T("|[~CTestTimerQueueFacade]|"));
   }
}

void CCallbackTimerFacadeTest::TestWaitForShutdownToCompleteNoTimeouts()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      {
         CTestTimerQueueFacade facade(timerQueue);

         facade.WaitForShutdownToComplete();

         log.CheckResult(
            _T("|[WaitForShutdownToComplete]")     // Facade         Does not lock before calling into queue..
            _T("|[BeginShutdown]")                 // Facade         Does not lock before calling into queue...
            _T("|BeginShutdown")                   // TimerQueue
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling")            // Facade         Can lock before calling into queue...
            _T("|WaitForShutdownToComplete")       // TimerQueue
            _T("|[EndTimeoutHandling]")            // Facade         Can lock before calling into queue...
            _T("|EndTimeoutHandling|"));           // TimerQueue
      }

      log.CheckResult(_T("|[~CTestTimerQueueFacade]|"));
   }
}


void CCallbackTimerFacadeTest::TestWaitForShutdownToCompleteAfterBeginShutdownNoTimeouts()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      {
         CTestTimerQueueFacade facade(timerQueue);

         facade.BeginShutdown();

         log.CheckResult(
            _T("|[BeginShutdown]")                 // Facade         Does not lock before calling into queue...
            _T("|BeginShutdown")                   // TimerQueue
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling|"));         // TimerQueue

         facade.WaitForShutdownToComplete();

         log.CheckResult(
            _T("|[WaitForShutdownToComplete]")     // Facade         Does not lock before calling into queue...
            _T("|WaitForShutdownToComplete")       // TimerQueue
            _T("|[EndTimeoutHandling]")            // Facade         Can lock before calling into queue...
            _T("|EndTimeoutHandling|"));           // TimerQueue
      }

      log.CheckResult(_T("|[~CTestTimerQueueFacade]|"));
   }
}

void CCallbackTimerFacadeTest::TestBeginShutdownTimersHandledInBeginShutdown()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      timerQueue.shutdownTimersHandledIn = CMockTimerQueue::ShutdownTimersHandledIn::BeginShutdown;

      CLoggingCallbackTimer timer(log);

      timer.supportsTimersFiringDuringShutdown = true;

      {
         CTestTimerQueueFacade facade(timerQueue);

         facade.SetTimer(timer, 100, 0);

         log.CheckResult(_T("|[SetTimer]|SetTimer: 100|"));

         facade.BeginShutdown();

         log.CheckResult(
            _T("|[BeginShutdown]")                 // Facade         Does not lock before calling into queue...
            _T("|BeginShutdown")                   // TimerQueue
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling")            // TimerQueue
            _T("|HandleTimeouts")                  // TimerQueue
            _T("|OnTimer: 0 [Shutdown]|"));        // Timer
      }

      log.CheckResult(_T("|[~CTestTimerQueueFacade]|"));
   }
}

void CCallbackTimerFacadeTest::TestWaitForShutdownToCompleteTimersHandledInBeginShutdown()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      timerQueue.shutdownTimersHandledIn = CMockTimerQueue::ShutdownTimersHandledIn::BeginShutdown;

      CLoggingCallbackTimer timer(log);

      timer.supportsTimersFiringDuringShutdown = true;

      {
         CTestTimerQueueFacade facade(timerQueue);

         facade.SetTimer(timer, 100, 0);

         log.CheckResult(_T("|[SetTimer]|SetTimer: 100|"));

         facade.WaitForShutdownToComplete();

         log.CheckResult(
            _T("|[WaitForShutdownToComplete]")     // Facade         Does not lock before calling into queue...
            _T("|[BeginShutdown]")                 // Facade         Does not lock before calling into queue...
            _T("|BeginShutdown")                   // TimerQueue
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling")            // TimerQueue
            _T("|HandleTimeouts")                  // TimerQueue
            _T("|OnTimer: 0 [Shutdown]")           // Timer
            _T("|WaitForShutdownToComplete")       // TimerQueue
            _T("|[EndTimeoutHandling]")            // Facade         Can lock before calling into queue...
            _T("|EndTimeoutHandling|"));           // TimerQueue
      }

      log.CheckResult(_T("|[~CTestTimerQueueFacade]|"));
   }
}

void CCallbackTimerFacadeTest::TestWaitForShutdownToCompleteAfterBeginShutdownTimersHandledInBeginShutdown()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      timerQueue.shutdownTimersHandledIn = CMockTimerQueue::ShutdownTimersHandledIn::BeginShutdown;

      CLoggingCallbackTimer timer(log);

      timer.supportsTimersFiringDuringShutdown = true;

      {
         CTestTimerQueueFacade facade(timerQueue);

         facade.SetTimer(timer, 100, 0);

         log.CheckResult(_T("|[SetTimer]|SetTimer: 100|"));

         facade.BeginShutdown();

         log.CheckResult(
            _T("|[BeginShutdown]")                 // Facade         Does not lock before calling into queue...
            _T("|BeginShutdown")                   // TimerQueue
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling")            // TimerQueue
            _T("|HandleTimeouts")                  // TimerQueue
            _T("|OnTimer: 0 [Shutdown]|"));        // Timer

         facade.WaitForShutdownToComplete();

         log.CheckResult(
            _T("|[WaitForShutdownToComplete]")     // Facade         Does not lock before calling into queue...
            _T("|WaitForShutdownToComplete")       // TimerQueue
            _T("|[EndTimeoutHandling]")            // Facade         Can lock before calling into queue...
            _T("|EndTimeoutHandling|"));           // TimerQueue
      }

      log.CheckResult(_T("|[~CTestTimerQueueFacade]|"));
   }
}

void CCallbackTimerFacadeTest::TestBeginShutdownTimersHandledInWaitForShutdownToComplete()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      timerQueue.shutdownTimersHandledIn = CMockTimerQueue::ShutdownTimersHandledIn::WaitForShutdownToComplete;

      CLoggingCallbackTimer timer(log);

      timer.supportsTimersFiringDuringShutdown = true;

      {
         CTestTimerQueueFacade facade(timerQueue);

         facade.SetTimer(timer, 100, 0);

         log.CheckResult(_T("|[SetTimer]|SetTimer: 100|"));

         facade.BeginShutdown();

         log.CheckResult(
            _T("|[BeginShutdown]")                 // Facade         Does not lock before calling into queue...
            _T("|BeginShutdown")                   // TimerQueue
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling|"));         // TimerQueue
      }

      log.CheckResult(_T("|[~CTestTimerQueueFacade]|"));
   }
}

void CCallbackTimerFacadeTest::TestWaitForShutdownToCompleteTimersHandledInWaitForShutdownToComplete()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      timerQueue.shutdownTimersHandledIn = CMockTimerQueue::ShutdownTimersHandledIn::WaitForShutdownToComplete;

      CLoggingCallbackTimer timer(log);

      timer.supportsTimersFiringDuringShutdown = true;

      {
         CTestTimerQueueFacade facade(timerQueue);

         facade.SetTimer(timer, 100, 0);

         log.CheckResult(_T("|[SetTimer]|SetTimer: 100|"));

         facade.WaitForShutdownToComplete();

         log.CheckResult(
            _T("|[WaitForShutdownToComplete]")     // Facade         Does not lock before calling into queue...
            _T("|[BeginShutdown]")                 // Facade         Does not lock before calling into queue...
            _T("|BeginShutdown")                   // TimerQueue
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling")            // TimerQueue
            _T("|WaitForShutdownToComplete")       // TimerQueue
            _T("|HandleTimeouts")                  // TimerQueue
            _T("|OnTimer: 0 [Shutdown]")           // Timer
            _T("|[EndTimeoutHandling]")            // Facade         Can lock before calling into queue...
            _T("|EndTimeoutHandling|"));           // TimerQueue
      }

      log.CheckResult(_T("|[~CTestTimerQueueFacade]|"));
   }
}

void CCallbackTimerFacadeTest::TestWaitForShutdownToCompleteAfterBeginShutdownTimersHandledInWaitForShutdownToComplete()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      timerQueue.shutdownTimersHandledIn = CMockTimerQueue::ShutdownTimersHandledIn::WaitForShutdownToComplete;

      CLoggingCallbackTimer timer(log);

      timer.supportsTimersFiringDuringShutdown = true;

      {
         CTestTimerQueueFacade facade(timerQueue);

         facade.SetTimer(timer, 100, 0);

         log.CheckResult(_T("|[SetTimer]|SetTimer: 100|"));

         facade.BeginShutdown();

         log.CheckResult(
            _T("|[BeginShutdown]")                 // Facade         Does not lock before calling into queue...
            _T("|BeginShutdown")                   // TimerQueue
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling|"));         // TimerQueue

         facade.WaitForShutdownToComplete();

         log.CheckResult(
            _T("|[WaitForShutdownToComplete]")     // Facade         Does not lock before calling into queue...
            _T("|WaitForShutdownToComplete")       // TimerQueue
            _T("|HandleTimeouts")                  // TimerQueue
            _T("|OnTimer: 0 [Shutdown]")           // Timer
            _T("|[EndTimeoutHandling]")            // Facade         Can lock before calling into queue...
            _T("|EndTimeoutHandling|"));           // TimerQueue
      }

      log.CheckResult(_T("|[~CTestTimerQueueFacade]|"));
   }
}

void CCallbackTimerFacadeTest::TestWaitForShutdownDuringDestructionNoTimeouts()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      log.CheckNoResults();

      {
         CTestTimerQueueFacade facade(timerQueue);

         facade.waitForShutdownDuringDestruction = true;

         log.CheckNoResults();
      }

      log.CheckResult(
            _T("|[~CTestTimerQueueFacade]")        // Facade
            _T("|[WaitForShutdownToComplete]")     // Facade         Does not lock before calling into queue..
            _T("|[BeginShutdown]")                 // Facade         Does not lock before calling into queue...
            _T("|BeginShutdown")                   // TimerQueue
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling")            // Facade         Can lock before calling into queue...
            _T("|WaitForShutdownToComplete")       // TimerQueue
            _T("|[EndTimeoutHandling]")            // Facade         Can lock before calling into queue...
            _T("|EndTimeoutHandling|"));           // TimerQueue
   }

   log.CheckNoResults();
}

void CCallbackTimerFacadeTest::TestWaitForShutdownDuringDestruction()
{
   CTestLog log;

   {
      CMockTimerQueue timerQueue(&log);

      timerQueue.shutdownTimersHandledIn = CMockTimerQueue::ShutdownTimersHandledIn::BeginShutdown;

      log.CheckNoResults();

      CLoggingCallbackTimer timer(log);

      timer.supportsTimersFiringDuringShutdown = true;

      {
         CTestTimerQueueFacade facade(timerQueue);

         log.CheckNoResults();

         facade.waitForShutdownDuringDestruction = true;

         facade.SetTimer(timer, 100, 0);

         log.CheckResult(_T("|[SetTimer]|SetTimer: 100|"));
      }

      log.CheckResult(
            _T("|[~CTestTimerQueueFacade]")        // Facade
            _T("|[WaitForShutdownToComplete]")     // Facade         Does not lock before calling into queue..
            _T("|[BeginShutdown]")                 // Facade         Does not lock before calling into queue...
            _T("|BeginShutdown")                   // TimerQueue
            _T("|[BeginTimeoutHandling]")          // Facade         Can lock before calling into queue...
            _T("|BeginTimeoutHandling")            // Facade         Can lock before calling into queue...
            _T("|HandleTimeouts")                  // TimerQueue
            _T("|OnTimer: 0 [Shutdown]")           // Timer
            _T("|WaitForShutdownToComplete")       // TimerQueue
            _T("|[EndTimeoutHandling]")            // Facade         Can lock before calling into queue...
            _T("|EndTimeoutHandling|"));           // TimerQueue
   }

   log.CheckNoResults();
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CCallbackTimerFacadeTest.cpp
///////////////////////////////////////////////////////////////////////////////

