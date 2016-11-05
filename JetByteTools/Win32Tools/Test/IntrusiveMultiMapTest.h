#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_TEST_INTRUSIVE_MULTI_MAP_TEST_INCLUDED__
#define JETBYTE_TOOLS_WIN32_TEST_INTRUSIVE_MULTI_MAP_TEST_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveMultiMapTest.h 
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

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools 
{
   namespace Test
   {
      class CTestMonitor;
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CIntrusiveMultiMapTest
///////////////////////////////////////////////////////////////////////////////

/// \test
/// \ingroup Win32ToolsTests

class CIntrusiveMultiMapTest
{
   public :

      static void TestAll(
         JetByteTools::Test::CTestMonitor &monitor);

      static void TestConstruct();
      static void TestInsert();
      static void TestInsertWithExplicitKey();
      static void TestInsertWithIncorrectExplicitKey();
      static void TestInsertDuplicate();
      static void TestFind();
      static void TestBigInsertInOrder();
      static void TestBigInsertReverseOrder();
      static void TestDestructDoesNotHarmNodes();
      static void TestRemove();
      static void TestRemoveNodeNotPresent();
      static void TestForwardIterate();
      static void TestReverseIterate();
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_WIN32_TEST_INTRUSIVE_MULTI_MAP_TEST_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveMultiMapTest.h
///////////////////////////////////////////////////////////////////////////////
