///////////////////////////////////////////////////////////////////////////////
// File: TestLog.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2003 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#include "JetByteTools/Admin/Admin.h"

#include "TestLog.h"
#include "TestException.h"
#include "TestMonitor.h"

#include <iostream>

#include "JetByteTools/CoreTools/StringUtils.h"
#include "JetByteTools/CoreTools/FileUtils.h"
#include "JetByteTools/CoreTools/DebugTrace.h"
#include "JetByteTools/CoreTools/StringConverter.h"
#include "JetByteTools/CoreTools/CompareStrings.h"
#include "JetByteTools/CoreTools/StringVector.h"

#pragma hdrstop

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#include "JetByteTools/CoreTools/DebugTrace.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Core::OutputEx;
using JetByteTools::Core::_tstring;
using JetByteTools::Core::CLockableObject;
using JetByteTools::Core::CStringConverter;
using JetByteTools::Core::FindAndReplace;
using JetByteTools::Core::ToString;
using JetByteTools::Core::CException;
using JetByteTools::Core::InPlaceFindAndReplace;
using JetByteTools::Core::CCompareStrings;
using JetByteTools::Core::StringVector;
using JetByteTools::Core::LoadFileAsString;
using JetByteTools::Core::SaveStringAsFile;

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
   RemoveMessages();
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

      for (const auto &errorMessage : errorMessages)
      {
         if (displayOnFailure == DisplayOnFailure)
         {
            OutputEx(errorMessage);
         }

         message += errorMessage + _T("\n");
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
