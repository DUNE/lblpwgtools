#include "common_fit_definitions.C"

#include "CAFAna/Analysis/PRISMExtrapolator.h"
#include "CAFAna/Prediction/PredictionPRISM.h"

const Var kTrueOffAxisPos =
    (SIMPLEVAR(dune.det_x) + (SIMPLEVAR(dune.vtx_x) * ana::Constant(1.0E-2)));
const Var kTrueEv = SIMPLEVAR(dune.Ev);
const Var kDetX = SIMPLEVAR(dune.det_x);

const Cut kIsOutOfTheDesert({"kIsOutOfTheDesert"},
                            [](const caf::StandardRecord *sr) {
                              return (fabs(sr->dune.vtx_x) < 200);
                            });

const Binning binsOffAxisPos = Binning::Simple(80, -0.25, 39.75);
const Binning binsOffAxisPos_fine = Binning::Simple(800, -0.25, 39.75);
const Binning binsETrue_PRISM = Binning::Simple(100, 0, 10);

const HistAxis trueOffAxisPos("Off axis position (m)", binsOffAxisPos,
                              kTrueOffAxisPos);

const HistAxis trueOffAxisPos_fine("Off axis position (m)", binsOffAxisPos_fine,
                                   kTrueOffAxisPos);

const HistAxis trueEvAxes("True E_{#nu} (GeV)", binsETrue_PRISM, kTrueEv);

const HistAxis trueEvAxes2D_fine("True E_{#nu} (GeV)", binsETrue_PRISM, kTrueEv,
                                 "Off axis position (m)", binsOffAxisPos_fine,
                                 kTrueOffAxisPos);
const HistAxis trueEvAxes2D("True E_{#nu} (GeV)", binsETrue_PRISM, kTrueEv,
                            "Off axis position (m)", binsOffAxisPos,
                            kTrueOffAxisPos);

const HistAxis PosDetAx("Det_x (m)", binsOffAxisPos, kDetX,
                        "Off axis position (m)", binsOffAxisPos,
                        kTrueOffAxisPos);

void PRISMPrediction(std::string const &off_axis_file,
                     std::string const &output_file, bool isfhc = true,
                     std::string const &state_file = "", bool reload = false) {

  std::unique_ptr<PredictionInterp> predInterpNDNumu;
  std::unique_ptr<PredictionInterp> predInterpFDNumu;
  std::unique_ptr<PredictionPRISM> ETruePred_NoSel;
  std::unique_ptr<PredictionNoExtrap> ETruePred_NoSel_FD;

  osc::IOscCalculatorAdjustable *this_calc = NuFitOscCalc(1);

  if (reload || !state_file.length() || TFile(state_file.c_str()).IsZombie()) {
    Loaders these_loaders;
    SpectrumLoader SpecLoader(off_axis_file, kBeam);
    these_loaders.AddLoader(&SpecLoader, caf::kNEARDET, Loaders::kMC);
    SpectrumLoader loaderFDNumu(
        "/home/picker24/software/CAFAna/inputs/MCC11/mcc11_v3/" +
            GetSampleName(kFDFHC) + "_nonswap.root",
        kBeam);
    these_loaders.AddLoader(&loaderFDNumu, caf::kFARDET, Loaders::kMC,
                            ana::kBeam, Loaders::kNonSwap);

    // Use to weight by Exposure
    const Var kRunPlanWeight({}, [&](const caf::StandardRecord *sr) -> double {
      return sr->dune.perPOTWeight * sr->dune.perFileWeight;
    });

    ana::FVMassCorrection fvmc;
    const Var kMassCorrection({}, [&](const caf::StandardRecord *sr) -> double {
      return fvmc.GetWeight(sr->dune.vtx_x);
    });

    std::vector<std::pair<std::string, std::unique_ptr<Spectrum>>> specs;
    //
    // specs.emplace_back(
    //     "EnuPos2D_fine",
    //     std::make_unique<Spectrum>(SpecLoader, trueEvAxes2D_fine,
    //                                kIsTrueFV && kIsOutOfTheDesert, kNoShift,
    //                                kGENIEWeights * kRunPlanWeight));
    // specs.emplace_back("EnuPos2D", std::make_unique<Spectrum>(
    //                                    SpecLoader, trueEvAxes2D,
    //                                    kIsTrueFV && kIsOutOfTheDesert,
    //                                    kNoShift, kGENIEWeights *
    //                                    kRunPlanWeight));
    //
    // specs.emplace_back("Pos1D", std::make_unique<Spectrum>(
    //                                 SpecLoader, trueOffAxisPos, kIsTrueFV,
    //                                 kNoShift, kGENIEWeights *
    //                                 kRunPlanWeight));
    //
    // specs.emplace_back(
    //     "Pos1D_OOD",
    //     std::make_unique<Spectrum>(SpecLoader, trueOffAxisPos,
    //                                kIsTrueFV && kIsOutOfTheDesert, kNoShift,
    //                                kGENIEWeights * kRunPlanWeight));
    //
    // specs.emplace_back("Pos1D_OOD_FVMass",
    //                    std::make_unique<Spectrum>(
    //                        SpecLoader, trueOffAxisPos,
    //                        kIsTrueFV && kIsOutOfTheDesert, kNoShift,
    //                        kGENIEWeights * kRunPlanWeight *
    //                        kMassCorrection));
    //
    // specs.emplace_back(
    //     "DetPos2D",
    //     std::make_unique<Spectrum>(SpecLoader, PosDetAx, kIsTrueFV, kNoShift,
    //                                kGENIEWeights * kRunPlanWeight));
    //
    // specs.emplace_back("DetPos2D_OOD",
    //                    std::make_unique<Spectrum>(
    //                        SpecLoader, PosDetAx, kIsTrueFV &&
    //                        kIsOutOfTheDesert, kNoShift, kGENIEWeights *
    //                        kRunPlanWeight));
    //
    // specs.emplace_back("DetPos2D_OOD_FVMass",
    //                    std::make_unique<Spectrum>(
    //                        SpecLoader, PosDetAx, kIsTrueFV &&
    //                        kIsOutOfTheDesert, kNoShift, kGENIEWeights *
    //                        kRunPlanWeight * kMassCorrection));

    NoOscPredictionGenerator genNDNumu_2D(
        trueEvAxes2D, kIsNumuCC && kIsTrueFV && kIsOutOfTheDesert,
        kGENIEWeights * kRunPlanWeight * kMassCorrection);
    predInterpNDNumu = std::make_unique<PredictionInterp>(
        std::vector<ISyst const *>{}, this_calc, genNDNumu_2D, these_loaders);

    NoExtrapPredictionGenerator genFDNumu(trueEvAxes, kIsNumuCC && kIsTrueFV,
                                          kGENIEWeights);
    predInterpFDNumu = std::make_unique<PredictionInterp>(
        std::vector<ISyst const *>{}, this_calc, genFDNumu, these_loaders);

    ETruePred_NoSel = std::make_unique<PredictionPRISM>(
        SpecLoader, trueEvAxes, trueOffAxisPos,
        kIsNumuCC && kIsTrueFV && kIsOutOfTheDesert, kNoShift,
        kGENIEWeights * kRunPlanWeight * kMassCorrection);

    ETruePred_NoSel_FD = std::make_unique<PredictionNoExtrap>(
        these_loaders, trueEvAxes, kIsNumuCC && kIsTrueFV, kNoShift,
        kGENIEWeights);
    these_loaders.Go();

    if (state_file.length()) {
      TFile fs(state_file.c_str(), "RECREATE");

      ETruePred_NoSel->SaveTo(fs.mkdir((std::string("nd_prism_interp_numu_") +
                                        std::string(isfhc ? "fhc" : "rhc"))
                                           .c_str()));
      predInterpNDNumu->SaveTo(fs.mkdir((std::string("nd_2dinterp_numu_") +
                                         std::string(isfhc ? "fhc" : "rhc"))
                                            .c_str()));
      predInterpFDNumu->SaveTo(fs.mkdir(
          (std::string("fd_interp_numu_") + std::string(isfhc ? "fhc" : "rhc"))
              .c_str()));
      ETruePred_NoSel_FD->SaveTo(fs.mkdir((std::string("fd_noextrap_numu_") +
                                           std::string(isfhc ? "fhc" : "rhc"))
                                              .c_str()));
      fs.Write();
      fs.Close();
    }
  }

  if (state_file.length()) { // reload them if we know we've just saved them.
    TFile fs(state_file.c_str());
    ETruePred_NoSel = PredictionPRISM::LoadFrom(
        fs.GetDirectory((std::string("nd_prism_interp_numu_") +
                         std::string(isfhc ? "fhc" : "rhc"))
                            .c_str()));
    predInterpNDNumu = PredictionInterp::LoadFrom(fs.GetDirectory(
        (std::string("nd_2dinterp_numu_") + std::string(isfhc ? "fhc" : "rhc"))
            .c_str()));
    predInterpFDNumu = PredictionInterp::LoadFrom(fs.GetDirectory(
        (std::string("fd_interp_numu_") + std::string(isfhc ? "fhc" : "rhc"))
            .c_str()));
    ETruePred_NoSel_FD = PredictionNoExtrap::LoadFrom(fs.GetDirectory(
        (std::string("fd_noextrap_numu_") + std::string(isfhc ? "fhc" : "rhc"))
            .c_str()));
    fs.Close();

    std::cout << "[INFO]: Reading input state from " << state_file << std::endl;
  }

  // PredictionPRISM ERecYPred(SpecLoader, *default_axes.NDAx, trueOffAxisPos,
  //                           (isfhc ? kPassND_FHC_NUMU : kPassND_RHC_NUMU) &&
  //                               kIsTrueFV,
  //                           kNoShift, kGENIEWeights * kRunPlanWeight, &pfm);
  //
  // PredictionPRISM ERecYPred_NoRunPlanWeight(
  //     SpecLoader, *default_axes.NDAx, trueOffAxisPos,
  //     (isfhc ? kPassND_FHC_NUMU : kPassND_RHC_NUMU) && kIsTrueFV, kNoShift,
  //     kGENIEWeights, &pfm);
  //
  // PredictionPRISM ETruePred_NoSelection(SpecLoader, trueEvAxes,
  // trueOffAxisPos,
  //                                       kIsTrueFV, kNoShift,
  //                                       kGENIEWeights * kRunPlanWeight,
  //                                       &pfm);
  //
  // PredictionPRISM ETruePred_NoSelection_NoWeight(SpecLoader, trueEvAxes,
  //                                                trueOffAxisPos, kIsTrueFV,
  //                                                kNoShift, kGENIEWeights,
  //                                                &pfm);
  //
  // PredictionPRISM ETruePred_Selection(
  //     SpecLoader, trueEvAxes, trueOffAxisPos,
  //     (isfhc ? kPassND_FHC_NUMU : kPassND_RHC_NUMU) && kIsTrueFV, kNoShift,
  //     kGENIEWeights * kRunPlanWeight, &pfm);
  //
  // PredictionNoOsc OffAxis_Selection(
  //     SpecLoader, trueOffAxisPos_fine,
  //     (isfhc ? kPassND_FHC_NUMU : kPassND_RHC_NUMU) && kIsTrueFV, kNoShift,
  //     kGENIEWeights * kRunPlanWeight);
  //
  // PredictionNoOsc OffAxis_NoSelection(SpecLoader, trueOffAxisPos_fine,
  // kNoCut,
  //                                     kNoShift, kGENIEWeights *
  //                                     kRunPlanWeight);
  //
  // PredictionNoOsc OffAxis_NoSelection_NoWeight(SpecLoader,
  // trueOffAxisPos_fine,
  //                                              kNoCut, kNoShift,
  //                                              kGENIEWeights);

  // PredictionPRISMGenerator genNDNumu(
  //     *default_axes.NDAx, trueOffAxisPos,
  //     (isfhc ? kPassND_FHC_NUMU : kPassND_RHC_NUMU) && kIsTrueFV,
  //     kGENIEWeights * kRunPlanWeight, &pfm);
  //
  // PredictionInterp PRISMInterpNDNumu({}, this_calc, genNDNumu,
  // these_loaders);
  //

  PRISMExtrapolator pfm;
  // pfm.InitializeFluxMatcher("DUNEFluxFineOptEngNov2017.2019_03_06.root", 1);
  pfm.InitializeEventRateMatcher(predInterpNDNumu.get(),
                                 predInterpFDNumu.get());
  pfm.SetStoreDebugMatches();
  pfm.SetTargetConditioning(5E-9, 0.5, 3.5);

  ETruePred_NoSel->SetFluxMatcher(&pfm);

  TFile f(output_file.c_str(), "RECREATE");
  // for (auto &s : specs) {
  //   s.second->OverridePOT(1);
  //   s.second->ToTHX(1)->Write(s.first.c_str());
  // }

  // TH1 *ERecYPred_h = ERecYPred.Predict(this_calc).ToTHX(1);
  // ERecYPred_h->SetTitle(";ERec (GeV);YRec;Pred. FD EvRate");
  // ERecYPred_h->Write("ERecYPred");
  //
  // TH1 *ERecYPred_NoRunPlanWeight_h =
  //     ERecYPred_NoRunPlanWeight.Predict(this_calc).ToTHX(1);
  // ERecYPred_NoRunPlanWeight_h->SetTitle(
  //     ";E_{#nu} (GeV);YRec;Pred. FD EvRate (No Run Weighting)");
  // ERecYPred_NoRunPlanWeight_h->Write("ERecYPred_NoRunPlanWeight");
  //
  // TH1 *ETruePred_NoSelection_h =
  //     ETruePred_NoSelection.Predict(this_calc).ToTHX(1);
  // ETruePred_NoSelection_h->SetTitle(
  //     ";E_{#nu} (GeV);Pred. FD EvRate (No Selection)");
  // ETruePred_NoSelection_h->Write("ETruePred_NoSelection");
  //
  // TH1 *ETruePred_NoSelection_NoWeight_h =
  //     ETruePred_NoSelection_NoWeight.Predict(this_calc).ToTHX(1);
  // ETruePred_NoSelection_NoWeight_h->SetTitle(
  //     ";E_{#nu} (GeV);Pred. FD EvRate (No Selection, No Run Weighting)");
  // ETruePred_NoSelection_NoWeight_h->Write("ETruePred_NoSelection_NoWeight");
  //
  // TH1 *ETruePred_Selection_h =
  // ETruePred_Selection.Predict(this_calc).ToTHX(1);
  // ETruePred_Selection_h->SetTitle(";E_{#nu} (GeV);Pred. FD EvRate
  // (Selection)"); ETruePred_Selection_h->Write("ETruePred_Selection");
  //
  // Spectrum OffAxis_Selection_s = OffAxis_Selection.Predict(this_calc);
  // OffAxis_Selection_s.OverridePOT(1);
  // TH1 *OffAxis_Selection_h = OffAxis_Selection_s.ToTHX(1);
  // OffAxis_Selection_h->SetTitle(";Off Axis Position (m);Event Rate");
  // OffAxis_Selection_h->Write("OffAxis_Selection");
  //
  // Spectrum OffAxis_NoSelection_s = OffAxis_NoSelection.Predict(this_calc);
  // OffAxis_NoSelection_s.OverridePOT(1);
  // TH1 *OffAxis_NoSelection_h = OffAxis_NoSelection_s.ToTHX(1);
  // OffAxis_NoSelection_h->SetTitle(";Off Axis Position (m);Event Rate");
  // OffAxis_NoSelection_h->Write("OffAxis_NoSelection");
  //
  // Spectrum OffAxis_NoSelection_NoWeight_s =
  //     OffAxis_NoSelection_NoWeight.Predict(this_calc);
  // OffAxis_NoSelection_NoWeight_s.OverridePOT(1);
  // TH1 *OffAxis_NoSelection_NoWeight_h =
  // OffAxis_NoSelection_NoWeight_s.ToTHX(1);
  // OffAxis_NoSelection_NoWeight_h->SetTitle(";Off Axis Position (m);Event
  // Rate");
  // OffAxis_NoSelection_NoWeight_h->Write("OffAxis_NoSelection_NoWeight");
  //
  // ETruePred_NoSelection.SaveTo(f.mkdir("ETruePred_NoSelection_state"));
  // ETruePred_NoSelection_NoWeight.SaveTo(
  //     f.mkdir("ETruePred_NoSelection_NoWeight_state"));

  // PRISMInterpNDNumu.SaveTo(f.mkdir((std::string("nd_prism_interp_numu_") +
  //                                   std::string(isfhc ? "fhc" : "rhc"))
  //                                      .c_str()));

  ana::Spectrum NDSpect = predInterpNDNumu->PredictComponent(
      nullptr, ana::Flavors::kAllNuMu, ana::Current::kCC, ana::Sign::kNu);
  NDSpect.OverridePOT(1);
  TH2 *NDHist = NDSpect.ToTH2(4E19);
  NDHist->Write("NDSpect");

  ana::Spectrum FDSpect = predInterpFDNumu->PredictComponent(
      this_calc, ana::Flavors::kAllNuMu, ana::Current::kCC, ana::Sign::kNu);
  FDSpect.OverridePOT(1);
  TH1 *FDHist = FDSpect.ToTH1(1);
  FDHist->Write("FDSpect");

  {
    double reg_factor = 5E-9;
    int NCoeffs = NDHist->GetYaxis()->FindFixBin(33);

    NDHist->RebinY(2);
    Eigen::MatrixXd NDFluxMatrix = GetEigenMatrix(NDHist, NCoeffs);
    Eigen::MatrixXd RegMatrix = Eigen::MatrixXd::Zero(NCoeffs, NCoeffs);

    if (reg_factor) {
      for (int row_it = 0; row_it < (NCoeffs - 1); ++row_it) {
        RegMatrix(row_it, row_it) = reg_factor;
        RegMatrix(row_it, row_it + 1) = -reg_factor;
      }
      RegMatrix(NCoeffs - 1, NCoeffs - 1) = reg_factor;
    }

    Eigen::VectorXd Target = GetEigenFlatVector(FDHist);

    Eigen::MatrixXd RHS = NDFluxMatrix.transpose() * Target;
    Eigen::MatrixXd LHS = (NDFluxMatrix.transpose() * NDFluxMatrix);
    // +      (RegMatrix.transpose() * RegMatrix);

    Eigen::VectorXd soln = LHS.inverse() * RHS;

    TH1D *SolnHist = new TH1D("soln", "", soln.size(), 0, soln.size());
    FillHistFromEigenVector(SolnHist, soln);
    SolnHist->Write("SolnHist");
  }

  TH1 *ETruePred_NoSel_h = ETruePred_NoSel->Predict(this_calc).ToTHX(1);
  ETruePred_NoSel_h->SetTitle(";E_{#nu} (GeV);Pred. FD EvRate");
  ETruePred_NoSel_h->Write("ETruePred_NoSel");

  TH1 *ETruePred_NoSel_FD_h = ETruePred_NoSel_FD->Predict(this_calc).ToTHX(1);
  ETruePred_NoSel_FD_h->SetTitle(";E_{#nu} (GeV);FD EvRate");
  ETruePred_NoSel_FD_h->Write("ETruePred_NoSel_FD");

  pfm.Write(f.mkdir("PRISMFluxMatches"));

  f.Write();
}

#ifndef __CINT__

bool strToBool(std::string const &str) {
  if (str == "true") {
    return true;
  }
  if (str == "True") {
    return true;
  }
  if (str == "1") {
    return true;
  }
  if (str == "false") {
    return false;
  }
  if (str == "False") {
    return false;
  }
  if (str == "0") {
    return false;
  }

  std::cout << "[ERROR]: Failed to parse " << str << " as bool." << std::endl;
  throw std::runtime_error(str);
}

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    std::cout << "[ERROR]: Expect at least 2 arguments." << std::endl;
    return 1;
  }
  std::string off_axis_file = argv[1];
  std::string output_file = argv[2];
  bool isfhc = (argc >= 4) ? strToBool(argv[3]) : true;
  std::string state_file = (argc >= 5) ? argv[4] : "";
  bool reload = (argc >= 6) ? strToBool(argv[5]) : false;
  PRISMPrediction(off_axis_file, output_file, isfhc, state_file, reload);
}
#endif
