///////////////////////////////////////////////////////////////////////////////
// File: DebugTrace.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2007 JetByte Limited.
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

#include "DebugTrace.h"
#include "SimpleMessageLog.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static void DebugTraceAtExitDetector();

///////////////////////////////////////////////////////////////////////////////
// Static variables
///////////////////////////////////////////////////////////////////////////////

static CSimpleMessageLog s_simpleMessageLog;

static bool s_processIsExiting = false;

// This reference is here purely to force the function scoped static to be
// initialised when all other 'file scope' statics are initialised - i.e. at
// program start up and in a thread-safe manner...

static CDebugTrace &s_notUsed = CDebugTrace::Instance(); //lint !e528 (static symbol not referenced)

///////////////////////////////////////////////////////////////////////////////
// CDebugTrace
///////////////////////////////////////////////////////////////////////////////

CDebugTrace::CDebugTrace()
   :  CMessageLog(s_simpleMessageLog)
{
   atexit(DebugTraceAtExitDetector);
}

bool CDebugTrace::IsValid()
{
   return !s_processIsExiting;
}

CDebugTrace &CDebugTrace::Instance()
{
#pragma warning(push)
#pragma warning(disable: 4640)   // construction of local static object is not thread-safe

   // Note that we call GetInstance() during 'file scope' static object
   // construction and this removes the thread-safety issues...

   static CDebugTrace s_instance;

#pragma warning(pop)

   return s_instance;
}

///////////////////////////////////////////////////////////////////////////////
// CDebugTrace::LogInstaller
///////////////////////////////////////////////////////////////////////////////

CDebugTrace::LogInstaller::LogInstaller(
   ILogMessages &log)
   :  m_pOldLog(CDebugTrace::Instance().SetLog(log))
{

}

CDebugTrace::LogInstaller::~LogInstaller()
{
   try
   {
      Uninstall();
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
}

void CDebugTrace::LogInstaller::Uninstall()
{
   if (m_pOldLog)
   {
      ILogMessages *pPreviousLog = CDebugTrace::Instance().SetLog(*m_pOldLog);

      (void)pPreviousLog;

      m_pOldLog = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static void DebugTraceAtExitDetector()
{
   s_processIsExiting = true;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: DebugTrace.cpp
///////////////////////////////////////////////////////////////////////////////
