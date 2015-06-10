///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimer.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2002 JetByte Limited.
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

#include "CallbackTimer.h"
#include "Utils.h"
#include "Exception.h"

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -save
//
//lint -e1551   function may throw exception in dtor
//
//  Class member is a reference
//lint -esym(1725, Node::m_callback)     
//
// Symbol did not appear in the constructor initialiser list 
//lint -esym(1928, CThread)
//lint -esym(1928, Node)
//
// Symbol's default constructor implicitly called
//lint -esym(1926, CCallbackTimer::m_pendingList)
//lint -esym(1926, CCallbackTimer::m_criticalSection)
//lint -esym(1926, CCallbackTimer::m_stateChangeEvent)
//
// Ignoring return value
//lint -esym(534, CCallbackTimer::CancelTimer)
//lint -esym(534, CEvent::Wait)
//lint -esym(534, InterlockedIncrement)
//
// Base class dtor isnt virtual (but it's protected, so why complain?)
//lint -esym(1509, Node)
//
// No default constructor   
//lint -esym(1712, Node)
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimer::Node
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimer::Node : public CNodeList::Node
{
   public :

      friend CCallbackTimer;

      explicit Node(
         CCallbackTimer::Handle::Callback &callback);

      void SetTimeout(
         DWORD millisecondTimeout,
         DWORD userData);

      void AddRef();
      void Release();

      void OnTimer();

   private :

      DWORD m_millisecondTimeout;

      long m_ref;

      CCallbackTimer::Handle::Callback &m_callback;

      DWORD m_userData;
};

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimer
///////////////////////////////////////////////////////////////////////////////

CCallbackTimer::CCallbackTimer()
   :  m_shutdown(false)
{
   Start();
}

CCallbackTimer::~CCallbackTimer()
{
   InitiateShutdown();

   Wait();

   Node *pNode = m_pendingList.Head();

   while (pNode)
   {
      CancelTimer(pNode);

      pNode = m_pendingList.Head();
   }
}

int CCallbackTimer::Run()
{
   while (!m_shutdown)
   {
      const DWORD timeout = HandleTimeouts();
      
      m_stateChangeEvent.Wait(timeout);
   }
   
   return 0;
}

DWORD CCallbackTimer::HandleTimeouts() const
{
   const DWORD now = ::GetTickCount();

   Node *pNode = m_pendingList.Head();
   
   while (pNode)
   {
      if (pNode->m_millisecondTimeout > now)
      {
         return pNode->m_millisecondTimeout - now;
      }
      else
      {
         HandleTimeout(pNode);
      }

      pNode = m_pendingList.Head();
   }

   return INFINITE;
}

void CCallbackTimer::HandleTimeout(
   Node *pNode) const
{
   pNode->RemoveFromList();

   pNode->OnTimer();

   pNode->Release();
}

void CCallbackTimer::SetTimer(
   const Handle &hnd,
   DWORD millisecondTimeout,
   DWORD userData /* = 0 */)
{
   Node *pNode = hnd.m_pNode;

   if (!pNode)
   {
      throw CException(_T("CCallbackTimer::SetTimer()"),_T("Invalid handle"));
   }

   InsertNodeIntoPendingList(pNode, millisecondTimeout, userData);

   SignalStateChange();
}

void CCallbackTimer::InsertNodeIntoPendingList(
   Node *pNewNode,
   DWORD millisecondTimeout,
   DWORD userData)
{
   CCriticalSection::Owner lock(m_criticalSection);

   pNewNode->RemoveFromList();

   pNewNode->SetTimeout(millisecondTimeout, userData);

   pNewNode->AddRef();

   // insert in list in time order ascending

   Node *pNode= m_pendingList.Head();

   while (pNode && pNode->m_millisecondTimeout < pNewNode->m_millisecondTimeout && m_pendingList.Next(pNode))
   {
      pNode = m_pendingList.Next(pNode);
   }

   if (pNode)
   {
//      Output(_T("Insert: ") + ToString(pNewNode) + _T(" time:") + ToString(pNewNode->m_millisecondTimeout));
//      Output(_T("After: ") + ToString(pNode) + _T(" time:") + ToString(pNode->m_millisecondTimeout));

      m_pendingList.InsertAfter(pNode, pNewNode);         
   }
   else
   {
//      Output(_T("Push: ") + ToString(pNewNode) + _T(" time:") + ToString(pNewNode->m_millisecondTimeout));

      m_pendingList.PushNode(pNewNode);
   }
}

bool CCallbackTimer::CancelTimer(
   const Handle &hnd)
{
   Node *pNode = hnd.m_pNode;

   if (!pNode)
   {
      throw CException(_T("CCallbackTimer::CancelTimer()"),_T("Invalid handle"));
   }

   const bool wasPending = CancelTimer(pNode);

   SignalStateChange();

   return wasPending;
}

bool CCallbackTimer::CancelTimer(
   Node *pNode)
{
   CCriticalSection::Owner lock(m_criticalSection);

   bool wasPending = false;

   if (pNode->InList(m_pendingList))
   {
      pNode->RemoveFromList();

      pNode->m_millisecondTimeout = 0;

      pNode->Release();

      wasPending = true;
   }

   return wasPending;
}

void CCallbackTimer::InitiateShutdown()
{
   m_shutdown = true;

   SignalStateChange();
}

void CCallbackTimer::SignalStateChange()
{
   m_stateChangeEvent.Set();
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimer::Handle
///////////////////////////////////////////////////////////////////////////////

CCallbackTimer::Handle::Handle(
   CCallbackTimer::Handle::Callback &callback)
   :  m_pNode(new Node(callback))
{

}

CCallbackTimer::Handle::Handle(
   CCallbackTimer::Handle::Data *pData)
   :  m_pNode(reinterpret_cast<CCallbackTimer::Node*>(pData))
{

}

CCallbackTimer::Handle::Handle(
   const Handle &rhs)
   :  m_pNode(SafeAddRef(rhs.m_pNode))
{

}

CCallbackTimer::Handle::Handle(
   CCallbackTimer::Node *pNode)
   :  m_pNode(SafeAddRef(pNode))
{

}

CCallbackTimer::Handle::~Handle()
{
   m_pNode = SafeRelease(m_pNode);

   //lint -e{1740} m_pNode not directly freed of zeroed in dtor
}

CCallbackTimer::Handle::Data *CCallbackTimer::Handle::Detatch()
{
   CCallbackTimer::Node *pNode = m_pNode;

   m_pNode = 0;

   return reinterpret_cast<Data*>(pNode);
}

CCallbackTimer::Handle &CCallbackTimer::Handle::operator =(
   const Handle &rhs)
{
   if (&rhs != this)
   {
      m_pNode = SafeRelease(m_pNode);
      
      m_pNode = SafeAddRef(rhs.m_pNode);
   }

   return *this;
}

bool CCallbackTimer::Handle::operator ==(
   const Handle &rhs) const
{
   return m_pNode == rhs.m_pNode;
}

bool CCallbackTimer::Handle::operator !=(
   const Handle &rhs) const
{
   return !operator ==(rhs);
}

bool CCallbackTimer::Handle::operator <(
   const Handle &rhs) const
{
   //lint -e{613} Possible use of null pointer m_pNode

   return m_pNode < rhs.m_pNode;
}

CCallbackTimer::Node *CCallbackTimer::Handle::SafeAddRef(
   CCallbackTimer::Node *pNode)
{
   if (pNode)
   {
      pNode->AddRef();
   }

   return pNode;
}

CCallbackTimer::Node *CCallbackTimer::Handle::SafeRelease(
   CCallbackTimer::Node *pNode)
{
   if (pNode)
   {
      pNode->Release();
   }

   return 0;

   //lint -e{429} Custodial pointer 'pNode' has not been freed or returned
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimer::Node
///////////////////////////////////////////////////////////////////////////////

CCallbackTimer::Node::Node(
   CCallbackTimer::Handle::Callback &callback)
   :  m_millisecondTimeout(0),
      m_ref(1),
      m_callback(callback),
      m_userData(0)
{

}

void CCallbackTimer::Node::SetTimeout(
   DWORD millisecondTimeout,
   DWORD userData)
{
   const DWORD absoluteTimeout = ::GetTickCount() + millisecondTimeout;

   m_millisecondTimeout = absoluteTimeout;

   m_userData = userData;
}

void CCallbackTimer::Node::OnTimer()
{
   Handle hnd(this);

   const DWORD userData = m_userData;

   m_userData = 0;

   m_callback.OnTimer(hnd, userData);
}

void CCallbackTimer::Node::AddRef()
{
   ::InterlockedIncrement(&m_ref);
}

void CCallbackTimer::Node::Release()
{
   if (m_ref == 0)
   {
      // Error! double release
      throw CException(_T("CCallbackTimer::Node::Release()"), _T("m_ref is already 0"));
   }

   if (0 == ::InterlockedDecrement(&m_ref))
   {
      delete this;
   }
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
// End of file: CallbackTimer.cpp
///////////////////////////////////////////////////////////////////////////////
