///////////////////////////////////////////////////////////////////////////////
// File: CrtReportHook.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2010 JetByte Limited.
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

#include "CrtReportHook.h"

#include "JetByteTools/CoreTools/DebugTrace.h"
#include "JetByteTools/CoreTools/StringConverter.h"
#include "JetByteTools/CoreTools/ExceptionLeakPrevention.h"

#pragma hdrstop

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Core::OutputEx;
using JetByteTools::Core::CStringConverter;

using std::cout;
using std::endl;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static void CrtReportHookAtExitDetector();

///////////////////////////////////////////////////////////////////////////////
// Static variables
///////////////////////////////////////////////////////////////////////////////

static bool s_processIsExiting = false;

///////////////////////////////////////////////////////////////////////////////
// CCrtReportHook
///////////////////////////////////////////////////////////////////////////////

CCrtReportHook::CCrtReportHook()
   :  m_oldHook(_CrtSetReportHook(ReportHook))
{
   (void)atexit(CrtReportHookAtExitDetector);
}

CCrtReportHook::~CCrtReportHook()
{
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START

   _CrtSetReportHook(m_oldHook);

   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END
}

int CCrtReportHook::ReportHook(
   int reportType,
   char *pMessage,
   int *pReturnValue)
{
   #if (JETBYTE_WIN32_CRT_REPORT_HOOK_TREAT_ASSERT_AS_WARN == 1)
   if (reportType == _CRT_ASSERT)
   {
      reportType == _CRT_WARN;
   }
   #endif

   if (s_processIsExiting)
   {
      // We call terminate process because that kills us dead immediately and doesn't continue to try and
      // fiddle with the heap...

      TerminateProcess(GetCurrentProcess(), 666);
   }

   // if this is a heap corruption then doing stuff which allocates or releases memory will just make things
   // far worse - so try other stuff first...

   if (reportType == _CRT_ASSERT ||
       reportType == _CRT_ERROR)
   {
      if (IsDebuggerPresent())
      {
         DebugBreak();
      }

      if (reportType != _CRT_WARN)
      {
         cout << "Terminating process" << endl;

         OutputEx(_T("CRT Debug: Terminating process"));

         TerminateProcess(GetCurrentProcess(), 254);
      }
   }
   else
   {
      OutputEx(_T("CRT Debug: ") + CStringConverter::AtoT(pMessage));
   }

   if (pReturnValue)
   {
      *pReturnValue = 0;  // continue normal execution
   }

   return TRUE;      // We have handled all reporting requirements
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static void CrtReportHookAtExitDetector()
{
   s_processIsExiting = true;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CrtReportHook.cpp
///////////////////////////////////////////////////////////////////////////////
