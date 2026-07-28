#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: PlatformAdmin.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2026 JetByte Limited.
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

/// This is the minimum Windows version that we support
/// You might get away with earlier versions but we don't support it.

#define JETBYTE_MINIMUM_SUPPORTED_WINDOWS_VERSION  0x0600      // _WIN32_WINNT_VISTA
#define JETBYTE_MINIMUM_SUPPORTED_NTDDI_VERSION    0x06000000  // NTDDI_VISTA

/// This is the latest Windows version that we've tested on.
/// You might get away with later versions but we don't support them yet.

#define JETBYTE_LATEST_TESTED_WINDOWS_VERSION      0x0A00      // WIN10
#define JETBYTE_LATEST_TESTED_NTDDI_VERSION        0x0A000000  // WIN10

/// This is the minimum compiler version that we support.
/// You might get away with earlier versions but we don't support them.
/// Note that 5.2.3 was the last version to support Visual Studio 6.
/// Note that 6.5.9 was the last version to support Visual Studio .NET (2002).
/// Note that 6.5.9 was the last version to support Visual Studio .NET (2003).
/// Note that 6.6.5 was the last version to support Visual Studio 2005.
/// Note that 6.6.5 was the last version to support Visual Studio 2008.
/// Note that 6.7.x was the last version to support Visual Studio 2010.
/// Note that 6.8.x was the last version to support Visual Studio 2012.
/// Note that 6.9.2 was the last version to support Visual Studio 2013.
/// Note that 7.3 was the last version to support Visual Studio 2015.
/// Note that 7.4 was the last version to support Visual Studio 2017.
/// Note that 7.5 was the last version to support Visual Studio 2019.

#define JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION 1920

/// This is the latest version of the compiler we will support from the
/// next major release

#define JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION 1920

/// This is the latest compiler version that we've tested on.
/// You might get away with later versions but, we don't support them yet.

#define JETBYTE_LATEST_TESTED_COMPILER_VERSION 1951

// JETBYTE_ALLOW_UNTESTED_COMPILE_ENV
// Define as 1 to suppress the errors for building with a later compile environment than
// we have tested with.

// This is only needed when the main version number, _MSC_VER, doesn't get
// bumped for breaking changes. As was the case with the 2019 16.9 previews
// which still reported as _MSCV_VER 1928 
//#define JETBYTE_LATEST_TESTED_FULL_COMPILER_VERSION 192929917

/// Don't let Windows.h define macros for min and max. Force the use of the stl
/// template versions

#define NOMINMAX

// You need to create a TargetWindowsVersion.h file in the JetByteTools\Admin directory
// before you can compile the code. Example TargetWindowsVersion.h files can be found
// in the ExampleConfigHeaders directory.

#include "JetByteTools/Admin/TargetWindowsVersion.h"

// Fix up optional defines from TargetWindowsVersion.h

#ifndef _WIN32_WINNT
#define _WIN32_WINNT JETBYTE_MINIMUM_SUPPORTED_WINDOWS_VERSION
#endif

#ifndef NTDDI_VERSION
#define NTDDI_VERSION JETBYTE_MINIMUM_SUPPORTED_NTDDI_VERSION
#endif

// Try and do the right thing in case nobody bothers to manually configure these.
// They can be set either in TargetWindowsVersion.h or in Config.h...

#ifndef JETBYTE_HAS_SRW_LOCK_TRY_ENTER
#if (_WIN32_WINNT >= 0x0601)
#define JETBYTE_HAS_SRW_LOCK_TRY_ENTER 1
#else
#define JETBYTE_HAS_SRW_LOCK_TRY_ENTER 0
#endif
#endif

#ifndef JETBYTE_NO_SUPPRESS_WINSOCK_HEADERS
#define _WINSOCKAPI_          // NEVER include winsock 1.0 header
#endif

#define JETBYTE_TOOLS_ADMIN_SUPPORTS_WIDE_STRING 1

///////////////////////////////////////////////////////////////////////////////
// End of file: PlatformAdmin.h
///////////////////////////////////////////////////////////////////////////////
