#pragma once
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

#include "JetByteTools/Admin/Types.h"

#include "tstring.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Functions defined in this file...
///////////////////////////////////////////////////////////////////////////////

unsigned short CalculateRequiredPrecision(
   double value);

enum ToHexStringHexDigitRepresentation
{
   HexDigitsLowerCase      = 0x0000,
   HexDigitsUpperCase      = 0x0001,
   HexDigitsNoPrefix       = 0x0000,
   HexDigitsWithPrefix     = 0x0010,      // Prefix with 0x
   HexDigitsNoPadding      = 0x0000,
   HexDigitsWithPadding    = 0x0100,      // Pad to full width for type with leading 0's
   HexDigitsSpacesBetween  = 0x1000,      // Only for blocks of 'void *' data,
                                          // space between each digit eg "00 AF FF" rather than "00AFFF"

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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
   return ToStringW(val);
#else
   return ToStringA(val);
#endif
}

std::string ToStringA(
   unsigned __int64 val);

std::wstring ToStringW(
   unsigned __int64 val);

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
   signed __int64 val);

std::wstring ToStringW(
   signed __int64 val);

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
   float val);

std::wstring ToStringW(
   float val);

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
   double val,
   unsigned short precision = 0);

std::wstring ToStringW(
   double val,
   unsigned short precision = 0);

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
   long double val,
   unsigned short precision = 0);

std::wstring ToStringW(
   long double val,
   unsigned short precision = 0);

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
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingNoPrefixUpperCase);

std::wstring PointerToStringW(
   const void *val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingNoPrefixUpperCase);

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
   unsigned char val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   unsigned char val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

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
   signed char val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   signed char val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

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
   unsigned int val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   unsigned int val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

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
   signed int val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   signed int val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

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
   unsigned short val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   unsigned short val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

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
   signed short val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   signed short val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

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
   unsigned long val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   unsigned long val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

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
   signed long val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   signed long val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

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
   unsigned __int64 val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   unsigned __int64 val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

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
   signed __int64 val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   signed __int64 val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

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
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingWithPrefixLowerCase);

std::wstring ToHexStringW(
   const void *val,
   ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsWithPaddingWithPrefixLowerCase);

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
#ifdef _UNICODE
   return ToHexStringW(pData, length, hexDigitRepresentation);
#else
   return ToHexStringA(pData, length, hexDigitRepresentation);
#endif
}

std::string ToHexA(
   BYTE val);

std::wstring ToHexW(
   BYTE val);

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
#ifdef _UNICODE
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
#ifdef _UNICODE
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
#ifdef _UNICODE
   return DumpDataW(linePrefix, pData, dataLength, lineLength, useCR, linePrefixOnFirstLine, lineFeedOnLastLine);
#else
   return DumpDataA(linePrefix, pData, dataLength, lineLength, useCR, linePrefixOnFirstLine, lineFeedOnLastLine);
#endif
}

_tstring BoolAsString(
   bool value);

std::string BoolAsStringA(
   bool value);

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_TO_STRING__

///////////////////////////////////////////////////////////////////////////////
// End of file: ToString.h
///////////////////////////////////////////////////////////////////////////////

