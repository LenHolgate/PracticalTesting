#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: LockableObject.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2012 JetByte Limited.
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

#include "JetByteTools/CoreTools/EmptyBase.h"
#include "JetByteTools/CoreTools/ILockableObject.h"
#include "JetByteTools/CoreTools/Exception.h"

#if (JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP == 1)
#include "JetByteTools/CoreTools/DebugTrace.h"
#include "JetByteTools/CoreTools/CrashDumpGenerator.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

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
         #if (JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1)
         : m_owningThreadId(0)
         #endif
      {
         #if (JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS == 0)
         InitializeSRWLock(&m_lock);
         #else
         ::InitializeCriticalSection(&m_lock);
         #endif
      }

      TLockableObject(
         const TLockableObject &rhs) = delete;

      #if (JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS == 1)
      ~TLockableObject()
      {
         ::DeleteCriticalSection(&m_lock);
      }
      #else
      ~TLockableObject() = default;
      #endif

      TLockableObject &operator=(
         const TLockableObject &rhs) = delete;

      bool TryLock()
      {
         #if (JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS == 0)
            const bool locked = (0 != TryAcquireSRWLockExclusive(&m_lock));
         #else
            const bool locked = (0 != ::TryEnterCriticalSection(&m_lock));

            #if (JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1)
            if (locked)
            {
               m_owningThreadId = GetCurrentThreadId();
            }
            else
            {
               CheckForReentrantUse();
            }
            #endif
         #endif

         return locked;
      }

      void Lock()
      {
         #if (JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1)
         CheckForReentrantUse();
         #endif

         #if (JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS == 0)
         AcquireSRWLockExclusive(&m_lock);
         #else
         ::EnterCriticalSection(&m_lock);

         #if (JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1)
         m_owningThreadId = GetCurrentThreadId();
         #endif
         #endif
      }

      void Unlock()
      {
         #if (JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS == 0)
         ReleaseSRWLockExclusive(&m_lock);
         #else
         #if (JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1)
         m_owningThreadId = 0;
         #endif
         ::LeaveCriticalSection(&m_lock);
         #endif
      }

   private :

      #if (JETBYTE_CORE_LOCKABLE_OBJECT_USE_CRITICAL_SECTIONS == 0)
      SRWLOCK m_lock;
      #else
      CRITICAL_SECTION m_lock;

      #if (JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE == 1)
      DWORD m_owningThreadId;

      void CheckForReentrantUse() const
      {
         if (m_owningThreadId == GetCurrentThreadId())
         {
            #if (JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_DEBUG_BREAK == 1)
            ::DebugBreak();
            #endif

            #if (JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_GENERATE_CRASH_DUMP == 1)
            if (JetByteTools::Core::CCrashDumpGenerator::GenerateDumpHere(_T("LockRecursion"), JetByteTools::Core::CCrashDumpGenerator::PerDumpTypeMaxDumpLimits))
            {
               JetByteTools::Core::OutputEx(_T("WARNING - LockableObject lock recursion detected - dump generated"));
            }
            #endif

            #if (JETBYTE_CORE_LOCKABLE_OBJECT_CHECK_FOR_REENTRANT_USE_EXCEPTION == 1)
            throw Core::CException(
               _T("CLockableObject::CheckForReentrantUse()"),
               _T("Reentrant use detected."));
            #endif
         }
      }
      #endif
      #endif
};

class CLockableObject : public TLockableObject<ILockableObject>
{
};

class CSlimLockableObject : public TLockableObject<CEmptyBase>
{
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: LockableObject.h
///////////////////////////////////////////////////////////////////////////////
