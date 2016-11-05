///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueEx.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2008 JetByte Limited.
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

#include "CallbackTimerQueueEx.h"
#include "TickCount64Provider.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

static const CTickCount64Provider s_tickProvider;

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueEx
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueueEx::CCallbackTimerQueueEx(
   const IProvideTickCount64 &tickProvider)
   :  m_tickProvider(tickProvider)
{

}

CCallbackTimerQueueEx::CCallbackTimerQueueEx()
   :  m_tickProvider(s_tickProvider)
{

}

CCallbackTimerQueueEx::~CCallbackTimerQueueEx()
{

}

ULONGLONG CCallbackTimerQueueEx::GetTickCount64()
{
   return m_tickProvider.GetTickCount64();
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
