#include "CAFAna/Systs/DUNEFluxSysts.h"

#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/Proxy/SRProxy.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include <cassert>

namespace ana {
  const DUNEFluxSystVector kFluxSysts = GetDUNEFluxSysts(10);

//----------------------------------------------------------------------
DUNEFluxSyst::~DUNEFluxSyst() {
  for (int i : {0, 1})
    for (int j : {0, 1})
      for (int k : {0, 1})
        for (int l : {0, 1})
          delete fScale[i][j][k][l];
}

//----------------------------------------------------------------------
void DUNEFluxSyst::Shift(double sigma,
                         caf::SRProxy *sr, double &weight) const {
  if (!fScale[0][0][0][0]) {
    std::string InputFileName;
    if (fUseCDR) {
      // CDROpt flux
      InputFileName = "flux_shifts_CDR.root";
    } else {
      // Nov17 opt engineered
      InputFileName = "flux_shifts_Nov17.root";
    }

    TFile f((FindCAFAnaDir() + "/Systs/" + InputFileName).c_str());
    assert(!f.IsZombie());

    for (int det : {0, 1}) {
      const std::string detStr = (det == 0) ? "ND" : "FD";
      for (int pdg : {0, 1}) {
        std::string pdgStr = (pdg == 0) ? "nue" : "numu";
        for (bool anti : {false, true}) {
          if (anti)
            pdgStr += "bar";

          for (int hc : {0, 1}) {
            const std::string hcStr = (hc == 0) ? "FHC" : "RHC";

            TH1 *&h = fScale[det][pdg][anti][hc];

            h = (TH1 *)f.Get(TString::Format("syst%d/%s_%s_%s", fIdx,
                                             detStr.c_str(), pdgStr.c_str(),
                                             hcStr.c_str())
                                 .Data());
            h = (TH1 *)h->Clone(UniqueName().c_str());
            assert(h);
            h->SetDirectory(0);
          }
        }
      }
    }
  } // end if

  if (abs(sr->nuPDGunosc) == 16)
    return;

  const int det = sr->isFD ? 0 : 1;
  const int pdg = (abs(sr->nuPDGunosc) == 12) ? 0 : 1;
  const int anti = (sr->nuPDGunosc > 0) ? 0 : 1;
  const int hc = sr->isFHC ? 0 : 1;

  double rel_weight = 1;

  TH1 *h = fScale[det][pdg][anti][hc];
  assert(h);
  const int bin = h->FindBin(sr->Ev);
  if (bin == 0 || bin == h->GetNbinsX() + 1) {
    return;
  }
  rel_weight = h->GetBinContent(bin);

  weight *= 1 + rel_weight * sigma;
}

//----------------------------------------------------------------------
const DUNEFluxSyst *GetDUNEFluxSyst(unsigned int i, bool applyPenalty,
                                    bool useCDR) {
  // Make sure we always give the same one back
  static std::vector<const DUNEFluxSyst *> cache_CDR;
  static std::vector<const DUNEFluxSyst *> cache_Nov17;

  auto c = useCDR ? &cache_CDR : &cache_Nov17;
  if (i >= c->size()) {
    c->resize(i + 1);
  }
  if (!c->at(i)) {
    c->at(i) = new DUNEFluxSyst(i, applyPenalty, useCDR);
  }
  return c->at(i);
}

//----------------------------------------------------------------------
DUNEFluxSystVector GetDUNEFluxSysts(unsigned int N, bool applyPenalty,
                                    bool useCDR) {
  DUNEFluxSystVector ret;
  for (unsigned int i = 0; i < N; ++i)
    ret.push_back(GetDUNEFluxSyst(i, applyPenalty, useCDR));
  return ret;
}

} // namespace ana
