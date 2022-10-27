#define NO_MAT_MAIN
#include "make_FC_throws_th13_test.C"

void Usage(char const *argv[]) {
  std::cout
      << "[USAGE]: " << argv[0]
      << " <RNG SEED> [/path/to/state_stub] [output_filename.root] [NThrows] "
         "[SystSet: e.g. noxsec:nodet] "
         "[FixPar: e.g. deltapi:0] "
         "[Sample: e.g. ndfd] [Throw: e.g. "
         "stat:fake:start] [Penalties: e.g. nopen] [Hierarchy: e.g. 1] "
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

  std::string stateFname = (argc > 1) ? argv[1] : def_stateFname;
  std::string outputFname = (argc > 2) ? argv[2] : def_outputFname;
  int nthrows = (argc > 3) ? atoi(argv[3]) : def_nthrows;
  std::string systSet = (argc > 4) ? argv[4] : def_systSet;
  std::string sampleString = (argc > 5) ? argv[5] : def_sampleString;
  std::string throwString = (argc > 6) ? argv[6] : def_throwString;
  int hie = (argc > 7) ? atoi(argv[7]) : def_hie;
  std::string asimov_set = (argc > 8) ? argv[8] : def_asimov_set;
  std::string fakeDataShift = (argc > 9) ? argv[9] : def_fakeDataShift;

  make_FC_throws_th13_test(stateFname, outputFname, nthrows, systSet, sampleString,
                           throwString, hie, asimov_set, fakeDataShift);

}
