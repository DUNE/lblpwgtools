#pragma once

#include <map>
#include <memory>
#include <vector>
#include <string>

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
    int FindBin(double x) const;
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
  /// No entries for underflow or overflow
  std::vector<double> TrueEnergyBinCenters();

  /// Default true-energy bin edges
  const Binning kTrueEnergyBins = TrueEnergyBins();
  /// No entries for underflow or overflow
  const std::vector<double> kTrueEnergyBinCenters = TrueEnergyBinCenters();
  
  /// LBL and ND analyses binnings
  const std::vector<double> kBinEdges = {0.,  0.5,  1.,  1.25, 1.5, 1.75,
                                   2.,  2.25, 2.5, 2.75, 3.,  3.25,
                                   3.5, 3.75, 4.,  5.,   6.,  10.};
  const std::vector<double> kYBinEdges = {0, 0.1, 0.2, 0.3, 0.4, 0.6, 1.0};
  const std::vector<double> kV3BinEdges = {0.,  0.75, 1.,  1.25, 1.5, 1.75, 2., 2.25,
                                      2.5, 2.75, 3.,  3.25, 3.5, 3.75, 4., 4.25,
                                      4.5, 5.,   5.5, 6.,   7.,  8.,   10.};
  const std::vector<double> kYV3BinEdges = {0, 0.1, 0.2, 0.3, 0.4, 0.6, 0.8, 1.0};

  /// Both Nue and Numu use the same binning at the moment
  /// Eventually define as k{ND,FD}{Nue,Numu}Binning
  const Binning kFDRecoBinning = Binning::Custom(kBinEdges);
  const Binning kNDRecoBinning = Binning::Custom(kBinEdges);
  const Binning kFDRecoV3Binning = Binning::Custom(kBinEdgesV3);
  const Binning kNDRecoV3Binning = Binning::Custom(kBinEdgesV3);
  const Binning kNDRecoOABinning = Binning::Simple(20, 0, 4);
  const Binning kYBinning = Binning::Custom(kYBinEdges);
  const Binning kYV3Binning = Binning::Custom(kYV3BinEdges);
  const Binning kTrueBinning = Binning::Simple(100, 0, 10);
  const Binning kTrueCoarseBinning = Binning::Simple(20, 0, 10);
  const Binning kRecoCoarseBinning = Binning::Simple(20, 0, 10);
  const Binning kRecoVeryCoarseBinning = Binning::Simple(5, 0, 10);
  const Binning kOneBinBinning = Binning::Simple(1, 0, 10);
}
