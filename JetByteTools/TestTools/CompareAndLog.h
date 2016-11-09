#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_TEST_COMPARE_AND_LOG_INCLUDED__
#define JETBYTE_TOOLS_TEST_COMPARE_AND_LOG_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CompareAndLog.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2007 JetByte Limited.
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

#include "JetByteTools\Win32Tools\Exception.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

bool FileContentsMatch(
   const JetByteTools::Win32::_tstring &fileName1,
   const JetByteTools::Win32::_tstring &fileName2);

bool FileExists(
   const JetByteTools::Win32::_tstring &fileName);

bool FileDoesNotExist(
   const JetByteTools::Win32::_tstring &fileName);

bool FileExistsAndIsReadLocked(
   const JetByteTools::Win32::_tstring &fileName);

bool FileExistsAndIsEmpty(
   const JetByteTools::Win32::_tstring &fileName);

bool FileExistsAndContains(
   const JetByteTools::Win32::_tstring &fileName,
   const std::wstring &data);

bool FileExistsAndContains(
   const JetByteTools::Win32::_tstring &fileName,
   const std::string &data);

bool DataAndFileContentsMatch(
   const BYTE * const pData,
   const DWORD dataLength,
   const JetByteTools::Win32::_tstring &fileName);

void EnsureDataMatches(
   const BYTE * const pData,
   const BYTE * const pExpectedData,
   const DWORD dataLength);

void EnsureStringsMatchA(
   const std::string &string1,
   const std::string &string2);

void EnsureStringsMatchW(
   const std::wstring &string1,
   const std::wstring &string2);

inline void EnsureStringsMatch(
   const JetByteTools::Win32::_tstring &string1,
   const JetByteTools::Win32::_tstring &string2)
{
#ifdef _UNICODE
   EnsureStringsMatchW(string1, string2);
#else
   EnsureStringsMatchA(string1, string2);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_TEST_COMPARE_AND_LOG_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CompareAndLog.h
///////////////////////////////////////////////////////////////////////////////
