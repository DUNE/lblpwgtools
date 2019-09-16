#include "common_fit_definitions.C"

#include "CAFAna/PRISM/PRISMUtils.h"

const Var kTrueOffAxisPos =
    (SIMPLEVAR(dune.det_x) + (SIMPLEVAR(dune.vtx_x) * ana::Constant(1.0E-2)));
const Var kTrueEv = SIMPLEVAR(dune.Ev);

const Binning Energy_binning = Binning::Simple(300, 0, 30);

const double OA_bin_width_m_PRISM = 0.5;
const double OA_min_m_PRISM = -0.25;
const double OA_max_m_PRISM = 32.75;
const size_t NOABins_PRISM =
    (OA_max_m_PRISM - OA_min_m_PRISM) / OA_bin_width_m_PRISM;
const Binning OffAxis_binning_PRISM =
    Binning::Simple(NOABins_PRISM, OA_min_m_PRISM, OA_max_m_PRISM);

const HistAxis HistAxis_ERec_PRISM("E_{Dep.} (GeV)", Energy_binning,
                                   kRecoE_FromDep, "Off axis position (m)",
                                   OffAxis_binning_PRISM, kTrueOffAxisPos);
const HistAxis HistAxis_ETrue_PRISM("E_{#nu} (GeV)", Energy_binning, kTrueEv,
                                    "Off axis position (m)",
                                    OffAxis_binning_PRISM, kTrueOffAxisPos);

const double OA_bin_width_m_Stop = 4;
const double OA_min_m_Stop = -2;
const double OA_max_m_Stop = 34;
const size_t NOABins_Stop =
    (OA_max_m_Stop - OA_min_m_Stop) / OA_bin_width_m_Stop;
const Binning OffAxis_binning_Stop =
    Binning::Simple(NOABins_Stop, OA_min_m_Stop, OA_max_m_Stop);

const HistAxis HistAxis_ERec_Stop("E_{Dep.} (GeV)", Energy_binning,
                                  kRecoE_FromDep, "Off axis position (m)",
                                  OffAxis_binning_Stop, kTrueOffAxisPos);
const HistAxis HistAxis_ETrue_Stop("E_{#nu} (GeV)", Energy_binning, kTrueEv,
                                   "Off axis position (m)",
                                   OffAxis_binning_Stop, kTrueOffAxisPos);

struct ProjectionDef {
  ProjectionDef(std::string n, HistAxis const &a,
                Var const &e = ana::Constant(1))
      : Name(n), Ax(a), ExtraWeight(e) {}
  std::string Name;
  HistAxis const Ax;
  Var const ExtraWeight;
  std::unique_ptr<PredictionInterp> NDInterp;
  std::unique_ptr<PredictionInterp> NDInterp_NoSel;
  std::unique_ptr<PredictionInterp> NDInterp_NoSel_CVWeighted;
  std::unique_ptr<PredictionInterp> NDInterp_CVWeighted;
};

void OffAxisEventRates(std::string const &off_axis_file,
                       std::string const &output_file) {

  bool isfhc = true;
  osc::IOscCalculatorAdjustable *calc = NuFitOscCalc(1);
  std::vector<ana::ISyst const *> systlist = {
      GetMissingProtonEnergyFakeDataSyst().front()};

  SystShifts kMissProtEFD(GetMissingProtonEnergyFakeDataSyst().front(), 1);

  std::vector<ProjectionDef> Projections;
  Projections.emplace_back("ERec_PRISM", HistAxis_ERec_PRISM);
  Projections.emplace_back("ETrue_PRISM", HistAxis_ETrue_PRISM);
  Projections.emplace_back("ERec_Stop", HistAxis_ERec_Stop);
  Projections.emplace_back("ETrue_Stop", HistAxis_ETrue_Stop);

  Loaders TheLoaders;
  SpectrumLoader PRISMNDLoader(off_axis_file, kBeam);
  TheLoaders.AddLoader(&PRISMNDLoader, caf::kNEARDET, Loaders::kMC);

  std::vector<std::unique_ptr<NoOscPredictionGenerator>> NDPredGens;

  for (auto &proj : Projections) {

    ana::Cut kSelectSignalND =
        kPassND_FHC_NUMU && kIsTrueFV && kIsOutOfTheDesert;

    ana::Cut kSignalND =
        kIsNumuCC && !kIsAntiNu && kIsTrueFV && kIsOutOfTheDesert;

    ana::Var kNDWeight = kRunPlanWeight * kMassCorrection * proj.ExtraWeight;

    // Gotta make sure these hang around until  after Loaders::Go has been
    // called.
    NDPredGens.emplace_back(
        new NoOscPredictionGenerator(proj.Ax, kSelectSignalND, kNDWeight));

    proj.NDInterp = std::make_unique<PredictionInterp>(
        systlist, calc, *NDPredGens.back(), TheLoaders);

    NDPredGens.emplace_back(new NoOscPredictionGenerator(
        proj.Ax, kSelectSignalND, kNDWeight * kGENIEWeights));

    proj.NDInterp_CVWeighted = std::make_unique<PredictionInterp>(
        systlist, calc, *NDPredGens.back(), TheLoaders);

    NDPredGens.emplace_back(
        new NoOscPredictionGenerator(proj.Ax, kSignalND, kNDWeight));

    proj.NDInterp_NoSel = std::make_unique<PredictionInterp>(
        std::vector<ana::ISyst const *>{}, calc, *NDPredGens.back(),
        TheLoaders);

    NDPredGens.emplace_back(new NoOscPredictionGenerator(
        proj.Ax, kSignalND, kNDWeight * kGENIEWeights));

    proj.NDInterp_NoSel_CVWeighted = std::make_unique<PredictionInterp>(
        std::vector<ana::ISyst const *>{}, calc, *NDPredGens.back(),
        TheLoaders);
  }

  TheLoaders.Go();

  TFile f(output_file.c_str(), "RECREATE");

  for (auto &proj : Projections) {
    proj.NDInterp->GetPredNomAs<PredictionNoOsc>()->OverridePOT(1);
    proj.NDInterp_CVWeighted->GetPredNomAs<PredictionNoOsc>()->OverridePOT(1);
    proj.NDInterp_NoSel->GetPredNomAs<PredictionNoOsc>()->OverridePOT(1);
    proj.NDInterp_NoSel_CVWeighted->GetPredNomAs<PredictionNoOsc>()
        ->OverridePOT(1);

    auto pdir = f.mkdir(proj.Name.c_str());
    pdir->cd();

    TH1 *NearDet_h = proj.NDInterp->Predict(calc).ToTHX(POT120);
    NearDet_h->SetTitle(";E_{#nu} (GeV);OffAxis;ND EvRate");
    NearDet_h->Write("NearDet");

    TH1 *NearDet_WSB_h = proj.NDInterp
                             ->PredictComponent(calc, Flavors::kAllNuMu,
                                                Current::kCC, Sign::kAntiNu)
                             .ToTHX(POT120);
    NearDet_WSB_h->SetTitle(";E_{#nu} (GeV);OffAxis;ND EvRate");
    NearDet_WSB_h->Write("NearDet_WSB");

    TH1 *NearDet_NC_h = proj.NDInterp
                            ->PredictComponent(calc, Flavors::kAllNuMu,
                                               Current::kNC, Sign::kBoth)
                            .ToTHX(POT120);
    NearDet_NC_h->SetTitle(";E_{#nu} (GeV);OffAxis;ND EvRate");
    NearDet_NC_h->Write("NearDet_NC");

    TH1 *NearDet_cv_h = proj.NDInterp_CVWeighted->Predict(calc).ToTHX(POT120);
    NearDet_cv_h->SetTitle(";E_{#nu} (GeV);OffAxis;ND EvRate");
    NearDet_cv_h->Write("NearDet_cv");

    TH1 *NearDet_WSB_cv_h = proj.NDInterp_CVWeighted
                                ->PredictComponent(calc, Flavors::kAllNuMu,
                                                   Current::kCC, Sign::kAntiNu)
                                .ToTHX(POT120);
    NearDet_WSB_cv_h->SetTitle(";E_{#nu} (GeV);OffAxis;ND EvRate");
    NearDet_WSB_cv_h->Write("NearDet_WSB_cv");

    TH1 *NearDet_NC_cv_h = proj.NDInterp_CVWeighted
                               ->PredictComponent(calc, Flavors::kAllNuMu,
                                                  Current::kNC, Sign::kBoth)
                               .ToTHX(POT120);
    NearDet_NC_cv_h->SetTitle(";E_{#nu} (GeV);OffAxis;ND EvRate");
    NearDet_NC_cv_h->Write("NearDet_NC_cv");

    TH1 *NearDet_prote_h =
        proj.NDInterp->PredictSyst(calc, kMissProtEFD).ToTHX(POT120);
    NearDet_prote_h->SetTitle(";E_{#nu} (GeV);OffAxis;ND EvRate");
    NearDet_prote_h->Write("NearDet_prote");

    TH1 *NearDet_NumuCC_h = proj.NDInterp_NoSel->Predict(calc).ToTHX(POT120);
    NearDet_NumuCC_h->SetTitle(";E_{#nu} (GeV);OffAxis;ND EvRate");
    NearDet_NumuCC_h->Write("NearDet_NumuCC");

    TH1 *NearDet_NumuCC_cv_h =
        proj.NDInterp_NoSel_CVWeighted->Predict(calc).ToTHX(POT120);
    NearDet_NumuCC_cv_h->SetTitle(";E_{#nu} (GeV);OffAxis;ND EvRate");
    NearDet_NumuCC_cv_h->Write("NearDet_NumuCC_cv");
  }

  f.Write();
}

#ifndef __CINT__
int main(int argc, char const *argv[]) { OffAxisEventRates(argv[1], argv[2]); }
#endif
