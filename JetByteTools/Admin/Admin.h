#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: Admin.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2004 JetByte Limited.
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

/// \file Admin.h
/// This file must be the first file included by all cpp files that are built
/// in any library in the JetByte Tools suite.
/// This file contains global <code>defines</code> that affect how the code
/// is built.
/// \ingroup Admin

// This is the readable version that we've had for a while which is,
// unfortunately, useless if you want to compile code conditionally based on
// the version of the JetByte Tools libraries that you're using...

// think about including dll runtime projects

#define JETBYTE_TOOLS_VERSION 7.5

// This is the new version define that can be used for conditional compilation.

#define JETBYTE_TOOLS_VER 0x0750

extern const int JetByteToolsVersion;

// VS 2010 and later want to warn us about this "problem" and we suppress it in Warnings.h
// but we need to suppress it here as we use the construct in this header file and
// in Config.h
// Note that the cause of the warning is
// #ifdef XXX
// if XXX is defined as 0...

#pragma warning(disable: 4574)   // 'x' is defined to be '0': did you mean to use '#if x'?

#include "Platform.h"

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

#define JETBYTE_MINIMUM_SUPPORTED_COMPILER_VERSION 1910

/// This is the latest version of the compiler we will support from the
/// next major release

#define JETBYTE_MINIMUM_NON_DEPRECATED_COMPILER_VERSION 1910

/// This is the latest compiler version that we've tested on.
/// You might get away with later versions but we don't support them yet.

#define JETBYTE_LATEST_TESTED_COMPILER_VERSION 1938

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

#include "TargetWindowsVersion.h"

// Fix up optional defines from TargetWindowsVersion.h

#ifndef _WIN32_WINNT
#define _WIN32_WINNT JETBYTE_MINIMUM_SUPPORTED_WINDOWS_VERSION
#endif

#ifndef NTDDI_VERSION
#define NTDDI_VERSION JETBYTE_MINIMUM_SUPPORTED_NTDDI_VERSION
#endif

// You need to create a Config.h file in the JetByteTools\Admin directory before you
// can compile the code. Example Config.h files can be found in the ExampleConfigHeaders
// directory. The header file is used to collect together user and environment specific
// configuration options.

// Defines that can be set in Config.h

// JETBYTE_ALLOW_UNTESTED_COMPILE_ENV
// Define as 1 to suppress the errors for building with a later compile environment than
// we have tested with.

// Try and do the right thing in case nobody bothers to manually configure these.
// They can be set either in TargetWindowsVersion.h or in Config.h...

#ifndef JETBYTE_HAS_SRW_LOCK_TRY_ENTER
#if (_WIN32_WINNT >= 0x0601)
#define JETBYTE_HAS_SRW_LOCK_TRY_ENTER 1
#else
#define JETBYTE_HAS_SRW_LOCK_TRY_ENTER 0
#endif
#endif

// this can fail on some versions of VS where it is supported but broken for paths
// that include a drive letter, so S:\blah.txt will fail but \blah.txt will work
// as expected...
// work out which ones! fails on 2019 16.6.5
#define JETBYTE_HAS_INCLUDE(_x) __has_include(_x)

#ifndef JETBYTE_NO_SUPPRESS_WINSOCK_HEADERS
#define _WINSOCKAPI_          // NEVER include winsock 1.0 header
#endif

#include "ThirdPartyCodeConfig.h"

#include "Config.h"

// Fix up optional defines that may not have been defined in Config.h

#ifndef JETBYTE_ALLOW_UNTESTED_COMPILE_ENV
#define JETBYTE_ALLOW_UNTESTED_COMPILE_ENV 0
#endif

#ifndef JETBYTE_SHOW_TODO
#define JETBYTE_SHOW_TODO 0
#endif

#ifndef JETBYTE_NO_PLATFORM_SDK
#define JETBYTE_NO_PLATFORM_SDK 0
#endif

#ifdef JETBYTE_CATCH_UNHANDLED_EXCEPTIONS
#if (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS == 1)
#ifndef JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS
#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS 1
#endif
#ifndef JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY
#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY 1
#endif
#ifndef JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_NOTHROW_FUNCTIONS
#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_NOTHROW_FUNCTIONS 1
#endif
#elif (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS == 0)
#ifndef JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS
#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS 0
#endif
#ifndef JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY
#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY 0
#endif
#ifndef JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_NOTHROW_FUNCTIONS
#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_NOTHROW_FUNCTIONS 0
#endif
#endif
#endif

#ifndef JETBYTE_ADMIN_ENABLE_ALL_LEAK_TRACKING
#define JETBYTE_ADMIN_ENABLE_ALL_LEAK_TRACKING 1
#endif

#ifndef JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS
#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS 1
#endif

#ifndef JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS
#define JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS 1
#endif

#ifndef JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY
#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY 1
#endif

#ifndef JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY
#define JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY 1
#endif

#ifndef JETBYTE_CATCH_UNHANDLED_EXCEPTIONS
#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS 1
#endif

#ifndef JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS
#define JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS 1
#endif

#if (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 0) || (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY == 0) || (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS == 0)
#ifdef __cplusplus
namespace JetByteTools
{
   namespace Admin
   {
      class CDummyCatchAllException {};
   }
}
#endif
#endif

#define JETBYTE_WIDEN2(x) L ## x
#define JETBYTE_WIDEN(x) JETBYTE_WIDEN2(x)

#ifdef _UNICODE
#define JETBYTE_T_FUNCTION JETBYTE_WIDEN(__FUNCTION__)
#else
#define JETBYTE_T_FUNCTION __FUNCTION__
#endif

#if (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#define JETBYTE_CATCH_ALL_IN_DESTRUCTORS_IF_ENABLED catch (...)
#else
#define JETBYTE_CATCH_ALL_IN_DESTRUCTORS_IF_ENABLED catch (const JetByteTools::Admin::CDummyCatchAllException &)
#endif

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED catch (...) { try{JetByteTools::Core::OutputEx("Unexpected exception in destructor: " __FUNCTION__ );}catch (...){} }
#elif (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED catch (...) {}
#else
#define JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED catch (const JetByteTools::Admin::CDummyCatchAllException &) {}
#endif

#if (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY == 1)
#define JETBYTE_CATCH_ALL_AT_THREAD_BOUNDARY_IF_ENABLED catch (...)
#else
#define JETBYTE_CATCH_ALL_AT_THREAD_BOUNDARY_IF_ENABLED catch (const JetByteTools::Admin::CDummyCatchAllException &)
#endif

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED catch (...) { try{JetByteTools::Core::OutputEx("Unexpected exception at thread boundary: " __FUNCTION__ );}catch (...){} }
#elif (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED catch (...) {}
#else
#define JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED catch (const JetByteTools::Admin::CDummyCatchAllException &) {}
#endif

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_IF_ENABLED catch (...) { try{JetByteTools::Core::OutputEx("Unexpected exception: " __FUNCTION__ );}catch (...){} }
#elif (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_IF_ENABLED catch (...) {}
#else
#define JETBYTE_CATCH_AND_LOG_ALL_IF_ENABLED catch (const JetByteTools::Admin::CDummyCatchAllException &) {}
#endif

#define JETBYTE_TESTS_CATCH_ALL_AT_THREAD_BOUNDARY_IF_ENABLED catch (...)

#define JETBYTE_TESTS_CATCH_ALL_IF_ENABLED catch (...)

#ifndef JETBYTE_ADMIN_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD
#define JETBYTE_ADMIN_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD 1
#endif


// We don't enable this as I've been unable to generate the 'skipping' situation
// during testing...

#ifndef JETBYTE_PERF_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS
#define JETBYTE_PERF_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS 0
#endif

#ifndef JETBYTE_PERF_FILE_READER_SKIP_COMPLETION_PORT_ON_SUCCESS
#define JETBYTE_PERF_FILE_READER_SKIP_COMPLETION_PORT_ON_SUCCESS 1
#endif

#ifndef JETBYTE_CAN_ENABLE_SKIP_COMPLETION_PORT_ON_SUCCESS_DEBUG
#define JETBYTE_CAN_ENABLE_SKIP_COMPLETION_PORT_ON_SUCCESS_DEBUG 0
#endif

#ifndef JETBYTE_CAN_ENABLE_SKIP_COMPLETION_PORT_ON_SUCCESS_DISPLAY_FAILURE_REASON
#define JETBYTE_CAN_ENABLE_SKIP_COMPLETION_PORT_ON_SUCCESS_DISPLAY_FAILURE_REASON 1
#endif

#ifndef JETBYTE_PERF_DATAGRAM_SOCKETS_LIMIT_IO_RECURSION_TO
#define JETBYTE_PERF_DATAGRAM_SOCKETS_LIMIT_IO_RECURSION_TO 10
#endif

// Map the user defines into the ones that are actually used in the code, note that
// this was to avoid us having to have this kind of construct
// #if (JETBYTE_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS == 1 && _WIN32_WINNT >= 0x0600)
// The extra level of indirection is due to the fact that we like to report (in Admin.cpp) on
// the config.h setting and also the actual achieved setting so we can't just only allow the
// config.h setting to work if we're on the right version of windows.
// BUT now it's always the right version of windows...

#ifdef JETBYTE_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS
#error JETBYTE_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS should not be defined by YOU it's internal, define JETBYTE_PERF_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS instead
#endif

#ifdef JETBYTE_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS
#error JETBYTE_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS should not be defined by YOU it's internal, define JETBYTE_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS instead
#endif

#ifdef JETBYTE_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS
#error JETBYTE_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS should not be defined by YOU it is internal, define JETBYTE_PERF_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS instead
#endif

#ifdef JETBYTE_FILE_READER_SKIP_COMPLETION_PORT_ON_SUCCESS
#error JETBYTE_FILE_READER_SKIP_COMPLETION_PORT_ON_SUCCESS should not be defined by YOU it is internal, define JETBYTE_PERF_FILE_READER_SKIP_COMPLETION_PORT_ON_SUCCESS instead
#endif

#ifdef JETBYTE_DATAGRAM_SOCKETS_LIMIT_IO_RECURSION_TO
#error JETBYTE_DATAGRAM_SOCKETS_LIMIT_IO_RECURSION_TO should not be defined by YOU it is internal, define JETBYTE_PERF_DATAGRAM_SOCKETS_LIMIT_IO_RECURSION_TO instead
#endif

#define JETBYTE_DATAGRAM_SOCKETS_LIMIT_IO_RECURSION_TO            JETBYTE_PERF_DATAGRAM_SOCKETS_LIMIT_IO_RECURSION_TO

#define JETBYTE_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS    JETBYTE_PERF_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS
#define JETBYTE_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS  JETBYTE_PERF_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS
#define JETBYTE_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS       JETBYTE_PERF_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS
#define JETBYTE_FILE_READER_SKIP_COMPLETION_PORT_ON_SUCCESS       JETBYTE_PERF_FILE_READER_SKIP_COMPLETION_PORT_ON_SUCCESS

#ifndef JETBYTE_DEBUG_BREAK_ON_CONNECTIONS_ACTIVE_DURING_DESTRUCTION
#define JETBYTE_DEBUG_BREAK_ON_CONNECTIONS_ACTIVE_DURING_DESTRUCTION 1
#endif

#ifndef JETBYTE_PERF_NO_ACTIVE_BUFFER_LIST
#define JETBYTE_PERF_NO_ACTIVE_BUFFER_LIST 0
#endif

#ifndef JETBYTE_PERF_BUFFER_ALLOCATION_CONTENTION_MONITORING
#define JETBYTE_PERF_BUFFER_ALLOCATION_CONTENTION_MONITORING 0
#endif

#ifndef JETBYTE_PERF_BUFFER_ALLOCATION_MONITORING
#define JETBYTE_PERF_BUFFER_ALLOCATION_MONITORING 1
#endif

#ifndef JETBYTE_DISABLE_POOLING_BUFFER_ALLOCATOR
#define JETBYTE_DISABLE_POOLING_BUFFER_ALLOCATOR 0
#endif

#ifndef JETBYTE_ILLEGAL_BUFFER_REFERENCE_COUNT_CRASH_DUMP_CREATION
#define JETBYTE_ILLEGAL_BUFFER_REFERENCE_COUNT_CRASH_DUMP_CREATION 0
#endif

#ifndef JETBYTE_CHECK_BUFFER_VALIDITY_ON_EVERY_ACCESS
#define JETBYTE_CHECK_BUFFER_VALIDITY_ON_EVERY_ACCESS 0
#endif

#ifndef JETBYTE_CRASH_DUMP_ON_NEXT_BUFFER_REF_CHANGE_AFTER_ILLEGAL_CHANGE
#define JETBYTE_CRASH_DUMP_ON_NEXT_BUFFER_REF_CHANGE_AFTER_ILLEGAL_CHANGE 0
#endif

#ifndef JETBYTE_OUT_OF_BUFFER_SPACE_CRASH_DUMP_CREATION
#define JETBYTE_OUT_OF_BUFFER_SPACE_CRASH_DUMP_CREATION 0
#endif

#ifndef JETBYTE_ILLEGAL_BUFFER_USER_DATA_EXCEPTIONS
#define JETBYTE_ILLEGAL_BUFFER_USER_DATA_EXCEPTIONS 0
#endif

#ifndef JETBYTE_ILLEGAL_SOCKET_REFERENCE_COUNT_CRASH_DUMP_CREATION
#define JETBYTE_ILLEGAL_SOCKET_REFERENCE_COUNT_CRASH_DUMP_CREATION 0
#endif

#ifndef JETBYTE_CRASH_DUMP_ON_NEXT_SOCKET_REF_CHANGE_AFTER_ILLEGAL_CHANGE
#define JETBYTE_CRASH_DUMP_ON_NEXT_SOCKET_REF_CHANGE_AFTER_ILLEGAL_CHANGE 0
#endif

#ifndef JETBYTE_SORTED_BUFFER_CHAIN_GENERATE_CRASH_DUMP_ON_DUPLICATE_BUFFER
#define JETBYTE_SORTED_BUFFER_CHAIN_GENERATE_CRASH_DUMP_ON_DUPLICATE_BUFFER 0
#endif

#ifndef JETBYTE_SORTED_BUFFER_CHAIN_GENERATE_CRASH_DUMP_ON_MULTIPLE_LISTS
#define JETBYTE_SORTED_BUFFER_CHAIN_GENERATE_CRASH_DUMP_ON_MULTIPLE_LISTS 0
#endif

#ifndef JETBYTE_TRACE_SERVICE_STATE_CHANGES
#define JETBYTE_TRACE_SERVICE_STATE_CHANGES 0
#endif

#ifndef JETBYTE_TRACE_SERVICE_FAILURE_ACTION_DETECTION
#define JETBYTE_TRACE_SERVICE_FAILURE_ACTION_DETECTION 0
#endif

#ifdef JETBYTE_SHOW_CONFIG_DETAILS
#ifndef JETBYTE_SHOW_ADMIN_CONFIG_DETAILS
#define JETBYTE_SHOW_ADMIN_CONFIG_DETAILS JETBYTE_SHOW_CONFIG_DETAILS
#endif
#endif

#ifndef JETBYTE_SHOW_ADMIN_CONFIG_DETAILS
#define JETBYTE_SHOW_ADMIN_CONFIG_DETAILS 0
#endif

#ifndef JETBYTE_PERFMON_CHECK_COUNTER_ALIGNMENT
#define JETBYTE_PERFMON_CHECK_COUNTER_ALIGNMENT 1
#endif

#ifndef JETBYTE_PERFMON_VALIDATE_COUNTER_SIZES
#define JETBYTE_PERFMON_VALIDATE_COUNTER_SIZES 1
#endif

#ifndef JETBYTE_ADMIN_DISABLE_SSL_CONNECTION_BUFFER_POOLING
#define JETBYTE_ADMIN_DISABLE_SSL_CONNECTION_BUFFER_POOLING 0
#endif

#ifndef JETBYTE_ADMIN_SHOW_THIRD_PARTY_CODE_LICENCES
#define JETBYTE_ADMIN_SHOW_THIRD_PARTY_CODE_LICENCES 1
#endif

#ifndef JETBYTE_ADMIN_SHOW_DERIVED_THIRD_PARTY_CODE_LICENCES
#define JETBYTE_ADMIN_SHOW_DERIVED_THIRD_PARTY_CODE_LICENCES 0
#endif

#ifndef JETBYTE_DEPRECATE_OPTEX
#define JETBYTE_DEPRECATE_OPTEX 1
#endif

#ifndef JETBYTE_BUFFER_HANDLE_ALLOCATOR_LSP_FAILURE_CRASH_DUMP_CREATION
#define JETBYTE_BUFFER_HANDLE_ALLOCATOR_LSP_FAILURE_CRASH_DUMP_CREATION 1
#endif

#include "Warnings.h"
#include "Todo.h"

///////////////////////////////////////////////////////////////////////////////
// End of file: Admin.h
///////////////////////////////////////////////////////////////////////////////
