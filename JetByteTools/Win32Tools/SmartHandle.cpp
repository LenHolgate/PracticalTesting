///////////////////////////////////////////////////////////////////////////////
// File: SmartHandle.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2005 JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors
// shall not be liable for any damages suffered by licensee as a result of
// using the software. In no SmartHandle will JetByte Limited be liable for any
// lost revenue, profit or data, or for direct, indirect, special,
// consequential, incidental or punitive damages, however caused and regardless
// of the theory of liability, arising out of the use of or inability to use
// software, even if JetByte Limited has been advised of the possibility of
// such damages.
//
///////////////////////////////////////////////////////////////////////////////

#include "JetByteTools\Admin\Admin.h"

#include "SmartHandle.h"
#include "Exception.h"
#include "Win32Exception.h"

#pragma hdrstop

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#include "DebugTrace.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

static const HANDLE s_currentThreadPseudoHandle = ::GetCurrentThread();
static const HANDLE s_currentProcessPseudoHandle = ::GetCurrentProcess();

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
      const DWORD lastError = ::GetLastError();

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
   HANDLE handle = m_handle;

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
         if (!::CloseHandle(m_handle))
         {
            const DWORD lastError = ::GetLastError();

            throw CWin32Exception(_T("CSmartHandle::Close()"), lastError);
         }
      }

      m_handle = INVALID_HANDLE_VALUE;
   }
} //lint !e1578 (Pointer member (m_handle) neither freed nor zeroed by cleanup function)

CSmartHandle &CSmartHandle::operator=(const HANDLE handle)
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
} //lint !e1529 (Symbol not first checking for assignment to this -- Effective C++ #17 & Eff. C++ 3rd Ed. item 11)

CSmartHandle::operator HANDLE() const
{
   return m_handle;
}

HANDLE &CSmartHandle::GetHandle()
{
   return m_handle; //lint !e1536 (Exposing low access member -- Effective C++ #30)
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
   IWaitable::Wait(m_handle);
}

bool CSmartHandle::Wait(
   const Milliseconds timeoutMillis) const
{
   return IWaitable::Wait(m_handle, timeoutMillis);
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
