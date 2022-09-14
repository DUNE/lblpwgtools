#include "common_fit_definitions.C"

const Var kCMToM({}, [](const caf::SRProxy *) { return 1.0E-2; });

const Var kTrueDetPos_m = SIMPLEVAR(dune.det_x);
const Var kTrueVtxPos_m = SIMPLEVAR(dune.vtx_x);
const Var kTrueEv = SIMPLEVAR(dune.Ev);

const Binning binsDetPos = Binning::Simple(10, 0, 40);
const Binning binsVtxPos = Binning::Simple(12, -300, 300);
const Binning binsETrue_PRISM = Binning::Simple(100, 0, 10);

const HistAxis EffAxes({"True E_{#nu} (GeV)", "Vtx position (m)",
                        "Det position (m)"},
                       {binsETrue_PRISM, binsVtxPos, binsDetPos},
                       {kTrueEv, kTrueVtxPos_m, kTrueDetPos_m});

void PRISMEfficiency(std::string const &off_axis_file,
                     std::string const &output_state_file, bool isfhc = true) {

  // Now ND
  Loaders these_loaders;
  SpectrumLoader SpecLoader(off_axis_file, kBeam);
  these_loaders.AddLoader(&SpecLoader, caf::kNEARDET, Loaders::kMC);

  osc::NoOscillations nocalc;

  NoOscPredictionGenerator genSelNuMu(
      EffAxes, (isfhc ? kPassND_FHC_NUMU : kPassND_RHC_NUMU) && kIsTrueFV,
      kGENIEWeights);
  PredictionInterp interpSelNuMu({}, &nocalc, genSelNuMu, these_loaders);

  NoOscPredictionGenerator genFVNuMu(EffAxes, kIsTrueFV, kGENIEWeights);
  PredictionInterp interpFVNuMu({}, &nocalc, genFVNuMu, these_loaders);

  these_loaders.Go();
  TFile f(output_state_file.c_str(), "RECREATE");

  interpSelNuMu.SaveTo(f.mkdir(
      (std::string("prism_eff_corr_sel_") + std::string(isfhc ? "fhc" : "rhc"))
          .c_str()));

  interpFVNuMu.SaveTo(f.mkdir(
      (std::string("prism_eff_corr_all_") + std::string(isfhc ? "fhc" : "rhc"))
          .c_str()));

  f.Write();
}
