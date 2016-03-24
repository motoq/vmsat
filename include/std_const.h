/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef STD_CONST_H
#define STD_CONST_H

/**
 * Mathematical constants such as pi or conversion factors based on standards
 * (vs. emperical "constants" such as GM).
 *
 * @author  Kurt Motekew
 * @date    20160314
 */

constexpr double PI {3.141592653589793238462643383279502884};

constexpr double ARCSEC_PER_RAD {180.0*3600.0/PI};         //! Per Radian
constexpr double MAS_PER_RAD {1000.0*ARCSEC_PER_RAD};      //! Milliarcsecond

#endif  // STD_CONST_H

