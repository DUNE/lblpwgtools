#pragma once

#include "CAFAna/Core/ISyst.h"

#include "TString.h"

class TH1;

namespace ana
{
  class DUNEFluxSyst: public ISyst
  {
  public:
    DUNEFluxSyst(int i) : fIdx(i), fScale() {}
    virtual ~DUNEFluxSyst();

    virtual std::string ShortName() const override {return TString::Format("flux%i", fIdx).Data();}
    virtual std::string LatexName() const override {return TString::Format("Flux ##%i", fIdx).Data();}

    virtual std::set<std::string> Requires() const override
    {
      return {"dune.beamPdg", "dune.Ev", "dune.run"};
    }
    
    virtual void Shift(double sigma,
                       Restorer& restore,
                       caf::StandardRecord* sr,
                       double& weight) const override;

  protected:
    int fIdx;

    mutable TH1* fScale[2][2][2][2]; // ND/FD, numu/nue, bar, FHC/RHC
  };

  const DUNEFluxSyst kFirstFluxSyst(0);
}
