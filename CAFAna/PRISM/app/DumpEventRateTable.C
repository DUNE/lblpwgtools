#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include <cassert>

double TPOT = 1.1e21;
double NTotalWeeks = 29;
double NWeeksOnAxis = 14;

enum EComponent {
  kAll = 0,
  kWSB,
  kNCB,
  kNumuCC,
  kAll_cv,
  kWSB_cv,
  kNCB_cv,
  kNumuCC_cv
};
std::vector<EComponent> EComponentVect = {
    kAll, kWSB, kNCB, kNumuCC, kAll_cv, kWSB_cv, kNCB_cv, kNumuCC_cv};
std::vector<std::string> EComponentStr = {
    "NearDet_select_AnaWeighted",       "NearDet_select_AnaWeighted_WSB",
    "NearDet_select_AnaWeighted_NC",    "NearDet_signal_AnaWeighted",
    "NearDet_select_CVXSecWeighted",    "NearDet_select_CVXSecWeighted_WSB",
    "NearDet_select_CVXSecWeighted_NC", "NearDet_signal_CVXSecWeighted",
};

std::string EvRateString(double i) {
  std::stringstream ss("");
  ss.precision(1);
  ss << std::scientific << i;
  return ss.str();
}

bool HasSpecRun(TH1 *h) {
  TH2D *h2 = dynamic_cast<TH2D *>(h);
  assert(h2);
  return (h2->GetYaxis()->GetBinWidth(1) > h2->GetYaxis()->GetBinWidth(2)) &&
         (h2->GetYaxis()->GetBinCenter(1) < 0);
}

TH1D *GetNDOnAxis(TH1 *h) {
  TH2D *h2 = dynamic_cast<TH2D *>(h);
  assert(h2);

  bool hsr = HasSpecRun(h);

  int lowbin = hsr ? 2 : 1;
  int upbin = hsr ? 5 : 8;

  std::cout << "On axis from [" << lowbin << "("
            << h2->GetYaxis()->GetBinLowEdge(lowbin) << "m)--" << upbin << "("
            << h2->GetYaxis()->GetBinUpEdge(upbin) << "m)]." << std::endl;

  TH1D *rtn = h2->ProjectionX((std::string(h->GetName()) + "_OnAxis").c_str(),
                              lowbin, upbin);
  rtn->GetYaxis()->SetTitle(h->GetZaxis()->GetTitle());
  rtn->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
  rtn->Scale(double(hsr ? 2 : 1));
  rtn->SetDirectory(nullptr);
  return rtn;
}

TH1D *GetNDOnAxisSpecRun(TH1 *h) {
  TH2D *h2 = dynamic_cast<TH2D *>(h);
  assert(h2);

  bool hsr = HasSpecRun(h);
  if (!hsr) {
    std::cout << "Tried to get spec run prediction when it doesn't exist."
              << std::endl;
    abort();
  }

  int lowbin = 1;
  int upbin = 1;

  std::cout << "Spec run from [" << lowbin << "("
            << h2->GetYaxis()->GetBinLowEdge(lowbin) << "m)--" << upbin << "("
            << h2->GetYaxis()->GetBinUpEdge(upbin) << "m)]." << std::endl;

  TH1D *rtn = h2->ProjectionX(
      (std::string(h->GetName()) + "_OnAxisSpecRun").c_str(), lowbin, upbin);
  rtn->GetYaxis()->SetTitle(h->GetZaxis()->GetTitle());
  rtn->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
  rtn->Scale(double(hsr ? 2 : 1));
  rtn->SetDirectory(nullptr);
  return rtn;
}

TH1D *GetNDStop(TH1 *h, double i) {
  TH2D *h2 = dynamic_cast<TH2D *>(h);
  assert(h2);

  if ((i <= 0)) {
    std::cout << "Only use GetNDStop to get an off axis prediction."
              << std::endl;
    abort();
  }

  bool hsr = HasSpecRun(h);

  int stop = i / 4;

  int lowbin = h2->GetYaxis()->FindFixBin(i - 2);
  int upbin = lowbin + 7;

  std::cout << "Stop " << stop << "(" << i << "m) from [" << lowbin << "("
            << h2->GetYaxis()->GetBinLowEdge(lowbin) << "m)--" << upbin << "("
            << h2->GetYaxis()->GetBinUpEdge(upbin) << "m)]." << std::endl;

  TH1D *rtn = h2->ProjectionX(
      (std::string(h->GetName()) + "_OffAxis_" + std::to_string(i)).c_str(),
      lowbin, upbin);
  rtn->GetYaxis()->SetTitle(h->GetZaxis()->GetTitle());
  rtn->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
  rtn->SetDirectory(nullptr);
  return rtn;
}

std::pair<double, double> GetMax(TH1 *h, double maxX) {
  double max = -std::numeric_limits<double>::max();
  double emax = 0;

  for (int i = 0; i < h->GetXaxis()->GetNbins(); ++i) {
    double e = h->GetXaxis()->GetBinCenter(i + 1);
    if (e > maxX) {
      break;
    }
    if (max < h->GetBinContent(i + 1)) {
      max = h->GetBinContent(i + 1);
      emax = e;
    }
  }
  return {max, emax};
}

struct EvRateLine {
  std::string OffAxisPosition;
  double RunFraction;
  double NNumuCC;
  double NSel_tot;
  double NSel_WSB;
  double NSel_NC;
  double NGas;
};

double vol_gas_cm3 = 400 * 200 * 200 * M_PI;
double density_gas_kg_cm3 = 1.784E-6 /*kg/cm3*/ * 10 /*bar*/;

double vol_liquid_stop_cm3 = 400 * 200 * 300;
double density_liquid_kg_cm3 = 1.3954 /*g/cm3*/ * 1E-3;

double StopToGasMassRatio = (vol_gas_cm3 * density_gas_kg_cm3) /
                            (vol_liquid_stop_cm3 * density_liquid_kg_cm3);

std::vector<EvRateLine> EventRateTable;

std::string Commerize(double d) {
  std::stringstream ss("");
  // I know
  if (d >= 1E6) {
    ss << (round(d * 1E-5) * 1E-1) << "M";
    return ss.str();
  } else if (d > 1E5) {
    ss << int(round(d * 1E-4) * 1E4);
  } else if (d > 1E4) {
    ss << int(round(d * 1E-3) * 1E3);
  } else {
    ss << int(round(d * 1E-2) * 1E2);
  }

  std::string s = ss.str();

  std::stringstream ss2("");
  for (size_t i = 0; i < s.length(); ++i) {
    ss2 << s[s.length() - i - 1];
    if (i && (i != (s.length() - 1)) && (!((i + 1) % 3))) {
      ss2 << ",";
    }
  }
  std::string s2 = ss2.str();
  std::stringstream ss3("");
  for (size_t i = 0; i < s2.length(); ++i) {
    ss3 << s2[s2.length() - i - 1];
  }
  return ss3.str();
}

void WriteTable(std::ofstream &tbl, std::vector<EvRateLine> &ert) {
  tbl << "\\begin{tabular}{|c|c|c|c|c|c|c|}\n";
  tbl << " \\multicolumn{2}{}{} & \\multicolumn{4}{|c|}{Liquid} & "
         "Gas\\\\\n\\hline\n";
  tbl << " \\multicolumn{2}{c|}{} & All int. &\\multicolumn{3}{|c|}{Selected} "
         "& All int. \\\\\n\\hline\n";
  tbl << "Stop&Run duration&N$\\nu_{\\mu}$CC & NSel & "
         "WSB & NC &N$\\nu_{\\mu}$CC "
         "\\\\\n\\hline\n";

  for (auto e : ert) {
    tbl.precision(4);

    double nweeks = (double(NTotalWeeks) * e.RunFraction);

    std::cout << e.OffAxisPosition << ", " << NTotalWeeks << " * "
              << e.RunFraction << " = " << nweeks << std::endl;
    if (round(nweeks) != nweeks) {
      tbl << e.OffAxisPosition << "\\,m & " << int(nweeks * 7) << " dys. ";
    } else {
      tbl << e.OffAxisPosition << "\\,m & " << int(nweeks) << " wk"
          << (nweeks > 1 ? "s" : "") << ". ";
    }

    tbl << "& " << Commerize(e.NNumuCC) << " & " << Commerize(e.NSel_tot)
        << " & ";
    tbl.precision(1);
    tbl << std::fixed << (e.NSel_WSB / e.NSel_tot) * 1.0E2 << "\\% & "
        << (e.NSel_NC / e.NSel_tot) * 1.0E2 << "\\% & " << Commerize(e.NGas)
        << "\\\\\n";
  }

  tbl << "\\end{tabular}";
}

void DumpEventRateTable(char const *OffAxisRateFile) {
  std::map<EComponent, TH1 *> Hists;

  TFile *f = new TFile(OffAxisRateFile);

  for (EComponent c : EComponentVect) {
    Hists[c] = dynamic_cast<TH1 *>(f->Get(EComponentStr[c].c_str()));

    if (!Hists[c]) {
      std::cout << "[ERROR]: Failed to get " << EComponentStr[c] << std::endl;
      throw;
    }

    Hists[c]->SetDirectory(nullptr);
  }

  double NumuCC_GENIETov3Scale = GetNDOnAxis(Hists[kNumuCC_cv])->Integral() /
                                 GetNDOnAxis(Hists[kNumuCC])->Integral();
  double Sel_GENIETov3Scale = GetNDOnAxis(Hists[kAll_cv])->Integral() /
                              GetNDOnAxis(Hists[kAll])->Integral();
  double WSB_GENIETov3Scale = GetNDOnAxis(Hists[kWSB_cv])->Integral() /
                              GetNDOnAxis(Hists[kWSB])->Integral();
  double NCB_GENIETov3Scale = GetNDOnAxis(Hists[kNCB_cv])->Integral() /
                              GetNDOnAxis(Hists[kNCB])->Integral();

  std::cout << "[INFO]: NumuCC_GENIETov3Scale = " << NumuCC_GENIETov3Scale
            << std::endl;
  std::cout << "[INFO]: Sel_GENIETov3Scale = " << Sel_GENIETov3Scale
            << std::endl;
  std::cout << "[INFO]: WSB_GENIETov3Scale = " << WSB_GENIETov3Scale
            << std::endl;
  std::cout << "[INFO]: NCB_GENIETov3Scale = " << NCB_GENIETov3Scale
            << std::endl;

  double NWeeksSpecRun = HasSpecRun(Hists[kNumuCC_cv]) ? 1 : 0;
  double NWeeksOffAxis = (NTotalWeeks - (NWeeksSpecRun + NWeeksOnAxis));

  std::vector<double> OffAxisStops = {4, 8, 12, 16, 20, 24, 28, 30.5};

  double NWeeksEachOffAxis = NWeeksOffAxis / double(OffAxisStops.size());
  std::cout << NWeeksEachOffAxis << std::endl;

  EvRateLine erl_onaxis;
  erl_onaxis.OffAxisPosition = "On axis (293 kA)";
  erl_onaxis.RunFraction = NWeeksOnAxis / NTotalWeeks;
  erl_onaxis.NNumuCC =
      GetNDOnAxis(Hists[kNumuCC])->Integral() * TPOT * erl_onaxis.RunFraction;
  erl_onaxis.NSel_tot =
      GetNDOnAxis(Hists[kAll])->Integral() * TPOT * erl_onaxis.RunFraction;
  erl_onaxis.NSel_WSB =
      GetNDOnAxis(Hists[kWSB])->Integral() * TPOT * erl_onaxis.RunFraction;
  erl_onaxis.NSel_NC =
      GetNDOnAxis(Hists[kNCB])->Integral() * TPOT * erl_onaxis.RunFraction;
  erl_onaxis.NGas = erl_onaxis.NNumuCC * StopToGasMassRatio;

  EventRateTable.push_back(erl_onaxis);

  if (HasSpecRun(Hists[kNumuCC_cv])) {
    EvRateLine erl_sr;

    erl_sr.OffAxisPosition = "On axis (280 kA)";
    erl_sr.RunFraction = NWeeksSpecRun / NTotalWeeks;
    erl_sr.NNumuCC = GetNDOnAxisSpecRun(Hists[kNumuCC])->Integral() * TPOT *
                     erl_sr.RunFraction;
    erl_sr.NSel_tot =
        GetNDOnAxisSpecRun(Hists[kAll])->Integral() * TPOT * erl_sr.RunFraction;
    erl_sr.NSel_WSB =
        GetNDOnAxisSpecRun(Hists[kWSB])->Integral() * TPOT * erl_sr.RunFraction;
    erl_sr.NSel_NC =
        GetNDOnAxisSpecRun(Hists[kNCB])->Integral() * TPOT * erl_sr.RunFraction;
    erl_sr.NGas = erl_sr.NNumuCC * StopToGasMassRatio;

    EventRateTable.push_back(erl_sr);
  }
  for (auto pos : OffAxisStops) {
    EvRateLine erl;

    erl.OffAxisPosition = std::to_string(pos) + " m off axis";
    erl.RunFraction = NWeeksEachOffAxis / NTotalWeeks;
    erl.NNumuCC =
        GetNDStop(Hists[kNumuCC], pos)->Integral() * TPOT * erl.RunFraction;
    erl.NSel_tot =
        GetNDStop(Hists[kAll], pos)->Integral() * TPOT * erl.RunFraction;
    erl.NSel_WSB =
        GetNDStop(Hists[kWSB], pos)->Integral() * TPOT * erl.RunFraction;
    erl.NSel_NC =
        GetNDStop(Hists[kNCB], pos)->Integral() * TPOT * erl.RunFraction;
    erl.NGas = erl.NNumuCC * StopToGasMassRatio;

    EventRateTable.push_back(erl);
  }

  std::ofstream tbl("StopTable.tex");
  tbl << "\\documentclass[multi=tabular]{standalone}\n\\usepackage{nicefrac}"
         "\n\\begin{"
         "document}\n";

  WriteTable(tbl, EventRateTable);

  std::vector<EvRateLine> CVModEvRateTable = EventRateTable;

  for (auto &erl : CVModEvRateTable) {
    erl.NNumuCC *= NumuCC_GENIETov3Scale;
    erl.NSel_tot *= Sel_GENIETov3Scale;
    erl.NSel_WSB *= WSB_GENIETov3Scale;
    erl.NSel_NC *= NCB_GENIETov3Scale;
    erl.NGas *= NumuCC_GENIETov3Scale;
  }

  WriteTable(tbl, CVModEvRateTable);

  tbl << "\n\\end{document}";
}

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cout << "[ERROR]: Expected to get 1 arguments." << std::endl;
    return 1;
  }
  DumpEventRateTable(argv[1]);
}