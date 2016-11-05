#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

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

#include "JetByteTools\C++Tools\ILogMessages.h"

#include "tstring.h"
#include "CriticalSection.h"
#include "ThreadLocalStorage.h"

#include <fstream>
#include <list>

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
         LogToAll                = 0x0111
      };

      CSimpleMessageLog();

      explicit CSimpleMessageLog(
         const _tstring &fileName,
         const LogTargets targets = LogToAll);

      // Implement ILogMessages

      virtual void SetThreadIdentifier(
         const std::string &identifier);

      virtual void SetThreadIdentifier(
         const std::wstring &identifier);

      virtual void SetLogName(
         const std::string &name);

      virtual void SetLogName(
         const std::wstring &name);

      virtual void LogMessage(
         const std::string &message);

      virtual void LogMessage(
         const std::wstring &message);

      virtual void LogMessage(
         const char * const pString);

      virtual void LogMessage(
         const wchar_t * const pString);

      virtual void LogMessage(
         const char * const pString,
         const DataLength stringLength);

      virtual void LogMessage(
         const wchar_t * const pString,
         const DataLength stringLength);

   private :

      const std::string &GetThreadID();

      const std::string *SetThreadID(
         const std::string &threadIdentifier);

      CCriticalSection m_criticalSection;

      CThreadLocalStorage m_tls;

      std::fstream m_output;

      std::string m_fileName;

      const bool m_logToCOUT;
      const bool m_logToOutputDebugString;
      const bool m_logToFile;

      typedef std::list<std::string> ThreadIDs;

      ThreadIDs m_threadIDs;

      /// No copies do not implement
      CSimpleMessageLog(const CSimpleMessageLog &rhs);
      /// No copies do not implement
      CSimpleMessageLog &operator=(const CSimpleMessageLog &rhs);
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
