///////////////////////////////////////////////////////////////////////////////
// File: AutoResetEvent.cpp
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

#include "JetByteTools/Admin/Admin.h"

#include "AutoResetEvent.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CAutoResetEvent
///////////////////////////////////////////////////////////////////////////////

CAutoResetEvent::CAutoResetEvent(
   const CEvent::InitialState initialState,
   SECURITY_ATTRIBUTES *pSecurityAttributes)
   :  m_event(pSecurityAttributes, CEvent::AutoReset, initialState)
{

}

CAutoResetEvent::CAutoResetEvent(
   const IKernelObjectName &name,
   SECURITY_ATTRIBUTES *pSecurityAttributes,
   const CEvent::InitialState initialState)
   :  m_event(pSecurityAttributes, CEvent::AutoReset, initialState, name)
{

}

CAutoResetEvent::CAutoResetEvent(
   const IKernelObjectName &name,
   const CEvent::CreationFlags creationFlags,
   SECURITY_ATTRIBUTES *pSecurityAttributes,
   const CEvent::InitialState initialState)
   :  m_event(pSecurityAttributes, CEvent::AutoReset, initialState, name, creationFlags)
{

}

void CAutoResetEvent::Set()
{
   m_event.Set();
}

void CAutoResetEvent::Pulse()
{
   m_event.Pulse();
}

HANDLE CAutoResetEvent::GetWaitHandle() const
{
   return m_event.GetWaitHandle();
}

void CAutoResetEvent::Wait() const
{
   m_event.Wait();
}

bool CAutoResetEvent::Wait(
   const Milliseconds timeout) const
{
   return m_event.Wait(timeout);
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: AutoResetEvent.cpp
///////////////////////////////////////////////////////////////////////////////
