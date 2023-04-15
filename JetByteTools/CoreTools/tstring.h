#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: tstring.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2002 JetByte Limited.
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

#include "JetByteTools/Admin/Platform.h"

#include <string>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
typedef std::wstring _tstring;
#else
typedef std::string _tstring;
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM

#ifndef __T
#define __T(x) L ## x
#endif

#ifndef _TCHAR_DEFINED
typedef wchar_t TCHAR;
#endif

#ifndef __TCHAR_DEFINED
typedef wchar_t     _TCHAR;
typedef wchar_t     _TSCHAR;
typedef wchar_t     _TUCHAR;
typedef wchar_t     _TXCHAR;
typedef wint_t      _TINT;
#define __TCHAR_DEFINED
#endif  /* __TCHAR_DEFINED */

#ifndef _TCHAR_DEFINED
#if !__STDC__
typedef wchar_t     TCHAR;
#endif  /* !__STDC__ */
#define _TCHAR_DEFINED
#endif  /* _TCHAR_DEFINED */
#else

#ifndef __T
#define __T(x) x
#endif

#ifndef _TCHAR_DEFINED
typedef char TCHAR;
#endif

#ifndef __TCHAR_DEFINED
typedef char            _TCHAR;
typedef signed char     _TSCHAR;
typedef unsigned char   _TUCHAR;
typedef unsigned char   _TXCHAR;
typedef unsigned int    _TINT;
#define __TCHAR_DEFINED
#endif  /* __TCHAR_DEFINED */

#ifndef _TCHAR_DEFINED
#if !__STDC__
typedef char     TCHAR;
#endif  /* !__STDC__ */
#define _TCHAR_DEFINED
#endif  /* _TCHAR_DEFINED */
#endif

#ifndef _T
#define _T(x)       __T(x)
#endif

#ifndef _TEXT
#define _TEXT(x)    __T(x)
#endif

///////////////////////////////////////////////////////////////////////////////
// End of file: tstring.h
///////////////////////////////////////////////////////////////////////////////

