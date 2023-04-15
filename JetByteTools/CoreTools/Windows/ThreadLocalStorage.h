#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ThreadLocalStorage.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2002 JetByte Limited.
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

#include <wtypes.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// CThreadLocalStorage
///////////////////////////////////////////////////////////////////////////////

class CThreadLocalStorage
{
   public :

      typedef DWORD TLSIndex;

      /// Allocates and becomes the owner of a thread local storage slot.
      /// Throws an exception if there are no more TLS slots available.

      CThreadLocalStorage();

      /// Connects to an existing TLS slot.

      explicit CThreadLocalStorage(
         TLSIndex tlsIndex);

      CThreadLocalStorage(
         const CThreadLocalStorage &rhs) = delete;

      /// If this object owns the TLS slot then it releases it.

      ~CThreadLocalStorage();

      CThreadLocalStorage &operator=(
         const CThreadLocalStorage &rhs) = delete;

      /// Returns the object's TLS index.

      TLSIndex GetIndex() const;

      /// Clears the value of the TLS slot (sets it to zero).

      void ClearValue() const;

      /// Sets the value of the TLS slot.

      void SetValue(
         void *pValue) const;

      /// Sets a DWORD value in the TLS stot without casting at the call site.

      void SetValue(
         DWORD value) const;

      /// Returns the stored value of the TLS slot.

      void *GetValue() const;

      /// Returns the stored value of the TLS slot as a DWORD without casting at the call site.

      DWORD GetValueAsDWORD() const;

   private :

      const TLSIndex m_index;

      const bool m_owner;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadLocalStorage.h
///////////////////////////////////////////////////////////////////////////////
