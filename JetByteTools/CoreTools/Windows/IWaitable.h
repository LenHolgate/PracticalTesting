#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: IWaitable.h
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

#include "JetByteTools/CoreTools/Types.h"

#include <wtypes.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

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

      /// Wait, with a time limit, for the object to become signalled. If you want to
      /// wait for 0 milliseconds and you get an "ambiguous call" with the Handle version
      /// then use IsSignalled() which is the same as Wait(0)

      virtual bool Wait(
         Milliseconds timeout) const = 0;

      /// Wait indefinitely for the supplied handle to become signalled.

      static void WaitForHandle(
         HANDLE handle);

      /// Wait, with a time limit, for the supplied handle to become signalled.

      static bool WaitForHandle(
         HANDLE handle,
         Milliseconds timeout);

      static DWORD WaitForMultipleHandles(
         DWORD numHandles,
         HANDLE *pHandles,
         bool waitAll,
         Milliseconds timeout);

      static bool WaitWithMessageLoop(
         HANDLE handle,
         Milliseconds timeout);

      static bool WaitWithMessageLoop(
         HANDLE handle,
         Milliseconds timeout,
         DWORD removeFlags);

      static bool WaitWithMessageLoop(
         DWORD numHandles,
         const HANDLE *pHandles,
         Milliseconds timeout);

      static bool WaitWithMessageLoop(
         DWORD numHandles,
         const HANDLE *pHandles,
         DWORD &signalledHandle,
         Milliseconds timeout);

      static bool WaitWithMessageLoop(
         DWORD numHandles,
         const HANDLE *pHandles,
         DWORD &signalledHandle,
         Milliseconds timeout,
         DWORD removeFlags);

   protected :

      IWaitable() = default;

      IWaitable(
         const IWaitable &rhs) = default;

      /// We never delete instances of this interface; you must manage the
      /// lifetime of the class that implements it.

      virtual ~IWaitable() = default;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IWaitable.h
///////////////////////////////////////////////////////////////////////////////
