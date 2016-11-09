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

#include <algorithm>          // for min

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::string;
using std::wstring;
using std::numeric_limits;
using std::min;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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

_tstring CStringConverter::AtoT(
   const char *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef _UNICODE
   return AtoW(pInput, inputLength, codePage);
#else
   (void)codePage;
   return _tstring(pInput, inputLength);
#endif
}

int CStringConverter::GetSpaceRequiredForAtoW(
   const string &input,
   const CCodePage &codePage)
{
   const int length = GetStringLength<int>(input, true);

   if (codePage.isSingleByteCharacterSet())
   {
      return length * sizeof(wchar_t);
   }
   else
   {
      return GetSpaceRequiredForAtoW(input.c_str(), length, codePage);
   }
}

int CStringConverter::GetSpaceRequiredForAtoW(
   const char *pInput,
   const CCodePage &codePage)
{
   const int length = GetStringLength<int>(pInput, true);

   if (codePage.isSingleByteCharacterSet())
   {
      return length * sizeof(wchar_t);
   }
   else
   {
      return GetSpaceRequiredForAtoW(pInput, length, codePage);
   }
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
   else
   {
      const int bytesRequired = ::MultiByteToWideChar(
         codePage.GetCodePage(),
         0,
         pInput,
         inputLength,
         0,
         0);

      if (bytesRequired == 0)
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("CStringConverter::GetSpaceRequiredForAtoW()"), lastError);
      }

      return bytesRequired;
   }
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
         result.resize(inputLength);
      }
      else
      {
         const int bytesRequired = ::MultiByteToWideChar(
            codePage.GetCodePage(),
            0,
            pInput,
            inputLength,
            0,
            0);

         if (bytesRequired == 0)
         {
            const DWORD lastError = ::GetLastError();

            throw CWin32Exception(_T("CStringConverter::AtoW()"), lastError);
         }

         result.resize(bytesRequired);
      }

      if (0 ==::MultiByteToWideChar(
         codePage.GetCodePage(),
         0,
         pInput,
         inputLength,
         const_cast<wchar_t *>(result.c_str()),
         GetStringLength<int>(result, true)))
      {
         throw CWin32Exception(_T("CStringConverter::AtoW()"), ::GetLastError());
      }
   }

   return result;
}

BSTR CStringConverter::AtoBSTR(
   const string &input,
   const CCodePage &codePage)
{
   const wstring output = AtoW(input.c_str(), GetStringLength<int>(input), codePage);

   return ::SysAllocStringLen(output.c_str(), GetStringLength<UINT>(output));
}

BSTR CStringConverter::AtoBSTR(
   const char *pInput,
   const CCodePage &codePage)
{
   if (!pInput)
   {
      return ::SysAllocString(L"");
   }

   return AtoBSTR(pInput, GetStringLength<int>(pInput), codePage);
}

BSTR CStringConverter::AtoBSTR(
   const char *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
   const wstring output = AtoW(pInput, inputLength, codePage);

   return ::SysAllocStringLen(output.c_str(), GetStringLength<unsigned int>(output));
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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

_tstring CStringConverter::WtoT(
   const wchar_t *pInput,
   const int inputLength,
   const CCodePage &codePage)
{
#ifdef _UNICODE
   (void)codePage;
   return _tstring(pInput, inputLength);
#else
   return WtoA(pInput, inputLength, codePage);
#endif
}

int CStringConverter::GetSpaceRequiredForWtoA(
   const wstring &input,
   const CCodePage &codePage)
{
   const int length = GetStringLength<int>(input, true);

   if (codePage.isSingleByteCharacterSet())
   {
      return length * sizeof(wchar_t);
   }
   else
   {
      return GetSpaceRequiredForWtoA(input.c_str(), length, codePage);
   }
}

int CStringConverter::GetSpaceRequiredForWtoA(
   const wchar_t *pInput,
   const CCodePage &codePage)
{
   const int length = GetStringLength<int>(pInput, true);

   if (codePage.isSingleByteCharacterSet())
   {
      return length * sizeof(wchar_t);
   }
   else
   {
      return GetSpaceRequiredForWtoA(pInput, length, codePage);
   }
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
   else
   {
      const int bytesRequired = ::WideCharToMultiByte(
         codePage.GetCodePage(),
         0,
         pInput,
         inputLength,
         0,
         0,
         0,
         0);

      if (bytesRequired == 0)
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("CStringConverter::GetSpaceRequiredForWtoA()"), lastError);
      }

      return bytesRequired;
   }
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
         const int bytesRequired = ::WideCharToMultiByte(
            codePage.GetCodePage(),
            0,
            pInput,
            inputLength,
            0,
            0,
            0,
            0);

         if (bytesRequired == 0)
         {
            const DWORD lastError = ::GetLastError();

            throw CWin32Exception(_T("CStringConverter::WtoA()"), lastError);
         }

         result.resize(bytesRequired);
      }

      if (0 == ::WideCharToMultiByte(
         codePage.GetCodePage(),
         0,
         pInput,
         inputLength,
         const_cast<char *>(result.c_str()),
         GetStringLength<int>(result, true),
         0,
         0))
      {
         throw CWin32Exception(_T("CStringConverter::WtoA()"), ::GetLastError());
      }
   }

   return result;
}

BSTR CStringConverter::WtoBSTR(
   const wstring &input)
{
   return ::SysAllocStringLen(input.c_str(), GetStringLength<UINT>(input));
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
      bufferUsed = ::WideCharToMultiByte(
         CP_UTF8,
         WC_ERR_INVALID_CHARS,
         pInput,
         inputLength,
         reinterpret_cast<LPSTR>(pBuffer),
         bufferSize,
         0,
         0);

      if (0 == bufferUsed)
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("CStringConverter::WtoUTF8()"), lastError);
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
      spaceRequired = ::WideCharToMultiByte(
         CP_UTF8,
         WC_ERR_INVALID_CHARS,
         pInput,
         inputLength,
         0,
         0,
         0,
         0);

      if (0 == spaceRequired)
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("CStringConverter::GetSpaceRequiredForWtoUTF8()"), lastError);
      }
   }

   return spaceRequired;
}

// Tto

string CStringConverter::TtoA(
   const _tstring &input,
   const CCodePage &codePage)
{
#ifdef _UNICODE
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
#ifdef _UNICODE

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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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

   return ::SysAllocStringLen(output.c_str(), GetStringLength<UINT>(output));
}

int CStringConverter::TtoUTF8(
   const _tstring &input,
   BYTE *pBuffer,
   const int bufferSize,
   const CCodePage &codePage)
{
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
   if (::SysStringLen(bstr) == 0)
   {
      return "";
   }

   return WtoA(bstr, ::SysStringLen(bstr), codePage);
}

_tstring CStringConverter::BSTRtoT(
   const BSTR bstr,
   const CCodePage &codePage)
{
   if (::SysStringLen(bstr) == 0)
   {
      return _T("");
   }

#ifdef _UNICODE
   (void)codePage;
   return bstr;
#else
   return WtoA(bstr, ::SysStringLen(bstr), codePage);
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
   const char **ppResult,
   const CCodePage &codePage)
{
   const size_t inputLength = ::SysStringLen(bstr);

   if (inputLength != 0)
   {
      if (inputLength == static_cast<size_t>(numeric_limits<int>::max())) //lint !e571 (Suspicious cast)
      {
         throw CException(_T("CStringConverter::BSTRtoA("), _T("String is too long to fit: ") + ToString(inputLength));
      }

      const int outputLength = static_cast<int>(inputLength + 1);

      char *pResult = new char[outputLength];

      if (0 == ::WideCharToMultiByte(
         codePage.GetCodePage(),
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

_tstring CStringConverter::UTF8toT(
   const string &input,
   const CCodePage &codePage)
{
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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

      const int numChars = ::MultiByteToWideChar(
         CP_UTF8,
         0,
         reinterpret_cast<LPCSTR>(pInput),
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

      //lint -e{613} (Possible use of null pointer 'pPartialCharacter' in argument to operator 'unary *'
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

         const int numCharsThisTime = ::MultiByteToWideChar(
            CP_UTF8,
            0,
            reinterpret_cast<LPCSTR>(splitCharacter),
            splitCharacterLength,
            const_cast<wchar_t *>(result.c_str()),
            GetStringLength<int>(result));

         if (0 == numCharsThisTime)
         {
            throw CWin32Exception(_T("CStringConverter::UTF8toW()"), ::GetLastError());
         }

         numChars += numCharsThisTime;
      }

      if (pInput && inputLength)
      {
         const int numCharsThisTime = ::MultiByteToWideChar(
            CP_UTF8,
            0,
            reinterpret_cast<LPCSTR>(pInput + remainingLength),
            inputLength - remainingLength,
            const_cast<wchar_t *>(result.c_str() + partialLengthInOutput),
            GetStringLength<int>(result) - partialLengthInOutput);

         if (0 == numCharsThisTime)
         {
            throw CWin32Exception(_T("CStringConverter::UTF8toW()"), ::GetLastError());
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
   else if ((input & 0xE0) == 0xE0)
   {
      return 3;
   }
   else if ((input & 0xC0) == 0xC0)
   {
      return 2;
   }
   else if ((input & 0x80) == 0x00)
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
      else if ((c & 0xE0) == 0xE0)
      {
         return 3 - i;
      }
      else if ((c & 0xC0) == 0xC0)
      {
         return 2 - i;
      }
      else if ((c & 0x80) == 0x00)
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
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: StringConverter.cpp
///////////////////////////////////////////////////////////////////////////////

