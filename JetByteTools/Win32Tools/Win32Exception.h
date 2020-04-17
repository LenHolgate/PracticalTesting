#pragma once
#ifndef JETBYTE_TOOLS_WIN32_WIN32_EXCEPTION__
#define JETBYTE_TOOLS_WIN32_WIN32_EXCEPTION__
///////////////////////////////////////////////////////////////////////////////
// File: Win32Exception.h
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

#include "Exception.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CWin32Exception
///////////////////////////////////////////////////////////////////////////////

/// A simple exception class to replace Win32 error returns.
/// \ingroup Exceptions

class CWin32Exception : public CException
{
   public :

      CWin32Exception(
         const _tstring &where,
         DWORD error);

      CWin32Exception(
         const _tstring &where,
         const _tstring &message);

      CWin32Exception(
         const _tstring &where,
         const _tstring &message,
         DWORD error);

      DWORD GetError() const;

   protected :

      DWORD m_error;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_WIN32_EXCEPTION__

///////////////////////////////////////////////////////////////////////////////
// End of file: Win32Exception.h
///////////////////////////////////////////////////////////////////////////////
