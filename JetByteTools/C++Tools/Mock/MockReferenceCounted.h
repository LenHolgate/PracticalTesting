#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_MOCK_REFERENCE_COUNTED_INCLUDED__
#define JETBYTE_TOOLS_MOCK_REFERENCE_COUNTED_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: MockReferenceCounted.h 
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

#include "JetByteTools\TestTools\TestLog.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockReferenceCounted
///////////////////////////////////////////////////////////////////////////////

/// A mock object that implements AddRef() and Release() and so can be used
/// where reference counted objects might be required.
/// \ingroup CPlusPlusToolsMocks

class CMockReferenceCounted : public JetByteTools::Test::CTestLog
{
   public :

      CMockReferenceCounted();

      void AddRef();

      void Release();

   private :

      long m_ref;
};
  
///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_MOCK_REFERENCE_COUNTED_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: MockReferenceCounted.h
///////////////////////////////////////////////////////////////////////////////
