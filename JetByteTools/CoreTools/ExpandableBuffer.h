#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ExpandableBuffer.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 1998 JetByte Limited.
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
#include "DebugTrace.h"
#include "Exception.h"
#include "ExceptionLeakPrevention.h"
#include "CheckedMemset.h"

#if JETBYTE_CORE_DUMP_ON_EXPANDABLE_BUFFER_BOUNDS_CHECK_FAILURE == 1
#include "CrashDumpGenerator.h"
#endif

#include <functional>   // for std::swap
#include <algorithm>    // for std::min
#include <utility>      // for std::move

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// TExpandableBuffer
///////////////////////////////////////////////////////////////////////////////

/// A template class for an expandable buffer, that is a buffer that can be
/// expanded and which will, if expanded, maintain its contents.
/// \ingroup Templates
/// \ingroup CPlusPlusTools

template <class T> class TExpandableBuffer
{
   public :

      /// Create a buffer of the specified initialSize. If the size is 0 then
      /// the buffer must be resized or expanded before use.
      explicit TExpandableBuffer(
         size_t initialLogicalSize = 0);

      TExpandableBuffer(
         size_t initialPhysicalSize,
         size_t initialLogicalSize);

      // Copy initial data into buffer

      TExpandableBuffer(
         const T *pInitialData,
         size_t initialLogicalSize);

      // Move initial data into buffer

      TExpandableBuffer(
         T *pInitialData,
         size_t initialLogicalSize);

      // Copy initial data into buffer

      TExpandableBuffer(
         const T *pInitialData,
         size_t initialPhysicalSize,
         size_t initialLogicalSize);

      // Move initial data into buffer

      TExpandableBuffer(
         T *pInitialData,
         size_t initialPhysicalSize,
         size_t initialLogicalSize);

      TExpandableBuffer(
         const TExpandableBuffer &rhs);

      ~TExpandableBuffer();

      TExpandableBuffer &operator=(
         const TExpandableBuffer &rhs);

      /// Access the buffer.

      T *GetBuffer() const;

      /// Access the buffer.

      operator T *() const;

      /// Obtain the size of the buffer

      size_t GetSize() const;

      enum class SizeChangeDataRetentionPolicy : BYTE
      {
         RetainData,
         DoNotRetainData
      };

      /// If the buffer is currently smaller than newSize then make the buffer
      /// bigger, but do not copy the previous contents to the new buffer. If
      /// the current buffer is larger than, or equal to newSize then set the
      /// logical size to be newSize and leave the contents alone.
      /// Return the new size.

      size_t Resize(
         size_t newSize,
         SizeChangeDataRetentionPolicy dataRetentionPolicy);

      /// Remove the actual memory used to store the data from the buffer. The
      /// buffer is left with a buffer of size 0. The caller is responsible for
      /// destroying the memory returned, using delete [], once they've finished
      /// with it

      T *ReleaseBuffer();

      /// Destroy the memory used to store the data from the buffer. The buffer
      // is left with a buffer of size 0.

      void DestroyBuffer();

      /// Swap the internal storage and state from this buffer with the supplied
      /// buffer

      void Swap(
         TExpandableBuffer &rhs);

      /// Moves 'numberOfElements' from 'sourceElement' to 'destinationElement' with
      /// bounds checking. Source and destination can overlap. Source is zeroed.

      void MoveElements(
         size_t sourceElement,
         size_t numberOfElements,
         size_t destinationElement);

      /// Copies 'numberOfElements' from 'sourceElement' to 'destinationElement' with
      /// bounds checking. Source and destination can overlap. Source is left unchanged.

      void CopyElements(
         size_t sourceElement,
         size_t numberOfElements,
         size_t destinationElement);

      void SetElements(
         size_t sourceElement,
         size_t numberOfElements,
         T value);

      void ZeroElements(
         size_t sourceElement,
         size_t numberOfElements);

      void AddDataAtOffset(
         size_t offset,
         const T *pData,
         size_t dataSize);

   protected :

      template <typename T2>
      static void ZeroFillBuffer(
         T2 *pT,
         size_t size);

   private :

      T *m_pBuffer;

      size_t m_logicalSize;

      size_t m_physicalSize;
};

///////////////////////////////////////////////////////////////////////////////
// Construction and destruction
///////////////////////////////////////////////////////////////////////////////

static size_t ValidatePhysicalSize(
   const size_t initialPhysicalSize,
   const size_t initialLogicalSize)
{
   if (initialLogicalSize > initialPhysicalSize)
   {
      throw CException(
         _T("TExpandableBuffer<T>::TExpandableBuffer()"),
         _T("Logical size must be <= physical size"));
   }

   return initialPhysicalSize;
}

template <class T>
TExpandableBuffer<T>::TExpandableBuffer(
   const size_t initialLogicalSize)
   :  TExpandableBuffer(initialLogicalSize, initialLogicalSize)
{
}

template <class T>
TExpandableBuffer<T>::TExpandableBuffer(
   const size_t initialPhysicalSize,
   const size_t initialLogicalSize)
      :  m_pBuffer(nullptr),
         m_logicalSize(0),
         m_physicalSize(0)
{
   Resize(ValidatePhysicalSize(initialPhysicalSize, initialLogicalSize), SizeChangeDataRetentionPolicy::DoNotRetainData);

   m_logicalSize = initialLogicalSize;
}

template <class T>
TExpandableBuffer<T>::TExpandableBuffer(
   const T *pInitialData,
   const size_t initialLogicalSize)
   :  TExpandableBuffer(pInitialData, initialLogicalSize, initialLogicalSize)
{
}

template <class T>
TExpandableBuffer<T>::TExpandableBuffer(
   T *pInitialData,
   const size_t initialLogicalSize)
   :  TExpandableBuffer(pInitialData, initialLogicalSize, initialLogicalSize)
{
}

template <class T>
TExpandableBuffer<T>::TExpandableBuffer(
   const T *pInitialData,
   const size_t initialPhysicalSize,
   const size_t initialLogicalSize)
      :  TExpandableBuffer(initialPhysicalSize, initialLogicalSize)
{
   if (pInitialData && initialLogicalSize)
   {
      for (size_t i = 0; i < initialLogicalSize; i++)
      {
         m_pBuffer[i] = pInitialData[i];
      }
   }
}

template <class T>
TExpandableBuffer<T>::TExpandableBuffer(
   T *pInitialData,
   const size_t initialPhysicalSize,
   const size_t initialLogicalSize)
   :  TExpandableBuffer(initialPhysicalSize, initialLogicalSize)
{
   if (pInitialData && initialLogicalSize)
   {
      for (size_t i = 0; i < initialLogicalSize; i++)
      {
         m_pBuffer[i] = std::move(pInitialData[i]);
      }
   }
}

template <class T>
TExpandableBuffer<T>::TExpandableBuffer(
   const TExpandableBuffer<T> &rhs)
   :  m_pBuffer(nullptr),
      m_logicalSize(0),
      m_physicalSize(0)
{
   Resize(rhs.m_logicalSize, SizeChangeDataRetentionPolicy::DoNotRetainData);

   for (size_t i = 0; i < m_logicalSize; i++)
   {
      m_pBuffer[i] = rhs.m_pBuffer[i];
   }
}

template <class T>
TExpandableBuffer<T>::~TExpandableBuffer()
{
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START

   delete[] m_pBuffer;

   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END
}

template <class T>
TExpandableBuffer<T> &TExpandableBuffer<T>::operator=(
   const TExpandableBuffer<T> &rhs)
{
   if (this != &rhs)
   {
      Resize(rhs.m_logicalSize, SizeChangeDataRetentionPolicy::DoNotRetainData);

      for (size_t i = 0; i < m_logicalSize; i++)
      {
         m_pBuffer[i] = rhs.m_pBuffer[i];
      }
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Access functions
///////////////////////////////////////////////////////////////////////////////

template <class T>
T *TExpandableBuffer<T>::GetBuffer() const
{
   return m_pBuffer;
}

template <class T>
TExpandableBuffer<T>::operator T *() const
{
   return GetBuffer();
}

template <class T>
size_t TExpandableBuffer<T>::GetSize() const
{
   return m_logicalSize;
}

template <class T>
T *TExpandableBuffer<T>::ReleaseBuffer()
{
   T *pBuffer = m_pBuffer;

   m_pBuffer = nullptr;
   m_logicalSize = 0;
   m_physicalSize = 0;

   return pBuffer;
}

template <class T>
void TExpandableBuffer<T>::DestroyBuffer()
{
   delete[] ReleaseBuffer();
}

template <class T>
void TExpandableBuffer<T>::Swap(
   TExpandableBuffer<T> &rhs)
{
   std::swap(m_pBuffer, rhs.m_pBuffer);
   std::swap(m_logicalSize, rhs.m_logicalSize);
   std::swap(m_physicalSize, rhs.m_physicalSize);
}

// By default, we default initialise types

template <class T>
template <class T2>
void TExpandableBuffer<T>::ZeroFillBuffer(
   T2 *pT,
   const size_t size)
{
   for (size_t i = 0; i < size; ++i)
   {
      pT[i] = T2();
   }
}

// It may be faster to simply zero the memory, if we're using POD types...
// profile and add additional specialisations?

template <>
template <>
inline void TExpandableBuffer<BYTE>::ZeroFillBuffer(
   BYTE *pT,
   const size_t size)
{
   CheckedZeroMemory(pT, size);
}

template <class T>
void TExpandableBuffer<T>::MoveElements(
   const size_t sourceElement,
   const size_t numberOfElements,
   const size_t destinationElement)
{
   if (sourceElement + numberOfElements > m_logicalSize)
   {
      #if JETBYTE_CORE_DUMP_ON_EXPANDABLE_BUFFER_BOUNDS_CHECK_FAILURE == 1
      CCrashDumpGenerator::GenerateDumpHere(_T("TExpandableBuffer-BoundsCheck"));
      #endif

      throw CException(
         _T("TExpandableBuffer<T>::MoveElements()"),
         _T("Bounds check failure: source element + number of elements > logical size"));
   }

   if (destinationElement + numberOfElements > m_logicalSize)
   {
      #if JETBYTE_CORE_DUMP_ON_EXPANDABLE_BUFFER_BOUNDS_CHECK_FAILURE == 1
      CCrashDumpGenerator::GenerateDumpHere(_T("TExpandableBuffer-BoundsCheck"));
      #endif

      throw CException(
         _T("TExpandableBuffer<T>::MoveElements()"),
         _T("Bounds check failure: destination element + number of elements > logical size"));
   }

   if (sourceElement == destinationElement)
   {
      return;
   }

   if (sourceElement < destinationElement)
   {
      // moving forward

      for (size_t i = numberOfElements; i != 0; --i)
      {
         m_pBuffer[destinationElement + i - 1] = m_pBuffer[sourceElement + i - 1];
      }

      const size_t emptySpaceStart = sourceElement;

      const size_t emptySpaceEnd = std::min(destinationElement, sourceElement + numberOfElements);

      const size_t emptySpaceLength = emptySpaceEnd - emptySpaceStart;

      ZeroFillBuffer<T>(m_pBuffer + emptySpaceStart, emptySpaceLength);
   }
   else
   {
      // moving backwards

      for (size_t i = 0; i < numberOfElements; ++i)
      {
         m_pBuffer[destinationElement + i] = m_pBuffer[sourceElement + i];
      }

      const size_t emptySpaceStart = std::max(destinationElement + numberOfElements, sourceElement);

      const size_t emptySpaceEnd = sourceElement + numberOfElements;

      const size_t emptySpaceLength = emptySpaceEnd - emptySpaceStart;

      ZeroFillBuffer<T>(m_pBuffer + emptySpaceStart, emptySpaceLength);
   }
}

template <class T>
void TExpandableBuffer<T>::CopyElements(
   const size_t sourceElement,
   const size_t numberOfElements,
   const size_t destinationElement)
{
   if (sourceElement + numberOfElements > m_logicalSize)
   {
      #if JETBYTE_CORE_DUMP_ON_EXPANDABLE_BUFFER_BOUNDS_CHECK_FAILURE == 1
      CCrashDumpGenerator::GenerateDumpHere(_T("TExpandableBuffer-BoundsCheck"));
      #endif

      throw CException(
         _T("TExpandableBuffer<T>::CopyElements()"),
         _T("Bounds check failure: source element + number of elements > size of buffer"));
   }

   if (destinationElement + numberOfElements > m_logicalSize)
   {
      #if JETBYTE_CORE_DUMP_ON_EXPANDABLE_BUFFER_BOUNDS_CHECK_FAILURE == 1
      CCrashDumpGenerator::GenerateDumpHere(_T("TExpandableBuffer-BoundsCheck"));
      #endif

      throw CException(
         _T("TExpandableBuffer<T>::CopyElements()"),
         _T("Bounds check failure: destination element + number of elements > size of buffer"));
   }

   if (sourceElement == destinationElement)
   {
      return;
   }

   if (sourceElement < destinationElement)
   {
      // copy forward

      for (size_t i = numberOfElements; i != 0; --i)
      {
         m_pBuffer[destinationElement + i - 1] = m_pBuffer[sourceElement + i - 1];
      }
   }
   else
   {
      // copy backwards

      for (size_t i = 0; i < numberOfElements; ++i)
      {
         m_pBuffer[destinationElement + i] = m_pBuffer[sourceElement + i];
      }
   }
}

template <class T>
void TExpandableBuffer<T>::SetElements(
   const size_t sourceElement,
   const size_t numberOfElements,
   const T value)
{
   if (sourceElement + numberOfElements > m_logicalSize)
   {
      #if JETBYTE_CORE_DUMP_ON_EXPANDABLE_BUFFER_BOUNDS_CHECK_FAILURE == 1
      CCrashDumpGenerator::GenerateDumpHere(_T("TExpandableBuffer-BoundsCheck"));
      #endif

      throw CException(
         _T("TExpandableBuffer<T>::SetElements()"),
         _T("Bounds check failure: sourceElement + numberOfElements > size of buffer"));
   }

   for (size_t i = 0; i < numberOfElements; ++i)
   {
      m_pBuffer[sourceElement + i] = value;
   }
}

template <class T>
void TExpandableBuffer<T>::ZeroElements(
   size_t sourceElement,
   size_t numberOfElements)
{
   if (sourceElement + numberOfElements > m_logicalSize)
   {
      #if JETBYTE_CORE_DUMP_ON_EXPANDABLE_BUFFER_BOUNDS_CHECK_FAILURE == 1
      CCrashDumpGenerator::GenerateDumpHere(_T("TExpandableBuffer-BoundsCheck"));
      #endif

      throw CException(
         _T("TExpandableBuffer<T>::ZeroElements()"),
         _T("Bounds check failure: sourceElement + numberOfElements > size of buffer"));
   }

   ZeroFillBuffer<T>(m_pBuffer + sourceElement, numberOfElements);
}

template <class T>
void TExpandableBuffer<T>::AddDataAtOffset(
   const size_t offset,
   const T *pData,
   const size_t dataSize)
{
   if (offset + dataSize > m_logicalSize)
   {
      #if JETBYTE_CORE_DUMP_ON_EXPANDABLE_BUFFER_BOUNDS_CHECK_FAILURE == 1
      CCrashDumpGenerator::GenerateDumpHere(_T("TExpandableBuffer-BoundsCheck"));
      #endif

      throw CException(
         _T("TExpandableBuffer<T>::AddDataAtOffset()"),
         _T("Bounds check failure: offset + data size > size of buffer"));
   }

   if (!pData && dataSize != 0)
   {
      throw CException(
         _T("TExpandableBuffer<T>::AddDataAtOffset()"),
         _T("pData is null and dataSize is not zero"));
   }

   for (size_t i = 0; i < dataSize; ++i)
   {
      m_pBuffer[offset + i] = pData[i];
   }
}

///////////////////////////////////////////////////////////////////////////////
// Change the size of the buffer
///////////////////////////////////////////////////////////////////////////////

template <class T>
size_t TExpandableBuffer<T>::Resize(
   const size_t newSize,
   const SizeChangeDataRetentionPolicy dataRetentionPolicy)
{
   if (m_physicalSize < newSize)
   {
      auto *pNewBuffer = new T[newSize];

      if (m_pBuffer &&
          dataRetentionPolicy == SizeChangeDataRetentionPolicy::RetainData)
      {
         for (size_t i = 0; i < m_logicalSize; i++)
         {
            JETBYTE_WARNING_SUPPRESS_STRINGOP_OVERFLOW
            pNewBuffer[i] = std::move(m_pBuffer[i]);
            JETBYTE_WARNING_SUPPRESS_POP
         }
      }

      delete[] m_pBuffer;

      m_pBuffer = pNewBuffer;

      m_logicalSize = newSize;
      m_physicalSize = newSize;
   }
   else
   {
      m_logicalSize = newSize;
   }

   return m_logicalSize;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ExpandableBuffer.h
///////////////////////////////////////////////////////////////////////////////
