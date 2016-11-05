#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_TEST_REFERENCE_COUNTED_SMART_POINTER_TEST_INCLUDED__
#define JETBYTE_TOOLS_TEST_REFERENCE_COUNTED_SMART_POINTER_TEST_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ReferenceCountedSmartPointerTest.h 
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2005 JetByte Limited.
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
// CReferenceCountedSmartPointerTest
///////////////////////////////////////////////////////////////////////////////

/// A test for TReferenceCountedSmartPointer
/// \test
/// \ingroup CPlusPlusToolsTests

class CReferenceCountedSmartPointerTest
{
   public :

      /// Runs all the tests.

      static void TestAll();

      /// Tests the construction of a TReferenceCountedSmartPointer

      static void TestConstruct();

      /// Tests that destruction releases a managed object.

      static void TestDestruct();

      /// Tests assigning a bare pointer to a smart pointer passes
      /// ownership to the TReferenceCountedSmartPointer without
      /// incurring an AddRef() call.

      static void TestAssignPointer();

      /// Tests assigning a TReferenceCountedSmartPointer to another
      /// TReferenceCountedSmartPointer

      static void TestAssignSmartPointer();

      /// Tests assigning a TReferenceCountedSmartPointer to itself.

      static void TestAssignSmartPointerToSelf();

      /// Tests copy constructing a TReferenceCountedSmartPointer

      static void TestCopyConstruct();

      /// Tests that calling TReferenceCountedSmartPointer::Detach()
      /// passes ownership of the managed pointer to the caller.

      static void TestDetach();
      
      /// Tests that calling TReferenceCountedSmartPointer::Get() returns
      /// the managed pointer without affecting the reference count.
      
      static void TestGet();

      /// Tests that the 'arrow' operator allows access to the underlying
      /// pointer's methods.

      static void TestOperatorArrow();
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_TEST_REFERENCE_COUNTED_SMART_POINTER_TEST_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ReferenceCountedSmartPointerTest.h
///////////////////////////////////////////////////////////////////////////////
