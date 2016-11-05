///////////////////////////////////////////////////////////////////////////////
// File: PerformanceCounter.cpp
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

#include "PerformanceCounter.h"
#include "Win32Exception.h"
#include "Utils.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static LARGE_INTEGER GetFrequency();

static LARGE_INTEGER GetCounter();

///////////////////////////////////////////////////////////////////////////////
// CPerformanceCounter
///////////////////////////////////////////////////////////////////////////////

CPerformanceCounter::CPerformanceCounter(
   ThreadAffinity threadAffinity)
   :  m_frequency(GetFrequency()),
      m_threadAffinity(threadAffinity == NoThreadAffinity ? CThreadAffinity::DontChangeAffinity : CThreadAffinity::SelectSingleProcessor),
      m_start(GetCounter())
{

}

void CPerformanceCounter::Reset()
{
   m_start = GetCounter();
}

DWORD CPerformanceCounter::GetElapsedTimeAsDWORD() const
{
   const LARGE_INTEGER elapsed = GetElapsedTime();

   if (elapsed.HighPart != 0)
   {
      throw CException(_T("CPerformanceCounter::GetElapsedTimeAsDWORD()"), _T("Elapsed time does not fit into a DWORD"));
   }

   return elapsed.LowPart;
}

LARGE_INTEGER CPerformanceCounter::GetElapsedTime() const
{
   const LARGE_INTEGER now = GetCounter();

   LARGE_INTEGER elapsed;

   elapsed.QuadPart = (now.QuadPart - m_start.QuadPart) / (m_frequency.QuadPart / 1000);

   return elapsed;
}

_tstring CPerformanceCounter::GetElapsedTimeAsString() const
{
   const LARGE_INTEGER elapsed = GetElapsedTime();

   return ToString(elapsed.QuadPart) + _T("ms");
}

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static LARGE_INTEGER GetFrequency()
{
   LARGE_INTEGER frequency;

   if (!::QueryPerformanceFrequency(&frequency))
   {
      throw CWin32Exception(_T("CPerformanceCounter::GetFrequency()"), ::GetLastError());
   }

   return frequency;
}

static LARGE_INTEGER GetCounter()
{
   LARGE_INTEGER counter;

   if (!::QueryPerformanceCounter(&counter))
   {
      throw CWin32Exception(_T("CPerformanceCounter::GetCounter()"), ::GetLastError());
   }

   return counter;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: PerformanceCounter.cpp
///////////////////////////////////////////////////////////////////////////////
