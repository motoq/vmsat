/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdexcept>
#include <string>

#include <comp_ifunction.h>

/*
 * Parses function label name and options.
 */
void CompIFunction::report_options(std::string rptstr)
{
    // Initialize function label with input string.  Remove options
    // while parsing until only the label is left.
  fnct_label = rptstr;
    // Since a label is present, ensure everything is turned off unless
    // turned on.
  do_ostream = false;
  do_label = false;
  do_file = false;
    // "Token" to indicate the presence of label options
  static const std::string separator(":");
  std::size_t found = fnct_label.find(separator);

    // Check to see if there are modifiers.  If none, turn on file output
    // by default.
  if (found != std::string::npos) {
      // Maximum allowable found not including ':'
    static constexpr std::size_t max_pos = static_cast<std::size_t>(3);
    if (found > max_pos) {
      throw std::invalid_argument("Invalid number of report options");
    } else {
      static constexpr std::size_t zero = static_cast<std::size_t>(0);
      static constexpr std::size_t delta = static_cast<std::size_t>(1);
      for (std::size_t ii = zero; ii < found; ii++) {
        char rpt_type = fnct_label[zero];
        switch(rpt_type) {
          case 'F':                         // File output
          case 'f':
            do_file = true;
            break;
          case 'L':                         // Label
          case 'l':
            do_label = true;
            break;
          case '*':                         // Standard formatted output
            do_ostream = true;
            break;
          default:
            throw std::invalid_argument("Invalid report option");
        }
        fnct_label.erase(zero, delta);      // Drop option from label
      }
      fnct_label.erase(zero, delta);        // Finally drop ':' from label
    }
  } else {
    do_file = true;                         // Default to filename
  }
}


void CompIFunction::add_unit_type(std::string lbl, double factor, int offset)
{
  nunits++;
  ulabels.push_back(lbl);
  ufactors.push_back(factor);
  ubands.push_back(offset);
}
