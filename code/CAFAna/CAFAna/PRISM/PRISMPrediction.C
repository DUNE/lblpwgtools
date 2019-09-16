#include "common_fit_definitions.C"

#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

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

  PRISMExtrapolator pfm;
  pfm.InitializeFluxMatcher(
      "DUNE_Flux_OffAxis_Nov2017Review_syst_shifts_fine.root", MergeFluxOABins);
  pfm.SetStoreDebugMatches();
  pfm.SetTargetConditioning(1E-8, 0.5, 3.5);

  osc::IOscCalculatorAdjustable *calc = NuFitOscCalc(1);
  std::vector<ana::ISyst const *> systlist = {};
  // {
  //     GetMissingProtonEnergyFakeDataSyst().front(),
  //     GetNuWroReweightFakeDataSyst().front()};

  std::map<std::string, PRISMComp> Predictions;

  // SystShifts kMissProtEFD(GetMissingProtonEnergyFakeDataSyst().front(), 1);
  // SystShifts kNuWroFD(GetNuWroReweightFakeDataSyst().front(), 1);

  std::vector<ProjectionDef> Projections;
  Projections.emplace_back("ETrue", trueEvAxes, ana::Constant(1));
  // Projections.emplace_back("ETrueNDEff", trueEvAxes, kNDEff);
  // Projections.emplace_back("ETrueNDFDEff", trueEvAxes, kNDEff * kFDEff);
  // Projections.emplace_back("ERecProxy", proxyEvAxes, ana::Constant(1));
  // Projections.emplace_back("ERecProxyEff", proxyEvAxes, kNDEff * kFDEff);
  // Projections.emplace_back("ERecProxyEff_20pclpe", proxy_20pclpeEvAxes,
  //                          kNDEff * kFDEff);
  Projections.emplace_back("ERecProxy", proxyEvAxes, ana::Constant(1));
  Projections.emplace_back("ERecProxy_20pclpe", proxy_20pclpeEvAxes,
                           ana::Constant(1));
  // Projections.emplace_back("ERecDep", ERecFromDepAxes, ana::Constant(1));
  // Projections.emplace_back("ELepTrue", ElepAxes, ana::Constant(1));

  if (reload || !state_file.length() || TFile(state_file.c_str()).IsZombie()) {
    Loaders TheLoaders;
    SpectrumLoader PRISMNDLoader(off_axis_file, kBeam);
    TheLoaders.AddLoader(&PRISMNDLoader, caf::kNEARDET, Loaders::kMC);
    SpectrumLoader loaderFDNumu(
        "/home/picker24/software/CAFAna/inputs/MCC11/mcc11_v3/" +
            GetSampleName(kFDFHC) + "_nonswap.root",
        kBeam);
    TheLoaders.AddLoader(&loaderFDNumu, caf::kFARDET, Loaders::kMC, ana::kBeam,
                         Loaders::kNonSwap);

    std::vector<std::unique_ptr<NoOscPredictionGenerator>> NDPredGens;
    std::vector<std::unique_ptr<NoExtrapPredictionGenerator>> FDPredGens;

    for (auto const &proj : Projections) {
      PRISMComp projComp;

      ana::Cut kSelectSignalND =
          kIsNumuCC && !kIsAntiNu && kIsTrueFV && kIsOutOfTheDesert;
      ana::Var kNDWeight = kRunPlanWeight * kMassCorrection * proj.ExtraWeight;

      projComp.PRISM = std::make_unique<PredictionPRISM>(
          PRISMNDLoader, proj.Ax, trueOffAxisPos, kSelectSignalND, kNDWeight);

      // Don't need to specify full truth signal here as it will not apply any
      // corrections by default, allows us to debug what the corrections would
      // be.
      // If you aren't doing systematic studies then this is just a waste of
      // time, but if you are you should pass a loader here and call
      // SetIgnoreData to use the 'MC' near detector signal prediction in the
      // linear combination.
      projComp.PRISM->AddNDMCLoader(TheLoaders, kIsTrueFV && kIsOutOfTheDesert,
                                    kNDWeight, systlist);

      projComp.PRISM->AddFDMCLoader(TheLoaders, kIsTrueFV, proj.ExtraWeight,
                                    systlist);

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
          NDSpectaAxis, kSelectSignalND, kNDWeight));

      ana::Cut kSelectSignalFD = kIsNumuCC && !kIsAntiNu && kIsTrueFV;

      FDPredGens.emplace_back(new NoExtrapPredictionGenerator(
          EventRateMatchAxis, kSelectSignalFD, proj.ExtraWeight));

      projComp.NDInterp = std::make_unique<PredictionInterp>(
          systlist, calc, *NDPredGens.back(), TheLoaders);
      projComp.FDInterp = std::make_unique<PredictionInterp>(
          systlist, calc, *FDPredGens.back(), TheLoaders);

      projComp.FarDet = std::make_unique<PredictionNoExtrap>(
          TheLoaders, proj.Ax, kSelectSignalFD, kNoShift, proj.ExtraWeight);

      Predictions[proj.Name] = std::move(projComp);
    }

    TheLoaders.Go();

    if (state_file.length()) {
      TFile fs(state_file.c_str(), "RECREATE");

      for (auto &projComp : Predictions) {
        projComp.second.PRISM->SaveTo(
            fs.mkdir((std::string("PRISM_") + projComp.first +
                      std::string(isfhc ? "_fhc" : "_rhc"))
                         .c_str()));

        projComp.second.NDInterp->GetPredNomAs<PredictionNoOsc>()->OverridePOT(
            1);
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

  TFile f(output_file.c_str(), "RECREATE");

  std::unique_ptr<osc::IOscCalculatorAdjustable> calc_extr(NuFitOscCalc(1));
  calc_extr->SetdCP(0.5 * M_PI);
  calc_extr->SetDmsq32(2.3E-3);

  std::unique_ptr<osc::IOscCalculatorAdjustable> calc_extr2(NuFitOscCalc(1));
  calc_extr2->SetdCP(-0.5 * M_PI);
  calc_extr2->SetDmsq32(2.6E-3);

  std::unique_ptr<osc::IOscCalculatorAdjustable> seta(NuFitOscCalc(1));
  seta->SetdCP(M_PI);
  seta->SetDmsq32(2.5E-3);
  seta->SetTh23(asin(sqrt(0.6)));

  std::unique_ptr<osc::IOscCalculatorAdjustable> setb(NuFitOscCalc(1));
  setb->SetdCP(-0.5 * M_PI);
  setb->SetDmsq32(2.6E-3);
  setb->SetTh23(asin(sqrt(0.56)));

  std::unique_ptr<osc::IOscCalculatorAdjustable> setc(NuFitOscCalc(1));
  setc->SetdCP(M_PI);
  setc->SetDmsq32(2.3E-3);
  setc->SetTh23(asin(sqrt(0.52)));

  std::vector<std::pair<std::string, osc::IOscCalculator *>> oscpars = {
      {"nufit", calc},
      {"nufit_lowdm23_halfpi", calc_extr.get()},
      {"nufit_highdm23_mhalfpi", calc_extr2.get()},
      {"seta", seta.get()},
      {"setb", setb.get()},
      {"setc", calc_extr2.get()},
  };

  for (auto clc : oscpars) {
    auto dir = f.mkdir(clc.first.c_str());
    dir->cd();

    for (auto &proj : Projections) {
      PRISMComp &projComp = Predictions[proj.Name];

      auto pdir = dir->mkdir(proj.Name.c_str());
      pdir->cd();

      projComp.PRISM->SetIgnoreData();

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

      // TH1 *PRISMPredFluxMatch_prote_h =
      //     projComp.PRISM->PredictSyst(clc.second,
      //     kMissProtEFD).ToTHX(pot_fd);
      // PRISMPredFluxMatch_prote_h->SetTitle(";E_{#nu} (GeV);Pred. FD EvRate");
      // PRISMPredFluxMatch_prote_h->Write("PRISMPredFluxMatch_prote");
      //
      // TH1 *PRISMPredFluxMatch_nuwro_h =
      //     projComp.PRISM->PredictSyst(clc.second, kNuWroFD).ToTHX(pot_fd);
      // PRISMPredFluxMatch_nuwro_h->SetTitle(";E_{#nu} (GeV);Pred. FD EvRate");
      // PRISMPredFluxMatch_nuwro_h->Write("PRISMPredFluxMatch_nuwro");

      int id = 0;
      for (double r : {3E-15, 4E-15, 5E-15, 6E-15, 7E-16}) {

        proj.PRISMEventRateMatcher.InitializeEventRateMatcher(
            Predictions["ETrue"].NDInterp.get(),
            Predictions["ETrue"].FDInterp.get());
        proj.PRISMEventRateMatcher.SetStoreDebugMatches();
        proj.PRISMEventRateMatcher.SetTargetConditioning(r, 0.5, 3.5);
        projComp.PRISM->SetFluxMatcher(&proj.PRISMEventRateMatcher);

        Spectrum PRISMPredEvRateMatchSpec = projComp.PRISM->Predict(clc.second);

        TH1 *PRISMPredEvRateMatch_h = PRISMPredEvRateMatchSpec.ToTHX(pot_fd);
        PRISMPredEvRateMatch_h->SetTitle(";E_{#nu} (GeV);Pred. FD EvRate");
        std::stringstream ss("");
        ss << "PRISMPredEvRateMatch_reg_" << id++;
        PRISMPredEvRateMatch_h->Write(ss.str().c_str());

        for (auto &compspec :
             projComp.PRISM->PredictPRISMComponents(clc.second)) {
          TH1 *comp = compspec.second.ToTHX(pot_fd);
          comp->SetTitle(";E_{#nu} (GeV);Pred. FD EvRate");
          comp->Write(
              (ss.str() + PredictionPRISM::GetComponentString(compspec.first))
                  .c_str());
        }
      }

      TH1 *FarDet_h = projComp.FarDet->Predict(clc.second).ToTHX(pot_fd);

      // TH1 *FarDet_prote_h =
      //     projComp.FDInterp->PredictSyst(clc.second, kMissProtEFD)
      //         .ToTHX(pot_fd);
      //
      // TH1 *FarDet_nuwro_h =
      //     projComp.FDInterp->PredictSyst(clc.second, kNuWroFD).ToTHX(pot_fd);

      for (int bin_it = 0; bin_it < FarDet_h->GetXaxis()->GetNbins();
           ++bin_it) {
        FarDet_h->SetBinError(bin_it + 1,
                              sqrt(FarDet_h->GetBinContent(bin_it + 1)));
        // FarDet_prote_h->SetBinError(
        //     bin_it + 1, sqrt(FarDet_prote_h->GetBinContent(bin_it + 1)));
        // FarDet_nuwro_h->SetBinError(
        //     bin_it + 1, sqrt(FarDet_nuwro_h->GetBinContent(bin_it + 1)));
      }

      FarDet_h->SetTitle(";E_{#nu} (GeV);FD EvRate");
      FarDet_h->Write("FarDet");
      // FarDet_prote_h->SetTitle(";E_{#nu} (GeV);FD EvRate");
      // FarDet_prote_h->Write("FarDet_prote");
      // FarDet_nuwro_h->SetTitle(";E_{#nu} (GeV);FD EvRate");
      // FarDet_nuwro_h->Write("FarDet_nuwro");

      TH1 *NearDet_h = projComp.NDInterp->Predict(clc.second).ToTHX(pot_fd);
      NearDet_h->SetTitle(";E_{#nu} (GeV);OffAxis;FD EvRate");
      NearDet_h->Write("NearDet");

      // TH1 *NearDet_prote_h =
      //     projComp.NDInterp->PredictSyst(clc.second, kMissProtEFD)
      //         .ToTHX(pot_fd);
      // NearDet_prote_h->SetTitle(";E_{#nu} (GeV);OffAxis;FD EvRate");
      // NearDet_prote_h->Write("NearDet_prote");
      //
      // TH1 *NearDet_nuwro_h =
      //     projComp.NDInterp->PredictSyst(clc.second, kNuWroFD).ToTHX(pot_fd);
      // NearDet_nuwro_h->SetTitle(";E_{#nu} (GeV);OffAxis;FD EvRate");
      // NearDet_nuwro_h->Write("NearDet_nuwro");

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
  gROOT->SetMustClean(false);

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
