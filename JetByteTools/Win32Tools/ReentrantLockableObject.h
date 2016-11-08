#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_REENTRANT_LOCKABLE_OBJECT_INCLUDED__
#define JETBYTE_TOOLS_WIN32_REENTRANT_LOCKABLE_OBJECT_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ReentrantLockableObject.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2012 JetByte Limited.
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

#include "EmptyBase.h"
#include "IReentrantLockableObject.h"
#include "Exception.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// TReentrantLockableObject
///////////////////////////////////////////////////////////////////////////////

/// \ingroup Synchronization

template <typename Base>
class TReentrantLockableObject : public Base
{
   public :

      typedef TLockableObjectOwner<TReentrantLockableObject<Base> > Owner;

      typedef TLockableObjectConditionalOwner<TReentrantLockableObject<Base> > ConditionalOwner;

      typedef TReentrantLockableObjectPotentialOwner<TReentrantLockableObject<Base> > PotentialOwner;

      /// Creates a lockable object.

      TReentrantLockableObject()
      {
         // On some platforms...
         // Can fail under low memory conditions and rase a STATUS_NO_MEMORY
         // exception.

         ::InitializeCriticalSection(&m_lock);
      }

      virtual ~TReentrantLockableObject()
      {
         ::DeleteCriticalSection(&m_lock);
      }

      // Implement IReentrantLockableObject

      virtual bool TryLock()
      {
         return 0 != ::TryEnterCriticalSection(&m_lock);
      }

      virtual void Lock()
      {
         // On some platforms...
         // Can fail if there's contention and the event can't be created and will
         // raise an EXCEPTION_INVALID_HANDLE exception.

         ::EnterCriticalSection(&m_lock);
      }

      virtual void Unlock()
      {
         ::LeaveCriticalSection(&m_lock);
      }

   protected :

      CRITICAL_SECTION m_lock;

   private :

      /// No copies do not implement
      TReentrantLockableObject(const TReentrantLockableObject &rhs);
      /// No copies do not implement
      TReentrantLockableObject &operator=(const TReentrantLockableObject &rhs);
};

class CReentrantLockableObject : public TReentrantLockableObject<IReentrantLockableObject>
{
};

class CSlimReentrantLockableObject : public TReentrantLockableObject<CEmptyBase>
{
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_REENTRANT_LOCKABLE_OBJECT_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ReentrantLockableObject.h
///////////////////////////////////////////////////////////////////////////////
