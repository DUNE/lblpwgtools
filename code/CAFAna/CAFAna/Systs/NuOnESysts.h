// NuOnESysts.h
// Systematics specific to neutrino-electron scattering samples
#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/StandardRecord.h"
#include "CAFAna/Core/Utilities.h"

#include <cassert>

namespace ana {
  
  // Expect ~20% shape uncertainty on nu_e CC background to nu+e --> nu+e
  class NuOnECCBkgSyst: public ISyst
  {
  public:
  NuOnECCBkgSyst() : ISyst("NuOnECCBkgSyst", "nu_e CC background to nu+e") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(!sr->dune.isFD) { // ND only
        // apply unc. only to nu_e CC background, not NC gamma background
        if( (sr->dune.nuPDG == 12 || sr->dune.nuPDG == -12) && sr->dune.isCC && sr->dune.mode != 7 ) {
          // apply it to everything?  or just selected?
          //double reco_ehad = sr->dune.Ehad_veto; // re-using this variable for "extra energy" cut in nu+e
          //double eth2 = sr->dune.Elep_reco * sr->dune.Elep_reco * sr->dune.reco_theta; // E * theta^2
          //if( sr->dune.reco_nue && reco_ehad < 20. && eth2 < 0.002 ) {
          weight *= (1 + .2*sigma); // 20% uncertainty on shape (norm is constrained by sideband)
        }
      }
    }
  };
  extern const NuOnECCBkgSyst kNuOnECCBkgSyst;

  // Expect ~20% shape uncertainty on nu_e CC background to nu+e --> nu+e
  class NuOnENCBkgSyst: public ISyst
  {
  public:
  NuOnENCBkgSyst() : ISyst("NuOnENCBkgSyst", "NC pi0 background to nu+e") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(!sr->dune.isFD) { // ND only
        // apply unc. only to NC gamma background (and not nu-on-e)
        if( !sr->dune.isCC && sr->dune.mode != 7) {
          // apply it to everything?  or just selected?
          //double reco_ehad = sr->dune.Ehad_veto; // re-using this variable for "extra energy" cut in nu+e
          //double eth2 = sr->dune.Elep_reco * sr->dune.Elep_reco * sr->dune.reco_theta; // E * theta^2
          //if( sr->dune.reco_nue && reco_ehad < 20. && eth2 < 0.002 ) {
          weight *= (1 + .1*sigma); // 10% uncertainty on shape (norm is constrained by sideband)
        }
      }
    }
  };
  extern const NuOnENCBkgSyst kNuOnENCBkgSyst;

  // Signal reconstruction efficiency systematic
  class NuOnERecoEffSyst: public ISyst
  {
  public:
  NuOnERecoEffSyst() : ISyst("NuOnERecoEffSyst", "nu+e signal reconstruction") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(!sr->dune.isFD) { // ND only
        if( sr->dune.mode == 7 ) {
          weight *= (1 + 0.01*sigma); // 1% uncertainty on reco eff
        }
      }
    }
  };
  extern const NuOnERecoEffSyst kNuOnERecoEffSyst;

  struct NuOnESystVector: public std::vector<const ISyst*>
  {

  };
  NuOnESystVector GetNuOnESysts();

}
