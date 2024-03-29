#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: IReentrantLockableObject.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2012 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the �Software�), to deal
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

#include "ILockableObject.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// IReentrantLockableObject
///////////////////////////////////////////////////////////////////////////////

/// An interface onto objects that can be locked and where the same thread
/// is able to call Lock() multiple times successfully and where each call to
/// Lock() needs to be balanced by a corresponding call to Unlock() to unlock
/// the object.
/// \ingroup Synchronization
/// \ingroup Interfaces

class IReentrantLockableObject : public ILockableObject
{
   public :

      typedef TLockableObjectOwner<IReentrantLockableObject> Owner;

      typedef TLockableObjectConditionalOwner<IReentrantLockableObject> ConditionalOwner;

      typedef TReentrantLockableObjectPotentialOwner<IReentrantLockableObject> PotentialOwner;

      /// Instances of this interface can be deleted by their users.

      ~IReentrantLockableObject() override = default;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IReentrantLockableObject.h
///////////////////////////////////////////////////////////////////////////////
