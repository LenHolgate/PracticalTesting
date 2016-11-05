///////////////////////////////////////////////////////////////////////////////
// File: NodeListTest.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
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

#include "NodeListTest.h"

#include "..\NodeList.h"

#include "..\Mock\MockNodeListNode.h"

#include "JetByteTools\Win32Tools\Utils.h"
#include "JetByteTools\Win32Tools\DebugTrace.h"

#include "JetByteTools\TestTools\TestException.h"

#include <string>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;

using JetByteTools::Win32::Output;
using JetByteTools::Win32::_tstring;

using JetByteTools::Mock::CMockNodeListNode;

using std::string;

//////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CNodeListTest
///////////////////////////////////////////////////////////////////////////////

void CNodeListTest::TestAll()
{
   TestConstruct();
   TestAddNode();
}

void CNodeListTest::TestConstruct()
{
   const _tstring functionName = _T("CNodeListTest::TestConstruct");
   
   Output(functionName + _T(" - start"));

   CNodeList list;

   THROW_ON_FAILURE(functionName, 0 == list.Count());

   THROW_ON_FAILURE(functionName, true == list.IsEmpty());

   THROW_ON_FAILURE(functionName, 0 == list.Head());

   THROW_ON_FAILURE(functionName, 0 == list.PopNode());


   Output(functionName + _T(" - stop"));
}

void CNodeListTest::TestAddNode()
{
   const _tstring functionName = _T("CNodeListTest::TestAddNode");
   
   Output(functionName + _T(" - start"));

   CNodeList list;

   CMockNodeListNode node;

   list.PushNode(&node);

   THROW_ON_FAILURE(functionName, 1 == list.Count());

   THROW_ON_FAILURE(functionName, false == list.IsEmpty());

   THROW_ON_FAILURE(functionName, &node == list.Head());

   THROW_ON_FAILURE(functionName, true == node.InList());

   THROW_ON_FAILURE(functionName, true == node.InList(list));

   CNodeList list2;

   THROW_ON_FAILURE(functionName, false == node.InList(list2));

   THROW_ON_FAILURE(functionName, 0 == node.Next());

   THROW_ON_FAILURE(functionName, &node == list.PopNode());

   THROW_ON_FAILURE(functionName, 0 == list.Count());

   THROW_ON_FAILURE(functionName, true == list.IsEmpty());

   THROW_ON_FAILURE(functionName, 0 == list.Head());

   THROW_ON_FAILURE(functionName, 0 == list.PopNode());

   Output(functionName + _T(" - stop"));
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: NodeListTest.cpp
///////////////////////////////////////////////////////////////////////////////

