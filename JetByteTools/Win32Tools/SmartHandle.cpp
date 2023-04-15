///////////////////////////////////////////////////////////////////////////////
// File: SmartHandle.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2005 JetByte Limited.
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

#include "SmartHandle.h"
#include "Win32Exception.h"

#include "JetByteTools/CoreTools/DebugTrace.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

static HANDLE s_currentThreadPseudoHandle = GetCurrentThread();
static HANDLE s_currentProcessPseudoHandle = GetCurrentProcess();

///////////////////////////////////////////////////////////////////////////////
// CSmartHandle
///////////////////////////////////////////////////////////////////////////////

CSmartHandle::CSmartHandle()
   :  m_handle(INVALID_HANDLE_VALUE)
{
}

CSmartHandle::CSmartHandle(
   HANDLE handle)
   :  m_handle(handle)
{

}

CSmartHandle::CSmartHandle(
   const CSmartHandle &rhs)
   :  IWaitable(rhs),
      m_handle(rhs.DuplicateHandle().Detach())
{
}

CSmartHandle::~CSmartHandle()
{
   try
   {
      Close();
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
}

CSmartHandle CSmartHandle::DuplicateHandle(
   HANDLE hInput)
{
   HANDLE hOutput;

   if (!::DuplicateHandle(
      GetCurrentProcess(),
      hInput,
      GetCurrentProcess(),
      &hOutput,
      0,
      FALSE,
      DUPLICATE_SAME_ACCESS))
   {
      const DWORD lastError = GetLastError();

      throw CWin32Exception(_T("CSmartHandle::DuplicateHandle()"), lastError);
   }

   return CSmartHandle(hOutput);
}

void CSmartHandle::Attach(
   HANDLE handle)
{
   Close();

   m_handle = handle;
}

HANDLE CSmartHandle::Detach()
{
   auto const handle = m_handle;

   m_handle = INVALID_HANDLE_VALUE;

   return handle;
}

CSmartHandle CSmartHandle::DuplicateHandle() const
{
   return DuplicateHandle(m_handle);
}

void CSmartHandle::Close()
{
   if (IsValid())
   {
      if (m_handle != s_currentProcessPseudoHandle &&
          m_handle != s_currentThreadPseudoHandle)
      {
         if (!CloseHandle(m_handle))
         {
            const DWORD lastError = GetLastError();

            throw CWin32Exception(_T("CSmartHandle::Close()"), lastError);
         }
      }

      m_handle = INVALID_HANDLE_VALUE;
   }
}

CSmartHandle &CSmartHandle::operator=(
   HANDLE handle)
{
   if (handle != m_handle)
   {
      Close();

      m_handle = handle;
   }

   return *this;
}

CSmartHandle &CSmartHandle::operator=(
   const CSmartHandle &rhs)
{
   CSmartHandle copy(rhs);

   Close();

   *this = copy.Detach();

   return *this;
}

CSmartHandle::operator HANDLE() const
{
   return m_handle;
}

HANDLE &CSmartHandle::GetHandle()
{
   return m_handle;
}

HANDLE CSmartHandle::GetHandle() const
{
   return m_handle;
}

HANDLE CSmartHandle::GetWaitHandle() const
{
   return m_handle;
}

void CSmartHandle::Wait() const
{
   WaitForHandle(m_handle);
}

bool CSmartHandle::Wait(
   const Milliseconds timeoutMillis) const
{
   return WaitForHandle(m_handle, timeoutMillis);
}

bool CSmartHandle::IsValid() const
{
   return m_handle != INVALID_HANDLE_VALUE && m_handle != nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: SmartHandle.cpp
///////////////////////////////////////////////////////////////////////////////
