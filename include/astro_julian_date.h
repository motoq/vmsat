/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ASTRO_JULIAN_DATE_H
#define ASTRO_JULIAN_DATE_H

#include <string>

#include <utl_greg_date.h>

/**
 * A Julian Date class designed to preserve precision by splitting the date
 * into high and low values along with an optional seconds storage counter.
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
class JulianDate {
  public:
      // Useful Epochs
    static constexpr double J2000 {2451545.0};   // Jan 1, 2000 12:00:00 TT
    static constexpr double J1900 {2415021.0};   // Jan 1, 1900 12:00:00 UT1
    static constexpr double GPS0  {2444244.5};   // Jan 6, 1980 00:00:00 UT
    static constexpr double MJD   {2400000.5};   // Subtract from JD to get MJD
      // Conversions
    static constexpr double HR_PER_DAY  {24.0};
    static constexpr double DAY_PER_HR  {1.0/HR_PER_DAY};
    static constexpr double MIN_PER_DAY {1440.0};
    static constexpr double DAY_PER_MIN {1.0/MIN_PER_DAY};
    static constexpr double SEC_PER_DAY {86400.0};
    static constexpr double DAY_PER_SEC {1.0/SEC_PER_DAY};

    /** Initialize with J2000 */
    JulianDate() {}

    /**
     * See equivalent set method for details.
     */
    JulianDate(double jdDays, double jdFrac = 0.0, double jdSeconds = 0.0);

    /**
     * See equivalent set method for details.
     */
    JulianDate(const JulianDate& jd);

    /**
     * See equivalent set method for details.
     */
    JulianDate(const GregDate& gd,
               int hr = 0, int min = 0, double sec = 0.0);

    /**
     * Initialize using Julian Date components
     *
     * @param   jdDays      Days portion of Julian Date, or full JD if other
     *                      parameters are zero
     * @param   jdFrac      Fractional days to add to jdDays
     * @param   jsSeconds   Seconds to add to jdDays and jdFrac
     */
    void set(double jdDays, double jdFrac = 0.0, double jdSeconds = 0.0);

    /**
     * Initialize using an existing JulianDate.
     *
     * @param   jd   JD to copy
     */
    void set(const JulianDate& jd);

    /**
     * Initialize with a Gregorian date and time of day.  The time parameters
     * are not bounded typical clock 12/24 and 60 base systems - as many
     * hours, minutes, and seconds from the gd, +/-, are allowed and
     * incorporated into the internal JD representation.
     *
     * @param   gd    Gregorian Date
     * @param   hr    Hours from gd
     * @param   min   Minutes from gd and hr
     * @param   sec   Seconds from gd, hr, and min
     */
    void set(const GregDate& gd, int hr = 0, int min = 0, double sec = 0.0);


    /**
     * @return   Julian Date, scalar form
     */
    double jd() const;

    /**
     * @return   Modified Julian Date, scalar
     */
    double mjd() const;

    /**
     * @return   Large portion of the Julian Date, on the order of
     *           2,400,000.  Ideally, this would be the Julian Date
     *           corresponding to noon, but this is not necessary,
     *           depending on the value of jdLowVal.  Units of days.
     */
    double jdHiVal()   const { return jdHi; }

    /**
     * @return   Small portion of the Julian Date, typically on the order
     *           of a fraction of a day.  Units of days.
     */
    double jdLowVal() const;

    /**
     * @param   days   Days to add to this Julian Date (or subtracted, if
     *                 negative).
     */
    JulianDate& operator+=(double days);

    /**
     * @param   days   Days to add (or subtract, if negative).
     *
     * @return   Copy of this JulianDate, adjusted by days
     */
    JulianDate operator+(double days);

    /**
     * @return  The days and fraction difference between the two Julian
     *          dates:  LargerJD - SmallerJD = PositiveDays
     */
    double operator-(const JulianDate& jd);

      /**
       * @return   Gregorian Date and time as a string.  Time is
       *           in base 24:60:60 format.
       */
    std::string to_str();

//    friend std::ostream& operator<<(std::ostream&, const JulianDate&);

  private:
    double jdHi {J2000};                    // Days
    double jdLow {0.0};                     // Fraction of a day
    double jdSec {0.0};                     // Seconds

    double gd2jd(int year, int month, int day);
    void normalize();
    void jd2gd(int& year, int& month, int& day,
               int& hour, int& minutes, double& seconds);
};

#endif  // ASTRO_JULIAN_DATE_H
