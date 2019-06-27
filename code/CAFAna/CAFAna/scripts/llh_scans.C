#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

char const *def_stateFname = "common_state_mcc11v3.root";
char const *def_outputFname = "llh_scans_mcc11v3.root";
char const *def_start_point = "";

void llh_scans(std::string stateFname = def_stateFname,
               std::string outputFname = def_outputFname,
               std::string start_point = def_start_point) {

  gROOT->SetBatch(1);

  // Get the prediction interpolators
  std::vector<std::unique_ptr<PredictionInterp>> return_list =
      GetPredictionInterps(stateFname, GetListOfSysts());
  PredictionInterp &predInterpFDNumuFHC = *return_list[0].release();
  PredictionInterp &predInterpFDNueFHC = *return_list[1].release();
  PredictionInterp &predInterpFDNumuRHC = *return_list[2].release();
  PredictionInterp &predInterpFDNueRHC = *return_list[3].release();
  PredictionInterp &predInterpNDNumuFHC = *return_list[4].release();
  PredictionInterp &predInterpNDNumuRHC = *return_list[5].release();

  // Get the systematics that the PredictionInterps use
  std::vector<const ISyst *> systlist =
      OrderListOfSysts(predInterpFDNumuFHC.GetAllSysts());

  std::map<std::string, double> relevantcvs;
  std::map<std::string, double> relevantfakedatavals;
  std::map<std::string, double> relevantBestFit;
  if (start_point.size()) {
    auto splits = SplitString(start_point, ':');
    if (splits.size() != 2) {
      std::cout
          << "[ERROR]: Expected input_file.root:<entry_number> but found: \""
          << start_point << "\"" << std::endl;
      abort();
    }
    int entnum = atoi(splits.back().c_str());
    std::cout << "[INFO]: Reading config from " << splits.front() << std::endl;

    TFile fin(splits.front().c_str(), "READ");
    TTree *tin;
    fin.GetObject("fit_info", tin);
    TTree *min;
    fin.GetObject("param_info", min);

    FitTreeBlob *fbr = FitTreeBlob::MakeReader(tin, min);

    std::cout << "Starting from: " << std::endl;
    tin->GetEntry(entnum);
    min->GetEntry(0);

    for (size_t i = 0; i < fbr->fParamNames->size(); ++i) {
      std::cout << "\t" << fbr->fParamNames->at(i)
                << " Prefit: {v: " << fbr->fPreFitValues->at(i)
                << ", pcv: " << fbr->fCentralValues->at(i)
                << ", e: " << fbr->fPreFitErrors->at(i)
                << " }, Postfit: {v: " << fbr->fPostFitValues->at(i)
                << ", e: " << fbr->fPostFitErrors->at(i) << "}" << std::endl;
      relevantcvs[fbr->fParamNames->at(i)] = fbr->fCentralValues->at(i);
      relevantfakedatavals[fbr->fParamNames->at(i)] = fbr->fFakeDataVals->at(i);
      relevantBestFit[fbr->fParamNames->at(i)] = fbr->fPostFitValues->at(i);
    }
  }

  SystShifts shifted = kNoShift;

  for (auto s : systlist) {
    if (relevantcvs.count(s->ShortName())) {
      std::cout << "Setting parameter " << s->ShortName()
                << " penalty CV: " << relevantcvs[s->ShortName()] << std::endl;
      s->SetCentral(relevantcvs[s->ShortName()]);
    }
    if (relevantfakedatavals.count(s->ShortName())) {
      std::cout << "Setting parameter true value " << s->ShortName()
                << " to: " << relevantfakedatavals[s->ShortName()] << std::endl;
      shifted.SetShift(s, relevantfakedatavals[s->ShortName()]);
    }
  }

  int hie = 1;
  const std::string hieStr = (hie > 0) ? "nh" : "ih";
  bool oscvar = true;

  osc::IOscCalculatorAdjustable *trueOsc = NuFitOscCalc(hie);

  for (auto v : GetOscVars("alloscvars", hie)) {
    if (relevantfakedatavals.count(v->ShortName())) {
      std::cout << "Setting parameter true value " << v->ShortName()
                << " to: " << relevantfakedatavals[v->ShortName()] << std::endl;
      v->SetValue(trueOsc, relevantfakedatavals[v->ShortName()]);
    }
  }

  // Open
  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");
  SaveParams(fout, systlist);

  const Spectrum data_nue_fhc_syst =
      predInterpFDNueFHC.PredictSyst(trueOsc, shifted).FakeData(pot_fd);
  SingleSampleExperiment app_expt_fhc_syst(&predInterpFDNueFHC,
                                           data_nue_fhc_syst);
  app_expt_fhc_syst.SetMaskHist(0.5, 8);

  const Spectrum data_nue_rhc_syst =
      predInterpFDNueRHC.PredictSyst(trueOsc, shifted).FakeData(pot_fd);
  SingleSampleExperiment app_expt_rhc_syst(&predInterpFDNueRHC,
                                           data_nue_rhc_syst);
  app_expt_rhc_syst.SetMaskHist(0.5, 8);

  const Spectrum data_numu_fhc_syst =
      predInterpFDNumuFHC.PredictSyst(trueOsc, shifted).FakeData(pot_fd);
  SingleSampleExperiment dis_expt_fhc_syst(&predInterpFDNumuFHC,
                                           data_numu_fhc_syst);
  dis_expt_fhc_syst.SetMaskHist(0.5, 8);

  const Spectrum data_numu_rhc_syst =
      predInterpFDNumuRHC.PredictSyst(trueOsc, shifted).FakeData(pot_fd);
  SingleSampleExperiment dis_expt_rhc_syst(&predInterpFDNumuRHC,
                                           data_numu_rhc_syst);
  dis_expt_rhc_syst.SetMaskHist(0.5, 8);

  const Spectrum nd_data_numu_fhc_syst =
      predInterpNDNumuFHC.PredictSyst(trueOsc, shifted).FakeData(pot_nd);
  SingleSampleExperiment nd_expt_fhc_syst(&predInterpNDNumuFHC,
                                          nd_data_numu_fhc_syst);
  nd_expt_fhc_syst.SetMaskHist(0.5, 10, 0, -1);

  const Spectrum nd_data_numu_rhc_syst =
      predInterpNDNumuRHC.PredictSyst(trueOsc, shifted).FakeData(pot_nd);
  SingleSampleExperiment nd_expt_rhc_syst(&predInterpNDNumuRHC,
                                          nd_data_numu_rhc_syst);
  nd_expt_rhc_syst.SetMaskHist(0.5, 10, 0, -1);

  // Add covariances to the two ND samples
  std::string covFileName = FindCAFAnaDir() + "/Systs/det_sys_cov.root";
  
  MultiExperiment expt_ND_FHC({&nd_expt_fhc_syst});
  expt_ND_FHC .AddCovarianceMatrix(covFileName, "nd_fhc_frac_cov", true, {0});
  
  MultiExperiment expt_ND_RHC({&nd_expt_rhc_syst});
  expt_ND_RHC .AddCovarianceMatrix(covFileName, "nd_rhc_frac_cov", true, {0});
				   
  MultiExperiment expt_nd_fd({&app_expt_fhc_syst, &app_expt_rhc_syst,
                              &dis_expt_fhc_syst, &dis_expt_rhc_syst,
                              &nd_expt_fhc_syst, &nd_expt_rhc_syst});

  expt_nd_fd.AddCovarianceMatrix(covFileName, "nd_all_frac_cov", true, {4, 5});

  MultiExperiment expt_fd({&app_expt_fhc_syst, &app_expt_rhc_syst,
                           &dis_expt_fhc_syst, &dis_expt_rhc_syst});

  std::vector<std::pair<std::string, IExperiment *>> myExpts = {
      {"FD_FHC_nue", &app_expt_fhc_syst},
      {"FD_RHC_nue", &app_expt_rhc_syst},
      {"FD_FHC_numu", &dis_expt_fhc_syst},
      {"FD_RHC_numu", &dis_expt_rhc_syst},
      {"ND_FHC", &expt_ND_FHC},
      {"ND_RHC", &expt_ND_RHC},
      {"ND_FHC_nocov", &nd_expt_fhc_syst},
      {"ND_RHC_nocov", &nd_expt_rhc_syst},
      {"FD_only", &expt_fd},
      {"ND_FD", &expt_nd_fd}};

  // Loop over the systematics and make a LLH scan for each one
  double range = 8;
  double half_range = range / 2.0;
  int nsteps = 401;
  double stride = range / double(nsteps - 1);
  for (auto &syst : systlist) {

    std::vector<double> syst_vals;

    double min = (relevantBestFit.count(syst->ShortName())
                      ? relevantBestFit[syst->ShortName()]
                      : 0) -
                 half_range;
    for (int i = 0; i < nsteps; ++i) {
      syst_vals.push_back(min + stride * i);
    }

    std::cout << "Making LLH scans for " << syst->ShortName() << ":"
              << std::endl;

    for (auto &expt : myExpts) {
      std::cout << "\t For expt. " << expt.first << std::endl;
      std::vector<double> this_llh_total;
      std::vector<double> this_llh_chisq;
      std::vector<double> this_llh_penalty;

      // Make a vector of chisq values for each var
      for (auto &shift : syst_vals) {
        SystShifts thisShift(syst, shift);
        this_llh_total.push_back(expt.second->ChiSq(trueOsc, thisShift) +
                                 syst->Penalty(shift));
        this_llh_chisq.push_back(expt.second->ChiSq(trueOsc, thisShift));
        this_llh_penalty.push_back(syst->Penalty(shift));
      }
      TGraph *thisTotal = new TGraph(nsteps, &syst_vals[0], &this_llh_total[0]);
      thisTotal->Write(
          (expt.first + "_" + syst->ShortName() + "_total").c_str());
      delete thisTotal;

      TGraph *thisChisq = new TGraph(nsteps, &syst_vals[0], &this_llh_chisq[0]);
      thisChisq->Write(
          (expt.first + "_" + syst->ShortName() + "_chisq").c_str());
      delete thisChisq;

      TGraph *thisPenalty =
          new TGraph(nsteps, &syst_vals[0], &this_llh_penalty[0]);
      thisPenalty->Write(
          (expt.first + "_" + syst->ShortName() + "_penalty").c_str());
      delete thisPenalty;
    }
  }
  std::cout << "Closing file" << std::endl;
  fout->Close();
}

#ifndef __CINT__
int main(int argc, char const *argv[]) {
  gROOT->SetMustClean(false);
  std::string stateFname = (argc > 1) ? argv[1] : def_stateFname;
  std::string outputFname = (argc > 2) ? argv[2] : def_outputFname;
  std::string start_point = (argc > 3) ? argv[3] : def_start_point;
  llh_scans(stateFname, outputFname, start_point);
}
#endif
