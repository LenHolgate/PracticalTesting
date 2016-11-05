#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_I_WAITABLE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_I_WAITABLE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IWaitable.h
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

#include "JetByteTools\Admin\Types.h"

#include <wtypes.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// IWaitable
///////////////////////////////////////////////////////////////////////////////

/// An interface to code that can be waited for, either via the methods on the
/// interface or by passing a handle to one of the
/// <a href="http://msdn2.microsoft.com/en-us/library/ms687069.aspx">Wait Functions</a>.
/// \ingroup Synchronization
/// \ingroup Interfaces
/// \ingroup ProtectedDestructors

class IWaitable
{
   public :

      /// Access a HANDLE to wait on by passing it to one of the Wait Functions

      virtual HANDLE GetWaitHandle() const = 0;

      /// Returns true if the object is signalled and false if it isn't; doesn't
      /// block.

      virtual bool IsSignalled() const;

      /// Wait indefinitely for the object to become signalled.

      virtual void Wait() const = 0;

      /// Wait, with a time limit, for the object to become signalled.

      virtual bool Wait(
         const Milliseconds timeout) const = 0;

      /// Wait indefinitely for the supplied handle to become signalled.

      static void Wait(
         HANDLE handle);

      /// Wait, with a time limit, for the supplied handle to become signalled.

      static bool Wait(
         HANDLE handle,
         const Milliseconds timeout);

      static bool WaitWithMessageLoop(
         HANDLE handle,
         const Milliseconds timeout);

      static bool WaitWithMessageLoop(
         HANDLE handle,
         const Milliseconds timeout,
         const DWORD removeFlags);

      static bool WaitWithMessageLoop(
         const DWORD numHandles,
         HANDLE *pHandles,
         const Milliseconds timeout);

      static bool WaitWithMessageLoop(
         const DWORD numHandles,
         HANDLE *pHandles,
         DWORD &signalledHandle,
         const Milliseconds timeout);

      static bool WaitWithMessageLoop(
         const DWORD numHandles,
         HANDLE *pHandles,
         DWORD &signalledHandle,
         const Milliseconds timeout,
         const DWORD removeFlags);

   protected :

      /// We never delete instances of this interface; you must manage the
      /// lifetime of the class that implements it.

      ~IWaitable() {}
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_I_WAITABLE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IWaitable.h
///////////////////////////////////////////////////////////////////////////////
