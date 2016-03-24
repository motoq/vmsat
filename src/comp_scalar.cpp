/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <memory.h>
#include <math.h>

#include <comp_irecord.h>
#include <comp_scalar.h>
#include <astro_julian_date.h>

CompScalar::CompScalar(const JulianDate &jd, double  s)
{
  CompIRecord::timeStamp(jd);
  sval = s;
}

double CompScalar::rss(CompIRecord *rec) const
{
  //std::unique_ptr<CompScalar> srec =
  //                          static_cast<std::unique_ptr<CompScalar>>(rec);
  CompScalar *srec = dynamic_cast<CompScalar*>(rec);

  double residual = sval - srec->scalarValue();
  return sqrt(residual*residual);
}
