#include "CAFAna/Analysis/common_fit_definitions.h"
#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/EnergySysts.h"
#include "CAFAna/Systs/FDRecoSysts.h"
#include "CAFAna/Systs/XSecSysts.h"

#include "TDecompChol.h"
#include "TVector.h"
#include <numeric>

using namespace ana;

int nthrows = 10000;

std::vector<std::string> samples = {"FD_nue_FHC",  "FD_numu_FHC", "FD_nue_RHC",
                                    "FD_numu_RHC", "ND_FHC",      "ND_RHC"};

void make_1D_errors(TDirectory *saveDir, std::vector<TH1 *> nominal,
                    std::vector<std::vector<TH1 *>> thrown_hists) {

  saveDir->cd();

  for (uint s = 0; s < samples.size(); ++s) {

    // Bit of a hack for now... use the nominal for the mean...
    TH1 *outHist = (TH1 *)nominal[s]->Clone((samples[s] + "_errs").c_str());
    outHist->SetNameTitle((samples[s] + "_errs").c_str(),
                          (samples[s] + "_errs").c_str());
    outHist->SetDirectory(saveDir);

    TH1D *total_hist =
        new TH1D((samples[s] + "_rate_ratio").c_str(),
                 (samples[s] + "_rate_ratio").c_str(), 100, 0.5, 1.5);
    double nom_int = nominal[s]->Integral();

    total_hist->SetDirectory(saveDir);
    total_hist->SetTitle(std::to_string(nom_int).c_str());

    for (uint i = 0; i < thrown_hists.size(); ++i)
      total_hist->Fill(thrown_hists[i][s]->Integral() / nom_int);

    // Loop over bins
    for (int x = 0; x < outHist->GetNbinsX(); ++x) {
      for (int y = 0; y < outHist->GetNbinsY(); ++y) {

        std::vector<double> values;

        for (uint i = 0; i < thrown_hists.size(); ++i)
          values.push_back(thrown_hists[i][s]->GetBinContent(x + 1, y + 1));

        // double sum = std::accumulate(values.begin(), values.end(), 0.0);
        // double mean = sum / values.size();

        double mean = outHist->GetBinContent(x + 1, y + 1);

        std::vector<double> diff(values.size());
        std::transform(values.begin(), values.end(), diff.begin(),
                       std::bind2nd(std::minus<double>(), mean));
        double sq_sum =
            std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
        double stdev = std::sqrt(sq_sum / values.size());

        TH1D theseThrows(
            (samples[s] + "_" + std::to_string(x) + "_" + std::to_string(y))
                .c_str(),
            (samples[s] + "_" + std::to_string(x) + "_" + std::to_string(y))
                .c_str(),
            1000, 0, mean * 4);
        for (auto v : values)
          theseThrows.Fill(v);

        // Make a histogram, and loop over throws
        outHist->SetBinContent(x + 1, y + 1, mean);
        outHist->SetBinError(x + 1, y + 1, stdev);
        theseThrows.Write();
      }
    }
    outHist->Write();
    total_hist->Write();
  }
  return;
}

std::vector<TH1 *> get_nominal_breakdown(std::string stateFname,
					 osc::IOscCalculatorAdjustable *trueOsc,
					 std::string sampleString){
  double pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue, pot_fd_rhc_nue,
    pot_fd_fhc_numu, pot_fd_rhc_numu;
  bool ndprefit;
  ParseDataSamples(sampleString, pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue,
                   pot_fd_rhc_nue, pot_fd_fhc_numu, pot_fd_rhc_numu, ndprefit);
  
  static std::vector<std::unique_ptr<PredictionInterp>> interp_list =
    GetPredictionInterps(stateFname, GetListOfSysts());
  static PredictionInterp &predFDNumuFHC = *interp_list[0].release();
  static PredictionInterp &predFDNueFHC = *interp_list[1].release();
  static PredictionInterp &predFDNumuRHC = *interp_list[2].release();
  static PredictionInterp &predFDNueRHC = *interp_list[3].release();
  static PredictionInterp &predNDNumuFHC = *interp_list[4].release();
  static PredictionInterp &predNDNumuRHC = *interp_list[5].release();

  std::vector<TH1 *> sample0 = GetMCComponents(&predFDNueFHC, trueOsc,
					       (samples[0] + "_nom").c_str(), pot_fd_fhc_nue);

  std::vector<TH1 *> sample1 = GetMCComponents(&predFDNumuFHC, trueOsc,
                                               (samples[1] + "_nom").c_str(), pot_fd_fhc_numu);

  std::vector<TH1 *> sample2 = GetMCComponents(&predFDNueRHC, trueOsc,
                                               (samples[2] + "_nom").c_str(), pot_fd_rhc_nue);

  std::vector<TH1 *> sample3 = GetMCComponents(&predFDNumuRHC, trueOsc,
                                               (samples[3] + "_nom").c_str(), pot_fd_rhc_numu);

  std::vector<TH1 *> sample4 = GetMCComponents(&predNDNumuFHC, trueOsc,
                                               (samples[4] + "_nom").c_str(), pot_nd_fhc);

  std::vector<TH1 *> sample5 = GetMCComponents(&predNDNumuRHC, trueOsc,
                                               (samples[5] + "_nom").c_str(), pot_nd_rhc);

  // Now to make one big vector
  std::vector<TH1 *> ret = sample0;

  ret.insert(ret.end(), sample1.begin(), sample1.end());
  ret.insert(ret.end(), sample2.begin(), sample2.end());
  ret.insert(ret.end(), sample3.begin(), sample3.end());
  ret.insert(ret.end(), sample4.begin(), sample4.end());
  ret.insert(ret.end(), sample5.begin(), sample5.end());
  return ret;
}


std::vector<TH1 *> make_syst_throw(std::string stateFname,
                                   osc::IOscCalculatorAdjustable *trueOsc,
                                   SystShifts theseSysts,
                                   std::string sampleString) {

  double pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue, pot_fd_rhc_nue,
      pot_fd_fhc_numu, pot_fd_rhc_numu;
  bool ndprefit;
  ParseDataSamples(sampleString, pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue,
                   pot_fd_rhc_nue, pot_fd_fhc_numu, pot_fd_rhc_numu, ndprefit);

  static std::vector<std::unique_ptr<PredictionInterp>> interp_list =
      GetPredictionInterps(stateFname, GetListOfSysts());
  static PredictionInterp &predFDNumuFHC = *interp_list[0].release();
  static PredictionInterp &predFDNueFHC = *interp_list[1].release();
  static PredictionInterp &predFDNumuRHC = *interp_list[2].release();
  static PredictionInterp &predFDNueRHC = *interp_list[3].release();
  static PredictionInterp &predNDNumuFHC = *interp_list[4].release();
  static PredictionInterp &predNDNumuRHC = *interp_list[5].release();

  static int haXX0r = 0;

  std::vector<TH1 *> ret;
  ret.push_back(GetMCSystTotal(&predFDNueFHC, trueOsc, theseSysts,
                               (samples[0] + std::to_string(haXX0r)).c_str(),
                               pot_fd_fhc_nue));
  ret.push_back(GetMCSystTotal(&predFDNumuFHC, trueOsc, theseSysts,
                               (samples[1] + std::to_string(haXX0r)).c_str(),
                               pot_fd_fhc_numu));
  ret.push_back(GetMCSystTotal(&predFDNueRHC, trueOsc, theseSysts,
                               (samples[2] + std::to_string(haXX0r)).c_str(),
                               pot_fd_rhc_nue));
  ret.push_back(GetMCSystTotal(&predFDNumuRHC, trueOsc, theseSysts,
                               (samples[3] + std::to_string(haXX0r)).c_str(),
                               pot_fd_rhc_numu));
  ret.push_back(GetMCSystTotal(&predNDNumuFHC, trueOsc, theseSysts,
                               (samples[4] + std::to_string(haXX0r)).c_str(),
                               pot_nd_fhc));
  ret.push_back(GetMCSystTotal(&predNDNumuRHC, trueOsc, theseSysts,
                               (samples[5] + std::to_string(haXX0r)).c_str(),
                               pot_nd_rhc));
  haXX0r += 1;

  return ret;
}

// A bit of a god function... just for fun
void throw_errors(std::string stateFname, std::string sampleString,
                  TDirectory *saveDir, std::vector<TH1 *> nom_vect,
                  std::vector<const ISyst *> systlist,
                  std::vector<std::string> systtokeep,
                  osc::IOscCalculatorAdjustable *trueOsc,
                  TMatrixDSym *chol = NULL) {

  std::vector<std::vector<TH1 *>> fit_vect;

  int nSysts = systlist.size();

  TVectorD onoffvect(nSysts);

  for (int i = 0; i < nSysts; ++i) {
    // Check to see if each syst should be kept
    if (std::find(systtokeep.begin(), systtokeep.end(),
                  systlist[i]->ShortName()) == systtokeep.end()) {
      onoffvect[i] = 0;
      std::cout << "   ---> Removing " << systlist[i]->ShortName() << std::endl;
    } else
      onoffvect[i] = 1;
  }

  for (int i = 0; i < nthrows; ++i) {

    TVectorD gaus_vect(nSysts);
    TVectorD throw_vect(nSysts);

    // Deal with the case with and without a matrix (e.g. a constraint)
    if (chol != NULL) {
      for (int i = 0; i < nSysts; ++i)
        gaus_vect[i] = gRandom->Gaus(0.0, 1.0);
      TVector temp_vect = (*chol) * gaus_vect;
      for (int i = 0; i < nSysts; ++i)
        throw_vect[i] = temp_vect[i] * onoffvect[i];
    } else {
      throw_vect = TVector(nSysts);
      for (int i = 0; i < nSysts; ++i)
        throw_vect[i] =
            GetBoundedGausThrow(systlist[i]->Min(), systlist[i]->Max()) *
            onoffvect[i];
    }

    // Make the shift set
    SystShifts throwSyst;
    for (int i = 0; i < nSysts; ++i) {
      throwSyst.SetShift(systlist[i], throw_vect[i]);
      // std::cout << "Setting " << systlist[i]->ShortName() << " to " <<
      // throw_vect[i] << std::endl;
    }

    fit_vect.push_back(
        make_syst_throw(stateFname, trueOsc, throwSyst, sampleString));
  }

  make_1D_errors(saveDir, nom_vect, fit_vect);
  for (auto s : fit_vect) {
    for (auto t : s)
      delete t;
  }
  fit_vect.clear();
  return;
}

char const *def_stateFname = "common_state_mcc11v3.root";
char const *def_outputFname = "throws_ndfd_nosyst.root";
char const *def_systSet = "nosyst";
char const *def_sampleString = "ndfd";
char const *def_penaltyString = "nopen";
char const *def_asimov_set = "0";

void sample_throws(std::string stateFname = def_stateFname,
                   std::string outputFname = def_outputFname,
                   std::string systSet = def_systSet,
                   std::string sampleString = def_sampleString,
                   std::string penaltyString = def_penaltyString,
                   std::string asimov_set = def_asimov_set) {

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // Start with an initial fit to get the covariance
  int hie = 1;

  bool useND = false;
  bool useFD = false;
  if (sampleString.find("nd") != std::string::npos)
    useND = true;
  if (sampleString.find("fd") != std::string::npos)
    useFD = true;

  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet, useND, useFD);
  std::vector<std::string> allsystnames;
  for (auto s : systlist)
    allsystnames.push_back(s->ShortName());

  // Oscillation parameters to use
  std::vector<const IFitVar *> oscVars = GetOscVars("alloscvars", hie);

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();

  osc::IOscCalculatorAdjustable *trueOsc = NuFitOscCalc(hie, 1, asimov_set);
  SystShifts trueSyst = kNoShift;

  // Move the input parameters a little, just to avoid asimov fit issues in
  // MINUIT
  osc::IOscCalculatorAdjustable *testOsc = NuFitOscCalc(hie, 1, asimov_set);
  SystShifts testSyst;
  for (auto s : systlist)
    testSyst.SetShift(s, GetBoundedGausThrow(s->Min() * 0.05, s->Max() * 0.05));
  std::map<const IFitVar *, std::vector<double>> oscSeeds = {};
  IExperiment *penalty = GetPenalty(hie, 1, penaltyString, asimov_set);

  // Now the good stuff
  double thischisq =
      RunFitPoint(stateFname, sampleString, trueOsc, trueSyst, false, oscVars,
                  systlist, testOsc, testSyst, oscSeeds, penalty,
                  Fitter::kNormal | Fitter::kIncludeHesse, fout);
  delete penalty;

  std::vector<TH1 *> breakdown_vect =  get_nominal_breakdown(stateFname, trueOsc, sampleString);

  std::vector<TH1 *> nom_vect =
      make_syst_throw(stateFname, trueOsc, kNoShift, sampleString);

  // Now look at the prefit uncertainties...
  std::cout << "Prefit all uncertainties..." << std::endl;
  TDirectory *prefit_dir = fout->mkdir("prefit_all");
  throw_errors(stateFname, sampleString, prefit_dir, nom_vect, systlist,
               allsystnames, trueOsc);

  // Try prefit subgroups
  // Get the flux list by removing anything from the allsystlist that doesn't
  // have "flux" in
  std::vector<std::string> flux_list = allsystnames;
  flux_list.erase(std::remove_if(flux_list.begin(), flux_list.end(),
                                 [](const std::string &s) {
                                   return s.find("flux_") == std::string::npos;
                                 }),
                  flux_list.end());

  // Make some parameter lists
  std::vector<std::string> all_xsec_list = GetXSecSystNames();
  std::vector<std::string> xsec_qe_list = GetXSecSystNames("QELike");
  std::vector<std::string> xsec_res_list = GetXSecSystNames("RES");
  std::vector<std::string> xsec_dis_list = GetXSecSystNames("DIS");
  std::vector<std::string> xsec_fsi_list = GetXSecSystNames("FSI");
  std::vector<std::string> xsec_nc_list = GetXSecSystNames("NC");
  std::vector<std::string> xsec_ratios_list = GetXSecSystNames("Ratios");

  std::vector<std::string> fd_det_list;
  for (auto s : GetEnergySysts())
    fd_det_list.push_back(s->ShortName());
  for (auto s : GetFDRecoSysts())
    fd_det_list.push_back(s->ShortName());

  std::vector<std::string> flux_xsec_list = all_xsec_list;
  flux_xsec_list.insert(flux_xsec_list.end(), flux_list.begin(),
                        flux_list.end());

  // std::cout << "Prefit flux uncertainties..." << std::endl;
  // TDirectory *prefit_flux_dir = fout->mkdir("prefit_flux");
  // throw_errors(stateFname, sampleString, prefit_flux_dir, nom_vect, systlist,
  //              flux_list, trueOsc);
  //
  // std::cout << "Prefit fd_det uncertainties..." << std::endl;
  // TDirectory *prefit_fd_det_dir = fout->mkdir("prefit_fd_det");
  // throw_errors(stateFname, sampleString, prefit_fd_det_dir, nom_vect, systlist,
  //              fd_det_list, trueOsc);
  //
  // std::cout << "Prefit xsec_qe uncertainties..." << std::endl;
  // TDirectory *prefit_xsec_qe_dir = fout->mkdir("prefit_xsec_qe");
  // throw_errors(stateFname, sampleString, prefit_xsec_qe_dir, nom_vect, systlist,
  //              xsec_qe_list, trueOsc);
  //
  // std::cout << "Prefit xsec_res uncertainties..." << std::endl;
  // TDirectory *prefit_xsec_res_dir = fout->mkdir("prefit_xsec_res");
  // throw_errors(stateFname, sampleString, prefit_xsec_res_dir, nom_vect,
  //              systlist, xsec_res_list, trueOsc);
  //
  // std::cout << "Prefit FSI uncertainties..." << std::endl;
  // TDirectory *prefit_fsi_dir = fout->mkdir("prefit_xsec_fsi");
  // throw_errors(stateFname, sampleString, prefit_fsi_dir, nom_vect, systlist,
  //              xsec_fsi_list, trueOsc);
  //
  // std::cout << "Prefit xsec_dis uncertainties..." << std::endl;
  // TDirectory *prefit_xsec_dis_dir = fout->mkdir("prefit_xsec_dis");
  // throw_errors(stateFname, sampleString, prefit_xsec_dis_dir, nom_vect,
  //              systlist, xsec_dis_list, trueOsc);
  //
  // std::cout << "Prefit xsec_NC uncertainties..." << std::endl;
  // TDirectory *prefit_xsec_nc_dir = fout->mkdir("prefit_xsec_nc");
  // throw_errors(stateFname, sampleString, prefit_xsec_nc_dir, nom_vect, systlist,
  //              xsec_nc_list, trueOsc);
  //
  // std::cout << "Prefit xsec_ratios uncertainties..." << std::endl;
  // TDirectory *prefit_xsec_ratios_dir = fout->mkdir("prefit_xsec_ratios");
  // throw_errors(stateFname, sampleString, prefit_xsec_ratios_dir, nom_vect,
  //              systlist, xsec_ratios_list, trueOsc);
  //
  // std::cout << "Prefit xsec uncertainties..." << std::endl;
  // TDirectory *prefit_xsec_dir = fout->mkdir("prefit_xsec");
  // throw_errors(stateFname, sampleString, prefit_xsec_dir, nom_vect, systlist,
  //              all_xsec_list, trueOsc);
  //
  // std::cout << "Prefit xsec+flux uncertainties..." << std::endl;
  // TDirectory *prefit_flux_xsec_dir = fout->mkdir("prefit_flux_xsec");
  // throw_errors(stateFname, sampleString, prefit_flux_xsec_dir, nom_vect,
  //              systlist, flux_xsec_list, trueOsc);

  // Now move to postfit!
  std::cout << "Postfit uncertainties..." << std::endl;
  TDirectory *postfit_dir = fout->mkdir("postfit_all");
  postfit_dir->cd();

  // Get the covariance matrix
  TMatrixDSym *covar = (TMatrixDSym *)fout->Get("covar_mat");
  TMatrixDSym sub_covar = covar->GetSub(oscVars.size(), covar->GetNrows() - 1,
                                        oscVars.size(), covar->GetNrows() - 1);
  sub_covar.Write("sub_covar_mat");

  TH2D hist_sub_covar = TH2D(sub_covar);
  hist_sub_covar.SetName("sub_covar");
  hist_sub_covar.Write();

  // Decompose, and same the output (as a TH1D too)
  TDecompChol LU = TDecompChol(sub_covar);
  LU.Decompose();

  TMatrixDSym *dec_mat =
      new TMatrixDSym(sub_covar.GetNrows(), LU.GetU().GetMatrixArray(), "");
  TH2D hist_chol = TH2D(*dec_mat);
  hist_chol.SetName("hist_chol");
  hist_chol.Write();

  std::cout << "Postfit all uncertainties..." << std::endl;
  throw_errors(stateFname, sampleString, postfit_dir, nom_vect, systlist,
               allsystnames, trueOsc, dec_mat);

  std::cout << "Postfit flux uncertainties..." << std::endl;
  TDirectory *postfit_flux_dir = fout->mkdir("postfit_flux");
  throw_errors(stateFname, sampleString, postfit_flux_dir, nom_vect, systlist,
               flux_list, trueOsc, dec_mat);

  std::cout << "Postfit fd_det uncertainties..." << std::endl;
  TDirectory *postfit_fd_det_dir = fout->mkdir("postfit_fd_det");
  throw_errors(stateFname, sampleString, postfit_fd_det_dir, nom_vect, systlist,
               fd_det_list, trueOsc, dec_mat);
  //
  // std::cout << "Postfit xsec_qe uncertainties..." << std::endl;
  // TDirectory *postfit_xsec_qe_dir = fout->mkdir("postfit_xsec_qe");
  // throw_errors(stateFname, sampleString, postfit_xsec_qe_dir, nom_vect,
  //              systlist, xsec_qe_list, trueOsc, dec_mat);
  //
  // std::cout << "Postfit xsec_res uncertainties..." << std::endl;
  // TDirectory *postfit_xsec_res_dir = fout->mkdir("postfit_xsec_res");
  // throw_errors(stateFname, sampleString, postfit_xsec_res_dir, nom_vect,
  //              systlist, xsec_res_list, trueOsc, dec_mat);
  //
  // std::cout << "Postfit FSI uncertainties..." << std::endl;
  // TDirectory *postfit_fsi_dir = fout->mkdir("postfit_xsec_fsi");
  // throw_errors(stateFname, sampleString, postfit_fsi_dir, nom_vect, systlist,
  //              xsec_fsi_list, trueOsc, dec_mat);
  //
  // std::cout << "Postfit xsec_dis uncertainties..." << std::endl;
  // TDirectory *postfit_xsec_dis_dir = fout->mkdir("postfit_xsec_dis");
  // throw_errors(stateFname, sampleString, postfit_xsec_dis_dir, nom_vect,
  //              systlist, xsec_dis_list, trueOsc, dec_mat);
  //
  // std::cout << "Postfit xsec_nc uncertainties..." << std::endl;
  // TDirectory *postfit_xsec_nc_dir = fout->mkdir("postfit_xsec_nc");
  // throw_errors(stateFname, sampleString, postfit_xsec_nc_dir, nom_vect,
  //              systlist, xsec_nc_list, trueOsc, dec_mat);
  //
  // std::cout << "Postfit xsec_ratios uncertainties..." << std::endl;
  // TDirectory *postfit_xsec_ratios_dir = fout->mkdir("postfit_xsec_ratios");
  // throw_errors(stateFname, sampleString, postfit_xsec_ratios_dir, nom_vect,
  //              systlist, xsec_ratios_list, trueOsc, dec_mat);

  std::cout << "Postfit xsec uncertainties..." << std::endl;
  TDirectory *postfit_xsec_dir = fout->mkdir("postfit_xsec");
  throw_errors(stateFname, sampleString, postfit_xsec_dir, nom_vect, systlist,
               all_xsec_list, trueOsc, dec_mat);

  // std::cout << "Postfit xsec+flux uncertainties..." << std::endl;
  // TDirectory *postfit_flux_xsec_dir = fout->mkdir("postfit_flux_xsec");
  // throw_errors(stateFname, sampleString, postfit_flux_xsec_dir, nom_vect,
  //              systlist, flux_xsec_list, trueOsc, dec_mat);

  fout->cd();

  // Save the breakdown
  for (auto &hist : breakdown_vect)
    hist->Write();


  // Now close the file
  fout->Close();
}

#ifndef __CINT__
int main(int argc, char const *argv[]) {

  gROOT->SetMustClean(false);

  std::string stateFname = (argc > 1) ? argv[1] : def_stateFname;
  std::string outputFname = (argc > 2) ? argv[2] : def_outputFname;
  std::string systSet = (argc > 3) ? argv[3] : def_systSet;
  std::string sampleString = (argc > 4) ? argv[4] : def_sampleString;
  std::string penaltyString = (argc > 5) ? argv[5] : def_penaltyString;
  std::string asimov_set = (argc > 6) ? argv[6] : def_asimov_set;

  sample_throws(stateFname, outputFname, systSet, sampleString, penaltyString,
                asimov_set);
}
#endif
