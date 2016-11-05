#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_I_KERNEL_OBJECT_NAME__
#define JETBYTE_TOOLS_WIN32_I_KERNEL_OBJECT_NAME__
///////////////////////////////////////////////////////////////////////////////
// File: IKernelObjectName.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2005 JetByte Limited.
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

#include "tstring.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// IKernelObjectName
///////////////////////////////////////////////////////////////////////////////

/// An interface that represents a name in the kernel object namespace 
/// See <a href="http://msdn2.microsoft.com/en-us/library/aa382954.aspx">here</a>
/// for more details.
/// \ingroup KernelObjects
/// \ingroup ProtectedDestructors

class IKernelObjectName
{
   public :

      virtual _tstring GetName() const = 0;

      virtual bool IsGlobalName() const = 0;

      virtual bool IsLocalName() const = 0;

      virtual bool IsUnqualifiedName() const = 0;

   protected :

      /// We never delete instances of this interface; you must manage the 
      /// lifetime of the class that implements it.

      ~IKernelObjectName() {}
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_WIN32_I_KERNEL_OBJECT_NAME__

///////////////////////////////////////////////////////////////////////////////
// End of file: IKernelObjectName.h
///////////////////////////////////////////////////////////////////////////////

