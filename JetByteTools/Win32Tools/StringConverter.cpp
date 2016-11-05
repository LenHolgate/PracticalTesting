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

#include "JetByteTools\Admin\Admin.h"

#include "StringConverter.h"
#include "Win32Exception.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions 
///////////////////////////////////////////////////////////////////////////////

static std::wstring InternalAtoW(
   const char *pInput,
   const size_t inputLength);

static std::string InternalWtoA(
   const wchar_t *pInput,
   const size_t inputLength);

///////////////////////////////////////////////////////////////////////////////
// CStringConverter
///////////////////////////////////////////////////////////////////////////////

_tstring CStringConverter::AtoT(
   const std::string &input)
{
#ifdef _UNICODE
	return AtoW(input);
#else
	return input;
#endif
}

std::string CStringConverter::TtoA(
   const _tstring &input)
{
#ifdef _UNICODE
	return WtoA(input);
#else
	return input;
#endif
}

std::wstring CStringConverter::TtoW(
   const _tstring &input)
{
#ifdef _UNICODE
	return input;
#else
	return AtoW(input);
#endif
}

_tstring CStringConverter::WtoT(
   const std::wstring &input)
{
#ifdef _UNICODE
	return input;
#else
	return WtoA(input);
#endif
}

std::string CStringConverter::BSTRtoA(
	const BSTR bstr)
{
	if (::SysStringLen(bstr) == 0)
	{
		return "";
	}

   return InternalWtoA(bstr, ::SysStringLen(bstr));
}

_tstring CStringConverter::BSTRtoT(
	const BSTR bstr)
{
	if (::SysStringLen(bstr) == 0)
	{
		return _T("");
	}

#ifdef _UNICODE
   return bstr;
#else
	return InternalWtoA(bstr, ::SysStringLen(bstr));
#endif
}

std::wstring CStringConverter::BSTRtoW(
	const BSTR bstr)
{
	if (::SysStringLen(bstr) == 0)
	{
		return L"";
	}

   return bstr;
}

std::wstring CStringConverter::AtoW(
   const std::string &input)
{
	return InternalAtoW(input.c_str(), input.length());
}

std::string CStringConverter::WtoA(
   const std::wstring &input)
{
	return InternalWtoA(input.c_str(), input.length());
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions 
///////////////////////////////////////////////////////////////////////////////

static std::wstring InternalAtoW(
   const char *pInput,
   const size_t inputLength)
{
	std::wstring result;

   if (inputLength != 0)
   {
	   result.resize(inputLength);

	   if (0 ==::MultiByteToWideChar(CP_ACP, 0, pInput, inputLength, const_cast<wchar_t *>(result.c_str()), result.length()))
	   {
		   throw CWin32Exception(_T("CStringConverter::AtoW()"), ::GetLastError());
	   }
   }

	return result;
}

static std::string InternalWtoA(
   const wchar_t *pInput,
   const size_t inputLength)
{
	std::string result;

	if (inputLength != 0)
	{
		result.resize(inputLength);

		if (0 == ::WideCharToMultiByte(CP_ACP, 0, pInput, inputLength, const_cast<char *>(result.c_str()), result.length() + 1, 0, 0))
		{
			throw CWin32Exception(_T("CStringConverter::WtoA()"), ::GetLastError());
		}
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: StringConverter.cpp
///////////////////////////////////////////////////////////////////////////////

