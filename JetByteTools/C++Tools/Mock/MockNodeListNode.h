#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_MOCK_NODE_LIST_NODE_INCLUDED__
#define JETBYTE_TOOLS_MOCK_NODE_LIST_NODE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: MockNodeListNode.h 
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

#include "..\NodeList.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockNodeListNode
///////////////////////////////////////////////////////////////////////////////

/// A mock object that derives from CNodeList::Node and so can be stored in
/// a CNodeList or a TNodeList<CMockNodeListNode>.
/// \ingroup CPlusPlusToolsMocks

class CMockNodeListNode : public CNodeList::Node
{
   public :

      CMockNodeListNode()
      {

      }
};
  
///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_MOCK_NODE_LIST_NODE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: MockNodeListNode.h
///////////////////////////////////////////////////////////////////////////////
