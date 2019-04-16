#include "common_fit_definitions.C"

void dump_spectra(std::string stateFname = "common_state_mcc11v3.root",
                  std::string outputFname = "covar_various_asimov.root",
                  std::string sampleString = "ndfd",
                  std::string FakeDataSysts = "", double FakeDataValue = 1) {

  int hie = 1;

  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts("allsyst");

  // Oscillation parameters to use
  std::vector<const IFitVar *> oscVars = GetOscVars("alloscvars", hie);

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");

  osc::IOscCalculatorAdjustable *trueOsc = NuFitOscCalc(hie);

  SystShifts trueSyst = kNoShift;
  SystShifts mcSyst = kNoShift;

  // Can build fake data fits easily by passing syst parameter names to fix to 1
  // and then remove from the fit.
  if (FakeDataSysts.size()) {
    // Get all of the systs (notably including missing proton energy fake data)
    std::vector<const ISyst *> all_systs = GetListOfSysts();
    for (ISyst const *s : all_systs) {
      if (FakeDataSysts.find(s->ShortName()) != std::string::npos) {
        trueSyst.SetShift(s, FakeDataValue);
        auto it = std::find(systlist.begin(), systlist.end(), s);
        if (it != systlist.end()) {
          systlist.erase(it);
        }
      }
    }
  }

  static std::vector<std::unique_ptr<PredictionInterp>> interp_list =
      GetPredictionInterps(stateFname, GetListOfSysts());
  static PredictionInterp &predFDNumuFHC = *interp_list[0].release();
  static PredictionInterp &predFDNueFHC = *interp_list[1].release();
  static PredictionInterp &predFDNumuRHC = *interp_list[2].release();
  static PredictionInterp &predFDNueRHC = *interp_list[3].release();
  static PredictionInterp &predNDNumuFHC = *interp_list[4].release();
  static PredictionInterp &predNDNumuRHC = *interp_list[5].release();

  double pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue, pot_fd_rhc_nue,
      pot_fd_fhc_numu, pot_fd_rhc_numu, additional_smear;
  ParseDataSamples(sampleString, pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue,
                   pot_fd_rhc_nue, pot_fd_fhc_numu, pot_fd_rhc_numu,
                   additional_smear);

  const Spectrum data_nue_fhc = predFDNueFHC.PredictSyst(trueOsc, trueSyst)
                                    .MockData(pot_fd_fhc_nue, false);
  SingleSampleExperiment app_expt_fhc(&predFDNueFHC, data_nue_fhc);
  app_expt_fhc.SetMaskHist(0.5, 8);

  const Spectrum data_numu_fhc = predFDNumuFHC.PredictSyst(trueOsc, trueSyst)
                                     .MockData(pot_fd_fhc_numu, false);
  SingleSampleExperiment dis_expt_fhc(&predFDNumuFHC, data_numu_fhc);
  dis_expt_fhc.SetMaskHist(0.5, 8);

  const Spectrum data_nue_rhc = predFDNueRHC.PredictSyst(trueOsc, trueSyst)
                                    .MockData(pot_fd_rhc_nue, false);
  SingleSampleExperiment app_expt_rhc(&predFDNueRHC, data_nue_rhc);
  app_expt_rhc.SetMaskHist(0.5, 8);

  const Spectrum data_numu_rhc = predFDNumuRHC.PredictSyst(trueOsc, trueSyst)
                                     .MockData(pot_fd_rhc_numu, false);
  SingleSampleExperiment dis_expt_rhc(&predFDNumuRHC, data_numu_rhc);
  dis_expt_rhc.SetMaskHist(0.5, 8);

  const Spectrum nd_data_numu_fhc =
      predNDNumuFHC.PredictSyst(trueOsc, trueSyst).MockData(pot_nd_fhc, false);
  SingleSampleExperiment nd_expt_fhc(&predNDNumuFHC, nd_data_numu_fhc);
  nd_expt_fhc.SetMaskHist(0.5, 10, 0, -1);

  const Spectrum nd_data_numu_rhc =
      predNDNumuRHC.PredictSyst(trueOsc, trueSyst).MockData(pot_nd_rhc, false);
  SingleSampleExperiment nd_expt_rhc(&predNDNumuRHC, nd_data_numu_rhc);
  nd_expt_rhc.SetMaskHist(0.5, 10, 0, -1);

  fout->cd();
  if (pot_fd_fhc_nue > 0) {
    TH1 *data_nue_fhc_hist = data_nue_fhc.ToTHX(pot_fd_fhc_nue);
    data_nue_fhc_hist->SetName("data_fd_nue_fhc");
    data_nue_fhc_hist->Write();
    TH1 *pre_fd_nue_fhc = GetMCSystTotal(&predFDNueFHC, trueOsc, mcSyst,
                                         "prefit_fd_nue_fhc", pot_fd_fhc_nue);
    pre_fd_nue_fhc->SetTitle(
        std::to_string(app_expt_fhc.ChiSq(trueOsc, mcSyst)).c_str());
    pre_fd_nue_fhc->Write();
  }
  if (pot_fd_fhc_numu > 0) {
    TH1 *data_numu_fhc_hist = data_numu_fhc.ToTHX(pot_fd_fhc_numu);
    data_numu_fhc_hist->SetName("data_fd_numu_fhc");
    data_numu_fhc_hist->Write();
    TH1 *pre_fd_numu_fhc = GetMCSystTotal(
        &predFDNumuFHC, trueOsc, mcSyst, "prefit_fd_numu_fhc", pot_fd_fhc_numu);
    pre_fd_numu_fhc->SetTitle(
        std::to_string(dis_expt_fhc.ChiSq(trueOsc, mcSyst)).c_str());
    pre_fd_numu_fhc->Write();
  }
  if (pot_fd_rhc_nue > 0) {
    TH1 *data_nue_rhc_hist = data_nue_rhc.ToTHX(pot_fd_rhc_nue);
    data_nue_rhc_hist->SetName("data_fd_nue_rhc");
    data_nue_rhc_hist->Write();
    TH1 *pre_fd_nue_rhc = GetMCSystTotal(&predFDNueRHC, trueOsc, mcSyst,
                                         "prefit_fd_nue_rhc", pot_fd_rhc_nue);
    pre_fd_nue_rhc->SetTitle(
        std::to_string(app_expt_rhc.ChiSq(trueOsc, mcSyst)).c_str());
    pre_fd_nue_rhc->Write();
  }
  if (pot_fd_rhc_numu > 0) {
    TH1 *data_numu_rhc_hist = data_numu_rhc.ToTHX(pot_fd_rhc_numu);
    data_numu_rhc_hist->SetName("data_fd_numu_rhc");
    data_numu_rhc_hist->Write();
    TH1 *pre_fd_numu_rhc = GetMCSystTotal(
        &predFDNumuRHC, trueOsc, mcSyst, "prefit_fd_numu_rhc", pot_fd_rhc_numu);
    pre_fd_numu_rhc->SetTitle(
        std::to_string(dis_expt_rhc.ChiSq(trueOsc, mcSyst)).c_str());
    pre_fd_numu_rhc->Write();
  }
  if (pot_nd_fhc > 0) {
    TH1 *nd_data_numu_fhc_hist = nd_data_numu_fhc.ToTHX(pot_nd_fhc);
    TH1 *nd_data_numu_fhc_hist_1D = nd_data_numu_fhc.ToTH1(pot_nd_fhc);
    nd_data_numu_fhc_hist->SetName("data_nd_numu_fhc");
    nd_data_numu_fhc_hist_1D->SetName("data_nd_numu_fhc_1D");
    TH1 *nd_data_numu_fhc_hist_px = static_cast<TH2 *>(nd_data_numu_fhc_hist)
                                        ->ProjectionX("data_nd_numu_fhc_px");
    nd_data_numu_fhc_hist->Write();
    nd_data_numu_fhc_hist_1D->Write();
    nd_data_numu_fhc_hist_px->Write();

    TH1 *pre_nd_numu_fhc = GetMCSystTotal(&predNDNumuFHC, trueOsc, mcSyst,
                                          "prefit_nd_numu_fhc", pot_nd_fhc);
    TH1 *pre_nd_numu_fhc_px = static_cast<TH2 *>(pre_nd_numu_fhc)
                                  ->ProjectionX("prefit_nd_numu_fhc_px");
    pre_nd_numu_fhc->SetTitle(
        std::to_string(nd_expt_fhc.ChiSq(trueOsc, mcSyst)).c_str());
    pre_nd_numu_fhc->Write();
    TH1 *pre_nd_numu_fhc_1D =
        GetMCSystTotal(&predNDNumuFHC, trueOsc, mcSyst, "prefit_nd_numu_fhc_1D",
                       pot_nd_fhc, true);
    pre_nd_numu_fhc_1D->SetTitle(
        std::to_string(nd_expt_fhc.ChiSq(trueOsc, mcSyst)).c_str());
    pre_nd_numu_fhc_1D->Write();
    pre_nd_numu_fhc_px->Write();
  }
  if (pot_nd_rhc) {
    TH1 *nd_data_numu_rhc_hist = nd_data_numu_rhc.ToTHX(pot_nd_rhc);
    TH1 *nd_data_numu_rhc_hist_1D = nd_data_numu_rhc.ToTH1(pot_nd_rhc);
    nd_data_numu_rhc_hist->SetName("data_nd_numu_rhc");
    nd_data_numu_rhc_hist_1D->SetName("data_nd_numu_rhc_1D");
    TH1 *nd_data_numu_rhc_hist_px = static_cast<TH2 *>(nd_data_numu_rhc_hist)
                                        ->ProjectionX("data_nd_numu_rhc_px");
    nd_data_numu_rhc_hist->Write();
    nd_data_numu_rhc_hist_1D->Write();
    nd_data_numu_rhc_hist_px->Write();

    TH1 *pre_nd_numu_rhc = GetMCSystTotal(&predNDNumuRHC, trueOsc, mcSyst,
                                          "prefit_nd_numu_rhc", pot_nd_rhc);
    TH1 *pre_nd_numu_rhc_px = static_cast<TH2 *>(pre_nd_numu_rhc)
                                  ->ProjectionX("prefit_nd_numu_rhc_px");

    pre_nd_numu_rhc->SetTitle(
        std::to_string(nd_expt_rhc.ChiSq(trueOsc, mcSyst)).c_str());
    pre_nd_numu_rhc->Write();
    TH1 *pre_nd_numu_rhc_1D =
        GetMCSystTotal(&predNDNumuRHC, trueOsc, mcSyst, "prefit_nd_numu_rhc_1D",
                       pot_nd_rhc, true);
    pre_nd_numu_rhc_1D->SetTitle(
        std::to_string(nd_expt_rhc.ChiSq(trueOsc, mcSyst)).c_str());
    pre_nd_numu_rhc_1D->Write();
    pre_nd_numu_rhc_px->Write();
  }

  // Now close the file
  fout->Close();
}
