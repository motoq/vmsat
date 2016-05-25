/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ASTRO_LEAP_SEC_H
#define ASTRO_LEAP_SEC_H

#include <astro_julian_date.h>

/**
 * A class to handle retrieval of leapseconds
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
class LeapSec {
  public:
    /**
     * @param   jd   The scalar form of a Julian Date for which to return
     *               the number of leap seconds.
     *
     * @return   The number of leapseconds, TAI - UTC
     */
    double taiMutc(double jd) const;

    /**
     * @param   jdc   The class form of a Julian Date for which to return
     *                the number of leap seconds.
     *
     * @return   The number of leapseconds, TAI - UTC
     */
    double taiMutc(const JulianDate& jdc) const;
};

#endif  // ASTRO_LEAP_SEC_H
