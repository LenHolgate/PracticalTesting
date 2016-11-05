#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_INTRUSIVE_MULTI_MAP_NODE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_INTRUSIVE_MULTI_MAP_NODE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveMultiMapNode.h 
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

#include "JetByteTools\Win32Tools\IntrusiveRedBlackTreeNode.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

template <class T, class K, class Pr, class TtoN, class KtoT>
class TIntrusiveMultiMap;

///////////////////////////////////////////////////////////////////////////////
// CIntrusiveMultiMapNode
///////////////////////////////////////////////////////////////////////////////

class CIntrusiveMultiMapNode : public CIntrusiveRedBlackTreeNode
{
   public :

      // In this multi-map multiple values are stored in the same tree node by
      // linking them together into a doubly linked list. If a multi-map insert
      // operation locates a node with the required key in the tree then the
      // new node is simply added to the front of the list of any nodes that
      // are already linked to the node.
      //
      // Conceptually you end up with something like this:
      //                3
      //               / \
      //              2   5-5-5-5-5
      //             /   / \
      //            1   4   6
      // Tree rebalancing can continue without needing to know that the node
      // is a multi-map node.
      // The list is doubly linked to enable O(1) removal of a multi-map node
      // given the node as a starying point. Tree rebalancing is only required
      // if the last node of a given value is removed. All nodes of a given value
      // can be treated equally (erase by key would erase the head of the linked
      // list first) and a specific node can be erased by passing either an iterator
      // or the node/data itself.
      // All nodes of a given value can be removed in a single operation.
      //
      // In practice only the first node in a multi-map needs an additional pointer
      // as we could reuse tree link pointers from the red black tree node that is
      // our base class to form the list. This optimisation may occur in a later
      // release.
      //
      // It was decided that this approach was simpler than adding a new type of
      // tree node to the red black tree.

      CIntrusiveMultiMapNode()
         :  m_pPrev(0),
            m_pNext(0)
      {

      }

   private :

      template <class T, class K, class Pr, class TtoN, class KtoT> friend class TIntrusiveMultiMap;

      CIntrusiveMultiMapNode *m_pPrev;
      CIntrusiveMultiMapNode *m_pNext;

      /// No copies do not implement
      CIntrusiveMultiMapNode(const CIntrusiveMultiMapNode &rhs);
      /// No copies do not implement
      CIntrusiveMultiMapNode &operator=(const CIntrusiveMultiMapNode &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_WIN32_INTRUSIVE_MULTI_MAP_NODE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveMultiMapNode.h
///////////////////////////////////////////////////////////////////////////////
