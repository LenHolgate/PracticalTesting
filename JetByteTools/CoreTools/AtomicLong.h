#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: AtomicLong.h
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

#include "Atomic.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// TAtomicLong
///////////////////////////////////////////////////////////////////////////////

template <typename T>
class TAtomicLong
{
   public :

      TAtomicLong()
         :  m_value{}
      {
      }

      explicit TAtomicLong(
         const T value)
         :  m_value(value)
      {
      }

      TAtomicLong(
         const TAtomicLong &rhs) = delete;

      TAtomicLong &operator=(
         const TAtomicLong &rhs) = delete;

      T Value() const
      {
         return m_value.AccessValue();
      }

      void Set(
         const T value)
      {
         m_value.Set(value);
      }

      T Increment()
      {
         return m_value.Increment();
      }

      T Decrement()
      {
         return m_value.Decrement();
      }

      T Add(
         const T value)
      {
         return m_value.Add(value);
      }

      T Subtract(
         const T value)
      {
         return m_value.Subtract(value);
      }

   private :

      TAtomic<T> m_value;
};

///////////////////////////////////////////////////////////////////////////////
// CAtomicLong
///////////////////////////////////////////////////////////////////////////////

typedef TAtomicLong<long> CAtomicLong;

///////////////////////////////////////////////////////////////////////////////
// CAtomicULong
///////////////////////////////////////////////////////////////////////////////

typedef TAtomicLong<unsigned long> CAtomicULong;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: AtomicLong.h
///////////////////////////////////////////////////////////////////////////////
