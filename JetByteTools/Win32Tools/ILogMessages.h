#pragma once
#ifndef JETBYTE_TOOLS_WIN32_I_LOG_MESSAGES_INCLUDED__
#define JETBYTE_TOOLS_WIN32_I_LOG_MESSAGES_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ILogMessages.h
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

#include <string>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// ILogMessages
///////////////////////////////////////////////////////////////////////////////

/// An interface for logging messages. These can be debug trace messages or
/// more structured logging. The messages are sent to a log, which can be
/// pretty much anything.
/// \ingroup Interfaces
/// \ingroup Logging
/// \ingroup ProtectedDestructors

class ILogMessages
{
   public :

      /// Sets the identifier used in the log for the current thread to the
      /// supplied string. The thread identifier defaults to the thread id
      /// in the form "XXX: ", the string supplied as an identifier does not
      /// need to include the ": " as this will be appended automatically.
      /// It can be any string that will help you to identify the thread
      /// concerned.

      virtual void SetThreadIdentifier(
         const std::string &identifier) = 0;

      /// Sets the identifier used in the log for the current thread to the
      /// supplied string. The thread identifier defaults to the thread id
      /// in the form "XXX: ", the string supplied as an identifier does not
      /// need to include the ": " as this will be appended automatically.
      /// It can be any string that will help you to identify the thread
      /// concerned.

      virtual void SetThreadIdentifier(
         const std::wstring &identifier) = 0;

      /// Sets the name of the log file to the supplied name. Note that this
      /// creates a new log file with the specified name, it does NOT rename
      /// the existing log file.
      /// Note that you should manually synchronise access to this function
      /// and the various LogMessage and IWaitable calls.

      virtual void SetLogName(
         const std::string &name) = 0;

      /// Sets the name of the log file to the supplied name. Note that this
      /// creates a new log file with the specified name, it does NOT rename
      /// the existing log file.
      /// Note that you should manually synchronise access to this function
      /// and the various LogMessage and IWaitable calls.

      virtual void SetLogName(
         const std::wstring &name) = 0;

      typedef unsigned long DataLength;

      /// Logs the message to the log.
      /// Note that you should manually synchronise access to this function
      /// and the various SetLogName calls.

      virtual void LogMessage(
         const std::string &message) = 0;

      /// Logs the message to the log.
      /// Note that you should manually synchronise access to this function
      /// and the various SetLogName calls.

      virtual void LogMessage(
         const std::wstring &message) = 0;

      /// Logs the message to the log.
      /// Note that you should manually synchronise access to this function
      /// and the various SetLogName calls.

      virtual void LogMessage(
         const char *pString) = 0;

      /// Logs the message to the log.
      /// Note that you should manually synchronise access to this function
      /// and the various SetLogName calls.

      virtual void LogMessage(
         const wchar_t *pString) = 0;

      /// Logs the message to the log.
      /// Note that you should manually synchronise access to this function
      /// and the various SetLogName calls.

      virtual void LogMessage(
         const char *pString,
         DataLength stringLength) = 0;

      /// Logs the message to the log.
      /// Note that you should manually synchronise access to this function
      /// and the various SetLogName calls.

      virtual void LogMessage(
         const wchar_t *pString,
         DataLength stringLength) = 0;

   protected :

      /// We never delete instances of this interface; you must manage the
      /// lifetime of the class that implements it.

      virtual ~ILogMessages() = default;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_I_LOG_MESSAGES_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ILogMessages.h
///////////////////////////////////////////////////////////////////////////////
