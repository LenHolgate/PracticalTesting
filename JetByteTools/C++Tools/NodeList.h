#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_NODE_LIST_INCLUDED__
#define JETBYTE_TOOLS_NODE_LIST_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: NodeList.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2002 JetByte Limited.
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
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

///////////////////////////////////////////////////////////////////////////////
// CNodeList
///////////////////////////////////////////////////////////////////////////////

/// An invasive doubly linked list where classes that wish to be nodes in the 
/// list must derive from CNodeList::Node. The list allows nodes to remove 
/// themselves from the list as a constant time operation.
/// \ingroup CPlusPlusTools

class CNodeList
{
   public :

      /// The node class that classes must derive from to be able to be stored
      /// in the list.

      class Node
      {
         public :

            /// Return the next node in the list

            Node *Next() const;

            /// Set the given node to be this nodes 'next node'.

            void Next(
               Node *pNext);

            /// If this node is currently in a list then it is removed.

            void RemoveFromList();

            /// Returns true if this node is currently in the specified list.

            bool InList(
               const CNodeList &list) const;

            /// Returns true if this node is currently in any list.

            bool InList() const;

         protected :

            Node();
            ~Node();

         private :

            friend class CNodeList;

            void AddToList(
               CNodeList *pList);

            void Unlink();

            Node *m_pNext;
            Node *m_pPrev;

            CNodeList *m_pList;

            /// No copies do not implement
            Node(const Node &rhs);
            /// No copies do not implement
            Node &operator=(const Node &rhs);
      };

      /// Construct an empty node list.

      CNodeList();

      /// Push the given node onto the list.

      void PushNode(
         Node *pNode);

      /// Pop the head of the list and return it. Returns 0 if the list is
      /// empty.

      Node *PopNode();

      /// Return the head of the list. Returns 0 if the list is empty.

      Node *Head() const;

      /// Insert pNewNode after pNode in the list. If pNode is 0 then this
      /// degenerates to a PushNode().

      void InsertAfter(
         Node *pNode, 
         Node *pNewNode);

      /// Returns the number of nodes in the list.

      unsigned long Count() const;

      /// Returns true if the list contains no nodes.

      bool IsEmpty() const;

   private :

      friend void Node::RemoveFromList();

      void RemoveNode(Node &node);

      Node *m_pHead; 

      unsigned long m_numNodes;

      /// No copies do not implement
      CNodeList(const CNodeList &rhs);
      /// No copies do not implement
      CNodeList &operator=(const CNodeList &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// TNodeList
///////////////////////////////////////////////////////////////////////////////

/// A template wrapper that makes an instance of CNodeList typesafe with respect
/// of the classes stored within it. Simply wraps the CNodeList::Node access
/// methods to work in terms of the template T rather than any class that 
/// happens to derive from CNodeList::Node.
/// \ingroup CPlusPlusTools
/// \ingroup Templates

template <class T> class TNodeList : public CNodeList
{
   public :
         
      T *PopNode();

      T *Head() const;

      static T *Next(
         const T *pNode);
};

template <class T>
T *TNodeList<T>::PopNode()
{
   return static_cast<T*>(CNodeList::PopNode());
}

template <class T>
T *TNodeList<T>::Head() const
{
   return static_cast<T*>(CNodeList::Head());
}

template <class T>
T *TNodeList<T>::Next(
   const T *pNode)
{
   return static_cast<T*>(pNode->Next());
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_NODE_LIST_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: NodeList.h
///////////////////////////////////////////////////////////////////////////////
