///////////////////////////////////////////////////////////////////////////////
// File: LoggingCallbackTimerHandle.cpp
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

#include "LoggingCallbackTimerHandle.h"

#include "JetByteTools\Win32Tools\Utils.h"

#include "JetByteTools\TestTools\TestException.h"

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -save
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;

using JetByteTools::Win32::Output;
using JetByteTools::Win32::_tstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CLoggingCallbackTimerHandle
///////////////////////////////////////////////////////////////////////////////

CLoggingCallbackTimerHandle::CLoggingCallbackTimerHandle()
   :  CCallbackTimer::Handle(static_cast<CCallbackTimer::Handle::Callback&>(*this))
{

}

void CLoggingCallbackTimerHandle::OnTimer(
   CCallbackTimer::Handle &hnd,
   DWORD userData)
{
   LogMessage(_T("OnTimer: ") + ToString(userData));

   m_event.Set();
}

bool CLoggingCallbackTimerHandle::WaitForTimer(
   DWORD timeoutMillis)
{
   return m_event.Wait(timeoutMillis);
}

void CLoggingCallbackTimerHandle::CheckTimerNotExpired()
{
   if (WaitForTimer(0))
   {
      throw CException(_T("CLoggingCallbackTimerHandle::CheckTimerNotExpired()"), _T("Timer has expired and it shouldn't have done"));
   }

   CheckResult(_T("|"));
}

void CLoggingCallbackTimerHandle::CheckTimerExpired(
   const DWORD userData,
   const DWORD timeoutMillis)
{
   const _tstring userDataAsString = ToString(userData);

   if (!WaitForTimer(timeoutMillis))
   {
      throw CException(_T("CLoggingCallbackTimerHandle::CheckTimerExpired()"), _T("Timer has not expired and it should have: ") + userDataAsString);
   }

   CheckResult(_T("|OnTimer: ") + userDataAsString + _T("|"));
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
// End of file: LoggingCallbackTimerHandle.cpp
///////////////////////////////////////////////////////////////////////////////

