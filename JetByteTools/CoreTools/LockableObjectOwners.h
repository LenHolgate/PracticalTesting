#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: LockableObjectOwners.h
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

#include "JetByteTools/Admin/Admin.h"

#include "Exception.h"

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#include "DebugTrace.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

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
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: LockableObjectOwners.h
///////////////////////////////////////////////////////////////////////////////
