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

#ifdef _DEBUG
   #define _STLP_DEBUG 
   #define _STLP_USE_DEBUG_LIB 
#endif

#pragma warning(push, 4)

#pragma warning(disable: 4097)   // typedef-name 'x' used as synonym for class-name 'y'
#pragma warning(disable: 4302)   // 'reinterpret_cast' : truncation from 'const void *' to 'unsigned long'
#pragma warning(disable: 4800)   // 'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4686)   // 'stlp_std::_Is_POD' : possible change in behavior, change in UDT return calling convention

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

// Things we need to disable to be able to compile with /Wall

#pragma warning(disable: 4548)   // expression before comma has no effect; expected expression with side-effect
#pragma warning(disable: 4820)   // 'x' bytes padding added after data member 'y'
#pragma warning(disable: 4265)   // class has virtual functions, but destructor is not virtual
#pragma warning(disable: 4668)   // 'x' is not defined as a preprocessor macro, replaceing with '0' for '#if/#elif'
#pragma warning(disable: 4619)   // #pragma warning : there is no warning number 'x'
#pragma warning(disable: 4571)   // catch(...) semantics changed since VC 7.1; SEH are no longer caught
#pragma warning(disable: 4917)   // 'x' : a GUID can only be associated with a class, interface or namespace
#pragma warning(disable: 4365)   // conversion from 'x' to 'y', signed/unsigned mismatch
#pragma warning(disable: 4640)   // 'x' : construction of local static object is not thread-safe
#pragma warning(disable: 4625)   // 'x' : copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626)   // 'x' : assignment operator could not be generated because a base class assignment operator is inaccessible

// This is needed for VS2005 without any platform sdk, basestd.h is full of these..

#pragma warning(disable: 4826)   // Conversion from 'x' to 'y' is sign-extended. This may cause unexpected runtime behavior.
#pragma warning(disable: 4191)   // type cast' : unsafe conversion from 'x' to 'y' Calling this function through the result pointer may cause your program to fail

#pragma warning(disable: 4370)   // layout of class has changed from a previous version of the compiler due to better packing

#if _MSC_VER == 1600
// This is a problem in crtdbg.h ...
#pragma warning(disable: 4986)   // 'x': exception specification does not match previous declaration
#endif

#pragma warning(disable: 4574)   // 'x' is defined to be '0': did you mean to use '#if x'?

//#if JETBYTE_PLATFORM_SDK_VERSION >= 0x070A
// In atlconv.h, atlalloc.h, etc
#pragma warning(disable: 4987)   // nonstandard extension used: 'throw (...)'
//#endif

#endif // JETBYTE_TOOLS_ADMIN_WARNINGS_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Warnings.h
///////////////////////////////////////////////////////////////////////////////
