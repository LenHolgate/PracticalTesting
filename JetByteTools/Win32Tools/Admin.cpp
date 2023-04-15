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

#ifndef JETBYTE_TOOLS_CONFIG_WIN32
#error You MUST include Win32Tools/Admin.h in your Admin/Config.h file to be able to use Win32Tools
#endif

#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools:---------------------------------------------------------------")

#if JETBYTE_SHOW_WIN32_CONFIG_DETAILS == 0

#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools:")
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: Define JETBYTE_SHOW_WIN32_CONFIG_DETAILS to 1 in Admin/Config.h")
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: before you include Win32Tools/Admin.h to display all configuration defines")

#else

#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools:")
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: Define JETBYTE_SHOW_WIN32_CONFIG_DETAILS to 0 in Admin/Config.h")
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: before you include Win32Tools/Admin.h to turn off the display")
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: of the following configuration defines")
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools:")

#if (JETBYTE_TRANSLATE_SEH_EXCEPTIONS == 1)
#if (JETBYTE_TRANSLATE_SEH_EXCEPTIONS_IN_DEBUGGER == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: Translate SEH exceptions and catch them at thread boundaries")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: Translate SEH exceptions and catch them at thread boundaries unless debugger present")
#endif
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: Do NOT translate SEH exceptions, allow them to propogate and remain uncaught")
#endif

#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_EXCEPTION_STACK_TRACES enabled: YES")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_EXCEPTION_STACK_TRACES enabled: NO")
#endif

#if (JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: YES")
#if (JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE enabled: YES")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_DUMP_ON_NULL_REFERENCE enabled: NO")
#endif
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_REFERENCE_COUNTED_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: NO")
#endif

#if (JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: YES")
#if (JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE enabled: YES")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE enabled: NO")
#endif
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE enabled: NO")
#endif

#if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_PERF_TIMER_QUEUE_MONITORING enabled: YES")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_PERF_TIMER_QUEUE_MONITORING enabled: NO")
#endif

#if (JETBYTE_PERF_TIMER_WHEEL_MONITORING == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_PERF_TIMER_WHEEL_MONITORING enabled: YES")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_PERF_TIMER_WHEEL_MONITORING enabled: NO")
#endif

#if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_PERF_TIMER_CONTENTION_MONITORING enabled: YES")
#else
#pragma JETBYTE_MESSAGE(" Build configuration: Win32Tools: JETBYTE_PERF_TIMER_CONTENTION_MONITORING enabled: NO")
#endif

#if (JETBYTE_WIN32_AUTOMATIC_SEH_CRASH_DUMP_CREATION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_WIN32_AUTOMATIC_SEH_CRASH_DUMP_CREATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_WIN32_AUTOMATIC_SEH_CRASH_DUMP_CREATION enabled: NO")
#endif

#if (JETBYTE_WIN32_PURE_CALL_CRASH_DUMP_CREATION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_WIN32_PURE_CALL_CRASH_DUMP_CREATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_WIN32_PURE_CALL_CRASH_DUMP_CREATION enabled: NO")
#endif

#if (JETBYTE_TERMINATE_CRASH_DUMP_CREATION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_TERMINATE_CRASH_DUMP_CREATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_TERMINATE_CRASH_DUMP_CREATION enabled: NO")
#endif

#if (JETBYTE_UNEXPECTED_CRASH_DUMP_CREATION == 1)
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_UNEXPECTED_CRASH_DUMP_CREATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_UNEXPECTED_CRASH_DUMP_CREATION enabled: NO")
#endif

#if JETBYTE_MINI_DUMP_GENERATOR_BREAK_INTO_DEBUGGER == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_MINI_DUMP_GENERATOR_BREAK_INTO_DEBUGGER enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_MINI_DUMP_GENERATOR_BREAK_INTO_DEBUGGER enabled: NO")
#endif

#if JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED enabled: NO")
#endif

#if JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED enabled: NO")
#endif

#if JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED enabled: NO")
#endif

#if JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION enabled: NO")
#endif

#if JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS enabled: NO")
#endif

#if JETBYTE_WIN32_DEPRECATE_OPTEX == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_WIN32_DEPRECATE_OPTEX enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_WIN32_DEPRECATE_OPTEX enabled: NO")
#endif

#if JETBYTE_WIN32_THIRD_PARTY_CODE_ENABLE_STACKWALKER == 1
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_WIN32_THIRD_PARTY_CODE_ENABLE_STACKWALKER enabled: YES")
#else
#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools: JETBYTE_WIN32_THIRD_PARTY_CODE_ENABLE_STACKWALKER enabled: NO")
#endif

#endif

#pragma JETBYTE_MESSAGE("Build configuration: Win32Tools:---------------------------------------------------------------")
#pragma JETBYTE_MESSAGE("")
#pragma JETBYTE_MESSAGE("")
#pragma JETBYTE_MESSAGE("")

SuppressNoSymbolsFoundWarning()

///////////////////////////////////////////////////////////////////////////////
// End of file: Admin.cpp
///////////////////////////////////////////////////////////////////////////////
