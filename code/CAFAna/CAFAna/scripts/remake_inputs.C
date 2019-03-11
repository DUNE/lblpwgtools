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

  AxisBlob AxToUse = GetAxisBlob(ax);

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
