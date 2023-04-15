#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: DebugTrace.h
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

#include "MessageLog.h"

#include "tstring.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Debugging defines...
///////////////////////////////////////////////////////////////////////////////

#ifndef DEBUG_ONLY
#ifdef _DEBUG
#define DEBUG_ONLY(x)   x
#else
#define DEBUG_ONLY(x)
#endif
#endif

///////////////////////////////////////////////////////////////////////////////
// CDebugTrace
///////////////////////////////////////////////////////////////////////////////

/// \ingroup DebugTrace
/// \ingroup Logging

class CDebugTrace : public CMessageLog
{
   public :

      static bool IsValid();

      static CDebugTrace &Instance();

      /// A class that takes an instance of ILogMessages and installs it as
      /// the message log used by the CDebugTrace class and then removes it
      /// and reinstalls the previous log in its destructor. It can, therefore,
      /// be used to support \ref RAII "scope based" log installation and
      /// removal.
      /// \ingroup Logging
      /// \ingroup RAII

      class LogInstaller
      {
         public :

            explicit LogInstaller(
               ILogMessages *pOptionalLog);

            explicit LogInstaller(
               ILogMessages &log);

            LogInstaller(
               const LogInstaller &rhs) = delete;

            ~LogInstaller();

            LogInstaller &operator=(
               const LogInstaller &rhs) = delete;

            void Uninstall();

         private :

            ILogMessages *m_pOldLog;
      };

   private :

      CDebugTrace();
};

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

/// \ingroup DebugTrace

inline void SetLogFileName(
   const std::string &s)
{
   if (CDebugTrace::IsValid())
   {
      CDebugTrace::Instance().SetLogName(s);
   }
}

/// \ingroup DebugTrace

inline void SetLogFileName(
   const std::wstring &s)
{
   if (CDebugTrace::IsValid())
   {
      CDebugTrace::Instance().SetLogName(s);
   }
}

/// \ingroup DebugTrace

inline void OutputEx(
   const ILogMessages::VectorOfLines &s)
{
   if (CDebugTrace::IsValid())
   {
      CDebugTrace::Instance().LogMessage(s);
   }
}

/// \ingroup DebugTrace

inline void OutputEx(
   const ILogMessages::DequeOfLines &s)
{
   if (CDebugTrace::IsValid())
   {
      CDebugTrace::Instance().LogMessage(s);
   }
}

/// \ingroup DebugTrace

inline void OutputEx(
   const std::string &s)
{
   if (CDebugTrace::IsValid())
   {
      CDebugTrace::Instance().LogMessage(s);
   }
}

#ifndef JETBYTE_DISABLE_WIDE_DEBUG_TRACE

/// \ingroup DebugTrace

inline void OutputEx(
   const std::wstring &s)
{
   if (CDebugTrace::IsValid())
   {
      CDebugTrace::Instance().LogMessage(s);
   }
}

#endif

/// \ingroup DebugTrace

inline void OutputEx(
   const char * const pString)
{
   if (CDebugTrace::IsValid())
   {
      CDebugTrace::Instance().LogMessage(pString);
   }
}

#ifndef JETBYTE_DISABLE_WIDE_DEBUG_TRACE

/// \ingroup DebugTrace

inline void OutputEx(
   const wchar_t * const pString)
{
   if (CDebugTrace::IsValid())
   {
      CDebugTrace::Instance().LogMessage(pString);
   }
}

#endif

/// \ingroup DebugTrace

inline void OutputEx(
   const char * const pString,
   const ILogMessages::DataLength stringLength)
{
   if (CDebugTrace::IsValid())
   {
      CDebugTrace::Instance().LogMessage(pString, stringLength);
   }
}

#ifndef JETBYTE_DISABLE_WIDE_DEBUG_TRACE

/// \ingroup DebugTrace

inline void OutputEx(
   const wchar_t * const pString,
   const ILogMessages::DataLength stringLength)
{
   if (CDebugTrace::IsValid())
   {
      CDebugTrace::Instance().LogMessage(pString, stringLength);
   }
}

#endif

#ifdef _DEBUG

/// \ingroup DebugTrace

inline void Output(
   const std::string &s)
{
   OutputEx(s);
}

#ifndef JETBYTE_DISABLE_WIDE_DEBUG_TRACE

/// \ingroup DebugTrace

inline void Output(
   const std::wstring &s)
{
   OutputEx(s);
}

#endif

/// \ingroup DebugTrace

inline void Output(
   const char * const pString)
{
   OutputEx(pString);
}

#ifndef JETBYTE_DISABLE_WIDE_DEBUG_TRACE

/// \ingroup DebugTrace

inline void Output(
   const wchar_t * const pString)
{
   OutputEx(pString);
}

#endif

/// \ingroup DebugTrace

inline void Output(
   const char * const pString,
   const ILogMessages::DataLength stringLength)
{
   OutputEx(pString, stringLength);
}

#ifndef JETBYTE_DISABLE_WIDE_DEBUG_TRACE

/// \ingroup DebugTrace

inline void Output(
   const wchar_t * const pString,
   const ILogMessages::DataLength stringLength)
{
   OutputEx(pString, stringLength);
}

#endif

#else

/// \ingroup DebugTrace

inline void Output(
   const std::string &s)
{
#if (JETBYTE_CORE_ENABLE_DEBUG_TRACE_OUTPUT == 1)
   OutputEx(s);
#else
   (void)s;
#endif
}

#ifndef JETBYTE_DISABLE_WIDE_DEBUG_TRACE

/// \ingroup DebugTrace

inline void Output(
   const std::wstring &s)
{
#if (JETBYTE_CORE_ENABLE_DEBUG_TRACE_OUTPUT == 1)
   OutputEx(s);
#else
   (void)s;
#endif
}

#endif

/// \ingroup DebugTrace

inline void Output(
   const char * const pString)
{
#if (JETBYTE_CORE_ENABLE_DEBUG_TRACE_OUTPUT == 1)
   OutputEx(pString);
#else
   (void)pString;
#endif
}

#ifndef JETBYTE_DISABLE_WIDE_DEBUG_TRACE

/// \ingroup DebugTrace

inline void Output(
   const wchar_t * const pString)
{
#if (JETBYTE_CORE_ENABLE_DEBUG_TRACE_OUTPUT == 1)
   OutputEx(pString);
#else
   (void)pString;
#endif
}

#endif

/// \ingroup DebugTrace

inline void Output(
   const char * const pString,
   const ILogMessages::DataLength stringLength)
{
#if (JETBYTE_CORE_ENABLE_DEBUG_TRACE_OUTPUT == 1)
   OutputEx(pString, stringLength);
#else
   (void)pString;
   (void)stringLength;
#endif
}

#ifndef JETBYTE_DISABLE_WIDE_DEBUG_TRACE
/// \ingroup DebugTrace

inline void Output(
   const wchar_t * const pString,
   const ILogMessages::DataLength stringLength)
{
#if (JETBYTE_CORE_ENABLE_DEBUG_TRACE_OUTPUT == 1)
   OutputEx(pString, stringLength);
#else
   (void)pString;
   (void)stringLength;
#endif
}

#endif

#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: DebugTrace.h
///////////////////////////////////////////////////////////////////////////////
