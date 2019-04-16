#include "common_fit_definitions.C"

#include "CAFAna/Analysis/PRISMExtrapolator.h"
#include "CAFAna/Prediction/PredictionPRISM.h"

const Var kTrueOffAxisPos =
    (SIMPLEVAR(dune.det_x) + (SIMPLEVAR(dune.vtx_x) * ana::Constant(1.0E-2)));
const Var kTrueEv = SIMPLEVAR(dune.Ev);
const Var kTrueELep = SIMPLEVAR(dune.LepE);

const Var kProxyE({}, [&](const caf::StandardRecord *sr) -> double {
  double eother = 0;
  if (std::isnormal(sr->dune.eOther)) {
    eother = sr->dune.eOther;
  }
  double eprox = sr->dune.LepE + sr->dune.eP + sr->dune.ePip + sr->dune.ePim +
                 sr->dune.ePi0 + 0.135 * sr->dune.nipi0 + eother;
  return eprox;
});

const Var kProxyE_20pclpe({}, [&](const caf::StandardRecord *sr) -> double {
  double eother = 0;
  if (std::isnormal(sr->dune.eOther)) {
    eother = sr->dune.eOther;
  }
  double eprox = sr->dune.LepE + 0.8 * sr->dune.eP + sr->dune.ePip +
                 sr->dune.ePim + sr->dune.ePi0 + 0.135 * sr->dune.nipi0 +
                 eother;
  return eprox;
});

const Cut kIsOutOfTheDesert({"kIsOutOfTheDesert"},
                            [](const caf::StandardRecord *sr) {
                              return (fabs(sr->dune.vtx_x) < 200);
                            });

const Cut kETrueLT10GeV({"kETrueLT10GeV"}, [](const caf::StandardRecord *sr) {
  return (sr->dune.Ev < 10);
});

const double OA_bin_width_m = 0.5;
const double OA_min_m = -0.25;
const double OA_max_m = 32.75;
const size_t NOABins = (OA_max_m - OA_min_m) / OA_bin_width_m;
const size_t MergeFluxOABins = 0.5 / OA_bin_width_m;
const Binning binsOffAxisPos = Binning::Simple(NOABins, OA_min_m, OA_max_m);
const Binning binsETrue_PRISM = Binning::Simple(100, 0, 10);

const HistAxis trueOffAxisPos("Off axis position (m)", binsOffAxisPos,
                              kTrueOffAxisPos);

const HistAxis trueEvAxes("True E_{#nu} (GeV)", binsETrue_PRISM, kTrueEv);
const HistAxis EventRateMatchAxis("True E_{#nu} (GeV)", binsETrue_PRISM,
                                  kTrueEv);

const HistAxis proxyEvAxes("Truth proxy E_{#nu} (GeV)", binsETrue_PRISM,
                           kProxyE);

const HistAxis ERecFromDepAxes("E_{Dep.} (GeV)", binsETrue_PRISM,
                               kRecoE_FromDep);

const HistAxis
    proxy_20pclpeEvAxes("Truth proxy (20%% less proton energy) E_{#nu} (GeV)",
                        binsETrue_PRISM, kProxyE_20pclpe);

const HistAxis ElepAxes("True E_{lep.} (GeV)", binsETrue_PRISM, kTrueELep);

struct PRISMComp {
  std::unique_ptr<PredictionPRISM> PRISM;
  std::unique_ptr<PredictionInterp> NDInterp;
  std::unique_ptr<PredictionInterp> FDInterp;
  std::unique_ptr<PredictionNoExtrap> FarDet;
};

std::vector<double> const FDnumuFHCSelEff_enu = {
    0.546, 0.683, 0.821, 0.945, 1.07, 1.19, 1.44, 1.67,
    1.94,  2.25,  2.86,  4.17,  4.58, 5.44, 6.57, 7.65};

std::vector<double> const FDnumuFHCSelEff_eff = {
    0.624, 0.708, 0.761, 0.808, 0.839, 0.869, 0.902, 0.914,
    0.930, 0.944, 0.956, 0.958, 0.960, 0.943, 0.929, 0.912};

std::vector<double> const ND_UnGeoCorrectible_enu = {
    0.183, 0.502, 0.870, 1.25, 1.68, 2.45, 3.27, 4.19, 5.28, 6.48, 9.05};

std::vector<double> const ND_UnGeoCorrectible_eff = {0.996, 0.996, 0.985, 0.966,
                                                     0.958, 0.932, 0.898, 0.867,
                                                     0.845, 0.807, 0.742};

struct ProjectionDef {
  ProjectionDef(std::string n, HistAxis const &a, Var const &e)
      : Name(n), Ax(a), ExtraWeight(e), PRISMEventRateMatcher() {}
  std::string Name;
  HistAxis const Ax;
  Var const ExtraWeight;
  PRISMExtrapolator PRISMEventRateMatcher;
};

void PRISMPrediction(std::string const &off_axis_file,
                     std::string const &output_file, bool isfhc = true,
                     std::string const &state_file = "", bool reload = false) {

  osc::IOscCalculatorAdjustable *calc = NuFitOscCalc(1);
  std::vector<ana::ISyst const *> systlist = {};

  std::map<std::string, PRISMComp> Predictions;

  TGraph FDnumuFHCSelEff(FDnumuFHCSelEff_enu.size(), FDnumuFHCSelEff_enu.data(),
                         FDnumuFHCSelEff_eff.data());
  const Var kFDEff({}, [&](const caf::StandardRecord *sr) -> double {
    return FDnumuFHCSelEff.Eval(sr->dune.Ev);
  });

  TGraph ND_UnGeoCorrectibleEff(ND_UnGeoCorrectible_enu.size(),
                                ND_UnGeoCorrectible_enu.data(),
                                ND_UnGeoCorrectible_eff.data());
  const Var kNDEff({}, [&](const caf::StandardRecord *sr) -> double {
    return ND_UnGeoCorrectibleEff.Eval(sr->dune.Ev);
  });

  std::vector<ProjectionDef> Projections;
  Projections.emplace_back("ETrue", trueEvAxes, ana::Constant(1));
  Projections.emplace_back("ETrueNDEff", trueEvAxes, kNDEff);
  Projections.emplace_back("ETrueNDFDEff", trueEvAxes, kNDEff * kFDEff);
  Projections.emplace_back("ERecProxy", proxyEvAxes, ana::Constant(1));
  Projections.emplace_back("ERecDep", ERecFromDepAxes, ana::Constant(1));
  Projections.emplace_back("ELepTrue", ElepAxes, ana::Constant(1));

  if (reload || !state_file.length() || TFile(state_file.c_str()).IsZombie()) {
    Loaders these_loaders;
    SpectrumLoader SpecLoader(off_axis_file, kBeam);
    these_loaders.AddLoader(&SpecLoader, caf::kNEARDET, Loaders::kMC);
    SpectrumLoader loaderFDNumu("/dune/data/users/marshalc/CAFs/mcc11_v3/" +
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

    std::vector<std::unique_ptr<NoOscPredictionGenerator>> NDPredGens;
    std::vector<std::unique_ptr<NoExtrapPredictionGenerator>> FDPredGens;

    for (auto const &proj : Projections) {
      PRISMComp projComp;
      projComp.PRISM = std::make_unique<PredictionPRISM>(
          SpecLoader, proj.Ax, trueOffAxisPos,
          kIsNumuCC && !kIsAntiNu && kIsTrueFV && kIsOutOfTheDesert &&
              kETrueLT10GeV,
          kNoShift,
          kGENIEWeights * kRunPlanWeight * kMassCorrection * proj.ExtraWeight);
      projComp.PRISM->AddFDMCLoader(loaderFDNumu, kIsTrueFV && kETrueLT10GeV,
                                    kNoShift, kGENIEWeights * proj.ExtraWeight);
      projComp.PRISM->AddNDMCBkgLoader(
          SpecLoader, kIsTrueFV && kETrueLT10GeV, kNoShift,
          kGENIEWeights * kRunPlanWeight * kMassCorrection * proj.ExtraWeight);

      // Make the ND prediction interp include the same off-axis axis used for
      // PRISM weighting.
      std::vector<std::string> Labels = EventRateMatchAxis.GetLabels();
      std::vector<Binning> Bins = EventRateMatchAxis.GetBinnings();
      std::vector<Var> Vars = EventRateMatchAxis.GetVars();
      Labels.push_back(trueOffAxisPos.GetLabels().front());
      Bins.push_back(trueOffAxisPos.GetBinnings().front());
      Vars.push_back(trueOffAxisPos.GetVars().front());
      HistAxis const NDSpectaAxis(Labels, Bins, Vars);

      // Gotta make sure these hang around until  after Loaders::Go has been
      // called.
      NDPredGens.emplace_back(new NoOscPredictionGenerator(
          NDSpectaAxis,
          kIsNumuCC && !kIsAntiNu && kIsTrueFV && kIsOutOfTheDesert &&
              kETrueLT10GeV,
          kGENIEWeights * kRunPlanWeight * kMassCorrection * proj.ExtraWeight));
      FDPredGens.emplace_back(new NoExtrapPredictionGenerator(
          EventRateMatchAxis,
          kIsNumuCC && !kIsAntiNu && kIsTrueFV && kETrueLT10GeV,
          kGENIEWeights * proj.ExtraWeight));

      projComp.NDInterp = std::make_unique<PredictionInterp>(
          systlist, calc, *NDPredGens.back(), these_loaders);
      projComp.FDInterp = std::make_unique<PredictionInterp>(
          systlist, calc, *FDPredGens.back(), these_loaders);

      projComp.FarDet = std::make_unique<PredictionNoExtrap>(
          these_loaders, proj.Ax,
          kIsNumuCC && !kIsAntiNu && kIsTrueFV && kETrueLT10GeV, kNoShift,
          kGENIEWeights * proj.ExtraWeight);

      Predictions[proj.Name] = std::move(projComp);
    }

    these_loaders.Go();

    if (state_file.length()) {
      TFile fs(state_file.c_str(), "RECREATE");

      for (auto &projComp : Predictions) {
        projComp.second.PRISM->SaveTo(
            fs.mkdir((std::string("PRISM_") + projComp.first +
                      std::string(isfhc ? "_fhc" : "_rhc"))
                         .c_str()));

        projComp.second.NDInterp->SaveTo(
            fs.mkdir((std::string("NDInterp_") + projComp.first +
                      std::string(isfhc ? "_fhc" : "_rhc"))
                         .c_str()));
        projComp.second.FDInterp->SaveTo(
            fs.mkdir((std::string("FDInterp_") + projComp.first +
                      std::string(isfhc ? "_fhc" : "_rhc"))
                         .c_str()));

        projComp.second.FarDet->SaveTo(
            fs.mkdir((std::string("FarDet_") + projComp.first +
                      std::string(isfhc ? "_fhc" : "_rhc"))
                         .c_str()));
      }

      fs.Write();
      fs.Close();
    }
  }

  if (state_file.length()) { // reload them if we know we've just saved them.
    TFile fs(state_file.c_str());

    for (auto &proj : Projections) {
      Predictions[proj.Name].PRISM = PredictionPRISM::LoadFrom(
          fs.GetDirectory((std::string("PRISM_") + proj.Name +
                           std::string(isfhc ? "_fhc" : "_rhc"))
                              .c_str()));

      Predictions[proj.Name].NDInterp = PredictionInterp::LoadFrom(
          fs.GetDirectory((std::string("NDInterp_") + proj.Name +
                           std::string(isfhc ? "_fhc" : "_rhc"))
                              .c_str()));

      Predictions[proj.Name].FDInterp = PredictionInterp::LoadFrom(
          fs.GetDirectory((std::string("FDInterp_") + proj.Name +
                           std::string(isfhc ? "_fhc" : "_rhc"))
                              .c_str()));

      Predictions[proj.Name].FarDet = PredictionNoExtrap::LoadFrom(
          fs.GetDirectory((std::string("FarDet_") + proj.Name +
                           std::string(isfhc ? "_fhc" : "_rhc"))
                              .c_str()));
    }
    fs.Close();

    std::cout << "[INFO]: Reading input state from " << state_file << std::endl;
  }

  PRISMExtrapolator pfm;
  pfm.InitializeFluxMatcher("DUNEFluxFineOptEngNov2017.2019_03_06.root",
                            MergeFluxOABins);
  pfm.SetStoreDebugMatches();
  pfm.SetTargetConditioning(10E-9, 0.5, 3.5);

  TFile f(output_file.c_str(), "RECREATE");

  std::unique_ptr<osc::IOscCalculatorAdjustable> calc_extr(NuFitOscCalc(1));
  calc_extr->SetdCP(0.5 * M_PI);
  calc_extr->SetDmsq32(2.3E-3);

  std::unique_ptr<osc::IOscCalculatorAdjustable> calc_extr2(NuFitOscCalc(1));
  calc_extr2->SetdCP(-0.5 * M_PI);
  calc_extr2->SetDmsq32(2.6E-3);

  std::vector<std::pair<std::string, osc::IOscCalculator *>> oscpars = {
      {"nufit", calc},
      {"nufit_lowdm23_halfpi", calc_extr.get()},
      {"nufit_highdm23_mhalfpi", calc_extr2.get()},
  };

  for (auto clc : oscpars) {
    auto dir = f.mkdir(clc.first.c_str());
    dir->cd();

    for (auto &proj : Projections) {
      PRISMComp &projComp = Predictions[proj.Name];

      auto pdir = dir->mkdir(proj.Name.c_str());
      pdir->cd();

      projComp.PRISM->SetFluxMatcher(&pfm);

      Spectrum PRISMPredFluxMatchSpec = projComp.PRISM->Predict(clc.second);

      TH1 *PRISMPredFluxMatch_h = PRISMPredFluxMatchSpec.ToTHX(pot_fd);
      PRISMPredFluxMatch_h->SetTitle(";E_{#nu} (GeV);Pred. FD EvRate");
      PRISMPredFluxMatch_h->Write("PRISMPredFluxMatch");

      for (auto &compspec :
           projComp.PRISM->PredictPRISMComponents(clc.second)) {
        TH1 *comp = compspec.second.ToTHX(pot_fd);
        comp->SetTitle(";E_{#nu} (GeV);Pred. FD EvRate");
        comp->Write((std::string("PRISMPredFluxMatch_") +
                     PredictionPRISM::GetComponentString(compspec.first))
                        .c_str());
      }

      proj.PRISMEventRateMatcher.InitializeEventRateMatcher(
          projComp.NDInterp.get(), projComp.FDInterp.get());
      proj.PRISMEventRateMatcher.SetStoreDebugMatches();
      proj.PRISMEventRateMatcher.SetTargetConditioning(10E-15, 0.5, 3.5);
      projComp.PRISM->SetFluxMatcher(&proj.PRISMEventRateMatcher);

      Spectrum PRISMPredEvRateMatchSpec = projComp.PRISM->Predict(clc.second);

      TH1 *PRISMPredEvRateMatch_h = PRISMPredEvRateMatchSpec.ToTHX(pot_fd);
      PRISMPredEvRateMatch_h->SetTitle(";E_{#nu} (GeV);Pred. FD EvRate");
      PRISMPredEvRateMatch_h->Write("PRISMPredEvRateMatch");

      for (auto &compspec :
           projComp.PRISM->PredictPRISMComponents(clc.second)) {
        TH1 *comp = compspec.second.ToTHX(pot_fd);
        comp->SetTitle(";E_{#nu} (GeV);Pred. FD EvRate");
        comp->Write((std::string("PRISMPredEvRateMatch_") +
                     PredictionPRISM::GetComponentString(compspec.first))
                        .c_str());
      }

      TH1 *FarDet_h = projComp.FarDet->Predict(clc.second).ToTHX(pot_fd);

      for (int bin_it = 0; bin_it < FarDet_h->GetXaxis()->GetNbins();
           ++bin_it) {
        FarDet_h->SetBinError(bin_it + 1,
                              sqrt(FarDet_h->GetBinContent(bin_it + 1)));
      }

      FarDet_h->SetTitle(";E_{#nu} (GeV);FD EvRate");
      FarDet_h->Write("FarDet");

      TH1 *NearDet_h = projComp.NDInterp->Predict(clc.second).ToTHX(pot_fd);
      NearDet_h->SetTitle(";E_{#nu} (GeV);OffAxis;FD EvRate");
      NearDet_h->Write("NearDet");

      proj.PRISMEventRateMatcher.Write(pdir->mkdir("PRISMEventRateMatches"));
    }
  }

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

  std::cout << "[ERROR]: Failed to parse \"" << str << "\" as bool."
            << std::endl;
  throw std::runtime_error(str);
}

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    std::cout << "[ERROR]: Expect at least 2 arguments." << std::endl;
    return 1;
  }
  std::stringstream cli;

  for (size_t i = 0; i < argc; ++i) {
    cli << "\"" << argv[i] << "\"" << ((i + 1) != argc ? " " : "");
  }
  std::cout << cli.str() << std::endl;
  std::string off_axis_file = argv[1];
  std::string output_file = argv[2];
  bool isfhc = (argc >= 4) ? strToBool(argv[3]) : true;
  std::string state_file = (argc >= 5) ? argv[4] : "";
  bool reload = (argc >= 6) ? strToBool(argv[5]) : false;
  PRISMPrediction(off_axis_file, output_file, isfhc, state_file, reload);
}
#endif
