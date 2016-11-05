///////////////////////////////////////////////////////////////////////////////
// File: ThreadedCallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
//
// JetByte Limited grants you ("Licensee") a non-exclusive, royalty free, 
// licence to use, modify and redistribute this software in source and binary 
// code form, provided that i) this copyright notice and licence appear on all 
// copies of the software; and ii) Licensee does not utilize the software in a 
// manner which is disparaging to JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All 
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors 
// shall not be liable for any damages suffered by licensee as a result of 
// using, modifying or distributing the software or its derivatives. In no
// event will JetByte Limited be liable for any lost revenue, profit or data,
// or for direct, indirect, special, consequential, incidental or punitive
// damages, however caused and regardless of the theory of liability, arising 
// out of the use of or inability to use software, even if JetByte Limited 
// has been advised of the possibility of such damages.
//
// This software is not designed or intended for use in on-line control of 
// aircraft, air traffic, aircraft navigation or aircraft communications; or in 
// the design, construction, operation or maintenance of any nuclear 
// facility. Licensee represents and warrants that it will not use or 
// redistribute the Software for such purposes. 
//
///////////////////////////////////////////////////////////////////////////////

#include "ThreadedCallbackTimerQueue.h"
#include "Utils.h"
#include "Exception.h"
#include "TickCountProvider.h"

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -save
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CThreadedCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const IProvideTickCount &tickProvider)
   :  CCallbackTimerQueue(tickProvider),
      m_shutdown(false)
{
   Start();
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const DWORD maxTimeout,
   const IProvideTickCount &tickProvider)
   :  CCallbackTimerQueue(maxTimeout, tickProvider),
      m_shutdown(false)
{   
   Start();
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue()
   :  m_shutdown(false)
{
   Start();
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const DWORD maxTimeout)
   :  CCallbackTimerQueue(maxTimeout),
      m_shutdown(false)
{
   Start();
}

CThreadedCallbackTimerQueue::~CThreadedCallbackTimerQueue()
{
   InitiateShutdown();

   Wait();
}

CThreadedCallbackTimerQueue::Handle CThreadedCallbackTimerQueue::SetTimer(
   Timer &timer,
   const DWORD timeoutMillis,
   const UserData userData)
{
   CCriticalSection::Owner lock(m_criticalSection);
   
   Handle handle = CCallbackTimerQueue::SetTimer(timer, timeoutMillis, userData);

   SignalStateChange();

   return handle;
}

bool CThreadedCallbackTimerQueue::CancelTimer(
   Handle handle)
{
   CCriticalSection::Owner lock(m_criticalSection);
   
   const bool wasPending = CCallbackTimerQueue::CancelTimer(handle);

   SignalStateChange();

   return wasPending;
}

int CThreadedCallbackTimerQueue::Run()
{
   while (!m_shutdown)
   {
      DWORD timeout = GetNextTimeout();
    
      if (timeout == 0)
      {
         CCriticalSection::Owner lock(m_criticalSection);
   
         HandleTimeouts();
      }
      else 
      {
         m_stateChangeEvent.Wait(timeout);
      }
   }
   
   return 0;
}

DWORD CThreadedCallbackTimerQueue::GetNextTimeout()
{
   CCriticalSection::Owner lock(m_criticalSection);

   return CCallbackTimerQueue::GetNextTimeout();
}

void CThreadedCallbackTimerQueue::InitiateShutdown()
{
   m_shutdown = true;

   SignalStateChange();
}

void CThreadedCallbackTimerQueue::SignalStateChange()
{
   m_stateChangeEvent.Set();
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -restore
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadedCallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
