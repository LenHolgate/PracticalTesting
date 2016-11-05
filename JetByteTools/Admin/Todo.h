#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_ADMIN_TODO_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_TODO_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: Todo.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2003 JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All 
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors 
// shall not be liable for any damages suffered by licensee as a result of 
// using the software. In no event will JetByte Limited be liable for any 
// lost revenue, profit or data, or for direct, indirect, special, 
// consequential, incidental or punitive damages, however caused and regardless 
// of the theory of liability, arising out of the use of or inability to use 
// software, even if JetByte Limited has been advised of the possibility of 
// such damages.
//
///////////////////////////////////////////////////////////////////////////////

/// \file Todo.h
/// This file provides some macros that allow #pragma TODO("message") to give
/// a clickable message in the build window from within Visual Studio. 
/// Note that all of these messages only show up in debug builds.
/// \ingroup Admin

// To allow #pragma TODO(blah blah) to give a 'clickable' message in the build
// window.

#define TODOSTRINGIZE(L) #L
#define TODOMAKESTRING(M,L) M(L)
#define TODOLINE TODOMAKESTRING( TODOSTRINGIZE, __LINE__) 
#define MACROASSTRING(x) TODOMAKESTRING( TODOSTRINGIZE, x) 

#if !defined(NDEBUG)

#if (JETBYTE_SHOW_TODO == 1)
/// TODO(message) - leave yourself notes that show up in the build window
/// like warnings
#define TODO(_msg) message(__FILE__ "(" TODOLINE ") : TODO : " _msg)
#else 
/// TODO(message) - leave yourself notes that show up in the build window
/// like warnings
#define TODO(x)
#endif // JETBYTE_SHOW_TODO

/// Don't document a bug with this, fix it!
#define BUG(_msg) message(__FILE__ "(" TODOLINE ") : BUG : " _msg)

/// Just a message
#define MESSAGE(_msg) message(__FILE__ "(" TODOLINE ") : " _msg)

/// Just an error
#define STOP(_msg) error(__FILE__ "(" TODOLINE ") : " _msg)

#else

#define TODO(x)
#define BUG(x)
#define MESSAGE(_msg) message(__FILE__ "(" TODOLINE ") : " _msg)
#define STOP(_msg) error(__FILE__ "(" TODOLINE ") : " _msg)

#endif // NDEBUG && JETBYTE_SHOW_TODO

#endif // JETBYTE_TOOLS_ADMIN_TODO_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Todo.h
///////////////////////////////////////////////////////////////////////////////

