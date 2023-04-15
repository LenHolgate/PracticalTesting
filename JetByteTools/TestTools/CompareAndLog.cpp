///////////////////////////////////////////////////////////////////////////////
// File: CompareAndLog.cpp
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

#include "JetByteTools/Admin/Admin.h"

#include "CompareAndLog.h"
#include "TestException.h"

#include "JetByteTools/CoreTools/FileUtils.h"
#include "JetByteTools/CoreTools/StringConverter.h"

#include <memory.h>  // for memcmp

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Core::_tstring;
using JetByteTools::Core::TExpandableBuffer;
using JetByteTools::Core::LoadFileAsString;
using JetByteTools::Core::LoadFileAsBinaryData;
using JetByteTools::Core::SaveStringAsFile;
using JetByteTools::Core::SaveBinaryDataAsFile;
using JetByteTools::Core::DumpData;
using JetByteTools::Core::CException;
using JetByteTools::Core::CStringConverter;

using std::string;
using std::wstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

bool FileContentsMatch(
   const _tstring &fileName1,
   const _tstring &fileName2)
{
   typedef TExpandableBuffer<BYTE> ByteBuffer;

   ByteBuffer fileContents1;

   LoadFileAsBinaryData(fileName1, fileContents1);

   try
   {
      ByteBuffer fileContents2;

      LoadFileAsBinaryData(fileName2, fileContents2);

      bool ok = fileContents1.GetSize() == fileContents2.GetSize();

      if (ok)
      {
         ok = (0 == memcmp(fileContents1.GetBuffer(), fileContents2.GetBuffer(), fileContents1.GetSize()));
      }

      if (!ok)
      {
         SaveBinaryDataAsFile(fileName2 + _T(".Actual"), fileContents1);
         SaveBinaryDataAsFile(fileName2 + _T(".Data.log"), fileContents2);
      }

      return ok;
   }
   catch (const CException &/*e*/)
   {
      SaveBinaryDataAsFile(fileName2 + _T(".Actual"), fileContents1);

      throw;
   }
}

bool FileExistsAndIsEmpty(
   const _tstring &fileName)
{
   const _tstring contents = LoadFileAsString(fileName);

   const bool ok = (contents.length() == 0);

   if (!ok)
   {
      SaveStringAsFile(fileName + _T(".Actual.log"), contents);
   }

   return ok;
}


bool FileExistsAndContains(
   const _tstring &fileName,
   const wstring &data)
{
   const _tstring contents = LoadFileAsString(fileName);

   const bool ok = (data == CStringConverter::TtoW(contents));

   if (!ok)
   {
      SaveStringAsFile(fileName + _T(".Actual.log"), contents);
      SaveStringAsFile(fileName + _T(".Data.log"), CStringConverter::WtoT(data));
   }

   return ok;
}

bool FileExistsAndContains(
   const _tstring &fileName,
   const string &data)
{
   const _tstring contents = LoadFileAsString(fileName);

   const bool ok = (CStringConverter::AtoT(data) == contents);

   if (!ok)
   {
      SaveStringAsFile(fileName + _T(".Actual.log"), contents);
      SaveStringAsFile(fileName + _T(".Data.log"), CStringConverter::AtoT(data));
   }

   return ok;
}

bool DataAndFileContentsMatch(
   const BYTE * const pData,
   const DWORD dataLength,
   const _tstring &fileName)
{
   const _tstring data = DumpData(pData, dataLength, 60, true);

   try
   {
      const _tstring fileContents = LoadFileAsString(fileName);

      const bool ok = (fileContents == data);

      if (!ok)
      {
         SaveStringAsFile(fileName + _T(".Actual.log"), data);
      }

      return ok;
   }
   catch (CException &/*e*/)
   {
      SaveStringAsFile(fileName + _T(".Actual.log"), data);

      throw;
   }
}

void EnsureDataMatches(
  const BYTE * const pData,
  const BYTE * const pExpectedData,
  const DWORD dataLength)
{
   if (0 != memcmp(pData, pExpectedData, dataLength))
   {
      throw CTestException(_T("Data does not match. Got: ") + DumpData(pData, dataLength) +
         _T(" Expected: ") + DumpData(pExpectedData, dataLength));
   }
}

void EnsureStringsMatchA(
   const string &string1,
   const string &string2)
{
   if (string1 != string2)
   {
      throw CTestException(_T("Strings do not match. Got: \"") + CStringConverter::AtoT(string1) +
         _T("\" Expected: \"") + CStringConverter::AtoT(string2) + _T("\""));
   }
}

void EnsureStringsMatchW(
   const wstring &string1,
   const wstring &string2)
{
   if (string1 != string2)
   {
      throw CTestException(_T("Strings do not match. Got: \"") + CStringConverter::WtoT(string1) +
         _T("\" Expected: \"") + CStringConverter::WtoT(string2) + _T("\""));
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CompareAndLog.cpp
///////////////////////////////////////////////////////////////////////////////
