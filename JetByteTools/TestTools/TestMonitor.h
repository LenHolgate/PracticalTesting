#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: TestMonitor.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2008 JetByte Limited.
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
#include "JetByteTools/CoreTools/tstring.h"
#include "JetByteTools/CoreTools/Thread.h"
#include "JetByteTools/CoreTools/IRunnable.h"
#include "JetByteTools/CoreTools/AutoResetEvent.h"
#include "JetByteTools/CoreTools/ManualResetEvent.h"
#include "JetByteTools/CoreTools/PerformanceCounter.h"

#include <list>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CTestMonitor
///////////////////////////////////////////////////////////////////////////////

class CTestMonitor : private JetByteTools::Core::IRunnable
{
   public :

      explicit CTestMonitor(
         const JetByteTools::Core::_tstring &name,
         bool includePerformanceTests = true,
         bool stopOnFailure = false,
         bool debugOnFailure = true);

      CTestMonitor();

      CTestMonitor(
         const CTestMonitor &rhs) = delete;

      ~CTestMonitor() override;

      CTestMonitor &operator=(
         const CTestMonitor &rhs) = delete;

      static bool DebugOnFailure();

      static void Trace(
         const JetByteTools::Core::_tstring &message);

      void StartTest(
         const JetByteTools::Core::_tstring &className,
         const JetByteTools::Core::_tstring &functionName,
         JetByteTools::Milliseconds timeout = 0);

      bool StartPerformanceTest(
         const JetByteTools::Core::_tstring &className,
         const JetByteTools::Core::_tstring &functionName,
         Milliseconds timeout = 0);

      void TestComplete();

      void TestException();

      void SkipTest(
         const JetByteTools::Core::_tstring &reason);

      void FailTest(
         const JetByteTools::Core::_tstring &reason);

      bool Report(
         size_t expectedTests,
         bool failIfTestsSkipped = false) const;

      void HandlePureCall();

   private :

      // Implement IRunnable

      unsigned int Run() override;

      void OutputTestDetails() const;

      static Milliseconds GetTimeoutForMachine();

      JetByteTools::Core::CThread m_testTimeoutThread;

      JetByteTools::Core::CManualResetEvent m_shutdownEvent;
      JetByteTools::Core::CAutoResetEvent m_startTimingEvent;
      JetByteTools::Core::CAutoResetEvent m_timingStartedEvent;
      JetByteTools::Core::CAutoResetEvent m_stopTimingEvent;

      JetByteTools::Core::CPerformanceCounter m_totalTimer;
      JetByteTools::Core::CPerformanceCounter m_testTimer;

      typedef void (PureCallHandlerFnc)();

      PureCallHandlerFnc *m_pPreviousHandler;

      const JetByteTools::Core::_tstring m_name;

      const bool m_includePerformanceTests;

      const bool m_stopOnFailure;

      const bool m_debugOnFailure;

      class TestDetails;

      typedef std::list<TestDetails *> Tests;

      Tests m_tests;

      TestDetails *m_pActiveTest;

      mutable bool m_reported;

      Milliseconds m_testTimeout;

      typedef std::list<JetByteTools::Core::_tstring> TraceMessages;

      TraceMessages m_traceMessages;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: TestMonitor.h
///////////////////////////////////////////////////////////////////////////////
