#pragma once
#ifndef JETBYTE_TOOLS_WIN32_EXCEPTION__
#define JETBYTE_TOOLS_WIN32_EXCEPTION__
///////////////////////////////////////////////////////////////////////////////
// File: Exception.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 1997 JetByte Limited.
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

#include "JetByteTools/Admin/Types.h"

#include "tstring.h"

#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)
#include "CallStack.h"
#endif

#include <wtypes.h>     // for HWND

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CException
///////////////////////////////////////////////////////////////////////////////

/// A simple exception class.
/// \ingroup Exceptions

class CException
{
   public :

      #ifdef _UNICODE
      CException(
         const std::string &where,
         const std::string &message);
      #endif

      CException(
         const _tstring &where,
         const _tstring &message);

      virtual ~CException() = default;

      virtual _tstring GetWhere() const;

      virtual _tstring GetMessage() const;

      virtual _tstring GetDetails() const;

      virtual std::string GetDetailsA() const;

      virtual _tstring GetCallStack() const;

      void MessageBox(
         HWND hWnd = nullptr) const;

   protected :

      const _tstring m_where;
      const _tstring m_message;
      const _tstring m_details;

#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)

      CCallStack m_stack;

#endif
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_EXCEPTION__

///////////////////////////////////////////////////////////////////////////////
// End of file: Exception.h
///////////////////////////////////////////////////////////////////////////////
