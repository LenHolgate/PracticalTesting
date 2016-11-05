#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_ADMIN_WINSOCK_WRAPPER_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_WINSOCK_WRAPPER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: WinsockWrapper.h
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

/// \file JetByteTools\Admin\WinsockWrapper.h
/// There are various issues with using various early versions of the platform
/// SDK (or using VC6 and no platform SDK) with the winsock2.h header. This is
/// part of a crude attempt to avoid the problems. 
/// \ingroup Admin

#ifdef JETBYTE_PLATFORM_SDK_VERSION
#if JETBYTE_PLATFORM_SDK_VERSION >= 0x060A
#if (_MSC_VER < 1400)   // VC6, VS.Net (2002), VS.Net (2003)...
// The 6.0a and later versions of the Platform SDK includes a 'clever' version of 
// this macro that doesn't appear to work with earlier compilers
#ifndef _WSPIAPI_COUNTOF
#if JETBYTE_WARN_ON_WSPIAPI_COUNTOF_DEF == 1
#pragma JETBYTE_MESSAGE("Platform SDK " JETBYTE_MACROASSTRING(JETBYTE_PLATFORM_SDK_VERSION) " and pre VS2005 - defining old style _WSPIAPI_COUNTOF macro")
#endif
#define _WSPIAPI_COUNTOF(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif
#endif
#endif
#endif

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#ifdef JETBYTE_PLATFORM_SDK_VERSION
#if JETBYTE_PLATFORM_SDK_VERSION >= 0x060A
// The 6.0a and later versions of the Platform SDK does not define a SOCKADDR_STORAGE typedef 
// for versions of windows less than 0x0501... Other releases of the Platform SDK
// do (I think)... So, if you're using that particular combination you need to make sure that 
// you've set the JETBYTE_PLATFORM_SDK_VERSION correctly to say so, in which case
// we will define one for you.
#if _WIN32_WINNT < 0x0501
#if JETBYTE_WARN_ON_SOCKADDR_STORAGE_DEF == 1
#pragma JETBYTE_MESSAGE("_WIN32_WINNT < 0x0501 and Platform SDK " JETBYTE_MACROASSTRING(JETBYTE_PLATFORM_SDK_VERSION) " - defining SOCKADDR_STORAGE")
#endif
typedef SOCKADDR_STORAGE_XP SOCKADDR_STORAGE;
typedef SOCKADDR_STORAGE *PSOCKADDR_STORAGE, FAR *LPSOCKADDR_STORAGE;

#endif
#endif
#endif

#endif // JETBYTE_TOOLS_ADMIN_WINSOCK_WRAPPER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: WinsockWrapper.h
///////////////////////////////////////////////////////////////////////////////
