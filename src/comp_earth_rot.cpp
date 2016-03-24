/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>

#include <comp_isimulation.h>
#include <comp_ifunction.h>
#include <comp_irecord.h>
#include <comp_scalar.h>
#include <astro_julian_date.h>
#include <std_const.h>
#include <comp_earth_rot.h>

#include <sofa.h>

CompEarthRot::CompEarthRot(const std::vector<std::string> &funct_params)
                                           : CompIFunction(CompType::EARTHROT)
{
  int nparams = static_cast<int>(funct_params.size());
  if (nparams < 5  &&  nparams > 2) {
    try {
      er_type = earth_rot_table.at(funct_params[1]);
    } catch(std::out_of_range &oor) {
      std::cerr << "\nNot an Earth Rotation type: " << funct_params[1] << '\n';
      throw std::invalid_argument("Wrong number of EarthRot parameters");
    }
    dt_min = std::stod(funct_params[2]);
    if (nparams == 4) {
      try {
        CompIFunction::report_options(funct_params[3]);
      } catch(std::invalid_argument &iae) {
        std::cerr << "\nInvalid report options: " << funct_params[3] << '\n';
        throw iae;
      }
    }
  } else {
    throw std::invalid_argument("Wrong number of EarthRot parameters");
  }

    // Set up output units
  std::string units = "radians";
  double ufactor = 1.0;
  int offset = 0;
  CompIFunction::add_unit_type(units, ufactor, offset);
}

void CompEarthRot::execute(const CompISimulation &ci)
{
  JulianDate jd_now = ci.startJD();
  JulianDate jd_stop = ci.startJD();
  jd_stop += ci.simDays();
  double dt_days = dt_min/1440.0;
  std::vector<double>::size_type npts =
                    static_cast<std::vector<double>::size_type>
                    (1 + static_cast<int>((jd_stop - jd_now)/dt_days));
  cmp_lst.reserve(npts);
 
    // Increment time until the current time exceeds the stop time
  while (jd_stop - jd_now >= 0.0) {
    double sval{0.0};
    switch (er_type) {
        // For use with the IAU 1976 Precession and 1980 Nutation models
      case EarthRotType::GMST1982:
        {
          double ut1mutc = delta_ut.ut1Mutc(jd_now);
          JulianDate jdUT1 = jd_now;
          jdUT1 += ut1mutc*JulianDate::DAY_PER_SEC;
          sval = iauGmst82(jdUT1.jdHiVal(), jdUT1.jdLowVal());
        }
        break;
        // For use with the IAU 2000+ Equinox based theories
      case EarthRotType::GMST2000:
        {
          double ut1mutc = delta_ut.ut1Mutc(jd_now);
          JulianDate jdUT1 = jd_now;
          jdUT1 += ut1mutc*JulianDate::DAY_PER_SEC;
          double leapsec = delta_at.taiMutc(jd_now);
          JulianDate jdTT = jd_now;
          jdTT += (leapsec + 32.184)*JulianDate::DAY_PER_SEC;
          sval = iauGmst00(jdUT1.jdHiVal(), jdUT1.jdLowVal(),
                           jdTT.jdHiVal(),  jdTT.jdLowVal());
        }
        break;
    }
    cmp_lst.push_back(CompScalar(jd_now, sval));
    jd_now += dt_days;
  }
  CompIFunction::num_rec(static_cast<unsigned int>(cmp_lst.size()));
}

void CompEarthRot::report(std::ostream &out) const
{
    // Set scale factor to zero - if not set below then something is broken
  double ufactor {0.0};
  std::string units {""};
  if (CompIFunction::num_unit_types() == 1) {
    units = CompIFunction::unit_labels(0);
    ufactor = CompIFunction::unit_factors(0);
  }

    // Text representation of the type of earth rotation being computed.
  std::string type {""};
  switch (er_type) {
    case EarthRotType::GMST1982:
      type = "GMST1982";
      break;
    case EarthRotType::GMST2000:
      type = "GMST2000";
      break;
  }
  int nval = static_cast<int>(cmp_lst.size());

    // Send readable text to stream output
  if (CompIFunction::report_stream()) {
    for (int ii=0; ii<nval; ++ii) {
      JulianDate jd = cmp_lst[ii].timeStamp();
      char buf[128];
      snprintf(buf, sizeof(buf),
               "\n%s:  %1.13f %s at %s",
                type.c_str(), ufactor*cmp_lst[ii].scalarValue(),
                units.c_str(), jd.to_str().c_str());
      out << buf;
    }
  }

    // Output to comma separated value file?
  if (CompIFunction::report_file()) {
    std::string outfile_root = CompIFunction::label();
    if (outfile_root.length() > 0) {
      std::string csv_filename = outfile_root + ".csv";
      std::ofstream csv_file;
      csv_file.open (csv_filename);
      if (csv_file.is_open()) {
        csv_file.precision(10);
        for (int ii=0; ii<nval; ++ii) {
          JulianDate jd = cmp_lst[ii].timeStamp();
          char buf[128];
          snprintf(buf, sizeof(buf), "%1.13f,%1.13f", jd.mjd(),
                                     ufactor*cmp_lst[ii].scalarValue());
          csv_file << buf << '\n';
        }
      } else {
        std::cerr << "\nCan't open output file " << csv_filename << '\n';
      }
      csv_file.close();
    }
  }
}

std::unique_ptr<CompIRecord> CompEarthRot::record(unsigned int ndx) const
{
  return std::unique_ptr<CompIRecord> (new CompScalar(cmp_lst[ndx].timeStamp(),
                                                  cmp_lst[ndx].scalarValue()));
}
