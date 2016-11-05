///////////////////////////////////////////////////////////////////////////////
// File: Event.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "Event.h"
#include "IKernelObjectName.h"
#include "Win32Exception.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static HANDLE Create(
   SECURITY_ATTRIBUTES *pEventAttributes, 
   bool bManualReset, 
   bool bInitialState, 
   LPCTSTR lpName,
   const CEvent::CreationFlags creationFlags);

static HANDLE Create(
   SECURITY_ATTRIBUTES *pEventAttributes, 
   LPCTSTR lpName);

///////////////////////////////////////////////////////////////////////////////
// CEvent
///////////////////////////////////////////////////////////////////////////////

CEvent::CEvent(
   SECURITY_ATTRIBUTES *pEventAttributes, 
   ResetType resetType, 
   InitialState initialState)
   :  m_hEvent(Create(pEventAttributes, (resetType == ManualReset), (initialState == Signaled), 0, CEvent::CreateOrConnect))
{

}

CEvent::CEvent(
   SECURITY_ATTRIBUTES *pEventAttributes, 
   ResetType resetType, 
   InitialState initialState,
   const IKernelObjectName &name,
   const CreationFlags creationFlags)
   :  m_hEvent(Create(pEventAttributes, (resetType == ManualReset), (initialState == Signaled), name.GetName().c_str(), creationFlags))
{

}

CEvent::CEvent(
   SECURITY_ATTRIBUTES *pEventAttributes, 
   const IKernelObjectName &name)
   :  m_hEvent(Create(pEventAttributes, name.GetName().c_str()))
{

}

void CEvent::Reset()
{
   if (!::ResetEvent(m_hEvent))
   {
      throw CWin32Exception(_T("CEvent::Reset()"), ::GetLastError());
   }
}

void CEvent::Set()
{
   if (!::SetEvent(m_hEvent))
   {
      throw CWin32Exception(_T("CEvent::Set()"), ::GetLastError());
   }
}

void CEvent::Pulse()
{
   if (!::PulseEvent(m_hEvent))
   {
      throw CWin32Exception(_T("CEvent::Pulse()"), ::GetLastError());
   }
}

HANDLE CEvent::GetWaitHandle() const
{
   return m_hEvent;
}

void CEvent::Wait() const
{
   IWaitable::Wait(m_hEvent);
}

bool CEvent::Wait(
   const Milliseconds timeout) const
{
   return IWaitable::Wait(m_hEvent, timeout);
}

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static HANDLE Create(
   SECURITY_ATTRIBUTES *pEventAttributes, 
   LPCTSTR lpName)
{
   static const bool notUsedWhenConnecting = false;

   return Create(pEventAttributes, notUsedWhenConnecting, notUsedWhenConnecting, lpName, CEvent::ConnectToExisting);
}


static HANDLE Create(
   SECURITY_ATTRIBUTES *pEventAttributes, 
   bool bManualReset, 
   bool bInitialState, 
   LPCTSTR lpName,
   const CEvent::CreationFlags creationFlags)
{
   ::SetLastError(ERROR_SUCCESS);

   CSmartHandle hEvent(::CreateEvent(pEventAttributes, bManualReset, bInitialState, lpName));

   const DWORD lastError = ::GetLastError();

   if (hEvent == NULL)
   {
      throw CWin32Exception(_T("CEvent::Create()"), lastError);
   }

   if (creationFlags == CEvent::CreateNew)
   {
      if (lastError == ERROR_ALREADY_EXISTS)
      {
         throw CWin32Exception(_T("CEvent::Create()"), lastError);
      }
   }
   else if (creationFlags == CEvent::ConnectToExisting)
   {
      if (lastError != ERROR_ALREADY_EXISTS)
      {
         throw CWin32Exception(_T("CEvent::Create()"), ERROR_NOT_FOUND);
      }
   }

   return hEvent.Detach();
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: Event.cpp
///////////////////////////////////////////////////////////////////////////////
