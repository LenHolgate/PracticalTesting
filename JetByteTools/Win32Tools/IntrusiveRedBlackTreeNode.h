#pragma once
#ifndef JETBYTE_TOOLS_WIN32_INTRUSIVE_RED_BLACK_TREE_NODE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_INTRUSIVE_RED_BLACK_TREE_NODE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveRedBlackTreeNode.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2014 JetByte Limited.
//
// Based on original public domain source code from:
// http://en.literateprograms.org/Red-black_tree_(C)?oldid=19567
// and also with ideas from original public domain source code from:
// http://www.eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx
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
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

template <class T, class K, class TtoN, class Pr, class TtoK>
class TIntrusiveRedBlackTree;

///////////////////////////////////////////////////////////////////////////////
// CIntrusiveRedBlackTreeNode
///////////////////////////////////////////////////////////////////////////////

//lint -save
//lint -e1060 (private member is not accessible to non-member non-friend functions)

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
            m_pParent(nullptr)
      {
         // Note that by using an array of 2 links rather than explicit left and right
         // pointers we can collapse much of the duplicated code into code which operates
         // on the array and a direction indicator.
         // See http://www.eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx
         // for details.

         m_pLinks[0] = nullptr;
         m_pLinks[1] = nullptr;
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

//lint -restore

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_INTRUSIVE_RED_BLACK_TREE_NODE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveRedBlackTreeNode.h
///////////////////////////////////////////////////////////////////////////////
