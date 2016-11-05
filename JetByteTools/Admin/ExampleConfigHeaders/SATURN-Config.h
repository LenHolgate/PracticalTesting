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

/// \file Config.h
/// This file is included by Admin.h and is used to set the various compilation
/// options that are related to your compile environment.
/// We use a separate header file for this so that it's easy to have multiple
/// copies for different target environments so that the continuous integration
/// build can copy a single file around to change the target environment.
/// \ingroup Admin

#define JETBYTE_AUTOMATIC_SEH_CRASH_DUMP_CREATION 1
#define JETBYTE_PURE_CALL_CRASH_DUMP_CREATION 1
#define JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE 1

#define JETBYTE_USE_STL_PORT 0
//#define JETBYTE_ALLOW_DEPRECATED_COMPILE_ENV 1

#define JETBYTE_DEPRECATE_IN_ORDER_BUFFER_LIST 0
#define JETBYTE_DEPRECATE_SEQUENTIAL_BUFFER_LIST 0
#define JETBYTE_DEPRECATE_SHARED_LOCK_SOCKETS 0
#define JETBYTE_DEPRECATE_SHARED_CRITICAL_SECTIONS 0
#define JETBYTE_DEPRECATE_CRITICAL_SECTION 0
#define JETBYTE_DEPRECATE_CRITICAL_SECTION_2 0
#define JETBYTE_DEPRECATE_LOW_CONTENTION_BUFFER_ALLOCATOR 0
#define JETBYTE_DEPRECATE_TLS_BUFFER_ALLOCATOR 0
#define JETBYTE_DEPRECATE_DEFLATING_STREAM_SOCKET_FILTER 0

#if (_MSC_VER != 1400)     // when building with no platform sdk, vs2005 doesnt have this header
#include "JetByteTools\Admin\DetectPlatformSDKVersion.h"
#endif

#ifdef JETBYTE_HAS_INTERLOCKED_64
#undef JETBYTE_HAS_INTERLOCKED_64
#endif

#define JETBYTE_HAS_INTERLOCKED_64 0

#define JETBYTE_PERF_STREAM_SOCKETS_SKIP_EVENT_ON_HANDLE 0

#define JETBYTE_PERF_STREAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS 0

#define JETBYTE_PERF_DATAGRAM_SOCKETS_SKIP_EVENT_ON_HANDLE 0

#define JETBYTE_PERF_DATAGRAM_SOCKETS_SKIP_COMPLETION_PORT_ON_SUCCESS 0

#define JETBYTE_PERF_FILE_WRITER_SKIP_EVENT_ON_HANDLE 0

#define JETBYTE_PERF_FILE_WRITER_SKIP_COMPLETION_PORT_ON_SUCCESS 0

#define JETBYTE_AUTOMATIC_SEH_CRASH_DUMP_CREATION 1
#define JETBYTE_PURE_CALL_CRASH_DUMP_CREATION 1

#endif // JETBYTE_TOOLS_ADMIN_CONFIG_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Config.h
///////////////////////////////////////////////////////////////////////////////
