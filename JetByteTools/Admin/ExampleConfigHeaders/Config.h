#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_ADMIN_CONFIG_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_CONFIG_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: Config.h 
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

/// \file ExampleConfigHeaders\Config.h
/// This file is included by Admin.h and is used to set the various compilation
/// options that are related to your compile environment.
/// We use a separate header file for this so that it's easy to have multiple
/// copies for different target environments so that the continuous integration
/// build can copy a single file around to change the target environment.
/// \ingroup Admin

//#define JETBYTE_USE_STL_PORT 1

#ifdef JETBYTE_PLATFORM_SDK_VERSION
#undef JETBYTE_PLATFORM_SDK_VERSION
#endif

//#define JETBYTE_PLATFORM_SDK_VERSION 0x0610

#ifdef JETBYTE_HAS_INTERLOCKED_64
#undef JETBYTE_HAS_INTERLOCKED_64
#endif 

#define JETBYTE_HAS_INTERLOCKED_64 0

//#define JETBYTE_SHOW_TODO 1

//#define JETBYTE_WARN_ON_WSPIAPI_COUNTOF_DEF 1

//#define JETBYTE_WARN_ON_SOCKADDR_STORAGE_DEF 1

//#define JETBYTE_ALLOW_UNTESTED_COMPILE_ENV 1

//#define JETBYTE_USE_TEMPLATE_TO_STRING 1

//#define JETBYTE_TRANSLATE_SEH_EXCEPTIONS 0

//#define JETBYTE_TRANSLATE_SEH_EXCEPTIONS_IN_DEBUGGER 0

//#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS 0
// OR some or all of these...
//#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS 0
//#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY 0
//#define JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_NOTHROW_FUNCTIONS 0

//#define JETBYTE_EXCEPTION_STACK_TRACES 1
//#define JETBYTE_EXCEPTION_WHERE_INCLUDES_STACK_TRACE 0

//#define JETBYTE_REFERENCE_TRACKING_DISPLAY_LOADED_PDBS 1

//#define JETBYTE_TRACK_IO_BUFFER_REFERENCES 1
//#define JETBYTE_ADDITIONAL_BUFFER_TRACKING_CONTEXT 2

//#define JETBYTE_TRACK_SOCKET_REFERENCES 1
//#define JETBYTE_ADDITIONAL_SOCKET_TRACKING_CONTEXT 2

//#define JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE 1

//#define JETBYTE_TINY_XML_2_5_3 0

//#define JETBYTE_PERF_STREAM_SOCKETS_SKIP_EVENT_ON_HANDLE 0

//#define JETBYTE_PERF_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS 0

//#define JETBYTE_PERF_DATAGRAM_SOCKETS_SKIP_EVENT_ON_HANDLE 0

#define JETBYTE_PERF_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS 1

//#define JETBYTE_PERF_FILE_WRITER_SKIP_EVENT_ON_HANDLE 0

#define JETBYTE_PERF_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS 1

//#define JETBYTE_DEBUG_BREAK_ON_CONNECTIONS_ACTIVE_DURING_DESTRUCTION 0

#endif // JETBYTE_TOOLS_ADMIN_CONFIG_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Config.h
///////////////////////////////////////////////////////////////////////////////
