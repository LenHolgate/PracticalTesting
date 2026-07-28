#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ExceptionLeakPrevention.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2025 JetByte Limited.
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

#include "JetByteTools/Admin/FunctionName.h"

#include "tstring.h"
#include "Types.h"
#include "DebugTrace.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Exception protection
///////////////////////////////////////////////////////////////////////////////

enum class ExceptionProtectionFailureType
{
   General,
   Destructor,
   ThreadExit,
   Timer
};

_tstring GetExceptionProtectionFailureTypeAsString(
   ExceptionProtectionFailureType type);

using ExceptionProtectionHandlerFnc = void(const _tstring &callingFunction, ExceptionProtectionFailureType type, bool log, ULONG_PTR userData);

extern ExceptionProtectionHandlerFnc *s_pHandler;

extern ULONG_PTR s_userData;

ExceptionProtectionHandlerFnc *SetExceptionProtectionHandler(ExceptionProtectionHandlerFnc *pHandler, ULONG_PTR userData = 0);

inline void HandleException(const _tstring &callingFunction, ExceptionProtectionFailureType type, bool log)
{
   try
   {
      try
      {
         if (s_pHandler)
         {
            s_pHandler(callingFunction, type, log, s_userData);
         }
      }
      catch (...)
      {
         OutputEx(_T("Exception protection handler function threw an exception"));
      }
   }
   catch (...)
   {
   }
}

///////////////////////////////////////////////////////////////////////////////
// Macros
///////////////////////////////////////////////////////////////////////////////

#ifndef EXCEPTION_LEAK_PREVENTION_START
#define EXCEPTION_LEAK_PREVENTION_START   try {
#endif

#ifndef EXCEPTION_LEAK_PREVENTION_END_WITH_CODE
#define EXCEPTION_LEAK_PREVENTION_END_WITH_CODE(_c, _log)  } catch(...) { JetByteTools::Core::HandleException(JETBYTE_FUNCTION_NAME, _c, _log); }
#endif

#ifndef EXCEPTION_LEAK_PREVENTION_END
#define EXCEPTION_LEAK_PREVENTION_END EXCEPTION_LEAK_PREVENTION_END_WITH_CODE(JetByteTools::Core::ExceptionProtectionFailureType::General, false)
#endif


#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START EXCEPTION_LEAK_PREVENTION_START
#define JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END EXCEPTION_LEAK_PREVENTION_END_WITH_CODE(JetByteTools::Core::ExceptionProtectionFailureType::Destructor, true)
#elif (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START EXCEPTION_LEAK_PREVENTION_START
#define JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END EXCEPTION_LEAK_PREVENTION_END_WITH_CODE(JetByteTools::Core::ExceptionProtectionFailureType::Destructor, false)
#else
#define JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START
#define JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END
#endif

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED_START EXCEPTION_LEAK_PREVENTION_START
#define JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED_END EXCEPTION_LEAK_PREVENTION_END_WITH_CODE(JetByteTools::Core::ExceptionProtectionFailureType::ThreadExit, true)
#elif (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_AT_THREAD_BOUNDARY == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED_START EXCEPTION_LEAK_PREVENTION_START
#define JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED_END EXCEPTION_LEAK_PREVENTION_END_WITH_CODE(JetByteTools::Core::ExceptionProtectionFailureType::ThreadExit, false)
#else
#define JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED_START
#define JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED_END
#endif

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_ON_TIMER == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_IN_ON_TIMER_IF_ENABLED_START EXCEPTION_LEAK_PREVENTION_START
#define JETBYTE_CATCH_AND_LOG_ALL_IN_ON_TIMER_IF_ENABLED_END EXCEPTION_LEAK_PREVENTION_END_WITH_CODE(JetByteTools::Core::ExceptionProtectionFailureType::Timer, true)
#elif (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS_IN_ON_TIMER == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_IN_ON_TIMER_IF_ENABLED_START EXCEPTION_LEAK_PREVENTION_START
#define JETBYTE_CATCH_AND_LOG_ALL_IN_ON_TIMER_IF_ENABLED_END EXCEPTION_LEAK_PREVENTION_END_WITH_CODE(JetByteTools::Core::ExceptionProtectionFailureType::Timer, false)
#else
#define JETBYTE_CATCH_AND_LOG_ALL_IN_ON_TIMER_IF_ENABLED_START
#define JETBYTE_CATCH_AND_LOG_ALL_IN_ON_TIMER_IF_ENABLED_END
#endif

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_IF_ENABLED_START EXCEPTION_LEAK_PREVENTION_START
#define JETBYTE_CATCH_AND_LOG_ALL_IF_ENABLED_END EXCEPTION_LEAK_PREVENTION_END_WITH_CODE(JetByteTools::Core::ExceptionProtectionFailureType::General, true)
#elif (JETBYTE_CATCH_UNHANDLED_EXCEPTIONS == 1)
#define JETBYTE_CATCH_AND_LOG_ALL_IF_ENABLED_START EXCEPTION_LEAK_PREVENTION_START
#define JETBYTE_CATCH_AND_LOG_ALL_IF_ENABLED_END EXCEPTION_LEAK_PREVENTION_END_WITH_CODE(JetByteTools::Core::ExceptionProtectionFailureType::General, false)
#else
#define JETBYTE_CATCH_AND_LOG_ALL_IF_ENABLED_START
#define JETBYTE_CATCH_AND_LOG_ALL_IF_ENABLED_END
#endif


///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ExceptionLeakPrevention.h
///////////////////////////////////////////////////////////////////////////////
