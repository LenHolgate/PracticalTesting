#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_REFERENCE_COUNTED_SMART_POINTER_INCLUDED__
#define JETBYTE_TOOLS_REFERENCE_COUNTED_SMART_POINTER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ReferenceCountedSmartPointer.h
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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

///////////////////////////////////////////////////////////////////////////////
// TReferenceCountedSmartPointer
///////////////////////////////////////////////////////////////////////////////

/// A template \ref SmartPointer "smart pointer" class that manages objects
/// that support AddRef() and Release() style reference counting.
/// \ingroup Templates
/// \ingroup CPlusPlusTools
/// \ingroup SmartPointer

template <class T>
class TReferenceCountedSmartPointer
{
   public :
   
      /// Construct an empty smart pointer.

      TReferenceCountedSmartPointer();

      /// Construct a smart pointer that takes ownership of the supplied object.
      /// The pointer does NOT call AddRef() in the constructor but DOES call
      /// Release() in the destructor.

      explicit TReferenceCountedSmartPointer(
         T *ref);

      /// Construct a smart pointer that takes ownership of the supplied object.
      /// The pointer does NOT call AddRef() in the constructor but DOES call
      /// Release() in the destructor.

      explicit TReferenceCountedSmartPointer(
         T &ref);

      TReferenceCountedSmartPointer(
         const TReferenceCountedSmartPointer<T> &rhs);

      TReferenceCountedSmartPointer & operator=(
         const TReferenceCountedSmartPointer<T> &rhs);

      TReferenceCountedSmartPointer & operator=(
         T *ref);

      ~TReferenceCountedSmartPointer();

      T* operator->();

      T *Get();

      /// Obtain a reference to the managed object. For some reason this DOES NOT
      /// throw an exception if the smart pointer is not currently managing an object!!

      T &GetRef();

      /// Detatch the managed object from the smart pointer and return it to the
      /// caller. The caller now owns the object and is responsible for calling Release()
      /// on it when it is finished with it.

      T *Detach();
      
   private :

      static void SafeAddRef(T *ref);
      
      static void SafeRelease(T *ref);

      T *m_ref;
};

template <class T>
TReferenceCountedSmartPointer<T>::TReferenceCountedSmartPointer()
   : m_ref(0)
{
}

template <class T>
TReferenceCountedSmartPointer<T>::TReferenceCountedSmartPointer(
   T *ref)
   : m_ref(ref)
{
}

template <class T>
TReferenceCountedSmartPointer<T>::TReferenceCountedSmartPointer(
   T &ref)
   : m_ref(&ref)
{
}

template <class T>
TReferenceCountedSmartPointer<T>::TReferenceCountedSmartPointer(
   const TReferenceCountedSmartPointer<T> &rhs)
   :  m_ref(rhs.m_ref)
{
   SafeAddRef(m_ref);
}

template <class T>
TReferenceCountedSmartPointer<T>::~TReferenceCountedSmartPointer()
{
   SafeRelease(m_ref);
}

template <class T>
TReferenceCountedSmartPointer<T> &TReferenceCountedSmartPointer<T>::operator=(
   const TReferenceCountedSmartPointer<T> &rhs)
{
   TReferenceCountedSmartPointer<T> temp(rhs);

   SafeRelease(m_ref);

   m_ref = rhs.m_ref;

   SafeAddRef(m_ref);

   return *this;
}

template <class T>
TReferenceCountedSmartPointer<T> &TReferenceCountedSmartPointer<T>::operator=(
   T *ref)
{
   TReferenceCountedSmartPointer<T> temp(ref);

   SafeRelease(m_ref);

   m_ref = ref;

   SafeAddRef(m_ref);

   return *this;
}

template <class T> 
T* TReferenceCountedSmartPointer<T>::operator->()
{
   return m_ref;
}

template <class T> 
T *TReferenceCountedSmartPointer<T>::Get()
{
   return m_ref;
}

template <class T> 
T &TReferenceCountedSmartPointer<T>::GetRef()
{
//   if (!m_ref)
//   {
//      throw CException(_T("TReferenceCountedSmartPointer<T>::GetRef()"), _T("GetRef() called on null reference"));
//   }

   return *m_ref;
}

template <class T> 
T *TReferenceCountedSmartPointer<T>::Detach()
{
   T *ref = m_ref;

   m_ref = 0;

   return ref;
}

template <class T> 
void TReferenceCountedSmartPointer<T>::SafeAddRef(
   T *ref)
{
   if (ref)
   {
      ref->AddRef();
   }
}

template <class T> 
void TReferenceCountedSmartPointer<T>::SafeRelease(
   T *ref)
{
   if (ref)
   {
      ref->Release();
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_REFERENCE_COUNTED_SMART_POINTER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ReferenceCountedSmartPointer.h
///////////////////////////////////////////////////////////////////////////////

