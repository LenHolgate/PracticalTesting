#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: GlobalErrorHandler.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2011 JetByte Limited.
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

#include "JetByteTools/Win32Tools/CrtReportHook.h"
#include "PerThreadErrorHandler.h"

#include <cstdlib>

#include <exception>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// CGlobalErrorHandler
///////////////////////////////////////////////////////////////////////////////

class CGlobalErrorHandler : private CPerThreadErrorHandler
{
   public :

      CGlobalErrorHandler();

      CGlobalErrorHandler(
         const CGlobalErrorHandler &rhs) = delete;

      ~CGlobalErrorHandler() override;

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

      Win32::CCrtReportHook m_crtReportHook;

      #if (JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED == 1)
      std::new_handler m_oldNewHandler;
      #endif

      #if (JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED == 1)
      typedef void(SignalHandlerFnc)(int);

      SignalHandlerFnc *m_pOldSigAbrtHandler;
      #endif
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: GlobalErrorHandler.h
///////////////////////////////////////////////////////////////////////////////
