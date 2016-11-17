#pragma once
#ifndef JETBYTE_TOOLS_WIN32_SMART_HANDLE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_SMART_HANDLE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: SmartHandle.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2005 JetByte Limited.
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

#include "IWaitable.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CSmartHandle
///////////////////////////////////////////////////////////////////////////////

/// A \ref SmartPointer "smart pointer" to a HANDLE. Calls ::CloseHandle() on
/// any HANDLE that the it owns when the CSmartHandle goes out of scope to aid
/// in \ref RAII "scope based" designs.
/// \ingroup SmartPointer

class CSmartHandle : public IWaitable
{
   public :

      /// Duplicate the owned handle with the same access for use within the
      /// same process.

      static CSmartHandle DuplicateHandle(
         HANDLE hInput);

      /// Construct a CSmartHandle that doesn't, currently, own a HANDLE.

      CSmartHandle();

      /// Construct a CSmartHandle that takes ownership of the supplied HANDLE.

      explicit CSmartHandle(
         HANDLE handle);

      CSmartHandle(
         const CSmartHandle &rhs);

      ~CSmartHandle();

      CSmartHandle &operator=(
         const HANDLE handle);

      CSmartHandle &operator=(
         const CSmartHandle &rhs);

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

      CSmartHandle DuplicateHandle() const;

      /// Close any handle that is managed by this CSmartHandle.

      //lint -sem(JetByteTools::Win32::CSmartHandle::Close,cleanup)
      void Close();

      /// Returns true if the CSmartHandle currently has ownership of a HANDLE.

      bool IsValid() const;

      // Implement IWaitable

      virtual HANDLE GetWaitHandle() const;

      virtual void Wait() const;

      virtual bool Wait(
         const Milliseconds timeoutMillis) const;

   private :

      HANDLE m_handle;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_SMART_HANDLE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: SmartHandle.h
///////////////////////////////////////////////////////////////////////////////
