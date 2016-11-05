#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

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

#include "JetByteTools\Win32Tools\tstring.h"
#include "JetByteTools\Win32Tools\LockableObject.h"

#include <vector>
#include <string>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
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

      ~CTestLog();

      void UnlinkLog();

      void ClearLog();

      void LogMessage(
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
         DoNotCheckAlternatives     = 0x000,
         CheckWildcards             = 0x001,
         CheckAlternatives          = 0x010,
         CheckRepeats               = 0x100,
         CheckAllAlternatives       = 0x111
      };

      void ResetLog();

      void CheckNoResults(
         const DisplayOnFailureMode displayOnFailure = DisplayOnFailure,
         const UseAlternativesMode useAlternatives = DoNotCheckAlternatives);

      void CheckResult(
         const JetByteTools::Win32::_tstring &expectedResult,
         const DisplayOnFailureMode displayOnFailure = DisplayOnFailure,
         const UseAlternativesMode useAlternatives = DoNotCheckAlternatives);

      void CheckResultA(
         const std::string &expectedResult,
         const DisplayOnFailureMode displayOnFailure = DisplayOnFailure,
         const UseAlternativesMode useAlternatives = DoNotCheckAlternatives);

      void CheckResultFromFile(
         const JetByteTools::Win32::_tstring &fileName,
         const bool fileContainsLineEndBars = false);

   protected :

      void CheckResult(
         const JetByteTools::Win32::_tstring &expectedResult,
         const JetByteTools::Win32::_tstring &actualResult,
         DisplayOnFailureMode displayOnFailure,
         const UseAlternativesMode useAlternatives);

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

      /// No copies do not implement
      CTestLog(const CTestLog &rhs);
      /// No copies do not implement
      CTestLog &operator=(const CTestLog &rhs);
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
