///////////////////////////////////////////////////////////////////////////////
// File: StringConverter.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2003 JetByte Limited.
//
// JetByte Limited grants you ("Licensee") a non-exclusive, royalty free, 
// licence to use, modify and redistribute this software in source and binary 
// code form, provided that i) this copyright notice and licence appear on all 
// copies of the software; and ii) Licensee does not utilize the software in a 
// manner which is disparaging to JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All 
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors 
// shall not be liable for any damages suffered by licensee as a result of 
// using, modifying or distributing the software or its derivatives. In no
// event will JetByte Limited be liable for any lost revenue, profit or data,
// or for direct, indirect, special, consequential, incidental or punitive
// damages, however caused and regardless of the theory of liability, arising 
// out of the use of or inability to use software, even if JetByte Limited 
// has been advised of the possibility of such damages.
//
// This software is not designed or intended for use in on-line control of 
// aircraft, air traffic, aircraft navigation or aircraft communications; or in 
// the design, construction, operation or maintenance of any nuclear 
// facility. Licensee represents and warrants that it will not use or 
// redistribute the Software for such purposes. 
//
///////////////////////////////////////////////////////////////////////////////

#include "StringConverter.h"

#include <atlbase.h>       // USES_CONVERSION

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -save
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CStringConverter
///////////////////////////////////////////////////////////////////////////////

_tstring CStringConverter::AtoT(
   const std::string &input)
{
   USES_CONVERSION;

   return A2T(const_cast<char *>(input.c_str()));
}

std::string CStringConverter::TtoA(
   const _tstring &input)
{
   USES_CONVERSION;

   return T2A(const_cast<TCHAR *>(input.c_str()));
}

std::wstring CStringConverter::TtoW(
   const _tstring &input)
{
   USES_CONVERSION;

   return T2W(const_cast<TCHAR *>(input.c_str()));
}

std::wstring CStringConverter::AtoW(
   const std::string &input)
{
   USES_CONVERSION;

   return A2W(const_cast<char *>(input.c_str()));
}

_tstring CStringConverter::WtoT(
   const std::wstring &input)
{
   USES_CONVERSION;

   return W2T(const_cast<wchar_t *>(input.c_str()));
}


///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -restore
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// End of file: StringConverter.cpp
///////////////////////////////////////////////////////////////////////////////

