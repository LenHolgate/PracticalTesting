///////////////////////////////////////////////////////////////////////////////
// File: DateTime.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2024 JetByte Limited.
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

#include "DateTime.h"
#include "IProvideCurrentDateTime.h"

#include "Tchar.h"

#include "Exception.h"
#include "ToString.h"
#include "StringUtils.h"
#include "Printf.h"
#include "CheckedMemcpy.h"

#pragma hdrstop

#include "JetByteTools/Admin/FunctionName.h"

#include <ctime>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::string;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// CDateTime
///////////////////////////////////////////////////////////////////////////////

DateTimeComponents::DateTimeComponents(
   const IProvideCurrentDateTime &timeProvider)
   :  year(0),
      month(0),
      day(0),
      hour(0),
      minute(0),
      second(0),
      milliseconds(0),
      dayOfWeek(0),
      dayOfYear(0)
{
   timeProvider.GetCurrentDateTime(*this);
}

DateTimeComponents::DateTimeComponents()
   :  year(0),
      month(0),
      day(0),
      hour(0),
      minute(0),
      second(0),
      milliseconds(0),
      dayOfWeek(0),
      dayOfYear(0)
{
}

DateTimeComponents::DateTimeComponents(
   const int year,
   const int month,
   const int day,
   const int hour,
   const int minute,
   const int second)
   :  year(year),
      month(month),
      day(day),
      hour(hour),
      minute(minute),
      second(second),
      milliseconds(0),
      dayOfWeek(0),
      dayOfYear(0)
{
}

DateTimeComponents::DateTimeComponents(
   const int year,
   const int month,
   const int day,
   const int hour,
   const int minute,
   const int second,
   const int milliseconds)
   :  year(year),
      month(month),
      day(day),
      hour(hour),
      minute(minute),
      second(second),
      milliseconds(milliseconds),
      dayOfWeek(0),
      dayOfYear(0)
{
}

///////////////////////////////////////////////////////////////////////////////
// CDateTime
///////////////////////////////////////////////////////////////////////////////

static void ValidateInRangeForDisplay(
   const DateTimeComponents &dateTime,
   const _tstring &location);

static int64_t SecondsSinceEpocToTm(
   int64_t secondsSinceEpoc,
   tm &t)
{
   #if defined(JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM)
   const __time64_t s = secondsSinceEpoc;

   const auto err = gmtime_s(&t, &s);
   #else
   const time_t s = static_cast<time_t>(secondsSinceEpoc);

   const int err = !gmtime_r(&s, &t) ? errno : 0;
   #endif

   return err;
}

static int64_t UpdateTmAndReturnSecondsSinceEpoc(
   tm &t)
{
   #if defined(JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM)
   const auto result = _mkgmtime(&t);
   #else
   const auto result = timegm(&t);
   #endif

   return result;
}

static bool ValidateAndUpdateTm(
   tm &t)
{
   return -1 != UpdateTmAndReturnSecondsSinceEpoc(t);
}

void CDateTime::GetUTCDateTime(
   DateTimeComponents &currentTime)
{
   #if defined(JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM)

   _timespec64 timespec{};

   if (TIME_UTC != _timespec64_get(&timespec, TIME_UTC))
   {
      throw CException(_T("CDateTime::GetUTCDateTime()"), _T("_timespec64_get failed"));
   }
   #else
   timespec timespec{};

   if (TIME_UTC != timespec_get(&timespec, TIME_UTC))
   {
      throw CException(_T("CDateTime::GetUTCDateTime()"), _T("_timespec64_get failed"));
   }
   #endif

   tm tm{};

   #if defined(JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM)
   _gmtime64_s(&tm, &timespec.tv_sec);
   #else
   gmtime_r(&timespec.tv_sec, &tm);
   #endif

   //time.nanoseconds = timespec.tv_nsec;
   currentTime.milliseconds = timespec.tv_nsec / 1000000;

   currentTime.second = tm.tm_sec;
   currentTime.minute = tm.tm_min;
   currentTime.hour = tm.tm_hour;

   currentTime.day = tm.tm_mday;
   currentTime.month = tm.tm_mon + 1;
   currentTime.year = tm.tm_year + 1900;

   currentTime.dayOfWeek = tm.tm_wday;
   currentTime.dayOfYear = tm.tm_yday;
}

const CDateTime CDateTime::Epoc(0);

CDateTime::CDateTime(
   const InitialSetting setting)
{
   JETBYTE_WARNING_SUPPRESS_MISSING_SWITCH_DEFAULT_LABEL
   switch (setting)
   {
      case InitialSetting::Epoc :
         *this = CDateTime::Epoc;
         break;
      case InitialSetting::Zero :
         // Do nothing, we default to zero values
         break;
      case InitialSetting::Invalid :
         // Do nothing, we default to zero values
         // and zero values are invalid
         break;
      case InitialSetting::Now :
         GetUTCDateTime(*this);
         break;
   }
   JETBYTE_WARNING_SUPPRESS_POP
}

CDateTime::CDateTime(
   const IProvideCurrentDateTime &timeProvider)
{
   timeProvider.GetCurrentDateTime(*this);
}

CDateTime::CDateTime(
   const DateTimeComponents &components)
{
   tm t{};

   if (components.year)
   {
      t.tm_year = components.year - 1900;
   }
   else
   {
      t.tm_year = 70;
   }

   if (components.month)
   {
      t.tm_mon = components.month - 1;
   }

   if (components.day)
   {
      t.tm_mday = components.day;
   }
   else
   {
      t.tm_mday = 1;
   }

   t.tm_hour = components.hour;
   t.tm_min = components.minute;
   t.tm_sec = components.second;

   if (components.milliseconds > 999)
   {
      t.tm_sec += (components.milliseconds / 1000);
   }

   milliseconds = (components.milliseconds % 1000);

   tm original;

   CheckedByteCopy(&original, sizeof original, &t, sizeof t);

   if (!ValidateAndUpdateTm(t))
   {
      throw CException(_T("CDateTime::CDateTime()"), _T("Invalid components"));
   }

   // since tm can include extra elements over and above those
   // required by the standard we need to explicitly compare the
   // pieces that we're interested in...

   if (t.tm_mday != original.tm_mday ||
       t.tm_mon != original.tm_mon ||
       t.tm_year != original.tm_year ||
       t.tm_hour != original.tm_hour ||
       t.tm_min != original.tm_min ||
       t.tm_sec != original.tm_sec)
   {
      // the usual reason for failure here is invalid input of the month
      // days and these not being valid for the month in question.
      // mktime() will adjust (so 31st of Sep will become 1st Oct) but
      // we don't want that flexibility here...

      throw CException(_T("CDateTime::CDateTime()"), _T("Invalid components"));
   }

   year = t.tm_year + 1900;
   month = t.tm_mon + 1;
   day = t.tm_mday;

   dayOfWeek = t.tm_wday;
   dayOfYear = t.tm_yday;

   hour = t.tm_hour;
   minute = t.tm_min;
   second = t.tm_sec;
}

CDateTime::CDateTime(
   const _tstring &yyyymmddhhmmssmmm)
{
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

         ParseDate(ddmmyyyy);

         if (setTime)
         {
            const _tstring hhmmssmmm = yyyymmddhhmmssmmm.substr(11, 2) + yyyymmddhhmmssmmm.substr(14, 2) + yyyymmddhhmmssmmm.substr(17, 2) + ((length == 23) ? yyyymmddhhmmssmmm.substr(20) : _T(""));

            ParseTime(hhmmssmmm);
         }
      }
      else
      {
         // YYYYMMDD HHMMSS[MMM] format
         const _tstring ddmmyyyy = yyyymmddhhmmssmmm.substr(6, 2) + yyyymmddhhmmssmmm.substr(4, 2) + yyyymmddhhmmssmmm.substr(0, 4);

         ParseDate(ddmmyyyy);

         if (setTime)
         {
            ParseTime(yyyymmddhhmmssmmm.substr(9));
         }
      }
   }
   else if (setTime)
   {
      ParseTime(yyyymmddhhmmssmmm);

      CopyTimeTo(*this);
   }

   if (!setDate && !setTime && !yyyymmddhhmmssmmm.empty())
   {
      throw CException(_T("CDateTime::CDateTime()"), _T("Invalid format: \"") + yyyymmddhhmmssmmm + _T("\" expected: \"YYYYMMDD HHMMSS[MMM] or YYYY-MM-DD HH:MM:SS[.MMM]\""));
   }
}

CDateTime::CDateTime(
   const MillisecondsSinceEpoc millisecondsSinceEpoc)
{
   const uint64 secondsSinceEpoc = millisecondsSinceEpoc / 1000;

   milliseconds = static_cast<int>(millisecondsSinceEpoc - (secondsSinceEpoc * 1000));

   tm t{};

   auto err = SecondsSinceEpocToTm(secondsSinceEpoc, t);

   if (err != 0)
   {
      throw CException(_T("DateTime::DateTime()"), _T("gmtime_s failed - ") + ToString(err));
   }

   year = t.tm_year + 1900;
   month = t.tm_mon + 1;
   day = t.tm_mday;

   dayOfWeek = t.tm_wday;
   dayOfYear = t.tm_yday;

   hour = t.tm_hour;
   minute = t.tm_min;
   second = t.tm_sec;
}

bool CDateTime::IsValid() const
{
   if (year >= 1900 && month > 0 && month < 13 &&
       day > 0 && day < 32 && hour < 25 && minute < 61 &&
       second < 62)
   {
      return true;
   }

   return false;
}

bool CDateTime::TryParseDate(
   const _tstring &ddmmyyyy)
{
   bool ok = false;

   if (ddmmyyyy.length() == 8)
   {
      ok = IsAllDigits(ddmmyyyy);

      if (ok)
      {
         tm t{};

         t.tm_mday = GetShortFromString(ddmmyyyy, 0, 2);
         t.tm_mon = GetShortFromString(ddmmyyyy, 2, 2);
         t.tm_year = GetShortFromString(ddmmyyyy, 4, 4);

         if (t.tm_mday < 1 || t.tm_mday > 31)
         {
            return false;
         }

         if (t.tm_mon < 1 || t.tm_mon > 12)
         {
            return false;
         }

         t.tm_mon -= 1;

         if (t.tm_year < 1900)
         {
            return false;
         }

         t.tm_year -= 1900;      // set base year

         tm original{};

         CheckedByteCopy(&original, sizeof original, &t, sizeof t);

         if (!ValidateAndUpdateTm(t))
         {
            return false;
         }

         // since tm can include extra elements over and above those
         // required by the standard we need to explicitly compare the
         // pieces that we're interested in...

         if (t.tm_mday != original.tm_mday ||
             t.tm_mon != original.tm_mon ||
             t.tm_year != original.tm_year)
         {
            // the usual reason for failure here is invalid input of the month
            // days and these not being valid for the month in question.
            // mktime() will adjust (so 31st of Sep will become 1st Oct) but
            // we don't want that flexibility here...

            return false;
         }

         year = t.tm_year + 1900;
         month = t.tm_mon + 1;
         day = t.tm_mday;

         dayOfWeek = t.tm_wday;
         dayOfYear = t.tm_yday;
      }
   }

   return ok;
}

void CDateTime::ParseDate(
   const _tstring &ddmmyyyy)
{
   if (!TryParseDate(ddmmyyyy))
   {
      throw CException(_T("CDateTime::ParseDate()"), _T("Invalid date format or invalid date: \"") + ddmmyyyy + _T("\" expected DDMMYYYY"));
   }
}

bool CDateTime::TryParseTime(
   const _tstring &hhmmssmmm)
{
   bool ok = false;

   const size_t length = hhmmssmmm.length();

   if (length >= 6 && length <= 9)
   {
      ok = IsAllDigits(hhmmssmmm);

      if (ok)
      {
         tm t{};

         t.tm_hour = GetShortFromString(hhmmssmmm, 0, 2);
         t.tm_min = GetShortFromString(hhmmssmmm, 2, 2);
         t.tm_sec = GetShortFromString(hhmmssmmm, 4, 2);

         if (t.tm_hour < 0 || t.tm_hour > 23)
         {
            return false;
         }

         if (t.tm_min < 0 || t.tm_min > 59)
         {
            return false;
         }

         if (t.tm_sec < 0 || t.tm_sec > 60)     // to allow for leap second
         {
            return false;
         }

         const int ms = (length > 6) ? GetShortFromString(hhmmssmmm, 6, 3) : 0;
 
         t.tm_mday = 1;          // set to epoc since they can't be zero
         t.tm_year = 70;         // set to epoc since they can't be zero

         tm original{};

         CheckedByteCopy(&original, sizeof original, &t, sizeof t);

         if (!ValidateAndUpdateTm(t))
         {
            return false;
         }

         // since tm can include extra elements over and above those
         // required by the standard we need to explicitly compare the
         // pieces that we're interested in...

         if (t.tm_hour != original.tm_hour ||
             t.tm_min != original.tm_min ||
             t.tm_sec != original.tm_sec)
         {
            // pretty much the only reason for failure here is if we 
            // specified a leap second and there shouldn't be one...

            return false;
         }

         hour = t.tm_hour;
         minute = t.tm_min;
         second = t.tm_sec;
         milliseconds = ms;
      }
   }

   return ok;
}

void CDateTime::ParseTime(
   const _tstring &hhmmssmmm)
{
   if (!TryParseTime(hhmmssmmm))
   {
      throw CException(_T("CDateTime::ParseTime()"), _T("Invalid time format or invalid time: \"") + hhmmssmmm + _T("\" expected HHMMSS[mmm]"));
   }
}

void CDateTime::CopyTo(
   DateTimeComponents &rhs) const
{
   CopyDateTo(rhs);
   CopyTimeTo(rhs);
}

void CDateTime::CopyDateTo(
   DateTimeComponents &rhs) const
{
   rhs.year = year;
   rhs.month = month;
   rhs.day = day;

   rhs.dayOfWeek = dayOfWeek;
   rhs.dayOfYear = dayOfYear;
}

void CDateTime::CopyTimeTo(
   DateTimeComponents &rhs) const
{
   rhs.milliseconds = milliseconds;
   rhs.second = second;
   rhs.minute = minute;
   rhs.hour = hour;
}

CDateTime::MillisecondsSinceEpoc CDateTime::GetAsMillisecondsSinceEpoc() const
{
   return (static_cast<MillisecondsSinceEpoc>(GetAsSecondsSinceEpoc()) * 1000) + milliseconds;
}

CDateTime::SecondsSinceEpoc CDateTime::GetAsSecondsSinceEpoc() const
{
   tm t{};

   t.tm_year = year - 1900;
   t.tm_mon = month - 1;
   t.tm_mday = day;

   t.tm_hour = hour;
   t.tm_min = minute;
   t.tm_sec = second;

   const auto result = UpdateTmAndReturnSecondsSinceEpoc(t);

   if (result == -1)
   {
      throw CException(_T("DateTime::GetAsSecondsSinceEpoc()"), _T("Date/time cannot be represented as seconds since epoc"));
   }

   return result;
}

_tstring CDateTime::GetAsYYYYMMDD() const
{
   ValidateInRangeForDisplay(*this, JETBYTE_FUNCTION_NAME);

   static constexpr size_t s_bufferSize = 9;

   TCHAR buffer[s_bufferSize];

   (void)_stprintf_s(buffer, s_bufferSize, _T("%04u%02u%02u"),
      static_cast<unsigned int>(year),
      static_cast<unsigned int>(month),
      static_cast<unsigned int>(day));

   return buffer;
}

_tstring CDateTime::GetAsHHMMSS() const
{
   ValidateInRangeForDisplay(*this, JETBYTE_FUNCTION_NAME);

   static constexpr size_t s_bufferSize = 7;

   TCHAR buffer[s_bufferSize];

   (void)_stprintf_s(buffer, s_bufferSize, _T("%02u%02u%02u"),
      static_cast<unsigned int>(hour),
      static_cast<unsigned int>(minute),
      static_cast<unsigned int>(second));

   return buffer;
}

_tstring CDateTime::GetAsHHMMSSMMM() const
{
   ValidateInRangeForDisplay(*this, JETBYTE_FUNCTION_NAME);

   static constexpr size_t s_bufferSize = 10;

   TCHAR buffer[s_bufferSize];

   (void)_stprintf_s(buffer, s_bufferSize, _T("%02u%02u%02u%03u"),
      static_cast<unsigned int>(hour),
      static_cast<unsigned int>(minute),
      static_cast<unsigned int>(second),
      static_cast<unsigned int>(milliseconds));

   return buffer;
}

_tstring CDateTime::GetAsDatabaseDateTimeStamp() const
{
   ValidateInRangeForDisplay(*this, JETBYTE_FUNCTION_NAME);

   static constexpr size_t s_bufferSize = 24;

   TCHAR buffer[s_bufferSize];

   (void)_stprintf_s(buffer, s_bufferSize, _T("%04u-%02u-%02u %02u:%02u:%02u.%03u"),
      static_cast<unsigned int>(year),
      static_cast<unsigned int>(month),
      static_cast<unsigned int>(day),
      static_cast<unsigned int>(hour),
      static_cast<unsigned int>(minute),
      static_cast<unsigned int>(second),
      static_cast<unsigned int>(milliseconds));

   return buffer;
}

string CDateTime::GetAsDatabaseDateTimeStampA() const
{
   ValidateInRangeForDisplay(*this, JETBYTE_FUNCTION_NAME);

   static constexpr size_t s_bufferSize = 24;

   char buffer[s_bufferSize];

   (void)sprintf_s(buffer, s_bufferSize, "%04u-%02u-%02u %02u:%02u:%02u.%03u",
      static_cast<unsigned int>(year),
      static_cast<unsigned int>(month),
      static_cast<unsigned int>(day),
      static_cast<unsigned int>(hour),
      static_cast<unsigned int>(minute),
      static_cast<unsigned int>(second),
      static_cast<unsigned int>(milliseconds));

   return buffer;
}

static constexpr __int64 s_millisecondsInADay = 1000 * 60 * 60 * 24;

void CDateTime::AddDays(
   const int days)
{
   const auto ms = GetAsMillisecondsSinceEpoc() + (days * s_millisecondsInADay);

   *this = CDateTime(ms);
}

void CDateTime::AddMonths(
   const int months)
{
   const int newMonths = month + months - 1;

   month = (((newMonths + 12) % 12) + 1);

   // This is rather horrible, I'm sure there's an easier way...

   int newYears = (newMonths - 1) / 11;

   if (newMonths < 0 && newYears >= 0)
   {
      newYears = -1;
   }

   AddYears(newYears);
}

void CDateTime::AddYears(
   const int years)
{
   year += years;

   // force us through a full conversion to update the week day and year day

   const auto ms = GetAsMillisecondsSinceEpoc();

   *this = CDateTime(ms);
}

static void ValidateInRangeForDisplay(
   const DateTimeComponents &dateTime,
   const _tstring &location)
{
   if (dateTime.year > 9999)
   {
      throw CException(
         location,
         _T("Year is out of range: ") + ToString(dateTime.year) + _T(", max is 9999"));
   }

   if (dateTime.month > 99)
   {
      throw CException(
         location,
         _T("Month is out of range: ") + ToString(dateTime.month) + _T(", max is 99 (!)"));
   }

   if (dateTime.day > 99)
   {
      throw CException(
         location,
         _T("Day is out of range: ") + ToString(dateTime.day) + _T(", max is 99 (!)"));
   }

   if (dateTime.hour > 99)
   {
      throw CException(
         location,
         _T("Hour is out of range: ") + ToString(dateTime.hour) + _T(", max is 99 (!)"));
   }

   if (dateTime.minute > 99)
   {
      throw CException(
         location,
         _T("Minute is out of range: ") + ToString(dateTime.minute) + _T(", max is 99 (!)"));
   }

   if (dateTime.second > 99)
   {
      throw CException(
         location,
         _T("Minute is out of range: ") + ToString(dateTime.second) + _T(", max is 99 (!)"));
   }

   if (dateTime.milliseconds > 999)
   {
      throw CException(
         location,
         _T("Milliseconds is out of range: ") + ToString(dateTime.milliseconds) + _T(", max is 999"));
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: DateTime.cpp
///////////////////////////////////////////////////////////////////////////////
