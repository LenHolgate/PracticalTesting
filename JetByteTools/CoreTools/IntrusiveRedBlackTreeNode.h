#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveRedBlackTreeNode.h
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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

template <class T, class K, class TtoN, class Pr, class TtoK>
class TIntrusiveRedBlackTree;

///////////////////////////////////////////////////////////////////////////////
// CIntrusiveRedBlackTreeNode
///////////////////////////////////////////////////////////////////////////////


class CIntrusiveRedBlackTreeNode
{
   public :

      CIntrusiveRedBlackTreeNode(
         const CIntrusiveRedBlackTreeNode &rhs) = delete;

      CIntrusiveRedBlackTreeNode &operator=(
         const CIntrusiveRedBlackTreeNode &rhs) = delete;

   protected :

      CIntrusiveRedBlackTreeNode()
         :  m_red(true),
            m_pParent(nullptr),
            m_pLinks{}
      {
         // Note that by using an array of 2 links rather than explicit left and right
         // pointers we can collapse much of the duplicated code into code which operates
         // on the array and a direction indicator.
         // See http://www.eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx
         // for details.
      }

      ~CIntrusiveRedBlackTreeNode() = default;

   private :

      template <class T, class K, class TtoK, class Pr, class TtoN> friend class TIntrusiveRedBlackTree;

      // We could optimise away m_red and store it in the parent pointer but then we'd
      // need to manipulate the parent pointer before each use. Need to profile and see
      // if this is worth doing to save a little space per node.

      bool m_red;

      CIntrusiveRedBlackTreeNode *m_pParent;
      CIntrusiveRedBlackTreeNode *m_pLinks[2];
};


///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveRedBlackTreeNode.h
///////////////////////////////////////////////////////////////////////////////
