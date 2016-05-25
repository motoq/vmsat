/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef COMP_SCALAR_H
#define COMP_SCALAR_H

#include <memory.h>

#include <comp_irecord.h>
#include <astro_julian_date.h>

/**
 * Record of data containing a single value and timestamp
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
class CompScalar : public CompIRecord {
  public:
    CompScalar(const JulianDate& jd, double  s);

    double scalarValue() const { return sval; }

    virtual double rss(CompIRecord* rec) const;

  private:
    double sval {0};
};

#endif  // COMP_SCALAR_H
