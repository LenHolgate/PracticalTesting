#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_THREAD_LOCAL_STORAGE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_THREAD_LOCAL_STORAGE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ThreadLocalStorage.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2002 JetByte Limited.
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
// CThreadLocalStorage
///////////////////////////////////////////////////////////////////////////////

/// A class that wraps the operating system 
/// <a href="http://msdn2.microsoft.com/en-us/library/ms686749.aspx">Thread  
/// Local Storage API</a>.
/// \ingroup Threading

class CThreadLocalStorage 
{
   public :

      /// Allocates and becomes the owner of a thread local storage slot. 
      /// Throws an exception if there are no more TLS slots available.

      CThreadLocalStorage();

      /// Connects to an existing TLS slot.

      explicit CThreadLocalStorage(
         DWORD tlsIndex);
   
      /// If this object owns the TLS slot then it releases it.

      ~CThreadLocalStorage();

      /// Returns the object's TLS index.

      DWORD GetIndex() const;

      /// Sets the value of the TLS slot.

      void SetValue(void *pValue) const;

      /// Returns the stored value of the TLS slot.

      void *GetValue() const;

   private :

      const DWORD m_index;

      const bool m_owner;

      /// No copies do not implement
      CThreadLocalStorage(const CThreadLocalStorage &rhs);
      /// No copies do not implement
      CThreadLocalStorage &operator=(const CThreadLocalStorage &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

#endif //JETBYTE_TOOLS_WIN32_THREAD_LOCAL_STORAGE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadLocalStorage.h
///////////////////////////////////////////////////////////////////////////////