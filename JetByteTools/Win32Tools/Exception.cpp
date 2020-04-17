///////////////////////////////////////////////////////////////////////////////
// File: Exception.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 1997 JetByte Limited.
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

#include "JetByteTools/Admin/Admin.h"

#include "Exception.h"

#include "StringConverter.h"

#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)
#include "CallStack.h"
#include "StackWalker.h"
#endif

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)
static void CaptureStackBackTrace(
   CCallStack &callStack);
#endif

///////////////////////////////////////////////////////////////////////////////
// CExceptionStackWalker
///////////////////////////////////////////////////////////////////////////////

#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)

class CExceptionStackWalker : public StackWalker
{
   public:

      CExceptionStackWalker()
      :  StackWalker(
            StackWalker::RetrieveVerbose |
            StackWalker::SymBuildPath)
      {
      }

      virtual void OnStackOutput(LPCSTR szText)
      {
         m_callStack += CStringConverter::AtoT(szText);
         m_callStack += _T("\r\n");
      }

      _tstring m_callStack;
};

#endif

///////////////////////////////////////////////////////////////////////////////
// CException
///////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
CException::CException(
   const std::string &where,
   const std::string &message)
   :  m_where(CStringConverter::AtoT(where)),
      m_message(CStringConverter::AtoT(message)),
      m_details(m_where + _T(" - ") + m_message)
{
#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)
   CaptureStackBackTrace(m_stack);
#endif
}
#endif

CException::CException(
   const _tstring &where,
   const _tstring &message)
   :  m_where(where),
      m_message(message),
      m_details(m_where + _T(" - ") + m_message)
{
#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)
   CaptureStackBackTrace(m_stack);
#endif
}

_tstring CException::GetWhere() const
{
#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)

#if (JETBYTE_EXCEPTION_WHERE_INCLUDES_STACK_TRACE == 1)

   return m_where + _T("\r\n") + GetCallStack();

#endif

#endif

   return m_where;
}

_tstring CException::GetMessage() const
{
   return m_message;
}

_tstring CException::GetDetails() const
{
   return m_details;
}

std::string CException::GetDetailsA() const
{
   return CStringConverter::TtoA(m_details);
}

void CException::MessageBox(
   const HWND hWnd) const
{
   ::MessageBox(hWnd, GetMessage().c_str(), GetWhere().c_str(), MB_ICONSTOP);
}

#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)

_tstring CException::GetCallStack() const
{
   CExceptionStackWalker sw;

   sw.ShowCallstack(m_stack);

   return sw.m_callStack;
}

#else

_tstring CException::GetCallStack() const
{
   return _T("Callstack collection not enabled");
}

#endif

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

#if (JETBYTE_EXCEPTION_STACK_TRACES == 1)

static void CaptureStackBackTrace(
   CCallStack &callStack)
{
   // xp and 2003 sum of framesToSkip + framesToCapture must be < 64

   const DWORD framesToCapture = 10;

   callStack.Resize(framesToCapture);

   void *pBuffer = callStack.GetMemory();

   const unsigned short numFrames = ::CaptureStackBackTrace(
      1,
      framesToCapture,
      (void **)pBuffer,
      0);

   callStack.Use(numFrames);

   // do we care about 'incomplete' stacks, if so, we can guess if the
   // stack uses up all available space then it's incomplete...
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Exception.cpp
///////////////////////////////////////////////////////////////////////////////
