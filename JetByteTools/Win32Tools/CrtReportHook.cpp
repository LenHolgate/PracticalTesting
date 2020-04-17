///////////////////////////////////////////////////////////////////////////////
// File: CrtReportHook.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2010 JetByte Limited.
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

#include "JetByteTools/Admin/Admin.h"

#include "CrtReportHook.h"

#include "DebugTrace.h"
#include "StringConverter.h"

#pragma hdrstop

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

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
   atexit(CrtReportHookAtExitDetector);
}

CCrtReportHook::~CCrtReportHook()
{
   // The lint suppression is needed because the _CrtSetReportHook() is nothing but
   // a cast to nothing in release builds...

   _CrtSetReportHook(m_oldHook); //lint !e522 (Highest operation, a 'cast', lacks side-effects)
}

int CCrtReportHook::ReportHook(
   const int reportType,
   char *pMessage, //lint !e818 (parameter could be pointer to const)
   int *pReturnValue)
{
   if (s_processIsExiting)
   {
      ExitProcess(666);
   }

   OutputEx(_T("CRT Debug: ") + CStringConverter::AtoT(pMessage));

   cout << "CRT Debug: " << pMessage << endl;

   if (reportType == _CRT_ASSERT ||
       reportType == _CRT_ERROR)
   {
      if (::IsDebuggerPresent())
      {
         DebugBreak();
      }

      if (reportType != _CRT_WARN)
      {
         cout << "Terminating process" << endl;

         OutputEx(_T("CRT Debug: Terminating process"));

         ExitProcess(254);
      }
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
