#pragma once
#ifndef JETBYTE_TOOLS_WIN32_PER_THREAD_ERROR_HANDLER_INCLUDED__
#define JETBYTE_TOOLS_WIN32_PER_THREAD_ERROR_HANDLER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: PerThreadErrorHandler.h
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

#include "SEHException.h"

#include <exception>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CPerThreadErrorHandler
///////////////////////////////////////////////////////////////////////////////

class CPerThreadErrorHandler
{
   public :

      CPerThreadErrorHandler();

      ~CPerThreadErrorHandler();

      static bool ProcessIsExiting();

   private :

      #if (JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED == 1)
      static void TerminateHandler();
      #endif

      #if (JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED == 1)
      static void UnexpectedHandler();
      #endif

      CSEHException::Translator m_sehTranslator;

      #if (JETBYTE_GLOBAL_ERROR_HANDLER_TERMINATE_HANDLER_ENABLED == 1)
      std::terminate_handler m_oldTerminateHandler;
      #endif
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_UNEXPECTED_HANDLER_ENABLED == 1)
      std::unexpected_handler m_oldUnexpectedHandler;
      #endif

      /// No copies do not implement
      CPerThreadErrorHandler(const CPerThreadErrorHandler &rhs);
      /// No copies do not implement
      CPerThreadErrorHandler &operator=(const CPerThreadErrorHandler &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_PER_THREAD_ERROR_HANDLER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: PerThreadErrorHandler.h
///////////////////////////////////////////////////////////////////////////////
