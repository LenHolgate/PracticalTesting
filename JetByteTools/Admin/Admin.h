#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_ADMIN_ADMIN_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_ADMIN_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: Admin.h 
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
//
// JetByte Limited grants you ("Licensee") a non-exclusive, royalty free, 
// licence to use, modify and redistribute this software in source and binary 
// code form, provided that i) this copyright notice and licence appear on all 
// copies of the software; and ii) Licensee does not utilize the software in a 
// manner which is disparaging to JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All 
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors 
// shall not be liable for any damages suffered by licensee as a result of 
// using, modifying or distributing the software or its derivatives. In no
// event will JetByte Limited be liable for any lost revenue, profit or data,
// or for direct, indirect, special, consequential, incidental or punitive
// damages, however caused and regardless of the theory of liability, arising 
// out of the use of or inability to use software, even if JetByte Limited 
// has been advised of the possibility of such damages.
//
// This software is not designed or intended for use in on-line control of 
// aircraft, air traffic, aircraft navigation or aircraft communications; or in 
// the design, construction, operation or maintenance of any nuclear 
// facility. Licensee represents and warrants that it will not use or 
// redistribute the Software for such purposes. 
//
///////////////////////////////////////////////////////////////////////////////

#define _WIN32_WINNT 0x0400

//#define JETBYTE_SHOW_TODO

#define JETBYTE_USE_STL_PORT
// If you define JETBYTE_USE_STL_PORT then you should set the environment variable
// STLPORT_ROOT to point to, eg, C:\STLport-4.6.1

#ifdef JETBYTE_USE_STL_PORT
// STLPort needs new platform sdk definitions of interlocked functions
#define JETBYTE_ADMIN_NO_PLATFORM_SDK_REPLACEMENT
#endif

//#define JETBYTE_NO_PLATFORM_SDK
//#define JETBYTE_INCLUDE_DEPRECATED

//#define JETBYTE_SHOW_TODO

#include "Warnings.h"
#include "Todo.h"

#ifndef JETBYTE_ADMIN_NO_PLATFORM_SDK_REPLACEMENT
#include "NoPlatformSDK.h"
#endif //JETBYTE_ADMIN_NO_PLATFORM_SDK_REPLACEMENT

// Check that we're using the version of STL that we expect to be

#ifdef JETBYTE_USE_STL_PORT
#define _STLP_NEW_DONT_THROW 1

#include <stl/_config.h>

// if this include fails then there's a configuration mismatch.
// #define JB_USE_STL_PORT is set but the STLPort headers dont appear to be being used
// This will cause LOTS of warnings as the MS STL doesn't compile cleanly with warning
// level 4. Either set STLPORT_ROOT to point to the root of your STLPort installation
// (e.g. C:\\STLport-4.6.1) or comment out the #define of JB_USE_STL_PORT in Admin.h and
// use the standard Microsoft STL

#else

// Prevent wierd "some of it is inline some of it is not" linkage issues when using 
// "standard" Microsoft supplied STL

#define _WCTYPE_INLINE_DEFINED

#endif

#endif // JETBYTE_TOOLS_ADMIN_ADMIN_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Admin.h
///////////////////////////////////////////////////////////////////////////////
