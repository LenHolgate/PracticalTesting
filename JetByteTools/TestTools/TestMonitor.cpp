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

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::Output;
using JetByteTools::Win32::_tstring;
using JetByteTools::Win32::CStringConverter;
using JetByteTools::Win32::CException;
using JetByteTools::Win32::CWin32Exception;
using JetByteTools::Win32::CSEHException;

using std::cout; 
using std::endl;
using std::string;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
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

      State GetState() const;

      void TestComplete(); 

      void TestException();

      void SkipTest(
         const _tstring &reason);

      void FailTest(
         const _tstring &reason);

   private :

      const _tstring m_name;

      _tstring m_message;

      State m_state;

      // no copies - do not implement
      TestDetails(const TestDetails &rhs);
      TestDetails &operator=(const TestDetails &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// CTestMonitor
///////////////////////////////////////////////////////////////////////////////

CTestMonitor::CTestMonitor(
   const _tstring &name,
   const bool includePerformanceTests)
   :  m_name(name),
      m_includePerformanceTests(includePerformanceTests),
      m_pActiveTest(0),
      m_reported(false)
{

}

CTestMonitor::CTestMonitor()
   :  m_pActiveTest(0),
      m_includePerformanceTests(false),
      m_reported(false)
{

}

CTestMonitor::~CTestMonitor()
{
   if (!m_reported)
   {
      Report();
   }

   for (Tests::const_iterator it = m_tests.begin(), end = m_tests.end();
      it != end;
      ++it)
   {
      TestDetails *pTest = *it;

      delete pTest;
   }
}

void CTestMonitor::StartTest(
   const _tstring &className,
   const _tstring &functionName)
{
   if (m_pActiveTest)
   {
      throw CException(
         _T("CTestMonitor::StartTest()"), _T("Test \"") + m_pActiveTest->GetName() + 
         _T("\" is still running when \"") + className + _T("::") + functionName + _T("\" is being started"));
   }

   m_pActiveTest = new TestDetails(className, functionName);

   m_tests.push_back(m_pActiveTest);
}

bool CTestMonitor::StartPerformanceTest(
   const _tstring &className,
   const _tstring &functionName)
{
   if (m_pActiveTest)
   {
      throw CException(
         _T("CTestMonitor::StartTest()"), _T("Test \"") + m_pActiveTest->GetName() + 
         _T("\" is still running when \"") + className + _T("::") + functionName + _T("\" is being started"));
   }

   m_pActiveTest = new TestDetails(className, functionName);

   m_tests.push_back(m_pActiveTest);

   if (!m_includePerformanceTests)
   {
      m_pActiveTest->SkipTest(_T("Skipped - not running performance tests."));

      m_pActiveTest = 0;
   }

   return m_includePerformanceTests;
}

void CTestMonitor::TestComplete()
{
   if (!m_pActiveTest)
   {
      throw CException(_T("CTestMonitor::TestComplete()"), _T("No test running!"));
   }

   m_pActiveTest->TestComplete();

   m_pActiveTest = 0;
}

void CTestMonitor::SkipTest(
   const _tstring &reason)
{
   if (!m_pActiveTest)
   {
      throw CException(_T("CTestMonitor::SkipTest()"), _T("No test running!"));
   }

   m_pActiveTest->SkipTest(reason);

   m_pActiveTest = 0;
}

void CTestMonitor::FailTest(
   const _tstring &reason)
{
   if (!m_pActiveTest)
   {
      throw CException(_T("CTestMonitor::FailTest()"), _T("No test running!"));
   }

   m_pActiveTest->FailTest(reason);

   m_pActiveTest = 0;
}

void CTestMonitor::TestException()
{
   if (!m_pActiveTest)
   {
      throw CException(_T("CTestMonitor::TestComplete()"), _T("No test running!"));
   }

   m_pActiveTest->TestException();

   m_pActiveTest = 0;
}

bool CTestMonitor::Report() const
{
   cout << endl;
   cout << CStringConverter::TtoA(m_name) << endl;
   cout << "Test run summary" << endl;
   cout << endl;

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
         const string status = "PASSED  : " + CStringConverter::TtoA(pTest->GetName());

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

   {  // VC6 for loop scoping...
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
   cout << endl;

   m_reported = true;

   return (numPassed + numSkipped) == m_tests.size();
}

///////////////////////////////////////////////////////////////////////////////
// CTestMonitor::TestDetails
///////////////////////////////////////////////////////////////////////////////

CTestMonitor::TestDetails::TestDetails(
   const _tstring &className,
   const _tstring &functionName)
   :  m_name(className + _T("::") + functionName),
      m_state(Running)
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

void CTestMonitor::TestDetails::TestComplete()
{
   m_state = Passed;

   Output(_T("Stop : ") + m_name);
}

void CTestMonitor::TestDetails::SkipTest(
   const _tstring &reason)
{
   m_message = reason;

   m_state = Skipped;

   Output(_T("Skip : ") + m_name + _T(" - ") + m_message);
}

void CTestMonitor::TestDetails::FailTest(
   const _tstring &reason)
{
   m_state = Failed;

   m_message = reason;

   Output(_T("Fail : ") + m_name + _T(" - ") + m_message);
}

void CTestMonitor::TestDetails::TestException()
{
   m_state = Failed;

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
   catch(const char *pE)
   {
      m_message = _T("Exception - ") + CStringConverter::AtoT(pE);
   }
   catch(const wchar_t *pE)
   {
      m_message = _T("Exception - ") + CStringConverter::WtoT(pE);
   }
   catch(...)
   {
      m_message = _T("Unexpected exception");
   }  

   if (m_state == Failed)
   {
      Output(_T("Fail : ") + m_name + _T(" - ") + m_message);
   }
   else
   {
      Output(_T("Skip : ") + m_name + _T(" - ") + m_message);
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: TestMonitor.cpp
///////////////////////////////////////////////////////////////////////////////

