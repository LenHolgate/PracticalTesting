#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_MANUAL_RESET_EVENT_INCLUDED__
#define JETBYTE_TOOLS_WIN32_MANUAL_RESET_EVENT_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ManualResetEvent.h
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

#include "Event.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CManualResetEvent
///////////////////////////////////////////////////////////////////////////////

/// A class that wraps the operating system
/// <a href="http://msdn2.microsoft.com/en-us/library/ms682655.aspx">Event API</a>
/// and exposes just the interface for an manual reset event.
/// \ingroup Synchronization
/// \ingroup KernelObjects

class CManualResetEvent : public IWaitable
{
   public :

      /// Create an anonymous CManualResetEvent object for in-process use.

      explicit CManualResetEvent(
         CEvent::InitialState initialState = CEvent::NonSignaled,
         SECURITY_ATTRIBUTES *pSecurityAttributes = 0);

      /// Create a new named CManualResetEvent object or connect to an existing
      /// one with the same name, for cross-process use.

      explicit CManualResetEvent(
         const IKernelObjectName &name,
         SECURITY_ATTRIBUTES *pSecurityAttributes = 0,
         CEvent::InitialState initialState = CEvent::NonSignaled);

      /// Create a new named CManualResetEvent object or connect to an existing
      /// one with the same name, for cross-process use. Use the CreationFlags
      /// to force creation only or connection only if required.

      CManualResetEvent(
         const IKernelObjectName &name,
         const CEvent::CreationFlags creationFlags,
         SECURITY_ATTRIBUTES *pSecurityAttributes = 0,
         CEvent::InitialState initialState = CEvent::NonSignaled);

      /// Set the event to the signalled state.

      void Set();

      /// Resets the event to the unsignalled state.

      void Reset();

      /// Pulse the event. Releases all waiting threads and resets the event.

      void Pulse();

      // Implement IWaitable

      virtual HANDLE GetWaitHandle() const;

      virtual void Wait() const;

      virtual bool Wait(
         const Milliseconds timeoutMillis) const;

   private :

      CEvent m_event;

      /// No copies do not implement
      CManualResetEvent(const CManualResetEvent &rhs);
      /// No copies do not implement
      CManualResetEvent &operator=(const CManualResetEvent &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_MANUAL_RESET_EVENT_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ManualResetEvent.h
///////////////////////////////////////////////////////////////////////////////

