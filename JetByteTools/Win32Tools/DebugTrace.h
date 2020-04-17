#pragma once
#ifndef JETBYTE_TOOLS_WIN32_DEBUG_TRACE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_DEBUG_TRACE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: DebugTrace.h
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

#include "MessageLog.h"

#include "StringConverter.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

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
               ILogMessages &log);

            LogInstaller(
               const LogInstaller &rhs) = delete;

            ~LogInstaller();

            LogInstaller &operator=(
               const LogInstaller &rhs) = delete;

            //lint -sem(JetByteTools::Win32::CDebugTrace::LogInstaller::Uninstall,cleanup)
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
   const std::string & /*s*/)
{
}

#ifndef JETBYTE_DISABLE_WIDE_DEBUG_TRACE

/// \ingroup DebugTrace

inline void Output(
   const std::wstring & /*s*/)
{
}

#endif

/// \ingroup DebugTrace

inline void Output(
   const char * const /*pString*/)
{
}

#ifndef JETBYTE_DISABLE_WIDE_DEBUG_TRACE

/// \ingroup DebugTrace

inline void Output(
   const wchar_t * const /*pString*/)
{
}

#endif

/// \ingroup DebugTrace

inline void Output(
   const char * const /*pString*/,
   const ILogMessages::DataLength /*stringLength*/)
{
}

#ifndef JETBYTE_DISABLE_WIDE_DEBUG_TRACE
/// \ingroup DebugTrace

inline void Output(
   const wchar_t * const /*pString*/,
   const ILogMessages::DataLength /*stringLength*/)
{
}

#endif

#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_DEBUG_TRACE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: DebugTrace.h
///////////////////////////////////////////////////////////////////////////////
