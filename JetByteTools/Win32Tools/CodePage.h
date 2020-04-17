#pragma once
#ifndef JETBYTE_TOOLS_WIN32_CODE_PAGE__
#define JETBYTE_TOOLS_WIN32_CODE_PAGE__
///////////////////////////////////////////////////////////////////////////////
// File: CodePage.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2011 JetByte Limited.
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

#include <Windows.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CCodePage
///////////////////////////////////////////////////////////////////////////////

/// \ingroup Win32
/// \ingroup Unicode

class CCodePage : public CPINFOEX
{
   public :

      explicit CCodePage(
         unsigned int codePage);

      unsigned int GetCodePage() const;

      bool isSingleByteCharacterSet() const;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_CODE_PAGE__

///////////////////////////////////////////////////////////////////////////////
// End of file: CodePage.h
///////////////////////////////////////////////////////////////////////////////

