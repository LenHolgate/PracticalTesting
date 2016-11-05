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

#include "JetByteTools\Admin\DetectPlatformSDKVersion.h"

#ifdef JETBYTE_HAS_INTERLOCKED_64
#undef JETBYTE_HAS_INTERLOCKED_64
#endif 

#define JETBYTE_HAS_INTERLOCKED_64 1

#define JETBYTE_PERF_TIMER_QUEUE_MONITORING_DISABLED 0
#define JETBYTE_PERF_TIMER_WHEEL_MONITORING_DISABLED 0

#define JETBYTE_TEST_SHOW_INDIVIDUAL_TIMES 0
#define JETBYTE_TEST_SHOW_NUM_TIMER_EVENTS 0
#define JETBYTE_PERF_NUM_TEST_RUNS 10

#endif // JETBYTE_TOOLS_ADMIN_CONFIG_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Config.h
///////////////////////////////////////////////////////////////////////////////