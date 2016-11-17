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

bool IWaitable::IsSignalled() const
{
   return Wait(static_cast<Milliseconds>(0));
}

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

bool IWaitable::WaitWithMessageLoop(
   const HANDLE handle,
   const Milliseconds timeout)
{
   DWORD notUsed;

   return WaitWithMessageLoop(1, &handle, notUsed, timeout, 0);
}

bool IWaitable::WaitWithMessageLoop(
   const HANDLE handle,
   const Milliseconds timeout,
   const DWORD removeFlags)
{
   DWORD notUsed;

   return WaitWithMessageLoop(1, &handle, notUsed, timeout, removeFlags);
}

bool IWaitable::WaitWithMessageLoop(
   const DWORD numHandles,
   const HANDLE *pHandles,
   const Milliseconds timeout)
{
   DWORD notUsed;

   return WaitWithMessageLoop(numHandles, pHandles, notUsed, timeout, 0);
}

bool IWaitable::WaitWithMessageLoop(
   const DWORD numHandles,
   const HANDLE *pHandles,
   DWORD &signalledHandle,
   const Milliseconds timeout)
{
   return WaitWithMessageLoop(numHandles, pHandles, signalledHandle, timeout, 0);
}

bool IWaitable::WaitWithMessageLoop(
   const DWORD numHandles,
   const HANDLE *pHandles,
   DWORD &signalledHandle,
   Milliseconds timeout,
   const DWORD removeFlags)
{
   bool done = false;

   bool result = false;

   const DWORD startTime = ::GetTickCount();

   const DWORD originalTimeout = timeout;

   while (!done)
   {
      // Check here in case the event is signalled and there are messages to dispatch as
      // we otherwise could continually loop trying to dispatch (if the remove flags are
      // 'wrong'). Bit of an ugly cludge but fixes the problem...

      DWORD dwRet = WaitForMultipleObjects(numHandles, pHandles, FALSE, 0);

      if (dwRet == WAIT_TIMEOUT)
      {
         // The real wait...

         dwRet = MsgWaitForMultipleObjects(
            numHandles,
            pHandles,
            FALSE,          // Wait for 1 handle
            timeout,        // Timeout value
            QS_ALLINPUT);   // Any message wakes up
      }

      if (dwRet == WAIT_TIMEOUT)
      {
         result = false;
         done = true;
      }
      else if (dwRet < (WAIT_OBJECT_0 + numHandles)) //lint !e835 (zero given as right argument to + in a constant expression)
      {
         // An event was signaled, return

         signalledHandle = dwRet - WAIT_OBJECT_0; //lint !e835 (zero given as right argument to + in a constant expression)

         result = true;
         done = true;
      }
      else if (dwRet == WAIT_OBJECT_0 + numHandles) //lint !e835 (zero given as right argument to + in a constant expression)
      {
         // There is a window message available. Dispatch it.

         MSG msg;

         while (::PeekMessage(&msg, nullptr, 0, 0, removeFlags | PM_REMOVE))
         {
            if (msg.message == WM_QUIT)
            {
               ::PostQuitMessage(0);

               return false;
            }

            ::TranslateMessage(&msg);

            ::DispatchMessage(&msg);
         }

         // Adjust the timeout so that we don't wait forever...

         const DWORD now = ::GetTickCount();

         if (now - startTime >= originalTimeout)
         {
            result = false;
            done = true;
         }
         else
         {
            timeout = originalTimeout - (now - startTime);
         }
      }
      else
      {
         // Something else happened. Return.

         result = false;
         done = true;
      }
   }

   return result;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IWaitable.cpp
///////////////////////////////////////////////////////////////////////////////
