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

#include "JetByteTools\Admin\Admin.h"

#include "CallStackCreator.h"

#include "JetByteTools\Win32Tools\DebugTrace.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// PureCallHandler
///////////////////////////////////////////////////////////////////////////////

static void PureCallHandler()
{
   JetByteTools::Test::CCallStackCreator stackWalker;

   const JetByteTools::Win32::_tstring stack = stackWalker.GetStack();

   if (::IsDebuggerPresent())
   {
      MessageBox(0, stack.c_str(), _T("Purecall!"), MB_OK);

      DebugBreak();
   }
   else
   {
      JetByteTools::Win32::OutputEx(_T("Purecall!"));
      JetByteTools::Win32::OutputEx(stack);
   }
}

static void InstallPureCallHandler()
{
   _set_purecall_handler(PureCallHandler);
}


///////////////////////////////////////////////////////////////////////////////
// End of file: PureCallHandler.h
///////////////////////////////////////////////////////////////////////////////
