///////////////////////////////////////////////////////////////////////////////
// File: PerformanceCounter.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2018 JetByte Limited.
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

#include "PerformanceCounter.h"

#include "JetByteTools/CoreTools/ToString.h"
#include "JetByteTools/CoreTools/ErrorCodeException.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::CThreadAffinity;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static LARGE_INTEGER GetFrequencyInternal();

static LARGE_INTEGER GetCounterInternal();

///////////////////////////////////////////////////////////////////////////////
// CPerformanceCounter
///////////////////////////////////////////////////////////////////////////////

CPerformanceCounter::CPerformanceCounter(
   const ThreadAffinity threadAffinity)
   :  m_frequency(GetFrequencyInternal()),
      m_threadAffinity(threadAffinity == NoThreadAffinity ? CThreadAffinity::DontChangeAffinity : CThreadAffinity::SelectSingleProcessor),
      m_start(GetCounterInternal())
{
}

void CPerformanceCounter::Reset()
{
   m_start = GetCounterInternal();
}

LARGE_INTEGER CPerformanceCounter::GetRawCounter() const
{
   return GetCounterInternal();
}

LARGE_INTEGER CPerformanceCounter::GetFrequency() const
{
   return GetFrequencyInternal();
}

LARGE_INTEGER CPerformanceCounter::GetCounter() const
{
   const LARGE_INTEGER raw = GetCounterInternal();

   LARGE_INTEGER scaled {};

   scaled.QuadPart = raw.QuadPart / m_frequency.QuadPart;

   return scaled;
}

Milliseconds CPerformanceCounter::GetCounterMilliseconds() const
{
   const LARGE_INTEGER raw = GetCounterInternal();

   LARGE_INTEGER scaled {};

   scaled.QuadPart = raw.QuadPart / (m_frequency.QuadPart / 1000);

   return scaled.u.LowPart;
}

DWORD CPerformanceCounter::GetElapsedTimeAsDWORD() const
{
   const LARGE_INTEGER elapsed = GetElapsedTime();

   if (elapsed.u.HighPart != 0)
   {
      throw CException(_T("CPerformanceCounter::GetElapsedTimeAsDWORD()"), _T("Elapsed time does not fit into a DWORD"));
   }

   return elapsed.u.LowPart;
}

LARGE_INTEGER CPerformanceCounter::GetElapsedTime() const
{
   const LARGE_INTEGER now = GetCounterInternal();

   LARGE_INTEGER elapsed {};

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

static LARGE_INTEGER GetFrequencyInternal()
{
   LARGE_INTEGER frequency;

   if (!QueryPerformanceFrequency(&frequency))
   {
      throw CErrorCodeException(_T("CPerformanceCounter::GetFrequency()"), GetLastError());
   }

   return frequency;
}

static LARGE_INTEGER GetCounterInternal()
{
   LARGE_INTEGER counter;

   if (!QueryPerformanceCounter(&counter))
   {
      throw CErrorCodeException(_T("CPerformanceCounter::GetCounter()"), GetLastError());
   }

   return counter;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: PerformanceCounter.cpp
///////////////////////////////////////////////////////////////////////////////
