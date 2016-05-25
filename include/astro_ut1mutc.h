/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ASTRO_UT1MUTC_H
#define ASTRO_UT1MUTC_H

#include <astro_julian_date.h>

/**
 * A class to handle retrieval of UT1-UTC.
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
class UT1mUTC {
  public:
    /**
     * @param   jd   The scalar form of a Julian Date for which to return
     *               the difference between UTC and UT1
     *
     * @return   UT1 - UTC, seconds
     */
    double ut1Mutc(double jd) const;

    /**
     * @param   jdc   The class form of a Julian Date for which to return
     *                the difference between UTC and UT1
     *
     * @return   UT1 - UTC, seconds
     */
    double ut1Mutc(const JulianDate& jdc) const;
};

#endif  // ASTRO_UT1MUTC_H
