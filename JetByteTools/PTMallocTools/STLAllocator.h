#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_PTMALOC_STL_ALLOCATOR_INCLUDED__
#define JETBYTE_TOOLS_PTMALOC_STL_ALLOCATOR_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: STLAllocator.h 
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2010 JetByte Limited.
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

#include <memory>

#include "Malloc.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::PTMalloc
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace PTMalloc {

///////////////////////////////////////////////////////////////////////////////
// CSTLAllocator
///////////////////////////////////////////////////////////////////////////////

template<typename T>
class CSTLAllocator
{
   public:

      // Typedefs
      typedef size_t    size_type;
      typedef ptrdiff_t difference_type;
      typedef T*        pointer;
      typedef const T*  const_pointer;
      typedef T&        reference;
      typedef const T&  const_reference;
      typedef T         value_type;

      template<typename U>
      struct rebind
      {
         typedef CSTLAllocator<U> other;
      };

      pointer address(reference _Val) const
      {   // return address of mutable _Val
         return (&_Val);
      }

      const_pointer address(const_reference _Val) const
      {   // return address of nonmutable _Val
         return (&_Val);
      }

      explicit CSTLAllocator(
         mspace hMalloc)
         :  m_hMalloc(hMalloc)
      {
      }

      CSTLAllocator(
         const CSTLAllocator<T> &rhs)
         :   m_hMalloc(rhs.m_hMalloc)
      {
      }

      template<typename U>
      CSTLAllocator(
         const CSTLAllocator<U> &rhs)
         :   m_hMalloc(rhs.m_hMalloc)
      {
      }

      template<typename U>
      CSTLAllocator<T>& operator=(
         const CSTLAllocator<U> &rhs)
         :  m_hMalloc(rhs.m_hMalloc)
      {
         return *this;
      }

      pointer allocate(
         size_type n,
         const void * /*p*/ = 0)
      {
         pointer pMem = reinterpret_cast<pointer>(mspace_malloc(m_hMalloc, n * sizeof(T)));

         if (!pMem)
         {
            throw std::bad_alloc();
         }

         return pMem;
      }

      void deallocate(
         pointer p, 
         size_type /*n*/)
      {
         if (p)
         {
            mspace_free(m_hMalloc, p);
         }
      }

      void construct(
         pointer p, 
         const_reference c)
      {
         std::_Construct(p, c);
      }

      void destroy(
         pointer p)
      {
         std::_Destroy(p);
      }

      size_type max_size() const
      {
        return std::numeric_limits<size_t>::max() / sizeof(T);
      }

   private:

      CSTLAllocator()
      {   // construct default allocator (do nothing)
      }

      mspace m_hMalloc;

      template <class U> friend class CSTLAllocator;

      template <typename T>
      friend bool operator!=(
         const CSTLAllocator<T> &lhs,
         const CSTLAllocator<T> &rhs) throw();

      template <typename T>
      friend bool operator!=(
         const CSTLAllocator<T> &lhs,
         const CSTLAllocator<T> &rhs) throw();
         
};

template <typename T>
bool operator==(
   const CSTLAllocator<T> &lhs, 
   const CSTLAllocator<T> &rhs) throw()
{
    return lhs.m_hMalloc == rhs.m_hMalloc;
}

template <typename T>
bool operator!=(
   const CSTLAllocator<T> &lhs,
   const CSTLAllocator<T> &rhs) throw()
{
    return lhs.m_hMalloc != rhs.m_hMalloc;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::PTMalloc
///////////////////////////////////////////////////////////////////////////////

} // End of namespace PTMalloc
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_PTMALOC_STL_ALLOCATOR_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: STLAllocator.h
///////////////////////////////////////////////////////////////////////////////
