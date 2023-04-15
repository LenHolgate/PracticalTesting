#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: CompareAndLog.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2007 JetByte Limited.
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

#include "JetByteTools/CoreTools/Types.h"
#include "JetByteTools/CoreTools/Exception.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

bool FileContentsMatch(
   const JetByteTools::Core::_tstring &fileName1,
   const JetByteTools::Core::_tstring &fileName2);

bool FileExistsAndIsEmpty(
   const JetByteTools::Core::_tstring &fileName);

bool FileExistsAndContains(
   const JetByteTools::Core::_tstring &fileName,
   const std::wstring &data);

bool FileExistsAndContains(
   const JetByteTools::Core::_tstring &fileName,
   const std::string &data);

bool DataAndFileContentsMatch(
   const BYTE *pData,
   DWORD dataLength,
   const JetByteTools::Core::_tstring &fileName);

void EnsureDataMatches(
   const BYTE *pData,
   const BYTE *pExpectedData,
   DWORD dataLength);

void EnsureStringsMatchA(
   const std::string &string1,
   const std::string &string2);

void EnsureStringsMatchW(
   const std::wstring &string1,
   const std::wstring &string2);

inline void EnsureStringsMatch(
   const JetByteTools::Core::_tstring &string1,
   const JetByteTools::Core::_tstring &string2)
{
#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
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

///////////////////////////////////////////////////////////////////////////////
// End of file: CompareAndLog.h
///////////////////////////////////////////////////////////////////////////////
