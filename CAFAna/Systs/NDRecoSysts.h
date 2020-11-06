// NDRecoSysts.h
// Systematics to simulate reconstruction systematics in the ND
#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Cuts/AnaCuts.h"

#include "StandardRecord/Proxy/SRProxy.h"

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
	       caf::SRProxy* sr, double& weight) const override;
  };
  extern const RecoNCSyst kRecoNCSyst;

  /// Systematic designed to mimic effect of lepton acceptance in the ND
  class LeptonAccSyst: public ISyst
  {
  public:
    LeptonAccSyst() : ISyst("LeptonAccSyst", "ND Lepton Acceptance Syst"), fHist(0) {}

    void Shift(double sigma,
	       caf::SRProxy* sr, double& weight) const override;
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
	       caf::SRProxy* sr, double& weight) const override;
  protected:
    mutable TH1* fHist;
  };
  extern const HadronAccSyst kHadronAccSyst;

  // Fiducial volume normalization for numus
  class FVNumuNDSyst: public ISyst
  {
  public:
  FVNumuNDSyst() : ISyst("FVNumuND", "Near Detector Numu Fiducial Volume") {}
    void Shift(double sigma, 
	       caf::SRProxy* sr,
	       double& weight) const override
    {
      const double scale = 1. + 0.01 * sigma;
      if (!sr->isFD && (kPassND_FHC_NUMU(sr) || kPassND_RHC_NUMU(sr))) {
	weight *= scale;
      }
    }
  };

  extern const FVNumuNDSyst kFVNumuNDSyst;

/*
  /// why have a structure (yet) if not needed
  /// make things simpler
  struct NDRecoSystVector: public std::vector<const ISyst*>
  {
  };
  NDRecoSystVector GetNDRecoSysts();
*/

  std::vector<const ISyst*> GetNDRecoSysts();
  
}
