///////////////////////////////////////////////////////////////////////////////
// File: Win32Exception.cpp
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

#include "JetByteTools/Admin/Admin.h"

#include "Win32Exception.h"
#include "GetLastErrorMessage.h"

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
// CWin32Exception
///////////////////////////////////////////////////////////////////////////////

CWin32Exception::CWin32Exception(
   const _tstring &where,
   const DWORD error)
   :  CErrorCodeException(where, GetLastErrorMessage(error, true), error)
{
}

CWin32Exception::CWin32Exception(
   const _tstring &where,
   HMODULE hModule,
   const DWORD error)
   :  CErrorCodeException(where, GetLastErrorMessage(hModule, error, true), error)
{
}

CWin32Exception::CWin32Exception(
   const _tstring &where,
   const _tstring &message)
   :  CErrorCodeException(where, message, NO_ERROR)
{
}

CWin32Exception::CWin32Exception(
   const _tstring &where,
   const _tstring &message,
   const DWORD error)
   :  CErrorCodeException(where, message + _T(" - ") + GetLastErrorMessage(error, true), error)
{
}

CWin32Exception::CWin32Exception(
   const _tstring &where,
   const _tstring &message,
   HMODULE hModule,
   const DWORD error)
   :  CErrorCodeException(where, message + _T(" - ") + GetLastErrorMessage(hModule, error, true), error)
{
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Win32Exception.cpp
///////////////////////////////////////////////////////////////////////////////
