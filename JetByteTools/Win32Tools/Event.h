#pragma once
#ifndef JETBYTE_TOOLS_WIN32_EVENT_INCLUDED__
#define JETBYTE_TOOLS_WIN32_EVENT_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: Event.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 1997 JetByte Limited.
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

#include "JetByteTools/Admin/Types.h"

#include "SmartHandle.h"
#include "IWaitable.h"

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

class CEvent : public IWaitable
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

#endif // JETBYTE_TOOLS_WIN32_EVENT_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Event.h
///////////////////////////////////////////////////////////////////////////////
