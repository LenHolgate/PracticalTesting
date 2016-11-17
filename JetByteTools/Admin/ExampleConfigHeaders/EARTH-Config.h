#pragma once
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

#include "JetByteTools\Admin\DetectPlatformSDKVersion.h"

#define JETBYTE_AUTOMATIC_SEH_CRASH_DUMP_CREATION 1
#define JETBYTE_PURE_CALL_CRASH_DUMP_CREATION 1
#define JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE 1

#define JETBYTE_SHOW_TODO 1

#define JETBYTE_AUTOMATIC_SEH_CRASH_DUMP_CREATION 1
#define JETBYTE_PURE_CALL_CRASH_DUMP_CREATION 1

#define JETBYTE_DEPRECATE_MULTI_BUFFER_HANDLE 0

#define JETBYTE_SHOW_ADMIN_CONFIG_DETAILS 1

#endif // JETBYTE_TOOLS_ADMIN_CONFIG_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Config.h
///////////////////////////////////////////////////////////////////////////////
