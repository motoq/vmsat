/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdexcept>
#include <string>

#include <utl_greg_date.h>

GregDate::GregDate(int year, int month, int day)
{
  this->set(year, month, day);
}

GregDate::GregDate(std::string year, std::string month, std::string day)
{
  this->set(std::stoi(year), std::stoi(month), std::stoi(day));
}

void GregDate::set(int year, int month, int day)
{
    // Days in a standard (non leap year) month.
  static constexpr int nDaysInMonth[] = {  0,
                                          31, 28, 31, 30,
                                          31, 30, 31, 31,
                                          30, 31, 30, 31 };

    // Check year and month ranges
  if (year < GREGYEAR  ||  year > MAXYEAR) {
    std::string bad_year = "Invalid Year: " + std::to_string(year);
    throw std::invalid_argument(bad_year);
  } else if (month < JAN  ||  month > DEC) {
    std::string bad_month = "Invalid Month: " + std::to_string(month);
    throw std::invalid_argument(bad_month);
  } else {
    int offset = 0;
    if (month == FEB  &&  isLeapYear(year)) {
      offset = 1;
    }

    int dim = nDaysInMonth[month] + offset;
    if (day < 1  ||  day > dim) {
      std::string bad_day = "Invalid Day: " + std::to_string(day);
      throw std::invalid_argument(bad_day);
    }
  }

  yr = year;
  mnth = month;
  dy = day;
}

/**
 * Determins if the given year is a leapyear.  The method used to determine
 * this is as follows.  If the year is divisible by 4 but not divisible by
 * 100 then the year is a leap year.  If the year is divisible by 4, and
 * divisible by 100, then it is a leap year only if it is also divisible by 
 * 400.  This method should be valid from the year 1582 forward since that is
 * when Pope Gregory XIII decided that this would be the method used to 
 * determie leap years as opposed to the previous method that only required 
 * the year be divisible by 4.  If the year is before 1582 then the year will
 * be considered a leap year if it is divisible by 4.  Not sure how far back 
 * that will work, but hey, nothing important happended back then.
 */
bool GregDate::isLeapYear(int year)
{
  bool leap = false;

  if (year < GREGYEAR) {
    if (year%4 == 0) {
      leap = true;
    }
  } else {
    if (year%4 == 0) {
      if (year%100 == 0) {
        if (year%400 == 0) {
            leap = true;
        }
      } else {
        leap = true;
      }
    }
  }
  return leap;
}
