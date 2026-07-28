///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveMultiMapTest.cpp
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

#include "IntrusiveMultiMapTest.h"

#include "JetByteTools/TestTools/TestException.h"
#include "JetByteTools/TestTools/RunTest.h"

#include "JetByteTools/CoreTools/DebugTrace.h"

#pragma hdrstop

#include "JetByteTools/CoreTools/IntrusiveMultiMap.h"
#include "JetByteTools/CoreTools/Mock/TestIntrusiveMultiMapNode.h"

#include <deque>
#include <set>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;
using JetByteTools::Test::CTestMonitor;

using JetByteTools::Core::Mock::CTestIntrusiveMultiMapNode;
using JetByteTools::Core::Mock::CTestIntrusiveMultiMapNodeKeyAccessor;

typedef JetByteTools::Core::TIntrusiveMultiMap<CTestIntrusiveMultiMapNode, int, CTestIntrusiveMultiMapNodeKeyAccessor> TestMap;

typedef std::deque<CTestIntrusiveMultiMapNode *> NodeList;

typedef std::set<CTestIntrusiveMultiMapNode *> NodeSet;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static void ValidateNodeInsert(
   CTestIntrusiveMultiMapNode &node,
   TestMap &map);

static void ValidateNodeInsert(
   CTestIntrusiveMultiMapNode &node,
   TestMap &map,
   NodeList &nodes);

static void ValidateNodeInsert(
   CTestIntrusiveMultiMapNode &node,
   TestMap &map,
   NodeSet &nodes);

static void RemoveNode(
   const CTestIntrusiveMultiMapNode &node,
   NodeList &nodes);

static void ValidateMapContainsNodes(
   const TestMap &map,
   const NodeList &nodes);

static void RemoveOneAndValidate(
   TestMap &map,
   const int value,
   const CTestIntrusiveMultiMapNode &expectedNode,
   NodeList &nodes);

static void ValidateMap(
   const TestMap &map);

///////////////////////////////////////////////////////////////////////////////
// CIntrusiveMultiMapTest
///////////////////////////////////////////////////////////////////////////////

void CIntrusiveMultiMapTest::TestAll(
   CTestMonitor &monitor)
{
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestConstruct);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestInsert);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestInsertWithExplicitKey);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestInsertWithIncorrectExplicitKey);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestInsertDuplicate);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestFind);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestBigInsertInOrder);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestBigInsertReverseOrder);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestDestructDoesNotHarmNodes);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestRemove);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestRemoveNodeNotPresent);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestRemoveOneMultipleNodesAtKey);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestRemoveAllMultipleNodesAtKey);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestForwardIterate);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestReverseIterate);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestClear);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestClearWithCallback);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestFastClear);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestFastClearWithCallback);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestFastAndDirtyClear);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestFastAndDirtyClearWithCallback);

   // test with large amount of random data with a known seed, inserts, finds and deletes
}

void CIntrusiveMultiMapTest::TestConstruct()
{
   {
      TIntrusiveMultiMap<CTestIntrusiveMultiMapNode, int, CTestIntrusiveMultiMapNodeKeyAccessor> map;
   }
}

void CIntrusiveMultiMapTest::TestInsert()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node2(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);
   CTestIntrusiveMultiMapNode node5(2);
   CTestIntrusiveMultiMapNode node6(2);
   CTestIntrusiveMultiMapNode node7(3);

   {
      TestMap map;

      ValidateMap(map);

      THROW_ON_FAILURE_EX(0 == map.Size());
      THROW_ON_FAILURE_EX(map.Begin() == map.End());

      {
         THROW_ON_FAILURE_EX(&node3 == *map.Insert(&node3));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(1 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node1 == *map.Insert(&node1));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(2 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node4 == *map.Insert(&node4));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(3 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node2 == *map.Insert(&node2));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(4 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node6 == *map.Insert(&node6));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(5 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());

         const CTestIntrusiveMultiMapNode *pNode = *it;

         THROW_ON_FAILURE_EX(pNode == &node2);        // value is 2
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node6);          // value is 2
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node5 == *map.Insert(&node5));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(6 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node2);          // value is 2
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node5);          // value is 2
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node6);          // value is 2
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node7 == *map.Insert(&node7));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(7 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node2);          // value is 2
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node5);          // value is 2
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node6);          // value is 2
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);          // value is 3
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node7);          // value is 3
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }
   }
}

void CIntrusiveMultiMapTest::TestInsertWithExplicitKey()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node2(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);
   CTestIntrusiveMultiMapNode node5(5);
   CTestIntrusiveMultiMapNode node6(6);
   CTestIntrusiveMultiMapNode node7(7);

   {
      TestMap map;

      ValidateMap(map);

      THROW_ON_FAILURE_EX(0 == map.Size());
      THROW_ON_FAILURE_EX(map.Begin() == map.End());

      {
         THROW_ON_FAILURE_EX(&node3 == *map.Insert(&node3, 3));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(1 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node1 == *map.Insert(&node1, 1));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(2 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node4 == *map.Insert(&node4, 4));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(3 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         THROW_IF_NOT_EQUAL_EX(4, it->Value());
         THROW_IF_NOT_EQUAL_EX(4, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node2 == *map.Insert(&node2, 2));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(4 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         THROW_IF_NOT_EQUAL_EX(2, it->Value());
         THROW_IF_NOT_EQUAL_EX(2, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         THROW_IF_NOT_EQUAL_EX(4, it->Value());
         THROW_IF_NOT_EQUAL_EX(4, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node6 == *map.Insert(&node6, 6));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(5 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         THROW_IF_NOT_EQUAL_EX(2, it->Value());
         THROW_IF_NOT_EQUAL_EX(2, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         THROW_IF_NOT_EQUAL_EX(4, it->Value());
         THROW_IF_NOT_EQUAL_EX(4, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node6);
         THROW_IF_NOT_EQUAL_EX(6, it->Value());
         THROW_IF_NOT_EQUAL_EX(6, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node5 == *map.Insert(&node5, 5));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(6 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         THROW_IF_NOT_EQUAL_EX(2, it->Value());
         THROW_IF_NOT_EQUAL_EX(2, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         THROW_IF_NOT_EQUAL_EX(4, it->Value());
         THROW_IF_NOT_EQUAL_EX(4, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node5);
         THROW_IF_NOT_EQUAL_EX(5, it->Value());
         THROW_IF_NOT_EQUAL_EX(5, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node6);
         THROW_IF_NOT_EQUAL_EX(6, it->Value());
         THROW_IF_NOT_EQUAL_EX(6, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node7 == *map.Insert(&node7, 7));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(7 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         THROW_IF_NOT_EQUAL_EX(2, it->Value());
         THROW_IF_NOT_EQUAL_EX(2, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node4);
         THROW_IF_NOT_EQUAL_EX(4, it->Value());
         THROW_IF_NOT_EQUAL_EX(4, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node5);
         THROW_IF_NOT_EQUAL_EX(5, it->Value());
         THROW_IF_NOT_EQUAL_EX(5, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node6);
         THROW_IF_NOT_EQUAL_EX(6, it->Value());
         THROW_IF_NOT_EQUAL_EX(6, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node7);
         THROW_IF_NOT_EQUAL_EX(7, it->Value());
         THROW_IF_NOT_EQUAL_EX(7, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }
   }
}

void CIntrusiveMultiMapTest::TestInsertWithIncorrectExplicitKey()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node2(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);

   {
      TestMap map;

      ValidateMap(map);

      THROW_ON_FAILURE_EX(0 == map.Size());
      THROW_ON_FAILURE_EX(map.Begin() == map.End());


      {
         THROW_ON_FAILURE_EX(&node3 == *map.Insert(&node3, 3));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(1 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node1 == *map.Insert(&node1, 1));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(2 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      // This key is wrong, it's not the same value that 'extracting the key from the node'
      // will give...

      {
         THROW_ON_NO_EXCEPTION_EX_2(map.Insert, &node4, 2);

         // Map is unchanged

         ValidateMap(map);

         THROW_ON_FAILURE_EX(2 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         THROW_IF_NOT_EQUAL_EX(1, it->Value());
         THROW_IF_NOT_EQUAL_EX(1, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         THROW_IF_NOT_EQUAL_EX(3, it->Value());
         THROW_IF_NOT_EQUAL_EX(3, it->Key());
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }
   }
}

void CIntrusiveMultiMapTest::TestInsertSameNodeTwice()
{
   CTestIntrusiveMultiMapNode node1(1);

   {
      TestMap map;

      ValidateMap(map);

      THROW_ON_FAILURE_EX(0 == map.Size());
      THROW_ON_FAILURE_EX(map.Begin() == map.End());

      {
         THROW_ON_FAILURE_EX(&node1 == *map.Insert(&node1));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(1 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_NO_EXCEPTION_EX_1(map.Insert, &node1);

         ValidateMap(map);

         THROW_ON_FAILURE_EX(2 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }
   }
}

void CIntrusiveMultiMapTest::TestInsertDuplicate()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node2(1);

   {
      TestMap map;

      ValidateMap(map);

      THROW_ON_FAILURE_EX(0 == map.Size());
      THROW_ON_FAILURE_EX(map.Begin() == map.End());

      {
         THROW_ON_FAILURE_EX(&node1 == *map.Insert(&node1));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(1 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node2 == *map.Insert(&node2));

         ValidateMap(map);

         THROW_ON_FAILURE_EX(2 == map.Size());

         TestMap::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node1);
         ++it;
         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node2);
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }
   }
}
void CIntrusiveMultiMapTest::TestFind()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node2(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);
   CTestIntrusiveMultiMapNode node5(2);
   CTestIntrusiveMultiMapNode node6(2);
   CTestIntrusiveMultiMapNode node7(3);

   {
      TestMap map;

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(map.End() == map.Find(1));
      THROW_ON_FAILURE_EX(map.End() == map.Find(2));
      THROW_ON_FAILURE_EX(map.End() == map.Find(3));
      THROW_ON_FAILURE_EX(map.End() == map.Find(4));

      THROW_ON_FAILURE_EX(&node1 == *map.Insert(&node1));

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
      THROW_ON_FAILURE_EX(map.End() == map.Find(2));
      THROW_ON_FAILURE_EX(map.End() == map.Find(3));
      THROW_ON_FAILURE_EX(map.End() == map.Find(4));

      THROW_ON_FAILURE_EX(&node2 == *map.Insert(&node2));

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *map.Find(2));
      THROW_ON_FAILURE_EX(map.End() == map.Find(3));
      THROW_ON_FAILURE_EX(map.End() == map.Find(4));

      THROW_ON_FAILURE_EX(&node3 == *map.Insert(&node3));

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *map.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *map.Find(3));
      THROW_ON_FAILURE_EX(map.End() == map.Find(4));

      THROW_ON_FAILURE_EX(&node4 == *map.Insert(&node4));

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *map.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *map.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));

      THROW_ON_FAILURE_EX(&node5 == *map.Insert(&node5));          // value is 2

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *map.Find(2));

      TestMap::Iterator it = map.Find(2);

      THROW_ON_FAILURE_EX(&node2 == *it);
      ++it;
      THROW_ON_FAILURE_EX(&node5 == *it);
      ++it;
      THROW_ON_FAILURE_EX(&node3 == *it);

      THROW_ON_FAILURE_EX(&node3 == *map.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));

      THROW_ON_FAILURE_EX(&node6 == *map.Insert(&node6));          // value is 2

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));

      it = map.Find(2);

      THROW_ON_FAILURE_EX(&node2 == *it);
      ++it;
      THROW_ON_FAILURE_EX(&node6 == *it);
      ++it;
      THROW_ON_FAILURE_EX(&node5 == *it);
      ++it;
      THROW_ON_FAILURE_EX(&node3 == *it);

      THROW_ON_FAILURE_EX(&node3 == *map.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));

      THROW_ON_FAILURE_EX(&node7 == *map.Insert(&node7));          // value is 3

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));

      it = map.Find(2);

      THROW_ON_FAILURE_EX(&node2 == *it);
      ++it;
      THROW_ON_FAILURE_EX(&node6 == *it);
      ++it;
      THROW_ON_FAILURE_EX(&node5 == *it);
      ++it;
      THROW_ON_FAILURE_EX(&node3 == *it);

      it = map.Find(3);

      THROW_ON_FAILURE_EX(&node3 == *it);
      ++it;
      THROW_ON_FAILURE_EX(&node7 == *it);
      ++it;
      THROW_ON_FAILURE_EX(&node4 == *it);

      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));
   }
}

void CIntrusiveMultiMapTest::TestBigInsertInOrder()
{
   NodeList nodeList;

   for (int i = 0; i < 100; ++i)
   {
      nodeList.push_back(new CTestIntrusiveMultiMapNode(i));
   }

   {
      {
         // what's this for?

         TestMap map;
         size_t i = 0;

         NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();

         for (;
            it != end && i < 7;
            ++it, ++i)
         {
            CTestIntrusiveMultiMapNode *pNode = *it;
            THROW_ON_FAILURE_EX(pNode == *map.Insert(pNode));

            ValidateMap(map);
         }

         CTestIntrusiveMultiMapNode *pNode = *it;
         THROW_ON_FAILURE_EX(pNode == *map.Insert(pNode));

         ValidateMap(map);
      }

      TestMap map;

      size_t size = 0;

      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
         it != end;
         ++it)
      {
         ValidateMap(map);

         THROW_ON_FAILURE_EX(size == map.Size());

         CTestIntrusiveMultiMapNode *pNode = *it;

         THROW_ON_FAILURE_EX(pNode == *map.Insert(pNode));

         size++;
      }

      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
         it != end;
         ++it)
      {
         CTestIntrusiveMultiMapNode *pNode = *it;

         THROW_ON_FAILURE_EX(pNode == *map.Find(pNode->Value()));
      }

      int i = 0;

      for (TestMap::Iterator it = map.Begin(), end = map.End(); it != end; ++it, ++i)
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

void CIntrusiveMultiMapTest::TestBigInsertReverseOrder()
{
   NodeList nodeList;

   for (int i = 0; i < 100; ++i)
   {
      nodeList.push_front(new CTestIntrusiveMultiMapNode(i));
   }

   {
      TestMap map;

      size_t size = 0;

      {
         TestMap map;
         size_t i = 0;

         NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();

         for (;
            it != end && i < 7;
            ++it, ++i)
         {
            CTestIntrusiveMultiMapNode *pNode = *it;
            THROW_ON_FAILURE_EX(pNode == *map.Insert(pNode));

            ValidateMap(map);
         }

         CTestIntrusiveMultiMapNode *pNode = *it;
         THROW_ON_FAILURE_EX(pNode == *map.Insert(pNode));

         ValidateMap(map);
      }

      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
         it != end;
         ++it)
      {
         ValidateMap(map);

         THROW_ON_FAILURE_EX(size == map.Size());

         CTestIntrusiveMultiMapNode *pNode = *it;

         THROW_ON_FAILURE_EX(pNode == *map.Insert(pNode));

         size++;
      }

      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
         it != end;
         ++it)
      {
         CTestIntrusiveMultiMapNode *pNode = *it;

         THROW_ON_FAILURE_EX(pNode == *map.Find(pNode->Value()));
      }

      int i = 0;

      for (TestMap::Iterator it = map.Begin(), end = map.End(); it != end; ++it, ++i)
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

void CIntrusiveMultiMapTest::TestDestructDoesNotHarmNodes()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node2(2);
   CTestIntrusiveMultiMapNode node3(3);

   {
      TestMap map;

      THROW_ON_FAILURE_EX(&node1 == *map.Insert(&node1));
      THROW_ON_FAILURE_EX(&node2 == *map.Insert(&node2));
      THROW_ON_FAILURE_EX(&node3 == *map.Insert(&node3));
   }

   THROW_ON_FAILURE_EX(node1.Value() == 1);
   THROW_ON_FAILURE_EX(node2.Value() == 2);
   THROW_ON_FAILURE_EX(node3.Value() == 3);
}

void CIntrusiveMultiMapTest::TestRemove()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node2(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);
   CTestIntrusiveMultiMapNode node5(5);
   CTestIntrusiveMultiMapNode node6(6);
   CTestIntrusiveMultiMapNode node7(7);
   CTestIntrusiveMultiMapNode node8(8);

   {
      TestMap map;

      THROW_ON_FAILURE_EX(&node1 == *map.Insert(&node1));
      THROW_ON_FAILURE_EX(&node2 == *map.Insert(&node2));
      THROW_ON_FAILURE_EX(&node3 == *map.Insert(&node3));
      THROW_ON_FAILURE_EX(&node4 == *map.Insert(&node4));
      THROW_ON_FAILURE_EX(&node5 == *map.Insert(&node5));
      THROW_ON_FAILURE_EX(&node6 == *map.Insert(&node6));
      THROW_ON_FAILURE_EX(&node7 == *map.Insert(&node7));
      THROW_ON_FAILURE_EX(&node8 == *map.Insert(&node8));

      ValidateMap(map);

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *map.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *map.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *map.Find(5));
      THROW_ON_FAILURE_EX(&node6 == *map.Find(6));
      THROW_ON_FAILURE_EX(&node7 == *map.Find(7));
      THROW_ON_FAILURE_EX(&node8 == *map.Find(8));
      THROW_ON_FAILURE_EX(map.End() == map.Find(9));

      THROW_ON_FAILURE_EX(&node3 == map.RemoveOne(3));

      ValidateMap(map);

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *map.Find(2));
      THROW_ON_FAILURE_EX(map.End() == map.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *map.Find(5));
      THROW_ON_FAILURE_EX(&node6 == *map.Find(6));
      THROW_ON_FAILURE_EX(&node7 == *map.Find(7));
      THROW_ON_FAILURE_EX(&node8 == *map.Find(8));
      THROW_ON_FAILURE_EX(map.End() == map.Find(9));

      THROW_ON_FAILURE_EX(&node4 == map.RemoveOne(4));

      ValidateMap(map);

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
      THROW_ON_FAILURE_EX(&node2 == *map.Find(2));
      THROW_ON_FAILURE_EX(map.End() == map.Find(3));
      THROW_ON_FAILURE_EX(map.End() == map.Find(4));
      THROW_ON_FAILURE_EX(&node5 == *map.Find(5));
      THROW_ON_FAILURE_EX(&node6 == *map.Find(6));
      THROW_ON_FAILURE_EX(&node7 == *map.Find(7));
      THROW_ON_FAILURE_EX(&node8 == *map.Find(8));
      THROW_ON_FAILURE_EX(map.End() == map.Find(9));

   }
}

void CIntrusiveMultiMapTest::TestRemoveNodeNotPresent()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node2(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);

   {
      TestMap map;

      THROW_ON_FAILURE_EX(&node1 == *map.Insert(&node1));
      THROW_ON_FAILURE_EX(&node2 == *map.Insert(&node2));
      THROW_ON_FAILURE_EX(&node3 == *map.Insert(&node3));
      THROW_ON_FAILURE_EX(&node4 == *map.Insert(&node4));

      ValidateMap(map);

      THROW_ON_FAILURE_EX(nullptr == map.RemoveOne(0));
      THROW_ON_FAILURE_EX(nullptr == map.RemoveOne(5));
      THROW_ON_FAILURE_EX(nullptr == map.RemoveOne(100));

   }
}

void CIntrusiveMultiMapTest::TestRemoveOneMultipleNodesAtKey()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node21(2);
   CTestIntrusiveMultiMapNode node22(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);
   CTestIntrusiveMultiMapNode node51(5);
   CTestIntrusiveMultiMapNode node52(5);
   CTestIntrusiveMultiMapNode node53(5);
   CTestIntrusiveMultiMapNode node61(6);
   CTestIntrusiveMultiMapNode node62(6);
   CTestIntrusiveMultiMapNode node63(6);
   CTestIntrusiveMultiMapNode node64(6);
   CTestIntrusiveMultiMapNode node71(7);
   CTestIntrusiveMultiMapNode node72(7);
   CTestIntrusiveMultiMapNode node8(8);

   NodeList nodes;

   {
      TestMap map;

      ValidateNodeInsert(node1, map, nodes);
      ValidateNodeInsert(node21, map, nodes);
      ValidateNodeInsert(node22, map, nodes);
      ValidateNodeInsert(node3, map, nodes);
      ValidateNodeInsert(node4, map, nodes);
      ValidateNodeInsert(node51, map, nodes);
      ValidateNodeInsert(node52, map, nodes);
      ValidateNodeInsert(node53, map, nodes);
      ValidateNodeInsert(node61, map, nodes);
      ValidateNodeInsert(node62, map, nodes);
      ValidateNodeInsert(node63, map, nodes);
      ValidateNodeInsert(node64, map, nodes);
      ValidateNodeInsert(node71, map, nodes);
      ValidateNodeInsert(node72, map, nodes);
      ValidateNodeInsert(node8, map, nodes);

      ValidateMap(map);

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
      THROW_ON_FAILURE_EX(&node21 == *map.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *map.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));
      THROW_ON_FAILURE_EX(&node51 == *map.Find(5));
      THROW_ON_FAILURE_EX(&node61 == *map.Find(6));
      THROW_ON_FAILURE_EX(&node71 == *map.Find(7));
      THROW_ON_FAILURE_EX(&node8 == *map.Find(8));
      THROW_ON_FAILURE_EX(map.End() == map.Find(9));

      ValidateMapContainsNodes(map, nodes);

      THROW_ON_FAILURE_EX(&node51 == map.RemoveOne(5));

      RemoveNode(node51, nodes);

      ValidateMap(map);

      ValidateMapContainsNodes(map, nodes);

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
      THROW_ON_FAILURE_EX(&node21 == *map.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *map.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));
      THROW_ON_FAILURE_EX(&node53 == *map.Find(5));
      THROW_ON_FAILURE_EX(&node61 == *map.Find(6));
      THROW_ON_FAILURE_EX(&node71 == *map.Find(7));
      THROW_ON_FAILURE_EX(&node8 == *map.Find(8));
      THROW_ON_FAILURE_EX(map.End() == map.Find(9));

      RemoveOneAndValidate(map, 5, node53, nodes);
      RemoveOneAndValidate(map, 5, node52, nodes);
      RemoveOneAndValidate(map, 8, node8, nodes);
      RemoveOneAndValidate(map, 7, node71, nodes);
      RemoveOneAndValidate(map, 7, node72, nodes);
   }
}

void CIntrusiveMultiMapTest::TestRemoveAllMultipleNodesAtKey()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node21(2);
   CTestIntrusiveMultiMapNode node22(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);
   CTestIntrusiveMultiMapNode node51(5);
   CTestIntrusiveMultiMapNode node52(5);
   CTestIntrusiveMultiMapNode node53(5);
   CTestIntrusiveMultiMapNode node61(6);
   CTestIntrusiveMultiMapNode node62(6);
   CTestIntrusiveMultiMapNode node63(6);
   CTestIntrusiveMultiMapNode node64(6);
   CTestIntrusiveMultiMapNode node71(7);
   CTestIntrusiveMultiMapNode node72(7);
   CTestIntrusiveMultiMapNode node8(8);

   NodeList nodes;

   {
      TestMap map;

      ValidateNodeInsert(node1, map, nodes);
      ValidateNodeInsert(node21, map, nodes);
      ValidateNodeInsert(node22, map, nodes);
      ValidateNodeInsert(node3, map, nodes);
      ValidateNodeInsert(node4, map, nodes);
      ValidateNodeInsert(node51, map, nodes);
      ValidateNodeInsert(node52, map, nodes);
      ValidateNodeInsert(node53, map, nodes);
      ValidateNodeInsert(node61, map, nodes);
      ValidateNodeInsert(node62, map, nodes);
      ValidateNodeInsert(node63, map, nodes);
      ValidateNodeInsert(node64, map, nodes);
      ValidateNodeInsert(node71, map, nodes);
      ValidateNodeInsert(node72, map, nodes);
      ValidateNodeInsert(node8, map, nodes);

      ValidateMap(map);

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
      THROW_ON_FAILURE_EX(&node21 == *map.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *map.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));
      THROW_ON_FAILURE_EX(&node51 == *map.Find(5));
      THROW_ON_FAILURE_EX(&node61 == *map.Find(6));
      THROW_ON_FAILURE_EX(&node71 == *map.Find(7));
      THROW_ON_FAILURE_EX(&node8 == *map.Find(8));
      THROW_ON_FAILURE_EX(map.End() == map.Find(9));

      ValidateMapContainsNodes(map, nodes);

      {
         TestMap::NodeCollection removedNodes;

         map.RemoveAll(5, removedNodes);

         TestMap::NodeCollection::Iterator it = removedNodes.Begin();

         THROW_ON_FAILURE_EX(it != removedNodes.End());
         THROW_ON_FAILURE_EX(&node51 == *it);
         RemoveNode(node51, nodes);

         ++it;

         THROW_ON_FAILURE_EX(it != removedNodes.End());
         THROW_ON_FAILURE_EX(&node53 == *it);
         RemoveNode(node53, nodes);

         ++it;

         THROW_ON_FAILURE_EX(it != removedNodes.End());
         THROW_ON_FAILURE_EX(&node52 == *it);
         RemoveNode(node52, nodes);

         ++it;

         THROW_ON_FAILURE_EX(it == removedNodes.End());
      }

      ValidateMap(map);

      ValidateMapContainsNodes(map, nodes);

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
      THROW_ON_FAILURE_EX(&node21 == *map.Find(2));
      THROW_ON_FAILURE_EX(&node3 == *map.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));
      THROW_ON_FAILURE_EX(map.End() == map.Find(5));
      THROW_ON_FAILURE_EX(&node61 == *map.Find(6));
      THROW_ON_FAILURE_EX(&node71 == *map.Find(7));
      THROW_ON_FAILURE_EX(&node8 == *map.Find(8));
      THROW_ON_FAILURE_EX(map.End() == map.Find(9));
   }
}

void CIntrusiveMultiMapTest::TestClear()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node21(2);
   CTestIntrusiveMultiMapNode node22(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);
   CTestIntrusiveMultiMapNode node51(5);
   CTestIntrusiveMultiMapNode node52(5);
   CTestIntrusiveMultiMapNode node53(5);
   CTestIntrusiveMultiMapNode node61(6);
   CTestIntrusiveMultiMapNode node62(6);
   CTestIntrusiveMultiMapNode node63(6);
   CTestIntrusiveMultiMapNode node64(6);
   CTestIntrusiveMultiMapNode node71(7);
   CTestIntrusiveMultiMapNode node72(7);
   CTestIntrusiveMultiMapNode node8(8);

   NodeList nodes;

   {
      TestMap map;

      ValidateNodeInsert(node1, map, nodes);
      ValidateNodeInsert(node21, map, nodes);
      ValidateNodeInsert(node22, map, nodes);
      ValidateNodeInsert(node3, map, nodes);
      ValidateNodeInsert(node4, map, nodes);
      ValidateNodeInsert(node51, map, nodes);
      ValidateNodeInsert(node52, map, nodes);
      ValidateNodeInsert(node53, map, nodes);
      ValidateNodeInsert(node61, map, nodes);
      ValidateNodeInsert(node62, map, nodes);
      ValidateNodeInsert(node63, map, nodes);
      ValidateNodeInsert(node64, map, nodes);
      ValidateNodeInsert(node71, map, nodes);
      ValidateNodeInsert(node72, map, nodes);
      ValidateNodeInsert(node8, map, nodes);

      ValidateMap(map);

      THROW_IF_NOT_EQUAL_EX(map.Size(), 15);

      map.Clear();

      ValidateMap(map);

      THROW_IF_NOT_EQUAL_EX(map.Size(), 0);

      for (auto *pNode : nodes)
      {
         THROW_ON_FAILURE_EX(pNode->IsActive() == false);
      }
   }
}

void CIntrusiveMultiMapTest::TestClearWithCallback()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node21(2);
   CTestIntrusiveMultiMapNode node22(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);
   CTestIntrusiveMultiMapNode node51(5);
   CTestIntrusiveMultiMapNode node52(5);
   CTestIntrusiveMultiMapNode node53(5);
   CTestIntrusiveMultiMapNode node61(6);
   CTestIntrusiveMultiMapNode node62(6);
   CTestIntrusiveMultiMapNode node63(6);
   CTestIntrusiveMultiMapNode node64(6);
   CTestIntrusiveMultiMapNode node71(7);
   CTestIntrusiveMultiMapNode node72(7);
   CTestIntrusiveMultiMapNode node8(8);

   NodeSet nodes;

   {
      TestMap map;

      ValidateNodeInsert(node1, map, nodes);
      ValidateNodeInsert(node21, map, nodes);
      ValidateNodeInsert(node22, map, nodes);
      ValidateNodeInsert(node3, map, nodes);
      ValidateNodeInsert(node4, map, nodes);
      ValidateNodeInsert(node51, map, nodes);
      ValidateNodeInsert(node52, map, nodes);
      ValidateNodeInsert(node53, map, nodes);
      ValidateNodeInsert(node61, map, nodes);
      ValidateNodeInsert(node62, map, nodes);
      ValidateNodeInsert(node63, map, nodes);
      ValidateNodeInsert(node64, map, nodes);
      ValidateNodeInsert(node71, map, nodes);
      ValidateNodeInsert(node72, map, nodes);
      ValidateNodeInsert(node8, map, nodes);

      ValidateMap(map);

      THROW_IF_NOT_EQUAL_EX(map.Size(), 15);

      NodeSet clearedNodes;

      map.Clear(
         TestMap::ClearFlags::Erase,
         [&](CTestIntrusiveMultiMapNode *pData) -> void {
         clearedNodes.insert(pData);
         });

      ValidateMap(map);

      THROW_IF_NOT_EQUAL_EX(map.Size(), 0);

      // iteration over "duplicate" keys in a multi-map is not
      // guaranteed to be in insertion order, so we just rely
      // on all the nodes being passed to us

      for (auto *pNode : nodes)
      {
         THROW_ON_FAILURE_EX(pNode->IsActive() == false);

         NodeSet::iterator it = clearedNodes.begin();

         THROW_ON_FAILURE_EX(pNode == *it);

         clearedNodes.erase(it);
      }

      THROW_ON_FAILURE_EX(clearedNodes.empty());
   }
}

void CIntrusiveMultiMapTest::TestFastClear()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node21(2);
   CTestIntrusiveMultiMapNode node22(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);
   CTestIntrusiveMultiMapNode node51(5);
   CTestIntrusiveMultiMapNode node52(5);
   CTestIntrusiveMultiMapNode node53(5);
   CTestIntrusiveMultiMapNode node61(6);
   CTestIntrusiveMultiMapNode node62(6);
   CTestIntrusiveMultiMapNode node63(6);
   CTestIntrusiveMultiMapNode node64(6);
   CTestIntrusiveMultiMapNode node71(7);
   CTestIntrusiveMultiMapNode node72(7);
   CTestIntrusiveMultiMapNode node8(8);

   NodeList nodes;

   {
      TestMap map;

      ValidateNodeInsert(node1, map, nodes);
      ValidateNodeInsert(node21, map, nodes);
      ValidateNodeInsert(node22, map, nodes);
      ValidateNodeInsert(node3, map, nodes);
      ValidateNodeInsert(node4, map, nodes);
      ValidateNodeInsert(node51, map, nodes);
      ValidateNodeInsert(node52, map, nodes);
      ValidateNodeInsert(node53, map, nodes);
      ValidateNodeInsert(node61, map, nodes);
      ValidateNodeInsert(node62, map, nodes);
      ValidateNodeInsert(node63, map, nodes);
      ValidateNodeInsert(node64, map, nodes);
      ValidateNodeInsert(node71, map, nodes);
      ValidateNodeInsert(node72, map, nodes);
      ValidateNodeInsert(node8, map, nodes);

      ValidateMap(map);

      THROW_IF_NOT_EQUAL_EX(map.Size(), 15);

      map.Clear(TestMap::ClearFlags::Fast);

      ValidateMap(map);

      THROW_IF_NOT_EQUAL_EX(map.Size(), 0);

      for (auto *pNode : nodes)
      {
         THROW_ON_FAILURE_EX(pNode->IsActive() == false);
      }
   }
}

void CIntrusiveMultiMapTest::TestFastClearWithCallback()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node21(2);
   CTestIntrusiveMultiMapNode node22(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);
   CTestIntrusiveMultiMapNode node51(5);
   CTestIntrusiveMultiMapNode node52(5);
   CTestIntrusiveMultiMapNode node53(5);
   CTestIntrusiveMultiMapNode node61(6);
   CTestIntrusiveMultiMapNode node62(6);
   CTestIntrusiveMultiMapNode node63(6);
   CTestIntrusiveMultiMapNode node64(6);
   CTestIntrusiveMultiMapNode node71(7);
   CTestIntrusiveMultiMapNode node72(7);
   CTestIntrusiveMultiMapNode node8(8);

   NodeSet nodes;

   {
      TestMap map;

      ValidateNodeInsert(node1, map, nodes);
      ValidateNodeInsert(node21, map, nodes);
      ValidateNodeInsert(node22, map, nodes);
      ValidateNodeInsert(node3, map, nodes);
      ValidateNodeInsert(node4, map, nodes);
      ValidateNodeInsert(node51, map, nodes);
      ValidateNodeInsert(node52, map, nodes);
      ValidateNodeInsert(node53, map, nodes);
      ValidateNodeInsert(node61, map, nodes);
      ValidateNodeInsert(node62, map, nodes);
      ValidateNodeInsert(node63, map, nodes);
      ValidateNodeInsert(node64, map, nodes);
      ValidateNodeInsert(node71, map, nodes);
      ValidateNodeInsert(node72, map, nodes);
      ValidateNodeInsert(node8, map, nodes);

      ValidateMap(map);

      THROW_IF_NOT_EQUAL_EX(map.Size(), 15);

      NodeSet clearedNodes;

      map.Clear(
         TestMap::ClearFlags::Fast,
         [&](CTestIntrusiveMultiMapNode *pData) -> void {
         clearedNodes.insert(pData);
         });

      ValidateMap(map);

      THROW_IF_NOT_EQUAL_EX(map.Size(), 0);

      // iteration over "duplicate" keys in a multi-map is not
      // guaranteed to be in insertion order, so we just rely
      // on all the nodes being passed to us

      for (auto *pNode : nodes)
      {
         THROW_ON_FAILURE_EX(pNode->IsActive() == false);

         NodeSet::iterator it = clearedNodes.begin();

         THROW_ON_FAILURE_EX(pNode == *it);

         clearedNodes.erase(it);
      }

      THROW_ON_FAILURE_EX(clearedNodes.empty());
   }
}

void CIntrusiveMultiMapTest::TestFastAndDirtyClear()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node21(2);
   CTestIntrusiveMultiMapNode node22(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);
   CTestIntrusiveMultiMapNode node51(5);
   CTestIntrusiveMultiMapNode node52(5);
   CTestIntrusiveMultiMapNode node53(5);
   CTestIntrusiveMultiMapNode node61(6);
   CTestIntrusiveMultiMapNode node62(6);
   CTestIntrusiveMultiMapNode node63(6);
   CTestIntrusiveMultiMapNode node64(6);
   CTestIntrusiveMultiMapNode node71(7);
   CTestIntrusiveMultiMapNode node72(7);
   CTestIntrusiveMultiMapNode node8(8);

   NodeList nodes;

   {
      TestMap map;

      ValidateNodeInsert(node1, map, nodes);
      ValidateNodeInsert(node21, map, nodes);
      ValidateNodeInsert(node22, map, nodes);
      ValidateNodeInsert(node3, map, nodes);
      ValidateNodeInsert(node4, map, nodes);
      ValidateNodeInsert(node51, map, nodes);
      ValidateNodeInsert(node52, map, nodes);
      ValidateNodeInsert(node53, map, nodes);
      ValidateNodeInsert(node61, map, nodes);
      ValidateNodeInsert(node62, map, nodes);
      ValidateNodeInsert(node63, map, nodes);
      ValidateNodeInsert(node64, map, nodes);
      ValidateNodeInsert(node71, map, nodes);
      ValidateNodeInsert(node72, map, nodes);
      ValidateNodeInsert(node8, map, nodes);

      ValidateMap(map);

      THROW_IF_NOT_EQUAL_EX(map.Size(), 15);

      map.Clear(TestMap::ClearFlags::FastAndDirty);

      ValidateMap(map);

      THROW_IF_NOT_EQUAL_EX(map.Size(), 0);

      for (auto *pNode : nodes)
      {
         THROW_ON_FAILURE_EX(pNode->IsActive() == true);
      }
   }
}

void CIntrusiveMultiMapTest::TestFastAndDirtyClearWithCallback()
{
   CTestIntrusiveMultiMapNode node1(1);
   CTestIntrusiveMultiMapNode node21(2);
   CTestIntrusiveMultiMapNode node22(2);
   CTestIntrusiveMultiMapNode node3(3);
   CTestIntrusiveMultiMapNode node4(4);
   CTestIntrusiveMultiMapNode node51(5);
   CTestIntrusiveMultiMapNode node52(5);
   CTestIntrusiveMultiMapNode node53(5);
   CTestIntrusiveMultiMapNode node61(6);
   CTestIntrusiveMultiMapNode node62(6);
   CTestIntrusiveMultiMapNode node63(6);
   CTestIntrusiveMultiMapNode node64(6);
   CTestIntrusiveMultiMapNode node71(7);
   CTestIntrusiveMultiMapNode node72(7);
   CTestIntrusiveMultiMapNode node8(8);

   NodeSet nodes;

   {
      TestMap map;

      ValidateNodeInsert(node1, map, nodes);
      ValidateNodeInsert(node21, map, nodes);
      ValidateNodeInsert(node22, map, nodes);
      ValidateNodeInsert(node3, map, nodes);
      ValidateNodeInsert(node4, map, nodes);
      ValidateNodeInsert(node51, map, nodes);
      ValidateNodeInsert(node52, map, nodes);
      ValidateNodeInsert(node53, map, nodes);
      ValidateNodeInsert(node61, map, nodes);
      ValidateNodeInsert(node62, map, nodes);
      ValidateNodeInsert(node63, map, nodes);
      ValidateNodeInsert(node64, map, nodes);
      ValidateNodeInsert(node71, map, nodes);
      ValidateNodeInsert(node72, map, nodes);
      ValidateNodeInsert(node8, map, nodes);

      ValidateMap(map);

      THROW_IF_NOT_EQUAL_EX(map.Size(), 15);

      NodeSet clearedNodes;

      map.Clear(
         TestMap::ClearFlags::FastAndDirty,
         [&](CTestIntrusiveMultiMapNode *pData) -> void {
         clearedNodes.insert(pData);
         });

      ValidateMap(map);

      THROW_IF_NOT_EQUAL_EX(map.Size(), 0);

      // iteration over "duplicate" keys in a multi-map is not
      // guaranteed to be in insertion order, so we just rely
      // on all the nodes being passed to us

      for (auto *pNode : nodes)
      {
         THROW_ON_FAILURE_EX(pNode->IsActive() == true);

         NodeSet::iterator it = clearedNodes.begin();

         THROW_ON_FAILURE_EX(pNode == *it);

         clearedNodes.erase(it);
      }

      THROW_ON_FAILURE_EX(clearedNodes.empty());
   }
}

void CIntrusiveMultiMapTest::TestForwardIterate()
{
   typedef std::deque<CTestIntrusiveMultiMapNode *> NodeList;

   NodeList nodeList;

   const int numNodes = 10;

   for (int i = 0; i < numNodes; ++i)
   {
      nodeList.push_back(new CTestIntrusiveMultiMapNode(i));
   }

   {
      TestMap map;

      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
         it != end;
         ++it)
      {
         CTestIntrusiveMultiMapNode *pNode = *it;

         THROW_ON_FAILURE_EX(pNode == *map.Insert(pNode));
      }

      ValidateMap(map);
      THROW_ON_FAILURE_EX(numNodes == map.Size());

      // now run the test...

      TestMap::Iterator it = map.Begin();

      TestMap::Iterator end = map.End();

      THROW_ON_FAILURE_EX(it != end);

      THROW_IF_NOT_EQUAL_EX(0, it->Value());

      TestMap::Iterator it2 = it;

      THROW_IF_NOT_EQUAL_EX(0, it2->Value());

      THROW_IF_NOT_EQUAL_EX(1, (++it)->Value());
      THROW_IF_NOT_EQUAL_EX(1, it->Value());
      THROW_IF_NOT_EQUAL_EX(0, it2->Value());

      THROW_IF_NOT_EQUAL_EX(1, (it++)->Value());
      THROW_IF_NOT_EQUAL_EX(2, it->Value());
      THROW_IF_NOT_EQUAL_EX(0, it2->Value());

      it += 1;

      THROW_IF_NOT_EQUAL_EX(3, it->Value());
      THROW_IF_NOT_EQUAL_EX(0, it2->Value());

      it2 += 4;

      THROW_IF_NOT_EQUAL_EX(3, it->Value());
      THROW_IF_NOT_EQUAL_EX(4, it2->Value());
   }

   for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
      it != end;
      ++it)
   {
      delete *it;
   }
}

void CIntrusiveMultiMapTest::TestReverseIterate()
{
   SKIP_TEST_EX(_T("Need to add -- to iterator"));
}

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static void ValidateNodeInsert(
   CTestIntrusiveMultiMapNode &node,
   TestMap &map)
{
   THROW_ON_FAILURE_EX(&node == *map.Insert(&node));

   ValidateMap(map);
}

static void ValidateNodeInsert(
   CTestIntrusiveMultiMapNode &node,
   TestMap &map,
   NodeList &nodes)
{
   ValidateNodeInsert(node, map);

   nodes.push_back(&node);
}

static void ValidateNodeInsert(
   CTestIntrusiveMultiMapNode &node,
   TestMap &map,
   NodeSet&nodes)
{
   ValidateNodeInsert(node, map);

   nodes.insert(&node);
}

static void RemoveNode(
   const CTestIntrusiveMultiMapNode &node,
   NodeList &nodes)
{
   for (NodeList::iterator it = nodes.begin(), end = nodes.end(); it != end; ++it)
   {
      if (&node == *it)
      {
         nodes.erase(it);

         return;
      }
   }

   throw CException(_T("RemoveNode()"), _T("Failed to find node"));
}

static void ValidateMapContainsNodes(
   const TestMap &map,
   const NodeList &nodes)
{
   NodeList::const_iterator currentNode = nodes.begin();

   for (TestMap::Iterator it = map.Begin(), end = map.End(); it != end; ++it)
   {
      THROW_IF_NOT_EQUAL_EX((*it)->Value(), (*currentNode)->Value());

      ++currentNode;
   }
}

static void RemoveOneAndValidate(
   TestMap &map,
   const int value,
   const CTestIntrusiveMultiMapNode &expectedNode,
   NodeList &nodes)
{
   THROW_ON_FAILURE_EX(&expectedNode == map.RemoveOne(value));

   RemoveNode(expectedNode, nodes);

   ValidateMap(map);

   ValidateMapContainsNodes(map, nodes);
}

static void ValidateMap(
   const TestMap &map)
{
   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_VALIDATION == 1
   map.Validate();
   #else
   (void)map;
   #endif
}


///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveMultiMapTest.cpp
///////////////////////////////////////////////////////////////////////////////

