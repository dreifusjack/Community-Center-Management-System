#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

/**
 * A wrapper class for C++ chrono library for DateTime functionality specfic to this management app.
 */
class DateTime
{
private:
  chrono::system_clock::time_point dateTime;
  string date;
  string time;

  tm parse_date_time(const string &date, const string &time) const
  {
    tm tm = {};
    stringstream ss(date + " " + time);
    ss >> get_time(&tm, "%m/%d/%Y %H:%M");
    return tm;
  }
  tm to_tm(const chrono::system_clock::time_point &tp) const
  {
    time_t time = chrono::system_clock::to_time_t(tp);
    tm tm = *localtime(&time);
    return tm;
  }

public:
  /**
   * A constructor for a DateTime object.
   *
   * @param date the date in MM/DD/YYYY format
   * @param time the time in HH:MM format
   */
  DateTime(const string &date, const string &time) : date(date), time(time)
  {
    tm tm = parse_date_time(date, time);
    dateTime = chrono::system_clock::from_time_t(mktime(&tm));
  }
  ~DateTime() = default;

  /**
   * Returns the date string of this DateTime.
   *
   * @return the date in MM/DD/YYYY format
   */
  string get_date_str() const { return date; }
  /**
   * Returns the time string of this DateTime.
   *
   * @return the time in HH:MM format
   */
  string get_time_str() const { return time; }

  // Util functions on DateTime
  /**
   * Returns the hour difference between two DateTime objects.
   *
   * @param other the other DateTime
   * @return the hours difference between the two DateTime objects
   */
  int hours_difference(const DateTime &other) const
  {
    auto duration = chrono::duration_cast<chrono::hours>(dateTime - other.dateTime);
    return static_cast<int>(duration.count());
  }
  /**
   * Returns the number of hours until the Facility closes (23:00).
   *
   * @return the number of hours until the Facility closes
   */
  int hours_until_facility_close() const
  {
    tm this_tm = to_tm(dateTime);
    return 23 - this_tm.tm_hour;
  }
  /**
   * Is this DateTime on the same day or after the other DateTime?
   *
   * @param other the other DateTime
   * @return is this DateTime on the same day or after the other DateTime
   */
  bool is_same_day_or_after(const DateTime &other) const
  {
    tm this_tm = to_tm(dateTime);
    tm other_tm = to_tm(other.dateTime);
    // Compare years, months, and days only
    if (this_tm.tm_year > other_tm.tm_year)
      return true;
    if (this_tm.tm_year == other_tm.tm_year && this_tm.tm_mon > other_tm.tm_mon)
      return true;
    if (this_tm.tm_year == other_tm.tm_year && this_tm.tm_mon == other_tm.tm_mon && this_tm.tm_mday >= other_tm.tm_mday)
      return true;
    return false;
  }

  // Overload comparison operators for DateTime
  bool operator==(const DateTime &other) const
  {
    return dateTime == other.dateTime;
  }
  bool operator!=(const DateTime &other) const
  {
    return !(*this == other);
  }
  bool operator<(const DateTime &other) const
  {
    return dateTime < other.dateTime;
  }
  bool operator<=(const DateTime &other) const
  {
    return dateTime <= other.dateTime;
  }
  bool operator>(const DateTime &other) const
  {
    return dateTime > other.dateTime;
  }
  bool operator>=(const DateTime &other) const
  {
    return dateTime >= other.dateTime;
  }
};
