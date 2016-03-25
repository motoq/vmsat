/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef COMP_FUNCTION_H
#define COMP_FUNCTION_H

#include <memory>
#include <ostream>
#include <array>
#include <map>
#include <vector>
#include <string>

#include <comp_isimulation.h>
#include <comp_irecord.h>

/**
 * Keywords associated with functions to be executed using case file objects
 */
enum class CompType {
  EARTHROT,
  RSS,
  NONE
};

/**
 * Table translating text function names into enum values
 */
const std::map<std::string,CompType> function_table {
  {"EarthRot", CompType::EARTHROT},
  {"RSS",      CompType::RSS}
};

/**
 * Abstract class defining an interface to be fullfilled by objects that
 * compute results composed of one or more records of data.  Public methods
 * common to these objects for accessing the data in a generic way along with
 * protected utility methods that aid subclass generation are included.
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
class CompIFunction {
  public:

    /**
     * Initialize with the type of function to be called
     *
     * @param   ct   Function type
     */
    CompIFunction(CompType ct): comp_type{ct} {}

    virtual ~CompIFunction() {}

    /** @return   The type of function */
    CompType ftype() const { return comp_type; }

    /**
     * Process analysis request using case definition values
     *
     * @param   cs   Reference to simulation calling this function
     */
    virtual void execute(const CompISimulation &cs) = 0;

    /**
     * Report analysis results
     *
     * @param   out   Output stream for formatted output
     */
    virtual void report(std::ostream &out) const = 0;

    /**
     * @return   Label attached to this instance of this function. This
     *           is not the CompType name itself, but a handle used to
     *           refer back to this set of computations and results.
     */
    std::string label() const { return fnct_label; }

    /**
     * Indicates if generated outputs are labeled for further use, such
     * as an input to another function or output to a file.
     *
     * @return   If true, function output is available for use besides
     *           the default formatted outout.
     */
    bool report_label() const { return do_label; }

    /**
     * Each record is a time stamped set of data computed by this function.
     *
     * @return  Number of output records.
     */
    unsigned int num_records() const { return nrec; }

    /**
     * Interface to return a record given the index number.  
     */
    virtual std::unique_ptr<CompIRecord> record(unsigned int ndx) const = 0;

    /**
     * Indicates the number of sets of units in a given record of data.
     * For example, ephemeris may have 3 types for position, velocity, and
     * acceleration.  A set is a consecutive grouping of the same units.
     * Three position values, followed by three velocity values, followed by
     * a position value would then return 3 types of units.
     *
     * @return   The number of sets of different unit types.  A set is a
     *           consecutive set of units.
     */
    int num_unit_types() const { return nunits; }

    /**
     * @param   ndx   Zero based index for retrieval of unit set of interest.
     *
     * @return   Zero based offset for the set of units associated with ndx.
     *           See num_unit_types(), unit_labels, etc...
     */
    int unit_offsets(int ndx) const { return ubands[ndx]; }

    /**
     * @param   ndx   Zero based index for retrieval of unit set of interest.
     *
     * @return   Scale factors required to convert from internal computational
     *           units to unit types indicated by unit_labels(int ndx).
     */
    double unit_factors(int ndx) const { return ufactors[ndx]; }

    /**
     * @param   ndx   Zero based index for retrieval of unit set of interest.
     *
     * @return   Label indicating units obtained by multiplying internal values
     *           by the factor returned with unit_factors(int ndx).
     */
    std::string unit_labels(int ndx) const { return ulabels[ndx]; }


  protected:
    /**
     * Utility function to parse the function label.  If only a function
     * label is supplied, assume this is the filename for output and turn
     * off all other modifiers.  The format for a label is "FL*:label" where
     * the ':' is a separator, 'F' indicates this is a filename, 'L' indicates
     * this is a label, and '*' indicates formatted output should still be
     * sent through the default stream.  All or none may be applied.  Note
     * that "F:label" is redundant.
     *
     * @param   String containing function label and labeling options.
     *
     * @throws   invalid_argument
     */
    void report_options(const std::string& report_str);

    /**
     * @return   If true, outputs are to be sent to the requested output
     *           stream
     */
    bool report_stream() const { return do_ostream; }
 
    /**
     * @return   If true, outputs are to be sent to a .csv file
     */
    bool report_file() const { return do_file; }

    /**
     * @param   Sets the number of records.
     */
    void num_rec(unsigned int nr) { nrec = nr; }

    /**
     * @param   lbl1   Name of first function to find in comps list
     * @param   lbl2   Name of second function to find in comps list
     * @param   comps  List to search for comps with lbl1 and lbl2 names
     *
     * @return   Locations in comps.  Negative values indicate label not
     *           found.
     */
    std::array<int, 2> find_comp_locs(
                     const std::string& lbl1, const  std::string& lbl2,
                     const std::vector<std::unique_ptr<CompIFunction>> &comps);
/*
    {
       std::array<int, 2> vals {{ -1, -1}};
       vals[0] = 7;
       vals[1] = 5;
       return vals;
    }
*/

    /**
     * Adds a set of units to this functions outputs.  See num_unit_types(),
     * unit_offsets(), unit_factors(), and unit_labels() for more info.
     *
     * @param   lbl      Name for units
     * @param   factor   Scale factor converting from internal units to 
     *                   these units
     * @param   offset   Indicates where new unit types start in a record,
     *                   zero based.
     */
    void add_unit_type(std::string lbl, double factor, int offset);

  private:
    unsigned int nrec {0};                  // Number of records of data
    CompType comp_type {CompType::NONE};    // Function type
    std::string fnct_label {""};            // Internal name and/or filename
    bool do_ostream {true};                 // Standard formatted output
    bool do_file {false};                   // .csv file
    bool do_label {false};                  // Labeled function (handle)
      // Units tracking for outputs
    int nunits {0};                         // Number of unit types in record
    std::vector<int> ubands;                // Offset for each unit type
    std::vector<double> ufactors;           // Conversion from comp units
    std::vector<std::string> ulabels;       // Labels for converted units
};


#endif  // COMP_FUNCTION_H
