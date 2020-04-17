#pragma once
#ifndef JETBYTE_TOOLS_WIN32_PURE_CALL_HANDLER_INCLUDED__
#define JETBYTE_TOOLS_WIN32_PURE_CALL_HANDLER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: PureCallHandler.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2006 JetByte Limited.
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

#include "DebugTrace.h"
#include "ManualResetEvent.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// PureCallHandler
///////////////////////////////////////////////////////////////////////////////

static void PureCallHandler()
{
   JetByteTools::Win32::OutputEx(_T("Purecall!"));

   #if (JETBYTE_PURE_CALL_BREAK_INTO_DEBUGGER_IF_PRESENT == 1)
   if (::IsDebuggerPresent())
   {
      MessageBox(0, _T("Purecall!"), _T("Purecall!"), MB_OK);

      ::DebugBreak();
   }
   #endif
}

//lint -esym(528, InstallPureCallHandler) (Symbol not referenced)
static void InstallPureCallHandler() 
{
   _set_purecall_handler(PureCallHandler);
}

#endif // JETBYTE_TOOLS_WIN32_PURE_CALL_HANDLER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: PureCallHandler.h
///////////////////////////////////////////////////////////////////////////////
