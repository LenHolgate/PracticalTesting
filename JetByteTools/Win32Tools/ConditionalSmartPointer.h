#pragma once
#ifndef JETBYTE_TOOLS_WIN32_CONDITIONAL_SMART_POINTER_INCLUDED__
#define JETBYTE_TOOLS_WIN32_CONDITIONAL_SMART_POINTER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ConditionalSmartPointer.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2009 JetByte Limited.
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

#include <wtypes.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

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
         const bool takeOwnershipOfMemory = true);

      ~TConditionalSmartPointer();

      TConditionalSmartPointer &operator=(T *pMemoryThatWasAllocatedWithNew);

      /// Assign the supplied pointer to the CConditionalSmartPointer with the
      /// indicated ownership.

      TConditionalSmartPointer &Assign(
         T *pMemoryThatWasAllocatedWithNew,
         const bool takeOwnershipOfMemory);

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

      // No copies do not implement
      TConditionalSmartPointer(const TConditionalSmartPointer &rhs);
      TConditionalSmartPointer &operator=(const TConditionalSmartPointer &rhs);
};

template <class T>
TConditionalSmartPointer<T>::TConditionalSmartPointer()
   :  m_pMemoryThatWasAllocatedWithNew(0),
      m_weOwnMemory(false)
{

}

template <class T>
TConditionalSmartPointer<T>::TConditionalSmartPointer(
   T *pMemoryThatWasAllocatedWithNew,
   const bool weOwnMemory)
   :  m_pMemoryThatWasAllocatedWithNew(pMemoryThatWasAllocatedWithNew),
      m_weOwnMemory(weOwnMemory)
{

}

template <class T>
TConditionalSmartPointer<T>::~TConditionalSmartPointer()
{
   if (m_weOwnMemory)
   {
      delete m_pMemoryThatWasAllocatedWithNew;
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
   const bool weOwnMemory)
{
   if (m_weOwnMemory)
   {
      delete m_pMemoryThatWasAllocatedWithNew;
   }

   m_pMemoryThatWasAllocatedWithNew = pMemoryThatWasAllocatedWithNew;

   m_weOwnMemory = weOwnMemory;

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
      throw CException(_T("TConditionalSmartPointer<T>::operator*()"), _T("operator*() called on null reference"));
   }
#endif

   return *m_pMemoryThatWasAllocatedWithNew;
}

template <class T>
bool TConditionalSmartPointer<T>::IsValid() const
{
   return m_pMemoryThatWasAllocatedWithNew != 0;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_CONDITIONAL_SMART_POINTER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ConditionalSmartPointer.h
///////////////////////////////////////////////////////////////////////////////
