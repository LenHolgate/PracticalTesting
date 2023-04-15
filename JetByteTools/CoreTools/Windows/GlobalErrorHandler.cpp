///////////////////////////////////////////////////////////////////////////////
// File: GlobalErrorHandler.cpp
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

#include "GlobalErrorHandler.h"

#include "JetByteTools/CoreTools/ManualResetEvent.h"
#include "JetByteTools/CoreTools/DebugTrace.h"

#pragma hdrstop

#include "JetByteTools/Win32Tools/PureCallHandler.h"

#include <csignal>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Core::OutputEx;
using JetByteTools::Core::_tstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// Namespace: CGlobalErrorHandler
///////////////////////////////////////////////////////////////////////////////

CGlobalErrorHandler::CGlobalErrorHandler()
   :  m_oldErrorMode(SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX)),
      m_oldPureCallHandler(_set_purecall_handler(PureCallHandler))
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED == 1)
      , m_oldNewHandler(std::set_new_handler(NewHandler))
      #endif
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_SIBABRT_HANDLER_ENABLED == 1)
      , m_pOldSigAbrtHandler(signal(SIGABRT, SigAbortHandler))
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
      _set_purecall_handler(m_oldPureCallHandler);

      SetErrorMode(m_oldErrorMode);
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
}

#if (JETBYTE_GLOBAL_ERROR_HANDLER_NEW_HANDLER_ENABLED == 1)
void CGlobalErrorHandler::NewHandler()
{
   if (!ProcessIsExiting())
   {
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_BREAK_IF_DEBUGGER_PRESENT == 1)
      if (IsDebuggerPresent())
      {
         MessageBox(nullptr, _T("Memory allocation failure"), _T("Memory allocation failure!"), MB_OK);

         DebugBreak();
      }
      #endif

      OutputEx(_T("Memory allocation failure!\r\n"));

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

   #if (JETBYTE_GLOBAL_ERROR_HANDLER_BREAK_IF_DEBUGGER_PRESENT == 1)
   if (::IsDebuggerPresent())
   {
      MessageBox(nullptr, _T("SIGABRT!"), _T("SIGABRT!"), MB_OK);

      DebugBreak();
   }
   #endif
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: GlobalErrorHandler.cpp
///////////////////////////////////////////////////////////////////////////////
