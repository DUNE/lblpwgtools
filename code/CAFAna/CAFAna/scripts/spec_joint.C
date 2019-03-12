#include "common_fit_definitions.C"

void spec_joint(std::string stateFname  = "common_state_mcc11v3.root",
		std::string outputFname = "spec_hists_mcc11v3.root"){
  
   gROOT->SetBatch(1);

  // Get the systematics to use
   std::vector<const ISyst*> systlist = GetListOfSysts();

  // Get the prediction interpolators
  std::vector<unique_ptr<PredictionInterp> > return_list = GetPredictionInterps(stateFname, systlist);
  PredictionInterp& predInterpFDNumuFHC = *return_list[0].release();
  PredictionInterp& predInterpFDNueFHC  = *return_list[1].release();
  PredictionInterp& predInterpFDNumuRHC = *return_list[2].release();
  PredictionInterp& predInterpFDNueRHC  = *return_list[3].release();
  PredictionInterp& predInterpNDNumuFHC = *return_list[4].release();
  PredictionInterp& predInterpNDNumuRHC = *return_list[5].release();

  // Open 
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");

  osc::NoOscillations noOsc;

  // Unoscillated FD histograms
  std::vector<TH1*> FD_FHCNumu_uohists = GetMCComponents(&predInterpFDNumuFHC, &noOsc, "FD_FHC_Numu_unosc", pot_fd);
  std::vector<TH1*> FD_FHCNue_uohists  = GetMCComponents(&predInterpFDNueFHC, &noOsc, "FD_FHC_Nue_unosc", pot_fd);
  std::vector<TH1*> FD_RHCNumu_uohists = GetMCComponents(&predInterpFDNumuRHC, &noOsc, "FD_RHC_Numu_unosc", pot_fd);
  std::vector<TH1*> FD_RHCNue_uohists  = GetMCComponents(&predInterpFDNueRHC, &noOsc, "FD_RHC_Nue_unosc", pot_fd);
  for (auto & hist : FD_FHCNumu_uohists) hist->Write();
  for (auto & hist : FD_FHCNue_uohists)  hist->Write();
  for (auto & hist : FD_RHCNumu_uohists) hist->Write();
  for (auto & hist : FD_RHCNue_uohists)  hist->Write();
  
  // Sort out the ND histograms
  std::vector<TH1*> ND_FHC_hists     = GetMCComponents(&predInterpNDNumuFHC, &noOsc, "ND_FHC", pot_nd);
  std::vector<TH1*> ND_RHC_hists     = GetMCComponents(&predInterpNDNumuRHC, &noOsc, "ND_RHC", pot_nd);

  std::vector<TH1*> ND_FHC_MChists   = GetMCComponents(&predInterpNDNumuFHC, &noOsc, "ND_FHC_MCSTAT", 0);
  std::vector<TH1*> ND_RHC_MChists   = GetMCComponents(&predInterpNDNumuRHC, &noOsc, "ND_RHC_MCSTAT", 0);

  std::vector<TH1*> ND_FHC_1Dhists   = GetMCComponents(&predInterpNDNumuFHC, &noOsc, "ND_FHC_1D", pot_nd, true);
  std::vector<TH1*> ND_RHC_1Dhists   = GetMCComponents(&predInterpNDNumuRHC, &noOsc, "ND_RHC_1D", pot_nd, true);  
  
  for (auto & hist : ND_FHC_hists)   hist->Write();
  for (auto & hist : ND_RHC_hists)   hist->Write();
  for (auto & hist : ND_FHC_MChists) hist->Write();
  for (auto & hist : ND_RHC_MChists) hist->Write();
  for (auto & hist : ND_FHC_1Dhists) hist->Write();
  for (auto & hist : ND_RHC_1Dhists) hist->Write();  
  
  std::string dcpnames[4] = {"0pi","piover2","pi","3piover2"};

  for(int hie = -1; hie <= +1; hie += 2){

    osc::IOscCalculatorAdjustable* inputOsc = NuFitOscCalc(hie);

    const std::string hieStr = (hie > 0) ? "nh" : "ih";

    for(int deltaIdx = 0; deltaIdx < 4; ++deltaIdx){
      inputOsc->SetdCP(deltaIdx/2.*TMath::Pi());
      const std::string dcpStr = dcpnames[deltaIdx];

      // FD components for this set of parameters
      std::vector<TH1*> FD_FHCNumu_hists = GetMCComponents(&predInterpFDNumuFHC, inputOsc, "FD_FHC_Numu_"+hieStr+"_"+dcpStr, pot_fd);
      std::vector<TH1*> FD_FHCNue_hists  = GetMCComponents(&predInterpFDNueFHC, inputOsc, "FD_FHC_Nue_"+hieStr+"_"+dcpStr, pot_fd);
      std::vector<TH1*> FD_RHCNumu_hists = GetMCComponents(&predInterpFDNumuRHC, inputOsc, "FD_RHC_Numu_"+hieStr+"_"+dcpStr, pot_fd);
      std::vector<TH1*> FD_RHCNue_hists  = GetMCComponents(&predInterpFDNueRHC, inputOsc, "FD_RHC_Nue_"+hieStr+"_"+dcpStr, pot_fd);
      
      for (auto & hist : FD_FHCNumu_hists) hist->Write();
      for (auto & hist : FD_FHCNue_hists)  hist->Write();
      for (auto & hist : FD_RHCNumu_hists) hist->Write();
      for (auto & hist : FD_RHCNue_hists)  hist->Write();
      
    }
  }
  fout->Close();
}

