///////////////////////////////////////////////////////////////////////////////
// File: Utils.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "Utils.h"
#include "Exception.h"
#include "Win32Exception.h"
#include "SmartHandle.h"

#include "ExpandableBuffer.h"

#pragma hdrstop

#include <iostream>
#include <algorithm>
#include <list>

#include <Psapi.h>

#include <Lmcons.h>     // UNLEN

#include <Shlwapi.h>    // PathCombine

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "psapi.lib")

#pragma comment(lib, "Shlwapi.lib")

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::endl;
using std::string;
using std::wstring;
using std::min;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static bool IsGoodPtr(
   HANDLE hProcess,
   const void *pv,
   ULONG cb,
   DWORD dwFlags);

static bool StringIsAllANSI(
   const std::wstring &data);

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

bool StringToBool(
   const _tstring &stringRepresentation)
{
   if (stringRepresentation == _T("1"))
   {
      return true;
   }

   if (stringRepresentation == _T("0"))
   {
      return false;
   }

   if (0 == _tcsncicmp(stringRepresentation.c_str(), _T("TRUE"), stringRepresentation.length()))
   {
      return true;
   }

   if (0 == _tcsncicmp(stringRepresentation.c_str(), _T("FALSE"), stringRepresentation.length()))
   {
      return false;
   }

   throw CException(_T("StringToBool()"), _T("Can't convert: \"") + stringRepresentation + _T("\" to a bool value"));
}

bool StringToBoolA(
   const string &stringRepresentation)
{
   if (stringRepresentation == "1")
   {
      return true;
   }

   if (stringRepresentation == "0")
   {
      return false;
   }

   if (0 == _strnicmp(stringRepresentation.c_str(), "TRUE", stringRepresentation.length()))
   {
      return true;
   }

   if (0 == _strnicmp(stringRepresentation.c_str(), "FALSE", stringRepresentation.length()))
   {
      return false;
   }

   throw CException(_T("StringToBool()"), _T("Can't convert: \"") + CStringConverter::AtoT(stringRepresentation) + _T("\" to a bool value"));
}

bool IsAllDigits(
   const _tstring &numeric)
{
   bool ok = (numeric.length() != 0);

   for (_tstring::const_iterator it = numeric.begin(); ok && it != numeric.end(); ++it)
   {
      ok = ToBool(_istdigit(*it));
   }

   return ok;
}

bool IsAllDigitsA(
   const string &numeric)
{
   bool ok = (numeric.length() != 0);

   for (string::const_iterator it = numeric.begin(); ok && it != numeric.end(); ++it)
   {
      ok = ToBool(isdigit(static_cast<unsigned char>(*it)));
   }

   return ok;
}

bool IsAllHexDigitsA(
   const string &numeric)
{
   bool ok = (numeric.length() != 0);

   for (string::const_iterator it = numeric.begin(); ok && it != numeric.end(); ++it)
   {
      ok = ToBool(isxdigit(static_cast<unsigned char>(*it)));
   }

   return ok;
}

_tstring GetLastErrorMessageIfPossible(
   DWORD last_error,
   bool stripTrailingLineFeed)
{
   TCHAR errmsg[512];

   if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      last_error,
      0,
      errmsg,
      511,
      nullptr))
   {
      return _T("");
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = _tcslen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

_tstring GetLastErrorMessageIfPossible(
   const HMODULE hModule, 
   DWORD last_error,
   bool stripTrailingLineFeed)
{
   TCHAR errmsg[512];

   if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
      hModule,
      last_error,
      0,
      errmsg,
      511,
      nullptr))
   {
      return _T("");
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = _tcslen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

_tstring GetLastErrorMessage(
   DWORD last_error,
   bool stripTrailingLineFeed)
{
   TCHAR errmsg[512];

   if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      last_error,
      0,
      errmsg,
      511,
      nullptr))
   {
      // if we fail, call ourself to find out why and return that error

      const DWORD thisError = ::GetLastError();

      if (thisError != last_error)
      {
         return GetLastErrorMessage(thisError, stripTrailingLineFeed);
      }

      // But don't get into an infinite loop...

      return _T("Failed to obtain error string: ") + ToString(last_error);
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = _tcslen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

_tstring GetLastErrorMessage(
   const HMODULE hModule, 
   DWORD last_error,
   bool stripTrailingLineFeed)
{
   TCHAR errmsg[512];

   if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
      hModule,
      last_error,
      0,
      errmsg,
      511,
      nullptr))
   {
      // if we fail, call ourself to find out why and return that error

      const DWORD thisError = ::GetLastError();

      if (thisError != last_error)
      {
         return GetLastErrorMessage(thisError, stripTrailingLineFeed);
      }

      // But don't get into an infinite loop...

      return _T("Failed to obtain error string: ") + ToString(last_error);
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = _tcslen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

string GetLastErrorMessageA(
   DWORD last_error,
   bool stripTrailingLineFeed)
{
   CHAR errmsg[512];

   if (!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      last_error,
      0,
      errmsg,
      511,
      nullptr))
   {
      // if we fail, call ourself to find out why and return that error

      const DWORD thisError = ::GetLastError();

      if (thisError != last_error)
      {
         return GetLastErrorMessageA(thisError, stripTrailingLineFeed);
      }

      // But don't get into an infinite loop...

      return "Failed to obtain error string: " + ToStringA(last_error);
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = strlen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

wstring GetLastErrorMessageW(
   DWORD last_error,
   bool stripTrailingLineFeed)
{
   wchar_t errmsg[512];

   if (!FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      last_error,
      0,
      errmsg,
      511,
      nullptr))
   {
      // if we fail, call ourself to find out why and return that error

      const DWORD thisError = ::GetLastError();

      if (thisError != last_error)
      {
         return GetLastErrorMessageW(thisError, stripTrailingLineFeed);
      }

      // But don't get into an infinite loop...

      return L"Failed to obtain error string: " + ToStringW(last_error);
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = wcslen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

void StringToHex(
   const _tstring &ts,
   BYTE *pBuffer,
   size_t nBytes)
{
   const string s = CStringConverter::TtoA(ts);

   for (size_t i = 0; i < nBytes; i++)
   {
      const size_t stringOffset = i * 2;

      const BYTE b = static_cast<const BYTE>(s[stringOffset]);

      BYTE val = isdigit(b) ? static_cast<BYTE>((b - '0') * 16) : static_cast<BYTE>(((toupper(b) - 'A') + 10) * 16);

      const BYTE b1 = static_cast<const BYTE >(s[stringOffset + 1]);

      val = isdigit(b1) ? static_cast<BYTE>(val + b1 - '0') : static_cast<BYTE>(val + (toupper(b1) - 'A') + 10);

      pBuffer[i] = val;
   }
}

void CreateDirectory(
   const _tstring &directory)
{
   if (!::CreateDirectory(directory.c_str(), 0))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("CreateDirectory()"), lastError);
   }
}

bool CreateDirectoryIfRequired(
   const _tstring &directory)
{
   bool created = false;

   if (!::CreateDirectory(directory.c_str(), 0))
   {
      created = true;

      const DWORD lastError = ::GetLastError();

      if (lastError != ERROR_ALREADY_EXISTS)
      {
         throw CWin32Exception(_T("CreateDirectoryIfRequired()"), lastError);
      }
   }

   return created;
}

size_t CreateDirectoriesIfRequired(
   const _tstring &directory)
{
   size_t numCreated = 0;

   if (!::CreateDirectory(directory.c_str(), 0))
   {
      const DWORD lastError = ::GetLastError();

      if (lastError == ERROR_PATH_NOT_FOUND)
      {
         const _tstring::size_type pos = directory.find_last_of(_T("\\"));

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
            throw CWin32Exception(_T("CreateDirectoriesIfRequired()"), lastError);
         }
      }
      else if (lastError != ERROR_ALREADY_EXISTS)
      {
         throw CWin32Exception(_T("CreateDirectoriesIfRequired()"), lastError);
      }
   }

   return numCreated;
}

_tstring GetCurrentDirectory()
{
   DWORD size = ::GetCurrentDirectory(0, nullptr);

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
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SetCurrentDirectory()"), lastError);
   }
}

_tstring EnsurePathEndsWithSingleBackslash(
   const _tstring &path)
{
   _tstring result = path;

   _tstring::size_type pos = result.find_last_of(_T("\\"));

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
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("CombinePath() failed for \"") + path1 + _T("\", \"") + path2 + _T("\""), lastError);
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

   if (0 != ::PathCombine(outBuffer, path1.c_str(), path2.c_str()))
   {
      combinedPath = outBuffer;

      ok = true;
   }

   return ok;
}

_tstring MakePathAbsolute(
   const _tstring &path)
{
   // Combine a path with the current directory to process any relative path
   // constructs within the path and create a "clean" absolute path.

   _tstring absolutePath;

   if (!TryCombinePath(absolutePath, path, _T(".")))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("MakePathAbsolute() failed for \"") + path + _T("\""), lastError);
   }

   return absolutePath;
}

_tstring GetDateStamp()
{
   SYSTEMTIME systime;
   GetLocalTime(&systime);

   TCHAR buffer[7];

   (void)_stprintf_s(
      buffer,
      _T("%02u%02u%02u"),
      static_cast<unsigned int>(systime.wDay),
      static_cast<unsigned int>(systime.wMonth),
      static_cast<unsigned int>(systime.wYear % 100));

   return buffer;
}

_tstring GetTimeStamp()
{
   SYSTEMTIME systime;
   GetLocalTime(&systime);

   TCHAR buffer[14];

   (void)_stprintf_s(
      buffer,
      _T("%02u:%02u:%02u.%04u"),
      static_cast<unsigned int>(systime.wHour),
      static_cast<unsigned int>(systime.wMinute),
      static_cast<unsigned int>(systime.wSecond),
      static_cast<unsigned int>(systime.wMilliseconds));

   return buffer;
}

string GetTimeStampA()
{
   SYSTEMTIME systime;
   GetLocalTime(&systime);

   char buffer[14];

   sprintf_s(
      buffer,
      sizeof(buffer),
      "%02u:%02u:%02u.%04u",
      static_cast<unsigned int>(systime.wHour),
      static_cast<unsigned int>(systime.wMinute),
      static_cast<unsigned int>(systime.wSecond),
      static_cast<unsigned int>(systime.wMilliseconds));

   return buffer;
}

_tstring GetComputerName()
{
   TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1] ;
   DWORD computerNameLen = MAX_COMPUTERNAME_LENGTH + 1;

   if (::GetComputerName(computerName, &computerNameLen))
   {
      return computerName;
   }

   return _T("UNAVAILABLE");
}

_tstring GetModuleFileName(
   HANDLE hProcess,
   HINSTANCE hModule)
{
   _tstring name = _T("UNAVAILABLE");

   const bool ok = GetModuleFileName(hProcess, hModule, name);

   (void)ok;

   return name;
}

bool GetModuleFileName(
   HANDLE hProcess,
   HINSTANCE hModule,
   _tstring &name)
{
   TCHAR moduleFileName[MAX_PATH + 1] ;
   DWORD moduleFileNameLen = MAX_PATH ;

   const bool ok = ToBool(::GetModuleFileNameEx(hProcess, hModule, moduleFileName, moduleFileNameLen));

   if (ok)
   {
      name = moduleFileName;
   }

   return ok;
}

_tstring GetModuleFileName(
   HINSTANCE hModule /* = 0 */)
{
   _tstring name = _T("UNAVAILABLE");

   TCHAR moduleFileName[MAX_PATH + 1] ;
   DWORD moduleFileNameLen = MAX_PATH ;

   if (::GetModuleFileName(hModule, moduleFileName, moduleFileNameLen))
   {
      name = moduleFileName;
   }

   return name;
}

_tstring GetModulePathName(
   HINSTANCE hModule /* = 0 */)
{
   _tstring path = GetModuleFileName(hModule);

   _tstring::size_type pos = path.find_last_of(_T("\\/:")); //lint !e691 (Suspicious use of backslash)

   return path.substr(0, pos);
}

_tstring StripFileExtension(
   const _tstring &filename)
{
   _tstring::size_type pos = filename.find_last_of(_T("."));

   return filename.substr(0, pos);
}

string StripFileExtensionA(
   const string &filename)
{
   string::size_type pos = filename.find_last_of(".");

   return filename.substr(0, pos);
}

_tstring GetSystemWindowsDirectory()
{
   TCHAR buffer[MAX_PATH + 1];

   DWORD bufferLen = MAX_PATH;

   const UINT result = ::GetSystemWindowsDirectory(buffer, bufferLen);

   if (result == 0)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("GetSystemWindowsDirectory()"), lastError);
   }

   if (result > bufferLen)
   {
      throw CException(_T("GetSystemWindowsDirectory()"), _T("System directory is more than: ") + ToString(bufferLen) + _T(" bytes long..."));
   }

   return buffer;
}

_tstring GetSystemDirectory()
{
   TCHAR buffer[MAX_PATH + 1];

   DWORD bufferLen = MAX_PATH;

   const UINT result = ::GetSystemDirectory(buffer, bufferLen);

   if (result == 0)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("GetSystemDirectory()"), lastError);
   }

   if (result > bufferLen)
   {
      throw CException(_T("GetSystemDirectory()"), _T("System directory is more than: ") + ToString(bufferLen) + _T(" bytes long..."));
   }

   return buffer;
}

_tstring GetFileNameFromPathName(
   const _tstring &pathName)
{
   return GetFileNameFromPathName(pathName, _T("\\"));
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
   return GetFileNameFromPathNameA(pathName, "\\");
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
   return StripFileNameFromPathName(pathName, _T("\\"));
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
   return StripFileNameFromPathNameA(pathName, "\\");
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
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("DeleteFile() - \"") + fileName + _T("\""), lastError);
   }
}

bool Is64bitSystem()
{
#if defined(_WIN64)

   return true;  // 64-bit programs run only on Win64

#elif defined(_WIN32)

   bool is64bit = false;

   typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

   LPFN_ISWOW64PROCESS pfnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(::GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process"));

   // 32-bit programs run on both 32-bit and 64-bit Windows
   // so must sniff

   if (pfnIsWow64Process)
   {
      BOOL f64 = FALSE;

      if (!pfnIsWow64Process(::GetCurrentProcess(), &f64))
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("Is64bitSystem()"), lastError);
      }

      is64bit = ToBool(f64);
   }

   return is64bit;
#else

 return false; // Win64 does not support Win16

#endif
}

// 11/3/2010 - #pragma JETBYTE_TODO("Move all x64 code into a new file")

bool IsWow64Process()
{
#if defined(_WIN64)

   return false;  // 64-bit programs run natively on Win64

#elif defined(_WIN32)

   bool is64bit = false;

   typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

   LPFN_ISWOW64PROCESS pfnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(::GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process"));

   // 32-bit programs run on both 32-bit and 64-bit Windows
   // so must sniff

   if (pfnIsWow64Process)
   {
      BOOL f64 = FALSE;

      if (!pfnIsWow64Process(::GetCurrentProcess(), &f64))
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("IsWow64Process()"), lastError);
      }

      is64bit = ToBool(f64);
   }

   return is64bit;
#else

 return false; // Win64 does not support Win16

#endif
}

bool Is64bitProcess()
{
#if defined(_WIN64)

   return true;  // 64-bit programs run natively on Win64

#else

   return false;

#endif
}

bool Is32bitProcess()
{
#if defined(_WIN64)

   return false;

#else

   return true ;  // Compiled as a 32-bit program

#endif
}

_tstring GetSystemWow64Directory()
{
   TCHAR buffer[MAX_PATH + 1];

   DWORD bufferLen = MAX_PATH;

   typedef UINT (WINAPI *LPFN_GETSYSTEMWOW64DIRECTORY) (LPTSTR, UINT);

#ifdef UNICODE
   const LPCSTR pFunctionName = "GetSystemWow64DirectoryW";
#else
   const LPCSTR pFunctionName = "GetSystemWow64DirectoryA";
#endif

   LPFN_GETSYSTEMWOW64DIRECTORY pfnGetSystemWow64Directory = reinterpret_cast<LPFN_GETSYSTEMWOW64DIRECTORY>(::GetProcAddress(::GetModuleHandle(_T("kernel32")), pFunctionName));

   if (pfnGetSystemWow64Directory)
   {
      const UINT result = (pfnGetSystemWow64Directory)(buffer, bufferLen);

      if (result == 0)
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("GetSystemWow64Directory()"), lastError);
      }

      if (result > bufferLen)
      {
         throw CException(_T("GetSystemWow64Directory()"), _T("System directory is more than: ") + ToString(bufferLen) + _T(" bytes long..."));
      }
   }
   else
   {
      throw CException(_T("GetSystemWow64Directory()"), _T("Functionality not available on this platform"));
   }

   return buffer;
}

_tstring GetUserName()
{
   _tstring name = _T("UNAVAILABLE");

   TCHAR userName[UNLEN + 1] ;
   DWORD userNameLen = UNLEN;

   if (::GetUserName(userName, &userNameLen))
   {
      name = userName;
   }

   return name;
}

_tstring StripWhiteSpace(
   const _tstring &source)
{
   _tstring destination;

   destination.resize(source.size());

   const TCHAR *pSrc = source.c_str();

   TCHAR *pDst = const_cast<TCHAR *>(destination.c_str());

   size_t i = 0;

   while (*pSrc)
   {
      if (!_istspace(*pSrc))
      {
         *pDst = *pSrc;
         pDst++;
         i++;
      }

      ++pSrc;
   }

   destination.resize(i);

   return destination;
}

string StripWhiteSpaceA(
   const string &source)
{
   string destination;

   destination.resize(source.size());

   const char *pSrc = source.c_str();

   char *pDst = const_cast<char *>(destination.c_str());

   size_t i = 0;

   while (*pSrc)
   {
      if (!isspace(static_cast<unsigned char>(*pSrc)))
      {
         *pDst = *pSrc;
         pDst++;
         i++;
      }

      ++pSrc;
   }

   destination.resize(i);

   return destination;
}

_tstring StripSurroundingWhiteSpace(
   const _tstring &source)
{
   const TCHAR *pSrc = source.c_str();

   while (pSrc && _istspace(*pSrc))
   {
      ++pSrc;
   }

   _tstring result = pSrc;

   size_t i = result.length();

   pSrc = result.c_str() + i;

   --pSrc;

   while (i && _istspace(*pSrc))
   {
      --pSrc;
      --i;
   }

   return result.substr(0, i);
}

string StripSurroundingWhiteSpaceA(
   const string &source)
{
   const char *pSrc = source.c_str();

   while (pSrc && isspace(static_cast<unsigned char>(*pSrc)))
   {
      ++pSrc;
   }

   string result = pSrc;

   size_t i = result.length();

   pSrc = result.c_str() + i;

   --pSrc;

   while (i && isspace(static_cast<unsigned char>(*pSrc)))
   {
      --pSrc;
      --i;
   }

   return result.substr(0, i);
}

_tstring StripLeading(
   const _tstring &source,
   const char toStrip)
{
   const TCHAR *pSrc = source.c_str();

   while (pSrc && *pSrc == toStrip)
   {
      ++pSrc;
   }

   return pSrc;
}

_tstring StripTrailing(
   const _tstring &source,
   const char toStrip)
{
   size_t i = source.length();
   const _TCHAR *pSrc = source.c_str() + i;

   --pSrc;

   while (i && *pSrc == toStrip)
   {
      --pSrc;
      --i;
   }

   return source.substr(0, i);
}

string StripLeadingA(
   const string &source,
   const char toStrip)
{
   const char *pSrc = source.c_str();

   while (pSrc && *pSrc == toStrip)
   {
      ++pSrc;
   }

   return pSrc;
}

string StripTrailingA(
   const string &source,
   const char toStrip)
{
   size_t i = source.length();
   const char *pSrc = source.c_str() + i;

   --pSrc;

   while (i && *pSrc == toStrip)
   {
      --pSrc;
      --i;
   }

   return source.substr(0, i);
}

#pragma comment(lib, "Version.lib")

static const _tstring s_fileVersionString(_T("ProductVersion"));

_tstring GetFileVersion()
{
   return GetFileVersionString(s_fileVersionString);
}

_tstring GetFileVersion(
   const _tstring &languageID)
{
   return GetFileVersionString(languageID, s_fileVersionString);
}

_tstring GetFileVersion(
   const _tstring &languageID,
   const _tstring &charsetID)
{
   return GetFileVersionString(languageID, charsetID, s_fileVersionString);
}

_tstring GetFileVersion(
   const HMODULE hModule)
{
   return GetFileVersionString(hModule, s_fileVersionString);
}

_tstring GetFileVersion(
   const HMODULE hModule,
   const _tstring &languageID)
{
   return GetFileVersionString(hModule, languageID, s_fileVersionString);
}

_tstring GetFileVersion(
   const HMODULE hModule,
   const _tstring &languageID,
   const _tstring &charsetID)
{
   return GetFileVersionString(hModule, languageID, charsetID, s_fileVersionString);
}

_tstring GetFileVersionString(
   const _tstring &requiredString)
{
   return GetFileVersionString(
      0,
      requiredString);
}

_tstring GetFileVersionString(
   const _tstring &languageID,
   const _tstring &requiredString)
{
   return GetFileVersionString(
      0,
      languageID,
      requiredString);
}

_tstring GetFileVersionString(
   const _tstring &languageID,
   const _tstring &charsetID,
   const _tstring &requiredString)
{
   return GetFileVersionString(
      0,
      languageID,
      charsetID,
      requiredString);
}

_tstring GetFileVersionString(
   const HMODULE hModule,
   const _tstring &requiredString)
{
   return GetFileVersionString(
      hModule,
      _T("0809"),          // UK english
      _T("04b0"),          // Unicode
      requiredString);
}

_tstring GetFileVersionString(
   const HMODULE hModule,
   const _tstring &languageID,
   const _tstring &requiredString)
{
   return GetFileVersionString(
      hModule,
      languageID,
      _T("04b0"),          // Unicode
      requiredString);
}

_tstring GetFileVersionString(
   const HMODULE hModule,
   const _tstring &languageID,
   const _tstring &charsetID,
   const _tstring &requiredString)
{
   _tstring version;

   const _tstring moduleFileName = GetModuleFileName(hModule);

   LPTSTR pModuleFileName = const_cast<LPTSTR>(moduleFileName.c_str());

   DWORD zero = 0;

   DWORD verSize = ::GetFileVersionInfoSize(pModuleFileName, &zero);

   if (verSize != 0)
   {
      TExpandableBuffer<BYTE> buffer(verSize);

      if (::GetFileVersionInfo(pModuleFileName, 0, verSize, buffer))
      {
         LPTSTR pVersion = 0;
         UINT verLen = 0;

         const _tstring query = _T("\\StringFileInfo\\") + languageID + charsetID + _T("\\") + requiredString;

         if (::VerQueryValue(buffer,
              const_cast<LPTSTR>(query.c_str()),
              reinterpret_cast<void**>(&pVersion),
              &verLen))
         {
            version = pVersion;
         }
      }
   }

   return version;
}

_tstring ToUpper(
   const _tstring &data)
{
   _tstring dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<TCHAR>(toupper(dataOut[i]));
   }

   return dataOut;
}

string ToUpperA(
   const string &data)
{
   string dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<char>(toupper(dataOut[i]));
   }

   return dataOut;
}

wstring ToUpperW(
   const wstring &data)
{
   wstring dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<wchar_t>(toupper(dataOut[i]));
   }

   return dataOut;
}

_tstring ToLower(
   const _tstring &data)
{
   _tstring dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<TCHAR>(tolower(dataOut[i]));
   }

   return dataOut;
}

string ToLowerA(
   const string &data)
{
   string dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<char>(tolower(dataOut[i]));
   }

   return dataOut;
}

wstring ToLowerW(
   const wstring &data)
{
   wstring dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<wchar_t>(tolower(dataOut[i]));
   }

   return dataOut;
}

void MoveFile(
   const _tstring &filenameFrom,
   const _tstring &filenameTo)
{
   if (!::MoveFile(filenameFrom.c_str(), filenameTo.c_str()))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("MoveFile"), lastError);
   }
}

void CopyFile(
   const _tstring &filenameFrom,
   const _tstring &filenameTo,
   const bool failIfExists)
{
   if (!::CopyFile(filenameFrom.c_str(), filenameTo.c_str(), failIfExists))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("CopyFile"), lastError);
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
   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_WRITE,
      0,
      0,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      0));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveStringAsFile() \"") + filename + _T("\""), lastError);
   }

   DWORD bytesWritten = 0;

   if (!::WriteFile(hFile, data.c_str(), GetStringLength<DWORD>(data), &bytesWritten, 0))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveStringAsFile()"), _T("\"") + filename + _T("\""), lastError);
   }

   if (bytesWritten != data.length())
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveStringAsFile()"), _T("\"") + filename + _T("\" - Failed to write all data"), lastError);
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

   if (!::WriteFile(hFile, data.c_str(), GetStringLength<DWORD>(data), &bytesWritten, 0))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveStringToFile()"), _T("\"") + filename + _T("\""), lastError);
   }

   if (bytesWritten != data.length())
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveStringToFile()"), _T("\"") + filename + _T("\" - Failed to write all data"), lastError);
   }
}

__int64 GetFileSize(
   const _tstring &filename)
{
   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      0,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      0));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("GetFileSize() \"") + filename + _T("\""), lastError);
   }

   BY_HANDLE_FILE_INFORMATION info;

   if (!::GetFileInformationByHandle(hFile, &info))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("GetFileSize() - GetFileInformationByHandle \"") + filename + _T("\""), lastError);
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

   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      0,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      0));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsUnicodeString() \"") + filename + _T("\""), lastError);
   }

   BY_HANDLE_FILE_INFORMATION info;

   if (!::GetFileInformationByHandle(hFile, &info))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsUnicodeString() - GetFileInformationByHandle \"") + filename + _T("\""), lastError);
   }

   DWORD bytesRead = 0;

   DWORD totalBytesRead = 0;

   BYTE buffer[2];

   while (::ReadFile(hFile, buffer + totalBytesRead, sizeof(buffer) - totalBytesRead, &bytesRead, nullptr) && bytesRead > 0)
   {
      totalBytesRead += bytesRead;
   }

   if (totalBytesRead != 2)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsUnicodeString() - Initial 2 byte read failed \"") + filename + _T("\""), lastError);
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
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsUnicodeString() - We don't handle UCS-2 big-endian yet. \"") + filename + _T("\""), lastError);
   }
   else if (buffer[0] == 0xEF && buffer[1] == 0xBB)
   {
      // UTF-8 ?
      // EF BB BF

      sizeOfBOM = 3;
      fileIsUnicode = true;
      fileIsUTF8 = true;

      if (0 == ::ReadFile(hFile, buffer, 1, &bytesRead, nullptr))
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("LoadFileAsUnicodeString() - Failed to read 3rd byte in UTF-8 BOM. \"") + filename + _T("\""), lastError);
      }

      if (buffer[0] != 0xBF)
      {
         throw CException(_T("LoadFileAsUnicodeString()"), _T("Unexpected 3rd byte in UTF-8 BOM. \"") + filename + _T("\""));
      }
   }
   else
   {
      // bare ascii file, we need those 2 bytes...

      if (INVALID_SET_FILE_POINTER == ::SetFilePointer(hFile, 0, nullptr, FILE_BEGIN))
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("LoadFileAsUnicodeString() - Failed to seek to start of file. \"") + filename + _T("\""), lastError);
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

   while (::ReadFile(hFile, reinterpret_cast<void*>(const_cast<char *>(fileAsString.c_str()) + totalBytesRead), bufferSize - totalBytesRead, &bytesRead, 0) && bytesRead > 0)
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
   bool saveAsANSIifPossible)
{
   if (saveAsANSIifPossible && StringIsAllANSI(data))
   {
      SaveStringAsFile(filename, CStringConverter::WtoT(data));
   }

   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_WRITE,
      0,
      0,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      0));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveUnicodeStringAsFile() \"") + filename + _T("\""), lastError);
   }

   DWORD bytesWritten = 0;

   const BYTE header[] = { 0xFF, 0xFE };

   if (!::WriteFile(hFile, header, sizeof(header), &bytesWritten, nullptr))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveUnicodeStringAsFile() \"") + filename + _T("\""), lastError);
   }

   const DWORD dataToWrite = GetStringLength<DWORD>(data) * sizeof(wchar_t);

   if (!::WriteFile(hFile, data.c_str(), dataToWrite, &bytesWritten, 0))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveUnicodeStringAsFile() \"") + filename + _T("\""), lastError);
   }

   if (bytesWritten != dataToWrite)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveUnicodeStringAsFile() \"") + filename + _T("\" - Failed to write all data"), lastError);
   }
}

void LoadFileAsBinaryData(
   const _tstring &filename,
   TExpandableBuffer<BYTE> &buffer)
{
   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
      0,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      0));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsBinaryData() \"") + filename + _T("\""), lastError);
   }

   BY_HANDLE_FILE_INFORMATION info;

   if (!::GetFileInformationByHandle(hFile, &info))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsBinaryData() - GetFileInformationByHandle \"") + filename + _T("\""), lastError);
   }

   LARGE_INTEGER fileSize;

   fileSize.LowPart = info.nFileSizeLow;
   fileSize.HighPart = info.nFileSizeHigh;

   const size_t bufferSize = static_cast<size_t>(fileSize.QuadPart);

   buffer.Resize(bufferSize);

   DWORD bytesRead = 0;

   size_t totalBytesRead = 0;

   while (::ReadFile(
      hFile,
      static_cast<void*>(buffer.GetBuffer() + totalBytesRead),
      static_cast<DWORD>(std::min<size_t>(bufferSize - totalBytesRead, std::numeric_limits<DWORD>::max())),
      &bytesRead, 0) && bytesRead > 0)
   {
      totalBytesRead += bytesRead;
   }
}

void SaveBinaryDataAsFile(
   const _tstring &filename,
   const TExpandableBuffer<BYTE> &buffer)
{
   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_WRITE,
      0,
      0,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      0));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveBinaryDataAsFile() \"") + filename + _T("\""), lastError);
   }

   DWORD bytesWritten = 0;

   // Does not work with big files, change to use multiple writes

   if (!::WriteFile(hFile, buffer.GetBuffer(), static_cast<DWORD>(buffer.GetSize()), &bytesWritten, 0))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveBinaryDataAsFile() \"") + filename + _T("\""), lastError);
   }

   if (bytesWritten != buffer.GetSize())
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveBinaryDataAsFile() \"") + filename + _T("\" - Failed to write all data"), lastError);
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
   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      0,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      0));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsString() \"") + filename + _T("\""), lastError);
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

   if (!::GetFileInformationByHandle(hFile, &info))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsString() - GetFileInformationByHandle \"") + filename + _T("\""), lastError);
   }

   string fileAsString;

   if (info.nFileSizeHigh != 0 || info.nFileSizeLow > fileAsString.max_size())
   {
      throw CException(_T("LoadFileAsString()"), _T("File \"") + filename + _T("\" too big"));
   }

   const DWORD currentPosition = ::SetFilePointer(hFile, 0, nullptr, FILE_CURRENT);

   if (currentPosition == INVALID_SET_FILE_POINTER)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsString()"), _T("Failed to read file position for file \"") + filename + _T("\""), lastError);
   }

   const DWORD bufferSize = info.nFileSizeLow - currentPosition;

   fileAsString.resize(bufferSize);

   DWORD bytesRead = 0;

   DWORD totalBytesRead = 0;

   while (::ReadFile(
      hFile,
      reinterpret_cast<void*>(const_cast<char *>(fileAsString.c_str()) + totalBytesRead),
      bufferSize - totalBytesRead,
      &bytesRead, 0) &&
      bytesRead > 0)
   {
      totalBytesRead += bytesRead;
   }

   return fileAsString;
}

bool FileExists(
   const _tstring &filename)
{
   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      0,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      0));

   return hFile.IsValid();
}

_tstring FindAndReplace(
   const _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString,
   const size_t numReplacements)
{
   _tstring result = phrase;

   InPlaceFindAndReplace(result, findString, replaceString, numReplacements);

   return result;
}

bool InPlaceFindAndReplace(
   _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString,
   size_t numReplacements)
{
   bool replaced = false;

   _tstring::size_type pos = phrase.find(findString);

   while (pos != _tstring::npos && numReplacements > 0)
   {
      phrase.replace(pos, findString.length(), replaceString);

      replaced = true;

      pos = phrase.find(findString, pos + 1);

      if (numReplacements != INFINITE)
      {
         --numReplacements;
      }
   }

   return replaced;
}

string FindAndReplaceA(
   const string &phrase,
   const string &findString,
   const string &replaceString,
   const size_t numReplacements)
{
   string result = phrase;

   InPlaceFindAndReplaceA(result, findString, replaceString, numReplacements);

   return result;
}

bool InPlaceFindAndReplaceA(
   string &phrase,
   const string &findString,
   const string &replaceString,
   size_t numReplacements)
{
   bool replaced = false;

   string::size_type pos = phrase.find(findString);

   while (pos != _tstring::npos && numReplacements > 0)
   {
      phrase.replace(pos, findString.length(), replaceString);

      replaced = true;

      pos = phrase.find(findString, pos + 1);

      if (numReplacements != INFINITE)
      {
         --numReplacements;
      }
   }

   return replaced;
}

bool IsGoodReadPtr(
   const void *pv,
   ULONG cb)
{
   return IsGoodReadPtr(GetCurrentProcess(), pv, cb);
}

bool IsGoodReadPtr(
   HANDLE hProcess,
   const void *pv,
   ULONG cb)
{
   return IsGoodPtr(
      hProcess,
      pv,
      cb,
      PAGE_READONLY |
      PAGE_READWRITE |
      PAGE_WRITECOPY |
      PAGE_EXECUTE_READ |
      PAGE_EXECUTE_READWRITE |
      PAGE_EXECUTE_WRITECOPY);
}

bool IsGoodWritePtr(
   const void *pv,
   ULONG cb)
{
   return IsGoodWritePtr(GetCurrentProcess(), pv, cb);
}

bool IsGoodWritePtr(
   HANDLE hProcess,
   const void *pv,
   ULONG cb)
{
   return IsGoodPtr(
      hProcess,
      pv,
      cb,
      PAGE_READWRITE |
      PAGE_WRITECOPY |
      PAGE_EXECUTE_READWRITE |
      PAGE_EXECUTE_WRITECOPY);
}

_tstring GetTempFileName(
   const _tstring &pathName,
   const _tstring &prefixString,
   const unsigned int unique)
{
   TCHAR buffer[MAX_PATH + 1];

   if (0 == ::GetTempFileName(pathName.c_str(), prefixString.c_str(), unique, buffer))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("GetTempFileName()"), lastError);
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

      throw CWin32Exception(_T("GetTempPath()"), lastError);
   }

   _tstring directory;

   directory.resize(spaceRequired - 1);

   const DWORD spaceUsed = ::GetTempPath(spaceRequired, const_cast<TCHAR*>(directory.c_str()));

   if (spaceUsed == 0)
   {
      const DWORD lastError = GetLastError();

      throw CWin32Exception(_T("GetTempPath()"), lastError);
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

unsigned long GetLongFromString(
   const _tstring &numeric,
   const size_t startOffset,
   const size_t length)
{
   if (startOffset + length > numeric.length())
   {
      throw CException(_T("GetLongFromString()"),
         _T("Invalid offset (") + ToString(startOffset) + _T(") or length (") + ToString(length) + _T(") string is only ") + ToString(numeric.length()) + _T(" long"));
   }

   return _ttol(numeric.substr(startOffset, length).c_str());
}

unsigned short GetShortFromString(
   const _tstring &numeric,
   const size_t startOffset,
   const size_t length)
{
   return static_cast<unsigned short>(GetLongFromString(numeric, startOffset, length));
}

unsigned long GetLongFromStringA(
   const string &numeric,
   const size_t startOffset,
   const size_t length)
{
   if (startOffset + length > numeric.length())
   {
      throw CException(_T("GetLongFromString()"),
         _T("Invalid offset (") + ToString(startOffset) + _T(") or length (") + ToString(length) + _T(") string is only ") + ToString(numeric.length()) + _T(" long"));
   }

   return atol(numeric.substr(startOffset, length).c_str());
}

unsigned short GetShortFromStringA(
   const string &numeric,
   const size_t startOffset,
   const size_t length)
{
   return static_cast<unsigned short>(GetLongFromStringA(numeric, startOffset, length));
}

void WriteResourceToFile(
   HANDLE hFile,
   const _tstring &resourceName,
   const _tstring &resourceType,
   HMODULE hModule)
{
   HRSRC hResource = ::FindResource(hModule, resourceName.c_str(), resourceType.c_str());

   if (!hResource)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("WriteResourceToFile() - FindResource"), lastError);
   }

   HGLOBAL hGlobal = ::LoadResource(hModule, hResource);

   if (!hGlobal)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("WriteResourceToFile() - LoadResource"), lastError);
   }

   void *pData = ::LockResource(hGlobal);

   if (!pData)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("WriteResourceToFile() - LockResource"), lastError);
   }

   const DWORD bytes = ::SizeofResource(hModule, hResource);

   DWORD bytesWritten = 0;

   if (!::WriteFile(hFile, pData, bytes, &bytesWritten, nullptr))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("WriteResourceToFile() - WriteFile"), lastError);
   }

   if (bytesWritten != bytes)
   {
      throw CWin32Exception(_T("WriteResourceToFile() - WriteFile"), _T("Wrote less bytes (") + ToString(bytesWritten) + _T("( than expected: ") + ToString(bytes));
   }
}

_tstring GUIDAsString(
   const GUID &guid,
   const bool stripBrackets)
{
   wchar_t *pString = nullptr;

   HRESULT hr = ::StringFromIID(guid, &pString);

   if (FAILED(hr))
   {
      ::CoTaskMemFree(pString);

      throw CWin32Exception(_T("GUIDAoString() - StringFromIID"), hr);
   }

   _tstring result = CStringConverter::WtoT(pString);

   if (stripBrackets)
   {
      result = result.substr(1, result.length() - 2);
   }

   ::CoTaskMemFree(pString);

   return result;
}

_tstring CreateGUIDAsString(
   const bool stripBrackets)
{
   GUID guid;

   HRESULT hr = ::CoCreateGuid(&guid);

   if (FAILED(hr))
   {
      throw CWin32Exception(_T("CreateGUIDAsString() - CoCreateGuid"), hr);
   }

   return GUIDAsString(guid, stripBrackets);
}

_tstring TranslateDeviceNamePathToDriveLetterPath(
   const _tstring &deviceNamePath)
{
   _tstring driveStrings;

   driveStrings.resize(512);

   if (::GetLogicalDriveStrings(GetStringLength<DWORD>(driveStrings), const_cast<TCHAR *>(driveStrings.c_str())))
   {
      TCHAR name[MAX_PATH];
      TCHAR drive[3] = _T(" :");

      const TCHAR *pDriveLetter = driveStrings.c_str();

      do
      {
         drive[0] = *pDriveLetter;

         // Look up each device name
         if (::QueryDosDevice(drive, name, MAX_PATH))
         {
            const size_t nameLength = _tcslen(name);

            if (nameLength < MAX_PATH)
            {
               if (0 == _tcsnicmp(deviceNamePath.c_str(), name, nameLength))
               {
                  return drive + deviceNamePath.substr(nameLength);
               }
            }
         }

         while (*pDriveLetter++)   // Go to the next nullptr character.
         {
         }

      }
      while (*pDriveLetter); // end of string
   }

   return deviceNamePath;
}

_tstring GetFileNameFromHandleIfPossible(
   const HANDLE hFile)
{
   _tstring fileName;

   DWORD fileSizeHi = 0;
   const DWORD fileSizeLo = ::GetFileSize(hFile, &fileSizeHi);

   if (fileSizeLo != 0 || fileSizeHi != 0)
   {
      CSmartHandle hFileMapping(::CreateFileMapping(
         hFile,
         nullptr,
         PAGE_READONLY,
         0,
         1,
         nullptr));

      if (hFileMapping.IsValid())
      {
         void *pMem = ::MapViewOfFile(hFileMapping.GetHandle(), FILE_MAP_READ, 0, 0, 1);

         if (pMem)
         {
            TCHAR buffer[MAX_PATH+1];

            if (::GetMappedFileName(
               GetCurrentProcess(),
               pMem,
               buffer,
               MAX_PATH))
            {
               fileName = TranslateDeviceNamePathToDriveLetterPath(buffer);
            }

            ::UnmapViewOfFile(pMem);
         }
      }
   }

   return fileName;
}

_tstring GetFileNameFromHandle(
   const HANDLE hFile)
{
   _tstring fileName;

   DWORD fileSizeHi = 0;
   const DWORD fileSizeLo = ::GetFileSize(hFile, &fileSizeHi);

   if (fileSizeLo == 0 && fileSizeHi == 0)
   {
      throw CException(
         _T("GetFileNameFromHandle()"),
         _T("Cannot obtain file name from handle for a file with a length of zero."));
   }

   CSmartHandle hFileMapping(::CreateFileMapping(
      hFile,
      nullptr,
      PAGE_READONLY,
      0,
      1,
      nullptr));

   if (hFileMapping.IsValid())
   {
      void *pMem = ::MapViewOfFile(hFileMapping.GetHandle(), FILE_MAP_READ, 0, 0, 1);

      if (pMem)
      {
         TCHAR buffer[MAX_PATH+1];

         if (::GetMappedFileName(
            GetCurrentProcess(),
            pMem,
            buffer,
            MAX_PATH))
         {
            fileName = TranslateDeviceNamePathToDriveLetterPath(buffer);
         }
      }
      else
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("GetFileNameFromHandle()"), lastError);
      }

      ::UnmapViewOfFile(pMem);
   }

   return fileName;
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static bool StringIsAllANSI(
   const std::wstring &data)
{
   const BYTE *pData = reinterpret_cast<const BYTE *>(data.c_str());

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

static bool IsGoodPtr(
   HANDLE hProcess,
   const void *pv,
   ULONG cb,
   DWORD dwFlags)
{
   MEMORY_BASIC_INFORMATION meminfo;

   if (nullptr == pv)
   {
     return false;
   }

   memset(&meminfo, 0x00, sizeof meminfo);

   const SIZE_T dwSize = VirtualQueryEx(hProcess, pv, &meminfo, sizeof meminfo);

   // If pv is a kernel-mode address then this may return zero for security reasons.
   // In that event it is certainly NOT a valid read pointer.

   if (0 == dwSize)
   {
     return false;
   }

   if (MEM_COMMIT != meminfo.State)
   {
     return false;
   }

   if (0 == (meminfo.Protect & dwFlags))
   {
     return false;
   }

   if (cb > meminfo.RegionSize)
   {
      return false;
   }

   if (static_cast<unsigned>(static_cast<const char *>(pv) - static_cast<char *>(meminfo.BaseAddress)) > static_cast<unsigned>(meminfo.RegionSize - cb))
   {
      return false;
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Utils.cpp
///////////////////////////////////////////////////////////////////////////////
