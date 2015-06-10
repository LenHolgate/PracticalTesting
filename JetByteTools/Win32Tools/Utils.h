#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_UTILS__
#define JETBYTE_TOOLS_WIN32_UTILS__
///////////////////////////////////////////////////////////////////////////////
// File: Utils.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 1997 JetByte Limited.
//
// JetByte Limited grants you ("Licensee") a non-exclusive, royalty free, 
// licence to use, modify and redistribute this software in source and binary 
// code form, provided that i) this copyright notice and licence appear on all 
// copies of the software; and ii) Licensee does not utilize the software in a 
// manner which is disparaging to JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All 
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors 
// shall not be liable for any damages suffered by licensee as a result of 
// using, modifying or distributing the software or its derivatives. In no
// event will JetByte Limited be liable for any lost revenue, profit or data,
// or for direct, indirect, special, consequential, incidental or punitive
// damages, however caused and regardless of the theory of liability, arising 
// out of the use of or inability to use software, even if JetByte Limited 
// has been advised of the possibility of such damages.
//
// This software is not designed or intended for use in on-line control of 
// aircraft, air traffic, aircraft navigation or aircraft communications; or in 
// the design, construction, operation or maintenance of any nuclear 
// facility. Licensee represents and warrants that it will not use or 
// redistribute the Software for such purposes. 
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -save
//lint -e537 repeated include files (atlbase.h)
//
///////////////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4201)   // nameless struct/union

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#include "tstring.h"
#include "StringConverter.h"

#include <strstream>

#pragma warning(default: 4201)

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Debugging defines...
///////////////////////////////////////////////////////////////////////////////

#ifndef DEBUG_ONLY
#ifdef _DEBUG
#define DEBUG_ONLY(x)   x
#else
#define DEBUG_ONLY(x)
#endif
#endif

///////////////////////////////////////////////////////////////////////////////
// Functions defined in this file...
///////////////////////////////////////////////////////////////////////////////


/**
 * Converts a type to a _tstring.
 * Convert a type to a string by streaming it. Requires that there's an ostream
 * inserter available for type T.
 */

template <class T>
_tstring ToString(T num)
{
	_tstring strNum = _T("");

   {
      std::strstream buf;

	   buf << num << std::ends;

#ifdef _UNICODE
      std::string temp = buf.str();

      strNum = CStringConverter::AtoW(temp);
#else 
	   strNum = buf.str();
#endif
	   buf.freeze(false);
   }

   return strNum;

   //lint -e{818} parameter num could be declared as a pointer to const
}

template <class T>
std::string ToStringA(T num)
{
   std::strstream buf;

	buf << num << std::ends;

   std::string strNum = buf.str();

   buf.freeze(false);

   return strNum;

   //lint -e{818} parameter num could be declared as a pointer to const
}

template <class T>
std::wstring ToStringW(T num)
{
   std::strstream buf;

	buf << num << std::ends;

   std::string temp = buf.str();

   std::wstring strNum = CStringConverter::AtoW(temp);

   buf.freeze(false);

   return strNum;

   //lint -e{818} parameter num could be declared as a pointer to const
}

template <class T>
_tstring ToHexString(T num)
{
   return HexToString((const BYTE *)&num, sizeof(T));
}

template <class T>
bool ToBool(const T &value)
{
   return (0 != value);
}

_tstring HexToString(
   const BYTE *pBuffer, 
   size_t iBytes);

std::string HexToStringA(
   const BYTE *pBuffer, 
   size_t iBytes,
   bool upperCase = true);

void StringToHex(
   const _tstring &str, 
   BYTE *pBuffer, 
   size_t nBytes);

_tstring GetLastErrorMessage(
   DWORD last_error);

std::string GetLastErrorMessageA(
   DWORD last_error);

_tstring GetCurrentDirectory();

_tstring GetDateStamp();

void SetLogFileName(
   const _tstring &name);

void Output(
   const std::string &message);

void Output(
   const std::wstring &message);

void OutputEx(
   const std::string &message);

void OutputEx(
   const std::wstring &message);

_tstring ToHex(BYTE c);

std::string ToHexA(BYTE c);

_tstring MakePrintable(
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength = 0);

_tstring DumpData(
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength = 0);

std::string DumpDataA(
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength = 0);

_tstring DumpData(
   const _tstring &linePrefix,
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength = 0);

std::string DumpDataA(
   const std::string &linePrefix,
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength = 0);

_tstring GetComputerName();

_tstring GetModuleFileName(
   HINSTANCE hModule = 0);

_tstring GetUserName();

_tstring GetFileVersion();

_tstring StripLeading(
   const _tstring &source, 
   const char toStrip);

_tstring StripTrailing(
   const _tstring &source, 
   const char toStrip);

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -restore
//
///////////////////////////////////////////////////////////////////////////////

#endif // JETBYTE_TOOLS_WIN32_UTILS__

///////////////////////////////////////////////////////////////////////////////
// End of file: SystemInfo.h
///////////////////////////////////////////////////////////////////////////////

