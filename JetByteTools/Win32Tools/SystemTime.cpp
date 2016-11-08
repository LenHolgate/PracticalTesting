///////////////////////////////////////////////////////////////////////////////
// File: SystemTime.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "SystemTime.h"
#include "Exception.h"
#include "Win32Exception.h"
#include "Utils.h"

#pragma hdrstop

#include "JetByteTools\Admin\FunctionName.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::string;

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

/// The number of 100-nanosecond intervals in a millisecond.

const __int64 CSystemTime::IntervalsInAMillisecond = 10000;

static const __int64 s_millisecondsInADay = 1000 * 60 * 60 * 24;

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static int GetDaysThisYearFrom(
   const SYSTEMTIME &date1,
   const SYSTEMTIME &date2);

static int GetDaysThisYearTo(
   const SYSTEMTIME &date);

static const TCHAR *GetDay(
   const WORD day);

static const TCHAR *GetMonth(
   const WORD month);

static const char *GetDayA(
   const WORD day);

static const char *GetMonthA(
   const WORD month);

static void ValidateInRangeForDisplay(
   const SYSTEMTIME &date,
   const _tstring &location);

///////////////////////////////////////////////////////////////////////////////
// CSystemTime
///////////////////////////////////////////////////////////////////////////////

CSystemTime::CSystemTime()
{
   ::ZeroMemory(this, sizeof(SYSTEMTIME));
}

CSystemTime::CSystemTime(
   const FILETIME &fileTime)
{
   ::ZeroMemory(this, sizeof(SYSTEMTIME));

   SetLastError(ERROR_SUCCESS);

   if (0 != ::FileTimeToSystemTime(&fileTime, this))
   {
      const DWORD lastError = ::GetLastError();

      if (lastError != ERROR_SUCCESS)
      {
         throw CWin32Exception(_T("CSystemTime::CSystemTime()"), lastError);
      }
   }
}

CSystemTime::CSystemTime(
   const __int64 dateTime)
{
   if (dateTime > 0 && dateTime < IntervalsInAMillisecond)
   {
      throw CWin32Exception(
         _T("CSystemTime::CSystemTime()"),
         _T("Value is too small: ") + ToString(dateTime) + _T(", less than 1ms (") + ToString(IntervalsInAMillisecond) + _T(")"));
   }

   ULARGE_INTEGER largeInteger;

   largeInteger.QuadPart = dateTime; //lint !e737 (loss of sign in promotion from 'const long long' to 'ULONGLONG')

   FILETIME fileTime;

   fileTime.dwLowDateTime = largeInteger.LowPart;
   fileTime.dwHighDateTime = largeInteger.HighPart;

   SetLastError(ERROR_SUCCESS);

   if (0 != ::FileTimeToSystemTime(&fileTime, this))
   {
      const DWORD lastError = ::GetLastError();

      if (lastError != ERROR_SUCCESS)
      {
         throw CWin32Exception(_T("CSystemTime::CSystemTime()"), lastError);
      }
      else
      {
         // Seeing failures for 129460032000000000
         // which gives 2011-03-31 00:00:00.000

         // Something strange about this date that means it doesn't have a 0:0:0 time?

         //throw CException(
         //   _T("CSystemTime::CSystemTime("),
         //   _T("Unexpected failure in FileTimeToSystemTime() for ") + ToString(dateTime) +
         //   _T(" resulting in: ") + GetAsDatabaseDateTimeStamp());
      }
   }
}

CSystemTime::CSystemTime(
   const SYSTEMTIME &systemTime)
{
   ::memcpy(this, &systemTime, sizeof(SYSTEMTIME));
}

CSystemTime::CSystemTime(
   const _tstring &yyyymmddhhmmssmmm)
{
   ::ZeroMemory(this, sizeof(SYSTEMTIME));

   bool setDate = false;

   bool setTime = false;

   const size_t length = yyyymmddhhmmssmmm.length();

   if (length == 15 ||      // YYYYMMDD HHMMSS
       length == 18 ||      // YYYYMMDD HHMMSSMMM
       length == 19 ||      // YYYY-MM-DD HH:MM:SS
       length == 23)        // YYYY-MM-DD HH:MM:SS.MMM
   {
      setDate = true;
      setTime = true;
   }

   if (length == 8)        // YYYYMMDD
   {
      setDate = true;
   }

   if (length == 6 ||      // HHMMSS
       length == 9)        // HHMMSSMMM
   {
      setTime = true;
   }

   if (setDate)
   {
      if (length == 19 || length == 23)
      {
         // YYYY-MM-DD HH:MM:SS[.MMM] format
         const _tstring ddmmyyyy = yyyymmddhhmmssmmm.substr(8, 2) + yyyymmddhhmmssmmm.substr(5, 2) + yyyymmddhhmmssmmm.substr(0, 4);

         CSystemTime date;

         date.ParseDate(ddmmyyyy);

         date.CopyDateTo(*this);

         if (setTime)
         {
            const _tstring hhmmssmmm = yyyymmddhhmmssmmm.substr(11, 2) + yyyymmddhhmmssmmm.substr(14, 2) + yyyymmddhhmmssmmm.substr(17, 2) + ((length == 23) ? yyyymmddhhmmssmmm.substr(20) : _T(""));

            CSystemTime time;

            time.ParseTime(hhmmssmmm);

            time.CopyTimeTo(*this);
         }
      }
      else
      {
         // YYYYMMDD HHMMSS[MMM] format
         const _tstring ddmmyyyy = yyyymmddhhmmssmmm.substr(6, 2) + yyyymmddhhmmssmmm.substr(4, 2) + yyyymmddhhmmssmmm.substr(0, 4);

         CSystemTime date;

         date.ParseDate(ddmmyyyy);

         date.CopyDateTo(*this);

         if (setTime)
         {
            CSystemTime time;

            time.ParseTime(yyyymmddhhmmssmmm.substr(9));

            time.CopyTimeTo(*this);
         }
      }
   }
   else if (setTime)
   {
      CSystemTime time;

      time.ParseTime(yyyymmddhhmmssmmm);

      time.CopyTimeTo(*this);
   }

   if (!setDate && !setTime && yyyymmddhhmmssmmm.length() != 0)
   {
      throw CException(_T("CSystemTime::CSystemTime()"), _T("Invalid format: \"") + yyyymmddhhmmssmmm + _T("\" expected: \"YYYYMMDD HHMMSS[MMM] or YYYY-MM-DD HH:MM:SS[.MMM]\""));
   }
}

void CSystemTime::GetSystemTime()
{
   ::GetSystemTime(this);
}

void CSystemTime::GetLocalTime()
{
   ::GetLocalTime(this);
}

void CSystemTime::CopyDateTo(
   const SYSTEMTIME &source,
   SYSTEMTIME &destination)
{
   destination.wYear = source.wYear;
   destination.wMonth = source.wMonth;
   destination.wDay = source.wDay;
   destination.wDayOfWeek = source.wDayOfWeek;
}

void CSystemTime::CopyDateTo(
   SYSTEMTIME &destination) const
{
   CopyDateTo(*this, destination);
}

void CSystemTime::CopyTimeTo(
   const SYSTEMTIME &source,
   SYSTEMTIME &destination)
{
   destination.wHour = source.wHour;
   destination.wMinute = source.wMinute;
   destination.wSecond = source.wSecond;
   destination.wMilliseconds = source.wMilliseconds;
}

void CSystemTime::CopyTimeTo(
   SYSTEMTIME &destination) const
{
   CopyTimeTo(*this, destination);
}

bool CSystemTime::TryParseDate(
   const _tstring &ddmmyyyy)
{
   bool ok = false;

   if (ddmmyyyy.length() == 8)
   {
      ok = IsAllDigits(ddmmyyyy);

      if (ok)
      {
         CSystemTime result;

         result.wDay = GetShortFromString(ddmmyyyy, 0, 2);
         result.wMonth = GetShortFromString(ddmmyyyy, 2, 2);
         result.wYear = GetShortFromString(ddmmyyyy, 4, 4);

         FILETIME temp;

         ok = ToBool(::SystemTimeToFileTime(&result, &temp));

         if (ok)
         {
            ok = ToBool(::FileTimeToSystemTime(&temp, &result));
         }

         if (ok)
         {
            result.CopyDateTo(*this);
         }
      }
   }

   return ok;
}

void CSystemTime::ParseDate(
   const _tstring &ddmmyyyy)
{
   if (!TryParseDate(ddmmyyyy))
   {
      throw CException(_T("CSystemTime::ParseDate()"), _T("Invalid date format or invalid date: \"") + ddmmyyyy + _T("\" expected DDMMYYYY"));
   }
}

bool CSystemTime::TryParseTime(
   const _tstring &hhmmssmmm)
{
   bool ok = false;

   const size_t length = hhmmssmmm.length();

   if (length >= 6 && length <= 9)
   {
      ok = IsAllDigits(hhmmssmmm);

      if (ok)
      {
         CSystemTime result;

         // Date must be set to something

         result.SetToBaseDate();

         result.wHour = GetShortFromString(hhmmssmmm, 0, 2);
         result.wMinute = GetShortFromString(hhmmssmmm, 2, 2);
         result.wSecond = GetShortFromString(hhmmssmmm, 4, 2);

         if (length > 6)
         {
            result.wMilliseconds = GetShortFromString(hhmmssmmm, 6, 3);
         }

         FILETIME temp;

         ok = ToBool(::SystemTimeToFileTime(&result, &temp));

         if (ok)
         {
            ok = ToBool(::FileTimeToSystemTime(&temp, &result));
         }

         if (ok)
         {
            result.CopyTimeTo(*this);
         }
      }
   }

   return ok;
}

void CSystemTime::ParseTime(
   const _tstring &hhmmssmmm)
{
   if (!TryParseTime(hhmmssmmm))
   {
      throw CException(_T("CSystemTime::ParseTime()"), _T("Invalid time format or invalid time: \"") + hhmmssmmm + _T("\" expected HHMMSS[mmm]"));
   }
}

_tstring CSystemTime::GetAsYYYYMMDD() const
{
   ValidateInRangeForDisplay(*this, JETBYTE_FUNCTION_NAME);

   TCHAR buffer[9];

   (void)_stprintf_s(buffer, _T("%04u%02u%02u"),
      static_cast<unsigned int>(wYear),
      static_cast<unsigned int>(wMonth),
      static_cast<unsigned int>(wDay));

   return buffer;
}

_tstring CSystemTime::GetAsHHMMSS() const
{
   ValidateInRangeForDisplay(*this, JETBYTE_FUNCTION_NAME);

   TCHAR buffer[7];

   (void)_stprintf_s(buffer, _T("%02u%02u%02u"),
      static_cast<unsigned int>(wHour),
      static_cast<unsigned int>(wMinute),
      static_cast<unsigned int>(wSecond));

   return buffer;
}

_tstring CSystemTime::GetAsHHMMSSMMM() const
{
   ValidateInRangeForDisplay(*this, JETBYTE_FUNCTION_NAME);

   TCHAR buffer[10];

   (void)_stprintf_s(buffer, _T("%02u%02u%02u%03u"),
      static_cast<unsigned int>(wHour),
      static_cast<unsigned int>(wMinute),
      static_cast<unsigned int>(wSecond),
      static_cast<unsigned int>(wMilliseconds));

   return buffer;
}

_tstring CSystemTime::GetAsDatabaseDateTimeStamp() const
{
   ValidateInRangeForDisplay(*this, JETBYTE_FUNCTION_NAME);

   TCHAR buffer[24];

   (void)_stprintf_s(buffer, _T("%04u-%02u-%02u %02u:%02u:%02u.%03u"),
      static_cast<unsigned int>(wYear),
      static_cast<unsigned int>(wMonth),
      static_cast<unsigned int>(wDay),
      static_cast<unsigned int>(wHour),
      static_cast<unsigned int>(wMinute),
      static_cast<unsigned int>(wSecond),
      static_cast<unsigned int>(wMilliseconds));

   return buffer;
}

string CSystemTime::GetAsDatabaseDateTimeStampA() const
{
   ValidateInRangeForDisplay(*this, JETBYTE_FUNCTION_NAME);

   char buffer[24];

   (void)sprintf_s(buffer, sizeof(buffer), "%04u-%02u-%02u %02u:%02u:%02u.%03u",
      static_cast<unsigned int>(wYear),
      static_cast<unsigned int>(wMonth),
      static_cast<unsigned int>(wDay),
      static_cast<unsigned int>(wHour),
      static_cast<unsigned int>(wMinute),
      static_cast<unsigned int>(wSecond),
      static_cast<unsigned int>(wMilliseconds));

   return buffer;
}

/// Returns the date/time in HTML format (Tue, 15 Nov 1994 08:12:31 GMT)

_tstring CSystemTime::GetAsHTTPDate() const
{
   ValidateInRangeForDisplay(*this, JETBYTE_FUNCTION_NAME);

   TCHAR buffer[30];

   (void)_stprintf_s(buffer, _T("%3s, %02u %3s %04u %02u:%02u:%02u GMT"),
      GetDay(wDayOfWeek),
      static_cast<unsigned int>(wDay),
      GetMonth(wMonth),
      static_cast<unsigned int>(wYear),
      static_cast<unsigned int>(wHour),
      static_cast<unsigned int>(wMinute),
      static_cast<unsigned int>(wSecond));

   return buffer;
}

string CSystemTime::GetAsHTTPDateA() const
{
   ValidateInRangeForDisplay(*this, JETBYTE_FUNCTION_NAME);

   char buffer[30];

   (void)sprintf_s(buffer, "%3s, %02u %3s %04u %02u:%02u:%02u GMT",
      GetDayA(wDayOfWeek),
      static_cast<unsigned int>(wDay),
      GetMonthA(wMonth),
      static_cast<unsigned int>(wYear),
      static_cast<unsigned int>(wHour),
      static_cast<unsigned int>(wMinute),
      static_cast<unsigned int>(wSecond));

   return buffer;
}

bool CSystemTime::IsValid() const
{
   bool isValid = false;

   FILETIME fileTime;

   if (::SystemTimeToFileTime(this, &fileTime))
   {
      ULARGE_INTEGER largeInteger;

      largeInteger.LowPart = fileTime.dwLowDateTime;
      largeInteger.HighPart = fileTime.dwHighDateTime;

      isValid = largeInteger.QuadPart != 0;
   }

   return isValid;
}

bool CSystemTime::ContainsDate() const
{
   bool containsDate = true;

   if (wYear == 0 && wMonth == 0 && wDay == 0)
   {
      containsDate = false;
   }

   return containsDate;
}

void CSystemTime::SetToBaseDate()
{
   wYear = 1601;
   wMonth = 1;
   wDay = 1;
}

__int64 CSystemTime::GetAsInt64() const
{
   return GetAsInt64(*this);
}

__int64 CSystemTime::GetDateAsInt64() const
{
   return GetDateAsInt64(*this);
}

__int64 CSystemTime::GetTimeAsInt64() const
{
   return GetTimeAsInt64(*this);
}

__int64 CSystemTime::GetAsMilliseconds() const
{
   return GetAsMilliseconds(*this);
}

__int64 CSystemTime::GetDateAsMilliseconds() const
{
   return GetDateAsMilliseconds(*this);
}

__int64 CSystemTime::GetTimeAsMilliseconds() const
{
   return GetTimeAsMilliseconds(*this);
}

void CSystemTime::AddDays(
   const int days)
{
   __int64 daysInIntervals = days * s_millisecondsInADay * IntervalsInAMillisecond;


   *this = CSystemTime(GetAsInt64() + daysInIntervals);
}

void CSystemTime::AddMonths(
   const int months)
{
   const int newMonths = wMonth + months - 1;

   wMonth = static_cast<WORD>(((newMonths + 12) % 12) + 1); //lint !e571 (Suspicious cast)

   // This is rather horrible, I'm sure there's an easier way...

   int newYears = (newMonths - 1) / 11;

   if (newMonths < 0 && newYears >= 0)
   {
      newYears = -1;
   }

   AddYears(newYears);
}

void CSystemTime::AddYears(
   const int years)
{
   wYear = static_cast<WORD>(wYear + years);
}

WORD CSystemTime::GetDaysInMonth() const
{
   return GetDaysInMonth(*this);
}

bool CSystemTime::IsLeapYear() const
{
   return IsLeapYear(wYear);
}

int CSystemTime::GetYearsDifferent(
   const SYSTEMTIME &secondDate) const
{
   return GetYearsDifferent(*this, secondDate);
}

int CSystemTime::GetMonthsDifferent(
   const SYSTEMTIME &secondDate) const
{
   return GetMonthsDifferent(*this, secondDate);
}

int CSystemTime::GetDaysDifferent(
   const SYSTEMTIME &secondDate) const
{
   return GetDaysDifferent(*this, secondDate);
}

__int64 CSystemTime::GetAsInt64(
   const SYSTEMTIME &source)
{
   FILETIME fileTime;

   if (!::SystemTimeToFileTime(&source, &fileTime))
   {
      throw CWin32Exception(_T("CSystemTime::GetAsInt64()"), ::GetLastError());
   }

   ULARGE_INTEGER largeInteger;

   largeInteger.LowPart = fileTime.dwLowDateTime;
   largeInteger.HighPart = fileTime.dwHighDateTime;

   return static_cast<__int64>(largeInteger.QuadPart);
}

__int64 CSystemTime::GetDateAsInt64(
   const SYSTEMTIME &source)
{
   CSystemTime dateOnly;

   CopyDateTo(source, dateOnly);

   return dateOnly.GetAsInt64();
}

__int64 CSystemTime::GetTimeAsInt64(
   const SYSTEMTIME &source)
{
   CSystemTime timeOnly;

   timeOnly.SetToBaseDate();

   CopyTimeTo(source, timeOnly);

   return timeOnly.GetAsInt64();
}

__int64 CSystemTime::GetAsMilliseconds(
   const SYSTEMTIME &source)
{
   return GetAsInt64(source) / IntervalsInAMillisecond;
}

__int64 CSystemTime::GetDateAsMilliseconds(
   const SYSTEMTIME &source)
{
   return GetDateAsInt64(source) / IntervalsInAMillisecond;
}

__int64 CSystemTime::GetTimeAsMilliseconds(
   const SYSTEMTIME &source)
{
   return GetTimeAsInt64(source) / IntervalsInAMillisecond;
}

int CSystemTime::GetYearsDifferent(
   const SYSTEMTIME &firstDate,
   const SYSTEMTIME &secondDate)
{
   int difference = secondDate.wYear - firstDate.wYear;

   if (difference != 0)
   {
      const int monthDifference = secondDate.wMonth - firstDate.wMonth;

      const int dayDifference = secondDate.wDay - firstDate.wDay;

      if (difference < 0)
      {
         if (monthDifference > 0)
         {
            difference++;
         }
         else if (dayDifference > 0)
         {
            difference++;
         }
      }
      else if (difference > 0)
      {
         if (monthDifference < 0)
         {
            difference--;
         }
         else if (dayDifference < 0)
         {
            difference--;
         }
      }
   }

   return difference;
}

int CSystemTime::GetMonthsDifferent(
   const SYSTEMTIME &firstDate,
   const SYSTEMTIME &secondDate)
{
   int yearDifference = secondDate.wYear - firstDate.wYear;

   int monthDifference = secondDate.wMonth - firstDate.wMonth;

   if (yearDifference != 0)
   {
      yearDifference = GetYearsDifferent(firstDate, secondDate);

      if (monthDifference < 0)
      {
         monthDifference += 12;
      }
      else if (monthDifference > 0)
      {
         monthDifference -= 12;
      }
   }

   if (monthDifference != 0)
   {
      const int dayDifference = secondDate.wDay - firstDate.wDay;

      if (monthDifference < 0)
      {
         if (dayDifference > 0)
         {
            monthDifference++;
         }
      }
      else if (monthDifference > 0)
      {
         if (dayDifference)
         {
            monthDifference--;
         }
      }
   }

   const int difference = (yearDifference * 12) + monthDifference;

   return difference;
}

int CSystemTime::GetDaysDifferent(
   const SYSTEMTIME &firstDate,
   const SYSTEMTIME &secondDate)
{
   const __int64 first = CSystemTime(firstDate).GetDateAsInt64();

   const __int64 second = CSystemTime(secondDate).GetDateAsInt64();

   if (first == second)
   {
      return 0;
   }

   const int multiplier = first < second ? 1 : -1;

   const SYSTEMTIME &date1 = first < second ? firstDate : secondDate;

   const SYSTEMTIME &date2 = first < second ? secondDate : firstDate;

   const bool sameYear = (date1.wYear == date2.wYear);

   int difference = GetDaysThisYearFrom(date1, date2);

   if (!sameYear)
   {
      for (WORD year = date1.wYear + 1; year < date2.wYear; ++year)
      {
         difference += 365;

         if (IsLeapYear(year))
         {
            difference++;
         }
      }

      difference += GetDaysThisYearTo(date2);
   }

   return difference * multiplier;
}

WORD CSystemTime::GetDaysInMonth(
   const SYSTEMTIME &date)
{
   return GetDaysInMonth(date.wYear, date.wMonth);
}

WORD CSystemTime::GetDaysInMonth(
   const WORD year,
   const WORD month)
{
   static const WORD daysInTheMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

   const bool isLeap = (month == 2 && CSystemTime::IsLeapYear(year));

   return daysInTheMonth[month - 1] + (isLeap ? 1 : 0);
}

bool CSystemTime::IsLeapYear(
   const SYSTEMTIME &date)
{
   return IsLeapYear(date.wYear);
}

bool CSystemTime::IsLeapYear(
   const WORD year)
{
   // From http://support.microsoft.com/kb/214019

   if (year % 4 == 0)
   {
      if (year % 100 != 0)
      {
         return true;
      }

      if (year % 400 == 0)
      {
         return true;
      }
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////
// CSystemTime::TimeDifference
///////////////////////////////////////////////////////////////////////////////

CSystemTime::TimeDifference::TimeDifference()
   :  m_difference(0)
{

}

CSystemTime::TimeDifference::TimeDifference(
   const SYSTEMTIME &now,
   const SYSTEMTIME &base)
   :  m_difference(CSystemTime::GetTimeAsInt64(base) - CSystemTime::GetTimeAsInt64(now))
{

}

void CSystemTime::TimeDifference::Apply(
   SYSTEMTIME &time) const
{
   time = CSystemTime(CSystemTime::GetAsInt64(time) + m_difference);
}

__int64 CSystemTime::TimeDifference::GetDifference() const
{
   return m_difference;
}

///////////////////////////////////////////////////////////////////////////////
// CSystemTime::DateDifference
///////////////////////////////////////////////////////////////////////////////

CSystemTime::DateDifference::DateDifference()
   :  m_difference(0)
{

}

CSystemTime::DateDifference::DateDifference(
   const SYSTEMTIME &now,
   const SYSTEMTIME &base)
   :  m_difference(CSystemTime::GetDateAsInt64(base) - CSystemTime::GetDateAsInt64(now))
{

}

void CSystemTime::DateDifference::Apply(
   SYSTEMTIME &date) const
{
   CSystemTime difference(CSystemTime::GetAsInt64(date) + m_difference);

   difference.CopyDateTo(date);
}

__int64 CSystemTime::DateDifference::GetDifference() const
{
   return m_difference;
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static int GetDaysThisYearFrom(
   const SYSTEMTIME &date1,
   const SYSTEMTIME &date2)
{
   const bool sameYear = (date1.wYear == date2.wYear);

   if (sameYear)
   {
      const bool sameMonth = (date1.wMonth == date2.wMonth);

      if (sameMonth)
      {
         return date2.wDay - date1.wDay;
      }
   }

   const WORD year = date1.wYear;

   WORD month = date1.wMonth;

   const WORD endMonth = sameYear ? date2.wMonth - 1 : 12;

   int days = 0;

   do
   {
      days += CSystemTime::GetDaysInMonth(year, month);
   }
   while (month++ != endMonth);

   days -= date1.wDay;

   if (sameYear)
   {
      days += date2.wDay;
   }

   return days;
}

static int GetDaysThisYearTo(
   const SYSTEMTIME &date)
{
   const WORD year = date.wYear;

   WORD month = 1;

   int days = 0;

   while (month != date.wMonth)
   {
      days += CSystemTime::GetDaysInMonth(year, month++);
   }

   days += date.wDay;

   return days;
}

static const TCHAR *GetDay(
   const WORD day)
{
   switch (day)
   {
      case 0 :
         return _T("Sun");
      case 1 :
         return _T("Mon");
      case 2 :
         return _T("Tue");
      case 3 :
         return _T("Wed");
      case 4 :
         return _T("Thu");
      case 5 :
         return _T("Fri");
      case 6 :
         return _T("Sat");
      default : 
         return _T(" ? ");
   }
}

static const TCHAR *GetMonth(
   const WORD month)
{
   switch (month)
   {
      case 1 :
         return _T("Jan");
      case 2 :
         return _T("Feb");
      case 3 :
         return _T("Mar");
      case 4 :
         return _T("Apr");
      case 5 :
         return _T("May");
      case 6 :
         return _T("Jun");
      case 7 :
         return _T("Jul");
      case 8 :
         return _T("Aug");
      case 9 :
         return _T("Sep");
      case 10 :
         return _T("Oct");
      case 11 :
         return _T("Nov");
      case 12 :
         return _T("Dec");
      default : 
         return _T(" ? ");
   }
}

static const char *GetDayA(
   const WORD day)
{
   switch (day)
   {
      case 0 :
         return "Sun";
      case 1 :
         return "Mon";
      case 2 :
         return "Tue";
      case 3 :
         return "Wed";
      case 4 :
         return "Thu";
      case 5 :
         return "Fri";
      case 6 :
         return "Sat";
      default : 
         return " ? ";
   }
}

static const char *GetMonthA(
   const WORD month)
{
   switch (month)
   {
      case 1 :
         return "Jan";
      case 2 :
         return "Feb";
      case 3 :
         return "Mar";
      case 4 :
         return "Apr";
      case 5 :
         return "May";
      case 6 :
         return "Jun";
      case 7 :
         return "Jul";
      case 8 :
         return "Aug";
      case 9 :
         return "Sep";
      case 10 :
         return "Oct";
      case 11 :
         return "Nov";
      case 12 :
         return "Dec";
      default : 
         return " ? ";
   }
}

static void ValidateInRangeForDisplay(
   const SYSTEMTIME &date,
   const _tstring &location)
{
   if (date.wYear > 9999)
   {
      throw CException(
         location,
         _T("Year is out of range: ") + ToString(date.wYear) + _T(", max is 9999"));
   }

   if (date.wMonth > 99)
   {
      throw CException(
         location,
         _T("Month is out of range: ") + ToString(date.wMonth) + _T(", max is 99 (!)"));
   }
   
   if (date.wDay > 99)
   {
      throw CException(
         location,
         _T("Day is out of range: ") + ToString(date.wDay) + _T(", max is 99 (!)"));
   }

   if (date.wHour > 99)
   {
      throw CException(
         location,
         _T("Hour is out of range: ") + ToString(date.wHour) + _T(", max is 99 (!)"));
   }

   if (date.wMinute > 99)
   {
      throw CException(
         location,
         _T("Minute is out of range: ") + ToString(date.wMinute) + _T(", max is 99 (!)"));
   }

   if (date.wSecond > 99)
   {
      throw CException(
         location,
         _T("Minute is out of range: ") + ToString(date.wSecond) + _T(", max is 99 (!)"));
   }

   if (date.wMilliseconds > 999)
   {
      throw CException(
         location,
         _T("Milliseconds is out of range: ") + ToString(date.wSecond) + _T(", max is 999"));
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: SystemTime.cpp
///////////////////////////////////////////////////////////////////////////////
