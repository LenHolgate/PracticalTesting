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

#if (JETBYTE_USE_TEMPLATE_TO_STRING == 1)

string HexToStringA(
   const BYTE *pBuffer, 
   size_t iBytes,
   bool upperCase)
{
   string result;
     
   for (size_t i = 0; i < iBytes; i++)
   {
      BYTE c ;

      BYTE b = static_cast<BYTE>(pBuffer[i] >> 4);
         
      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + (upperCase ? 'A' : 'a'));
      }

      result += (char)c;

      b = static_cast<BYTE>(pBuffer[i] & 0x0f);

      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + (upperCase ? 'A' : 'a'));
      }

      result += (char)c;
   }

   return result;
}

_tstring HexToString(
   const BYTE *pBuffer, 
   size_t iBytes)
{
   _tstring result;
     
   for (size_t i = 0; i < iBytes; i++)
   {
      BYTE c ;

      BYTE b = static_cast<BYTE>(pBuffer[i] >> 4);
         
      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + 'A');
      }

      result += (TCHAR)c;

      b = static_cast<BYTE>(pBuffer[i] & 0x0f);

      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + 'A');
      }

      result += (TCHAR)c;
   }

   return result;
}

#else

string ToStringA(
   const unsigned int val)
{
   static const size_t bufferSize = 10 + 1;

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
   static const size_t bufferSize = 10 + 1;

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
   static const size_t bufferSize = 10 + 1 + 1;

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
   static const size_t bufferSize = 10 + 1 + 1;

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
   static const size_t bufferSize = 5 + 1;

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
   static const size_t bufferSize = 5 + 1;

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
   static const size_t bufferSize = 5 + 1 + 1;

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
   static const size_t bufferSize = 5 + 1 + 1;

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
   static const size_t bufferSize = 10 + 1;

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
   static const size_t bufferSize = 10 + 1;

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
   static const size_t bufferSize = 10 + 1 + 1;

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
   static const size_t bufferSize = 10 + 1 + 1;

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
   static const size_t bufferSize = 20 + 1;

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
   static const size_t bufferSize = 20 + 1;

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
   static const size_t bufferSize = 19 + 1 + 1;

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
   static const size_t bufferSize = 19 + 1 + 1;

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

string ToStringA(
   const double val)
{
   static const size_t bufferSize = 20 + 1 + 1;

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%g", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const double val)
{
   static const size_t bufferSize = 20 + 1 + 1;

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%g", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const long double val)
{
   static const size_t bufferSize = 256 + 1 + 1;

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, "%lg", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToStringW(
   const long double val)
{
   static const size_t bufferSize = 256 + 1 + 1;

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%lg", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string ToStringA(
   const void *val)
{
   static const size_t bufferSize = 16 + 1;

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
   static const size_t bufferSize = 16 + 1;

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, L"%p", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

string PointerToStringA(
   const void *val)
{
   static const size_t bufferSize = 16 + 1;

   char buffer[bufferSize];

   unsigned __int64 value = reinterpret_cast<unsigned __int64>(val);

   if (-1 == sprintf_s(buffer, bufferSize, "%16.16I64X", value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring PointerToStringW(
   const void *val)
{
   static const size_t bufferSize = 16 + 1;

   wchar_t buffer[bufferSize];

   unsigned __int64 value = reinterpret_cast<unsigned __int64>(val);

   if (-1 == swprintf_s(buffer, bufferSize, L"%16.16I64X", value))
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

string ToHexStringA(
   const unsigned char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 8 + 1;

   char buffer[bufferSize];

   if (-1 == sprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? "%2.2X" : "%2.2x", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToHexStringW(
   const unsigned char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 8 + 1;

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? L"%2.2X" : L"%2.2x", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

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

   if (-1 == sprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? "0x%X" : "0x%x", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToHexStringW(
   const unsigned int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 8 + 1;

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? L"0x%X" : L"0x%x", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

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

   if (-1 == sprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? "0x%hX" : "0x%hx", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToHexStringW(
   const unsigned short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 4 + 1;

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? L"0x%hX" : L"0x%hx", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

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

   if (-1 == sprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? "0x%lX" : "0x%lx", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToHexStringW(
   const unsigned long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 8 + 1;

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? L"0x%lX" : L"0x%lx", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

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

   if (-1 == sprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? "0x%I64X" : "0x%I64x", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

   return buffer;
}

wstring ToHexStringW(
   const unsigned __int64 val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation)
{
   static const size_t bufferSize = 2 + 16 + 1;

   wchar_t buffer[bufferSize];

   if (-1 == swprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? L"0x%I64X" : L"0x%I64x", val))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

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

   if (-1 == sprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? "0x%16.16I64X" : "0x%16.16I64x", value))
   {
      throw "TODO";
   }

   return buffer;

#else

   static const size_t bufferSize = 2 + 8 + 1;

   char buffer[bufferSize];

   const ULONG_PTR value = reinterpret_cast<ULONG_PTR>(val);

   if (-1 == sprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? "0x%8.8lX" : "0x%8.8lx", value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

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

   if (-1 == swprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? L"0x%16.16llX" : L"0x%16.16llx", value))
   {
      throw "TODO";
   }

   return buffer;

#else

   static const size_t bufferSize = 2 + 8 + 1;

   wchar_t buffer[bufferSize];

   const ULONG_PTR value = reinterpret_cast<ULONG_PTR>(val);

   if (-1 == swprintf_s(buffer, bufferSize, IsUpperCaseRepresentation(hexDigitRepresentation) ? L"0x%8.8lX" : L"0x%8.8lx", value))
   {
      throw CException(_T("ToString"), _T("sprintf_s failed"));
   }

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

#endif


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

string MakePrintableA(
   const BYTE * const pData, 
   const size_t dataLength, 
   const size_t lineLength,
   const bool useCR)
{
   static const string linePrefix;

   return InternalDumpDataA(linePrefix, pData, dataLength, lineLength, true, useCR, true, true);
}

wstring MakePrintableW(
   const BYTE * const pData, 
   const size_t dataLength, 
   const size_t lineLength,
   const bool useCR)
{
   static const wstring linePrefix;

   return InternalDumpDataW(linePrefix, pData, dataLength, lineLength, true, useCR, true, true);
}

string DumpDataA(
   const BYTE * const pData, 
   const size_t dataLength, 
   const size_t lineLength,
   const bool useCR)
{
   static const string linePrefix;

   return InternalDumpDataA(linePrefix, pData, dataLength, lineLength, false, useCR, true, true);
}

wstring DumpDataW(
   const BYTE * const pData, 
   const size_t dataLength, 
   const size_t lineLength,
   const bool useCR)
{
   static const wstring linePrefix;

   return InternalDumpDataW(linePrefix, pData, dataLength, lineLength, false, useCR, true, true);
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

      if (isprint(c))
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

      if (isprint(c))
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
