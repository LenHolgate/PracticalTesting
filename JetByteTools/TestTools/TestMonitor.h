#pragma once
#ifndef JETBYTE_TOOLS_TEST_TEST_MONITOR_INCLUDED__
#define JETBYTE_TOOLS_TEST_TEST_MONITOR_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: TestMonitor.h
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

#include "JetByteTools\Win32Tools\tstring.h"
#include "JetByteTools\Win32Tools\Thread.h"
#include "JetByteTools\Win32Tools\IRunnable.h"
#include "JetByteTools\Win32Tools\AutoResetEvent.h"
#include "JetByteTools\Win32Tools\ManualResetEvent.h"
#include "JetByteTools\Win32Tools\PerformanceCounter.h"

#include <list>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CTestMonitor
///////////////////////////////////////////////////////////////////////////////

class CTestMonitor : private JetByteTools::Win32::IRunnable
{
   public :

      CTestMonitor(
         const JetByteTools::Win32::_tstring &name,
         const bool includePerformanceTests = true,
         const bool stopOnFailure = false,
         const bool debugOnFailure = true);

      CTestMonitor();

      ~CTestMonitor();

      static bool DebugOnFailure();

      static void Trace(
         const JetByteTools::Win32::_tstring &message);

      void StartTest(
         const JetByteTools::Win32::_tstring &className,
         const JetByteTools::Win32::_tstring &functionName,
         const JetByteTools::Milliseconds timeout = 0);

      bool StartPerformanceTest(
         const JetByteTools::Win32::_tstring &className,
         const JetByteTools::Win32::_tstring &functionName,
         const JetByteTools::Milliseconds timeout = 0);

      void TestComplete();

      void TestException();

      void SkipTest(
         const JetByteTools::Win32::_tstring &reason);

      void FailTest(
         const JetByteTools::Win32::_tstring &reason);

      bool Report(
         const size_t expectedTests) const;

      void HandlePureCall();

   private :

      // Implement IRunnable

      unsigned int Run() override;

      void OutputTestDetails() const;

      static JetByteTools::Milliseconds GetTimeoutForMachine();

      JetByteTools::Win32::CThread m_testTimeoutThread;

      JetByteTools::Win32::CManualResetEvent m_shutdownEvent;
      JetByteTools::Win32::CAutoResetEvent m_startTimingEvent;
      JetByteTools::Win32::CAutoResetEvent m_timingStartedEvent;
      JetByteTools::Win32::CAutoResetEvent m_stopTimingEvent;

      JetByteTools::Win32::CPerformanceCounter m_totalTimer;
      JetByteTools::Win32::CPerformanceCounter m_testTimer;

      typedef void (PureCallHandlerFnc)();

      PureCallHandlerFnc *m_pPreviousHandler;

      const JetByteTools::Win32::_tstring m_name;

      const bool m_includePerformanceTests;

      const bool m_stopOnFailure;

      const bool m_debugOnFailure;

      class TestDetails;

      typedef std::list<TestDetails *> Tests;

      Tests m_tests;

      TestDetails *m_pActiveTest;

      mutable bool m_reported;

      Milliseconds m_testTimeout;

      typedef std::list<JetByteTools::Win32::_tstring> TraceMessages;

      TraceMessages m_traceMessages;

      // no copies - do not implement
      CTestMonitor(const CTestMonitor &rhs);
      CTestMonitor &operator=(const CTestMonitor &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_TEST_TEST_MONITOR_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: TestMonitor.h
///////////////////////////////////////////////////////////////////////////////
