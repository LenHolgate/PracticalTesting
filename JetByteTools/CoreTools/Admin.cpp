///////////////////////////////////////////////////////////////////////////////
// File: Admin.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2018 JetByte Limited.
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

#pragma hdrstop

#ifndef JETBYTE_TOOLS_CONFIG_CORE
#error You MUST include CoreTools/Admin.h in your Admin/Config.h file to be able to use CoreTools
#endif

#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools:---------------------------------------------------------------")

#if JETBYTE_SHOW_CORE_CONFIG_DETAILS == 0

#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools:")
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: Define JETBYTE_SHOW_CORE_CONFIG_DETAILS to 1 in Admin/Config.h")
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: before you include CoreTools/Admin.h to display all configuration defines")

#else

#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools:")
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: Define JETBYTE_SHOW_CORE_CONFIG_DETAILS to 0 in Admin/Config.h")
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: before you include CoreTools/Admin.h to turn off the display")
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: of the following configuration defines")
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools:")

#if JETBYTE_CORE_THREAD_USE_STD_THREADING
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_CORE_THREAD_USE_STD_THREADING enabled: YES")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_CORE_THREAD_USE_STD_THREADING enabled: NO")
#endif

#if JETBYTE_CORE_ATOMIC_USE_STD_ATOMIC
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_CORE_ATOMIC_USE_STD_ATOMIC enabled: YES")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_CORE_ATOMIC_USE_STD_ATOMIC enabled: NO")
#endif

#if (JETBYTE_TRANSLATE_SEH_EXCEPTIONS == 1)
#if (JETBYTE_TRANSLATE_SEH_EXCEPTIONS_IN_DEBUGGER == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: Translate SEH exceptions and catch them at thread boundaries")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: Translate SEH exceptions and catch them at thread boundaries unless debugger present")
#endif
#else
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: Do NOT translate SEH exceptions, allow them to propogate and remain uncaught")
#endif

#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_EXCEPTION_STACK_TRACES enabled: YES")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_EXCEPTION_STACK_TRACES enabled: NO")
#endif

#if (JETBYTE_REFERENCE_TRACKING_DISPLAY_LOADED_PDBS == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_REFERENCE_TRACKING_DISPLAY_LOADED_PDBS enabled: YES")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_REFERENCE_TRACKING_DISPLAY_LOADED_PDBS enabled: NO")
#endif

#if (JETBYTE_REFERENCE_TRACKING_DISPLAY_ONLY_NON_TOOLS_LIB_CHANGES == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_REFERENCE_TRACKING_DISPLAY_ONLY_NON_TOOLS_LIB_CHANGES enabled: YES")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_REFERENCE_TRACKING_DISPLAY_ONLY_NON_TOOLS_LIB_CHANGES enabled: NO")
#endif

#if (JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: YES")
#if (JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE enabled: YES")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE enabled: NO")
#endif
#else
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: NO")
#endif

#if (JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: YES")
#if (JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: CoreTools: JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE enabled: NO")
#endif
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: NO")
#endif

#if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_PERF_TIMER_QUEUE_MONITORING enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_PERF_TIMER_QUEUE_MONITORING enabled: NO")
#endif

#if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_PERF_TIMER_WHEEL_MONITORING enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_PERF_TIMER_WHEEL_MONITORING enabled: NO")
#endif

#if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_PERF_TIMER_CONTENTION_MONITORING enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_PERF_TIMER_CONTENTION_MONITORING enabled: NO")
#endif

#if (JETBYTE_CORE_TRACK_THREAD_NAMES == 1)
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_TRACK_THREAD_NAMES enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_TRACK_THREAD_NAMES enabled: NO")
#endif

#if (JETBYTE_TERMINATE_CRASH_DUMP_CREATION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_TERMINATE_CRASH_DUMP_CREATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_TERMINATE_CRASH_DUMP_CREATION enabled: NO")
#endif

#if (JETBYTE_UNEXPECTED_CRASH_DUMP_CREATION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_UNEXPECTED_CRASH_DUMP_CREATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_UNEXPECTED_CRASH_DUMP_CREATION enabled: NO")
#endif

#if JETBYTE_DEPRECATE_TYPE_SAFE_TYPEDEF_CAST_TO_BASE_TYPE == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_DEPRECATE_TYPE_SAFE_TYPEDEF_CAST_TO_BASE_TYPE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_DEPRECATE_TYPE_SAFE_TYPEDEF_CAST_TO_BASE_TYPE enabled: NO")
#endif

#if JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS enabled: YES")
#if JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 0
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: WARNING - JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS forced due to JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 0")
#endif
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS enabled: NO")
#endif

#if JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE enabled: YES")
#if JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_DEBUG_BREAK == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_DEBUG_BREAK enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_DEBUG_BREAK enabled: NO")
#endif
#if JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP enabled: NO")
#endif
#if JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION enabled: NO")
#endif
#if JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_OVERRIDE_EXCEPTION == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: WARNING - JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION forcibly enabled due SRWL use")
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: WARNING - Set JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS to 1 if you want to test for recursive")
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: WARNING - behaviour without causing fatal runtime errors.")
#endif
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE enabled: NO")
#endif

#if JETBYTE_MINI_DUMP_GENERATOR_BREAK_INTO_DEBUGGER == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_MINI_DUMP_GENERATOR_BREAK_INTO_DEBUGGER enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_MINI_DUMP_GENERATOR_BREAK_INTO_DEBUGGER enabled: NO")
#endif

#if JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED enabled: NO")
#endif

#if JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED enabled: NO")
#endif

#if JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED enabled: NO")
#endif

#if JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS enabled: NO")
#endif

#if JETBYTE_MULTIPLE_RANGE_REUSABLE_ID_MANAGER_VALIDATE_FREED_IDS == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_MULTIPLE_RANGE_REUSABLE_ID_MANAGER_VALIDATE_FREED_IDS enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_MULTIPLE_RANGE_REUSABLE_ID_MANAGER_VALIDATE_FREED_IDS enabled: NO")
#endif

#if JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES enabled: NO")
#endif

#if JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES enabled: NO")
#endif

#if JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_MD5 == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_MD5 enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_MD5 enabled: NO")
#endif

#if JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_SHA1 == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_SHA1 enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_SHA1 enabled: NO")
#endif

#if JETBYTE_CORE_TOOLS_CALL_STACK_SUPPORTS_HASH == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_TOOLS_CALL_STACK_SUPPORTS_HASH enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_TOOLS_CALL_STACK_SUPPORTS_HASH enabled: NO")
#endif

#if JETBYTE_CORE_TOOLS_STACK_WALKING_SUPPORTED == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_TOOLS_STACK_WALKING_SUPPORTED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_TOOLS_STACK_WALKING_SUPPORTED enabled: NO")
#endif

#if JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32C == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32C enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32C enabled: NO")
#endif

#if JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32 == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32 enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32 enabled: NO")
#endif

#if JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH enabled: NO")
#endif

#if JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_UTF8 == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_UTF8 enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_UTF8 enabled: NO")
#endif

#if JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_BASE64 == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_BASE64 enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_BASE64 enabled: NO")
#endif

#if JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_MD5 == 0
#if JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5 == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5 enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5 enabled: NO")
#ifdef JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: Enable JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_MD5 to use MD5 with no third-party code")
#endif
#endif
#endif

#if JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_SHA1 == 0
#if JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1 == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1 enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1 enabled: NO")
#ifdef JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: Enable JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_SHA1 to use SHA1 with no third-party code")
#endif
#endif
#endif

#if JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_OPTEX == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_OPTEX enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_OPTEX enabled: NO")
#endif

#if JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_SWMR_LOCK == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_SWMR_LOCK enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_SWMR_LOCK enabled: NO")
#endif

#ifndef JETBYTE_TOOLS_ADMIN_UNIX_PLATFORM
#if JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_PEVENTS == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_PEVENTS enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_PEVENTS enabled: NO")
#endif
#endif

#if JETBYTE_CORE_ENABLE_DEBUG_TRACE_OUTPUT == 1
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_ENABLE_DEBUG_TRACE_OUTPUT enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: CoreTools: JETBYTE_CORE_ENABLE_DEBUG_TRACE_OUTPUT enabled: NO")
#endif


#endif

#pragma JETBYTE_MESSAGE("Build configuration: CoreTools:---------------------------------------------------------------")
#pragma JETBYTE_MESSAGE("")
#pragma JETBYTE_MESSAGE("")
#pragma JETBYTE_MESSAGE("")

SuppressNoSymbolsFoundWarning()

///////////////////////////////////////////////////////////////////////////////
// End of file: Admin.cpp
///////////////////////////////////////////////////////////////////////////////
