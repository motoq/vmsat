/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef VMSAT_CASE_H
#define VMSAT_CASE_H

#include <memory>
#include <ostream>
#include <string>
#include <map>
#include <vector>

#include <comp_isimulation.h>
#include <comp_ifunction.h>
#include <astro_julian_date.h>

/**
 * Keywords associated with inputs related to configuring a case file
 */
enum class CaseKeyWord {
  NONE,                           // Do nothing keyword
  COMPUTE,                        // Use definitions to compute something
  SIMSTART,                       // Simulation start time
  SIMDAYS                         // Simulation duration
};

/**
 * Table translating text keywords from input stream into enum values
 */
const std::map<std::string,CaseKeyWord> keyword_table {
  {"None",     CaseKeyWord::NONE},
  {"Compute",  CaseKeyWord::COMPUTE},
  {"SimStart", CaseKeyWord::SIMSTART},
  {"SimDays",  CaseKeyWord::SIMDAYS}
};

/**
 * This type of case (simulation) is initialized with an input stream of
 * keywords and definitions.  Information w.r.t. any parsing errors is relayed
 * back to the user.  A list of functions to be executed based on the inputs
 * is stored.  A summary of the inputs can be output to the desired stream.
 * The functions can be executed and then reports output to a stream and/or
 * .csv files.
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
class VmsatCase : public CompISimulation {
  public:
    /**
     * Parses an input stream defining the case.  Instead of throwing an
     * exception when an error is encountered, the validity flag is set to
     * false and the location in the stream (about) where the error occured
     * along with the offending token is recorded.  If an exception does
     * manage to escape, then something went very wrong...
     *
     * @param   Input source for case definition.
     */
    VmsatCase(std::istream&);

    /**
     * Executes each requested "Compute" function.
     */
    void execute();

    /**
     * Sends each report to appropriate outputs (outpout stream in a human
     * readable format and/or .csv file)
     */
    void report();

    /** @return  Simulation start time */
    virtual JulianDate startJD() const;

    /** @return  Simulation period in days */
    virtual double simDays() const;

    /**
     * This summary of the case is meant to verify the input stream was
     * properly interpreted.
     *
     * @return   Summary info about this case
     */
    std::string to_str();

    /**
     * @param   Stream for which to push details about the requested scenario.
     */
    void to_stream(std::ostream &os);

    /** @return   Stream location at beginning of case description */
    std::streampos case_start() const { return case_pos0; }

    /** @return   If true, no errors were encountered parsing the case */
    bool is_valid() const { return valid; }

    /** @return   Stream location after last valid entry */
    std::streampos err_start() const { return err_pos0; }

    /** @return   Stream location after beginning of error */
    std::streampos err_stop()  const { return err_pos; }

    /** @return   Token associated with input error */
    std::string err_token() const { return etoken; }

  private:
      // Error handling/reporting
    bool valid {true};
    std::streampos case_pos0 {0};
    std::streampos  err_pos0 {0};
    std::streampos  err_pos  {0};
    std::string etoken;

      // Case data
    JulianDate sim_start_jd;
    double sim_days {1.0};
    std::vector<std::unique_ptr<CompIFunction>> comp_requests;

   /**
    * @param   ndx      Keyword type to parse
    * @param   inputs   Inputs associated with keyword
    *
    * @throws  std::out_of_range
    * @throws  std::invalid_argument
    */
    void parse_keyword_block(CaseKeyWord ndx,
                             const std::vector<std::string>& inputs);
};


#endif  // VMSAT_CASE_H
