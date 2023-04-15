///////////////////////////////////////////////////////////////////////////////
// File: ErrorCodeException.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2021 JetByte Limited.
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

#include "ErrorCodeException.h"
#include "ErrorCodeToErrorMessage.h"

#include "StringConverter.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::string;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// CErrorCodeException
///////////////////////////////////////////////////////////////////////////////

#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
CErrorCodeException::CErrorCodeException(
   const string &where,
   const string &message,
   const DWORD errorCode)
   :  CException(where, message),
      m_errorCode(errorCode)
{
}

CErrorCodeException::CErrorCodeException(
   const string &where,
   DWORD errorCode)
   :  CException(where, ErrorCodeToErrorMessageA(errorCode)),
      m_errorCode(errorCode)
{
}
#endif

CErrorCodeException::CErrorCodeException(
   const _tstring &where,
   const _tstring &message,
   const DWORD errorCode)
   :  CException(where, message),
      m_errorCode(errorCode)
{
}

CErrorCodeException::CErrorCodeException(
   const _tstring &where,
   const DWORD errorCode)
   :  CException(where, ErrorCodeToErrorMessage(errorCode)),
      m_errorCode(errorCode)
{
}

DWORD CErrorCodeException::GetError() const
{
   return m_errorCode;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ErrorCodeException.cpp
///////////////////////////////////////////////////////////////////////////////
