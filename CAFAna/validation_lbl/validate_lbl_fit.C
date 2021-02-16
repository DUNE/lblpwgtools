#include "helper_validation_lbl.h"

using namespace ana;

// maximum difference between profiles to test and the orifinals
const float maxdifference = 1e-5;
bool passedtest = false;

// set the experiment to test against
// currently only stats only and systs with 
// 2 detectors, 2 horn currents and 2 neutrino types.
TString def_detectors = "fdnd";
TString def_horns     = "rhcfhc";
TString def_neutrinos = "nuenumu";
bool def_systs = false;

void validate_lbl_fit(
  TString detectors="fdnd",
  TString horns="rhcfhc",
  TString neutrinos="nuenumu",
  bool systs = false)
{

  // Systematics to use: 1 or 2 for quick test
  std::string systtag = "stats";
  std::vector<const ISyst*> systlist = {};
  if(systs){
    systtag  = "systs";
    systlist = {&kEnergyScaleFD,&kFDRecoNumuSyst};
  }

  std::vector<std::string> neutrino = {};
  std::vector<std::string> detector = {};
  std::vector<std::string> horn     = {};
  std::vector<float> pot = {};

  if(neutrinos.Contains("nue")) neutrino.push_back("nue");
  if(neutrinos.Contains("numu")) neutrino.push_back("numu");

  if(detectors.Contains("nd")) detector.push_back("nd");
  if(detectors.Contains("fd")) detector.push_back("fd");

  if(horns.Contains("fhc")) horn.push_back("fhc");
  if(horns.Contains("rhc")) horn.push_back("rhc");

  const int nhorn = horn.size();
  const int ndetector = detector.size();
  const int nneutrino = neutrino.size();

  bool TwoBeams = (nhorn == 2);
  bool IsFHC = (horns.Contains("fhc"));


  // Get ND covariance matrix or not
  bool UseNDCovMat = true;
  bool useND = detectors.Contains("nd");

  ///// Load Predictions once
  std::vector<TString> tags; // to keep track of spectra types. I could add a Title to Spectrum or define a structure but I rather no go there atm.
  std::vector<PredictionInterp*> predictions;
  std::string fdir = "/pnfs/dune/persistent/users/dmendez/lbl_fit_validation/";

  for(int detId=0; detId<ndetector; detId++){
    for(int hornId=0; hornId<nhorn; hornId++){
      for(int nuId=0; nuId<nneutrino; nuId++){

        if(detector[detId]=="nd" && neutrino[nuId]=="nue") continue; // no nd nue prediction

        std::string fname = fdir+Form("state_%s_%s.root", detector[detId].c_str(), horn[hornId].c_str());
        std::string din = Form("%s_interp_%s_%s", detector[detId].c_str(), neutrino[nuId].c_str(), horn[hornId].c_str());
        tags.push_back(detector[detId]+neutrino[nuId]+horn[hornId]);
        TFile *fin = TFile::Open(fname.c_str(), "READ");
        predictions.push_back(LoadFrom<PredictionInterp>(fin, din).release());
        fin->Close();
        // Get nominal exposure per year from Analysis/Exposures.h
        if(horn[hornId]=="fhc"){
          if(detector[detId]=="nd") pot.push_back(kNDPOT[15] * 7.5);
          else pot.push_back(kFDPOT[15] * 7.5);
        } // fhc pot
        else{
          if(detector[detId]=="nd") pot.push_back(kNDPOT[15] * 7.5);
          else pot.push_back(kFDPOT[15] * 7.5);
        } // rhc pot

      } // nuId
    } // hornId
  } // detId

  // We could add many validation layers but let's not add one for the calculators
  // Fix the values here instead of setting them with NuFitOscCalc in case it changes.
  osc::IOscCalcAdjustable* trueOsc = GetTestCalculator();
  osc::IOscCalcAdjustable* testOsc = GetTestCalculator(); 

  // defining the predictions and fake data here might seem repetitive
  // but at least it's not reopening the prediction files at each steep
  std::vector<Spectrum> s_predictions;
  std::vector<Spectrum> s_fakedata;
  // fill the fake data in a separate loop so the covexpt doesn't try to find sth that doesn't exist 
  for(unsigned int predId=0; predId<predictions.size(); predId++){
    s_predictions.push_back(predictions[predId]->Predict(trueOsc));
    s_fakedata.push_back(s_predictions[predId].FakeData(pot[predId])); // second argument = 0 defaults to random throw number
  }

  // Determine what experiment this is
  MultiExperiment experiments = GetMultiExperimentLBL(predictions, s_fakedata, tags, UseNDCovMat, TwoBeams);

  std::map<const IFitVar*, std::vector<double>> oscSeeds = {};
      	      	
  SystShifts trueSyst = kNoShift;
  SystShifts testSyst = kNoShift;

  std::cout << "Beginning fit for LBL log-likelihood test" << std::endl;
  TH1* hdcp_test  = SqrtProfile(&experiments, testOsc, &kFitDeltaInPiUnits, 4, 0.0, 2.0, -1, {}, systlist);
  TH1* hss23_test = SqrtProfile(&experiments, testOsc, &kFitSinSqTheta23,   4, 0.4, 0.6, -1, {}, systlist);

  // get the profiles to compare with
  TString fname_comp = fdir+"/test_fhcrhc_ndfd_"+systtag+"_15years.root";
  TFile* fcomp = new TFile(fname_comp, "READ");
  TH1* hdcp  = (TH1*)fcomp->Get("sqrtprof_dcp");
  TH1* hss23 = (TH1*)fcomp->Get("sqrtprof_ss23");
  // take the difference between this profiles and the ones to test
  TH1* hdcp_diff  = (TH1*)hdcp->Clone();
  TH1* hss23_diff = (TH1*)hss23->Clone();
  hdcp_diff->Add(hdcp_test,-1);
  hss23_diff->Add(hss23_test,-1);

  // loop through the profile bins and print a warning if difference is greater than the set threshold
  for(int bin = 0; bin < hdcp_diff->GetNbinsX(); bin++){
    float difference = hdcp_diff->GetBinContent(bin);
    if(difference >= maxdifference) std::cerr << "Warning\n--->>> ";
    else passedtest = true;
    std::cout << "SqrtProfile difference (delta CP) = " << difference << "\n";
    // else std::cout << "Passed fit test" << std::endl;
  }
  for(int bin = 0; bin < hss23_diff->GetNbinsX(); bin++){
    float difference = hss23_diff->GetBinContent(bin);
    if(difference >= maxdifference) std::cerr << "Warning\n--->>> ";
    else passedtest = true;
    std::cout << "SqrtProfile difference (sstheta23) = " << difference << "\n";
  }

  if(passedtest) std::cout << "* Passed LBL fit test with flying colours *" << std::endl;
  else{
    std::cout << "===== FAILED LBL fit test =====" << std::endl;
    std::cout << "===== FAILED LBL fit test =====" << std::endl;
    std::cout << "===== FAILED LBL fit test =====" << std::endl;
  }

  hdcp       ->Delete();
  hss23      ->Delete();
  hdcp_test  ->Delete();
  hss23_test ->Delete();
  hdcp_diff  ->Delete();
  hss23_diff ->Delete();

  fcomp->Close();

}

int main(int argc, char const *argv[]){

  TString detectors = (argc > 1) ? TString(argv[1]) : def_detectors;
  TString horns     = (argc > 2) ? TString(argv[2]) : def_horns;
  TString neutrinos = (argc > 3) ? TString(argv[3]) : def_neutrinos;
  bool systs  = (argc > 4) ? atoi(argv[4]) : def_systs;

  validate_lbl_fit(detectors,horns,neutrinos,systs);

}
