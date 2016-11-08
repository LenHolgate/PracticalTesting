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

#if (JETBYTE_CATCH_AND_LOG_UNHANDLED_EXCEPTIONS_IN_DESTRUCTORS == 1)
#include "DebugTrace.h"
#endif

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
// Lint options
///////////////////////////////////////////////////////////////////////////////
//lint -esym(1566, JetByteTools::Win32::CCallbackTimerQueue::m_maintenanceTimerHandler)
//lint -esym(1506, JetByteTools::Win32::CCallbackTimerQueueBase::CreateTimer)

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::CCallbackTimerQueue()
   :  m_tickProvider(s_tickProvider),
      m_lastCount(s_zeroLargeInteger),
      m_maintenanceTimer(CreateTimer())
{
   SetMaintenanceTimer();
}

CCallbackTimerQueue::CCallbackTimerQueue(
   IMonitorCallbackTimerQueue &monitor)
   :  CCallbackTimerQueueBase(monitor),
      m_tickProvider(s_tickProvider),
      m_lastCount(s_zeroLargeInteger),
      m_maintenanceTimer(CreateTimer())
{
   SetMaintenanceTimer();
}

CCallbackTimerQueue::CCallbackTimerQueue(
   const IProvideTickCount &tickProvider)
   :  m_tickProvider(tickProvider),
      m_lastCount(s_zeroLargeInteger),
      m_maintenanceTimer(CreateTimer())
{
   SetMaintenanceTimer();
}

CCallbackTimerQueue::CCallbackTimerQueue(
   IMonitorCallbackTimerQueue &monitor,
   const IProvideTickCount &tickProvider)
   :  CCallbackTimerQueueBase(monitor),
      m_tickProvider(tickProvider),
      m_lastCount(s_zeroLargeInteger),
      m_maintenanceTimer(CreateTimer())
{
   SetMaintenanceTimer();
}

CCallbackTimerQueue::~CCallbackTimerQueue()
{
   try
   {
      const bool wasPending = DestroyTimer(m_maintenanceTimer); //lint !e1506 (Call to virtual function within a constructor or destructor --- Eff. C++ 3rd Ed. item 9)

      (void)wasPending;
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
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

   return static_cast<ULONGLONG>(m_lastCount.QuadPart);
}

void CCallbackTimerQueue::SetMaintenanceTimer()
{
   const ULONGLONG now = GetTickCount64();

   const ULONGLONG absoluteTimeout = (now & 0xFFFFFFFF00000000) + 0x0000000100000000;

   const bool wasPending = SetInternalTimer(m_maintenanceTimer, m_maintenanceTimerHandler, absoluteTimeout);

   (void)wasPending;
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
