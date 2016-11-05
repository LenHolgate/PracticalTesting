///////////////////////////////////////////////////////////////////////////////
// File: Test.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
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

#include "JetByteTools\Win32Tools\DebugTrace.h"

#include "JetByteTools\TestTools\TestMonitor.h"
#include "JetByteTools\TestTools\TestException.h"

#include "CallbackTimerQueueTest.h"
#include "CallbackTimerQueueExTest.h"
#include "CallbackTimerWheelTest.h"
#include "ThreadedCallbackTimerQueueTest.h"

#include "JetByteTools\Win32Tools\Utils.h"
#include "JetByteTools\Win32Tools\Exception.h"
#include "JetByteTools\Win32Tools\SEHException.h"
#include "JetByteTools\Win32Tools\StringConverter.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::OutputEx;
using JetByteTools::Win32::CDebugTrace;
using JetByteTools::Win32::CException;
using JetByteTools::Win32::CStringConverter;
using JetByteTools::Win32::CSEHException;
using JetByteTools::Win32::SetCurrentDirectory;
using JetByteTools::Win32::ToBool;

using JetByteTools::Test::CTestException;
using JetByteTools::Test::CTestMonitor;

using namespace JetByteTools::Win32::Test;


///////////////////////////////////////////////////////////////////////////////
// Program Entry Point
///////////////////////////////////////////////////////////////////////////////

int main(int /*argc*/, char * /*argv[ ]*/)
{
   int nArgc = __argc;
#ifdef UNICODE
   PWSTR *ppArgv = CommandLineToArgvW(GetCommandLine(), &nArgc);
#else
   PCSTR *ppArgv = (PCSTR*) __argv;
#endif

   ::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

   CSEHException::Translator sehTranslator;

   CDebugTrace::Instance().SetLogName("Win32ToolsTest.log");

   bool ok = false;

   try
   {
      if (nArgc == 2)
      {
         SetCurrentDirectory(ppArgv[1]);
      }


#ifdef _DEBUG
      const bool includePerformanceTests = false;
#else
      const bool includePerformanceTests = true;
#endif
      const bool stopOnFailure = ToBool(::IsDebuggerPresent());

      CTestMonitor monitor(_T("Win32 Tools"), includePerformanceTests, stopOnFailure);

      CCallbackTimerWheelTest::TestAll(monitor);
      CCallbackTimerQueueTest::TestAll(monitor);
      CCallbackTimerQueueExTest::TestAll(monitor);
      CThreadedCallbackTimerQueueTest::TestAll(monitor);

      const size_t expectedTests = 153;

      ok = monitor.Report(expectedTests);
   }
   catch(const CTestException &e)
   {
      OutputEx("Test Exception: " + CStringConverter::TtoA(e.GetWhere() + _T(" - ") + e.GetMessage()));

      ok = false;
   }
   catch(const CException &e)
   {
      OutputEx("Exception: " + CStringConverter::TtoA(e.GetWhere() + _T(" - ") + e.GetMessage()));

      ok = false;
   }
   catch(const CSEHException &e)
   {
      OutputEx("Exception: " + CStringConverter::TtoA(e.GetWhere() + _T(" - ") + e.GetMessage()));

      ok = false;
   }
   catch(const char *p)
   {
      OutputEx(std::string("Exception: ") + p);
   }
   JETBYTE_TESTS_CATCH_ALL_IF_ENABLED
   {
      OutputEx("Unexpected exception");

      ok = false;
   }

   OutputEx(std::string("Test ") + (ok ? "Passed" : "Failed"));

   return ok ? 0 : 1;
}

///////////////////////////////////////////////////////////////////////////////
// End of file: Test.cpp
///////////////////////////////////////////////////////////////////////////////

