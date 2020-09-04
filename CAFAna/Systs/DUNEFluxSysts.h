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

  virtual void Shift(double sigma, Restorer &restore, caf::StandardRecord *sr,
                     double &weight) const override;

protected:
  friend const DUNEFluxSyst *GetDUNEFluxSyst(unsigned int, bool, bool);
  DUNEFluxSyst(int i, bool applyPenalty, bool useCDR)
      : ISyst(TString::Format("flux_%s_%i", useCDR ? "CDR" : "Nov17", i).Data(),
              TString::Format("Flux #%i (%s)", i, useCDR ? "CDR" : "Nov17")
                  .Data(),
              applyPenalty),
        fIdx(i), fScale(), fUseCDR(useCDR) {}

  int fIdx;

  mutable TH1 *fScale[2][2][2][2]; // ND/FD, numu/nue, bar, FHC/RHC

  bool fUseCDR;
};

const DUNEFluxSyst *GetDUNEFluxSyst(unsigned int i, bool applyPenalty = true,
                                    bool useCDR = true);

// Because vector<T*> won't automatically convert to vector<U*> even when U
// inherits from V.
struct DUNEFluxSystVector : public std::vector<const DUNEFluxSyst *> {
  operator std::vector<const ISyst *>() {
    return std::vector<const ISyst *>(begin(), end());
  }
};

DUNEFluxSystVector GetDUNEFluxSysts(unsigned int N, bool applyPenalty = true,
                                    bool useCDR = true);

// instantiate them all at global scope so that they can be found in the
// Registry
extern const DUNEFluxSystVector kFluxSysts;

// Given a SystShifts, extract only those that are flux shifts
SystShifts FilterFluxSystShifts(SystShifts);

// Get the index of a given Flux syst
static const size_t kNotValidFluxSyst = std::numeric_limits<size_t>::max();
size_t GetFluxSystIndex(const ana::ISyst *syst);

} // namespace ana
