#pragma once
#ifndef JETBYTE_TOOLS_WIN32_SIMPLE_MESSAGE_LOG_INCLUDED__
#define JETBYTE_TOOLS_WIN32_SIMPLE_MESSAGE_LOG_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: SimpleMessageLog.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2007 JetByte Limited.
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

#include "ILogMessages.h"

#include "tstring.h"
#include "LockableObject.h"
#include "ThreadLocalStorage.h"

#include <fstream>
#include <deque>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CSimpleMessageLog
///////////////////////////////////////////////////////////////////////////////

/// An object that implements ILogMessages and
/// \ingroup Logging

class CSimpleMessageLog : public ILogMessages
{
   public :

      enum LogTargets
      {
         LogToCOUT               = 0x0001,
         LogToOutputDebugString  = 0x0010,
         LogToFile               = 0x0100,
         LogToAll                = 0x0111,
         LogToMemoryBuffer       = 0x1000,
         LogToAllAndMemory       = 0x1111
      };

      enum LogEntryFormat
      {
         JustMessage           = 0x000,
         IncludeThreadId       = 0x001,
         IncludeTimestamp      = 0x010
      };

      CSimpleMessageLog();

      explicit CSimpleMessageLog(
         const _tstring &fileName,
         DWORD targets = LogToAll,
         DWORD logEntryFormat = IncludeThreadId);

      CSimpleMessageLog(
         const CSimpleMessageLog &rhs) = delete;

      CSimpleMessageLog &operator=(
         const CSimpleMessageLog &rhs) = delete;

      _tstring GetLogMessages() const;

      // Implement ILogMessages

      void SetThreadIdentifier(
         const std::string &identifier) override;

      void SetThreadIdentifier(
         const std::wstring &identifier) override;

      void SetLogName(
         const std::string &fileName) override;

      void SetLogName(
         const std::wstring &fileName) override;

      void LogMessage(
         const std::string &message) override;

      void LogMessage(
         const std::wstring &message) override;

      void LogMessage(
         const char *pString) override;

      void LogMessage(
         const wchar_t *pString) override;

      void LogMessage(
         const char *pString,
         DataLength stringLength) override;

      void LogMessage(
         const wchar_t *pString,
         DataLength stringLength) override;

   private :

      std::string GetTimestamp() const;

      const std::string &GetThreadID();

      const std::string *InternalSetThreadIdentifier(
         const std::string &threadIdentifier);

      mutable CLockableObject m_lock;

      CThreadLocalStorage m_tls;

      std::fstream m_output;

      std::string m_fileName;

      const bool m_logToCOUT;
      const bool m_logToOutputDebugString;
      const bool m_logToFile;
      const bool m_logToMemory;

      const LogEntryFormat m_logEntryFormat;

      typedef std::deque<std::string> ThreadIDs;

      ThreadIDs m_threadIDs;

      typedef std::deque<std::string> Messages;

      Messages m_messages;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_SIMPLE_MESSAGE_LOG_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: SimpleMessageLog.h
///////////////////////////////////////////////////////////////////////////////
