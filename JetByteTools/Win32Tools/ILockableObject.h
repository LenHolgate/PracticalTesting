#pragma once
#ifndef JETBYTE_TOOLS_WIN32_I_LOCKABLE_OBJECT_INCLUDED__
#define JETBYTE_TOOLS_WIN32_I_LOCKABLE_OBJECT_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ILockableObject.h
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

#include "LockableObjectOwners.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// ILockableObject
///////////////////////////////////////////////////////////////////////////////

/// An interface onto objects that can be locked.
/// \ingroup Synchronization
/// \ingroup Interfaces

class ILockableObject
{
   public :

      //lint -esym(1516, JetByteTools::Win32::*::Owner) (Member declaration hides inherited member --- Eff. C++ 3rd Ed. item 33)
      typedef TLockableObjectOwner<ILockableObject> Owner;

      //lint -esym(1516, JetByteTools::Win32::*::ConditionalOwner) (Member declaration hides inherited member --- Eff. C++ 3rd Ed. item 33)
      typedef TLockableObjectConditionalOwner<ILockableObject> ConditionalOwner;

      //lint -esym(1516, JetByteTools::Win32::*::PotentialOwner) (Member declaration hides inherited member --- Eff. C++ 3rd Ed. item 33)
      typedef TLockableObjectPotentialOwner<ILockableObject> PotentialOwner;

      /// Try to lock the lockable object, returns true if successful
      /// and false if the lock is already locked.

      virtual bool TryLock() = 0;

      /// Lock the lockable object.

      virtual void Lock() = 0;

      /// Unlock the lockable object.

      virtual void Unlock() = 0;

      /// Instances of this interface can be deleted by their users.

      virtual ~ILockableObject() {}
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_I_LOCKABLE_OBJECT_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ILockableObject.h
///////////////////////////////////////////////////////////////////////////////
