#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: Admin.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2018 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the �Software�), to deal
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

#ifndef JETBYTE_TOOLS_CONFIG_CORE
#define JETBYTE_TOOLS_CONFIG_CORE
#endif

#ifdef JETBYTE_SHOW_CONFIG_DETAILS
#ifndef JETBYTE_SHOW_CORE_CONFIG_DETAILS
#define JETBYTE_SHOW_CORE_CONFIG_DETAILS JETBYTE_SHOW_CONFIG_DETAILS
#endif
#endif

#ifndef JETBYTE_SHOW_CORE_CONFIG_DETAILS
#define JETBYTE_SHOW_CORE_CONFIG_DETAILS 0
#endif

#ifndef JETBYTE_CORE_ENABLE_DEBUG_TRACE_OUTPUT
#define JETBYTE_CORE_ENABLE_DEBUG_TRACE_OUTPUT 1
#endif

#ifndef JETBYTE_CORE_THREAD_USE_STD_THREADING
#define JETBYTE_CORE_THREAD_USE_STD_THREADING 0
#endif

#ifndef JETBYTE_CORE_ATOMIC_USE_STD_ATOMIC
#define JETBYTE_CORE_ATOMIC_USE_STD_ATOMIC 0
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

#ifndef JETBYTE_EXCEPTION_STACK_TRACES
#define JETBYTE_EXCEPTION_STACK_TRACES 0
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

#ifndef JETBYTE_PERF_TIMER_QUEUE_MONITORING
#define JETBYTE_PERF_TIMER_QUEUE_MONITORING 1
#endif

#ifndef JETBYTE_PERF_TIMER_CONTENTION_MONITORING
#define JETBYTE_PERF_TIMER_CONTENTION_MONITORING 0
#endif

#ifndef JETBYTE_PERF_TIMER_WHEEL_MONITORING
#define JETBYTE_PERF_TIMER_WHEEL_MONITORING 1
#endif

#ifndef JETBYTE_CORE_TRACK_THREAD_NAMES
#define JETBYTE_CORE_TRACK_THREAD_NAMES 0
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

#ifndef JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE
#define JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE 0
#endif

#if JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1
#ifdef JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS
#if JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS == 0
#error If JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE is enabled then JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS must be enabled
#endif
#else
#define JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS 1
#endif
#endif

#ifndef JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS
#if JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 0
#define JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS 1
#else
#define JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS 0
#endif
#endif

#ifndef JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_DEBUG_BREAK
#define JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_DEBUG_BREAK 0
#endif

#ifndef JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP
#define JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP 0
#endif

#ifndef JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION
#define JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION 1
#endif

#if (JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS == 0 && JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1 && JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION == 0)
#undef JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION
#define JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION 1
#define JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_OVERRIDE_EXCEPTION 1
#endif

#ifndef JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_OVERRIDE_EXCEPTION
#define JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_OVERRIDE_EXCEPTION 0
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

#ifndef JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES
#define JETBYTE_PERF_TIMER_QUEUE_VALIDATE_HANDLES 1
#endif

#ifndef JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES
#define JETBYTE_PERF_TIMER_WHEEL_VALIDATE_HANDLES 1
#endif

#ifndef JETBYTE_RANDOM_BYTE_PROVIDER_ENUMERATE_ALL_PROVIDERS
#define JETBYTE_RANDOM_BYTE_PROVIDER_ENUMERATE_ALL_PROVIDERS 0
#endif

#ifndef JETBYTE_DUMP_NAMED_INDEX_DETAILS_ON_LOCK
#define JETBYTE_DUMP_NAMED_INDEX_DETAILS_ON_LOCK 0
#endif

#ifdef JETBYTE_ADMIN_ENABLE_ALL_THIRD_PARTY_CODE

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH 1
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32 1
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32C
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32C 1
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_UTF8
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_UTF8 1
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_BASE64
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_BASE64 1
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5 1
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1 1
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_OPTEX
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_OPTEX 1
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_SWMR_LOCK
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_SWMR_LOCK 1
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_PEVENTS
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_PEVENTS 1
#endif

#else
#ifndef JETBYTE_ADMIN_ENABLE_THIRD_PARTY_CODE

#ifdef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH
#if (JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH == 1)
#undef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH 0
#endif
#endif

#ifdef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32C
#if (JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32C == 1)
#undef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32C
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32C 0
#endif
#endif

#ifdef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32
#if (JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32 == 1)
#undef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32 0
#endif
#endif

#ifdef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_UTF8
#if (JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_UTF8 == 1)
#undef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_UTF8
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_UTF8 0
#endif
#endif

#ifdef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_BASE64
#if (JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_BASE64 == 1)
#undef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_BASE64
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_BASE64 0
#endif
#endif

#ifdef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5
#if (JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5 == 1)
#undef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5 0
#endif
#endif

#ifdef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1
#if (JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1 == 1)
#undef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1 0
#endif
#endif

#ifdef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_OPTEX
#if (JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_OPTEX == 1)
#undef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_OPTEX
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_OPTEX 0
#endif
#endif

#ifdef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_SWMR_LOCK
#if (JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_SWMR_LOCK == 1)
#undef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_SWMR_LOCK
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_SWMR_LOCK 0
#endif
#endif

#ifdef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_PEVENTS
#if (JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_PEVENTS == 1)
#undef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_PEVENTS
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_PEVENTS 0
#endif
#endif

#endif
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH 0
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32 0
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32C
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_CRC32C 0
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_UTF8
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_UTF8 0
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_BASE64
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_BASE64 0
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5 0
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1 0
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_OPTEX
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_OPTEX 0
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_SWMR_LOCK
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_RICHTER_SWMR_LOCK 0
#endif

#ifndef JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_PEVENTS
#define JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_PEVENTS 0
#endif

#ifndef JETBYTE_CORE_TOOLS_CALL_STACK_SUPPORTS_HASH
#define JETBYTE_CORE_TOOLS_CALL_STACK_SUPPORTS_HASH 0
#elif JETBYTE_CORE_TOOLS_CALL_STACK_SUPPORTS_HASH == 1
#if JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH == 0
#error JETBYTE_CORE_TOOLS_CALL_STACK_SUPPORTS_HASH requires JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MURMURHASH to be enabled
#endif
#endif


#ifndef JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_MD5
#if !defined(JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5) || JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_MD5 == 0
#define JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_MD5 1
#endif
#endif
#ifndef JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_SHA1
#if !defined(JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1) || JETBYTE_CORE_THIRD_PARTY_CODE_ENABLE_SHA1 == 0
#define JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_SHA1 1
#endif
#endif

#ifndef JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_MD5
#define JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_MD5 0
#endif

#ifndef JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_SHA1
#define JETBYTE_CORE_TOOLS_USE_WINDOWS_NATIVE_SHA1 0
#endif

#ifndef JETBYTE_CORE_TOOLS_STACK_WALKING_SUPPORTED
#define JETBYTE_CORE_TOOLS_STACK_WALKING_SUPPORTED JETBYTE_WIN32_THIRD_PARTY_CODE_ENABLE_STACKWALKER
#elif JETBYTE_CORE_TOOLS_STACK_WALKING_SUPPORTED == 1
#if JETBYTE_WIN32_THIRD_PARTY_CODE_ENABLE_STACKWALKER == 0
#error JETBYTE_CORE_TOOLS_CALL_STACK_SUPPORTS_HASH requires JETBYTE_WIN32_THIRD_PARTY_CODE_ENABLE_STACKWALKER to be enabled
#endif
#endif

///////////////////////////////////////////////////////////////////////////////
// End of file: Admin.h
///////////////////////////////////////////////////////////////////////////////
