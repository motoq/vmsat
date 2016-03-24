/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdexcept>
#include <string>

#include <utl_time_of_day.h>

TimeOfDay::TimeOfDay(int hours, int minutes, double seconds)
{
  this->set(hours, minutes, seconds);
}

TimeOfDay::TimeOfDay(std::string hours, std::string minutes,
                                        std::string seconds)
{
  this->set(std::stoi(hours), std::stoi(minutes), std::stod(seconds));
}

void TimeOfDay::set(int hours, int minutes, double seconds)
{
  if (hours < 0  ||  hours > MAXHOUR) {
    std::string bad_hour = "Invalid Hour: " + std::to_string(hours);
    throw std::invalid_argument(bad_hour);
  } else if (minutes < 0  ||  minutes > MAXMINUTE) {
    std::string bad_min = "Invalid Minutes: " + std::to_string(minutes);
    throw std::invalid_argument(bad_min);
  } else if (seconds < 0.0  ||  seconds > MAXSECOND) {
    std::string bad_sec = "Invalid Seconds: " + std::to_string(seconds);
    throw std::invalid_argument(bad_sec);
  }
  hr = hours;
  mn = minutes;
  sec = seconds;
}
