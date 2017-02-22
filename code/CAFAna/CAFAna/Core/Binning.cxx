#include "CAFAna/Core/Binning.h"

#include "TDirectory.h"
#include "TH1.h"
#include "TObjString.h"
#include "TVectorD.h"

namespace ana
{
  //----------------------------------------------------------------------
  Binning Binning::Simple(int n, double lo, double hi)
  {
    Binning bins;
    bins.fNBins = n;
    bins.fMin = lo;
    bins.fMax = hi;
    return bins;
  }

  //----------------------------------------------------------------------
  Binning Binning::LogUniform(int n, double lo, double hi)
  {
    Binning bins;
    bins.fNBins = n;
    bins.fMin = lo;
    bins.fMax = hi;
    bins.fEdges.resize(n+1);    
    double logSpacing = exp( (log(hi) - log(lo))/(n) ); 
    for (int i = 0; i <= n; ++i) {
      if (i == 0) bins.fEdges[i] = lo;
      else        bins.fEdges[i] = logSpacing*bins.fEdges[i-1];
    }
    return bins;
  }
  
  //----------------------------------------------------------------------
  Binning Binning::Custom(const std::vector<double>& edges)
  {
    assert(edges.size() > 1);

    Binning bins;
    bins.fEdges = edges;
    bins.fNBins = edges.size()-1;
    bins.fMin = edges.front();
    bins.fMax = edges.back();
    return bins;
  }

  int Binning::FindBin(float x) const
  {
    // Treat anything outside [fMin, fMax) at Underflow / Overflow
    if (x <  fMin) return 0;               // Underflow
    if (x >= fMax) return fEdges.size();   // Overflow

    // Follow ROOT convention, first bin of histogram is bin 1
    if (this->IsSimple()){
      double binwidth = (fMax - fMin) / fNBins;
      int bin = (x - fMin) / binwidth + 1;
      return bin;
    }

    int bin =
      std::lower_bound(fEdges.begin(), fEdges.end(), x) - fEdges.begin();
    if (x == fEdges[bin]) bin++;
    assert(bin >= 0 && bin < (int)fEdges.size());
    return bin;
  }

  //----------------------------------------------------------------------
  Binning Binning::FromTAxis(const TAxis* ax)
  {
    // Evenly spaced binning
    if(!ax->GetXbins()->GetArray())
      return Binning::Simple(ax->GetNbins(), ax->GetXmin(), ax->GetXmax());

    std::vector<double> edges(ax->GetNbins()+1);
    ax->GetLowEdge(&edges.front());
    edges[ax->GetNbins()] = ax->GetBinUpEdge(ax->GetNbins());

    return Binning::Custom(edges);
  }

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
  Binning TrueLOverTrueEBins()
  {
    // constant binnig
    
    const int kNumTrueLOverTrueEBins = 2000;
    const double klow = 0.0;
    const double khigh = 5.0;
    
    return Binning::Simple(kNumTrueLOverTrueEBins, klow, khigh);
  }

  //----------------------------------------------------------------------
  void Binning::SaveTo(TDirectory* dir) const
  {
    TDirectory* tmp = gDirectory;
    dir->cd();

    TObjString("Binning").Write("type");

    TVectorD nminmax(3);

    nminmax[0] = fNBins;
    nminmax[1] = fMin;
    nminmax[2] = fMax;

    nminmax.Write("nminmax");

    if(!fEdges.empty()){
      TVectorD edges(fEdges.size());
      for(unsigned int i = 0; i < fEdges.size(); ++i)
        edges[i] = fEdges[i];

      edges.Write("edges");
    }

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<Binning> Binning::LoadFrom(TDirectory* dir)
  {
    TObjString* tag = (TObjString*)dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "Binning");

    TVectorD* vMinMax = (TVectorD*)dir->Get("nminmax");
    assert(vMinMax);

    std::unique_ptr<Binning> ret(new Binning(Binning::Simple((*vMinMax)[0],
                                                             (*vMinMax)[1],
                                                             (*vMinMax)[2])));

    TVectorD* vEdges = (TVectorD*)dir->Get("edges");
    if(vEdges){
      for(int i = 0; i < vEdges->GetNrows(); ++i)
        ret->fEdges.push_back((*vEdges)[i]);
    }

    return ret;
  }
}
