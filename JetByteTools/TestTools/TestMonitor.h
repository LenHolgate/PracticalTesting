#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

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

#include <list>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CTestMonitor
///////////////////////////////////////////////////////////////////////////////

class CTestMonitor 
{
   public :

      CTestMonitor(
         const JetByteTools::Win32::_tstring &name,
         const bool includePerformanceTests = true);

      CTestMonitor();

      ~CTestMonitor();

      void StartTest(
         const JetByteTools::Win32::_tstring &className,
         const JetByteTools::Win32::_tstring &functionName);

      bool StartPerformanceTest(
         const JetByteTools::Win32::_tstring &className,
         const JetByteTools::Win32::_tstring &functionName);

      void TestComplete();

      void TestException();

      void SkipTest(
         const JetByteTools::Win32::_tstring &reason);

      void FailTest(
         const JetByteTools::Win32::_tstring &reason);

      bool Report() const;

   private :

      const JetByteTools::Win32::_tstring m_name;

      const bool m_includePerformanceTests;

      class TestDetails;

      typedef std::list<TestDetails *> Tests;

      Tests m_tests;

      TestDetails *m_pActiveTest;

      mutable bool m_reported;

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
