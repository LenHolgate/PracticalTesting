///////////////////////////////////////////////////////////////////////////////
// File: ToString.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2008 JetByte Limited.
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

#include "ToString.h"
#include "Exception.h"
#include "Printf.h"

#pragma hdrstop

#include <limits>

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

enum DumpType
{
   HexDump     = 0x01,
   Printable   = 0x10,
   Both        = 0x11
};

static string InternalDumpDataA(
   const string &linePrefix,
   const BYTE *pData,
   size_t dataLength,
   size_t lineLength,
   DumpType dumpType,
   bool useCR,
   bool linePrefixOnFirstLine,
   bool lineFeedOnLastLine);

static wstring InternalDumpDataW(
   const wstring &linePrefix,
   const BYTE *pData,
   size_t dataLength,
   size_t lineLength,
   DumpType dumpType,
   bool useCR,
   bool linePrefixOnFirstLine,
   bool lineFeedOnLastLine);

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

unsigned short CalculateRequiredPrecision(
   const double value)
{
   // Prior to VS2015 sprintf() would only ever return
   // 17 digits of precision, so 3.1415899999999999 sprintf'd
   // with a precision of 30 would end up as 3.1415899999999999.
   // VS2015 (and GNU) return more, 3.14158999999999988261834005243
   // To retain backwards compatibility we truncate the precision to
   // 17

   const string asString = ToStringA(value, 17);

   unsigned short precision = 0;

   const char *pStart = asString.c_str();

   const char *p = pStart;

   bool started = false;

   size_t zeroes = 0;

   size_t nines = 0;

   unsigned short numDigits = 0;

   while (*p)
   {
      if (*p == '.')
      {
         started = true;
      }
      else
      {
         numDigits++;

         if (started)
         {
            if (*p == '0')
            {
               if (nines)
               {
                  nines = 0;

                  precision = static_cast<unsigned short>(numDigits - 1);
               }

               zeroes++;
            }
            else if (*p == '9')
            {
               if (zeroes)
               {
                  zeroes = 0;

                  precision = static_cast<unsigned short>(numDigits - 1);
               }

               nines++;
            }
            else
            {
               if (*(p + 1) == 0)
               {
                  if (zeroes + nines > 2)
                  {
                     if (numDigits < 6)
                     {
                        precision = numDigits;
                     }

                     break;
                  }
               }

               precision = numDigits;

               zeroes = 0;
               nines = 0;
            }
         }
         else
         {
            precision++;
         }
      }
      p++;
   }

   if (zeroes + nines < 2)
   {
      precision = numDigits;
   }

   return precision;
}

string ToStringA(
   const bool val)
{
   return val ? "1" : "0";
}

wstring ToStringW(
   const bool val)
{
   return val ? L"1" : L"0";
}


template <typename T>
constexpr size_t SpaceRequiredForType(
   const T & /*notUsed*/)
{
#if _MSC_VER < 1910
   return 200;
#else
   constexpr size_t spaceRequiredForSign = std::numeric_limits<T>::is_signed ? 1 : 0;

   // digits10 gives us the maximum number of base 10 digits that can be
   // represented, but allow 1 extra as we may not be able to represent all
   // values 0-9 but could represent 0-4...

   constexpr size_t spaceRequiredForMaxDigits = std::numeric_limits<T>::digits10 + 1;

   constexpr size_t spaceRequiredForNullTerm = 1;

   return spaceRequiredForSign + spaceRequiredForMaxDigits + spaceRequiredForNullTerm;
#endif
}

string ToStringA(
   const unsigned int val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%u", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const unsigned int val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%u", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const signed int val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%i", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const signed int val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%i", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const unsigned short val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%hu", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const unsigned short val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%hu", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const signed short val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%hi", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const signed short val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%hi", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const unsigned long val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%lu", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const unsigned long val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%lu", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const signed long val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%li", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const signed long val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%li", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const unsigned long long val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%llu", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const unsigned long long val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%llu", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const signed long long val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%lli", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const signed long long val)
{
   const size_t bufferSize = SpaceRequiredForType(val);

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%lli", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const float val)
{
   return ToStringA(static_cast<const double>(val));
}

wstring ToStringW(
   const float val)
{
   return ToStringW(static_cast<const double>(val));
}

inline unsigned short ValidatePrecision(
   unsigned short precision)
{
   if (precision == 0)
   {
      // If no precision is supplied then you get 6, so we
      // default to 6

      precision = 6;
   }
   else if (precision > 17)
   {
      // Prior to VS2015 sprintf() would only ever return
      // 17 digits of precision, so 3.1415899999999999 sprintf'd
      // with a precision of 30 would end up as 3.1415899999999999.
      // VS2015 (and GNU) return more, 3.14158999999999988261834005243
      // To retain backwards compatibility we truncate the precision to
      // 17

      precision = 17;
   }

   return precision;
}

string ToStringA(
   const double val,
   unsigned short precision)
{
   const size_t bufferSize = 256 + 1 + 1;

   char buffer[bufferSize];

   precision = ValidatePrecision(precision);

   if (-1 == sprintf_s(buffer, bufferSize, "%.*g", precision, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const double val,
   unsigned short precision)
{
   const size_t bufferSize = 256 + 1 + 1;

   wchar_t buffer[bufferSize];

   precision = ValidatePrecision(precision);

   if (-1 == swprintf_s(buffer, bufferSize, L"%.*g", precision, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const long double val,
   unsigned short precision)
{
   const size_t bufferSize = 256 + 1 + 1;

   char buffer[bufferSize];

   precision = ValidatePrecision(precision);

   if (-1 == sprintf_s(buffer, bufferSize, "%.*Lg", precision, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const long double val,
   unsigned short precision)
{
   const size_t bufferSize = 256 + 1 + 1;

   wchar_t buffer[bufferSize];

   precision = ValidatePrecision(precision);

   if (-1 == swprintf_s(buffer, bufferSize, L"%.*Lg", precision, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const void *val)
{
   const size_t bufferSize = 2 + 16 + 1;

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%p", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const void *val)
{
   const size_t bufferSize = 2 + 16 + 1;

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%p", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

inline bool IsUpperCaseRepresentation(
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return (hexDigitRepresentation & HexDigitsUpperCase) == HexDigitsUpperCase;
}

inline bool IncludePrefix(
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return (hexDigitRepresentation & HexDigitsWithPrefix) == HexDigitsWithPrefix;
}

inline bool IncludePrefixEveryByte(
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return (hexDigitRepresentation & HexDigitsPrefixEveryByte) == HexDigitsPrefixEveryByte;
}

inline bool IncludePadding(
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return (hexDigitRepresentation & HexDigitsWithPadding) == HexDigitsWithPadding;
}

#define DETERMINE_FORMAT_STRING_A(rep, w, fU, fL)           \
   (IncludePrefix(rep) ?                                    \
      (IsUpperCaseRepresentation(rep) ?                     \
      (IncludePadding(rep) ? "0x%" w fU : "0x%" fU) :       \
         (IncludePadding(rep) ? "0x%" w fL : "0x%" fL)) :   \
      (IsUpperCaseRepresentation(rep) ?                     \
         (IncludePadding(rep) ? "%" w fU : "%" fU) :        \
         (IncludePadding(rep) ? "%" w fL : "%" fL)))

#define DETERMINE_FORMAT_STRING_W(rep, w, fU, fL)           \
   (IncludePrefix(rep) ?                                    \
      (IsUpperCaseRepresentation(rep) ?                     \
      (IncludePadding(rep) ? L"0x%" w fU : L"0x%" fU) :     \
         (IncludePadding(rep) ? L"0x%" w fL : L"0x%" fL)) : \
      (IsUpperCaseRepresentation(rep) ?                     \
         (IncludePadding(rep) ? L"%" w fU : L"%" fU) :      \
         (IncludePadding(rep) ? L"%" w fL : L"%" fL)))

typedef unsigned __int64 PointerValueType;

string PointerToStringA(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 200 + 2 + 16 + 1;

   char buffer[bufferSize];

   const auto value = reinterpret_cast<PointerValueType>(val);

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "16.16", "I64X", "I64x");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;
}

wstring PointerToStringW(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 200 + 2 + 16 + 1;

   wchar_t buffer[bufferSize];

   const auto value = reinterpret_cast<PointerValueType>(val);

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"16.16", L"I64X", L"I64x");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;
}

string ToHexStringA(
   const unsigned char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 200 + 2 + 2 + 1;

   char buffer[bufferSize];

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "2.2", "X", "x");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;
}

wstring ToHexStringW(
   const unsigned char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 200 + 2 + 2 + 1;

   wchar_t buffer[bufferSize];

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"2.2", L"X", L"x");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;
}

string ToHexStringA(
   const signed char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return ToHexStringA(static_cast<unsigned char>(val), hexDigitRepresentation);
}

wstring ToHexStringW(
   const signed char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return ToHexStringW(static_cast<unsigned char>(val), hexDigitRepresentation);
}

string ToHexStringA(
   const unsigned int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 200 + 2 + 8 + 1;

   char buffer[bufferSize];

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "8.8", "X", "x");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;
}

wstring ToHexStringW(
   const unsigned int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 200 + 2 + 8 + 1;

   wchar_t buffer[bufferSize];

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"8.8", L"X", L"x");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;
}

string ToHexStringA(
   const signed int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return ToHexStringA(static_cast<unsigned int>(val), hexDigitRepresentation);
}

wstring ToHexStringW(
   const signed int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return ToHexStringW(static_cast<unsigned int>(val), hexDigitRepresentation);
}

string ToHexStringA(
   const unsigned short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 200 + 2 + 4 + 1;

   char buffer[bufferSize];

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "4.4", "hX", "hx");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;
}

wstring ToHexStringW(
   const unsigned short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 200 + 2 + 4 + 1;

   wchar_t buffer[bufferSize];

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"4.4", L"hX", L"hx");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;
}

string ToHexStringA(
   const signed short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return ToHexStringA(static_cast<unsigned short>(val), hexDigitRepresentation);
}

wstring ToHexStringW(
   const signed short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return ToHexStringW(static_cast<unsigned short>(val), hexDigitRepresentation);
}

string ToHexStringA(
   const unsigned long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 200 + 2 + 8 + 1;

   char buffer[bufferSize];

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "8.8", "lX", "lx");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;
}

wstring ToHexStringW(
   const unsigned long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 200 + 2 + 8 + 1;

   wchar_t buffer[bufferSize];

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"8.8", L"lX", L"lx");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;
}

string ToHexStringA(
   const signed long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return ToHexStringA(static_cast<unsigned long>(val), hexDigitRepresentation);
}

wstring ToHexStringW(
   const signed long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return ToHexStringW(static_cast<unsigned long>(val), hexDigitRepresentation);
}

string ToHexStringA(
   const unsigned long long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 200 + 2 + 16 + 1;

   char buffer[bufferSize];

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "16.16", "llX", "llx");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;
}

wstring ToHexStringW(
   const unsigned long long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 200 + 2 + 16 + 1;

   wchar_t buffer[bufferSize];

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"16.16", L"llX", L"llx");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;
}

string ToHexStringA(
   const signed long long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return ToHexStringA(static_cast<unsigned long long>(val), hexDigitRepresentation);
}

wstring ToHexStringW(
   const signed long long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return ToHexStringW(static_cast<unsigned long long>(val), hexDigitRepresentation);
}

// Since there isn't a %p for lower case hex conversion of pointer types we
// do it all ourselves.

string ToHexStringA(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
#if defined(_WIN64)

   static const size_t bufferSize = 200 + 2 + 16 + 1;

   char buffer[bufferSize];

   const __int64 value = reinterpret_cast<__int64>(val);

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "16.16", "I64X", "I64x");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;

#else

   static const size_t bufferSize = 2 + 8 + 1;

   char buffer[bufferSize];

   const auto value = reinterpret_cast<ULONG_PTR>(val);

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "8.8", "lX", "lx");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;

#endif
}

wstring ToHexStringW(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
#if defined(_WIN64)

   static const size_t bufferSize = 200 + 2 + 16 + 1;

   wchar_t buffer[bufferSize];

   const ULONG_PTR value = reinterpret_cast<ULONG_PTR>(val);

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"16.16", L"I64X", L"I64x");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;

#else

   static const size_t bufferSize = 2 + 8 + 1;

   wchar_t buffer[bufferSize];

   const auto value = reinterpret_cast<ULONG_PTR>(val);

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"8.8", L"lX", L"lx");

   JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
   JETBYTE_WARNING_SUPPRESS_POP

   return buffer;

#endif
}

string ToHexStringA(
   const void *pData,
   const size_t length,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   const auto *pBytes = static_cast<const BYTE *>(pData);

   string result;

   const size_t outputSize = IncludePrefixEveryByte(hexDigitRepresentation) ? (4 * length) : (((IncludePrefix(hexDigitRepresentation) ? 1 : 0) + length) * 2);

   result.reserve(outputSize);

   if (IncludePrefix(hexDigitRepresentation))
   {
      result += "0x";
   }

   for (size_t i = 0; i < length; i++)
   {
      if (((hexDigitRepresentation & HexDigitsSpacesBetween) == HexDigitsSpacesBetween) &&
          i > 0)
      {
         if (IncludePrefixEveryByte((hexDigitRepresentation)))
         {
            result += ", 0x";
         }
         else
         {
            result += " ";
         }
      }

      BYTE c;

      auto b = static_cast<BYTE>(pBytes[i] >> 4);

      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + (IsUpperCaseRepresentation(hexDigitRepresentation) ? 'A' : 'a'));
      }

      result += static_cast<char>(c);

      b = static_cast<BYTE>(pBytes[i] & 0x0f);

      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + (IsUpperCaseRepresentation(hexDigitRepresentation) ? 'A' : 'a'));
      }

      result += static_cast<char>(c);
   }

   return result;
}

wstring ToHexStringW(
   const void *pData,
   const size_t length,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   const auto *pBytes = static_cast<const BYTE *>(pData);

   wstring result;

   const size_t outputSize = IncludePrefixEveryByte(hexDigitRepresentation) ? (4 * length) : (((IncludePrefix(hexDigitRepresentation) ? 1 : 0) + length) * 2);

   result.reserve(outputSize);

   if (IncludePrefix(hexDigitRepresentation))
   {
      result += L"0x";
   }

   for (size_t i = 0; i < length; i++)
   {
      if (((hexDigitRepresentation & HexDigitsSpacesBetween) == HexDigitsSpacesBetween) &&
          i > 0)
      {
         if (IncludePrefixEveryByte((hexDigitRepresentation)))
         {
            result += L", 0x";
         }
         else
         {
            result += L" ";
         }
      }

      BYTE c;

      auto b = static_cast<BYTE>(pBytes[i] >> 4);

      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + (IsUpperCaseRepresentation(hexDigitRepresentation) ? 'A' : 'a'));
      }

      result += static_cast<wchar_t>(c);

      b = static_cast<BYTE>(pBytes[i] & 0x0f);

      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + (IsUpperCaseRepresentation(hexDigitRepresentation) ? 'A' : 'a'));
      }

      result += static_cast<char>(c);
   }

   return result;
}

_tstring BoolAsString(
   const bool value)
{
   return value ? _T("True") : _T("False");
}

string BoolAsStringA(
   const bool value)
{
   return value ? "True" : "False";
}

string ToHexA(
   const BYTE val)
{
   static const size_t bufferSize = 2 + 1;

   char buffer[3];

   const unsigned int i = val;

   if (-1 == sprintf_s(buffer, bufferSize, "%02X", i))
   {
      throw CException(_T("ToHex"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToHexW(
   const BYTE val)
{
   static const size_t bufferSize = 2 + 1;

   wchar_t buffer[3];

   const int i = val;

   if (-1 == swprintf_s(buffer, bufferSize, L"%02X", i))
   {
      throw CException(_T("ToHex"), _T("sprintf_s failed"));
   }

   return buffer;
}

static const string s_emptyStringLinePrefixA;
static const wstring s_emptyStringLinePrefixW;

string MakePrintableA(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const bool useCR)
{
   return InternalDumpDataA(s_emptyStringLinePrefixA, pData, dataLength, lineLength, DumpType::Printable, useCR, true, true);
}

wstring MakePrintableW(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const bool useCR)
{
   return InternalDumpDataW(s_emptyStringLinePrefixW, pData, dataLength, lineLength, DumpType::Printable, useCR, true, true);
}

string DumpDataA(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const bool useCR)
{
   return InternalDumpDataA(s_emptyStringLinePrefixA, pData, dataLength, lineLength, DumpType::Both, useCR, true, true);
}

wstring DumpDataW(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const bool useCR)
{
   return InternalDumpDataW(s_emptyStringLinePrefixW, pData, dataLength, lineLength, DumpType::Both, useCR, true, true);
}

string DumpDataA(
   const string &linePrefix,
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const bool useCR,
   const bool linePrefixOnFirstLine,
   const bool lineFeedOnLastLine)
{
   return InternalDumpDataA(linePrefix, pData, dataLength, lineLength, DumpType::Both, useCR, linePrefixOnFirstLine, lineFeedOnLastLine);
}

wstring DumpDataW(
   const wstring &linePrefix,
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const bool useCR,
   const bool linePrefixOnFirstLine,
   const bool lineFeedOnLastLine)
{
   return InternalDumpDataW(linePrefix, pData, dataLength, lineLength, DumpType::Both, useCR, linePrefixOnFirstLine, lineFeedOnLastLine);
}

string DumpDataAsHexA(
   const BYTE * const pData,
   const size_t dataLength)
{
   return InternalDumpDataA(s_emptyStringLinePrefixA, pData, dataLength, 0, DumpType::HexDump, false, false, false);
}

wstring DumpDataAsHexW(
   const BYTE * const pData,
   const size_t dataLength)
{
   return InternalDumpDataW(s_emptyStringLinePrefixW, pData, dataLength, 0, DumpType::HexDump, false, false, false);
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static string InternalDumpDataA(
   const string &linePrefix,
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const DumpType dumpType,
   const bool useCR,
   const bool linePrefixOnFirstLine,
   const bool lineFeedOnLastLine)
{
   const string lineSep = useCR ? "\r\n" : "\n";

   const size_t bytesPerLine = lineLength != 0 ? (lineLength - 1) / 3 : 0;

   string result;

   string hexDisplay;
   string display;

   size_t i = 0;

   bool firstLine = true;

   while (i < dataLength)
   {
      const BYTE c = pData[i++];

      if (dumpType & DumpType::HexDump)
      {
         hexDisplay += ToHexA(c) + " ";
      }

      if (dumpType & DumpType::Printable)
      {
         if (isprint(c))
         {
            display += static_cast<char>(c);
         }
         else
         {
            display += '.';
         }
      }

      if ((bytesPerLine && (i % bytesPerLine == 0 && i != 0)) || i == dataLength)
      {
         if (dumpType & DumpType::HexDump)
         {
            if (i == dataLength && (bytesPerLine && (i % bytesPerLine != 0)))
            {
               for (size_t pad = i % bytesPerLine; pad < bytesPerLine; pad++)
               {
                  hexDisplay += "   ";
               }
            }
         }

         const string prefix = firstLine ? (linePrefixOnFirstLine ? linePrefix : "" ) : linePrefix;

         result += prefix;

         if (dumpType & DumpType::HexDump)
         {
            result += hexDisplay;

            if (dumpType & DumpType::Printable)
            {
               result += " - ";
            }
         }

         if (dumpType & DumpType::Printable)
         {
            result += display;
         }

         result += (bytesPerLine ? lineSep : "");

         firstLine = false;

         hexDisplay = "";
         display = "";
      }
   }

   if (!lineFeedOnLastLine)
   {
      result = result.substr(0, result.length() - lineSep.length());
   }

   return result;
}

static wstring InternalDumpDataW(
   const wstring &linePrefix,
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const DumpType dumpType,
   const bool useCR,
   const bool linePrefixOnFirstLine,
   const bool lineFeedOnLastLine)
{
   const wstring lineSep = useCR ? L"\r\n" : L"\n";

   const size_t bytesPerLine = lineLength != 0 ? (lineLength - 1) / 3 : 0;

   wstring result;

   wstring hexDisplay;
   wstring display;

   size_t i = 0;

   bool firstLine = true;

   while (i < dataLength)
   {
      const BYTE c = pData[i++];

      if (dumpType & DumpType::HexDump)
      {
         hexDisplay += ToHexW(c) + L" ";
      }

      if (dumpType & DumpType::Printable)
      {
         if (isprint(c))
         {
            display += static_cast<wchar_t>(c);
         }
         else
         {
            display += L'.';
         }
      }

      if ((bytesPerLine && (i % bytesPerLine == 0 && i != 0)) || i == dataLength)
      {
         if (dumpType & DumpType::HexDump)
         {
            if (i == dataLength && (bytesPerLine && (i % bytesPerLine != 0)))
            {
               for (size_t pad = i % bytesPerLine; pad < bytesPerLine; pad++)
               {
                  hexDisplay += L"   ";
               }
            }
         }

         const wstring prefix = firstLine ? (linePrefixOnFirstLine ? linePrefix : L"" ) : linePrefix;

         result += prefix;

         if (dumpType & DumpType::HexDump)
         {
            result += hexDisplay;

            if (dumpType & DumpType::Printable)
            {
               result += L" - ";
            }
         }

         if (dumpType & DumpType::Printable)
         {
            result += display;
         }

         result += (bytesPerLine ? lineSep : L"");

         firstLine = false;

         hexDisplay = L"";
         display = L"";
      }
   }

   if (!lineFeedOnLastLine)
   {
      result = result.substr(0, result.length() - lineSep.length());
   }

   return result;
}

bool IsAllPrintable(
   const BYTE *pData,
   const size_t length)
{
   for (size_t i = 0; i < length; ++i)
   {
      const BYTE b = *(pData + i);

      if (!isspace(b) && !isprint(b))
      {
         return false;
      }
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ToString.cpp
///////////////////////////////////////////////////////////////////////////////
