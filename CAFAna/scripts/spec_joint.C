#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

char const *def_stateFname = "common_state_mcc11v3.root";
char const *def_outputFname = "spec_hists_mcc11v3.root";
char const *def_sampleString = "ndfd:336";
int const def_hie = 1;
char const *def_asimov_set = "0";
char const *def_fakeDataShift = "";

void spec_joint(std::string stateFname = def_stateFname,
                std::string outputFname = def_outputFname,
		std::string sampleString = def_sampleString,
		int hie=def_hie, std::string asimov_set=def_asimov_set,
		std::string fakeDataShift = def_fakeDataShift) {

  gROOT->SetBatch(1);

  double pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue, pot_fd_rhc_nue,
    pot_fd_fhc_numu, pot_fd_rhc_numu;
  bool ndprefit;
  ParseDataSamples(sampleString, pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue,
                   pot_fd_rhc_nue, pot_fd_fhc_numu, pot_fd_rhc_numu, ndprefit);


  // This function allows any dial to be set to non-nominal
  SystShifts trueSyst = GetFakeDataSystShift(fakeDataShift);

  // Get the prediction interpolators
  std::vector<std::unique_ptr<PredictionInterp>> return_list =
    GetPredictionInterps(stateFname, GetListOfSysts());
  PredictionInterp &predInterpFDNumuFHC = *return_list[0].release();
  PredictionInterp &predInterpFDNueFHC = *return_list[1].release();
  PredictionInterp &predInterpFDNumuRHC = *return_list[2].release();
  PredictionInterp &predInterpFDNueRHC = *return_list[3].release();
  PredictionInterp &predInterpNDNumuFHC = *return_list[4].release();
  PredictionInterp &predInterpNDNumuRHC = *return_list[5].release();

  // Open
  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");

  osc::NoOscillations noOsc;

  // Unoscillated FD histograms
  std::vector<TH1 *> FD_FHCNumu_uohists = GetMCSystComponents(
      &predInterpFDNumuFHC, &noOsc, trueSyst, "FD_FHC_Numu_unosc", pot_fd_fhc_numu);
  std::vector<TH1 *> FD_FHCNue_uohists =
      GetMCSystComponents(&predInterpFDNueFHC, &noOsc, trueSyst, "FD_FHC_Nue_unosc", pot_fd_fhc_nue);
  std::vector<TH1 *> FD_RHCNumu_uohists = GetMCSystComponents(
      &predInterpFDNumuRHC, &noOsc, trueSyst, "FD_RHC_Numu_unosc", pot_fd_rhc_numu);
  std::vector<TH1 *> FD_RHCNue_uohists =
      GetMCSystComponents(&predInterpFDNueRHC, &noOsc, trueSyst, "FD_RHC_Nue_unosc", pot_fd_rhc_nue);
  for (auto &hist : FD_FHCNumu_uohists)
    hist->Write();
  for (auto &hist : FD_FHCNue_uohists)
    hist->Write();
  for (auto &hist : FD_RHCNumu_uohists)
    hist->Write();
  for (auto &hist : FD_RHCNue_uohists)
    hist->Write();

  // Sort out the ND histograms
  std::vector<TH1 *> ND_FHC_hists =
    GetMCSystComponents(&predInterpNDNumuFHC, &noOsc, trueSyst, "ND_FHC", pot_nd_fhc);
  std::vector<TH1 *> ND_RHC_hists =
    GetMCSystComponents(&predInterpNDNumuRHC, &noOsc, trueSyst, "ND_RHC", pot_nd_rhc);
  
  std::vector<TH1 *> ND_FHC_MChists =
    GetMCSystComponents(&predInterpNDNumuFHC, &noOsc, trueSyst, "ND_FHC_MCSTAT", 0);
  std::vector<TH1 *> ND_RHC_MChists =
    GetMCSystComponents(&predInterpNDNumuRHC, &noOsc, trueSyst, "ND_RHC_MCSTAT", 0);
  
  std::vector<TH1 *> ND_FHC_1Dhists =
    GetMCSystComponents(&predInterpNDNumuFHC, &noOsc, trueSyst, "ND_FHC_1D", pot_nd_fhc, true);
  std::vector<TH1 *> ND_RHC_1Dhists =
    GetMCSystComponents(&predInterpNDNumuRHC, &noOsc, trueSyst, "ND_RHC_1D", pot_nd_rhc, true);

  for (auto &hist : ND_FHC_hists)
    hist->Write();
  for (auto &hist : ND_RHC_hists)
    hist->Write();
  for (auto &hist : ND_FHC_MChists)
    hist->Write();
  for (auto &hist : ND_RHC_MChists)
    hist->Write();
  for (auto &hist : ND_FHC_1Dhists)
    hist->Write();
  for (auto &hist : ND_RHC_1Dhists)
    hist->Write();

  // Get in the asimov set
  osc::IOscCalculatorAdjustable *inputOsc = NuFitOscCalc(hie, 1, asimov_set);

  // FD components for this set of parameters
  std::vector<TH1 *> FD_FHCNumu_hists =
    GetMCSystComponents(&predInterpFDNumuFHC, inputOsc, trueSyst, "FD_FHC_Numu", pot_fd_fhc_numu);
  std::vector<TH1 *> FD_FHCNue_hists =
    GetMCSystComponents(&predInterpFDNueFHC, inputOsc, trueSyst, "FD_FHC_Nue", pot_fd_fhc_nue);
  std::vector<TH1 *> FD_RHCNumu_hists =
    GetMCSystComponents(&predInterpFDNumuRHC, inputOsc, trueSyst, "FD_RHC_Numu", pot_fd_rhc_numu);
  std::vector<TH1 *> FD_RHCNue_hists =
    GetMCSystComponents(&predInterpFDNueRHC, inputOsc, trueSyst, "FD_RHC_Nue", pot_fd_rhc_nue);

  for (auto &hist : FD_FHCNumu_hists)
    hist->Write();
  for (auto &hist : FD_FHCNue_hists)
    hist->Write();
  for (auto &hist : FD_RHCNumu_hists)
    hist->Write();
  for (auto &hist : FD_RHCNue_hists)
    hist->Write();
  
  fout->Close();
}

#ifndef __CINT__
int main(int argc, char const *argv[]) {
  gROOT->SetMustClean(false);
  std::string stateFname = (argc > 1) ? argv[1] : def_stateFname;
  std::string outputFname = (argc > 2) ? argv[2] : def_outputFname;
  std::string sampleString = (argc > 3) ? argv[3] : def_sampleString;
  int hie = (argc > 4) ? atoi(argv[4]) : def_hie;
  std::string asimov_set = (argc > 5) ? argv[5] : def_asimov_set;
  std::string fakeDataShift = (argc > 6) ? argv[6] : def_fakeDataShift;
  spec_joint(stateFname,outputFname,sampleString,hie,asimov_set,fakeDataShift);
}
#endif
