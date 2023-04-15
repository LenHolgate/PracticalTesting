///////////////////////////////////////////////////////////////////////////////
// File: Event.cpp
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

#include "JetByteTools/Admin/Admin.h"

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
   CEvent::CreationFlags creationFlags);

static HANDLE Create(
   SECURITY_ATTRIBUTES *pEventAttributes,
   LPCTSTR lpName);

///////////////////////////////////////////////////////////////////////////////
// CEvent
///////////////////////////////////////////////////////////////////////////////

CEvent::CEvent(
   SECURITY_ATTRIBUTES *pSecurityAttributes,
   const ResetType resetType,
   const InitialState initialState)
   :  m_hEvent(Create(pSecurityAttributes, (resetType == ManualReset), (initialState == Signaled), nullptr, CreateOrConnect))
{

}

CEvent::CEvent(
   SECURITY_ATTRIBUTES *pSecurityAttributes,
   const ResetType resetType,
   const InitialState initialState,
   const IKernelObjectName &name,
   const CreationFlags creationFlags)
   :  m_hEvent(Create(pSecurityAttributes, (resetType == ManualReset), (initialState == Signaled), name.GetName().c_str(), creationFlags))
{

}

CEvent::CEvent(
   SECURITY_ATTRIBUTES *pSecurityAttributes,
   const IKernelObjectName &name)
   :  m_hEvent(Create(pSecurityAttributes, name.GetName().c_str()))
{

}

void CEvent::Reset()
{
   if (!ResetEvent(m_hEvent))
   {
      throw CWin32Exception(_T("CEvent::Reset()"), GetLastError());
   }
}

void CEvent::Set()
{
   if (!SetEvent(m_hEvent))
   {
      const DWORD lastError = GetLastError();

      throw CWin32Exception(_T("CEvent::Set()"), lastError);
   }
}

void CEvent::Pulse()
{
   if (!PulseEvent(m_hEvent))
   {
      const DWORD lastError = GetLastError();

      throw CWin32Exception(_T("CEvent::Pulse()"), lastError);
   }
}

HANDLE CEvent::GetWaitHandle() const
{
   return m_hEvent;
}

void CEvent::Wait() const
{
   WaitForHandle(m_hEvent);
}

bool CEvent::Wait(
   const Milliseconds timeout) const
{
   return WaitForHandle(m_hEvent, timeout);
}

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static HANDLE Create(
   SECURITY_ATTRIBUTES *pEventAttributes,
   const LPCTSTR lpName)
{
   static const bool notUsedWhenConnecting = false;

   return Create(pEventAttributes, notUsedWhenConnecting, notUsedWhenConnecting, lpName, CEvent::ConnectToExisting);
}


static HANDLE Create(
   SECURITY_ATTRIBUTES *pEventAttributes,
   const bool bManualReset,
   const bool bInitialState,
   const LPCTSTR lpName,
   const CEvent::CreationFlags creationFlags)
{
   SetLastError(ERROR_SUCCESS);

   CSmartHandle hEvent;

   if (creationFlags == CEvent::ConnectToExisting ||
       creationFlags == CEvent::CreateOrConnect)
   {
      hEvent.Attach(::OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, lpName));

      if (creationFlags == CEvent::ConnectToExisting &&
          hEvent == nullptr)
      {
         const DWORD lastError = GetLastError();

         throw CWin32Exception(_T("CEvent::Create()"), lastError);
      }
   }

   if (!hEvent.IsValid() &&
      (creationFlags == CEvent::CreateNew ||
       creationFlags == CEvent::CreateOrConnect))
   {
      hEvent.Attach(::CreateEvent(pEventAttributes, bManualReset, bInitialState, lpName));

      const DWORD lastError = GetLastError();

      if (hEvent == nullptr)
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
