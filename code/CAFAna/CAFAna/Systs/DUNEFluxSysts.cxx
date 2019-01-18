#include "CAFAna/Systs/DUNEFluxSysts.h"

#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include <cassert>

namespace ana {
//----------------------------------------------------------------------
DUNEFluxSyst::~DUNEFluxSyst() {
  for (int i : {0, 1})
    for (int j : {0, 1})
      for (int k : {0, 1})
        for (int l : {0, 1})
          delete fScale[i][j][k][l];
}

//----------------------------------------------------------------------
void DUNEFluxSyst::Shift(double sigma, Restorer &restore,
                         caf::StandardRecord *sr, double &weight) const {
  if (!fScale[0][0][0][0]) {
    TFile f((FindCAFAnaDir() + "/Systs/flux_shifts" +
             (fIncludeOffAxis ? "_wOffAxis" : "") + ".root")
                .c_str());
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
            if (fIncludeOffAxis && !det) {
              fScale2D[pdg][anti][hc] = dynamic_cast<TH2 *>(h);
              assert(fScale2D[pdg][anti][hc]);
            }
            h->SetDirectory(0);
          }
        }
      }
    }
  } // end if

  if (abs(sr->dune.nuPDGunosc) == 16)
    return;

  const int det = sr->dune.isFD ? 0 : 1;
  const int pdg = (abs(sr->dune.nuPDGunosc) == 12) ? 0 : 1;
  const int anti = (sr->dune.nuPDGunosc > 0) ? 0 : 1;
  const int hc = sr->dune.isFHC ? 0 : 1;

  double rel_weight = 1;

  if (fIncludeOffAxis && !sr->dune.isFD) {
    // ND is a TH2 where x is enu and y is absolute
    // off-axis position in m
    TH2 *h = fScale2D[pdg][anti][hc];
    assert(h);
    const int xbin = h->GetXaxis()->FindFixBin(sr->dune.Ev);
    const int ybin =
        h->GetYaxis()->FindFixBin(sr->dune.det_x + (sr->dune.vtx_x * 1E-2));

    if (xbin == 0 || xbin == h->GetXaxis()->GetNbins() + 1) {
      return;
    }
    if (ybin == 0 || ybin == h->GetYaxis()->GetNbins() + 1) {
      return;
    }
    rel_weight = h->GetBinContent(xbin, ybin);
  } else {
    TH1 *h = fScale[det][pdg][anti][hc];
    assert(h);
    const int bin = h->FindBin(sr->dune.Ev);
    if (bin == 0 || bin == h->GetNbinsX() + 1) {
      return;
    }
    rel_weight = h->GetBinContent(bin);
  }

  weight *= 1 + rel_weight * sigma;
}

//----------------------------------------------------------------------
const DUNEFluxSyst *GetDUNEFluxSyst(unsigned int i, bool applyPenalty,
                                    bool includeOffAxis) {
  // Make sure we always give the same one back
  static std::vector<const DUNEFluxSyst *> cache;
  if (i >= cache.size())
    cache.resize(i + 1);
  if (!cache[i])
    cache[i] = new DUNEFluxSyst(i, applyPenalty, includeOffAxis);
  return cache[i];
}

//----------------------------------------------------------------------
DUNEFluxSystVector GetDUNEFluxSysts(unsigned int N, bool applyPenalty,
                                    bool includeOffAxis) {
  DUNEFluxSystVector ret;
  for (unsigned int i = 0; i < N; ++i)
    ret.push_back(GetDUNEFluxSyst(i, applyPenalty, includeOffAxis));
  return ret;
}
} // namespace ana
