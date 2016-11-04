///////////////////////////////////////////////////////////////////////////////
// File: WaitableCounter.cpp
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

#include "WaitableCounter.h"

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
// CWaitableCounter
///////////////////////////////////////////////////////////////////////////////

CWaitableCounter::CWaitableCounter(
   const long initialCount)
{
   SetValue(initialCount);
}

void CWaitableCounter::WaitForZero() const
{
   m_atZeroEvent.Wait();
}

bool CWaitableCounter::WaitForZero(
   DWORD timeoutMillis) const
{
   return m_atZeroEvent.Wait(timeoutMillis);
}

void CWaitableCounter::WaitForNonZero() const
{
   m_notAtZeroEvent.Wait();
}

bool CWaitableCounter::WaitForNonZero(
   DWORD timeoutMillis) const
{
   return m_notAtZeroEvent.Wait(timeoutMillis);
}

void CWaitableCounter::SetValue(
   const long value)
{
   CCriticalSection::Owner lock(m_criticalSection);

   m_count = value;
   
   if (value == 0)
   {
      m_atZeroEvent.Set();
      m_notAtZeroEvent.Reset();
   }
   else
   {
      m_atZeroEvent.Reset();
      m_notAtZeroEvent.Set();
   }         
}

void CWaitableCounter::Increment()
{
   CCriticalSection::Owner lock(m_criticalSection);

   if (0 == m_count)
   {
      m_atZeroEvent.Reset();
      m_notAtZeroEvent.Set();
   }         

   m_count++;
}

void CWaitableCounter::Decrement()
{
   CCriticalSection::Owner lock(m_criticalSection);

   m_count--;

   if (0 == m_count)
   {
      m_atZeroEvent.Set();
      m_notAtZeroEvent.Reset();
   }         
}

long CWaitableCounter::GetValue() const
{
   CCriticalSection::Owner lock(m_criticalSection);

   return m_count;
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
// End of file: Event.cpp
///////////////////////////////////////////////////////////////////////////////
