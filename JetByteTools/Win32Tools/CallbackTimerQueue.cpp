///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueue.cpp
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

#include "CallbackTimerQueue.h"
#include "Utils.h"
#include "Exception.h"
#include "TickCountProvider.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

static const CTickCountProvider s_tickProvider;

#pragma warning(push, 4)
#pragma warning(disable: 4268)   // 'const' static/global data initialized with 
                                 // compiler generated default constructor fills 
                                 // the object with zeros

static const LARGE_INTEGER s_zeroLargeInteger;  // Rely on static init to provide 
                                                // us with a LARGE_INTEGER zero.
#pragma warning(pop)

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::CCallbackTimerQueue(
   const IProvideTickCount &tickProvider)
   :  m_tickProvider(tickProvider),
      m_lastCount(s_zeroLargeInteger),
      m_maintenanceTimer(CreateTimer())
{   
   SetMaintenanceTimer();
}

CCallbackTimerQueue::CCallbackTimerQueue()
   :  m_tickProvider(s_tickProvider),
      m_lastCount(s_zeroLargeInteger),
      m_maintenanceTimer(CreateTimer())
{
   SetMaintenanceTimer();
}

CCallbackTimerQueue::~CCallbackTimerQueue()
{
   DestroyTimer(m_maintenanceTimer);
}

ULONGLONG CCallbackTimerQueue::GetTickCount64()
{
   const DWORD lastCount = m_lastCount.LowPart;

   m_lastCount.LowPart = m_tickProvider.GetTickCount();

   if (m_lastCount.LowPart < lastCount)
   {
      if (m_lastCount.LowPart < lastCount)
      {
         m_lastCount.HighPart++;

         SetMaintenanceTimer();
      }
   }

   return m_lastCount.QuadPart;
}

void CCallbackTimerQueue::SetMaintenanceTimer()
{
   const ULONGLONG now = GetTickCount64();

   const ULONGLONG absoluteTimeout = (now & 0xFFFFFFFF00000000) + 0x0000000100000000;

   SetInternalTimer(m_maintenanceTimer, m_maintenanceTimerHandler, absoluteTimeout);
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::MaintenanceTimerHandler
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerQueue::MaintenanceTimerHandler::OnTimer(
   UserData userData)
{
   reinterpret_cast<CCallbackTimerQueue *>(userData)->SetMaintenanceTimer();
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
