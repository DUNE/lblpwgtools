// ETW May 2018
// Standard script for DUNE spectra
// Input files TensorFlow CVN training from Fall 2018 

#include "CAFAna/Analysis/Fit.h"
#include "CAFAna/Analysis/CalcsNuFit.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

using namespace ana;

#include "Utilities/rootlogon.C"

#include "OscLib/func/IOscCalculator.h"

#include "StandardRecord/StandardRecord.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2.h"

const Var kRecoE_nue = SIMPLEVAR(dune.Ev_reco_nue);
const Var kRecoE_numu = SIMPLEVAR(dune.Ev_reco_numu);
const Var kPIDFD_NUMU = SIMPLEVAR(dune.cvnnumu);
const Var kPIDFD_NUE = SIMPLEVAR(dune.cvnnue);
const Var kPID_MVA_NUMU = SIMPLEVAR(dune.mvanumu);

const Var kvtxx_truth = SIMPLEVAR(dune.vtx_x);
const Var kvtxy_truth = SIMPLEVAR(dune.vtx_y);
const Var kvtxz_truth = SIMPLEVAR(dune.vtx_z);

const Cut kPassCVN_NUE = kPIDFD_NUE>0.7 && kPIDFD_NUMU<0.5;
const Cut kPassCVN_NUMU = kPIDFD_NUMU>0.5 && kPIDFD_NUE<0.7;

// 125 MeV bins from 0.0 to 8GeV
const HistAxis axis_nue("Reconstructed energy (GeV)",
                    Binning::Simple(64, 0.0, 8.0),
                    kRecoE_nue);

const HistAxis axis_numu("Reconstructed energy (GeV)",
                    Binning::Simple(64, 0.0, 8.0),
                    kRecoE_numu);


// POT/yr * 3.5yrs * mass correction
const double potFD = 3.5 * 1.1e21 * 40/1.13;

const char* stateFname = "spec_state.root";
const char* outputFname = "spec_hist.root";

void spec(bool reload = false)
{
  rootlogon(); // style
  
  if(reload || TFile(stateFname).IsZombie()){

    SpectrumLoader loaderFDFHCNonswap("/dune/data/users/marshalc/CAFs/mcc11_test/FD_FHC_nonswap.root");
    SpectrumLoader loaderFDFHCNue("/dune/data/users/marshalc/CAFs/mcc11_test/FD_FHC_nueswap.root");
    SpectrumLoaderBase& loaderFDFHCNuTau = kNullLoader;

    SpectrumLoader loaderFDRHCNonswap("/dune/data/users/marshalc/CAFs/mcc11_test/FD_RHC_nonswap.root");
    SpectrumLoader loaderFDRHCNue("/dune/data/users/marshalc/CAFs/mcc11_test/FD_RHC_nueswap.root");
    SpectrumLoaderBase& loaderFDRHCNuTau = kNullLoader;

    Loaders dummyLoaders; // PredictionGenerator insists on this

    //Selection applied
    PredictionNoExtrap predFDNumuFHC(loaderFDFHCNonswap,
                                     loaderFDFHCNue,
                                     loaderFDFHCNuTau,
                                     axis_numu,
                                     kPassCVN_NUMU && kIsTrueFV);

    PredictionNoExtrap predFDNueFHC(loaderFDFHCNonswap,
                                    loaderFDFHCNue,
                                    loaderFDFHCNuTau,
                                    axis_nue,
                                    kPassCVN_NUE && kIsTrueFV);

    PredictionNoExtrap predFDNumuRHC(loaderFDRHCNonswap,
                                     loaderFDRHCNue,
                                     loaderFDRHCNuTau,
                                     axis_numu,
                                     kPassCVN_NUMU && kIsTrueFV);

    PredictionNoExtrap predFDNueRHC(loaderFDRHCNonswap,
                                    loaderFDRHCNue,
                                    loaderFDRHCNuTau,
                                    axis_nue,
                                    kPassCVN_NUE && kIsTrueFV);


    //Fiducial Only for Efficiencies
    PredictionNoExtrap predFDNumuFHC_Fid(loaderFDFHCNonswap,
                                     loaderFDFHCNue,
                                     loaderFDFHCNuTau,
                                     axis_numu,
                                     kIsTrueFV);

    PredictionNoExtrap predFDNueFHC_Fid(loaderFDFHCNonswap,
                                    loaderFDFHCNue,
                                    loaderFDFHCNuTau,
                                    axis_nue,
                                    kIsTrueFV);

    PredictionNoExtrap predFDNumuRHC_Fid(loaderFDRHCNonswap,
                                     loaderFDRHCNue,
                                     loaderFDRHCNuTau,
                                     axis_numu,
                                     kIsTrueFV);

    PredictionNoExtrap predFDNueRHC_Fid(loaderFDRHCNonswap,
                                    loaderFDRHCNue,
                                    loaderFDRHCNuTau,
                                    axis_nue,
                                    kIsTrueFV);

    loaderFDFHCNonswap.Go();
    loaderFDFHCNue.Go();
    loaderFDRHCNonswap.Go();
    loaderFDRHCNue.Go();

    TFile fout(stateFname, "RECREATE");
    predFDNumuFHC.SaveTo(fout.mkdir("fd_numu_fhc"));
    predFDNueFHC.SaveTo(fout.mkdir("fd_nue_fhc"));
    predFDNumuRHC.SaveTo(fout.mkdir("fd_numu_rhc"));
    predFDNueRHC.SaveTo(fout.mkdir("fd_nue_rhc"));

    predFDNumuFHC_Fid.SaveTo(fout.mkdir("fd_numu_fhc_fid"));
    predFDNueFHC_Fid.SaveTo(fout.mkdir("fd_nue_fhc_fid"));
    predFDNumuRHC_Fid.SaveTo(fout.mkdir("fd_numu_rhc_fid"));
    predFDNueRHC_Fid.SaveTo(fout.mkdir("fd_nue_rhc_fid"));


    std::cout << "All done making state..." << std::endl;

    }
  else{    
    std::cout << "Loading state from " << stateFname << std::endl; 
  }

  TFile fin(stateFname);
  PredictionNoExtrap& predFDNumuFHC = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_numu_fhc")).release();
  PredictionNoExtrap& predFDNueFHC = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_nue_fhc")).release();
  PredictionNoExtrap& predFDNumuRHC = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_numu_rhc")).release();
  PredictionNoExtrap& predFDNueRHC = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_nue_rhc")).release();
  PredictionNoExtrap& predFDNumuFHC_Fid = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_numu_fhc_fid")).release();
  PredictionNoExtrap& predFDNueFHC_Fid = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_nue_fhc_fid")).release();
  PredictionNoExtrap& predFDNumuRHC_Fid = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_numu_rhc_fid")).release();
  PredictionNoExtrap& predFDNueRHC_Fid = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_nue_rhc_fid")).release();


  fin.Close();
  std::cout << "Done loading state" << std::endl;

  TFile* fout = new TFile(outputFname, "RECREATE");

  osc::NoOscillations noOsc;
  TH1* hnumufhc_sig_unosc = predFDNumuFHC.PredictComponent(&noOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* hnumufhc_sig_unosc_fid = predFDNumuFHC_Fid.PredictComponent(&noOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);

  std::string dcpnames[4] = {"0pi","piover2","pi","3piover2"};

  for(int hie = -1; hie <= +1; hie += 2){

    osc::IOscCalculatorAdjustable* inputOsc = NuFitOscCalc(hie);

    const std::string hieStr = (hie > 0) ? "nh" : "ih";

    for(int deltaIdx = 0; deltaIdx < 4; ++deltaIdx){
      inputOsc->SetdCP(deltaIdx/2.*TMath::Pi());
      const std::string dcpStr = dcpnames[deltaIdx];

      TH1* hnumufhc = predFDNumuFHC.Predict(inputOsc).ToTH1(potFD);
      TH1* hnuefhc = predFDNueFHC.Predict(inputOsc).ToTH1(potFD);
      TH1* hnumurhc = predFDNumuRHC.Predict(inputOsc).ToTH1(potFD);
      TH1* hnuerhc = predFDNueRHC.Predict(inputOsc).ToTH1(potFD);

      //FHC Dis
      TH1* hnumufhc_sig = predFDNumuFHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);
      TH1* hnumufhc_ncbg = predFDNumuFHC.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kBoth).ToTH1(potFD);
      TH1* hnumufhc_nutaubg = predFDNumuFHC.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnumufhc_wsbg = predFDNumuFHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
      TH1* hnumufhc_nuebg = predFDNumuFHC.PredictComponent(inputOsc, Flavors::kAllNuE, Current::kCC, Sign::kBoth).ToTH1(potFD);

      
      TH1* hnumufhc_sig_fid = predFDNumuFHC_Fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);
      TH1* hnumufhc_ncbg_fid = predFDNumuFHC_Fid.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kBoth).ToTH1(potFD);
      TH1* hnumufhc_nutaubg_fid = predFDNumuFHC_Fid.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnumufhc_wsbg_fid = predFDNumuFHC_Fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
      TH1* hnumufhc_nuebg_fid = predFDNumuFHC_Fid.PredictComponent(inputOsc, Flavors::kAllNuE, Current::kCC, Sign::kBoth).ToTH1(potFD);

      //RHC Dis
      TH1* hnumurhc_sig = predFDNumuRHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
      TH1* hnumurhc_ncbg = predFDNumuRHC.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kBoth).ToTH1(potFD);
      TH1* hnumurhc_nutaubg = predFDNumuRHC.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnumurhc_wsbg = predFDNumuRHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);

      TH1* hnumurhc_sig_fid = predFDNumuRHC_Fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
      TH1* hnumurhc_ncbg_fid = predFDNumuRHC_Fid.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kBoth).ToTH1(potFD);
      TH1* hnumurhc_nutaubg_fid = predFDNumuRHC_Fid.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnumurhc_wsbg_fid = predFDNumuRHC_Fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);

      //FHC App
      TH1* hnuefhc_sig = predFDNueFHC.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuefhc_ncbg = predFDNueFHC.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuefhc_beambg = predFDNueFHC.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuefhc_nutaubg = predFDNueFHC.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuefhc_numubg = predFDNueFHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kBoth).ToTH1(potFD);

      TH1* hnuefhc_sig_fid = predFDNueFHC_Fid.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuefhc_ncbg_fid = predFDNueFHC_Fid.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuefhc_beambg_fid = predFDNueFHC_Fid.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuefhc_nutaubg_fid = predFDNueFHC_Fid.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuefhc_numubg_fid = predFDNueFHC_Fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kBoth).ToTH1(potFD);

      //RHC App
      TH1* hnuerhc_sig = predFDNueRHC.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuerhc_ncbg = predFDNueRHC.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuerhc_beambg = predFDNueRHC.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuerhc_nutaubg = predFDNueRHC.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuerhc_numubg = predFDNueRHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kBoth).ToTH1(potFD);

      TH1* hnuerhc_sig_fid = predFDNueRHC_Fid.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuerhc_ncbg_fid = predFDNueRHC_Fid.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuerhc_beambg_fid = predFDNueRHC_Fid.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuerhc_nutaubg_fid = predFDNueRHC_Fid.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kBoth).ToTH1(potFD);
      TH1* hnuerhc_numubg_fid = predFDNueRHC_Fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kBoth).ToTH1(potFD);

      hnumufhc_sig_unosc->Write("numu_fhc_unosc_sig");
      hnumufhc_sig_unosc_fid->Write("fid_numu_fhc_unosc_sig");

      hnumufhc->Write(("numu_fhc_"+hieStr+"_"+dcpStr).c_str());
      hnumufhc_sig->Write(("numu_fhc_sig_"+hieStr+"_"+dcpStr).c_str());
      hnumufhc_ncbg->Write(("numu_fhc_ncbg_"+hieStr+"_"+dcpStr).c_str());
      hnumufhc_nutaubg->Write(("numu_fhc_nutaubg_"+hieStr+"_"+dcpStr).c_str());
      hnumufhc_nuebg->Write(("numu_fhc_nuebg_"+hieStr+"_"+dcpStr).c_str());
      hnumufhc_wsbg->Write(("numu_fhc_wsbg_"+hieStr+"_"+dcpStr).c_str());

      hnuefhc->Write(("nue_fhc_"+hieStr+"_"+dcpStr).c_str());
      hnuefhc_sig->Write(("nue_fhc_sig_"+hieStr+"_"+dcpStr).c_str());
      hnuefhc_ncbg->Write(("nue_fhc_ncbg_"+hieStr+"_"+dcpStr).c_str());
      hnuefhc_beambg->Write(("nue_fhc_beambg_"+hieStr+"_"+dcpStr).c_str());
      hnuefhc_nutaubg->Write(("nue_fhc_nutaubg_"+hieStr+"_"+dcpStr).c_str());
      hnuefhc_numubg->Write(("nue_fhc_numubg_"+hieStr+"_"+dcpStr).c_str());
	
      hnumurhc->Write(("numu_rhc_"+hieStr+"_"+dcpStr).c_str());
      hnumurhc_sig->Write(("numu_rhc_sig_"+hieStr+"_"+dcpStr).c_str());
      hnumurhc_ncbg->Write(("numu_rhc_ncbg_"+hieStr+"_"+dcpStr).c_str());
      hnumurhc_nutaubg->Write(("numu_rhc_nutaubg_"+hieStr+"_"+dcpStr).c_str());
      hnumurhc_wsbg->Write(("numu_rhc_wsbg_"+hieStr+"_"+dcpStr).c_str());

      hnuerhc->Write(("nue_rhc_"+hieStr+"_"+dcpStr).c_str());
      hnuerhc_sig->Write(("nue_rhc_sig_"+hieStr+"_"+dcpStr).c_str());
      hnuerhc_ncbg->Write(("nue_rhc_ncbg_"+hieStr+"_"+dcpStr).c_str());
      hnuerhc_beambg->Write(("nue_rhc_beambg_"+hieStr+"_"+dcpStr).c_str());
      hnuerhc_nutaubg->Write(("nue_rhc_nutaubg_"+hieStr+"_"+dcpStr).c_str());
      hnuerhc_numubg->Write(("nue_rhc_numubg_"+hieStr+"_"+dcpStr).c_str());

      hnumufhc_sig_fid->Write(("fid_numu_fhc_sig_"+hieStr+"_"+dcpStr).c_str());
      hnumufhc_ncbg_fid->Write(("fid_numu_fhc_ncbg_"+hieStr+"_"+dcpStr).c_str());
      hnumufhc_nutaubg_fid->Write(("fid_numu_fhc_nutaubg_"+hieStr+"_"+dcpStr).c_str());
      hnumufhc_nuebg_fid->Write(("fid_numu_fhc_nuebg_"+hieStr+"_"+dcpStr).c_str());
      hnumufhc_wsbg_fid->Write(("fid_numu_fhc_wsbg_"+hieStr+"_"+dcpStr).c_str());

      hnuefhc_sig_fid->Write(("fid_nue_fhc_sig_"+hieStr+"_"+dcpStr).c_str());
      hnuefhc_ncbg_fid->Write(("fid_nue_fhc_ncbg_"+hieStr+"_"+dcpStr).c_str());
      hnuefhc_beambg_fid->Write(("fid_nue_fhc_beambg_"+hieStr+"_"+dcpStr).c_str());
      hnuefhc_nutaubg_fid->Write(("fid_nue_fhc_nutaubg_"+hieStr+"_"+dcpStr).c_str());
      hnuefhc_numubg_fid->Write(("fid_nue_fhc_numubg_"+hieStr+"_"+dcpStr).c_str());
	
      hnumurhc_sig_fid->Write(("fid_numu_rhc_sig_"+hieStr+"_"+dcpStr).c_str());
      hnumurhc_ncbg_fid->Write(("fid_numu_rhc_ncbg_"+hieStr+"_"+dcpStr).c_str());
      hnumurhc_nutaubg_fid->Write(("fid_numu_rhc_nutaubg_"+hieStr+"_"+dcpStr).c_str());
      hnumurhc_wsbg_fid->Write(("fid_numu_rhc_wsbg_"+hieStr+"_"+dcpStr).c_str());

      hnuerhc_sig_fid->Write(("fid_nue_rhc_sig_"+hieStr+"_"+dcpStr).c_str());
      hnuerhc_ncbg_fid->Write(("fid_nue_rhc_ncbg_"+hieStr+"_"+dcpStr).c_str());
      hnuerhc_beambg_fid->Write(("fid_nue_rhc_beambg_"+hieStr+"_"+dcpStr).c_str());
      hnuerhc_nutaubg_fid->Write(("fid_nue_rhc_nutaubg_"+hieStr+"_"+dcpStr).c_str());
      hnuerhc_numubg_fid->Write(("fid_nue_rhc_numubg_"+hieStr+"_"+dcpStr).c_str());

    }
  }
  fout->Close();
}

