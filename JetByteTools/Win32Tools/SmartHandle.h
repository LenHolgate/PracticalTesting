#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: SmartHandle.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2005 JetByte Limited.
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

#include "JetByteTools/CoreTools/Types.h"

#include "JetByteTools/CoreTools/IWaitable.h"

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

class CSmartHandle : public Core::IWaitable
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

      ~CSmartHandle() override;

      CSmartHandle &operator=(
         HANDLE handle);

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

      void Close();

      /// Returns true if the CSmartHandle currently has ownership of a HANDLE.

      bool IsValid() const;

      // Implement IWaitable

      HANDLE GetWaitHandle() const override;

      void Wait() const override;

      bool Wait(
         Milliseconds timeoutMillis) const override;

   private :

      HANDLE m_handle;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: SmartHandle.h
///////////////////////////////////////////////////////////////////////////////
