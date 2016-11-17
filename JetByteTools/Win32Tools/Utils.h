#pragma once
#ifndef JETBYTE_TOOLS_WIN32_UTILS__
#define JETBYTE_TOOLS_WIN32_UTILS__
///////////////////////////////////////////////////////////////////////////////
// File: Utils.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 1997 JetByte Limited.
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

#include <wtypes.h>

#include "tstring.h"
#include "StringConverter.h"
#include "Exception.h"
#include "ToString.h"
#include "ExpandableBuffer.h"

#include <limits>

#ifdef min
#pragma push_macro("min")
#define JETBYTE_UTILS_PUSHED_MIN
#undef min
#endif

#ifdef max
#pragma push_macro("max")
#define JETBYTE_UTILS_PUSHED_MAX
#undef max
#endif

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

template <typename TV, typename TM>
inline TV RoundDown(TV Value, TM Multiple)
{
   return((Value / Multiple) * Multiple);
}

template <typename TV, typename TM>
inline TV RoundUp(TV Value, TM Multiple)
{
   return(RoundDown(Value, Multiple) + (((Value % Multiple) > 0) ? Multiple : 0));
}

template <typename result, class s>
result GetStringLength(
   const s &theString,
   const bool includeNullTerminator = false)
{
   const unsigned __int64 length = theString.length() + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<unsigned __int64>(std::numeric_limits<result>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<result>(length);
}

template <typename result>
result GetStringLength(
   const char *pString,
   const bool includeNullTerminator = false)
{
   const unsigned __int64 length = strlen(pString) + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<unsigned __int64>(std::numeric_limits<result>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<result>(length);
}

template <typename result>
result GetStringLength(
   const wchar_t *pString,
   const bool includeNullTerminator = false)
{
   const unsigned __int64 length = wcslen(pString) + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<unsigned __int64>(std::numeric_limits<result>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<result>(length);
}

inline void OutputDebugString(
   const _tstring &message)
{
   ::OutputDebugString(message.c_str());
}

void WriteResourceToFile(
   HANDLE hFile,
   const _tstring &resourceName,
   const _tstring &resourceType,
   HMODULE hModule = 0);

unsigned long GetLongFromString(
   const _tstring &numeric,
   const size_t startOffset,
   const size_t length);

unsigned short GetShortFromString(
   const _tstring &numeric,
   const size_t startOffset,
   const size_t length);

unsigned long GetLongFromStringA(
   const std::string &numeric,
   const size_t startOffset,
   const size_t length);

unsigned short GetShortFromStringA(
   const std::string &numeric,
   const size_t startOffset,
   const size_t length);

bool IsAllDigits(
   const _tstring &numeric);

bool IsAllDigitsA(
   const std::string &numeric);

bool IsAllHexDigitsA(
   const std::string &numeric);

bool StringToBool(
   const _tstring &stringRepresentation);

bool StringToBoolA(
   const std::string &stringRepresentation);

template <class T>
bool ToBool(const T &value)
{
   return (0 != value);
}

void StringToHex(
   const _tstring &str,
   BYTE *pBuffer,
   size_t nBytes);

_tstring GetLastErrorMessageIfPossible(
   DWORD last_error,
   bool stripTrailingLineFeed = false);

_tstring GetLastErrorMessageIfPossible(
   const HMODULE hModule, 
   DWORD last_error,
   bool stripTrailingLineFeed = false);

_tstring GetLastErrorMessage(
   DWORD last_error,
   bool stripTrailingLineFeed = false);

_tstring GetLastErrorMessage(
   const HMODULE hModule, 
   DWORD last_error,
   bool stripTrailingLineFeed = false);

std::string GetLastErrorMessageA(
   DWORD last_error,
   bool stripTrailingLineFeed = false);

std::wstring GetLastErrorMessageW(
   DWORD last_error,
   bool stripTrailingLineFeed = false);

void CreateDirectory(
   const _tstring &directory);

bool CreateDirectoryIfRequired(
   const _tstring &directory);

size_t CreateDirectoriesIfRequired(
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

_tstring MakePathAbsolute(
   const _tstring &path);

_tstring GetDateStamp();

_tstring GetTimeStamp();

std::string GetTimeStampA();

_tstring ToUpper(
   const _tstring &data);

std::string ToUpperA(
   const std::string &data);

std::wstring ToUpperW(
   const std::wstring &data);

_tstring ToLower(
   const _tstring &data);

std::string ToLowerA(
   const std::string &data);

std::wstring ToLowerW(
   const std::wstring &data);

_tstring GetTempPath();

_tstring GetTempFileName(
   const _tstring &pathName,
   const _tstring &prefixString,
   const unsigned int unique = 0);

_tstring GetTempFileName(
   const _tstring &prefixString,
   const unsigned int unique = 0);

_tstring GetComputerName();

_tstring GetModuleFileName(
   HANDLE hProcess,
   HINSTANCE hModule);

bool GetModuleFileName(
   HANDLE hProcess,
   HINSTANCE hModule,
   _tstring &name);

_tstring GetModuleFileName(
   HINSTANCE hModule = 0);

_tstring GetModulePathName(
   HINSTANCE hModule = 0);

_tstring StripFileExtension(
   const _tstring &filename);

std::string StripFileExtensionA(
   const std::string &filename);

_tstring GetSystemWindowsDirectory();

_tstring GetSystemDirectory();

bool Is64bitSystem();

bool IsWow64Process();

bool Is64bitProcess();

bool Is32bitProcess();

_tstring GetSystemWow64Directory();

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

void RemoveDirectoryAndContents(
   const _tstring &directory);

void RemoveDirectoryContents(
   const _tstring &directory);

_tstring GetUserName();

_tstring GetFileVersion();

_tstring GetFileVersion(
   const _tstring &languageID);

_tstring GetFileVersion(
   const _tstring &languageID,
   const _tstring &charsetID);

_tstring GetFileVersion(
   const HMODULE hModule);

_tstring GetFileVersion(
   const HMODULE hModule,
   const _tstring &languageID);

_tstring GetFileVersion(
   const HMODULE hModule,
   const _tstring &languageID,
   const _tstring &charsetID);

_tstring GetFileVersionString(
   const _tstring &requiredString);

_tstring GetFileVersionString(
   const _tstring &languageID,
   const _tstring &requiredString);

_tstring GetFileVersionString(
   const _tstring &languageID,
   const _tstring &charsetID,
   const _tstring &requiredString);

_tstring GetFileVersionString(
   const HMODULE hModule,
   const _tstring &requiredString);

_tstring GetFileVersionString(
   const HMODULE hModule,
   const _tstring &languageID,
   const _tstring &requiredString);

_tstring GetFileVersionString(
   const HMODULE hModule,
   const _tstring &languageID,
   const _tstring &charsetID,
   const _tstring &requiredString);

_tstring StripWhiteSpace(
   const _tstring &source);

std::string StripWhiteSpaceA(
   const std::string &source);

_tstring StripSurroundingWhiteSpace(
   const _tstring &source);

std::string StripSurroundingWhiteSpaceA(
   const std::string &source);

_tstring StripLeading(
   const _tstring &source,
   const char toStrip);

_tstring StripTrailing(
   const _tstring &source,
   const char toStrip);

std::string StripLeadingA(
   const std::string &source,
   const char toStrip);

std::string StripTrailingA(
   const std::string &source,
   const char toStrip);

void MoveFile(
   const _tstring &filenameFrom,
   const _tstring &filenameTo);

void CopyFile(
   const _tstring &filenameFrom,
   const _tstring &filenameTo,
   const bool failIfExists);

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
   bool *pFileWasUnicode = 0);

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

_tstring FindAndReplace(
   const _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString,
   const size_t numReplacements = INFINITE);

//lint -esym(534, JetByteTools::Win32::InPlaceFindAndReplace) (Ignoring return value of function)
bool InPlaceFindAndReplace(
   _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString,
   const size_t numReplacements = INFINITE);

std::string FindAndReplaceA(
   const std::string &phrase,
   const std::string &findString,
   const std::string &replaceString,
   const size_t numReplacements = INFINITE);

//lint -esym(534, JetByteTools::Win32::InPlaceFindAndReplaceA) (Ignoring return value of function)
bool InPlaceFindAndReplaceA(
   std::string &phrase,
   const std::string &findString,
   const std::string &replaceString,
   const size_t numReplacements = INFINITE);

bool IsGoodReadPtr(
   const void *pv,
   ULONG cb);

bool IsGoodReadPtr(
   HANDLE hProcess,
   const void *pv,
   ULONG cb);

bool IsGoodWritePtr(
   const void *pv,
   ULONG cb);

bool IsGoodWritePtr(
   HANDLE hProcess,
   const void *pv,
   ULONG cb);

_tstring GUIDAsString(
   const GUID &guid,
   const bool stripBrackets = true);

_tstring CreateGUIDAsString(
   const bool stripBrackets = true);

_tstring GetFileNameFromHandle(
   const HANDLE hFile);

_tstring GetFileNameFromHandleIfPossible(
   const HANDLE hFile);

_tstring TranslateDeviceNamePathToDriveLetterPath(
   const _tstring &deviceNamePath);

#ifdef JETBYTE_UTILS_PUSHED_MIN
#pragma pop_macro("min")
#endif

#ifdef JETBYTE_UTILS_PUSHED_MAX
#pragma pop_macro("max")
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_UTILS__

///////////////////////////////////////////////////////////////////////////////
// End of file: Utils.h
///////////////////////////////////////////////////////////////////////////////

