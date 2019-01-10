#include "common_fit_definitions.C"
  
// This function can be called with different sample types to parallelise the jobs
void remake_inputs(std::string outFileName, std::string sampleName, int max=0, bool reload=true){

  if (reload || TFile(outFileName.c_str()).IsZombie())
    std::cout << "Making file: " << outFileName << std::endl;
  else return;
  
  osc::IOscCalculatorAdjustable* this_calc = NuFitOscCalc(1);      
  std::vector<const ISyst*> systlist = GetListOfSysts();

  TFile fout(outFileName.c_str(), "RECREATE");
  MakePredictionInterp(&fout, GetSampleType(sampleName), systlist, max);
  fout.Close();

  return;
}
