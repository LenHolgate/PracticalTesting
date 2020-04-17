///////////////////////////////////////////////////////////////////////////////
// File: CriticalSection.cpp
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

#include "CriticalSection.h"
#include "Utils.h"
#include "Win32Exception.h"

#pragma hdrstop

#if (JETBYTE_DEPRECATE_CRITICAL_SECTION == 0)

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CCriticalSection
///////////////////////////////////////////////////////////////////////////////

CCriticalSection::CCriticalSection()
   : m_crit()
{
   // On some platforms...
   // Can fail under low memory conditions and rase a STATUS_NO_MEMORY
   // exception.

   InitializeCriticalSection(&m_crit);
}

CCriticalSection::CCriticalSection(
   const DWORD spinCount)
   :  m_crit()
{
   if (!InitializeCriticalSectionAndSpinCount(&m_crit, spinCount))
   {
      const DWORD lastError = GetLastError();

      throw CWin32Exception(_T("CCriticalSection::CCriticalSection()"), lastError);
   }
}

CCriticalSection::~CCriticalSection()
{
   DeleteCriticalSection(&m_crit);
}

void CCriticalSection::SetSpinCount(
   const DWORD spinCount)
{
   SetCriticalSectionSpinCount(&m_crit, spinCount);
}

bool CCriticalSection::TryEnter()
{
   return ToBool(TryEnterCriticalSection(&m_crit));
}

void CCriticalSection::Enter()
{
   // On some platforms...
   // Can fail if there's contention and the event can't be created and will
   // raise an EXCEPTION_INVALID_HANDLE exception.

   EnterCriticalSection(&m_crit);
}

void CCriticalSection::Leave()
{
   LeaveCriticalSection(&m_crit);
}

bool CCriticalSection::IsLockedByThisThread() const
{
   const DWORD threadId = GetCurrentThreadId();

   return m_crit.OwningThread == reinterpret_cast<HANDLE>(static_cast<ULONG_PTR>(threadId));
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#else
SuppressLNK4221Warning()
#endif // JETBYTE_DEPRECATE_CRITICAL_SECTION

///////////////////////////////////////////////////////////////////////////////
// End of file: CriticalSection.cpp
///////////////////////////////////////////////////////////////////////////////
