#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveMultiMapNode.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2014 JetByte Limited.
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

#include "IntrusiveRedBlackTreeNode.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

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
         :  m_pPrev(nullptr),
            m_pNext(nullptr)
      {

      }

      CIntrusiveMultiMapNode(
         const CIntrusiveMultiMapNode &rhs) = delete;

      CIntrusiveMultiMapNode &operator=(
         const CIntrusiveMultiMapNode &rhs) = delete;

   private :

      template <class T, class K, class Pr, class TtoN, class KtoT> friend class TIntrusiveMultiMap;

      CIntrusiveMultiMapNode *m_pPrev;
      CIntrusiveMultiMapNode *m_pNext;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveMultiMapNode.h
///////////////////////////////////////////////////////////////////////////////
