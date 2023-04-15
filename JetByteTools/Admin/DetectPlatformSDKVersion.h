#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: DetectPlatformSDKVersion.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2009 JetByte Limited.
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

/// \file DetectPlatformSDKVersion.h
/// \ingroup Admin
/// This file attempts to automatically detect the Windows SDK version that
/// is installed.
/// Note that it only works if a Windows SDK is actually installed as it requires
/// that sdkddkver.h exists.

#if JETBYTE_HAS_INCLUDE(<sdkddkver.h>)
#include <sdkddkver.h>

// We COULD include mscoree.h and check the CLR version defines and determine
// if we have 7.0A installed (CLR major == 4) but that involves pulling in
// a LOT of headers to get mscoree.h to compile and every file we build will
// need to compile those headers...

#if defined(_WIN32_WINNT_WIN10)
#define JETBYTE_PLATFORM_SDK_VERSION 0x0A0A
#elif defined(_WIN32_WINNT_WINBLUE)
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
#else
#error Failed to detect Windows SDK
#endif

///////////////////////////////////////////////////////////////////////////////
// End of file: DetectPlatformSDKVersion.h
///////////////////////////////////////////////////////////////////////////////
