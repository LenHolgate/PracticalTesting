///////////////////////////////////////////////////////////////////////////////
// File: SmartHeapHandle.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2010 JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All 
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors 
// shall not be liable for any damages suffered by licensee as a result of 
// using the software. In no SmartHeapPointer will JetByte Limited be liable for any 
// lost revenue, profit or data, or for direct, indirect, special, 
// consequential, incidental or punitive damages, however caused and regardless 
// of the theory of liability, arising out of the use of or inability to use 
// software, even if JetByte Limited has been advised of the possibility of 
// such damages.
//
///////////////////////////////////////////////////////////////////////////////

#include "JetByteTools\Admin\Admin.h"

#include "SmartHeapHandle.h"
#include "Exception.h"
#include "Win32Exception.h"
#include "Utils.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CSmartHeapHandle
///////////////////////////////////////////////////////////////////////////////

CSmartHeapHandle::CSmartHeapHandle()
   :  m_handle(0)
{

}

CSmartHeapHandle::CSmartHeapHandle(
   HANDLE handle)
   :  m_handle(handle)
{

}

CSmartHeapHandle::CSmartHeapHandle(
   const CSmartHeapHandle &rhs)
   :  m_handle(rhs.DuplicateHandle().Detach())
{
}

CSmartHeapHandle::~CSmartHeapHandle()
{
   try
   {
      Close();
   }
   catch(CWin32Exception & /*e*/)
   {
   }
}

CSmartHeapHandle CSmartHeapHandle::DuplicateHandle(
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

      throw CWin32Exception(_T("CSmartHeapHandle::DuplicateHandle()"), lastError);
   }
   
   return CSmartHeapHandle(hOutput);
}

void CSmartHeapHandle::Attach(
   HANDLE handle)
{
   Close();

   m_handle = handle;
}

HANDLE CSmartHeapHandle::Detach()
{
   HANDLE handle = m_handle;

   m_handle = INVALID_HANDLE_VALUE;

   return handle;
}

CSmartHeapHandle CSmartHeapHandle::DuplicateHandle() const
{
   return DuplicateHandle(m_handle);
}

void CSmartHeapHandle::Close()
{
   if (IsValid())       // check for pseudo handles from current thread and process?
   {
      if (!::HeapDestroy(m_handle))
      {
         const DWORD lastError = ::GetLastError();

         OutputDebugString(_T("~CSmartHeapHandle() - ") + GetLastErrorMessage(lastError, false));
      }

      m_handle = 0;
   }
}

CSmartHeapHandle &CSmartHeapHandle::operator=(
   const HANDLE handle)
{
   if (handle != m_handle)
   {
      Close();

      m_handle = handle;
   }

   return *this;
}

CSmartHeapHandle &CSmartHeapHandle::operator=(
   const CSmartHeapHandle &rhs)
{
   CSmartHeapHandle copy(rhs);

   Close();

   *this = copy.Detach();

   return *this;
}

CSmartHeapHandle::operator HANDLE() const
{
   return m_handle;
}

HANDLE &CSmartHeapHandle::GetHandle()
{
   return m_handle;
}

HANDLE CSmartHeapHandle::GetHandle() const
{
   return m_handle;
}

bool CSmartHeapHandle::IsValid() const
{
   return m_handle != 0;
}


///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: SmartHeapHandle.cpp
///////////////////////////////////////////////////////////////////////////////
