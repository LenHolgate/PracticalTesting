///////////////////////////////////////////////////////////////////////////////
// File: GlobalErrorHandler.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2011 JetByte Limited.
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

#include "GlobalErrorHandler.h"

#pragma hdrstop

#include "PureCallHandler.h"

#include <signal.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Namespace: CGlobalErrorHandler
///////////////////////////////////////////////////////////////////////////////

CGlobalErrorHandler::CGlobalErrorHandler()
   :  m_oldErrorMode(::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX)),
      m_oldPureCallHandler(::_set_purecall_handler(PureCallHandler))
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED == 1)
      ,m_oldNewHandler(std::set_new_handler(NewHandler))
      #endif
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED == 1)
      ,m_pOldSigAbrtHandler(signal(SIGABRT, SigAbortHandler))
      #endif
{

}

CGlobalErrorHandler::~CGlobalErrorHandler()
{
   try
   {
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED == 1)
      signal(SIGABRT, m_pOldSigAbrtHandler);
      #endif
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED == 1)
      std::set_new_handler(m_oldNewHandler);
      #endif
      ::_set_purecall_handler(m_oldPureCallHandler);

      ::SetErrorMode(m_oldErrorMode);
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
}

#if (JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED == 1)
void CGlobalErrorHandler::NewHandler()
{
   if (!ProcessIsExiting())
   {
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_BREAK_IF_DEBUGGER_PRESENT == 1)
      if (::IsDebuggerPresent())
      {
         MessageBox(0, _T("Memory allocation failure"), _T("Memory allocation failure!"), MB_OK);

         DebugBreak();
      }
      #endif

      JetByteTools::Win32::OutputEx(_T("Memory allocation failure!"));

      // It would be nice to have the option of a crash dump here but since we don't have
      // any memory left we can't do that...

      // It would also be nice to be able to hang here and allow a debugger to be attached, but
      // since we don't have any memory the debugger is able to inject its thread and so that
      // doesn't work either...
   }

   throw std::bad_alloc();
}
#endif

#if (JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED == 1)
void CGlobalErrorHandler::SigAbortHandler(
   int sig)
{
   (void)sig;

   if (!ProcessIsExiting())
   {
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_BREAK_IF_DEBUGGER_PRESENT == 1)
      if (::IsDebuggerPresent())
      {
         MessageBox(0, _T("SIGABRT!"), _T("SIGABRT!"), MB_OK);

         DebugBreak();
      }
      #endif

      JetByteTools::Win32::OutputEx(_T("SIGABRT!"));
   }
   else
   {
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_BREAK_IF_DEBUGGER_PRESENT == 1)
      if (::IsDebuggerPresent())
      {
         MessageBox(0, _T("SIGABRT!") , _T("SIGABRT!"), MB_OK);

         DebugBreak();
      }
      #endif
   }
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: GlobalErrorHandler.cpp
///////////////////////////////////////////////////////////////////////////////
