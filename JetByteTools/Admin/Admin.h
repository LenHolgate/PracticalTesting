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

/// \file Admin.h
/// This file must be the first file included by all cpp files that are built 
/// in any library in the JetByte Tools suite.
/// This file contains global <code>##defines</code> that affect how the code 
/// is built.
/// \ingroup Admin

#define JETBYTE_TOOLS_VERSION 5.2.3

/// This is the minimum Windows version that we support
/// You might get away with earlier versions but we dont support it.
/// Note that at present:
/// * DebugTools needs NTDDI_WIN2KSP1
/// * APIHookTools needs _WIN32_WINNT_WINXP
/// * Everything else would be happy with NTDDI_WIN2K.

#define JETBYTE_MINIMUM_SUPPORTED_WINDOWS_VERSION  0x0500      // _WIN32_WINNT_WIN2K
#define JETBYTE_MINIMUM_SUPPORTED_NTDDI_VERSION    0x05000000  // NTDDI_WIN2K

/// If you want to set different values for _WIN32_WINNT and NTDDI_VERSION you should
/// do so in TargetWindowsVersion.h

#include "TargetWindowsVersion.h"

/// Check that we support the windows version that the user requires

#ifndef _WIN32_WINNT
#define _WIN32_WINNT JETBYTE_MINIMUM_SUPPORTED_WINDOWS_VERSION
#else 
#if _WIN32_WINNT < JETBYTE_MINIMUM_SUPPORTED_WINDOWS_VERSION
#error Unsupported Windows Version: _WIN32_WINNT must be larger than or equal to JETBYTE_MINIMUM_SUPPORTED_WINDOWS_VERSION
#endif
#endif

#ifndef NTDDI_VERSION
#define NTDDI_VERSION JETBYTE_MINIMUM_SUPPORTED_NTDDI_VERSION
#else 
#if NTDDI_VERSION < JETBYTE_MINIMUM_SUPPORTED_NTDDI_VERSION
#error Unsupported Windows Version: NTDDI_VERSION must be larger than or equal to JETBYTE_MINIMUM_SUPPORTED_NTDDI_VERSION
#endif
#endif

/// Don't let Windows.h define macros for min and max. Force the use of the stl
/// template versions

#define NOMINMAX

/// If you would rather build with STLPort than the standard STL that ships with 
/// Visual Studio then change the 0 to a 1 on the next line
/// Note that we don't support building with VC6 with the standard STL as it has
/// too many bugs if unpatched.
/// If you define JETBYTE_USE_STL_PORT then you should set the environment variable
/// STLPORT_ROOT to point to, eg, C:\STLport-5.0.0

#define JETBYTE_USE_STL_PORT 0

#if (JETBYTE_USE_STL_PORT == 1)
#ifdef _WIN64
#error STLPort on x64 is currently an untested configuration
#endif
#endif

/// We don't support building with VC6 with the standard STL as it has too many bugs 
/// if unpatched and, more's the pity, there's an ambiguity when trying to compile
/// the ToString() template for __int64, so, the define below forces us to use STLPort
/// even if you've set JETBYTE_USE_STL_PORT to 0... 
/// Note that changing the value of JETBYTE_FORCE_STL_PORT_FOR_VC6 to 0 and 
/// insisting on setting JETBYTE_USE_STL_PORT to 0 will display an error message and 
/// terminate the build. If you STILL want to try this build then you need to comment 
/// out the #error line below... And then, you're on your own...

#define JETBYTE_FORCE_STL_PORT_FOR_VC6 1

#if (_MSC_VER < 1300)  
#if (JETBYTE_USE_STL_PORT == 0)
#if (JETBYTE_FORCE_STL_PORT_FOR_VC6 == 1)
#undef JETBYTE_USE_STL_PORT
#define JETBYTE_USE_STL_PORT 1
#else
#error Unsupported configuration - VC6 and default STL is not supported - use a later compiler or STLPort
#endif
#endif
#endif

/// If you are building with STLPort then the way we work out that the settings are
/// all sane is to try and include an STLPort specific header file... Unfortunately
/// they moved them around at version 5.1.0 and so we now need this define so that
/// you can tell us the version that you're using so that we know where to look...
/// The version is in the form of a number, where version 5.1.3 would be represented
/// as 513 and version 5.0.0 would be represented as 500

#define JETBYTE_STL_PORT_VERSION 513

/// If you are using VC6 and you do NOT have a platform SDK installed then change
/// the following define from 0 to 1 and things might still build. Ideally install 
/// the latest platfor SDK as that's what all the code is tested with. We don't 
/// support building with VC6 without a platform SDK.
/// See <a href="http://en.wikipedia.org/wiki/Microsoft_Platform_SDK">here</a> for more 
/// details on how to get the latest copy of the Platform SDK.

#define JETBYTE_NO_PLATFORM_SDK 0

#if (JETBYTE_NO_PLATFORM_SDK == 1)
#if (_MSC_VER < 1300) 
#error Unsupported configuration - VC6 without Platform SDK is not supported - use a later compiler or the latest Platform SDK
#endif
#endif 

/// Sometimes there are bugs in versions of the Platform SDK (or, perhaps, things
/// we just don't quite understand how to correctly configure...). Rarely we work around
/// these. To be able to work around them we need to know which version of the 
/// Platform SDK is being used and the SDK itself cant tell us. So... You can define
/// JETBYTE_PLATFORM_SDK_VERSION with the following values:
/// 0x060A - v6.0a - Vista
/// 0x0610 - v6.1 - Windows Server 2008 and .Net Framework 3.5
/// At present we just fix the SOCKADDR_STORAGE bug for _WIN32_WINNT < 0x0501

//#define JETBYTE_PLATFORM_SDK_VERSION 0x0610

/// Note that we don't support building for Windows Vista or later (_WIN32_WINNT >= 0x0600)
/// without a platform SDK of v6.0a or later, as if _WIN32_WINNT >= 0x0600 then we 
/// assume that Vista specific functions are available in the Platform SDK headers.

#if (_WIN32_WINNT >= 0x0600)
#ifndef JETBYTE_PLATFORM_SDK_VERSION
#error Unsupported configuration - _WIN32_WINNT >= 0x0600 and JETBYTE_PLATFORM_SDK_VERSION not defined.
#elif JETBYTE_PLATFORM_SDK_VERSION < 0x0600
#error Unsupported configuration - _WIN32_WINNT >= 0x0600 and JETBYTE_PLATFORM_SDK_VERSION < 0x600.
#endif
#endif

/// Sometimes we make design mistakes. Some clients still depend on code that we'd
/// like to get rid of. If you'd like to include deprecated code in a library then
/// set JETBYTE_ENABLE_DEPRECATED to 1, else set it to 0.
#define JETBYTE_ENABLE_DEPRECATED 0

// don't ask... 
#if (JETBYTE_ENABLE_DEPRECATED == 1)
#define JETBYTE_INCLUDE_DEPRECATED
#endif

/// If you're building with _WIN32_WINNT >= 0x0502 and you know that you will be
/// running on a system that supports the Interlocked64() functions then you can
/// set the following define to 1 and we'll use them. Otherwise we'll cludge around
/// the lack of these 64bit operations with critical sections. Note that you can
/// always compile with _WIN32_WINNT >= 0x0502 and all will be well but if the 
/// functions arent exported from Kernel32.dll on the machine that you run on
/// then the process will die at runtime!

#if (_WIN32_WINNT >= 0x0502)
#ifndef JETBYTE_HAS_INTERLOCKED_64           // try and do the right thing if nobody 
#ifdef _WIN64                                // bothers to manually configure this...
#define JETBYTE_HAS_INTERLOCKED_64 1
#else
#define JETBYTE_HAS_INTERLOCKED_64 0
#endif
#endif
#endif

/// If you want to see all of the 'fix this poor design' and 'what about this case?' 
/// messages then set the JETBYTE_SHOW_TODO define to 1, else set it to 0.

#define JETBYTE_SHOW_TODO 1

#include "Warnings.h"
#include "Todo.h"
#include "NoPlatformSDK.h"

// Check that we're using the version of STL that we expect to be

#if (JETBYTE_USE_STL_PORT == 1)

#include <string>

#ifdef _CPPLIB_VER
#error Unexpected, JETBYTE_USE_STL_PORT == 1 and _CPPLIB_VER defined?
#endif

#ifdef _WIN64
#pragma MESSAGE("STLPort on x64 is currently an untested configuration")
#endif


#define _STLP_NEW_DONT_THROW 1

// If the following include fails then there's a configuration mismatch.
// #define JB_USE_STL_PORT is set but the STLPort headers dont appear to be being used
// This will cause LOTS of warnings as the MS STL doesn't compile cleanly with warning
// level 4. Either set STLPORT_ROOT to point to the root of your STLPort installation
// (e.g. C:\\STLport-4.6.1) or comment out the #define of JB_USE_STL_PORT in Admin.h and
// use the standard Microsoft STL

#if (JETBYTE_STL_PORT_VERSION < 500)
#include <stl/_config.h>
#else
#include <stl/_stlport_version.h>
#endif

#else

// Prevent wierd "some of it is inline some of it is not" linkage issues when using 
// "standard" Microsoft supplied STL

#define _WCTYPE_INLINE_DEFINED

#endif

#if (_MSC_VER < 1300)   // VC6...

// If the following include fails then you need to copy the sal.h file from
// Admin\VC6Compatibility\sal.h to the Visual C++ 6 or VS.Net (2002) or
// VS 2003 include directory.

// Version 6.1 of the Platform SDK requires that <sal.h> exists. It is shipped as
// part of all later versions of Visual Studio, but VC6, VS.Net (2002) and VS 2003 
// are no longer officially supported, it seems...

#include <sal.h>

#endif

#endif // JETBYTE_TOOLS_ADMIN_ADMIN_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Admin.h
///////////////////////////////////////////////////////////////////////////////
