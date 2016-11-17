///////////////////////////////////////////////////////////////////////////////
// File: Exception.cpp
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

#include "Exception.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CException
///////////////////////////////////////////////////////////////////////////////

CException::CException(
   const _tstring &where, 
   const _tstring &message)
   :  m_where(where), 
      m_message(message)
{
}

_tstring CException::GetWhere() const 
{ 
   return m_where; 
}

_tstring CException::GetMessage() const 
{ 
   return m_message; 
}

void CException::MessageBox(HWND hWnd /* = nullptr */) const
{ 
   ::MessageBox(hWnd, GetMessage().c_str(), GetWhere().c_str(), MB_ICONSTOP); 
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Exception.cpp
///////////////////////////////////////////////////////////////////////////////
