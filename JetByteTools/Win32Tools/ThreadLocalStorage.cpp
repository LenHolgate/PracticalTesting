///////////////////////////////////////////////////////////////////////////////
// File: ThreadLocalStorage.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2002 JetByte Limited.
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

#include "ThreadLocalStorage.h"
#include "Win32Exception.h"
#include "Utils.h"
#include "DebugTrace.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CThreadLocalStorage
///////////////////////////////////////////////////////////////////////////////

CThreadLocalStorage::CThreadLocalStorage()
   :  m_index(::TlsAlloc()),
      m_owner(true)
{
   if (m_index == TLS_OUT_OF_INDEXES)
   {
      throw CWin32Exception(_T("CThreadLocalStorage::CThreadLocalStorage()"), ::GetLastError());
   }
}

CThreadLocalStorage::CThreadLocalStorage(
   DWORD tlsIndex)
   :  m_index(tlsIndex),
      m_owner(false)
{
}

CThreadLocalStorage::~CThreadLocalStorage()
{
   try
   {
      if (m_owner)
      {
         if (0 == ::TlsFree(m_index))
         {
            DEBUG_ONLY(Output(_T("~CThreadLocalStorage() - error:") + GetLastErrorMessage(::GetLastError())));
         }
      }
   }
   JETBYTE_CATCH_ALL_IN_DESTRUCTORS_IF_ENABLED
   {
   }
}

DWORD CThreadLocalStorage::GetIndex() const
{
   return m_index;
}

void CThreadLocalStorage::ClearValue() const
{
   if (0 == ::TlsSetValue(m_index, 0))
   {
      throw CWin32Exception(_T("CThreadLocalStorage::ClearValue()"), ::GetLastError());
   }
}

void CThreadLocalStorage::SetValue(
   void *pValue) const
{
   if (0 == ::TlsSetValue(m_index, pValue))
   {
      throw CWin32Exception(_T("CThreadLocalStorage::SetValue()"), ::GetLastError());
   }
}

void CThreadLocalStorage::SetValue(
   const DWORD value) const
{
   if (0 == ::TlsSetValue(m_index, reinterpret_cast<void *>(static_cast<ULONG_PTR>(value))))
   {
      throw CWin32Exception(_T("CThreadLocalStorage::SetValue()"), ::GetLastError());
   }
}

void *CThreadLocalStorage::GetValue() const
{
   void *pV = ::TlsGetValue(m_index);

   if (pV == 0)
   {
      DWORD lastError = ::GetLastError();

      if (lastError != NO_ERROR)
      {
         throw CWin32Exception(_T("CThreadLocalStorage::GetValue()"), ::GetLastError());
      }
   }

   return pV;
}

DWORD CThreadLocalStorage::GetValueAsDWORD() const
{
	void *pV = ::TlsGetValue(m_index);

	if (pV == 0)
	{
		DWORD lastError = ::GetLastError();

		if (lastError != NO_ERROR)
		{
			throw CWin32Exception(_T("CThreadLocalStorage::GetValue()"), ::GetLastError());
		}
	}

	return static_cast<DWORD>(reinterpret_cast<ULONG_PTR>(pV));
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadLocalStorage.cpp
///////////////////////////////////////////////////////////////////////////////
