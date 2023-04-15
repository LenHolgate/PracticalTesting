#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ErrorCodeException.h
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

#include "Exception.h"
#include "Types.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// CErrorCodeException
///////////////////////////////////////////////////////////////////////////////

/// A simple exception class.
/// \ingroup Exceptions

class CErrorCodeException : public CException
{
   public :

      #ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
      CErrorCodeException(
         const std::string &where,
         const std::string &message,
         DWORD errorCode);

      CErrorCodeException(
         const std::string &where,
         DWORD errorCode);
      #endif

      CErrorCodeException(
         const _tstring &where,
         const _tstring &message,
         DWORD errorCode);

      CErrorCodeException(
         const _tstring &where,
         DWORD errorCode);

      ~CErrorCodeException() override = default;

      virtual DWORD GetError() const;

   protected :

      const DWORD m_errorCode;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ErrorCodeException.h
///////////////////////////////////////////////////////////////////////////////
