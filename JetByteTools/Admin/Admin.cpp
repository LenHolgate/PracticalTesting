///////////////////////////////////////////////////////////////////////////////
// File: Admin.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2007 JetByte Limited.
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

#include "JetByteTools/Admin/Admin.h"
#include "JetByteTools/Admin/CompilerName.h"

#pragma hdrstop

#pragma JETBYTE_MESSAGE(" Build configuration:---------------------------------------------------------------")

#ifdef JETBYTE_TOOLS_VERSION
#pragma JETBYTE_MESSAGE(" Build configuration: JetByte Tools Library Version = " JETBYTE_MACROASSTRING(JETBYTE_TOOLS_VERSION))
#else
#pragma JETBYTE_MESSAGE(" Build configuration: JetByte Tools Library Version = UNSET")
#endif

#ifdef JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION
#pragma JETBYTE_MESSAGE(" Build configuration: Minimum supported compiler version = " JETBYTE_MACROASSTRING(JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION))
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Minimum supported compiler version = UNSET")

#ifdef JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION
#if (JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION != JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION)
#pragma JETBYTE_MESSAGE(" Build configuration: Minimum non deprecated compiler version = " JETBYTE_MACROASSTRING(JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION))
#pragma JETBYTE_MESSAGE(" Build configuration: Deprecated compilers will become unsupported from the next release")
#endif
#endif
#endif

#ifdef _MSC_VER
#pragma JETBYTE_MESSAGE(" Build configuration: Compiler Version = " JETBYTE_MACROASSTRING(_MSC_VER) " - " JETBYTE_MACROASSTRING(JETBYTE_COMPILER_LONG_NAME_A))
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Compiler Version = UNSET")
#endif

#ifdef _MSC_FULL_VER
#pragma JETBYTE_MESSAGE(" Build configuration: Full Compiler Version = " JETBYTE_MACROASSTRING(_MSC_FULL_VER) " - " JETBYTE_MACROASSTRING(JETBYTE_COMPILER_LONG_NAME_A))
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Full Compiler Version = UNSET")
#endif

#ifdef _MSVC_LANG
#pragma JETBYTE_MESSAGE(" Build configuration: C++ Language Version = " JETBYTE_MACROASSTRING(_MSVC_LANG))
#else
#pragma JETBYTE_MESSAGE(" Build configuration: C++ Language Version = UNSET")
#endif

#pragma JETBYTE_MESSAGE(" Build configuration:")
#pragma JETBYTE_MESSAGE(" Build configuration: Target Platform can be adjusted by editing the values in Admin\\TargetWindowsVersion.h")
#pragma JETBYTE_MESSAGE(" Build configuration: Other options can be adjusted editing the values in Admin\\Config.h")
#pragma JETBYTE_MESSAGE(" Build configuration:")

#ifdef _WIN32_WINNT
#if (_WIN32_WINNT < JETBYTE_MINIMUM_SUPPORTED_WINDOWS_VERSION)
#pragma JETBYTE_MESSAGE(" **** Build configuration UNSUPPORTED ****")
#elif (_WIN32_WINNT == 0x0600)
#pragma JETBYTE_MESSAGE(" Build configuration: Target Platform = Windows Vista - (_WIN32_WINNT = " JETBYTE_MACROASSTRING(_WIN32_WINNT) ")")
#elif (_WIN32_WINNT == 0x0601)
#pragma JETBYTE_MESSAGE(" Build configuration: Target Platform = Windows 7 - (_WIN32_WINNT = " JETBYTE_MACROASSTRING(_WIN32_WINNT) ")")
#elif (_WIN32_WINNT == 0x0602)
#pragma JETBYTE_MESSAGE(" Build configuration: Target Platform = Windows 8 - (_WIN32_WINNT = " JETBYTE_MACROASSTRING(_WIN32_WINNT) ")")
#elif (_WIN32_WINNT == 0x0603)
#pragma JETBYTE_MESSAGE(" Build configuration: Target Platform = Windows 8.1 - (_WIN32_WINNT = " JETBYTE_MACROASSTRING(_WIN32_WINNT) ")")
#elif (_WIN32_WINNT == 0x0A00)
#pragma JETBYTE_MESSAGE(" Build configuration: Target Platform = Windows 10 - (_WIN32_WINNT = " JETBYTE_MACROASSTRING(_WIN32_WINNT) ")")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Target Platform = Unknown - (_WIN32_WINNT = " JETBYTE_MACROASSTRING(_WIN32_WINNT) ")")
#endif
#else
#pragma JETBYTE_MESSAGE(" Build configuration: _WIN32_WINNT = UNSET")
#endif

#ifdef NTDDI_VERSION
#if (NTDDI_VERSION < JETBYTE_MINIMUM_SUPPORTED_NTDDI_VERSION)
#pragma JETBYTE_MESSAGE(" **** Build configuration UNSUPPORTED ****")
#elif (NTDDI_VERSION == 0x06000000)
#pragma JETBYTE_MESSAGE(" Build configuration: NTDDI_VERSION = " JETBYTE_MACROASSTRING(NTDDI_VERSION) " (Windows Vista)")
#elif (NTDDI_VERSION == 0x06000100)
#pragma JETBYTE_MESSAGE("Build configuration: NTDDI_VERSION = " JETBYTE_MACROASSTRING(NTDDI_VERSION) " (Windows Vista SP1/Windows Server 2008)")
#elif (NTDDI_VERSION == 0x06000200)
#pragma JETBYTE_MESSAGE("Build configuration: NTDDI_VERSION = " JETBYTE_MACROASSTRING(NTDDI_VERSION) " (Windows Vista/Windows Server 2008 SP2)")
#elif (NTDDI_VERSION == 0x06000300)
#pragma JETBYTE_MESSAGE("Build configuration: NTDDI_VERSION = " JETBYTE_MACROASSTRING(NTDDI_VERSION) " (Windows Vista/Windows Server 2008 SP3)")
#elif (NTDDI_VERSION == 0x06000400)
#pragma JETBYTE_MESSAGE("Build configuration: NTDDI_VERSION = " JETBYTE_MACROASSTRING(NTDDI_VERSION) " (Windows Vista/Windows Server 2008 SP4)")
#elif (NTDDI_VERSION == 0x06010000)
#pragma JETBYTE_MESSAGE("Build configuration: NTDDI_VERSION = " JETBYTE_MACROASSTRING(NTDDI_VERSION) " (Windows 7)")
#elif (NTDDI_VERSION == 0x06020000)
#pragma JETBYTE_MESSAGE("Build configuration: NTDDI_VERSION = " JETBYTE_MACROASSTRING(NTDDI_VERSION) " (Windows 8)")
#elif (NTDDI_VERSION == 0x06030000)
#pragma JETBYTE_MESSAGE("Build configuration: NTDDI_VERSION = " JETBYTE_MACROASSTRING(NTDDI_VERSION) " (Windows 8.1)")
#elif (NTDDI_VERSION == 0x0A000000)
#pragma JETBYTE_MESSAGE("Build configuration: NTDDI_VERSION = " JETBYTE_MACROASSTRING(NTDDI_VERSION) " (Windows 10)")
#else
#pragma JETBYTE_MESSAGE("Build configuration: NTDDI_VERSION = " JETBYTE_MACROASSTRING(NTDDI_VERSION) " (Unknown)")
#endif

#else
#pragma JETBYTE_MESSAGE("Build configuration: NTDDI_VERSION = UNSET")
#endif

#ifdef JETBYTE_PLATFORM_SDK_VERSION

#if (JETBYTE_PLATFORM_SDK_VERSION == 0x0001)
#pragma JETBYTE_MESSAGE("Build configuration: Platform SDK: Using the Platform SDK that ships with the compiler")
#elif (JETBYTE_PLATFORM_SDK_VERSION == 0x0600)
#pragma JETBYTE_MESSAGE("Build configuration: Platform SDK: UNSUPPORTED Version 6.0 - Windows Server 2003 R2")
#elif (JETBYTE_PLATFORM_SDK_VERSION == 0x060A)
#pragma JETBYTE_MESSAGE("Build configuration: Platform SDK: Version 6.0a - Windows SDK Update for Windows Vista and .NET Framework 3.0")
#elif (JETBYTE_PLATFORM_SDK_VERSION == 0x0610)
#pragma JETBYTE_MESSAGE("Build configuration: Platform SDK: Version 6.1 - Windows Server 2008 and .Net Framework 3.5")
#elif (JETBYTE_PLATFORM_SDK_VERSION == 0x0700)
#pragma JETBYTE_MESSAGE("Build configuration: Platform SDK: Version 7.0 - Windows 7")
#elif (JETBYTE_PLATFORM_SDK_VERSION == 0x070A)
#pragma JETBYTE_MESSAGE("Build configuration: Platform SDK: Version 7.0a - Windows 7 and .Net Framework 4.0")
#elif (JETBYTE_PLATFORM_SDK_VERSION == 0x0800)
#pragma JETBYTE_MESSAGE("Build configuration: Platform SDK: Version 8.0 - Windows 8")
#elif (JETBYTE_PLATFORM_SDK_VERSION == 0x0810)
#pragma JETBYTE_MESSAGE("Build configuration: Platform SDK: Version 8.1 - Windows 8.1 and .Net Framework 4.5")
#elif (JETBYTE_PLATFORM_SDK_VERSION == 0xA0A)
#pragma JETBYTE_MESSAGE("Build configuration: Platform SDK: Version 10.0a - Windows 10 and .Net Framework 4.6")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Platform SDK: Version " JETBYTE_MACROASSTRING(JETBYTE_PLATFORM_SDK_VERSION) " (Unknown)")
#endif

#else

#pragma JETBYTE_MESSAGE("Build configuration: No Platform SDK. Using header and libs that came with your compiler.")
#pragma JETBYTE_MESSAGE("Build configuration: You can specify the version of the Platform SDK in use by defining JETBYTE_PLATFORM_SDK_VERSION")
#pragma JETBYTE_MESSAGE("Build configuration: See JETBYTE_PLATFORM_SDK_VERSION in Admin.h for details, we use it to work around some")
#pragma JETBYTE_MESSAGE("Build configuration: 'issues' in some of the versions of the Platform SDK.")

#endif

// Check that we support the compile environment.

// Check that we support the compiler.

#ifndef _MSC_VER
#error Unsupported compiler version: _MSC_VER not defined!
#else
#if _MSC_VER < JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION
#error Unsupported compiler version: _MSC_VER < JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION
#elif _MSC_VER > JETBYTE_LATEST_TESTED_COMPILER_VERSION
#if JETBYTE_ALLOW_UNTESTED_COMPILE_ENV == 0
#error Untested compiler version: _MSC_VER > JETBYTE_LATEST_TESTED_COMPILER_VERSION
#else
#pragma JETBYTE_MESSAGE("Build configuration: Untested compiler version: _MSC_VER (" JETBYTE_MACROASSTRING(_MSC_VER) ") > JETBYTE_LATEST_TESTED_COMPILER_VERSION")
#endif
#endif
#if _MSC_VER < JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION
#if JETBYTE_ALLOW_DEPRECATED_COMPILE_ENV == 0
#error Deprecated compiler version: _MSC_VER < JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION - unsupported from the next release.
// define JETBYTE_ALLOW_DEPRECATED_COMPILE_ENV to 1 in Config.h to continue to use this compiler version.
#else
#pragma JETBYTE_MESSAGE("Build configuration: Deprecated compiler version: _MSC_VER < JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION - unsupported from the next release")
#endif
#endif
#endif

/// Check that we support the windows version that the user requires

#if _WIN32_WINNT < JETBYTE_MINIMUM_SUPPORTED_WINDOWS_VERSION
#error Unsupported Windows Version: _WIN32_WINNT must be larger than or equal to JETBYTE_MINIMUM_SUPPORTED_WINDOWS_VERSION
#elif _WIN32_WINNT > JETBYTE_LATEST_TESTED_WINDOWS_VERSION
#if JETBYTE_ALLOW_UNTESTED_COMPILE_ENV == 0
#error Untested windows version: _WIN32_WINNT > JETBYTE_LATEST_TESTED_WINDOWS_VERSION
#else
#pragma JETBYTE_MESSAGE("Build configuration: Untested windows version: _WIN32_WINNT > JETBYTE_LATEST_TESTED_WINDOWS_VERSION")
#endif
#endif

#if NTDDI_VERSION < JETBYTE_MINIMUM_SUPPORTED_NTDDI_VERSION
#error Unsupported Windows Version: NTDDI_VERSION must be larger than or equal to JETBYTE_MINIMUM_SUPPORTED_NTDDI_VERSION
#elif NTDDI_VERSION > JETBYTE_LATEST_TESTED_NTDDI_VERSION
#if JETBYTE_ALLOW_UNTESTED_COMPILE_ENV == 0
#error Untested windows version: NTDDI_VERSION > JETBYTE_LATEST_TESTED_NTDDI_VERSION
#else
#pragma JETBYTE_MESSAGE("Build configuration: Untested windows version: NTDDI_VERSION > JETBYTE_LATEST_TESTED_NTDDI_VERSION")
#endif
#endif

#ifndef JETBYTE_PLATFORM_SDK_VERSION
#error Unsupported configuration - _WIN32_WINNT >= 0x0600 and JETBYTE_PLATFORM_SDK_VERSION not defined.
#endif

// Note that we don't support building for Windows 7 or later (_WIN32_WINNT >= 0x0601)
// without a platform SDK of v7.0 or later, as if _WIN32_WINNT >= 0x0601 then we
// assume that Windows 7 specific functions are available in the Platform SDK headers.

#if (_WIN32_WINNT >= 0x0601)
#if JETBYTE_PLATFORM_SDK_VERSION < 0x0700
#error Unsupported configuration - _WIN32_WINNT >= 0x0601 and JETBYTE_PLATFORM_SDK_VERSION < 0x0700.
#endif
#endif

#ifdef JETBYTE_PLATFORM_SDK_VERSION
#if JETBYTE_PLATFORM_SDK_VERSION == 0x0001
#elif JETBYTE_PLATFORM_SDK_VERSION == 0x060A
#elif JETBYTE_PLATFORM_SDK_VERSION == 0x0610
#elif JETBYTE_PLATFORM_SDK_VERSION == 0x0700
#elif JETBYTE_PLATFORM_SDK_VERSION == 0x070A
#elif JETBYTE_PLATFORM_SDK_VERSION == 0x0800
#elif JETBYTE_PLATFORM_SDK_VERSION == 0x0810
#elif JETBYTE_PLATFORM_SDK_VERSION == 0x0A0A
#else
#if JETBYTE_ALLOW_UNTESTED_COMPILE_ENV == 0
#error Untested Platform SDK Version
#else
#pragma JETBYTE_MESSAGE("Build configuration: Untested Platform SDK Version")
#endif
#endif
#endif

#if JETBYTE_SHOW_ADMIN_CONFIG_DETAILS == 0

#pragma JETBYTE_MESSAGE("Build configuration:")
#pragma JETBYTE_MESSAGE("Build configuration: Define JETBYTE_SHOW_ADMIN_CONFIG_DETAILS to 1 in Admin\\Config.h")
#pragma JETBYTE_MESSAGE("Build configuration: to display all configuration defines")

#else

#pragma JETBYTE_MESSAGE("Build configuration:")
#pragma JETBYTE_MESSAGE("Build configuration: Define JETBYTE_SHOW_ADMIN_CONFIG_DETAILS to 0 in Admin\\Config.h")
#pragma JETBYTE_MESSAGE("Build configuration: to turn off the display of the following configuration defines")
#pragma JETBYTE_MESSAGE("Build configuration:")

#if (JETBYTE_SHOW_TODO == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TODO enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TODO enabled: NO")
#endif

#if (JETBYTE_ADMIN_ENABLE_ALL_LEAK_TRACKING == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_ENABLE_ALL_LEAK_TRACKING enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_ENABLE_ALL_LEAK_TRACKING enabled: NO")
#endif

#if (JETBYTE_ADMIN_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD enabled: NO")
#endif

#if (JETBYTE_TRANSLATE_SEH_EXCEPTIONS == 1)
#if (JETBYTE_TRANSLATE_SEH_EXCEPTIONS_IN_DEBUGGER == 1)
#pragma JETBYTE_MESSAGE("Build configuration: Translate SEH exceptions and catch them at thread boundaries")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Translate SEH exceptions and catch them at thread boundaries unless debugger present")
#endif
#else
#pragma JETBYTE_MESSAGE("Do NOT translate SEH exceptions, allow them to propogate and remain uncaught")
#endif

#if (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#pragma JETBYTE_MESSAGE("Build configuration: Catch unhandled exceptions in destructors with catch (...)")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Do NOT catch unhandled exceptions in destructors with catch (...), allow them to propogate and remain uncaught")
#endif

#if (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY == 1)
#pragma JETBYTE_MESSAGE("Build configuration: Catch unhandled exceptions at thread boundaries with catch (...)")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Do NOT catch unhandled exceptions at thread boundaries with catch (...), allow them to propogate and remain uncaught")
#endif

#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_EXCEPTION_STACK_TRACES enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_EXCEPTION_STACK_TRACES enabled: NO")
#endif

#if (JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: YES")
#if (JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE enabled: NO")
#endif
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: NO")
#endif

#if (JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: YES")
#if (JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE enabled: NO")
#endif
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: NO")
#endif

#if defined(JETBYTE_TINY_XML_2_6_2) && (JETBYTE_TINY_XML_2_6_2 == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TINY_XML_2_6_2 enabled: YES (using TinyXML 2.6.2) in XMLTools")
#elif defined(JETBYTE_TINY_XML_2_6_1) && (JETBYTE_TINY_XML_2_6_1 == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TINY_XML_2_6_1 enabled: YES (using TinyXML 2.6.1) in XMLTools")
#elif defined(JETBYTE_TINY_XML_2_5_3) && (JETBYTE_TINY_XML_2_5_3 == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TINY_XML_2_5_3 enabled: YES (using TinyXML 2.5.3) in XMLTools")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TINY_XML_2_5_2 enabled: YES (using TinyXML 2.5.2) in XMLTools")
#endif

#if defined(JETBYTE_ZLIB_1_2_5) && (JETBYTE_ZLIB_1_2_5 == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ZLIB_1_2_5 enabled: YES (using ZLib 1.2.5) in ZLibTools")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ZLIB_1_2_3 enabled: YES (using ZLib 1.2.3) in ZLibTools")
#endif



#if (JETBYTE_PERF_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS == 1)
#if (_WIN32_WINNT < 0x0600)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS enabled: NO - Cannot be enabled if _WIN32_WINNT < 0x0600")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS enabled: YES")
#endif
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS enabled: NO")
#endif

#if (JETBYTE_PERF_FILE_READER_SKIP_COMPLETION_PORT_ON_SUCCESS == 1)
#if (_WIN32_WINNT < 0x0600)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_FILE_READER_SKIP_COMPLETION_PORT_ON_SUCCESS enabled: NO - Cannot be enabled if _WIN32_WINNT < 0x0600")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_FILE_READER_SKIP_COMPLETION_PORT_ON_SUCCESS enabled: YES")
#endif
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_FILE_READER_SKIP_COMPLETION_PORT_ON_SUCCESS enabled: NO")
#endif

#if (JETBYTE_CAN_ENABLE_SKIP_COMPLETION_PORT_ON_SUCCESS_DEBUG == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_CAN_ENABLE_SKIP_COMPLETION_PORT_ON_SUCCESS_DEBUG enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_CAN_ENABLE_SKIP_COMPLETION_PORT_ON_SUCCESS_DEBUG enabled: NO")
#endif

#if (JETBYTE_CAN_ENABLE_SKIP_COMPLETION_PORT_ON_SUCCESS_DISPLAY_FAILURE_REASON == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_CAN_ENABLE_SKIP_COMPLETION_PORT_ON_SUCCESS_DISPLAY_FAILURE_REASON enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_CAN_ENABLE_SKIP_COMPLETION_PORT_ON_SUCCESS_DISPLAY_FAILURE_REASON enabled: NO")
#endif

#if (JETBYTE_DEBUG_BREAK_ON_CONNECTIONS_ACTIVE_DURING_DESTRUCTION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_DEBUG_BREAK_ON_CONNECTIONS_ACTIVE_DURING_DESTRUCTION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_DEBUG_BREAK_ON_CONNECTIONS_ACTIVE_DURING_DESTRUCTION enabled: NO")
#endif


#if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_TIMER_QUEUE_MONITORING enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_TIMER_QUEUE_MONITORING enabled: NO")
#endif

#if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_TIMER_WHEEL_MONITORING enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_TIMER_WHEEL_MONITORING enabled: NO")
#endif

#if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_TIMER_CONTENTION_MONITORING enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_TIMER_CONTENTION_MONITORING enabled: NO")
#endif


#if (JETBYTE_PERF_NO_ACTIVE_BUFFER_LIST == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_NO_ACTIVE_BUFFER_LIST enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_NO_ACTIVE_BUFFER_LIST enabled: NO")
#endif

#if (JETBYTE_PERF_BUFFER_ALLOCATION_CONTENTION_MONITORING == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_BUFFER_ALLOCATION_CONTENTION_MONITORING enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_BUFFER_ALLOCATION_CONTENTION_MONITORING enabled: NO")
#endif

#if (JETBYTE_ILLEGAL_BUFFER_REFERENCE_COUNT_CRASH_DUMP_CREATION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ILLEGAL_BUFFER_REFERENCE_COUNT_CRASH_DUMP_CREATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ILLEGAL_BUFFER_REFERENCE_COUNT_CRASH_DUMP_CREATION enabled: NO")
#endif

#if (JETBYTE_CRASH_DUMP_ON_NEXT_BUFFER_REF_CHANGE_AFTER_ILLEGAL_CHANGE == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_CRASH_DUMP_ON_NEXT_BUFFER_REF_CHANGE_AFTER_ILLEGAL_CHANGE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_CRASH_DUMP_ON_NEXT_BUFFER_REF_CHANGE_AFTER_ILLEGAL_CHANGE enabled: NO")
#endif

#if (JETBYTE_ILLEGAL_SOCKET_REFERENCE_COUNT_CRASH_DUMP_CREATION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ILLEGAL_SOCKET_REFERENCE_COUNT_CRASH_DUMP_CREATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ILLEGAL_SOCKET_REFERENCE_COUNT_CRASH_DUMP_CREATION enabled: NO")
#endif

#if (JETBYTE_CRASH_DUMP_ON_NEXT_SOCKET_REF_CHANGE_AFTER_ILLEGAL_CHANGE == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_CRASH_DUMP_ON_NEXT_SOCKET_REF_CHANGE_AFTER_ILLEGAL_CHANGE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_CRASH_DUMP_ON_NEXT_SOCKET_REF_CHANGE_AFTER_ILLEGAL_CHANGE enabled: NO")
#endif

#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_REENTRANT_SINGLE_WRITER_MULTIPLE_READER_LOCK_2_MAX_THREADS set to " JETBYTE_MACROASSTRING(JETBYTE_REENTRANT_SINGLE_WRITER_MULTIPLE_READER_LOCK_2_MAX_THREADS))

#ifdef _SECURE_SCL
#if (_SECURE_SCL == 1)
#pragma JETBYTE_MESSAGE("Build configuration: _SECURE_SCL enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: _SECURE_SCL enabled: NO")
#endif

#ifdef _SECURE_SCL_THROWS
#if (_SECURE_SCL_THROWS == 1)
#pragma JETBYTE_MESSAGE("Build configuration: _SECURE_SCL_THROWS enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: _SECURE_SCL_THROWS enabled: NO")
#endif
#endif

#ifdef _SCL_SECURE_NO_WARNINGS
#pragma JETBYTE_MESSAGE("Build configuration: _SCL_SECURE_NO_WARNINGS enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: _SCL_SECURE_NO_WARNINGS enabled: NO")
#endif

#else
#pragma JETBYTE_MESSAGE("Build configuration: _SECURE_SCL enabled: NO")
#endif

#if (JETBYTE_TERMINATE_CRASH_DUMP_CREATION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TERMINATE_CRASH_DUMP_CREATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TERMINATE_CRASH_DUMP_CREATION enabled: NO")
#endif

#if (JETBYTE_UNEXPECTED_CRASH_DUMP_CREATION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_UNEXPECTED_CRASH_DUMP_CREATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_UNEXPECTED_CRASH_DUMP_CREATION enabled: NO")
#endif

#if JETBYTE_MINI_DUMP_GENERATOR_BREAK_INTO_DEBUGGER == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_MINI_DUMP_GENERATOR_BREAK_INTO_DEBUGGER enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_MINI_DUMP_GENERATOR_BREAK_INTO_DEBUGGER enabled: NO")
#endif

#if JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED enabled: NO")
#endif

#if JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED enabled: NO")
#endif

#if JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED enabled: NO")
#endif

#if JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS enabled: NO")
#endif

#if JETBYTE_HAS_ATL_HEADERS_INSTALLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_HAS_ATL_HEADERS_INSTALLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_HAS_ATL_HEADERS_INSTALLED enabled: NO")
#endif

#if JETBYTE_PERFMON_CHECK_COUNTER_ALIGNMENT == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERFMON_CHECK_COUNTER_ALIGNMENT enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERFMON_CHECK_COUNTER_ALIGNMENT enabled: NO")
#endif

#if JETBYTE_PERFMON_VALIDATE_COUNTER_SIZES == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERFMON_VALIDATE_COUNTER_SIZES enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERFMON_VALIDATE_COUNTER_SIZES enabled: NO")
#endif

#if !defined(JETBYTE_ADMIN_ENABLE_THIRD_PARTY_CODE) && !defined(JETBYTE_ADMIN_ENABLE_ALL_THIRD_PARTY_CODE)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_ENABLE_THIRD_PARTY_CODE enabled: NO")
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_ENABLE_ALL_THIRD_PARTY_CODE enabled: NO")
#ifndef JETBYTE_ADMIN_DISABLE_DISABLED_THIRD_PARTY_CODE_WARNINGS
#pragma JETBYTE_MESSAGE("Build configuration: ")
#pragma JETBYTE_MESSAGE("Build configuration: *******************************************************")
#pragma JETBYTE_MESSAGE("Build configuration: *                                                     *")
#pragma JETBYTE_MESSAGE("Build configuration: * WARNING - All third-party code is disabled          *")
#pragma JETBYTE_MESSAGE("Build configuration: * Some aspects of the framework will have reduced     *")
#pragma JETBYTE_MESSAGE("Build configuration: * functionality.                                      *")
#pragma JETBYTE_MESSAGE("Build configuration: *                                                     *")
#pragma JETBYTE_MESSAGE("Build configuration: * Define JETBYTE_ADMIN_ENABLE_THIRD_PARTY_CODE in     *")
#pragma JETBYTE_MESSAGE("Build configuration: * your ThirdPartyCodeConfig.h to enable the use of    *")
#pragma JETBYTE_MESSAGE("Build configuration: * third-party code by the framework.                  *")
#pragma JETBYTE_MESSAGE("Build configuration: * Each piece of third-party code can be disabled      *")
#pragma JETBYTE_MESSAGE("Build configuration: * independently, if desired.                          *")
#pragma JETBYTE_MESSAGE("Build configuration: * See each library's Admin.h for details.             *")
#pragma JETBYTE_MESSAGE("Build configuration: *                                                     *")
#pragma JETBYTE_MESSAGE("Build configuration: *******************************************************")
#pragma JETBYTE_MESSAGE("Build configuration: ")
#endif

#ifndef JETBYTE_ADMIN_DISABLING_ALL_THIRD_PARTY_CODE_IS_NON_FATAL
#error Define JETBYTE_ADMIN_DISABLING_ALL_THIRD_PARTY_CODE_IS_NON_FATAL in config.h to accept the above warning and continue with the build
#endif

#else

#ifdef JETBYTE_ADMIN_ENABLE_THIRD_PARTY_CODE
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_ENABLE_THIRD_PARTY_CODE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_ENABLE_THIRD_PARTY_CODE enabled: NO")
#endif

#ifdef JETBYTE_ADMIN_ENABLE_ALL_THIRD_PARTY_CODE
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_ENABLE_ALL_THIRD_PARTY_CODE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_ENABLE_ALL_THIRD_PARTY_CODE enabled: NO")
#endif
#endif

#ifndef JETBYTE_ADMIN_DISABLE_DISABLED_THIRD_PARTY_CODE_WARNINGS
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_DISABLE_DISABLED_THIRD_PARTY_CODE_WARNINGS enabled: NO")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_DISABLE_DISABLED_THIRD_PARTY_CODE_WARNINGS enabled: YES")
#endif

#if JETBYTE_ADMIN_SHOW_THIRD_PARTY_CODE_LICENCES == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_SHOW_THIRD_PARTY_CODE_LICENCES enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_SHOW_THIRD_PARTY_CODE_LICENCES enabled: NO")
#endif

#if JETBYTE_ADMIN_SHOW_DERIVED_THIRD_PARTY_CODE_LICENCES == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_SHOW_DERIVED_THIRD_PARTY_CODE_LICENCES enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_ADMIN_SHOW_DERIVED_THIRD_PARTY_CODE_LICENCES enabled: NO")
#endif

#ifdef JETBYTE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS
#error JETBYTE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS has been replaced with JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS
#endif

#ifdef JETBYTE_LOCKABLE_OBJECT_USE_STD_THREADING
#error JETBYTE_LOCKABLE_OBJECT_USE_STD_THREADING has been replaced with JETBYTE_CORE_LOCKABLE_OBJECT_USE_STD_THREADING
#endif

#ifdef JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE
#error JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE has been replaced with JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE
#endif

#ifdef JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP
#error JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP has been replaced with JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP
#endif

#ifdef JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION
#error JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION has been replaced with JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION
#endif

#endif

#pragma JETBYTE_MESSAGE("Build configuration:---------------------------------------------------------------")
#pragma JETBYTE_MESSAGE("")
#pragma JETBYTE_MESSAGE("")
#pragma JETBYTE_MESSAGE("")

const int JetByteToolsVersion = JETBYTE_TOOLS_VER;

///////////////////////////////////////////////////////////////////////////////
// End of file: Admin.cpp
///////////////////////////////////////////////////////////////////////////////
