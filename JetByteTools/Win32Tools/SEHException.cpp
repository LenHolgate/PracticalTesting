///////////////////////////////////////////////////////////////////////////////
// File: SEHException.cpp
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

#include "SEHException.h"

#include "Utils.h"

// There's an issue with VS2002 where the ordering of includes is important
// with regards to the secure CRT stuff, until I spend the time to get it
// right, keep this at the end of the precompiled header..
#include "JetByteTools\Admin\SecureCRT.h"

#pragma hdrstop


///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static _tstring Where(
   EXCEPTION_POINTERS *pPointers);

static const _tstring &Message(
   unsigned int code);

///////////////////////////////////////////////////////////////////////////////
// CSEHException
///////////////////////////////////////////////////////////////////////////////

CSEHException::CSEHException(
   unsigned int code,
   EXCEPTION_POINTERS *pPointers)
   :  m_code(code),
      m_pPointers(pPointers)
{
}

unsigned int CSEHException::GetCode() const
{
   return m_code;
}

const EXCEPTION_POINTERS &CSEHException::GetExceptionPointers() const
{
   return *m_pPointers;
}

const CONTEXT &CSEHException::GetContext() const
{
   return *m_pPointers->ContextRecord;
}

_tstring CSEHException::GetWhere() const
{
   return Where(m_pPointers);
}

const _tstring &CSEHException::GetMessage() const
{
   return Message(m_code);
}


///////////////////////////////////////////////////////////////////////////////
// CSEHException::Translator
///////////////////////////////////////////////////////////////////////////////

CSEHException::Translator::Translator()
   :  m_prev(_set_se_translator(Translator::trans_func))
{

}

CSEHException::Translator::~Translator()
{
   _set_se_translator(m_prev);
}

void CSEHException::Translator::trans_func(
   unsigned int code,
   EXCEPTION_POINTERS *pPointers)
{
   throw CSEHException(code, pPointers);
}

///////////////////////////////////////////////////////////////////////////////
// Helpful defines
///////////////////////////////////////////////////////////////////////////////

#define CASE_MSG(c) case EXCEPTION_##c: \
   return s_##c##_msg

#define DEFINE_MSG(c) static const _tstring s_##c##_msg = _T(#c)

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static _tstring Where(
   EXCEPTION_POINTERS *pPointers)
{
   TCHAR buffer[2 + 16 + 1];

   const int bytesWritten = _stprintf_s(buffer, _T("0x%p"), pPointers->ExceptionRecord->ExceptionAddress);

   (void)bytesWritten;

   return buffer;
}

DEFINE_MSG(ACCESS_VIOLATION);
DEFINE_MSG(DATATYPE_MISALIGNMENT);
DEFINE_MSG(BREAKPOINT);
DEFINE_MSG(SINGLE_STEP);
DEFINE_MSG(ARRAY_BOUNDS_EXCEEDED);
DEFINE_MSG(FLT_DENORMAL_OPERAND);
DEFINE_MSG(FLT_DIVIDE_BY_ZERO);
DEFINE_MSG(FLT_INEXACT_RESULT);
DEFINE_MSG(FLT_INVALID_OPERATION);
DEFINE_MSG(FLT_OVERFLOW);
DEFINE_MSG(FLT_STACK_CHECK);
DEFINE_MSG(FLT_UNDERFLOW);
DEFINE_MSG(INT_DIVIDE_BY_ZERO);
DEFINE_MSG(INT_OVERFLOW);
DEFINE_MSG(PRIV_INSTRUCTION);
DEFINE_MSG(IN_PAGE_ERROR);
DEFINE_MSG(ILLEGAL_INSTRUCTION);
DEFINE_MSG(NONCONTINUABLE_EXCEPTION);
DEFINE_MSG(STACK_OVERFLOW);
DEFINE_MSG(INVALID_DISPOSITION);
DEFINE_MSG(GUARD_PAGE);
DEFINE_MSG(INVALID_HANDLE);

static const _tstring s_unknown = _T("Unknown exception");

static const _tstring &Message(
   unsigned int code)
{
   switch (code)
   {
      CASE_MSG(ACCESS_VIOLATION);
      CASE_MSG(DATATYPE_MISALIGNMENT);
      CASE_MSG(BREAKPOINT);
      CASE_MSG(SINGLE_STEP);
      CASE_MSG(ARRAY_BOUNDS_EXCEEDED);
      CASE_MSG(FLT_DENORMAL_OPERAND);
      CASE_MSG(FLT_DIVIDE_BY_ZERO);
      CASE_MSG(FLT_INEXACT_RESULT);
      CASE_MSG(FLT_INVALID_OPERATION);
      CASE_MSG(FLT_OVERFLOW);
      CASE_MSG(FLT_STACK_CHECK);
      CASE_MSG(FLT_UNDERFLOW);
      CASE_MSG(INT_DIVIDE_BY_ZERO);
      CASE_MSG(INT_OVERFLOW);
      CASE_MSG(PRIV_INSTRUCTION);
      CASE_MSG(IN_PAGE_ERROR);
      CASE_MSG(ILLEGAL_INSTRUCTION);
      CASE_MSG(NONCONTINUABLE_EXCEPTION);
      CASE_MSG(STACK_OVERFLOW);
      CASE_MSG(INVALID_DISPOSITION);
      CASE_MSG(GUARD_PAGE);
      CASE_MSG(INVALID_HANDLE);
   }

   return s_unknown;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: SEHException.cpp
///////////////////////////////////////////////////////////////////////////////
