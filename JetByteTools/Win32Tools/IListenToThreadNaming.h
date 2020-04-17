#pragma once
#ifndef JETBYTE_TOOLS_WIN32_I_LISTEN_TO_THREAD_NAMING_INCLUDED__
#define JETBYTE_TOOLS_WIN32_I_LISTEN_TO_THREAD_NAMING_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IListenToThreadNaming.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2018 JetByte Limited.
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
// IListenToThreadNaming
///////////////////////////////////////////////////////////////////////////////

class IListenToThreadNaming
{
   public :

      virtual void OnThreadNaming(
         const _tstring &threaId,
         const _tstring &threadName) = 0;

   protected :

      /// We never delete instances of this interface; you must manage the
      /// lifetime of the class that implements it.

      virtual ~IListenToThreadNaming() = default;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_I_LISTEN_TO_THREAD_NAMING_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IRunnable.h
///////////////////////////////////////////////////////////////////////////////
