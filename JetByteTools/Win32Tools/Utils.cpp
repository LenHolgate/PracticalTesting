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
#include "CriticalSection.h"
#include "SmartHandle.h"

#include "ExpandableBuffer.h"

#pragma hdrstop

#include "JetByteTools\Admin\SecureCRT.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include <Psapi.h>

#include "Lmcons.h"     // UNLEN

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "psapi.lib")

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
   void *pv, 
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
   else if (stringRepresentation == _T("0"))
   {
      return false;
   }
   else if (0 == _tcsncicmp(stringRepresentation.c_str(), _T("TRUE"), stringRepresentation.length()))
   {
      return true;
   }
   else if (0 == _tcsncicmp(stringRepresentation.c_str(), _T("FALSE"), stringRepresentation.length()))
   {
      return false;
   }

   throw CException(_T("StringToBool()"), _T("Can't convert: \"") + stringRepresentation + _T("\" to a bool value"));
}

DWORD GetStringLengthAsDWORD(
   const string &theString,
   const bool includeNullTerminator)
{
#if JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION >= 1400
#error Compiler specific code that is no longer required
#endif

#if (_MSC_VER < 1400)
   const size_t length = theString.length() + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<size_t>(std::numeric_limits<DWORD>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<DWORD>(length);
#else
   return GetStringLength<DWORD>(theString, includeNullTerminator);
#endif
}

DWORD GetStringLengthAsDWORD(
   const wstring &theString,
   const bool includeNullTerminator)
{
#if JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION >= 1400
#error Compiler specific code that is no longer required
#endif

#if (_MSC_VER < 1400)
   const size_t length = theString.length() + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<size_t>(std::numeric_limits<DWORD>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<DWORD>(length);
#else
   return GetStringLength<DWORD>(theString, includeNullTerminator);
#endif
}

DWORD GetStringLengthAsDWORD(
   const char *pString,
   const bool includeNullTerminator)
{
#if JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION >= 1400
#error Compiler specific code that is no longer required
#endif
 
#if (_MSC_VER < 1400)
   const size_t length = strlen(pString) + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<size_t>(std::numeric_limits<DWORD>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<DWORD>(length);
#else
   return GetStringLength<DWORD>(pString, includeNullTerminator);
#endif
}

DWORD GetStringLengthAsDWORD(
   const wchar_t *pString,
   const bool includeNullTerminator)
{
#if JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION >= 1400
#error Compiler specific code that is no longer required
#endif

#if (_MSC_VER < 1400)
   const size_t length = wcslen(pString) + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<size_t>(std::numeric_limits<DWORD>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<DWORD>(length);
#else
   return GetStringLength<DWORD>(pString, includeNullTerminator);
#endif
}

int GetStringLengthAsInt(
   const string &theString,
   const bool includeNullTerminator)
{
#if JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION >= 1400
#error Compiler specific code that is no longer required
#endif

#if (_MSC_VER < 1400)
   const size_t length = theString.length() + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<size_t>(std::numeric_limits<int>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<int>(length);
#else
   return GetStringLength<int>(theString, includeNullTerminator);
#endif
}

int GetStringLengthAsInt(
   const wstring &theString,
   const bool includeNullTerminator)
{
#if JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION >= 1400
#error Compiler specific code that is no longer required
#endif

#if (_MSC_VER < 1400)
   const size_t length = theString.length() + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<size_t>(std::numeric_limits<int>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<int>(length);
#else
	return GetStringLength<int>(theString, includeNullTerminator);
#endif
}

int GetStringLengthAsInt(
   const char *pString,
   const bool includeNullTerminator)
{
#if JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION >= 1400
#error Compiler specific code that is no longer required
#endif

#if (_MSC_VER < 1400)
   const size_t length = strlen(pString) + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<size_t>(std::numeric_limits<int>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<int>(length);
#else
	return GetStringLength<int>(pString, includeNullTerminator);
#endif
}

int GetStringLengthAsInt(
   const wchar_t *pString,
   const bool includeNullTerminator)
{
#if JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION >= 1400
#error Compiler specific code that is no longer required
#endif

#if (_MSC_VER < 1400)
   const size_t length = wcslen(pString) + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<size_t>(std::numeric_limits<int>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<int>(length);
#else
   return GetStringLength<int>(pString, includeNullTerminator);
#endif
}

UINT GetStringLengthAsUInt(
   const string &theString,
   const bool includeNullTerminator)
{
#if JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION >= 1400
#error Compiler specific code that is no longer required
#endif

#if (_MSC_VER < 1400)
   const size_t length = theString.length() + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<size_t>(std::numeric_limits<UINT>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<UINT>(length);
#else
   return GetStringLength<UINT>(theString, includeNullTerminator);
#endif
}

UINT GetStringLengthAsUInt(
   const wstring &theString,
   const bool includeNullTerminator)
{
#if JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION >= 1400
#error Compiler specific code that is no longer required
#endif

#if (_MSC_VER < 1400)
   const size_t length = theString.length() + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<size_t>(std::numeric_limits<UINT>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<UINT>(length);
#else
   return GetStringLength<UINT>(theString, includeNullTerminator);
#endif
}

UINT GetStringLengthAsUInt(
   const char *pString,
   const bool includeNullTerminator)
{
#if JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION >= 1400
#error Compiler specific code that is no longer required
#endif

#if (_MSC_VER < 1400)
   const size_t length = strlen(pString) + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<size_t>(std::numeric_limits<UINT>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<UINT>(length);
#else
	return GetStringLength<UINT>(pString, includeNullTerminator);
#endif
}

UINT GetStringLengthAsUInt(
   const wchar_t *pString,
   const bool includeNullTerminator)
{
#if JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION >= 1400
#error Compiler specific code that is no longer required
#endif

#if (_MSC_VER < 1400)
   const size_t length = wcslen(pString) + (includeNullTerminator ? 1 : 0);

   if (length > static_cast<size_t>(std::numeric_limits<UINT>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<UINT>(length);
#else
   return GetStringLength<UINT>(pString, includeNullTerminator);
#endif
}

bool IsAllDigits(
   const _tstring &numeric)
{
   bool ok = true;

   for (_tstring::const_iterator it = numeric.begin(); ok && it != numeric.end(); ++it)
   {
      ok = ToBool(_istdigit(*it));
   }

   return ok;
}

_tstring GetLastErrorMessageIfPossible(
   DWORD last_error,
   bool stripTrailingLineFeed)
{
   TCHAR errmsg[512];

   if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
      0,
      last_error,
      0,
      errmsg, 
      511,
      NULL))
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
      0,
      last_error,
      0,
      errmsg, 
      511,
      NULL))
   {
      // if we fail, call ourself to find out why and return that error 

      const DWORD thisError = ::GetLastError();

      if (thisError != last_error)
      {
         return GetLastErrorMessage(thisError, stripTrailingLineFeed);
      }
      else
      {
         // But don't get into an infinite loop...

         return _T("Failed to obtain error string: ") + ToString(last_error);
      }
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
      0,
      last_error,
      0,
      errmsg, 
      511,
      NULL))
   {
      // if we fail, call ourself to find out why and return that error 

      const DWORD thisError = ::GetLastError();

      if (thisError != last_error)
      {
         return GetLastErrorMessageA(thisError, stripTrailingLineFeed);
      }
      else
      {
         // But don't get into an infinite loop...

         return "Failed to obtain error string: " + ToStringA(last_error);
      }
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
      0,
      last_error,
      0,
      errmsg, 
      511,
      NULL))
   {
      // if we fail, call ourself to find out why and return that error 

      const DWORD thisError = ::GetLastError();

      if (thisError != last_error)
      {
         return GetLastErrorMessageW(thisError, stripTrailingLineFeed);
      }
      else
      {
         // But don't get into an infinite loop...

         return L"Failed to obtain error string: " + ToStringW(last_error);
      }
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

      BYTE val = 0;

      const BYTE b = s[stringOffset];

      if (isdigit(b)) 
      {
         val = static_cast<BYTE>((b - '0') * 16); 
      }
      else 
      {
         val = static_cast<BYTE>(((toupper(b) - 'A') + 10) * 16); 
      }

      const BYTE b1 = s[stringOffset + 1];

      if (isdigit(b1)) 
      {
         val = static_cast<BYTE>(val + b1 - '0'); 
      }
      else 
      {
         val = static_cast<BYTE>(val + (toupper(b1) - 'A') + 10); 
      }

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

void CreateDirectoryIfRequired(
   const _tstring &directory)
{
   if (!::CreateDirectory(directory.c_str(), 0))
   {
      const DWORD lastError = ::GetLastError();

      if (lastError != ERROR_ALREADY_EXISTS)
      {
         throw CWin32Exception(_T("CreateDirectoryIfRequired()"), lastError);
      }
   }
}

void CreateDirectoriesIfRequired(
   const _tstring &directory)
{
   if (!::CreateDirectory(directory.c_str(), 0))
   {
      const DWORD lastError = ::GetLastError();

      if (lastError == ERROR_PATH_NOT_FOUND)
      {
         const _tstring::size_type pos = directory.find_last_of(_T("\\"));

         if (pos != _tstring::npos)
         {
            const _tstring previousDirectory = directory.substr(0, pos);

            CreateDirectoriesIfRequired(previousDirectory);   

            // We use 'if required' here because we might get passed a path with a
            // trailing \ which means we will try and create "lastDirectory" and then
            // "lastDirectory\"...

            CreateDirectoryIfRequired(directory);
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
}

_tstring GetCurrentDirectory()
{
   DWORD size = ::GetCurrentDirectory(0, 0);

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


_tstring GetDateStamp()
{
   SYSTEMTIME systime;
   GetLocalTime(&systime);

   TCHAR buffer[7];

   _stprintf_s(buffer, _T("%02d%02d%02d"),
                     systime.wDay,
                     systime.wMonth,
                     systime.wYear % 100);

   return buffer;
}

_tstring GetTimeStamp()
{
   SYSTEMTIME systime;
   GetLocalTime(&systime);

   TCHAR buffer[14];

   _stprintf_s(buffer, _T("%02d:%02d:%02d.%04d"),
                     systime.wHour,
                     systime.wMinute,
                     systime.wSecond,
                     systime.wMilliseconds);

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
      "%02d:%02d:%02d.%04d",
      systime.wHour,
      systime.wMinute,
      systime.wSecond,
      systime.wMilliseconds);

   return buffer;
}

_tstring GetComputerName()
{
   static bool gotName = false;

   static _tstring name = _T("UNAVAILABLE");

   if (!gotName)
   {
      TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1] ;
      DWORD computerNameLen = MAX_COMPUTERNAME_LENGTH ;

      if (::GetComputerName(computerName, &computerNameLen))
      {
         name = computerName;
      }

      gotName = true;
   }

   return name;
}

_tstring GetModuleFileName(
   HANDLE hProcess,
   HINSTANCE hModule)
{
   _tstring name = _T("UNAVAILABLE");

   GetModuleFileName(hProcess, hModule, name);

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

   _tstring::size_type pos = path.find_last_of(_T("\\/:"));

   return path.substr(0, pos);
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
   else if (result > bufferLen)
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
   else if (result > bufferLen)
   {
      throw CException(_T("GetSystemDirectory()"), _T("System directory is more than: ") + ToString(bufferLen) + _T(" bytes long..."));
   }

   return buffer;
}

_tstring GetFileNameFromPathName(
   const _tstring &pathName)
{
   _tstring fileName;

   const _tstring::size_type pos = pathName.find_last_of(_T("\\"));

   if (pos != _tstring::npos)
   {
      fileName = pathName.substr(pos + 1);
   }   

   return fileName;
}

_tstring StripFileNameFromPathName(
   const _tstring &pathName)
{
   _tstring strippedPathName;

   const _tstring::size_type pos = pathName.find_last_of(_T("\\"));

   if (pos != _tstring::npos)
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

#pragma warning(push)
#pragma warning(disable: 4191)   // type cast' : unsafe conversion from 'x' to 'y' Calling this function through the result pointer may cause your program to fail

   static LPFN_ISWOW64PROCESS s_fnIsWow64Process = (LPFN_ISWOW64PROCESS)::GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");

#pragma warning(pop)

   // 32-bit programs run on both 32-bit and 64-bit Windows
   // so must sniff

   if (s_fnIsWow64Process)
   {
      BOOL f64 = FALSE;

      if (!s_fnIsWow64Process(::GetCurrentProcess(), &f64))
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

#pragma warning(push)
#pragma warning(disable: 4191)   // type cast' : unsafe conversion from 'x' to 'y' Calling this function through the result pointer may cause your program to fail

   static LPFN_ISWOW64PROCESS s_fnIsWow64Process = (LPFN_ISWOW64PROCESS)::GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");

#pragma warning(pop)

   // 32-bit programs run on both 32-bit and 64-bit Windows
   // so must sniff

   if (s_fnIsWow64Process)
   {
      BOOL f64 = FALSE;

      if (!s_fnIsWow64Process(::GetCurrentProcess(), &f64))
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
#if defined(_WIN32)

   return true ;  // Compiled as a 32-bit program

#else 

   return false;

#endif
}


#if _WIN32_WINNT >= 0x0501 || defined(WINBASE_DECLARE_GET_SYSTEM_WOW64_DIRECTORY)

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

#pragma warning(push)
#pragma warning(disable: 4191)   // type cast' : unsafe conversion from 'x' to 'y' Calling this function through the result pointer may cause your program to fail

   static LPFN_GETSYSTEMWOW64DIRECTORY s_fnGetSystemWow64Directory = (LPFN_GETSYSTEMWOW64DIRECTORY)::GetProcAddress(::GetModuleHandle(_T("kernel32")), pFunctionName);

#pragma warning(pop)

   if (s_fnGetSystemWow64Directory)
   {
      const UINT result = (s_fnGetSystemWow64Directory)(buffer, bufferLen);

      if (result == 0)
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("GetSystemWow64Directory()"), lastError);
      }
      else if (result > bufferLen)
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

#else

_tstring GetSystemWow64Directory()
{
   throw CException(_T("GetSystemWow64Directory()"), _T("Functionality not available on this platform"));
}

#endif // _WIN32_WINNT >= 0x0501 || defined(WINBASE_DECLARE_GET_SYSTEM_WOW64_DIRECTORY)

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

#pragma comment(lib, "Version.lib")

_tstring GetFileVersion()
{
   _tstring version;

   const _tstring moduleFileName = GetModuleFileName(NULL);

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

         if (::VerQueryValue(buffer, 
              const_cast<LPTSTR>(_T("\\StringFileInfo\\080904b0\\ProductVersion")), 
              (void**)&pVersion, 
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

      throw CWin32Exception(_T("SaveStringAsFile() \"") + filename + _T("\""), lastError);
   }

   if (bytesWritten != data.length())
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveStringAsFile() \"") + filename + _T("\" - Failed to write all data"), lastError);
   }
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

   while (::ReadFile(hFile, buffer + totalBytesRead, sizeof(buffer) - totalBytesRead, &bytesRead, 0) && bytesRead > 0)
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

      if (0 == ::ReadFile(hFile, buffer, 1, &bytesRead, 0))
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("LoadFileAsUnicodeString() - Failed to read 3rd byte in UTF-8 BOM. \"") + filename + _T("\""), lastError);
      }

      totalBytesRead += bytesRead;

      if (buffer[0] != 0xBF)
      {
         throw CException(_T("LoadFileAsUnicodeString()"), _T("Unexpected 3rd byte in UTF-8 BOM. \"") + filename + _T("\""));
      }
   }
   else
   {
      // bare ascii file, we need those 2 bytes...

      if (INVALID_SET_FILE_POINTER == ::SetFilePointer(hFile, 0, 0, FILE_BEGIN))
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

   while (::ReadFile(hFile, (void*)(fileAsString.c_str() + totalBytesRead), bufferSize - totalBytesRead, &bytesRead, 0) && bytesRead > 0)
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
      else
      {
         return wstring(reinterpret_cast<const wchar_t*>(fileAsString.c_str()), bufferSize / sizeof(wchar_t));
      }
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

   if (!::WriteFile(hFile, header, sizeof(header), &bytesWritten, 0))
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
      FILE_SHARE_READ,
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
      (void*)(buffer.GetBuffer() + totalBytesRead), 
	  static_cast<DWORD>(std::min<size_t>(bufferSize - totalBytesRead, std::numeric_limits<DWORD>::max())), 
      &bytesRead, 0) && bytesRead > 0)
   {
      totalBytesRead += bytesRead;
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
   
   const DWORD bufferSize = info.nFileSizeLow;

   fileAsString.resize(bufferSize);

   DWORD bytesRead = 0;

   DWORD totalBytesRead = 0;

   while (::ReadFile(hFile, (void*)(fileAsString.c_str() + totalBytesRead), bufferSize - totalBytesRead, &bytesRead, 0) && bytesRead > 0)
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
   const _tstring &replaceString)
{
   _tstring result = phrase;

   InPlaceFindAndReplace(result, findString, replaceString);

   return result;
}

void InPlaceFindAndReplace(
   _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString)
{
   _tstring::size_type pos = phrase.find(findString);

   while (pos != _tstring::npos)
   {
      phrase.replace(pos, findString.length(), replaceString);
   
      pos = phrase.find(findString, pos + 1);
   }
}

bool IsGoodReadPtr(
   void *pv, 
   ULONG cb)
{
   return IsGoodReadPtr(GetCurrentProcess(), pv, cb);
}

bool IsGoodReadPtr(
   HANDLE hProcess,
   void *pv, 
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
   void *pv, 
   ULONG cb)
{
   return IsGoodWritePtr(GetCurrentProcess(), pv, cb);
}

bool IsGoodWritePtr(
   HANDLE hProcess,
   void *pv, 
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
   const DWORD spaceRequired = ::GetTempPath(0, 0);

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

   if (spaceUsed > spaceRequired)
   {
      throw CException(
         _T("GetTempPath()"), 
         _T("Failed to get temp path, second call needed more space ") + ToString(spaceUsed) + 
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

   if (!::WriteFile(hFile, pData, bytes, &bytesWritten,0))
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
   wchar_t *pString = 0;

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
   const _tstring deviceNamePath)
{
   _tstring driveStrings;

   driveStrings.resize(512);

   if (::GetLogicalDriveStrings(GetStringLengthAsDWORD(driveStrings), const_cast<TCHAR *>(driveStrings.c_str()))) 
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
         
         while (*pDriveLetter++);   // Go to the next NULL character.

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
   const DWORD fileSizeLo = GetFileSize(hFile, &fileSizeHi); 

   if (fileSizeLo != 0 || fileSizeHi != 0)
   {
      CSmartHandle hFileMapping(::CreateFileMapping(
         hFile, 
         NULL, 
         PAGE_READONLY,
         0, 
         1,
         NULL));

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
   const DWORD fileSizeLo = GetFileSize(hFile, &fileSizeHi); 

   if (fileSizeLo == 0 && fileSizeHi == 0)
   {
      throw CException(
         _T("GetFileNameFromHandle()"), 
         _T("Cannot obtain file name from handle for a file with a length of zero."));
   }

   CSmartHandle hFileMapping(::CreateFileMapping(
      hFile, 
      NULL, 
      PAGE_READONLY,
      0, 
      1,
      NULL));

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

   bool allANSI = false;

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
   void *pv, 
   ULONG cb, 
   DWORD dwFlags)
{
   MEMORY_BASIC_INFORMATION meminfo;

   if (NULL == pv)
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

   if ((unsigned)((char *)pv - (char *)meminfo.BaseAddress) > (unsigned)(meminfo.RegionSize - cb))
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
