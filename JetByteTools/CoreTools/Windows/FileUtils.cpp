///////////////////////////////////////////////////////////////////////////////
// File: FileUtils.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2018 JetByte Limited.
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

#include "FileUtils.h"

#include "JetByteTools/Win32Tools/SmartHandle.h"

#include "JetByteTools/CoreTools/ErrorCodeException.h"
#include "JetByteTools/CoreTools/ToString.h"
#include "JetByteTools/CoreTools/ToBool.h"
#include "JetByteTools/CoreTools/StringConverter.h"
#include "JetByteTools/CoreTools/StringUtils.h"

#pragma hdrstop

#include <Shlwapi.h>
#include <Psapi.h>

#include <algorithm>
#include <tchar.h>
#include <list>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "Shlwapi.lib")

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::CSmartHandle;

using std::string;
using std::wstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static bool StringIsAllANSI(
   const std::wstring &data);

///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////

void WriteResourceToFile(
   HANDLE hFile,
   const _tstring &resourceName,
   const _tstring &resourceType,
   const HMODULE hModule)
{
   auto *const hResource = ::FindResource(hModule, resourceName.c_str(), resourceType.c_str());

   if (!hResource)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("WriteResourceToFile() - FindResource"), lastError);
   }

   const HGLOBAL hGlobal = LoadResource(hModule, hResource);

   if (!hGlobal)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("WriteResourceToFile() - LoadResource"), lastError);
   }

   void *pData = LockResource(hGlobal);

   if (!pData)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("WriteResourceToFile() - LockResource"), lastError);
   }

   const DWORD bytes = SizeofResource(hModule, hResource);

   DWORD bytesWritten = 0;

   if (!WriteFile(hFile, pData, bytes, &bytesWritten, nullptr))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("WriteResourceToFile() - WriteFile"), lastError);
   }

   if (bytesWritten != bytes)
   {
      throw CException(_T("WriteResourceToFile() - WriteFile"), _T("Wrote less bytes (") + ToString(bytesWritten) + _T("( than expected: ") + ToString(bytes));
   }
}

void CreateDirectory(
   const _tstring &directory)
{
   if (!::CreateDirectory(directory.c_str(), nullptr))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("CreateDirectory()"), lastError);
   }
}

bool CreateDirectoryIfRequired(
   const _tstring &directory)
{
   bool created = false;

   if (!::CreateDirectory(directory.c_str(), nullptr))
   {
      created = true;

      const DWORD lastError = GetLastError();

      if (lastError != ERROR_ALREADY_EXISTS)
      {
         throw CErrorCodeException(_T("CreateDirectoryIfRequired()"), lastError);
      }
   }

   return created;
}

size_t CreateDirectoriesIfRequired(
   const _tstring &directory)
{
   size_t numCreated = 0;

   if (!::CreateDirectory(directory.c_str(), nullptr))
   {
      const DWORD lastError = GetLastError();

      if (lastError == ERROR_PATH_NOT_FOUND)
      {
         const _tstring::size_type pos = directory.find_last_of(_T('\\'));

         if (pos != _tstring::npos)
         {
            const _tstring previousDirectory = directory.substr(0, pos);

            numCreated += CreateDirectoriesIfRequired(previousDirectory);

            // We use 'if required' here because we might get passed a path with a
            // trailing \ which means we will try and create "lastDirectory" and then
            // "lastDirectory\"...

            if (CreateDirectoryIfRequired(directory))
            {
               numCreated++;
            }
         }
         else
         {
            throw CErrorCodeException(_T("CreateDirectoriesIfRequired()"), lastError);
         }
      }
      else if (lastError != ERROR_ALREADY_EXISTS)
      {
         throw CErrorCodeException(_T("CreateDirectoriesIfRequired()"), lastError);
      }
   }

   return numCreated;
}

_tstring GetCurrentDirectory()
{
   const DWORD size = ::GetCurrentDirectory(0, nullptr);

   _tstring result;

   result.resize(size);

   if (0 == ::GetCurrentDirectory(size, const_cast<TCHAR *>(result.c_str())))
   {
      throw CException(_T("GetCurrentDirectory()"), _T("Failed to get current directory"));
   }

   result.resize(size - 1);

   return result;
}

void SetCurrentDirectory(
   const _tstring &directory)
{
   if (!::SetCurrentDirectory(directory.c_str()))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SetCurrentDirectory()"), lastError);
   }
}

bool DirectoryExists(
   const _tstring &directory)
{
   const _tstring currentDirectory = GetCurrentDirectory();

   if (!::SetCurrentDirectory(directory.c_str()))
   {
      const DWORD lastError = GetLastError();

      if (lastError == ERROR_FILE_NOT_FOUND ||
          lastError == ERROR_PATH_NOT_FOUND ||
          lastError == ERROR_DIRECTORY)
      {
         return false;
      }

      throw CErrorCodeException(
         _T("DirectoryExists()"),
         _T("\"") + directory + _T("\""),
         lastError);
   }

   SetCurrentDirectory(currentDirectory);

   return true;
}

_tstring EnsurePathEndsWithSingleBackslash(
   const _tstring &path)
{
   _tstring result = path;

   _tstring::size_type pos = result.find_last_of(_T('\\'));

   const _tstring::size_type lastPos = result.length() - 1;

   if (pos == lastPos)
   {
      // strip all but the last trailing backslash

      while (pos - 1 != 0 &&
             result.c_str()[pos -1] == '\\')
      {
         pos--;
      }

      if (pos != lastPos)
      {
         result = result.substr(0, pos + 1);
      }
   }
   else
   {
      result += _T("\\");
   }

   return result;
}

_tstring CombinePath(
   const _tstring &path1,
   const _tstring &path2)
{
   _tstring combinedPath;

   if (!TryCombinePath(combinedPath, path1, path2))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("CombinePath() failed for \"") + path1 + _T("\", \"") + path2 + _T("\""), lastError);
   }

   return combinedPath;
}

bool TryCombinePath(
   _tstring &combinedPath,
   const _tstring &path1,
   const _tstring &path2)
{
   bool ok = false;

   TCHAR outBuffer[MAX_PATH];

   if (nullptr != ::PathCombine(outBuffer, path1.c_str(), path2.c_str()))
   {
      combinedPath = outBuffer;

      ok = true;
   }

   return ok;
}

_tstring BuildPath(
   const _tstring &path1,
   const _tstring &path2)
{
   return path1 + _T("\\") + path2;
}

bool PathHasRelativeRoot(
   const _tstring &path)
{
   // could probably just look for a single "."

   _tstring::size_type pos = path.find(_T(".\\"));

   if (pos == 0)
   {
      return true;
   }

   pos = path.find(_T("..\\"));

   if (pos == 0)
   {
      return true;
   }

   pos = path.find(_T("./"));

   if (pos == 0)
   {
      return true;
   }

   pos = path.find(_T("../"));

   if (pos == 0)
   {
      return true;
   }

   return false;
}

_tstring ResolveRelativePath(
   const _tstring &path)
{
   return ResolveRelativePath(path, GetCurrentDirectory());
}

_tstring ResolveRelativePath(
   const _tstring &path,
   const _tstring &root)
{
   if (PathHasRelativeRoot(path))
   {
      return BuildPath(root, path);
   }

   return path;
}

_tstring MakePathAbsolute(
   const _tstring &path)
{
   return MakePathAbsolute(path, GetCurrentDirectory());
}

_tstring MakePathAbsolute(
   const _tstring &path,
   const _tstring &root)
{
   // If the path starts with a relative path indicator, .\ ..\ etc
   // then build the path so that it refers to the supplied root...

   _tstring absolutePath(ResolveRelativePath(path, root));

   // now combine the path with the current directory to process any relative path
   // constructs within the path and create a "clean" absolute path.

   if (!TryCombinePath(absolutePath, absolutePath, _T(".")))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("MakePathAbsolute() failed for \"") + path + _T("\""), lastError);
   }

   return absolutePath;
}

_tstring GetTempFileName(
   const _tstring &pathName,
   const _tstring &prefixString,
   const unsigned int unique)
{
   TCHAR buffer[MAX_PATH + 1];

   if (0 == ::GetTempFileName(pathName.c_str(), prefixString.c_str(), unique, buffer))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("GetTempFileName()"), lastError);
   }

   return buffer;
}

_tstring GetTempFileName(
   const _tstring &prefixString,
   const unsigned int unique)
{
   return GetTempFileName(GetTempPath(), prefixString, unique);
}

_tstring GetTempPath()
{
   const DWORD spaceRequired = ::GetTempPath(0, nullptr);

   if (spaceRequired == 0)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("GetTempPath()"), lastError);
   }

   _tstring directory;

   directory.resize(spaceRequired - 1);

   const DWORD spaceUsed = ::GetTempPath(spaceRequired, const_cast<TCHAR*>(directory.c_str()));

   if (spaceUsed == 0)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("GetTempPath()"), lastError);
   }

   if (spaceUsed >= spaceRequired)
   {
      throw CException(
         _T("GetTempPath()"),
         _T("Failed to get temp path, second call needed more space ") + ToString(spaceUsed + 1) +
         _T(" than first call allocated ") + ToString(spaceRequired));
   }

   directory.resize(spaceUsed);

   return directory;
}

_tstring GetFileExtension(
   const _tstring &filename)
{
   const _tstring::size_type pos = filename.find_last_of(_T('.'));

   if (pos != _tstring::npos)
   {
      return filename.substr(pos);
   }

   return _T("");
}


_tstring StripFileExtension(
   const _tstring &filename)
{
   const _tstring::size_type pos = filename.find_last_of(_T('.'));

   return filename.substr(0, pos);
}

string StripFileExtensionA(
   const string &filename)
{
   const string::size_type pos = filename.find_last_of('.');

   return filename.substr(0, pos);
}

_tstring GetFileNameFromPathName(
   const _tstring &pathName)
{
   return GetFileNameFromPathName(pathName, _T("\\/"));
}

_tstring GetFileNameFromPathName(
   const _tstring &pathName,
   const _tstring &separators)
{
   _tstring fileName;

   const _tstring::size_type pos = pathName.find_last_of(separators);

   if (pos != _tstring::npos)
   {
      fileName = pathName.substr(pos + 1);
   }

   return fileName;
}

string GetFileNameFromPathNameA(
   const string &pathName)
{
   return GetFileNameFromPathNameA(pathName, "\\/");
}

string GetFileNameFromPathNameA(
   const string &pathName,
   const string &separators)
{
   string fileName;

   const string::size_type pos = pathName.find_last_of(separators);

   if (pos != string::npos)
   {
      fileName = pathName.substr(pos + 1);
   }

   return fileName;
}

_tstring StripFileNameFromPathName(
   const _tstring &pathName)
{
   return StripFileNameFromPathName(pathName, _T("\\/"));
}

_tstring StripFileNameFromPathName(
   const _tstring &pathName,
   const _tstring &separators)
{
   _tstring strippedPathName;

   const _tstring::size_type pos = pathName.find_last_of(separators);

   if (pos != _tstring::npos)
   {
      strippedPathName = pathName.substr(0, pos);
   }

   return strippedPathName;
}

string StripFileNameFromPathNameA(
   const string &pathName)
{
   return StripFileNameFromPathNameA(pathName, "\\/");
}
string StripFileNameFromPathNameA(
   const string &pathName,
   const string &separators)
{
   string strippedPathName;

   const string::size_type pos = pathName.find_last_of(separators);

   if (pos != string::npos)
   {
      strippedPathName = pathName.substr(0, pos);
   }

   return strippedPathName;
}

void DeleteFile(
   const _tstring &fileName)
{
   if (!::DeleteFile(fileName.c_str()))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("DeleteFile() - \"") + fileName + _T("\""), lastError);
   }
}

void DeleteFileIfExists(
   const _tstring &fileName)
{
   if (!::DeleteFile(fileName.c_str()))
   {
      const DWORD lastError = GetLastError();

      if (lastError != ERROR_FILE_NOT_FOUND)
      {
         throw CErrorCodeException(_T("DeleteFile() - \"") + fileName + _T("\""), lastError);
      }
   }
}

void MoveFile(
   const _tstring &filenameFrom,
   const _tstring &filenameTo)
{
   if (!::MoveFile(filenameFrom.c_str(), filenameTo.c_str()))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("MoveFile"), lastError);
   }
}

void CopyFile(
   const _tstring &filenameFrom,
   const _tstring &filenameTo,
   const bool failIfExists)
{
   if (!::CopyFile(filenameFrom.c_str(), filenameTo.c_str(), failIfExists))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("CopyFile"), lastError);
   }
}

void SaveStringAsFile(
   const _tstring &filename,
   const _tstring &data)
{
   SaveStringAsFileA(filename, CStringConverter::TtoA(data));
}

void SaveStringAsFileA(
   const _tstring &filename,
   const string &data)
{
   const CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_WRITE,
      0,
      nullptr,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      nullptr));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SaveStringAsFile() \"") + filename + _T("\""), lastError);
   }

   DWORD bytesWritten = 0;

   if (!WriteFile(hFile, data.c_str(), GetStringLength<DWORD>(data), &bytesWritten, nullptr))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SaveStringAsFile()"), _T("\"") + filename + _T("\""), lastError);
   }

   if (bytesWritten != data.length())
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SaveStringAsFile()"), _T("\"") + filename + _T("\" - Failed to write all data"), lastError);
   }
}

void SaveStringToFile(
   const _tstring &filename,
   HANDLE hFile,
   const _tstring &data)
{
   SaveStringToFileA(filename, hFile, CStringConverter::TtoA(data));
}

void SaveStringToFileA(
   const _tstring &filename,
   HANDLE hFile,
   const string &data)
{
   DWORD bytesWritten = 0;

   if (!WriteFile(hFile, data.c_str(), GetStringLength<DWORD>(data), &bytesWritten, nullptr))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SaveStringToFile()"), _T("\"") + filename + _T("\""), lastError);
   }

   if (bytesWritten != data.length())
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SaveStringToFile()"), _T("\"") + filename + _T("\" - Failed to write all data"), lastError);
   }
}

__int64 GetFileSize(
   const _tstring &filename)
{
   const CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      nullptr,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      nullptr));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("GetFileSize() \"") + filename + _T("\""), lastError);
   }

   BY_HANDLE_FILE_INFORMATION info;

   if (!GetFileInformationByHandle(hFile, &info))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("GetFileSize() - GetFileInformationByHandle \"") + filename + _T("\""), lastError);
   }

   ULARGE_INTEGER large;

   large.HighPart = info.nFileSizeHigh;
   large.LowPart = info.nFileSizeLow;

   return large.QuadPart;
}

wstring LoadFileAsUnicodeString(
   const _tstring &filename,
   bool *pFileWasUnicode)
{
   // Look for the Unicode stream indicator in the file for (UCS-2) format and these files start 0xFF 0xFE to indicate
   // unicode little-endian 0xFE 0xFF indicates unicode big-endian but we don't support that right now. Anything else
   // means ASCII, UTF-8 or MBCS and we assume ASCII here

   const CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      nullptr,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      nullptr));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("LoadFileAsUnicodeString() \"") + filename + _T("\""), lastError);
   }

   BY_HANDLE_FILE_INFORMATION info;

   if (!GetFileInformationByHandle(hFile, &info))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("LoadFileAsUnicodeString() - GetFileInformationByHandle \"") + filename + _T("\""), lastError);
   }

   DWORD bytesRead = 0;

   DWORD totalBytesRead = 0;

   BYTE buffer[2];

   while (ReadFile(hFile, buffer + totalBytesRead, sizeof(buffer) - totalBytesRead, &bytesRead, nullptr) && bytesRead > 0)
   {
      totalBytesRead += bytesRead;
   }

   if (totalBytesRead != 2)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("LoadFileAsUnicodeString() - Initial 2 byte read failed \"") + filename + _T("\""), lastError);
   }

   bool fileIsUnicode = false;
   bool fileIsUTF8 = false;

   DWORD sizeOfBOM = 0;

   if (buffer[0] == 0xFF && buffer[1] == 0xFE)
   {
      sizeOfBOM = 2;
      fileIsUnicode = true;
   }
   else if (buffer[0] == 0xFE && buffer[1] == 0xFF)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("LoadFileAsUnicodeString() - We don't handle UCS-2 big-endian yet. \"") + filename + _T("\""), lastError);
   }
   else if (buffer[0] == 0xEF && buffer[1] == 0xBB)
   {
      // UTF-8 ?
      // EF BB BF

      sizeOfBOM = 3;
      fileIsUnicode = true;
      fileIsUTF8 = true;

      if (0 == ReadFile(hFile, buffer, 1, &bytesRead, nullptr))
      {
         const DWORD lastError = GetLastError();

         throw CErrorCodeException(_T("LoadFileAsUnicodeString() - Failed to read 3rd byte in UTF-8 BOM. \"") + filename + _T("\""), lastError);
      }

      if (buffer[0] != 0xBF)
      {
         throw CException(_T("LoadFileAsUnicodeString()"), _T("Unexpected 3rd byte in UTF-8 BOM. \"") + filename + _T("\""));
      }
   }
   else
   {
      // bare ascii file, we need those 2 bytes...

      if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, nullptr, FILE_BEGIN))
      {
         const DWORD lastError = GetLastError();

         throw CErrorCodeException(_T("LoadFileAsUnicodeString() - Failed to seek to start of file. \"") + filename + _T("\""), lastError);
      }
   }

   bytesRead = 0;

   totalBytesRead = 0;

   string fileAsString;

   if (info.nFileSizeHigh != 0 || info.nFileSizeLow > fileAsString.max_size())
   {
      throw CException(_T("LoadFileAsUnicodeString()"), _T("File \"") + filename + _T("\" too big"));
   }

   const DWORD bufferSize = info.nFileSizeLow - sizeOfBOM;

   fileAsString.resize(bufferSize);

   while (ReadFile(hFile, reinterpret_cast<void*>(const_cast<char *>(fileAsString.c_str()) + totalBytesRead), bufferSize - totalBytesRead, &bytesRead, nullptr) && bytesRead > 0)
   {
      totalBytesRead += bytesRead;
   }

   if (pFileWasUnicode)
   {
      *pFileWasUnicode = fileIsUnicode;
   }

   if (fileIsUnicode)
   {
      if (fileIsUTF8)
      {
         return CStringConverter::UTF8toW(fileAsString);
      }

      return wstring(reinterpret_cast<const wchar_t*>(fileAsString.c_str()), bufferSize / sizeof(wchar_t));
   }

   return CStringConverter::AtoW(fileAsString);
}

void SaveUnicodeStringAsFile(
   const _tstring &filename,
   const std::wstring &data,
   const bool saveAsANSIifPossible)
{
   if (saveAsANSIifPossible && StringIsAllANSI(data))
   {
      SaveStringAsFile(filename, CStringConverter::WtoT(data));
   }

   const CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_WRITE,
      0,
      nullptr,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      nullptr));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SaveUnicodeStringAsFile() \"") + filename + _T("\""), lastError);
   }

   DWORD bytesWritten = 0;

   const BYTE header[] = { 0xFF, 0xFE };

   if (!WriteFile(hFile, header, sizeof(header), &bytesWritten, nullptr))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SaveUnicodeStringAsFile() \"") + filename + _T("\""), lastError);
   }

   const DWORD dataToWrite = GetStringLength<DWORD>(data) * sizeof(wchar_t);

   if (!WriteFile(hFile, data.c_str(), dataToWrite, &bytesWritten, nullptr))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SaveUnicodeStringAsFile() \"") + filename + _T("\""), lastError);
   }

   if (bytesWritten != dataToWrite)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SaveUnicodeStringAsFile() \"") + filename + _T("\" - Failed to write all data"), lastError);
   }
}

void LoadFileAsBinaryData(
   const _tstring &filename,
   TExpandableBuffer<BYTE> &buffer)
{
   const CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
      nullptr,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      nullptr));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("LoadFileAsBinaryData() \"") + filename + _T("\""), lastError);
   }

   BY_HANDLE_FILE_INFORMATION info;

   if (!GetFileInformationByHandle(hFile, &info))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("LoadFileAsBinaryData() - GetFileInformationByHandle \"") + filename + _T("\""), lastError);
   }

   LARGE_INTEGER fileSize;

   fileSize.LowPart = info.nFileSizeLow;
   fileSize.HighPart = info.nFileSizeHigh;

   const auto bufferSize = static_cast<size_t>(fileSize.QuadPart);

   buffer.Resize(bufferSize);

   DWORD bytesRead = 0;

   size_t totalBytesRead = 0;

   while (ReadFile(
      hFile,
      static_cast<void*>(buffer.GetBuffer() + totalBytesRead),
      static_cast<DWORD>(std::min<size_t>(bufferSize - totalBytesRead, std::numeric_limits<DWORD>::max())),
      &bytesRead, nullptr) && bytesRead > 0)
   {
      totalBytesRead += bytesRead;
   }
}

void SaveBinaryDataAsFile(
   const _tstring &filename,
   const TExpandableBuffer<BYTE> &buffer)
{
   const CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_WRITE,
      0,
      nullptr,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      nullptr));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SaveBinaryDataAsFile() \"") + filename + _T("\""), lastError);
   }

   DWORD bytesWritten = 0;

   // Does not work with big files, change to use multiple writes

   if (!WriteFile(hFile, buffer.GetBuffer(), static_cast<DWORD>(buffer.GetSize()), &bytesWritten, nullptr))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SaveBinaryDataAsFile() \"") + filename + _T("\""), lastError);
   }

   if (bytesWritten != buffer.GetSize())
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("SaveBinaryDataAsFile() \"") + filename + _T("\" - Failed to write all data"), lastError);
   }
}

_tstring LoadFileAsString(
   const _tstring &filename)
{
   return CStringConverter::AtoT(LoadFileAsStringA(filename));
}

string LoadFileAsStringA(
   const _tstring &filename)
{
   const CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      nullptr,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      nullptr));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("LoadFileAsString() \"") + filename + _T("\""), lastError);
   }

   return LoadFileAsStringA(hFile, filename);
}

_tstring LoadFileAsString(
   HANDLE hFile,
   const _tstring &filename)
{
   return CStringConverter::AtoT(LoadFileAsStringA(hFile, filename));
}

string LoadFileAsStringA(
   HANDLE hFile,
   const _tstring &filename)
{
   BY_HANDLE_FILE_INFORMATION info;

   if (!GetFileInformationByHandle(hFile, &info))
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("LoadFileAsString() - GetFileInformationByHandle \"") + filename + _T("\""), lastError);
   }

   string fileAsString;

   if (info.nFileSizeHigh != 0 || info.nFileSizeLow > fileAsString.max_size())
   {
      throw CException(_T("LoadFileAsString()"), _T("File \"") + filename + _T("\" too big"));
   }

   const DWORD currentPosition = SetFilePointer(hFile, 0, nullptr, FILE_CURRENT);

   if (currentPosition == INVALID_SET_FILE_POINTER)
   {
      const DWORD lastError = GetLastError();

      throw CErrorCodeException(_T("LoadFileAsString()"), _T("Failed to read file position for file \"") + filename + _T("\""), lastError);
   }

   const DWORD bufferSize = info.nFileSizeLow - currentPosition;

   fileAsString.resize(bufferSize);

   DWORD bytesRead = 0;

   DWORD totalBytesRead = 0;

   while (ReadFile(
      hFile,
      reinterpret_cast<void*>(const_cast<char *>(fileAsString.c_str()) + totalBytesRead),
      bufferSize - totalBytesRead,
      &bytesRead, nullptr) &&
      bytesRead > 0)
   {
      totalBytesRead += bytesRead;
   }

   return fileAsString;
}

bool FileExists(
   const _tstring &fileName)
{
   const CSmartHandle hFile(::CreateFile(
      fileName.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      nullptr,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      nullptr));

   bool exists = hFile.IsValid();

   if (!exists)
   {
      const DWORD lastError = GetLastError();

      if (lastError == ERROR_SHARING_VIOLATION)
      {
         exists = true;
      }
   }

   return exists;
}

bool FileExistsA(
   const string &fileName)
{
   const CSmartHandle hFile(::CreateFileA(
      fileName.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      nullptr,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      nullptr));

   bool exists = hFile.IsValid();

   if (!exists)
   {
      const DWORD lastError = GetLastError();

      if (lastError == ERROR_SHARING_VIOLATION)
      {
         exists = true;
      }
   }

   return exists;
}

bool FileDoesNotExist(
   const _tstring &fileName)
{
   const CSmartHandle hFile(::CreateFile(
      fileName.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      nullptr,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      nullptr));

   const DWORD lastError = GetLastError();

   return !hFile.IsValid() && (lastError == ERROR_FILE_NOT_FOUND || lastError == ERROR_PATH_NOT_FOUND);
}

bool FileExistsAndIsReadLocked(
   const _tstring &fileName)
{
   bool ok = false;

   const CSmartHandle hFile(::CreateFile(
      fileName.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      nullptr,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      nullptr));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = GetLastError();

      if (lastError == ERROR_SHARING_VIOLATION)
      {
         ok = true;
      }
      else
      {
         throw CErrorCodeException(_T("FileExistsAndIsReadLocked() \"") + fileName + _T("\""), lastError);
      }
   }

   return ok;
}


///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static bool StringIsAllANSI(
   const std::wstring &data)
{
   const auto *pData = reinterpret_cast<const BYTE *>(data.c_str());

   const size_t dataLength = data.length() * sizeof(wchar_t);

   bool allANSI = true;

   for (size_t i = 1; allANSI && i < dataLength; i += 2)
   {
      if (pData[i] != 0x00)
      {
         allANSI = !(i != 1 || pData[i] != 0xFE);
      }
   }

   return allANSI;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: FileUtils.cpp
///////////////////////////////////////////////////////////////////////////////
