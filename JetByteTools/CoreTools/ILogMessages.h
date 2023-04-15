#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ILogMessages.h
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

#include <string>
#include <vector>
#include <deque>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

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

      /// Logs the messages to the log.
      /// Note that you should manually synchronise access to this function
      /// and the various SetLogName calls.

      typedef std::vector<std::string> VectorOfLines;

      virtual void LogMessage(
         const VectorOfLines &messages) = 0;

      /// Logs the messages to the log.
      /// Note that you should manually synchronise access to this function
      /// and the various SetLogName calls.

      typedef std::deque<std::string> DequeOfLines;

      virtual void LogMessage(
         const DequeOfLines &messages) = 0;

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
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ILogMessages.h
///////////////////////////////////////////////////////////////////////////////
