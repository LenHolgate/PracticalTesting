///////////////////////////////////////////////////////////////////////////////
// File: CodePage.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "CodePage.h"
#include "Win32Exception.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CodePage
///////////////////////////////////////////////////////////////////////////////

CCodePage::CCodePage(
   const unsigned int codePage)
{
   ::GetCPInfoEx(codePage, 0, this);
}

unsigned int CCodePage::GetCodePage() const
{
   return CodePage;
}

bool CCodePage::isSingleByteCharacterSet() const
{
   return MaxCharSize == 1;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CodePage.cpp
///////////////////////////////////////////////////////////////////////////////

