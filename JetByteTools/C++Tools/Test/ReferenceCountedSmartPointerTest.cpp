///////////////////////////////////////////////////////////////////////////////
// File: ReferenceCountedSmartPointerTest.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "ReferenceCountedSmartPointerTest.h"

#include "..\ReferenceCountedSmartPointer.h"

#include "..\Mock\MockReferenceCounted.h"

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

using JetByteTools::Mock::CMockReferenceCounted;

//////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CReferenceCountedSmartPointerTest
///////////////////////////////////////////////////////////////////////////////

void CReferenceCountedSmartPointerTest::TestAll()
{
   TestConstruct();
   TestDestruct();
   TestAssignPointer();
   TestAssignSmartPointer();
   TestAssignSmartPointerToSelf();
   TestCopyConstruct();
   TestDetach();
   TestGet();
   TestOperatorArrow();
}

void CReferenceCountedSmartPointerTest::TestConstruct()
{
   const _tstring functionName = _T("CReferenceCountedSmartPointerTest::TestConstruct");
   
   Output(functionName + _T(" - start"));

   TReferenceCountedSmartPointer<CMockReferenceCounted> pointer1;

   CMockReferenceCounted object;

   TReferenceCountedSmartPointer<CMockReferenceCounted> pointer2(&object);

   object.CheckNoResults();

   Output(functionName + _T(" - stop"));
}

void CReferenceCountedSmartPointerTest::TestDestruct()
{
   const _tstring functionName = _T("CReferenceCountedSmartPointerTest::TestDestruct");
   
   Output(functionName + _T(" - start"));

   CMockReferenceCounted object;

   {
      TReferenceCountedSmartPointer<CMockReferenceCounted> pointer(&object);
   }

   object.CheckResult(_T("|Release: 0|"));

   Output(functionName + _T(" - stop"));
}

void CReferenceCountedSmartPointerTest::TestAssignPointer()
{
   const _tstring functionName = _T("CReferenceCountedSmartPointerTest::TestAssignPointer");
   
   Output(functionName + _T(" - start"));

   CMockReferenceCounted object1;

   {
      CMockReferenceCounted object2;

      TReferenceCountedSmartPointer<CMockReferenceCounted> pointer(&object2);

      pointer = &object1;

      object2.CheckResult(_T("|Release: 0|"));
      object1.CheckResult(_T("|AddRef: 2|Release: 1|"));
   }

   object1.CheckResult(_T("|Release: 0|"));

   Output(functionName + _T(" - stop"));
}

void CReferenceCountedSmartPointerTest::TestAssignSmartPointer()
{
   const _tstring functionName = _T("CReferenceCountedSmartPointerTest::TestAssignSmartPointer");
   
   Output(functionName + _T(" - start"));

   CMockReferenceCounted object1;

   {
      CMockReferenceCounted object2;

      TReferenceCountedSmartPointer<CMockReferenceCounted> pointer1(&object2);

      {
         TReferenceCountedSmartPointer<CMockReferenceCounted> pointer2(&object1);

         pointer1 = pointer2;

         object2.CheckResult(_T("|Release: 0|"));
         object1.CheckResult(_T("|AddRef: 2|AddRef: 3|Release: 2|"));
      }

      object1.CheckResult(_T("|Release: 1|"));
   }

   object1.CheckResult(_T("|Release: 0|"));

   Output(functionName + _T(" - stop"));
}

void CReferenceCountedSmartPointerTest::TestAssignSmartPointerToSelf()
{
   const _tstring functionName = _T("CReferenceCountedSmartPointerTest::TestAssignSmartPointerToSelf");
   
   Output(functionName + _T(" - start"));

   CMockReferenceCounted object;

   {
      TReferenceCountedSmartPointer<CMockReferenceCounted> pointer(&object);

      pointer = pointer;

      object.CheckResult(_T("|AddRef: 2|Release: 1|AddRef: 2|Release: 1|"));
   }

   object.CheckResult(_T("|Release: 0|"));

   Output(functionName + _T(" - stop"));
}

void CReferenceCountedSmartPointerTest::TestCopyConstruct()
{
   const _tstring functionName = _T("CReferenceCountedSmartPointerTest::TestCopyConstruct");
   
   Output(functionName + _T(" - start"));

   CMockReferenceCounted object;

   {
      TReferenceCountedSmartPointer<CMockReferenceCounted> pointer1(&object);

      {
         TReferenceCountedSmartPointer<CMockReferenceCounted> pointer2(pointer1);

         object.CheckResult(_T("|AddRef: 2|"));
      }

      object.CheckResult(_T("|Release: 1|"));
   }

   object.CheckResult(_T("|Release: 0|"));

   Output(functionName + _T(" - stop"));
}

void CReferenceCountedSmartPointerTest::TestDetach()
{
   const _tstring functionName = _T("CReferenceCountedSmartPointerTest::TestDetach");
   
   Output(functionName + _T(" - start"));

   {
      TReferenceCountedSmartPointer<CMockReferenceCounted> pointer;

      THROW_ON_FAILURE(functionName, 0 == pointer.Detach());

      THROW_ON_FAILURE(functionName, 0 == pointer.Get());
   }

   CMockReferenceCounted object;

   {
      TReferenceCountedSmartPointer<CMockReferenceCounted> pointer(&object);

      THROW_ON_FAILURE(functionName, &object == pointer.Detach());

      object.CheckNoResults();

      THROW_ON_FAILURE(functionName, 0 == pointer.Get());
   }

   object.CheckNoResults();

   Output(functionName + _T(" - stop"));
}

void CReferenceCountedSmartPointerTest::TestGet()
{
   const _tstring functionName = _T("CReferenceCountedSmartPointerTest::TestGet");
   
   Output(functionName + _T(" - start"));

   {
      TReferenceCountedSmartPointer<CMockReferenceCounted> pointer;

      THROW_ON_FAILURE(functionName, 0 == pointer.Get());
   }


   CMockReferenceCounted object;

   {
      TReferenceCountedSmartPointer<CMockReferenceCounted> pointer(&object);

      THROW_ON_FAILURE(functionName, &object == pointer.Get());

      object.CheckNoResults();
   }

   object.CheckResult(_T("|Release: 0|"));

   Output(functionName + _T(" - stop"));
}

void CReferenceCountedSmartPointerTest::TestOperatorArrow()
{
   const _tstring functionName = _T("CReferenceCountedSmartPointerTest::TestOperatorArrow");
   
   Output(functionName + _T(" - start"));

   CMockReferenceCounted object;

   {
      TReferenceCountedSmartPointer<CMockReferenceCounted> pointer(&object);

      pointer->AddRef();

      object.CheckResult(_T("|AddRef: 2|"));
   }

   object.CheckResult(_T("|Release: 1|"));

   Output(functionName + _T(" - stop"));
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: ReferenceCountedSmartPointerTest.cpp
///////////////////////////////////////////////////////////////////////////////

