#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: Atomic.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2019 JetByte Limited.
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

#include "JetByteTools/Admin/Platform.h"

#if defined(JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM)
#include "JetByteTools/CoreTools/Windows/Atomic.h"
#endif

#include <limits>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// CAtomic
///////////////////////////////////////////////////////////////////////////////

#if (JETBYTE_ATOMIC_USE_STD_ATOMIC == 1)
using JetByteTools::Core::Std::TAtomicOperations;
#elif defined(JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM)
using JetByteTools::Core::Windows::TAtomicOperations;
#endif

template <typename T>
class TAtomic
{
   public :

      static T Increment(
         T &value)
      {
         return TAtomicOperations<T>::Increment(&value);
      }

      static T Increment(
         T *pValue)
      {
         return TAtomicOperations<T>::Increment(pValue);
      }

      static T Decrement(
         T &value)
      {
         return TAtomicOperations<T>::Decrement(&value);
      }

      static T Decrement(
         T *pValue)
      {
         return TAtomicOperations<T>::Decrement(pValue);
      }

      static T Add(
         T &addend,
         const T value)
      {
         return TAtomicOperations<T>::Add(&addend, value);
      }

      static T Add(
         T *pAddend,
         const T value)
      {
         return TAtomicOperations<T>::Add(pAddend, value);
      }

      static T Subtract(
         T &subtrahend,
         const T value)
      {
         return TAtomicOperations<T>::Subtract(&subtrahend, value);
      }

      static T Subtract(
         T *pSubtrahend,
         const T value)
      {
         return TAtomicOperations<T>::Subtract(pSubtrahend, value);
      }

      static void Set(
         T &value,
         const T newValue)
      {
         TAtomicOperations<T>::Exchange(&value, newValue);
      }

      static void Set(
         T *pValue,
         const T newValue)
      {
         TAtomicOperations<T>::Exchange(pValue, newValue);
      }

      static T Exchange(
         T &value,
         const T newValue)
      {
         return TAtomicOperations<T>::Exchange(&value, newValue);
      }

      static T Exchange(
         T *pValue,
         const T newValue)
      {
         return TAtomicOperations<T>::Exchange(pValue, newValue);
      }

      static T CompareExchange(
         T &value,
         const T exchangeValue,
         const T compareValue)
      {
         return TAtomicOperations<T>::CompareExchange(&value, exchangeValue, compareValue);
      }

      static T CompareExchange(
         T *pValue,
         const T exchangeValue,
         const T compareValue)
      {
         return TAtomicOperations<T>::CompareExchange(pValue, exchangeValue, compareValue);
      }

      TAtomic()
         : m_value{}
      {
      }

      TAtomic(
         const T value)
         : m_value(value)
      {
      }

      void Set(
         const T newValue)
      {
         Exchange(newValue);
      }

      T Increment()
      {
         return Increment(&m_value);
      }

      T Decrement()
      {
         return Decrement(&m_value);
      }

      T Add(
         const T value)
      {
         return Add(&m_value, value);
      }

      T Subtract(
         const T value)
      {
         return Subtract(&m_value, value);
      }

      T Exchange(
         const T value)
      {
         return Exchange(&m_value, value);
      }

      T CompareExchange(
         const T exchangeValue,
         const T compareValue)
      {
         return CompareExchange(&m_value, exchangeValue, compareValue);
      }

      T AccessValue() const 
      {
         // Use InterlockedCompareExchange to exchange an unlikely
         // value for itself as it is unlikely to cause the exchange
         // to happen and if it does it doesn't change the value.
         // Avoiding the exchange means we can avoid dirtying the cache line.
         // Of course we could access the value directly as that would
         // be atomic in terms of accessing the value but imposes no
         // synchronisation or memory ordering semantics - which we may want...

         static const T unlikelyValue = std::numeric_limits<T>::max();

         return const_cast<TAtomic<T> *>(this)->CompareExchange(unlikelyValue,unlikelyValue);
      }

   private :

      T m_value;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Atomic.h
///////////////////////////////////////////////////////////////////////////////
