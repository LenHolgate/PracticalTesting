#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_INTRUSIVE_SET_INCLUDED__
#define JETBYTE_TOOLS_WIN32_INTRUSIVE_SET_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveSet.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2014 JetByte Limited.
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

#include "JetByteTools\Win32Tools\IntrusiveRedBlackTree.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveSetNodeKeyAccessorKeyIsAddress
///////////////////////////////////////////////////////////////////////////////

template <class T>
class TIntrusiveSetNodeKeyAccessorKeyIsAddress
{
   public :

      static ULONG_PTR GetKeyFromT(
         const T *pNode)
      {
         return reinterpret_cast<ULONG_PTR>(const_cast<T *>(pNode));
      }
};

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveSet
///////////////////////////////////////////////////////////////////////////////

template <
   class T,
   class K = ULONG_PTR,
   class TtoK = TIntrusiveSetNodeKeyAccessorKeyIsAddress<T>,
   class Pr = std::less<K>,
   class TtoN = TIntrusiveRedBlackTreeNodeIsBaseClass<T> >
class TIntrusiveSet : public TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN>
{
   public :

      typedef TtoK key_accessor;

      typedef TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN> Base;

      Iterator Find(
         const T *pData) const;
}; //lint !e1509 (base class destructor for class 'TIntrusiveRedBlackTree' is not virtual -- Effective C++ #14 & Eff. C++ 3rd Ed. item 7)

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveSet<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveSet<T,K,TtoK,Pr,TtoN>::Find(
   const T *pData) const
{
   return Base::Find(key_accessor::GetKeyFromT(pData));
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_WIN32_INTRUSIVE_SET_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveSet.h
///////////////////////////////////////////////////////////////////////////////
