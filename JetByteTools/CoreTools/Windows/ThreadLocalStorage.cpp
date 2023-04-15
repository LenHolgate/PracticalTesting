///////////////////////////////////////////////////////////////////////////////
// File: ThreadLocalStorage.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2002 JetByte Limited.
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

#include "ThreadLocalStorage.h"

#include "JetByteTools/CoreTools/DebugTrace.h"
#include "JetByteTools/CoreTools/ErrorCodeException.h"
#include "JetByteTools/CoreTools/ErrorCodeToErrorMessage.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// CThreadLocalStorage
///////////////////////////////////////////////////////////////////////////////

CThreadLocalStorage::CThreadLocalStorage()
   :  m_index(TlsAlloc()),
      m_owner(true)
{
   if (m_index == TLS_OUT_OF_INDEXES)
   {
      throw CErrorCodeException(_T("CThreadLocalStorage::CThreadLocalStorage()"), GetLastError());
   }
}

CThreadLocalStorage::CThreadLocalStorage(
   const DWORD tlsIndex)
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
         if (0 == TlsFree(m_index))
         {
            DEBUG_ONLY(Output(_T("~CThreadLocalStorage() - error:") + ErrorCodeToErrorMessage(::GetLastError())));
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
   if (0 == TlsSetValue(m_index, nullptr))
   {
      throw CErrorCodeException(_T("CThreadLocalStorage::ClearValue()"), GetLastError());
   }
}

void CThreadLocalStorage::SetValue(
   void *pValue) const
{
   if (0 == TlsSetValue(m_index, pValue))
   {
      throw CErrorCodeException(_T("CThreadLocalStorage::SetValue()"), GetLastError());
   }
}

void CThreadLocalStorage::SetValue(
   const DWORD value) const
{
   if (0 == TlsSetValue(m_index, reinterpret_cast<void *>(static_cast<ULONG_PTR>(value))))
   {
      throw CErrorCodeException(_T("CThreadLocalStorage::SetValue()"), GetLastError());
   }
}

void *CThreadLocalStorage::GetValue() const
{
   void *pV = TlsGetValue(m_index);

   if (pV == nullptr)
   {
      const DWORD lastError = GetLastError();

      if (lastError != NO_ERROR)
      {
         throw CErrorCodeException(_T("CThreadLocalStorage::GetValue()"), GetLastError());
      }
   }

   return pV;
}

DWORD CThreadLocalStorage::GetValueAsDWORD() const
{
   void *pV = TlsGetValue(m_index);

   if (pV == nullptr)
   {
      const DWORD lastError = GetLastError();

      if (lastError != NO_ERROR)
      {
         throw CErrorCodeException(_T("CThreadLocalStorage::GetValue()"), GetLastError());
      }
   }

   return static_cast<DWORD>(reinterpret_cast<ULONG_PTR>(pV));
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadLocalStorage.cpp
///////////////////////////////////////////////////////////////////////////////
