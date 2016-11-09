///////////////////////////////////////////////////////////////////////////////
// File: Admin.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2007 JetByte Limited.
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

#include "JetByteTools\Admin\Admin.h"
#include "JetByteTools\Admin\CompilerName.h"

#pragma hdrstop

#pragma JETBYTE_MESSAGE(" Build configuration:---------------------------------------------------------------")

#ifdef JETBYTE_TOOLS_VERSION
#pragma JETBYTE_MESSAGE(" Build configuration: JetByte Tools Library Version = " JETBYTE_MACROASSTRING(JETBYTE_TOOLS_VERSION))
#else
#pragma JETBYTE_MESSAGE(" Build configuration: JetByte Tools Library Version = UNSET")
#endif

#ifdef JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION
#if (JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION == 1400)
#pragma JETBYTE_MESSAGE(" Build configuration: Minimum supported compiler version = " JETBYTE_MACROASSTRING(JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION) " - \"Visual Studio 2005\"")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Minimum supported compiler version = " JETBYTE_MACROASSTRING(JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION))
#endif
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Minimum supported compiler version = UNSET")
#endif

#ifdef JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION
#if (JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION != JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION)
#if (JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION == 1400)
#pragma JETBYTE_MESSAGE(" Build configuration: Minimum non deprecated compiler version = " JETBYTE_MACROASSTRING(JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION) " - \"Visual Studio 2005\"")
#pragma JETBYTE_MESSAGE(" Build configuration: Deprecated compilers will become unsupported from the next release")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Minimum non deprecated compiler version = " JETBYTE_MACROASSTRING(JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION))
#pragma JETBYTE_MESSAGE(" Build configuration: Deprecated compilers will become unsupported from the next release")
#endif
#endif
#endif

#ifdef _MSC_VER
#pragma JETBYTE_MESSAGE(" Build configuration: Compiler Version = " JETBYTE_MACROASSTRING(_MSC_VER) " - " JETBYTE_MACROASSTRING(JETBYTE_COMPILER_LONG_NAME_A))
#else
#pragma JETBYTE_MESSAGE("Build configuration: Compiler Library Version = UNSET")
#endif

#pragma JETBYTE_MESSAGE(" Build configuration: Target Platform can be adjusted by editing the values in Admin\\TargetWindowsVersion.h")
#pragma JETBYTE_MESSAGE(" Build configuration: Other options can be adjusted editing the values in Admin\\Config.h")


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
#pragma JETBYTE_MESSAGE("Build configuration: NTDDI_VERSION = " JETBYTE_MACROASSTRING(NTDDI_VERSION) " (Windows Vista)")
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

#if (JETBYTE_SHOW_TODO == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TODO enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TODO enabled: NO")
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
#pragma JETBYTE_MESSAGE("Build configuration: Untested compiler version: _MSC_VER > JETBYTE_LATEST_TESTED_COMPILER_VERSION")
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

#if (JETBYTE_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD == 1)
#pragma JETBYTE_MESSAGE("Build configuration: Install a per thread error handler in CThread::Run()")
#else
#pragma JETBYTE_MESSAGE("Build configuration: do NOT install a per thread error handler in CThread::Run()")
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
#pragma JETBYTE_MESSAGE("Build configuration: Catch unhandled exceptions in destructors with catch(...)")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Do NOT catch unhandled exceptions in destructors with catch(...), allow them to propogate and remain uncaught")
#endif

#if (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY == 1)
#pragma JETBYTE_MESSAGE("Build configuration: Catch unhandled exceptions at thread boundaries with catch(...)")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Do NOT catch unhandled exceptions at thread boundaries with catch(...), allow them to propogate and remain uncaught")
#endif

#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_EXCEPTION_STACK_TRACES enabled: YES")
#if (JETBYTE_EXCEPTION_WHERE_INCLUDES_STACK_TRACE == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_EXCEPTION_WHERE_INCLUDES_STACK_TRACE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_EXCEPTION_WHERE_INCLUDES_STACK_TRACE enabled: NO")
#endif
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_EXCEPTION_STACK_TRACES enabled: NO")
#endif

#if (JETBYTE_REFERENCE_TRACKING_DISPLAY_LOADED_PDBS == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_REFERENCE_TRACKING_DISPLAY_LOADED_PDBS enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_REFERENCE_TRACKING_DISPLAY_LOADED_PDBS enabled: NO")
#endif

#if (JETBYTE_REFERENCE_TRACKING_DISPLAY_ONLY_NON_TOOLS_LIB_CHANGES == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_REFERENCE_TRACKING_DISPLAY_ONLY_NON_TOOLS_LIB_CHANGES enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_REFERENCE_TRACKING_DISPLAY_ONLY_NON_TOOLS_LIB_CHANGES enabled: NO")
#endif

#if (JETBYTE_TRACK_IO_BUFFER_REFERENCES == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TRACK_IO_BUFFER_REFERENCES enabled: YES")
#pragma JETBYTE_MESSAGE("Build configuration:  JETBYTE_ADDITIONAL_BUFFER_TRACKING_CONTEXT = " JETBYTE_MACROASSTRING(JETBYTE_ADDITIONAL_BUFFER_TRACKING_CONTEXT))
#pragma JETBYTE_MESSAGE("Build configuration:  JETBYTE_BUFFER_TRACKING_INSTANCES = " JETBYTE_MACROASSTRING(JETBYTE_BUFFER_TRACKING_INSTANCES))
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TRACK_IO_BUFFER_REFERENCES enabled: NO")
#endif

#if (JETBYTE_TRACK_SOCKET_REFERENCES == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TRACK_SOCKET_REFERENCES enabled: YES")
#pragma JETBYTE_MESSAGE("Build configuration:  JETBYTE_ADDITIONAL_SOCKET_TRACKING_CONTEXT = " JETBYTE_MACROASSTRING(JETBYTE_ADDITIONAL_SOCKET_TRACKING_CONTEXT))
#pragma JETBYTE_MESSAGE("Build configuration:  JETBYTE_SOCKET_TRACKING_INSTANCES = " JETBYTE_MACROASSTRING(JETBYTE_SOCKET_TRACKING_INSTANCES))
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TRACK_SOCKET_REFERENCES enabled: NO")
#endif

#if (JETBYTE_TRACK_ADDRESS_REFERENCES == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TRACK_ADDRESS_REFERENCES enabled: YES")
#pragma JETBYTE_MESSAGE("Build configuration:  JETBYTE_ADDITIONAL_ADDRESS_TRACKING_CONTEXT = " JETBYTE_MACROASSTRING(JETBYTE_ADDITIONAL_ADDRESS_TRACKING_CONTEXT))
#pragma JETBYTE_MESSAGE("Build configuration:  JETBYTE_ADDRESS_TRACKING_INSTANCES = " JETBYTE_MACROASSTRING(JETBYTE_ADDRESS_TRACKING_INSTANCES))
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TRACK_ADDRESS_REFERENCES enabled: NO")
#endif

#if (JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: NO")
#endif

#if (JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: YES")
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

#if (JETBYTE_PERF_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS == 1)
#if (_WIN32_WINNT < 0x0600)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS enabled: NO - Cannot be enabled if _WIN32_WINNT < 0x0600")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS enabled: YES")
#endif
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS enabled: NO")
#endif

#if (JETBYTE_PERF_DATAGRAM_SOCKETS_LIMIT_IO_RECURSION_TO != 0)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_DATAGRAM_SOCKETS_LIMIT_IO_RECURSION_TO enabled: YES - limited to " JETBYTE_MACROASSTRING(JETBYTE_PERF_DATAGRAM_SOCKETS_LIMIT_IO_RECURSION_TO) " recursive calls")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_DATAGRAM_SOCKETS_LIMIT_IO_RECURSION_TO enabled: NO")
#endif

#if (JETBYTE_PERF_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS == 1)
#if (_WIN32_WINNT < 0x0600)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS enabled: NO - Cannot be enabled if _WIN32_WINNT < 0x0600")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS enabled: YES")
#endif
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS enabled: NO")
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

#if (JETBYTE_DUMP_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_DUMP_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG enabled: YES")
#elif (JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_DUMP_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG enabled: NO")
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


#if (JETBYTE_PERF_REUSE_DATAGRAM_SOCKET_AND_BUFFER_FOR_NEXT_READ_IF_POSSIBLE == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_REUSE_DATAGRAM_SOCKET_AND_BUFFER_FOR_NEXT_READ_IF_POSSIBLE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_REUSE_DATAGRAM_SOCKET_AND_BUFFER_FOR_NEXT_READ_IF_POSSIBLE enabled: NO")
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

#if (JETBYTE_AUTOMATIC_SEH_CRASH_DUMP_CREATION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_AUTOMATIC_SEH_CRASH_DUMP_CREATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_AUTOMATIC_SEH_CRASH_DUMP_CREATION enabled: NO")
#endif

#if (JETBYTE_PURE_CALL_CRASH_DUMP_CREATION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PURE_CALL_CRASH_DUMP_CREATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PURE_CALL_CRASH_DUMP_CREATION enabled: NO")
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

#if (JETBYTE_TRACK_THREAD_NAMES == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TRACK_THREAD_NAMES enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_TRACK_THREAD_NAMES enabled: NO")
#endif

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

#if (JETBYTE_WEBSOCKET_ALLOW_BAD_SPACES_IN_HIXIE76_HANDSHAKE == 1)
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_WEBSOCKET_ALLOW_BAD_SPACES_IN_HIXIE76_HANDSHAKE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_WEBSOCKET_ALLOW_BAD_SPACES_IN_HIXIE76_HANDSHAKE enabled: NO")
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

#if JETBYTE_DEPRECATE_IN_ORDER_BUFFER_LIST == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_DEPRECATE_IN_ORDER_BUFFER_LIST enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_DEPRECATE_IN_ORDER_BUFFER_LIST enabled: NO")
#endif

#if JETBYTE_DEPRECATE_SEQUENTIAL_BUFFER_LIST == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_DEPRECATE_SEQUENTIAL_BUFFER_LIST enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_DEPRECATE_SEQUENTIAL_BUFFER_LIST enabled: NO")
#endif

#if JETBYTE_DEPRECATE_CRITICAL_SECTION == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_DEPRECATE_CRITICAL_SECTION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_DEPRECATE_CRITICAL_SECTION enabled: NO")
#endif

#if JETBYTE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS enabled: YES")
#if JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 0
#pragma JETBYTE_MESSAGE("Build configuration: WARNING - JETBYTE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS forced due to JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 0")
#endif
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS enabled: NO")
#endif

#if JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE enabled: YES")
#if JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_DEBUG_BREAK == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_DEBUG_BREAK enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_DEBUG_BREAK enabled: NO")
#endif
#if JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP enabled: NO")
#endif
#if JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION enabled: NO")
#endif
#if JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_OVERRIDE_EXCEPTION == 1
#pragma JETBYTE_MESSAGE("Build configuration: WARNING - JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION forcibly enabled due SRWL use")
#pragma JETBYTE_MESSAGE("Build configuration: WARNING - Set JETBYTE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS to 1 if you want to test for recursive")
#pragma JETBYTE_MESSAGE("Build configuration: WARNING - behaviour without causing fatal runtime errors.")
#endif
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE enabled: NO")
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

#if JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES enabled: NO")
#endif

#if JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES == 1
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES enabled: NO")
#endif

#pragma JETBYTE_MESSAGE("Build configuration:---------------------------------------------------------------")
#pragma JETBYTE_MESSAGE("")
#pragma JETBYTE_MESSAGE("")
#pragma JETBYTE_MESSAGE("")

///////////////////////////////////////////////////////////////////////////////
// End of file: Admin.cpp
///////////////////////////////////////////////////////////////////////////////
