#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_SMART_HEAP_HANDLE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_SMART_HEAP_HANDLE_INCLUDED__
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

#include <wtypes.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CSmartHeapHandle
///////////////////////////////////////////////////////////////////////////////

class CSmartHeapHandle
{
   public :

      static CSmartHeapHandle DuplicateHandle(
         HANDLE hInput);

      CSmartHeapHandle();

      CSmartHeapHandle(
         HANDLE handle);

      CSmartHeapHandle(
         const CSmartHeapHandle &rhs);

      ~CSmartHeapHandle();

      CSmartHeapHandle &operator=(
         const HANDLE handle);

      CSmartHeapHandle &operator=(
         const CSmartHeapHandle &rhs);

      /// Access the HANDLE... We should remove this...

      operator HANDLE() const;

      HANDLE &GetHandle();

      HANDLE GetHandle() const;

      /// Take ownership of the supplied HANDLE.

      void Attach(
         HANDLE handle);

      /// No longer manage ownership of the HANDLE and return it.

      HANDLE Detach();

      /// Duplicate the owned handle with the same access for use within the 
      /// same process. 

      CSmartHeapHandle DuplicateHandle() const;

      /// Close any handle that is managed by this CSmartHandle.
      
      void Close();

      /// Returns true if the CSmartHandle currently has ownership of a HANDLE.
      
      bool IsValid() const;

   private :

      HANDLE m_handle;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_WIN32_SMART_HEAP_HANDLE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: SmartHeapHandle.h
///////////////////////////////////////////////////////////////////////////////
