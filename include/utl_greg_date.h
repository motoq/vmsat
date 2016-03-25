/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UTL_GREG_DATE_H
#define UTL_GREG_DATE_H

#include <string>

/**
 * A simple Gregorian Date class primarily used to error verify that a
 * year, month, and day are valid.
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
class GregDate {
  public:
    static constexpr int GREGYEAR {1582};        ///< Gregorian Calendar epoch
    static constexpr int MAXYEAR  {9999};

    /**
     * Initialized to the date of the Sputnik launch.
     */
    GregDate() {}
    
    /**
     * Initialize with integer representations of calendar values.
     *
     * @param   year   Four digit representation of the year
     * @param   month  1 <= month <= 12
     * @param   day    1 <= day <= {28, 29, 30, 31}, depending on the
     *                 month/year combination.
     *
     * @throws invalid_argument
     */
    GregDate(int year, int month, int day);
    
    /**
     * Initialize with a string representations of calendar values.
     *
     * @param   year   Four digit representation of the year
     * @param   month  1 <= month <= 12
     * @param   day    1 <= day <= {28, 29, 30, 31}, depending on the
     *                 month/year combination.
     *
     * @throws invalid_argument
     */
    GregDate(std::string year, std::string month, std::string day);

    /**
     * Set date with integer representations of calendar values.
     *
     * @param   year   Four digit representation of the year
     * @param   month  1 <= month <= 12
     * @param   day    1 <= day <= {28, 29, 30, 31}, depending on the
     *                 month/year combination.
     *
     * @throws invalid_argument
     */
    void set(int year, int month, int day);

    /** @return   Four digit representation of the year */
    double year() const { return yr; }

    /** @return   Month number, 1->12 */
    double month() const { return mnth; }

    /** @return   Day of the month, 1->{28,29,30,31} */
    double day() const { return dy; }

    /**
     * @param   year   Four digit representation of the year
     */
    bool isLeapYear(int year);

  private:
    static constexpr int JAN  {1};
    static constexpr int FEB  {2};
    static constexpr int DEC {12};
    int yr {1957}, mnth {10}, dy {4};
};

#endif  // UTL_GREG_DATE_H
