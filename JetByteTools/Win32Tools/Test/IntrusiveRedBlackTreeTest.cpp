///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveRedBlackTreeTest.cpp
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

#include "JetByteTools/Admin/Admin.h"

#include "IntrusiveRedBlackTreeTest.h"

#include "JetByteTools/TestTools/TestException.h"
#include "JetByteTools/TestTools/RunTest.h"

#include "JetByteTools/Win32Tools/DebugTrace.h"

#pragma hdrstop

#include "JetByteTools/Win32Tools/IntrusiveRedBlackTree.h"
#include "JetByteTools/Win32Tools/Mock/TestIntrusiveRedBlackTreeNode.h"

#include <deque>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;
using JetByteTools::Test::CTestMonitor;

using JetByteTools::Win32::OutputEx;

using JetByteTools::Win32::Mock::CTestIntrusiveRedBlackTreeNode;
using JetByteTools::Win32::Mock::CTestIntrusiveRedBlackTreeNodeKeyAccessor;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CIntrusiveRedBlackTreeTest
///////////////////////////////////////////////////////////////////////////////

void CIntrusiveRedBlackTreeTest::TestAll(
   CTestMonitor &monitor)
{
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestConstruct);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestInsert);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestInsertWithExplicitKey);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestInsertWithIncorrectExplicitKey);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestInsertDuplicate);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestFind);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestBigInsertInOrder);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestBigInsertReverseOrder);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestDestructDoesNotHarmNodes);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestRemove);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestRemoveNodeNotPresent);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestForwardIterate);

   // test with large amount of random data with a known seed, inserts, finds and deletes
}

void CIntrusiveRedBlackTreeTest::TestConstruct()
{
   {
      TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> tree;

      tree.ValidateTree();
   }
}

void CIntrusiveRedBlackTreeTest::TestInsert()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);
   CTestIntrusiveRedBlackTreeNode node5(5);
   CTestIntrusiveRedBlackTreeNode node6(6);
   CTestIntrusiveRedBlackTreeNode node7(7);

   {
      typedef TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> Tree;

      Tree tree;

      tree.ValidateTree();

      THROW_ON_FAILURE_EX(0 == tree.Size());
      THROW_ON_FAILURE_EX(tree.Begin() == tree.End());

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node3).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(1 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node1).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(2 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node4).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(3 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node2).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(4 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node6).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(5 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());

         const CTestIntrusiveRedBlackTreeNode *pNode = *it;

         THROW_ON_FAILURE_EX(pNode == &node2);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node6);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node5).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(6 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node5);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node6);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node7).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(7 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node5);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node6);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node7);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }
   }
}

void CIntrusiveRedBlackTreeTest::TestInsertWithExplicitKey()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);
   CTestIntrusiveRedBlackTreeNode node5(5);
   CTestIntrusiveRedBlackTreeNode node6(6);
   CTestIntrusiveRedBlackTreeNode node7(7);

   {
      typedef TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> Tree;

      Tree tree;

      tree.ValidateTree();

      THROW_ON_FAILURE_EX(0 == tree.Size());
      THROW_ON_FAILURE_EX(tree.Begin() == tree.End());


      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node3, 3).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(1 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node1, 1).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(2 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node4, 4).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(3 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         THROW_IF_NOT_EQUAL_EX(4, it->Value());
         THROW_IF_NOT_EQUAL_EX(4, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node2, 2).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(4 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         THROW_IF_NOT_EQUAL_EX(2, it->Value());
         THROW_IF_NOT_EQUAL_EX(2, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         THROW_IF_NOT_EQUAL_EX(4, it->Value());
         THROW_IF_NOT_EQUAL_EX(4, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node6, 6).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(5 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         THROW_IF_NOT_EQUAL_EX(2, it->Value());
         THROW_IF_NOT_EQUAL_EX(2, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         THROW_IF_NOT_EQUAL_EX(4, it->Value());
         THROW_IF_NOT_EQUAL_EX(4, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node6);
         THROW_IF_NOT_EQUAL_EX(6, it->Value());
         THROW_IF_NOT_EQUAL_EX(6, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node5, 5).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(6 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         THROW_IF_NOT_EQUAL_EX(2, it->Value());
         THROW_IF_NOT_EQUAL_EX(2, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         THROW_IF_NOT_EQUAL_EX(4, it->Value());
         THROW_IF_NOT_EQUAL_EX(4, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node5);
         THROW_IF_NOT_EQUAL_EX(5, it->Value());
         THROW_IF_NOT_EQUAL_EX(5, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node6);
         THROW_IF_NOT_EQUAL_EX(6, it->Value());
         THROW_IF_NOT_EQUAL_EX(6, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node7, 7).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(7 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         THROW_IF_NOT_EQUAL_EX(2, it->Value());
         THROW_IF_NOT_EQUAL_EX(2, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         THROW_IF_NOT_EQUAL_EX(4, it->Value());
         THROW_IF_NOT_EQUAL_EX(4, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node5);
         THROW_IF_NOT_EQUAL_EX(5, it->Value());
         THROW_IF_NOT_EQUAL_EX(5, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node6);
         THROW_IF_NOT_EQUAL_EX(6, it->Value());
         THROW_IF_NOT_EQUAL_EX(6, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node7);
         THROW_IF_NOT_EQUAL_EX(7, it->Value());
         THROW_IF_NOT_EQUAL_EX(7, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }
   }
}

void CIntrusiveRedBlackTreeTest::TestInsertWithIncorrectExplicitKey()
{
   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
   SKIP_TEST_EX(_T("Test not supported when tree validation is enabled"));
   #endif

   CTestIntrusiveRedBlackTreeNode node1(1);
   //CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);

   {
      typedef TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> Tree;

      Tree tree;

      tree.ValidateTree();

      THROW_ON_FAILURE_EX(0 == tree.Size());
      THROW_ON_FAILURE_EX(tree.Begin() == tree.End());


      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node3, 3).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(1 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node1, 1).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(2 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      // This key is wrong, it's not the same value that 'extracting the key from the node'
      // will give...

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node4, 2).second);

         // Tree is no longer valid...

         THROW_ON_NO_EXCEPTION_EX(tree.ValidateTree);

         THROW_ON_FAILURE_EX(3 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         THROW_IF_NOT_EQUAL_EX(4, it->Value());
         THROW_IF_NOT_EQUAL_EX(4, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }
   }
}

void CIntrusiveRedBlackTreeTest::TestInsertDuplicate()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(3);     // duplicate key of node 3

   {
      typedef TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> Tree;

      Tree tree;

      tree.ValidateTree();

      THROW_ON_FAILURE_EX(0 == tree.Size());
      THROW_ON_FAILURE_EX(tree.Begin() == tree.End());


      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node3).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(1 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(false == tree.Insert(&node4).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(1 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }


      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node1).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(2 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(false == tree.Insert(&node4).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(2 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node2).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(3 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(false == tree.Insert(&node4).second);

         tree.ValidateTree();

         THROW_ON_FAILURE_EX(3 == tree.Size());

         Tree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         ++it;
         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

   }
}

void CIntrusiveRedBlackTreeTest::TestFind()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);
   CTestIntrusiveRedBlackTreeNode node5(5);
   CTestIntrusiveRedBlackTreeNode node6(6);
   CTestIntrusiveRedBlackTreeNode node7(7);

   {
      typedef TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> Tree;

      Tree tree;

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(1));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(2));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(3));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(4));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(5));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(6));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(7));

      THROW_ON_FAILURE_EX(true == tree.Insert(&node1).second);

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(2));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(3));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(4));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(5));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(6));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(7));

      THROW_ON_FAILURE_EX(true == tree.Insert(&node2).second);

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(3));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(4));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(5));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(6));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(7));

      THROW_ON_FAILURE_EX(true == tree.Insert(&node3).second);

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *tree.Find(3));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(4));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(5));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(6));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(7));

      THROW_ON_FAILURE_EX(true == tree.Insert(&node4).second);

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *tree.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *tree.Find(4));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(5));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(6));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(7));

      THROW_ON_FAILURE_EX(true == tree.Insert(&node5).second);

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *tree.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *tree.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *tree.Find(5));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(6));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(7));

      THROW_ON_FAILURE_EX(true == tree.Insert(&node6).second);

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *tree.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *tree.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *tree.Find(5));
      THROW_ON_FAILURE_EX(&node6 == *tree.Find(6));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(7));

      THROW_ON_FAILURE_EX(true == tree.Insert(&node7).second);

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *tree.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *tree.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *tree.Find(5));
      THROW_ON_FAILURE_EX(&node6 == *tree.Find(6));
      THROW_ON_FAILURE_EX(&node7 == *tree.Find(7));
   }
}

void CIntrusiveRedBlackTreeTest::TestBigInsertInOrder()
{
   typedef TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> Tree;

   typedef std::deque<CTestIntrusiveRedBlackTreeNode *> NodeList;

   NodeList nodeList;

   for (int i = 0; i < 100; ++i)
   {
      nodeList.push_back(new CTestIntrusiveRedBlackTreeNode(i));
   }

   {
      {
         Tree tree;
         size_t i = 0;

         NodeList::const_iterator it = nodeList.begin();

         const NodeList::const_iterator end = nodeList.end();

         for (;
            it != end && i < 7;
            ++it, ++i)
         {
            CTestIntrusiveRedBlackTreeNode *pNode = *it;
            THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

            tree.ValidateTree();
         }

         CTestIntrusiveRedBlackTreeNode *pNode = *it;
         THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

         tree.ValidateTree();
      }

      Tree tree;

      size_t size = 0;

      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
         it != end;
         ++it)
      {
         tree.ValidateTree();

         THROW_ON_FAILURE_EX(size == tree.Size());

         CTestIntrusiveRedBlackTreeNode *pNode = *it;

         THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

         size++;
      }

      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
         it != end;
         ++it)
      {
         CTestIntrusiveRedBlackTreeNode *pNode = *it;

         THROW_ON_FAILURE_EX(pNode == *tree.Find(pNode->Value()));
      }

      int i = 0;

      for (Tree::Iterator it = tree.Begin(), end = tree.End(); it != end; ++it, ++i)
      {
         THROW_IF_NOT_EQUAL_EX((*it)->Value(), i);
      }
   }

   for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
      it != end;
      ++it)
   {
      delete *it;
   }
}

void CIntrusiveRedBlackTreeTest::TestBigInsertReverseOrder()
{
   typedef TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> Tree;

   typedef std::deque<CTestIntrusiveRedBlackTreeNode *> NodeList;

   NodeList nodeList;

   for (int i = 0; i < 100; ++i)
   {
      nodeList.push_front(new CTestIntrusiveRedBlackTreeNode(i));
   }

   {
      {
         Tree tree;
         size_t i = 0;

         NodeList::const_iterator it = nodeList.begin();

         const NodeList::const_iterator end = nodeList.end();

         for (;
            it != end && i < 7;
            ++it, ++i)
         {
            CTestIntrusiveRedBlackTreeNode *pNode = *it;
            THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

            tree.ValidateTree();
         }

         CTestIntrusiveRedBlackTreeNode *pNode = *it;
         THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

         tree.ValidateTree();
      }

      Tree tree;

      size_t size = 0;

      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
         it != end;
         ++it)
      {
         tree.ValidateTree();

         THROW_ON_FAILURE_EX(size == tree.Size());

         CTestIntrusiveRedBlackTreeNode *pNode = *it;

         THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

         size++;
      }

      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
         it != end;
         ++it)
      {
         CTestIntrusiveRedBlackTreeNode *pNode = *it;

         THROW_ON_FAILURE_EX(pNode == *tree.Find(pNode->Value()));
      }

      int i = 0;

      for (Tree::Iterator it = tree.Begin(), end = tree.End(); it != end; ++it, ++i)
      {
         THROW_IF_NOT_EQUAL_EX((*it)->Value(), i);
      }
   }

   for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
      it != end;
      ++it)
   {
      delete *it;
   }
}

void CIntrusiveRedBlackTreeTest::TestDestructDoesNotHarmNodes()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);

   {
      typedef TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> Tree;

      Tree tree;

      THROW_ON_FAILURE_EX(true == tree.Insert(&node1).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node2).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node3).second);
   }

   THROW_ON_FAILURE_EX(node1.Value() == 1);
   THROW_ON_FAILURE_EX(node2.Value() == 2);
   THROW_ON_FAILURE_EX(node3.Value() == 3);
}

void CIntrusiveRedBlackTreeTest::TestRemove()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);
   CTestIntrusiveRedBlackTreeNode node5(5);
   CTestIntrusiveRedBlackTreeNode node6(6);
   CTestIntrusiveRedBlackTreeNode node7(7);
   CTestIntrusiveRedBlackTreeNode node8(8);

   {
      typedef TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> Tree;

      Tree tree;

      THROW_ON_FAILURE_EX(true == tree.Insert(&node1).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node2).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node3).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node4).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node5).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node6).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node7).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node8).second);

      tree.ValidateTree();

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *tree.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *tree.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *tree.Find(5));
      THROW_ON_FAILURE_EX(&node6 == *tree.Find(6));
      THROW_ON_FAILURE_EX(&node7 == *tree.Find(7));
      THROW_ON_FAILURE_EX(&node8 == *tree.Find(8));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(9));

      THROW_ON_FAILURE_EX(&node3 == tree.Remove(3));

      tree.ValidateTree();

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *tree.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *tree.Find(5));
      THROW_ON_FAILURE_EX(&node6 == *tree.Find(6));
      THROW_ON_FAILURE_EX(&node7 == *tree.Find(7));
      THROW_ON_FAILURE_EX(&node8 == *tree.Find(8));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(9));

      THROW_ON_FAILURE_EX(&node4 == tree.Remove(4));

      tree.ValidateTree();

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(3));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *tree.Find(5));
      THROW_ON_FAILURE_EX(&node6 == *tree.Find(6));
      THROW_ON_FAILURE_EX(&node7 == *tree.Find(7));
      THROW_ON_FAILURE_EX(&node8 == *tree.Find(8));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(9));

   }
}

void CIntrusiveRedBlackTreeTest::TestRemoveNodeNotPresent()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);

   {
      typedef TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> Tree;

      Tree tree;

      THROW_ON_FAILURE_EX(true == tree.Insert(&node1).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node2).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node3).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node4).second);

      tree.ValidateTree();

      THROW_ON_FAILURE_EX(nullptr == tree.Remove(0));
      THROW_ON_FAILURE_EX(nullptr == tree.Remove(5));
      THROW_ON_FAILURE_EX(nullptr == tree.Remove(100));

   }
}

void CIntrusiveRedBlackTreeTest::TestForwardIterate()
{
   typedef TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> Tree;

   typedef std::deque<CTestIntrusiveRedBlackTreeNode *> NodeList;

   NodeList nodeList;

   const int numNodes = 10;

   for (int i = 0; i < numNodes; ++i)
   {
      nodeList.push_back(new CTestIntrusiveRedBlackTreeNode(i));
   }

   {
      Tree tree;

      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
         it != end;
         ++it)
      {
         CTestIntrusiveRedBlackTreeNode *pNode = *it;

         THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);
      }

      tree.ValidateTree();
      THROW_ON_FAILURE_EX(numNodes == tree.Size());

      // now run the test...

      Tree::Iterator it = tree.Begin();

      const Tree::Iterator end = tree.End();

      THROW_ON_FAILURE_EX(it != end);

      THROW_IF_NOT_EQUAL_EX(0, it->Value());
      THROW_IF_NOT_EQUAL_EX(0, it.Key());

      Tree::Iterator it2 = it;

      THROW_IF_NOT_EQUAL_EX(0, it2->Value());
      THROW_IF_NOT_EQUAL_EX(0, it2.Key());

      THROW_IF_NOT_EQUAL_EX(1, (++it)->Value());
      THROW_IF_NOT_EQUAL_EX(1, it->Value());
      THROW_IF_NOT_EQUAL_EX(1, it.Key());
      THROW_IF_NOT_EQUAL_EX(0, it2->Value());
      THROW_IF_NOT_EQUAL_EX(0, it2.Key());

      THROW_IF_NOT_EQUAL_EX(1, (it++)->Value());
      THROW_IF_NOT_EQUAL_EX(2, it->Value());
      THROW_IF_NOT_EQUAL_EX(2, it.Key());
      THROW_IF_NOT_EQUAL_EX(0, it2->Value());
      THROW_IF_NOT_EQUAL_EX(0, it2.Key());

      it += 1;

      THROW_IF_NOT_EQUAL_EX(3, it->Value());
      THROW_IF_NOT_EQUAL_EX(3, it.Key());
      THROW_IF_NOT_EQUAL_EX(0, it2->Value());
      THROW_IF_NOT_EQUAL_EX(0, it2.Key());

      it2 += 4;

      THROW_IF_NOT_EQUAL_EX(3, it->Value());
      THROW_IF_NOT_EQUAL_EX(3, it.Key());
      THROW_IF_NOT_EQUAL_EX(4, it2->Value());
      THROW_IF_NOT_EQUAL_EX(4, it2.Key());
   }

   for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
      it != end;
      ++it)
   {
      delete *it;
   }
}

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveRedBlackTreeTest.cpp
///////////////////////////////////////////////////////////////////////////////

