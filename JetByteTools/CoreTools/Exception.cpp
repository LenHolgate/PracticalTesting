///////////////////////////////////////////////////////////////////////////////
// File: Exception.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 1997 JetByte Limited.
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

#include "Exception.h"
#include "ExceptionLeakPrevention.h"
#include "StringConverter.h"

#if JETBYTE_CORE_MONITOR_EXCEPTION_CREATION == 1
#include "NullExceptionCreationMonitor.h"
#include "ThreadLocalBoolToggle.h"
#include "DebugTrace.h"
#endif

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

#if JETBYTE_CORE_MONITOR_EXCEPTION_CREATION == 1

///////////////////////////////////////////////////////////////////////////////
// CRAIIToggle
///////////////////////////////////////////////////////////////////////////////

class CRAIIToggle
{
   public :

      explicit CRAIIToggle(
         CThreadLocalBoolToggle &value)
         :  m_value(value),
            m_toggled(false)
      {
      }

      CRAIIToggle(
         const CRAIIToggle &rhs) = delete;

      ~CRAIIToggle()
      {
         JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START

         if (m_toggled)
         {
            m_value = false;
         }

         JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END
      }

      bool ToggleIfFalse()
      {
         if (!m_toggled)
         {
            m_toggled = m_value.ToggleIfFalse();

            return m_toggled;
         }

         return false;
      }

      CRAIIToggle &operator=(
         const CRAIIToggle &rhs) = delete;

   private :

      CThreadLocalBoolToggle &m_value;

      bool m_toggled;
};

///////////////////////////////////////////////////////////////////////////////
// File level statics
///////////////////////////////////////////////////////////////////////////////

static CNullExceptionCreationMonitor s_nullExceptionMonitor;

static IMonitorExceptionCreation *s_pExceptionMonitor = &s_nullExceptionMonitor;

static CThreadLocalBoolToggle s_callingMonitor(false);

#endif

///////////////////////////////////////////////////////////////////////////////
// CException
///////////////////////////////////////////////////////////////////////////////

#if JETBYTE_CORE_MONITOR_EXCEPTION_CREATION == 1
void CException::SetExceptionMonitor(
   IMonitorExceptionCreation &monitor)
{
   s_pExceptionMonitor = &monitor;
}

void CException::ClearExceptionMonitor()
{
   s_pExceptionMonitor = &s_nullExceptionMonitor;
}
#endif

#ifdef JETBYTE_TOOLS_ADMIN_WIDE_STRING_PLATFORM
CException::CException(
   const std::string &where,
   const std::string &message)
   :  CException(CStringConverter::AtoT(where), CStringConverter::AtoT(message))
{
}
#endif

CException::CException(
   const _tstring &where,
   const _tstring &message)
   :  m_where(where),
      m_message(message),
      m_details(m_where + _T(" - ") + m_message)
{
   #if JETBYTE_CORE_MONITOR_EXCEPTION_CREATION == 1

   CRAIIToggle toggle(s_callingMonitor);

   if (toggle.ToggleIfFalse())
   {
      s_pExceptionMonitor->OnExceptionCreated();
   }
   #endif

   #if (JETBYTE_EXCEPTION_STACK_TRACES == 1)
   CaptureStackBackTrace(m_stack);
   #endif
}

_tstring CException::GetWhere() const
{
#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)

#if (JETBYTE_EXCEPTION_WHERE_INCLUDES_STACK_TRACE == 1)

   return m_where + _T("\r\n") + GetCallStack();

#endif

#endif

   return m_where;
}

_tstring CException::GetWhat() const
{
   return m_message;
}

_tstring CException::GetDetails() const
{
   return m_details;
}

std::string CException::GetDetailsA() const
{
   return CStringConverter::TtoA(m_details);
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)

static void CaptureStackBackTrace(
   CCallStack &callStack)
{
   // xp and 2003 sum of framesToSkip + framesToCapture must be < 64

   const DWORD framesToCapture = 10;

   callStack.Resize(framesToCapture);

   void *pBuffer = callStack.GetMemory();

   const unsigned short numFrames = ::CaptureStackBackTrace(
      1,
      framesToCapture,
      (void **)pBuffer,
      0);

   callStack.Use(numFrames);

   // do we care about 'incomplete' stacks, if so, we can guess if the
   // stack uses up all available space then it's incomplete...
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Exception.cpp
///////////////////////////////////////////////////////////////////////////////
