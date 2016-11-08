///////////////////////////////////////////////////////////////////////////////
// File: ToString.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2008 JetByte Limited.
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
#include "JetByteTools\Admin\SecureCRT.h"

#include "Utils.h"            // For ToBool()
#include "ToString.h"
#include "Exception.h"

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
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static string InternalDumpDataA(
   const string &linePrefix,
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const bool makePrintable,
   const bool useCR,
   const bool linePrefixOnFirstLine,
   const bool lineFeedOnLastLine);

static wstring InternalDumpDataW(
   const wstring &linePrefix,
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const bool makePrintable,
   const bool useCR,
   const bool linePrefixOnFirstLine,
   const bool lineFeedOnLastLine);

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

                  precision = numDigits - 1;
               }

               zeroes++;
            }
            else if (*p == '9')
            {
               if (zeroes)
               {
                  zeroes = 0;

                  precision = numDigits - 1;
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

string ToStringA(
   const unsigned int val)
{
   const size_t bufferSize = 10 + 1;

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
   const size_t bufferSize = 10 + 1;

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
   const size_t bufferSize = 10 + 1 + 1;

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
   const size_t bufferSize = 10 + 1 + 1;

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
   const size_t bufferSize = 5 + 1;

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
   const size_t bufferSize = 5 + 1;

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
   const size_t bufferSize = 5 + 1 + 1;

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
   const size_t bufferSize = 5 + 1 + 1;

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
   const size_t bufferSize = 10 + 1;

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
   const size_t bufferSize = 10 + 1;

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
   const size_t bufferSize = 10 + 1 + 1;

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
   const size_t bufferSize = 10 + 1 + 1;

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%li", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const unsigned __int64 val)
{
   const size_t bufferSize = 20 + 1;

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%I64u", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const unsigned __int64 val)
{
   const size_t bufferSize = 20 + 1;

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%I64u", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const signed __int64 val)
{
   const size_t bufferSize = 19 + 1 + 1;

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%I64i", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const signed __int64 val)
{
   const size_t bufferSize = 19 + 1 + 1;

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%I64i", val))
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
   const size_t bufferSize = 16 + 1;

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
   const size_t bufferSize = 16 + 1;

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

inline bool IncludePadding(
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return (hexDigitRepresentation & HexDigitsWithPadding) == HexDigitsWithPadding;
}

#define DETERMINE_FORMAT_STRING_A(rep, w, fU, fL)           \
   IncludePrefix(rep) ?                                     \
      (IsUpperCaseRepresentation(rep) ?                     \
      (IncludePadding(rep) ? "0x%" w fU : "0x%" fU) :       \
         (IncludePadding(rep) ? "0x%" w fL : "0x%" fL)) :   \
      (IsUpperCaseRepresentation(rep) ?                     \
         (IncludePadding(rep) ? "%" w fU : "%" fU) :        \
         (IncludePadding(rep) ? "%" w fL : "%" fL))

#define DETERMINE_FORMAT_STRING_W(rep, w, fU, fL)           \
   IncludePrefix(rep) ?                                     \
      (IsUpperCaseRepresentation(rep) ?                     \
      (IncludePadding(rep) ? L"0x%" w fU : L"0x%" fU) :       \
         (IncludePadding(rep) ? L"0x%" w fL : L"0x%" fL)) :   \
      (IsUpperCaseRepresentation(rep) ?                     \
         (IncludePadding(rep) ? L"%" w fU : L"%" fU) :        \
         (IncludePadding(rep) ? L"%" w fL : L"%" fL))

string PointerToStringA(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 16 + 1;

   char buffer[bufferSize];

   unsigned __int64 value = reinterpret_cast<unsigned __int64>(val);

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "16.16", "I64X", "I64x");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

   return buffer;
}

wstring PointerToStringW(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 16 + 1;

   wchar_t buffer[bufferSize];

   unsigned __int64 value = reinterpret_cast<unsigned __int64>(val);

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"16.16", L"I64X", L"I64x");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

   return buffer;
}

string ToHexStringA(
   const unsigned char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 2 + 1;

   char buffer[bufferSize];

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "2.2", "X", "x");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

   return buffer;
}

wstring ToHexStringW(
   const unsigned char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 2 + 1;

   wchar_t buffer[bufferSize];

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"2.2", L"X", L"x");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

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
   static const size_t bufferSize = 2 + 8 + 1;

   char buffer[bufferSize];

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "8.8", "X", "x");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

   return buffer;
}

wstring ToHexStringW(
   const unsigned int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 8 + 1;

   wchar_t buffer[bufferSize];

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"8.8", L"X", L"x");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

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
   static const size_t bufferSize = 2 + 4 + 1;

   char buffer[bufferSize];

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "4.4", "hX", "hx");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

   return buffer;
}

wstring ToHexStringW(
   const unsigned short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 4 + 1;

   wchar_t buffer[bufferSize];

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"4.4", L"hX", L"hx");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

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
   static const size_t bufferSize = 2 + 8 + 1;

   char buffer[bufferSize];

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "8.8", "lX", "lx");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

   return buffer;
}

wstring ToHexStringW(
   const unsigned long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 8 + 1;

   wchar_t buffer[bufferSize];

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"8.8", L"lX", L"lx");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

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
   const unsigned __int64 val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 16 + 1;

   char buffer[bufferSize];

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "16.16", "I64X", "I64x");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

   return buffer;
}

wstring ToHexStringW(
   const unsigned __int64 val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 16 + 1;

   wchar_t buffer[bufferSize];

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"16.16", L"I64X", L"I64x");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

   return buffer;
}

string ToHexStringA(
   const signed __int64 val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return ToHexStringA(static_cast<unsigned __int64>(val), hexDigitRepresentation);
}

wstring ToHexStringW(
   const signed __int64 val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   return ToHexStringW(static_cast<unsigned __int64>(val), hexDigitRepresentation);
}

// Since there isn't a %p for lower case hex conversion of pointer types we
// do it all ourselves.

string ToHexStringA(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
#if defined(_WIN64)

   static const size_t bufferSize = 2 + 16 + 1;

   char buffer[bufferSize];

   const __int64 value = reinterpret_cast<__int64>(val);

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "16.16", "I64X", "I64x");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

   return buffer;

#else

   static const size_t bufferSize = 2 + 8 + 1;

   char buffer[bufferSize];

   const ULONG_PTR value = reinterpret_cast<ULONG_PTR>(val);

   const char *pFormat = DETERMINE_FORMAT_STRING_A(hexDigitRepresentation, "8.8", "lX", "lx");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == sprintf_s(buffer, bufferSize, pFormat, value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

   return buffer;

#endif
}

wstring ToHexStringW(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
#if defined(_WIN64)

   static const size_t bufferSize = 2 + 16 + 1;

   wchar_t buffer[bufferSize];

   const ULONG_PTR value = reinterpret_cast<ULONG_PTR>(val);

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"16.16", L"I64X", L"I64x");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

   return buffer;

#else

   static const size_t bufferSize = 2 + 8 + 1;

   wchar_t buffer[bufferSize];

   const ULONG_PTR value = reinterpret_cast<ULONG_PTR>(val);

   const wchar_t *pFormat = DETERMINE_FORMAT_STRING_W(hexDigitRepresentation, L"8.8", L"lX", L"lx");

#pragma warning(push)
#pragma warning(disable: 4774)      // format string expected in argument 3 is not a string literal
   if (-1 == swprintf_s(buffer, bufferSize, pFormat, value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }
#pragma warning(pop)

   return buffer;

#endif
}

string ToHexStringA(
   const void *pData,
   const size_t length,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   const BYTE *pBytes = reinterpret_cast<const BYTE *>(pData);

   string result;

   const size_t outputSize = ((IncludePrefix(hexDigitRepresentation) ? 1 : 0) + length) * 2;

   result.reserve(outputSize);

   if (IncludePrefix(hexDigitRepresentation))
   {
      result += "0x";
   }

   for (size_t i = 0; i < length; i++)
   {
      BYTE c ;

      BYTE b = static_cast<BYTE>(pBytes[i] >> 4);

      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + (IsUpperCaseRepresentation(hexDigitRepresentation) ? 'A' : 'a'));
      }

      result += (char)c;

      b = static_cast<BYTE>(pBytes[i] & 0x0f);

      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + (IsUpperCaseRepresentation(hexDigitRepresentation) ? 'A' : 'a'));
      }

      result += (char)c;
   }

   return result;
}

wstring ToHexStringW(
   const void *pData,
   const size_t length,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   const BYTE *pBytes = reinterpret_cast<const BYTE *>(pData);

   wstring result;

   const size_t outputSize = ((IncludePrefix(hexDigitRepresentation) ? 1 : 0) + length) * 2;

   result.reserve(outputSize);

   if (IncludePrefix(hexDigitRepresentation))
   {
      result += L"0x";
   }

   for (size_t i = 0; i < length; i++)
   {
      BYTE c ;

      BYTE b = static_cast<BYTE>(pBytes[i] >> 4);

      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + (IsUpperCaseRepresentation(hexDigitRepresentation) ? 'A' : 'a'));
      }

      result += (wchar_t)c;

      b = static_cast<BYTE>(pBytes[i] & 0x0f);

      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + (IsUpperCaseRepresentation(hexDigitRepresentation) ? 'A' : 'a'));
      }

      result += (char)c;
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

   const int i = val;

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
   return InternalDumpDataA(s_emptyStringLinePrefixA, pData, dataLength, lineLength, true, useCR, true, true);
}

wstring MakePrintableW(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const bool useCR)
{
   return InternalDumpDataW(s_emptyStringLinePrefixW, pData, dataLength, lineLength, true, useCR, true, true);
}

string DumpDataA(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const bool useCR)
{
   return InternalDumpDataA(s_emptyStringLinePrefixA, pData, dataLength, lineLength, false, useCR, true, true);
}

wstring DumpDataW(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const bool useCR)
{
   return InternalDumpDataW(s_emptyStringLinePrefixW, pData, dataLength, lineLength, false, useCR, true, true);
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
   return InternalDumpDataA(linePrefix, pData, dataLength, lineLength, false, useCR, linePrefixOnFirstLine, lineFeedOnLastLine);
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
   return InternalDumpDataW(linePrefix, pData, dataLength, lineLength, false, useCR, linePrefixOnFirstLine, lineFeedOnLastLine);
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static string InternalDumpDataA(
   const string &linePrefix,
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength,
   const bool makePrintable,
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

      if (!makePrintable)
      {
         hexDisplay += ToHexA(c) + " ";
      }

      #if (VS2013_PREVIEW_BROKEN_ISPRINT == 1)
      if (isprint(c) && c != 9)
      #else
      if (isprint(c))
      #endif
      {
         display += (char)c;
      }
      else
      {
         display += '.';
      }

      if ((bytesPerLine && (i % bytesPerLine == 0 && i != 0)) || i == dataLength)
      {
         if (!makePrintable)
         {
            if (i == dataLength && (bytesPerLine && (i % bytesPerLine != 0)))
            {
               for (size_t pad = i % bytesPerLine; pad < bytesPerLine; pad++)
               {
                  hexDisplay += "   ";
               }
            }
         }

         const string prefix = (firstLine ? (linePrefixOnFirstLine ? linePrefix : "" ) : linePrefix);

         result += prefix + (!makePrintable ? hexDisplay + " - " : "") + display + (bytesPerLine ? lineSep : "");

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
   const bool makePrintable,
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

      if (!makePrintable)
      {
         hexDisplay += ToHexW(c) + L" ";
      }

      #if (VS2013_PREVIEW_BROKEN_ISPRINT == 1)
      if (isprint(c) && c != 9)
      #else
      if (isprint(c))
      #endif
      {
         display += (wchar_t)c;
      }
      else
      {
         display += L'.';
      }

      if ((bytesPerLine && (i % bytesPerLine == 0 && i != 0)) || i == dataLength)
      {
         if (!makePrintable)
         {
            if (i == dataLength && (bytesPerLine && (i % bytesPerLine != 0)))
            {
               for (size_t pad = i % bytesPerLine; pad < bytesPerLine; pad++)
               {
                  hexDisplay += L"   ";
               }
            }
         }

         const wstring prefix = (firstLine ? (linePrefixOnFirstLine ? linePrefix : L"" ) : linePrefix);

         result += prefix + (!makePrintable ? hexDisplay + L" - " : L"") + display + (bytesPerLine ? lineSep : L"");

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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ToString.cpp
///////////////////////////////////////////////////////////////////////////////
