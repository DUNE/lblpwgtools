#pragma once

#include "CAFAna/Core/ISyst.h"

#include "TString.h"

class TH1;

namespace ana
{
  class DUNEFluxSyst: public ISyst
  {
  public:
    virtual ~DUNEFluxSyst();

    virtual std::string ShortName() const override {return TString::Format("flux%i", fIdx).Data();}
    virtual std::string LatexName() const override {return TString::Format("Flux #%i", fIdx).Data();}
    
    virtual void Shift(double sigma,
                       Restorer& restore,
                       caf::StandardRecord* sr,
                       double& weight) const override;

  protected:
    friend const DUNEFluxSyst* GetDUNEFluxSyst(unsigned int i);
    DUNEFluxSyst(int i) : fIdx(i), fScale() {}

    int fIdx;

    mutable TH1* fScale[2][2][2][2]; // ND/FD, numu/nue, bar, FHC/RHC
  };

  const DUNEFluxSyst* GetDUNEFluxSyst(unsigned int i);

  // Because vector<T*> won't automatically convert to vector<U*> even when U
  // inherits from V.
  struct DUNEFluxSystVector: public std::vector<const DUNEFluxSyst*>
  {
    operator std::vector<const ISyst*>()
    {
      return std::vector<const ISyst*>(begin(), end());
    }    
  };

  DUNEFluxSystVector GetDUNEFluxSysts(unsigned int N);
}
