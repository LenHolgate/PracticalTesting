#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_REFERENCE_COUNTED_INCLUDED__
#define JETBYTE_TOOLS_REFERENCE_COUNTED_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ReferenceCounted.h 
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 1998 JetByte Limited.
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
// TReferenceCounted
///////////////////////////////////////////////////////////////////////////////

/// A template class that wraps a class to provide it with and manage an 
/// external reference count.
/// \ingroup Templates
/// \ingroup CPlusPlusTools

template <class T, class B> class TReferenceCounted
{
   public :

      // Construction and destruction

      TReferenceCounted(T counted);

      // Access functions 

      const T &GetCounted() const;

      T &GetCounted();

      operator T &() const;

      operator T *() const;

      // Manage the reference count

      B *AddRef();

      /// \ingroup NoThrowSpec
      B *Release() throw();

      // Copy a pointer to a TReferenceCounted...

      static B *Copy(
         B *pRefCounted);

   protected :

      virtual void PreRelease() = 0;

      T m_counted;
      
      // We can't be deleted except by ourself...

      virtual ~TReferenceCounted();

   private :

      long m_count;

      // Restrict copying, don't implement

      TReferenceCounted(const TReferenceCounted &rhs);
      TReferenceCounted &operator=(const TReferenceCounted &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Construction and destruction
///////////////////////////////////////////////////////////////////////////////

template <class T, class B>
TReferenceCounted<T,B>::TReferenceCounted(T counted) 
   : m_counted(counted), m_count(1) 
{ 

}

template <class T, class B>
TReferenceCounted<T,B>::~TReferenceCounted() 
{ 
   
}

///////////////////////////////////////////////////////////////////////////////
// Access the underlying object
///////////////////////////////////////////////////////////////////////////////

template <class T, class B>
const T &TReferenceCounted<T,B>::GetCounted() const
{
   return m_counted;
}

template <class T, class B>
T &TReferenceCounted<T,B>::GetCounted()
{
   return m_counted;
}

template <class T, class B>
TReferenceCounted<T,B>::operator T &() const
{
   return m_counted;
}

template <class T, class B>
TReferenceCounted<T,B>::operator T *() const
{
   return &m_counted;
}

///////////////////////////////////////////////////////////////////////////////
// Manage the reference count
///////////////////////////////////////////////////////////////////////////////

template <class T, class B>
B *TReferenceCounted<T,B>::AddRef()
{
   ::InterlockedIncrement(&m_count);

   return (B*)this;
}

template <class T, class B>
B *TReferenceCounted<T,B>::Release() throw()
{
   if (0 == ::InterlockedDecrement(&m_count))
   {
      try
      {
         PreRelease();
      }
      catch (...)
      {
         // Release() mustn't throw...
      }

      delete this;
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Copy a reference counted pointer
///////////////////////////////////////////////////////////////////////////////

template <class T, class B>
B *TReferenceCounted<T,B>::Copy(
   B *pRefCounted)
{
   return (pRefCounted ? pRefCounted->AddRef() : 0);
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_REFERENCE_COUNTED_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ReferenceCounted.h 
///////////////////////////////////////////////////////////////////////////////
