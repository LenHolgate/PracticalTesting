#pragma once
#ifndef JETBYTE_TOOLS_WIN32_STRING_CONVERTER__
#define JETBYTE_TOOLS_WIN32_STRING_CONVERTER__
///////////////////////////////////////////////////////////////////////////////
// File: StringConverter.h
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

#include "tstring.h"

#include "CodePage.h"

#include <objbase.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

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

      static const CCodePage s_CP_ACP;

      // Ato

      static int GetSpaceRequiredForAtoT(
         const std::string &input,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoT(
         const char *pInput,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoT(
         const char *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static _tstring AtoT(
         const std::string &input,
         const CCodePage &codePage = s_CP_ACP);

      static _tstring AtoT(
         const char *pInput,
         const CCodePage &codePage = s_CP_ACP);

      static _tstring AtoT(
         const char *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoW(
         const std::string &input,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoW(
         const char *pInput,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoW(
         const char *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static std::wstring AtoW(
         const std::string &input,
         const CCodePage &codePage = s_CP_ACP);

      static std::wstring AtoW(
         const char *pInput,
         const CCodePage &codePage = s_CP_ACP);

      static std::wstring AtoW(
         const char *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static BSTR AtoBSTR(
         const std::string &input,
         const CCodePage &codePage = s_CP_ACP);

      static BSTR AtoBSTR(
         const char *pInput,
         const CCodePage &codePage = s_CP_ACP);

      static BSTR AtoBSTR(
         const char *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static int AtoUTF8(
         const std::string &input,
         BYTE *pBuffer,
         const int bufferSize,
         const CCodePage &codePage = s_CP_ACP);

      static int AtoUTF8(
         const char *pInput,
         const int inputLength,
         BYTE *pBuffer,
         const int bufferSize,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoUTF8(
         const std::string &input,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForAtoUTF8(
         const char *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      // Wto

      static int GetSpaceRequiredForWtoT(
         const std::wstring &input,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForWtoT(
         const wchar_t *pInput,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForWtoT(
         const wchar_t *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static _tstring WtoT(
         const std::wstring &input,
         const CCodePage &codePage = s_CP_ACP);

      static _tstring WtoT(
         const wchar_t *pInput,
         const CCodePage &codePage = s_CP_ACP);

      static _tstring WtoT(
         const wchar_t *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForWtoA(
         const std::wstring &input,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForWtoA(
         const wchar_t *pInput,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForWtoA(
         const wchar_t *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static std::string WtoA(
         const std::wstring &input,
         const CCodePage &codePage = s_CP_ACP);

      static std::string WtoA(
         const wchar_t *pInput,
         const CCodePage &codePage = s_CP_ACP);

      static std::string WtoA(
         const wchar_t *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static BSTR WtoBSTR(
         const std::wstring &input);

      static int WtoUTF8(
         const std::wstring &input,
         BYTE *pBuffer,
         const int bufferSize);

      static int WtoUTF8(
         const wchar_t *pInput,
         const int inputLength,
         BYTE *pBuffer,
         const int bufferSize);

      static int GetSpaceRequiredForWtoUTF8(
         const std::wstring &input);

      static int GetSpaceRequiredForWtoUTF8(
         const wchar_t *pInput,
         const int inputLength);

      // Tto

      static std::string TtoA(
         const _tstring &input,
         const CCodePage &codePage = s_CP_ACP);

      static std::string TtoA(
         const TCHAR *pInput,
         const CCodePage &codePage = s_CP_ACP);

      static std::string TtoA(
         const TCHAR *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static std::wstring TtoW(
         const _tstring &input,
         const CCodePage &codePage = s_CP_ACP);

      static std::wstring TtoW(
         const TCHAR *pInput,
         const CCodePage &codePage = s_CP_ACP);

      static std::wstring TtoW(
         const TCHAR *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static BSTR TtoBSTR(
         const _tstring &input,
         const CCodePage &codePage = s_CP_ACP);

      static int TtoUTF8(
         const _tstring &input,
         BYTE *pBuffer,
         const int bufferSize,
         const CCodePage &codePage = s_CP_ACP);

      static int TtoUTF8(
         const TCHAR *pInput,
         const int inputLength,
         BYTE *pBuffer,
         const int bufferSize,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForTtoUTF8(
         const _tstring &input,
         const CCodePage &codePage = s_CP_ACP);

      static int GetSpaceRequiredForTtoUTF8(
         const TCHAR *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      // BSTRto

      static _tstring BSTRtoT(
         const BSTR bstr,
         const CCodePage &codePage = s_CP_ACP);

      static std::string BSTRtoA(
         const BSTR bstr,
         const CCodePage &codePage = s_CP_ACP);

      static std::wstring BSTRtoW(
         const BSTR bstr);

      static void BSTRtoA(
         const BSTR bstr,
         const char **ppResult,
         const CCodePage &codePage = s_CP_ACP);

      // UTF8to

      static _tstring UTF8toT(
         const std::string &input,
         const CCodePage &codePage = s_CP_ACP);

      static _tstring UTF8toT(
         const BYTE *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static _tstring UTF8toT(
         const BYTE *pPartialCharacter,
         const int partialLength,
         const BYTE *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static std::string UTF8toA(
         const std::string &input,
         const CCodePage &codePage = s_CP_ACP);

      static std::string UTF8toA(
         const BYTE *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static std::string UTF8toA(
         const BYTE *pPartialCharacter,
         const int partialLength,
         const BYTE *pInput,
         const int inputLength,
         const CCodePage &codePage = s_CP_ACP);

      static std::wstring UTF8toW(
         const std::string &input);

      static std::wstring UTF8toW(
         const BYTE *pInput,
         const int inputLength);

      static std::wstring UTF8toW(
         const BYTE *pPartialCharacter,
         const int partialLength,
         const BYTE *pInput,
         const int inputLength);

      static int GetBytesInUTF8Character(
         const BYTE input);

      static int GetBytesAfterLastCompleteUTF8Character(
         const BYTE *pInput,
         const int inputLength);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_STRING_CONVERTER__

///////////////////////////////////////////////////////////////////////////////
// End of file: StringConverter.h
///////////////////////////////////////////////////////////////////////////////

