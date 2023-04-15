#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: Win32Exception.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 1997 JetByte Limited.
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
#include "JetByteTools/CoreTools/ErrorCodeException.h"

#include <Windows.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CWin32Exception
///////////////////////////////////////////////////////////////////////////////

/// A simple exception class to replace Win32 error returns.
/// \ingroup Exceptions

class CWin32Exception : public Core::CErrorCodeException
{
   public :

      CWin32Exception(
         const Core::_tstring &where,
         DWORD error);

      CWin32Exception(
         const Core::_tstring &where,
         HMODULE hModule,
         DWORD error);

      CWin32Exception(
         const Core::_tstring &where,
         const Core::_tstring &message);

      CWin32Exception(
         const Core::_tstring &where,
         const Core::_tstring &message,
         DWORD error);

      CWin32Exception(
         const Core::_tstring &where,
         const Core::_tstring &message,
         HMODULE hModule,
         DWORD error);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Win32Exception.h
///////////////////////////////////////////////////////////////////////////////
