/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <memory>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>

#include <comp_isimulation.h>
#include <comp_ifunction.h>
#include <comp_irecord.h>
#include <comp_scalar.h>
#include <comp_rss.h>
#include <astro_julian_date.h>


CompRSS::CompRSS(const std::vector<std::string> &funct_params,
                 const std::vector<std::unique_ptr<CompIFunction>> &comps)
                                                 : CompIFunction(CompType::RSS)
{
  comps_ptr = &comps;
  unsigned int nparams = static_cast<unsigned int>(funct_params.size());
  if (nparams < 5  &&  nparams > 2) {
    bool found_first = false;
    bool found_second = false;
    unsigned int nrpts = static_cast<unsigned int>(comps.size());
      // Search through list of comp functions and locate index
      // corresponding to function labels that are to be compared
    for (unsigned int ii=0; ii<nrpts; ++ii) {
      if (comps[ii]->report_label()) {
        if (!found_first && comps[ii]->label().compare(funct_params[1]) == 0) {
          label1 = comps[ii]->label();
          found_first = true;
          f1ndx = ii;
        } else if (!found_second  &&
                   comps[ii]->label().compare(funct_params[2]) == 0) {
          label2 = comps[ii]->label();
          found_second = true;
          f2ndx = ii;
        }
        if (found_first  &&  found_second) {
          found = true;
          break;
        }
      }
    }
    if (!found) {
      std::cerr << "\nRSS types not found\n";
      throw std::invalid_argument("Invalid RSS parameters");
    }
      // Set units if types match
    if (comps[f1ndx]->ftype() != comps[f2ndx]->ftype()) {
      std::cerr << "\nFunctions to RSS don't match\n";
      throw std::invalid_argument("Functions to RSS don't match");
    }
    if (comps[f1ndx]->num_unit_types() == 1) {
      CompIFunction::add_unit_type(comps[f1ndx]->unit_labels(0),
                                  comps[f1ndx]->unit_factors(0),
                                  comps[f1ndx]->unit_offsets(0));
    } else {
      std::cerr << "\nCan't RSS results with multiple types\n";
      throw std::invalid_argument("Can't RSS results with multiple types");
    }
    if (nparams == 4) {
      try {
        CompIFunction::report_options(funct_params[3]);
      } catch(std::invalid_argument &iae) {
        std::cerr << "\nInvalid report options: " << funct_params[3] << '\n';
        throw iae;
      }
    }
  } else {
    throw std::invalid_argument("Wrong number of RSS parameters");
  }
}

void CompRSS::execute(const CompISimulation &ci)
{
    // check compatibility (type, number, delta)
  if (found  &&  (*comps_ptr)[f1ndx]->label().compare(label1) == 0  &&
                 (*comps_ptr)[f2ndx]->label().compare(label2) == 0) {
    unsigned int npts1 = (*comps_ptr)[f1ndx]->num_records();
    unsigned int npts2 = (*comps_ptr)[f2ndx]->num_records();
    if (npts1 == npts2) {
      cmp_lst.reserve(npts1);
      for (unsigned int ii=0; ii<npts1; ++ii) {
        std::unique_ptr<CompIRecord> cr1 = (*comps_ptr)[f1ndx]->record(ii);
        std::unique_ptr<CompIRecord> cr2 = (*comps_ptr)[f2ndx]->record(ii);
        CompIRecord *cs1ptr = cr1.get();
        CompIRecord *cs2ptr = cr2.get();
        JulianDate jd1 = cs1ptr->timeStamp();
        JulianDate jd2 = cs2ptr->timeStamp();
        if (jd1 - jd2  !=  0.0) {
          std::cerr << "\nJDs in RSS not equal";
        }
        double rss_val = cs1ptr->rss(cs2ptr);
        cmp_lst.push_back(CompScalar(jd1, rss_val));
      }
    } else {
      std::cerr << "\nRSS types don't match or have moved\n";
    }
  } 
}

void CompRSS::report(std::ostream &out) const
{
    // Set scale factor to zero - if not set below then something is broken
  double ufactor {0.0};
  std::string units {""};
  if (CompIFunction::num_unit_types() == 1) {
    units = CompIFunction::unit_labels(0);
    ufactor = CompIFunction::unit_factors(0);
  }

  int nval = static_cast<int>(cmp_lst.size());

    // Send readable text to stream output
  out << "\nRSS " << (*comps_ptr)[f1ndx]->label() <<
            " & " << (*comps_ptr)[f2ndx]->label();
  if (CompIFunction::report_stream()) {
    for (int ii=0; ii<nval; ++ii) {
      JulianDate jd = cmp_lst[ii].timeStamp();
      char buf[128];                          
      snprintf(buf, sizeof(buf),
               "\n %1.13f %s at %s", ufactor*cmp_lst[ii].scalarValue(),
                                     units.c_str(), jd.to_str().c_str());
      out << buf;
    }
  }

    // Output to comma separated value file?
  if (CompIFunction::report_file()) {
    ;
  }
}

std::unique_ptr<CompIRecord> CompRSS::record(unsigned int ndx) const
{
  return std::unique_ptr<CompIRecord> (new CompScalar(cmp_lst[ndx].timeStamp(),
                                                  cmp_lst[ndx].scalarValue()));
}
