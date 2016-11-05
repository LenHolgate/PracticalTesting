#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_DEBUG_TRACE_INCLUDED__
#define JETBYTE_TOOLS_DEBUG_TRACE_INCLUDED__
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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

///////////////////////////////////////////////////////////////////////////////
// CDebugTrace
///////////////////////////////////////////////////////////////////////////////

/// \ingroup DebugTrace
/// \ingroup Logging

class CDebugTrace : public CMessageLog
{
   public :

      static CDebugTrace &Instance();

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
   CDebugTrace::Instance().SetLogName(s);
}

/// \ingroup DebugTrace

inline void SetLogFileName(
   const std::wstring &s)
{
   CDebugTrace::Instance().SetLogName(s);
}

/// \ingroup DebugTrace

inline void OutputEx(
   const std::string &s)
{
   CDebugTrace::Instance().LogMessage(s);
}

/// \ingroup DebugTrace

inline void OutputEx(
   const std::wstring &s)
{
   CDebugTrace::Instance().LogMessage(s);
}

/// \ingroup DebugTrace

inline void OutputEx(
   const char * const pString)
{
   CDebugTrace::Instance().LogMessage(pString);
}

/// \ingroup DebugTrace

inline void OutputEx(
   const wchar_t * const pString)
{
   CDebugTrace::Instance().LogMessage(pString);
}

/// \ingroup DebugTrace

inline void OutputEx(
   const char * const pString,
   const ILogMessages::DataLength stringLength)
{
   CDebugTrace::Instance().LogMessage(pString, stringLength);
}

/// \ingroup DebugTrace

inline void OutputEx(
   const wchar_t * const pString,
   const ILogMessages::DataLength stringLength)
{
   CDebugTrace::Instance().LogMessage(pString, stringLength);
}

#ifdef _DEBUG

/// \ingroup DebugTrace

inline void Output(
   const std::string &s)
{
   OutputEx(s);
}

/// \ingroup DebugTrace

inline void Output(
   const std::wstring &s)
{
   OutputEx(s);
}

/// \ingroup DebugTrace

inline void Output(
   const char * const pString)
{
   OutputEx(pString);
}

/// \ingroup DebugTrace

inline void Output(
   const wchar_t * const pString)
{
   OutputEx(pString);
}

/// \ingroup DebugTrace

inline void Output(
   const char * const pString,
   const ILogMessages::DataLength stringLength)
{
   OutputEx(pString, stringLength);
}

/// \ingroup DebugTrace

inline void Output(
   const wchar_t * const pString,
   const ILogMessages::DataLength stringLength)
{
   OutputEx(pString, stringLength);
}

#else

/// \ingroup DebugTrace

inline void Output(
   const std::string & /*s*/)
{
}

/// \ingroup DebugTrace

inline void Output(
   const std::wstring & /*s*/)
{
}

/// \ingroup DebugTrace

inline void Output(
   const char * const /*pString*/)
{
}

/// \ingroup DebugTrace

inline void Output(
   const wchar_t * const /*pString*/)
{
}

/// \ingroup DebugTrace

inline void Output(
   const char * const /*pString*/,
   const ILogMessages::DataLength /*stringLength*/)
{
}

/// \ingroup DebugTrace

inline void Output(
   const wchar_t * const /*pString*/,
   const ILogMessages::DataLength /*stringLength*/)
{
}

#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_DEBUG_TRACE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: DebugTrace.h 
///////////////////////////////////////////////////////////////////////////////