#pragma once
#ifndef JETBYTE_TOOLS_NULL_MESSAGE_LOG_INCLUDED__
#define JETBYTE_TOOLS_NULL_MESSAGE_LOG_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: NullMessageLog.h
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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CNullMessageLog
///////////////////////////////////////////////////////////////////////////////

/// An object that implements ILogMessages and does nothing.
/// \ingroup NullObj
/// \ingroup Logging

class CNullMessageLog : public ILogMessages
{
   public :

      void SetThreadIdentifier(
         const std::string & /*identifier*/) override {}

      void SetThreadIdentifier(
         const std::wstring & /*identifier*/) override {}

      void SetLogName(
         const std::string & /*name*/) override {}

      void SetLogName(
         const std::wstring & /*name*/) override {}

      void LogMessage(
         const std::string & /*message*/) override {}

      void LogMessage(
         const std::wstring & /*message*/) override {}

      void LogMessage(
         const char * const /*pString*/) override {}

      void LogMessage(
         const wchar_t * const /*pString*/) override {}

      void LogMessage(
         const char * const /*pString*/,
         const DataLength /*stringLength*/) override {}

      void LogMessage(
         const wchar_t * const /*pString*/,
         const DataLength /*stringLength*/) override {}
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_NULL_MESSAGE_LOG_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: NullMessageLog.h
///////////////////////////////////////////////////////////////////////////////
