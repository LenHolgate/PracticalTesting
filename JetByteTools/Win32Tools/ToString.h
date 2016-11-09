#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_TO_STRING__
#define JETBYTE_TOOLS_WIN32_TO_STRING__
///////////////////////////////////////////////////////////////////////////////
// File: ToString.h
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

#include "tstring.h"

#include <wtypes.h>

#include "StringConverter.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Functions defined in this file...
///////////////////////////////////////////////////////////////////////////////

unsigned short CalculateRequiredPrecision(
   const double value);

enum ToHexStringHexDigitRepresentation
{
   HexDigitsLowerCase   = 0x000,
   HexDigitsUpperCase   = 0x001,
   HexDigitsNoPrefix    = 0x000,
   HexDigitsWithPrefix  = 0x010,       // Prefix with 0x
   HexDigitsNoPadding   = 0x000,
   HexDigitsWithPadding = 0x100,       // Pad to full width for type with leading 0's

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
   const bool val);

std::wstring ToStringW(
   const bool val);

inline _tstring ToString(
   const bool val)
{
#ifdef _UNICODE
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   const unsigned int val);

std::wstring ToStringW(
   const unsigned int val);

inline _tstring ToString(
   const unsigned int val)
{
#ifdef _UNICODE
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   const signed int val);

std::wstring ToStringW(
   const signed int val);

inline _tstring ToString(
   const signed int val)
{
#ifdef _UNICODE
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   const unsigned short val);

std::wstring ToStringW(
   const unsigned short val);

inline _tstring ToString(
   const unsigned short val)
{
#ifdef _UNICODE
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   const signed short val);

std::wstring ToStringW(
   const signed short val);

inline _tstring ToString(
   const signed short val)
{
#ifdef _UNICODE
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   const unsigned long val);

std::wstring ToStringW(
   const unsigned long val);

inline _tstring ToString(
   const unsigned long val)
{
#ifdef _UNICODE
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   const signed long val);

std::wstring ToStringW(
   const signed long val);

inline _tstring ToString(
   const signed long val)
{
#ifdef _UNICODE
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   const unsigned __int64 val);

std::wstring ToStringW(
   const unsigned __int64 val);

inline _tstring ToString(
   const unsigned __int64 val)
{
#ifdef _UNICODE
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   const signed __int64 val);

std::wstring ToStringW(
   const signed __int64 val);

inline _tstring ToString(
   const signed __int64 val)
{
#ifdef _UNICODE
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   const float val);

std::wstring ToStringW(
   const float val);

inline _tstring ToString(
   const float val)
{
#ifdef _UNICODE
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   const double val,
   const unsigned short precision = 0);

std::wstring ToStringW(
   const double val,
   const unsigned short precision = 0);

inline _tstring ToString(
   const double val,
   const unsigned short precision = 0)
{
#ifdef _UNICODE
   return ToStringW(val, precision);
#else
   return ToStringA(val, precision);
#endif
}

std::string ToStringA(
   const long double val,
   const unsigned short precision = 0);

std::wstring ToStringW(
   const long double val,
   const unsigned short precision = 0);

inline _tstring ToString(
   const long double val,
   const unsigned short precision = 0)
{
#ifdef _UNICODE
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
#ifdef _UNICODE
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string PointerToStringA(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingNoPrefixUpperCase);

std::wstring PointerToStringW(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingNoPrefixUpperCase);

inline _tstring PointerToString(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingNoPrefixUpperCase)
{
#ifdef _UNICODE
   return PointerToStringW(val, hexDigitRepresentation);
#else
   return PointerToStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const unsigned char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const unsigned char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const unsigned char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef _UNICODE
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const signed char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const signed char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const signed char val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef _UNICODE
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const unsigned int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const unsigned int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const unsigned int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef _UNICODE
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const signed int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const signed int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const signed int val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef _UNICODE
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const unsigned short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const unsigned short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const unsigned short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef _UNICODE
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const signed short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const signed short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const signed short val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef _UNICODE
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const unsigned long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const unsigned long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const unsigned long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef _UNICODE
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const signed long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const signed long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const signed long val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef _UNICODE
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const unsigned __int64 val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const unsigned __int64 val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const unsigned __int64 val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef _UNICODE
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const signed __int64 val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const signed __int64 val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const signed __int64 val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef _UNICODE
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingWithPrefixLowerCase);

std::wstring ToHexStringW(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingWithPrefixLowerCase);

inline _tstring ToHexString(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingWithPrefixLowerCase)
{
#ifdef _UNICODE
   return ToHexStringW(val, hexDigitRepresentation);
#else
   return ToHexStringA(val, hexDigitRepresentation);
#endif
}

std::string ToHexStringA(
   const void *pData,
   const size_t length,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const void *pData,
   const size_t length,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const void *pData,
   const size_t length,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
{
#ifdef _UNICODE
   return ToHexStringW(pData, length, hexDigitRepresentation);
#else
   return ToHexStringA(pData, length, hexDigitRepresentation);
#endif
}

_tstring BoolAsString(
   const bool value);

std::string ToHexA(
   const BYTE val);

std::wstring ToHexW(
   const BYTE val);

inline _tstring ToHex(
   const BYTE val)
{
#ifdef _UNICODE
   return ToHexW(val);
#else
   return ToHexA(val);
#endif
}

std::string MakePrintableA(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength = 0,
   const bool useCR = false);

std::wstring MakePrintableW(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength = 0,
   const bool useCR = false);

inline _tstring MakePrintable(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength = 0,
   const bool useCR = false)
{
#ifdef _UNICODE
   return MakePrintableW(pData, dataLength, lineLength, useCR);
#else
   return MakePrintableA(pData, dataLength, lineLength, useCR);
#endif
}

std::string DumpDataA(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength = 0,
   const bool useCR = false);

std::wstring DumpDataW(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength = 0,
   const bool useCR = false);

inline _tstring DumpData(
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength = 0,
   const bool useCR = false)
{
#ifdef _UNICODE
   return DumpDataW(pData, dataLength, lineLength, useCR);
#else
   return DumpDataA(pData, dataLength, lineLength, useCR);
#endif
}

std::string DumpDataA(
   const std::string &linePrefix,
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength = 0,
   const bool useCR = false,
   const bool linePrefixOnFirstLine = true,
   const bool lineFeedOnLastLine = true);

std::wstring DumpDataW(
   const std::wstring &linePrefix,
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength = 0,
   const bool useCR = false,
   const bool linePrefixOnFirstLine = true,
   const bool lineFeedOnLastLine = true);

inline _tstring DumpData(
   const _tstring &linePrefix,
   const BYTE * const pData,
   const size_t dataLength,
   const size_t lineLength = 0,
   const bool useCR = false,
   const bool linePrefixOnFirstLine = true,
   const bool lineFeedOnLastLine = true)
{
#ifdef _UNICODE
   return DumpDataW(linePrefix, pData, dataLength, lineLength, useCR, linePrefixOnFirstLine, lineFeedOnLastLine);
#else
   return DumpDataA(linePrefix, pData, dataLength, lineLength, useCR, linePrefixOnFirstLine, lineFeedOnLastLine);
#endif
}

_tstring BoolAsString(
   const bool value);

std::string BoolAsStringA(
   const bool value);

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_TO_STRING__

///////////////////////////////////////////////////////////////////////////////
// End of file: ToString.h
///////////////////////////////////////////////////////////////////////////////

