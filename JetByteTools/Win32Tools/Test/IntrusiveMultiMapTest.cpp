///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveMultiMapTest.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "IntrusiveMultiMapTest.h"

#include "JetByteTools\TestTools\TestException.h"
#include "JetByteTools\TestTools\RunTest.h"

#include "JetByteTools\Win32Tools\DebugTrace.h"

#pragma hdrstop

#include "..\IntrusiveMultiMap.h"
#include "..\Mock\TestIntrusiveMultiMapNode.h"

#include <deque>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;
using JetByteTools::Test::TestRequiresVistaOrLater;
using JetByteTools::Test::CTestMonitor;

using JetByteTools::Win32::OutputEx;

using JetByteTools::Win32::Mock::CTestIntrusiveMultiMapNode;
using JetByteTools::Win32::Mock::CTestIntrusiveMultiMapNodeKeyAccessor;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CIntrusiveMultiMapTest
///////////////////////////////////////////////////////////////////////////////

void CIntrusiveMultiMapTest::TestAll(
   CTestMonitor &monitor)
{
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestConstruct);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestInsert);
   //RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestInsertWithExplicitKey);
   //RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestInsertWithIncorrectExplicitKey);
   RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestFind);
   //RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestBigInsertInOrder);
   //RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestBigInsertReverseOrder);
   //RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestDestructDoesNotHarmNodes);
   //RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestRemove);
   //RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestRemoveNodeNotPresent);
   //RUN_TEST_EX(monitor, CIntrusiveMultiMapTest, TestForwardIterate);

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
      typedef TIntrusiveMultiMap<CTestIntrusiveMultiMapNode, int, CTestIntrusiveMultiMapNodeKeyAccessor> Map;

      Map map;

      map.Validate();

      THROW_ON_FAILURE_EX(0 == map.Size());
      THROW_ON_FAILURE_EX(map.Begin() == map.End());

      {
         THROW_ON_FAILURE_EX(&node3 == *map.Insert(&node3));

         map.Validate();

         THROW_ON_FAILURE_EX(1 == map.Size());

         Map::Iterator it = map.Begin();

         THROW_ON_FAILURE_EX(it != map.End());
         THROW_ON_FAILURE_EX(*it == &node3);
         ++it;
         THROW_ON_FAILURE_EX(it == map.End());
      }

      {
         THROW_ON_FAILURE_EX(&node1 == *map.Insert(&node1));

         map.Validate();

         THROW_ON_FAILURE_EX(2 == map.Size());

         Map::Iterator it = map.Begin();

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

         map.Validate();

         THROW_ON_FAILURE_EX(3 == map.Size());

         Map::Iterator it = map.Begin();

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

         map.Validate();

         THROW_ON_FAILURE_EX(4 == map.Size());

         Map::Iterator it = map.Begin();

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

         map.Validate();

         THROW_ON_FAILURE_EX(5 == map.Size());

         Map::Iterator it = map.Begin();

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

         map.Validate();

         THROW_ON_FAILURE_EX(6 == map.Size());

         Map::Iterator it = map.Begin();

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

         map.Validate();

         THROW_ON_FAILURE_EX(7 == map.Size());

         Map::Iterator it = map.Begin();

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

//void CIntrusiveMultiMapTest::TestInsertWithExplicitKey()
//{
//   CTestIntrusiveMultiMapNode node1(1);
//   CTestIntrusiveMultiMapNode node2(2);
//   CTestIntrusiveMultiMapNode node3(3);
//   CTestIntrusiveMultiMapNode node4(4);
//   CTestIntrusiveMultiMapNode node5(5);
//   CTestIntrusiveMultiMapNode node6(6);
//   CTestIntrusiveMultiMapNode node7(7);
//
//   {
//      typedef TIntrusiveMultiMap<CTestIntrusiveMultiMapNode, int, CTestIntrusiveMultiMapNodeKeyAccessor> Map;
//
//      Map map;
//
//      map.Validate();
//
//      THROW_ON_FAILURE_EX(0 == map.Size());
//      THROW_ON_FAILURE_EX(map.Begin() == map.End());
//
//
//      {
//         THROW_ON_FAILURE_EX(true == map.Insert(&node3, 3).second);
//
//         map.Validate();
//
//         THROW_ON_FAILURE_EX(1 == map.Size());
//
//         Map::Iterator it = map.Begin();
//
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node3);
//         THROW_IF_NOT_EQUAL_EX(3, it->Value());
//         THROW_IF_NOT_EQUAL_EX(3, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it == map.End());
//      }
//
//      {
//         THROW_ON_FAILURE_EX(true == map.Insert(&node1, 1).second);
//
//         map.Validate();
//
//         THROW_ON_FAILURE_EX(2 == map.Size());
//
//         Map::Iterator it = map.Begin();
//
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node1);
//         THROW_IF_NOT_EQUAL_EX(1, it->Value());
//         THROW_IF_NOT_EQUAL_EX(1, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node3);
//         THROW_IF_NOT_EQUAL_EX(3, it->Value());
//         THROW_IF_NOT_EQUAL_EX(3, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it == map.End());
//      }
//
//      {
//         THROW_ON_FAILURE_EX(true == map.Insert(&node4, 4).second);
//
//         map.Validate();
//
//         THROW_ON_FAILURE_EX(3 == map.Size());
//
//         Map::Iterator it = map.Begin();
//
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node1);
//         THROW_IF_NOT_EQUAL_EX(1, it->Value());
//         THROW_IF_NOT_EQUAL_EX(1, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node3);
//         THROW_IF_NOT_EQUAL_EX(3, it->Value());
//         THROW_IF_NOT_EQUAL_EX(3, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node4);
//         THROW_IF_NOT_EQUAL_EX(4, it->Value());
//         THROW_IF_NOT_EQUAL_EX(4, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it == map.End());
//      }
//
//      {
//         THROW_ON_FAILURE_EX(true == map.Insert(&node2, 2).second);
//
//         map.Validate();
//
//         THROW_ON_FAILURE_EX(4 == map.Size());
//
//         Map::Iterator it = map.Begin();
//
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node1);
//         THROW_IF_NOT_EQUAL_EX(1, it->Value());
//         THROW_IF_NOT_EQUAL_EX(1, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node2);
//         THROW_IF_NOT_EQUAL_EX(2, it->Value());
//         THROW_IF_NOT_EQUAL_EX(2, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node3);
//         THROW_IF_NOT_EQUAL_EX(3, it->Value());
//         THROW_IF_NOT_EQUAL_EX(3, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node4);
//         THROW_IF_NOT_EQUAL_EX(4, it->Value());
//         THROW_IF_NOT_EQUAL_EX(4, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it == map.End());
//      }
//
//      {
//         THROW_ON_FAILURE_EX(true == map.Insert(&node6, 6).second);
//
//         map.Validate();
//
//         THROW_ON_FAILURE_EX(5 == map.Size());
//
//         Map::Iterator it = map.Begin();
//
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node1);
//         THROW_IF_NOT_EQUAL_EX(1, it->Value());
//         THROW_IF_NOT_EQUAL_EX(1, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node2);
//         THROW_IF_NOT_EQUAL_EX(2, it->Value());
//         THROW_IF_NOT_EQUAL_EX(2, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node3);
//         THROW_IF_NOT_EQUAL_EX(3, it->Value());
//         THROW_IF_NOT_EQUAL_EX(3, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node4);
//         THROW_IF_NOT_EQUAL_EX(4, it->Value());
//         THROW_IF_NOT_EQUAL_EX(4, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node6);
//         THROW_IF_NOT_EQUAL_EX(6, it->Value());
//         THROW_IF_NOT_EQUAL_EX(6, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it == map.End());
//      }
//
//      {
//         THROW_ON_FAILURE_EX(true == map.Insert(&node5, 5).second);
//
//         map.Validate();
//
//         THROW_ON_FAILURE_EX(6 == map.Size());
//
//         Map::Iterator it = map.Begin();
//
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node1);
//         THROW_IF_NOT_EQUAL_EX(1, it->Value());
//         THROW_IF_NOT_EQUAL_EX(1, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node2);
//         THROW_IF_NOT_EQUAL_EX(2, it->Value());
//         THROW_IF_NOT_EQUAL_EX(2, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node3);
//         THROW_IF_NOT_EQUAL_EX(3, it->Value());
//         THROW_IF_NOT_EQUAL_EX(3, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node4);
//         THROW_IF_NOT_EQUAL_EX(4, it->Value());
//         THROW_IF_NOT_EQUAL_EX(4, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node5);
//         THROW_IF_NOT_EQUAL_EX(5, it->Value());
//         THROW_IF_NOT_EQUAL_EX(5, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node6);
//         THROW_IF_NOT_EQUAL_EX(6, it->Value());
//         THROW_IF_NOT_EQUAL_EX(6, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it == map.End());
//      }
//
//      {
//         THROW_ON_FAILURE_EX(true == map.Insert(&node7, 7).second);
//
//         map.Validate();
//
//         THROW_ON_FAILURE_EX(7 == map.Size());
//
//         Map::Iterator it = map.Begin();
//
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node1);
//         THROW_IF_NOT_EQUAL_EX(1, it->Value());
//         THROW_IF_NOT_EQUAL_EX(1, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node2);
//         THROW_IF_NOT_EQUAL_EX(2, it->Value());
//         THROW_IF_NOT_EQUAL_EX(2, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node3);
//         THROW_IF_NOT_EQUAL_EX(3, it->Value());
//         THROW_IF_NOT_EQUAL_EX(3, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node4);
//         THROW_IF_NOT_EQUAL_EX(4, it->Value());
//         THROW_IF_NOT_EQUAL_EX(4, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node5);
//         THROW_IF_NOT_EQUAL_EX(5, it->Value());
//         THROW_IF_NOT_EQUAL_EX(5, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node6);
//         THROW_IF_NOT_EQUAL_EX(6, it->Value());
//         THROW_IF_NOT_EQUAL_EX(6, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node7);
//         THROW_IF_NOT_EQUAL_EX(7, it->Value());
//         THROW_IF_NOT_EQUAL_EX(7, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it == map.End());
//      }
//   }
//}
//
//void CIntrusiveMultiMapTest::TestInsertWithIncorrectExplicitKey()
//{
//   CTestIntrusiveMultiMapNode node1(1);
//   CTestIntrusiveMultiMapNode node2(2);
//   CTestIntrusiveMultiMapNode node3(3);
//   CTestIntrusiveMultiMapNode node4(4);
//
//   {
//      typedef TIntrusiveMultiMap<CTestIntrusiveMultiMapNode, int, CTestIntrusiveMultiMapNodeKeyAccessor> Map;
//
//      Map map;
//
//      map.Validate();
//
//      THROW_ON_FAILURE_EX(0 == map.Size());
//      THROW_ON_FAILURE_EX(map.Begin() == map.End());
//
//
//      {
//         THROW_ON_FAILURE_EX(true == map.Insert(&node3, 3).second);
//
//         map.Validate();
//
//         THROW_ON_FAILURE_EX(1 == map.Size());
//
//         Map::Iterator it = map.Begin();
//
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node3);
//         THROW_IF_NOT_EQUAL_EX(3, it->Value());
//         THROW_IF_NOT_EQUAL_EX(3, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it == map.End());
//      }
//
//      {
//         THROW_ON_FAILURE_EX(true == map.Insert(&node1, 1).second);
//
//         map.Validate();
//
//         THROW_ON_FAILURE_EX(2 == map.Size());
//
//         Map::Iterator it = map.Begin();
//
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node1);
//         THROW_IF_NOT_EQUAL_EX(1, it->Value());
//         THROW_IF_NOT_EQUAL_EX(1, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node3);
//         THROW_IF_NOT_EQUAL_EX(3, it->Value());
//         THROW_IF_NOT_EQUAL_EX(3, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it == map.End());
//      }
//
//      // This key is wrong, it's not the same value that 'extracting the key from the node'
//      // will give...
//
//      {
//         THROW_ON_FAILURE_EX(true == map.Insert(&node4, 2).second);
//
//         // Map is no longer valid...
//
//         THROW_ON_NO_EXCEPTION_EX(map.Validate);
//
//         THROW_ON_FAILURE_EX(3 == map.Size());
//
//         Map::Iterator it = map.Begin();
//
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node1);
//         THROW_IF_NOT_EQUAL_EX(1, it->Value());
//         THROW_IF_NOT_EQUAL_EX(1, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node4);
//         THROW_IF_NOT_EQUAL_EX(4, it->Value());
//         THROW_IF_NOT_EQUAL_EX(4, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it != map.End());
//         THROW_ON_FAILURE_EX(*it == &node3);
//         THROW_IF_NOT_EQUAL_EX(3, it->Value());
//         THROW_IF_NOT_EQUAL_EX(3, it->Key());
//         ++it;
//         THROW_ON_FAILURE_EX(it == map.End());
//      }
//   }
//}

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
      typedef TIntrusiveMultiMap<CTestIntrusiveMultiMapNode, int, CTestIntrusiveMultiMapNodeKeyAccessor> Map;

      Map map;

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

      Map::Iterator it = map.Find(2);

      THROW_ON_FAILURE_EX(&node2 == *it);
      it++;
      THROW_ON_FAILURE_EX(&node5 == *it);
      it++;
      THROW_ON_FAILURE_EX(&node3 == *it);

      THROW_ON_FAILURE_EX(&node3 == *map.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));

      THROW_ON_FAILURE_EX(&node6 == *map.Insert(&node6));          // value is 2

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));

      it = map.Find(2);

      THROW_ON_FAILURE_EX(&node2 == *it);
      it++;
      THROW_ON_FAILURE_EX(&node6 == *it);
      it++;
      THROW_ON_FAILURE_EX(&node5 == *it);
      it++;
      THROW_ON_FAILURE_EX(&node3 == *it);

      THROW_ON_FAILURE_EX(&node3 == *map.Find(3));
      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));

      THROW_ON_FAILURE_EX(&node7 == *map.Insert(&node7));          // value is 3

      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));

      it = map.Find(2);

      THROW_ON_FAILURE_EX(&node2 == *it);
      it++;
      THROW_ON_FAILURE_EX(&node6 == *it);
      it++;
      THROW_ON_FAILURE_EX(&node5 == *it);
      it++;
      THROW_ON_FAILURE_EX(&node3 == *it);

      it = map.Find(3);

      THROW_ON_FAILURE_EX(&node3 == *it);
      it++;
      THROW_ON_FAILURE_EX(&node7 == *it);
      it++;
      THROW_ON_FAILURE_EX(&node4 == *it);

      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));
   }
}
//
//void CIntrusiveMultiMapTest::TestBigInsertInOrder()
//{
//   typedef TIntrusiveMultiMap<CTestIntrusiveMultiMapNode, int, CTestIntrusiveMultiMapNodeKeyAccessor> Map;
//
//   typedef std::deque<CTestIntrusiveMultiMapNode *> NodeList;
//
//   NodeList nodeList;
//
//   for (size_t i = 0; i < 100; ++i)
//   {
//      nodeList.push_back(new CTestIntrusiveMultiMapNode(i));
//   }
//
//   {
//      Map map;
//
//      size_t size = 0;
//
//      {
//         Map map;
//         size_t i = 0;
//
//         NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
//
//         for (; 
//            it != end && i < 7;
//            ++it, ++i)
//         {
//            CTestIntrusiveMultiMapNode *pNode = *it;
//            THROW_ON_FAILURE_EX(true == map.Insert(pNode).second);
//
//            map.Validate();
//         }
//
//         CTestIntrusiveMultiMapNode *pNode = *it;
//         THROW_ON_FAILURE_EX(true == map.Insert(pNode).second);
//
//         map.Validate();
//      }
//
//      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end(); 
//         it != end;
//         ++it)
//      {
//         map.Validate();
//
//         THROW_ON_FAILURE_EX(size == map.Size());
//
//         CTestIntrusiveMultiMapNode *pNode = *it;
//
//         THROW_ON_FAILURE_EX(true == map.Insert(pNode).second);
//
//         size++;
//      }
//
//      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end(); 
//         it != end;
//         ++it)
//      {
//         CTestIntrusiveMultiMapNode *pNode = *it;
//
//         THROW_ON_FAILURE_EX(pNode == *map.Find(pNode->Value()));
//      }
//
//      int i = 0;
//
//      for (Map::Iterator it = map.Begin(), end = map.End(); it != end; ++it, ++i)
//      {
//         THROW_IF_NOT_EQUAL_EX((*it)->Value(), i);
//      }
//   }
//
//   for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end(); 
//      it != end;
//      ++it)
//   {
//      delete *it;
//   }
//}
//
//void CIntrusiveMultiMapTest::TestBigInsertReverseOrder()
//{
//   typedef TIntrusiveMultiMap<CTestIntrusiveMultiMapNode, int, CTestIntrusiveMultiMapNodeKeyAccessor> Map;
//
//   typedef std::deque<CTestIntrusiveMultiMapNode *> NodeList;
//
//   NodeList nodeList;
//
//   for (size_t i = 0; i < 100; ++i)
//   {
//      nodeList.push_front(new CTestIntrusiveMultiMapNode(i));
//   }
//
//   {
//      Map map;
//
//      size_t size = 0;
//
//      {
//         Map map;
//         size_t i = 0;
//
//         NodeList::const_iterator it = nodeList.begin(), end = nodeList.end();
//
//         for (; 
//            it != end && i < 7;
//            ++it, ++i)
//         {
//            CTestIntrusiveMultiMapNode *pNode = *it;
//            THROW_ON_FAILURE_EX(true == map.Insert(pNode).second);
//
//            map.Validate();
//         }
//
//         CTestIntrusiveMultiMapNode *pNode = *it;
//         THROW_ON_FAILURE_EX(true == map.Insert(pNode).second);
//
//         map.Validate();
//      }
//
//      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end(); 
//         it != end;
//         ++it)
//      {
//         map.Validate();
//
//         THROW_ON_FAILURE_EX(size == map.Size());
//
//         CTestIntrusiveMultiMapNode *pNode = *it;
//
//         THROW_ON_FAILURE_EX(true == map.Insert(pNode).second);
//
//         size++;
//      }
//
//      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end(); 
//         it != end;
//         ++it)
//      {
//         CTestIntrusiveMultiMapNode *pNode = *it;
//
//         THROW_ON_FAILURE_EX(pNode == *map.Find(pNode->Value()));
//      }
//
//      int i = 0;
//
//      for (Map::Iterator it = map.Begin(), end = map.End(); it != end; ++it, ++i)
//      {
//         THROW_IF_NOT_EQUAL_EX((*it)->Value(), i);
//      }
//   }
//
//   for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end(); 
//      it != end;
//      ++it)
//   {
//      delete *it;
//   }
//}
//
//void CIntrusiveMultiMapTest::TestDestructDoesNotHarmNodes()
//{
//   CTestIntrusiveMultiMapNode node1(1);
//   CTestIntrusiveMultiMapNode node2(2);
//   CTestIntrusiveMultiMapNode node3(3);
//
//   {
//      typedef TIntrusiveMultiMap<CTestIntrusiveMultiMapNode, int, CTestIntrusiveMultiMapNodeKeyAccessor> Map;
//
//      Map map;
//
//      THROW_ON_FAILURE_EX(true == map.Insert(&node1).second);
//      THROW_ON_FAILURE_EX(true == map.Insert(&node2).second);
//      THROW_ON_FAILURE_EX(true == map.Insert(&node3).second);
//   }
//
//   THROW_ON_FAILURE_EX(node1.Value() == 1);
//   THROW_ON_FAILURE_EX(node2.Value() == 2);
//   THROW_ON_FAILURE_EX(node3.Value() == 3);
//}
//
//void CIntrusiveMultiMapTest::TestRemove()
//{
//   CTestIntrusiveMultiMapNode node1(1);
//   CTestIntrusiveMultiMapNode node2(2);
//   CTestIntrusiveMultiMapNode node3(3);
//   CTestIntrusiveMultiMapNode node4(4);
//   CTestIntrusiveMultiMapNode node5(5);
//   CTestIntrusiveMultiMapNode node6(6);
//   CTestIntrusiveMultiMapNode node7(7);
//   CTestIntrusiveMultiMapNode node8(8);
//
//   {
//      typedef TIntrusiveMultiMap<CTestIntrusiveMultiMapNode, int, CTestIntrusiveMultiMapNodeKeyAccessor> Map;
//
//      Map map;
//
//      THROW_ON_FAILURE_EX(true == map.Insert(&node1).second);
//      THROW_ON_FAILURE_EX(true == map.Insert(&node2).second);
//      THROW_ON_FAILURE_EX(true == map.Insert(&node3).second);
//      THROW_ON_FAILURE_EX(true == map.Insert(&node4).second);
//      THROW_ON_FAILURE_EX(true == map.Insert(&node5).second);
//      THROW_ON_FAILURE_EX(true == map.Insert(&node6).second);
//      THROW_ON_FAILURE_EX(true == map.Insert(&node7).second);
//      THROW_ON_FAILURE_EX(true == map.Insert(&node8).second);
//
//      map.Validate();
//
//      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
//      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
//      THROW_ON_FAILURE_EX(&node2 == *map.Find(2));
//      THROW_ON_FAILURE_EX(&node3 == *map.Find(3));
//      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));
//      THROW_ON_FAILURE_EX(&node5 == *map.Find(5));
//      THROW_ON_FAILURE_EX(&node6 == *map.Find(6));
//      THROW_ON_FAILURE_EX(&node7 == *map.Find(7));
//      THROW_ON_FAILURE_EX(&node8 == *map.Find(8));
//      THROW_ON_FAILURE_EX(map.End() == map.Find(9));
//
//      THROW_ON_FAILURE_EX(&node3 == map.Remove(3));
//
//      map.Validate();
//
//      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
//      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
//      THROW_ON_FAILURE_EX(&node2 == *map.Find(2));
//      THROW_ON_FAILURE_EX(map.End() == map.Find(3));
//      THROW_ON_FAILURE_EX(&node4 == *map.Find(4));
//      THROW_ON_FAILURE_EX(&node5 == *map.Find(5));
//      THROW_ON_FAILURE_EX(&node6 == *map.Find(6));
//      THROW_ON_FAILURE_EX(&node7 == *map.Find(7));
//      THROW_ON_FAILURE_EX(&node8 == *map.Find(8));
//      THROW_ON_FAILURE_EX(map.End() == map.Find(9));
//
//      THROW_ON_FAILURE_EX(&node4 == map.Remove(4));
//
//      map.Validate();
//
//      THROW_ON_FAILURE_EX(map.End() == map.Find(0));
//      THROW_ON_FAILURE_EX(&node1 == *map.Find(1));
//      THROW_ON_FAILURE_EX(&node2 == *map.Find(2));
//      THROW_ON_FAILURE_EX(map.End() == map.Find(3));
//      THROW_ON_FAILURE_EX(map.End() == map.Find(4));
//      THROW_ON_FAILURE_EX(&node5 == *map.Find(5));
//      THROW_ON_FAILURE_EX(&node6 == *map.Find(6));
//      THROW_ON_FAILURE_EX(&node7 == *map.Find(7));
//      THROW_ON_FAILURE_EX(&node8 == *map.Find(8));
//      THROW_ON_FAILURE_EX(map.End() == map.Find(9));
//
//   }
//}
//
//void CIntrusiveMultiMapTest::TestRemoveNodeNotPresent()
//{
//   CTestIntrusiveMultiMapNode node1(1);
//   CTestIntrusiveMultiMapNode node2(2);
//   CTestIntrusiveMultiMapNode node3(3);
//   CTestIntrusiveMultiMapNode node4(4);
//
//   {
//      typedef TIntrusiveMultiMap<CTestIntrusiveMultiMapNode, int, CTestIntrusiveMultiMapNodeKeyAccessor> Map;
//
//      Map map;
//
//      THROW_ON_FAILURE_EX(true == map.Insert(&node1).second);
//      THROW_ON_FAILURE_EX(true == map.Insert(&node2).second);
//      THROW_ON_FAILURE_EX(true == map.Insert(&node3).second);
//      THROW_ON_FAILURE_EX(true == map.Insert(&node4).second);
//
//      map.Validate();
//
//      THROW_ON_FAILURE_EX(0 == map.Remove(0));
//      THROW_ON_FAILURE_EX(0 == map.Remove(5));
//      THROW_ON_FAILURE_EX(0 == map.Remove(100));
//
//   }
//}
//
//void CIntrusiveMultiMapTest::TestForwardIterate()
//{
//   typedef TIntrusiveMultiMap<CTestIntrusiveMultiMapNode, int, CTestIntrusiveMultiMapNodeKeyAccessor> Map;
//
//   typedef std::deque<CTestIntrusiveMultiMapNode *> NodeList;
//
//   NodeList nodeList;
//
//   const size_t numNodes = 10;
//
//   for (size_t i = 0; i < numNodes; ++i)
//   {
//      nodeList.push_back(new CTestIntrusiveMultiMapNode(i));
//   }
//
//   {
//      Map map;
//
//      for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end(); 
//         it != end;
//         ++it)
//      {
//         CTestIntrusiveMultiMapNode *pNode = *it;
//
//         THROW_ON_FAILURE_EX(true == map.Insert(pNode).second);
//      }
//
//      map.Validate();
//      THROW_ON_FAILURE_EX(numNodes == map.Size());
//
//      // now run the test...
//
//      Map::Iterator it = map.Begin();
//
//      Map::Iterator end = map.End();
//
//      THROW_ON_FAILURE_EX(it != end);
//
//      THROW_IF_NOT_EQUAL_EX(0, it->Value());
//
//      Map::Iterator it2 = it;
//
//      THROW_IF_NOT_EQUAL_EX(0, it2->Value());
//
//      THROW_IF_NOT_EQUAL_EX(1, (++it)->Value());
//      THROW_IF_NOT_EQUAL_EX(1, it->Value());
//      THROW_IF_NOT_EQUAL_EX(0, it2->Value());
//
//      THROW_IF_NOT_EQUAL_EX(1, (it++)->Value());
//      THROW_IF_NOT_EQUAL_EX(2, it->Value());
//      THROW_IF_NOT_EQUAL_EX(0, it2->Value());
//
//      it += 1;
//
//      THROW_IF_NOT_EQUAL_EX(3, it->Value());
//      THROW_IF_NOT_EQUAL_EX(0, it2->Value());
//
//      it2 += 4;
//
//      THROW_IF_NOT_EQUAL_EX(3, it->Value());
//      THROW_IF_NOT_EQUAL_EX(4, it2->Value());
//   }
//
//   for (NodeList::const_iterator it = nodeList.begin(), end = nodeList.end(); 
//      it != end;
//      ++it)
//   {
//      delete *it;
//   }
//}

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
// End of file: IntrusiveMultiMapTest.cpp
///////////////////////////////////////////////////////////////////////////////

