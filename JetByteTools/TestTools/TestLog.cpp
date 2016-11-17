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

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#include "JetByteTools\Win32Tools\DebugTrace.h"
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

using std::string;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static _tstring RemoveOptionalTags(
   const _tstring &input);

static _tstring RemoveTagMarks(
   const _tstring &input);

static _tstring RemoveFirstAlternative(
   const _tstring &input);

static _tstring RemoveSecondAlternative(
   const _tstring &input);

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

static bool CheckWildcards(
   const _tstring &expectedResult,
   const _tstring &actualResult,
   const CTestLog::UseAlternativesMode useAlternatives)
{
   bool ok = true;

   if ((useAlternatives & CTestLog::CheckWildcards) == CTestLog::CheckWildcards)
   {
      const _tstring::size_type expectedLength = expectedResult.length();

      const _tstring::size_type actualLength = actualResult.length();

      _tstring::size_type actualStart = 0;

      _tstring::size_type expectedStart = 0;

      _tstring::size_type wildCardStart = expectedResult.find(_T("@@@"));

      _tstring::size_type matchLength = expectedLength;

      if (wildCardStart != _tstring::npos)
      {
         matchLength = wildCardStart - expectedStart;
      }

      while (ok && matchLength != 0)
      {
         const _tstring expectedMatch = expectedResult.substr(expectedStart, matchLength);

         const _tstring actualMatch = actualResult.substr(actualStart, matchLength);

         ok = (actualMatch == expectedMatch);

         if (ok && (expectedStart + matchLength < expectedLength))
         {
            // Find the length of the next match

            const _tstring::size_type newExpectedStart = expectedStart + matchLength + 3;

            wildCardStart = expectedResult.find(_T("@@@"), newExpectedStart);

            matchLength = expectedLength - newExpectedStart;

            if (wildCardStart != _tstring::npos)
            {
               matchLength = wildCardStart - newExpectedStart;
            }

            // Skip the wildcard data in the actual result by scanning to the next
            // non wildcard character.

            const TCHAR nextExpectedChar = expectedResult[newExpectedStart];

            _tstring::size_type i = actualStart + actualMatch.length();

            while (i < actualLength &&
                     actualResult[i] != nextExpectedChar)
            {
               ++i;
            }

            if (actualResult[i] != nextExpectedChar)
            {
               ok = false;
            }
            else
            {
               actualStart = i;

               expectedStart = newExpectedStart;
            }
         }
         else
         {
            matchLength = 0;
         }
      }
   }
   else
   {
      ok = (expectedResult == actualResult);
   }

   return ok;
}

static bool CheckResultInternal(
   const _tstring &expectedResult,
   const _tstring &actualResult,
   const CTestLog::UseAlternativesMode useAlternatives)
{
   bool ok = true;

   if ((useAlternatives & CTestLog::CheckRepeats) == CTestLog::CheckRepeats && _tstring::npos != expectedResult.find(_T("[[")))
   {
      // TOOD - be able to specify a number (min and max?) [[0]]MATCHME[[2]] (0-2 instances of MATCHME) [[2]]MATCHME[[5]] (2-5 instances)?

      // repeats are specified like this [[|AddRef|Release]] and can match 1 or more repeats of the enclosed phrase before
      // continuing with the rest of the string

      // build the correct 'expected' string by expanding the repeats to the correct number.

      //const _tstring::size_type expectedLength = expectedResult.length();

      const _tstring::size_type actualLength = actualResult.length();

      _tstring::size_type actualStart = 0;

      _tstring::size_type repeatStart = expectedResult.find(_T("[["));

      _tstring expandedExpectedResult;

      while (ok && repeatStart != _tstring::npos)
      {
         size_t patternMatchCount = 0;

         repeatStart += 2;

         const _tstring::size_type repeatEnd = expectedResult.find(_T("]]"), repeatStart);

         const _tstring::size_type repeatLength = (repeatEnd != _tstring::npos) ? repeatEnd - repeatStart : 0;

         const _tstring repeatPattern = expectedResult.substr(repeatStart, repeatLength);

         bool done = false;

         while (ok && !done)
         {
            if (actualStart + repeatLength > actualLength)
            {
               // not enough data in 'actualResult' to match the pattern...

               if (patternMatchCount == 0)
               {
                  // Must match at least once!
                  ok = false;
               }
               else
               {
                  done = true;
               }
            }

            if (ok && !done)
            {
               // actual pattern can be longer/shorter than repeatPattern if repeat contains @@@ wildcards
               // need to work with remaining actualResult and compare wildcards from pattern and get 
               // given the amount of actual consumed by the match

               const _tstring actualPattern = actualResult.substr(actualStart, repeatLength);

               done = !CheckWildcards(repeatPattern, actualPattern, useAlternatives);

               if (!done)
               {
                  patternMatchCount++;

                  actualStart += repeatLength;

                  expandedExpectedResult += repeatPattern;
               }
               else if (patternMatchCount == 0)
               {
                  ok = false;
               }
            }
         }

         const _tstring::size_type endOfRepeatTag = repeatEnd + 2;

         const _tstring::size_type nextStart = expectedResult.find(_T("[["), endOfRepeatTag);

         expandedExpectedResult += expectedResult.substr(endOfRepeatTag, nextStart);

         repeatStart = nextStart;
      }

      if (ok)
      {
         if (expandedExpectedResult.length() == 0)
         {
            expandedExpectedResult = expectedResult;
         }

         ok = CheckWildcards(expandedExpectedResult, actualResult, useAlternatives);
      }
   }
   else
   {
      ok = CheckWildcards(expectedResult, actualResult, useAlternatives);
   }

   return ok;
}

void CTestLog::CheckResult(
   const _tstring &expectedResult,
   const _tstring &actualResult,
   const DisplayOnFailureMode displayOnFailure,
   const UseAlternativesMode useAlternatives)
{
   if (((useAlternatives & CheckAlternatives) == CheckAlternatives) && _tstring::npos != expectedResult.find(_T("{")))
   {
      // Optional, eg "{|Release|}", try as "|Release|" and ""

      const _tstring expectedResultWithoutOptional = RemoveOptionalTags(expectedResult);

      if (!CheckResultInternal(expectedResultWithoutOptional, actualResult, useAlternatives))
      {
         const _tstring expectedResultWithOptional = RemoveTagMarks(expectedResult);

         if (!CheckResultInternal(expectedResultWithOptional, actualResult, useAlternatives))
         {
            if (displayOnFailure == DisplayOnFailure)
            {
               OutputEx(_T("  result: ") + actualResult);
               OutputEx(_T("expected: ") + expectedResultWithOptional);
               OutputEx(_T("      or: ") + expectedResultWithoutOptional);
            }

            throw CTestException(_T("CTestLog::CheckResult()"),
               _T("Log does not contain expected result\n")
               _T("  result: ") + actualResult + _T("\n")
               _T("expected: ") + expectedResultWithOptional + _T("\n")
               _T("      or: ") + expectedResultWithoutOptional);
         }
      }
   }
   else if (((useAlternatives & CheckAlternatives) == CheckAlternatives) && _tstring::npos != expectedResult.find(_T("<<<")))
   {
      // alternate, eg <<<|Release>>>|AddRef<<<|Release>>>|, try as "|Release|AddRef|" and "|AddRef|Release|"

      const _tstring firstAlternative = RemoveSecondAlternative(expectedResult);

      if (!CheckResultInternal(firstAlternative, actualResult, useAlternatives))
      {
         const _tstring secondAlternative = RemoveFirstAlternative(expectedResult);

         if (!CheckResultInternal(secondAlternative, actualResult, useAlternatives))
         {
            if (displayOnFailure == DisplayOnFailure)
            {
               OutputEx(_T("  result: ") + actualResult);
               OutputEx(_T("expected: ") + firstAlternative);
               OutputEx(_T("      or: ") + secondAlternative);
            }

            throw CTestException(_T("CTestLog::CheckResult()"),
               _T("Log does not contain expected result\n")
               _T("  result: ") + actualResult + _T("\n")
               _T("expected: ") + firstAlternative + _T("\n")
               _T("      or: ") + secondAlternative);
         }
      }
   }
   else if (!CheckResultInternal(expectedResult, actualResult, useAlternatives))
   {
      if (displayOnFailure == DisplayOnFailure)
      {
         OutputEx(_T("  result: ") + actualResult);
         OutputEx(_T("expected: ") + expectedResult);
      }

      throw CTestException(_T("CTestLog::CheckResult()"),
         _T("Log does not contain expected result\n")
         _T("result:   ") + actualResult + _T("\n")
         _T("expected: ") + expectedResult);
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
         catch(const CException &/*e*/)
         {
            //SaveStringAsFile(fileNameBase + _T(".Expected.log"), expectedResults);
            //SaveStringAsFile(fileNameBase + _T(".Compared.log"), comparedTo);

            throw;
         }
      }
      catch(const CException &/*e*/)
      {
         SaveStringAsFile(fileNameBase + _T(".Actual.log"), actualResults);

         throw;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static _tstring RemoveOptionalTags(
   const _tstring &input)
{
   _tstring output = input;

   const _tstring startTag = _T("{");

   const _tstring endTag = _T("}");

   _tstring::size_type pos = output.find(startTag);

   while (pos != _tstring::npos)
   {
      _tstring::size_type end = output.find(endTag, pos);

      output.erase(pos, (end - pos) + 1);

      pos = output.find(startTag, end + 1);
   }

   return output;
}

static _tstring RemoveTagMarks(
   const _tstring &input)
{
   _tstring output = FindAndReplace(input, _T("{"), _T(""));

   InPlaceFindAndReplace(output, _T("}"), _T(""));

   return output;
}

static _tstring RemoveFirstAlternative(
   const _tstring &input)
{
   const _tstring::size_type startPos = input.find(_T("<<<"));

   if (startPos != _tstring::npos)
   {
      const _tstring::size_type stopPos = input.find(_T(">>>"), startPos);

      if (stopPos != _tstring::npos)
      {
         const _tstring::size_type secondStartPos = input.find(_T("<<<"), stopPos);

         if (secondStartPos != _tstring::npos)
         {
            const _tstring::size_type secondStopPos = input.find(_T(">>>"), secondStartPos);

            if (secondStopPos != _tstring::npos)
            {
               _tstring output = input;

               output.replace(startPos, (stopPos - startPos) + 3, _T(""));

               InPlaceFindAndReplace(output, _T("<<<"), _T(""));
               InPlaceFindAndReplace(output, _T(">>>"), _T(""));

               return output;
            }
         }
      }
   }

   return input;
}

static _tstring RemoveSecondAlternative(
   const _tstring &input)
{
   const _tstring::size_type startPos = input.find(_T("<<<"));

   if (startPos != _tstring::npos)
   {
      const _tstring::size_type stopPos = input.find(_T(">>>"), startPos);

      if (stopPos != _tstring::npos)
      {
         const _tstring::size_type secondStartPos = input.find(_T("<<<"), stopPos);

         if (secondStartPos != _tstring::npos)
         {
            const _tstring::size_type secondStopPos = input.find(_T(">>>"), secondStartPos);

            if (secondStopPos != _tstring::npos)
            {
               _tstring output = input;

               output.replace(secondStartPos, (secondStopPos - secondStartPos) + 3, _T(""));

               InPlaceFindAndReplace(output, _T("<<<"), _T(""));
               InPlaceFindAndReplace(output, _T(">>>"), _T(""));

               return output;
            }
         }
      }
   }

   return input;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: TestLog.cpp
///////////////////////////////////////////////////////////////////////////////

