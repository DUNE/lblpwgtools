#pragma once

#include "CAFAnaCore/CAFAna/Core/Binning.h"

namespace ana
{
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

  const std::vector<double> kPRISMBinEdges = {0.,  0.5,  1.,  1.25, 1.5, 1.75,
                                              2.,  2.25, 2.5, 2.75, 3.,  3.25,
                                              3.5, 3.75, 4., 4.5, 5., 5.5, 6., 
                                              8., 10., 12.5, 15.};

  /*const std::vector<double> kPRISMBinEdges = {0., 0.5, 0.75, 1., 1.125, 1.25, 1.375,
                                              1.5, 1.625, 1.75, 1.875, 2., 2.125, 2.25,
                                              2.375, 2.5, 2.625, 2.75, 2.875, 3., 3.125,
                                              3.25, 3.375, 3.5, 3.625, 3.75, 3.875, 4.,
                                              4.5, 5., 5.5, 6., 8., 10., 12.5, 15.};*/

  /// Both Nue and Numu use the same binning at the moment
  /// Eventually define as k{ND,FD}{Nue,Numu}Binning
  const Binning kFDRecoBinning = Binning::Custom(kBinEdges);
  const Binning kNDRecoBinning = Binning::Custom(kBinEdges);
  const Binning kPRISMRecoBinning = Binning::Custom(kPRISMBinEdges);
  const Binning kFDRecoV3Binning = Binning::Custom(kV3BinEdges);
  const Binning kNDRecoV3Binning = Binning::Custom(kV3BinEdges);
  const Binning kNDRecoOABinning = Binning::Simple(20, 0, 4);
  const Binning kYBinning = Binning::Custom(kYBinEdges);
  const Binning kYV3Binning = Binning::Custom(kYV3BinEdges);
  const Binning kTrueBinning = Binning::Simple(100, 0, 10);
  const Binning kTrueCoarseBinning = Binning::Simple(20, 0, 10);
  const Binning kRecoCoarseBinning = Binning::Simple(20, 0, 10);
  const Binning kRecoVeryCoarseBinning = Binning::Simple(5, 0, 10);
  const Binning kOneBinBinning = Binning::Simple(1, 0, 10);
}
