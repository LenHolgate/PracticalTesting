#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_ADMIN_SECURE_CRT_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_SECURE_CRT_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: SecureCRT.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2005 JetByte Limited.
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

/// \file SecureCRT.h
/// This file provides backwards compatibility macros for the secure CRT 
/// functions that Microsoft began supplying starting with VS2005. This allows
/// us to work as if the secure CRT was always available...
/// See <a href="http://msdn2.microsoft.com/en-us/library/8ef0s5kh(VS.80).aspx">here</a>
/// for more details.
/// \ingroup Admin

#include <stdio.h>
#include <tchar.h>

#if (_MSC_VER < 1400)			// Need this for less than VS2005

#define _tcsncpy_s         _tcsncpy
#define _stprintf_s        _stprintf
#define sprintf_s          sprintf
#define strcpy_s(s,l,d)    strcpy(s,d)
#define wcscpy_s(s,l,d)    wcscpy(s,d)

#if (_MSC_VER == 1300)        // Vs.Net screws with the above...
#define _STRSAFE_H_INCLUDED_
#undef swprintf
#endif 

#endif

#endif // JETBYTE_TOOLS_ADMIN_SECURE_CRT_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: SecureCRT.h
///////////////////////////////////////////////////////////////////////////////
