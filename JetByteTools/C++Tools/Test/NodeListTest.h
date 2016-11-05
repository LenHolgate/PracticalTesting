#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_TEST_NODE_LIST_TEST_INCLUDED__
#define JETBYTE_TOOLS_TEST_NODE_LIST_TEST_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: NodeListTest.h 
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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CNodeListTest
///////////////////////////////////////////////////////////////////////////////

/// A test for CNodeList
/// \test
/// \ingroup CPlusPlusToolsTests

class CNodeListTest
{
   public :

      /// Runs all the tests.
      
      static void TestAll();

      /// Tests the construction of a CNodeList
      
      static void TestConstruct();

      /// Tests CNodeList::AddNode()

      static void TestAddNode();
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_TEST_NODE_LIST_TEST_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: NodeListTest.h
///////////////////////////////////////////////////////////////////////////////
