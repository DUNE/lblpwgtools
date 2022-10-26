#include "CAFAna/Core/Binning.h"

#include <cassert>

#include "TDirectory.h"
#include "TH1.h"
#include "TObjString.h"
#include "TVectorD.h"

#include <memory>

namespace ana
{
  // Can we give trueE bin a special ID?
  //----------------------------------------------------------------------
  Binning TrueEnergyBins()
  {
    // Osc P is ~sin^2(1/E). Difference in prob across a bin is ~dP/dE which
    // goes as 1/E^2 times a trigonometric term depending on the parameters but
    // bounded between -1 and +1. So bins should have width ~1/E^2. E_i~1/i
    // achieves that.

    const int kNumTrueEnergyBins = 100;

    // N+1 bin low edges
    std::vector<double> edges(kNumTrueEnergyBins+1);

    const double Emin = .5; // 500 MeV: there's really no events below there

    // How many edges to generate. Allow room for 0-Emin bin
    const double N = kNumTrueEnergyBins-1;
    const double A = N*Emin;

    edges[0] = 0;

    for(int i = 1; i <= N; ++i){
      edges[kNumTrueEnergyBins-i] = A/i;
    }

    edges[kNumTrueEnergyBins] = 120; // Replace the infinity that would be here

    return Binning::Custom(edges);
  }

  //----------------------------------------------------------------------
  std::vector<double> TrueEnergyBinCenters()
  {
    std::vector<double> energies;
    std::vector<double> edges = TrueEnergyBins().Edges();

    for(std::size_t i = 0; i < edges.size()-1; i++) {
      energies.push_back((edges[i+1] + edges[i])/2);
    }

    return energies;
  }

  //----------------------------------------------------------------------
  Binning TrueLOverTrueEBins()
  {
    // constant binnig

    const int kNumTrueLOverTrueEBins = 2000;
    const double klow = 0.0;
    const double khigh = 5.0;

    return Binning::Simple(kNumTrueLOverTrueEBins, klow, khigh);
  }

  //----------------------------------------------------------------------
  Binning TrueEnergyCoarseBins()
  {
    std::vector<double> edges = TrueEnergyBins().Edges();

    // Same binning style just halved in size:
    std::vector<double> coarse_edges;
    coarse_edges.push_back(0); // First edge at 0.
    coarse_edges.push_back(edges[1]); // should be 0.5
    int x(0);
    for (size_t i = 2; i < edges.size(); ++i) {
      x++;
      if (x == 2) {
        coarse_edges.push_back(edges.at(i));
        x = 0;
      }
    }
    coarse_edges.push_back(120);

    return Binning::Custom(coarse_edges);
  }
}
