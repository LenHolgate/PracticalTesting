#pragma once
#ifndef JETBYTE_TOOLS_WIN32_GLOBAL_ERROR_HANDLER_INCLUDED__
#define JETBYTE_TOOLS_WIN32_GLOBAL_ERROR_HANDLER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: GlobalErrorHandler.h
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

#include "JetByteTools/Admin/Admin.h"

#include "CrtReportHook.h"
#include "SEHException.h"
#include "PerThreadErrorHandler.h"

#include <cstdlib>

#include <exception>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CGlobalErrorHandler
///////////////////////////////////////////////////////////////////////////////

class CGlobalErrorHandler : private CPerThreadErrorHandler
{
   public :

      CGlobalErrorHandler();

      CGlobalErrorHandler(
         const CGlobalErrorHandler &rhs) = delete;

      ~CGlobalErrorHandler();

      CGlobalErrorHandler &operator=(
         const CGlobalErrorHandler &rhs) = delete;

   private :

      #if (JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED == 1)
      static void NewHandler();
      #endif

      #if (JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED == 1)
      static void SigAbortHandler(int);
      #endif

      const UINT m_oldErrorMode;

      _purecall_handler m_oldPureCallHandler;

      CCrtReportHook m_crtReportHook;

      #if (JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED == 1)
      std::new_handler m_oldNewHandler;
      #endif

      #if (JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED == 1)
      typedef void(SignalHandlerFnc)(int);

      SignalHandlerFnc *m_pOldSigAbrtHandler;
      #endif
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_GLOBAL_ERROR_HANDLER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: GlobalErrorHandler.h
///////////////////////////////////////////////////////////////////////////////
