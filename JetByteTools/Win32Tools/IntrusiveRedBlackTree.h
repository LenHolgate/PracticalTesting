#pragma once
#ifndef JETBYTE_TOOLS_WIN32_INTRUSIVE_RED_BLACK_TREE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_INTRUSIVE_RED_BLACK_TREE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IntrusiveRedBlackTree.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2014 JetByte Limited.
//
// Based on original public domain source code from:
// http://en.literateprograms.org/Red-black_tree_(C)?oldid=19567
// and also with ideas from original public domain source code from:
// http://www.eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx
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

#include "IntrusiveRedBlackTreeNode.h"

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1 || JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1 || JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
#include "DebugTrace.h"
#include "tstring.h"
#endif

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1 && JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_WITH_TRACE_ENABLED == 1
#define JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED 1
#else
#define JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED 0
#endif

#include "Exception.h"
#include "ToString.h"

#include <functional>      // for std::less<>

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#include "DebugTrace.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

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
   class T,                                                 // The type to store
   class K,                                                 // The type of the key
   class TtoK,                                              // Functions to access a key from a T
   class Pr = std::less<K>,                                 // Predicate
   class TtoN = TIntrusiveRedBlackTreeNodeIsBaseClass<T> >  // Functions to access the node from a T
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

      size_t Size() const;

      pairib Insert(
         const T *pItemToInsert);

      pairib Insert(
         const T *pItemToInsert,
         const K &key);

      Iterator Find(
         const K &key) const;

      T *Remove(
         const K &key);

      void Erase(
         const Iterator &it);

      void Erase(
         const T *pDataToErase);

      void Clear();

      static bool IsInTree(
         const CIntrusiveRedBlackTreeNode *pNode);

      Iterator CreateIterator(
         const T *pItem) const;

      void SwapNode(
         const CIntrusiveRedBlackTreeNode *pSourceNode,
         CIntrusiveRedBlackTreeNode *pDestNode);

      static void RemoveFromTree(
         CIntrusiveRedBlackTreeNode *pNode);

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

            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
            Iterator(
               CIntrusiveRedBlackTreeNode *pNode,
               const int depth);
            #endif

            explicit Iterator(
               CIntrusiveRedBlackTreeNode *pNode);

            CIntrusiveRedBlackTreeNode *m_pNode;

            enum NextMove
            {
               GoUp,
               GoRight
            };

            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
            int m_depth;
            #endif

            NextMove m_nextMove;
      };

      Iterator Begin() const;

      Iterator End() const;

      typedef void (ValidateNodeFnc)(const T *pNode, ULONG_PTR userData);

      void ValidateTree(
         ValidateNodeFnc *pValidateNodeFnc = nullptr,
         ULONG_PTR userData = 0) const;

      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
      static _tstring KeyAsString(
         const CIntrusiveRedBlackTreeNode *pNode);

      _tstring DumpTree() const;
      #endif

   private :

      pairib InternalInsert(
         CIntrusiveRedBlackTreeNode *pNode,
         const K &newKey);

      void ReplaceNode(
         CIntrusiveRedBlackTreeNode *pOldNode,
         CIntrusiveRedBlackTreeNode *pNewNode);

      void Rotate(
         CIntrusiveRedBlackTreeNode *pNode,
         int dir);

      void InsertRebalance(
         CIntrusiveRedBlackTreeNode *pNode);

      void InternalErase(
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

      static int ValidateTree(
         CIntrusiveRedBlackTreeNode *pRoot,
         ValidateNodeFnc *pValidateNodeFnc,
         ULONG_PTR userData);

      CIntrusiveRedBlackTreeNode *m_pRoot;

      size_t m_size;

      key_compare m_comp;

      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
      mutable bool m_isValid;
      #endif

      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 0
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
      _tstring m_previousDump;
      #endif
      #endif
};

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::TIntrusiveRedBlackTree()
   :  m_pRoot(nullptr),
      m_size(0),
      m_comp()
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
      , m_isValid(true)
      #endif
{
}

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::~TIntrusiveRedBlackTree()
{
   try
   {
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
      if (m_isValid)
      {
         Clear();
      }
      #else
      Clear();
      #endif
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED

   m_pRoot = nullptr;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Clear()
{
   Iterator it = Begin();

   const Iterator end = End();

   while (it != end)
   {
      const Iterator next = it + 1;             // iterators are invalidated after erase
                                                // so increment now

      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
      CIntrusiveRedBlackTreeNode *pNode = it.m_pNode;
      #endif

      Erase(it);

      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
      if (IsInTree(pNode))
      {
         throw CException(
            _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Clear()"),
            _T("Removed node is still in the tree"));
      }
      #endif

      it = next;
   }
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::CreateIterator(
   const T *pItem) const
{
   return Iterator(node_accessor::GetNodeFromT(pItem));
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::SwapNode(
   const CIntrusiveRedBlackTreeNode *pSourceNode,
   CIntrusiveRedBlackTreeNode *pDestNode)
{
   if (!pSourceNode->m_pParent)
   {
      m_pRoot = pDestNode;
   }
   else
   {
      const int dir = (pSourceNode == pSourceNode->m_pParent->m_pLinks[1]);

      pSourceNode->m_pParent->m_pLinks[dir] = pDestNode;
   }

   pDestNode->m_pParent = pSourceNode->m_pParent;

   if (pSourceNode->m_pLinks[0])
   {
      pSourceNode->m_pLinks[0]->m_pParent = pDestNode;
   }

   pDestNode->m_pLinks[0] = pSourceNode->m_pLinks[0];

   if (pSourceNode->m_pLinks[1])
   {
      pSourceNode->m_pLinks[1]->m_pParent = pDestNode;
   }

   pDestNode->m_pLinks[1] = pSourceNode->m_pLinks[1];

   pDestNode->m_red = pSourceNode->m_red;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
size_t TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Size() const
{
   return m_size;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Find(
   const K &key) const
{
   CIntrusiveRedBlackTreeNode *pIt = m_pRoot;

   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
   int depth = 0;
   #endif

   while (pIt)
   {
      const K currentKey = key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pIt));

      const int dir = m_comp(currentKey, key);

      if (!dir && !m_comp(key, currentKey))    // !(a < b) && !(b < a) = (a==b)
      {
         break;
      }

      pIt = pIt->m_pLinks[dir];

      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
      depth++;
      #endif
   }

   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
   return Iterator(pIt, depth);
   #else
   return Iterator(pIt);
   #endif
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::ReplaceNode(
   CIntrusiveRedBlackTreeNode *pOldNode,
   CIntrusiveRedBlackTreeNode *pNewNode)
{
   if (!pOldNode->m_pParent)
   {
      m_pRoot = pNewNode;
   }
   else
   {
      const int dir = (pOldNode == pOldNode->m_pParent->m_pLinks[1]);

      pOldNode->m_pParent->m_pLinks[dir] = pNewNode;
   }

   if (pNewNode)
   {
      pNewNode->m_pParent = pOldNode->m_pParent;
   }
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Rotate(
   CIntrusiveRedBlackTreeNode *pNode,
   const int dir)
{
   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
   const _tstring direction = (!dir ? _T(" left") : _T(" right"));
   OutputEx(_T("Rotate: ") + KeyAsString(pNode) + direction);
   #endif

   CIntrusiveRedBlackTreeNode *pTemp = pNode->m_pLinks[!dir]; //lint !e514 (Unusual use of a Boolean expression)

   ReplaceNode(pNode, pTemp);

   pNode->m_pLinks[!dir] = pTemp->m_pLinks[dir]; //lint !e514 (Unusual use of a Boolean expression)

   if (pTemp->m_pLinks[dir])
   {
      pTemp->m_pLinks[dir]->m_pParent = pNode;
   }

   pTemp->m_pLinks[dir] = pNode;
   pNode->m_pParent = pTemp;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::IsInTree(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   return !pNode->m_red || pNode->m_pParent || pNode->m_pLinks[0] || pNode->m_pLinks[1];
}

template <class T, class K, class TtoK, class Pr, class TtoN>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::IsBlack(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   return pNode ? !pNode->m_red : true;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::IsRed(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   return pNode ? pNode->m_red : false;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
CIntrusiveRedBlackTreeNode *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Sibling(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (!pNode)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Sibling()"),
         _T("pNode is null"));
   }
   #endif

   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (!pNode->m_pParent)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Sibling()"),
         _T("pNode->m_pParent is null"));
   }
   #endif

   const int dir = (pNode == pNode->m_pParent->m_pLinks[1]);

   return pNode->m_pParent->m_pLinks[!dir]; //lint !e514 (Unusual use of a Boolean expression)
}

template <class T, class K, class TtoK, class Pr, class TtoN>
CIntrusiveRedBlackTreeNode *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Uncle(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (!pNode)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Uncle()"),
         _T("pNode is null"));
   }

   if (!pNode->m_pParent)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Uncle()"),
         _T("pNode->m_pParent is null"));
   }

   if (!pNode->m_pParent->m_pParent)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Uncle()"),
         _T("pNode->m_pParent->m_pParent is null"));
   }
   #endif

   return Sibling(pNode->m_pParent);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
CIntrusiveRedBlackTreeNode *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::GrandParent(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (!pNode)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::GrandParent()"),
         _T("pNode is null"));
   }

   if (!pNode->m_pParent)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::GrandParent()"),
         _T("pNode->m_pParent is null"));
   }

   if (!pNode->m_pParent->m_pParent)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::GrandParent()"),
         _T("pNode->m_pParent->m_pParent is null"));
   }
   #endif

   return pNode->m_pParent->m_pParent;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::RemoveFromTree(
   CIntrusiveRedBlackTreeNode *pNode)
{
   if (!pNode)
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::RemoveFromTree()"),
         _T("pNode is null"));
   }

   pNode->m_pParent = nullptr;
   pNode->m_pLinks[0] = nullptr;
   pNode->m_pLinks[1] = nullptr;
   pNode->m_red = true;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::pairib TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Insert(
   const T *pItemToInsert)
{
   if (!pItemToInsert)
   {
      throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Insert()"), _T("Node is null"));
   }

   CIntrusiveRedBlackTreeNode *pNode = node_accessor::GetNodeFromT(pItemToInsert);

   if (IsInTree(pNode))
   {
      throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Insert()"), _T("Node is already in a tree"));
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

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::pairib TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Insert(
   const T *pItemToInsert,
   const K &key)
{
   if (!pItemToInsert)
   {
      throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Insert()"), _T("Node is null"));
   }

   CIntrusiveRedBlackTreeNode *pNode = node_accessor::GetNodeFromT(pItemToInsert);

   if (IsInTree(pNode))
   {
      throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Insert()"), _T("Node is already in a tree"));
   }

   return InternalInsert(pNode, key);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::pairib TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::InternalInsert(
   CIntrusiveRedBlackTreeNode *pNode,
   const K &newKey)
{
   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
   int depth = 0;
   #endif

   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
   if (!IsRed(pNode))
   {
      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::InternalInsert()"),
         _T("Unexpected node state"));
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
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
      OutputEx(_T("Insert: ") + ToString(newKey));
      #endif

      // Search down the tree for a place to insert

      while (!inserted &&
             pIt != pNode)
      {
         const K currentKey = key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pIt));

         const int dir = m_comp(currentKey, newKey);

         if (dir == 0 && !m_comp(newKey,currentKey))
         {
            // We have found the key in the tree already, return an iterator
            // to the node we found.

            pNode = pIt;
         }
         else
         {
            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
            depth++;
            #endif

            if (!pIt->m_pLinks[dir])
            {
               // Insert the new node here

               pIt->m_pLinks[dir] = pNode;

               pNode->m_pParent = pIt;

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

      m_pRoot->m_red = false; //lint !e613 (Possible use of null pointer in left argument to operator '->'
      m_size++;

      if (m_size > 1)
      {
         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(_T("Rebalance: ") + KeyAsString(pIt));
         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
         OutputEx(DumpTree());
         #elif JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
         m_previousDump = DumpTree();
         #endif
         #endif

         InsertRebalance(pIt);

         m_pRoot->m_red = false;
      }

      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
      OutputEx(_T("Insert done"));
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
      OutputEx(DumpTree());
      #elif JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
      m_previousDump = DumpTree();
      #endif
      #endif

      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
      ValidateTree();
      #endif
   }

   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
   return pairib(Iterator(pNode, depth), inserted);
   #else
   return pairib(Iterator(pNode), inserted);
   #endif
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::InsertRebalance(
   CIntrusiveRedBlackTreeNode *pNode)
{
   bool done = false;

   while (!done)
   {
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
      OutputEx(_T("Rebalancing: ") + KeyAsString(pNode));
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
      OutputEx(DumpTree());
      #elif JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
      m_previousDump = DumpTree();
      #endif
      #endif

      if (!pNode->m_pParent)
      {
         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(_T("Parent is null: ") + KeyAsString(pNode));
         #endif

         pNode->m_red = false;                 // insert case 1

         done = true;
      }
      else
      {
         if (IsBlack(pNode->m_pParent))
         {
            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            if (pNode->m_pParent)
            {
               OutputEx(_T("Parent is black: ") + KeyAsString(pNode) +
                  _T(" P: ") + KeyAsString(pNode->m_pParent));
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
               #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
               OutputEx(_T("Uncle is red: ") + KeyAsString(pNode));
               #endif

               // insert case 3

               pNode->m_pParent->m_red = false;
               Uncle(pNode)->m_red = false;
               GrandParent(pNode)->m_red = true;

               pNode = GrandParent(pNode);

               // back to case 1
            }
            else
            {
               #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
               OutputEx(_T("Case 4: ") + KeyAsString(pNode));
               #endif

               // insert case 4

               int dir = (pNode == pNode->m_pParent->m_pLinks[1]);

               int parentDir = (pNode->m_pParent == GrandParent(pNode)->m_pLinks[1]);

               if (dir != parentDir)
               {
                  Rotate(pNode->m_pParent, !dir);

                  pNode = pNode->m_pLinks[!dir]; //lint !e514 (Unusual use of a Boolean expression)
               }

               // fall through to case 5
               // insert case 5

               #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
               OutputEx(_T("Case 5: ") + KeyAsString(pNode));
               #endif

               pNode->m_pParent->m_red = false;
               GrandParent(pNode)->m_red = true;

               // These two may have changed if we moved pNode in case 4...

               dir = (pNode == pNode->m_pParent->m_pLinks[1]); //lint !e578 (Declaration of symbol 'dir' hides symbol 'dir')

               parentDir = (pNode->m_pParent == GrandParent(pNode)->m_pLinks[1]); //lint !e578 (Declaration of symbol 'parentDir' hides symbol 'parentDir')

               if (dir == parentDir)
               {
                  Rotate(GrandParent(pNode), !dir);
               }

               done = true;
            }
         }
      }
   }
}

template <class T, class K, class TtoK, class Pr, class TtoN>
T *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Remove(
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

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Erase(
   const Iterator &it)
{
   InternalErase(it.m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Erase(
   const T *pDataToErase)
{
   InternalErase(node_accessor::GetNodeFromT(pDataToErase));
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::InternalErase(
   CIntrusiveRedBlackTreeNode *pNode)
{
   if (pNode && IsInTree(pNode))
   {
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
      OutputEx(_T("Erase: ") + KeyAsString(pNode));
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
      OutputEx(DumpTree());
      #elif JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
      m_previousDump = DumpTree();
      #endif
      #endif

      if (pNode->m_pLinks[0] && pNode->m_pLinks[1])
      {
         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
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

         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(_T("Prev: ") + KeyAsString(pPrev));
         #endif

         const bool prevIsRed = pPrev->m_red;

         pPrev->m_red = pNode->m_red;
         pNode->m_red = prevIsRed;

         // splice our right sub-tree (which must exist as we have two children)
         // on to the prev node (which can't have a right sub-tree of its own as
         // we are the node which would appear on its right)

         pPrev->m_pLinks[1] = pNode->m_pLinks[1];
         pPrev->m_pLinks[1]->m_pParent = pPrev;
         pNode->m_pLinks[1] = nullptr;

         // swap our parent with our prev node's parent

         CIntrusiveRedBlackTreeNode *pPrevParent = pPrev->m_pParent;

         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(_T("Prev Parent: ") + KeyAsString(pPrevParent));
         #endif

         // The prev node MUST have a parent, prev node's parent
         // might be us. We might not have a parent as we might
         // be the root of the tree.

         if (pNode->m_pParent)
         {
            const int dir = (pNode == pNode->m_pParent->m_pLinks[1]);

            pNode->m_pParent->m_pLinks[dir] = pPrev;
         }
         else
         {
            m_pRoot = pPrev;
         }

         pPrev->m_pParent = pNode->m_pParent;

         if (pPrevParent == pNode)
         {
            // we are the prev node's parent...
            // it can only be our left node

            pNode->m_pParent = pPrev;

            pNode->m_pLinks[0] = pPrev->m_pLinks[0];

            if (pNode->m_pLinks[0])
            {
               pNode->m_pLinks[0]->m_pParent = pNode;
            }

            pPrev->m_pLinks[0] = pNode;
         }
         else
         {
            const int dir = (pPrev == pPrevParent->m_pLinks[1]);

            pNode->m_pParent = pPrevParent;
            pPrevParent->m_pLinks[dir] = pNode;

            // swap our left sub-tree with our prev node

            CIntrusiveRedBlackTreeNode *pPrevLeft = pPrev->m_pLinks[0];

            pPrev->m_pLinks[0] = pNode->m_pLinks[0];
            pPrev->m_pLinks[0]->m_pParent = pPrev;

            pNode->m_pLinks[0] = pPrevLeft;

            if (pNode->m_pLinks[0])
            {
               pNode->m_pLinks[0]->m_pParent = pNode;
            }
         }
      }

      // The node we need to delete has, at most, one child.

      CIntrusiveRedBlackTreeNode *pChild = !pNode->m_pLinks[1] ? pNode->m_pLinks[0] : pNode->m_pLinks[1];

      if (IsBlack(pNode))
      {
         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         if (pChild)
         {
            OutputEx(_T("Rebalance: ") + KeyAsString(pNode) + _T(" Child: ") + KeyAsString(pChild));
         }
         else
         {
            OutputEx(_T("Rebalance: ") + KeyAsString(pNode));
         }
         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
         OutputEx(DumpTree());
         #elif JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
         m_previousDump = DumpTree();
         #endif
         #endif

         pNode->m_red = IsRed(pChild);

         DeleteRebalance(pNode);
      }

      ReplaceNode(pNode, pChild);

      RemoveFromTree(pNode);

      m_size--;

      // Root must be black

      if (m_pRoot)
      {
         m_pRoot->m_red = false;
      }
   }

   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
   OutputEx(_T("Remove complete:"));
   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
   OutputEx(DumpTree());
   #endif
   #endif

   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_VALIDATE_ON_EVERY_OPERATION == 1
   ValidateTree();
   #endif
}

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::DeleteRebalance(
   CIntrusiveRedBlackTreeNode *pNode)
{
   try
   {
      bool done = false;

      while (!done)
      {
         done = true;

         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(_T("Rebalancing: ") + KeyAsString(pNode));
         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 1
         OutputEx(DumpTree());
         #elif JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
         m_previousDump = DumpTree();
         #endif
         #endif

         if (!pNode->m_pParent)
         {
            // delete case 1

            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(_T("Parent is null: ") + KeyAsString(pNode));
            #endif

            break;
         }

         CIntrusiveRedBlackTreeNode *pSibling = Sibling(pNode);

         if (IsRed(pSibling))
         {
            // delete case 2

            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(_T("Case 2: ") + KeyAsString(pNode) + _T(" Sibling: ") + KeyAsString(pSibling));
            #endif

            pNode->m_pParent->m_red = true;
            pSibling->m_red = false;

            const int dir = (pNode == pNode->m_pParent->m_pLinks[1]);

            Rotate(pNode->m_pParent, dir);

            pSibling = Sibling(pNode);
         }

         if (IsBlack(pNode->m_pParent) &&
             IsBlack(pSibling) &&
             IsBlack(pSibling->m_pLinks[0]) &&
             IsBlack(pSibling->m_pLinks[1]))
         {
            // delete case 3

            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(_T("Case 3: ") + KeyAsString(pNode) + _T(" Sibling: ") + KeyAsString(pSibling));
            #endif

            pSibling->m_red = true;

            // back to case 1

            pNode = pNode->m_pParent;

            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(_T("Back to case 1: ") + KeyAsString(pNode));
            #endif

            done = false;
         }
         else
         {
            if (IsRed(pNode->m_pParent) &&
                IsBlack(pSibling) &&
                IsBlack(pSibling->m_pLinks[0]) &&
                IsBlack(pSibling->m_pLinks[1]))
            {
               // delete case 4

               #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
               OutputEx(_T("Case 4: ") + KeyAsString(pNode) + _T(" Sibling: ") + KeyAsString(pSibling));
               #endif

               pSibling->m_red = true;
               pNode->m_pParent->m_red = false;
            }
            else
            {
               // delete case 5

               int dir = (pNode == pNode->m_pParent->m_pLinks[1]);

               if (IsBlack(pSibling) &&
                   IsRed(pSibling->m_pLinks[dir]) &&
                   IsBlack(pSibling->m_pLinks[!dir])) //lint !e514 (Unusual use of a Boolean expression)
               {
                  #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
                  OutputEx(_T("Case 5: ") + KeyAsString(pNode) + _T(" Sibling: ") + KeyAsString(pSibling));
                  #endif

                  pSibling->m_red = true;
                  pSibling->m_pLinks[dir]->m_red = false;
                  Rotate(pSibling, !dir);

                  pSibling = Sibling(pNode);
               }

               // fall through to case 6

               // delete case 6

               #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
               OutputEx(_T("Case 6: ") + KeyAsString(pNode) + _T(" Sibling: ") + KeyAsString(pSibling));
               #endif

               pSibling->m_red = pNode->m_pParent->m_red;
               pNode->m_pParent->m_red = false;

               dir = (pNode == pNode->m_pParent->m_pLinks[1]);

               #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_INTERNAL_STATE_FAILURE_EXCEPTIONS == 1
               if (!IsRed(pSibling->m_pLinks[!dir])) //lint !e514 (Unusual use of a Boolean expression)
               {
                  throw CException(
                     _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::DeleteRebalance()"),
                     _T("Unexpected tree state"));
               }
               #endif

               pSibling->m_pLinks[!dir]->m_red = false; //lint !e514 (Unusual use of a Boolean expression)

               Rotate(pNode->m_pParent, dir);
            }
         }
      }
   }
   catch (...)
   {
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
      m_isValid = false;
      #endif

      throw;
   }
}

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1

template <class T, class K, class TtoK, class Pr, class TtoN>
_tstring TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::KeyAsString(
   const CIntrusiveRedBlackTreeNode *pNode)
{
   return ToString(key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pNode)));
}

template <class T, class K, class TtoK, class Pr, class TtoN>
_tstring TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::DumpTree() const
{
   _tstring result;

   size_t i = 0;

   for (Iterator it = Begin(), end = End(); it != end; ++it, ++i)
   {
      const _tstring colour = it.m_pNode ? (it.m_pNode->m_red ? _T("r") : _T("b")) : _T("*");

      const K key = key_accessor::GetKeyFromT(*it);

      // actually need a 'toString' on the key accessor

      result += ToString(key) + _T("(") + ToString(it.m_depth) + _T(", ") + colour + _T("), ");

      if (i > m_size)
      {
         const _tstring message = _T("Dump of tree does not contain the right number of nodes: expected: ") + ToString(m_size) + _T(" got at least: ") + ToString(i);

         OutputEx(message);

         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 0
         OutputEx(_T("Tree before last operation"));
         OutputEx(m_previousDump);
         OutputEx(_T("Tree now"));
         #endif

         OutputEx(result);

         throw CException(
            _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::DumpTree()"),
            message);
      }
   }

   if (i != m_size)
   {
      const _tstring message = _T("Dump of tree does not contain the right number of nodes: expected: ") + ToString(m_size) + _T(" got at least: ") + ToString(i);

      OutputEx(message);

      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 0
      OutputEx(_T("Tree before last operation"));
      OutputEx(m_previousDump);
      OutputEx(_T("Tree now"));
      #endif

      OutputEx(result);

      throw CException(
         _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::DumpTree()"),
         message);
   }

   return result;
}
#endif

///////////////////////////////////////////////////////////////////////////////
// ValidateTree
///////////////////////////////////////////////////////////////////////////////

template <class T, class K, class TtoK, class Pr, class TtoN>
void TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::ValidateTree(
   ValidateNodeFnc *pValidateNodeFnc,
   const ULONG_PTR userData) const
{
   try
   {
      // First walk the tree in order and make sure we dont get any loops

      size_t i = 0;

      for (Iterator it = Begin(), end = End(); it != end; ++it, ++i)
      {
         if (i > m_size)
         {
            const _tstring message = _T("A walk of the tree does not contain the right number of nodes: expected:") + ToString(m_size) + _T(" got at least: ") + ToString(i);

            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(message);
            #endif

            throw CException(
               _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::ValidateTree()"),
               message);
         }
      }

      if (i != m_size)
      {
         const _tstring message = _T("A walk of the tree does not contain the right number of nodes: expected:") + ToString(m_size) + _T(" got: ") + ToString(i);

         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
         OutputEx(message);
         #endif

         throw CException(
            _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::ValidateTree()"),
            message);
      }

      try
      {
         (void)ValidateTree(m_pRoot, pValidateNodeFnc, userData);
      }
      catch (...)
      {
         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
         OutputEx(_T("Exception during tree validation"));

         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ON_TRACE_ENABLED == 0
         OutputEx(_T("Tree before last operation"));
         OutputEx(m_previousDump);
         OutputEx(_T("Tree now"));
         #endif
         OutputEx(DumpTree());
         #endif

         throw;
      }
   }
   catch (...)
   {
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DO_NOT_CLEANUP_ON_FAILED_VALIDATION == 1
      m_isValid = false;
      #endif

      throw;
   }
}

template <class T, class K, class TtoK, class Pr, class TtoN>
int TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::ValidateTree(
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
            const _tstring message = _T("Red violation");

            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(message);
            #endif

            throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::ValidateTree()"), message);
         }
      }

      if (pValidateNodeFnc)
      {
         pValidateNodeFnc(node_accessor::GetTFromNode(pRoot), userData);
      }

      const int leftHeight = ValidateTree(pLeftNode, pValidateNodeFnc, userData);

      const int rightHeight = ValidateTree(pRightNode, pValidateNodeFnc, userData);

      // Invalid binary search tree

      const K rootKey = key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pRoot));

      if (pLeftNode)
      {
         const K leftKey = key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pLeftNode));

         if (comp(rootKey, leftKey))
         {
            const _tstring message = _T("Binary tree violation");

            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(message);
            #endif

            throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::ValidateTree()"), message);
         }
      }

      if (pRightNode)
      {
         const K rightKey = key_accessor::GetKeyFromT(node_accessor::GetTFromNode(pRightNode));

         if (comp(rightKey, rootKey))
         {
            const _tstring message = _T("Binary tree violation");

            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(message);
            #endif

            throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::ValidateTree()"), message);
         }
      }

      // Black height mismatch

      if (leftHeight != 0 && rightHeight != 0 && leftHeight != rightHeight)
      {
            const _tstring message = _T("Black violation");

            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DEBUG_TRACE == 1
            OutputEx(message);
            #endif

            throw CException(_T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::ValidateTree()"), message);
      }

      // Only count black links

      if (leftHeight != 0 && rightHeight != 0)
      {
         return rootIsRed ? leftHeight : leftHeight + 1;
      }
   }

   return 0;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Begin() const
{
   CIntrusiveRedBlackTreeNode *pNode = m_pRoot;

   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
   int depth = 0;
   #endif

   while (pNode)
   {
      if (pNode->m_pLinks[0])
      {
         pNode = pNode->m_pLinks[0];

         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
         depth++;
         #endif
      }
      else
      {
         break;
      }
   }

   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
   return Iterator(pNode, depth);
   #else
   return Iterator(pNode);
   #endif
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::End() const
{
   return Iterator();
}

///////////////////////////////////////////////////////////////////////////////
// TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator
///////////////////////////////////////////////////////////////////////////////

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::Iterator()
   :  m_pNode(nullptr),
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
      m_depth(0),
      #endif
      m_nextMove(GoRight)
{
}

template <class T, class K, class TtoK, class Pr, class TtoN>
K TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::Key() const
{
   return key_accessor::GetKeyFromT(node_accessor::GetTFromNode(m_pNode));
}

#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::Iterator(
   CIntrusiveRedBlackTreeNode *pNode,
   const int depth)
   :  m_pNode(pNode),
      m_depth(pNode ? depth : 0),
      m_nextMove(GoRight)
{
}
#endif

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::Iterator(
   CIntrusiveRedBlackTreeNode *pNode)
   :  m_pNode(pNode),
#if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
      m_depth(0),
#endif
      m_nextMove(GoRight)
{
}

template <class T, class K, class TtoK, class Pr, class TtoN>
TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::Iterator(
   const Iterator &rhs)
   :  m_pNode(rhs.m_pNode),
      #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
      m_depth(rhs.m_depth),
      #endif
      m_nextMove(rhs.m_nextMove)
{
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator &TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::operator=(
   const Iterator &rhs)
{
   m_pNode = rhs.m_pNode; //lint !e1555 (shallow pointer copy of in copy assignment operator
   #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
   m_depth = rhs.m_depth;
   #endif
   m_nextMove = rhs.m_nextMove;

   return *this;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator &TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::operator++()
{
   if (!m_pNode)
   {
      return *this;
   }

   if (m_nextMove == GoRight)
   {
      if (m_pNode->m_pLinks[1])
      {
         // If we can go right, do so

         m_pNode = m_pNode->m_pLinks[1];

         #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
         m_depth++;
         #endif

         //  and then go as far left as we can

         if (m_pNode->m_pLinks[0])
         {
            while (m_pNode->m_pLinks[0])
            {
               m_pNode = m_pNode->m_pLinks[0];

               #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
               m_depth++;
               #endif
            }
         }

         return *this;
      }

      m_nextMove = GoUp;
   }

   if (m_nextMove == GoUp)
   {
      if (m_pNode->m_pParent)
      {
         while (m_pNode &&
                m_pNode->m_pParent &&
                m_nextMove == GoUp)
         {
            if (m_pNode == m_pNode->m_pParent->m_pLinks[0])
            {
               // coming up from left

               m_nextMove = GoRight;
            }

            m_pNode = m_pNode->m_pParent;

            #if JETBYTE_INTRUSIVE_RED_BLACK_TREE_DUMP_TREE_ENABLED == 1
            m_depth--;

            if (m_depth < 0)
            {
               throw CException(
                  _T("TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::operator++()"),
                  _T("Invalid tree, depth has gone negative"));
            }
            #endif
         }

         if (m_pNode && m_nextMove == GoUp)
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

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::operator++(int)
{
   Iterator result(*this);

   this->operator++();

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator &TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::operator+=(
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
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::operator+(
   const size_t value) const
{
   Iterator result = *this;

   result += value;

   return result;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::operator==(
   const Iterator &rhs) const
{
   return m_pNode == rhs.m_pNode && m_nextMove == rhs.m_nextMove;
}

template <class T, class K, class TtoK, class Pr, class TtoN>
bool TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::operator!=(
   const Iterator &rhs) const
{
   return !(*this == rhs);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::value_type *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::operator *()
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
const typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::value_type *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::operator *() const
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::value_type *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::operator ->()
{
   return node_accessor::GetTFromNode(m_pNode);
}

template <class T, class K, class TtoK, class Pr, class TtoN>
const typename TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::value_type *TIntrusiveRedBlackTree<T,K,TtoK,Pr,TtoN>::Iterator::operator ->() const
{
   return node_accessor::GetTFromNode(m_pNode);
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_INTRUSIVE_RED_BLACK_TREE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IntrusiveRedBlackTree.h
///////////////////////////////////////////////////////////////////////////////
