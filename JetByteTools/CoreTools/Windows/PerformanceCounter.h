#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: PerformanceCounter.h
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

#include "JetByteTools/CoreTools/Types.h"
#include "JetByteTools/CoreTools/tstring.h"

#include "JetByteTools/Win32Tools/ThreadAffinity.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

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

      LARGE_INTEGER GetRawCounter() const;

      LARGE_INTEGER GetCounter() const;

      LARGE_INTEGER GetFrequency() const;

      Milliseconds GetCounterMilliseconds() const;

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

      Win32::CThreadAffinity m_threadAffinity;

      LARGE_INTEGER m_start;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: PerformanceCounter.h
///////////////////////////////////////////////////////////////////////////////

