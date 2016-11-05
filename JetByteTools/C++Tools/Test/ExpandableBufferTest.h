#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_TEST_EXPANDABLE_BUFFER_TEST_INCLUDED__
#define JETBYTE_TOOLS_TEST_EXPANDABLE_BUFFER_TEST_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ExpandableBufferTest.h 
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
// CExpandableBufferTest
///////////////////////////////////////////////////////////////////////////////

/// A test for TExpandableBuffer
/// \test
/// \ingroup CPlusPlusToolsTests

class CExpandableBufferTest
{
   public :

      /// Runs all the tests.

      static void TestAll();

      /// Tests the construction of a TExpandableBuffer of various types.
      
      static void TestConstruct();

      /// Tests assignment.
      
      static void TestAssign();

      /// Tests copy construction.

      static void TestCopyConstruct();

      /// Tests that TExpandableBuffer::Expand() is non destructive.
      
      static void TestExpand();

      /// Tests that TExpandableBuffer::Resize() is destructive.

      static void TestResize();

      /// Tests that TExpandableBuffer::ReleaseBuffer() passes ownership to
      /// the caller.

      static void TestRelease();
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_TEST_EXPANDABLE_BUFFER_TEST_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ExpandableBufferTest.h
///////////////////////////////////////////////////////////////////////////////
