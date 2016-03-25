VMSAT
=====

Vehicle Modeling & Simulation Analysis Tool
-------------------------------------------

The Vehicle Modeling & Simulation Analysis Tool (VMSAT) is a modeling
language focusing on astrodynamics related functionality.  VMSAT is
being developed as a platform in which existing modeling and simulation
libraries can be accessed with results streamed to formatted human
readable or .csv file formats.  Academic implementations of models and
analysis tools will be provided to serve as templates for more mature
external vetted/legacy libraries.

VMSAT utilizes two mature libraries upon which built-in models will rely
and external libraries may leverage.  The first is the International
Astronomical Union Standards of Fundamental Astronomy (IAU SOFA) C
library <http://www.iausofa.org/>.  Installation of SOFA-Issue
2015-02-09 or newer is required to build VMSAT.  The second (not yet
required) is the Eigen C++ template library for linear algebra
<http://eigen.tuxfamily.org>.

Core concepts behind VMSAT are:

1. Provide useful built in astrodynamics models and analysis tools
2. Provide convenient access to SOFA functionality
3. Supply an interface to external legacy libraries that have fallen out
   of use simply because the GUI supporting them can no longer be
   maintained
4. Provide a tool that allows for easy comparison of results from
   different functions and/or libraries
5. Provide a framework in which short response analysis can be addressed
   or algorithms prototyped

C++ was chosen for development because:

1. It plays well with legacy C and FORTRAN libraries
2. A number of mature and fast C++ math libraries are available
3. Code designed to run on hardware can be tested
4. The author is delving into C++ for the first time in a while and this
   project provides a good opportunity to get up to speed on the
   language (the code should hopefully evolve quickly as the author learns
   more about C++ vs. his usual C or Java preferences).

The Mozilla Public License 2.0 (MPL2) was chosen because it protects the
open source nature of this code while still allowing it to be used with
proprietary and closed source tools:
<https://www.mozilla.org/en-US/MPL/2.0/>

