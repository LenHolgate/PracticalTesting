#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: FileUtils.h
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

#include "JetByteTools/CoreTools/Types.h"

#include <Windows.h>

#include "JetByteTools/CoreTools/tstring.h"
#include "JetByteTools/CoreTools/ExpandableBuffer.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Functions defined in this file...
///////////////////////////////////////////////////////////////////////////////

_tstring GetExePath();

_tstring GetExeFileName();

void WriteResourceToFile(
   HANDLE hFile,
   const _tstring &resourceName,
   const _tstring &resourceType,
   HMODULE hModule = nullptr);

void CreateDirectory(
   const _tstring &directory);

bool CreateDirectoryIfRequired(
   const _tstring &directory);

size_t CreateDirectoriesIfRequired(
   const _tstring &directory);

bool DirectoryExists(
   const _tstring &directory);

_tstring GetCurrentDirectory();

void SetCurrentDirectory(
   const _tstring &directory);

_tstring EnsurePathEndsWithSingleBackslash(
   const _tstring &path);

bool TryCombinePath(
   _tstring &combinedPath,
   const _tstring &path1,
   const _tstring &path2);

_tstring CombinePath(
   const _tstring &path1,
   const _tstring &path2);

bool PathHasRelativeRoot(
   const _tstring &path);

_tstring ResolveRelativePath(
   const _tstring &path);

_tstring ResolveRelativePath(
   const _tstring &path,
   const _tstring &root);

_tstring MakePathAbsolute(
   const _tstring &path);

_tstring MakePathAbsolute(
   const _tstring &path,
   const _tstring &root);

_tstring BuildPath(
   const _tstring &path1,
   const _tstring &path2);

_tstring GetTempPath();

_tstring GetTempFileName(
   const _tstring &pathName,
   const _tstring &prefixString,
   unsigned int unique = 0);

_tstring GetTempFileName(
   const _tstring &prefixString,
   unsigned int unique = 0);

_tstring GetFileExtension(
   const _tstring &filename);

_tstring StripFileExtension(
   const _tstring &filename);

std::string StripFileExtensionA(
   const std::string &filename);

_tstring GetFileNameFromPathName(
   const _tstring &pathName);

_tstring GetFileNameFromPathName(
   const _tstring &pathName,
   const _tstring &separators);

std::string GetFileNameFromPathNameA(
   const std::string &pathName);

std::string GetFileNameFromPathNameA(
   const std::string &pathName,
   const std::string &separators);

_tstring StripFileNameFromPathName(
   const _tstring &pathName);

_tstring StripFileNameFromPathName(
   const _tstring &pathName,
   const _tstring &separators);

std::string StripFileNameFromPathNameA(
   const std::string &pathName);

std::string StripFileNameFromPathNameA(
   const std::string &pathName,
   const std::string &separators);

inline _tstring StripLastDirectoryFromPathName(
   const _tstring &pathName)
{
   return StripFileNameFromPathName(pathName);
}

void DeleteFile(
   const _tstring &fileName);

void DeleteFileIfExists(
   const _tstring &fileName);

void MoveFile(
   const _tstring &filenameFrom,
   const _tstring &filenameTo);

void CopyFile(
   const _tstring &filenameFrom,
   const _tstring &filenameTo,
   bool failIfExists);

__int64 GetFileSize(
   const _tstring &filename);

void LoadFileAsBinaryData(
   const _tstring &filename,
   TExpandableBuffer<BYTE> &buffer);

void SaveBinaryDataAsFile(
   const _tstring &filename,
   const TExpandableBuffer<BYTE> &buffer);

std::wstring LoadFileAsUnicodeString(
   const _tstring &filename,
   bool *pFileWasUnicode = nullptr);

void SaveUnicodeStringAsFile(
   const _tstring &filename,
   const std::wstring &data,
   bool saveAsANSIifPossible = false);

_tstring LoadFileAsString(
   const _tstring &filename);

std::string LoadFileAsStringA(
   const _tstring &filename);

_tstring LoadFileAsString(
   HANDLE hFile,
   const _tstring &filename);

std::string LoadFileAsStringA(
   HANDLE hFile,
   const _tstring &filename);

void SaveStringAsFile(
   const _tstring &filename,
   const _tstring &data);

void SaveStringAsFileA(
   const _tstring &filename,
   const std::string &data);

void SaveStringToFile(
   const _tstring &filename,
   HANDLE hFile,
   const _tstring &data);

void SaveStringToFileA(
   const _tstring &filename,
   HANDLE hFile,
   const std::string &data);

bool FileExists(
   const _tstring &filename);

bool FileExistsA(
   const std::string &filename);

bool FileDoesNotExist(
   const _tstring &fileName);

bool FileExistsAndIsReadLocked(
   const _tstring &fileName);

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: FileUtils.h
///////////////////////////////////////////////////////////////////////////////

