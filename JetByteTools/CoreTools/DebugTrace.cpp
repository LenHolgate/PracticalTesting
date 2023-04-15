///////////////////////////////////////////////////////////////////////////////
// File: DebugTrace.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2007 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#include "JetByteTools/Admin/Admin.h"

#include "DebugTrace.h"
#include "SimpleMessageLog.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

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

static CDebugTrace &s_notUsed = CDebugTrace::Instance();

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
   ILogMessages *pOptionalLog)
   :  m_pOldLog(pOptionalLog ? Instance().SetLog(*pOptionalLog) : nullptr)
{
}

CDebugTrace::LogInstaller::LogInstaller(
   ILogMessages &log)
   :  m_pOldLog(Instance().SetLog(log))
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
      ILogMessages *pPreviousLog = Instance().SetLog(*m_pOldLog);

      (void)pPreviousLog;

      m_pOldLog = nullptr;
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
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: DebugTrace.cpp
///////////////////////////////////////////////////////////////////////////////
