#pragma once
#ifndef JETBYTE_TOOLS_WIN32_PERFORMANCE_COUNTER_INCLUDED__
#define JETBYTE_TOOLS_WIN32_PERFORMANCE_COUNTER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: PerformanceCounter.h
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

#include "ThreadAffinity.h"
#include "tstring.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CPerformanceCounter
///////////////////////////////////////////////////////////////////////////////

/// A class that wraps the ::QueryPerformanceCounter() API for high performance
/// timing of sections of code.
/// \ingroup Win32

class CPerformanceCounter
{
   public :

      enum ThreadAffinity
      {
         NoThreadAffinity,
         LockThreadToOneCPU
      };

      /// Constructs a timer with the specified thread affinity and starts it.

      explicit CPerformanceCounter(
         ThreadAffinity threadAffinity = NoThreadAffinity);

      CPerformanceCounter(
         const CPerformanceCounter &rhs) = delete;

      CPerformanceCounter &operator=(
         const CPerformanceCounter &rhs) = delete;

      /// Resets a counter.

      void Reset();

      /// Gets the elapsed time since the counter was either constructed or
      /// reset.

      DWORD GetElapsedTimeAsDWORD() const;

      /// Gets the elapsed time since the counter was either constructed or
      /// reset.

      LARGE_INTEGER GetElapsedTime() const;

      /// Gets the elapsed time since the counter was either constructed or
      /// reset.

      _tstring GetElapsedTimeAsString() const;

   private :

      const LARGE_INTEGER m_frequency;

      CThreadAffinity m_threadAffinity;

      LARGE_INTEGER m_start;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_PERFORMANCE_COUNTER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: PerformanceCounter.h
///////////////////////////////////////////////////////////////////////////////

