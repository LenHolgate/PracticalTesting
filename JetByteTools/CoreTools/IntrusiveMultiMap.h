#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveMultiMap.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2014 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#include "IntrusiveMultiMapNode.h"
#include "IntrusiveRedBlackTree.h"

#include "Exception.h"
#include "ToString.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveMultiMapNodeIsBaseClass
///////////////////////////////////////////////////////////////////////////////

template <class T>
class TIntrusiveMultiMapNodeIsBaseClass
{
   public :

      static CIntrusiveMultiMapNode * GetNodeFromT(
         const T *pT)
      {
         CIntrusiveMultiMapNode *pNode = const_cast<CIntrusiveMultiMapNode *>(static_cast<const CIntrusiveMultiMapNode *>(pT));

         return pNode;
      }

      static T *GetTFromNode(
         const CIntrusiveMultiMapNode *pNode)
      {
         return const_cast<T*>(static_cast<const T*>(pNode));
      }

      static T *GetTFromNode(
         const CIntrusiveRedBlackTreeNode *pNode)
      {
         return const_cast<T*>(static_cast<const T*>(pNode));
      }
};

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveMultiMap
///////////////////////////////////////////////////////////////////////////////

// In this multi-map multiple values are stored in the same tree node by
// linking them together into a doubly linked list. If a multi-map insert
// operation locates a node with the required key in the tree then the
// new node is simply added to the front of the list of any nodes that
// are already linked to the node.
//
// Conceptually you end up with something like this:
// +--------------------------------+
// |               3                |
// |              / \               |
// |             2   5-5-5-5-5      |
// |            /   / \             |
// |           1   4   6            |
// +--------------------------------+
// Tree rebalancing can continue without needing to know that the node
// is a multi-map node.
// The list is doubly linked to enable O(1) removal of a multi-map node
// given the node as a starting point. Tree rebalancing is only required
// if the last node of a given value is removed. All nodes of a given value
// can be treated equally (erase by key would erase the head of the linked
// list first) and a specific node can be erased by passing either an iterator
// or the node/data itself.
// All nodes of a given value can be removed in a single operation.
//
// In practice only the first node in a multi-map needs an additional pointer
// as we could reuse tree link pointers from the red black tree node that is
// our base class to form the list. This optimisation may occur in a later
// release.
//
// It was decided that this approach was simpler than adding a new type of
// tree node to the red black tree.

template <
   class T,
   class K,
   class TtoK,
   class Pr = std::less<K>,
   class TtoN = TIntrusiveMultiMapNodeIsBaseClass<T> >
class TIntrusiveMultiMap
{
   public :

      typedef T value_type;

      class Iterator;

      class NodeCollection;

      TIntrusiveMultiMap();

      TIntrusiveMultiMap(
         const TIntrusiveMultiMap &rhs) = delete;

      ~TIntrusiveMultiMap();

      TIntrusiveMultiMap &operator=(
         const TIntrusiveMultiMap &rhs) = delete;

      bool IsEmpty() const;

      size_t Size() const;

      Iterator Insert(
         const T *pItemToInsert);

      Iterator Insert(
         const T *pItemToInsert,
         K key);

      Iterator Find(
         const K &key) const;

      void RemoveAll(
         const Iterator &it,
         NodeCollection &nodes);

      void RemoveAll(
         const K &key,
         NodeCollection &nodes);

      T *RemoveOne(
         const K &key);

      void Erase(
         const Iterator &it);

      void Erase(
         const T *pItemToErase);

      void Clear();

      Iterator Begin() const;

      Iterator End() const;

      void Validate() const;

      static bool IsInMap(
         const CIntrusiveMultiMapNode *pNode);

      class Iterator
      {
         public :

            Iterator(
               const Iterator &rhs);

            K Key() const;

            Iterator &operator=(const Iterator &rhs);

            Iterator &operator++();    // prefix
            Iterator operator++(int);  //postfix

            Iterator &operator+=(
               size_t value);

            Iterator operator+(
               size_t value);

            bool operator==(const Iterator &rhs) const;

            bool operator!=(const Iterator &rhs) const;

            value_type * operator*();

            const value_type * operator*() const;

            value_type * operator->();

            const value_type * operator->() const;

         private :

            friend class TIntrusiveMultiMap;

            typedef TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN> Tree;

            typedef Pr key_compare;
            typedef TtoN node_accessor;
            typedef TtoK key_accessor;

            explicit Iterator(
               const typename Tree::Iterator &it);

            Iterator(
               const typename Tree::Iterator &it,
               CIntrusiveMultiMapNode *pNode);

            typename Tree::Iterator m_it;

            CIntrusiveMultiMapNode *m_pNode;
      };

      class NodeCollection
      {
         public :

            class Iterator
            {
               public :

                  Iterator(
                     const Iterator &rhs);

                  K Key() const;

                  Iterator &operator=(
                     const Iterator &rhs);

                  Iterator &operator++();    // prefix
                  Iterator operator++(int);  //postfix

                  Iterator &operator+=(
                     size_t value);

                  Iterator operator+(
                     size_t value);

                  bool operator==(
                     const Iterator &rhs) const;

                  bool operator!=(
                     const Iterator &rhs) const;

                  value_type * operator*();

                  const value_type * operator*() const;

                  value_type * operator->();

                  const value_type * operator->() const;

               private :

                  friend class NodeCollection;

                  typedef TtoN node_accessor;
                  typedef TtoK key_accessor;

                  Iterator();

                  explicit Iterator(
                     CIntrusiveMultiMapNode *pNode);

                  CIntrusiveMultiMapNode *m_pNode;
            };

            NodeCollection();

            NodeCollection(
               const NodeCollection &rhs) = delete;

            ~NodeCollection();

            NodeCollection &operator=(
               const NodeCollection &rhs) = delete;

            size_t Size() const;

            bool Empty() const;

            T *Pop();

            void Erase(
               const Iterator &it);

            Iterator Begin() const;

            Iterator End() const;

         private :

            friend class TIntrusiveMultiMap;

            void SetNodes(
               CIntrusiveMultiMapNode *pNode);

            CIntrusiveMultiMapNode *m_pNode;
      };

   private :

      typedef TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN> Tree;

      typedef Pr key_compare;
      typedef TtoN node_accessor;
      typedef TtoK key_accessor;

      Iterator InternalInsert(
         const typename Tree::pairib &result,
         CIntrusiveMultiMapNode *pNode);

      T *RemoveOne(
         const T *pItemToRemove);

      void RemoveAll(
         const typename Tree::Iterator &it,
         NodeCollection &nodes);

      static void AddNodeToList(
         CIntrusiveMultiMapNode *pNode,
         CIntrusiveMultiMapNode *pNodeToAdd);

      CIntrusiveMultiMapNode *RemoveOneNode(
         CIntrusiveMultiMapNode *pNode);

      static void RemoveFromMap(
         CIntrusiveMultiMapNode *pNode);

      static void ValidateNode(
         const T *pNode,
         ULONG_PTR userData);

      Tree m_tree;

      size_t m_size;
};

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::TIntrusiveMultiMap()
   :  m_size(0)
{
}

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::~TIntrusiveMultiMap()
{
   Clear();
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Clear()
{
   Iterator it = Begin();
   const Iterator end = End();

   while (it != end)
   {
      #if JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
      CIntrusiveRedBlackTreeNode *pNode = it.m_pNode;
      #endif

      Erase(it);

      #if JETBYTE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
      if (IsInMap(pNode))
      {
         throw CException(
            _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Clear()"),
            _T("Removed node is still in the tree"));
      }
      #endif

      it = Begin();              // iterators are invalidated after erase
   }
}

template <class T, class K, class TtoK, class Pr, class TtoN>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::IsEmpty() const
{
   return m_size == 0;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
size_t TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Size() const
{
   return m_size;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Find(
   const K &key) const
{
   return Iterator(m_tree.Find(key));
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Insert(
   const T *pItemToInsert)
{
   if (!pItemToInsert)
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Insert()"), _T("Node is null"));
   }

   CIntrusiveMultiMapNode *pNode = node_accessor::GetNodeFromT(pItemToInsert);

   if (IsInMap(pNode))
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Insert()"), _T("Node is already in a map"));
   }

   return InternalInsert(m_tree.Insert(pItemToInsert), pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Insert(
   const T *pItemToInsert,
   const K key)
{
   if (!pItemToInsert)
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Insert()"), _T("Node is null"));
   }

   CIntrusiveMultiMapNode *pNode = node_accessor::GetNodeFromT(pItemToInsert);

   if (IsInMap(pNode))
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Insert()"), _T("Node is already in a map"));
   }

   return InternalInsert(m_tree.Insert(pNode, key), pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::InternalInsert(
   const typename Tree::pairib &result,
   CIntrusiveMultiMapNode *pNode)
{
   if (!result.second)
   {
      // There was already an item in the tree with this key.
      // No problem, add it to the linked list of nodes with this key

      const T *pData = *(result.first);

      CIntrusiveMultiMapNode *pFoundNode = node_accessor::GetNodeFromT(pData);

      AddNodeToList(pFoundNode, pNode);
   }

   m_size++;

   return Iterator(result.first, pNode);
}

// We can't return an iterator here because the items need to be linked together
// but not into the tree and once we're done with them we need to be able to
// unlink them to remove them from the multi-map.

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::RemoveAll(
   const K &key,
   NodeCollection &nodes)
{
   RemoveAll(m_tree.Find(key), nodes);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::RemoveAll(
   const Iterator &it,
   NodeCollection &nodes)
{
   const T *pNode = node_accessor::GetTFromNode(it.m_pNode);

   RemoveAll(m_tree.CreateIterator(pNode), nodes);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::RemoveAll(
   const typename Tree::Iterator &it,
   NodeCollection &nodes)
{
   T *pData = nullptr;

   if (it != m_tree.End())
   {
      pData = const_cast<T *>(*it);

      m_tree.Erase(it);
   }

   nodes.SetNodes(node_accessor::GetNodeFromT(pData));

   m_size -= nodes.Size();
}

template <class T, class K, class TtoK, class Pr, class TtoN>
T *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::RemoveOne(
   const K &key)
{
   T *pData = nullptr;

   typename Tree::Iterator it = m_tree.Find(key);

   if (it != m_tree.End())
   {
      pData = RemoveOne(it.m_pNode, false);
   }

   return pData;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
T *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::RemoveOne(
   const T *pItemToRemove)
{
   // See if we can take one of the chained nodes, if we have multiple values at this
   // node...

   T *pData = node_accessor::GetTFromNode(RemoveOneNode(node_accessor::GetNodeFromT(pItemToRemove)));

   if (!pData)
   {
      // We don't have multiple values, so we need to use this node and erase it from
      // the underlying tree...

      pData = const_cast<T *>(pItemToRemove);

      m_tree.Erase(pItemToRemove);
   }

   if (pData)
   {
      m_size--;

      RemoveFromMap(node_accessor::GetNodeFromT(pData));
   }

   return pData;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Erase(
   const Iterator &it)
{
   RemoveOne(node_accessor::GetTFromNode(it.m_pNode));
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Erase(
   const T *pItemToErase)
{
   RemoveOne(pItemToErase);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Begin() const
{
   return Iterator(m_tree.Begin());
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::End() const
{
   return Iterator(m_tree.End());
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::AddNodeToList(
   CIntrusiveMultiMapNode *pNode,
   CIntrusiveMultiMapNode *pNodeToAdd)
{
   pNodeToAdd->m_pNext = pNode->m_pNext;

   if (pNodeToAdd->m_pNext)
   {
      pNodeToAdd->m_pNext->m_pPrev = pNodeToAdd;
   }

   pNodeToAdd->m_pPrev = pNode;

   pNode->m_pNext = pNodeToAdd;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Validate() const
{
   std::pair<size_t, size_t> sizeCheck;

   sizeCheck.first = 0;
   sizeCheck.second = m_size;

   m_tree.ValidateTree(ValidateNode, reinterpret_cast<ULONG_PTR>(&sizeCheck));
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::ValidateNode(
   const T *pNode,
   const ULONG_PTR userData)
{
   std::pair<size_t, size_t> &sizeCheck = *reinterpret_cast<std::pair<size_t, size_t> *>(userData);

   CIntrusiveMultiMapNode *pThisNode = node_accessor::GetNodeFromT(pNode);

   do
   {
      sizeCheck.first++;

      if (sizeCheck.first > sizeCheck.second)
      {
         throw CException(
            _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Validate()"),
            _T("A walk of the map does not contain the right number of nodes: expected:") + ToString(sizeCheck.second) + _T(" got at least: ") + ToString(sizeCheck.first));
      }

      CIntrusiveMultiMapNode *pNextNode = pThisNode->m_pNext;

      if (pNextNode)
      {
         if (pNextNode->m_pPrev != pThisNode)
         {
            throw CException(
               _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Validate()"),
               _T("Multi-value node not correctly linked to parent"));
         }
      }

      pThisNode = pNextNode;

   } while (pThisNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::IsInMap(
   const CIntrusiveMultiMapNode *pNode)
{
   return pNode->m_pPrev || pNode->m_pNext || Tree::IsInTree(pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::RemoveFromMap(
   CIntrusiveMultiMapNode *pNode)
{
   if (!pNode)
   {
      throw CException(
         _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::RemoveFromMap()"),
         _T("pNode is null"));
   }

   pNode->m_pPrev = nullptr;
   pNode->m_pNext = nullptr;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
CIntrusiveMultiMapNode *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::RemoveOneNode(
   CIntrusiveMultiMapNode *pNode)
{
   CIntrusiveMultiMapNode *pRemovedNode = nullptr;

   if (pNode)
   {
      if (m_tree.IsInTree(pNode))
      {
         if (pNode->m_pNext)
         {
            pRemovedNode = pNode;

            pNode = pNode->m_pNext;

            pNode->m_pPrev = nullptr;

            m_tree.SwapNode(pRemovedNode, pNode);

            m_tree.RemoveFromTree(pRemovedNode);
         }
         else
         {
            // node must be removed by erasure from the tree
            // we do nothing here but return 0;
         }
      }
      else
      {
         // This node is not directly "in the tree", it's
         // part of the multi-map node chain for this tree node.

         pRemovedNode = pNode;

         if (pRemovedNode->m_pNext)
         {
            pRemovedNode->m_pNext->m_pPrev = pRemovedNode->m_pPrev;
         }

         if (pRemovedNode->m_pPrev)
         {
            pRemovedNode->m_pPrev->m_pNext = pRemovedNode->m_pNext;
         }
      }

      if (pRemovedNode)
      {
         pRemovedNode->m_pPrev = nullptr;
         pRemovedNode->m_pNext = nullptr;
      }
   }

   return pRemovedNode;
}

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator
///////////////////////////////////////////////////////////////////////////////

template <class T, class K, class TtoK, class Pr, class TtoN>
K TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::Key() const
{
   return key_accessor::GetKeyFromT(node_accessor::GetTFromNode(m_pNode));
}

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::Iterator(
   const typename Tree::Iterator &it,
   CIntrusiveMultiMapNode *pNode)
   :  m_it(it),
      m_pNode(pNode)
{
}

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::Iterator(
   const typename Tree::Iterator &it)
   :  m_it(it),
      m_pNode(node_accessor::GetNodeFromT(const_cast<T*>(*it)))
{
}

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::Iterator(
   const Iterator &rhs)
   :  m_it(rhs.m_it),
      m_pNode(rhs.m_pNode)
{
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator &TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::operator=(
   const Iterator &rhs)
{
   m_it = rhs.m_it;
   m_pNode = rhs.m_pNode;

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator &TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::operator++()
{
   if (!m_pNode)
   {
      return *this;
   }

   if (!m_pNode->m_pNext)
   {
      ++m_it;

      m_pNode = node_accessor::GetNodeFromT(*m_it);
   }
   else
   {
      m_pNode = m_pNode->m_pNext;
   }

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::operator++(int)
{
   Iterator result(*this);

   this->operator++();

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator &TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::operator+=(
   const size_t value)
{
   size_t added = 0;

   while (m_pNode && added != value)
   {
      operator++();

      added++;
   }

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::operator+(
   const size_t value)
{
   Iterator result = *this;

   result += value;

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::operator==(
   const Iterator &rhs) const
{
   return m_pNode == rhs.m_pNode && m_it == rhs.m_it;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::operator!=(
   const Iterator &rhs) const
{
   return !(*this == rhs);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::operator *()
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
const typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::operator *() const
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::operator ->()
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
const typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::Iterator::operator ->() const
{
   return node_accessor::GetTFromNode(m_pNode);
}

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection
///////////////////////////////////////////////////////////////////////////////

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::NodeCollection()
   :  m_pNode(nullptr)
{

}

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::~NodeCollection()
{
   while (m_pNode)
   {
      CIntrusiveMultiMapNode *pNext = m_pNode->m_pNext;

      m_pNode->m_pNext = nullptr;
      m_pNode->m_pPrev = nullptr;

      m_pNode = pNext;
   }
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::SetNodes(
   CIntrusiveMultiMapNode *pNode)
{
   if (m_pNode)
   {
      throw CException(
         _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::SetNodes()"),
         _T("Collection already contains nodes"));
   }

   m_pNode = pNode;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Empty() const
{
   return m_pNode == nullptr;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
size_t TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Size() const
{
   size_t size = 0;

   CIntrusiveMultiMapNode *pNode = m_pNode;

   while (pNode)
   {
      size++;

      pNode = pNode->m_pNext;
   }

   return size;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
T *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Pop()
{
   T *pData = nullptr;

   if (m_pNode)
   {
      CIntrusiveMultiMapNode *pPoppedNode = m_pNode;

      m_pNode = m_pNode->m_pNext;

      if (m_pNode)
      {
         m_pNode->m_pPrev = nullptr;
      }

      pPoppedNode->m_pNext = nullptr;

      pData = node_accessor::GetTFromNode(pPoppedNode);
   }

   return pData;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Erase(
   const Iterator &it)
{
   if (it.m_pNode)
   {
      if (it.m_pNode == m_pNode)
      {
         m_pNode = it.m_pNode->m_pNext;
      }

      if (it.m_pNode->m_pPrev)
      {
         it.m_pNode->m_pPrev = it.m_pNode->m_pNext;
      }

      if (it.m_pNode->m_pNext)
      {
         it.m_pNode->m_pNext = it.m_pNode->m_pPrev;
      }

      RemoveFromMap(it.m_pNode);
   }
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Begin() const
{
   return Iterator(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::End() const
{
   return Iterator();
}

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator
///////////////////////////////////////////////////////////////////////////////

template <class T, class K, class TtoK, class Pr, class TtoN>
K TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::Key() const
{
   return key_accessor::GetKeyFromT(node_accessor::GetTFromNode(m_pNode));
}

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::Iterator()
   :  m_pNode(nullptr)
{
}

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::Iterator(
   CIntrusiveMultiMapNode *pNode)
   :  m_pNode(pNode)
{
}

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::Iterator(
   const Iterator &rhs)
   :  m_pNode(rhs.m_pNode)
{
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator &TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::operator=(
   const Iterator &rhs)
{
   m_pNode = rhs.m_pNode;

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator &TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::operator++()
{
   if (!m_pNode)
   {
      return *this;
   }

   m_pNode = m_pNode->m_pNext;

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::operator++(int)
{
   Iterator result(*this);

   this->operator++();

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator &TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::operator+=(
   const size_t value)
{
   size_t added = 0;

   while (m_pNode && added != value)
   {
      operator++();

      added++;
   }

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::operator+(
   const size_t value)
{
   Iterator result = *this;

   result += value;

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::operator==(
   const Iterator &rhs) const
{
   return m_pNode == rhs.m_pNode;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::operator!=(
   const Iterator &rhs) const
{
   return !(*this == rhs);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::operator *()
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
const typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::operator *() const
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::operator ->()
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
const typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN>::NodeCollection::Iterator::operator ->() const
{
   return node_accessor::GetTFromNode(m_pNode);
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveMultiMap.h
///////////////////////////////////////////////////////////////////////////////
