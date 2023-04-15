///////////////////////////////////////////////////////////////////////////////
// File: PerThreadErrorHandler.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2016 JetByte Limited.
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

#include "PerThreadErrorHandler.h"

#include "JetByteTools/CoreTools/DebugTrace.h"
#include "JetByteTools/CoreTools/tstring.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows{

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
      #if (JETBYTE_BREAK_SEH_EXCEPTION_TRANSLATOR_COMPATABILITY == 1)
      m_sehTranslator(0),
      #endif
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
         MessageBox(0, _T("std::terminate!"), _T("std::terminate!"), MB_OK);

         DebugBreak();
      }
      #endif

      JetByteTools::Core::OutputEx(_T("std::terminate!"));
   }
   else
   {
      #if (JETBYTE_GLOBAL_ERROR_HANDLER_BREAK_IF_DEBUGGER_PRESENT == 1)
      if (IsDebuggerPresent())
      {
         MessageBox(nullptr, _T("std::terminate!"), _T("std::terminate!"), MB_OK);

         DebugBreak();
      }
      #endif
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
         MessageBox(nullptr, _T("std::unexpected!"), _T("std::unexpected!"), MB_OK);

         DebugBreak();
      }
      #endif

      JetByteTools::Core::OutputEx(_T("std::unexpected!"));
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
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: PerThreadErrorHandler.cpp
///////////////////////////////////////////////////////////////////////////////
