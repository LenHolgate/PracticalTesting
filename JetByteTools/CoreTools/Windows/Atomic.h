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

#include <Windows.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// TAtomicOperations
///////////////////////////////////////////////////////////////////////////////

template <typename T>
class TAtomicOperations
{
   public :

      static T Increment(
         T *pValue);

      static T Decrement(
         T *pValue);

      static T Add(
         T *pAddend,
         T value);

      static T Exchange(
         T *pValue,
         T newValue);

      static T CompareExchange(
         T *pValue,
         T exchangeValue,
         T compareValue);
};

template <>
class TAtomicOperations<__int64>
{
   public :

      typedef __int64 T;

      static T Increment(
         T *pValue)
      {
         return ::InterlockedIncrement64(pValue);
      }

      static T Decrement(
         T *pValue)
      {
         return ::InterlockedDecrement64(pValue);
      }

      static T Add(
         T *pAddend,
         const T value)
      {
         return ::InterlockedExchangeAdd64(pAddend, value);
      }

      static T Subtract(
         T *pSubtrahend,
         const T value)
      {
         return ::InterlockedExchangeAdd64(pSubtrahend, -value);
      }

      static T Exchange(
         T *pValue,
         const T newValue)
      {
         return ::InterlockedExchange64(pValue, newValue);
      }

      static T CompareExchange(
         T *pValue,
         const T exchangeValue,
         const T compareValue)
      {
         return ::InterlockedCompareExchange64(pValue, exchangeValue, compareValue);
      }
};

template <>
class TAtomicOperations<long>
{
   public :

      typedef long T;

      static T Increment(
         T *pValue)
      {
         return ::InterlockedIncrement(pValue);
      }

      static T Decrement(
         T *pValue)
      {
         return ::InterlockedDecrement(pValue);
      }

      static T Add(
         T *pAddend,
         const T value)
      {
         return ::InterlockedExchangeAdd(pAddend, value);
      }

      static T Subtract(
         T *pSubtrahend,
         const T value)
      {
         return ::InterlockedExchangeAdd(pSubtrahend, -value);
      }

      static T Exchange(
         T *pValue,
         const T newValue)
      {
         return ::InterlockedExchange(pValue, newValue);
      }

      static T CompareExchange(
         T *pValue,
         const T exchangeValue,
         const T compareValue)
      {
         return ::InterlockedCompareExchange(pValue, exchangeValue, compareValue);
      }
};

template <>
class TAtomicOperations<unsigned long>
{
   public :

      typedef unsigned long T;

      static T Increment(
         T *pValue)
      {
         return ::InterlockedIncrement(pValue);
      }

      static T Decrement(
         T *pValue)
      {
         return ::InterlockedDecrement(pValue);
      }

      static T Add(
         T *pAddend,
         const T value)
      {
         return ::InterlockedExchangeAdd(pAddend, value);
      }

      static T Subtract(
         T *pSubtrahend,
         const T value)
      {
         return ::InterlockedExchangeSubtract(pSubtrahend, value);
      }

      static T Exchange(
         T *pValue,
         const T newValue)
      {
         return ::InterlockedExchange(pValue, newValue);
      }

      static T CompareExchange(
         T *pValue,
         const T exchangeValue,
         const T compareValue)
      {
         return ::InterlockedCompareExchange(pValue, exchangeValue, compareValue);
      }
};

template <>
class TAtomicOperations<void *>
{
   public :

   typedef void * T;

   //static T Increment(
   //   T *pValue)
   //{
   //   return ::InterlockedIncrement(pValue);
   //}

   //static T Decrement(
   //   T *pValue)
   //{
   //   return ::InterlockedDecrement(pValue);
   //}

   //static T Add(
   //   T *pAddend,
   //   const T value)
   //{
   //   return ::InterlockedExchangeAdd(pAddend, value);
   //}

   //static T Subtract(
   //   T *pSubtrahend,
   //   const T value)
   //{
   //   return ::InterlockedExchangeSubtract(pSubtrahend, value);
   //}

   static T Exchange(
      T *pValue,
      const T newValue)
   {
      return ::InterlockedExchangePointer(pValue, newValue);
   }

   static T CompareExchange(
      T *pValue,
      const T exchangeValue,
      const T compareValue)
   {
      return ::InterlockedCompareExchangePointer(pValue, exchangeValue, compareValue);
   }
};

template <>
class TAtomicOperations<unsigned char *>
{
   public :

   typedef unsigned char * T;

   //static T Increment(
   //   T *pValue)
   //{
   //   return ::InterlockedIncrement(pValue);
   //}

   //static T Decrement(
   //   T *pValue)
   //{
   //   return ::InterlockedDecrement(pValue);
   //}

   //static T Add(
   //   T *pAddend,
   //   const T value)
   //{
   //   return ::InterlockedExchangeAdd(pAddend, value);
   //}

   //static T Subtract(
   //   T *pSubtrahend,
   //   const T value)
   //{
   //   return ::InterlockedExchangeSubtract(pSubtrahend, value);
   //}

   static T Exchange(
      T *pValue,
      const T newValue)
   {
      return static_cast<T >(::InterlockedExchangePointer(reinterpret_cast<void **>(pValue), newValue));
   }

   static T CompareExchange(
      T *pValue,
      const T exchangeValue,
      const T compareValue)
   {
      return static_cast<T >(::InterlockedCompareExchangePointer(reinterpret_cast<void **>(pValue), exchangeValue, compareValue));
   }
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Atomic.h
///////////////////////////////////////////////////////////////////////////////
