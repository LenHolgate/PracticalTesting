///////////////////////////////////////////////////////////////////////////////
// File: SystemUtils.cpp
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

#include "JetByteTools/CoreTools/ToBool.h"
#include "JetByteTools/CoreTools/ToString.h"
#include "JetByteTools/CoreTools/Exception.h"

#include "SystemUtils.h"
#include "Win32Exception.h"

#pragma hdrstop

#include <Psapi.h>   // GetModuleFileNameEx
#include <lmcons.h>  // UNLEN

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Core::_tstring;
using JetByteTools::Core::ToString;
using JetByteTools::Core::ToBool;
using JetByteTools::Core::CException;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

_tstring GetUserName()
{
   _tstring name = _T("UNAVAILABLE");

   TCHAR userName[UNLEN + 1]{};
   DWORD userNameLen = UNLEN;

   if (::GetUserName(userName, &userNameLen))
   {
      name = userName;
   }

   return name;
}

_tstring GetComputerName()
{
   TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1]{};
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
   TCHAR moduleFileName[MAX_PATH + 1]{};
   constexpr DWORD moduleFileNameLen = MAX_PATH;

   const bool ok = ToBool(::GetModuleFileNameEx(hProcess, hModule, moduleFileName, moduleFileNameLen));

   if (ok)
   {
      name = moduleFileName;
   }

   return ok;
}

_tstring GetModuleFileName(
   HINSTANCE hModule)
{
   _tstring name = _T("UNAVAILABLE");

   TCHAR moduleFileName[MAX_PATH + 1]{};
   constexpr DWORD moduleFileNameLen = MAX_PATH;

   if (::GetModuleFileName(hModule, moduleFileName, moduleFileNameLen))
   {
      name = moduleFileName;
   }

   return name;
}

_tstring GetModulePathName(
   HINSTANCE hModule)
{
   const _tstring path = GetModuleFileName(hModule);

   const _tstring::size_type pos = path.find_last_of(_T("\\/:"));

   return path.substr(0, pos);
}

bool Is64bitSystem()
{
#if defined(_WIN64)

   return true;  // 64-bit programs run only on Win64

#elif defined(_WIN32)

   bool is64bit = false;

   typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

   const auto pfnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(::GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process"));

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

bool IsWow64Process()
{
#if defined(_WIN64)

   return false;  // 64-bit programs run natively on Win64

#elif defined(_WIN32)

   bool is64bit = false;

   typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

   const auto pfnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(::GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process"));

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

   return true;  // Compiled as a 32-bit program

#endif
}

_tstring GetSystemWow64Directory()
{
   TCHAR buffer[MAX_PATH + 1]{};

   const DWORD bufferLen = MAX_PATH;

   typedef UINT (WINAPI *LPFN_GETSYSTEMWOW64DIRECTORY) (LPTSTR, UINT);

#ifdef UNICODE
   const LPCSTR pFunctionName = "GetSystemWow64DirectoryW";
#else
   const LPCSTR pFunctionName = "GetSystemWow64DirectoryA";
#endif

   if (const auto pfnGetSystemWow64Directory = reinterpret_cast<LPFN_GETSYSTEMWOW64DIRECTORY>(GetProcAddress(::GetModuleHandle(_T("kernel32")), pFunctionName)))
   {
      const UINT result = (pfnGetSystemWow64Directory)(buffer, bufferLen);

      if (result == 0)
      {
         const DWORD lastError = GetLastError();

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

_tstring GetSystemWindowsDirectory()
{
   TCHAR buffer[MAX_PATH + 1]{};

   constexpr DWORD bufferLen = MAX_PATH;

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
   TCHAR buffer[MAX_PATH + 1]{};

   constexpr DWORD bufferLen = MAX_PATH;

   const UINT result = ::GetSystemDirectory(buffer, bufferLen);

   if (result == 0)
   {
      const DWORD lastError = GetLastError();

      throw CWin32Exception(_T("GetSystemDirectory()"), lastError);
   }

   if (result > bufferLen)
   {
      throw CException(_T("GetSystemDirectory()"), _T("System directory is more than: ") + ToString(bufferLen) + _T(" bytes long..."));
   }

   return buffer;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: SystemUtils.cpp
///////////////////////////////////////////////////////////////////////////////
