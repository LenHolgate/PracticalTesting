#pragma once
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
/// This file contains global <code>defines</code> that affect how the code
/// is built.
/// \ingroup Admin

// This is the readable version that we've had for a while which is,
// unfortunately, useless if you want to compile code conditionally based on
// the version of the JetByte Tools libraries that you're using...

// think about including dll runtime projects

#define JETBYTE_TOOLS_VERSION 6.9.5

// This is the new version define that can be used for conditional compilation.

#define JETBYTE_TOOLS_VER 0x0695

// VS 2010 and later want to warn us about this "problem" and we suppress it in Warnings.h
// but we need to suppress it here as we use the construct in this header file and
// in Config.h
// Note that the cause of the warning is
// #ifdef XXX
// if XXX is defined as 0...

#pragma warning(disable: 4574)   // 'x' is defined to be '0': did you mean to use '#if x'?

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

#define JETBYTE_LATEST_TESTED_COMPILER_VERSION 1936

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

// JETBYTE_PLATFORM_SDK_VERSION
// Sometimes there are bugs in versions of the Platform SDK (or, perhaps, things
// we just don't quite understand how to correctly configure...). Rarely we work around
// these. To be able to work around them we need to know which version of the
// Platform SDK is being used and the SDK itself cant tell us. So... You can define
// JETBYTE_PLATFORM_SDK_VERSION with the following values:
// 0x060A - v6.0a - Vista
// 0x0610 - v6.1 - Windows Server 2008 and .Net Framework 3.5
// 0x0700 - v7.0 - Windows 7
// 0x0800 - v8.0 - Windows 8
// 0x0810 - v8.1 - Windows 8.1
// At present we only disable the deprecation of the GetVersion functions in the
// Windows 8.1 SDK by defining BUILD_WINDOWS.

// JETBYTE_SHOW_TODO
// Define as 1 to see internal 'todo' messages.

// JETBYTE_WARN_ON_SOCKADDR_STORAGE_DEF
// Define as 1 to get warnings when we define our own SOCKADDR_STORAGE structure
// on versions of Windows < 0x0501

// JETBYTE_WARN_ON_WSPIAPI_COUNTOF_DEF
// Define as 1 to get warnings when we redefine WSPIAPI_COUNTOF with a version
// that works for earlier compilers.

// JETBYTE_TRANSLATE_SEH_EXCEPTIONS
// Define to 1 to have all SEH exceptions tranlated into C++ exceptions (and caught
// in final catch handlers in all framework threads)

// JETBYTE_TRANSLATE_SEH_EXCEPTIONS_IN_DEBUGGER
// Define to 1 to have all SEH exceptions tranlated into C++ exceptions (and caught
// in final catch handlers in all framework threads) when running in the debugger,
// if 0 then SEH exceptions are translated only if the debugger is not present
// when the tranlator is be installed.

// JETBYTE_CATCH_UNHANDLED_EXCEPTIONS
// Define to 1 to enable catch (...) handlers. When disabled all catch (...) handlers
// are removed from library code.

// JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS
// Define to 1 to enable catch (...) handlers in destructors. Some destructors have
// catch (...) handlers present to prevent exceptions leaking from them. This option
// allows you to remove these handlers.
// When disabled all catch (...) handlers in destructors are removed from library code.

// JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY
// Define to 1 to enable catch (...) handlers in library threads. Most threads that
// the libraries create have catch (...) handlers present to prevent exceptions leaking
// from them. This option allows you to remove these handlers.
// When disabled all catch (...) handlers in threads are removed from library code.

// JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_NOTHROW_FUNCTIONS
// Define to 1 to enable catch (...) handlers in 'nothrow' functions in the libraries.
// Most functions that are marked with a 'no throw' (<code>throw()</code>) exception
// specification to indicate that the function does not throw exceptions also contain
// a catch (...) handler to ensure that they don't. This option allows you to remove
// these handlers. When disabled all catch (...) handlers in 'nothrow' functions are
// removed from library code.

// JETBYTE_EXCEPTION_STACK_TRACES
// Define to 1 to capture the stack strace when an exception derived from
// JetByteTools::Win32::CException is thrown. Enables the use of GetCallStack()
// to report the callstack at the point of the exception.

// JETBYTE_EXCEPTION_WHERE_INCLUDES_STACK_TRACE
// Only valid if JETBYTE_EXCEPTION_STACK_TRACES is defined to 1.
// Causes CException::Where() to also return the call stack of where the exception
// was thrown.

// JETBYTE_REFERENCE_TRACKING_DISPLAY_LOADED_PDBS
// Define to 1 to dump details of the pdb files loaded during the tracking of
// buffer or socket references.

// JETBYTE_REFERENCE_TRACKING_DISPLAY_ONLY_NON_TOOLS_LIB_CHANGES
// Define to 0 to show all reference changes, by default we only show changes
// which are triggered by code outside of the JetByteTools libs

// JETBYTE_TRACK_IO_BUFFER_REFERENCES
// Define to 1 to track where AddRef() and Release() are called from for instances
// of JetByteTools::IO::CBuffer. If a buffer is over released (i.e. release is called
// on a buffer where the reference count is currently zero) then a trace of all
// calls to AddRef() and Release() will be printed to the debug trace output. If
// a buffer is destroyed and the reference count is not zero then a trace is also
// printed. Note that there is a considerable run-time cost for this functionality.

// JETBYTE_ADDITIONAL_BUFFER_TRACKING_CONTEXT
// By default the buffer reference tracking code keeps one level of call stack context
// outside of the actual call within the I/O Tools lib code that caused the reference
// to change. This is often enough context to track down the problem and keeps the
// amount of memory required for storing call stacks down. Sometimes, however, you
// need a little more context, in those situations you should define
// JETBYTE_ADDITIONAL_BUFFER_TRACKING_CONTEXT to a value that represents the additional
// number of stack frames that you want saved and displayed. Note that each additional
// level of call stack that is stored requires a DWORD64 of space per call so these
// additional stack frames can soon add up.

// JETBYTE_TRACK_SOCKET_REFERENCES
// Define to 1 to track where AddRef() and Release() are called from for instances
// of JetByteTools::Socket::TAsyncSocket derived classes. If a socket is over
// released (i.e. release is called on a socket where the reference count is currently
// zero) then a trace of all calls to AddRef() and Release() will be printed to the
// debug trace output. If a socket is destroyed and the reference count is not zero
// then a trace is also printed. Note that there is a considerable run-time performance
// cost for this functionality.

// JETBYTE_ADDITIONAL_SOCKET_TRACKING_CONTEXT
// By default the socket reference tracking code keeps one level of call stack context
// outside of the actual call within the Socket Tools lib code that caused the reference
// to change. This is often enough context to track down the problem and keeps the
// amount of memory required for storing call stacks down. Sometimes, however, you
// need a little more context, in those situations you should define
// JETBYTE_ADDITIONAL_SOCKET_TRACKING_CONTEXT to a value that represents the additional
// number of stack frames that you want saved and displayed. Note that each additional
// level of call stack that is stored requires a DWORD64 of space per call so these
// additional stack frames can soon add up.

// JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE
// Define to 1 to throw exceptions from TReferenceCountedSmartPointer<T> dereferences
// if the reference is null. Normally we simply return the null reference and the
// calling code goes boom...

// JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE
// Define to 1 to generate a mini dump from TReferenceCountedSmartPointer<T> dereferences
// if the reference is null. Requires JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE

// JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE
// Define to 1 to throw exceptions from TConditionalSmartPointer<T> dereferences
// if the reference is null. Normally we simply return the null reference and the
// calling code goes boom...

// JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE
// Define to 1 to generate a mini dump from TConditionalSmartPointer<T> dereferences
// if the reference is null. Requires JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE

// JETBYTE_TINY_XML_2_6_2
// From 6.5 TinyXML version 2.6.2 is available and is used by default. If you
// would prefer to continue to use TinyXML 2.6.1 then define JETBYTE_TINY_XML_2_6_2
// to 0 in your Config.h

// JETBYTE_TINY_XML_2_6_1
// From 6.3 TinyXML version 2.6.1 is available and is used by default. If you
// would prefer to continue to use TinyXML 2.5.3 then define JETBYTE_TINY_XML_2_5_3
// to 1 in your Config.h

// JETBYTE_TINY_XML_2_5_3
// From 6.2 TinyXML version 2.5.3 is available and is used by default. If you
// would prefer to continue to use TinyXML 2.5.2 then define JETBYTE_TINY_XML_2_5_3
// to 0 in your Config.h

// JETBYTE_ZLIB_1_2_5
// From 6.5 ZLib version 1.2.5 is available and is used by default. If you
// would prefer to continue to use ZLib 1.2.3 then define JETBYTE_ZLIB_1_2_5
// to 0 in your Config.h

// JETBYTE_PERF_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS
// JETBYTE_PERF_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS
// JETBYTE_PERF_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS
// JETBYTE_PERF_FILE_READER_SKIP_COMPLETION_PORT_ON_SUCCESS
// These options enable FILE_SKIP_COMPLETION_PORT_ON_SUCCESS on the appropriate
// handles and potentially provide a performance improvement in situations where
// overlapped I/O operations complete straight away as the completion is handled on
// the thread that initiated the I/O operation rather than being marshalled to an
// I/O thread.

// JETBYTE_DEBUG_BREAK_ON_CONNECTIONS_ACTIVE_DURING_DESTRUCTION
// When enabled a breakpoint is triggered if you are in the debugger and there
// are connections active when a connection manager is destroyed.

// JETBYTE_DUMP_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
// When enabled the low level socket I/O code dumps complete details of all data
// sent and received to the default debug trace log.

// JETBYTE_DUMP_STREAM_SOCKET_READ_TO_DEBUG_LOG
// JETBYTE_DUMP_STREAM_SOCKET_WRITE_TO_DEBUG_LOG
// JETBYTE_DUMP_DATAGRAM_SOCKET_READ_TO_DEBUG_LOG
// JETBYTE_DUMP_DATAGRAM_SOCKET_WRITE_TO_DEBUG_LOG

// Try and do the right thing in case nobody bothers to manually configure these.
// They can be set either in TargetWindowsVersion.h or in Config.h...

#ifndef JETBYTE_HAS_SRW_LOCK_TRY_ENTER
#if (_WIN32_WINNT >= 0x0601)
#define JETBYTE_HAS_SRW_LOCK_TRY_ENTER 1
#endif
#endif

#include "Config.h"

// Fix up optional defines that may not have been defined in Config.h

#ifndef JETBYTE_ALLOW_UNTESTED_COMPILE_ENV
#define JETBYTE_ALLOW_UNTESTED_COMPILE_ENV 0
#endif

#ifdef JETBYTE_USE_STL_PORT
#error JETBYTE_USE_STL_PORT is no longer supported.
#endif

#ifndef JETBYTE_SHOW_TODO
#define JETBYTE_SHOW_TODO 0
#endif

#ifndef JETBYTE_NO_PLATFORM_SDK
#define JETBYTE_NO_PLATFORM_SDK 0
#endif

#ifdef JETBYTE_USE_TEMPLATE_TO_STRING
// JETBYTE_USE_TEMPLATE_TO_STRING
// In version 6.0 we changed how we convert numeric types to strings. Prior to this
// version we used strstream and templates and streamed the type into a string.
// However this required the acquisition of a process wide lock (for accessing
// locales and facets) in some versions of STL which was less than ideal for
// multi-threaded use. From version 6.0 we defaulted to using a custom sprintf based
// system to convert from numbers to strings. We also decided to change how 0 is
// represened in ToHexString(), it's converted to 0x0 in the new code whereas it
// was converted to 0 in the old. ToHexString() was also enhanced to allow for the
// user to decide if they want upper or lower case hex digits and we added
// PointerToString() which gives a consistent represenation of pointers across
// 32bit and 64bit operating systems (always representing pointers as 64bit hex
// values). To finish all of these things were moved out of Utils.h and into the
// ToString.h header file.
// Up until 6.4 you could switch back to the old strstream based implementation
// by defining JETBYTE_USE_TEMPLATE_TO_STRING as 1. From now on you can't.
// Some of the old functions can be found in OldToString.h but their use is
// unsupported.
#error The pre v6.0 ToString() functions are no longer supported. Use OldToString.h if you really must.
#endif

#ifndef JETBYTE_HAS_SRW_LOCK_TRY_ENTER
#define JETBYTE_HAS_SRW_LOCK_TRY_ENTER 0
#endif

#ifndef JETBYTE_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD
#define JETBYTE_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD 1
#endif

#ifndef JETBYTE_BREAK_SEH_EXCEPTION_TRANSLATOR_COMPATABILITY
#define JETBYTE_BREAK_SEH_EXCEPTION_TRANSLATOR_COMPATABILITY 1
#endif

#ifndef JETBYTE_TRANSLATE_SEH_EXCEPTIONS
#define JETBYTE_TRANSLATE_SEH_EXCEPTIONS 1
#endif

#ifndef JETBYTE_TRANSLATE_SEH_EXCEPTIONS_IN_DEBUGGER
#define JETBYTE_TRANSLATE_SEH_EXCEPTIONS_IN_DEBUGGER 1
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
#define JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED catch (...) { try{JetByteTools::Win32::OutputEx("Unexpected exception in destructor: " __FUNCTION__ );}catch (...){} }
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
#define JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED catch (...) { try{JetByteTools::Win32::OutputEx("Unexpected exception at thread boundary: " __FUNCTION__ );}catch (...){} }
#elif (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED catch (...) {}
#else
#define JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED catch (const JetByteTools::Admin::CDummyCatchAllException &) {}
#endif

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_IF_ENABLED catch (...) { try{JetByteTools::Win32::OutputEx("Unexpected exception: " __FUNCTION__ );}catch (...){} }
#elif (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_IF_ENABLED catch (...) {}
#else
#define JETBYTE_CATCH_AND_LOG_ALL_IF_ENABLED catch (const JetByteTools::Admin::CDummyCatchAllException &) {}
#endif

#define JETBYTE_TESTS_CATCH_ALL_AT_THREAD_BOUNDARY_IF_ENABLED catch (...)

#define JETBYTE_TESTS_CATCH_ALL_IF_ENABLED catch (...)

#ifndef JETBYTE_EXCEPTION_STACK_TRACES
#define JETBYTE_EXCEPTION_STACK_TRACES 0
#endif

#ifndef JETBYTE_EXCEPTION_WHERE_INCLUDES_STACK_TRACE
#define JETBYTE_EXCEPTION_WHERE_INCLUDES_STACK_TRACE 0
#endif

#ifndef JETBYTE_REFERENCE_TRACKING_DISPLAY_LOADED_PDBS
#define JETBYTE_REFERENCE_TRACKING_DISPLAY_LOADED_PDBS 0
#endif

#ifndef JETBYTE_REFERENCE_TRACKING_DISPLAY_ONLY_NON_TOOLS_LIB_CHANGES
#define JETBYTE_REFERENCE_TRACKING_DISPLAY_ONLY_NON_TOOLS_LIB_CHANGES 1
#endif

#ifndef JETBYTE_REFERENCE_TRACKING_SMART_POINTERS_NOISEY_REFERENCE_TRACKING
#define JETBYTE_REFERENCE_TRACKING_SMART_POINTERS_NOISEY_REFERENCE_TRACKING 0
#endif

#ifndef JETBYTE_TRACK_IO_BUFFER_REFERENCES
#define JETBYTE_TRACK_IO_BUFFER_REFERENCES 0
#endif

#ifndef JETBYTE_BUFFER_TRACKING_INSTANCES
#define JETBYTE_BUFFER_TRACKING_INSTANCES 1
#endif

#ifndef JETBYTE_ADDITIONAL_BUFFER_TRACKING_CONTEXT
#define JETBYTE_ADDITIONAL_BUFFER_TRACKING_CONTEXT 0
#endif

#ifndef JETBYTE_TRACK_SOCKET_REFERENCES
#define JETBYTE_TRACK_SOCKET_REFERENCES 0
#endif

#ifndef JETBYTE_SOCKET_TRACKING_INSTANCES
#define JETBYTE_SOCKET_TRACKING_INSTANCES 1
#endif

#ifndef JETBYTE_ADDITIONAL_SOCKET_TRACKING_CONTEXT
#define JETBYTE_ADDITIONAL_SOCKET_TRACKING_CONTEXT 0
#endif

#ifndef JETBYTE_TRACK_ADDRESS_REFERENCES
#define JETBYTE_TRACK_ADDRESS_REFERENCES 0
#endif

#ifndef JETBYTE_ADDRESS_TRACKING_INSTANCES
#define JETBYTE_ADDRESS_TRACKING_INSTANCES 1
#endif

#ifndef JETBYTE_ADDITIONAL_ADDRESS_TRACKING_CONTEXT
#define JETBYTE_ADDITIONAL_ADDRESS_TRACKING_CONTEXT 0
#endif

#ifndef JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE
#define JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE 0
#endif

#ifndef JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE
#define JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE 0
#endif

#if (JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE == 1)
#if (JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE == 0)
#error JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE requires JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE
#endif
#endif

#ifndef JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE
#define JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE 0
#endif

#ifndef JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE
#define JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE 0
#endif


#if (JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE == 1)
#if (JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE == 0)
#error JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE requires JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE
#endif
#endif

#if defined(JETBYTE_TINY_XML_2_5_2) && (JETBYTE_TINY_XML_2_5_2 == 0)          // Cannot explicitly disable 2.5.2
#error JETBYTE_TINY_XML_2_5_2 should not be explicitly disabled, explicitly enable another version to select an alternative
#endif

#if !defined(JETBYTE_TINY_XML_2_6_2)                                          // 2.6.2 is NOT explicitly enabled or disabled
   #if !defined(JETBYTE_TINY_XML_2_6_1) || (JETBYTE_TINY_XML_2_6_1 == 0)      // 2.6.1 is NOT explicitly enabled
      #if !defined(JETBYTE_TINY_XML_2_5_3) || (JETBYTE_TINY_XML_2_5_3 == 0)   // 2.5.3 is NOT explicitly enabled
         #if !defined(JETBYTE_TINY_XML_2_5_2) || (JETBYTE_TINY_XML_2_5_2 == 0)// 2.5.2 is NOT explicitly enabled
            #define JETBYTE_TINY_XML_2_6_2 1                                  // Default to 2.6.2
         #endif
      #endif
   #endif
#elif (JETBYTE_TINY_XML_2_6_2 == 0)                                           // 2.6.2 is explicitly disabled
   #if !defined(JETBYTE_TINY_XML_2_6_1)                                       // 2.6.1 is NOT explicitly enabled or disabled
      #if !defined(JETBYTE_TINY_XML_2_5_3) || (JETBYTE_TINY_XML_2_5_3 == 0)   // 2.5.3 is NOT explicitly enabled
         #if !defined(JETBYTE_TINY_XML_2_5_2) || (JETBYTE_TINY_XML_2_5_2 == 0)// 2.5.2 is NOT explicitly enabled
            #define JETBYTE_TINY_XML_2_6_1 1                                  // Default to 2.6.1
         #endif
      #endif
   #elif (JETBYTE_TINY_XML_2_6_1 == 0)                                        // 2.6.1 is explicitly disabled
      #if !defined(JETBYTE_TINY_XML_2_5_3)                                    // 2.5.3 is NOT explicitly enabled or disabled
         #if !defined(JETBYTE_TINY_XML_2_5_2) || (JETBYTE_TINY_XML_2_5_2 == 0)// 2.5.2 is NOT explicitly enabled
            #define JETBYTE_TINY_XML_2_5_3 1                                  // Default to 2.5.3
         #endif
      #endif
   #endif
#endif

#if defined(JETBYTE_ZLIB_1_2_3) && (JETBYTE_ZLIB_1_2_3 == 0)      // Cannot explicitly disable 1.2.3
#error JETBYTE_ZLIB_1_2_3 should not be explicitly disabled, explicitly enable another version to select an alternative
#endif

#if !defined(JETBYTE_ZLIB_1_2_5)                                  // 1.2.5 is NOT explicitly enabled or disabled
   #if !defined(JETBYTE_ZLIB_1_2_3) || (JETBYTE_ZLIB_1_2_3 == 0)  // 1.2.3 is NOT explicitly enabled
      #define JETBYTE_ZLIB_1_2_5 1                                // Default to 1.2.5
   #endif
#elif (JETBYTE_ZLIB_1_2_5 == 0)                                   // 1.2.5 is explicitly disabled
   // Default to 1.2.3
#endif

// By default we enable these performance improvements; you can disable them if you
// find they cause problems. See here for deatils: http://www.lenholgate.com/archives/000889.html

#ifndef JETBYTE_PERF_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS
#define JETBYTE_PERF_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS 1
#endif

#ifndef JETBYTE_STREAM_SOCKETS_DISPATCH_OTHER_SOCKETS_DURING_COMPLETION_HANDLING
#define JETBYTE_STREAM_SOCKETS_DISPATCH_OTHER_SOCKETS_DURING_COMPLETION_HANDLING 0
#endif


#ifndef JETBYTE_DATAGRAM_SOCKETS_DISPATCH_OTHER_SOCKETS_DURING_COMPLETION_HANDLING
#define JETBYTE_DATAGRAM_SOCKETS_DISPATCH_OTHER_SOCKETS_DURING_COMPLETION_HANDLING 0
#endif

// We don't enable this for datagram sockets at present as there is a potential
// issue with errors getting incorrectly reported (or NOT reported) when it is
// enabled. See here for details:
// http://blogs.technet.com/winserverperformance/archive/2008/06/26/designing-applications-for-high-performance-part-iii.aspx#3304073

#ifndef JETBYTE_PERF_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS
#define JETBYTE_PERF_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS 0
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

// Note that by enabling the various SKIP_COMPLETION_PORT_ON_SUCCESS options you
// potentially expose yourself to recursive calls where you are not exposed to these
// if you don't skip the completion port on success. For example, if you have a read
// complete "inline" on an I/O pool thread and all your work is done on the I/O
// threads then the next read to be issued will occur on that thread and that too
// could complete "inline" if there is network data waiting, you then issue another
// read, etc...

// You can limit the number of reads that are allowed to be recursively issued by
// setting the following values to non zero. Zero means "no limit". A non zero value
// will allow that number of recursive read calls before a read is dispatched to
// the I/O pool thus breaking the recursion.

#ifdef JETBYTE_PERF_STREAM_SOCKETS_LIMIT_IO_RECURSION_TO
#error JETBYTE_PERF_STREAM_SOCKETS_LIMIT_IO_RECURSION_TO is no longer required
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
#error JETBYTE_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS should not be defined by YOU it is internal, define JETBYTE_PERF_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS instead
#endif

#ifdef JETBYTE_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS
#error JETBYTE_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS should not be defined by YOU it is internal, define JETBYTE_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS instead
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

#ifdef JETBYTE_STREAM_SOCKET_FULL_DEBUG
#undef JETBYTE_DUMP_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#define JETBYTE_DUMP_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG 1
#undef JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#define JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG 1
#endif

#ifndef JETBYTE_DUMP_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#define JETBYTE_DUMP_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_DUMP_STREAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#define JETBYTE_DUMP_STREAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG JETBYTE_DUMP_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_DUMP_DATAGRAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#define JETBYTE_DUMP_DATAGRAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG JETBYTE_DUMP_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#define JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_TRACE_STREAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#define JETBYTE_TRACE_STREAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_TRACE_DATAGRAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#define JETBYTE_TRACE_DATAGRAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

////

#ifndef JETBYTE_DUMP_STREAM_SOCKET_READ_TO_DEBUG_LOG
#define JETBYTE_DUMP_STREAM_SOCKET_READ_TO_DEBUG_LOG JETBYTE_DUMP_STREAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_DUMP_STREAM_SOCKET_READ_TO_DEBUG_LOG_FULL_BUFFER_DUMPS
#define JETBYTE_DUMP_STREAM_SOCKET_READ_TO_DEBUG_LOG_FULL_BUFFER_DUMPS JETBYTE_DUMP_STREAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_DUMP_STREAM_SOCKET_WRITE_TO_DEBUG_LOG
#define JETBYTE_DUMP_STREAM_SOCKET_WRITE_TO_DEBUG_LOG JETBYTE_DUMP_STREAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_DUMP_STREAM_SOCKET_WRITE_TO_DEBUG_LOG_FULL_BUFFER_DUMPS
#define JETBYTE_DUMP_STREAM_SOCKET_WRITE_TO_DEBUG_LOG_FULL_BUFFER_DUMPS JETBYTE_DUMP_STREAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#define JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_TRACE_STREAM_SOCKET_READ_ERRORS_TO_DEBUG_LOG
#define JETBYTE_TRACE_STREAM_SOCKET_READ_ERRORS_TO_DEBUG_LOG JETBYTE_TRACE_STREAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_TRACE_STREAM_SOCKET_READ_TO_DEBUG_LOG
#define JETBYTE_TRACE_STREAM_SOCKET_READ_TO_DEBUG_LOG JETBYTE_TRACE_STREAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_TRACE_STREAM_SOCKET_WRITE_TO_DEBUG_LOG
#define JETBYTE_TRACE_STREAM_SOCKET_WRITE_TO_DEBUG_LOG JETBYTE_TRACE_STREAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_TRACE_STREAM_SOCKET_WRITE_ERRORS_TO_DEBUG_LOG
#define JETBYTE_TRACE_STREAM_SOCKET_WRITE_ERRORS_TO_DEBUG_LOG JETBYTE_TRACE_STREAM_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

///

#ifndef JETBYTE_DUMP_DATAGRAM_SOCKET_READ_TO_DEBUG_LOG
#define JETBYTE_DUMP_DATAGRAM_SOCKET_READ_TO_DEBUG_LOG JETBYTE_DUMP_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_DUMP_DATAGRAM_SOCKET_WRITE_TO_DEBUG_LOG
#define JETBYTE_DUMP_DATAGRAM_SOCKET_WRITE_TO_DEBUG_LOG JETBYTE_DUMP_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_TRACE_STREAM_SOCKET_INTERNAL_ABORTS
#define JETBYTE_TRACE_STREAM_SOCKET_INTERNAL_ABORTS 0
#endif

#ifndef JETBYTE_TRACE_DATAGRAM_SOCKET_READ_TO_DEBUG_LOG
#define JETBYTE_TRACE_DATAGRAM_SOCKET_READ_TO_DEBUG_LOG JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_TRACE_DATAGRAM_SOCKET_READ_ERRORS_TO_DEBUG_LOG
#define JETBYTE_TRACE_DATAGRAM_SOCKET_READ_ERRORS_TO_DEBUG_LOG JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_TRACE_DATAGRAM_SOCKET_WRITE_TO_DEBUG_LOG
#define JETBYTE_TRACE_DATAGRAM_SOCKET_WRITE_TO_DEBUG_LOG JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_TRACE_DATAGRAM_SOCKET_WRITE_ERRORS_TO_DEBUG_LOG
#define JETBYTE_TRACE_DATAGRAM_SOCKET_WRITE_ERRORS_TO_DEBUG_LOG JETBYTE_TRACE_SOCKET_READ_AND_WRITE_DATA_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_TRACE_PRE_PROCESSING_COMPLETIONS_TO_DEBUG_LOG
#define JETBYTE_TRACE_PRE_PROCESSING_COMPLETIONS_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_TRACE_STREAM_SOCKET_READ_TIMEOUT_CONNECTION_FILTER
#define JETBYTE_TRACE_STREAM_SOCKET_READ_TIMEOUT_CONNECTION_FILTER 0
#endif

#ifndef JETBYTE_TRACE_COMPRESSION_TO_DEBUG_LOG
#define JETBYTE_TRACE_COMPRESSION_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_TRACE_FLOW_CONTROL_TO_DEBUG_LOG
#define JETBYTE_TRACE_FLOW_CONTROL_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_TRACE_WEBSOCKET_READ_TO_DEBUG_LOG
#define JETBYTE_TRACE_WEBSOCKET_READ_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_DUMP_WEBSOCKET_READ_TO_DEBUG_LOG
#define JETBYTE_DUMP_WEBSOCKET_READ_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_TRACE_WEBSOCKET_WRITE_TO_DEBUG_LOG
#define JETBYTE_TRACE_WEBSOCKET_WRITE_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_DUMP_WEBSOCKET_WRITE_TO_DEBUG_LOG
#define JETBYTE_DUMP_WEBSOCKET_WRITE_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_TRACE_WEBSOCKET_INTERNAL_STATE_TO_DEBUG_LOG
#define JETBYTE_TRACE_WEBSOCKET_INTERNAL_STATE_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_LINK_TO_JB_NAME_FORMAT_OPENSSL_LIBS
#define JETBYTE_LINK_TO_JB_NAME_FORMAT_OPENSSL_LIBS 1
#endif

#ifndef JETBYTE_TRACE_OPENSSL_TO_DEBUG_LOG
#define JETBYTE_TRACE_OPENSSL_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_TRACE_OPENSSL_READ_TO_DEBUG_LOG
#define JETBYTE_TRACE_OPENSSL_READ_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_TRACE_OPENSSL_WRITE_TO_DEBUG_LOG
#define JETBYTE_TRACE_OPENSSL_WRITE_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_DUMP_OPENSSL_READ_TO_DEBUG_LOG
#define JETBYTE_DUMP_OPENSSL_READ_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_DUMP_OPENSSL_READ_TO_DEBUG_LOG_FULL_BUFFER_DUMPS
#define JETBYTE_DUMP_OPENSSL_READ_TO_DEBUG_LOG_FULL_BUFFER_DUMPS 0
#endif

#ifndef JETBYTE_DUMP_OPENSSL_ENC_READ_TO_DEBUG_LOG_FULL_BUFFER_DUMPS
#define JETBYTE_DUMP_OPENSSL_ENC_READ_TO_DEBUG_LOG_FULL_BUFFER_DUMPS 0
#endif

#ifndef JETBYTE_DUMP_OPENSSL_WRITE_TO_DEBUG_LOG
#define JETBYTE_DUMP_OPENSSL_WRITE_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_DUMP_OPENSSL_WRITE_TO_DEBUG_LOG_FULL_BUFFER_DUMPS
#define JETBYTE_DUMP_OPENSSL_WRITE_TO_DEBUG_LOG_FULL_BUFFER_DUMPS 0
#endif

#ifndef JETBYTE_DUMP_OPENSSL_ENC_WRITE_TO_DEBUG_LOG_FULL_BUFFER_DUMPS
#define JETBYTE_DUMP_OPENSSL_ENC_WRITE_TO_DEBUG_LOG_FULL_BUFFER_DUMPS 0
#endif

#ifndef JETBYTE_TRACE_SCHANNEL_READ_TO_DEBUG_LOG
#define JETBYTE_TRACE_SCHANNEL_READ_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_TRACE_SCHANNEL_WRITE_TO_DEBUG_LOG
#define JETBYTE_TRACE_SCHANNEL_WRITE_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_DUMP_SCHANNEL_READ_TO_DEBUG_LOG
#define JETBYTE_DUMP_SCHANNEL_READ_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_DUMP_SCHANNEL_WRITE_TO_DEBUG_LOG
#define JETBYTE_DUMP_SCHANNEL_WRITE_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_DUMP_SCHANNEL_READ_TO_DEBUG_LOG_FULL_BUFFER_DUMPS
#define JETBYTE_DUMP_SCHANNEL_READ_TO_DEBUG_LOG_FULL_BUFFER_DUMPS 0
#endif

#ifndef JETBYTE_DUMP_SCHANNEL_WRITE_TO_DEBUG_LOG_FULL_BUFFER_DUMPS
#define JETBYTE_DUMP_SCHANNEL_WRITE_TO_DEBUG_LOG_FULL_BUFFER_DUMPS 0
#endif

#ifndef JETBYTE_DUMP_SCHANNEL_ENC_READ_TO_DEBUG_LOG_FULL_BUFFER_DUMPS
#define JETBYTE_DUMP_SCHANNEL_ENC_READ_TO_DEBUG_LOG_FULL_BUFFER_DUMPS 0
#endif

#ifndef JETBYTE_DUMP_SCHANNEL_ENC_WRITE_TO_DEBUG_LOG_FULL_BUFFER_DUMPS
#define JETBYTE_DUMP_SCHANNEL_ENC_WRITE_TO_DEBUG_LOG_FULL_BUFFER_DUMPS 0
#endif

#ifndef JETBYTE_TRACE_SCHANNEL_CONTEXT
#define JETBYTE_TRACE_SCHANNEL_CONTEXT 0
#endif

#ifdef JETBYTE_DISABLE_VS2005_DEBUG_UUIDLIB_CHECK
#error JETBYTE_DISABLE_VS2005_DEBUG_UUIDLIB_CHECK is no longer supported. Apply the hotfix detailed here: http://www.lenholgate.com/archives/000864.html
#endif

#ifndef JETBYTE_PERF_TIMER_QUEUE_MONITORING
#define JETBYTE_PERF_TIMER_QUEUE_MONITORING 1
#endif

#ifndef JETBYTE_PERF_TIMER_CONTENTION_MONITORING
#define JETBYTE_PERF_TIMER_CONTENTION_MONITORING 0
#endif

#ifndef JETBYTE_PERF_TIMER_WHEEL_MONITORING
#define JETBYTE_PERF_TIMER_WHEEL_MONITORING 1
#endif

#ifndef JETBYTE_PERF_REUSE_DATAGRAM_SOCKET_AND_BUFFER_FOR_NEXT_READ_IF_POSSIBLE
#define JETBYTE_PERF_REUSE_DATAGRAM_SOCKET_AND_BUFFER_FOR_NEXT_READ_IF_POSSIBLE 0
#endif

#ifndef JETBYTE_PERF_NO_ACTIVE_BUFFER_LIST
#define JETBYTE_PERF_NO_ACTIVE_BUFFER_LIST 0
#elif (JETBYTE_PERF_NO_ACTIVE_BUFFER_LIST == 1)
#if (JETBYTE_TRACK_IO_BUFFER_REFERENCES == 1)
#error JETBYTE_TRACK_IO_BUFFER_REFERENCES is invalid if JETBYTE_PERF_NO_ACTIVE_BUFFER_LIST == 1, Cannot track buffer leaks
#endif
#endif

#ifndef JETBYTE_PERF_BUFFER_ALLOCATION_CONTENTION_MONITORING
#define JETBYTE_PERF_BUFFER_ALLOCATION_CONTENTION_MONITORING 0
#endif

#ifndef JETBYTE_DISABLE_POOLING_BUFFER_ALLOCATOR
#define JETBYTE_DISABLE_POOLING_BUFFER_ALLOCATOR 0
#endif

#ifndef JETBYTE_AUTOMATIC_SEH_CRASH_DUMP_CREATION
#define JETBYTE_AUTOMATIC_SEH_CRASH_DUMP_CREATION 0
#endif

#ifndef JETBYTE_PURE_CALL_CRASH_DUMP_CREATION
#define JETBYTE_PURE_CALL_CRASH_DUMP_CREATION 0
#endif

#ifndef JETBYTE_REF_TRACKING_INCLUDE_TIMESTAMP
#define JETBYTE_REF_TRACKING_INCLUDE_TIMESTAMP 1
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

#ifndef JETBYTE_DUMP_NAMED_INDEX_DETAILS_ON_LOCK
#define JETBYTE_DUMP_NAMED_INDEX_DETAILS_ON_LOCK 0
#endif

#ifndef JETBYTE_ILLEGAL_SOCKET_REFERENCE_COUNT_CRASH_DUMP_CREATION
#define JETBYTE_ILLEGAL_SOCKET_REFERENCE_COUNT_CRASH_DUMP_CREATION 0
#endif

#ifndef JETBYTE_CRASH_DUMP_ON_NEXT_SOCKET_REF_CHANGE_AFTER_ILLEGAL_CHANGE
#define JETBYTE_CRASH_DUMP_ON_NEXT_SOCKET_REF_CHANGE_AFTER_ILLEGAL_CHANGE 0
#endif

#ifdef JETBYTE_DEFAULT_PURE_CALL_HANDLER_INSTALL_ON_EARLY_COMPILERS
#error JETBYTE_DEFAULT_PURE_CALL_HANDLER_INSTALL_ON_EARLY_COMPILERS is no longer supported
#endif

#ifndef JETBYTE_REENTRANT_SINGLE_WRITER_MULTIPLE_READER_LOCK_2_MAX_THREADS
#define JETBYTE_REENTRANT_SINGLE_WRITER_MULTIPLE_READER_LOCK_2_MAX_THREADS 100
#endif

#ifndef JETBYTE_TRACK_THREAD_NAMES
#define JETBYTE_TRACK_THREAD_NAMES 0
#endif

#ifndef JETBYTE_DEPRECATE_HIXIE_WEBSOCKETS
#define JETBYTE_DEPRECATE_HIXIE_WEBSOCKETS 1
#endif

#if JETBYTE_DEPRECATE_HIXIE_WEBSOCKETS != 0
#ifndef JETBYTE_WEBSOCKET_ALLOW_BAD_SPACES_IN_HIXIE76_HANDSHAKE
#define JETBYTE_WEBSOCKET_ALLOW_BAD_SPACES_IN_HIXIE76_HANDSHAKE 0
#endif
#endif

#ifndef JETBYTE_WEBSOCKET_ALLOW_MISSING_UPGRADE_HEADER_IN_HANDSHAKE
#define JETBYTE_WEBSOCKET_ALLOW_MISSING_UPGRADE_HEADER_IN_HANDSHAKE 0
#endif

#ifndef JETBYTE_WEBSOCKET_ALLOW_MISSING_CONNECTION_HEADER_IN_HANDSHAKE
#define JETBYTE_WEBSOCKET_ALLOW_MISSING_CONNECTION_HEADER_IN_HANDSHAKE 0
#endif

#ifndef JETBYTE_TERMINATE_CRASH_DUMP_CREATION
#define JETBYTE_TERMINATE_CRASH_DUMP_CREATION 0
#endif

#ifndef JETBYTE_UNEXPECTED_CRASH_DUMP_CREATION
#define JETBYTE_UNEXPECTED_CRASH_DUMP_CREATION 0
#endif

#ifndef JETBYTE_DEPRECATE_TYPE_SAFE_TYPEDEF_CAST_TO_BASE_TYPE
#define JETBYTE_DEPRECATE_TYPE_SAFE_TYPEDEF_CAST_TO_BASE_TYPE 1
#endif

#ifndef JETBYTE_DEPRECATE_CRITICAL_SECTION
#define JETBYTE_DEPRECATE_CRITICAL_SECTION 0
#endif

#ifndef JETBYTE_DEPRECATE_MULTI_CRITICAL_SECTION
#define JETBYTE_DEPRECATE_MULTI_CRITICAL_SECTION 0
#endif

#ifndef JETBYTE_DEPRECATE_IO_TOOLS_CODE_USED_IN_WEBSOCKETS_TOOLS
#if JETBYTE_DEPRECATE_HIXIE_WEBSOCKETS == 1
#define JETBYTE_DEPRECATE_IO_TOOLS_CODE_USED_IN_WEBSOCKETS_TOOLS 1
#else
#define JETBYTE_DEPRECATE_IO_TOOLS_CODE_USED_IN_WEBSOCKETS_TOOLS 0
#endif
#endif

#ifndef JETBYTE_DEPRECATE_SOCKET_READ_AGAIN
#define JETBYTE_DEPRECATE_SOCKET_READ_AGAIN JETBYTE_DEPRECATE_HIXIE_WEBSOCKETS
#endif

#ifndef JETBYTE_DEPRECATE_BUFFER_PROCESSOR
#define JETBYTE_DEPRECATE_BUFFER_PROCESSOR 0
#endif

#ifndef JETBYTE_DEPRECATE_SORTED_BUFFER_PROCESSOR
#define JETBYTE_DEPRECATE_SORTED_BUFFER_PROCESSOR 0
#endif

#ifndef JETBYTE_DEPRECATE_LOCKABLE_BUFFER_PROCESSOR
#define JETBYTE_DEPRECATE_LOCKABLE_BUFFER_PROCESSOR JETBYTE_DEPRECATE_IO_TOOLS_CODE_USED_IN_WEBSOCKETS_TOOLS
#endif

#ifndef JETBYTE_DEPRECATE_LOCKABLE_SORTED_BUFFER_PROCESSOR
#define JETBYTE_DEPRECATE_LOCKABLE_SORTED_BUFFER_PROCESSOR JETBYTE_DEPRECATE_IO_TOOLS_CODE_USED_IN_WEBSOCKETS_TOOLS
#endif

#ifndef JETBYTE_DEPRECATE_UNSORTED_BUFFER_PROCESSOR
#define JETBYTE_DEPRECATE_UNSORTED_BUFFER_PROCESSOR JETBYTE_DEPRECATE_IO_TOOLS_CODE_USED_IN_WEBSOCKETS_TOOLS
#endif

#ifndef JETBYTE_DEPRECATE_INPUT_FILTER_PROCESSOR
#define JETBYTE_DEPRECATE_INPUT_FILTER_PROCESSOR 1
#endif

#ifndef JETBYTE_DEPRECATE_STACKABLE_WRITE_FILTERS
#define JETBYTE_DEPRECATE_STACKABLE_WRITE_FILTERS 1
#endif

#ifndef JETBYTE_DEPRECATE_REGISTRY_CONFIGURATION
#define JETBYTE_DEPRECATE_REGISTRY_CONFIGURATION 1
#endif

#if JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 0
#define JETBYTE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS 1
#endif

#ifndef JETBYTE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS
#define JETBYTE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS 0
#endif

#ifndef JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE
#define JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE 0
#endif

#ifndef JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_DEBUG_BREAK
#define JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_DEBUG_BREAK 0
#endif

#ifndef JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP
#define JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP 0
#endif

#ifndef JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION
#define JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION 1
#endif

#if (JETBYTE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS == 0 && JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1 && JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION == 0)
#undef JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION
#define JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION 1
#define JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_OVERRIDE_EXCEPTION 1
#endif

#ifndef JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_OVERRIDE_EXCEPTION
#define JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_OVERRIDE_EXCEPTION 0
#endif

#ifndef JETBYTE_SORTED_BUFFER_CHAIN_GENERATE_CRASH_DUMP_ON_DUPLICATE_BUFFER
#define JETBYTE_SORTED_BUFFER_CHAIN_GENERATE_CRASH_DUMP_ON_DUPLICATE_BUFFER 0
#endif

#ifndef JETBYTE_SORTED_BUFFER_CHAIN_GENERATE_CRASH_DUMP_ON_MULTIPLE_LISTS
#define JETBYTE_SORTED_BUFFER_CHAIN_GENERATE_CRASH_DUMP_ON_MULTIPLE_LISTS 0
#endif

#ifndef JETBYTE_MINI_DUMP_GENERATOR_BREAK_INTO_DEBUGGER
#define JETBYTE_MINI_DUMP_GENERATOR_BREAK_INTO_DEBUGGER 0
#endif

#ifndef JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED
#define JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED 1
#endif

#ifdef _MSVC_LANG
#if _MSVC_LANG > 201402
#ifndef JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED
#define JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED 0
#elif JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED == 1
#error JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED cannot be enabled if C++/17 is being used
#endif
#endif
#endif

#ifndef JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED
#define JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED 1
#endif

#ifndef JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED
#define JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED 1
#endif

#ifndef JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED
#define JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED 1
#endif

#ifndef JETBYTE_GLOBAL_ERROR_HANDLER_BREAK_IF_DEBUGGER_PRESENT
#define JETBYTE_GLOBAL_ERROR_HANDLER_BREAK_IF_DEBUGGER_PRESENT 1
#endif

#ifndef JETBYTE_GLOBAL_ERROR_HANDLER_SIG_ABORT_CRASH_DUMP_CREATION
#define JETBYTE_GLOBAL_ERROR_HANDLER_SIG_ABORT_CRASH_DUMP_CREATION 1
#endif

#ifndef JETBYTE_GLOBAL_ERROR_HANDLER_HANG_ON_TERMINATE
#define JETBYTE_GLOBAL_ERROR_HANDLER_HANG_ON_TERMINATE 0
#endif

#ifndef JETBYTE_GLOBAL_ERROR_HANDLER_HANG_ON_SIGABRT
#define JETBYTE_GLOBAL_ERROR_HANDLER_HANG_ON_SIGABRT 0
#endif

#ifndef JETBYTE_GLOBAL_ERROR_HANDLER_HANG_ON_UNEXPECTED
#define JETBYTE_GLOBAL_ERROR_HANDLER_HANG_ON_UNEXPECTED 0
#endif

#ifndef JETBYTE_CRT_REPORT_HOOK_HANG_ON_ERROR
#define JETBYTE_CRT_REPORT_HOOK_HANG_ON_ERROR 0
#endif

#ifndef JETBYTE_PURE_CALL_BREAK_INTO_DEBUGGER_IF_PRESENT
#define JETBYTE_PURE_CALL_BREAK_INTO_DEBUGGER_IF_PRESENT 0
#endif

#ifndef JETBYTE_PURE_CALL_BREAK_INTO_DEBUGGER_IF_PRESENT
#define JETBYTE_PURE_CALL_BREAK_INTO_DEBUGGER_IF_PRESENT 0
#endif

#ifndef JETBYTE_PURE_CALL_HANG_ON_ERROR
#define JETBYTE_PURE_CALL_HANG_ON_ERROR 0
#endif

#ifndef JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE
#define JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE 0
#endif

#ifndef JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION
#define JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION 0
#endif

#ifndef JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS
#define JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS 0
#endif

#ifndef JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED
#define JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED 0
#endif

#ifndef JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION
#define JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION 0
#endif

#ifndef JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS
#define JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS 0
#endif

#ifndef JETBYTE_MULTIPLE_RANGE_REUSABLE_ID_MANAGER_VALIDATE_FREED_IDS
#define JETBYTE_MULTIPLE_RANGE_REUSABLE_ID_MANAGER_VALIDATE_FREED_IDS 1
#endif

#ifndef JETBYTE_TRACE_FILTER_BASE_READ_TO_DEBUG_LOG
#define JETBYTE_TRACE_FILTER_BASE_READ_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_FILTER_BASE_ALL_CONNECTIONS_ABORTED_CRASH_DUMP_CREATION
#define JETBYTE_FILTER_BASE_ALL_CONNECTIONS_ABORTED_CRASH_DUMP_CREATION 0
#endif

#ifndef JETBYTE_TRACE_FILTER_BASE_TO_DEBUG_LOG
#define JETBYTE_TRACE_FILTER_BASE_TO_DEBUG_LOG 0
#endif

#ifndef JETBYTE_TRACE_FILTER_BASE_WRITE_TO_DEBUG_LOG
#define JETBYTE_TRACE_FILTER_BASE_WRITE_TO_DEBUG_LOG JETBYTE_TRACE_FILTER_BASE_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_TRACE_FILTER_BASE_READ_TO_DEBUG_LOG
#define JETBYTE_TRACE_FILTER_BASE_READ_TO_DEBUG_LOG JETBYTE_TRACE_FILTER_BASE_TO_DEBUG_LOG
#endif

#ifndef JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES
#define JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES 1
#endif

#ifndef JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES
#define JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES 1
#endif

#ifndef JETBYTE_DEPRECATE_STACKABLE_FILTERS
#define JETBYTE_DEPRECATE_STACKABLE_FILTERS 1
#endif

#ifndef JETBYTE_TRACE_SERVICE_STATE_CHANGES
#define JETBYTE_TRACE_SERVICE_STATE_CHANGES 0
#endif

#ifndef JETBYTE_TRACE_SERVICE_FAILURE_ACTION_DETECTION
#define JETBYTE_TRACE_SERVICE_FAILURE_ACTION_DETECTION 0
#endif

// Define this to 1 if you have atl installed

#ifndef JETBYTE_HAS_ATL_HEADERS_INSTALLED
#define JETBYTE_HAS_ATL_HEADERS_INSTALLED 0
#endif

#ifndef JETBYTE_SHOW_ADMIN_CONFIG_DETAILS
#define JETBYTE_SHOW_ADMIN_CONFIG_DETAILS 0
#endif

#ifndef JETBYTE_RANDOM_BYTE_PROVIDER_ENUMERATE_ALL_PROVIDERS
#define JETBYTE_RANDOM_BYTE_PROVIDER_ENUMERATE_ALL_PROVIDERS 0
#endif

#ifndef JETBYTE_PERFMON_CHECK_COUNTER_ALIGNMENT
#define JETBYTE_PERFMON_CHECK_COUNTER_ALIGNMENT 1
#endif

#ifndef JETBYTE_PERFMON_VALIDATE_COUNTER_SIZES
#define JETBYTE_PERFMON_VALIDATE_COUNTER_SIZES 1
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Warnings.h"
#include "Todo.h"

// Prevent wierd "some of it is inline some of it is not" linkage issues when using
// "standard" Microsoft supplied STL

#define _WCTYPE_INLINE_DEFINED

// The following macro "SuppressLNK4221Warning()" can be put into a file
// in order suppress the MS Visual C++ Linker warning 4221
//
// warning LNK4221: no public symbols found; archive member will be inaccessible
//
// This warning occurs when a file compiles out completely
// has no externally visible symbols which may be dependant on configuration
// #defines and options.

#define SuppressLNK4221Warning()   namespace { char NoEmptyFileDummy##__LINE__; }

#if (_MSC_VER > 1900)
#define JETBYTE_NOEXCEPT noexcept
#else
#define JETBYTE_NOEXCEPT throw()
#endif

#endif // JETBYTE_TOOLS_ADMIN_ADMIN_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Admin.h
///////////////////////////////////////////////////////////////////////////////
