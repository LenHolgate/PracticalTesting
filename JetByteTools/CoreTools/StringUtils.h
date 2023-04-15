#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: StringUtils.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2018 JetByte Limited.
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

#include "Types.h"
#include "tstring.h"
#include "StringConverter.h"
#include "Exception.h"
#include "ToString.h"

#include <limits>
#include <cstring>     // strlen

#ifdef min
#pragma push_macro("min")
#define JETBYTE_UTILS_PUSHED_MIN
#undef min
#endif

#ifdef max
#pragma push_macro("max")
#define JETBYTE_UTILS_PUSHED_MAX
#undef max
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Functions defined in this file...
///////////////////////////////////////////////////////////////////////////////

template <typename result, class s>
result GetStringLength(
   const s &theString,
   const bool includeNullTerminator = false)
{
   const uint64 length = theString.length() + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<uint64>(std::numeric_limits<result>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<result>(length);
}

template <typename result>
result GetStringLength(
   const char *pString,
   const bool includeNullTerminator = false)
{
   const uint64 length = strlen(pString) + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<uint64>(std::numeric_limits<result>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<result>(length);
}

template <typename result>
result GetStringLength(
   const wchar_t *pString,
   const bool includeNullTerminator = false)
{
   const uint64 length = wcslen(pString) + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<uint64>(std::numeric_limits<result>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<result>(length);
}

template <class s>
bool StringEndsWith(
   const s &theString,
   const typename s::value_type value)
{
   // if we don't have C++20 ends_with...

   const typename s::size_type pos = theString.find_last_of(value);

   if (pos != s::npos)
   {
      if (pos + 1 == theString.length())
      {
         return true;
      }
   }
   return false;
}

unsigned long GetLongFromString(
   const _tstring &numeric,
   size_t startOffset,
   size_t length);

unsigned short GetShortFromString(
   const _tstring &numeric,
   size_t startOffset,
   size_t length);

unsigned long GetLongFromStringA(
   const std::string &numeric,
   size_t startOffset,
   size_t length);

unsigned short GetShortFromStringA(
   const std::string &numeric,
   size_t startOffset,
   size_t length);

bool ContainsDigits(
   const _tstring &source);

bool IsAllDigits(
   const _tstring &numeric);

bool IsAllDigitsA(
   const std::string &numeric);

bool IsAllDigitsOr(
   const _tstring &numeric,
   TCHAR orThis);

bool IsAllDigitsOrA(
   const std::string &numeric,
   char orThis);

bool IsAllHexDigits(
   const _tstring &hex);

bool IsAllHexDigitsA(
   const std::string &hex);

bool IsAllHexDigitsOr(
   const _tstring &hex,
   TCHAR orThis);

bool IsAllHexDigitsOrA(
   const std::string &hex,
   char orThis);

bool IsAllAphaNum(
   const _tstring &alphaNum);

bool IsAllAphaNumA(
   const std::string &alphaNum);

bool IsAllAphaNumOr(
   const _tstring &alphaNum,
   TCHAR orThis);

bool IsAllAphaNumOrA(
   const std::string &alphaNum,
   char orThis);

bool StringToBool(
   const _tstring &stringRepresentation);

bool StringToBoolA(
   const std::string &stringRepresentation);

void StringToHex(
   const _tstring &str,
   BYTE *pBuffer,
   size_t nBytes);

_tstring ToUpper(
   const _tstring &data);

std::string ToUpperA(
   const std::string &data);

std::wstring ToUpperW(
   const std::wstring &data);

_tstring ToLower(
   const _tstring &data);

std::string ToLowerA(
   const std::string &data);

std::wstring ToLowerW(
   const std::wstring &data);

_tstring StripWhiteSpace(
   const _tstring &source);

std::string StripWhiteSpaceA(
   const std::string &source);

_tstring StripSurroundingWhiteSpace(
   const _tstring &source);

std::string StripSurroundingWhiteSpaceA(
   const std::string &source);

_tstring StripLeading(
   const _tstring &source,
   char toStrip);

_tstring StripTrailing(
   const _tstring &source,
   char toStrip);

std::string StripLeadingA(
   const std::string &source,
   char toStrip);

std::string StripTrailingA(
   const std::string &source,
   char toStrip);

_tstring FindAndReplace(
   const _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString,
   size_t numReplacements = INFINITE);

bool InPlaceFindAndReplace(
   _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString,
   size_t numReplacements = INFINITE);

std::string FindAndReplaceA(
   const std::string &phrase,
   const std::string &findString,
   const std::string &replaceString,
   size_t numReplacements = INFINITE);

bool InPlaceFindAndReplaceA(
   std::string &phrase,
   const std::string &findString,
   const std::string &replaceString,
   size_t numReplacements = INFINITE);

#ifdef JETBYTE_UTILS_PUSHED_MIN
#pragma pop_macro("min")
#endif

#ifdef JETBYTE_UTILS_PUSHED_MAX
#pragma pop_macro("max")
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: StringUtils.h
///////////////////////////////////////////////////////////////////////////////

