#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

class TAxis;
class TDirectory;

namespace ana
{
  /// \brief Represent the binning of a Spectrum's x-axis
  ///
  /// May be "Simple" (equally spaced) or "Custom" (arbitrary binning)
  class Binning
  {
  public:
    static Binning Simple(int n, double lo, double hi,
                          const std::vector<std::string>& labels = {});
    static Binning LogUniform(int n, double lo, double hi);
    static Binning Custom(const std::vector<double>& edges);
    static Binning FromTAxis(const TAxis* ax);

    int NBins() const {return fNBins;}
    double Min() const {return fMin;}
    double Max() const {return fMax;}
    int FindBin(float x) const;
    bool IsSimple() const {return fIsSimple;}
    const std::vector<double>& Edges() const
    {
      return fEdges;
    }

    const std::vector<std::string>& Labels() const {return fLabels;}

    void SaveTo(TDirectory* dir, const std::string& name) const;
    static std::unique_ptr<Binning> LoadFrom(TDirectory* dir, const std::string& name);

    int ID() const {return fID;}
    static int MaxID() {return fgNextID-1;}

    bool operator==(const Binning& rhs) const;
    bool operator<(const Binning& rhs) const;
  protected:
    Binning();

    static Binning SimpleHelper(int n, double lo, double hi,
                                const std::vector<std::string>& labels = {});

    static Binning CustomHelper(const std::vector<double>& edges);

    std::vector<double> fEdges;
    std::vector<std::string> fLabels;
    int fNBins;
    double fMin, fMax;
    bool fIsSimple;

    int fID;
    /// The next ID that hasn't yet been assigned
    static int fgNextID;

    static std::map<Binning, int>& IDMap();
  };

  /// Default true-energy bin edges
  Binning TrueEnergyBins();
  std::vector<double> TrueEnergyBinCenters();

  /// Default true-energy bin edges
  const Binning kTrueEnergyBins = TrueEnergyBins();
  const std::vector<double> kTrueEnergyBinCenters = TrueEnergyBinCenters();
  
  /// LBL and ND analyses binning
  /// Eventually define as k{ND,FD}{Nue,Numu}Binning
  const std::vector<double> binEEdges = {0.,  0.5,  1.,  1.25, 1.5, 1.75,
                                   2.,  2.25, 2.5, 2.75, 3.,  3.25,
                                   3.5, 3.75, 4.,  5.,   6.,  10.};
  const std::vector<double> binYEdges = {0, 0.1, 0.2, 0.3, 0.4, 0.6, 1.0};
  const std::vector<double> binEEdges_v3 = {0.,  0.75, 1.,  1.25, 1.5, 1.75, 2., 2.25,
                                      2.5, 2.75, 3.,  3.25, 3.5, 3.75, 4., 4.25,
                                      4.5, 5.,   5.5, 6.,   7.,  8.,   10.};
  const std::vector<double> binYEdges_v3 = {0, 0.1, 0.2, 0.3, 0.4, 0.6, 0.8, 1.0};

  const Binning binsFDEreco = Binning::Custom(binEEdges);
  const Binning binsNDEreco = Binning::Custom(binEEdges);
  const Binning binsFDEreco_v3 = Binning::Custom(binEEdges_v3);
  const Binning binsNDEreco_v3 = Binning::Custom(binEEdges_v3);
  const Binning binsNDEreco_OA = Binning::Simple(20, 0, 4);
  const Binning binsY = Binning::Custom(binYEdges);
  const Binning binsY_v3 = Binning::Custom(binYEdges_v3);
  const Binning binsETrue = Binning::Simple(100, 0, 10);
  const Binning binsETrue_Coarse = Binning::Simple(20, 0, 10);
  const Binning binsEreco_Coarse = Binning::Simple(20, 0, 10);
  const Binning binsEreco_VeryCoarse = Binning::Simple(5, 0, 10);
  const Binning onebin = Binning::Simple(1, 0, 10);

//// git test no public stupidity

}
