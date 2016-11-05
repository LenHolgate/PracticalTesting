///////////////////////////////////////////////////////////////////////////////
// File: StringConverter.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2003 JetByte Limited.
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

#include "StringConverter.h"
#include "Win32Exception.h"
#include "Utils.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::string;
using std::wstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CStringConverter
///////////////////////////////////////////////////////////////////////////////

// Ato

_tstring CStringConverter::AtoT(
   const string &input)
{
#ifdef _UNICODE
   return AtoW(input.c_str(), GetStringLength<int>(input));
#else
   return input;
#endif
}

_tstring CStringConverter::AtoT(
   const char *pInput)
{
   if (!pInput)
   {
      return _T("");
   }

   return AtoT(pInput, GetStringLengthAsInt(pInput));
}

_tstring CStringConverter::AtoT(
   const char *pInput,
   const int inputLength)
{
#ifdef _UNICODE
   return AtoW(pInput, inputLength);
#else
   return _tstring(pInput, inputLength);
#endif
}

wstring CStringConverter::AtoW(
   const string &input)
{
   return AtoW(input.c_str(), GetStringLength<int>(input));
}

wstring CStringConverter::AtoW(
   const char *pInput)
{
   if (!pInput)
   {
      return L"";
   }

   return AtoW(pInput, GetStringLengthAsInt(pInput));
}

wstring CStringConverter::AtoW(
   const char *pInput,
   const int inputLength)
{
   wstring result;

   if (inputLength != 0)
   {
      result.resize(inputLength);

      if (0 ==::MultiByteToWideChar(
         CP_ACP, 
         0, 
         pInput, 
         inputLength, 
         const_cast<wchar_t *>(result.c_str()), 
         GetStringLength<int>(result)))
      {
         throw CWin32Exception(_T("CStringConverter::AtoW()"), ::GetLastError());
      }
   }

   return result;
}

BSTR CStringConverter::AtoBSTR(
   const string &input)
{
   const wstring output = AtoW(input.c_str(), GetStringLength<int>(input));

   return ::SysAllocStringLen(output.c_str(), GetStringLength<UINT>(output));
}

BSTR CStringConverter::AtoBSTR(
   const char *pInput)
{
   if (!pInput)
   {
      return ::SysAllocString(L"");
   }

   return AtoBSTR(pInput, GetStringLengthAsInt(pInput));
}


BSTR CStringConverter::AtoBSTR(
   const char *pInput,
   const int inputLength)
{
   const wstring output = AtoW(pInput, inputLength);

   return ::SysAllocStringLen(output.c_str(), GetStringLength<unsigned int>(output));
}

// Wto

_tstring CStringConverter::WtoT(
   const wstring &input)
{
#ifdef _UNICODE
   return input;
#else
   return WtoA(input.c_str(), GetStringLength<int>(input));
#endif
}

_tstring CStringConverter::WtoT(
   const wchar_t *pInput)
{
   if (!pInput)
   {
      return _T("");
   }

   return WtoT(pInput, GetStringLengthAsInt(pInput));
}

_tstring CStringConverter::WtoT(
   const wchar_t *pInput,
   const int inputLength)
{
#ifdef _UNICODE
   return _tstring(pInput, inputLength);
#else
   return WtoA(pInput, inputLength);
#endif
}

string CStringConverter::WtoA(
   const wstring &input)
{
   return WtoA(input.c_str(), GetStringLength<int>(input));
}

string CStringConverter::WtoA(
   const wchar_t *pInput)
{
   if (!pInput)
   {
      return "";
   }

   return WtoA(pInput, GetStringLengthAsInt(pInput));
}

string CStringConverter::WtoA(
   const wchar_t *pInput,
   const int inputLength)
{
   string result;

   if (inputLength != 0)
   {
      result.resize(inputLength);

      if (0 == ::WideCharToMultiByte(
         CP_ACP, 
         0, 
         pInput, 
         inputLength, 
         const_cast<char *>(result.c_str()), 
         GetStringLength<int>(result, true), 0, 0))
      {
         throw CWin32Exception(_T("CStringConverter::WtoA()"), ::GetLastError());
      }
   }

   return result;
}

BSTR CStringConverter::WtoBSTR(
   const wstring &input)
{
   return ::SysAllocStringLen(input.c_str(), GetStringLengthAsUInt(input));
}

// Tto

string CStringConverter::TtoA(
   const _tstring &input)
{
#ifdef _UNICODE
   return WtoA(input.c_str(), GetStringLength<int>(input));
#else
   return input;
#endif
}

wstring CStringConverter::TtoW(
   const _tstring &input)
{
#ifdef _UNICODE
   return input;
#else
   return AtoW(input.c_str(), GetStringLength<int>(input));
#endif
}

BSTR CStringConverter::TtoBSTR(
   const _tstring &input)
{
   const wstring output = TtoW(input);

   return ::SysAllocStringLen(output.c_str(), GetStringLength<UINT>(output));
}

// BSTRto

string CStringConverter::BSTRtoA(
   const BSTR bstr)
{
   if (::SysStringLen(bstr) == 0)
   {
      return "";
   }

   return WtoA(bstr, ::SysStringLen(bstr));
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
   return WtoA(bstr, ::SysStringLen(bstr));
#endif
}

wstring CStringConverter::BSTRtoW(
   const BSTR bstr)
{
   if (::SysStringLen(bstr) == 0)
   {
      return L"";
   }

   return bstr;
}

void CStringConverter::BSTRtoA(
   const BSTR bstr,
   const char **ppResult)
{
   const size_t inputLength = ::SysStringLen(bstr);

   if (inputLength != 0)
   {
      if (inputLength + 1 > static_cast<size_t>(std::numeric_limits<int>::max()))
      {
         throw CException(_T("CStringConverter::BSTRtoA("), _T("String is too long to fit: ") + ToString(inputLength));
      }

      const int outputLength = static_cast<int>(inputLength + 1);

      char *pResult = new char[outputLength];

      if (0 == ::WideCharToMultiByte(
         CP_ACP, 
         0, 
         bstr, 
         static_cast<int>(inputLength), 
         pResult, 
         outputLength, 
         0, 
         0))
      {
         delete [] pResult;

         throw CWin32Exception(_T("CStringConverter::BSTRtoA()"), ::GetLastError());
      }

      pResult[outputLength - 1] = 0;         // Ensure that the string is null terminated.

      *ppResult = pResult;
   }
   else
   {
      *ppResult = 0;
   }
}

wstring CStringConverter::UTF8toW(
   const string &input)
{
   const int inputLength = GetStringLength<int>(input);

   wstring result;

   if (inputLength != 0)
   {
      result.resize(inputLength);

      const int numChars = ::MultiByteToWideChar(
         CP_UTF8, 
         0, 
         input.c_str(), 
         inputLength, 
         const_cast<wchar_t *>(result.c_str()), 
         GetStringLength<int>(result));

      if (0 == numChars)
      {
         throw CWin32Exception(_T("CStringConverter::UTF8toW()"), ::GetLastError());
      }

      result.resize(numChars);
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

