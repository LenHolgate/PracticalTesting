#pragma once
#ifndef JETBYTE_TOOLS_WIN32_SYSTEM_TIME_INCLUDED__
#define JETBYTE_TOOLS_WIN32_SYSTEM_TIME_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: SystemTime.h
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

#include <wtypes.h>

#include "tstring.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CSystemTime
///////////////////////////////////////////////////////////////////////////////

/// A simple class that wraps, and initialises, and manipulates a SYSTEMTIME
/// structure.
/// \ingroup Win32

class CSystemTime : public SYSTEMTIME
{
   public:

      class TimeDifference;

      class DateDifference;

      /// Construct a zero initialised SYSTEMTIME.

      CSystemTime();

      /// Constructs a SYSTEMTIME with the specified date/time.

      explicit CSystemTime(
         const __int64 dateTime);

      /// Constructs a SYSTEMTIME from a FILETIME...

      explicit CSystemTime(
         const FILETIME &fileTime);

      /// Constructs a SYSTEMTIME with the specified SYSTEMTIME...

      explicit CSystemTime(
         const SYSTEMTIME &systemTime);

      /// Constructs a SYSTEMTIME with the specified date/time in "YYYYMMDD HHMMSS[MMM]" or
      /// "YYYYMMDD" or "HHMMSS[MMM]" or format (in all cases the millisecond time component
      /// is optional).

      explicit CSystemTime(
         const _tstring &yyyymmddhhmmss);

      /// Calls the operating system's GetSystemTime() with this object.

      void GetSystemTime();

      /// Calls the operating system's GetLocalTime() with this object.

      void GetLocalTime();

      /// Copies just the date portion of this object to the supplied
      /// SYSTEMTIME.

      void CopyDateTo(
         SYSTEMTIME &destination) const;

      /// Copies just the time portion of this object to the supplied
      /// SYSTEMTIME.

      void CopyTimeTo(
         SYSTEMTIME &destination) const;

      /// Attempts to parse a DDMMYYYY date into the current object,
      /// returns false on failure.

      bool TryParseDate(
         const _tstring &ddmmyyyy);

      /// Parses a DDMMYYYY date into the current object, throws an
      /// exception on failure.

      void ParseDate(
         const _tstring &ddmmyyyy);

      /// Attempts to parse an HHMMSS[MMM] time into the current object,
      /// returns false on failure (the millisecond time component
      /// is optional).

      bool TryParseTime(
         const _tstring &hhmmssmmm);

      /// Parses an HHMMSS[MMM] time into the current object, throws an
      /// exception on failure (the millisecond time component
      /// is optional).

      void ParseTime(
         const _tstring &hhmmssmmm);

      void SetAsSystemTimeFromLocalTime(
         const SYSTEMTIME &localtime);

      void SetAsLocalTimeFromSystemTime(
         const SYSTEMTIME &systemtime);

      void SetFromTimeT32(
         const __time32_t timet);

      void SetFromTimeT64(
         const __time64_t timet);

      __time32_t GetAsTimeT32() const;

      __time64_t GetAsTimeT64() const;

      #ifdef _USE_32BIT_TIME_T
      inline time_t GetAsTimeT() const { return GetAsTimeT32(); }
      #else
      inline time_t GetAsTimeT() const { return GetAsTimeT64(); }
      #endif

      //void ParseFromNTP(
      //   const __int64 ntp);

      //__int64 GetAsNTP() const;

      /// Returns the date in YYYYMMDD format.

      _tstring GetAsYYYYMMDD() const;

      /// Returns the time HHMMSS format (and strips any milliseconds that may be present).

      _tstring GetAsHHMMSS() const;

      /// Returns the time HHMMSSMMM format (includes milliseconds).

      _tstring GetAsHHMMSSMMM() const;

      /// Returns the time YYYY-MM-DD HH:MM:SS.MMM format (SQL Server Date/Time format).

      _tstring GetAsDatabaseDateTimeStamp() const;

      std::string GetAsDatabaseDateTimeStampA() const;

      /// Returns the date/time in HTML format (Tue, 15 Nov 1994 08:12:31 GMT)

      _tstring GetAsHTTPDate() const;

      std::string GetAsHTTPDateA() const;

      bool ContainsDate() const;

      bool IsValid() const;

      /// Sets the object's date portion to the base date for a SYSTEMTIME, 1st Jan 1601.

      void SetToBaseDate();

      /// Gets the SYSTEMTIME as an Int64 value representing the number of 100-nanosecond
      /// intervals since January 1, 1601 (UTC).

      __int64 GetAsInt64() const;

      /// Gets the date portion of this SYSTEMTIME as an Int64 value representing
      /// the number of 100-nanosecond intervals since January 1, 1601 (UTC).

      __int64 GetDateAsInt64() const;

      /// Gets the time portion of this SYSTEMTIME as an Int64 value representing
      /// the number of 100-nanosecond intervals since the day began.

      __int64 GetTimeAsInt64() const;

      /// Gets the SYSTEMTIME as an Int64 value representing the number of milliseconds
      /// since January 1, 1601 (UTC).

      __int64 GetAsMilliseconds() const;

      /// Gets the date portion of this SYSTEMTIME as an Int64 value representing
      /// the number of milliseconds since January 1, 1601 (UTC).

      __int64 GetDateAsMilliseconds() const;

      /// Gets the time portion of this SYSTEMTIME as an Int64 value representing
      /// the number of milliseconds since the day began.

      __int64 GetTimeAsMilliseconds() const;

      /// Adds the specified number of months to the date.

      void AddDays(
         const int days);

      /// Adds the specified number of months to the date.

      void AddMonths(
         const int months);

      /// Adds the specified number of years to the date.

      void AddYears(
         const int years);

      /// Returns the number of days in the month.

      WORD GetDaysInMonth() const;

      /// Returns true if the year is a leap year.

      bool IsLeapYear() const;

      /// Returns the number of complete years between the two dates.

      int GetYearsDifferent(
         const SYSTEMTIME &secondDate) const;

      /// Returns the number of complete months between the two dates.

      int GetMonthsDifferent(
         const SYSTEMTIME &secondDate) const;

      /// Returns the number of days between the two dates.

      int GetDaysDifferent(
         const SYSTEMTIME &secondDate) const;

      /// Returns the number of days in the specified month.

      static WORD GetDaysInMonth(
         const WORD year,
         const WORD month);

      /// Returns the number of days in the month of the supplied date.

      static WORD GetDaysInMonth(
         const SYSTEMTIME &date);

      /// Returns true if the supplied year is a leap year.

      static bool IsLeapYear(
         const WORD year);

      /// Returns true if the supplied date is in a leap year.

      static bool IsLeapYear(
         const SYSTEMTIME &date);

      /// Returns the number of complete years between the two dates.

      static int GetYearsDifferent(
         const SYSTEMTIME &firstDate,
         const SYSTEMTIME &secondDate);

      /// Returns the number of complete months between the two dates.

      static int GetMonthsDifferent(
         const SYSTEMTIME &firstDate,
         const SYSTEMTIME &secondDate);

      /// Returns the number of days between the two dates.

      static int GetDaysDifferent(
         const SYSTEMTIME &firstDate,
         const SYSTEMTIME &secondDate);

      /// Copies just the date portion of the source to the destination.

      static void CopyDateTo(
         const SYSTEMTIME &source,
         SYSTEMTIME &destination);

      /// Copies just the time portion of the source to the destination.

      static void CopyTimeTo(
         const SYSTEMTIME &source,
         SYSTEMTIME &destination);

      /// Gets the SYSTEMTIME as an Int64 value representing the number of 100-nanosecond
      /// intervals since January 1, 1601 (UTC).

      static __int64 GetAsInt64(
         const SYSTEMTIME &source);

      /// Gets the date portion of this SYSTEMTIME as an Int64 value representing
      /// the number of 100-nanosecond intervals since January 1, 1601 (UTC).

      static __int64 GetDateAsInt64(
         const SYSTEMTIME &source);

      /// Gets the time portion of this SYSTEMTIME as an Int64 value representing
      /// the number of 100-nanosecond intervals since the day began.

      static __int64 GetTimeAsInt64(
         const SYSTEMTIME &source);

      /// Gets the SYSTEMTIME as an Int64 value representing the number of milliseconds
      /// since January 1, 1601 (UTC).

      static __int64 GetAsMilliseconds(
         const SYSTEMTIME &source);

      /// Gets the date portion of this SYSTEMTIME as an Int64 value representing
      /// the number of milliseconds since January 1, 1601 (UTC).

      static __int64 GetDateAsMilliseconds(
         const SYSTEMTIME &source);

      /// Gets the time portion of this SYSTEMTIME as an Int64 value representing
      /// the number of milliseconds since the day began.

      static __int64 GetTimeAsMilliseconds(
         const SYSTEMTIME &source);

      /// The number of Int64 intervals in a Millisecond, you can use this
      /// to convert between the Int64 values returned and Milliseconds.

      static const __int64 IntervalsInAMillisecond;
};

///////////////////////////////////////////////////////////////////////////////
// CSystemTime::TimeDifference
///////////////////////////////////////////////////////////////////////////////

/// A simple class that helps to manipluate the difference in time (only)
/// between two SYSTEMTIMEs.
/// \ingroup Win32

class CSystemTime::TimeDifference
{
   public :

      /// Create a zero time difference.

      TimeDifference();

      /// Take the time portion of now from the time portion of base and
      /// store as our difference.

      TimeDifference(
         const SYSTEMTIME &now,
         const SYSTEMTIME &base);

      /// Add our difference to the supplied SYSTEMTIME.

      void Apply(
         SYSTEMTIME &time) const;

      /// Returns our difference.

      __int64 GetDifference() const;

   private :

      __int64 m_difference;
};

///////////////////////////////////////////////////////////////////////////////
// CSystemTime::DateDifference
///////////////////////////////////////////////////////////////////////////////

/// A simple class that helps to manipluate the difference in date (only)
/// between two SYSTEMTIMEs.
/// \ingroup Win32

class CSystemTime::DateDifference
{
   public :

      /// Create a zero date difference

      DateDifference();

      /// Take the date portion of now from the date portion of base and
      /// store as our difference.

      DateDifference(
         const SYSTEMTIME &now,
         const SYSTEMTIME &base);

      /// Add our difference to the supplied SYSTEMTIME.

      void Apply(
         SYSTEMTIME &date) const;

      /// Returns our difference.

      __int64 GetDifference() const;

   private :

      __int64 m_difference;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_SYSTEM_TIME_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: SystemTime.h
///////////////////////////////////////////////////////////////////////////////

