// CheckPRISMState.C -- quick sanity check of a (merged) PRISM state file.
//
//   root -l -b -q 'CheckPRISMState.C("prism_state_all.root", "EnuReco")'
//
// 1. Lists which of the directories MakePRISMPredInterps writes are present
//    or missing, per beam mode (nu = FHC, nub = RHC).
// 2. Compares the nominal 293 kA and 280 kA content of every ND object that
//    has a 280 kA version. MakePRISMPredInterps writes the 280 kA directories
//    even when no 280 kA event passed, so only their content tells you
//    whether 280 kA events made it in.
//
// Plain ROOT only (no CAFAna libraries needed), so it also works when the
// rootls / PyROOT command-line tools are broken.

#include "TDirectory.h"
#include "TFile.h"
#include "TH1.h"

#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

namespace {

// CAFAna layout: PredictionInterp/pred_nom/[PredictionNoOsc]spect/{hist,pot}.
// PRISMReweightableSpectrum keeps {hist,pot} directly in its own directory.
TDirectory *FindNominal(TDirectory *dir) {
  for (int depth = 0; dir && depth < 5; ++depth) {
    if (dir->GetKey("hist")) return dir;
    TDirectory *next = dir->GetDirectory("pred_nom");
    if (!next) next = dir->GetDirectory("spect");
    dir = next;
  }
  return nullptr;
}

// How trustworthy the stored per-bin errors are.
enum ErrState { kNoErrs, kBadErrs, kGoodErrs };

struct Content {
  bool found = false;
  double sum = 0;  // sum of weights over all bins (incl. under/overflow)
  double neff = 0; // effective number of MC events = (sum w)^2 / sum w^2
  double pot = 0;
  ErrState errs = kNoErrs;
};

Content GetContent(TFile &f, const std::string &path) {
  Content c;
  TDirectory *nom = FindNominal(f.GetDirectory(path.c_str()));
  if (!nom) return c;
  TH1 *h = nullptr, *hpot = nullptr;
  nom->GetObject("hist", h);
  nom->GetObject("pot", hpot);
  if (!h) return c;
  c.found = true;
  if (hpot) c.pot = hpot->GetBinContent(1);

  double sumw2 = 0;
  for (int i = 0; i < h->GetNcells(); ++i) { // any dimension
    c.sum += h->GetBinContent(i);
    sumw2 += std::pow(h->GetBinError(i), 2);
  }
  // No Sumw2 array: nothing was stored (e.g. ReweightableSpectrum, whose
  // errors PRISM rebuilds from the rate at load time). ROOT would otherwise
  // fall back to sqrt(content), which is meaningless for per-POT weights.
  if (h->GetSumw2N() == 0) return c;
  c.neff = (sumw2 > 0) ? c.sum * c.sum / sumw2 : 0;
  // For non-negative weights neff >= 1 whenever there is any content, so
  // anything else means the stored errors are corrupt (e.g. uninitialised
  // memory in CAFAnaCore's Hist::Fill), not that the histogram is empty.
  c.errs = (std::isfinite(sumw2) && (c.sum == 0 || c.neff >= 1)) ? kGoodErrs
                                                                 : kBadErrs;
  return c;
}

std::string Fmt(const Content &c, double v, const char *fmt) {
  if (!c.found) return "-";
  char buf[64];
  snprintf(buf, sizeof(buf), fmt, v);
  return buf;
}

std::string FmtNeff(const Content &c) {
  if (!c.found) return "-";
  if (c.errs == kNoErrs) return "no errs";
  if (c.errs == kBadErrs) return "bad errs";
  return Fmt(c, c.neff, "%.0f");
}

bool SameContent(const Content &a, const Content &b) {
  return a.found && b.found && a.sum != 0 &&
         std::fabs(a.sum - b.sum) <= 1e-9 * std::fabs(a.sum);
}

} // namespace

void CheckPRISMState(const char *fname, const char *var = "EnuReco") {
  TFile f(fname, "READ");
  if (f.IsZombie()) {
    printf("Cannot open %s\n", fname);
    return;
  }
  const std::string v = var;
  const std::string prism = "PRISM_" + v;
  const std::vector<std::string> modes = {"nu", "nub"};
  const std::vector<std::string> flavours = {"numu", "nue", "nutau"};

  // ---------------------------------------------------------------- 1
  printf("\n== Directories in %s (variable %s) ==\n", fname, var);
  if (!f.GetDirectory(prism.c_str()))
    printf("  MISSING %s -> wrong variable name, or no run was merged in\n",
           prism.c_str());

  for (const std::string &m : modes) {
    const bool nu = (m == "nu");
    const std::string nd_ch =
        nu ? "numu_ccinc_sel_numode" : "numubar_ccinc_sel_nubmode";

    std::vector<std::string> nd, fd;
    for (const std::string kA : {"293kA", "280kA"})
      for (const std::string obj :
           {"NDMatchInterp_ETrue_", "NDSelected_ETrue_", "NDUnSelected_ETrue_"})
        nd.push_back(obj + kA + "_" + m);
    nd.push_back("NDMatrixInterp_ERecETrue_" + m);
    for (const std::string pre : {"M_ND_", "M_ND_280kA_", "P_ND_", "P_ND_280kA_"})
      nd.push_back(prism + "/" + pre + nd_ch);

    fd.push_back("FDMatchInterp_ETrue_numu_" + m);
    for (const std::string &fl : flavours) {
      for (const std::string &obj : std::vector<std::string>{
               "FDMatrixInterp_ERecETrue_", "FDSelected_ETrue_",
               "FDUnSelected_ETrue_", "FDDataPred_" + v + "_",
               "FDFakeDataBiasPred_" + v + "_"})
        fd.push_back(obj + fl + "_" + m);
      fd.push_back(prism + "/P_FD_" + fl + (nu ? "" : "bar") + "_ccinc_sel_" +
                   (nu ? "numode" : "nubmode"));
    }

    for (auto *grp : {&nd, &fd}) {
      int n = 0;
      for (auto &x : *grp)
        if (f.GetDirectory(x.c_str())) ++n;
      printf("  %-3s %s: %2d/%zu present%s\n", m.c_str(),
             grp == &nd ? "ND" : "FD", n, grp->size(),
             n == 0 ? "  (this run is not in the file)" : "");
      if (n == 0) continue;
      for (auto &x : *grp)
        if (!f.GetDirectory(x.c_str())) printf("        missing: %s\n", x.c_str());
    }
  }

  // ---------------------------------------------------------------- 2
  printf("\n== ND 293 kA vs 280 kA, nominal (no syst shift) ==\n"
         "  sum  = sum of event weights. ND weights are per POT and include\n"
         "         per-file and slice-width factors, so the numbers are tiny and\n"
         "         NOT comparable between 293 and 280 kA. Empty means sum == 0.\n"
         "  neff = effective number of MC events, from the stored errors.\n"
         "         'no errs'  = none stored (normal for M_ND: PRISM rebuilds\n"
         "                      them from the rate when it loads the file)\n"
         "         'bad errs' = stored errors are corrupt; content is fine\n");

  for (const std::string &m : modes) {
    const std::string nd_ch = (m == "nu") ? "numu_ccinc_sel_numode"
                                          : "numubar_ccinc_sel_nubmode";
    struct Row {
      std::string label, p293, p280;
    };
    const std::vector<Row> rows = {
        {"NDMatchInterp_ETrue (flux-match input)",
         "NDMatchInterp_ETrue_293kA_" + m, "NDMatchInterp_ETrue_280kA_" + m},
        {"NDSelected_ETrue", "NDSelected_ETrue_293kA_" + m,
         "NDSelected_ETrue_280kA_" + m},
        {"NDUnSelected_ETrue", "NDUnSelected_ETrue_293kA_" + m,
         "NDUnSelected_ETrue_280kA_" + m},
        {"PRISM M_ND (ND 'data')", prism + "/M_ND_" + nd_ch,
         prism + "/M_ND_280kA_" + nd_ch},
        {"PRISM P_ND (ND MC)", prism + "/P_ND_" + nd_ch,
         prism + "/P_ND_280kA_" + nd_ch},
    };

    printf("\n  [%s]\n  %-40s %11s %9s %10s | %11s %9s %10s\n", m.c_str(),
           "object", "293kA sum", "neff", "POT", "280kA sum", "neff", "POT");
    bool any = false, match_empty = false, bad = false;
    for (const Row &r : rows) {
      const Content a = GetContent(f, r.p293), b = GetContent(f, r.p280);
      if (!a.found && !b.found) continue;
      any = true;
      const bool empty = b.found && b.sum == 0;
      if (empty && r.p293.rfind("NDMatchInterp", 0) == 0) match_empty = true;
      bad = bad || a.errs == kBadErrs || b.errs == kBadErrs;
      printf("  %-40s %11s %9s %10s | %11s %9s %10s%s\n", r.label.c_str(),
             Fmt(a, a.sum, "%.4g").c_str(), FmtNeff(a).c_str(),
             Fmt(a, a.pot, "%.3g").c_str(), Fmt(b, b.sum, "%.4g").c_str(),
             FmtNeff(b).c_str(), Fmt(b, b.pot, "%.3g").c_str(),
             empty ? "  <-- 280 kA EMPTY" : "");
    }
    if (!any) {
      printf("  (no ND objects for this beam mode)\n");
      continue;
    }
    printf(match_empty
               ? "  => no 280 kA events in the flux-matching input: the match "
                 "will use the 293 kA off-axis fluxes only.\n"
               : "  => 280 kA events are present in the flux-matching input.\n");
    if (bad)
      printf("  => some stored statistical errors are corrupt ('bad errs'). "
             "Bin contents are unaffected.\n");

    // With --UseSelection the selected spectrum must differ from the
    // unselected (true-signal) one; identical content means it was not used.
    if (SameContent(GetContent(f, "NDSelected_ETrue_293kA_" + m),
                    GetContent(f, "NDUnSelected_ETrue_293kA_" + m)))
      printf("  => NDSelected == NDUnSelected: the ND %s run was built "
             "WITHOUT --UseSelection (true-signal cuts).\n", m.c_str());
  }
  printf("\n");
}
