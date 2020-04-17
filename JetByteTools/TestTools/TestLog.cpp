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

#include "JetByteTools/Admin/Admin.h"

#include "TestLog.h"
#include "TestException.h"
#include "TestMonitor.h"

#include <iostream>

#include "JetByteTools/Win32Tools/Utils.h"
#include "JetByteTools/Win32Tools/DebugTrace.h"
#include "JetByteTools/Win32Tools/StringConverter.h"
#include "JetByteTools/Win32Tools/CompareStrings.h"
#include "JetByteTools/Win32Tools/StringVector.h"

#pragma hdrstop

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#include "JetByteTools/Win32Tools/DebugTrace.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::OutputEx;
using JetByteTools::Win32::_tstring;
using JetByteTools::Win32::CLockableObject;
using JetByteTools::Win32::CStringConverter;
using JetByteTools::Win32::FindAndReplace;
using JetByteTools::Win32::ToString;
using JetByteTools::Win32::LoadFileAsString;
using JetByteTools::Win32::SaveStringAsFile;
using JetByteTools::Win32::CException;
using JetByteTools::Win32::FindAndReplace;
using JetByteTools::Win32::InPlaceFindAndReplace;
using JetByteTools::Win32::CCompareStrings;
using JetByteTools::Win32::StringVector;

using std::string;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
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
      m_pLog(nullptr)
{

}

CTestLog::CTestLog()
   :  m_fileNumber(1),
      m_separator(_T("|")),
      m_pLog(nullptr)
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
   try
   {
      const _tstring messages = GetMessages();

      if (messages.length() != 0)
      {
         CTestMonitor::Trace(messages);
      }

      m_pLog = nullptr;
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
}

CTestLog *CTestLog::GetLogObject() const
{
   if (m_pLog)
   {
      return m_pLog;
   }

   return const_cast<CTestLog *>(this);
}


void CTestLog::UnlinkLog()
{
   if (m_pLog)
   {
      m_pLog->LogMessage(_T("Unlinked"));

      m_pLog = nullptr;
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
      CLockableObject::Owner lock(m_lock);

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
      CLockableObject::Owner lock(m_lock);

      m_log.push_back(message);
   }
}

_tstring CTestLog::GetMessages() const
{
   if (m_pLog)
   {
      return m_pLog->GetMessages();
   }

   CLockableObject::Owner lock(m_lock);

   return InternalGetMessages();
}

_tstring CTestLog::InternalGetMessages() const
{
   if (m_pLog)
   {
      return m_pLog->GetMessages();
   }

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

   CLockableObject::Owner lock(m_lock);

   return InternalRemoveMessages();
}

_tstring CTestLog::InternalRemoveMessages()
{
   _tstring result = InternalGetMessages();

   m_log.clear();

   return result;
}

void CTestLog::ResetLog()
{
   RemoveMessages(); //lint !e534 (Ignoring return value of function)
}

void CTestLog::CheckNoResults(
   const DisplayOnFailureMode displayOnFailure,
   const UseAlternativesMode useAlternatives)
{
   CheckResult(m_separator, displayOnFailure, useAlternatives);
}

void CTestLog::CheckResult(
   const _tstring &expectedResult,
   const DisplayOnFailureMode displayOnFailure,
   const UseAlternativesMode useAlternatives)
{
   CheckResult(expectedResult, m_separator + RemoveMessages(), displayOnFailure, useAlternatives);
}

void CTestLog::CheckResult(
   const _tstring &expectedResult,
   const _tstring &actualResult,
   const DisplayOnFailureMode displayOnFailure,
   const UseAlternativesMode useAlternatives) const
{
   StringVector errorMessages;

   if (!CCompareStrings::CompareStrings(
      expectedResult,
      actualResult,
      static_cast<CCompareStrings::UseAlternativesMode>(useAlternatives),
      errorMessages))
   {
      _tstring message = _T("Log does not contain expected result\n");

      for (StringVector::const_iterator it = errorMessages.begin(), end = errorMessages.end();
         it != end;
         ++it)
      {
         if (displayOnFailure == DisplayOnFailure)
         {
            OutputEx(*it);
         }

         message += *it + _T("\n");
      }

      throw CTestException(_T("CTestLog::CheckResult()"), message);
   }

   // tell the monitor of a successful check

   CTestMonitor::Trace(expectedResult);
}

void CTestLog::CheckResultA(
   const string &expectedResult,
   const DisplayOnFailureMode displayOnFailure,
   const UseAlternativesMode useAlternatives)
{
   CheckResult(CStringConverter::AtoT(expectedResult), displayOnFailure, useAlternatives);
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

      CLockableObject::Owner lock(m_lock);

      _tstring actualResults = InternalGetMessages();

      actualResults = FindAndReplace(actualResults, m_separator, lineEnd);

      const _tstring fileNameBase = GetFileName(fileName);

      try
      {
         _tstring expectedResults = LoadFileAsString(fileNameBase + _T(".log"));

         expectedResults = FindAndReplace(expectedResults, lineEnd, m_separator);

         const _tstring comparedTo = InternalRemoveMessages();

         try
         {
            CheckResult(m_separator + expectedResults, m_separator + comparedTo, DoNotDisplayOnFailure, DoNotCheckAlternatives);
         }
         catch (const CException &/*e*/)
         {
            //SaveStringAsFile(fileNameBase + _T(".Expected.log"), expectedResults);
            //SaveStringAsFile(fileNameBase + _T(".Compared.log"), comparedTo);

            throw;
         }
      }
      catch (const CException &/*e*/)
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

