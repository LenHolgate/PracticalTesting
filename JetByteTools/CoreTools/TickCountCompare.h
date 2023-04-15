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

      static const DWORD s_defaultOverflowValue = 86400000;

      static DWORD TickCountDifference(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return a - b >= overflow ? b - a : a - b;
      }

      static DWORD TickCountLess(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return a - b >= overflow;
      }

      static DWORD TickCountLessOrEqual(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return !TickCountGreater(a, b, overflow);
      }

      static DWORD TickCountGreater(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return b - a >= overflow;
      }

      static DWORD TickCountGreaterOrEqual(
         const DWORD a,
         const DWORD b,
         const DWORD overflow = s_defaultOverflowValue)
      {
         return a == b || TickCountGreater(a, b, overflow);
      }
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: TickCountCompare.h
///////////////////////////////////////////////////////////////////////////////
