#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_ADMIN_DETECT_PLATFORM_SDK_VERSION_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_DETECT_PLATFORM_SDK_VERSION_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: DetectPlatformSDKVersion.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2009 JetByte Limited.
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

/// \file DetectPlatformSDKVersion.h
/// \ingroup Admin
/// This file attempts to automatically detect the Windows SDK version that
/// is installed.
/// Note that it only works if a Windows SDK is actually installed as it requires
/// that sdkddkver.h exists.

#include <sdkddkver.h>

// We COULD include mscoree.h and check the CLR version defines and determine
// if we have 7.0A installed (CLR major == 4) but that involves pulling in
// a LOT of headers to get mscoree.h to compile and every file we build will
// need to compile those headers...

#if defined(_WIN32_WINNT_WINBLUE)
#define JETBYTE_PLATFORM_SDK_VERSION 0x0810
#elif defined(_WIN32_WINNT_WIN8)
#define JETBYTE_PLATFORM_SDK_VERSION 0x0800
#elif defined(_WIN32_WINNT_WIN7)
#define JETBYTE_PLATFORM_SDK_VERSION 0x0700
#elif defined(_WIN32_WINNT_WIN6)
#define JETBYTE_PLATFORM_SDK_VERSION 0x0610
#elif defined(_WIN32_WINNT_WINXP)
#define JETBYTE_PLATFORM_SDK_VERSION 0x060A
#else
#error Failed to detect Windows SDK
#endif

#endif // JETBYTE_TOOLS_ADMIN_DETECT_PLATFORM_SDK_VERSION_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: DetectPlatformSDKVersion.h
///////////////////////////////////////////////////////////////////////////////
