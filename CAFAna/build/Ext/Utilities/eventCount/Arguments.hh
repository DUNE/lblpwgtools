#ifndef Utilities_eventCount_Arguments_hh
#define Utilities_eventCount_Arguments_hh
//
// Parse and validate the argument list for the artProductSizes executable.
//
// See the implementation of usage() for the documentation.
//

#include <string>
#include <vector>

namespace nova {

  struct Arguments{

    // Style of printout.
    enum PrintStyle { minimal, full };

    Arguments ( int argc, char**argv );

    std::vector<std::string> fileNames;

    PrintStyle style = minimal;

  private:
    void usage() const;

  };

}

#endif /* Utilities_eventCount_Arguments_hh */
