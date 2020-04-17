///////////////////////////////////////////////////////////////////////////////
// File: CompareStrings.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2017 JetByte Limited.
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

#include "CompareStrings.h"
#include "Utils.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

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
// CCompareStrings
///////////////////////////////////////////////////////////////////////////////

bool CCompareStrings::CompareStrings(
   const _tstring &expectedResult,
   const _tstring &actualResult,
   const UseAlternativesMode useAlternatives,
   StringVector &errorMessages)
{
   bool ok = false;

   if (((useAlternatives & CheckAlternatives) == CheckAlternatives) && _tstring::npos != expectedResult.find(_T('{')))
   {
      // Optional, eg "{|Release|}", try as "|Release|" and ""

      const _tstring expectedResultWithoutOptional = RemoveOptionalTags(expectedResult);

      ok = DoCompareStrings(expectedResultWithoutOptional, actualResult, useAlternatives);

      if (!ok)
      {
         const _tstring expectedResultWithOptional = RemoveTagMarks(expectedResult);

         ok = DoCompareStrings(expectedResultWithOptional, actualResult, useAlternatives);

         if (!ok)
         {
            errorMessages.push_back(_T("  result: ") + actualResult);
            errorMessages.push_back(_T("expected: ") + expectedResultWithOptional);
            errorMessages.push_back(_T("      or: ") + expectedResultWithoutOptional);
         }
      }
   }
   else if (((useAlternatives & CheckAlternatives) == CheckAlternatives) && _tstring::npos != expectedResult.find(_T("<<<")))
   {
      // alternate, eg <<<|Release>>>|AddRef<<<|Release>>>|, try as "|Release|AddRef|" and "|AddRef|Release|"

      const _tstring firstAlternative = RemoveSecondAlternative(expectedResult);

      ok = DoCompareStrings(firstAlternative, actualResult, useAlternatives);

      if (!ok)
      {
         const _tstring secondAlternative = RemoveFirstAlternative(expectedResult);

         ok =DoCompareStrings(secondAlternative, actualResult, useAlternatives);

         if (!ok)
         {
            errorMessages.push_back(_T("  result: ") + actualResult);
            errorMessages.push_back(_T("expected: ") + firstAlternative);
            errorMessages.push_back(_T("      or: ") + secondAlternative);
         }
      }
   }
   else
   {
      ok = DoCompareStrings(expectedResult, actualResult, useAlternatives);

      if (!ok)
      {
         errorMessages.push_back(_T("  result: ") + actualResult);
         errorMessages.push_back(_T("expected: ") + expectedResult);
      }
   }

   return ok;
}

bool CCompareStrings::DoCompareStrings(
   const _tstring &expectedResult,
   const _tstring &actualResult,
   const UseAlternativesMode useAlternatives)
{
   bool ok = true;

   if ((useAlternatives & CheckRepeats) == CheckRepeats && _tstring::npos != expectedResult.find(_T("[[")))
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

               done = !DoCheckWildcards(repeatPattern, actualPattern, useAlternatives);

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

         ok = DoCheckWildcards(expandedExpectedResult, actualResult, useAlternatives);
      }
   }
   else
   {
      ok = DoCheckWildcards(expectedResult, actualResult, useAlternatives);
   }

   return ok;
}

bool CCompareStrings::DoCheckWildcards(
   const _tstring &expectedResult,
   const _tstring &actualResult,
   const UseAlternativesMode useAlternatives)
{
   bool ok = true;

   if ((useAlternatives & CheckWildcards) == CheckWildcards)
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
      const _tstring::size_type end = output.find(endTag, pos);

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
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CompareStrings.cpp
///////////////////////////////////////////////////////////////////////////////
