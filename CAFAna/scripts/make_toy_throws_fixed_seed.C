
#define NO_MTT_MAIN
#define PROFILE_COUTS
#include "make_toy_throws.C"

int main(int argc, char const *argv[]) {

  gROOT->SetMustClean(false);

  gRNGSeed = atoi(argv[1]);

  argc--;
  char const **argv2 = &argv[1];

  std::string stateFname = (argc > 1) ? argv2[1] : def_stateFname;
  std::string outputFname = (argc > 2) ? argv2[2] : def_outputFname;
  int nthrows = (argc > 3) ? atoi(argv2[3]) : def_nthrows;
  std::string systSet = (argc > 4) ? argv2[4] : def_systSet;
  std::string sampleString = (argc > 5) ? argv2[5] : def_sampleString;
  std::string throwString = (argc > 6) ? argv2[6] : def_throwString;
  std::string penaltyString = (argc > 7) ? argv2[7] : def_penaltyString;
  int hie = (argc > 8) ? atoi(argv2[8]) : def_hie;
  std::string asimov_set = (argc > 9) ? argv2[9] : def_asimov_set;
  std::string oscVarString = (argc > 10) ? argv2[10] : def_oscVarString;

  make_toy_throws(stateFname, outputFname, nthrows, systSet, sampleString,
                  throwString, penaltyString, hie, asimov_set, oscVarString);
}
