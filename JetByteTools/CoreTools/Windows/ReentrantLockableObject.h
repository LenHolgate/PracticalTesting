#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ReentrantLockableObject.h
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

#include "JetByteTools/CoreTools/Types.h"

#include "JetByteTools/CoreTools/IReentrantLockableObject.h"
#include "JetByteTools/CoreTools/EmptyBase.h"

#include <Windows.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

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
         :  m_lock()
      {
         // On some platforms...
         // Can fail under low memory conditions and rase a STATUS_NO_MEMORY
         // exception.

         InitializeCriticalSection(&m_lock);
      }

      TReentrantLockableObject(
         const TReentrantLockableObject &rhs) = delete;

      virtual ~TReentrantLockableObject()
      {
         DeleteCriticalSection(&m_lock);
      }

      TReentrantLockableObject &operator=(
         const TReentrantLockableObject &rhs) = delete;

      // Implement IReentrantLockableObject

      virtual bool TryLock()
      {
         return 0 != TryEnterCriticalSection(&m_lock);
      }

      virtual void Lock()
      {
         // On some platforms...
         // Can fail if there's contention and the event can't be created and will
         // raise an EXCEPTION_INVALID_HANDLE exception.

         EnterCriticalSection(&m_lock);
      }

      virtual void Unlock()
      {
         LeaveCriticalSection(&m_lock);
      }

   protected :

      CRITICAL_SECTION m_lock;
};

class CReentrantLockableObject : public TReentrantLockableObject<Core::IReentrantLockableObject>
{
};

class CSlimReentrantLockableObject : public TReentrantLockableObject<Core::CEmptyBase>
{
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ReentrantLockableObject.h
///////////////////////////////////////////////////////////////////////////////
