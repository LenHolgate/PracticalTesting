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
#include "ExceptionLeakPrevention.h"
#include "ToString.h"

#include <set>

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

template <class T, class TtoK>
class TIntrusiveMultiMapKeyIsSimpleToPrint
{
   public:

      static _tstring GetKeyAsStringFromT(
         const T *pT)
      {
         return ToString(TtoK::GetKeyFromT(pT));
      }
};

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveMultiMap
///////////////////////////////////////////////////////////////////////////////

// In this multimap multiple values are stored in the same tree node by
// linking them together into a doubly linked list. If a multimap insert
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
// is a multimap node.
// The list is doubly linked to enable O(1) removal of a multi-map node
// given the node as a starting point. Tree rebalancing is only required
// if the last node of a given value is removed. All nodes of a given value
// can be treated equally (erase by key would erase the head of the linked
// list first) and a specific node can be erased by passing either an iterator
// or the node/data itself.
// All nodes of a given value can be removed in a single operation.
//
// In practice only the first node in a multimap needs an additional pointer
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
   class TtoN = TIntrusiveMultiMapNodeIsBaseClass<T>,
   class TtoKS = TIntrusiveMultiMapKeyIsSimpleToPrint<T, TtoK> >
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

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
      void EnableValidation();
      #endif

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

      bool Erase(
         const Iterator &it);

      bool Erase(
         const T *pItemToErase);

      typedef TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN, TtoKS> Tree;

      using ClearCallback = std::function<void(T *)>;

      typedef typename Tree::ClearFlags ClearFlags;

      void Clear(
         ClearFlags flags = ClearFlags::Erase,
         const ClearCallback &clearCallback = nullptr);

      Iterator Begin() const;

      Iterator End() const;

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_VALIDATION == 1
      void Validate() const;

      void Validate(
         const _tstring &callingFunction) const;
      #endif

      static bool IsInMap(
         const CIntrusiveMultiMapNode *pNode);

      _tstring DumpMap(
         bool printNodeAddresses = true) const;

      class Iterator
      {
         public :

            typedef TIntrusiveMultiMap<T, K, TtoK, Pr, TtoN> Map;

            Iterator(
               const Iterator &rhs);

            #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
            bool IsValid() const;
            #endif

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

            typedef TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN, TtoKS> Tree;

            typedef Pr key_compare;
            typedef TtoN node_accessor;
            typedef TtoK key_accessor;

            Iterator(
               const Map &map,
               const typename Tree::Iterator &it);

            Iterator(
               const typename Tree::Iterator &it);

            Iterator(
               const Map &map,
               const typename Tree::Iterator &it,
               CIntrusiveMultiMapNode *pNode);

            typename Tree::Iterator m_it;

            CIntrusiveMultiMapNode *m_pNode;

            #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
            const Map *m_pMap;

            size_t m_mapChangeNumber;
            #endif
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

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
      size_t GetCurrentChangeNumber() const;
      #endif

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

      void AddNodeToList(
         CIntrusiveMultiMapNode *pNode,
         CIntrusiveMultiMapNode *pNodeToAdd);

      CIntrusiveMultiMapNode *RemoveOneNode(
         CIntrusiveMultiMapNode *pNode);

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_VALIDATION == 1

      typedef std::pair<size_t, size_t> SizeCheck;
      typedef std::pair<SizeCheck, std::set<CIntrusiveMultiMapNode *>> CheckData;

      static void ValidateNode(
         const _tstring &callingFunction,
         const T *pNode,
         ULONG_PTR userData);
      #endif

      Tree m_tree;

      size_t m_size;

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
      size_t m_changeNumber;
      #endif

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
      mutable std::deque<_tstring> m_previousOperations;
      mutable _tstring m_previousDump;
      #endif
      bool m_validationEnabled;
      #endif
};

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::TIntrusiveMultiMap()
   :  m_size(0)
      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
      , m_changeNumber(0)
      #endif
      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
      , m_validationEnabled(false)
      #endif
{
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::~TIntrusiveMultiMap()
{
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START

   Clear();

   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END
}

#if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
size_t TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::GetCurrentChangeNumber() const
{
   return m_changeNumber;
}
#endif

#if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::EnableValidation()
{
   m_validationEnabled = true;

   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
   m_tree.EnableValidation();
   #endif
}
#endif

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Clear(
   const ClearFlags flags,
   const ClearCallback &clearCallback)
{
   m_tree.Clear(
      flags,
      [&](const T *pT) -> void {

         CIntrusiveMultiMapNode *pNode = node_accessor::GetNodeFromT(pT);

         if (flags != ClearFlags::FastAndDirty || clearCallback)
         {
            while (pNode)
            {
               if (clearCallback)
               {
                  clearCallback(node_accessor::GetTFromNode(pNode));
               }

               CIntrusiveMultiMapNode *pNextNode = pNode->m_pNext;

               if (flags != ClearFlags::FastAndDirty)
               {
                  pNode->m_pPrev = nullptr;
                  pNode->m_pNext = nullptr;
               }

               pNode = pNextNode;
            }
      }
   });

   m_size = 0;

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
   if (flags != ClearFlags::Erase)
   {
      ++m_changeNumber;
   }
   #endif

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
   if (m_validationEnabled)
   {
      m_previousOperations.emplace_back(_T("Clear-post"));
   }
   #endif
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::IsEmpty() const
{
   return m_size == 0;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
size_t TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Size() const
{
   return m_size;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Find(
   const K &key) const
{
   return Iterator(*this, m_tree.Find(key));
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Insert(
   const T *pItemToInsert)
{
   if (!pItemToInsert)
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Insert()"), _T("Node is null"));
   }

   CIntrusiveMultiMapNode *pNode = node_accessor::GetNodeFromT(pItemToInsert);

   if (IsInMap(pNode))
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Insert()"), _T("Node is already in a map"));
   }

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
   if (m_validationEnabled)
   {
      Validate(_T("InternalInsert-pre - ") + PointerToString(static_cast<const CIntrusiveRedBlackTreeNode*>(pNode)) + _T(" - ") + ToString(key_accessor::GetKeyFromT(pItemToInsert)));

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
      m_previousDump = DumpMap();
      #endif
   }
   #endif

   return InternalInsert(m_tree.Insert(pItemToInsert), pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Insert(
   const T *pItemToInsert,
   const K key)
{
   if (!pItemToInsert)
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Insert()"), _T("Node is null"));
   }

   CIntrusiveMultiMapNode *pNode = node_accessor::GetNodeFromT(pItemToInsert);

   if (key != key_accessor::GetKeyFromT(pItemToInsert))
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Insert()"), _T("Supplied key is not the same as extracted key"));
   }

   if (IsInMap(pNode))
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Insert()"), _T("Node is already in a map"));
   }

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
   if (m_validationEnabled)
   {
      Validate(_T("InternalInsert-pre - ") + PointerToString(static_cast<const CIntrusiveRedBlackTreeNode*>(pNode)) + _T(" - ") + ToString(key));

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
      m_previousDump = DumpMap();
      #endif
   }
   #endif

   return InternalInsert(m_tree.Insert(pItemToInsert, key), pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::InternalInsert(
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

   ++m_size;

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
   if (m_validationEnabled)
   {
      Validate(_T("InternalInsert-post -") + PointerToString(static_cast<const CIntrusiveRedBlackTreeNode*>(pNode)) + _T(" - ") + ToString(key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pNode))));
   }
   #endif

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
   ++m_changeNumber;
   #endif

   return Iterator(*this, result.first, pNode);
}

// We can't return an iterator here because the items need to be linked together
// but not into the tree and once we're done with them, we need to be able to
// unlink them to remove them from the multimap.

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::RemoveAll(
   const K &key,
   NodeCollection &nodes)
{
   RemoveAll(m_tree.Find(key), nodes);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::RemoveAll(
   const Iterator &it,
   NodeCollection &nodes)
{
   const T *pNode = node_accessor::GetTFromNode(it.m_pNode);

   RemoveAll(m_tree.CreateIterator(pNode), nodes);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::RemoveAll(
   const typename Tree::Iterator &it,
   NodeCollection &nodes)
{
   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
   if (m_validationEnabled)
   {
      Validate(_T("RemoveAll-pre"));

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
      m_previousDump = DumpMap();
      #endif
   }
   #endif

   T *pData = nullptr;

   if (it != m_tree.End())
   {
      pData = const_cast<T *>(*it);

      m_tree.Erase(it);
   }

   nodes.SetNodes(node_accessor::GetNodeFromT(pData));

   const size_t numNodes = nodes.Size();

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (numNodes > m_size)
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::RemoveAll()"), _T("numNodes > m_size"));
   }
   #endif

   m_size -= numNodes;

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
   if (m_validationEnabled)
   {
      Validate(_T("RemoveAll-post"));

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
      m_previousDump = DumpMap();
      #endif
   }
   #endif
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
T *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::RemoveOne(
   const K &key)
{
   T *pData = nullptr;

   typename Tree::Iterator it = m_tree.Find(key);

   if (it != m_tree.End())
   {
      pData = RemoveOne(*it);
   }

   return pData;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
T *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::RemoveOne(
   const T *pItemToRemove)
{
   T *pData = nullptr;

   CIntrusiveMultiMapNode *pNodeToRemove = node_accessor::GetNodeFromT(pItemToRemove);

   if (IsInMap(pNodeToRemove))
   {
      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
      if (m_validationEnabled)
      {
         Validate(_T("RemoveOne-pre - ") + PointerToString(static_cast<const CIntrusiveRedBlackTreeNode*>(pNodeToRemove)) + _T(" - ") + ToString(key_accessor::GetKeyFromT(pItemToRemove)));

         #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
         m_previousDump = DumpMap();
         #endif
      }
      #endif

      // See if we can take one of the chained nodes, if we have multiple values at this
      // node...

      pData = node_accessor::GetTFromNode(RemoveOneNode(pNodeToRemove));

      if (!pData)
      {
         // We don't have multiple values, so we need to use this node and erase it from
         // the underlying tree...

         pData = const_cast<T *>(pItemToRemove);

         m_tree.Erase(pItemToRemove);
      }

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
      if (m_size == 0)
      {
         #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
         if (m_validationEnabled)
         {
            OutputEx(_T("RemoveOne: m_size == 0 - ") + m_previousDump);
         }
         #endif

         #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_DEBUG_TRACE == 1 || JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
         const _tstring dump = DumpMap();

         OutputEx(dump);

         #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
         m_previousDump = dump;
         #endif
         #endif

         throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::RemoveOne()"), _T("m_size == 0"));
      }
      #endif

      --m_size;

      node_accessor::GetNodeFromT(pData)->ResetNode();

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
      if (m_validationEnabled)
      {
         Validate(_T("RemoveOne-post - ") + PointerToString(static_cast<const CIntrusiveRedBlackTreeNode*>(pNodeToRemove)) + _T(" - ") + ToString(key_accessor::GetKeyFromT(pItemToRemove)));
      }
      #endif

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
      ++m_changeNumber;
      #endif
   }
   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
   else
   {
      if (m_validationEnabled)
      {
         // too expensive to validate
         //Validate(_T("RemoveOne-NotInTree - ") + PointerToString(static_cast<const CIntrusiveRedBlackTreeNode*>(pNodeToRemove)));

         m_previousDump = DumpMap();

         m_previousOperations.emplace_back(_T("RemoveOne-NotInTree - ") + PointerToString(static_cast<const CIntrusiveRedBlackTreeNode*>(pNodeToRemove)) + _T(" - ") + ToString(key_accessor::GetKeyFromT(pItemToRemove)));
      }
   }
   #endif

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (pNodeToRemove->IsActive())
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::RemoveOne()"), _T("Node still active"));
   }
   #endif

   return pData;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Erase(
   const Iterator &it)
{
   const T *pItemToErase = node_accessor::GetTFromNode(it.m_pNode);

   return Erase(pItemToErase);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Erase(
   const T *pItemToErase)
{
   const T *pItemRemoved = RemoveOne(pItemToErase);

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (pItemRemoved && pItemRemoved != pItemToErase)
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Erase()"), _T("Erased wrong node"));
   }
   #endif

   return pItemToErase == pItemRemoved;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Begin() const
{
   return Iterator(*this, m_tree.Begin());
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::End() const
{
   return Iterator(m_tree.End());
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::AddNodeToList(
   CIntrusiveMultiMapNode *pNode,
   CIntrusiveMultiMapNode *pNodeToAdd)
{
   if (!pNode)
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::AddNodeToList()"), _T("Node is null"));
   }

   if (IsInMap(pNodeToAdd))
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::AddNodeToList()"), _T("Node is already in map"));
   }

   if (pNode == pNodeToAdd)
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::AddNodeToList()"), _T("pNode == pNodeToAdd"));
   }

   if (pNode->m_pPrev)
   {
      throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::AddNodeToList()"), _T("pNode->m_pPrev != null"));
   }

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION == 1
   CIntrusiveMultiMapNode *pN = pNode;

   size_t numInList = 0;

   while (pN)
   {
      if (pN == pNodeToAdd)
      {
         throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::AddNodeToList()"), _T("pN == pNodeToAdd, node already in list"));
      }

      ++numInList;

      if (numInList > m_size)
      {
         throw CException(_T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::AddNodeToList()"), _T("More nodes in this list than in entire map!"));
      }

      pN = pN->m_pNext;
   }
   #endif

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_DEBUG_TRACE == 1
   OutputEx(_T("Insert: ") + ToString(key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pNodeToAdd))));
   #endif

   pNodeToAdd->m_pNext = pNode->m_pNext;

   if (pNodeToAdd->m_pNext)
   {
      pNodeToAdd->m_pNext->m_pPrev = pNodeToAdd;
   }

   pNodeToAdd->m_pPrev = pNode;

   pNode->m_pNext = pNodeToAdd;

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
   if (m_validationEnabled)
   {
      m_previousOperations.emplace_back(_T("AddNodeToList - ") + PointerToString(pNode) + _T(" + ") + PointerToString(pNodeToAdd));
   }
   #endif
}

#if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_VALIDATION == 1
template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Validate() const
{
   Validate(_T("Validate"));
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Validate(
   const _tstring &callingFunction) const
{
   CheckData checkData;
   checkData.first.first = 0;
   checkData.first.second = m_size;

   try
   {
      m_tree.ValidateTree(callingFunction, ValidateNode, reinterpret_cast<ULONG_PTR>(&checkData));

      if (checkData.first.first != checkData.first.second)
      {
         #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
         if (m_validationEnabled)
         {
            OutputEx(_T("Validation Failed: ") + callingFunction + _T(" - ") + m_previousDump);
         }
         #endif
         throw CException(
            _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Validate()"),
            callingFunction + _T(" - A walk of the map does not contain the right number of nodes: expected:") + ToString(checkData.first.second) + _T(" got: ") + ToString(checkData.first.first));
      }
   }
   catch (...)
   {
      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
      if (m_validationEnabled)
      {
         OutputEx(_T("Validation Failed: ") + callingFunction + _T(" - ") + m_previousDump);
      }
      #endif

      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_DEBUG_TRACE == 1
      const _tstring dump = DumpMap();

      OutputEx(callingFunction + _T(" - ") + dump);
      #endif

      throw;
   }

   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
   if (m_validationEnabled)
   {
      m_previousOperations.emplace_back(callingFunction);
   }
   #endif
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::ValidateNode(
   const _tstring &callingFunction,
   const T *pNode,
   const ULONG_PTR userData)
{
   CheckData &checkData = *reinterpret_cast<CheckData*>(userData);

   CIntrusiveMultiMapNode *pThisNode = node_accessor::GetNodeFromT(pNode);

   if (pThisNode->m_pPrev)
   {
      throw CException(
         _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Validate()"),
         callingFunction + _T(" - first node in chain should not have a previous node"));
   }

   do
   {
      if (!checkData.second.emplace(pThisNode).second)
      {
         throw CException(
            _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Validate()"),
            callingFunction + _T(" - duplicate node"));
      }

      checkData.first.first++;

      if (checkData.first.first > checkData.first.second)
      {
         throw CException(
            _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Validate()"),
            callingFunction + _T(" - A walk of the map does not contain the right number of nodes: expected:") + ToString(checkData.first.second) + _T(" got at least: ") + ToString(checkData.first.first));
      }

      CIntrusiveMultiMapNode *pNextNode = pThisNode->m_pNext;

      if (pNextNode)
      {
         if (pNextNode->m_pPrev != pThisNode)
         {
            throw CException(
               _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Validate()"),
               callingFunction + _T(" - Multi-value node not correctly linked to parent"));
         }
      }

      pThisNode = pNextNode;

   } while (pThisNode);
}
#endif

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
_tstring TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::DumpMap(
   const bool printNodeAddresses) const
{
   const _tstring dump = ToString(m_size) + _T(" - ") + m_tree.DumpTree(printNodeAddresses, true, [&](const T *pT) -> _tstring {
      const CIntrusiveMultiMapNode *pNode = node_accessor::GetNodeFromT(pT);

      _tstring dump = _T(" {");

      size_t count = 0;

      while (pNode)
      {
         if (printNodeAddresses)
         {
            dump += PointerToString(static_cast<const CIntrusiveRedBlackTreeNode *>(pNode)) + _T(", ");
         }

         ++count;

         pNode = pNode->m_pNext;
      }

      dump += ToString(count) + _T("} ");

      return dump;
         });

   return dump;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::IsInMap(
   const CIntrusiveMultiMapNode *pNode)
{
   return pNode->IsActive();
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
CIntrusiveMultiMapNode *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::RemoveOneNode(
   CIntrusiveMultiMapNode *pNode)
{
   CIntrusiveMultiMapNode *pRemovedNode = nullptr;

   if (pNode)
   {
      if (m_tree.IsInTree(pNode))
      {
         if (pNode->m_pPrev)
         {
            throw CException(
               _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::RemoveOneNode()"),
               _T("pNode is linked to a previous node"));
         }

         if (pNode->m_pNext)
         {
            pRemovedNode = pNode;

            pNode = pNode->m_pNext;

            pNode->m_pPrev = nullptr;

            m_tree.SwapNode(pRemovedNode, pNode);

            pRemovedNode->ResetNode();
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
// TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator
///////////////////////////////////////////////////////////////////////////////

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
K TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::Key() const
{
   return key_accessor::GetKeyFromT(node_accessor::GetTFromNode(m_pNode));
}

#if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::IsValid() const
{
   return m_pMap && m_pMap->GetCurrentChangeNumber() == m_mapChangeNumber;
}
#endif

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::Iterator(
   const Map &map,
   const typename Tree::Iterator &it,
   CIntrusiveMultiMapNode *pNode)
   :  m_it(it),
      m_pNode(pNode)
      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
      , m_pMap(&map),
      m_mapChangeNumber(m_pMap->GetCurrentChangeNumber())
      #endif
{
   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION != 1
   (void)map;
   #endif
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::Iterator(
   const Map &map,
   const typename Tree::Iterator &it)
   :  m_it(it),
      m_pNode(node_accessor::GetNodeFromT(const_cast<T*>(*it)))
      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
      , m_pMap(&map),
      m_mapChangeNumber(m_pMap->GetCurrentChangeNumber())
      #endif
{
   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION != 1
   (void)map;
   #endif
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::Iterator(
   const typename Tree::Iterator &it)
   :  m_it(it),
      m_pNode(node_accessor::GetNodeFromT(const_cast<T*>(*it)))
      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
      , m_pMap(nullptr),
      m_mapChangeNumber(0)
      #endif
{
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::Iterator(
   const Iterator &rhs)
   :  m_it(rhs.m_it),
      m_pNode(rhs.m_pNode)
      #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
      , m_pMap(rhs.m_pMap),
      m_mapChangeNumber(rhs.m_mapChangeNumber)
      #endif
{
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator &TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator=(
   const Iterator &rhs)
{
   m_it = rhs.m_it;
   m_pNode = rhs.m_pNode;
   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
   m_pMap = rhs.m_pMap;
   m_mapChangeNumber = rhs.m_mapChangeNumber;
   #endif

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator &TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator++()
{
   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
   if (!IsValid())
   {
      throw CException(
         _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator++()"),
         _T("Iterator is not valid"));
   }
   #endif

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

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator++(int)
{
   #if JETBYTE_CORE_INTRUSIVE_MULTI_MAP_ENABLE_ITERATOR_VALIDATION == 1
   if (!IsValid())
   {
      throw CException(
         _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator++(int)"),
         _T("Iterator is not valid"));
   }
   #endif

   Iterator result(*this);

   this->operator++();

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator &TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator+=(
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

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator+(
   const size_t value)
{
   Iterator result = *this;

   result += value;

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator==(
   const Iterator &rhs) const
{
   return m_pNode == rhs.m_pNode && m_it == rhs.m_it;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator!=(
   const Iterator &rhs) const
{
   return !(*this == rhs);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator *()
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
const typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator *() const
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator ->()
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
const typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator ->() const
{
   return node_accessor::GetTFromNode(m_pNode);
}

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection
///////////////////////////////////////////////////////////////////////////////

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::NodeCollection()
   :  m_pNode(nullptr)
{

}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::~NodeCollection()
{
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START

   while (m_pNode)
   {
      CIntrusiveMultiMapNode *pNext = m_pNode->m_pNext;

      m_pNode->m_pNext = nullptr;
      m_pNode->m_pPrev = nullptr;

      m_pNode = pNext;
   }

   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::SetNodes(
   CIntrusiveMultiMapNode *pNode)
{
   if (m_pNode)
   {
      throw CException(
         _T("TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::SetNodes()"),
         _T("Collection already contains nodes"));
   }

   m_pNode = pNode;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Empty() const
{
   return m_pNode == nullptr;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
size_t TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Size() const
{
   size_t size = 0;

   const CIntrusiveMultiMapNode *pNode = m_pNode;

   while (pNode)
   {
      size++;

      pNode = pNode->m_pNext;
   }

   return size;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
T *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Pop()
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

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Erase(
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

      it.m_pNode->ResetNode();
   }
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Begin() const
{
   return Iterator(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::End() const
{
   return Iterator();
}

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator
///////////////////////////////////////////////////////////////////////////////

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
K TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::Key() const
{
   return key_accessor::GetKeyFromT(node_accessor::GetTFromNode(m_pNode));
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::Iterator()
   :  m_pNode(nullptr)
{
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::Iterator(
   CIntrusiveMultiMapNode *pNode)
   :  m_pNode(pNode)
{
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::Iterator(
   const Iterator &rhs)
   :  m_pNode(rhs.m_pNode)
{
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator &TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::operator=(
   const Iterator &rhs)
{
   m_pNode = rhs.m_pNode;

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator &TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::operator++()
{
   if (!m_pNode)
   {
      return *this;
   }

   m_pNode = m_pNode->m_pNext;

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::operator++(int)
{
   Iterator result(*this);

   this->operator++();

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator &TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::operator+=(
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

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::operator+(
   const size_t value)
{
   Iterator result = *this;

   result += value;

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::operator==(
   const Iterator &rhs) const
{
   return m_pNode == rhs.m_pNode;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::operator!=(
   const Iterator &rhs) const
{
   return !(*this == rhs);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::operator *()
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
const typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::operator *() const
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::operator ->()
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
const typename TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::value_type *TIntrusiveMultiMap<T,K,TtoK,Pr,TtoN,TtoKS>::NodeCollection::Iterator::operator ->() const
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
