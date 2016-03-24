/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <astro_julian_date.h>
#include <astro_leap_sec.h>

constexpr static int NLEAP {27};
constexpr static int taimutc[NLEAP] =
{
  10,  //  1
  11,  //  2
  12,  //  3
  13,  //  4
  14,  //  5
  15,  //  6
  16,  //  7
  17,  //  8
  18,  //  9
  19,  // 10
  20,  // 11
  21,  // 12
  22,  // 13
  23,  // 14
  24,  // 15
  25,  // 16
  26,  // 17
  27,  // 18
  28,  // 19
  29,  // 20
  30,  // 21
  31,  // 22
  32,  // 23
  33,  // 24
  34,  // 25
  35,  // 26
  36   // 27
};
constexpr static double jd_vals[NLEAP] =
{                                       
  2441317.5,
  2441500.5,
  2441683.5,
  2442048.5,
  2442413.5,
  2442778.5,
  2443144.5,
  2443509.5,
  2443874.5,
  2444239.5,
  2444786.5,
  2445151.5,
  2445516.5,
  2446247.5,
  2447161.5,
  2447892.5,
  2448257.5,
  2448804.5,
  2449169.5,
  2449534.5,
  2450083.5,
  2450630.5,
  2451179.5,
  2453736.5,
  2454832.5,
  2456109.5,
  2457204.5
};


double LeapSec::taiMutc(double jd) const
{
  double leapsec {0.0};
  for (int ii=(NLEAP-1); ii>=0; --ii) {
    if (jd > jd_vals[ii]) {
      leapsec = static_cast<int>(taimutc[ii]);
      break;
    }
  }
  return leapsec;
}


double LeapSec::taiMutc(const JulianDate &jdc) const
{
  double jd {jdc.jd()};
  double leapsec {0.0};
  for (int ii=(NLEAP-1); ii>=0; --ii) {
    if (jd > jd_vals[ii]) {
      leapsec = static_cast<int>(taimutc[ii]);
      break;
    }
  }
  return leapsec;
}
