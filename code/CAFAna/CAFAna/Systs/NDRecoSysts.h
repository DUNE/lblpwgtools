// NDRecoSysts.h
// Systematics to simulate reconstruction systematics in the ND
#pragma once

#include "CAFAna/Core/ISyst.h"

#include <vector>

class TH1;
class TH2;

namespace ana
{
  /// Take ND events which pass the CC selection cuts but are NC and reweight
  /// by 20%
  class RecoNCSyst: public ISyst
  {
  public:
    RecoNCSyst() : ISyst("RecoNCSyst", "ND Neutral Current Reconstruction Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override;
  };
  extern const RecoNCSyst kRecoNCSyst;

  /// Systematic designed to mimic effect of lepton acceptance in the ND
  class LeptonAccSyst: public ISyst
  {
  public:
    LeptonAccSyst() : ISyst("LeptonAccSyst", "ND Lepton Acceptance Syst"), fHist(0) {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override;
  protected:
    mutable TH2* fHist;
  };
  extern const LeptonAccSyst kLeptonAccSyst;

  /// Systematic designed to mimic effect of hadron acceptance in the ND
  class HadronAccSyst: public ISyst
  {
  public:
    HadronAccSyst() : ISyst("HadronAccSyst", "ND Hadron Acceptance Syst"), fHist(0) {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override;
  protected:
    mutable TH1* fHist;
  };
  extern const HadronAccSyst kHadronAccSyst;

  struct NDRecoSystVector: public std::vector<const ISyst*>
  {
  };
  NDRecoSystVector GetNDRecoSysts();
}
