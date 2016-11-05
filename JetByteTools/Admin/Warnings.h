#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_ADMIN_WARNINGS_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_WARNINGS_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: Warnings.h 
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
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

/// \file Warnings.h
/// This file deals with warning level setting and masking of known safe 
/// warnings for either STLPort or native Microsoft STL. It's automatically
/// included in all files because they include Admin.h
/// \ingroup Admin
/// \ingroup Warnings

#if (JETBYTE_USE_STL_PORT == 1)

#if (_MSC_VER < 1300)			// Need this for anything earlier than VS.Net 2003
#define _STLP_NEW_PLATFORM_SDK
#endif

#define _REENTRANT 1

#ifdef _DEBUG
   #define _STLP_DEBUG 
   #define _STLP_USE_DEBUG_LIB 
#endif

#pragma warning(push, 4)

#pragma warning(disable: 4097)   // typedef-name 'x' used as synonym for class-name 'y'
#pragma warning(disable: 4800)   // 'int' : forcing value to bool 'true' or 'false' (performance warning)

#else // !JETBYTE_USE_STL_PORT

#if (_MSC_VER < 1300)   // VC6...
// Native Microsoft STL - can't use warning level 4 as the stl headers do all 
// kinds of crap things.

#pragma warning(push, 3)

// And in fstream...
#pragma warning(disable: 4701)   // local variable '_C' may be used without having been initialized

#endif // _MSC_VER

#endif // JETBYTE_USE_STL_PORT

#pragma warning(disable: 4201)   // nonstandard extension used : nameless struct/union

#pragma warning(disable: 4503)   // decorated name length exceeded, name was truncated

#pragma warning(disable: 4505)   // unreferenced local function has been removed

#pragma warning(disable: 4510)   // default constructor could not be generated
#pragma warning(disable: 4511)   // copy constructor could not be generated
#pragma warning(disable: 4512)   // assignment operator could not be generated
#pragma warning(disable: 4514)   // unreferenced inline function has been removed
#pragma warning(disable: 4610)   // can never be instantiated - user defined constructor required

#pragma warning(disable: 4702)   // unreachable code
#pragma warning(disable: 4710)   // function 'x' not inlined
#pragma warning(disable: 4786)   // identifier was truncated to '255' characters in the debug information

#endif // JETBYTE_TOOLS_ADMIN_WARNINGS_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Warnings.h
///////////////////////////////////////////////////////////////////////////////
