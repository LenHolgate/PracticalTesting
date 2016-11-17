#pragma once
#ifndef JETBYTE_TOOLS_WIN32_INTRUSIVE_MAP_INCLUDED__
#define JETBYTE_TOOLS_WIN32_INTRUSIVE_MAP_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveMap.h
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
// TIntrusiveMapNodeKeyAccessorKeyIsAddress
///////////////////////////////////////////////////////////////////////////////

template <class T>
class TIntrusiveMapNodeKeyAccessorKeyIsAddress
{
   public :

      static ULONG_PTR GetKeyFromT(
         const T *pNode)
      {
         return reinterpret_cast<ULONG_PTR>(const_cast<T *>(pNode));
      }
};

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveMap
///////////////////////////////////////////////////////////////////////////////

template <
   class T,
   class K,
   class TtoK,
   class Pr = std::less<K>,
   class TtoN = TIntrusiveRedBlackTreeNodeIsBaseClass<T> >
class TIntrusiveMap : public TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN>
{
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_WIN32_INTRUSIVE_MAP_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveMap.h
///////////////////////////////////////////////////////////////////////////////
