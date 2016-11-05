///////////////////////////////////////////////////////////////////////////////
// File: IWaitable.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2007 JetByte Limited.
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

#include "IWaitable.h"
#include "Win32Exception.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// IWaitable
///////////////////////////////////////////////////////////////////////////////

void IWaitable::Wait(
   HANDLE handle)
{
   if (!Wait(handle, INFINITE))
   {
      throw CException(_T("IWaitable::Wait()"), _T("Unexpected timeout on infinite wait"));
   }
}

bool IWaitable::Wait(
   HANDLE handle,
   const Milliseconds timeout)
{
   if (handle == INVALID_HANDLE_VALUE)
   {
      throw CException(_T("IWaitable::Wait()"), _T("Handle is invalid"));
   }

   bool ok;

   DWORD result = ::WaitForSingleObject(handle, timeout);

   if (result == WAIT_TIMEOUT)
   {
      ok = false;
   }
   else if (result == WAIT_OBJECT_0)
   {
      ok = true;
   }
   else
   {
      throw CWin32Exception(_T("IWaitable::Wait() - WaitForSingleObject"), ::GetLastError());
   }
    
   return ok;
}   

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: IWaitable.cpp
///////////////////////////////////////////////////////////////////////////////
