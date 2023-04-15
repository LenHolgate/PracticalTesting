#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ToString.h
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

#include "Types.h"

#include "tstring.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Functions defined in this file...
///////////////////////////////////////////////////////////////////////////////

unsigned short CalculateRequiredPrecision(
   double value);

enum ToHexStringHexDigitRepresentation
{
   HexDigitsLowerCase       = 0x00000,
   HexDigitsUpperCase       = 0x00001,
   HexDigitsNoPrefix        = 0x00000,
   HexDigitsWithPrefix      = 0x00010,      // Prefix with 0x
   HexDigitsNoPadding       = 0x00000,
   HexDigitsWithPadding     = 0x00100,      // Pad to full width for type with leading 0's
   HexDigitsSpacesBetween   = 0x01000,      // Only for blocks of 'void *' data,
                                            // space between each digit eg "00 AF FF" rather than "00AFFF"
   HexDigitsPrefixEveryByte = 0x11010,      // eg 0x00, 0xAF, 0xFF

   HexDigitsNoPrefixUpperCase                = HexDigitsUpperCase | HexDigitsNoPadding | HexDigitsNoPrefix,
   HexDigitsNoPrefixLowerCase                = HexDigitsLowerCase | HexDigitsNoPadding | HexDigitsNoPrefix,

   HexDigitsWithPrefixUpperCase              = HexDigitsUpperCase | HexDigitsNoPadding | HexDigitsWithPrefix,
   HexDigitsWithPrefixLowerCase              = HexDigitsLowerCase | HexDigitsNoPadding | HexDigitsWithPrefix,

   HexDigitsNoPaddingNoPrefixUpperCase       = HexDigitsUpperCase | HexDigitsNoPadding | HexDigitsNoPrefix,
   HexDigitsNoPaddingNoPrefixLowerCase       = HexDigitsLowerCase | HexDigitsNoPadding | HexDigitsNoPrefix,

   HexDigitsWithPaddingNoPrefixUpperCase     = HexDigitsUpperCase | HexDigitsWithPadding | HexDigitsNoPrefix,
   HexDigitsWithPaddingNoPrefixLowerCase     = HexDigitsLowerCase | HexDigitsWithPadding | HexDigitsNoPrefix,

   HexDigitsNoPaddingWithPrefixUpperCase     = HexDigitsUpperCase | HexDigitsNoPadding | HexDigitsWithPrefix,
   HexDigitsNoPaddingWithPrefixLowerCase     = HexDigitsLowerCase | HexDigitsNoPadding | HexDigitsWithPrefix,

   HexDigitsWithPaddingWithPrefixUpperCase   = HexDigitsUpperCase | HexDigitsWithPadding | HexDigitsWithPrefix,
   HexDigitsWithPaddingWithPrefixLowerCase   = HexDigitsLowerCase | HexDigitsWithPadding | HexDigitsWithPrefix,

   HexDigitsDefault                          = HexDigitsWithPrefixLowerCase
};

// In version 6.0 we changed how we convert numeric types to strings. Prior to this
// version we used strstream and templates and streamed the type into a string. However
// this requires the acquisition of a process wide lock (for accessing locales and facets)
// in some versions of STL which is less than ideal for multi-threaded use.
// From version 6.0 we default to using a custom sprintf based system to convert from
// numbers to strings. We also decided to change how 0 is represened in ToHexString(),
// it's converted to 0x0 in the new code whereas it was converted to 0 in the old.
// ToHexString() was also enhanced to allow for the user to decide if they want upper
// or lower case hex digits and we added PointerToString() which gives a consistent
// represenation of pointers across 32bit and 64bit operating systems (always
// representing pointers as 64bit hex values). To finish all of these things were moved
// out of Utils.h and into this header file.

std::string ToStringA(
   bool val);

std::wstring ToStringW(
   bool val);

inline _tstring ToString(
   const bool val)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   unsigned int val);

std::wstring ToStringW(
   unsigned int val);

inline _tstring ToString(
   const unsigned int val)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   signed int val);

std::wstring ToStringW(
   signed int val);

inline _tstring ToString(
   const signed int val)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   unsigned short val);

std::wstring ToStringW(
   unsigned short val);

inline _tstring ToString(
   const unsigned short val)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   signed short val);

std::wstring ToStringW(
   signed short val);

inline _tstring ToString(
   const signed short val)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   unsigned long val);

std::wstring ToStringW(
   unsigned long val);

inline _tstring ToString(
   const unsigned long val)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   signed long val);

std::wstring ToStringW(
   signed long val);

inline _tstring ToString(
   const signed long val)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   unsigned long long val);

std::wstring ToStringW(
   unsigned long long val);

inline _tstring ToString(
   const unsigned long long val)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   signed long long val);

std::wstring ToStringW(
   signed long long val);

inline _tstring ToString(
   const signed long long val)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   float val);

std::wstring ToStringW(
   float val);

inline _tstring ToString(
   const float val)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   double val,
   unsigned short precision = 0);

std::wstring ToStringW(
   double val,
   unsigned short precision = 0);

inline _tstring ToString(
   const double val,
   const unsigned short precision = 0)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val, precision);
#else
   return ToStringA(val, precision);
#endif
}

std::string ToStringA(
   long double val,
   unsigned short precision = 0);

std::wstring ToStringW(
   long double val,
   unsigned short precision = 0);

inline _tstring ToString(
   const long double val,
   const unsigned short precision = 0)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val, precision);
#else
   return ToStringA(val, precision);
#endif
}

std::string ToStringA(
   const void *val);

std::wstring ToStringW(
   const void *val);

inline _tstring ToString(
   const void *val)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string PointerToStringA(
   const void *val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingNoPrefixUpperCase);

std::wstring PointerToStringW(
   const void *val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingNoPrefixUpperCase);

inline _tstring PointerToString(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingNoPrefixUpperCase)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return PointerToStringW(val, hexDigitRepresentation);
#else
   return PointerToStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   unsigned char val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   unsigned char val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const unsigned char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   signed char val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   signed char val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const signed char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   unsigned int val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   unsigned int val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const unsigned int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   signed int val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   signed int val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const signed int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   unsigned short val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   unsigned short val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const unsigned short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   signed short val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   signed short val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const signed short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   unsigned long val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   unsigned long val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const unsigned long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   signed long val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   signed long val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const signed long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   unsigned long long val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   unsigned long long val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const unsigned long long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   signed long long val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   signed long long val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const signed long long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const void *val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingWithPrefixLowerCase);

std::wstring ToHexStringW(
   const void *val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingWithPrefixLowerCase);

inline _tstring ToHexString(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingWithPrefixLowerCase)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const void *pData,
   size_t length,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const void *pData,
   size_t length,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const void *pData,
   const size_t length,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexStringW(pData, length, hexDigitRepresentation);
#else
   return ToHexStringA(pData, length, hexDigitRepresentation);
#endif
}

inline std::string ToByteArrayInitialiserA(
   const void *pData,
   const size_t length,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
   return ToHexStringA(pData, length, static_cast<ToHexStringHexDigitRepresentation>(hexDigitRepresentation | HexDigitsPrefixEveryByte));
}

inline std::wstring ToByteArrayInitialiserW(
   const void *pData,
   const size_t length,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
   return ToHexStringW(pData, length, static_cast<ToHexStringHexDigitRepresentation>(hexDigitRepresentation | HexDigitsPrefixEveryByte));
}

inline _tstring ToByteArrayInitialiser(
   const void *pData,
   const size_t length,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToByteArrayInitialiserW(pData, length, hexDigitRepresentation);
#else
   return ToByteArrayInitialiserA(pData, length, hexDigitRepresentation);
#endif
}

std::string ToHexA(
   BYTE val);

std::wstring ToHexW(
   BYTE val);

inline _tstring ToHex(
   const BYTE val)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return ToHexW(val);
#else
   return ToHexA(val);
#endif
}

std::string MakePrintableA(
   const BYTE *pData,
   size_t dataLength,
   size_t lineLength = 0,
   bool useCR = false);

std::wstring MakePrintableW(
   const BYTE *pData,
   size_t dataLength,
   size_t lineLength = 0,
   bool useCR = false);

inline _tstring MakePrintable(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength = 0,
   const bool useCR = false)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return MakePrintableW(pData, dataLength, lineLength, useCR);
#else
   return MakePrintableA(pData, dataLength, lineLength, useCR);
#endif
}

std::string DumpDataA(
   const BYTE *pData,
   size_t dataLength,
   size_t lineLength = 0,
   bool useCR = false);

std::wstring DumpDataW(
   const BYTE *pData,
   size_t dataLength,
   size_t lineLength = 0,
   bool useCR = false);

inline _tstring DumpData(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength = 0,
   const bool useCR = false)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return DumpDataW(pData, dataLength, lineLength, useCR);
#else
   return DumpDataA(pData, dataLength, lineLength, useCR);
#endif
}

std::string DumpDataA(
   const std::string &linePrefix,
   const BYTE *pData,
   size_t dataLength,
   size_t lineLength = 0,
   bool useCR = false,
   bool linePrefixOnFirstLine = true,
   bool lineFeedOnLastLine = true);

std::wstring DumpDataW(
   const std::wstring &linePrefix,
   const BYTE *pData,
   size_t dataLength,
   size_t lineLength = 0,
   bool useCR = false,
   bool linePrefixOnFirstLine = true,
   bool lineFeedOnLastLine = true);

inline _tstring DumpData(
   const _tstring &linePrefix,
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength = 0,
   const bool useCR = false,
   const bool linePrefixOnFirstLine = true,
   const bool lineFeedOnLastLine = true)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return DumpDataW(linePrefix, pData, dataLength, lineLength, useCR, linePrefixOnFirstLine, lineFeedOnLastLine);
#else
   return DumpDataA(linePrefix, pData, dataLength, lineLength, useCR, linePrefixOnFirstLine, lineFeedOnLastLine);
#endif
}

std::string DumpDataAsHexA(
   const BYTE *pData,
   size_t dataLength);

std::wstring DumpDataAsHexW(
   const BYTE *pData,
   size_t dataLength);

inline _tstring DumpDataAsHex(
   const BYTE * const pData,
   const size_t dataLength)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
   return DumpDataAsHexW(pData, dataLength);
#else
   return DumpDataAsHexA(pData, dataLength);
#endif
}

_tstring BoolAsString(
   bool value);

std::string BoolAsStringA(
   bool value);

bool IsAllPrintable(
   const BYTE *pData,
   const size_t length);

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ToString.h
///////////////////////////////////////////////////////////////////////////////

