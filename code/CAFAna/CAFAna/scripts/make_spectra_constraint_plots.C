#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"
#include "TLine.h"

#include <string>
#include <vector>

struct inp_plot {
  std::string input_file;
  std::string legend_name;
};

std::vector<inp_plot> Inputs;

std::map<std::string, std::vector<std::pair<double, double>>> FitResults;

std::string output_file = "banff.pdf";
bool IncludeOA = false;
std::string hdr = "";
std::string paramset = "all";

void SayUsage(char const *argv[]) {
  std::cout
      << "[USAGE]: " << argv[0] << "\n"
      << "\t-i <file,leg name>        : Add fit result to plot\n"
      << "\t-o output.<img>           : File name to print the canvas to.\n"
      << "\t-h <legend header>        : Legend header text.\n"
      << "\t-p <throw set>            : Subset to use. 'all' by default\n"
      << std::endl;
}

void handleOpts(int argc, char const *argv[]) {
  int opt = 1;
  while (opt < argc) {
    if (std::string(argv[opt]) == "-?" || std::string(argv[opt]) == "--help") {
      SayUsage(argv);
      exit(0);
    } else if (std::string(argv[opt]) == "-i") {
      auto split = SplitString(argv[++opt], ',');
      if (split.size() != 2) {
        std::cout << "[ERROR]: Argment \"" << argv[opt] << "\" malformed."
                  << std::endl;
        SayUsage(argv);
        exit(1);
      }
      Inputs.push_back({split[0], split[1]});
    } else if (std::string(argv[opt]) == "-o") {
      output_file = argv[++opt];
    } else if (std::string(argv[opt]) == "-h") {
      hdr = argv[++opt];
    } else if (std::string(argv[opt]) == "--OA") {
      IncludeOA = true;
    } else {
      std::cout << "[ERROR]: Unknown option: " << argv[opt] << std::endl;
      SayUsage(argv);
      exit(1);
    }
    opt++;
  }
}

int kDUNEOrange;
int kDUNEBlue;

void DefineColors() {

  int BaseColor = 50;
  int DO[3] = {242, 159, 84};
  kDUNEOrange = BaseColor + 1;
  int DB[3] = {125, 172, 213};
  kDUNEBlue = BaseColor + 2;
  TColor *DO_col = gROOT->GetColor(BaseColor + 1);
  if (!DO_col) {
    DO_col = new TColor(BaseColor + 1, Float_t(DO[0]) / 255.0,
                        Float_t(DO[1]) / 255.0, Float_t(DO[2]) / 255.0,
                        "DUNE_Orange");
  } else {
    DO_col->SetRGB(Float_t(DO[0]) / 255.0, Float_t(DO[1]) / 255.0,
                   Float_t(DO[2]) / 255.0);
  }
  TColor *DB_col = gROOT->GetColor(BaseColor + 2);
  if (!DB_col) {
    DB_col = new TColor(BaseColor + 2, Float_t(DB[0]) / 255.0,
                        Float_t(DB[1]) / 255.0, Float_t(DB[2]) / 255.0,
                        "DUNE_Orange");
  } else {
    DB_col->SetRGB(Float_t(DB[0]) / 255.0, Float_t(DB[1]) / 255.0,
                   Float_t(DB[2]) / 255.0);
  }
}

int main(int argc, char const *argv[]) {
  DefineColors();

  gStyle->SetOptStat(0);
  handleOpts(argc, argv);

  TCanvas c1("c1", "", 800, 600);
  c1.Print((output_file + "[").c_str());

  int colorwheel[] = {kDUNEBlue, kDUNEOrange, kGreen - 3, kRed + 3};
  int ncolors = 4;

  std::vector<TH1D> ND_FHC_hists, ND_RHC_hists, FD_FHC_hists, FD_RHC_hists;
  std::vector<double> ND_ybins;

  size_t int_it = 0;
  for (auto p : Inputs) {

    TFile fin(p.input_file.c_str(), "READ");

    TH2D *ND_FHC_Err;
    fin.GetObject("postfit_all/ND_FHC_errs", ND_FHC_Err);
    if (!ND_FHC_Err) {
      std::cout << "Failed to get: postfit_all/ND_FHC_errs" << std::endl;
      abort();
    }
    TH2D *ND_RHC_Err;
    fin.GetObject("postfit_all/ND_RHC_errs", ND_RHC_Err);
    if (!ND_RHC_Err) {
      std::cout << "Failed to get: postfit_all/ND_RHC_errs" << std::endl;
      abort();
    }

    TH1D *FD_FHC_Err;
    fin.GetObject("postfit_all/FD_numu_FHC_errs", FD_FHC_Err);
    if (!FD_FHC_Err) {
      std::cout << "Failed to get: postfit_all/FD_numu_FHC_errs" << std::endl;
      abort();
    }
    TH1D *FD_RHC_Err;
    fin.GetObject("postfit_all/FD_numu_RHC_errs", FD_RHC_Err);
    if (!FD_RHC_Err) {
      std::cout << "Failed to get: postfit_all/FD_numu_RHC_errs" << std::endl;
      abort();
    }

    std::vector<double> ND_xbins;
    ND_xbins.push_back(ND_FHC_Err->GetXaxis()->GetBinLowEdge(1));
    for (int j = 0; j < ND_FHC_Err->GetYaxis()->GetNbins(); ++j) {
      for (int i = 0; i < ND_FHC_Err->GetXaxis()->GetNbins(); ++i) {
        ND_xbins.push_back(ND_xbins.back() +
                           ND_FHC_Err->GetXaxis()->GetBinWidth(i + 1));
        std::cout << "i = " << i << ", j = " << j
                  << ", xbw = " << ND_FHC_Err->GetXaxis()->GetBinWidth(i + 1)
                  << ", xback = " << ND_xbins.back() << std::endl;
      }
      ND_ybins.push_back(ND_xbins.back());
      std::cout << "j = " << j << ", yback = " << ND_ybins.back() << std::endl;
    }

    std::vector<double> FD_xbins;
    FD_xbins.push_back(FD_FHC_Err->GetXaxis()->GetBinLowEdge(1));
    for (int i = 0; i < FD_FHC_Err->GetXaxis()->GetNbins(); ++i) {
      FD_xbins.push_back(FD_xbins.back() +
                         FD_FHC_Err->GetXaxis()->GetBinWidth(i + 1));
    }

    ND_FHC_hists.emplace_back((p.legend_name + "_ND_FHC").c_str(), "",
                              (ND_xbins.size() - 1), ND_xbins.data());
    ND_RHC_hists.emplace_back((p.legend_name + "_ND_RHC").c_str(), "",
                              (ND_xbins.size() - 1), ND_xbins.data());
    FD_FHC_hists.emplace_back((p.legend_name + "_FD_FHC").c_str(), "",
                              (FD_xbins.size() - 1), FD_xbins.data());
    FD_RHC_hists.emplace_back((p.legend_name + "_FD_RHC").c_str(), "",
                              (FD_xbins.size() - 1), FD_xbins.data());

    ND_FHC_hists.back().SetDirectory(nullptr);
    ND_RHC_hists.back().SetDirectory(nullptr);
    FD_FHC_hists.back().SetDirectory(nullptr);
    FD_RHC_hists.back().SetDirectory(nullptr);

    size_t ND_bin_it = 0;
    for (int j = 0; j < ND_FHC_Err->GetYaxis()->GetNbins(); ++j) {
      for (int i = 0; i < ND_FHC_Err->GetXaxis()->GetNbins(); ++i) {
        double ND_FHC_E = ND_FHC_Err->GetBinError(i + 1, j + 1);
        double ND_FHC_C = ND_FHC_Err->GetBinContent(i + 1, j + 1);

        std::cout << "ND_FHC: "
                  << ND_FHC_hists.back().GetXaxis()->GetBinCenter(ND_bin_it + 1)
                  << " = " << ND_FHC_E << "/" << ND_FHC_C << std::endl;

        if (ND_FHC_C > 0) {
          ND_FHC_hists.back().SetBinError(ND_bin_it + 1, ND_FHC_E / ND_FHC_C);
        }

        double ND_RHC_E = ND_RHC_Err->GetBinError(i + 1, j + 1);
        double ND_RHC_C = ND_RHC_Err->GetBinContent(i + 1, j + 1);

        std::cout << "ND_RHC: "
                  << ND_RHC_hists.back().GetXaxis()->GetBinCenter(ND_bin_it + 1)
                  << " = " << ND_RHC_E << "/" << ND_RHC_C << std::endl;

        if (ND_RHC_C > 0) {
          ND_RHC_hists.back().SetBinError(ND_bin_it + 1, ND_RHC_E / ND_RHC_C);
        }
        ND_bin_it++;
      }
    }
    for (int i = 0; i < FD_FHC_Err->GetXaxis()->GetNbins(); ++i) {
      double FD_FHC_E = FD_FHC_Err->GetBinError(i + 1);
      double FD_FHC_C = FD_FHC_Err->GetBinContent(i + 1);

      if (FD_FHC_C > 0) {
        FD_FHC_hists.back().SetBinError(i + 1, FD_FHC_E / FD_FHC_C);
      }

      double FD_RHC_E = FD_RHC_Err->GetBinError(i + 1);
      double FD_RHC_C = FD_RHC_Err->GetBinContent(i + 1);

      if (FD_RHC_C > 0) {
        FD_RHC_hists.back().SetBinError(i + 1, FD_RHC_E / FD_RHC_C);
      }
    }

    if (!int_it) {
      for (TH1D *h :
           {ND_FHC_hists.data() + int_it, ND_RHC_hists.data() + int_it,
            FD_FHC_hists.data() + int_it, FD_RHC_hists.data() + int_it}) {

        h->GetYaxis()->SetRangeUser(-0.5, 0.5);
        h->GetYaxis()->SetTitle("Fractional Uncertainty");
        h->GetXaxis()->SetTitle("Analysis Bin (A.U.)");
      }
    }

    for (TH1D *h :
         {ND_FHC_hists.data() + int_it, ND_RHC_hists.data() + int_it,
          FD_FHC_hists.data() + int_it, FD_RHC_hists.data() + int_it}) {

      h->SetLineStyle(0);
      h->SetLineColor(0);
      h->SetLineWidth(0);
      h->SetFillStyle(1001);
      h->SetFillColor(colorwheel[int_it % ncolors]);
      h->SetMarkerStyle(0);
      h->SetMarkerSize(0);
      h->SetMarkerColor(0);
    }

    int_it++;
  }

  gPad->SetTopMargin(0.16);
  gPad->SetLeftMargin(0.125);
  gPad->SetRightMargin(0.02);
  gPad->SetBottomMargin(0.125);

  TLine l(0, 0, 0, 0);
  l.SetLineColor(kBlack);
  l.SetLineStyle(1);
  l.SetLineWidth(2);

  TLegend leg(0.1, 0.9, 0.85, 0.99);
  leg.SetNColumns(4);
  for (size_t inp_it = 0; inp_it < int_it; ++inp_it) {
    leg.AddEntry(&ND_FHC_hists[inp_it], Inputs[inp_it].legend_name.c_str(),
                 "f");
  }
  leg.SetBorderSize(0);

  for (size_t inp_it = 0; inp_it < int_it; ++inp_it) {
    ND_FHC_hists[inp_it].Draw(inp_it ? "SAMEE2" : "E2");
  }
  gPad->RedrawAxis();
  for (auto &e : ND_ybins) {
    l.DrawLine(e, -0.5, e, 0.5);
  }
  leg.SetHeader("ND_FHC");
  leg.Draw();
  c1.Print((output_file).c_str());

  for (size_t inp_it = 0; inp_it < int_it; ++inp_it) {
    ND_RHC_hists[inp_it].Draw(inp_it ? "SAMEE2" : "E2");
  }
  for (auto &e : ND_ybins) {
    l.DrawLine(e, -0.5, e, 0.5);
  }
  leg.SetHeader("ND_RHC");
  leg.Draw();
  gPad->RedrawAxis();

  c1.Print((output_file).c_str());

  for (size_t inp_it = 0; inp_it < int_it; ++inp_it) {
    FD_FHC_hists[inp_it].Draw(inp_it ? "SAMEE2" : "E2");
  }
  leg.SetHeader("FD_FHC");
  leg.Draw();
  gPad->RedrawAxis();

  c1.Print((output_file).c_str());

  for (size_t inp_it = 0; inp_it < int_it; ++inp_it) {
    FD_FHC_hists[inp_it].Draw(inp_it ? "SAMEE2" : "E2");
  }
  leg.SetHeader("FD_RHC");
  leg.Draw();
  gPad->RedrawAxis();

  c1.Print((output_file).c_str());
  c1.Print((output_file + "]").c_str());
}
