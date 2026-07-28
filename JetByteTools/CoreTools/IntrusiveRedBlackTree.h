#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveRedBlackTree.h
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

#if JETBYTE_ADMIN_SHOW_DERIVED_THIRD_PARTY_CODE_LICENCES  == 1
#pragma JETBYTE_MESSAGE("Build configuration: Based on original public domain source code from: http://en.literateprograms.org/Red-black_tree_(C)?oldid=19567")
#pragma JETBYTE_MESSAGE("Build configuration: With ideas from original public domain source code from:: http://www.eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx")
#pragma JETBYTE_MESSAGE("Build configuration: License: Public Domain")
#endif

#include "IntrusiveRedBlackTreeNode.h"

#include "Exception.h"
#include "ExceptionLeakPrevention.h"
#include "ToString.h"
#include "tstring.h"

#if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1 || JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
#include "DebugTrace.h"
#endif

#include <functional>      // for std::less<>
#include <vector>


///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveRedBlackTree
///////////////////////////////////////////////////////////////////////////////

template <class T>
class TIntrusiveRedBlackTreeNodeIsBaseClass
{
   public :

      static CIntrusiveRedBlackTreeNode * GetNodeFromT(
         const T *pT)
      {
         auto *pNode = const_cast<CIntrusiveRedBlackTreeNode *>(static_cast<const CIntrusiveRedBlackTreeNode *>(pT));

         return pNode;
      }

      static T *GetTFromNode(
         const CIntrusiveRedBlackTreeNode *pNode)
      {
         return const_cast<T*>(static_cast<const T*>(pNode));
      }
};

template <class T, class TtoK>
class TIntrusiveRedBlackTreeKeyIsSimpleToPrint
{
   public:

      static _tstring GetKeyAsStringFromT(
         const T *pT)
      {
         return ToString(TtoK::GetKeyFromT(pT));
      }
};

// Note that this only works if the node member is public or if T is a friend of this
// class.
// Use like this:
//   TIntrusiveRedBlackTreeNodeIsEmbeddedMember<MyClass, &MyClass::m_node>

template <class T, CIntrusiveRedBlackTreeNode T::*memberPointer>
class TIntrusiveRedBlackTreeNodeIsEmbeddedMember
{
   public :

   static CIntrusiveRedBlackTreeNode * GetNodeFromT(
      const T *pT)
   {
      CIntrusiveRedBlackTreeNode *pNode = &(const_cast<T*>(pT)->*memberPointer);

      return pNode;
   }

   static T *GetTFromNode(
      const CIntrusiveRedBlackTreeNode *pNode)
   {
      CIntrusiveRedBlackTreeNode *pNodeOffset = &(reinterpret_cast<T*>(0)->*memberPointer);

      const ULONG_PTR offset = reinterpret_cast<ULONG_PTR>(pNodeOffset);

      T *pT = const_cast<T *>(reinterpret_cast<const T *>(reinterpret_cast<const char *>(pNode) - offset));

      return pT;
   }
};

template <
   class T,                                                          // The type to store
   class K,                                                          // The type of the key
   class TtoK,                                                       // Functions to access a key from a T
   class Pr = std::less<K>,                                          // Predicate
   class TtoN = TIntrusiveRedBlackTreeNodeIsBaseClass<T>,            // Functions to access the node from a T
   class TtoKS = TIntrusiveRedBlackTreeKeyIsSimpleToPrint<T, TtoK> > // A function to convert the key into a printed version for dumping
class TIntrusiveRedBlackTree
{
   public :

      typedef T value_type;

      class Iterator;

      typedef std::pair<Iterator, bool> pairib;

      TIntrusiveRedBlackTree();

      TIntrusiveRedBlackTree(
         const TIntrusiveRedBlackTree &rhs) = delete;

      ~TIntrusiveRedBlackTree();

      TIntrusiveRedBlackTree &operator=(
         const TIntrusiveRedBlackTree &rhs) = delete;

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
      void EnableValidation();
      #endif

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ALLOW_UNCHECKED_ITERATORS == 1
      void AllowUncheckedIterators();
      #endif

      bool IsEmpty() const;

      size_t Size() const;

      pairib Insert(
         const T *pItemToInsert);

      pairib Insert(
         const T *pItemToInsert,
         const K &key);

      Iterator Find(
         const K &key) const;

      Iterator GetIteratorFromEntry(
         const T &entry) const;

      Iterator LowerBound(
         const K &key) const;

      //Iterator UpperBound(
      //   const K &key) const;

      T *Remove(
         const K &key);

      bool Erase(
         const Iterator &it);

      bool Erase(
         const T *pDataToErase);

      using ClearCallback = std::function<void(T *)>;

      enum class ClearFlags : BYTE
      {
         Erase,
         Fast,
         FastAndDirty
      };

      void Clear(
         ClearFlags flags = ClearFlags::Erase,
         const ClearCallback &clearCallback = nullptr);

      static bool IsInTree(
         const CIntrusiveRedBlackTreeNode *pNode);

      Iterator CreateIterator(
         const T *pItem) const;

      void SwapNode(
         const CIntrusiveRedBlackTreeNode *pSourceNode,
         CIntrusiveRedBlackTreeNode *pDestNode);

      class Iterator
      {
         public :

            typedef TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN, TtoKS> Tree;

            Iterator(
               const Iterator &rhs);

            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
            bool IsValid() const;
            #endif

            K Key() const;

            Iterator &operator=(
               const Iterator &rhs);

            Iterator &operator++();    // prefix
            Iterator operator++(int);  //postfix

            Iterator &operator+=(
               size_t value);

            Iterator operator+(
               size_t value) const;

            Iterator &operator--();    // prefix
            Iterator operator--(int);  //postfix

            Iterator &operator-=(
               size_t value);

            Iterator operator-(
               size_t value) const;

            bool operator==(const Iterator &rhs) const;

            bool operator!=(const Iterator &rhs) const;

            value_type * operator*();

            const value_type * operator*() const;

            value_type * operator->();

            const value_type * operator->() const;

         private :

            friend class TIntrusiveRedBlackTree;

            typedef Pr key_compare;
            typedef TtoN node_accessor;
            typedef TtoK key_accessor;

            Iterator();

            Iterator(
               const Tree &tree,
               CIntrusiveRedBlackTreeNode *pNode);

            CIntrusiveRedBlackTreeNode *m_pNode;

            enum NextMove : BYTE
            {
               GoUp,
               GoRight
            };

            NextMove m_nextMove;

            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
            const Tree *m_pTree;

            size_t m_treeChangeNumber;
            #endif
      };

      Iterator Begin() const;

      Iterator RBegin() const;

      Iterator End() const;

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_VALIDATION == 1
      using ValidateNodeFnc = void(const _tstring &callingFunction, const T *pNode, ULONG_PTR userData);

      void ValidateTree(
         ValidateNodeFnc *pValidateNodeFnc = nullptr,
         ULONG_PTR userData = 0) const;

      void ValidateTree(
         const _tstring &callingFunction,
         ValidateNodeFnc *pValidateNodeFnc = nullptr,
         ULONG_PTR userData = 0) const;
      #endif

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
      static _tstring KeyAsString(
         const CIntrusiveRedBlackTreeNode *pNode);
      #endif

      using DumpCallback = std::function<_tstring(const T *)>;

      _tstring DumpTree(
         bool printNodeAddresses = true,
         bool allowIncorrectNodeCount = false,
         const DumpCallback &dumpCallback = nullptr) const;

   private :

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
      size_t GetCurrentChangeNumber() const;
      #endif

      pairib InternalInsert(
         CIntrusiveRedBlackTreeNode *pNode,
         const K &newKey);

      void ReplaceNode(
         const CIntrusiveRedBlackTreeNode *pOldNode,
         CIntrusiveRedBlackTreeNode *pNewNode);

      void Rotate(
         CIntrusiveRedBlackTreeNode *pNode,
         int dir);

      void InsertRebalance(
         CIntrusiveRedBlackTreeNode *pNode);

      bool InternalErase(
         CIntrusiveRedBlackTreeNode *pNode);

      void DeleteRebalance(
         CIntrusiveRedBlackTreeNode *pNode);

      static bool IsBlack(
         const CIntrusiveRedBlackTreeNode *pNode);

      static bool IsRed(
         const CIntrusiveRedBlackTreeNode *pNode);

      static CIntrusiveRedBlackTreeNode *Sibling(
         const CIntrusiveRedBlackTreeNode *pNode);

      static CIntrusiveRedBlackTreeNode *Uncle(
         const CIntrusiveRedBlackTreeNode *pNode);

      static CIntrusiveRedBlackTreeNode *GrandParent(
         const CIntrusiveRedBlackTreeNode *pNode);

      typedef Pr key_compare;
      typedef TtoN node_accessor;
      typedef TtoK key_accessor;
      typedef TtoKS key_printer;

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_VALIDATION == 1
      static int ValidateTree(
         const _tstring &callingFunction,
         CIntrusiveRedBlackTreeNode *pRoot,
         ValidateNodeFnc *pValidateNodeFnc,
         ULONG_PTR userData);
      #endif

      CIntrusiveRedBlackTreeNode *m_pRoot;

      size_t m_size;

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
      size_t m_changeNumber;
      #endif

      key_compare m_comp;

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
      mutable bool m_isValid;
      #endif

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
      mutable std::deque<_tstring> m_previousOperations;
      mutable _tstring m_previousDump;
      #endif
      bool m_validationEnabled;
      #endif

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ALLOW_UNCHECKED_ITERATORS == 1
      bool m_iteratorsAreChecked;
      #endif
      #endif
};

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::TIntrusiveRedBlackTree()
   :  m_pRoot(nullptr),
      m_size(0),
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
      m_changeNumber(0),
      #endif
      m_comp()
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
      , m_isValid(true)
      #endif
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
      , m_validationEnabled(false)
      #endif
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ALLOW_UNCHECKED_ITERATORS == 1
      , m_iteratorsAreChecked(true)
      #endif
      #endif
{
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::~TIntrusiveRedBlackTree()
{
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START

   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
   if (m_isValid)
   {
      Clear();
   }
   #else
   Clear();
   #endif

   m_pRoot = nullptr;

   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Clear(
   const ClearFlags flags,
   const ClearCallback &clearCallback)
{
   if (flags != ClearFlags::FastAndDirty || clearCallback)        // have to iterate if callback supplied
   {
      Iterator it = Begin();

      const Iterator end = End();

      typedef std::vector<CIntrusiveRedBlackTreeNode *> Nodes;

      Nodes nodes;

      size_t i = 0;

      if (flags == ClearFlags::Fast)
      {
         nodes.resize(Size());
      }

      while (it != end)
      {
         CIntrusiveRedBlackTreeNode *pNode = it.m_pNode;

         if (flags == ClearFlags::Erase)
         {
            Erase(it);

            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
            if (IsInTree(pNode))
            {
               throw CException(
                  _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Clear()"),
                  _T("Removed node is still in the tree"));
            }
            #endif
         }
         else if (flags == ClearFlags::Fast)
         {
            nodes[i] = pNode;
            ++i;
         }

         if (flags == ClearFlags::Erase)
         {
            it = Begin();              // iterators are invalidated after erase
         }
         else
         {
            ++it;
         }

         if (clearCallback)
         {
            clearCallback(node_accessor::GetTFromNode(pNode));
         }
      }

      if (flags == ClearFlags::Fast)
      {
         for (auto *pNode : nodes)
         {
            pNode->ResetNode();
         }
      }
   }

   if (flags != ClearFlags::Erase)
   {
      m_size = 0;

      m_pRoot = nullptr;

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
      ++m_changeNumber;
      #endif
   }

   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
   if (m_validationEnabled)
   {
      m_previousOperations.emplace_back(_T("Clear"));
   }
   #endif
}

#if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
size_t TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::GetCurrentChangeNumber() const
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ALLOW_UNCHECKED_ITERATORS == 1
   if (!m_iteratorsAreChecked)
   {
      return 0;
   }
   #endif

   return m_changeNumber;
}
#endif

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::CreateIterator(
   const T *pItem) const
{
   return Iterator(*this, node_accessor::GetNodeFromT(pItem));
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::SwapNode(
   const CIntrusiveRedBlackTreeNode *pSourceNode,
   CIntrusiveRedBlackTreeNode *pDestNode)
{
   CIntrusiveRedBlackTreeNode *pParent = CIntrusiveRedBlackTreeNode::GetParent(pSourceNode);

   if (!pParent)
   {
      m_pRoot = pDestNode;
   }
   else
   {
      const int dir = (pSourceNode == pParent->m_pLinks[1]);

      pParent->m_pLinks[dir] = pDestNode;
   }

   // also copies red/black state in parent pointer...
   pDestNode->m_pParent = pSourceNode->m_pParent;

   if (pSourceNode->m_pLinks[0])
   {
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
      if (CIntrusiveRedBlackTreeNode::GetParent(pSourceNode->m_pLinks[0]) != pSourceNode)
      {
         throw CException(
            _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::SwapNode()"),
            _T("Unexpected parent of child node"));
      }
      #endif

      CIntrusiveRedBlackTreeNode::SetParent(pSourceNode->m_pLinks[0], pDestNode);
   }

   pDestNode->m_pLinks[0] = pSourceNode->m_pLinks[0];

   if (pSourceNode->m_pLinks[1])
   {
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
      if (CIntrusiveRedBlackTreeNode::GetParent(pSourceNode->m_pLinks[1]) != pSourceNode)
      {
         throw CException(
            _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::SwapNode()"),
            _T("Unexpected parent of child node"));
      }
      #endif

      CIntrusiveRedBlackTreeNode::SetParent(pSourceNode->m_pLinks[1], pDestNode);
   }

   pDestNode->m_pLinks[1] = pSourceNode->m_pLinks[1];

   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
   if (m_validationEnabled)
   {
      m_previousOperations.emplace_back(_T("SwapNode: ") + PointerToString(pSourceNode) + _T(" -> ") + PointerToString(pDestNode));
   }
   #endif
}

#if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::EnableValidation()
{
   m_validationEnabled = true;
}
#endif

#if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ALLOW_UNCHECKED_ITERATORS == 1
template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::AllowUncheckedIterators()
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
   m_iteratorsAreChecked = false;
   #endif
}
#endif

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::IsEmpty() const
{
   return m_size == 0;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
size_t TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Size() const
{
   return m_size;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Find(
   const K &key) const
{
   CIntrusiveRedBlackTreeNode *pIt = m_pRoot;

   while (pIt)
   {
      const K currentKey = key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pIt));

      const int dir = m_comp(currentKey, key);

      if (!dir && !m_comp(key, currentKey))    // !(a < b) && !(b < a) = (a==b)
      {
         break;
      }

      pIt = pIt->m_pLinks[dir];
   }

   return Iterator(*this, pIt);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::GetIteratorFromEntry(
   const T &entry) const
{
   CIntrusiveRedBlackTreeNode *pNode = node_accessor::GetNodeFromT(&entry);

   if (IsInTree(pNode))
   {
      return Iterator(*this, pNode);
   }

   return End();
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::LowerBound(
   const K &key) const
{
   // Returns an iterator pointing to the first element in the container which is not considered to go before val (i.e., either it is equivalent or goes after).

   CIntrusiveRedBlackTreeNode *pIt = m_pRoot;

   while (pIt)
   {
      const K currentKey = key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pIt));

      //const auto keyString = key_printer::GetKeyAsStringFromT(node_accessor::GetTFromNode(pIt));

      const int dir = m_comp(currentKey, key);

      if (!dir && !m_comp(key, currentKey))    // !(a < b) && !(b < a) = (a==b)
      {
         break;
      }

      if (!pIt->m_pLinks[dir])
      {
         if (dir)                // (currentKey < key)
         {
            auto result = Iterator(*this, pIt);

            ++result;

            return result;
         }

         break;
      }

      pIt = pIt->m_pLinks[dir];
   }

   return Iterator(*this, pIt);
}

//template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
//typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::UpperBound(
//   const K &key) const
//{
//   // Not implemented - upper bound is harder than lower bound, and we don't currently need it
//}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::ReplaceNode(
   const CIntrusiveRedBlackTreeNode *pOldNode,
   CIntrusiveRedBlackTreeNode *pNewNode)
{
   CIntrusiveRedBlackTreeNode *pParent = CIntrusiveRedBlackTreeNode::GetParent(pOldNode);

   if (!pParent)
   {
      m_pRoot = pNewNode;
   }
   else
   {
      const int dir = (pOldNode == pParent->m_pLinks[1]);

      pParent->m_pLinks[dir] = pNewNode;
   }

   if (pNewNode)
   {
      CIntrusiveRedBlackTreeNode::SetParent(pNewNode, pParent);
   }

   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
   if (m_validationEnabled)
   {
      m_previousOperations.emplace_back(_T("ReplaceNode: ") + PointerToString(pOldNode) + _T(" -> ") + PointerToString(pNewNode));
   }
   #endif
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Rotate(
   CIntrusiveRedBlackTreeNode *pNode,
   const int dir)
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
   const _tstring direction = (!dir ? _T(" left") : _T(" right"));
   OutputEx(_T("Rotate: ") + KeyAsString(pNode) + direction);
   #endif

   CIntrusiveRedBlackTreeNode *pTemp = pNode->m_pLinks[!dir];

   ReplaceNode(pNode, pTemp);

   pNode->m_pLinks[!dir] = pTemp->m_pLinks[dir];

   if (pTemp->m_pLinks[dir])
   {
      CIntrusiveRedBlackTreeNode::SetParent(pTemp->m_pLinks[dir], pNode);
   }

   pTemp->m_pLinks[dir] = pNode;
   CIntrusiveRedBlackTreeNode::SetParent(pNode, pTemp);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::IsInTree(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   //return !CIntrusiveRedBlackTreeNode::IsRed(pNode) || CIntrusiveRedBlackTreeNode::GetParent(pNode) || pNode->m_pLinks[0] || pNode->m_pLinks[1];
   return pNode->IsActive();
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::IsBlack(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   return pNode ? !CIntrusiveRedBlackTreeNode::IsRed(pNode) : true;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::IsRed(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   return pNode ? CIntrusiveRedBlackTreeNode::IsRed(pNode) : false;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
CIntrusiveRedBlackTreeNode *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Sibling(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (!pNode)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Sibling()"),
         _T("pNode is null"));
   }
   #endif

   const CIntrusiveRedBlackTreeNode *pParent = CIntrusiveRedBlackTreeNode::GetParent(pNode);

   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (!pParent)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Sibling()"),
         _T("pNode->m_pParent is null"));
   }
   #endif

   const int dir = (pNode == pParent->m_pLinks[1]);

   return pParent->m_pLinks[!dir];
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
CIntrusiveRedBlackTreeNode *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Uncle(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   const CIntrusiveRedBlackTreeNode *pParent = CIntrusiveRedBlackTreeNode::GetParent(pNode);

   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (!pNode)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Uncle()"),
         _T("pNode is null"));
   }

   if (!pParent)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Uncle()"),
         _T("pNode->m_pParent is null"));
   }

   if (!CIntrusiveRedBlackTreeNode::GetParent(pParent))
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Uncle()"),
         _T("pNode->m_pParent->m_pParent is null"));
   }
   #endif

   return Sibling(pParent);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
CIntrusiveRedBlackTreeNode *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::GrandParent(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   const CIntrusiveRedBlackTreeNode *pParent = CIntrusiveRedBlackTreeNode::GetParent(pNode);

   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (!pNode)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::GrandParent()"),
         _T("pNode is null"));
   }

   if (!pParent)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::GrandParent()"),
         _T("pNode->m_pParent is null"));
   }

   if (!CIntrusiveRedBlackTreeNode::GetParent(pParent))
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::GrandParent()"),
         _T("pNode->m_pParent->m_pParent is null"));
   }
   #endif

   return CIntrusiveRedBlackTreeNode::GetParent(pParent);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::pairib TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Insert(
   const T *pItemToInsert)
{
   if (!pItemToInsert)
   {
      throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Insert()"), _T("Node is null"));
   }

   CIntrusiveRedBlackTreeNode *pNode = node_accessor::GetNodeFromT(pItemToInsert);

   if (IsInTree(pNode))
   {
      throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Insert()"), _T("Node is already in a tree"));
   }

   K key = K();

   if (m_pRoot)
   {
      key = key_accessor::GetKeyFromT(pItemToInsert);
   }

   // This case is really designed for expensive to copy keys, like strings, where the 'node' includes space to
   // store the key and the key can be 'set' in the node at this point. Ideally we'd use move semantics to
   // pass the key in to set it and always use a key accessor which returns a const ref to the key.

   // As it is this function can lead to the tree being invalid if the key supplied is not the key returned
   // by the key accessor. Use with care.

   return InternalInsert(pNode, key);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::pairib TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Insert(
   const T *pItemToInsert,
   const K &key)
{
   if (!pItemToInsert)
   {
      throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Insert()"), _T("Node is null"));
   }

   if (key != key_accessor::GetKeyFromT(pItemToInsert))
   {
      throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Insert()"), _T("Supplied key is not the same as extracted key"));
   }

   CIntrusiveRedBlackTreeNode *pNode = node_accessor::GetNodeFromT(pItemToInsert);

   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (IsInTree(pNode))
   {
      throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Insert()"), _T("Node is already in a tree"));
   }
   #endif

   return InternalInsert(pNode, key);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::pairib TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::InternalInsert(
   CIntrusiveRedBlackTreeNode *pNode,
   const K &newKey)
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (!IsRed(pNode))
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::InternalInsert()"),
         _T("Unexpected node state"));
   }
   #endif

   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
   if (m_validationEnabled)
   {
      ValidateTree(_T("InternalInsert-pre - ") + PointerToString(pNode));

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
      m_previousDump = DumpTree();
      #endif
   }
   #endif

   bool inserted = false;

   CIntrusiveRedBlackTreeNode *pIt = m_pRoot;

   if (!pIt)
   {
      // We have an empty tree; attach the new node directly to the root

      m_pRoot = pNode;

      pIt = m_pRoot;

      inserted = true;
   }
   else
   {
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
      OutputEx(_T("Insert: ") + ToString(newKey));
      #endif

      // Search down the tree for a place to insert

      while (!inserted &&
             pIt != pNode)
      {
         const K currentKey = key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pIt));

         const int dir = m_comp(currentKey, newKey);

         if (dir == 0 && !m_comp(newKey, currentKey))
         {
            // We have found the key in the tree already, return an iterator
            // to the node we found.

            pNode = pIt;
         }
         else
         {
            if (!pIt->m_pLinks[dir])
            {
               // Insert the new node here

               pIt->m_pLinks[dir] = pNode;

               CIntrusiveRedBlackTreeNode::SetParent(pNode, pIt);
               CIntrusiveRedBlackTreeNode::MakeRed(pNode);           // stay red...

               pIt = pNode;

               inserted = true;
            }
            else
            {
               // continue down the tree

               pIt = pIt->m_pLinks[dir];
            }
         }
      }
   }

   if (inserted)
   {
      // We set the root colour and increment the size here so that
      // when tree dumping is enabled the tree walks and validations are
      // successful during rebalancing.

      CIntrusiveRedBlackTreeNode::MakeBlack(m_pRoot);

      m_size++;

      if (m_size > 1)
      {
         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(_T("Rebalance: ") + KeyAsString(pIt));
         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
         OutputEx(DumpTree());
         #endif
         #endif

         InsertRebalance(pIt);

         CIntrusiveRedBlackTreeNode::MakeBlack(m_pRoot);
      }

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
      OutputEx(_T("Insert done"));
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
      OutputEx(DumpTree());
      #endif
      #endif

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
      if (m_validationEnabled)
      {
         ValidateTree(_T("InternalInsert-post - ") + PointerToString(pNode));

         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
         m_previousDump = DumpTree();
         #endif
      }
      #endif

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
      ++m_changeNumber;
      #endif
   }

   return pairib(Iterator(*this, pNode), inserted);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::InsertRebalance(
   CIntrusiveRedBlackTreeNode *pNode)
{
   bool done = false;

   while (!done)
   {
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
      OutputEx(_T("Rebalancing: ") + KeyAsString(pNode));
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
      OutputEx(DumpTree());
      #endif
      #endif

      CIntrusiveRedBlackTreeNode *pParent = CIntrusiveRedBlackTreeNode::GetParent(pNode);

      if (!pParent)
      {
         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(_T("Parent is null: ") + KeyAsString(pNode));
         #endif

         CIntrusiveRedBlackTreeNode::MakeBlack(pNode);                 // insert case 1

         done = true;
      }
      else
      {
         if (IsBlack(pParent))
         {
            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            if (pParent)
            {
               OutputEx(_T("Parent is black: ") + KeyAsString(pNode) +
                  _T(" P: ") + KeyAsString(pParent));
            }
            else
            {
               OutputEx(_T("Parent is black: ") + KeyAsString(pNode));
            }
            #endif

            // insert case 2

            done = true;
         }
         else
         {
            if (IsRed(Uncle(pNode)))
            {
               #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
               OutputEx(_T("Uncle is red: ") + KeyAsString(pNode));
               #endif

               // insert case 3

               CIntrusiveRedBlackTreeNode::MakeBlack(pParent);
               CIntrusiveRedBlackTreeNode::MakeBlack(Uncle(pNode));

               CIntrusiveRedBlackTreeNode *pGrandParent = CIntrusiveRedBlackTreeNode::GetParent(pParent);

               CIntrusiveRedBlackTreeNode::MakeRed(pGrandParent);

               pNode = pGrandParent;

               // back to case 1
            }
            else
            {
               #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
               OutputEx(_T("Case 4: ") + KeyAsString(pNode));
               #endif

               // insert case 4

               CIntrusiveRedBlackTreeNode *pParent = CIntrusiveRedBlackTreeNode::GetParent(pNode);

               int dir = (pNode == pParent->m_pLinks[1]);

               int parentDir = (pParent == GrandParent(pNode)->m_pLinks[1]);

               if (dir != parentDir)
               {
                  Rotate(pParent, !dir);

                  pNode = pNode->m_pLinks[!dir];
               }

               // fall through to case 5
               // insert case 5

               pParent = CIntrusiveRedBlackTreeNode::GetParent(pNode);

               CIntrusiveRedBlackTreeNode *pGrandParent = CIntrusiveRedBlackTreeNode::GetParent(pParent);

               #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
               OutputEx(_T("Case 5: ") + KeyAsString(pNode));
               #endif

               CIntrusiveRedBlackTreeNode::MakeBlack(pParent);

               CIntrusiveRedBlackTreeNode::MakeRed(pGrandParent);

               // These two may have changed if we moved pNode in case 4...

               dir = (pNode == pParent->m_pLinks[1]);

               parentDir = (pParent == pGrandParent->m_pLinks[1]);

               if (dir == parentDir)
               {
                  Rotate(pGrandParent, !dir);
               }

               done = true;
            }
         }
      }
   }
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
T *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Remove(
   const K &key)
{
   T *pData = nullptr;

   const Iterator it = Find(key);

   if (it != End())
   {
      pData = node_accessor::GetTFromNode(it.m_pNode);

      Erase(it);
   }

   return pData;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Erase(
   const Iterator &it)
{
   return InternalErase(it.m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Erase(
   const T *pDataToErase)
{
   return InternalErase(node_accessor::GetNodeFromT(pDataToErase));
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::InternalErase(
   CIntrusiveRedBlackTreeNode *pNode)
{
   bool erased = false;

   if (pNode && IsInTree(pNode))
   {
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
      if (m_validationEnabled)
      {
         m_previousOperations.emplace_back(_T("InternalErase-pre - ") + PointerToString(pNode));
      }
      #endif

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
      OutputEx(_T("Erase: ") + KeyAsString(pNode));
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
      OutputEx(DumpTree());
      #endif
      #endif

      if (pNode->m_pLinks[0] && pNode->m_pLinks[1])
      {
         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(_T("Two children: ") + KeyAsString(pNode));
         #endif

         // Two children... This is the "hard" case. Simplify by
         // converting this to a one child case by swapping the node we need
         // to remove with its predecessor in the tree. We then have a "one child"
         // deletion to perform on this node and the predecessor node has two
         // children.

         // Find the predecessor node

         CIntrusiveRedBlackTreeNode *pPrev = pNode->m_pLinks[0];

         while (pPrev->m_pLinks[1])
         {
            pPrev = pPrev->m_pLinks[1];
         }

         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(_T("Prev: ") + KeyAsString(pPrev));
         #endif

         const bool prevIsRed = CIntrusiveRedBlackTreeNode::IsRed(pPrev);

         CIntrusiveRedBlackTreeNode::CopyColour(pPrev, pNode);

         CIntrusiveRedBlackTreeNode::SetColourAs(pNode, prevIsRed);

         // splice our right subtree (which must exist as we have two children)
         // on to the prev node (which can't have a right subtree of its own as
         // we are the node which would appear on its right)

         pPrev->m_pLinks[1] = pNode->m_pLinks[1];
         CIntrusiveRedBlackTreeNode::SetParent(pPrev->m_pLinks[1], pPrev);
         pNode->m_pLinks[1] = nullptr;

         // swap our parent with our prev node's parent

         CIntrusiveRedBlackTreeNode *pPrevParent = CIntrusiveRedBlackTreeNode::GetParent(pPrev);

         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(_T("Prev Parent: ") + KeyAsString(pPrevParent));
         #endif

         // The prev node MUST have a parent, prev node's parent
         // might be us. We might not have a parent as we might
         // be the root of the tree.

         CIntrusiveRedBlackTreeNode *pParent = CIntrusiveRedBlackTreeNode::GetParent(pNode);

         if (pParent)
         {
            const int dir = (pNode == pParent->m_pLinks[1]);

            pParent->m_pLinks[dir] = pPrev;
         }
         else
         {
            m_pRoot = pPrev;
         }

         CIntrusiveRedBlackTreeNode::SetParent(pPrev, pParent);

         if (pPrevParent == pNode)
         {
            // we are the prev node's parent...
            // it can only be our left node

            CIntrusiveRedBlackTreeNode::SetParent(pNode, pPrev);

            pNode->m_pLinks[0] = pPrev->m_pLinks[0];

            if (pNode->m_pLinks[0])
            {
               CIntrusiveRedBlackTreeNode::SetParent(pNode->m_pLinks[0], pNode);
            }

            pPrev->m_pLinks[0] = pNode;
         }
         else
         {
            const int dir = (pPrev == pPrevParent->m_pLinks[1]);

            CIntrusiveRedBlackTreeNode::SetParent(pNode, pPrevParent);
            pPrevParent->m_pLinks[dir] = pNode;

            // swap our left subtree with our prev node

            CIntrusiveRedBlackTreeNode *pPrevLeft = pPrev->m_pLinks[0];

            pPrev->m_pLinks[0] = pNode->m_pLinks[0];
            CIntrusiveRedBlackTreeNode::SetParent(pPrev->m_pLinks[0], pPrev);

            pNode->m_pLinks[0] = pPrevLeft;

            if (pNode->m_pLinks[0])
            {
               CIntrusiveRedBlackTreeNode::SetParent(pNode->m_pLinks[0], pNode);
            }
         }
      }

      // The node we need to delete has, at most, one child.

      CIntrusiveRedBlackTreeNode *pChild = !pNode->m_pLinks[1] ? pNode->m_pLinks[0] : pNode->m_pLinks[1];

      if (IsBlack(pNode))
      {
         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         if (pChild)
         {
            OutputEx(_T("Rebalance: ") + KeyAsString(pNode) + _T(" Child: ") + KeyAsString(pChild));
         }
         else
         {
            OutputEx(_T("Rebalance: ") + KeyAsString(pNode));
         }
         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
         OutputEx(DumpTree());
         #endif
         #endif

         CIntrusiveRedBlackTreeNode::SetColourAs(pNode, IsRed(pChild));

         DeleteRebalance(pNode);
      }

      ReplaceNode(pNode, pChild);

      pNode->ResetNode();

      m_size--;

      // Root must be black

      if (m_pRoot)
      {
         CIntrusiveRedBlackTreeNode::MakeBlack(m_pRoot);
      }

      erased = true;

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
      ++m_changeNumber;
      #endif

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
      if (IsInTree(pNode))
      {
         throw CException(
            _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::InternalErase()"),
            _T("Node is still in tree after erase"));
      }
      #endif
   }

   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
   OutputEx(_T("Remove complete:"));
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
   OutputEx(DumpTree());
   #endif
   #endif

   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
   if (m_validationEnabled)
   {
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION_GENERATE_OPERATION_TRACE == 1
      m_previousOperations.emplace_back(_T("InternalErase-post - ") + PointerToString(pNode));
      #endif
      ValidateTree();
   }
   #endif

   return erased;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::DeleteRebalance(
   CIntrusiveRedBlackTreeNode *pNode)
{
   try
   {
      bool done = false;

      while (!done)
      {
         done = true;

         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(_T("Rebalancing: ") + KeyAsString(pNode));
         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
         OutputEx(DumpTree());
         #endif
         #endif

         CIntrusiveRedBlackTreeNode *pParent = CIntrusiveRedBlackTreeNode::GetParent(pNode);

         if (!pParent)
         {
            // delete case 1

            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(_T("Parent is null: ") + KeyAsString(pNode));
            #endif

            break;
         }

         CIntrusiveRedBlackTreeNode *pSibling = Sibling(pNode);

         if (pSibling && IsRed(pSibling))
         {
            // delete case 2

            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(_T("Case 2: ") + KeyAsString(pNode) + _T(" Sibling: ") + KeyAsString(pSibling));
            #endif

            CIntrusiveRedBlackTreeNode::MakeRed(pParent);
            CIntrusiveRedBlackTreeNode::MakeBlack(pSibling);

            const int dir = (pNode == pParent->m_pLinks[1]);

            Rotate(pParent, dir);

            pSibling = Sibling(pNode);
         }

         pParent = CIntrusiveRedBlackTreeNode::GetParent(pNode);

         if (pSibling &&
             IsBlack(pParent) &&
             IsBlack(pSibling) &&
             IsBlack(pSibling->m_pLinks[0]) &&
             IsBlack(pSibling->m_pLinks[1]))
         {
            // delete case 3

            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(_T("Case 3: ") + KeyAsString(pNode) + _T(" Sibling: ") + KeyAsString(pSibling));
            #endif

            CIntrusiveRedBlackTreeNode::MakeRed(pSibling);

            // back to case 1

            pNode = pParent;

            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(_T("Back to case 1: ") + KeyAsString(pNode));
            #endif

            done = false;
         }
         else if (pSibling)
         {
            if (IsRed(pParent) &&
                IsBlack(pSibling) &&
                IsBlack(pSibling->m_pLinks[0]) &&
                IsBlack(pSibling->m_pLinks[1]))
            {
               // delete case 4

               #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
               OutputEx(_T("Case 4: ") + KeyAsString(pNode) + _T(" Sibling: ") + KeyAsString(pSibling));
               #endif

               CIntrusiveRedBlackTreeNode::MakeRed(pSibling);
               CIntrusiveRedBlackTreeNode::MakeBlack(pParent);
            }
            else
            {
               // delete case 5

               int dir = (pNode == pParent->m_pLinks[1]);

               if (IsBlack(pSibling) &&
                   IsRed(pSibling->m_pLinks[dir]) &&
                   IsBlack(pSibling->m_pLinks[!dir]))
               {
                  #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
                  OutputEx(_T("Case 5: ") + KeyAsString(pNode) + _T(" Sibling: ") + KeyAsString(pSibling));
                  #endif

                  CIntrusiveRedBlackTreeNode::MakeRed(pSibling);
                  CIntrusiveRedBlackTreeNode::MakeBlack(pSibling->m_pLinks[dir]);
                  Rotate(pSibling, !dir);

                  pSibling = Sibling(pNode);
               }

               // fall through to case 6

               if (pSibling)
               {
                  // delete case 6

                  #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
                  OutputEx(_T("Case 6: ") + KeyAsString(pNode) + _T(" Sibling: ") + KeyAsString(pSibling));
                  #endif

                  pParent = CIntrusiveRedBlackTreeNode::GetParent(pNode);

                  CIntrusiveRedBlackTreeNode::CopyColour(pSibling, pParent);
                  CIntrusiveRedBlackTreeNode::MakeBlack(pParent);

                  dir = (pNode == pParent->m_pLinks[1]);

                  #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
                  if (!IsRed(pSibling->m_pLinks[!dir]))
                  {
                     throw CException(
                        _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::DeleteRebalance()"),
                        _T("Unexpected tree state"));
                  }
                  #endif

                  CIntrusiveRedBlackTreeNode::MakeBlack(pSibling->m_pLinks[!dir]);

                  Rotate(pParent, dir);
               }
            }
         }
      }
   }
   catch (...)
   {
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
      m_isValid = false;
      #endif

      throw;
   }
}

#if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
_tstring TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::KeyAsString(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   return ToString(key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pNode)));
}
#endif

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
_tstring TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::DumpTree(
   const bool printNodeAddresses,
   const bool allowIncorrectNodeCount,
   const DumpCallback &dumpCallback) const
{
   _tstring result;

   size_t numNodes = 0;

   size_t depth = 0;

   // We can't use iterators here as we need to track the depth in the tree and this means that
   // all iterators are invalid after an erase, and we don't want that.
   // So, instead, we're doing a manual walk of the tree...

   CIntrusiveRedBlackTreeNode *pNode = m_pRoot;

   // 1) go as far left as possible

   while (pNode)
   {
      if (pNode->m_pLinks[0])
      {
         pNode = pNode->m_pLinks[0];
         ++depth;
      }
      else
      {
         break;
      }
   }

   // now iterate

   typename Iterator::NextMove nextMove = Iterator::NextMove::GoRight;

   while (pNode)
   {
      // process this node...

      const _tstring colour = pNode ? (IsRed(pNode) ? _T("r") : _T("b")) : _T("*");

      const T *pT = node_accessor::GetTFromNode(pNode);

      _tstring nodeDetails;

      if (dumpCallback)
      {
         nodeDetails = dumpCallback(pT);
      }

      if (printNodeAddresses)
      {
         result += PointerToString(pNode) + _T(" - ");
      }

      result += _T("K[") + key_printer::GetKeyAsStringFromT(pT) + _T("]") + nodeDetails + _T("(") + ToString(depth) + _T(", ") + colour + _T("), ");

      ++numNodes;

      if (numNodes > m_size)
      {
         const _tstring message = _T("Dump of tree does not contain the right number of nodes: expected: ") + ToString(m_size) + _T(" got at least: ") + ToString(numNodes);

         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(message);

         OutputEx(result);
         #endif

         if (!allowIncorrectNodeCount)
         {
            throw CException(
               _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::DumpTree()"),
               message);
         }
      }

      // step to next node

      if (nextMove == Iterator::NextMove::GoRight)
      {
         if (pNode->m_pLinks[1])
         {
            // If we can go right, do so

            pNode = pNode->m_pLinks[1];
            ++depth;

            //  and then go as far left as we can

            if (pNode->m_pLinks[0])
            {
               while (pNode->m_pLinks[0])
               {
                  pNode = pNode->m_pLinks[0];
                  ++depth;
               }
            }

            continue;
         }

         nextMove = Iterator::NextMove::GoUp;
      }

      if (nextMove == Iterator::NextMove::GoUp)
      {
         if (CIntrusiveRedBlackTreeNode::GetParent(pNode))
         {
            while (pNode &&
                   CIntrusiveRedBlackTreeNode::GetParent(pNode) &&
                   nextMove == Iterator::NextMove::GoUp)
            {
               auto *pParent = CIntrusiveRedBlackTreeNode::GetParent(pNode);

               if (pNode == pParent->m_pLinks[0])
               {
                  // coming up from left

                  nextMove = Iterator::NextMove::GoRight;
               }

               pNode = pParent;
               --depth;
            }

            if (pNode && nextMove == Iterator::NextMove::GoUp)
            {
               pNode = nullptr;
            }

            continue;
         }
      }

      pNode = nullptr;
   }

   if (numNodes != m_size)
   {
      const _tstring message = _T("Dump of tree does not contain the right number of nodes: expected: ") + ToString(m_size) + _T(" got: ") + ToString(numNodes);

      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
      OutputEx(message);

      OutputEx(result);
      #endif

      if (!allowIncorrectNodeCount)
      {
         throw CException(
            _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::DumpTree()"),
            message);
      }
   }

   return result;
}

///////////////////////////////////////////////////////////////////////////////
// ValidateTree
///////////////////////////////////////////////////////////////////////////////

#if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_VALIDATION == 1
template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::ValidateTree(
   ValidateNodeFnc *pValidateNodeFnc,
   const ULONG_PTR userData) const
{
   ValidateTree(_T("ValidateTree()"), pValidateNodeFnc, userData);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::ValidateTree(
   const _tstring &callingFunction,
   ValidateNodeFnc *pValidateNodeFnc,
   const ULONG_PTR userData) const
{
   try
   {
      if (!IsBlack(m_pRoot))
      {
         const _tstring message = callingFunction + _T(" - Root must be black");

         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(message);
         #endif

         throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::ValidateTree()"), message);
      }

      // First walk the tree in order and make sure we dont get any loops

      size_t i = 0;

      for (Iterator it = Begin(), end = End(); it != end; ++it, ++i)
      {
         if (i > m_size)
         {
            const _tstring message = callingFunction + _T(" - A walk of the tree does not contain the right number of nodes: expected:") + ToString(m_size) + _T(" got at least: ") + ToString(i);

            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(message);
            #endif

            throw CException(
               _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::ValidateTree()"),
               message);
         }
      }

      if (i != m_size)
      {
         const _tstring message = callingFunction + _T(" - A walk of the tree does not contain the right number of nodes: expected:") + ToString(m_size) + _T(" got: ") + ToString(i);

         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(message);
         #endif

         throw CException(
            _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::ValidateTree()"),
            message);
      }

      try
      {
         (void)ValidateTree(callingFunction, m_pRoot, pValidateNodeFnc, userData);
      }
      catch (...)
      {
         #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(_T("Exception during tree validation"));
         OutputEx(DumpTree());
         #endif

         throw;
      }
   }
   catch (...)
   {
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
      m_isValid = false;
      #endif

      throw;
   }
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
int TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::ValidateTree(
   const _tstring &callingFunction,
   CIntrusiveRedBlackTreeNode *pRoot,
   ValidateNodeFnc *pValidateNodeFnc,
   ULONG_PTR userData)
{
   key_compare comp;

   if (pRoot)
   {
      CIntrusiveRedBlackTreeNode *pLeftNode = pRoot->m_pLinks[0];

      CIntrusiveRedBlackTreeNode *pRightNode = pRoot->m_pLinks[1];

      // Consecutive red links

      const bool rootIsRed = IsRed(pRoot);

      if (rootIsRed)
      {
         if (IsRed(pLeftNode) ||
             IsRed(pRightNode))
         {
            const _tstring message = callingFunction + _T(" - Red violation");

            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(message);
            #endif

            throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::ValidateTree()"), message);
         }
      }

      if (pValidateNodeFnc)
      {
         pValidateNodeFnc(callingFunction, node_accessor::GetTFromNode(pRoot), userData);
      }

      const int leftHeight = ValidateTree(callingFunction, pLeftNode, pValidateNodeFnc, userData);

      const int rightHeight = ValidateTree(callingFunction, pRightNode, pValidateNodeFnc, userData);

      // Invalid binary search tree

      const K rootKey = key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pRoot));

      if (pLeftNode)
      {
         const K leftKey = key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pLeftNode));

         if (comp(rootKey, leftKey))
         {
            const _tstring message = callingFunction + _T(" - Binary tree violation");

            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(message);
            #endif

            throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::ValidateTree()"), message);
         }
      }

      if (pRightNode)
      {
         const K rightKey = key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pRightNode));

         if (comp(rightKey, rootKey))
         {
            const _tstring message = callingFunction + _T(" - Binary tree violation");

            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(message);
            #endif

            throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::ValidateTree()"), message);
         }
      }

      // Black height mismatch

      if (leftHeight != 0 && rightHeight != 0 && leftHeight != rightHeight)
      {
            const _tstring message = callingFunction + _T(" - Black violation");

            #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(message);
            #endif

            throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::ValidateTree()"), message);
      }

      // Only count black links

      if (leftHeight != 0 && rightHeight != 0)
      {
         return rootIsRed ? leftHeight : leftHeight + 1;
      }
   }

   return 0;
}
#endif

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Begin() const
{
   CIntrusiveRedBlackTreeNode *pNode = m_pRoot;

   while (pNode)
   {
      if (pNode->m_pLinks[0])
      {
         pNode = pNode->m_pLinks[0];
      }
      else
      {
         break;
      }
   }

   return Iterator(*this, pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::RBegin() const
{
   CIntrusiveRedBlackTreeNode *pNode = m_pRoot;

   while (pNode)
   {
      if (pNode->m_pLinks[1])
      {
         pNode = pNode->m_pLinks[1];
      }
      else
      {
         break;
      }
   }

   return Iterator(*this, pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::End() const
{
   return Iterator();
}

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator
///////////////////////////////////////////////////////////////////////////////

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::Iterator()
   :  m_pNode(nullptr),
      m_nextMove(GoRight)
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
      , m_pTree(nullptr),
      m_treeChangeNumber(0)
      #endif
{
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
K TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::Key() const
{
   return key_accessor::GetKeyFromT(node_accessor::GetTFromNode(m_pNode));
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::Iterator(
   const Tree &tree,
   CIntrusiveRedBlackTreeNode *pNode)
   :  m_pNode(pNode),
      m_nextMove(GoRight)
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
      , m_pTree(&tree),
      m_treeChangeNumber(tree.GetCurrentChangeNumber())
      #endif
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION != 1
   (void)tree;
   #endif
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::Iterator(
   const Iterator &rhs)
   :  m_pNode(rhs.m_pNode),
      m_nextMove(rhs.m_nextMove)
      #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
      , m_pTree(rhs.m_pTree),
      m_treeChangeNumber(rhs.m_treeChangeNumber)
      #endif
{
}

#if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::IsValid() const
{
   return m_pTree && m_pTree->GetCurrentChangeNumber() == m_treeChangeNumber;
}
#endif

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator &TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator=(
   const Iterator &rhs)
{
   m_pNode = rhs.m_pNode;
   m_nextMove = rhs.m_nextMove;
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
   m_pTree = rhs.m_pTree;
   m_treeChangeNumber = rhs.m_treeChangeNumber;
   #endif

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator &TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator++()
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
   if (!IsValid())
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator++()"),
         _T("Iterator is not valid"));
   }
   #endif

   if (!m_pNode)
   {
      return *this;
   }

   if (m_nextMove == GoRight)
   {
      if (CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode)->m_pLinks[1])
      {
         // If we can go right, do so

         m_pNode = m_pNode->m_pLinks[1];

         //  and then go as far left as we can

         if (CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode)->m_pLinks[0])
         {
            while (CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode)->m_pLinks[0])
            {
               m_pNode = m_pNode->m_pLinks[0];
            }
         }

         return *this;
      }

      m_nextMove = GoUp;
   }

   if (m_nextMove == GoUp)
   {
      if (CIntrusiveRedBlackTreeNode *pParent = CIntrusiveRedBlackTreeNode::GetParent(m_pNode))
      {
         while (CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode) &&
                pParent &&
                m_nextMove == GoUp)
         {
            if (m_pNode == pParent->m_pLinks[0])
            {
               // coming up from left

               m_nextMove = GoRight;
            }

            m_pNode = pParent;

            pParent = CIntrusiveRedBlackTreeNode::GetParent(m_pNode);
         }

         if (CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode) && m_nextMove == GoUp)
         {
            m_pNode = nullptr;
            m_nextMove = GoRight;
         }

         return *this;
      }
   }

   m_pNode = nullptr;
   m_nextMove = GoRight;

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator++(int)
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
   if (!IsValid())
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator++(int)"),
         _T("Iterator is not valid"));
   }
   #endif

   Iterator result(*this);

   this->operator++();

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator &TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator+=(
   const size_t value)
{
   size_t added = 0;

   while (CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode) && added != value)
   {
      operator++();

      added++;
   }

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator+(
   const size_t value) const
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
   if (!IsValid())
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator+()"),
         _T("Iterator is not valid"));
   }
   #endif

   Iterator result = *this;

   result += value;

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN,TtoKS>::Iterator &TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN,TtoKS>::Iterator::operator--()
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
   if (!IsValid())
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator--()"),
         _T("Iterator is not valid"));
   }
   #endif

   if (!CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode))
   {
      return *this;
   }

   if (m_nextMove == GoRight)             // This is inverted in here....
   {
      if (CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode)->m_pLinks[0])
      {
         // If we can go left, do so

         m_pNode = CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode)->m_pLinks[0];

         //  and then go as far right as we can

         if (CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode)->m_pLinks[1])
         {
            while (CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode)->m_pLinks[1])
            {
               m_pNode = CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode)->m_pLinks[1];
            }
         }

         return *this;
      }

      m_nextMove = GoUp;
   }

   if (m_nextMove == GoUp)
   {
      if (CIntrusiveRedBlackTreeNode *pParent = CIntrusiveRedBlackTreeNode::GetParent(m_pNode))
      {
         while (CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode) &&
            pParent &&
            m_nextMove == GoUp)
         {
            if (m_pNode == pParent->m_pLinks[1])
            {
               // coming up from right

               m_nextMove = GoRight;
            }

            m_pNode = pParent;

            pParent = CIntrusiveRedBlackTreeNode::GetParent(m_pNode);
         }

         if (CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode) && m_nextMove == GoUp)
         {
            m_pNode = nullptr;
            m_nextMove = GoRight;
         }

         return *this;
      }
   }

   m_pNode = nullptr;
   m_nextMove = GoRight;

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN,TtoKS>::Iterator TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN,TtoKS>::Iterator::operator--(int)
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
   if (!IsValid())
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator--()"),
         _T("Iterator is not valid"));
   }
   #endif

   Iterator result(*this);

   this->operator--();

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN,TtoKS>::Iterator &TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN,TtoKS>::Iterator::operator-=(
   const size_t value)
{
   size_t subtracted = 0;

   while (CIntrusiveRedBlackTreeNode::GetCleanNode(m_pNode) && subtracted != value)
   {
      operator--();

      subtracted++;
   }

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN,TtoKS>::Iterator TIntrusiveRedBlackTree<T, K, TtoK, Pr, TtoN,TtoKS>::Iterator::operator-(
   const size_t value) const
{
   #if JETBYTE_CORE_INTRUSIVE_RED_BLACK_TREE_ENABLE_ITERATOR_VALIDATION == 1
   if (!IsValid())
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator-()"),
         _T("Iterator is not valid"));
   }
   #endif

   Iterator result = *this;

   result -= value;

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator==(
   const Iterator &rhs) const
{
   return m_pNode == rhs.m_pNode && m_nextMove == rhs.m_nextMove;
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator!=(
   const Iterator &rhs) const
{
   return !(*this == rhs);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::value_type *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator *()
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
const typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::value_type *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator *() const
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::value_type *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator ->()
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN, class TtoKS>
const typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::value_type *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN,TtoKS>::Iterator::operator ->() const
{
   return node_accessor::GetTFromNode(m_pNode);
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveRedBlackTree.h
///////////////////////////////////////////////////////////////////////////////
