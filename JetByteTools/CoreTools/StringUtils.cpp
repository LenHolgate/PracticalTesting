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

#pragma hdrstop

#include <algorithm>
#include <list>

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
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static bool StringIsAllANSI(
   const std::wstring &data);

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

   for (_tstring::const_iterator it = source.begin(); it != source.end(); ++it)
   {
      if (!ok)
      {
         ok = ToBool(_istdigit(*it));
      }
   }

   return ok;
}

bool IsAllDigits(
   const _tstring &numeric)
{
   bool ok = (numeric.length() != 0);

   for (_tstring::const_iterator it = numeric.begin(); ok && it != numeric.end(); ++it)
   {
      ok = ToBool(_istdigit(*it));
   }

   return ok;
}

bool IsAllDigitsA(
   const string &numeric)
{
   bool ok = (numeric.length() != 0);

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
   bool ok = (numeric.length() != 0);

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
   bool ok = (numeric.length() != 0);

   for (string::const_iterator it = numeric.begin(); ok && it != numeric.end(); ++it)
   {
      ok = ToBool(*it == orThis || isdigit(static_cast<unsigned char>(*it)));
   }

   return ok;
}

bool IsAllHexDigits(
   const _tstring &hex)
{
   bool ok = (hex.length() != 0);

   for (_tstring::const_iterator it = hex.begin(); ok && it != hex.end(); ++it)
   {
      ok = ToBool(_istxdigit(*it));
   }

   return ok;
}

bool IsAllHexDigitsA(
   const string &hex)
{
   bool ok = (hex.length() != 0);

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
   bool ok = (hex.length() != 0);

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
   bool ok = (hex.length() != 0);

   for (string::const_iterator it = hex.begin(); ok && it != hex.end(); ++it)
   {
      ok = ToBool(*it == orThis || isxdigit(static_cast<unsigned char>(*it)));
   }

   return ok;
}

bool IsAllAphaNum(
   const _tstring &alphaNum)
{
   bool ok = (alphaNum.length() != 0);

   for (auto it = alphaNum.begin(); ok && it != alphaNum.end(); ++it)
   {
      ok = ToBool(_istalnum(*it));
   }

   return ok;
}

bool IsAllAphaNumA(
   const string &alphaNum)
{
   bool ok = (alphaNum.length() != 0);

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
   bool ok = (alphaNum.length() != 0);

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
   bool ok = (alphaNum.length() != 0);

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
   const TCHAR *pSrc = source.c_str();

   while (pSrc && *pSrc == toStrip)
   {
      ++pSrc;
   }

   return pSrc;
}

_tstring StripTrailing(
   const _tstring &source,
   const char toStrip)
{
   size_t i = source.length();
   const _TCHAR *pSrc = source.c_str() + i;

   --pSrc;

   while (i && *pSrc == toStrip)
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

   _tstring::size_type pos = phrase.find(findString);

   while (pos != _tstring::npos && numReplacements > 0)
   {
      phrase.replace(pos, findString.length(), replaceString);

      replaced = true;

      pos = phrase.find(findString, pos + 1);

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

   string::size_type pos = phrase.find(findString);

   while (pos != _tstring::npos && numReplacements > 0)
   {
      phrase.replace(pos, findString.length(), replaceString);

      replaced = true;

      pos = phrase.find(findString, pos + 1);

      if (numReplacements != INFINITE)
      {
         --numReplacements;
      }
   }

   return replaced;
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

   return atol(numeric.substr(startOffset, length).c_str());
}

unsigned short GetShortFromStringA(
   const string &numeric,
   const size_t startOffset,
   const size_t length)
{
   return static_cast<unsigned short>(GetLongFromStringA(numeric, startOffset, length));
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static bool StringIsAllANSI(
   const std::wstring &data)
{
   const auto *pData = reinterpret_cast<const BYTE *>(data.c_str());

   const size_t dataLength = data.length() * sizeof(wchar_t);

   bool allANSI = true;

   for (size_t i = 1; allANSI && i < dataLength; i += 2)
   {
      if (pData[i] != 0x00)
      {
         allANSI = !(i != 1 || pData[i] != 0xFE);
      }
   }

   return allANSI;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: StringUtils.cpp
///////////////////////////////////////////////////////////////////////////////
