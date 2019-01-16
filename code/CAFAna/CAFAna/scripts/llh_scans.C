#include "common_fit_definitions.C"

void llh_scans(std::string stateFname  = "common_state_mcc11v3.root",
	       std::string outputFname = "llh_scans_mcc11v3.root"){
  
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

  int hie = 1;
  const std::string hieStr = (hie > 0) ? "nh" : "ih";  
  bool oscvar = true;

  osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie);

  const Spectrum data_nue_fhc_syst = predInterpFDNueFHC.Predict(trueOsc).FakeData(pot_fd);
  SingleSampleExperiment app_expt_fhc_syst(&predInterpFDNueFHC, data_nue_fhc_syst);
  app_expt_fhc_syst.SetMaskHist(0.5, 8);
      
  const Spectrum data_nue_rhc_syst = predInterpFDNueRHC.Predict(trueOsc).FakeData(pot_fd);
  SingleSampleExperiment app_expt_rhc_syst(&predInterpFDNueRHC, data_nue_rhc_syst);
  app_expt_rhc_syst.SetMaskHist(0.5, 8);
  
  const Spectrum data_numu_fhc_syst = predInterpFDNumuFHC.Predict(trueOsc).FakeData(pot_fd);
  SingleSampleExperiment dis_expt_fhc_syst(&predInterpFDNumuFHC, data_numu_fhc_syst);
  dis_expt_fhc_syst.SetMaskHist(0.5, 8);

  const Spectrum data_numu_rhc_syst = predInterpFDNumuRHC.Predict(trueOsc).FakeData(pot_fd);
  SingleSampleExperiment dis_expt_rhc_syst(&predInterpFDNumuRHC, data_numu_rhc_syst);
  dis_expt_rhc_syst.SetMaskHist(0.5, 8);
  
  const Spectrum nd_data_numu_fhc_syst = predInterpNDNumuFHC.PredictUnoscillated().FakeData(pot_nd);
  SingleSampleExperiment nd_expt_fhc_syst(&predInterpNDNumuFHC, nd_data_numu_fhc_syst);
  nd_expt_fhc_syst.SetMaskHist(0.5, 10, 0, -1);
  
  const Spectrum nd_data_numu_rhc_syst = predInterpNDNumuRHC.PredictUnoscillated().FakeData(pot_nd);
  SingleSampleExperiment nd_expt_rhc_syst(&predInterpNDNumuRHC, nd_data_numu_rhc_syst);
  nd_expt_rhc_syst.SetMaskHist(0.5, 10, 0, -1);

  MultiExperiment expt_nd_fd({&app_expt_fhc_syst, &app_expt_rhc_syst,
	&dis_expt_fhc_syst, &dis_expt_rhc_syst,
   	&nd_expt_fhc_syst, &nd_expt_rhc_syst});
  
  MultiExperiment expt_fd({&app_expt_fhc_syst, &app_expt_rhc_syst,
	&dis_expt_fhc_syst, &dis_expt_rhc_syst});
  
  std::vector<std::pair<std::string, IExperiment*> >myExpts = {
    {"FD_FHC_nue", &app_expt_fhc_syst},
    {"FD_RHC_nue", &app_expt_rhc_syst},
    {"FD_FHC_numu", &dis_expt_fhc_syst},
    {"FD_RHC_numu", &dis_expt_rhc_syst},
    {"ND_FHC", &nd_expt_fhc_syst},
    {"ND_RHC", &nd_expt_rhc_syst},
    {"FD_only", &expt_fd},
    {"ND_FD", &expt_nd_fd} };
  
  // Make a list of dial values to scan over, common to all...
  std::vector<double> syst_vals;
  for (int i=0; i<201;++i){
    double range = 10;
    double stride = range/200;
    syst_vals .push_back(stride*i-5);
  }
  
  // Loop over the systematics and make a LLH scan for each one
  for (auto & syst : systlist){

    std::cout << "Making LLH scans for " << syst->ShortName() << ":" << std::endl;

    for (auto & expt : myExpts){
      std::cout << "\t For expt. " << expt.first << std::endl;
      std::vector<double> this_llh_total;
      std::vector<double> this_llh_chisq;
      std::vector<double> this_llh_penalty;      
      
      // Make a vector of chisq values for each var
      for (auto & shift : syst_vals){
	SystShifts thisShift(syst, shift);
	this_llh_total  .push_back(expt.second->ChiSq(trueOsc, thisShift)+syst->Penalty(shift));
	this_llh_chisq  .push_back(expt.second->ChiSq(trueOsc, thisShift));
	this_llh_penalty.push_back(syst->Penalty(shift));	
      }
      TGraph *thisTotal = new TGraph(201, &syst_vals[0], &this_llh_total[0]);
      thisTotal ->Write((expt.first + "_" +syst->ShortName()+"_total").c_str());
      delete thisTotal;
      
      TGraph *thisChisq = new TGraph(201, &syst_vals[0], &this_llh_chisq[0]);
      thisChisq ->Write((expt.first + "_" +syst->ShortName()+"_chisq").c_str());
      delete thisChisq;

      TGraph *thisPenalty = new TGraph(201, &syst_vals[0], &this_llh_penalty[0]);
      thisPenalty ->Write((expt.first + "_" +syst->ShortName()+"_penalty").c_str());
      delete thisPenalty;
    }
  }
  
  fout->Close();
}
