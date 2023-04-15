#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: Todo.h
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

/// \file Todo.h
/// This file provides some macros that allow pragma JETBYTE_TODO("message")
/// to give a clickable message in the build window from within Visual Studio.
/// Note that all of these messages only show up in debug builds.
/// \ingroup Admin

// To allow #pragma JETBYTE_TODO(blah blah) to give a 'clickable' message in the build
// window.

#define JETBYTE_TODOSTRINGIZE(L) #L
#define JETBYTE_TODOMAKESTRING(M,L) M(L)
#define JETBYTE_TODOLINE JETBYTE_TODOMAKESTRING( JETBYTE_TODOSTRINGIZE, __LINE__)
#define JETBYTE_MACROASSTRING(x) JETBYTE_TODOMAKESTRING( JETBYTE_TODOSTRINGIZE, x)

#if !defined(NDEBUG)

#if (JETBYTE_SHOW_TODO == 1)
/// JETBYTE_TODO(message) - leave yourself notes that show up in the build window
/// like warnings
#define JETBYTE_TODO(_msg) message(__FILE__ "(" JETBYTE_TODOLINE ") : TODO : " _msg)
#else
/// JETBYTE_TODO(message) - leave yourself notes that show up in the build window
/// like warnings
#define JETBYTE_TODO(x)
#endif // JETBYTE_SHOW_TODO

/// Don't document a bug with this, fix it!
#define JETBYTE_BUG(_msg) message(__FILE__ "(" JETBYTE_TODOLINE ") : BUG : " _msg)

/// Just a message
#define JETBYTE_MESSAGE(_msg) message(__FILE__ "(" JETBYTE_TODOLINE ") : " _msg)

/// Just an error
#define JETBYTE_STOP(_msg) error(__FILE__ "(" JETBYTE_TODOLINE ") : " _msg)

#else

#define JETBYTE_TODO(x)
#define JETBYTE_BUG(x)
#define JETBYTE_MESSAGE(_msg) message(__FILE__ "(" JETBYTE_TODOLINE ") : " _msg)
#define JETBYTE_STOP(_msg) error(__FILE__ "(" JETBYTE_TODOLINE ") : " _msg)

#endif // NDEBUG && JETBYTE_SHOW_TODO

///////////////////////////////////////////////////////////////////////////////
// End of file: Todo.h
///////////////////////////////////////////////////////////////////////////////

