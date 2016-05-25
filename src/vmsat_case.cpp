/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <vmsat_case.h>
#include <comp_ifunction.h>
#include <comp_earth_rot.h>
#include <comp_rss.h>
#include <utl_greg_date.h>
#include <utl_time_of_day.h>
#include <astro_julian_date.h>

static void reset_stream(std::istream&);

VmsatCase::VmsatCase(std::istream& is)
{
    // Record start of case description in stream for error feedback
  this->case_pos0 = is.tellg();
    // Read one token at a time skipping to EOL when encountering a
    // comment.  Push the token into a vector buffer for processing of
    // each major "input block"
  bool parsing_block {false};
  std::vector<std::string> input_block;
  CaseKeyWord kw_ndx = CaseKeyWord::NONE;
  std::string token;
  while (this->valid  &&  (is >> token)) {
    if (token.front() == '#') {
      // If this is a comment, read until EOL
      char ch;
      do {
        if (!is.get(ch)) return;
      } while (ch != '\n'  && ch != '\r');
    } else if (/*{*/ token == "}" && parsing_block) {
      // End of a block encountered - process
      try {
        this->parse_keyword_block(kw_ndx, input_block);
      } catch (std::invalid_argument &ia) {
        reset_stream(is);
        this->err_pos = is.tellg();
        this->valid = false;
      }
      input_block.clear();
      parsing_block = false;
    } else if (!parsing_block) {
      // Expecting a keyword followed by an opening bracket
      this->etoken = token;
      try {
        this->err_pos0 = is.tellg();
        kw_ndx = keyword_table.at(token);
        std::string start_bracket;
        is >> start_bracket;
        if (start_bracket == "{" /*}*/) {
          parsing_block = true;
        } else {
          std::cerr << '\n' << "Expecting end bracket, not " << token << '\n';
          reset_stream(is);
          this->err_pos = is.tellg();
          this->valid = false;
        }
      } catch(std::out_of_range &oor) {
        std::cerr << '\n' << "Not a keyword: " << token << '\n';
        reset_stream(is);
        this->err_pos = is.tellg();
        this->valid = false;
      } catch (std::exception &e) {
        reset_stream(is);
        this->err_pos = is.tellg();
        this->valid = false;
      }
    } else if (parsing_block) {
      // Add token to input block that will be evaluated based on keyword
      input_block.push_back(token);
    } else {
      reset_stream(is);
      this->err_pos = is.tellg();
      this->valid = false;
    }
  }
    // If the input block isn't empty, then the input stream ended before
    // the last input block was ingested.
  if (!input_block.empty()) {
    std::cerr << '\n' << "Unexpected end of input stream: " << token << '\n';
    reset_stream(is);
    this->err_pos = is.tellg();
    this->valid = false;
  }
}

void VmsatCase::execute()
{
  unsigned int nrpts = static_cast<unsigned int>(comp_requests.size());
  for (unsigned int ii=0; ii<nrpts; ++ii) {
    comp_requests[ii]->execute(*this);
  }
  /*
  std::vector<std::unique_ptr<CompIFunction>>::iterator itr;
  for (itr = comp_requests.begin(); itr !=  comp_requests.end(); ++ itr) {
    (*itr)->execute(*this);
  }
  */
}


void VmsatCase::report()
{
  unsigned int nrpts = static_cast<unsigned int>(comp_requests.size());
  for (unsigned int ii=0; ii<nrpts; ++ii) {
    comp_requests[ii]->report(std::cout);
  }
}


JulianDate VmsatCase::startJD() const
{
  return sim_start_jd;
}


double VmsatCase::simDays() const
{
  return sim_days;
}


std::string VmsatCase::to_str()
{
  char buf[128];
  std::size_t bsz = sizeof(buf);
  std::string ret_str = "\nSimulation Time Period:  Starting " +
                        sim_start_jd.to_str();

    // Epoch and duration
  ret_str.append(" for ");
  if (sim_days >= 1.0) {
    snprintf(buf, bsz, "%1.1f", sim_days);
    ret_str.append(buf);
    if (sim_days > 1.0) {
      ret_str.append(" days");
    } else {
      ret_str.append(" day");
    }
  } else {
    snprintf(buf, bsz, "%1.2f", JulianDate::MIN_PER_DAY*sim_days);
    ret_str.append(buf);
    ret_str.append(" minutes");
  }
  ret_str.append("\n");

  return ret_str;
}


void VmsatCase::to_stream(std::ostream &out)
{
  out << to_str();
}


void VmsatCase::parse_keyword_block(CaseKeyWord ndx,
                                    const std::vector<std::string>& inputs)
{
  switch (ndx) {
    case CaseKeyWord::NONE:
      ;  // Just a block of inputs to throw away
      break;
    case CaseKeyWord::COMPUTE:
      if (1 <= static_cast<int>(inputs.size())) {
        try {
          CompType cf_ndx = function_table.at(inputs[0]);
          switch (cf_ndx) {
            case CompType::EARTHROT:
              comp_requests.emplace_back(new CompEarthRot(inputs));
              break;
            case CompType::RSS:
              comp_requests.emplace_back(new CompRSS(inputs, comp_requests));
              break;
            case CompType::NONE:
              ;
          }
        } catch(std::out_of_range& oor) {
          std::cerr << "\n" << "Not a COMPUTE Function: " << inputs[0] << "\n";
          throw;
        }
      }
      break;
    case CaseKeyWord::SIMSTART:
      if (6 == static_cast<int>(inputs.size())) {
        GregDate gd(inputs[0], inputs[1], inputs[2]);
        TimeOfDay tod(inputs[3], inputs[4], inputs[5]);
        this->sim_start_jd.set(gd, tod.hour(), tod.minutes(), tod.seconds());
      } else if (3 == static_cast<int>(inputs.size())) {
        GregDate gd(inputs[0], inputs[1], inputs[2]);
        this->sim_start_jd.set(gd);
      } else {
        throw std::invalid_argument("Wrong number of SIMSTART parameters");
      }
      break;
    case CaseKeyWord::SIMDAYS:
      if (1 == static_cast<int>(inputs.size())) {
        std::istringstream iss(inputs[0]);
        if (!(iss >> this->sim_days)) {
          throw std::invalid_argument("Bad Duration");
        }
      } else {
        throw std::invalid_argument("Wrong number of SIMDAYS parameters");
      }
  }
}


/*
 * This should be called when there is an error parsing a value
 * from the stream so the end location of the error in the stream
 * can correctly be determined.  It should not be called when sucessfully
 * parsed data (say, a set of characters becomes an int) are logically
 * bad (say, trying to create Feb 31st).
 */
static void reset_stream(std::istream& bad_is)
{
  bad_is.clear();
  std::string bad_token;
  if (!(bad_is >> bad_token)) {
    bad_is.clear();
  }
}
