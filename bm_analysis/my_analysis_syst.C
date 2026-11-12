#include <iostream>

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/Navigate.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Multiverse.h"
#include "CAFAna/Core/EnsembleSpectrum.h"
#include "TGraphAsymmErrors.h"

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TRatioPlot.h"
#include "TStyle.h"
#include "TLine.h"

#include <iomanip>
#include <cmath>
#include <fstream>

using namespace ana;

// parameter index in gl.wgts.flux_params, taken by matching the name

#include <fstream>
#include <iostream>
#include <map>
#include <string>

int FluxParamIndex(const std::string& fname,
                   const std::string& param_name) {

  static std::map<std::string, std::map<std::string, int>> cache;

  // read the json file the first time and store the parameter indices in a map
  if (!cache.count(fname)) {
    std::ifstream file(fname);

    if (!file.is_open()) {
      std::cerr << "Cannot open " << fname << std::endl;
      return -1;
    }

    auto& indices = cache[fname];
    std::string line;
    std::string name;
    while (std::getline(file, line)) {
      if (line.find("\": {") != std::string::npos) {
        size_t start = line.find('"') + 1;
        size_t end = line.find('"', start);
        name = line.substr(start, end - start);
      }
      else if (line.find("\"id\"") != std::string::npos) {
        int id;
        size_t pos = line.find(':');
        id = std::stoi(line.substr(pos + 1));
        indices[name] = id - 3000;
      }
    }
  }

  // each time the function is called, look up the parameter index in the cached map
  const auto& indices = cache.at(fname);
  auto it = indices.find(param_name);

  return (it != indices.end()) ? it->second : -1;
}

class FluxDialSyst : public ISyst {
  public:
    FluxDialSyst(const std::string& shortName, const std::string& longName, int idx) : ISyst(shortName, longName), fIdx(idx) {};
    
    void Shift(double sigma, caf::SRInteractionProxy* ixn,
           double& weight) const override {

      if (fIdx < 0 || ixn->truth.empty()) {
          std::cerr << "Error: Invalid flux parameter index or empty truth information." << std::endl;
          return;
      }

      const caf::SRProxy *top = ixn->Ancestor<caf::SRProxy>();
      if (!top) {
          std::cerr << "Error: Could not find ancestor SRProxy." << std::endl;
          return;
      }

      const caf::SRTrueInteractionProxy* nu = caf::FindInteraction(top->mc, ixn->truth[0]);
      if (!nu) {
          std::cerr << "Error: Could not find matching true interaction." << std::endl;
          return;
      }

      const auto& w = nu->flux_systs[fIdx].weights;
      // take the weight corresponding to the nominal value (w[0]) and apply the shift based on sigma
      weight *= w[0] + sigma * (w[1] - w[0]);

    }
  private:
    int fIdx;
};

const Var kRecoNuEnergy = SIMPLEVAR(Enu.calo);


void my_analysis_syst()
{
  const std::string fname_n = "/pnfs/dune/persistent/users/gsantoni/beam_monitoring/data/fast_reco/nominal/sand_events_nominal_FHC.caffluxsyst.sand.full.sandstt1.0000001.CAF.flat.root";
  
  // extrapolate the index of the systematics
  const std::string syst_name = "HornCurrent";
  const std::string json_file = "/exp/dune/app/users/gsantoni/beam_monitoring/lblpwgtools/bm_analysis/flux_systs.json";
  const int idx = FluxParamIndex(json_file, syst_name);
  if (idx < 0) {
    std::cerr << "Error: " << syst_name << " parameter not found in flux_params." << std::endl;
    return;
  }
  std::cout << syst_name << " parameter index: " << idx << std::endl;

  // build the systematics object in CAFAna
  const FluxDialSyst* kHornCurrentSyst = new FluxDialSyst("HornCurrent", "Horn Current", idx);
  std::vector<const ISyst*> systs = {kHornCurrentSyst};
  const Multiverse& cross = Multiverse::Hypercross(systs, 1);
  
  SpectrumLoader loader_n(fname_n);

  const HistAxis axRecoNuEnergy("Reco Nu Energy (GeV)", Binning::Simple(50,0,20), kRecoNuEnergy);
  
  EnsembleSpectrum sMuonEnsembleX(loader_n.Interactions(RecoType::kSandreco).Ensemble(cross), axRecoNuEnergy);

  loader_n.Go();

  const double pot_n = sMuonEnsembleX.POT();

  std::vector<ana::Spectrum> specs;

  specs.push_back(sMuonEnsembleX.Nominal());
  specs.push_back(sMuonEnsembleX.Universe(1));
  specs.push_back(sMuonEnsembleX.Universe(2));

  auto ensemble = EnsembleSpectrum::MergeSpectra(specs, &cross);

  // ************ ENERGY RECO SYSTS
  // Canvas with two pads
  auto c1_n = std::make_unique<TCanvas>("c1_n", "c1_n");

  auto h1_n = sMuonEnsembleX.Nominal().ToTH1(pot_n, kBlack);
  std::cout<< "nr universes: " << sMuonEnsembleX.NUniverses() << std::endl;
  auto h1_up = sMuonEnsembleX.Universe(1).ToTH1(pot_n, kRed);
  auto h1_down = sMuonEnsembleX.Universe(2).ToTH1(pot_n, kBlue);
  auto band = sMuonEnsembleX.ErrorBand(pot_n, kPOT);
  
  // Drawing
  h1_n->SetLineColor(kBlack);
  h1_n->SetLineStyle(1);
  h1_n->SetLineWidth(1);

  h1_up->SetLineColor(kRed);
  h1_up->SetLineStyle(1);
  h1_up->SetLineWidth(1);

  h1_down->SetLineColor(kBlue);
  h1_down->SetLineStyle(1);
  h1_down->SetLineWidth(1);

  // Make room for all histograms
  double ymax = std::max({
      h1_n->GetMaximum(),
      h1_up->GetMaximum(),
      h1_down->GetMaximum()
  });

  h1_n->SetMaximum(ymax * 1.2);

  h1_n->Draw("HIST");
  h1_up->Draw("HIST SAME");
  h1_down->Draw("HIST SAME");

  c1_n->SaveAs("workspace/energy_reco_systs.png");

}
  

