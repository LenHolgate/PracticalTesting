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

#if (JETBYTE_USE_TEMPLATE_TO_STRING == 1)

#include <sstream>
#include <iomanip>

#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Functions defined in this file...
///////////////////////////////////////////////////////////////////////////////

enum ToHexStringHexDigitRepresentation
{
   HexDigitsLowerCase   = 0x00,
   HexDigitsUpperCase   = 0x01,
   HexDigitsNoPrefix    = 0x00,
   HexDigitsWithPrefix  = 0x10,

   HexDigitsNoPrefixUpperCase = HexDigitsUpperCase | HexDigitsNoPrefix,
   HexDigitsNoPrefixLowerCase = HexDigitsLowerCase | HexDigitsNoPrefix,

   HexDigitsWithPrefixUpperCase = HexDigitsUpperCase | HexDigitsWithPrefix,
   HexDigitsWithPrefixLowerCase = HexDigitsLowerCase | HexDigitsWithPrefix,

   HexDigitsDefault     = HexDigitsLowerCase | HexDigitsWithPrefix
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
// If you JETBYTE_USE_TEMPLATE_TO_STRING is defined then you will revert to the pre 
// version 6.0 behaviour and functionality using strstream.

#if (JETBYTE_USE_TEMPLATE_TO_STRING == 1)

/**
 * Converts a type to a _tstring.
 * Convert a type to a string by streaming it. Requires that there's an ostream
 * inserter available for type T.
 */

//_tstring ToString(__int64)
template <class T>
_tstring ToString(T num)
{
#ifdef _UNICODE
   return ToStringW(num);
#else 
   return ToStringA(num);
#endif
}

#pragma warning(push)
#pragma warning(disable: 4701)
#pragma warning(disable: 4267)   // warns of size_t to unsigned int truncation in win32 build with x64 warnings on
#pragma warning(disable: 4244)   // warns of ulong_ptr to unsigned long truncation in win32 build with x64 warnings on
template <class T>
std::string ToStringA(T num)
{
   std::ostringstream buf;

   buf << num;

   return buf.str();
}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4701)
#pragma warning(disable: 4267)   // warns of size_t to unsigned int truncation in win32 build with x64 warnings on
#pragma warning(disable: 4244)   // warns of ulong_ptr to unsigned long truncation in win32 build with x64 warnings on
template <class T>
std::wstring ToStringW(T num)
{
   // Note that we can't just use a wostringstream for the wide version
   // as that cannot format unsigned shorts correctly because in 
   // VC6 and optionally in later compilers wchar is unsigned short and so
   // it formats unsigned shorts as strings of characters...

   std::ostringstream buf;

   buf << num;

   return CStringConverter::AtoW(buf.str());
}
#pragma warning(pop)

template <class T>
_tstring ToString(T num, const size_t decimalPlaces)
{
#ifdef _UNICODE
   return ToStringW(num, decimalPlaces);
#else 
   return ToStringA(num, decimalPlaces);
#endif
}

template <class T>
std::string ToStringA(T num, const size_t decimalPlaces)
{
   std::ostringstream buf;

   buf << std::fixed << std::setprecision(decimalPlaces) << num;

   return buf.str();
}

template <class T>
std::wstring ToStringW(T num, const size_t decimalPlaces)
{
   // Note that we can't just use a wostringstream for the wide version
   // as that cannot format unsigned shorts correctly because in 
   // VC6 and optionally in later compilers wchar is unsigned short...

   std::ostringstream buf;

   buf << std::fixed << std::setprecision(decimalPlaces) << num;

   return CStringConverter::AtoW(buf.str());
}

_tstring HexToString(
   const BYTE *pBuffer, 
   const size_t iBytes);

std::string HexToStringA(
   const BYTE *pBuffer, 
   const size_t iBytes,
   bool upperCase = true);

std::wstring HexToStringW(
   const BYTE *pBuffer, 
   const size_t iBytes,
   bool upperCase = true);

template <class T>
_tstring ToHexString(T num)
{
#ifdef _UNICODE
   return ToHexStringW(num);
#else 
   return ToHexStringA(num);
#endif
}

template <class T>
std::string ToHexStringA(T num)
{
   // Note that we can't just use a wostringstream for the wide version
   // as that cannot format unsigned shorts correctly because in 
   // VC6 and optionally in later compilers wchar is unsigned short...

   std::ostringstream buf;

   buf.setf(std::ios::showbase);

   buf << std::hex << num;

   return buf.str();
}

template <class T>
std::wstring ToHexStringW(T num)
{
   // Note that we can't just use a wostringstream for the wide version
   // as that cannot format unsigned shorts correctly because in 
   // VC6 and optionally in later compilers wchar is unsigned short...

   std::ostringstream buf;

   buf.setf(std::ios::showbase);

   buf << std::hex << num;

   return CStringConverter::AtoW(buf.str());
}

template <class T>
_tstring HexToString(T num)
{
   return HexToString(reinterpret_cast<const BYTE*>(&num), sizeof(T));
}

#else

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
   const double val);

std::wstring ToStringW(
   const double val);

inline _tstring ToString(
   const double val)
{
#ifdef _UNICODE
   return ToStringW(val);
#else 
   return ToStringA(val);
#endif
}

std::string ToStringA(
   const long double val);

std::wstring ToStringW(
   const long double val);

inline _tstring ToString(
   const long double val)
{
#ifdef _UNICODE
   return ToStringW(val);
#else 
   return ToStringA(val);
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
   const void *val);

std::wstring PointerToStringW(
   const void *val);

inline _tstring PointerToString(
   const void *val)
{
#ifdef _UNICODE
   return PointerToStringW(val);
#else 
   return PointerToStringA(val);
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
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

std::wstring ToHexStringW(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault);

inline _tstring ToHexString(
   const void *val,
   const ToHexStringHexDigitRepresentation hexDigitRepresentation = HexDigitsDefault)
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

#endif // (JETBYTE_USE_TEMPLATE_TO_STRING == 1)

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

