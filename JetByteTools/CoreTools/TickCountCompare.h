#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: TickCountCompare.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2017 JetByte Limited.
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

#include "Types.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// CTickCountCompare
///////////////////////////////////////////////////////////////////////////////

class CTickCountCompare
{
   public:

      static constexpr DWORD s_defaultOverflowValue = 86400000;

      static DWORD Difference(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return a - b >= overflow ? b - a : a - b;
      }

      static bool FirstLessThanSecond(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return a - b >= overflow;
      }

      static bool FirstLessOrEqualToSecond(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return !FirstGreaterThanSecond(a, b, overflow);
      }

      static bool FirstGreaterThanSecond(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return b - a >= overflow;
      }

      static bool FirstGreaterOrEqualToSecond(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return a == b || FirstGreaterThanSecond(a, b, overflow);
      }


      #if (JETBYTE_CORE_DEPRECATE_OLD_AND_VAGUE_TICK_COUNT_COMPARE_METHODS == 0)
      static DWORD TickCountDifference(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return Difference(a, b, overflow);
      }

      static bool TickCountLess(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return FirstLessThanSecond(a, b, overflow);
      }

      static bool TickCountLessOrEqual(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return FirstLessOrEqualToSecond(a, b, overflow);
      }

      static bool TickCountGreater(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return FirstGreaterThanSecond(a, b, overflow);
      }

      static bool TickCountGreaterOrEqual(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return FirstGreaterOrEqualToSecond(a, b, overflow);
      }
      #endif
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: TickCountCompare.h
///////////////////////////////////////////////////////////////////////////////
