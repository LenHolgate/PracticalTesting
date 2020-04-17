#pragma once
#ifndef JETBYTE_TOOLS_WIN32_TEST_INTRUSIVE_MULTI_MAP_NODE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_TEST_INTRUSIVE_MULTI_MAP_NODE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: TestIntrusiveMultiMapNode.h
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

#include "JetByteTools/Win32Tools/IntrusiveMultiMapNode.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CTestIntrusiveMultiMapNode
///////////////////////////////////////////////////////////////////////////////

class CTestIntrusiveMultiMapNode : public CIntrusiveMultiMapNode
{
   public :

      explicit CTestIntrusiveMultiMapNode(
         int value);

      CTestIntrusiveMultiMapNode(
         const CTestIntrusiveMultiMapNode &rhs) = delete;

      CTestIntrusiveMultiMapNode &operator=(
         const CTestIntrusiveMultiMapNode &rhs) = delete;

      int Value() const;

      int Key() const;

   private :

      const int m_value;
};

class CTestIntrusiveMultiMapNodeKeyAccessor
{
   public :

      static unsigned long GetKeyFromT(
         const CTestIntrusiveMultiMapNode *pNode)
      {
         return pNode->Key();
      }
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_TEST_INTRUSIVE_MULTI_MAP_NODE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: TestIntrusiveMultiMapNode.h
///////////////////////////////////////////////////////////////////////////////
