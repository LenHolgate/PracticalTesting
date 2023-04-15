#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ConditionalSmartPointer.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2009 JetByte Limited.
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

#if (JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE == 1)
#include "Tchar.h"
#include "Exception.h"
#if (JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE == 1)
#include "CrashDumpGenerator.h"
#endif
#endif

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#include "DebugTrace.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// CConditionalSmartPointer
///////////////////////////////////////////////////////////////////////////////

/// A \ref SmartPointer "smart pointer" to memory using the C++ new API.
/// Calls delete on any memory that the TConditionalSmartPointer owns when it
/// goes out of scope to aid in \ref RAII "scope based" designs. Note that the
/// TConditionalSmartPointer can be told that it does NOT own the memory that
/// it is given; this is useful if sometimes it holds dynamically allocated
/// memory that it should own and delete and sometimes it holds a pointer to,
/// for example, a static null object implementation.
/// \ingroup SmartPointer

template <class T> class TConditionalSmartPointer
{
   public :

      /// Create a TConditionalSmartPointer that currently owns no memory.

      TConditionalSmartPointer();

      /// Create a CConditionalSmartPointer that owns the supplied memory which must
      /// have been allocated using new. Can optionally be told NOT to take ownership
      /// of the supplied memory.

      explicit TConditionalSmartPointer(
         T *pMemoryThatWasAllocatedWithNew,
         bool takeOwnershipOfMemory = true);

      TConditionalSmartPointer(
         const TConditionalSmartPointer &rhs) = delete;

      ~TConditionalSmartPointer();

      TConditionalSmartPointer &operator=(
         const TConditionalSmartPointer &rhs) = delete;

      TConditionalSmartPointer &operator=(T *pMemoryThatWasAllocatedWithNew);

      /// Assign the supplied pointer to the CConditionalSmartPointer with the
      /// indicated ownership.

      TConditionalSmartPointer &Assign(
         T *pMemoryThatWasAllocatedWithNew,
         bool takeOwnershipOfMemory);

      /// Obtain access to any memory that is owned by the pointer.

      T *Get() const;

      /// Give up ownership and return any memory that is owned.

      T *Detach();

      /// Provide access to any memory that is owned by the pointer.

      operator T *() const;

      T *operator->() const;

      T &operator*() const;

      /// Returns true if the TConditionalSmartPointer currently points to some memory.

      bool IsValid() const;

   private :

      T *m_pMemoryThatWasAllocatedWithNew;

      bool m_weOwnMemory;
};

template <class T>
TConditionalSmartPointer<T>::TConditionalSmartPointer()
   :  m_pMemoryThatWasAllocatedWithNew(nullptr),
      m_weOwnMemory(false)
{

}

template <class T>
TConditionalSmartPointer<T>::TConditionalSmartPointer(
   T *pMemoryThatWasAllocatedWithNew,
   const bool takeOwnershipOfMemory)
   :  m_pMemoryThatWasAllocatedWithNew(pMemoryThatWasAllocatedWithNew),
      m_weOwnMemory(takeOwnershipOfMemory)
{

}

template <class T>
TConditionalSmartPointer<T>::~TConditionalSmartPointer()
{
   if (m_weOwnMemory)
   {
      try
      {
         delete m_pMemoryThatWasAllocatedWithNew;
      }
      JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
   }
}

template <class T>
TConditionalSmartPointer<T> &TConditionalSmartPointer<T>::operator=(
   T *pMemoryThatWasAllocatedWithNew)
{
   if (m_weOwnMemory)
   {
      delete m_pMemoryThatWasAllocatedWithNew;
   }

   m_pMemoryThatWasAllocatedWithNew = pMemoryThatWasAllocatedWithNew;

   m_weOwnMemory = true;

   return *this;
}

template <class T>
TConditionalSmartPointer<T> &TConditionalSmartPointer<T>::Assign(
   T *pMemoryThatWasAllocatedWithNew,
   const bool takeOwnershipOfMemory)
{
   if (m_weOwnMemory)
   {
      delete m_pMemoryThatWasAllocatedWithNew;
   }

   m_pMemoryThatWasAllocatedWithNew = pMemoryThatWasAllocatedWithNew;

   m_weOwnMemory = takeOwnershipOfMemory;

   return *this;
}

template <class T>
T *TConditionalSmartPointer<T>::Get() const
{
   return m_pMemoryThatWasAllocatedWithNew;
}

template <class T>
T *TConditionalSmartPointer<T>::Detach()
{
   T *pMemory = m_pMemoryThatWasAllocatedWithNew;

   m_pMemoryThatWasAllocatedWithNew = 0;

   m_weOwnMemory = false;

   return pMemory;
}

template <class T>
TConditionalSmartPointer<T>::operator T *() const
{
   #if (JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE == 1)
   if (!m_pMemoryThatWasAllocatedWithNew)
   {
      #if (JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE == 1)
      CCrashDumpGenerator::GenerateDumpHere(_T("SmartPointerNullReference"), CCrashDumpGenerator::PerDumpTypeMaxDumpLimits);
      #endif

      throw CException(_T("TConditionalSmartPointer<T>::operator*()"), _T("operator*() called on null reference"));
   }
   #endif

   return m_pMemoryThatWasAllocatedWithNew;
}

template<class T>
T* TConditionalSmartPointer<T>::operator->() const
{
   #if (JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE == 1)
   if (!m_pMemoryThatWasAllocatedWithNew)
   {
      #if (JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE == 1)
      JetByteTools::Core::CCrashDumpGenerator::GenerateDumpHere(_T("SmartPointerNullReference"), JetByteTools::Core::CCrashDumpGenerator::PerDumpTypeMaxDumpLimits);
      #endif

      throw CException(_T("TConditionalSmartPointer<T>::operator->()"), _T("operator->() called on null reference"));
   }
   #endif

   return m_pMemoryThatWasAllocatedWithNew;
}

template<class T>
T& TConditionalSmartPointer<T>::operator*() const
{
   #if (JETBYTE_SMART_POINTER_THROW_ON_NULL_REFERENCE == 1)
   if (!m_pMemoryThatWasAllocatedWithNew)
   {
      #if (JETBYTE_SMART_POINTER_DUMP_ON_NULL_REFERENCE == 1)
      JetByteTools::Core::CCrashDumpGenerator::GenerateDumpHere(_T("SmartPointerNullReference"), JetByteTools::Core::CCrashDumpGenerator::PerDumpTypeMaxDumpLimits);
      #endif

      throw CException(_T("TConditionalSmartPointer<T>::operator*()"), _T("operator*() called on null reference"));
   }
   #endif

   return *m_pMemoryThatWasAllocatedWithNew;
}

template <class T>
bool TConditionalSmartPointer<T>::IsValid() const
{
   return m_pMemoryThatWasAllocatedWithNew != nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ConditionalSmartPointer.h
///////////////////////////////////////////////////////////////////////////////
