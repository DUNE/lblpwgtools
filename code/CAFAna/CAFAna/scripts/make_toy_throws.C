#ifdef __CINT__
void make_toy_throws(int nthrows=100, bool reload = false){}
#else

#include "common_fit_definitions.C"

std::string stateFname  = "common_state_ndfd_xsecflux_mcc11.root";
std::string outputFname = "throws_allsyst.root";

// Which systematics to throw and fit
bool detsyst  = true;
bool fluxsyst = true;
bool xsecsyst = true;
bool use_nd   = true;


// Need to accept filename, ND/FD, systs and reload as arguments
void make_toy_throws(int nthrows=100, bool reload = false){
  
  gROOT->SetBatch(1);

  // Have to load with the same systematics I generated to common state with
  std::vector<const ISyst*> systlist_forload = GetListOfSysts(true, true, true, true, true);

  // Make a list of systematics I'm actually going to use here (controlled above)
  std::vector<const ISyst*> systlist_ndfd = GetListOfSysts(fluxsyst, xsecsyst, detsyst, true, true);

  // OA parameters (chuck in rho for shits and giggles)
  std::vector<const IFitVar*> oscVars = {&kFitDmSq32Scaled, &kFitSinSqTheta23, &kFitTheta13,
					 &kFitDeltaInPiUnits, &kFitSinSq2Theta12, &kFitDmSq21,
					 &kFitRho};
  
  std::vector<unique_ptr<PredictionInterp> > return_list_nd = GetPredictionInterps(stateFname, systlist_forload);
  PredictionInterp& predFDNumuFHC = *return_list_nd[0].release();
  PredictionInterp& predFDNueFHC  = *return_list_nd[1].release();
  PredictionInterp& predFDNumuRHC = *return_list_nd[2].release();
  PredictionInterp& predFDNueRHC  = *return_list_nd[3].release();
  PredictionInterp& predNDNumuFHC = *return_list_nd[4].release();
  PredictionInterp& predNDNumuRHC = *return_list_nd[5].release();
  
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();
  int hie = 1;
  const std::string hieStr = (hie > 0) ? "nh" : "ih";  

  // What's the difference?
  osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie);

  // Throw stats and systematics before making fake data
  // Should have an option whether I want to turn off the penalty or not for OA and nuisance systematics
  osc::NoOscillations noOsc;

  // SystShifts throwSyst = kNoShift;
  
  for (int i=0; i<nthrows;++i){

    std::cout << "Starting throw " << i << std::endl;
    TDirectory* subDir = (TDirectory*) fout->mkdir(("throw_"+std::to_string(i)).c_str());
    subDir ->cd();

    // Do the throwing for fake data
    osc::IOscCalculatorAdjustable* fakeThrowOsc = ThrownNuFitOscCalc(hie);    
    SystShifts fakeThrowSyst;
    for(auto s: systlist_ndfd) fakeThrowSyst.SetShift(s, gRandom->Gaus());
    
    const Spectrum data_nue_fhc = predFDNueFHC.PredictSyst(fakeThrowOsc, fakeThrowSyst).MockData(pot_fd);
    SingleSampleExperiment app_expt_fhc(&predFDNueFHC, data_nue_fhc);
    app_expt_fhc.SetMaskHist(0.5, 8);
    TH1* data_nue_fhc_hist = data_nue_fhc.ToTHX(pot_fd);   // Wait, POT twice? EHH?
    data_nue_fhc_hist ->SetName("data_fd_nue_fhc");
    data_nue_fhc_hist ->Write();
    
    const Spectrum data_nue_rhc = predFDNueRHC.PredictSyst(fakeThrowOsc, fakeThrowSyst).MockData(pot_fd);
    SingleSampleExperiment app_expt_rhc(&predFDNueRHC, data_nue_rhc);
    app_expt_rhc.SetMaskHist(0.5, 8);
    TH1* data_nue_rhc_hist = data_nue_rhc.ToTHX(pot_fd);
    data_nue_rhc_hist ->SetName("data_fd_nue_rhc");
    data_nue_rhc_hist ->Write();
   
    const Spectrum data_numu_fhc = predFDNumuFHC.PredictSyst(fakeThrowOsc, fakeThrowSyst).MockData(pot_fd);
    SingleSampleExperiment dis_expt_fhc(&predFDNumuFHC, data_numu_fhc);
    dis_expt_fhc.SetMaskHist(0.5, 8);
    TH1* data_numu_fhc_hist = data_numu_fhc.ToTHX(pot_fd);
    data_numu_fhc_hist ->SetName("data_fd_numu_fhc");
    data_numu_fhc_hist ->Write();

    const Spectrum data_numu_rhc = predFDNumuRHC.PredictSyst(fakeThrowOsc, fakeThrowSyst).MockData(pot_fd);
    SingleSampleExperiment dis_expt_rhc(&predFDNumuRHC, data_numu_rhc);
    dis_expt_rhc.SetMaskHist(0.5, 8);
    TH1* data_numu_rhc_hist = data_numu_rhc.ToTHX(pot_fd);
    data_numu_rhc_hist ->SetName("data_fd_numu_rhc");
    data_numu_rhc_hist ->Write();

    // I think there's an issue in MockData... seems 1D specific
    const Spectrum nd_data_numu_fhc = predNDNumuFHC.PredictSyst(fakeThrowOsc, fakeThrowSyst).MockData(pot_nd);
    SingleSampleExperiment nd_expt_fhc(&predNDNumuFHC, nd_data_numu_fhc);
    nd_expt_fhc.SetMaskHist(0, -1, 0.1, 1);
    TH1* nd_data_numu_fhc_hist = nd_data_numu_fhc.ToTHX(pot_nd);
    TH1* nd_data_numu_fhc_hist_1D = nd_data_numu_fhc.ToTH1(pot_nd);
    nd_data_numu_fhc_hist ->SetName("data_nd_numu_fhc");
    nd_data_numu_fhc_hist_1D ->SetName("data_nd_numu_fhc_1D");
    nd_data_numu_fhc_hist ->Write();
    nd_data_numu_fhc_hist_1D ->Write();

    const Spectrum nd_data_numu_rhc = predNDNumuRHC.PredictSyst(fakeThrowOsc, fakeThrowSyst).MockData(pot_nd);
    SingleSampleExperiment nd_expt_rhc(&predNDNumuRHC, nd_data_numu_rhc);
    nd_expt_rhc.SetMaskHist(0, -1, 0.1, 1);
    TH1* nd_data_numu_rhc_hist = nd_data_numu_rhc.ToTHX(pot_nd);
    TH1* nd_data_numu_rhc_hist_1D = nd_data_numu_rhc.ToTH1(pot_nd);
    nd_data_numu_rhc_hist ->SetName("data_nd_numu_rhc");
    nd_data_numu_rhc_hist_1D ->SetName("data_nd_numu_rhc_1D");
    nd_data_numu_rhc_hist ->Write();
    nd_data_numu_rhc_hist_1D ->Write();
    
    // Setup thrown seeds
    SystShifts fitThrowSyst;
    for(auto s: systlist_ndfd) fitThrowSyst.SetShift(s, gRandom->Gaus());
    osc::IOscCalculatorAdjustable* fitThrowOsc = ThrownNuFitOscCalc(hie);

    // Save prefit starting distributions
    TH1* pre_fd_nue_fhc = GetMCSystTotal(&predFDNueFHC, fitThrowOsc, fitThrowSyst, "prefit_fd_nue_fhc", pot_fd);
    pre_fd_nue_fhc   ->Write();
    TH1* pre_fd_nue_rhc = GetMCSystTotal(&predFDNueRHC, fitThrowOsc, fitThrowSyst, "prefit_fd_nue_rhc", pot_fd);
    pre_fd_nue_rhc   ->Write();
    TH1* pre_fd_numu_fhc = GetMCSystTotal(&predFDNumuFHC, fitThrowOsc, fitThrowSyst, "prefit_fd_numu_fhc", pot_fd);
    pre_fd_numu_fhc   ->Write();
    TH1* pre_fd_numu_rhc = GetMCSystTotal(&predFDNumuRHC, fitThrowOsc, fitThrowSyst, "prefit_fd_numu_rhc", pot_fd);
    pre_fd_numu_rhc   ->Write();
    TH1* pre_nd_numu_fhc = GetMCSystTotal(&predNDNumuFHC, fitThrowOsc, fitThrowSyst, "prefit_nd_numu_fhc", pot_nd);
    pre_nd_numu_fhc   ->Write();
    TH1* pre_nd_numu_fhc_1D = GetMCSystTotal(&predNDNumuFHC, fitThrowOsc, fitThrowSyst, "prefit_nd_numu_fhc_1D", pot_nd, true);
    pre_nd_numu_fhc_1D   ->Write();
    TH1* pre_nd_numu_rhc = GetMCSystTotal(&predNDNumuRHC, fitThrowOsc, fitThrowSyst, "prefit_nd_numu_rhc", pot_nd);
    pre_nd_numu_rhc   ->Write();
    TH1* pre_nd_numu_rhc_1D = GetMCSystTotal(&predNDNumuRHC, fitThrowOsc, fitThrowSyst, "prefit_nd_numu_rhc_1D", pot_nd, true);
    pre_nd_numu_rhc_1D   ->Write();
    
    // Currently, put constraints only on 12 sector and rho
    // Should probably also try with constraints on both
    // This needs to use the same oscillation calculator that the fit itself uses, so seed with the thrown values
    Penalizer_GlbLike penalty(fitThrowOsc,hie, true);
    
    // Combine into a thrown multi-experiment and setup the fitter
    MultiExperiment expt_ndfd_pen({&app_expt_fhc, &app_expt_rhc, &dis_expt_fhc,
	  &dis_expt_rhc, &nd_expt_fhc, &nd_expt_rhc, &penalty}); 
    Fitter this_fit(&expt_ndfd_pen, oscVars, systlist_ndfd, Fitter::kCareful|Fitter::kIncludeSimplex);

    // Now do a fit with thrown seeds
    double thischisq = this_fit.Fit(fitThrowOsc, fitThrowSyst, Fitter::kVerbose);
    std::cout << "Throw " << i << ": found minimum chi2 = " << thischisq << std::endl;
    
    // Information to keep
    TMatrixDSym* covar = (TMatrixDSym*)this_fit.GetCovariance();
    std::vector<std::string> fParamNames = this_fit.GetParamNames();
    std::vector<double> fPreFitValues  = this_fit.GetPreFitValues();
    std::vector<double> fPreFitErrors  = this_fit.GetPreFitErrors();
    std::vector<double> fPostFitValues = this_fit.GetPostFitValues();
    std::vector<double> fPostFitErrors = this_fit.GetPostFitErrors();   
    
    // TH2D hist_covar = TH2D(*covar);
    // hist_covar .SetNameTitle("covar", "covar");
    // TH2D hist_corr = make_corr_from_covar(hist_covar);
    
    // Also get the information about experiments in the fit
    // Need to save data and best fit histograms... also pre-fit hists (for throws?)
    // Now save the postfit distributions

    // Could save a breakdown... but I actually want to look at the files for now, so I'm not going to bother
    TH1* post_fd_nue_fhc = GetMCSystTotal(&predFDNueFHC, fitThrowOsc, fitThrowSyst, "postfit_fd_nue_fhc", pot_fd);
    post_fd_nue_fhc   ->Write();
    TH1* post_fd_nue_rhc = GetMCSystTotal(&predFDNueRHC, fitThrowOsc, fitThrowSyst, "postfit_fd_nue_rhc", pot_fd);
    post_fd_nue_rhc   ->Write();
    TH1* post_fd_numu_fhc = GetMCSystTotal(&predFDNumuFHC, fitThrowOsc, fitThrowSyst, "postfit_fd_numu_fhc", pot_fd);
    post_fd_numu_fhc   ->Write();
    TH1* post_fd_numu_rhc = GetMCSystTotal(&predFDNumuRHC, fitThrowOsc, fitThrowSyst, "postfit_fd_numu_rhc", pot_fd);
    post_fd_numu_rhc   ->Write();
    TH1* post_nd_numu_fhc = GetMCSystTotal(&predNDNumuFHC, fitThrowOsc, fitThrowSyst, "postfit_nd_numu_fhc", pot_nd);
    post_nd_numu_fhc   ->Write();
    TH1* post_nd_numu_fhc_1D = GetMCSystTotal(&predNDNumuFHC, fitThrowOsc, fitThrowSyst, "postfit_nd_numu_fhc_1D", pot_nd, true);
    post_nd_numu_fhc_1D   ->Write();
    TH1* post_nd_numu_rhc = GetMCSystTotal(&predNDNumuRHC, fitThrowOsc, fitThrowSyst, "postfit_nd_numu_rhc", pot_nd);
    post_nd_numu_rhc   ->Write();
    TH1* post_nd_numu_rhc_1D = GetMCSystTotal(&predNDNumuRHC, fitThrowOsc, fitThrowSyst, "postfit_nd_numu_rhc_1D", pot_nd, true);
    post_nd_numu_rhc_1D   ->Write();

    // Save information
    TTree *t = new TTree("fit_info", "fit_info");
    t->Branch("chisq", &thischisq);
    t->Branch("fParamNames",&fParamNames);
    t->Branch("fPreFitValues",&fPreFitValues);
    t->Branch("fPreFitErrors",&fPreFitErrors);
    t->Branch("fPostFitValues",&fPostFitValues);
    t->Branch("fPostFitErrors",&fPostFitErrors);
    t->Fill();
    t->Write();
    // hist_covar.Write();
    // hist_corr.Write();
    fout->cd();
    subDir->Write();
    delete t;
    
    // Done with this systematic throw
  }

  // Now close the file
  fout->Close();
}

#endif
