///////////////////////////////////////////////////////////////////////////////
// File: ThreadAffinity.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2005 JetByte Limited.
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

#include "ThreadAffinity.h"
#include "Win32Exception.h"
#include "ToString.h"

#pragma hdrstop

#include <winbase.h>

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#include "DebugTrace.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static DWORD_PTR SetThreadAffinity(
   HANDLE hThread,
   const CThreadAffinity::SelectHow selectHow,
   DWORD_PTR affinityMask = 0);

static DWORD_PTR SelectSingleProcessor();

///////////////////////////////////////////////////////////////////////////////
// CThreadAffinity
///////////////////////////////////////////////////////////////////////////////

CThreadAffinity::CThreadAffinity(
   HANDLE hThread,
   const SelectHow selectHow,
   const DWORD_PTR affinityMask)
   :  m_previousThreadAffinity(SetThreadAffinity(hThread, selectHow, affinityMask)),
      m_hThread(hThread)
{

}

CThreadAffinity::CThreadAffinity(
   const SelectHow selectHow,
   const DWORD_PTR affinityMask)
   :  m_previousThreadAffinity(SetThreadAffinity(::GetCurrentThread(), selectHow, affinityMask)),
      m_hThread(::GetCurrentThread())
{

}

CThreadAffinity::~CThreadAffinity()
{
   try
   {
      if (m_previousThreadAffinity != 0)
      {
         const DWORD_PTR previousMask = SetThreadAffinity(m_hThread, UseAffinityMask, m_previousThreadAffinity);

         (void)previousMask;
      }
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
}

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static DWORD_PTR SetThreadAffinity(
   HANDLE hThread,
   const CThreadAffinity::SelectHow selectHow,
   DWORD_PTR affinityMask)
{
   DWORD_PTR previousMask = 0;

   switch (selectHow)
   {
      case CThreadAffinity::DontChangeAffinity :

         // Nothing to do

      break;

      case CThreadAffinity::SelectSingleProcessor :

         previousMask = SetThreadAffinity(hThread, CThreadAffinity::UseAffinityMask, SelectSingleProcessor());

      break;

      case CThreadAffinity::UseAffinityMask :

         previousMask = ::SetThreadAffinityMask(hThread, affinityMask);

         if (0 == previousMask)
         {
            throw CWin32Exception(_T("CThreadAffinity::SetThreadAffinity()"), ::GetLastError());
         }

      break ;

      default :

         throw CException(_T("CThreadAffinity::SetThreadAffinity()"), _T("Unknown 'selectHow' value: ") + ToString(selectHow));
   }

   return previousMask;
}

static DWORD_PTR SelectSingleProcessor()
{
   DWORD_PTR processAffinity;

   DWORD_PTR systemAffinity;

   if (!::GetProcessAffinityMask(::GetCurrentProcess(), &processAffinity, &systemAffinity))
   {
      throw CWin32Exception(_T("CThreadAffinity::SelectSingleProcessor()"), ::GetLastError());
   }

// 11/3/2010 - #pragma JETBYTE_TODO("Make this static so we select a different one each time")
   DWORD_PTR affinityMask = 1;

   // need to loop around this code twice so we guarentee to try all possible bit positions

   for (size_t i = 0; i < sizeof(DWORD_PTR); ++i)
   {
      if ((affinityMask & processAffinity) == affinityMask)
      {
         return affinityMask;
      }

      affinityMask <<= 1;
   }

   throw CException(_T("CThreadAffinity::SelectSingleProcessor()"), _T("Unexpected - processs has no affinity for any processor!"));
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadAffinity.cpp
///////////////////////////////////////////////////////////////////////////////

