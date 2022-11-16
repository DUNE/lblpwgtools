#pragma once

#include "CAFAna/Core/ISyst.h"

#include "TString.h"

class TH1;
class TH2;

namespace ana {
class SystShifts;

class DUNEFluxSyst : public ISyst {
public:
  virtual ~DUNEFluxSyst();

  virtual void Shift(double sigma, caf::SRProxy *sr,
                     double &weight) const override;

protected:
  friend const DUNEFluxSyst *GetDUNEFluxSyst(unsigned int, bool, bool, bool);
  DUNEFluxSyst(int i, bool applyPenalty, bool useCDR, bool useSept21)
      : ISyst(TString::Format("flux_%s_%i", 
                              useCDR ? "CDR" : 
                              (useSept21 ? "Sept21" : "Nov17"), i).Data(),
              TString::Format("Flux #%i (%s)", i, 
                              useCDR ? "CDR" : 
                              (useSept21 ? "Sept21" : "Nov17")).Data(),
              applyPenalty),
        fIdx(i), fScale(), fUseCDR(useCDR), fUseSept21(useSept21) {}

  int fIdx;

  mutable TH1 *fScale[2][2][2][2]; // ND/FD, numu/nue, bar, FHC/RHC

  bool fUseCDR;
  bool fUseSept21;
};

const DUNEFluxSyst *GetDUNEFluxSyst(unsigned int i, bool applyPenalty = true,
                                    bool useCDR = true, bool useSept21 = false);

// Because vector<T*> won't automatically convert to vector<U*> even when U
// inherits from V.
struct DUNEFluxSystVector : public std::vector<const DUNEFluxSyst *> {
  operator std::vector<const ISyst *>() {
    return std::vector<const ISyst *>(begin(), end());
  }
};

DUNEFluxSystVector GetDUNEFluxSysts(unsigned int N, bool applyPenalty = true,
                                    bool useCDR = true, bool useSept21 = false);

// instantiate them all at global scope so that they can be found in the
// Registry
extern const DUNEFluxSystVector kFluxSysts;

// Given a SystShifts, extract only those that are flux shifts
SystShifts FilterFluxSystShifts(SystShifts);

// Get the index of a given Flux syst
static const size_t kNotValidFluxSyst = std::numeric_limits<size_t>::max();
size_t GetFluxSystIndex(const ana::ISyst *syst);

} // namespace ana
