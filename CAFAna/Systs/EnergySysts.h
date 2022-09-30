#pragma once

#include "CAFAna/Core/ISyst.h"
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"

#include <cassert>

namespace ana
{
  // FD global energy scale syst
  // Don't shift muon energies with this
  class EnergyScaleFD: public ISyst {
  public:
    EnergyScaleFD() : ISyst("EnergyScaleFD", "Global FD Energy Scale Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const EnergyScaleFD kEnergyScaleFD;

  // Total energy scale shape systematics
  class UncorrFDTotSqrt: public ISyst {
  public:
    UncorrFDTotSqrt() : ISyst("UncorrFDTotSqrt", "Uncorrelated FD Sqrt Total Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const UncorrFDTotSqrt kUncorrFDTotSqrt;

  class UncorrFDTotInvSqrt: public ISyst {
  public:
    UncorrFDTotInvSqrt() : ISyst("UncorrFDTotInvSqrt", "Uncorrelated FD Inverse Sqrt Total Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const UncorrFDTotInvSqrt kUncorrFDTotInvSqrt;

  //------------------------------------------------------------------------------------------------

  // FD three parameter systematics to match ND ones
  // Slope energy scale systematics
  // Charged hadrons
  class UncorrFDHadSqrt: public ISyst {
  public:
    UncorrFDHadSqrt() : ISyst("UncorrFDHadSqrt", "Uncorrelated FD Sqrt Hadron Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const UncorrFDHadSqrt kUncorrFDHadSqrt;

  class UncorrFDHadInvSqrt: public ISyst {
  public:
    UncorrFDHadInvSqrt() : ISyst("UncorrFDHadInvSqrt", "Uncorrelated FD Inv Sqrt Hadron Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const UncorrFDHadInvSqrt kUncorrFDHadInvSqrt;

  //------------------------------------------------------------------------------------

  // Muons
  class UncorrFDMuSqrt: public ISyst {
  public:
    UncorrFDMuSqrt() : ISyst("UncorrFDMuSqrt", "Uncorrelated FD Sqrt Muon Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const UncorrFDMuSqrt kUncorrFDMuSqrt;

  class UncorrFDMuInvSqrt: public ISyst {
  public:
    UncorrFDMuInvSqrt() : ISyst("UncorrFDMuInvSqrt", "Uncorrelated FD Inv Sqrt Muon Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const UncorrFDMuInvSqrt kUncorrFDMuInvSqrt;

  //------------------------------------------------------------------------------------

  //Neutrons

  class UncorrFDNSqrt: public ISyst {
  public:
    UncorrFDNSqrt() : ISyst("UncorrFDNSqrt", "Uncorrelated FD Sqrt Neutron Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const UncorrFDNSqrt kUncorrFDNSqrt;

  class UncorrFDNInvSqrt: public ISyst {
  public:
    UncorrFDNInvSqrt() : ISyst("UncorrFDNInvSqrt", "Uncorrelated FD Inv Sqrt Neutron Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const UncorrFDNInvSqrt kUncorrFDNInvSqrt;

  //------------------------------------------------------------------------------------

  // Electromagnetic
  class UncorrFDEMSqrt: public ISyst {
  public:
    UncorrFDEMSqrt() : ISyst("UncorrFDEMSqrt", "Uncorrelated FD Sqrt EM Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const UncorrFDEMSqrt kUncorrFDEMSqrt;

  class UncorrFDEMInvSqrt: public ISyst {
  public:
    UncorrFDEMInvSqrt() : ISyst("UncorrFDEMInvSqrt", "Uncorrelated FD Inv Sqrt EM Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const UncorrFDEMInvSqrt kUncorrFDEMInvSqrt;

  // FD muon LAr systematic
  // 2% on CC numu events
  class EScaleMuLArFD: public ISyst {
  public:
    EScaleMuLArFD() : ISyst("EScaleMuLArFD", "Muon Energy Scale LAr Far Detector") {}

    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const EScaleMuLArFD kEScaleMuLArFD;

  // 5% uncorrelated FD syst for charged hadrons
  class ChargedHadUncorrFD: public ISyst {
  public:
    ChargedHadUncorrFD() : ISyst("ChargedHadUncorrFD", "Charged Hadron Uncorrelated FD Syst") {}

    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const ChargedHadUncorrFD kChargedHadUncorrFD;

  // Neutron energy scale
  // 20% on visible energy
  class NUncorrFD: public ISyst {
  public:
    NUncorrFD() : ISyst("NUncorrFD", "Neutron Energy Scale FD") {}

    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const NUncorrFD kNUncorrFD;

  // FD systematic correlated for pi0s and electrons
  // 2.5% on reco energy for electrons and pi0s
  class EMUncorrFD: public ISyst {
  public:
    EMUncorrFD() : ISyst("EMUncorrFD", "Electromagnetic shower FD Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const EMUncorrFD kEMUncorrFD;

  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Resolution systematics
  // Muon energy resolution
  class MuonResFD: public ISyst {
  public:
    MuonResFD() : ISyst("MuonResFD", "Muon Far Detector Resolution Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const MuonResFD kMuonResFD;

  // Electron/pi0 energy resolution
  class EMResFD: public ISyst {
  public:
    EMResFD() : ISyst("EMResFD", "EM shower Far Detector Resolution Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const EMResFD kEMResFD;

  // Charged hadron energy resolution
  class ChargedHadResFD: public ISyst {
  public:
    ChargedHadResFD() : ISyst("ChargedHadResFD", "Charged Hadron Far Detector Resolution Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const ChargedHadResFD kChargedHadResFD;

  // Neutron energy resolution
  class NResFD: public ISyst {
  public:
    NResFD() : ISyst("NResFD", "Neutron Far Detector Resolution Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override;
  };

  extern const NResFD kNResFD;

/*
  /// why have a structure (yet) if not needed
  /// make things simpler
  // Vector of energy scale systematics
  struct EnergySystVector: public std::vector<const ISyst*>
  {
  };
  EnergySystVector GetEnergySysts();
*/

  std::vector<const ISyst*> GetEnergySysts();

  std::vector<ana::ISyst const *> FilterOutFDSysts(std::vector<ana::ISyst const *> systs);
}
