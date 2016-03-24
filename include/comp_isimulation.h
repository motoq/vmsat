/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef COMP_ISIMULATION_H
#define COMP_ISIMULATION_H

#include <astro_julian_date.h>

/**
 * Interface defining methods associated with a simulation or a subset
 * of a larger simulation (case).
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
class CompISimulation {
  public:
    CompISimulation() {}

    virtual ~CompISimulation() {}

    /** @return  Simulation start time */
    virtual JulianDate startJD() const = 0;

    /** @return  Simulation period in days */
    virtual double simDays() const = 0;
};


#endif  // COMP_ISIMULATION_H
