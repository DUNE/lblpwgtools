#pragma once

#include <cassert>
#include <memory>
#include <vector>

class TAxis;
class TDirectory;

namespace ana
{
  /// \brief Represent the binning of a Spectrum's x-axis
  ///
  /// Maybe be "Simple" (equally spaced) or "Custom" (arbitrary binning)
  class Binning
  {
  public:
    static Binning Simple(int n, double lo, double hi);
    static Binning LogUniform(int n, double lo, double hi);
    static Binning Custom(const std::vector<double>& edges);
    static Binning FromTAxis(const TAxis* ax);

    int NBins() const {return fNBins;}
    double Min() const {return fMin;}
    double Max() const {return fMax;}
    int FindBin(float x) const;
    bool IsSimple() const {return fEdges.empty();}
    const std::vector<double>& Edges() const
    {
      assert(!IsSimple());
      return fEdges;
    }

    void SaveTo(TDirectory* dir) const;
    static std::unique_ptr<Binning> LoadFrom(TDirectory* dir);
  protected:
    std::vector<double> fEdges;
    int fNBins;
    double fMin, fMax;
  };

  /// Default true-energy bin edges
  Binning TrueEnergyBins();
  /// Default true-energy bin edges
  const Binning kTrueEnergyBins = TrueEnergyBins();

  /// Default trueL Over true-energy bin edges
  Binning TrueLOverTrueEBins();
  /// Default true-energy bin edges
  const Binning kTrueLOverTrueEBins = TrueLOverTrueEBins();

  // These are just guesses
  // No optimization done for these binnings.
  const Binning kNueEnergyBinning  = Binning::Simple( 8, 0, 4 );
  const Binning kNumuEnergyBinning  = Binning::Simple( 20, 0, 5);
  const Binning kNCDisappearanceEnergyBinning = Binning::Simple(16, 0, 4);

  //Energy binnings used in first analysis for Nue Extrapolation
  const Binning kNueEnergyBinningFirstAna = Binning::Simple(20, 0, 5);
  const Binning kNumuEnergyBinningFirstAna = Binning::Simple(50, 0, 5);

  /// \brief Binning for plotting remid attractively
  ///
  /// There are 81 possible values, from 0/80 to 80/80. Want them centered in
  /// their bins.
  const Binning kRemidBinning = Binning::Simple(81, -1/160., 1+1/160.);

  /// \brief Binning for plotting qepid attractively
  ///
  /// There are 51 possible values, from 0/50 to 50/50. Want them centered in
  /// their bins.
  const Binning kQepidBinning = Binning::Simple(51, -1/100., 1+1/100.);

  /// The energy part of the SA 2D binning
  const Binning kNueSAEnergyBinning  = Binning::Simple( 10, 0, 5 );

  /// \brief Nonlinear binning for LEM
  ///
  /// There are 3 bins. The boundary of the highest one is set at the 
  /// value of the S/Sqrt(B) cut

  const Binning kLEMNLBinning = Binning::Custom({0.47,0.65,0.80,1});

  /// \brief Nonlinear binning for LID
  ///
  /// There are 3 bins. The boundary of the highest one is set at the 
  /// value of the S/Sqrt(B) cut

  const Binning kLIDNLBinning = Binning::Custom({0.63,0.8,0.95,2});
  
  /// \brief Nonlinear binning for CVN
  ///
  /// There are 3 bins. The boundary of the highest one is set at the 
  /// value of the S/Sqrt(B) cut

  const Binning kCVNNLBinning = Binning::Custom({0.75,0.87,0.95,1});

  /// 10 energy bins x 3 pid bins is 30 bins in 1D
  const Binning kNue2DBinning = Binning::Simple(30,0.0,30.0);

  /// 8 Cos theta bins. Theta is the angle between the muon and the incident
  /// neutrino. Based on angular resolution from  Numu CC Inclusive Cross 
  /// section studies
  const Binning kNumuCCIncluXSecCosThetaBinning = Binning::Custom({0.0, 0.75, 0.8, 0.85, 0.88, 0.91, 0.94, 0.97, 1.001});


  /// \brief Optimised binning for numuCCE from L. Vinton. See docdb 16332. This was close to 'custC' in that talk, with a slight modification agreed at the collaboration meeting

  const Binning kNumuCCEOptimisedBinning  = Binning::Custom( { 0, 0.75, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.25, 2.5, 2.75, 3, 3.5, 4, 5 } );


}
