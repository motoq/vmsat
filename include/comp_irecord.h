/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef COMP_RECORD_H
#define COMP_RECORD_H

#include <memory>
#include <string>

#include <astro_julian_date.h>

/**
 * Abstract class for a record of timestamped data
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
class CompIRecord {
  public:
    JulianDate timeStamp() const { return jdval; }

    virtual double rss(CompIRecord *rec) const = 0;

  protected:
    void timeStamp(const JulianDate &jd) { jdval = jd; }

  private:
    JulianDate jdval;
};

#endif  // COMP_RECORD_H
