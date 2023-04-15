#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: Event.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 1997 JetByte Limited.
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

#include "SmartHandle.h"

#include "JetByteTools/CoreTools/IWaitable.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

class IKernelObjectName;

///////////////////////////////////////////////////////////////////////////////
// CEvent
///////////////////////////////////////////////////////////////////////////////

/// A class that wraps the operating system
/// <a href="http://msdn2.microsoft.com/en-us/library/ms682655.aspx">Event API</a>.
/// See <a href="http://www.lenholgate.com/archives/000279.html ">here</a> for
/// more details.
/// \ingroup Synchronization
/// \ingroup KernelObjects

class CEvent : public Core::IWaitable
{
   public :

      /// Specify the type of event object.

      enum ResetType
      {
         ManualReset,
         AutoReset
      };

      /// Specify the initial state of an event object.

      enum InitialState
      {
         Signaled,
         NonSignaled
      };

      /// Specify how to create or connect to an existing event object.

      enum CreationFlags
      {
         CreateOrConnect,
         CreateNew,
         ConnectToExisting
      };

      /// Create an anonymous CEvent object for in-process use.

      CEvent(
         SECURITY_ATTRIBUTES *pSecurityAttributes,
         ResetType resetType,
         InitialState initialState);

      /// Create a new named CEvent object or connect to an existing
      /// one with the same name, for cross-process use. Use the CreationFlags
      /// to force creation only or connection only if required.

      CEvent(
         SECURITY_ATTRIBUTES *pSecurityAttributes,
         ResetType resetType,
         InitialState initialState,
         const IKernelObjectName &name,
         CreationFlags creationFlags = CreateOrConnect);

      /// Connect to an existing named CEvent object for cross-process use.

      CEvent(
         SECURITY_ATTRIBUTES *pSecurityAttributes,
         const IKernelObjectName &name);

      CEvent(
         const CEvent &rhs) = delete;

      CEvent &operator=(
         const CEvent &rhs) = delete;

      /// Reset the event to the unsignalled state.

      void Reset();

      /// Set the event to the signalled state.

      void Set();

      /// Pulse the event.

      void Pulse();

      // Implement IWaitable

      HANDLE GetWaitHandle() const override;

      void Wait() const override;

      bool Wait(
         Milliseconds timeout) const override;

   private :

      CSmartHandle m_hEvent;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Event.h
///////////////////////////////////////////////////////////////////////////////
