///////////////////////////////////////////////////////////////////////////////
// File: ThreadAffinity.cpp
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

#include "ThreadAffinity.h"
#include "Win32Exception.h"

#include "JetByteTools/CoreTools/ToString.h"
#include "JetByteTools/CoreTools/DebugTrace.h"

#pragma hdrstop

#include <WinBase.h>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Core::CException;
using JetByteTools::Core::ToString;

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
   CThreadAffinity::SelectHow selectHow,
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
   :  m_previousThreadAffinity(SetThreadAffinity(GetCurrentThread(), selectHow, affinityMask)),
      m_hThread(GetCurrentThread())
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
   const DWORD_PTR affinityMask)
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

         previousMask = SetThreadAffinityMask(hThread, affinityMask);

         if (0 == previousMask)
         {
            throw CWin32Exception(_T("CThreadAffinity::SetThreadAffinity()"), GetLastError());
         }

      break;

      default :

         throw CException(_T("CThreadAffinity::SetThreadAffinity()"), _T("Unknown 'selectHow' value: ") + ToString(selectHow));
   }

   return previousMask;
}

static DWORD_PTR SelectSingleProcessor()
{
   DWORD_PTR processAffinity;

   DWORD_PTR systemAffinity;

   if (!GetProcessAffinityMask(GetCurrentProcess(), &processAffinity, &systemAffinity))
   {
      throw CWin32Exception(_T("CThreadAffinity::SelectSingleProcessor()"), GetLastError());
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
