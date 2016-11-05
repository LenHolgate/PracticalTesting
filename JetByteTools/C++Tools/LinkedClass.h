#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_LINKED_CLASS_INCLUDED__
#define JETBYTE_TOOLS_LINKED_CLASS_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: LinkedClass.h 
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 1997 JetByte Limited.
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
// Templates defined in this file...
///////////////////////////////////////////////////////////////////////////////

template <class T> class TLinkedClassIterator;

///////////////////////////////////////////////////////////////////////////////
// TLinkedClass
///////////////////////////////////////////////////////////////////////////////

/// A template class for classes that are linked together via a singly linked
/// list that can be navigated either from an instance of the class or from
/// the template.
/// \ingroup Templates
/// \ingroup CPlusPlusTools

template <class T> class TLinkedClass
{
   public :

      typedef unsigned short Index;

      friend class TLinkedClassIterator<T>;     // So we can access GetNext()
  
      /// Exceptions we throw.
      /// \ingroup Exceptions
      
      class BadIndex {};
            
      // Construction and destruction

      TLinkedClass();

      virtual ~TLinkedClass();

   protected :

      typedef TLinkedClassIterator<T> Iterator;

      /// Return an iterator to the start of the list of instances of this class.
      static Iterator Begin();

      /// Return an iterator to the end of the list of instances of this class.
      static Iterator End();

      // Iterator interface

      /// Obtain a pointer to the next class in the list.
      T *GetNext() const;

      // Derived class interface

      const Index m_nObjectIndex;

      /// Return the number of objects in the list.
      static Index GetObjectCount();
      
      /// Return an object by index, throws a BadIndex exception if the index 
      /// is invalid.
      static T *GetObjectByIndex(
         const Index nIndex);

   private :

      TLinkedClass<T> *m_pNext;

      static TLinkedClass<T> *s_pListHead; // Head of the list of all objects

      static Index s_nObjectCount;   // How many objects in existance

      /// No copies do not implement
      TLinkedClass(const TLinkedClass<T> &rhs);
      /// No copies do not implement
      TLinkedClass<T> operator=(const TLinkedClass<T> &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Static variables
///////////////////////////////////////////////////////////////////////////////

template <class T> TLinkedClass<T> *TLinkedClass<T>::s_pListHead = 0;
template <class T> TLinkedClass<T>::Index TLinkedClass<T>::s_nObjectCount = 0;

///////////////////////////////////////////////////////////////////////////////
// Construction and destruction
///////////////////////////////////////////////////////////////////////////////

template <class T> TLinkedClass<T>::TLinkedClass()
   :  m_nObjectIndex(s_nObjectCount++)
{
   m_pNext = s_pListHead;    // Add ourselves to the static list of all 
   s_pListHead = this;       // CTaskBarApplet derived objects.
}

template <class T> TLinkedClass<T>::~TLinkedClass()
{
   TLinkedClass<T> *pObject = s_pListHead;
   TLinkedClass<T> **ppLastLink = &s_pListHead;

   while (pObject)
   {
      if (pObject == this)
      {
         *ppLastLink = m_pNext;
         pObject = 0;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// Iteration
///////////////////////////////////////////////////////////////////////////////

template <class T> TLinkedClass<T>::Iterator TLinkedClass<T>::Begin()
{
   return Iterator(s_pListHead);
}

template <class T> TLinkedClass<T>::Iterator TLinkedClass<T>::End()
{
   return Iterator(0);
}

///////////////////////////////////////////////////////////////////////////////
// Iterator interface
///////////////////////////////////////////////////////////////////////////////

template <class T> T *TLinkedClass<T>::GetNext() const
{
   return (T*)m_pNext;
}

///////////////////////////////////////////////////////////////////////////////
// Derived class interface
///////////////////////////////////////////////////////////////////////////////

template <class T> int TLinkedClass<T>::GetObjectCount()
{ 
   return s_nObjectCount;
}

template <class T> T *TLinkedClass<T>::GetObjectByIndex(
   const Index nIndex)
{
   bool found = false;

   Iterator it = Begin(); 

   while (!found && it != End())
   {
      if (it->m_nObjectIndex != nIndex)
      {
         it++;
      }
      else
      {
         found = true;
      }
   }
   
   if (!found)
   {
      throw BadIndex();
   }

   return &it;
}

///////////////////////////////////////////////////////////////////////////////
// TLinkedClassIterator
///////////////////////////////////////////////////////////////////////////////

/// An iterator over a list of linked classes.
/// \ingroup Templates
/// \ingroup Iteration

template <class T> class TLinkedClassIterator
{
   public :

      /// Exceptions we throw.
      /// \ingroup Exceptions

      class NullIterator {};

      // Construction and destruction

      explicit TLinkedClassIterator(
         TLinkedClass<T> *pLink);
      
      ~TLinkedClassIterator();

      // Operators

      TLinkedClassIterator &operator++();
      TLinkedClassIterator operator++(int);

      T* operator->();
      
      T* operator&();

      bool operator!=(
         const TLinkedClassIterator<T> &rhs);

      bool operator==(
         const TLinkedClassIterator<T> &rhs);

   private :

      // Helper function used by operator++

      void Advance();

      TLinkedClass<T> *m_pLink;
};

///////////////////////////////////////////////////////////////////////////////
// Construction and destruction
///////////////////////////////////////////////////////////////////////////////

template <class T> 
TLinkedClassIterator<T>::TLinkedClassIterator(
   TLinkedClass<T> *pLink)
   :  m_pLink(pLink)
{
}

template <class T> TLinkedClassIterator<T>::~TLinkedClassIterator()
{
   m_pLink = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////

// Prefix ++:  ++it

template <class T> 
TLinkedClassIterator<T> &TLinkedClassIterator<T>::operator++()
{
   Advance();

   return *this;
}

// Postfix ++:  it++

template <class T> 
TLinkedClassIterator<T> TLinkedClassIterator<T>::operator++(int)
{
   TLinkedClassIterator<T> oldThis = *this;

   Advance();

   return oldThis;
}

template <class T> 
T* TLinkedClassIterator<T>::operator->()
{
   // Defer the work to operator&

   return &(*this);
}

template <class T> 
T* TLinkedClassIterator<T>::operator&()
{
   if (!m_pLink)
   {
      throw NullIterator();
   }

   return (T*)m_pLink;
}

template <class T> 
bool TLinkedClassIterator<T>::operator!=(
   const TLinkedClassIterator<T> &rhs)
{
   return !(*this == rhs);
}

template <class T> 
bool TLinkedClassIterator<T>::operator==(
   const TLinkedClassIterator<T> &rhs)
{
   return (m_pLink == rhs.m_pLink);
}

///////////////////////////////////////////////////////////////////////////////
// Helper function for operator ++
///////////////////////////////////////////////////////////////////////////////

template <class T> 
void TLinkedClassIterator<T>::Advance()
{
   if (m_pLink)
   {
      m_pLink = m_pLink->GetNext();
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_LINKED_CLASS_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: LinkedClass.h 
///////////////////////////////////////////////////////////////////////////////
