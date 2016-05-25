/*
 * Copyright 2016 Kurt Motekew
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef COMP_RSS_H
#define COMP_RSS_H

#include <memory>
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <comp_isimulation.h>
#include <comp_ifunction.h>
#include <comp_irecord.h>
#include <comp_scalar.h>

/**
 * This function computes the Root Sum Square of the difference (residual)
 * of two previously computed functions that have been labeled.  It may be
 * more accurate to call it RRSS, the Root Residual Sum Square.  The term
 * RSS is typically thrown around to mean the L^2-Norm of the difference
 * of two compatible vectors.
 *
 * @author  Kurt Motekew
 * @date    20160314
 */
class CompRSS : public CompIFunction {
  public:

    /**
     * Initialize RSS function.
     *
     * @param   funct_params  Parameter list consisting of two previously
     *                        computed functions that are to be compared,
     *                        with the first parameter being RSS.
     *                        The remaining indices:
     *                        [1] = Label of first function results to compare
     *                        [2] = Label of second function results to cmpare
     *                        [3] = Optional label/filename
     * @param   comps         List of candidate functions from which to find
     *                        corresponding labels that are to be compared.
     *
     * @throws   invalid_argument  Given a syntax error or inability to find
     *                             requested/compatible input labels in the
     *                             current list of functions being processed.
     */
    CompRSS(const std::vector<std::string>& funct_params,
            const std::vector<std::unique_ptr<CompIFunction>>& comps);

    /**
     * Process analysis request using case definition values
     *
     * @param   cs   Calling simulation with general scenario information
     */
    virtual void execute(const CompISimulation& cs);

    /**
     * Create report of computed results consisting of formatted output
     * and/or a .csv file.
     *
     * @param   out   Stream for standard formatted output, if enabled.
     */
    virtual void report(std::ostream& out) const;

    /**
     * Retrieve a record of computed data given an index number.
     * Index limit available through super class.
     */
    virtual std::unique_ptr<CompIRecord> record(unsigned int ndx) const;

  private:
    bool found{false};
    unsigned int f1ndx {0};
    unsigned int f2ndx {0};
    std::string label1 {""};
    std::string label2 {""};
    const std::vector<std::unique_ptr<CompIFunction>> *comps_ptr;
    std::vector<CompScalar> cmp_lst; 
};


#endif  // COMP_RSS_H
