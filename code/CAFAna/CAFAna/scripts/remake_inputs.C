#include "common_fit_definitions.C"

// This function can be called with different sample types to parallelise the
// jobs
void remake_inputs(std::string outFileName, std::string sampleName, int max = 0,
                   std::string ax = "", bool reload = true) {

  if (reload || TFile(outFileName.c_str()).IsZombie())
    std::cout << "Making file: " << outFileName << std::endl;
  else
    return;

  osc::IOscCalculatorAdjustable *this_calc = NuFitOscCalc(1);
  std::vector<const ISyst *> systlist = GetListOfSysts();

  TFile fout(outFileName.c_str(), "RECREATE");

  HistAxis const *ND_axtouse = &axErecYrecND;
  HistAxis const *FDnumu_axtouse = &axRecoEnuFDnumu;
  HistAxis const *FDnue_axtouse = &axRecoEnuFDnue;

  if (ax == "1DND") {
    ND_axtouse = &axErecND;
  } else if (ax == "FromDep") {
    ND_axtouse = &axErecYrecND_FromDep;
    FDnumu_axtouse = &axRecoEnuFD_FromDep;
    FDnue_axtouse = &axRecoEnuFD_FromDep;
  } else if (ax == "1DNDFromDep") {
    ND_axtouse = &axErec_FromDep;
    FDnumu_axtouse = &axRecoEnuFD_FromDep;
    FDnue_axtouse = &axRecoEnuFD_FromDep;
  } else if (ax == "CoarseBin1D") {
    ND_axtouse = &axErecND_coarsebin;
    FDnumu_axtouse = &axRecoEnuFDnumu_coarsebin;
    FDnue_axtouse = &axRecoEnuFDnue_coarsebin;
  } else if (ax == "VeryCoarseBin1D") {
    ND_axtouse = &axErecND_verycoarsebin;
    FDnumu_axtouse = &axRecoEnuFDnumu_verycoarsebin;
    FDnue_axtouse = &axRecoEnuFDnue_verycoarsebin;
  } else if (ax == "OneBin") {
    ND_axtouse = &axErecND_onebin;
    FDnumu_axtouse = &axRecoEnuFDnumu_onebin;
    FDnue_axtouse = &axRecoEnuFDnue_onebin;
  } else if (ax == "ETrue") {
    ND_axtouse = &axTrueE_unibin_coarse;
    FDnumu_axtouse = &axTrueE_unibin_coarse;
    FDnue_axtouse = &axTrueE_unibin_coarse;
  }

  MakePredictionInterp(&fout, GetSampleType(sampleName), systlist, max,
                       *ND_axtouse, *FDnumu_axtouse, *FDnue_axtouse);
  fout.Close();

  return;
}

#ifndef __CINT__
int main(int argc, char const *argv[]) {
  bool Use1DNDAxis = (argc >= 5);
  std::cout << "Remaking inputs into " << argv[1] << " for sample " << argv[2]
            << ", using at most " << std::atoi(argv[3]) << " events, and a "
            << (Use1DNDAxis ? "1D" : "2D") << " ND distribution." << std::endl;
  remake_inputs(argv[1], argv[2], std::atoi(argv[3]),
                Use1DNDAxis ? argv[4] : "");
}
#endif
