///////////////////////////////////////////////////////////////////////////////
// File: StringConverter.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2003 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#include "JetByteTools/Admin/Admin.h"

#include "StringConverter.h"

#include "JetByteTools/CoreTools/CheckedStaticCast.h"
#include "JetByteTools/CoreTools/StringUtils.h"
#include "JetByteTools/CoreTools/ErrorCodeException.h"

#pragma hdrstop

#include <algorithm>          // for min

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::CCodePage;

using std::string;
using std::wstring;
using std::numeric_limits;
using std::min;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

const CCodePage CStringConverter::s_CP_ACP(CP_ACP);

///////////////////////////////////////////////////////////////////////////////
// CStringConverter
///////////////////////////////////////////////////////////////////////////////

// Ato

int CStringConverter::GetSpaceRequiredForAtoT(
   const string &input,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return GetSpaceRequiredForAtoW(input, codePage);
#else
   (void)codePage;
   return GetStringLength<int>(input, true);
#endif
}

int CStringConverter::GetSpaceRequiredForAtoT(
   const char *pInput,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return GetSpaceRequiredForAtoW(pInput, codePage);
#else
   (void)codePage;
   return GetStringLength<int>(pInput, true);
#endif
}

int CStringConverter::GetSpaceRequiredForAtoT(
   const char *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return GetSpaceRequiredForAtoW(pInput, inputLength, codePage);
#else
   (void)codePage;
   (void)pInput;
   return inputLength;
#endif
}

_tstring CStringConverter::AtoT(
   const string &input,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return AtoW(input.c_str(), GetStringLength<int>(input), codePage);
#else
   (void)codePage;
   return input;
#endif
}

_tstring CStringConverter::AtoT(
   const char *pInput,
   const CCodePage &codePage)
{
   if (!pInput)
   {
      return _T("");
   }

   return AtoT(pInput, GetStringLength<int>(pInput), codePage);
}

size_t CStringConverter::AtoT(
   void *pBuffer,
   const size_t size,
   const char *pInput,
   const CCodePage &codePage)
{
   if (!pInput)
   {
      return 0;
   }

   return AtoT(pBuffer, size, pInput, GetStringLength<int>(pInput), codePage);
}

_tstring CStringConverter::AtoT(
   const char *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return AtoW(pInput, inputLength, codePage);
#else
   (void)codePage;
   return _tstring(pInput, inputLength);
#endif
}

size_t CStringConverter::AtoT(
   void *pBuffer,
   const size_t size,
   const char *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return AtoW(pBuffer, size, pInput, inputLength, codePage);
#else
   (void)codePage;

   if (static_cast<size_t>(inputLength) > size)
   {
      throw "TODO";  // what about terminating null???
   }

   memcpy(pBuffer, pInput, inputLength);

   return inputLength;
#endif
}

int CStringConverter::GetSpaceRequiredForAtoW(
   const string &input,
   const CCodePage &codePage)
{
   const auto length = GetStringLength<int>(input, true);

   if (codePage.isSingleByteCharacterSet())
   {
      return length * sizeof(wchar_t);
   }

   return GetSpaceRequiredForAtoW(input.c_str(), length, codePage);
}

int CStringConverter::GetSpaceRequiredForAtoW(
   const char *pInput,
   const CCodePage &codePage)
{
   const auto length = GetStringLength<int>(pInput, true);

   if (codePage.isSingleByteCharacterSet())
   {
      return length * sizeof(wchar_t);
   }

   return GetSpaceRequiredForAtoW(pInput, length, codePage);
}

int CStringConverter::GetSpaceRequiredForAtoW(
   const char *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
   if (codePage.isSingleByteCharacterSet())
   {
      return inputLength * sizeof(wchar_t);
   }

   const int bytesRequired = MultiByteToWideChar(
      codePage.GetCodePage(),
      0,
      pInput,
      inputLength,
      nullptr,
      0);

   if (bytesRequired == 0)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("CStringConverter::GetSpaceRequiredForAtoW()"), lastError);
   }

   return bytesRequired;
}

wstring CStringConverter::AtoW(
   const string &input,
   const CCodePage &codePage)
{
   return AtoW(input.c_str(), GetStringLength<int>(input), codePage);
}

wstring CStringConverter::AtoW(
   const char *pInput,
   const CCodePage &codePage)
{
   if (!pInput)
   {
      return L"";
   }

   return AtoW(pInput, GetStringLength<int>(pInput), codePage);
}

size_t CStringConverter::AtoW(
   void *pBuffer,
   const size_t size,
   const char *pInput,
   const CCodePage &codePage)
{
   if (!pInput)
   {
      return 0;
   }

   return AtoW(pBuffer, size, pInput, GetStringLength<int>(pInput), codePage);
}

wstring CStringConverter::AtoW(
   const char *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
   wstring result;

   if (inputLength != 0)
   {
      if (codePage.isSingleByteCharacterSet())
      {
         result.resize(static_cast<size_t>(inputLength));
      }
      else
      {
         const int bytesRequired = MultiByteToWideChar(
            codePage.GetCodePage(),
            0,
            pInput,
            inputLength,
            nullptr,
            0);

         if (bytesRequired == 0)
         {
            const DWORD lastError = GetLastError();

            throw CErrorCodeException(_T("CStringConverter::AtoW()"), lastError);
         }

         result.resize(bytesRequired);
      }

      if (0 == MultiByteToWideChar(
         codePage.GetCodePage(),
         0,
         pInput,
         inputLength,
         const_cast<wchar_t *>(result.c_str()),
         GetStringLength<int>(result, true)))
      {
         throw CErrorCodeException(_T("CStringConverter::AtoW()"), GetLastError());
      }
   }

   return result;
}

size_t CStringConverter::AtoW(
   void *pBuffer,
   const size_t size,
   const char *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
   if (inputLength != 0)
   {
      const int charsRequired = MultiByteToWideChar(
         codePage.GetCodePage(),
         0,
         pInput,
         inputLength,
         static_cast<LPWSTR>(pBuffer),
         checked_static_cast<int>(size / sizeof(wchar_t)));

      if (0 == charsRequired)
      {
         throw CErrorCodeException(_T("CStringConverter::AtoW()"), GetLastError());
      }

      return charsRequired * sizeof(wchar_t);
   }
 
   return 0;
}

BSTR CStringConverter::AtoBSTR(
   const string &input,
   const CCodePage &codePage)
{
   const wstring output = AtoW(input.c_str(), GetStringLength<int>(input), codePage);

   return SysAllocStringLen(output.c_str(), GetStringLength<UINT>(output));
}

BSTR CStringConverter::AtoBSTR(
   const char *pInput,
   const CCodePage &codePage)
{
   if (!pInput)
   {
      return SysAllocString(L"");
   }

   return AtoBSTR(pInput, GetStringLength<int>(pInput), codePage);
}

BSTR CStringConverter::AtoBSTR(
   const char *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
   const wstring output = AtoW(pInput, inputLength, codePage);

   return SysAllocStringLen(output.c_str(), GetStringLength<unsigned int>(output));
}

int CStringConverter::AtoUTF8(
   const string &input,
   BYTE *pBuffer,
   const int bufferSize,
   const CCodePage &codePage)
{
   const wstring wideInput = AtoW(input, codePage);

   return WtoUTF8(wideInput, pBuffer, bufferSize);
}

int CStringConverter::AtoUTF8(
   const char *pInput,
   const int inputLength,
   BYTE *pBuffer,
   const int bufferSize,
   const CCodePage &codePage)
{
   const wstring wideInput = AtoW(pInput, inputLength, codePage);

   return WtoUTF8(wideInput.c_str(), GetStringLength<int>(wideInput), pBuffer, bufferSize);
}

int CStringConverter::GetSpaceRequiredForAtoUTF8(
   const string &input,
   const CCodePage &codePage)
{
   const wstring wideInput = AtoW(input, codePage);

   return GetSpaceRequiredForWtoUTF8(wideInput);
}

int CStringConverter::GetSpaceRequiredForAtoUTF8(
   const char *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
   const wstring wideInput = AtoW(pInput, inputLength, codePage);

   return GetSpaceRequiredForWtoUTF8(wideInput.c_str(), GetStringLength<int>(wideInput));
}

// Wto

int CStringConverter::GetSpaceRequiredForWtoT(
   const wstring &input,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return GetStringLength<int>(input, true) * sizeof(wchar_t);
#else
   return GetSpaceRequiredForWtoA(input, codePage);
#endif
}

int CStringConverter::GetSpaceRequiredForWtoT(
   const wchar_t *pInput,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return GetStringLength<int>(pInput, true) * sizeof(wchar_t);
#else
   return GetSpaceRequiredForWtoA(pInput, codePage);
#endif
}

int CStringConverter::GetSpaceRequiredForWtoT(
   const wchar_t *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   (void)pInput;
   return inputLength * sizeof(wchar_t);
#else
   return GetSpaceRequiredForWtoA(pInput, inputLength, codePage);
#endif
}

_tstring CStringConverter::WtoT(
   const wstring &input,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return input;
#else
   return WtoA(input.c_str(), GetStringLength<int>(input), codePage);
#endif
}

_tstring CStringConverter::WtoT(
   const wchar_t *pInput,
   const CCodePage &codePage)
{
   if (!pInput)
   {
      return _T("");
   }

   return WtoT(pInput, GetStringLength<int>(pInput), codePage);
}

size_t CStringConverter::WtoT(
   void *pBuffer,
   const size_t size,
   const wchar_t *pInput,
   const CCodePage &codePage)
{
   if (!pInput)
   {
      return 0;
   }

   return WtoT(pBuffer, size, pInput, GetStringLength<int>(pInput), codePage);
}

_tstring CStringConverter::WtoT(
   const wchar_t *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return _tstring(pInput, inputLength);
#else
   return WtoA(pInput, inputLength, codePage);
#endif
}

size_t CStringConverter::WtoT(
   void *pBuffer,
   const size_t size,
   const wchar_t *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;

   const size_t byteLength = inputLength * sizeof(wchar_t);

   if (byteLength > size)
   {
      throw CException(_T("CStringConverter::WtoT()"), _T("byteLength > size; what about terminating null?"));
   }

   memcpy(pBuffer, pInput, byteLength);

   return byteLength;
#else
   return WtoA(pBuffer, size, pInput, inputLength, codePage);
#endif
}

int CStringConverter::GetSpaceRequiredForWtoA(
   const wstring &input,
   const CCodePage &codePage)
{
   const auto length = GetStringLength<int>(input, true);

   if (codePage.isSingleByteCharacterSet())
   {
      return length * sizeof(wchar_t);
   }

   return GetSpaceRequiredForWtoA(input.c_str(), length, codePage);
}

int CStringConverter::GetSpaceRequiredForWtoA(
   const wchar_t *pInput,
   const CCodePage &codePage)
{
   const auto length = GetStringLength<int>(pInput, true);

   if (codePage.isSingleByteCharacterSet())
   {
      return length * sizeof(wchar_t);
   }

   return GetSpaceRequiredForWtoA(pInput, length, codePage);
}

int CStringConverter::GetSpaceRequiredForWtoA(
   const wchar_t *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
   if (codePage.isSingleByteCharacterSet())
   {
      return inputLength * sizeof(wchar_t);
   }

   const int bytesRequired = WideCharToMultiByte(
      codePage.GetCodePage(),
      0,
      pInput,
      inputLength,
      nullptr,
      0,
      nullptr,
      nullptr);

   if (bytesRequired == 0)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("CStringConverter::GetSpaceRequiredForWtoA()"), lastError);
   }

   return bytesRequired;
}

string CStringConverter::WtoA(
   const wstring &input,
   const CCodePage &codePage)
{
   return WtoA(input.c_str(), GetStringLength<int>(input), codePage);
}

string CStringConverter::WtoA(
   const wchar_t *pInput,
   const CCodePage &codePage)
{
   if (!pInput)
   {
      return "";
   }

   return WtoA(pInput, GetStringLength<int>(pInput), codePage);
}

size_t CStringConverter::WtoA(
   void *pBuffer,
   const size_t size,
   const wchar_t *pInput,
   const CCodePage &codePage)
{
   if (!pInput)
   {
      return 0;
   }

   return WtoA(pBuffer, size, pInput, GetStringLength<int>(pInput), codePage);
}

string CStringConverter::WtoA(
   const wchar_t *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
   string result;

   if (inputLength != 0)
   {
      if (codePage.isSingleByteCharacterSet())
      {
         result.resize(inputLength);
      }
      else
      {
         const int bytesRequired = WideCharToMultiByte(
            codePage.GetCodePage(),
            0,
            pInput,
            inputLength,
            nullptr,
            0,
            nullptr,
            nullptr);

         if (bytesRequired == 0)
         {
            const DWORD lastError = GetLastError();

            throw CErrorCodeException(_T("CStringConverter::WtoA()"), lastError);
         }

         result.resize(bytesRequired);
      }

      if (0 == WideCharToMultiByte(
         codePage.GetCodePage(),
         0,
         pInput,
         inputLength,
         const_cast<char *>(result.c_str()),
         GetStringLength<int>(result, true),
         nullptr,
         nullptr))
      {
         throw CErrorCodeException(_T("CStringConverter::WtoA()"), GetLastError());
      }
   }

   return result;
}

size_t CStringConverter::WtoA(
   void *pBuffer,
   const size_t size,
   const wchar_t *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
   if (inputLength != 0)
   {
      const int charsRequired = WideCharToMultiByte(
         codePage.GetCodePage(),
         0,
         pInput,
         inputLength,
         static_cast<char *>(pBuffer),
         checked_static_cast<int>(size),           // hmm, actually just min(size, max int val)
         nullptr,
         nullptr);

      if (0 == charsRequired)
      {
         throw CErrorCodeException(_T("CStringConverter::WtoA()"), GetLastError());
      }

      return charsRequired;
   }
 
   return 0;
}

BSTR CStringConverter::WtoBSTR(
   const wstring &input)
{
   return SysAllocStringLen(input.c_str(), GetStringLength<UINT>(input));
}

int CStringConverter::WtoUTF8(
   const wstring &input,
   BYTE *pBuffer,
   const int bufferSize)
{
   return WtoUTF8(input.c_str(), GetStringLength<int>(input, true), pBuffer, bufferSize);
}

int CStringConverter::WtoUTF8(
   const wchar_t *pInput,
   const int inputLength,
   BYTE *pBuffer,
   const int bufferSize)
{
   int bufferUsed = 0;

   if (inputLength != 0)
   {
      bufferUsed = WideCharToMultiByte(
         CP_UTF8,
         WC_ERR_INVALID_CHARS,
         pInput,
         inputLength,
         reinterpret_cast<LPSTR>(pBuffer),
         bufferSize,
         nullptr,
         nullptr);

      if (0 == bufferUsed)
      {
         const DWORD lastError = GetLastError();

         throw CErrorCodeException(_T("CStringConverter::WtoUTF8()"), lastError);
      }
   }

   return bufferUsed;
}

int CStringConverter::GetSpaceRequiredForWtoUTF8(
   const wstring &input)
{
   return GetSpaceRequiredForWtoUTF8(input.c_str(), GetStringLength<int>(input, true));
}

int CStringConverter::GetSpaceRequiredForWtoUTF8(
   const wchar_t *pInput,
   const int inputLength)
{
   int spaceRequired = 0;

   if (inputLength != 0)
   {
      spaceRequired = WideCharToMultiByte(
         CP_UTF8,
         WC_ERR_INVALID_CHARS,
         pInput,
         inputLength,
         nullptr,
         0,
         nullptr,
         nullptr);

      if (0 == spaceRequired)
      {
         const DWORD lastError = GetLastError();

         throw CErrorCodeException(_T("CStringConverter::GetSpaceRequiredForWtoUTF8()"), lastError);
      }
   }

   return spaceRequired;
}

// Tto

string CStringConverter::TtoA(
   const _tstring &input,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return WtoA(input.c_str(), GetStringLength<int>(input), codePage);
#else
   (void)codePage;
   return input;
#endif
}

string CStringConverter::TtoA(
   const TCHAR *pInput,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM

   if (!pInput)
   {
      return "";
   }

   return WtoA(pInput, GetStringLength<int>(pInput), codePage);
#else
   (void)codePage;
   return pInput;
#endif
}

string CStringConverter::TtoA(
   const TCHAR *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return WtoA(pInput, inputLength, codePage);
#else
   (void)codePage;
   return string(pInput, inputLength);
#endif
}

wstring CStringConverter::TtoW(
   const _tstring &input,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return input;
#else
   return AtoW(input.c_str(), GetStringLength<int>(input), codePage);
#endif
}

wstring CStringConverter::TtoW(
   const TCHAR *pInput,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return pInput;
#else

   if (!pInput)
   {
      return L"";
   }

   return AtoW(pInput, GetStringLength<int>(pInput), codePage);
#endif
}

wstring CStringConverter::TtoW(
   const TCHAR *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return wstring(pInput, inputLength);
#else
   return AtoW(pInput, inputLength, codePage);
#endif
}

BSTR CStringConverter::TtoBSTR(
   const _tstring &input,
   const CCodePage &codePage)
{
   const wstring output = TtoW(input, codePage);

   return SysAllocStringLen(output.c_str(), GetStringLength<UINT>(output));
}

int CStringConverter::TtoUTF8(
   const _tstring &input,
   BYTE *pBuffer,
   const int bufferSize,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return WtoUTF8(input, pBuffer, bufferSize);
#else
   return AtoUTF8(input, pBuffer, bufferSize, codePage);
#endif
}

int CStringConverter::TtoUTF8(
   const TCHAR *pInput,
   const int inputLength,
   BYTE *pBuffer,
   const int bufferSize,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return WtoUTF8(pInput, inputLength, pBuffer, bufferSize);
#else
   return AtoUTF8(pInput, inputLength, pBuffer, bufferSize, codePage);
#endif
}

int CStringConverter::GetSpaceRequiredForTtoUTF8(
   const _tstring &input,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return GetSpaceRequiredForWtoUTF8(input);
#else
   return GetSpaceRequiredForAtoUTF8(input, codePage);
#endif
}

int CStringConverter::GetSpaceRequiredForTtoUTF8(
   const TCHAR *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return GetSpaceRequiredForWtoUTF8(pInput, inputLength);
#else
   return GetSpaceRequiredForAtoUTF8(pInput, inputLength, codePage);
#endif
}

// BSTRto

string CStringConverter::BSTRtoA(
   const BSTR bstr,
   const CCodePage &codePage)
{
   if (SysStringLen(bstr) == 0)
   {
      return "";
   }

   return WtoA(bstr, SysStringLen(bstr), codePage);
}

_tstring CStringConverter::BSTRtoT(
   BSTR bstr,
   const CCodePage &codePage)
{
   if (SysStringLen(bstr) == 0)
   {
      return _T("");
   }

#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return bstr;
#else
   return WtoA(bstr, ::SysStringLen(bstr), codePage);
#endif
}

wstring CStringConverter::BSTRtoW(
   BSTR bstr)
{
   if (SysStringLen(bstr) == 0)
   {
      return L"";
   }

   return bstr;
}

void CStringConverter::BSTRtoA(
   BSTR bstr,
   const char **ppResult,
   const CCodePage &codePage)
{
   const size_t inputLength = SysStringLen(bstr);

   if (inputLength != 0)
   {
      if (inputLength == static_cast<size_t>(numeric_limits<int>::max()))
      {
         throw CException(_T("CStringConverter::BSTRtoA("), _T("String is too long to fit: ") + ToString(inputLength));
      }

      const auto outputLength = static_cast<int>(inputLength + 1);

      auto *pResult = new char[outputLength];

      if (0 == WideCharToMultiByte(
         codePage.GetCodePage(),
         0,
         bstr,
         static_cast<int>(inputLength),
         pResult,
         outputLength,
         nullptr,
         nullptr))
      {
         delete [] pResult;

         throw CErrorCodeException(_T("CStringConverter::BSTRtoA()"), GetLastError());
      }

      pResult[outputLength - 1] = 0;         // Ensure that the string is null terminated.

      *ppResult = pResult;
   }
   else
   {
      *ppResult = nullptr;
   }
}

_tstring CStringConverter::UTF8toT(
   const string &input,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return UTF8toW(reinterpret_cast<const BYTE *>(input.c_str()), GetStringLength<int>(input));
#else
   return UTF8toA(reinterpret_cast<const BYTE *>(input.c_str()), GetStringLength<int>(input), codePage);
#endif
}

_tstring CStringConverter::UTF8toT(
   const BYTE *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return UTF8toW(pInput, inputLength);
#else
   return UTF8toA(pInput, inputLength, codePage);
#endif
}

_tstring CStringConverter::UTF8toT(
   const BYTE *pPartialCharacter,
   const int partialLength,
   const BYTE *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   (void)codePage;
   return UTF8toW(pPartialCharacter, partialLength, pInput, inputLength);
#else
   return UTF8toA(pPartialCharacter, partialLength, pInput, inputLength, codePage);
#endif
}

string CStringConverter::UTF8toA(
   const string &input,
   const CCodePage &codePage)
{
   return WtoA(UTF8toW(reinterpret_cast<const BYTE *>(input.c_str()), GetStringLength<int>(input)), codePage);
}

string CStringConverter::UTF8toA(
   const BYTE *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
   return WtoA(UTF8toW(pInput, inputLength), codePage);
}

string CStringConverter::UTF8toA(
   const BYTE *pPartialCharacter,
   const int partialLength,
   const BYTE *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
   return WtoA(UTF8toW(pPartialCharacter, partialLength, pInput, inputLength), codePage);
}

wstring CStringConverter::UTF8toW(
   const string &input)
{
   return UTF8toW(reinterpret_cast<const BYTE *>(input.c_str()), GetStringLength<int>(input));
}

wstring CStringConverter::UTF8toW(
   const BYTE *pInput,
   const int inputLength)
{
   wstring result;

   if (inputLength != 0)
   {
      result.resize(inputLength);

      const int numChars = MultiByteToWideChar(
         CP_UTF8,
         0,
         reinterpret_cast<LPCSTR>(pInput),
         inputLength,
         const_cast<wchar_t *>(result.c_str()),
         GetStringLength<int>(result));

      if (0 == numChars)
      {
         throw CErrorCodeException(_T("CStringConverter::UTF8toW()"), GetLastError());
      }

      result.resize(numChars);
   }

   return result;
}

wstring CStringConverter::UTF8toW(
   const BYTE *pPartialCharacter,
   const int partialLength,
   const BYTE *pInput,
   const int inputLength)
{
   wstring result;

   if (inputLength != 0)
   {
      const bool containsPartialLength = (pPartialCharacter && partialLength != 0);

      const int partialLengthInOutput = (containsPartialLength ? 1 : 0);

      const size_t resultLength = inputLength + partialLengthInOutput;

      result.resize(resultLength);

      int numChars = 0;

      const int splitCharacterLength = containsPartialLength ? GetBytesInUTF8Character(*pPartialCharacter) : 0;

      if (partialLength > splitCharacterLength)
      {
         throw CException(
            _T("CStringConverter::UTF8toW()"),
            _T("Partial character contains too many bytes:  ") + ToString(partialLength) + _T("and character is only: ") + ToString(splitCharacterLength) + _T(" bytes long"));
      }

      const int remainingLength = splitCharacterLength - partialLength;

      if (splitCharacterLength)
      {
         BYTE splitCharacter[4];

         memcpy(splitCharacter, pPartialCharacter, partialLength);

         if (remainingLength)
         {
            memcpy(splitCharacter + partialLength, pInput, remainingLength);
         }

         const int numCharsThisTime = MultiByteToWideChar(
            CP_UTF8,
            0,
            reinterpret_cast<LPCSTR>(splitCharacter),
            splitCharacterLength,
            const_cast<wchar_t *>(result.c_str()),
            GetStringLength<int>(result));

         if (0 == numCharsThisTime)
         {
            throw CErrorCodeException(_T("CStringConverter::UTF8toW()"), GetLastError());
         }

         numChars += numCharsThisTime;
      }

      if (pInput && inputLength)
      {
         const int numCharsThisTime = MultiByteToWideChar(
            CP_UTF8,
            0,
            reinterpret_cast<LPCSTR>(pInput + remainingLength),
            inputLength - remainingLength,
            const_cast<wchar_t *>(result.c_str() + partialLengthInOutput),
            GetStringLength<int>(result) - partialLengthInOutput);

         if (0 == numCharsThisTime)
         {
            throw CErrorCodeException(_T("CStringConverter::UTF8toW()"), GetLastError());
         }

         numChars += numCharsThisTime;
      }

      result.resize(numChars);
   }

   return result;
}

int CStringConverter::GetBytesInUTF8Character(
   const BYTE input)
{
   if ((input & 0xF0) == 0xF0)
   {
      return 4;
   }

   if ((input & 0xE0) == 0xE0)
   {
      return 3;
   }

   if ((input & 0xC0) == 0xC0)
   {
      return 2;
   }

   if ((input & 0x80) == 0x00)
   {
      return 1;
   }

   throw CException(_T("CStringConverter::GetBytesInUTF8Character()"), _T("Not a start character: ") + ToHexString(input, HexDigitsWithPaddingWithPrefixUpperCase));
}

int CStringConverter::GetBytesAfterLastCompleteUTF8Character(
   const BYTE *pInput,
   const int inputLength)
{
   if (!pInput)
   {
      throw CException(_T("CStringConverter::GetBytesAfterLastCompleteUTF8Character()"), _T("pInput is null"));
   }

   if (!inputLength)
   {
      throw CException(_T("CStringConverter::GetBytesAfterLastCompleteUTF8Character()"), _T("Need at least one byte"));
   }

   for (int i = 1; i <= 4; i++)
   {
      const BYTE c = *(pInput + inputLength - i);

      if ((c & 0xF0) == 0xF0)
      {
         return 4 - i;
      }

      if ((c & 0xE0) == 0xE0)
      {
         return 3 - i;
      }

      if ((c & 0xC0) == 0xC0)
      {
         return 2 - i;
      }

      if ((c & 0x80) == 0x00)
      {
         return 1 - i;
      }
   }

   const int minLength = min(inputLength, 4);

   throw CException(
      _T("CStringConverter::GetBytesInUTF8Character()"),
      _T("Not a UTF-8 character: ") + DumpData(pInput + inputLength - minLength, minLength));
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: StringConverter.cpp
///////////////////////////////////////////////////////////////////////////////
