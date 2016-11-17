///////////////////////////////////////////////////////////////////////////////
// File: PerThreadErrorHandler.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 JetByte Limited.
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

#include "PerThreadErrorHandler.h"

#include "DebugTrace.h"

#pragma hdrstop

#include <signal.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static void PerThreadHandlerAtExitDetector();

///////////////////////////////////////////////////////////////////////////////
// Static variables
///////////////////////////////////////////////////////////////////////////////

static bool s_processIsExiting = false;

///////////////////////////////////////////////////////////////////////////////
// Namespace: CPerThreadErrorHandler
///////////////////////////////////////////////////////////////////////////////

CPerThreadErrorHandler::CPerThreadErrorHandler()
   :
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED == 1)
      m_oldTerminateHandler(std::set_terminate(TerminateHandler))
      #endif
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED == 1)
      ,m_oldUnexpectedHandler(std::set_unexpected(UnexpectedHandler))
      #endif
{
   atexit(PerThreadHandlerAtExitDetector);
}

CPerThreadErrorHandler::~CPerThreadErrorHandler()
{
   try
   {
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED == 1)
      std::set_unexpected(m_oldUnexpectedHandler);
      #endif
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED == 1)
      std::set_terminate(m_oldTerminateHandler);
      #endif
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
}

#if (JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED == 1)
void CPerThreadErrorHandler::TerminateHandler()
{
   if (!s_processIsExiting)
   {
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_BREAK_IF_DEBUGGER_PRESENT == 1)
      if (::IsDebuggerPresent())
      {
         MessageBox(0, _T("std::terminate!") , _T("std::terminate!"), MB_OK);

         DebugBreak();
      }
      #endif

      JetByteTools::Win32::OutputEx(_T("std::terminate!"));
   }
   else
   {
      if (::IsDebuggerPresent())
      {
         MessageBox(nullptr, _T("std::terminate!") , _T("std::terminate!"), MB_OK);

         DebugBreak();
      }
   }
}
#endif

#if (JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED == 1)
void CPerThreadErrorHandler::UnexpectedHandler()
{
   if (!s_processIsExiting)
   {
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_BREAK_IF_DEBUGGER_PRESENT == 1)
      if (::IsDebuggerPresent())
      {
         MessageBox(nullptr, _T("std::unexpected!") , _T("std::unexpected!"), MB_OK);

         DebugBreak();
      }
      #endif

      JetByteTools::Win32::OutputEx(_T("std::unexpected!"));
   }
}
#endif

bool CPerThreadErrorHandler::ProcessIsExiting()
{
   return s_processIsExiting;
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static void PerThreadHandlerAtExitDetector()
{
   s_processIsExiting = true;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: PerThreadErrorHandler.cpp
///////////////////////////////////////////////////////////////////////////////
