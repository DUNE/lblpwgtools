#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>

int kDUNEOrange;
int kDUNEBlue;
int kDUNERed;

void BuildColors() {
  int BaseColor = 50;
  int DO[3] = {242, 159, 84};
  kDUNEOrange = BaseColor + 1;
  TColor *DO_col = gROOT->GetColor(BaseColor + 1);
  if (!DO_col) {
    DO_col = new TColor(BaseColor + 1, Float_t(DO[0]) / 255.0,
                        Float_t(DO[1]) / 255.0, Float_t(DO[2]) / 255.0,
                        "DUNE_Orange");
  } else {
    DO_col->SetRGB(Float_t(DO[0]) / 255.0, Float_t(DO[1]) / 255.0,
                   Float_t(DO[2]) / 255.0);
  }

  int DB[3] = {125, 172, 213};
  kDUNEBlue = BaseColor + 2;
  TColor *DB_col = gROOT->GetColor(BaseColor + 2);
  if (!DB_col) {
    DB_col = new TColor(BaseColor + 2, Float_t(DB[0]) / 255.0,
                        Float_t(DB[1]) / 255.0, Float_t(DB[2]) / 255.0,
                        "DUNE_Orange");
  } else {
    DB_col->SetRGB(Float_t(DB[0]) / 255.0, Float_t(DB[1]) / 255.0,
                   Float_t(DB[2]) / 255.0);
  }
  int DR[3] = {241, 104, 44};
  kDUNERed = BaseColor + 3;
  TColor *DR_col = gROOT->GetColor(BaseColor + 3);
  if (!DR_col) {
    DR_col = new TColor(BaseColor + 3, Float_t(DR[0]) / 255.0,
                        Float_t(DR[1]) / 255.0, Float_t(DR[2]) / 255.0,
                        "DUNE_Orange");
  } else {
    DR_col->SetRGB(Float_t(DR[0]) / 255.0, Float_t(DR[1]) / 255.0,
                   Float_t(DR[2]) / 255.0);
  }
}

enum EBinWidth { kPRISM = 0, kStop };
std::vector<EBinWidth> EBinWidthVect = {kPRISM, kStop};
std::vector<std::string> EBinWidthStr = {"PRISM", "Stop"};

enum EComponent {
  kAll = 0,
  kWSB,
  kNCB,
  kProton,
  kNumuCC,
  kAll_cv,
  kWSB_cv,
  kNCB_cv,
  kNumuCC_cv
};
std::vector<EComponent> EComponentVect = {
    kAll, kWSB, kNCB, kProton, kNumuCC, kAll_cv, kWSB_cv, kNCB_cv, kNumuCC_cv};
std::vector<std::string> EComponentStr = {"",        "_WSB",    "_NC",
                                          "_prote",  "_NumuCC", "_cv",
                                          "_WSB_cv", "_NC_cv",  "_NumuCC_cv"};

enum EProjection { kETrue, kERecDep };
std::vector<EProjection> EProjectionVect = {kETrue, kERecDep};
std::vector<std::string> EProjectionStr = {"ETrue", "ERec"};

std::string EvRateString(double i) {
  std::stringstream ss("");
  ss.precision(1);
  ss << std::scientific << i;
  return ss.str();
}

TH1D *GetNDOnAxis(TH1 *h) {
  TH2D *h2 = dynamic_cast<TH2D *>(h);
  assert(h2);

  TH1D *rtn =
      h2->ProjectionX((std::string(h->GetName()) + "_OnAxis").c_str(), 1, 6);
  rtn->GetYaxis()->SetTitle(h->GetZaxis()->GetTitle());
  rtn->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
  rtn->Scale(1.0 / 6.0);
  rtn->SetDirectory(nullptr);
  return rtn;
}

TH1D *GetNDSlice(TH1 *h, size_t i) {
  TH2D *h2 = dynamic_cast<TH2D *>(h);
  assert(h2);

  TH1D *rtn = h2->ProjectionX(
      (std::string(h->GetName()) + "_OffAxis_" + std::to_string(i)).c_str(),
      i + 1, i + 1);
  rtn->GetYaxis()->SetTitle(h->GetZaxis()->GetTitle());
  rtn->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());

  double x = h2->GetYaxis()->GetBinCenter(i + 1);

  // Scale for dumbplan
  if (x > 3.9) {
    rtn->Scale(1.0 / 16.0);
  } else {
    rtn->Scale(1.0 / 2.0);
  }

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
  double OffAxisPosition;
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

double GasToStopMassRatio = (vol_gas_cm3 * density_gas_kg_cm3) /
                            (vol_liquid_stop_cm3 * density_liquid_kg_cm3);

std::vector<EvRateLine> EventRateTable;
std::vector<EvRateLine> EventRateTable_GHAXX;

std::string Commerize(double d) {
  std::stringstream ss("");
  // I know
  if (d > 1E6) {
    ss << (round(d * 1E-5) * 1E-1) << "M";
    return ss.str();
  } else if (d > 1E5) {
    ss << int(round(d * 1E-4) * 1E4);
  } else {
    ss << int(round(d * 1E-3) * 1E3);
  }

  std::string s = ss.str();

  std::stringstream ss2("");
  for (int i = 0; i < s.length(); ++i) {
    ss2 << s[s.length() - i - 1];
    if (i && (i != (s.length() - 1)) && (!((i + 1) % 3))) {
      ss2 << ",";
    }
  }
  std::string s2 = ss2.str();
  std::stringstream ss3("");
  for (int i = 0; i < s2.length(); ++i) {
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
    tbl << int(e.OffAxisPosition) << "\\,m & "
        << "\\nicefrac{1}{" << int(e.RunFraction) << " } yr. & "
        << Commerize(e.NNumuCC) << " & " << Commerize(e.NSel_tot) << " & ";
    tbl.precision(1);
    tbl << std::fixed << (e.NSel_WSB / e.NSel_tot) * 1.0E2 << "\\% & "
        << (e.NSel_NC / e.NSel_tot) * 1.0E2 << "\\% & " << Commerize(e.NGas)
        << "\\\\\n";
  }

  tbl << "\\end{tabular}";
}

void MakeOffAxisPlots() {
  BuildColors();

  std::map<EBinWidth, std::map<EProjection, std::map<EComponent, TH1 *>>> Hists;

  TFile *f = new TFile("OffAxisEventRates_30GeV.root");

  TFile *fcaf = new TFile("CAF_FHC_PRISM_PROD4-14.root");
  TTree *fst = (TTree *)fcaf->Get("FileSummaryTree");
  double POT, det_x;
  fst->SetBranchAddress("POT", &POT);
  fst->SetBranchAddress("det_x", &det_x);
  std::map<double, double> POTScaleUp;
  for (size_t it = 0; it < fst->GetEntries(); ++it) {
    fst->GetEntry(it);
    if (!POTScaleUp.count(det_x)) {
      POTScaleUp[det_x] = 0;
    }
    POTScaleUp[det_x] += POT;
  }
  double TPOT = 1.1e21;

  TH1D *POTScaleUpH =
      new TH1D("POTScaleUpH", ";Off axis position (GeV); POT Scale factor",
               double(34 - -2) / 4.0, -2, 34);
  for (auto &psc : POTScaleUp) {
    double RunPlan_OnAxis = TPOT / 2.0;
    double RunPlan_OffAxis = TPOT / 16.0;
    if (psc.first == 0) {
      psc.second = (RunPlan_OnAxis / psc.second);
    } else {
      psc.second = (RunPlan_OffAxis / psc.second);
    }
    POTScaleUpH->Fill(psc.first, psc.second);
  }

  for (EBinWidth b : EBinWidthVect) {
    for (EProjection p : EProjectionVect) {
      for (EComponent c : EComponentVect) {
        std::stringstream hname("");
        hname << EProjectionStr[p] << "_" << EBinWidthStr[b] << "/NearDet"
              << EComponentStr[c];

        Hists[b][p][c] = dynamic_cast<TH1 *>(f->Get(hname.str().c_str()));

        if (!Hists[b][p][c]) {
          std::cout << "[ERROR]: Failed to get " << hname.str() << std::endl;
          throw;
        }

        Hists[b][p][c]->SetDirectory(nullptr);
      }
    }
  }

  std::vector<int> ColorWheel = {kDUNEOrange, kDUNEBlue, kGray + 2,
                                 kMagenta - 3};
  size_t NCols = ColorWheel.size();

  // On axis plots
  TCanvas c1("c1", "", 1600, 1200);
  TGaxis::SetMaxDigits(3);
  c1.SetLeftMargin(0.2);
  c1.SetRightMargin(0.03);
  c1.SetBottomMargin(0.175);
  c1.SetTopMargin(0.1);
  gStyle->SetOptStat(0);

  c1.Print("OffAxisEventRatePredictionPlots.pdf[");

  double NumuCC_GENIETov3Scale =
      GetNDSlice(Hists[kStop][kETrue][kNumuCC_cv], 1)->Integral() /
      GetNDSlice(Hists[kStop][kETrue][kNumuCC], 1)->Integral();
  double Sel_GENIETov3Scale =
      GetNDSlice(Hists[kStop][kETrue][kAll_cv], 1)->Integral() /
      GetNDSlice(Hists[kStop][kETrue][kAll], 1)->Integral();
  double WSB_GENIETov3Scale =
      GetNDSlice(Hists[kStop][kETrue][kWSB_cv], 1)->Integral() /
      GetNDSlice(Hists[kStop][kETrue][kWSB], 1)->Integral();
  double NCB_GENIETov3Scale =
      GetNDSlice(Hists[kStop][kETrue][kNCB_cv], 1)->Integral() /
      GetNDSlice(Hists[kStop][kETrue][kNCB], 1)->Integral();

  std::cout << "[INFO]: NumuCC_GENIETov3Scale = " << NumuCC_GENIETov3Scale
            << std::endl;
  std::cout << "[INFO]: Sel_GENIETov3Scale = " << Sel_GENIETov3Scale
            << std::endl;
  std::cout << "[INFO]: WSB_GENIETov3Scale = " << WSB_GENIETov3Scale
            << std::endl;
  std::cout << "[INFO]: NCB_GENIETov3Scale = " << NCB_GENIETov3Scale
            << std::endl;

  // Off axis plot
  double xmx = 6;
  POTScaleUpH->Draw();
  c1.Print("OffAxisEventRatePredictionPlots.pdf");
  for (EBinWidth b : {kStop, kPRISM}) {
    for (EProjection p : EProjectionVect) {
      for (bool SimpleLeg : {true, false}) {

        TH2 *h_tot_etrue = static_cast<TH2 *>(Hists[b][p][kAll]);
        TH2 *h_WSB_etrue = static_cast<TH2 *>(Hists[b][p][kWSB]);
        TH2 *h_NC_etrue = static_cast<TH2 *>(Hists[b][p][kNCB]);
        TH2 *h_NumuCC_etrue = static_cast<TH2 *>(Hists[b][p][kNumuCC]);
        TH2 *h_tot_prote = static_cast<TH2 *>(Hists[b][p][kProton]);

        TH2 *h_tot_etrue_cv = static_cast<TH2 *>(Hists[b][p][kAll_cv]);
        TH2 *h_WSB_etrue_cv = static_cast<TH2 *>(Hists[b][p][kWSB_cv]);
        TH2 *h_NC_etrue_cv = static_cast<TH2 *>(Hists[b][p][kNCB_cv]);
        TH2 *h_NumuCC_etrue_cv = static_cast<TH2 *>(Hists[b][p][kNumuCC_cv]);

        size_t NSlices = h_tot_etrue->GetYaxis()->GetNbins();

        if ((b == kStop) && (p == kETrue)) {

          for (size_t slice_it = 0; slice_it < NSlices; ++slice_it) {
            double SliceCenter_m =
                h_tot_etrue->GetYaxis()->GetBinCenter(slice_it + 1);

            EvRateLine erl;
            erl.OffAxisPosition = SliceCenter_m;
            erl.RunFraction = ((SliceCenter_m == 0) ? 2.0 : 16.0);
            erl.NNumuCC = GetNDSlice(h_NumuCC_etrue, slice_it)->Integral();
            erl.NSel_tot = GetNDSlice(h_tot_etrue, slice_it)->Integral();
            erl.NSel_WSB = GetNDSlice(h_WSB_etrue, slice_it)->Integral();
            erl.NSel_NC = GetNDSlice(h_NC_etrue, slice_it)->Integral();
            erl.NGas = GasToStopMassRatio * erl.NNumuCC;

            bool onaxisprod = (SliceCenter_m < 4);

            EvRateLine erl_GHAXX;
            erl_GHAXX.OffAxisPosition = SliceCenter_m;
            erl_GHAXX.RunFraction = ((SliceCenter_m == 0) ? 2.0 : 16.0);
            erl_GHAXX.NNumuCC =
                GetNDSlice(onaxisprod ? h_NumuCC_etrue : h_NumuCC_etrue_cv,
                           slice_it)
                    ->Integral() *
                (onaxisprod ? NumuCC_GENIETov3Scale : 1);
            erl_GHAXX.NSel_tot =
                GetNDSlice(onaxisprod ? h_tot_etrue : h_tot_etrue_cv, slice_it)
                    ->Integral() *
                (onaxisprod ? Sel_GENIETov3Scale : 1);
            erl_GHAXX.NSel_WSB =
                GetNDSlice(onaxisprod ? h_WSB_etrue : h_WSB_etrue_cv, slice_it)
                    ->Integral() *
                (onaxisprod ? WSB_GENIETov3Scale : 1);
            erl_GHAXX.NSel_NC =
                GetNDSlice(onaxisprod ? h_NC_etrue : h_NC_etrue_cv, slice_it)
                    ->Integral() *
                (onaxisprod ? NCB_GENIETov3Scale : 1);
            erl_GHAXX.NGas = GasToStopMassRatio * erl_GHAXX.NNumuCC;

            if (!SimpleLeg) {
              EventRateTable.push_back(erl);
              EventRateTable_GHAXX.push_back(erl_GHAXX);
            }
          }
          continue;
        }

        for (size_t slice_it = 0; slice_it < NSlices; ++slice_it) {
          double SliceCenter_m =
              h_tot_etrue->GetYaxis()->GetBinCenter(slice_it + 1);

          if (SliceCenter_m < 0) {
            continue;
          }

          int su_bin = POTScaleUpH->GetXaxis()->FindFixBin(SliceCenter_m);

          std::stringstream lbl("");

          TLegend t(0.3, 0.725, 0.96, 0.89);
          if (b == kPRISM) {
            lbl.precision(4);
            lbl << "CAFAna ND, #nu-mode, 50x200x300 cm^{3}, " << SliceCenter_m;
            lbl.precision(2);
            lbl << " m (MC Scale " << POTScaleUpH->GetBinContent(su_bin)
                << "x)";
            lbl.precision(4);
          } else {
            lbl.precision(4);
            lbl << "CAFAna ND, #nu-mode, 4x2x3 m^{3}, ";
            if (SliceCenter_m == 0) {
              lbl << " On axis";
            } else {
              lbl.precision(2);
              lbl << SliceCenter_m << " m Off axis";
            }
            lbl << " (MC Scale " << POTScaleUpH->GetBinContent(su_bin) << "x)";
            lbl.precision(4);
          }
          t.SetHeader(lbl.str().c_str());
          t.SetBorderSize(0);

          std::vector<TH1 *> HistsToPlot;

          lbl.str("");
          HistsToPlot.push_back(GetNDSlice(h_tot_etrue, slice_it));
          HistsToPlot.back()->SetLineColor(kDUNERed);
          HistsToPlot.back()->SetLineWidth(3);
          lbl << "Selected ND "
              << " (" << EvRateString(HistsToPlot.back()->Integral())
              << " events)";
          t.AddEntry(HistsToPlot.back(), lbl.str().c_str(), "l");

          if (p == kERecDep) {
            lbl.str("");
            HistsToPlot.push_back(GetNDSlice(h_WSB_etrue, slice_it));
            HistsToPlot.back()->SetLineColor(kDUNEOrange);
            HistsToPlot.back()->SetFillColor(kDUNEOrange);
            HistsToPlot.back()->SetFillStyle(1001);
            HistsToPlot.back()->SetLineWidth(3);
            HistsToPlot.back()->Add(GetNDSlice(h_NC_etrue, slice_it));
            lbl << "Sel. wrong sign "
                << " (" << EvRateString(HistsToPlot.back()->Integral())
                << " events)";
            t.AddEntry(HistsToPlot.back(), lbl.str().c_str(), "f");

            lbl.str("");
            HistsToPlot.push_back(GetNDSlice(h_NC_etrue, slice_it));
            HistsToPlot.back()->SetLineColor(kDUNEBlue);
            HistsToPlot.back()->SetFillColor(kDUNEBlue);
            HistsToPlot.back()->SetFillStyle(1001);
            HistsToPlot.back()->SetLineWidth(3);
            lbl << "Sel. NC "
                << " (" << EvRateString(HistsToPlot.back()->Integral())
                << " events)";
            t.AddEntry(HistsToPlot.back(), lbl.str().c_str(), "f");

            lbl.str("");
            HistsToPlot.push_back(GetNDSlice(h_tot_prote, slice_it));
            HistsToPlot.back()->SetLineColor(kBlack);
            HistsToPlot.back()->SetMarkerStyle(20);
            HistsToPlot.back()->SetMarkerSize(1);
            HistsToPlot.back()->SetLineWidth(2);
            lbl << "Sel. 20% Missing Proton Energy";
            t.AddEntry(HistsToPlot.back(), lbl.str().c_str(), "lp");

            // stackystack
            HistsToPlot[1]->Add(HistsToPlot[2]);
          }

          if (p == kETrue) {
            lbl.str("");
            HistsToPlot.push_back(GetNDSlice(h_NumuCC_etrue, slice_it));
            HistsToPlot.back()->SetLineColor(kGray + 2);
            HistsToPlot.back()->SetLineWidth(3);
            lbl << "#nu_{#mu} CC-total "
                << " (" << EvRateString(HistsToPlot.back()->Integral())
                << " events)";
            t.AddEntry(HistsToPlot.back(), lbl.str().c_str(), "l");
          }

          double max = -std::numeric_limits<double>::max();
          double min_nu_max = std::numeric_limits<double>::max();
          std::vector<double> MaxXes;
          for (size_t i = 0; i < HistsToPlot.size(); ++i) {
            TH1 *h = HistsToPlot[i];
            std::pair<double, double> maxes = GetMax(h, xmx);
            max = std::max(max, maxes.first);
            min_nu_max = std::min(min_nu_max, maxes.first);
            MaxXes.push_back(maxes.second);
          }

          for (size_t i = 0; i < HistsToPlot.size(); ++i) {
            TH1 *h = HistsToPlot[i];
            if (!i) {
              h->GetYaxis()->SetRangeUser(0, max * (SimpleLeg ? 1.1 : 1.45));
              h->GetYaxis()->SetNdivisions(505);
              h->GetYaxis()->SetTitleSize(2 * h->GetYaxis()->GetTitleSize());
              h->GetYaxis()->SetTitleOffset(1);
              h->GetYaxis()->SetLabelSize(2 * h->GetYaxis()->GetLabelSize());

              h->GetXaxis()->SetRangeUser(0, xmx);
              h->GetXaxis()->SetNdivisions(505);
              h->GetXaxis()->SetTitleSize(2 * h->GetXaxis()->GetTitleSize());
              h->GetXaxis()->SetTitleOffset(1);
              h->GetXaxis()->SetLabelSize(2 * h->GetXaxis()->GetLabelSize());

              if (SliceCenter_m < 4) {
                h->GetYaxis()->SetTitle("Event rate / (1.1x10^{21}/2) POT");
              } else {
                h->GetYaxis()->SetTitle("Event rate / (1.1x10^{21}/16) POT");
              }
              if (p == kETrue) {
                h->GetXaxis()->SetTitle("E_{#nu} (GeV)");
              } else {
                h->GetXaxis()->SetTitle("E_{Rec.}^{Dep.} (GeV)");
              }
              h->Draw("HIST");
            } else {
              if ((p == kERecDep) && (i == 3)) {
                h->Draw("P SAME");
              } else {
                h->Draw("EHIST SAME");
              }
            }
          }
          if (!SimpleLeg) {
            t.Draw();
          } else {
            TLatex l;
            l.SetTextAlign(32);
            std::stringstream lbl("");
            if (SliceCenter_m == 0) {
              lbl << "On axis";
            } else {
              lbl.precision((b == kPRISM) ? 3 : 2);
              lbl << SliceCenter_m << " m Off axis";
            }
            l.SetTextSize(0.09);
            l.DrawLatexNDC(0.9, 0.85, lbl.str().c_str());

            std::stringstream ss("");
            ss << "OffAxisEventRatePredictionPlots_" << EBinWidthStr[b]
               << ".gif+";

            if ((slice_it + 1) == NSlices) {
              ss << "+" << ((b == kPRISM) ? 100 : 100);
            } else {
              if (!slice_it) {
                ss << ((b == kPRISM) ? 100 : 100);
              } else {
                ss << ((b == kPRISM) ? 100 : 100);
              }
            }
            if (!slice_it) { // print twice for the first one...
              c1.Print(ss.str().c_str());
            }
            std::cout << "Printing " << ss.str() << " @ " << SliceCenter_m
                      << std::endl;
            c1.Print(ss.str().c_str());
          }
          c1.Print("OffAxisEventRatePredictionPlots.pdf");
        }
      }
    }
  }
  c1.Print("OffAxisEventRatePredictionPlots.pdf]");

  std::ofstream tbl("StopTable.tex");
  tbl << "\\documentclass[multi=tabular]{standalone}\n\\usepackage{nicefrac}"
         "\n\\begin{"
         "document}\n";

  WriteTable(tbl, EventRateTable);

  std::vector<EvRateLine> EqualRunPlan = EventRateTable;

  for (auto &erl : EqualRunPlan) {
    double fact = erl.RunFraction / 9;
    erl.RunFraction /= fact;
    erl.NNumuCC *= fact;
    erl.NSel_tot *= fact;
    erl.NSel_WSB *= fact;
    erl.NSel_NC *= fact;
    erl.NGas *= fact;
  }

  WriteTable(tbl, EqualRunPlan);

  std::vector<EvRateLine> EqualEventRate = EventRateTable;

  double target_evrate = 5E5;
  double TotalNYears = 0;
  for (auto &erl : EqualEventRate) {
    double yr_evrate = erl.NNumuCC * erl.RunFraction;
    double NYears = target_evrate / yr_evrate;
    std::cout << "Stop " << erl.OffAxisPosition << " Takes " << target_evrate
              << " in " << NYears << " years." << std::endl;
    TotalNYears += NYears;

    double fact = erl.RunFraction * NYears;
    erl.RunFraction /= fact;
    erl.NNumuCC *= fact;
    erl.NSel_tot *= fact;
    erl.NSel_WSB *= fact;
    erl.NSel_NC *= fact;
    erl.NGas *= fact;
  }

  for (auto &erl : EqualEventRate) {
    double fact = 1.0 / TotalNYears;
    erl.RunFraction /= fact;
    erl.NNumuCC *= fact;
    erl.NSel_tot *= fact;
    erl.NSel_WSB *= fact;
    erl.NSel_NC *= fact;
    erl.NGas *= fact;
  }

  std::cout << "Total years: " << TotalNYears << std::endl;

  WriteTable(tbl, EqualEventRate);

  WriteTable(tbl, EventRateTable_GHAXX);

  tbl << "\n\\end{document}";
}
