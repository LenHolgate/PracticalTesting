///////////////////////////////////////////////////////////////////////////////
// File: IWaitable.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2007 JetByte Limited.
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

#include "IWaitable.h"

#include "JetByteTools/CoreTools/ErrorCodeException.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Widows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// IWaitable
///////////////////////////////////////////////////////////////////////////////

bool IWaitable::IsSignalled() const
{
   return Wait(static_cast<Milliseconds>(0));
}

void IWaitable::WaitForHandle(
   HANDLE handle)
{
   if (!WaitForHandle(handle, INFINITE))
   {
      throw CException(_T("IWaitable::WaitForHandle()"), _T("Unexpected timeout on infinite wait"));
   }
}

bool IWaitable::WaitForHandle(
   HANDLE handle,
   const Milliseconds timeout)
{
   if (handle == INVALID_HANDLE_VALUE)
   {
      throw CException(_T("IWaitable::WaitForHandle()"), _T("Handle is invalid"));
   }

   bool ok;

   const DWORD result = WaitForMultipleHandles(1, const_cast<HANDLE *>(&handle), true, timeout);

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
      throw CErrorCodeException(_T("IWaitable::Wait() - WaitForSingleObject"), GetLastError());
   }

   return ok;
}

DWORD IWaitable::WaitForMultipleHandles(
   const DWORD numHandles,
   HANDLE *pHandles,
   const bool waitAll,
   const Milliseconds timeout)
{
   return WaitForMultipleObjects(numHandles, pHandles, waitAll, timeout);
}

bool IWaitable::WaitWithMessageLoop(
   HANDLE handle,
   const Milliseconds timeout)
{
   DWORD notUsed;

   return WaitWithMessageLoop(1, &handle, notUsed, timeout, 0);
}

bool IWaitable::WaitWithMessageLoop(
   HANDLE handle,
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

   const DWORD startTime = GetTickCount();

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
      else if (dwRet < (WAIT_OBJECT_0 + numHandles))
      {
         // An event was signaled, return

         signalledHandle = dwRet - WAIT_OBJECT_0;

         result = true;
         done = true;
      }
      else if (dwRet == WAIT_OBJECT_0 + numHandles)
      {
         // There is a window message available. Dispatch it.

         MSG msg;

         while (::PeekMessage(&msg, nullptr, 0, 0, removeFlags | PM_REMOVE))
         {
            if (msg.message == WM_QUIT)
            {
               PostQuitMessage(0);

               return false;
            }

            TranslateMessage(&msg);

            ::DispatchMessage(&msg);
         }

         // Adjust the timeout so that we don't wait forever...

         const DWORD now = GetTickCount();

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
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IWaitable.cpp
///////////////////////////////////////////////////////////////////////////////
