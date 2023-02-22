#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/SRProxy.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"

#include <cassert>

// Energy systematics that are 100% correclated between ND and FD

namespace ana
{
  // FD global energy scale syst still uncorrelated

  //------------------------------------------------------------------------------------------------

  // Slope energy scale systematics
  // Charged hadrons
  class CorrHadSqrt: public ISyst {
  public:
    CorrHadSqrt() : ISyst("CorrHadSqrt", "Correlated Sqrt Hadron Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const CorrHadSqrt kCorrHadSqrt;

  class CorrHadInvSqrt: public ISyst {
  public:
    CorrHadInvSqrt() : ISyst("CorrHadInvSqrt", "Correlated Inv Sqrt Hadron Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const CorrHadInvSqrt kCorrHadInvSqrt;

  //------------------------------------------------------------------------------------

  // Muons
  class CorrMuSqrt: public ISyst {
  public:
    CorrMuSqrt() : ISyst("CorrMuSqrt", "Correlated Sqrt Muon Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const CorrMuSqrt kCorrMuSqrt;

  class CorrMuInvSqrt: public ISyst {
  public:
    CorrMuInvSqrt() : ISyst("CorrMuInvSqrt", "Correlated Inv Sqrt Muon Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const CorrMuInvSqrt kCorrMuInvSqrt;

  //------------------------------------------------------------------------------------

  //Neutrons

  class CorrNSqrt: public ISyst {
  public:
    CorrNSqrt() : ISyst("CorrNSqrt", "Correlated Sqrt Neutron Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const CorrNSqrt kCorrNSqrt;

  class CorrNInvSqrt: public ISyst {
  public:
    CorrNInvSqrt() : ISyst("CorrNInvSqrt", "Correlated Inv Sqrt Neutron Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const CorrNInvSqrt kCorrNInvSqrt;

  //------------------------------------------------------------------------------------

  // Electromagnetic
  class CorrEMSqrt: public ISyst {
  public:
    CorrEMSqrt() : ISyst("CorrEMSqrt", "Correlated Sqrt EM Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const CorrEMSqrt kCorrEMSqrt;

  class CorrEMInvSqrt: public ISyst {
  public:
    CorrEMInvSqrt() : ISyst("CorrEMInvSqrt", "Correlated Inv Sqrt EM Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const CorrEMInvSqrt kCorrEMInvSqrt;

  //-----------------------------------------------------------------------------------

  // muon LAr systematic
  // 2% on CC numu events
  class EScaleMuLAr: public ISyst {
  public:
    EScaleMuLAr() : ISyst("EScaleMuLAr", "Muon Energy Scale LAr") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const EScaleMuLAr kEScaleMuLAr;

  // 5% correlated syst for charged hadrons
  class ChargedHadCorr: public ISyst {
  public:
    ChargedHadCorr() : ISyst("ChargedHadCorr", "Charged Hadron Correlated Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const ChargedHadCorr kChargedHadCorr;

  // Neutron energy scale
  // 20% on visible energy
  class NCorr: public ISyst {
  public:
    NCorr() : ISyst("NCorr", "Neutron Energy Scale") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const NCorr kNCorr;

  // systematic correlated for pi0s and electrons
  // 2.5% on reco energy for electrons and pi0s
  class EMCorr: public ISyst {
  public:
    EMCorr() : ISyst("EMCorr", "Electromagnetic shower Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const EMCorr kEMCorr;

  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Resolution systematics
  // Muon energy resolution
  class MuonRes: public ISyst {
  public:
    MuonRes() : ISyst("MuonRes", "Muon Resolution Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const MuonRes kMuonRes;

  // Electron/pi0 energy resolution
  class EMRes: public ISyst {
  public:
    EMRes() : ISyst("EMRes", "EM shower Resolution Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const EMRes kEMRes;

  // Charged hadron energy resolution
  class ChargedHadRes: public ISyst {
  public:
    ChargedHadRes() : ISyst("ChargedHadRes", "Charged Hadron Resolution Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const ChargedHadRes kChargedHadRes;

  // Neutron energy resolution
  class NRes: public ISyst {
  public:
    NRes() : ISyst("NRes", "Neutron Resolution Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const NRes kNRes;

  std::vector<const ISyst*> GetCorrEnergySysts();

}
