#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ILockableObject.h
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

#include "LockableObjectOwners.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// ILockableObject
///////////////////////////////////////////////////////////////////////////////

/// An interface onto objects that can be locked.
/// \ingroup Synchronization
/// \ingroup Interfaces

class ILockableObject
{
   public :

      typedef TLockableObjectOwner<ILockableObject> Owner;

      typedef TLockableObjectConditionalOwner<ILockableObject> ConditionalOwner;

      typedef TLockableObjectPotentialOwner<ILockableObject> PotentialOwner;

      /// Try to lock the lockable object, returns true if successful
      /// and false if the lock is already locked.

      virtual bool TryLock() = 0;

      /// Lock the lockable object.

      virtual void Lock() = 0;

      /// Unlock the lockable object.

      virtual void Unlock() = 0;

      /// Instances of this interface can be deleted by their users.

      virtual ~ILockableObject() = default;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ILockableObject.h
///////////////////////////////////////////////////////////////////////////////
