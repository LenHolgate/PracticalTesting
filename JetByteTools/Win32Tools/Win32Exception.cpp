///////////////////////////////////////////////////////////////////////////////
// File: Win32Exception.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "Win32Exception.h"
#include "Utils.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CWin32Exception
///////////////////////////////////////////////////////////////////////////////

CWin32Exception::CWin32Exception(
   const _tstring &where,
   const DWORD error)
   :  CException(where, GetLastErrorMessage(error, true)),
      m_error(error)
{
}

CWin32Exception::CWin32Exception(
   const _tstring &where,
   const _tstring &message)
   :  CException(where, message),
      m_error(NO_ERROR)
{
}

CWin32Exception::CWin32Exception(
   const _tstring &where,
   const _tstring &message,
   const DWORD error)
   :  CException(where, message + _T(" - ") + GetLastErrorMessage(error, true)),
      m_error(error)
{
}

DWORD CWin32Exception::GetError() const
{
   return m_error;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Win32Exception.cpp
///////////////////////////////////////////////////////////////////////////////
