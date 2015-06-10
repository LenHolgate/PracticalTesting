#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_CALLBACK_TIMER_INCLUDED__
#define JETBYTE_TOOLS_CALLBACK_TIMER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimer.h
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

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -save
//
// Private copy constructor
//lint -esym(1704, Handle::Handle)
//
// No default constructor   
//lint -esym(1712, Handle)
//
///////////////////////////////////////////////////////////////////////////////

#include "CriticalSection.h"
#include "AutoResetEvent.h"
#include "Thread.h"
#include "TODO.h"

#include "JetByteTools\C++Tools\NodeList.h"

#pragma TODO("BUG: - need to handle GetTickCount() wrap")

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimer
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimer : protected CThread 
{
   public :

      class Handle;

      friend class Handle;

      CCallbackTimer();

      ~CCallbackTimer();

      void SetTimer(
         const Handle &hnd,
         DWORD millisecondTimeout,
         DWORD userData = 0);

      bool CancelTimer(
         const Handle &hnd);

      void InitiateShutdown();

   private :

      // Implement CThread

      virtual int Run();

      void SignalStateChange();

      class Node;

      friend class Node;

      void InsertNodeIntoPendingList(
         Node *pNode,
         DWORD millisecondTimeout,
         DWORD userData);

      DWORD HandleTimeouts() const;

      void HandleTimeout(
         Node *pNode) const;

      bool CancelTimer(
         Node *pNode);

      TNodeList<Node> m_pendingList;

      CCriticalSection m_criticalSection;
      CAutoResetEvent m_stateChangeEvent;

      volatile bool m_shutdown;
};

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimer::Handle
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimer::Handle
{
   public :

      class Callback
      {
         public :

            virtual void OnTimer(
               Handle &hnd,
               DWORD userData) = 0;

            virtual ~Callback() {}
      };

      class Data;

      explicit Handle(
         Callback &callback);

      explicit Handle(
         Data *pData);
      
      Handle(
         const Handle &rhs);

      ~Handle();

      Handle &operator =(const Handle &rhs);

      bool operator ==(const Handle &rhs) const;
      bool operator !=(const Handle &rhs) const;
      bool operator <(const Handle &rhs) const;

      Data *Detatch();

//      void CancelTimer() const;

   private :

      friend class CCallbackTimer::Node;
      friend class CCallbackTimer;

      explicit Handle(
         CCallbackTimer::Node *pNode);

      static CCallbackTimer::Node *SafeAddRef(
         CCallbackTimer::Node *pNode);

      static CCallbackTimer::Node *SafeRelease(
         CCallbackTimer::Node *pNode);

      CCallbackTimer::Node *m_pNode;
};

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

#endif // JETBYTE_TOOLS_CALLBACK_TIMER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimer.h
///////////////////////////////////////////////////////////////////////////////
