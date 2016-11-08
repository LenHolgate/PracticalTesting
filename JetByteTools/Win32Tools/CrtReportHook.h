#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_CRT_REPORT_HOOK_INCLUDED__
#define JETBYTE_TOOLS_WIN32_CRT_REPORT_HOOK_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CrtReportHook.h
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

#include "JetByteTools\Admin\Admin.h"

#pragma hdrstop

#include <crtdbg.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CCrtReportHook
///////////////////////////////////////////////////////////////////////////////

class CCrtReportHook
{
   public :

      CCrtReportHook();

      ~CCrtReportHook();

   private :

      static int ReportHook(
         int reportType,
         char *message,
         int *returnValue);

      _CRT_REPORT_HOOK m_oldHook;

      /// No copies do not implement
      CCrtReportHook(const CCrtReportHook &rhs);
      /// No copies do not implement
      CCrtReportHook &operator=(const CCrtReportHook &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_CRT_REPORT_HOOK_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CrtReportHook.h
///////////////////////////////////////////////////////////////////////////////
