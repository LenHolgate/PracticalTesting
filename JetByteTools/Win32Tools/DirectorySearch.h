#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: DirectorySearch.h
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

#include "JetByteTools/CoreTools/Types.h"

#include "JetByteTools/CoreTools/tstring.h"

#include <wtypes.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CDirectorySearch
///////////////////////////////////////////////////////////////////////////////

/// A simple class that wraps the
/// <a href="http://msdn2.microsoft.com/en-us/library/aa364418.aspx">FindFirstFile()</a>
/// /<a href="http://msdn2.microsoft.com/en-us/library/aa364428.aspx">FindNextFile()</a>
/// API.
/// \ingroup Win32

class CDirectorySearch : private WIN32_FIND_DATA
{
   public :

      /// Construct a directory search on the supplied filename

      explicit CDirectorySearch(
         const JetByteTools::Core::_tstring &filename);

      ~CDirectorySearch();

      /// True if the search contains another file.

      bool HasFile() const;

      /// True if the search contains another file and moves to the next file.

      bool NextFile();

      /// Access the file size.

      ULONGLONG GetFileSize() const;

      /// Access the file name.

      JetByteTools::Core::_tstring GetFileName() const;

      /// Access the file attributes.

      DWORD GetAttributes() const;

   private :

      HANDLE m_hFind;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: DirectorySearch.h
///////////////////////////////////////////////////////////////////////////////
