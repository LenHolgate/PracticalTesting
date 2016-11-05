///////////////////////////////////////////////////////////////////////////////
// File: Utils.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "Utils.h"
#include "Exception.h"
#include "Win32Exception.h"
#include "CriticalSection.h"
#include "SmartHandle.h"

#include "JetByteTools\C++Tools\ExpandableBuffer.h"

#pragma hdrstop

#include "JetByteTools\Admin\SecureCRT.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#if (_MSC_VER < 1300) && (JETBYTE_NO_PLATFORM_SDK == 1)
#pragma MESSAGE("PSAPI.h not available with VC6 without an appropriate Platform SDK")
#else
#include <Psapi.h>
#endif

#include "Lmcons.h"     // UNLEN

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "psapi.lib")

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::endl;
using std::string;
using std::wstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static _tstring DumpData(
   const _tstring &linePrefix,
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength, 
   const bool makePrintable,
   const bool useCR,
   const bool linePrefixOnFirstLine,
   const bool lineFeedOnLastLine);

static string DumpDataA(
   const string &linePrefix,
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength, 
   const bool makePrintable,
   const bool useCR,
   const bool linePrefixOnFirstLine,
   const bool lineFeedOnLastLine);

static bool IsGoodPtr(
   HANDLE hProcess,
   void *pv, 
   ULONG cb, 
   DWORD dwFlags);

static bool StringIsAllANSI(
   const std::wstring &data);

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

bool StringToBool(
   const _tstring &stringRepresentation)
{
   if (stringRepresentation == _T("1"))
   {
      return true;
   }
   else if (stringRepresentation == _T("0"))
   {
      return false;
   }
   else if (0 == _tcsncicmp(stringRepresentation.c_str(), _T("TRUE"), stringRepresentation.length()))
   {
      return true;
   }
   else if (0 == _tcsncicmp(stringRepresentation.c_str(), _T("FALSE"), stringRepresentation.length()))
   {
      return false;
   }

   throw CException(_T("StringToBool()"), _T("Can't convert: \"") + stringRepresentation + _T("\" to a bool value"));
}

DWORD GetStringLengthAsDWORD(
   const string &theString)
{
   const size_t length = theString.length();

   if (length > static_cast<size_t>(std::numeric_limits<DWORD>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<DWORD>(length);
}

DWORD GetStringLengthAsDWORD(
   const wstring &theString)
{
   const size_t length = theString.length();

   if (length > static_cast<size_t>(std::numeric_limits<DWORD>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<DWORD>(length);
}

int GetStringLengthAsInt(
   const string &theString)
{
   const size_t length = theString.length();

   if (length > static_cast<size_t>(std::numeric_limits<int>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<int>(length);
}

int GetStringLengthAsInt(
   const wstring &theString)
{
   const size_t length = theString.length();

   if (length > static_cast<size_t>(std::numeric_limits<int>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<int>(length);
}

int GetStringLengthAsInt(
   const char *pString)
{
   const size_t length = strlen(pString);

   if (length > static_cast<size_t>(std::numeric_limits<int>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<int>(length);
}

int GetStringLengthAsInt(
   const wchar_t *pString)
{
   const size_t length = wcslen(pString);

   if (length > static_cast<size_t>(std::numeric_limits<int>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<int>(length);
}

UINT GetStringLengthAsUInt(
   const string &theString)
{
   const size_t length = theString.length();

   if (length > static_cast<size_t>(std::numeric_limits<UINT>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<UINT>(length);
}

UINT GetStringLengthAsUInt(
   const wstring &theString)
{
   const size_t length = theString.length();

   if (length > static_cast<size_t>(std::numeric_limits<UINT>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<UINT>(length);
}

UINT GetStringLengthAsUInt(
   const char *pString)
{
   const size_t length = strlen(pString);

   if (length > static_cast<size_t>(std::numeric_limits<UINT>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<UINT>(length);
}

UINT GetStringLengthAsUInt(
   const wchar_t *pString)
{
   const size_t length = wcslen(pString);

   if (length > static_cast<size_t>(std::numeric_limits<UINT>::max()))
   {
      throw CException(_T("GetStringLength()"), _T("String is too long to fit: ") + ToString(length));
   }

   return static_cast<UINT>(length);
}

bool IsAllDigits(
   const _tstring &numeric)
{
   bool ok = true;

   for (_tstring::const_iterator it = numeric.begin(); ok && it != numeric.end(); ++it)
   {
      ok = ToBool(_istdigit(*it));
   }

   return ok;
}

_tstring GetLastErrorMessage(
   DWORD last_error,
   bool stripTrailingLineFeed)
{
   TCHAR errmsg[512];

   if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
      0,
      last_error,
      0,
      errmsg, 
      511,
      NULL))
   {
      /* if we fail, call ourself to find out why and return that error */
      return (GetLastErrorMessage(GetLastError()));  
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = _tcslen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }
  
   return errmsg;
}

string GetLastErrorMessageA(
   DWORD last_error,
   bool stripTrailingLineFeed)
{
   CHAR errmsg[512];

   if (!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 
      0,
      last_error,
      0,
      errmsg, 
      511,
      NULL))
   {
      /* if we fail, call ourself to find out why and return that error */
      return (GetLastErrorMessageA(GetLastError()));  
   }
  
   if (stripTrailingLineFeed)
   {
      const size_t length = strlen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

wstring GetLastErrorMessageW(
   DWORD last_error,
   bool stripTrailingLineFeed)
{
   wchar_t errmsg[512];

   if (!FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, 
      0,
      last_error,
      0,
      errmsg, 
      511,
      NULL))
   {
      /* if we fail, call ourself to find out why and return that error */
      return (GetLastErrorMessageW(GetLastError()));  
   }
  
   if (stripTrailingLineFeed)
   {
      const size_t length = wcslen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

string HexToStringA(
   const BYTE *pBuffer, 
   size_t iBytes,
   bool upperCase)
{
   string result;
     
   for (size_t i = 0; i < iBytes; i++)
   {
      BYTE c ;

      BYTE b = static_cast<BYTE>(pBuffer[i] >> 4);
         
      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + (upperCase ? 'A' : 'a'));
      }

      result += (char)c;

      b = static_cast<BYTE>(pBuffer[i] & 0x0f);

      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + (upperCase ? 'A' : 'a'));
      }

      result += (char)c;
   }

   return result;
}

_tstring HexToString(
   const BYTE *pBuffer, 
   size_t iBytes)
{
   _tstring result;
     
   for (size_t i = 0; i < iBytes; i++)
   {
      BYTE c ;

      BYTE b = static_cast<BYTE>(pBuffer[i] >> 4);
         
      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + 'A');
      }

      result += (TCHAR)c;

      b = static_cast<BYTE>(pBuffer[i] & 0x0f);

      if (9 >= b)
      {
         c = static_cast<BYTE>(b + '0');
      }
      else
      {
         c = static_cast<BYTE>((b - 10) + 'A');
      }

      result += (TCHAR)c;
   }

   return result;
}

void StringToHex(
   const _tstring &ts, 
   BYTE *pBuffer, 
   size_t nBytes)
{
   const string s = CStringConverter::TtoA(ts);

   for (size_t i = 0; i < nBytes; i++)
   {
      const size_t stringOffset = i * 2;

      BYTE val = 0;

      const BYTE b = s[stringOffset];

      if (isdigit(b)) 
      {
         val = static_cast<BYTE>((b - '0') * 16); 
      }
      else 
      {
         val = static_cast<BYTE>(((toupper(b) - 'A') + 10) * 16); 
      }

      const BYTE b1 = s[stringOffset + 1];

      if (isdigit(b1)) 
      {
         val = static_cast<BYTE>(val + b1 - '0'); 
      }
      else 
      {
         val = static_cast<BYTE>(val + (toupper(b1) - 'A') + 10); 
      }

      pBuffer[i] = val;
   }
}

void CreateDirectory(
   const _tstring &directory)
{
   if (!::CreateDirectory(directory.c_str(), 0))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("CreateDirectory()"), lastError);
   }
}

void CreateDirectoryIfRequired(
   const _tstring &directory)
{
   if (!::CreateDirectory(directory.c_str(), 0))
   {
      const DWORD lastError = ::GetLastError();

      if (lastError != ERROR_ALREADY_EXISTS)
      {
         throw CWin32Exception(_T("CreateDirectory()"), lastError);
      }
   }
}
_tstring GetCurrentDirectory()
{
   DWORD size = ::GetCurrentDirectory(0, 0);

   _tstring result;

   result.resize(size);

   if (0 == ::GetCurrentDirectory(size, const_cast<TCHAR *>(result.c_str())))
   {
      throw CException(_T("GetCurrentDirectory()"), _T("Failed to get current directory"));
   }

   result.resize(size - 1);

   return result;
}

void SetCurrentDirectory(
   const _tstring &directory)
{
   if (!::SetCurrentDirectory(directory.c_str()))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SetCurrentDirectory()"), lastError);
   }
}


_tstring GetDateStamp()
{
   SYSTEMTIME systime;
   GetLocalTime(&systime);

   TCHAR buffer[7];

   _stprintf_s(buffer, _T("%02d%02d%02d"),
                     systime.wDay,
                     systime.wMonth,
                     systime.wYear % 100);

   return buffer;
}

_tstring GetTimeStamp()
{
   SYSTEMTIME systime;
   GetLocalTime(&systime);

   TCHAR buffer[14];

   _stprintf_s(buffer, _T("%02d:%02d:%02d.%04d"),
                     systime.wHour,
                     systime.wMinute,
                     systime.wSecond,
                     systime.wMilliseconds);

   return buffer;
}

string GetTimeStampA()
{
   SYSTEMTIME systime;
   GetLocalTime(&systime);

   char buffer[14];

   sprintf_s(buffer, "%02d:%02d:%02d.%04d",
                   systime.wHour,
                   systime.wMinute,
                   systime.wSecond,
                   systime.wMilliseconds);

   return buffer;
}

_tstring ToHex(
   const BYTE c)
{
   TCHAR hex[3];
   
   const int val = c;

   _stprintf_s(hex, _T("%02X"), val);

   return hex;
}

string ToHexA(
   const BYTE c)
{
   char hex[3];

   const int val = c;

   sprintf_s(hex, "%02X", val);

   return hex;
}

_tstring MakePrintable(
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength,
   bool useCR)
{
   static const _tstring linePrefix;

   return DumpData(linePrefix, pData, dataLength, lineLength, true, useCR, true, true);
}

_tstring DumpData(
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength,
   bool useCR)
{
   static const _tstring linePrefix;

   return DumpData(linePrefix, pData, dataLength, lineLength, false, useCR, true, true);
}

_tstring DumpData(
   const _tstring &linePrefix,
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength,
   bool useCR,
   bool linePrefixOnFirstLine,
   bool lineFeedOnLastLine)
{
   return DumpData(linePrefix, pData, dataLength, lineLength, false, useCR, linePrefixOnFirstLine, lineFeedOnLastLine);
}

string DumpDataA(
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength,
   bool useCR)
{
   static const string linePrefix;

   return DumpDataA(linePrefix, pData, dataLength, lineLength, false, useCR, true, true);
}

string DumpDataA(
   const string &linePrefix,
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength,
   bool useCR,
   bool linePrefixOnFirstLine,
   bool lineFeedOnLastLine)
{
   return DumpDataA(linePrefix, pData, dataLength, lineLength, false, useCR, linePrefixOnFirstLine, lineFeedOnLastLine);
}

_tstring GetComputerName()
{
   static bool gotName = false;

   static _tstring name = _T("UNAVAILABLE");

   if (!gotName)
   {
      TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1] ;
      DWORD computerNameLen = MAX_COMPUTERNAME_LENGTH ;

      if (::GetComputerName(computerName, &computerNameLen))
      {
         name = computerName;
      }

      gotName = true;
   }

   return name;
}

#if (_MSC_VER < 1300) && (JETBYTE_NO_PLATFORM_SDK == 1)
#pragma MESSAGE("GetModuleFileName(hProcess, hModule) not available with VC6 without an appropriate Platform SDK")
// Need psapi.h for GetModuleFileNameEx
#else

_tstring GetModuleFileName(
   HANDLE hProcess,
   HINSTANCE hModule)
{
   _tstring name = _T("UNAVAILABLE");

   GetModuleFileName(hProcess, hModule, name);

   return name;
}

bool GetModuleFileName(
   HANDLE hProcess,
   HINSTANCE hModule,
   _tstring &name)
{
   TCHAR moduleFileName[MAX_PATH + 1] ;
   DWORD moduleFileNameLen = MAX_PATH ;

   const bool ok = ToBool(::GetModuleFileNameEx(hProcess, hModule, moduleFileName, moduleFileNameLen));

   if (ok)
   {
      name = moduleFileName;
   }

   return ok;
}

#endif

_tstring GetModuleFileName(
   HINSTANCE hModule /* = 0 */)
{
   _tstring name = _T("UNAVAILABLE");

   TCHAR moduleFileName[MAX_PATH + 1] ;
   DWORD moduleFileNameLen = MAX_PATH ;

   if (::GetModuleFileName(hModule, moduleFileName, moduleFileNameLen))
   {
      name = moduleFileName;
   }

   return name;
}

_tstring GetModulePathName(
   HINSTANCE hModule /* = 0 */)
{
   _tstring path = GetModuleFileName(hModule);

   _tstring::size_type pos = path.find_last_of(_T("\\/:"));

   return path.substr(0, pos);
}

#if (_MSC_VER < 1300) && (JETBYTE_NO_PLATFORM_SDK == 1)
#pragma MESSAGE("GetSystemWindowsDirectory() not available with VC6 without an appropriate Platform SDK")
// Need psapi.h for GetSystemWindowsDirectory
#else

_tstring GetSystemWindowsDirectory()
{
   TCHAR buffer[MAX_PATH + 1];

   DWORD bufferLen = MAX_PATH;
   
   const UINT result = ::GetSystemWindowsDirectory(buffer, bufferLen);

   if (result == 0)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("GetSystemWindowsDirectory()"), lastError);
   }
   else if (result > bufferLen)
   {
      throw CException(_T("GetSystemWindowsDirectory()"), _T("System directory is more than: ") + ToString(bufferLen) + _T(" bytes long..."));
   }

   return buffer;
}

#endif

_tstring GetSystemDirectory()
{
//#pragma TODO("Vista x64 gives wrong result for GetSystemDirectoty()? C:\\Windows\\system rather than C:\\Windows\\System")

   TCHAR buffer[MAX_PATH + 1];

   DWORD bufferLen = MAX_PATH;
   
   const UINT result = ::GetSystemDirectory(buffer, bufferLen);

   if (result == 0)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("GetSystemDirectory()"), lastError);
   }
   else if (result > bufferLen)
   {
      throw CException(_T("GetSystemDirectory()"), _T("System directory is more than: ") + ToString(bufferLen) + _T(" bytes long..."));
   }

   return buffer;

//   return GetSystemWindowsDirectory() + _T("\\System");
}

_tstring GetFileNameFromPathName(
   const _tstring &pathName)
{
   _tstring fileName;

   const _tstring::size_type pos = pathName.find_last_of(_T("\\"));

   if (pos != _tstring::npos)
   {
      fileName = pathName.substr(pos + 1);
   }   

   return fileName;
}

_tstring StripFileNameFromPathName(
   const _tstring &pathName)
{
   _tstring strippedPathName;

   const _tstring::size_type pos = pathName.find_last_of(_T("\\"));

   if (pos != _tstring::npos)
   {
      strippedPathName = pathName.substr(0, pos);
   }   

   return strippedPathName;
}

void DeleteFile(
   const _tstring &fileName)
{
   if (!::DeleteFile(fileName.c_str()))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("DeleteFile() - \"") + fileName + _T("\""), lastError);
   }
}

bool Is64bitSystem()
{
#if defined(_WIN64)

   return true;  // 64-bit programs run only on Win64

#elif defined(_WIN32)

   bool is64bit = false;

   typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

   static LPFN_ISWOW64PROCESS s_fnIsWow64Process = (LPFN_ISWOW64PROCESS)::GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");

   // 32-bit programs run on both 32-bit and 64-bit Windows
   // so must sniff

   if (s_fnIsWow64Process)
   {
      BOOL f64 = FALSE;

      if (!s_fnIsWow64Process(::GetCurrentProcess(), &f64))
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("Is64bitSystem()"), lastError);
      }

      is64bit = ToBool(f64);
   }

   return is64bit;
#else

 return false; // Win64 does not support Win16

#endif
}

#pragma TODO("Move all x64 code into a new file")

bool IsWow64Process()
{
#if defined(_WIN64)

   return false;  // 64-bit programs run natively on Win64

#elif defined(_WIN32)

   bool is64bit = false;

   typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

   static LPFN_ISWOW64PROCESS s_fnIsWow64Process = (LPFN_ISWOW64PROCESS)::GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");

   // 32-bit programs run on both 32-bit and 64-bit Windows
   // so must sniff

   if (s_fnIsWow64Process)
   {
      BOOL f64 = FALSE;

      if (!s_fnIsWow64Process(::GetCurrentProcess(), &f64))
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("IsWow64Process()"), lastError);
      }

      is64bit = ToBool(f64);
   }

   return is64bit;
#else

 return false; // Win64 does not support Win16

#endif
}

bool Is64bitProcess()
{
#if defined(_WIN64)

   return true;  // 64-bit programs run natively on Win64

#else

   return false;

#endif
}

bool Is32bitProcess()
{
#if defined(_WIN32)

   return true ;  // Compiled as a 32-bit program

#else 

   return false;

#endif
}


#if _WIN32_WINNT >= 0x0501 || defined(WINBASE_DECLARE_GET_SYSTEM_WOW64_DIRECTORY)

_tstring GetSystemWow64Directory()
{
   TCHAR buffer[MAX_PATH + 1];

   DWORD bufferLen = MAX_PATH;

   typedef UINT (WINAPI *LPFN_GETSYSTEMWOW64DIRECTORY) (LPTSTR, UINT);

#ifdef UNICODE
   const LPCSTR pFunctionName = "GetSystemWow64DirectoryW";
#else
   const LPCSTR pFunctionName = "GetSystemWow64DirectoryA";
#endif

   LPFN_GETSYSTEMWOW64DIRECTORY s_fnGetSystemWow64Directory = (LPFN_GETSYSTEMWOW64DIRECTORY)::GetProcAddress(::GetModuleHandle(_T("kernel32")), pFunctionName);

   if (s_fnGetSystemWow64Directory)
   {
      const UINT result = (s_fnGetSystemWow64Directory)(buffer, bufferLen);

      if (result == 0)
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("GetSystemWow64Directory()"), lastError);
      }
      else if (result > bufferLen)
      {
         throw CException(_T("GetSystemWow64Directory()"), _T("System directory is more than: ") + ToString(bufferLen) + _T(" bytes long..."));
      }
   }
   else
   {
      throw CException(_T("GetSystemWow64Directory()"), _T("Functionality not available on this platform"));
   }

   return buffer;
}

#else

_tstring GetSystemWow64Directory()
{
   throw CException(_T("GetSystemWow64Directory()"), _T("Functionality not available on this platform"));

#if (_MSC_VER < 1300)   // VC6...
   return _T("");
#endif
}

#endif // _WIN32_WINNT >= 0x0501 || defined(WINBASE_DECLARE_GET_SYSTEM_WOW64_DIRECTORY)

_tstring GetUserName()
{
   _tstring name = _T("UNAVAILABLE");

   TCHAR userName[UNLEN + 1] ;
   DWORD userNameLen = UNLEN;

   if (::GetUserName(userName, &userNameLen))
   {
      name = userName;
   }

   return name;
}

_tstring StripSurroundingWhiteSpace(
   const _tstring &source)
{
   const TCHAR *pSrc = source.c_str();

   while (pSrc && _istspace(*pSrc))
   {
      ++pSrc;
   }

   _tstring result = pSrc;

   size_t i = result.length();
   
   pSrc = result.c_str() + i;
   
   --pSrc;

   while (i && _istspace(*pSrc))
   {
      --pSrc;
      --i;
   }
   
   return result.substr(0, i);
}

_tstring StripLeading(
   const _tstring &source, 
   const char toStrip)
{
   const TCHAR *pSrc = source.c_str();

   while (pSrc && *pSrc == toStrip)
   {
      ++pSrc;
   }

   return pSrc;
}

_tstring StripTrailing(
   const _tstring &source, 
   const char toStrip)
{
   size_t i = source.length();
   const _TCHAR *pSrc = source.c_str() + i;
   
   --pSrc;

   while (i && *pSrc == toStrip)
   {
      --pSrc;
      --i;
   }
   
   return source.substr(0, i);
}

#pragma comment(lib, "Version.lib")

_tstring GetFileVersion()
{
   _tstring version;

   const _tstring moduleFileName = GetModuleFileName(NULL);

   LPTSTR pModuleFileName = const_cast<LPTSTR>(moduleFileName.c_str());

   DWORD zero = 0;

   DWORD verSize = ::GetFileVersionInfoSize(pModuleFileName, &zero);

   if (verSize != 0)
   {
      TExpandableBuffer<BYTE> buffer(verSize);

      if (::GetFileVersionInfo(pModuleFileName, 0, verSize, buffer))
      {
         LPTSTR pVersion = 0;
         UINT verLen = 0;

         if (::VerQueryValue(buffer, 
              const_cast<LPTSTR>(_T("\\StringFileInfo\\080904b0\\ProductVersion")), 
              (void**)&pVersion, 
              &verLen))
         {
            version = pVersion;
         }
      }
   }

   return version;
}

_tstring ToUpper(
   const _tstring &data)
{
   _tstring dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<TCHAR>(toupper(dataOut[i]));
   }

   return dataOut;
}

string ToUpperA(
   const string &data)
{
   string dataOut = data;

   const size_t length = dataOut.length();

   for (size_t i = 0; i < length; ++i)
   {
      dataOut[i] = static_cast<char>(toupper(dataOut[i]));
   }

   return dataOut;
}

void CopyFile(
   const _tstring &filenameFrom,
   const _tstring &filenameTo,
   const bool failIfExists)
{
   if (!::CopyFile(filenameFrom.c_str(), filenameTo.c_str(), failIfExists))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("CopyFile"), lastError);
   }
}

void SaveStringAsFile(
   const _tstring &filename,
   const _tstring &data)
{
   SaveStringAsFileA(filename, CStringConverter::TtoA(data));
}

void SaveStringAsFileA(
   const _tstring &filename,
   const string &data)
{
   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_WRITE,
      0,
      0,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      0));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveStringAsFile() \"") + filename + _T("\""), lastError);
   }

   DWORD bytesWritten = 0;

   if (!::WriteFile(hFile, data.c_str(), GetStringLength<DWORD>(data), &bytesWritten, 0))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveStringAsFile() \"") + filename + _T("\""), lastError);
   }

   if (bytesWritten != data.length())
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveStringAsFile() \"") + filename + _T("\" - Failed to write all data"), lastError);
   }
}

wstring LoadFileAsUnicodeString(
   const _tstring &filename,
   bool *pFileWasUnicode)
{
   // Look for the Unicode stream indicator in the file for (UCS-2) format and these files start 0xFF 0xFE to indicate 
   // unicode little-endian 0xFE 0xFF indicates unicode big-endian but we don't support that right now. Anything else 
   // means ASCII, UTF-8 or MBCS and we assume ASCII here

   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      0,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      0));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsUnicodeString() \"") + filename + _T("\""), lastError);
   }

   BY_HANDLE_FILE_INFORMATION info;

   if (!::GetFileInformationByHandle(hFile, &info))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsUnicodeString() - GetFileInformationByHandle \"") + filename + _T("\""), lastError);
   }

   DWORD bytesRead = 0;

   DWORD totalBytesRead = 0;

   BYTE buffer[2];

   while (::ReadFile(hFile, buffer + totalBytesRead, sizeof(buffer) - totalBytesRead, &bytesRead, 0) && bytesRead > 0)
   {
      totalBytesRead += bytesRead;
   }

   if (totalBytesRead != 2)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsUnicodeString() - Initial 2 byte read failed \"") + filename + _T("\""), lastError);
   }

   bool fileIsUnicode = false;
   bool fileIsUTF8 = false;

   DWORD sizeOfBOM = 0;

   if (buffer[0] == 0xFF && buffer[1] == 0xFE)
   {
      sizeOfBOM = 2;
      fileIsUnicode = true;
   }
   else if (buffer[0] == 0xFE && buffer[1] == 0xFF)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsUnicodeString() - We don't handle UCS-2 big-endian yet. \"") + filename + _T("\""), lastError);
   }
   else if (buffer[0] == 0xEF && buffer[1] == 0xBB)
   {
      // UTF-8 ?
      // EF BB BF

      sizeOfBOM = 3;
      fileIsUnicode = true;
      fileIsUTF8 = true;

      if (0 == ::ReadFile(hFile, buffer, 1, &bytesRead, 0))
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("LoadFileAsUnicodeString() - Failed to read 3rd byte in UTF-8 BOM. \"") + filename + _T("\""), lastError);
      }

      totalBytesRead += bytesRead;

      if (buffer[0] != 0xBF)
      {
         throw CException(_T("LoadFileAsUnicodeString()"), _T("Unexpected 3rd byte in UTF-8 BOM. \"") + filename + _T("\""));
      }
   }
   else
   {
      // bare ascii file, we need those 2 bytes...

      if (INVALID_SET_FILE_POINTER == ::SetFilePointer(hFile, 0, 0, FILE_BEGIN))
      {
         const DWORD lastError = ::GetLastError();

         throw CWin32Exception(_T("LoadFileAsUnicodeString() - Failed to seek to start of file. \"") + filename + _T("\""), lastError);
      }
   }

   bytesRead = 0;

   totalBytesRead = 0;

   string fileAsString;

   if (info.nFileSizeHigh != 0 || info.nFileSizeLow > fileAsString.max_size())
   {
      throw CException(_T("LoadFileAsUnicodeString()"), _T("File \"") + filename + _T("\" too big"));
   }
   
   const DWORD bufferSize = info.nFileSizeLow - sizeOfBOM;

   fileAsString.resize(bufferSize);

   while (::ReadFile(hFile, (void*)(fileAsString.c_str() + totalBytesRead), bufferSize - totalBytesRead, &bytesRead, 0) && bytesRead > 0)
   {
      totalBytesRead += bytesRead;
   }

   if (pFileWasUnicode)
   {
      *pFileWasUnicode = fileIsUnicode;
   }

   if (fileIsUnicode)
   {
      if (fileIsUTF8)
      {
         return CStringConverter::UTF8toW(fileAsString);
      }
      else
      {
         return wstring(reinterpret_cast<const wchar_t*>(fileAsString.c_str()), bufferSize / sizeof(wchar_t));
      }
   }

   return CStringConverter::AtoW(fileAsString);
}

void SaveUnicodeStringAsFile(
   const _tstring &filename,
   const std::wstring &data,
   bool saveAsANSIifPossible)
{
   if (saveAsANSIifPossible && StringIsAllANSI(data))
   {
      SaveStringAsFile(filename, CStringConverter::WtoT(data));
   }
   
   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_WRITE,
      0,
      0,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      0));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveUnicodeStringAsFile() \"") + filename + _T("\""), lastError);
   }

   DWORD bytesWritten = 0;

   const BYTE header[] = { 0xFF, 0xFE };

   if (!::WriteFile(hFile, header, sizeof(header), &bytesWritten, 0))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveUnicodeStringAsFile() \"") + filename + _T("\""), lastError);
   }

   const DWORD dataToWrite = GetStringLength<DWORD>(data) * sizeof(wchar_t);

   if (!::WriteFile(hFile, data.c_str(), dataToWrite, &bytesWritten, 0))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveUnicodeStringAsFile() \"") + filename + _T("\""), lastError);
   }

   if (bytesWritten != dataToWrite)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("SaveUnicodeStringAsFile() \"") + filename + _T("\" - Failed to write all data"), lastError);
   }
}

_tstring LoadFileAsString(
   const _tstring &filename)
{
   return CStringConverter::AtoT(LoadFileAsStringA(filename));
}

string LoadFileAsStringA(
   const _tstring &filename)
{
   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      0,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      0));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsString() \"") + filename + _T("\""), lastError);
   }

   BY_HANDLE_FILE_INFORMATION info;

   if (!::GetFileInformationByHandle(hFile, &info))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("LoadFileAsString() - GetFileInformationByHandle \"") + filename + _T("\""), lastError);
   }

   string fileAsString;

   if (info.nFileSizeHigh != 0 || info.nFileSizeLow > fileAsString.max_size())
   {
      throw CException(_T("LoadFileAsString()"), _T("File \"") + filename + _T("\" too big"));
   }
   
   const DWORD bufferSize = info.nFileSizeLow;

   fileAsString.resize(bufferSize);

   DWORD bytesRead = 0;

   DWORD totalBytesRead = 0;

   while (::ReadFile(hFile, (void*)(fileAsString.c_str() + totalBytesRead), bufferSize - totalBytesRead, &bytesRead, 0) && bytesRead > 0)
   {
      totalBytesRead += bytesRead;
   }

   return fileAsString;
}

bool FileExists(
   const _tstring &filename)
{
   CSmartHandle hFile(::CreateFile(
      filename.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      0,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      0));

   return hFile.IsValid();
}

_tstring FindAndReplace(
   const _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString)
{
   _tstring result = phrase;

   InPlaceFindAndReplace(result, findString, replaceString);

   return result;
}

void InPlaceFindAndReplace(
   _tstring &phrase,
   const _tstring &findString,
   const _tstring &replaceString)
{
   _tstring::size_type pos = phrase.find(findString);

   while (pos != _tstring::npos)
   {
      phrase.replace(pos, findString.length(), replaceString);
   
      pos = phrase.find(findString, pos + 1);
   }
}

bool IsGoodReadPtr(
   void *pv, 
   ULONG cb)
{
   return IsGoodReadPtr(GetCurrentProcess(), pv, cb);
}

bool IsGoodReadPtr(
   HANDLE hProcess,
   void *pv, 
   ULONG cb)
{
   return IsGoodPtr(
      hProcess,
      pv, 
      cb, 
      PAGE_READONLY | 
      PAGE_READWRITE | 
      PAGE_WRITECOPY | 
      PAGE_EXECUTE_READ | 
      PAGE_EXECUTE_READWRITE | 
      PAGE_EXECUTE_WRITECOPY);
}

bool IsGoodWritePtr(
   void *pv, 
   ULONG cb)
{
   return IsGoodWritePtr(GetCurrentProcess(), pv, cb);
}

bool IsGoodWritePtr(
   HANDLE hProcess,
   void *pv, 
   ULONG cb)
{
   return IsGoodPtr(
      hProcess,
      pv, 
      cb, 
      PAGE_READWRITE | 
      PAGE_WRITECOPY |
      PAGE_EXECUTE_READWRITE | 
      PAGE_EXECUTE_WRITECOPY);
}

_tstring BoolAsString(
   const bool value)
{
   return value ? _T("True") : _T("False");
}

_tstring GetTempFileName(
   const _tstring &pathName,
   const _tstring &prefixString,
   const unsigned int unique)
{
   TCHAR buffer[MAX_PATH + 1];

   if (0 == ::GetTempFileName(pathName.c_str(), prefixString.c_str(), unique, buffer))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("GetTempFileName()"), lastError);
   }

   return buffer;
}

_tstring GetTempFileName(
   const _tstring &prefixString,
   const unsigned int unique)
{
   return GetTempFileName(GetTempPath(), prefixString, unique);
}

_tstring GetTempPath()
{
   const DWORD spaceRequired = ::GetTempPath(0, 0);

   _tstring directory;

   directory.resize(spaceRequired - 1);

   const DWORD spaceUsed = 1 + ::GetTempPath(spaceRequired, const_cast<TCHAR*>(directory.c_str()));

   if (spaceRequired != spaceUsed)
   {
      throw CException(_T("GetTempPath()"), _T("Failed to get temp path"));
   }

   return directory;
}

unsigned long GetLongFromString(
   const _tstring &numeric,
   const size_t startOffset,
   const size_t length)
{
   if (startOffset + length > numeric.length())
   {
      throw CException(_T("GetLongFromString()"), 
         _T("Invalid offset (") + ToString(startOffset) + _T(") or length (") + ToString(length) + _T(") string is only ") + ToString(numeric.length()) + _T(" long"));
   }

   return _ttol(numeric.substr(startOffset, length).c_str());
}

unsigned short GetShortFromString(
   const _tstring &numeric,
   const size_t startOffset,
   const size_t length)
{
   return static_cast<unsigned short>(GetLongFromString(numeric, startOffset, length));
}

void WriteResourceToFile(
   HANDLE hFile,
   const _tstring &resourceName,
   const _tstring &resourceType,
   HMODULE hModule)
{
   HRSRC hResource = ::FindResource(hModule, resourceName.c_str(), resourceType.c_str());

   if (!hResource)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("WriteResourceToFile() - FindResource"), lastError);
   }

   HGLOBAL hGlobal = ::LoadResource(hModule, hResource);

   if (!hGlobal)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("WriteResourceToFile() - LoadResource"), lastError);
   }

   void *pData = ::LockResource(hGlobal);

   if (!pData)
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("WriteResourceToFile() - LockResource"), lastError);
   }

   const DWORD bytes = ::SizeofResource(hModule, hResource);

   DWORD bytesWritten = 0;

   if (!::WriteFile(hFile, pData, bytes, &bytesWritten,0))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("WriteResourceToFile() - WriteFile"), lastError);
   }

   if (bytesWritten != bytes)
   {
      throw CWin32Exception(_T("WriteResourceToFile() - WriteFile"), _T("Wrote less bytes (") + ToString(bytesWritten) + _T("( than expected: ") + ToString(bytes));
   }
}

_tstring CreateGUIDAsString(
   const bool stripBrackets)
{
   GUID guid;

   HRESULT hr = ::CoCreateGuid(&guid);

   if (FAILED(hr))
   {
      throw CWin32Exception(_T("CreateGUIDAsString() - CoCreateGuid"), hr);
   }

   wchar_t *pString = 0;

   hr = ::StringFromIID(guid, &pString);

   if (FAILED(hr))
   {
      ::CoTaskMemFree(pString);

      throw CWin32Exception(_T("CreateGUIDAsString() - CoCreateGuid"), hr);
   }

   _tstring result = CStringConverter::WtoT(pString);

   if (stripBrackets)
   {
      result = result.substr(1, result.length() - 2);
   }

   ::CoTaskMemFree(pString);

   return result;
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static string DumpDataA(
   const string &linePrefix,
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength, 
   const bool makePrintable,
   const bool useCR,
   const bool linePrefixOnFirstLine,
   const bool lineFeedOnLastLine)
{
   const string lineSep = useCR ? "\r\n" : "\n";

   const size_t bytesPerLine = lineLength != 0 ? (lineLength - 1) / 3 : 0;

   string result;

   string hexDisplay;
   string display;

   size_t i = 0;

   bool firstLine = true;

   while (i < dataLength)
   {
      const BYTE c = pData[i++];

      if (!makePrintable)
      {
         hexDisplay += ToHexA(c) + " ";   
      }

      if (isprint(c))
      {
         display += (char)c;
      }
      else
      {
         display += '.';
      }

      if ((bytesPerLine && (i % bytesPerLine == 0 && i != 0)) || i == dataLength)
      {
         if (!makePrintable)
         {
            if (i == dataLength && (bytesPerLine && (i % bytesPerLine != 0)))
            {
               for (size_t pad = i % bytesPerLine; pad < bytesPerLine; pad++)
               {
                  hexDisplay += "   ";
               }
            }
         }

         const string prefix = (firstLine ? (linePrefixOnFirstLine ? linePrefix : "" ) : linePrefix);

         result += prefix + (!makePrintable ? hexDisplay + " - " : "") + display + (bytesPerLine ? lineSep : "");

         firstLine = false;

         hexDisplay = "";
         display = "";
      }
   }


   if (!lineFeedOnLastLine)
   {
      result = result.substr(0, result.length() - lineSep.length());
   }

   return result;
}

static bool StringIsAllANSI(
   const std::wstring &data)
{
   const BYTE *pData = reinterpret_cast<const BYTE *>(data.c_str());

   const size_t dataLength = data.length() * sizeof(wchar_t);

   bool allANSI = false;

   for (size_t i = 1; allANSI && i < dataLength; i += 2)
   {
      if (pData[i] != 0x00)
      {
         allANSI = !(i != 1 || pData[i] != 0xFE);
      }
   }

   return allANSI;
}

static _tstring DumpData(
   const _tstring &linePrefix,
   const BYTE * const pData, 
   size_t dataLength, 
   size_t lineLength, 
   const bool makePrintable,
   const bool useCR,
   const bool linePrefixOnFirstLine,
   const bool lineFeedOnLastLine)
{
   const _tstring lineSep = useCR ? _T("\r\n") : _T("\n");

   const size_t bytesPerLine = lineLength != 0 ? (lineLength - 1) / 3 : 0;

   _tstring result;

   _tstring hexDisplay;
   _tstring display;

   size_t i = 0;

   bool firstLine = true;

   while (i < dataLength)
   {
      const BYTE c = pData[i++];

      if (!makePrintable)
      {
         hexDisplay += ToHex(c) + _T(" ");   
      }

      if (isprint(c))
      {
         display += (TCHAR)c;
      }
      else
      {
         display += _T('.');
      }

      if ((bytesPerLine && (i % bytesPerLine == 0 && i != 0)) || i == dataLength)
      {
         if (!makePrintable)
         {
            if (i == dataLength && (bytesPerLine && (i % bytesPerLine != 0)))
            {
               for (size_t pad = i % bytesPerLine; pad < bytesPerLine; pad++)
               {
                  hexDisplay += _T("   ");
               }
            }
         }

         const _tstring prefix = (firstLine ? (linePrefixOnFirstLine ? linePrefix : _T("") ) : linePrefix);

         result += prefix + (!makePrintable ? hexDisplay + _T(" - ") : _T("")) + display + (bytesPerLine ? lineSep : _T(""));

         firstLine = false;

         hexDisplay = _T("");
         display = _T("");
      }
   }

   if (!lineFeedOnLastLine)
   {
      result = result.substr(0, result.length() - lineSep.length());
   }

   return result;
}

static bool IsGoodPtr(
   HANDLE hProcess,
   void *pv, 
   ULONG cb, 
   DWORD dwFlags)
{
   MEMORY_BASIC_INFORMATION meminfo;

   if (NULL == pv)
   {
     return false;
   }

   memset(&meminfo, 0x00, sizeof meminfo);
   
   const SIZE_T dwSize = VirtualQueryEx(hProcess, pv, &meminfo, sizeof meminfo);

   // If pv is a kernel-mode address then this may return zero for security reasons.
   // In that event it is certainly NOT a valid read pointer.

   if (0 == dwSize)
   {
     return false;
   }

   if (MEM_COMMIT != meminfo.State)
   {
     return false;
   }

   if (0 == (meminfo.Protect & dwFlags))
   {
     return false;
   }

   if (cb > meminfo.RegionSize)
   {
      return false;
   }

   if ((unsigned)((char *)pv - (char *)meminfo.BaseAddress) > (unsigned)(meminfo.RegionSize - cb))
   {
      return false;
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: Utils.cpp
///////////////////////////////////////////////////////////////////////////////
