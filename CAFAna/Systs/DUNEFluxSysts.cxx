#include "CAFAna/Systs/DUNEFluxSysts.h"

#ifdef USE_TH2JAGGED
#include "CAFAna/Systs/OffAxisFluxUncertaintyHelper.h"
static ana::OffAxisFluxUncertaintyHelper const *fOffAxisFluxParamHelper =
    nullptr;
#include "CAFAna/Systs/NewOffAxisFluxUncertainty2022Helper.h"
static ana::NewOffAxisFluxUncertainty2022Helper const *fNewOffAxisFluxUncertainty2022Helper = 
    nullptr;
#endif

#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/SRProxy.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include <iostream>
#include <cassert>
#include <algorithm>

namespace ana {
const DUNEFluxSystVector kFluxSysts = GetDUNEFluxSysts(23);

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
                         caf::SRProxy *sr, double &weight) const {

  if (abs(sr->nuPDGunosc) == 16) {
    return;
  }

#ifdef USE_TH2JAGGED
  if (fUseCDR) {
    std::cout
        << "[ERROR]: Off axis flux uncertainties did not exist for the CDR."
        << std::endl;
    abort();
  }

  bool isSpecHCRun(false);
  if (std::abs(sr->SpecialHCRunId) != 293) isSpecHCRun = true;

  if (fUseSept21) {
    sr->OffAxis2022FluxBin = ana::NewOffAxisFluxUncertainty2022Helper::Get().GetBin(
        sr->nuPDGunosc, sr->Ev, sr->abspos_x * -1E-2, fIdx, !sr->isFD,
        sr->isFHC, isSpecHCRun);
    if (sr->OffAxis2022FluxConfig == -1) {
      sr->OffAxis2022FluxConfig = 
          ana::NewOffAxisFluxUncertainty2022Helper::Get().GetNuConfig_checked(
              sr->nuPDGunosc, sr->Ev, sr->abspos_x * -1E-2, fIdx, !sr->isFD,
              sr->isFHC, isSpecHCRun); 
    } 
    weight = fNewOffAxisFluxUncertainty2022Helper->GetFluxWeight(
        fIdx, sigma, sr->abspos_x * -1E-2, sr->OffAxis2022FluxBin, sr->OffAxis2022FluxConfig);
  } else {
    if (sr->OffAxisFluxBin == -1) {
      sr->OffAxisFluxBin = ana::OffAxisFluxUncertaintyHelper::Get().GetBin(
          sr->nuPDGunosc, sr->Ev, sr->abspos_x * -1E-2, 0, !sr->isFD,
          sr->isFHC, isSpecHCRun);
    }
    if (sr->OffAxisFluxConfig == -1) {
      sr->OffAxisFluxConfig = 
          ana::OffAxisFluxUncertaintyHelper::Get().GetNuConfig_checked(
              sr->nuPDGunosc, sr->Ev, sr->abspos_x * -1E-2, 0, !sr->isFD,
              sr->isFHC, isSpecHCRun);
    }
    weight = fOffAxisFluxParamHelper->GetFluxWeight(
        fIdx, sigma, sr->OffAxisFluxBin, sr->OffAxisFluxConfig);
  }
#else
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

  double /*const*/ rel_weight = 1;
  double const enu = sr->Ev;

  const int det = sr->isFD ? 0 : 1;
  const int pdg = (abs(sr->nuPDGunosc) == 12) ? 0 : 1;
  const int anti = (sr->nuPDGunosc > 0) ? 0 : 1;
  const int hc = sr->isFHC ? 0 : 1;

  TH1 *h = fScale[det][pdg][anti][hc];
  assert(h);

  const int bin = h->FindBin(enu);

  if (bin == 0 || bin == h->GetNbinsX() + 1) {
    return;
  }
  rel_weight = h->GetBinContent(bin);

  weight *= 1 + rel_weight * sigma;
#endif
}

//----------------------------------------------------------------------
const DUNEFluxSyst *GetDUNEFluxSyst(unsigned int i, bool applyPenalty,
                                    bool useCDR, bool useSept21) {
  // Make sure we always give the same one back
  static std::vector<const DUNEFluxSyst *> cache_CDR;
  static std::vector<const DUNEFluxSyst *> cache_Nov17;
  static std::vector<const DUNEFluxSyst *> cache_Sept21;

  auto c = useCDR ? &cache_CDR : (useSept21 ? &cache_Sept21 : &cache_Nov17);
  if (i >= c->size()) {
    c->resize(i + 1);
  }
  if (!c->at(i)) {
    c->at(i) = new DUNEFluxSyst(i, applyPenalty, useCDR, useSept21);
  }
  return c->at(i);
}

//----------------------------------------------------------------------
DUNEFluxSystVector GetDUNEFluxSysts(unsigned int N, bool applyPenalty,
                                    bool useCDR, bool useSept21) {

#ifdef USE_TH2JAGGED
  if (!fOffAxisFluxParamHelper) {
    fOffAxisFluxParamHelper = &ana::OffAxisFluxUncertaintyHelper::Get();
  }
  if (!fNewOffAxisFluxUncertainty2022Helper) {
    fNewOffAxisFluxUncertainty2022Helper = 
      &ana::NewOffAxisFluxUncertainty2022Helper::Get();
  }
#endif

  DUNEFluxSystVector ret;
  for (unsigned int i = 0; i < N; ++i) {
    ret.push_back(GetDUNEFluxSyst(i, applyPenalty, useCDR, useSept21));
  }
  return ret;
}

SystShifts FilterFluxSystShifts(SystShifts shift) {
  SystShifts outs;
  std::vector<ana::ISyst const *> fsysts =
      ana::GetDUNEFluxSysts(ana::kFluxSysts.size(), true, false, false);
  std::vector<ana::ISyst const *> fsysts_sept21 =
      ana::GetDUNEFluxSysts(ana::kFluxSysts.size(), true, false, true);
  fsysts.insert(fsysts.end(), fsysts_sept21.begin(), fsysts_sept21.end());

  for (auto syst : shift.ActiveSysts()) {
    if (std::find(fsysts.begin(), fsysts.end(), syst) != fsysts.end()) {
      outs.SetShift(syst, shift.GetShift(syst));
    }
  }
  return outs;
}

size_t GetFluxSystIndex(const ana::ISyst *syst) {
  std::vector<ana::ISyst const *> fsysts =
      ana::GetDUNEFluxSysts(ana::kFluxSysts.size(), true, false, false);
  std::vector<ana::ISyst const *> fsysts_sept21 =
      ana::GetDUNEFluxSysts(ana::kFluxSysts.size(), true, false, true);
  fsysts.insert(fsysts.end(), fsysts_sept21.begin(), fsysts_sept21.end());

  for (size_t i = 0; i < fsysts.size(); ++i) {
    if (fsysts[i] == syst) {
      return i;
    }
  }

  return kNotValidFluxSyst;
}

} // namespace ana
