#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_STL_HEAP_ALLOCATOR_INCLUDED__
#define JETBYTE_TOOLS_WIN32_STL_HEAP_ALLOCATOR_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: STLHeapAllocator.h 
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

#include <winbase.h>

#include <memory>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CSTLHeapAllocator
///////////////////////////////////////////////////////////////////////////////

template<typename T>
class CSTLHeapAllocator
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
         typedef CSTLHeapAllocator<U> other;
      };

      pointer address(reference _Val) const
      {   // return address of mutable _Val
         return (&_Val);
      }

      const_pointer address(const_reference _Val) const
      {   // return address of nonmutable _Val
         return (&_Val);
      }

      CSTLHeapAllocator(
         const HANDLE &hHeap)
         :  m_hHeap(hHeap)
      {
      }

      CSTLHeapAllocator(
         const CSTLHeapAllocator<T> &rhs)
         :  m_hHeap(rhs.m_hHeap)
      {
      }

      template<typename U>
      CSTLHeapAllocator(
         const CSTLHeapAllocator<U> &rhs)
         :  m_hHeap(rhs.m_hHeap)
      {
      }

      template<typename U>
      CSTLHeapAllocator<T>& operator=(
         const CSTLHeapAllocator<U> &rhs)
         :  m_hHeap(rhs.m_hHeap)
      {
         return *this;
      }

      pointer allocate(
         size_type n,
         const void * /*p*/ = 0)
      {
         pointer pMem = reinterpret_cast<pointer>(::HeapAlloc(m_hHeap, 0, n * sizeof(T)));

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
            if (!::HeapFree(m_hHeap, 0, p))
            {
               // Hmm
            }
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

      CSTLHeapAllocator()
      {   // construct default allocator (do nothing)
      }

      HANDLE m_hHeap;

      template <class U> friend class CSTLHeapAllocator;

      template <typename T>
      friend bool operator!=(
         const CSTLHeapAllocator<T> &lhs,
         const CSTLHeapAllocator<T> &rhs) throw();

      template <typename T>
      friend bool operator!=(
         const CSTLHeapAllocator<T> &lhs,
         const CSTLHeapAllocator<T> &rhs) throw();
         
};

template <typename T>
bool operator==(
   const CSTLHeapAllocator<T> &lhs, 
   const CSTLHeapAllocator<T> &rhs) throw()
{
    return lhs.m_hHeap == rhs.m_hHeap;
}

template <typename T>
bool operator!=(
   const CSTLHeapAllocator<T> &lhs,
   const CSTLHeapAllocator<T> &rhs) throw()
{
    return lhs.m_hHeap != rhs.m_hHeap;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_WIN32_STL_HEAP_ALLOCATOR_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: STLHeapAllocator.h
///////////////////////////////////////////////////////////////////////////////
