///////////////////////////////////////////////////////////////////////////////
// File: StringUtils.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 1997 JetByte Limited.
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

#include "StringUtils.h"
#include "ToBool.h"
#include "Tchar.h"
#include "ErrorCodeException.h"
#include "CheckedAtoL.h"

#pragma hdrstop

#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::string;
using std::wstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

bool StringToBool(
   const _tstring &stringRepresentation)
{
   if (stringRepresentation == _T("1"))
   {
      return true;
   }

   if (stringRepresentation == _T("0"))
   {
      return false;
   }

   if (0 == _tcsncicmp(stringRepresentation.c_str(), _T("TRUE"), stringRepresentation.length()))
   {
      return true;
   }

   if (0 == _tcsncicmp(stringRepresentation.c_str(), _T("FALSE"), stringRepresentation.length()))
   {
      return false;
   }

   throw CException(_T("StringToBool()"), _T("Can't convert: \"") + stringRepresentation + _T("\" to a bool value"));
}

bool StringToBoolA(
   const string &stringRepresentation)
{
   if (stringRepresentation == "1")
   {
      return true;
   }

   if (stringRepresentation == "0")
   {
      return false;
   }

   if (0 == _strnicmp(stringRepresentation.c_str(), "TRUE", stringRepresentation.length()))
   {
      return true;
   }

   if (0 == _strnicmp(stringRepresentation.c_str(), "FALSE", stringRepresentation.length()))
   {
      return false;
   }

   throw CException(_T("StringToBool()"), _T("Can't convert: \"") + CStringConverter::AtoT(stringRepresentation) + _T("\" to a bool value"));
}

bool ContainsDigits(
   const _tstring &source)
{
   bool ok = false;

   for (auto c : source)
   {
      if (!ok)
      {
         ok = ToBool(_istdigit(c));
      }
   }

   return ok;
}

bool IsAllDigits(
   const _tstring &numeric)
{
   bool ok = (!numeric.empty());

   for (_tstring::const_iterator it = numeric.begin(); ok && it != numeric.end(); ++it)
   {
      ok = ToBool(_istdigit(*it));
   }

   return ok;
}

bool IsAllDigitsA(
   const string &numeric)
{
   bool ok = (!numeric.empty());

   for (string::const_iterator it = numeric.begin(); ok && it != numeric.end(); ++it)
   {
      ok = ToBool(isdigit(static_cast<unsigned char>(*it)));
   }

   return ok;
}

bool IsAllDigitsOr(
   const _tstring &numeric,
   const TCHAR orThis)
{
   bool ok = (!numeric.empty());

   for (_tstring::const_iterator it = numeric.begin(); ok && it != numeric.end(); ++it)
   {
      ok = ToBool(*it == orThis || _istdigit(*it));
   }

   return ok;
}

bool IsAllDigitsOrA(
   const string &numeric,
   const char orThis)
{
   bool ok = (!numeric.empty());

   for (string::const_iterator it = numeric.begin(); ok && it != numeric.end(); ++it)
   {
      ok = ToBool(*it == orThis || isdigit(static_cast<unsigned char>(*it)));
   }

   return ok;
}

bool IsAllHexDigits(
   const _tstring &hex)
{
   bool ok = (!hex.empty());

   for (_tstring::const_iterator it = hex.begin(); ok && it != hex.end(); ++it)
   {
      ok = ToBool(_istxdigit(*it));
   }

   return ok;
}

bool IsAllHexDigitsA(
   const string &hex)
{
   bool ok = (!hex.empty());

   for (string::const_iterator it = hex.begin(); ok && it != hex.end(); ++it)
   {
      ok = ToBool(isxdigit(static_cast<unsigned char>(*it)));
   }

   return ok;
}

bool IsAllHexDigitsOr(
   const _tstring &hex,
   const TCHAR orThis)
{
   bool ok = (!hex.empty());

   for (_tstring::const_iterator it = hex.begin(); ok && it != hex.end(); ++it)
   {
      ok = ToBool(*it == orThis || _istxdigit(*it));
   }

   return ok;
}

bool IsAllHexDigitsOrA(
   const string &hex,
   const char orThis)
{
   bool ok = (!hex.empty());

   for (string::const_iterator it = hex.begin(); ok && it != hex.end(); ++it)
   {
      ok = ToBool(*it == orThis || isxdigit(static_cast<unsigned char>(*it)));
   }

   return ok;
}

bool IsAllAphaNum(
   const _tstring &alphaNum)
{
   bool ok = (!alphaNum.empty());

   for (auto it = alphaNum.begin(); ok && it != alphaNum.end(); ++it)
   {
      ok = ToBool(_istalnum(*it));
   }

   return ok;
}

bool IsAllAphaNumA(
   const string &alphaNum)
{
   bool ok = (!alphaNum.empty());

   for (auto it = alphaNum.begin(); ok && it != alphaNum.end(); ++it)
   {
      ok = ToBool(isalnum(*it));
   }

   return ok;
}

bool IsAllAphaNumOr(
   const _tstring &alphaNum,
   const TCHAR orThis)
{
   bool ok = (!alphaNum.empty());

   for (auto it = alphaNum.begin(); ok && it != alphaNum.end(); ++it)
   {
      ok = ToBool(*it == orThis || _istalnum(*it));
   }

   return ok;
}

bool IsAllAphaNumOrA(
   const string &alphaNum,
   const char orThis)
{
   bool ok = (!alphaNum.empty());

   for (auto it = alphaNum.begin(); ok && it != alphaNum.end(); ++it)
   {
      ok = ToBool(*it == orThis || isalnum(*it));
   }

   return ok;
}

void StringToHex(
   const _tstring &str,
   BYTE *pBuffer,
   const size_t nBytes)
{
   const string s = CStringConverter::TtoA(str);

   for (size_t i = 0; i < nBytes; i++)
   {
      const size_t stringOffset = i * 2;

      const auto b = static_cast<const BYTE>(s[stringOffset]);

      BYTE val = isdigit(b) ? static_cast<BYTE>((b - '0') * 16) : static_cast<BYTE>(((toupper(b) - 'A') + 10) * 16);

      const auto b1 = static_cast<const BYTE >(s[stringOffset + 1]);

      val = isdigit(b1) ? static_cast<BYTE>(val + b1 - '0') : static_cast<BYTE>(val + (toupper(b1) - 'A') + 10);

      pBuffer[i] = val;
   }
}

_tstring StripWhiteSpace(
   const _tstring &source)
{
   _tstring destination;

   destination.resize(source.size());

   const TCHAR *pSrc = source.c_str();

   auto *pDst = const_cast<TCHAR *>(destination.c_str());

   size_t i = 0;

   while (*pSrc)
   {
      if (!_istspace(*pSrc))
      {
         *pDst = *pSrc;
         pDst++;
         i++;
      }

      ++pSrc;
   }

   destination.resize(i);

   return destination;
}

string StripWhiteSpaceA(
   const string &source)
{
   string destination;

   destination.resize(source.size());

   const char *pSrc = source.c_str();

   auto *pDst = const_cast<char *>(destination.c_str());

   size_t i = 0;

   while (*pSrc)
   {
      if (!isspace(static_cast<unsigned char>(*pSrc)))
      {
         *pDst = *pSrc;
         pDst++;
         i++;
      }

      ++pSrc;
   }

   destination.resize(i);

   return destination;
}

_tstring StripSurroundingWhiteSpace(
   const _tstring &source)
{
   const TCHAR *pSrc = source.c_str();

   while (pSrc && _istspace(*pSrc))
   {
      ++pSrc;
   }

   _tstring result;

   if (pSrc)
   {
      result = pSrc;
   }

   size_t i = result.length();

   pSrc = result.c_str() + i;

   --pSrc;

   while (i && _istspace(*pSrc))
   {
      --pSrc;
      --i;
   }

   return result.substr(0, i);
}

string StripSurroundingWhiteSpaceA(
   const string &source)
{
   const char *pSrc = source.c_str();

   while (pSrc && isspace(static_cast<unsigned char>(*pSrc)))
   {
      ++pSrc;
   }

   string result;

   if (pSrc)
   {
      result = pSrc;
   }

   size_t i = result.length();

   pSrc = result.c_str() + i;

   --pSrc;

   while (i && isspace(static_cast<unsigned char>(*pSrc)))
   {
      --pSrc;
      --i;
   }

   return result.substr(0, i);
}

_tstring StripLeading(
   const _tstring &source,
   const char toStrip)
{
   const TCHAR _ttoStrip = toStrip;

   const TCHAR *pSrc = source.c_str();

   while (pSrc && *pSrc == _ttoStrip)
   {
      ++pSrc;
   }

   return pSrc;
}

_tstring StripTrailing(
   const _tstring &source,
   const char toStrip)
{
   const TCHAR _ttoStrip = toStrip;

   size_t i = source.length();
   const _TCHAR *pSrc = source.c_str() + i;

   --pSrc;

   while (i && *pSrc == _ttoStrip)
   {
      --pSrc;
      --i;
   }

   return source.substr(0, i);
}

string StripLeadingA(
   const string &source,
   const char toStrip)
{
   const char *pSrc = source.c_str();

   while (pSrc && *pSrc == toStrip)
   {
      ++pSrc;
   }

   return pSrc;
}

string StripTrailingA(
   const string &source,
   const char toStrip)
{
   size_t i = source.length();
   const char *pSrc = source.c_str() + i;

   --pSrc;

   while (i && *pSrc == toStrip)
   {
      --pSrc;
      --i;
   }

   return source.substr(0, i);
}

_tstring ToUpper(
   const _tstring &data)
{
   _tstring dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<TCHAR>(toupper(dataOut[i]));
   }

   return dataOut;
}

string ToUpperA(
   const string &data)
{
   string dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<char>(toupper(dataOut[i]));
   }

   return dataOut;
}

wstring ToUpperW(
   const wstring &data)
{
   wstring dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<wchar_t>(toupper(dataOut[i]));
   }

   return dataOut;
}

_tstring ToLower(
   const _tstring &data)
{
   _tstring dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<TCHAR>(tolower(dataOut[i]));
   }

   return dataOut;
}

string ToLowerA(
   const char *pData)
{
   string dataOut(pData);

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<char>(tolower(dataOut[i]));
   }

   return dataOut;
}

string ToLowerA(
   const string &data)
{
   string dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<char>(tolower(dataOut[i]));
   }

   return dataOut;
}

wstring ToLowerW(
   const wstring &data)
{
   wstring dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<wchar_t>(tolower(dataOut[i]));
   }

   return dataOut;
}

_tstring FindAndReplace(
   const _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString,
   const size_t numReplacements)
{
   _tstring result = phrase;

   InPlaceFindAndReplace(result, findString, replaceString, numReplacements);

   return result;
}

bool InPlaceFindAndReplace(
   _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString,
   size_t numReplacements)
{
   bool replaced = false;

   const _tstring::size_type replacedLength = replaceString.length();

   _tstring::size_type pos = phrase.find(findString);

   while (pos != _tstring::npos && numReplacements > 0)
   {
      phrase.replace(pos, findString.length(), replaceString);

      replaced = true;

      pos = phrase.find(findString, pos + replacedLength);

      if (numReplacements != INFINITE)
      {
         --numReplacements;
      }
   }

   return replaced;
}

string FindAndReplaceA(
   const string &phrase,
   const string &findString,
   const string &replaceString,
   const size_t numReplacements)
{
   string result = phrase;

   InPlaceFindAndReplaceA(result, findString, replaceString, numReplacements);

   return result;
}

bool InPlaceFindAndReplaceA(
   string &phrase,
   const string &findString,
   const string &replaceString,
   size_t numReplacements)
{
   bool replaced = false;

   const string::size_type replacedLength = replaceString.length();

   string::size_type pos = phrase.find(findString);

   while (pos != _tstring::npos && numReplacements > 0)
   {
      phrase.replace(pos, findString.length(), replaceString);

      replaced = true;

      pos = phrase.find(findString, pos + replacedLength);

      if (numReplacements != INFINITE)
      {
         --numReplacements;
      }
   }

   return replaced;
}

_tstring CaseInsensitiveFindAndReplace(
   const _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString,
   const size_t numReplacements)
{
   _tstring result = phrase;

   CaseInsensitiveInPlaceFindAndReplace(result, findString, replaceString, numReplacements);

   return result;
}

bool CaseInsensitiveInPlaceFindAndReplace(
   _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString,
   size_t numReplacements)
{
   bool replaced = false;

   const _tstring::size_type replacedLength = replaceString.length();

   _tstring upperCasePhrase = ToUpper(phrase);

   const _tstring upperCaseFindString = ToUpper(findString);

   _tstring::size_type pos = upperCasePhrase.find(upperCaseFindString);

   while (pos != _tstring::npos && numReplacements > 0)
   {
      // we have to replace in both to keep the lengths and offsets the same...

      upperCasePhrase.replace(pos, upperCaseFindString.length(), replaceString);

      phrase.replace(pos, upperCaseFindString.length(), replaceString);

      replaced = true;

      pos = upperCasePhrase.find(upperCaseFindString, pos + replacedLength);

      if (numReplacements != INFINITE)
      {
         --numReplacements;
      }
   }

   return replaced;
}

string CaseInsensitiveFindAndReplaceA(
   const string &phrase,
   const string &findString,
   const string &replaceString,
   const size_t numReplacements)
{
   string result = phrase;

   CaseInsensitiveInPlaceFindAndReplaceA(result, findString, replaceString, numReplacements);

   return result;
}

bool CaseInsensitiveInPlaceFindAndReplaceA(
   string &phrase,
   const string &findString,
   const string &replaceString,
   size_t numReplacements)
{
   bool replaced = false;

   const string::size_type replacedLength = replaceString.length();

   string upperCasePhrase = ToUpperA(phrase);

   const string upperCaseFindString = ToUpperA(findString);

   string::size_type pos = upperCasePhrase.find(upperCaseFindString);

   while (pos != _tstring::npos && numReplacements > 0)
   {
      // we have to replace in both to keep the lengths and offsets the same...

      upperCasePhrase.replace(pos, upperCaseFindString.length(), replaceString);

      phrase.replace(pos, upperCaseFindString.length(), replaceString);

      replaced = true;

      pos = upperCasePhrase.find(upperCaseFindString, pos + replacedLength);

      if (numReplacements != INFINITE)
      {
         --numReplacements;
      }
   }

   return replaced;
}

bool FindAndRemoveString(
   _tstring &source,
   const _tstring &target)
{
   const _tstring tag = _T("|") + target + _T("|");

   if (source == tag)
   {
      source.clear();

      return true;
   }

   bool found = false;

   _tstring::size_type pos = source.find(tag);

   if (pos != _tstring::npos)
   {
      found = true;

      source = FindAndReplace(source, target, _T(""));
   }

   if (_tstring::npos == source.find_first_not_of('|'))
   {
      source.clear();
   }

   return found;
}

unsigned long GetLongFromString(
   const _tstring &numeric,
   const size_t startOffset,
   const size_t length)
{
   if (startOffset + length > numeric.length())
   {
      throw CException(_T("GetLongFromString()"),
         _T("Invalid offset (") + ToString(startOffset) + _T(") or length (") + ToString(length) + _T(") string is only ") + ToString(numeric.length()) + _T(" long"));
   }

   return _ttol(numeric.substr(startOffset, length).c_str());
}

unsigned short GetShortFromString(
   const _tstring &numeric,
   const size_t startOffset,
   const size_t length)
{
   return static_cast<unsigned short>(GetLongFromString(numeric, startOffset, length));
}

unsigned long GetLongFromStringA(
   const string &numeric,
   const size_t startOffset,
   const size_t length)
{
   if (startOffset + length > numeric.length())
   {
      throw CException(_T("GetLongFromString()"),
         _T("Invalid offset (") + ToString(startOffset) + _T(") or length (") + ToString(length) + _T(") string is only ") + ToString(numeric.length()) + _T(" long"));
   }

   return CheckedAtoL(numeric.substr(startOffset, length));
}

unsigned short GetShortFromStringA(
   const string &numeric,
   const size_t startOffset,
   const size_t length)
{
   return static_cast<unsigned short>(GetLongFromStringA(numeric, startOffset, length));
}

_tstring BuildMultiString(
   const _tstring &target,
   const _tstring &newString)
{
   _tstring result = target;

   const _tstring::size_type length = result.length();

   result += _T(" ") + newString;

   result[length] = '\0';

   return result;
}

string BuildMultiStringA(
   const string &target,
   const string &newString)
{
   string result = target;

   const string::size_type length = result.length();

   result += " " + newString;

   result[length] = '\0';

   return result;
}

_tstring GetStringFromMultiString(
   _tstring &source)
{
   const _tstring::size_type pos = source.find(_T('\0'));

   if (pos != _tstring::npos)
   {
      const _tstring result = source.substr(0, pos);

      source = source.substr(pos + 1);

      return result;
   }

   const _tstring result = source;

   source.clear();

   return result;
}

string GetStringFromMultiStringA(
   string &source)
{
   const string::size_type pos = source.find('\0');

   if (pos != string::npos)
   {
      const string result = source.substr(0, pos);

      source = source.substr(pos + 1);

      return result;
   }

   const string result = source;

   source.clear();

   return result;
}

#if _MSC_VER < 1920 || (JETBYTE_CORE_STRING_UTILS_CONVERT_TO_BYTES_IS_CONSTEXPR != 1)

string ConvertToBytes(
   const _tstring &input,
   const bool hasSpaces)
{
   std::string output;

   const size_t length = input.length();

   size_t i = 0;

   while (i < length)
   {
      const TCHAR c1 = input[i++];
      const TCHAR c2 = input[i++];

      const BYTE n1 = static_cast<BYTE>((c1 >= 'A') ? c1 - 'A' + 10 : c1 - '0');
      const BYTE n2 = static_cast<BYTE>((c2 >= 'A') ? c2 - 'A' + 10 : c2 - '0');

      const BYTE b = (n1 << 4) | n2;

      output.push_back(b);

      if (hasSpaces)
      {
         i++;
      }
   }

   return output;
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: StringUtils.cpp
///////////////////////////////////////////////////////////////////////////////
