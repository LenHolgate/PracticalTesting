#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: SimpleMessageLog.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2007 JetByte Limited.
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

#include "ILogMessages.h"
#include "IListenToThreadNaming.h"

#include "tstring.h"
#include "LockableObject.h"
#include "ThreadLocalStorage.h"

#include <fstream>
#include <deque>
#include <map>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// CSimpleMessageLog
///////////////////////////////////////////////////////////////////////////////

/// An object that implements ILogMessages and
/// \ingroup Logging

class CSimpleMessageLog : public ILogMessages, private IListenToThreadNaming
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
         JustMessage                = 0x000,
         IncludeThreadId            = 0x001,
         IncludeSequentialThreadId  = 0x002,
         IncludeThreadName          = 0x004,
         IncludeTimestamp           = 0x010,
         NoNewLogBanner             = 0x100
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

      _tstring MapThreadIdToLogThreadId(
         const _tstring &threadId);

      std::string MapThreadIdToLogThreadIdA(
         const std::string &threadId);

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
         const VectorOfLines &messages) override;

      void LogMessage(
         const DequeOfLines &messages) override;

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

      // Implement IListenToThreadNaming

      void OnThreadNaming(
         const _tstring &threadId,
         const _tstring &threadName) override;

      void LogMessageInternal(
         const std::string &message);

      std::string GetTimestamp() const;

      const std::string &GetThreadID();

      const std::string *InternalSetThreadIdentifier(
         const std::string &id,
         const std::string &identifier);

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

      typedef std::map<std::string, std::string> ThreadIdMap;

      ThreadIdMap m_threadIdMap;

      ThreadIdMap m_threadNameMap;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: SimpleMessageLog.h
///////////////////////////////////////////////////////////////////////////////
