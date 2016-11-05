///////////////////////////////////////////////////////////////////////////////
// File: CompareAndLog.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "CompareAndLog.h"
#include "TestException.h"

#include "JetByteTools\Win32Tools\Win32Exception.h"
#include "JetByteTools\Win32Tools\Utils.h"
#include "JetByteTools\Win32Tools\StringConverter.h"
#include "JetByteTools\Win32Tools\SmartHandle.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::_tstring;
using JetByteTools::Win32::TExpandableBuffer;
using JetByteTools::Win32::LoadFileAsString;
using JetByteTools::Win32::LoadFileAsBinaryData;
using JetByteTools::Win32::SaveStringAsFile;
using JetByteTools::Win32::SaveBinaryDataAsFile;
using JetByteTools::Win32::DumpData;
using JetByteTools::Win32::CException;
using JetByteTools::Win32::CWin32Exception;
using JetByteTools::Win32::CStringConverter;
using JetByteTools::Win32::FileExists;
using JetByteTools::Win32::CSmartHandle;

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

   bool ok = false;

   ByteBuffer fileContents1;

   LoadFileAsBinaryData(fileName1, fileContents1);

   try
   {
      ByteBuffer fileContents2;

      LoadFileAsBinaryData(fileName2, fileContents2);

      ok = fileContents1.GetSize() == fileContents2.GetSize();

      if (ok)
      {

         ok = (0 == memcmp(fileContents1.GetBuffer(), fileContents2.GetBuffer(), fileContents1.GetSize()));
      }

      if (!ok)
      {
         SaveBinaryDataAsFile(fileName2 + _T(".Actual"), fileContents1);
      }

   }
   catch(const CException &/*e*/)
   {
      SaveBinaryDataAsFile(fileName2 + _T(".Actual"), fileContents1);

      throw;
   }

   return ok;
}

bool FileDoesNotExist(
   const _tstring &fileName)
{
   return !FileExists(fileName);
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

bool FileExistsAndIsReadLocked(
   const _tstring &fileName)
{
   bool ok = false;

   CSmartHandle hFile(::CreateFile(
      fileName.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      0,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      0));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = ::GetLastError();

      if (lastError == ERROR_SHARING_VIOLATION)
      {
         ok = true;
      }
      else
      {
         throw CWin32Exception(_T("FileExistsAndIsReadLocked() \"") + fileName + _T("\""), lastError);
      }
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
   const JetByteTools::Win32::_tstring &fileName)
{
   bool ok = false;

   const _tstring data = DumpData(pData, dataLength, 60, true);

   try
   {
      const _tstring fileContents = LoadFileAsString(fileName);

      ok = fileContents == data;

      if (!ok)
      {
         SaveStringAsFile(fileName + _T(".Actual.log"), data);
      }

   }
   catch(CException &/*e*/)
   {
      SaveStringAsFile(fileName + _T(".Actual.log"), data);

      throw;
   }

   return ok;
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

