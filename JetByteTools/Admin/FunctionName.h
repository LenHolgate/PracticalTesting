#pragma once
#ifndef JETBYTE_TOOLS_ADMIN_FUNCTION_NAME_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_FUNCTION_NAME_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: FunctionName.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2014 JetByte Limited.
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

#ifdef _UNICODE
#define JETBYTE_WIDEN2(x) L ## x
#define JETBYTE_WIDEN(x) JETBYTE_WIDEN2(x)
#define JETBYTE_FUNCTION_NAME JETBYTE_WIDEN(__FUNCTION__)
#else
#define JETBYTE_FUNCTION_NAME __FUNCTION__
#endif

#endif // JETBYTE_TOOLS_ADMIN_FUNCTION_NAME_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: FunctionName.h
///////////////////////////////////////////////////////////////////////////////

