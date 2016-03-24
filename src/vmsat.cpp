/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <fstream>

#include <vmsat_case.h>

/**
 * This is the main function to the Vehicle Modeling & Simulation Analysis
 * Tool.  It ingests a single input file to create a single case, regurgitates
 * a summary of the inputs, computes the requested analysis, and outputs the
 * results in the requested formats.  This driver accepts a single case
 * definition in what the underlying libraries would support as multiple cases
 * within a larger simulation.
 * <P>
 * Usage:  "vmsat inputfilename"
 * <P>
 * See supplemental documentation for input file syntax
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
int main(int argc, char* argv[])
{
    // Check for filename
  if (argc != 2) {
    std::cerr << "\nProper use is:  " << argv[0] << " <in_file>\n";
    return 0;
  }

    // Try to open for input
  std::ifstream in_file(argv[1]);
  if (!in_file.is_open()) {
    std::cerr << "\nError opening " << argv[1] << "\n";
    return 0;
  }

    // Ingest case file and close
  VmsatCase vc{in_file};
  if (!vc.is_valid()) {
    char c;
    int linenum = 1;
    in_file.seekg(in_file.beg);
    for (std::streamoff ii=0; ii<vc.err_start(); ++ii) {
      in_file.get(c);
      if (c == '\n') {
        linenum++;
      }
    }
    std::streamoff str_chunk_size = vc.err_stop() - vc.err_start();
    in_file.seekg(vc.err_start());
    std::cerr << "\nProblem on line " << linenum << ":  " << vc.err_token();
    for (std::streamoff ii=0; ii<str_chunk_size; ++ii) {
      in_file.get(c);
      std::cerr << c;
    }
    std::cerr << "\n";
  }
  in_file.close();

    // Output summary of input, run functions, and create reports
  if (vc.is_valid()) {
    vc.to_stream(std::cout);
    vc.execute();
    vc.report();
  }

  std::cout << "\n";
}
