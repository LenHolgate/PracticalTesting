#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: StringConverter.h
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

#include "JetByteTools/CoreTools/tstring.h"

#include "JetByteTools/Win32Tools/CodePage.h"

#include <objbase.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// CStringConverter
///////////////////////////////////////////////////////////////////////////////

/// String conversion functions, use these in preference to USES_CONVERSION.
/// Doesn't suffer from the stack blowing limitations of USES_CONVERSION.
/// \ingroup Win32
/// \ingroup Unicode

class CStringConverter
{
   public :

      static const Win32::CCodePage s_CP_ACP;

      // Ato

      static int GetSpaceRequiredForAtoT(
         const std::string &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoT(
         const char *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoT(
         const char *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static Core::_tstring AtoT(
         const std::string &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static Core::_tstring AtoT(
         const char *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static size_t AtoT(
         void *pBuffer,
         size_t size,
         const char *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static Core::_tstring AtoT(
         const char *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static size_t AtoT(
         void *pBuffer,
         size_t size,
         const char *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoW(
         const std::string &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoW(
         const char *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoW(
         const char *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::wstring AtoW(
         const std::string &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::wstring AtoW(
         const char *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static size_t AtoW(
         void *pBuffer,
         size_t size,
         const char *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::wstring AtoW(
         const char *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static size_t AtoW(
         void *pBuffer,
         size_t size,
         const char *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static BSTR AtoBSTR(
         const std::string &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static BSTR AtoBSTR(
         const char *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static BSTR AtoBSTR(
         const char *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int AtoUTF8(
         const std::string &input,
         BYTE *pBuffer,
         int bufferSize,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int AtoUTF8(
         const char *pInput,
         int inputLength,
         BYTE *pBuffer,
         int bufferSize,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoUTF8(
         const std::string &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoUTF8(
         const char *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      // Wto

      static int GetSpaceRequiredForWtoT(
         const std::wstring &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForWtoT(
         const wchar_t *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForWtoT(
         const wchar_t *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static Core::_tstring WtoT(
         const std::wstring &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static Core::_tstring WtoT(
         const wchar_t *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static size_t WtoT(
         void *pBuffer,
         size_t size,
         const wchar_t *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static Core::_tstring WtoT(
         const wchar_t *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static size_t WtoT(
         void *pBuffer,
         size_t size,
         const wchar_t *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForWtoA(
         const std::wstring &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForWtoA(
         const wchar_t *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForWtoA(
         const wchar_t *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::string WtoA(
         const std::wstring &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::string WtoA(
         const wchar_t *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static size_t WtoA(
         void *pBuffer,
         size_t size,
         const wchar_t *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::string WtoA(
         const wchar_t *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static size_t WtoA(
         void *pBuffer,
         size_t size,
         const wchar_t *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static BSTR WtoBSTR(
         const std::wstring &input);

      static int WtoUTF8(
         const std::wstring &input,
         BYTE *pBuffer,
         int bufferSize);

      static int WtoUTF8(
         const wchar_t *pInput,
         int inputLength,
         BYTE *pBuffer,
         int bufferSize);

      static int GetSpaceRequiredForWtoUTF8(
         const std::wstring &input);

      static int GetSpaceRequiredForWtoUTF8(
         const wchar_t *pInput,
         int inputLength);

      // Tto

      static std::string TtoA(
         const Core::_tstring &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::string TtoA(
         const TCHAR *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::string TtoA(
         const TCHAR *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::wstring TtoW(
         const Core::_tstring &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::wstring TtoW(
         const TCHAR *pInput,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::wstring TtoW(
         const TCHAR *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static BSTR TtoBSTR(
         const Core::_tstring &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int TtoUTF8(
         const Core::_tstring &input,
         BYTE *pBuffer,
         int bufferSize,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int TtoUTF8(
         const TCHAR *pInput,
         int inputLength,
         BYTE *pBuffer,
         int bufferSize,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForTtoUTF8(
         const Core::_tstring &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForTtoUTF8(
         const TCHAR *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      // BSTRto

      static Core::_tstring BSTRtoT(
         BSTR bstr,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::string BSTRtoA(
         BSTR bstr,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::wstring BSTRtoW(
         BSTR bstr);

      static void BSTRtoA(
         BSTR bstr,
         const char **ppResult,
         const Win32::CCodePage &codePage = s_CP_ACP);

      // UTF8to

      static Core::_tstring UTF8toT(
         const std::string &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static Core::_tstring UTF8toT(
         const BYTE *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static Core::_tstring UTF8toT(
         const BYTE *pPartialCharacter,
         int partialLength,
         const BYTE *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::string UTF8toA(
         const std::string &input,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::string UTF8toA(
         const BYTE *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::string UTF8toA(
         const BYTE *pPartialCharacter,
         int partialLength,
         const BYTE *pInput,
         int inputLength,
         const Win32::CCodePage &codePage = s_CP_ACP);

      static std::wstring UTF8toW(
         const std::string &input);

      static std::wstring UTF8toW(
         const BYTE *pInput,
         int inputLength);

      static std::wstring UTF8toW(
         const BYTE *pPartialCharacter,
         int partialLength,
         const BYTE *pInput,
         int inputLength);

      static int GetBytesInUTF8Character(
         BYTE input);

      static int GetBytesAfterLastCompleteUTF8Character(
         const BYTE *pInput,
         int inputLength);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: StringConverter.h
///////////////////////////////////////////////////////////////////////////////

