///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveRedBlackTreeTest.cpp
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

#include "JetByteTools/Admin/Admin.h"

#include "IntrusiveRedBlackTreeTest.h"

#include "JetByteTools/TestTools/TestException.h"
#include "JetByteTools/TestTools/RunTest.h"

#pragma hdrstop

#include "JetByteTools/CoreTools/IntrusiveRedBlackTree.h"
#include "JetByteTools/CoreTools/Mock/TestIntrusiveRedBlackTreeNode.h"

#include <deque>
#include <map>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;
using JetByteTools::Test::CTestMonitor;

using JetByteTools::Core::Mock::CTestIntrusiveRedBlackTreeNode;
using JetByteTools::Core::Mock::CTestIntrusiveRedBlackTreeNodeKeyAccessor;

typedef JetByteTools::Core::TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> TestTree;

typedef std::deque<CTestIntrusiveRedBlackTreeNode *> NodeList;

typedef std::map<int, CTestIntrusiveRedBlackTreeNode *> NodeMap;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

template <typename T>
static void ValidateTree(const T &tree)
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_VALIDATION == 1
   tree.ValidateTree();
   #else
   (void)tree;
   #endif
}

static void ValidateNodeInsert(
   CTestIntrusiveRedBlackTreeNode &node,
   TestTree &tree);

static void ValidateNodeInsert(
   CTestIntrusiveRedBlackTreeNode &node,
   TestTree &tree,
   NodeList &nodes);

static void ValidateNodeInsert(
   CTestIntrusiveRedBlackTreeNode &node,
   TestTree &tree,
   NodeMap &nodes);

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
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestLowerBound);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestLowerBoundAgain);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestUpperBound);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestBigInsertInOrder);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestBigInsertReverseOrder);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestDestructDoesNotHarmNodes);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestRemove);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestRemoveNodeNotPresent);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestForwardIterate);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestRBegin);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestReverseIterate);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestClear);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestClearWithCallback);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestFastClear);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestFastClearWithCallback);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestFastAndDirtyClear);
   RUN_TEST_EX(monitor, CIntrusiveRedBlackTreeTest, TestFastAndDirtyClearWithCallback);

   // test with large amount of random data with a known seed, inserts, finds and deletes
}

void CIntrusiveRedBlackTreeTest::TestConstruct()
{
   {
      const TIntrusiveRedBlackTree<CTestIntrusiveRedBlackTreeNode, int, CTestIntrusiveRedBlackTreeNodeKeyAccessor> tree;

      ValidateTree(tree);
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
      TestTree tree;

      ValidateTree(tree);

      THROW_ON_FAILURE_EX(0 == tree.Size());
      THROW_ON_FAILURE_EX(tree.Begin() == tree.End());

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node3).second);

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(1 == tree.Size());

         TestTree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node1).second);

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(2 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(3 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(4 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(5 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(6 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(7 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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
      TestTree tree;

      ValidateTree(tree);

      THROW_ON_FAILURE_EX(0 == tree.Size());
      THROW_ON_FAILURE_EX(tree.Begin() == tree.End());


      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node3, 3).second);

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(1 == tree.Size());

         TestTree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node1, 1).second);

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(2 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(3 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(4 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(5 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(6 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(7 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
   SKIP_TEST_EX(_T("Test not supported when tree validation is enabled"));
   #endif

   CTestIntrusiveRedBlackTreeNode node1(1);
   //CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);

   {
      TestTree tree;

      ValidateTree(tree);

      THROW_ON_FAILURE_EX(0 == tree.Size());
      THROW_ON_FAILURE_EX(tree.Begin() == tree.End());


      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node3, 3).second);

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(1 == tree.Size());

         TestTree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it.Key());
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node1, 1).second);

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(2 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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
         THROW_ON_NO_EXCEPTION_EX_2(tree.Insert, &node4, 2);

         // Tree is still valid...

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(2 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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
   }
}

void CIntrusiveRedBlackTreeTest::TestInsertDuplicate()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(3);     // duplicate key of node 3

   {
      TestTree tree;

      ValidateTree(tree);

      THROW_ON_FAILURE_EX(0 == tree.Size());
      THROW_ON_FAILURE_EX(tree.Begin() == tree.End());


      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node3).second);

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(1 == tree.Size());

         TestTree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }

      {
         THROW_ON_FAILURE_EX(false == tree.Insert(&node4).second);

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(1 == tree.Size());

         TestTree::Iterator it = tree.Begin();

         THROW_ON_FAILURE_EX(it != tree.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == tree.End());
      }


      {
         THROW_ON_FAILURE_EX(true == tree.Insert(&node1).second);

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(2 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(2 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(3 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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

         ValidateTree(tree);

         THROW_ON_FAILURE_EX(3 == tree.Size());

         TestTree::Iterator it = tree.Begin();

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
      TestTree tree;

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

void CIntrusiveRedBlackTreeTest::TestLowerBound()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node5(5);
   CTestIntrusiveRedBlackTreeNode node7(7);

   {

      TestTree tree;

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


      THROW_ON_FAILURE_EX(true == tree.Insert(&node5).second);

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(3));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *tree.Find(5));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(6));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(7));

      THROW_ON_FAILURE_EX(true == tree.Insert(&node7).second);

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(3));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *tree.Find(5));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(6));
      THROW_ON_FAILURE_EX(&node7 == *tree.Find(7));

      THROW_IF_STRINGS_DONT_MATCH_EX(tree.DumpTree(false), _T("K[1](1, b), K[2](0, b), K[5](1, b), K[7](2, r), "));


      // now the test...


      ValidateTree(tree);
      THROW_ON_FAILURE_EX(&node1 == *tree.LowerBound(0));
      ValidateTree(tree);
      THROW_ON_FAILURE_EX(&node1 == *tree.LowerBound(1));
      ValidateTree(tree);
      THROW_ON_FAILURE_EX(&node2 == *tree.LowerBound(2));
      ValidateTree(tree);
      THROW_ON_FAILURE_EX(&node5 == *tree.LowerBound(3));
      ValidateTree(tree);
      THROW_ON_FAILURE_EX(&node5 == *tree.LowerBound(4));
      ValidateTree(tree);
      THROW_ON_FAILURE_EX(&node5 == *tree.LowerBound(5));
      ValidateTree(tree);
      THROW_ON_FAILURE_EX(&node7 == *tree.LowerBound(6));
      ValidateTree(tree);
      THROW_ON_FAILURE_EX(&node7 == *tree.LowerBound(7));
      ValidateTree(tree);
      THROW_ON_FAILURE_EX(tree.End() == tree.LowerBound(8));
      ValidateTree(tree);
   }
}

void CIntrusiveRedBlackTreeTest::TestLowerBoundAgain()
{
   CTestIntrusiveRedBlackTreeNode node37(37);
   CTestIntrusiveRedBlackTreeNode node45(45);
   CTestIntrusiveRedBlackTreeNode node53(53);
   CTestIntrusiveRedBlackTreeNode node57(57);
   CTestIntrusiveRedBlackTreeNode node58(58);
   CTestIntrusiveRedBlackTreeNode node59(59);
   CTestIntrusiveRedBlackTreeNode node60(60);
   CTestIntrusiveRedBlackTreeNode node61(61);

   {

      TestTree tree;

      THROW_ON_FAILURE_EX(true == tree.Insert(&node37).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node45).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node53).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node57).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node58).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node59).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node60).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node61).second);


      THROW_IF_STRINGS_DONT_MATCH_EX(tree.DumpTree(false), _T("K[37](2, b), K[45](1, r), K[53](2, b), K[57](0, b), K[58](2, b), K[59](1, r), K[60](2, b), K[61](3, r), "));

      // now the test...

      THROW_ON_FAILURE_EX(&node37 == *tree.LowerBound(0));
      THROW_ON_FAILURE_EX(&node45 == *tree.LowerBound(45));
      THROW_ON_FAILURE_EX(&node45 == *tree.LowerBound(44));
      THROW_ON_FAILURE_EX(&node53 == *tree.LowerBound(53));
      THROW_ON_FAILURE_EX(&node57 == *tree.LowerBound(54));
      THROW_ON_FAILURE_EX(&node57 == *tree.LowerBound(57));
      THROW_ON_FAILURE_EX(&node58 == *tree.LowerBound(58));
      THROW_ON_FAILURE_EX(&node59 == *tree.LowerBound(59));
      THROW_ON_FAILURE_EX(&node60 == *tree.LowerBound(60));
      THROW_ON_FAILURE_EX(&node61 == *tree.LowerBound(61));
      THROW_ON_FAILURE_EX(tree.End() == tree.LowerBound(62));
   }
}


void CIntrusiveRedBlackTreeTest::TestUpperBound()
{
   SKIP_TEST_EX(_T("Not implemented - upper bound is harder than lower bound and we don't currently need it"));
   /*
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node5(5);
   CTestIntrusiveRedBlackTreeNode node7(7);

   {

      TestTree tree;

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


      THROW_ON_FAILURE_EX(true == tree.Insert(&node5).second);

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(3));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *tree.Find(5));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(6));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(7));

      THROW_ON_FAILURE_EX(true == tree.Insert(&node7).second);

      THROW_ON_FAILURE_EX(tree.End() == tree.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *tree.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *tree.Find(2));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(3));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *tree.Find(5));
      THROW_ON_FAILURE_EX(tree.End() == tree.Find(6));
      THROW_ON_FAILURE_EX(&node7 == *tree.Find(7));

      // now the test...


      THROW_ON_FAILURE_EX(&node1 == *tree.UpperBound(0));
      THROW_ON_FAILURE_EX(&node2 == *tree.UpperBound(1));
      THROW_ON_FAILURE_EX(&node5 == *tree.UpperBound(2));
      THROW_ON_FAILURE_EX(&node5 == *tree.UpperBound(3));
      THROW_ON_FAILURE_EX(&node5 == *tree.UpperBound(4));
      THROW_ON_FAILURE_EX(&node7 == *tree.UpperBound(5));
      THROW_ON_FAILURE_EX(&node7 == *tree.UpperBound(6));
      THROW_ON_FAILURE_EX(tree.End() == tree.UpperBound(7));
   }
   */
}

void CIntrusiveRedBlackTreeTest::TestBigInsertInOrder()
{
   NodeList nodeList;

   for (int i = 0; i < 100; ++i)
   {
      nodeList.push_back(new CTestIntrusiveRedBlackTreeNode(i));
   }

   {
      {
         TestTree tree;
         size_t i = 0;

         NodeList::const_iterator it = nodeList.begin();

         const NodeList::const_iterator end = nodeList.end();

         for (;
            it != end && i < 7;
            ++it, ++i)
         {
            CTestIntrusiveRedBlackTreeNode *pNode = *it;
            THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

            ValidateTree(tree);
         }

         CTestIntrusiveRedBlackTreeNode *pNode = *it;
         THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

         ValidateTree(tree);
      }

      TestTree tree;

      size_t size = 0;

      for (auto *pNode : nodeList)
      {
         ValidateTree(tree);

         THROW_ON_FAILURE_EX(size == tree.Size());

         THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

         size++;
      }

      for (auto *pNode : nodeList)
      {
         THROW_ON_FAILURE_EX(pNode == *tree.Find(pNode->Value()));
      }

      int i = 0;

      for (auto it = tree.Begin(), end = tree.End(); it != end; ++it, ++i)
      {
         THROW_IF_NOT_EQUAL_EX((*it)->Value(), i);
      }
   }

   for (auto *pNode : nodeList)
   {
      delete pNode;
   }
}

void CIntrusiveRedBlackTreeTest::TestBigInsertReverseOrder()
{
   NodeList nodeList;

   for (auto i = 0; i < 100; ++i)
   {
      nodeList.push_front(new CTestIntrusiveRedBlackTreeNode(i));
   }

   {
      {
         TestTree tree;
         size_t i = 0;

         NodeList::const_iterator it = nodeList.begin();

         const NodeList::const_iterator end = nodeList.end();

         for (;
            it != end && i < 7;
            ++it, ++i)
         {
            CTestIntrusiveRedBlackTreeNode *pNode = *it;
            THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

            ValidateTree(tree);
         }

         CTestIntrusiveRedBlackTreeNode *pNode = *it;
         THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

         ValidateTree(tree);
      }

      TestTree tree;

      size_t size = 0;

      for (auto *pNode : nodeList)
      {
         ValidateTree(tree);

         THROW_ON_FAILURE_EX(size == tree.Size());

         THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

         size++;
      }

      for (auto *pNode : nodeList)
      {
         THROW_ON_FAILURE_EX(pNode == *tree.Find(pNode->Value()));
      }

      int i = 0;

      for (auto it = tree.Begin(), end = tree.End(); it != end; ++it, ++i)
      {
         THROW_IF_NOT_EQUAL_EX((*it)->Value(), i);
      }
   }

   for (auto *pNode : nodeList)
   {
      delete pNode;
   }
}

void CIntrusiveRedBlackTreeTest::TestDestructDoesNotHarmNodes()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);

   {
      TestTree tree;

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
      TestTree tree;

      THROW_ON_FAILURE_EX(true == tree.Insert(&node1).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node2).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node3).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node4).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node5).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node6).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node7).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node8).second);

      ValidateTree(tree);

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

      ValidateTree(tree);

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

      ValidateTree(tree);

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
      TestTree tree;

      THROW_ON_FAILURE_EX(true == tree.Insert(&node1).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node2).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node3).second);
      THROW_ON_FAILURE_EX(true == tree.Insert(&node4).second);

      ValidateTree(tree);

      THROW_ON_FAILURE_EX(nullptr == tree.Remove(0));
      THROW_ON_FAILURE_EX(nullptr == tree.Remove(5));
      THROW_ON_FAILURE_EX(nullptr == tree.Remove(100));

   }
}

void CIntrusiveRedBlackTreeTest::TestForwardIterate()
{
   NodeList nodeList;

   const int numNodes = 10;

   for (auto i = 0; i < numNodes; ++i)
   {
      nodeList.push_back(new CTestIntrusiveRedBlackTreeNode(i));
   }

   {
      TestTree tree;

      for (auto *pNode : nodeList)
      {
         THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);
      }

      ValidateTree(tree);
      THROW_ON_FAILURE_EX(numNodes == tree.Size());

      // now run the test...

      TestTree::Iterator it = tree.Begin();

      const TestTree::Iterator end = tree.End();

      THROW_ON_FAILURE_EX(it != end);

      THROW_IF_NOT_EQUAL_EX(0, it->Value());
      THROW_IF_NOT_EQUAL_EX(0, it.Key());

      TestTree::Iterator it2 = it;

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

   for (auto *pNode : nodeList)
   {
      delete pNode;
   }
}

void CIntrusiveRedBlackTreeTest::TestRBegin()
{
   NodeList nodeList;

   const int numNodes = 10;

   for (auto i = 0; i < numNodes; ++i)
   {
      nodeList.push_back(new CTestIntrusiveRedBlackTreeNode(i));
   }

   {
      TestTree tree;

      size_t size = 0;

      for (auto *pNode : nodeList)
      {
         THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);

         ++size;

         ValidateTree(tree);
         THROW_ON_FAILURE_EX(size == tree.Size());

         // now run the test...

         TestTree::Iterator it = tree.RBegin();

         const TestTree::Iterator end = tree.End();

         THROW_ON_FAILURE_EX(it != end);

         THROW_IF_NOT_EQUAL_EX(pNode->Value(), it->Value());
         THROW_IF_NOT_EQUAL_EX(pNode->Key(), it.Key());
      }
   }

   for (auto *pNode : nodeList)
   {
      delete pNode;
   }
}


void CIntrusiveRedBlackTreeTest::TestReverseIterate()
{
   NodeList nodeList;

   const int numNodes = 10;

   for (auto i = 0; i < numNodes; ++i)
   {
      nodeList.push_back(new CTestIntrusiveRedBlackTreeNode(i));
   }

   {
      TestTree tree;

      for (auto *pNode : nodeList)
      {
         THROW_ON_FAILURE_EX(true == tree.Insert(pNode).second);
      }

      ValidateTree(tree);
      THROW_ON_FAILURE_EX(numNodes == tree.Size());

      // now run the test...

      TestTree::Iterator it = tree.RBegin();

      const TestTree::Iterator end = tree.End();

      THROW_ON_FAILURE_EX(it != end);

      THROW_IF_NOT_EQUAL_EX(9, it->Value());
      THROW_IF_NOT_EQUAL_EX(9, it.Key());

      TestTree::Iterator it2 = it;

      THROW_IF_NOT_EQUAL_EX(9, it2->Value());
      THROW_IF_NOT_EQUAL_EX(9, it2.Key());

      THROW_IF_NOT_EQUAL_EX(8, (--it)->Value());
      THROW_IF_NOT_EQUAL_EX(8, it->Value());
      THROW_IF_NOT_EQUAL_EX(8, it.Key());
      THROW_IF_NOT_EQUAL_EX(9, it2->Value());
      THROW_IF_NOT_EQUAL_EX(9, it2.Key());

      THROW_IF_NOT_EQUAL_EX(8, (it--)->Value());
      THROW_IF_NOT_EQUAL_EX(7, it->Value());
      THROW_IF_NOT_EQUAL_EX(7, it.Key());
      THROW_IF_NOT_EQUAL_EX(9, it2->Value());
      THROW_IF_NOT_EQUAL_EX(9, it2.Key());

      it -= 1;

      THROW_IF_NOT_EQUAL_EX(6, it->Value());
      THROW_IF_NOT_EQUAL_EX(6, it.Key());
      THROW_IF_NOT_EQUAL_EX(9, it2->Value());
      THROW_IF_NOT_EQUAL_EX(9, it2.Key());

      it2 -= 4;

      THROW_IF_NOT_EQUAL_EX(6, it->Value());
      THROW_IF_NOT_EQUAL_EX(6, it.Key());
      THROW_IF_NOT_EQUAL_EX(5, it2->Value());
      THROW_IF_NOT_EQUAL_EX(5, it2.Key());
   }

   for (auto *pNode : nodeList)
   {
      delete pNode;
   }
}

void CIntrusiveRedBlackTreeTest::TestClear()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);
   CTestIntrusiveRedBlackTreeNode node5(5);
   CTestIntrusiveRedBlackTreeNode node6(6);
   CTestIntrusiveRedBlackTreeNode node7(7);

   NodeList nodes;

   {
      TestTree tree;

      ValidateTree(tree);

      ValidateNodeInsert(node3, tree, nodes);
      ValidateNodeInsert(node1, tree, nodes);
      ValidateNodeInsert(node4, tree, nodes);
      ValidateNodeInsert(node2, tree, nodes);
      ValidateNodeInsert(node6, tree, nodes);
      ValidateNodeInsert(node5, tree, nodes);
      ValidateNodeInsert(node7, tree, nodes);

      THROW_IF_NOT_EQUAL_EX(tree.Size(), 7);

      tree.Clear();

      ValidateTree(tree);

      THROW_IF_NOT_EQUAL_EX(tree.Size(), 0);

      for (auto *pNode : nodes)
      {
         THROW_ON_FAILURE_EX(pNode->IsActive() == false);
      }
   }
}

void CIntrusiveRedBlackTreeTest::TestClearWithCallback()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);
   CTestIntrusiveRedBlackTreeNode node5(5);
   CTestIntrusiveRedBlackTreeNode node6(6);
   CTestIntrusiveRedBlackTreeNode node7(7);

   NodeMap nodes;

   {
      TestTree tree;

      ValidateTree(tree);

      ValidateNodeInsert(node3, tree, nodes);
      ValidateNodeInsert(node1, tree, nodes);
      ValidateNodeInsert(node4, tree, nodes);
      ValidateNodeInsert(node2, tree, nodes);
      ValidateNodeInsert(node6, tree, nodes);
      ValidateNodeInsert(node5, tree, nodes);
      ValidateNodeInsert(node7, tree, nodes);

      THROW_IF_NOT_EQUAL_EX(tree.Size(), 7);

      NodeList clearedNodes;

      tree.Clear(
         TestTree::ClearFlags::Erase,
         [&](CTestIntrusiveRedBlackTreeNode *pData) -> void {
            clearedNodes.push_back(pData);
         });

      ValidateTree(tree);

      THROW_IF_NOT_EQUAL_EX(tree.Size(), 0);

      THROW_IF_NOT_EQUAL_EX(clearedNodes.size(), 7);

      for (auto node : nodes)
      {
         THROW_ON_FAILURE_EX(node.second->IsActive() == false);

         THROW_ON_FAILURE_EX(node.second == clearedNodes.front());

         clearedNodes.pop_front();
      }

      THROW_ON_FAILURE_EX(clearedNodes.empty());
   }
}

void CIntrusiveRedBlackTreeTest::TestFastClear()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);
   CTestIntrusiveRedBlackTreeNode node5(5);
   CTestIntrusiveRedBlackTreeNode node6(6);
   CTestIntrusiveRedBlackTreeNode node7(7);

   NodeList nodes;

   {
      TestTree tree;

      ValidateTree(tree);

      ValidateNodeInsert(node3, tree, nodes);
      ValidateNodeInsert(node1, tree, nodes);
      ValidateNodeInsert(node4, tree, nodes);
      ValidateNodeInsert(node2, tree, nodes);
      ValidateNodeInsert(node6, tree, nodes);
      ValidateNodeInsert(node5, tree, nodes);
      ValidateNodeInsert(node7, tree, nodes);

      THROW_IF_NOT_EQUAL_EX(tree.Size(), 7);

      tree.Clear(TestTree::ClearFlags::Fast);

      ValidateTree(tree);

      THROW_IF_NOT_EQUAL_EX(tree.Size(), 0);

      for (auto *pNode : nodes)
      {
         THROW_ON_FAILURE_EX(pNode->IsActive() == false);
      }
   }
}

void CIntrusiveRedBlackTreeTest::TestFastClearWithCallback()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);
   CTestIntrusiveRedBlackTreeNode node5(5);
   CTestIntrusiveRedBlackTreeNode node6(6);
   CTestIntrusiveRedBlackTreeNode node7(7);

   NodeMap nodes;

   {
      TestTree tree;

      ValidateTree(tree);

      ValidateNodeInsert(node3, tree, nodes);
      ValidateNodeInsert(node1, tree, nodes);
      ValidateNodeInsert(node4, tree, nodes);
      ValidateNodeInsert(node2, tree, nodes);
      ValidateNodeInsert(node6, tree, nodes);
      ValidateNodeInsert(node5, tree, nodes);
      ValidateNodeInsert(node7, tree, nodes);

      THROW_IF_NOT_EQUAL_EX(tree.Size(), 7);

      NodeList clearedNodes;

      tree.Clear(
         TestTree::ClearFlags::Fast,
         [&](CTestIntrusiveRedBlackTreeNode *pData) -> void {
         clearedNodes.push_back(pData);
         });

      ValidateTree(tree);

      THROW_IF_NOT_EQUAL_EX(tree.Size(), 0);

      THROW_IF_NOT_EQUAL_EX(clearedNodes.size(), 7);

      for (auto node : nodes)
      {
         THROW_ON_FAILURE_EX(node.second->IsActive() == false);

         THROW_ON_FAILURE_EX(node.second == clearedNodes.front());

         clearedNodes.pop_front();
      }
   }
}

void CIntrusiveRedBlackTreeTest::TestFastAndDirtyClear()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);
   CTestIntrusiveRedBlackTreeNode node5(5);
   CTestIntrusiveRedBlackTreeNode node6(6);
   CTestIntrusiveRedBlackTreeNode node7(7);

   NodeList nodes;

   {
      TestTree tree;

      ValidateTree(tree);

      ValidateNodeInsert(node3, tree, nodes);
      ValidateNodeInsert(node1, tree, nodes);
      ValidateNodeInsert(node4, tree, nodes);
      ValidateNodeInsert(node2, tree, nodes);
      ValidateNodeInsert(node6, tree, nodes);
      ValidateNodeInsert(node5, tree, nodes);
      ValidateNodeInsert(node7, tree, nodes);

      THROW_IF_NOT_EQUAL_EX(tree.Size(), 7);

      // Nodes are left as they are and cannot be inserted
      // into another tree unless you call RemoveFromTree() on them.
      // The tree is empty though.

      tree.Clear(TestTree::ClearFlags::FastAndDirty);

      ValidateTree(tree);

      THROW_IF_NOT_EQUAL_EX(tree.Size(), 0);

      for (auto *pNode : nodes)
      {
         THROW_ON_FAILURE_EX(pNode->IsActive() == true);
      }
   }
}

void CIntrusiveRedBlackTreeTest::TestFastAndDirtyClearWithCallback()
{
   CTestIntrusiveRedBlackTreeNode node1(1);
   CTestIntrusiveRedBlackTreeNode node2(2);
   CTestIntrusiveRedBlackTreeNode node3(3);
   CTestIntrusiveRedBlackTreeNode node4(4);
   CTestIntrusiveRedBlackTreeNode node5(5);
   CTestIntrusiveRedBlackTreeNode node6(6);
   CTestIntrusiveRedBlackTreeNode node7(7);

   NodeMap nodes;

   {
      TestTree tree;

      ValidateTree(tree);

      ValidateNodeInsert(node3, tree, nodes);
      ValidateNodeInsert(node1, tree, nodes);
      ValidateNodeInsert(node4, tree, nodes);
      ValidateNodeInsert(node2, tree, nodes);
      ValidateNodeInsert(node6, tree, nodes);
      ValidateNodeInsert(node5, tree, nodes);
      ValidateNodeInsert(node7, tree, nodes);

      THROW_IF_NOT_EQUAL_EX(tree.Size(), 7);

      NodeList clearedNodes;

      // Nodes are left as they are and cannot be inserted
      // into another tree unless you call RemoveFromTree() on them.
      // The tree is empty though.

      ValidateTree(tree);

      tree.Clear(
         TestTree::ClearFlags::FastAndDirty,
         [&](CTestIntrusiveRedBlackTreeNode *pData) -> void {
         clearedNodes.push_back(pData);
         });

      ValidateTree(tree);

      THROW_IF_NOT_EQUAL_EX(tree.Size(), 0);

      THROW_IF_NOT_EQUAL_EX(clearedNodes.size(), 7);

      for (auto node : nodes)
      {
         THROW_ON_FAILURE_EX(node.second->IsActive() == true);

         THROW_ON_FAILURE_EX(node.second == clearedNodes.front());

         clearedNodes.pop_front();
      }
   }
}
///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static void ValidateNodeInsert(
   CTestIntrusiveRedBlackTreeNode &node,
   TestTree &tree)
{
   THROW_ON_FAILURE_EX(true == tree.Insert(&node).second);

   ValidateTree(tree);
}

static void ValidateNodeInsert(
   CTestIntrusiveRedBlackTreeNode &node,
   TestTree &tree,
   NodeList &nodes)
{
   ValidateNodeInsert(node, tree);

   nodes.push_back(&node);
}

static void ValidateNodeInsert(
   CTestIntrusiveRedBlackTreeNode &node,
   TestTree &tree,
   NodeMap &nodes)
{
   ValidateNodeInsert(node, tree);

   nodes.emplace(node.Key(), &node);
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveRedBlackTreeTest.cpp
///////////////////////////////////////////////////////////////////////////////

