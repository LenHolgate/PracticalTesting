#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_LOCKABLE_OBJECT_INCLUDED__
#define JETBYTE_TOOLS_WIN32_LOCKABLE_OBJECT_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: LockableObject.h
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
#include "ILockableObject.h"
#include "Exception.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// TLockableObject
///////////////////////////////////////////////////////////////////////////////

/// \ingroup Synchronization

template <typename Base>
class TLockableObject : public Base
{
   public :

      typedef TLockableObjectOwner<TLockableObject<Base> > Owner;

      typedef TLockableObjectConditionalOwner<TLockableObject<Base> > ConditionalOwner;

      typedef TLockableObjectPotentialOwner<TLockableObject<Base> > PotentialOwner;

      TLockableObject()
         #if (JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1)
         : m_owningThreadId(0)
         #endif
      {
         #if (JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 1)
         ::InitializeSRWLock(&m_lock);
         #else
         ::InitializeCriticalSection(&m_lock);
         #endif
      }

      ~TLockableObject()
      {
         #if (JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 1)
         #else
         ::DeleteCriticalSection(&m_lock);
         #endif
      }

      bool TryLock()
      {
         #if (JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1)
         #if (JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 1)
            const bool locked = (0 != ::TryAcquireSRWLockExclusive(&m_lock));
         #else
            const bool locked = (0 != ::TryEnterCriticalSection(&m_lock));
         #endif

         if (locked)
         {
            m_owningThreadId = ::GetCurrentThreadId();
         }
         else
         {
            CheckForReentrantUse();
         }

         return locked;
         #else
         #if (JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 1)
            return 0 != ::TryAcquireSRWLockExclusive(&m_lock);
         #else
            return 0 != ::TryEnterCriticalSection(&m_lock);
         #endif
         #endif
      }

      void Lock()
      {
         #if (JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1)
         CheckForReentrantUse();
         #endif

         #if (JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 1)
         ::AcquireSRWLockExclusive(&m_lock);
         #else
         ::EnterCriticalSection(&m_lock);
         #endif

         #if (JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1)
         m_owningThreadId = ::GetCurrentThreadId();
         #endif
      }

      void Unlock()
      {
         #if (JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1)
         m_owningThreadId = 0;
         #endif

         #if (JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 1)
         ::ReleaseSRWLockExclusive(&m_lock);
         #else
         ::LeaveCriticalSection(&m_lock);
         #endif
      }

   private :

      #if (JETBYTE_HAS_SRW_LOCK_TRY_ENTER == 1)
      SRWLOCK m_lock;
      #else
      CRITICAL_SECTION m_lock;
      #endif

      #if (JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1)
      DWORD m_owningThreadId;

      inline void CheckForReentrantUse()
      {
         if (m_owningThreadId == ::GetCurrentThreadId())
         {
            #ifdef JETBYTE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_DEBUG_BREAK
            ::DebugBreak();
            #endif

            throw CException(
               _T("CLockableObject::CheckForRecursion()"),
               _T("Reentrant use detected."));
         }
      }
      #endif

      /// No copies do not implement
      TLockableObject(const TLockableObject &rhs);
      /// No copies do not implement
      TLockableObject &operator=(const TLockableObject &rhs);
};

class CLockableObject : public TLockableObject<ILockableObject>
{
};

class CSlimLockableObject : public TLockableObject<CEmptyBase>
{
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_LOCKABLE_OBJECT_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: LockableObject.h
///////////////////////////////////////////////////////////////////////////////
