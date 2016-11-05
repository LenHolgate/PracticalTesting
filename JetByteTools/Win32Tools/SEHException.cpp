///////////////////////////////////////////////////////////////////////////////
// File: SEHException.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "SEHException.h"

#include "Utils.h"

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

static _tstring Message(
   unsigned int code);

///////////////////////////////////////////////////////////////////////////////
// CSEHException
///////////////////////////////////////////////////////////////////////////////

CSEHException::CSEHException(
   unsigned int code, 
   EXCEPTION_POINTERS *pPointers)
   :  m_where(Where(pPointers)), 
      m_message(Message(code)),
      m_code(code)
{
}

unsigned int CSEHException::GetCode() const
{
   return m_code;
}

_tstring CSEHException::GetWhere() const
{
   return m_where;
}

_tstring CSEHException::GetMessage() const
{
   return m_message;
}

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
// Helpful define
///////////////////////////////////////////////////////////////////////////////

#define CASE_MSG(c) case EXCEPTION_##c: \
{ static const _tstring msg = _T(#c); \
return msg; } \
break;

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static _tstring Where(
   EXCEPTION_POINTERS *pPointers)
{
   TCHAR buffer[11];

   _stprintf(buffer, _T("0x%.8x"), pPointers->ExceptionRecord->ExceptionAddress);
   
   return buffer;
}

static _tstring Message(
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

   return _T("Unknown exception");
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: SEHException.cpp
///////////////////////////////////////////////////////////////////////////////
