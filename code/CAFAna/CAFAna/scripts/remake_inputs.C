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

  AxisBlob AxToUse = default_axes;
  if (ax == "1DND") {
    AxToUse.NDAx = &axErecND;
  } else if (ax == "FromDep") {
    AxToUse.NDAx = &axErecYrecND_FromDep;
    AxToUse.FDAx_numu = &axErecFD_FromDep;
    AxToUse.FDAx_nue = &axErecFD_FromDep;
  } else if ((ax == "1DNDFromDep") || (ax == "FakeData")) {
    AxToUse.NDAx = &axErecND_FromDep;
    AxToUse.FDAx_numu = &axErecFD_FromDep;
    AxToUse.FDAx_nue = &axErecFD_FromDep;
  } else if (ax == "CoarseBin1D") {
    AxToUse.NDAx = &axErecND_coarsebin;
    AxToUse.FDAx_numu = &axRecoEnuFDnumu_coarsebin;
    AxToUse.FDAx_nue = &axRecoEnuFDnue_coarsebin;
  } else if (ax == "VeryCoarseBin1D") {
    AxToUse.NDAx = &axErecND_verycoarsebin;
    AxToUse.FDAx_numu = &axRecoEnuFDnumu_verycoarsebin;
    AxToUse.FDAx_nue = &axRecoEnuFDnue_verycoarsebin;
  } else if (ax == "OneBin") {
    AxToUse.NDAx = &axErecND_onebin;
    AxToUse.FDAx_numu = &axRecoEnuFDnumu_onebin;
    AxToUse.FDAx_nue = &axRecoEnuFDnue_onebin;
  } else if (ax == "ETrue") {
    AxToUse.NDAx = &axTrueE_unibin;
    AxToUse.FDAx_numu = &axTrueE_unibin;
    AxToUse.FDAx_nue = &axTrueE_unibin;
  } else if (ax == "ETrueCoarse") {
    AxToUse.NDAx = &axTrueE_unibin_coarse;
    AxToUse.FDAx_numu = &axTrueE_unibin_coarse;
    AxToUse.FDAx_nue = &axTrueE_unibin_coarse;
  }

  MakePredictionInterp(&fout, GetSampleType(sampleName), systlist, max,
                       AxToUse);
  fout.Close();

  return;
}

#ifndef __CINT__
int main(int argc, char const *argv[]) {

  bool UseNonStandardAxes = (argc >= 5);
  std::cout << "Remaking inputs into " << argv[1] << " for sample " << argv[2]
            << ", using at most " << std::atoi(argv[3]) << " events"
            << (UseNonStandardAxes
                    ? (std::string(" with ") + argv[4] + " axes.")
                    : std::string("."))
            << std::endl;
  remake_inputs(argv[1], argv[2], (argc >= 4) ? std::atoi(argv[3]) : 0,
                UseNonStandardAxes ? argv[4] : "");
}
#endif
