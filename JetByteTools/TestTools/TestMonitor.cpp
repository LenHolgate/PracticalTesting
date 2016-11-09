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

#include "JetByteTools\Admin\Admin.h"

#include "TestMonitor.h"
#include "TestException.h"

#include "JetByteTools\Win32Tools\Utils.h"
#include "JetByteTools\Win32Tools\DebugTrace.h"
#include "JetByteTools\Win32Tools\Exception.h"
#include "JetByteTools\Win32Tools\Win32Exception.h"
#include "JetByteTools\Win32Tools\SEHException.h"
#include "JetByteTools\Win32Tools\StringConverter.h"

#pragma hdrstop

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1) || (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY == 1)
#include "JetByteTools\Win32Tools\DebugTrace.h"
#endif

#include "JetByteTools\Win32Tools\PerThreadErrorHandler.h"

#include "JetByteTools\Admin\CompilerName.h"

#include <iostream>

#pragma warning(disable: 4355) // this used in base member initialiser list

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
using JetByteTools::Win32::GetLastErrorMessage;
using JetByteTools::Win32::GetComputerName;
using JetByteTools::Win32::CPerThreadErrorHandler;

using std::cout;
using std::endl;
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
         const DWORD timeTaken);

      void TestException(
         const DWORD timeTaken);

      void SkipTest(
         const _tstring &reason);

      void FailTest(
         const _tstring &reason,
         const DWORD timeTaken);

   private :

      const _tstring m_name;

      _tstring m_message;

      State m_state;

      DWORD m_timeTaken;

      // no copies - do not implement
      TestDetails(const TestDetails &rhs);
      TestDetails &operator=(const TestDetails &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Purecall handler
///////////////////////////////////////////////////////////////////////////////

static CTestMonitor *s_pMonitor = 0;

static void PureCallHandler()
{
   if (s_pMonitor)
   {
      s_pMonitor->HandlePureCall();
   }
}

///////////////////////////////////////////////////////////////////////////////
// CTestMonitor
///////////////////////////////////////////////////////////////////////////////

CTestMonitor::CTestMonitor(
   const _tstring &name,
   const bool includePerformanceTests,
   const bool stopOnFailure,
   const bool debugOnFailure)
   :  m_testTimeoutThread(*this),
      m_pPreviousHandler(0),
      m_name(name),
      m_includePerformanceTests(includePerformanceTests),
      m_pActiveTest(0),
      m_reported(false),
      m_stopOnFailure(stopOnFailure),
      m_debugOnFailure(debugOnFailure && ::IsDebuggerPresent()),
      m_testTimeout(INFINITE)
{
   s_pMonitor = this;

   m_testTimeoutThread.Start();
   m_testTimeoutThread.SetThreadName(_T("TestTimeoutThread"));

   OutputTestDetails();
}

CTestMonitor::CTestMonitor()
   :  m_testTimeoutThread(*this),
      m_pPreviousHandler(0),
      m_pActiveTest(0),
      m_includePerformanceTests(false),
      m_reported(false),
      m_stopOnFailure(false),
      m_debugOnFailure(false),
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

      s_pMonitor = 0;
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
}

void CTestMonitor::OutputTestDetails()
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

      m_pActiveTest = 0;
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
   const _tstring machine = GetComputerName();

   if (machine == _T("SATURN"))
   {
      return 20000;
   }
   else if (machine == _T("GALLIFREY"))
   {
      return 20000;
   }
   else if (machine == _T("MERCURY"))
   {
      return 20000;
   }

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

   m_testTimeout = ::IsDebuggerPresent() ? INFINITE : (timeout == 0 ? defaultTimeout : timeout);

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

      m_pActiveTest = 0; //lint !e423 (Creation of memory leak in assignment to)
   }
   else
   {
      // Performance tests take longer, but 60 seconds is a LONG time for a test to run
      static const Milliseconds defaultTimeout = 60000;

      m_testTimeout = ::IsDebuggerPresent() ? INFINITE : (timeout == 0 ? defaultTimeout : timeout);

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

   m_pActiveTest = 0; //lint !e423 (Creation of memory leak in assignment to)

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

   m_pActiveTest = 0; //lint !e423 (Creation of memory leak in assignment to)

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

   m_pActiveTest = 0; //lint !e423 (Creation of memory leak in assignment to)

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

   m_pActiveTest = 0; //lint !e423 (Creation of memory leak in assignment to)

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
      //   catch(CTestSkippedException & /*e*/)
      //   {
      //      // do not break into debugger on skipped tests
      //   }
      //   catch(...)
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
   const size_t expectedTests) const
{
   const string timeTaken = CStringConverter::TtoA(m_totalTimer.GetElapsedTimeAsString());

   cout << endl;
   cout << CStringConverter::TtoA(m_name) << endl;
   cout << "Test run summary" << endl;
   cout << endl;

#ifdef _UNICODE
   cout << "Unicode build" << endl;
#endif

#ifdef _DEBUG
   cout << "Debug build" << endl;
#endif

   if (m_pActiveTest)
   {
      cout << "Test still active: " << CStringConverter::TtoA(m_pActiveTest->GetName()) << endl;
   }

   size_t numPassed = 0;
   size_t numFailed = 0;
   size_t numSkipped = 0;

   typedef std::list<string> Results;

   Results results;

   Results failed;
   Results skipped;

   for (Tests::const_iterator it = m_tests.begin(), end = m_tests.end();
      it != end;
      ++it)
   {
      TestDetails *pTest = *it;

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

   if (!::IsDebuggerPresent())
   {
      cout << endl;
      cout << "  Tests : " << m_tests.size() << endl;
      cout << " Passed : " << numPassed << endl;
      cout << " Failed : " << numFailed << endl;
      cout << "Skipped : " << numSkipped << endl;
      cout << endl;

      if (numFailed != 0)
      {
         cout << endl;
         cout << " Failed : " << numFailed << endl;
         cout << endl;

         for (Results::const_iterator it = failed.begin(), end = failed.end(); it != end; ++it)
         {
            cout << *it << endl;
         }
      }

      if (numSkipped != 0)
      {
         cout << endl;
         cout << "Skipped : " << numSkipped << endl;
         cout << endl;

         for (Results::const_iterator it = skipped.begin(), end = skipped.end(); it != end; ++it)
         {
            cout << *it << endl;
         }
      }

      cout << endl;
      cout << "Full test run output" << endl;
      cout << endl;

      for (Results::const_iterator it = results.begin(), end = results.end(); it != end; ++it)
      {
         cout << *it << endl;
      }
   }

   cout << endl;
   cout << "  Tests : " << m_tests.size() << endl;
   cout << " Passed : " << numPassed << endl;
   cout << " Failed : " << numFailed << endl;
   cout << "Skipped : " << numSkipped << endl;
   cout << "   Time : " << timeTaken << endl;

   cout << endl;

   const size_t actualTestsRun = m_tests.size();

   bool passed = (numPassed + numSkipped) == actualTestsRun;

   if (expectedTests != 0)
   {
      if (actualTestsRun < expectedTests)
      {
         cout << "Not all tests have been run: expected: " << expectedTests << " only ran: " << actualTestsRun << endl;

         passed = false;
      }
      else if (actualTestsRun > expectedTests)
      {
         cout << "More tests than expected have been run: expected: " << expectedTests << " actually ran: " << actualTestsRun << endl;
         cout << "Update the expectedTests value in Test.cpp!" << endl;

         passed = false;
      }
   }

   cout << endl;

   m_reported = true;

   return passed;
}

int CTestMonitor::Run()
{
   #if (JETBYTE_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD == 0)
   CPerThreadErrorHandler errorHandler;
   #endif

   try
   {
      try
      {
         HANDLE handlesToWaitFor[2];

         handlesToWaitFor[0] = m_shutdownEvent.GetWaitHandle();
         handlesToWaitFor[1] = m_startTimingEvent.GetWaitHandle();

         while (!m_shutdownEvent.Wait(0))
         {
            DWORD waitResult = ::WaitForMultipleObjects(2, handlesToWaitFor, false, INFINITE);

            if (waitResult == WAIT_OBJECT_0)
            {
               // Time to shutdown
               break;
            }
            else if (waitResult == WAIT_OBJECT_0 + 1)
            {
               if (m_pActiveTest)
               {
                  // Time to time a test...

                  // Grab the data that we need about the test we're about to time...

                  const string testName = CStringConverter::TtoA(m_pActiveTest->GetName());

                  const Milliseconds timeout = m_testTimeout;

                  m_timingStartedEvent.Set();

                  HANDLE handlesToWaitFor[2]; //lint !e578 (Declaration of symbol 'handlesToWaitFor' hides symbol 'handlesToWaitFor')

                  handlesToWaitFor[0] = m_shutdownEvent.GetWaitHandle();
                  handlesToWaitFor[1] = m_stopTimingEvent.GetWaitHandle();

                  DWORD waitResult = ::WaitForMultipleObjects(2, handlesToWaitFor, false, timeout); //lint !e578 (Declaration of symbol 'waitResult' hides symbol 'waitResult')

                  if (waitResult == WAIT_OBJECT_0)
                  {
                     // Time to shutdown
                     break;
                  }
                  else if (waitResult == WAIT_OBJECT_0 + 1)
                  {
                     // test completed within the time limit
                  }
                  else if (waitResult == WAIT_TIMEOUT)
                  {
                     // the test timed out...

                     cout << "Test: " << testName << " timed out. Timeout was: " << ToStringA(timeout) << "ms" << endl;

                     if (m_traceMessages.size() != 0)
                     {
                        cout << "Trace messages:" << endl;

                        for (TraceMessages::const_iterator it = m_traceMessages.begin(), end = m_traceMessages.end();
                           it != end;
                           ++it)
                        {
                           cout << CStringConverter::TtoA(*it) << "\"" << endl;
                        }
                     }

                     exit(2);
                  }
                  else if (waitResult == WAIT_FAILED)
                  {
                     const DWORD lastError = ::GetLastError();

                     cout << "Unexpected result during test timeout wait for test to end" << CStringConverter::TtoA(GetLastErrorMessage(lastError)) << endl;

                     exit(3);
                  }
               }
               else
               {
                  cout << "Unexpected; no active test to time!" << endl;
               }
            }
            else
            {
               const DWORD lastError = ::GetLastError();

               cout << "Unexpected result during test timeout wait for test to start" << CStringConverter::TtoA(GetLastErrorMessage(lastError)) << endl;
            }
         }
      }
      catch(const CException &e)
      {
         cout << "Exception during test timeout processing: " << CStringConverter::TtoA(e.GetWhere() + _T(" - ") + e.GetMessage()) << endl;
      }
      catch(const CSEHException &e)
      {
         cout << "Exception during test timeout processing: " << CStringConverter::TtoA(e.GetWhere() + _T(" - ") + e.GetMessage()) << endl;
      }
      JETBYTE_TESTS_CATCH_ALL_AT_THREAD_BOUNDARY_IF_ENABLED
      {
         cout << "Unecpected exception during test timeout processing" << endl;
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
   catch(CTestException &e)
   {
      if (e.GetWhere() != _T(""))
      {
         m_message = _T("CTestException - ") + e.GetWhere() + _T(" - ") + e.GetMessage();
      }
      else
      {
         m_message = _T("CTestException - ") + e.GetMessage();
      }
   }
   catch(CTestSkippedException &e)
   {
      if (e.GetWhere() != _T(""))
      {
         m_message = e.GetWhere() + _T(" - ") + e.GetMessage();
      }
      else
      {
         m_message = e.GetMessage();
      }

      m_state = Skipped;
   }
   catch(CWin32Exception &e)
   {
      m_message = _T("CWin32Exception - ") + e.GetWhere() + _T(" - ") + e.GetMessage();
   }
   catch(CException &e)
   {
      m_message = _T("CException - ") + e.GetWhere() + _T(" - ") + e.GetMessage();
   }
   catch(CSEHException &e)
   {
      m_message = _T("CSEHException - ") + e.GetWhere() + _T(" - ") + e.GetMessage();
   }
   catch(std::exception &e)
   {
      m_message = _T("std::exception - ") + CStringConverter::AtoT(e.what());
   }
   catch(const char *pE)
   {
      m_message = _T("Exception - ") + CStringConverter::AtoT(pE);
   }
   catch(const wchar_t *pE)
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

