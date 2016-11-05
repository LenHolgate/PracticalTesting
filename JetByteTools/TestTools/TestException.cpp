///////////////////////////////////////////////////////////////////////////////
// File: TestException.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2003 JetByte Limited.
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

#include "TestException.h"
#include "TestMonitor.h"

#pragma hdrstop

#include "JetByteTools\Win32Tools\OSVersionInfo.h"

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::CException;
using JetByteTools::Win32::_tstring;
using JetByteTools::Win32::COSVersionInfo;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CTestException
///////////////////////////////////////////////////////////////////////////////

CTestException::CTestException(
   const _tstring &message)
   :  CException(_T(""), message)
{
   if (CTestMonitor::DebugOnFailure())
   {
      DebugBreak();
   }
}

CTestException::CTestException(
   const _tstring &where, 
   const _tstring &message)
   :  CException(where, message)
{
   if (CTestMonitor::DebugOnFailure())
   {
      DebugBreak();
   }
}

CTestException::CTestException(
   const _tstring &where, 
   const _tstring &message,
   const bool nonFatal)
   :  CException(where, message)
{
   if (!nonFatal && CTestMonitor::DebugOnFailure())
   {
      DebugBreak();
   }
}

///////////////////////////////////////////////////////////////////////////////
// CNonFatalTestException
///////////////////////////////////////////////////////////////////////////////

CNonFatalTestException::CNonFatalTestException(
   const _tstring &message)
   :  CTestException(_T(""), message, true)
{

}

CNonFatalTestException::CNonFatalTestException(
   const _tstring &where, 
   const _tstring &message)
   :  CTestException(where, message, true)
{

}

///////////////////////////////////////////////////////////////////////////////
// CTestSkippedException
///////////////////////////////////////////////////////////////////////////////

CTestSkippedException::CTestSkippedException(
   const _tstring &message)
   :  CException(_T(""), message)
{
}

CTestSkippedException::CTestSkippedException(
   const _tstring &where, 
   const _tstring &message)
   :  CException(where, message)
{
}

///////////////////////////////////////////////////////////////////////////////
// TestRequiresVistaOrLater
///////////////////////////////////////////////////////////////////////////////

void TestRequiresVistaOrLater()
{
#if (_WIN32_WINNT >= 0x0600) 
   if (!COSVersionInfo().IsVistaOrLater())
   {
      throw CTestSkippedException(_T("Functionality not supported on this platform"));
   }
#else
   throw CTestSkippedException(_T("Functionality not supported on this platform"));
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: TestException.cpp
///////////////////////////////////////////////////////////////////////////////
