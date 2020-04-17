///////////////////////////////////////////////////////////////////////////////
// File: TestMonitor.cpp
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

#include "JetByteTools/Admin/Admin.h"

#include "TestMonitor.h"
#include "TestException.h"

#include "JetByteTools/Win32Tools/Utils.h"
#include "JetByteTools/Win32Tools/DebugTrace.h"
#include "JetByteTools/Win32Tools/Exception.h"
#include "JetByteTools/Win32Tools/Win32Exception.h"
#include "JetByteTools/Win32Tools/SEHException.h"
#include "JetByteTools/Win32Tools/StringConverter.h"

#pragma hdrstop

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1) || (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY == 1)
#include "JetByteTools/Win32Tools/DebugTrace.h"
#endif

#include "JetByteTools/Win32Tools/PerThreadErrorHandler.h"

#include "JetByteTools/Admin/CompilerName.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::Output;
using JetByteTools::Win32::OutputEx;
using JetByteTools::Win32::ToString;
using JetByteTools::Win32::ToStringA;
using JetByteTools::Win32::_tstring;
using JetByteTools::Win32::CStringConverter;
using JetByteTools::Win32::CException;
using JetByteTools::Win32::CWin32Exception;
using JetByteTools::Win32::CSEHException;
using JetByteTools::Win32::GetLastErrorMessageA;
using JetByteTools::Win32::GetComputerName;
using JetByteTools::Win32::CPerThreadErrorHandler;
using JetByteTools::Win32::CDebugTrace;

using std::string;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CTestMonitor::TestDetails
///////////////////////////////////////////////////////////////////////////////

class CTestMonitor::TestDetails
{
   public :

      TestDetails(
         const _tstring &className,
         const _tstring &functionName);

      TestDetails(
         const TestDetails &rhs) = delete;

      TestDetails &operator=(
         const TestDetails &rhs) = delete;

      enum State
      {
         Running,
         Passed,
         Failed,
         Skipped
      };

      const _tstring &GetName() const;

      const _tstring &GetMessage() const;

      _tstring GetTimeTakenAsString() const;

      State GetState() const;

      void TestComplete(
         DWORD timeTaken);

      void TestException(
         DWORD timeTaken);

      void SkipTest(
         const _tstring &reason);

      void FailTest(
         const _tstring &reason,
         DWORD timeTaken);

   private :

      const _tstring m_name;

      _tstring m_message;

      State m_state;

      DWORD m_timeTaken;
};

///////////////////////////////////////////////////////////////////////////////
// File level static variables
///////////////////////////////////////////////////////////////////////////////

static CTestMonitor *s_pMonitor = nullptr;

///////////////////////////////////////////////////////////////////////////////
// CTestMonitor
///////////////////////////////////////////////////////////////////////////////

CTestMonitor::CTestMonitor(
   const _tstring &name,
   const bool includePerformanceTests,
   const bool stopOnFailure,
   const bool debugOnFailure)
   :  m_testTimeoutThread(*this),
      m_pPreviousHandler(nullptr),
      m_name(name),
      m_includePerformanceTests(includePerformanceTests),
      m_stopOnFailure(stopOnFailure),
      m_debugOnFailure(debugOnFailure && IsDebuggerPresent()),
      m_pActiveTest(nullptr),
      m_reported(false),
      m_testTimeout(INFINITE)
{
   s_pMonitor = this;

   m_testTimeoutThread.Start();
   m_testTimeoutThread.SetThreadName(_T("TestTimeoutThread"));

   OutputTestDetails();
}

CTestMonitor::CTestMonitor()
   :  m_testTimeoutThread(*this),
      m_pPreviousHandler(nullptr),
      m_includePerformanceTests(false),
      m_stopOnFailure(false),
      m_debugOnFailure(false),
      m_pActiveTest(nullptr),
      m_reported(false),
      m_testTimeout(INFINITE)
{
   s_pMonitor = this;

   m_testTimeoutThread.Start();
   m_testTimeoutThread.SetThreadName(_T("TestTimeoutThread"));

   OutputTestDetails();
}

CTestMonitor::~CTestMonitor()
{
   try
   {
      m_shutdownEvent.Set();
      m_testTimeoutThread.Wait();

      if (!m_reported)
      {
         (void)Report(0);
      }

      for (Tests::const_iterator it = m_tests.begin(), end = m_tests.end();
         it != end;
         ++it)
      {
         TestDetails *pTest = *it;

         delete pTest;
      }

      s_pMonitor = nullptr;
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
}

void CTestMonitor::OutputTestDetails() const
{
#ifdef UNICODE
#ifdef _DEBUG
   OutputEx(m_name + _T(" - UNICODE - Debug - ") + JETBYTE_COMPILER_NAME);
#else
   OutputEx(m_name + _T(" - UNICODE - Release - ") + JETBYTE_COMPILER_NAME);
#endif
#else
#ifdef _DEBUG
   OutputEx(m_name + _T(" - Debug - ") + JETBYTE_COMPILER_NAME);
#else
   OutputEx(m_name + _T(" - Release - ") + JETBYTE_COMPILER_NAME);
#endif
#endif
}

void CTestMonitor::HandlePureCall()
{
   if (!m_pActiveTest)
   {
      OutputEx(_T("Pure call - No test running!"));
   }
   else
   {
      OutputEx(_T("PureCall: ") + m_pActiveTest->GetName());

      m_pActiveTest->FailTest(_T("Pure call"), 0);

      m_pActiveTest = nullptr;
   }

   if (m_pPreviousHandler)
   {
      m_pPreviousHandler();
   }

   //Report(0);
}

bool CTestMonitor::DebugOnFailure()
{
   return s_pMonitor && s_pMonitor->m_stopOnFailure && s_pMonitor->m_debugOnFailure;
}

void CTestMonitor::Trace(
   const _tstring &message)
{
   if (s_pMonitor)
   {
      s_pMonitor->m_traceMessages.push_back(message);
   }
}

Milliseconds CTestMonitor::GetTimeoutForMachine()
{
   return 10000;     // 10 seconds is a LONG time for a test to run!
}

void CTestMonitor::StartTest(
   const _tstring &className,
   const _tstring &functionName,
   const Milliseconds timeout)
{
   if (m_pActiveTest)
   {
      throw CException(
         _T("CTestMonitor::StartTest()"), _T("Test \"") + m_pActiveTest->GetName() +
         _T("\" is still running when \"") + className + _T("::") + functionName + _T("\" is being started"));
   }

   m_pActiveTest = new TestDetails(className, functionName);

   m_tests.push_back(m_pActiveTest);

   m_traceMessages.clear();

   const Milliseconds defaultTimeout = GetTimeoutForMachine();

   m_testTimeout = IsDebuggerPresent() ? INFINITE : (timeout == 0 ? defaultTimeout : timeout);

   m_startTimingEvent.Set();
   m_timingStartedEvent.Wait();

   m_testTimer.Reset();
}

bool CTestMonitor::StartPerformanceTest(
   const _tstring &className,
   const _tstring &functionName,
   const Milliseconds timeout)
{
   if (m_pActiveTest)
   {
      throw CException(
         _T("CTestMonitor::StartTest()"), _T("Test \"") + m_pActiveTest->GetName() +
         _T("\" is still running when \"") + className + _T("::") + functionName + _T("\" is being started"));
   }

   m_pActiveTest = new TestDetails(className, functionName);

   m_tests.push_back(m_pActiveTest);

   m_traceMessages.clear();

   if (!m_includePerformanceTests)
   {
      m_pActiveTest->SkipTest(_T("Skipped - not running performance tests."));

      m_pActiveTest = nullptr; //lint !e423 (Creation of memory leak in assignment to)
   }
   else
   {
      // Performance tests take longer, but 60 seconds is a LONG time for a test to run
      static const Milliseconds defaultTimeout = 60000;

      m_testTimeout = IsDebuggerPresent() ? INFINITE : (timeout == 0 ? defaultTimeout : timeout);

      m_startTimingEvent.Set();
      m_timingStartedEvent.Wait();

      m_testTimer.Reset();
   }

   return m_includePerformanceTests;
}

void CTestMonitor::TestComplete()
{
   if (!m_pActiveTest)
   {
      throw CException(_T("CTestMonitor::TestComplete()"), _T("No test running!"));
   }

   m_pActiveTest->TestComplete(m_testTimer.GetElapsedTimeAsDWORD());

   m_pActiveTest = nullptr; //lint !e423 (Creation of memory leak in assignment to)

   m_stopTimingEvent.Set();
}

void CTestMonitor::SkipTest(
   const _tstring &reason)
{
   if (!m_pActiveTest)
   {
      throw CException(_T("CTestMonitor::SkipTest()"), _T("No test running!"));
   }

   m_pActiveTest->SkipTest(reason);

   m_pActiveTest = nullptr; //lint !e423 (Creation of memory leak in assignment to)

   m_stopTimingEvent.Set();
}

void CTestMonitor::FailTest(
   const _tstring &reason)
{
   if (!m_pActiveTest)
   {
      throw CException(_T("CTestMonitor::FailTest()"), _T("No test running!"));
   }

   m_pActiveTest->FailTest(reason, m_testTimer.GetElapsedTimeAsDWORD());

   m_pActiveTest = nullptr; //lint !e423 (Creation of memory leak in assignment to)

   m_stopTimingEvent.Set();

   if (m_stopOnFailure)
   {
      if (m_debugOnFailure)
      {
         DebugBreak();
      }

      throw CException(_T("CTestMonitor::FailTest()"), _T("Test failed"));
   }
}

void CTestMonitor::TestException()
{
   if (!m_pActiveTest)
   {
      throw CException(_T("CTestMonitor::TestComplete()"), _T("No test running!"));
   }

   m_pActiveTest->TestException(m_testTimer.GetElapsedTimeAsDWORD());

   const TestDetails::State state = m_pActiveTest->GetState();

   m_pActiveTest = nullptr; //lint !e423 (Creation of memory leak in assignment to)

   m_stopTimingEvent.Set();

   if (m_stopOnFailure)
   {
      // This gives no useful information in the debuggers callstack...
      //if (m_debugOnFailure)
      //{
      //   try
      //   {
      //      // re throw the exception that we're processing to
      //      // check its type...

      //      throw;
      //   }
      //   catch (CTestSkippedException & /*e*/)
      //   {
      //      // do not break into debugger on skipped tests
      //   }
      //   catch (...)
      //   {
      //      DebugBreak();
      //   }
      //}

      if (TestDetails::Failed == state)
      {
         throw CException(_T("CTestMonitor::TestException()"), _T("Test failed"));
      }
   }
}

bool CTestMonitor::Report(
   const size_t expectedTests,
   const bool failIfTestsSkipped) const
{
   const string timeTaken = CStringConverter::TtoA(m_totalTimer.GetElapsedTimeAsString());

   CDebugTrace::Instance().SetThreadIdentifier("");

   OutputEx("");
   OutputEx("--------------------------------------------------");
   OutputEx(m_name);
   OutputEx("Test run summary");

#ifdef _UNICODE
   OutputEx("Unicode build");
#endif

#ifdef _DEBUG
   OutputEx("Debug build");
#endif

   OutputEx("--------------------------------------------------");

   if (m_pActiveTest)
   {
      OutputEx("");
      OutputEx("**************************************************");
      OutputEx(_T("Test still active: ") + m_pActiveTest->GetName());
      OutputEx("**************************************************");
   }

   size_t numPassed = 0;
   size_t numFailed = 0;
   size_t numSkipped = 0;

   typedef std::list<string> Results;

   Results results;

   Results failed;
   Results skipped;

   for (auto pTest : m_tests)
   {
      const TestDetails::State state = pTest->GetState();

      if (state == TestDetails::Running)
      {
         const string status = "RUNNING : " + CStringConverter::TtoA(pTest->GetName());

         results.push_back(status);
      }
      else if (state == TestDetails::Passed)
      {
         const string status = "PASSED  : " +
            CStringConverter::TtoA(pTest->GetTimeTakenAsString()) +
            ": " +
            CStringConverter::TtoA(pTest->GetName());

         results.push_back(status);

         numPassed++;
      }
      else if (state == TestDetails::Failed)
      {
         const string status = "FAILED  : " + CStringConverter::TtoA(pTest->GetName());
         const string message = "          " + CStringConverter::TtoA(pTest->GetMessage());

         results.push_back(status);
         results.push_back(message);

         failed.push_back(status);
         failed.push_back(message);

         numFailed++;
      }
      else if (state == TestDetails::Skipped)
      {
         const string status = "SKIPPED : " + CStringConverter::TtoA(pTest->GetName());
         const string message = "          " + CStringConverter::TtoA(pTest->GetMessage());

         results.push_back(status);
         results.push_back(message);

         skipped.push_back(status);
         skipped.push_back(message);

         numSkipped++;
      }
   }

   if (!IsDebuggerPresent())
   {
      OutputEx("");
      OutputEx("--------------------------------------------------");
      OutputEx("  Tests : " + ToStringA(m_tests.size()));
      OutputEx(" Passed : " + ToStringA(numPassed));
      OutputEx(" Failed : " + ToStringA(numFailed));
      OutputEx("Skipped : " + ToStringA(numSkipped));

      if (numFailed != 0)
      {
         OutputEx("");
         OutputEx("**************************************************");
         OutputEx(" Failed : " + ToStringA(numFailed));

         for (Results::const_iterator it = failed.begin(), end = failed.end(); it != end; ++it)
         {
            OutputEx(*it);
         }

         OutputEx("**************************************************");
      }

      if (numSkipped != 0)
      {
         OutputEx("");
         OutputEx("**************************************************");
         OutputEx("Skipped : " + ToStringA(numSkipped));

         for (Results::const_iterator it = skipped.begin(), end = skipped.end(); it != end; ++it)
         {
            OutputEx(*it);
         }

         OutputEx("**************************************************");
      }

      OutputEx("Full test run output");
      OutputEx("--------------------------------------------------");

      for (Results::const_iterator it = results.begin(), end = results.end(); it != end; ++it)
      {
         OutputEx(*it);
      }
   }

   OutputEx("--------------------------------------------------");
   OutputEx("  Tests : " + ToStringA(m_tests.size()));
   OutputEx(" Passed : " + ToStringA(numPassed));
   OutputEx(" Failed : " + ToStringA(numFailed));
   OutputEx("Skipped : " + ToStringA(numSkipped));
   OutputEx("   Time : " + timeTaken);
   OutputEx("--------------------------------------------------");
   
   const size_t actualTestsRun = m_tests.size();

   bool passed = (numPassed + numSkipped) == actualTestsRun;

   if (expectedTests != 0)
   {
      if (actualTestsRun < expectedTests)
      {
         OutputEx("");
         OutputEx("**************************************************");
         OutputEx("Not all tests have been run: expected: " + ToStringA(expectedTests) + " only ran: " + ToStringA(actualTestsRun));
         OutputEx("**************************************************");

         passed = false;
      }
      else if (actualTestsRun > expectedTests)
      {
         OutputEx("");
         OutputEx("**************************************************");
         OutputEx("More tests than expected have been run: expected: " + ToStringA(expectedTests) + " actually ran: " + ToStringA(actualTestsRun));
         OutputEx("Update the expectedTests value in Test.cpp!");
         OutputEx("**************************************************");

         passed = false;
      }
   }

   if (failIfTestsSkipped && numSkipped != 0)
   {
      passed = false;
   }

   m_reported = true;

   return passed;
}

unsigned int CTestMonitor::Run()
{
   #if (JETBYTE_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD == 0)
   CPerThreadErrorHandler errorHandler;
   #endif

   try
   {
      try
      {
         HANDLE shutdownAndStartHandles[] =
         {
            m_shutdownEvent.GetWaitHandle(),
            m_startTimingEvent.GetWaitHandle()
         };

         while (!m_shutdownEvent.Wait(0))
         {
            const DWORD shutdownAndStartWaitResult = WaitForMultipleObjects(2, shutdownAndStartHandles, false, INFINITE);

            if (shutdownAndStartWaitResult == WAIT_OBJECT_0)
            {
               // Time to shutdown
               break;
            }

            if (shutdownAndStartWaitResult == WAIT_OBJECT_0 + 1)
            {
               if (m_pActiveTest)
               {
                  // Time to time a test...

                  // Grab the data that we need about the test we're about to time...

                  const string testName = CStringConverter::TtoA(m_pActiveTest->GetName());

                  const Milliseconds timeout = m_testTimeout;

                  m_timingStartedEvent.Set();

                  HANDLE shutdownAndStopHandles[] =
                  {
                     m_shutdownEvent.GetWaitHandle(),
                     m_stopTimingEvent.GetWaitHandle()
                  };

                  const DWORD shutdownAndStopWaitResult = WaitForMultipleObjects(2, shutdownAndStopHandles, false, timeout);

                  if (shutdownAndStopWaitResult == WAIT_OBJECT_0)
                  {
                     // Time to shutdown
                     break;
                  }
                  
                  if (shutdownAndStopWaitResult == WAIT_OBJECT_0 + 1)
                  {
                     // test completed within the time limit
                  }
                  else if (shutdownAndStopWaitResult == WAIT_TIMEOUT)
                  {
                     // the test timed out...

                     OutputEx("Test: " + testName + " timed out. Timeout was: " + ToStringA(timeout) + "ms");

                     if (!m_traceMessages.empty())
                     {
                        OutputEx("Trace messages:");

                        for (TraceMessages::const_iterator it = m_traceMessages.begin(), end = m_traceMessages.end();
                             it != end;
                             ++it)
                        {
                           OutputEx(*it);
                        }
                     }

                     exit(2);
                  }
                  else if (shutdownAndStopWaitResult == WAIT_FAILED)
                  {
                     const DWORD lastError = GetLastError();

                     OutputEx("Unexpected result during test timeout wait for test to end" + GetLastErrorMessageA(lastError, true));

                     exit(3);
                  }
               }
               else
               {
                  OutputEx("Unexpected; no active test to time!");
               }
            }
            else
            {
               const DWORD lastError = GetLastError();

               OutputEx("Unexpected result during test timeout wait for test to start" + GetLastErrorMessageA(lastError, true));
            }
         }
      }
      catch (const CException &e)
      {
         OutputEx(_T("Exception during test timeout processing: ") + e.GetWhere() + _T(" - ") + e.GetMessage());
      }
      catch (const CSEHException &e)
      {
         OutputEx(_T("Exception during test timeout processing: ") + e.GetWhere() + _T(" - ") + e.GetMessage());
      }
      JETBYTE_TESTS_CATCH_ALL_AT_THREAD_BOUNDARY_IF_ENABLED
      {
         OutputEx("Unecpected exception during test timeout processing");
      }
   }
   JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED

   return 0;
}

///////////////////////////////////////////////////////////////////////////////
// CTestMonitor::TestDetails
///////////////////////////////////////////////////////////////////////////////

CTestMonitor::TestDetails::TestDetails(
   const _tstring &className,
   const _tstring &functionName)
   :  m_name(className + _T("::") + functionName),
      m_state(Running),
      m_timeTaken(0)
{
   Output(_T("Start: ") + m_name);
}

const _tstring &CTestMonitor::TestDetails::GetName() const
{
   return m_name;
}

const _tstring &CTestMonitor::TestDetails::GetMessage() const
{
   return m_message;
}

CTestMonitor::TestDetails::State CTestMonitor::TestDetails::GetState() const
{
   return m_state;
}

void CTestMonitor::TestDetails::TestComplete(
   const DWORD timeTaken)
{
   m_state = Passed;

   m_timeTaken = timeTaken;

   Output(_T("Stop : ") + GetTimeTakenAsString() + m_name);
}

void CTestMonitor::TestDetails::SkipTest(
   const _tstring &reason)
{
   m_message = reason;

   m_state = Skipped;

   Output(_T("Skip : ") + m_name + _T(" - ") + m_message);
}

void CTestMonitor::TestDetails::FailTest(
   const _tstring &reason,
   const DWORD timeTaken)
{
   m_state = Failed;

   m_message = reason;

   m_timeTaken = timeTaken;

   Output(_T("Fail : ") + GetTimeTakenAsString() + m_name + _T(" - ") + m_message);
}

void CTestMonitor::TestDetails::TestException(
   const DWORD timeTaken)
{
   m_state = Failed;

   m_timeTaken = timeTaken;

   try
   {
      // dispatch the exception that must have been caught for us to be called...

      throw;
   }
   catch (CTestException &e)
   {
      if (!e.GetWhere().empty())
      {
         m_message = _T("CTestException - ") + e.GetWhere() + _T(" - ") + e.GetMessage();
      }
      else
      {
         m_message = _T("CTestException - ") + e.GetMessage();
      }
   }
   catch (CTestSkippedException &e)
   {
      if (!e.GetWhere().empty())
      {
         m_message = e.GetWhere() + _T(" - ") + e.GetMessage();
      }
      else
      {
         m_message = e.GetMessage();
      }

      m_state = Skipped;
   }
   catch (CWin32Exception &e)
   {
      m_message = _T("CWin32Exception - ") + e.GetWhere() + _T(" - ") + e.GetMessage();
   }
   catch (CException &e)
   {
      m_message = _T("CException - ") + e.GetWhere() + _T(" - ") + e.GetMessage();
   }
   catch (CSEHException &e)
   {
      m_message = _T("CSEHException - ") + e.GetWhere() + _T(" - ") + e.GetMessage();
   }
   catch (std::exception &e)
   {
      m_message = _T("std::exception - ") + CStringConverter::AtoT(e.what());
   }
   catch (const char *pE) //lint !e1752 (catch parameter is not a reference)
   {
      m_message = _T("Exception - ") + CStringConverter::AtoT(pE);
   }
   catch (const wchar_t *pE) //lint !e1752 (catch parameter is not a reference)
   {
      m_message = _T("Exception - ") + CStringConverter::WtoT(pE);
   }
   JETBYTE_TESTS_CATCH_ALL_IF_ENABLED
   {
      m_message = _T("Unexpected exception");
   }

   if (m_state == Failed)
   {
      Output(_T("Fail : ") + GetTimeTakenAsString() + m_name + _T(" - ") + m_message);
   }
   else
   {
      Output(_T("Skip : ") + m_name + _T(" - ") + m_message);
   }
}

_tstring CTestMonitor::TestDetails::GetTimeTakenAsString() const
{
   if (m_timeTaken == 0)
   {
      return _T("");
   }

   return ToString(m_timeTaken) + _T("ms ");
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: TestMonitor.cpp
///////////////////////////////////////////////////////////////////////////////
