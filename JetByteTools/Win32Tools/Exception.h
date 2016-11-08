#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

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

#include <wtypes.h>

#include "tstring.h"

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

      CException(
         const _tstring &where, 
         const _tstring &message);

      virtual ~CException() {}

      virtual _tstring GetWhere() const;

      virtual _tstring GetMessage() const; 

      void MessageBox(
         HWND hWnd = NULL) const; 

   protected :
      
      const _tstring m_where;
      const _tstring m_message;
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
