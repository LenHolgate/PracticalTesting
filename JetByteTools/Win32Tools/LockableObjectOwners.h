#pragma once
#ifndef JETBYTE_TOOLS_WIN32_LOCKABLE_OBJECT_OWNERS_INCLUDED__
#define JETBYTE_TOOLS_WIN32_LOCKABLE_OBJECT_OWNERS_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: LockableObjectOwners.h
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

#include "Exception.h"

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#include "DebugTrace.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// TLockableObjectOwner
///////////////////////////////////////////////////////////////////////////////

/// A class that takes ownership of a lockable object.
/// That is it calls Lock() in the constructor and Unlock() in the destructor
/// and can therefore be used to support \ref RAII "scope based" locking
/// and unlocking of instances of the object.
/// \ingroup Synchronization
/// \ingroup RAII

template <typename T>
class TLockableObjectOwner
{
   public:

      explicit TLockableObjectOwner(
         T &lock)
         : m_lock(lock)
      {
         m_lock.Lock();
      }

      TLockableObjectOwner(
         const TLockableObjectOwner &rhs) = delete;

      ~TLockableObjectOwner()
      {
         try
         {
            m_lock.Unlock();
         }
         JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
      }

      TLockableObjectOwner &operator=(
         const TLockableObjectOwner &rhs) = delete;

   private :

      T &m_lock;
};

/// A class that may take ownership of a lockable object.
/// That is it calls Lock() in the constructor and Unlock() in the
/// destructor (but only if locked passed as true to the constructor)
/// and can therefore be used to support \ref RAII "scope based" locking
/// and unlocking of instances of the object.
/// \ingroup Synchronization
/// \ingroup RAII

template <typename T>
class TLockableObjectConditionalOwner
{
   public:

      explicit TLockableObjectConditionalOwner(
         T &lock,
         const bool locked = true)
         :  m_lock(lock),
            m_locked(locked)
      {
         if (m_locked)
         {
            m_lock.Lock();
         }
      }

      TLockableObjectConditionalOwner(
         const TLockableObjectConditionalOwner &rhs) = delete;

      ~TLockableObjectConditionalOwner()
      {
         try
         {
            if (m_locked)
            {
               m_lock.Unlock();
            }
         }
         JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
      }

      TLockableObjectConditionalOwner &operator=(
         const TLockableObjectConditionalOwner &rhs) = delete;

      void Unlock()
      {
         if (m_locked)
         {
            m_lock.Unlock();

            m_locked = false;
         }
      }

   private :

      T &m_lock;

      bool m_locked;
};

/// A class that could take ownership of an instance of a lockable object.
/// If you call Lock() or TryLock() on this class it will keep track
/// of the fact that the lockable object has been locked and will
/// call Unlock() in the destructor. This can therefore be used to
/// support \ref RAII "scope based" locking and unlocking of instances
/// of the object.
/// \ingroup Synchronization
/// \ingroup RAII

template <typename T>
class TLockableObjectPotentialOwner
{
   public:

      explicit TLockableObjectPotentialOwner(
         T &lock,
         const bool locked = false)
         :  m_lock(lock),
            m_locked(locked)
      {
         if (m_locked)
         {
            m_lock.Lock();
         }
      }

      TLockableObjectPotentialOwner(
         const TLockableObjectPotentialOwner &rhs) = delete;

      ~TLockableObjectPotentialOwner()
      {
         try
         {
            if (m_locked)
            {
               m_lock.Unlock();
            }
         }
         JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
      }

      TLockableObjectPotentialOwner &operator=(
         const TLockableObjectPotentialOwner &rhs) = delete;

      void Lock()
      {
         if (m_locked)
         {
            throw CException(
               _T("TLockableObjectPotentialOwner<>::Lock()"),
               _T("Already locked"));
         }

         m_lock.Lock();

         m_locked = true;
      }

      bool TryLock()
      {
         if (m_locked)
         {
            throw CException(
               _T("TLockableObjectPotentialOwner<>::Lock()"),
               _T("Already locked"));
         }

         m_locked = m_lock.TryLock();

         return m_locked;
      }

      void Unlock()
      {
         if (m_locked)
         {
            m_lock.Unlock();

            m_locked = false;
         }
      }

   private :

      T &m_lock;

      bool m_locked;
};

template <typename T>
class TReentrantLockableObjectPotentialOwner
{
   public:

      explicit TReentrantLockableObjectPotentialOwner(
         T &lock)
         :  m_lock(lock),
            m_locked(false)
      {
      }


      TReentrantLockableObjectPotentialOwner(
         const TReentrantLockableObjectPotentialOwner &rhs) = delete;

      ~TReentrantLockableObjectPotentialOwner()
      {
         try
         {
            if (m_locked)
            {
               m_lock.Unlock();
            }
         }
         JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
      }

      TReentrantLockableObjectPotentialOwner &operator=(
         const TReentrantLockableObjectPotentialOwner &rhs) = delete;

      void Lock()
      {
         if (m_locked)
         {
            throw CException(
               _T("TReentrantLockableObjectPotentialOwner<>::Lock()"),
               _T("Already locked"));
         }

         m_lock.Lock();

         m_locked = true;
      }

      bool TryLock()
      {
         if (m_locked)
         {
            throw CException(
               _T("TReentrantLockableObjectPotentialOwner<>::Lock()"),
               _T("Already locked"));
         }

         const bool locked = m_lock.TryLock();

         if (locked)
         {
            m_locked = true;
         }

         return locked;
      }

      void Unlock()
      {
         if (m_locked)
         {
            m_lock.Unlock();

            m_locked = false;
         }
      }

   private :

      T &m_lock;

      bool m_locked;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_LOCKABLE_OBJECT_OWNERS_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: LockableObjectOwners.h
///////////////////////////////////////////////////////////////////////////////
