///////////////////////////////////////////////////////////////////////////////
// File: TestLog.cpp
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

#include "TestLog.h"
#include "TestException.h"
#include "TestMonitor.h"

#include <iostream>

#include "JetByteTools\Win32Tools\Utils.h"
#include "JetByteTools\Win32Tools\DebugTrace.h"
#include "JetByteTools\Win32Tools\StringConverter.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::OutputEx;
using JetByteTools::Win32::_tstring;
using JetByteTools::Win32::CCriticalSection;
using JetByteTools::Win32::CStringConverter;
using JetByteTools::Win32::FindAndReplace;
using JetByteTools::Win32::ToString;
using JetByteTools::Win32::LoadFileAsString;
using JetByteTools::Win32::SaveStringAsFile;
using JetByteTools::Win32::CException;

using std::string;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CTestLog
///////////////////////////////////////////////////////////////////////////////

CTestLog::CTestLog(
   const _tstring &separator)
   :  m_fileNumber(1),
      m_separator(separator),
      m_pLog(0)
{

}

CTestLog::CTestLog()
   :  m_fileNumber(1),
      m_separator(_T("|")),
      m_pLog(0)
{

}

CTestLog::CTestLog(
   CTestLog *pLinkedLog)
   :  m_fileNumber(1),
      m_separator(_T("|")),
      m_pLog(pLinkedLog)
{

}

CTestLog::CTestLog(
   CTestLog *pLinkedLog,
   const _tstring &separator)
   :  m_fileNumber(1),
      m_separator(separator),
      m_pLog(pLinkedLog)
{

}

CTestLog::~CTestLog()
{
   const _tstring messages = GetMessages();

   if (messages.length() != 0)
   {
      CTestMonitor::Trace(messages);
   }
}

void CTestLog::ClearLog()
{
   if (m_pLog)
   {
      m_pLog->ClearLog();
   }
   else
   {
      CCriticalSection::Owner lock(m_criticalSection);

      m_log.clear();
   }
}

void CTestLog::LogMessage(
   const _tstring &message) const
{
   if (m_pLog)
   {
      m_pLog->LogMessage(message);
   }
   else
   {
      CCriticalSection::Owner lock(m_criticalSection);

      m_log.push_back(message);
   }
}

_tstring CTestLog::GetMessages() const
{
   if (m_pLog)
   {
      return m_pLog->GetMessages();
   }

   CCriticalSection::Owner lock(m_criticalSection);

   _tstring result;

   for (Log::const_iterator it = m_log.begin(); it != m_log.end(); ++it)
   {
      result += *it;
      result += m_separator;
   }

   return result;
}

_tstring CTestLog::RemoveMessages() 
{
   if (m_pLog)
   {
      return m_pLog->RemoveMessages();
   }

   CCriticalSection::Owner lock(m_criticalSection);

   _tstring result = GetMessages();
   
   m_log.clear();

   return result;
}

void CTestLog::CheckNoResults(
   const DisplayOnFailureMode displayOnFailure)
{
   CheckResult(m_separator, displayOnFailure);
}

void CTestLog::CheckResult(
   const _tstring &expectedResult, 
   const DisplayOnFailureMode displayOnFailure)
{
   CheckResult(expectedResult, m_separator + RemoveMessages(), displayOnFailure);
}

void CTestLog::CheckResult(
   const _tstring &expectedResult, 
   const _tstring &actualResult, 
   const DisplayOnFailureMode displayOnFailure)
{
   if (actualResult != expectedResult)
   {
      if (displayOnFailure == DisplayOnFailure)
      {
         OutputEx(_T("result:   ") + actualResult);
         OutputEx(_T("expected: ") + expectedResult);
      }

      throw CTestException(_T("CTestLog::CheckResult()"), 
         _T("Log does not contain expected result\n")
         _T("result:   ") + actualResult + _T("\n")
         _T("expected: ") + expectedResult);
   }
   else
   {
      // tell the monitor of a successful check

      CTestMonitor::Trace(expectedResult);
   }
}

void CTestLog::CheckResultA(
   const string &expectedResult, 
   const DisplayOnFailureMode displayOnFailure)
{
   CheckResult(CStringConverter::AtoT(expectedResult), displayOnFailure);
}

_tstring CTestLog::GetFileName(
   const _tstring &fileName)
{
   return fileName + _T(".") + ToString(m_fileNumber++);
}

void CTestLog::CheckResultFromFile(
   const _tstring &fileName,
   const bool fileContainsLineEndBars)
{
   if (m_pLog)
   {
      m_pLog->CheckResultFromFile(fileName, fileContainsLineEndBars);
   }
   else
   {
      const _tstring lineEnd = (fileContainsLineEndBars ? m_separator + _T("\r\n") : _T("\r\n"));

      CCriticalSection::Owner lock(m_criticalSection);

      _tstring actualResults = GetMessages();

      actualResults = FindAndReplace(actualResults, m_separator, lineEnd);

      const _tstring fileNameBase = GetFileName(fileName);

      try
      {
         _tstring expectedResults = LoadFileAsString(fileNameBase + _T(".log"));

         expectedResults = FindAndReplace(expectedResults, lineEnd, m_separator);

         CheckResult(m_separator + expectedResults, DoNotDisplayOnFailure);
      }
      catch(const CException &/*e*/)
      {
         SaveStringAsFile(fileNameBase + _T(".Actual.log"), actualResults);
         
         throw;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: TestLog.cpp
///////////////////////////////////////////////////////////////////////////////

