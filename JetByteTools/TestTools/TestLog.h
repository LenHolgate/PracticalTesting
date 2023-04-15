#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: TestLog.h
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

#include "JetByteTools/CoreTools/tstring.h"
#include "JetByteTools/CoreTools/LockableObject.h"
#include "JetByteTools/CoreTools/CompareStrings.h"

#include <vector>
#include <string>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CTestLog
///////////////////////////////////////////////////////////////////////////////

class CTestLog
{
   public :

      explicit CTestLog(
         const JetByteTools::Core::_tstring &separator);

      CTestLog();

      explicit CTestLog(
         CTestLog *pLinkedLog);

      CTestLog(
         CTestLog *pLinkedLog,
         const JetByteTools::Core::_tstring &separator);

      CTestLog(
         const CTestLog &rhs) = delete;

      virtual ~CTestLog();

      CTestLog &operator=(
         const CTestLog &rhs) = delete;

      CTestLog *GetLogObject() const;

      void UnlinkLog();

      void ClearLog();

      virtual void LogMessage(
         const JetByteTools::Core::_tstring &message) const;

      JetByteTools::Core::_tstring GetMessages() const;

      JetByteTools::Core::_tstring RemoveMessages();

      enum DisplayOnFailureMode
      {
         DisplayOnFailure,
         DoNotDisplayOnFailure
      };

      enum UseAlternativesMode
      {
         DoNotCheckAlternatives     = JetByteTools::Core::CCompareStrings::DoNotCheckAlternatives,
         CheckWildcards             = JetByteTools::Core::CCompareStrings::CheckWildcards,
         CheckAlternatives          = JetByteTools::Core::CCompareStrings::CheckAlternatives,
         CheckRepeats               = JetByteTools::Core::CCompareStrings::CheckRepeats,
         CheckAllAlternatives       = JetByteTools::Core::CCompareStrings::CheckAllAlternatives
      };

      void ResetLog();

      void CheckNoResults(
         DisplayOnFailureMode displayOnFailure = DisplayOnFailure,
         UseAlternativesMode useAlternatives = DoNotCheckAlternatives);

      void CheckResult(
         const JetByteTools::Core::_tstring &expectedResult,
         DisplayOnFailureMode displayOnFailure = DisplayOnFailure,
         UseAlternativesMode useAlternatives = DoNotCheckAlternatives);

      void CheckResultA(
         const std::string &expectedResult,
         DisplayOnFailureMode displayOnFailure = DisplayOnFailure,
         UseAlternativesMode useAlternatives = DoNotCheckAlternatives);

      void CheckResultFromFile(
         const JetByteTools::Core::_tstring &fileName,
         bool fileContainsLineEndBars = false);

   protected :

      void CheckResult(
         const JetByteTools::Core::_tstring &expectedResult,
         const JetByteTools::Core::_tstring &actualResult,
         DisplayOnFailureMode displayOnFailure,
         UseAlternativesMode useAlternatives) const;

      JetByteTools::Core::_tstring GetFileName(
         const JetByteTools::Core::_tstring &fileName);

   private :

      JetByteTools::Core::_tstring InternalGetMessages() const;

      JetByteTools::Core::_tstring InternalRemoveMessages();

      unsigned int m_fileNumber;

      const JetByteTools::Core::_tstring m_separator;

      mutable JetByteTools::Core::CLockableObject m_lock;

      typedef std::vector<JetByteTools::Core::_tstring> Log;

      mutable Log m_log;

      CTestLog *m_pLog;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CTestLog.h
///////////////////////////////////////////////////////////////////////////////
