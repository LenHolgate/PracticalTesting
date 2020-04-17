#pragma once
#ifndef JETBYTE_TOOLS_TEST_TEST_LOG_INCLUDED__
#define JETBYTE_TOOLS_TEST_TEST_LOG_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: TestLog.h
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

#include "JetByteTools/Win32Tools/tstring.h"
#include "JetByteTools/Win32Tools/LockableObject.h"
#include "JetByteTools/Win32Tools/CompareStrings.h"

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
         const JetByteTools::Win32::_tstring &separator);

      CTestLog();

      explicit CTestLog(
         CTestLog *pLinkedLog);

      CTestLog(
         CTestLog *pLinkedLog,
         const JetByteTools::Win32::_tstring &separator);

      CTestLog(
         const CTestLog &rhs) = delete;

      virtual ~CTestLog();

      CTestLog &operator=(
         const CTestLog &rhs) = delete;

      CTestLog *GetLogObject() const;

      void UnlinkLog();

      void ClearLog();

      virtual void LogMessage(
         const JetByteTools::Win32::_tstring &message) const;

      JetByteTools::Win32::_tstring GetMessages() const;

      JetByteTools::Win32::_tstring RemoveMessages();

      enum DisplayOnFailureMode
      {
         DisplayOnFailure,
         DoNotDisplayOnFailure
      };

      enum UseAlternativesMode
      {
         DoNotCheckAlternatives     = JetByteTools::Win32::CCompareStrings::DoNotCheckAlternatives,
         CheckWildcards             = JetByteTools::Win32::CCompareStrings::CheckWildcards,
         CheckAlternatives          = JetByteTools::Win32::CCompareStrings::CheckAlternatives,
         CheckRepeats               = JetByteTools::Win32::CCompareStrings::CheckRepeats,
         CheckAllAlternatives       = JetByteTools::Win32::CCompareStrings::CheckAllAlternatives
      };

      void ResetLog();

      void CheckNoResults(
         DisplayOnFailureMode displayOnFailure = DisplayOnFailure,
         UseAlternativesMode useAlternatives = DoNotCheckAlternatives);

      void CheckResult(
         const JetByteTools::Win32::_tstring &expectedResult,
         DisplayOnFailureMode displayOnFailure = DisplayOnFailure,
         UseAlternativesMode useAlternatives = DoNotCheckAlternatives);

      void CheckResultA(
         const std::string &expectedResult,
         DisplayOnFailureMode displayOnFailure = DisplayOnFailure,
         UseAlternativesMode useAlternatives = DoNotCheckAlternatives);

      void CheckResultFromFile(
         const JetByteTools::Win32::_tstring &fileName,
         bool fileContainsLineEndBars = false);

   protected :

      void CheckResult(
         const JetByteTools::Win32::_tstring &expectedResult,
         const JetByteTools::Win32::_tstring &actualResult,
         DisplayOnFailureMode displayOnFailure,
         UseAlternativesMode useAlternatives) const;

      JetByteTools::Win32::_tstring GetFileName(
         const JetByteTools::Win32::_tstring &fileName);

   private :

      JetByteTools::Win32::_tstring InternalGetMessages() const;

      JetByteTools::Win32::_tstring InternalRemoveMessages();

      unsigned int m_fileNumber;

      const JetByteTools::Win32::_tstring m_separator;

      mutable JetByteTools::Win32::CLockableObject m_lock;

      typedef std::vector<JetByteTools::Win32::_tstring> Log;

      mutable Log m_log;

      CTestLog *m_pLog;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_TEST_TEST_LOG_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CTestLog.h
///////////////////////////////////////////////////////////////////////////////
