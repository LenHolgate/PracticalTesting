///////////////////////////////////////////////////////////////////////////////
// File: MockTickCountProvider.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
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

#include "JetByteTools\Admin\Admin.h"

#include "MockTickCountProvider.h"

#include "JetByteTools\Win32Tools\Utils.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::ToString;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockTickCountProvider
///////////////////////////////////////////////////////////////////////////////

CMockTickCountProvider::CMockTickCountProvider()
   :  logMessages(true),
      logTickCount(true),
      m_tickCount(0)
{
}

CMockTickCountProvider::CMockTickCountProvider(
   const Milliseconds tickCount)
   :  logMessages(true),
      logTickCount(true),
      m_tickCount(tickCount)
{
}

void CMockTickCountProvider::SetTickCount(
   const Milliseconds tickCount)
{
   ::InterlockedExchange(const_cast<long *>(reinterpret_cast<volatile long*>(&m_tickCount)), tickCount);
}

Milliseconds CMockTickCountProvider::GetTickCount() const
{
   if (logMessages)
   {
      if (logTickCount)
      {
         LogMessage(_T("GetTickCount: ") + ToString(m_tickCount));
      }
      else
      {
         LogMessage(_T("GetTickCount"));
      }
   }

   return m_tickCount;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTickCountProvider.cpp
///////////////////////////////////////////////////////////////////////////////

