#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_PTMALLOC_SMART_HEAP_HANDLE_INCLUDED_
#define JETBYTE_TOOLS_PTMALLOC_SMART_HEAP_HANDLE_INCLUDED_
///////////////////////////////////////////////////////////////////////////////
// File: SmartHeapHandle.h
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

#include "Malloc.h"

#include "JetByteTools\Win32Tools\Exception.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::PTMalloc
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace PTMalloc {

///////////////////////////////////////////////////////////////////////////////
// CSmartHeapHandle
///////////////////////////////////////////////////////////////////////////////

class CSmartHeapHandle
{
   public :

      CSmartHeapHandle()
         :  m_hMalloc(create_mspace(0, false))
      {
         if (!m_hMalloc)
         {
            throw JetByteTools::Win32::CException(_T("CSmartHeapHandle::CSmartHeapHandle()"), _T("Failed to create heap"));
         }
      }

      ~CSmartHeapHandle()
      {
         destroy_mspace(m_hMalloc);
      }

      operator mspace() const
      {
         return m_hMalloc;
      }

      void *Allocate(
         const size_t size)
      {
         return mspace_malloc(m_hMalloc, size);
      }

      void Deallocate(
         void *pMem)
      {
         mspace_free(m_hMalloc, pMem);
      }

      template <typename T>
      void Destroy(
         T *pT)
      {
         pT->~T();

         mspace_free(m_hMalloc, (void*)pT);
      }

   private :

      mspace m_hMalloc;

      /// No copies do not implement
      CSmartHeapHandle(const CSmartHeapHandle &rhs);
      /// No copies do not implement
      CSmartHeapHandle &operator=(const CSmartHeapHandle &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::PTMalloc
///////////////////////////////////////////////////////////////////////////////

} // End of namespace PTMalloc
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_PTMALLOC_SMART_HEAP_HANDLE_INCLUDED_

///////////////////////////////////////////////////////////////////////////////
// End of file: SmartHeapHandle.h
///////////////////////////////////////////////////////////////////////////////

