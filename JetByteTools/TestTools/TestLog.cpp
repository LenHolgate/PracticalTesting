///////////////////////////////////////////////////////////////////////////////
// File: TestLog.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2003 JetByte Limited.
//
// JetByte Limited grants you ("Licensee") a non-exclusive, royalty free, 
// licence to use, modify and redistribute this software in source and binary 
// code form, provided that i) this copyright notice and licence appear on all 
// copies of the software; and ii) Licensee does not utilize the software in a 
// manner which is disparaging to JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All 
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors 
// shall not be liable for any damages suffered by licensee as a result of 
// using, modifying or distributing the software or its derivatives. In no
// event will JetByte Limited be liable for any lost revenue, profit or data,
// or for direct, indirect, special, consequential, incidental or punitive
// damages, however caused and regardless of the theory of liability, arising 
// out of the use of or inability to use software, even if JetByte Limited 
// has been advised of the possibility of such damages.
//
// This software is not designed or intended for use in on-line control of 
// aircraft, air traffic, aircraft navigation or aircraft communications; or in 
// the design, construction, operation or maintenance of any nuclear 
// facility. Licensee represents and warrants that it will not use or 
// redistribute the Software for such purposes. 
//
///////////////////////////////////////////////////////////////////////////////

#include "JetByteTools\Admin\Admin.h"

#include "TestLog.h"
#include "TestException.h"

#include <iostream>

#include "JetByteTools\Win32Tools\Utils.h"
#include "JetByteTools\Win32Tools\StringConverter.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::Output;
using JetByteTools::Win32::_tstring;
using JetByteTools::Win32::CCriticalSection;
using JetByteTools::Win32::CStringConverter;

using std::string;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CTestLog
///////////////////////////////////////////////////////////////////////////////

void CTestLog::ClearLog()
{
   CCriticalSection::Owner lock(m_criticalSection);

   m_log.clear();
}

void CTestLog::LogMessage(
   const _tstring &message) const
{
   CCriticalSection::Owner lock(m_criticalSection);

   m_log.push_back(message);
}

_tstring CTestLog::GetMessages() const
{
   CCriticalSection::Owner lock(m_criticalSection);

   _tstring result = _T("|");

   for (Log::const_iterator it = m_log.begin(); it != m_log.end(); ++it)
   {
      result += *it;
      result += _T("|");
   }

   return result;
}

_tstring CTestLog::RemoveMessages() 
{
   CCriticalSection::Owner lock(m_criticalSection);

   _tstring result = GetMessages();
   
   m_log.clear();

   return result;
}

void CTestLog::CheckNoResults(
   bool displayOnFailure)
{
   CheckResult(_T("|"), displayOnFailure);
}

void CTestLog::CheckResult(
   const _tstring &expectedResult, 
   bool displayOnFailure)
{
   const _tstring result = RemoveMessages();

   if (result != expectedResult)
   {
      if (displayOnFailure)
      {
         Output(_T("result:   ") + result);
         Output(_T("expected: ") + expectedResult);
      }

      throw CTestException(_T("CTestLog::CheckResult()"), _T("Log does not contain expected result"));
   }
}

void CTestLog::CheckResultA(
   const string &expectedResult, 
   bool displayOnFailure)
{
   CheckResult(CStringConverter::AtoT(expectedResult), displayOnFailure);
}


///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: TestLog.cpp
///////////////////////////////////////////////////////////////////////////////

