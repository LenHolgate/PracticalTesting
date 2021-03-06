#pragma once
#ifndef JETBYTE_TOOLS_WIN32_I_REENTRANT_LOCKABLE_OBJECT_INCLUDED__
#define JETBYTE_TOOLS_WIN32_I_REENTRANT_LOCKABLE_OBJECT_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IReentrantLockableObject.h
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

#include "ILockableObject.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// IReentrantLockableObject
///////////////////////////////////////////////////////////////////////////////

/// An interface onto objects that can be locked and where the same thread
/// is able to call Lock() multiple times successfully and where each call to
/// Lock() needs to be balanced by a corresponding call to Unlock() to unlock
/// the object.
/// \ingroup Synchronization
/// \ingroup Interfaces

class IReentrantLockableObject : public virtual ILockableObject
{
   public :

      typedef TLockableObjectOwner<IReentrantLockableObject> Owner;

      typedef TLockableObjectConditionalOwner<IReentrantLockableObject> ConditionalOwner;

      typedef TReentrantLockableObjectPotentialOwner<IReentrantLockableObject> PotentialOwner;

      /// Instances of this interface can be deleted by their users.

      virtual ~IReentrantLockableObject() = default;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_I_REENTRANT_LOCKABLE_OBJECT_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IReentrantLockableObject.h
///////////////////////////////////////////////////////////////////////////////
