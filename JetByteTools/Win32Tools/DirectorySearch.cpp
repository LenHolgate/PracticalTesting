///////////////////////////////////////////////////////////////////////////////
// File: DirectorySearch.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2003 JetByte Limited.
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

#include "DirectorySearch.h"
#include "Win32Exception.h"

#include "JetByteTools/CoreTools/ExceptionLeakPrevention.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Core::_tstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CDirectorySearch
///////////////////////////////////////////////////////////////////////////////

CDirectorySearch::CDirectorySearch(
   const _tstring &filename)
   :  WIN32_FIND_DATA{},
      m_hFind(::FindFirstFile(filename.c_str(), this))
{
   if (m_hFind == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = GetLastError();

      if (lastError != ERROR_FILE_NOT_FOUND)
      {
         throw CWin32Exception(_T("CDirectorySearch::CDirectorySearch(\"") + filename + _T("\")"), lastError);
      }
   }
}

CDirectorySearch::~CDirectorySearch()
{
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START

   if (m_hFind != INVALID_HANDLE_VALUE)
   {
      FindClose(m_hFind);
   }

   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END
}

bool CDirectorySearch::HasFile() const
{
   return (m_hFind != INVALID_HANDLE_VALUE);
}

bool CDirectorySearch::NextFile()
{
   if (HasFile() && !::FindNextFile(m_hFind, this))
   {
      const DWORD lastError = GetLastError();

      if (lastError == ERROR_NO_MORE_FILES)
      {
         auto const hFind = m_hFind;

         m_hFind = INVALID_HANDLE_VALUE;

         if (!FindClose(hFind))
         {
            throw CWin32Exception(_T("CDirectorySearch::NextFile() - FindClose"), GetLastError());
         }
      }
      else
      {
         throw CWin32Exception(_T("CDirectorySearch::NextFile() - FindNextFile"), lastError);
      }
   }

   return HasFile();
}

ULONGLONG CDirectorySearch::GetFileSize() const
{
   const ULONGLONG result = nFileSizeHigh * (static_cast<ULONGLONG>(MAXDWORD) + 1) + nFileSizeLow;

   return result;
}

_tstring CDirectorySearch::GetFileName() const
{
   return cFileName;
}

DWORD CDirectorySearch::GetAttributes() const
{
   return dwFileAttributes;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: DirectorySearch.cpp
///////////////////////////////////////////////////////////////////////////////
