///////////////////////////////////////////////////////////////////////////////
// File: TestCallbackTimer.cpp
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

#pragma warning(disable: 4355)   // 'this' used in base member initialiser list

#include "TestCallbackTimer.h"

#include "..\Exception.h"
#include "..\Utils.h"

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -save
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CTestCallbackTimer
///////////////////////////////////////////////////////////////////////////////

CTestCallbackTimer::CTestCallbackTimer(
   const DWORD tickCount,
   const DWORD operationTimeoutMillis)
   :  CMockTickCountProvider(tickCount),
      CCallbackTimer(static_cast<IProvideTickCount &>(*this)),
      m_operationTimeoutMillis(operationTimeoutMillis)
{
   if (!AllowCalls(1, m_operationTimeoutMillis))
   {
      throw CException(_T("CTestCallbackTimer::CTestCallbackTimer()"), _T("AllowCalls() failed"));
   }

   m_tickCountAsString = ToString(tickCount);

   const _tstring expectedResult = _T("|GetTickCount: Another Thread: ") + m_tickCountAsString + _T("|");
//   const _tstring expectedResult = _T("|GetTickCount: Main Thread: ") + m_tickCountAsString + _T("|GetTickCount: Another Thread: ") + m_tickCountAsString + _T("|");

   CheckResult(expectedResult);
}

CTestCallbackTimer::~CTestCallbackTimer()
{
   AllowCalls(1000);      
}

void CTestCallbackTimer::SetTimerAndWait(
   CCallbackTimer::Handle &handle,
   const DWORD timeoutMillis,
   const DWORD userData,
   const bool waitForBlock)
{
   SetTimer(handle, timeoutMillis, userData);

   if (!AllowCalls(1, m_operationTimeoutMillis))
   {
      throw CException(_T("CTestCallbackTimer::SetTimerAndWait()"), _T("AllowCalls() failed"));
   }
   
   if (waitForBlock && !WaitForBlockedCall(m_operationTimeoutMillis))
   {
      throw CException(_T("CTestCallbackTimer::SetTimerAndWait()"), _T("WaitForBlockedCall() failed"));
   }

   const _tstring expectedResult = _T("|GetTickCount: Main Thread: ") + m_tickCountAsString + _T("|GetTickCount: Another Thread: " + m_tickCountAsString + _T("|"));

   CheckResult(expectedResult);
}

bool CTestCallbackTimer::CancelTimer(
   CCallbackTimer::Handle &handle)
{
   const bool wasPending = CCallbackTimer::CancelTimer(handle);

   if (!AllowCalls(1, m_operationTimeoutMillis))
   {
      throw CException(_T("CTestCallbackTimer::CancelTimer()"), _T("AllowCalls() failed"));
   }

   const _tstring expectedResult = _T("|GetTickCount: Another Thread: " + m_tickCountAsString + _T("|"));

   CheckResult(expectedResult);

   return wasPending;
}

void CTestCallbackTimer::SetTickCount(
   const DWORD tickCount)
{
   m_tickCountAsString = ToString(tickCount);

   CMockTickCountProvider::SetTickCount(tickCount);
}

void CTestCallbackTimer::SetTickCountAndWait(
   const DWORD tickCount,
   const bool waitForBlock)
{
   m_tickCountAsString = ToString(tickCount);

   CMockTickCountProvider::SetTickCount(tickCount);

   if (!AllowCalls(1, m_operationTimeoutMillis))
   {
      throw CException(_T("CTestCallbackTimer::SetTickCountAndWait()"), _T("AllowCalls() failed"));
   }

   const _tstring expectedResult = _T("|GetTickCount: Another Thread: ") + m_tickCountAsString + _T("|");

   CheckResult(expectedResult);

   if (waitForBlock && !WaitForBlockedCall(m_operationTimeoutMillis))
   {
      throw CException(_T("CTestCallbackTimer::SetTickCountAndWait()"), _T("WaitForBlockedCall() failed"));
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -restore
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// End of file: TestCallbackTimer.cpp
///////////////////////////////////////////////////////////////////////////////

