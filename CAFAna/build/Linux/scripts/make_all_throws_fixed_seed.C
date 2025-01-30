#define NO_MAT_MAIN
#include "make_all_throws.C"

void Usage(char const *argv[]) {
  std::cout
      << "[USAGE]: " << argv[0]
      << " <RNG SEED> [/path/to/state_stub] [output_filename.root] [NThrows] "
         "[SystSet: e.g. noxsec:nodet] [Sample: e.g. ndfd] [Throw: e.g. "
         "stat:fake:start] [Penalties: e.g. nopen] [Hierarchy: e.g. 1] "
         "[Throws to use all/mh/cpv/oct]"
      << std::endl;
}

int main(int argc, char const *argv[]) {

  gROOT->SetMustClean(false);

  if ((argc >= 2) &&
      ((std::string("-?") == argv[1]) || (std::string("--help") == argv[1]) ||
       (std::string("-h") == argv[1]))) {
    Usage(argv);
    return 0;
  }

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
  std::string types = (argc > 9) ? argv2[9] : def_types;
  std::string fakeDataShift = (argc > 10) ? argv2[10] : def_fakeDataShift;

  make_all_throws(stateFname, outputFname, nthrows, systSet, sampleString,
                  throwString, penaltyString, hie, types, fakeDataShift);
}
