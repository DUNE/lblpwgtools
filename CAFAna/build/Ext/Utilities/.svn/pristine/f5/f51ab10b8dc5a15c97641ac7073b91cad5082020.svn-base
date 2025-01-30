//
// Main program for a utility that prints the number of events/subruns/runs
// that are present in an art format event-data file.
//

#include "Utilities/eventCount/Arguments.hh"
#include "Utilities/eventCount/FileInfo.hh"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main( int argc, char** argv ){

  // Parse and validate command line arguments.
  nova::Arguments arg(argc, argv);

  // Separates the output for multiple files.
  string separator = "\n" + string(60,'=') + "\n";

  std::vector<nova::FileInfo> infos;
  infos.reserve( arg.fileNames.size() );

  bool full = arg.style == nova::Arguments::full;

  for ( size_t i=0; i<arg.fileNames.size(); ++i ){
    auto const& filename = arg.fileNames.at(i);

    //if ( i!=0 && full ) cout << separator << endl;

    infos.emplace_back( filename );
    auto const& info(infos.back());

    if ( full ){
      info.fullPrint(cout);
    } else{
      info.minimalPrint(cout);
    }

  }

  return 0;
}
