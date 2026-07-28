///////////////////////////////////////////////////////////////////////////////
// File: LibraryLoader.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2004 JetByte Limited.
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

#include "LibraryLoader.h"
#include "Win32Exception.h"

#include "JetByteTools/CoreTools/Exception.h"
#include "JetByteTools/CoreTools/StringConverter.h"
#include "JetByteTools/CoreTools/DebugTrace.h"
#include "JetByteTools/CoreTools/ExceptionLeakPrevention.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Core::CException;
using JetByteTools::Core::_tstring;
using JetByteTools::Core::CStringConverter;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CLibraryLoader
///////////////////////////////////////////////////////////////////////////////

CLibraryLoader::CLibraryLoader()
   :  m_hModule(nullptr)
{
}

CLibraryLoader::CLibraryLoader(
   const _tstring &fileName,
   const bool failWithException)
   :  m_hModule(nullptr)
{
   if (!LoadIfPossible(fileName) && failWithException)
   {
      throw CWin32Exception(_T("CLibraryLoader::CLibraryLoader() - \"") + fileName + _T("\""), GetLastError());
   }
}

CLibraryLoader::CLibraryLoader(
   const HMODULE hModule)
   :  m_hModule(hModule)
{
   if (nullptr == m_hModule)
   {
      throw CException(_T("CLibraryLoader::CLibraryLoader()"), _T("Invalid module handle"));
   }
}

CLibraryLoader::~CLibraryLoader()
{
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START

   ::FreeLibrary(m_hModule);

   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END
}

bool CLibraryLoader::IsLoaded() const
{
   return m_hModule != nullptr;
}

HMODULE CLibraryLoader::GetHMODULE() const
{
   return m_hModule;
}

void CLibraryLoader::FreeLibrary()
{
   if (m_hModule != nullptr)
   {
      if (!::FreeLibrary(m_hModule))
      {
         throw CWin32Exception(_T("CLibraryLoader::FreeLibrary()"), GetLastError());
      }

      m_hModule = nullptr;
   }
}

ULONG_PTR CLibraryLoader::GetProcAddressOffset(
   const _tstring &functionName) const
{
   FARPROC proc = GetProcAddress(functionName);

   return reinterpret_cast<ULONG_PTR>(proc) - reinterpret_cast<ULONG_PTR>(m_hModule);
}

FARPROC CLibraryLoader::GetProcAddressInThisModule(
   const _tstring &functionName,
   HMODULE hModule) const
{
   const ULONG_PTR offset = GetProcAddressOffset(functionName);

   return reinterpret_cast<FARPROC>(reinterpret_cast<ULONG_PTR>(hModule) + offset);
}

void CLibraryLoader::LoadLibrary(
   const _tstring &fileName,
   const bool run)
{
   if (!LoadIfPossible(fileName, run))
   {
      throw CWin32Exception(_T("CLibraryLoader::LoadLibrary() - \"") + fileName + _T("\""), GetLastError());
   }
}

bool CLibraryLoader::LoadIfPossible(
   const _tstring &fileName,
   const bool run)
{
   FreeLibrary();

   DWORD flags = LOAD_WITH_ALTERED_SEARCH_PATH;

   if (!run)
   {
      flags |= DONT_RESOLVE_DLL_REFERENCES;
   }

   m_hModule = ::LoadLibraryEx(fileName.c_str(), nullptr, flags);

   return (nullptr != m_hModule);
}

FARPROC CLibraryLoader::GetProcAddress(
   const _tstring &functionName) const
{
   auto const procAddress = GetOptionalProcAddress(functionName);

   if (!procAddress)
   {
      throw CWin32Exception(_T("CLibraryLoader::GetProcAddress() - \"") + functionName + _T("\""), GetLastError());
   }

   return procAddress;
}

FARPROC CLibraryLoader::GetOptionalProcAddress(
   const _tstring &functionName) const
{
   return ::GetProcAddress(m_hModule, CStringConverter::TtoA(functionName).c_str());
}

bool CLibraryLoader::IsValidProcAddress(
   const _tstring &functionName) const
{
   return (nullptr != ::GetProcAddress(m_hModule, CStringConverter::TtoA(functionName).c_str()));
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: LibraryLoader.cpp
///////////////////////////////////////////////////////////////////////////////
