/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef COMP_EARTH_ROT_H
#define COMP_EARTH_ROT_H

#include <memory>
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <comp_isimulation.h>
#include <comp_ifunction.h>
#include <comp_irecord.h>
#include <comp_scalar.h>
#include <astro_julian_date.h>
#include <astro_leap_sec.h>
#include <astro_ut1mutc.h>

/**
 * Earth rotation types
 */
enum class EarthRotType {
  GMST1982,
  GMST2000
};

/**
 * Table translating earth rotation labels to enum values
 */
const std::map<std::string,EarthRotType> earth_rot_table {
  {"GMST1982", EarthRotType::GMST1982},
  {"GMST2000", EarthRotType::GMST2000}
};

/**
 * Set of functions for computing different forms of earth rotation
 * (sidereal time, earth rotation angle, or anything related).
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
class CompEarthRot : public CompIFunction {
  public:

    /**
     * Initialize Earth Rotation analysis request
     *
     * @param   funct_params  Parameter list for an earth rotation
     *                        function, with the first being
     *                        EARTHROT.  The remaining indices:
     *                        [1] = An EarthRotType string
     *                        [2] = Output rate
     *                        [3] = Optional label/filename
     *
     * @throws   invalid_argument if there is an error parsing the inputs
     */
    CompEarthRot(const std::vector<std::string> &funct_params);

    /**
     * Process analysis request using case definition values
     *
     * @param   cs   Calling simulation with general scenario information
     */
    virtual void execute(const CompISimulation & cs);

    /**
     * Create report of computed results consisting of formatted output
     * and/or a .csv file.
     *
     * @param   out   Stream for standard formatted output, if enabled.
     */
    virtual void report(std::ostream &out) const;

    /**
     * Retrieve a record of computed data given an index number.
     * Index limit available through super class.
     */
    virtual std::unique_ptr<CompIRecord> record(unsigned int ndx) const;

  private:
    LeapSec delta_at;
    UT1mUTC delta_ut;
    EarthRotType er_type;
    double dt_min{1.0};
    std::vector<CompScalar> cmp_lst;             // Saved outputs
};


#endif  // COMP_EARTH_ROT_H
