/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UTL_TIME_OF_DAY_H
#define UTL_TIME_OF_DAY_H

#include <string>

/**
 * A simple time of day class primarily used to error verify that hours,
 * minutes, and seconds are valid.  Hours, minutes, and seconds are not
 * allowed to be negative.  0 <= Hours < 24, 0 <= Minutes < 60,
 * 0 <= Seconds <= 60.
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
class TimeOfDay {
  public:
    static constexpr int MAXHOUR      {23};
    static constexpr int MAXMINUTE    {59};
    static constexpr double MAXSECOND {60.0};

    /**
     * Initialize at the beginning of the day.
     */
    TimeOfDay() {}
    
    /**
     * Initialize with given time values. 
     *
     * @param   hours    Hour of the day, 0 <= hour < 24
     * @param   minutes  Minutes, 0 <= minutes < 60
     * @param   seconds  Seconds, 0 <= seconds <= 60
     *
     * @throws invalid_argument
     */
    TimeOfDay(int hours, int minutes, double seconds);
    
    /**
     * Initialize with a string representations of time values.
     *
     * @param   hours    Hour of the day, 0 <= hour < 24
     * @param   minutes  Minutes, 0 <= minutes < 60
     * @param   seconds  Seconds, 0 <= seconds <= 60
     *
     * @throws invalid_argument
     */
    TimeOfDay(std::string hours, std::string minutes, std::string seconds);

    /**
     * Set with given time values. 
     *
     * @param   hours    Hour of the day, 0 <= hour < 24
     * @param   minutes  Minutes, 0 <= minutes < 60
     * @param   seconds  Seconds, 0 <= seconds <= 60
     *
     * @throws invalid_argument
     */
    void set(int hours, int minutes, double seconds);

    /** @return   Hour of the day */
    double hour() const { return hr; }

    /** @return   Minutes */
    double minutes() const { return mn; }

    /** @return   Seconds */
    double seconds() const { return sec; }

  private:
    int hr {0}, mn {0};
    double sec {0.0};
};

#endif  // UTL_TIME_OF_DAY_H
