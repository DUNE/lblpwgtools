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

void SayUsage(char const *argv[]) {
  std::cout
      << "[USAGE]: " << argv[0] << "\n"
      << "\t-i <file,leg name>        : Add fit result to plot\n"
      << "\t-o output.<img>           : File name to print the canvas to.\n"
      << "\t-h <legend header>        : Legend header text.\n"
      << "\t--OA                      : Include OA parameters.\n"
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
  size_t NTreesRead = 0;
  for (auto p : Inputs) {
    TFile f(p.input_file.c_str());
    if (f.IsZombie()) {
      std::cout << "[ERROR]: Failed to read file " << p.input_file << std::endl;
      abort();
    }
    std::cout << "[INFO]: Opened file " << p.input_file << std::endl;
    TTree *t;
    f.GetObject("fit_info", t);
    if (!t) {
      std::cout << "[ERROR]: Failed to load fit_info tree from " << p.input_file
                << std::endl;
      abort();
    }

    TTree *m;
    f.GetObject("meta_tree", m);
    if (!m) {
      std::cout << "[ERROR]: Failed to load meta tree from " << p.input_file
                << std::endl;
      abort();
    }

    std::cout << "[INFO]: fit_info tree has " << t->GetEntries() << " entries."
              << std::endl;

    FitTreeBlob *ftb = FitTreeBlob::MakeReader(t, m);
    t->GetEntry(0);
    m->GetEntry(0);

    std::cout << "[INFO]: Found parameters: " << std::endl;
    size_t NParams = ftb->fPostFitValues->size();
    for (size_t pit = 0; pit < NParams; ++pit) {
      std::string const &pname = ftb->fParamNames->at(pit);
      std::cout << "\t" << pname << " = {" << ftb->fPostFitValues->at(pit)
                << ", " << ftb->fPostFitErrors->at(pit) << "}." << std::endl;
      if (!FitResults.count(pname)) {
        // If this parameter hasn't yet been added to the map, but others have,
        // make sure to padd with zeros
        FitResults[pname] = std::vector<std::pair<double, double>>(
            NTreesRead, std::pair<double, double>{0, 0});
      }
      FitResults[pname].emplace_back(ftb->fPostFitValues->at(pit),
                                     ftb->fPostFitErrors->at(pit));
    }

    for (auto &fr : FitResults) {
      if (fr.second.size() !=
          (NTreesRead +
           1)) { // If this input didn't use this parameter, then skip it.
        fr.second.emplace_back(0, 0);
      }
    }

    NTreesRead++;
  }

  TCanvas c1("c1", "", 3200, 800);
  int colorwheel[] = {kDUNEBlue, kDUNEOrange, kGreen - 3, kRed + 3};
  int ncolors = 4;

  std::vector<std::pair<std::string, std::string>> superorder;
  size_t NOscVars = 0;
  if (IncludeOA) {
    for (auto &s : GetOscVars()) {
      superorder.emplace_back(s->ShortName(), s->LatexName());
      NOscVars++;
    }
  }
  size_t NFlux = 0;
  for (auto &s : GetListOfSysts("flux:noxsec:nodet")) {
    superorder.emplace_back(s->ShortName(), s->LatexName());
    NFlux++;
  }
  for (auto &s : GetListOfSysts("cdrflux:noxsec:nodet")) {
    superorder.emplace_back(s->ShortName(), s->LatexName());
    NFlux++;
  }
  size_t NXSec = 0;
  for (auto &s : GetListOfSysts("noflux:xsec:nodet")) {
    superorder.emplace_back(s->ShortName(), s->LatexName());
    NXSec++;
  }
  for (auto &s : GetListOfSysts("noflux:noxsec:det")) {
    superorder.emplace_back(s->ShortName(), s->LatexName());
  }

  size_t LastOscVars = 0;
  size_t LastFlux = 0;
  size_t LastXSec = 0;

  size_t NPars = 0;

  for (size_t s_it = 0; s_it < superorder.size(); ++s_it) {
    if (s_it == NOscVars) {
      LastOscVars = NPars;
    }
    if (s_it == (NOscVars + NFlux)) {
      LastFlux = NPars;
    }
    if (s_it == (NOscVars + NFlux + NXSec)) {
      LastXSec = NPars;
    }

    if (!FitResults.count(superorder[s_it].first)) {
      continue;
    }
    NPars++;
  }

  size_t p_it = 0;
  std::vector<TH1D> hists;
  for (auto p : Inputs) {
    hists.emplace_back(p.legend_name.c_str(), "", NPars, 0, NPars);
    hists.back().SetDirectory(nullptr);
  }

  for (auto frn : superorder) {
    if (!FitResults.count(frn.first)) {
      continue;
    }
    auto &fr = FitResults[frn.first];
    for (size_t i = 0; i < fr.size(); ++i) {
      if (!i) {
        hists[i].GetXaxis()->SetBinLabel(p_it + 1, frn.second.c_str());
        hists[i].GetYaxis()->SetRangeUser(-1.1, 1.1);
        hists[i].GetYaxis()->SetNdivisions(505);
        hists[i].GetYaxis()->SetLabelSize(0.08);
        hists[i].GetXaxis()->SetLabelSize(0.06);
      }
      if (!p_it) {
        hists[i].SetLineStyle(1);
        hists[i].SetLineColor(colorwheel[i % ncolors]);
        hists[i].SetLineWidth(10 - (3 * i));
        hists[i].SetMarkerStyle(0);
        hists[i].SetMarkerSize(0);
        hists[i].SetMarkerColor(colorwheel[i % ncolors]);
      }
      hists[i].SetBinContent(p_it + 1, fr[i].first);
      hists[i].SetBinError(p_it + 1, fr[i].second);
    }
    p_it++;
  }

  TLegend leg(0.1, 0.9, 0.85, 0.99);
  leg.SetNColumns(4);

  bool drawnfirst = false;

  for (size_t h_it = 0; h_it < hists.size(); ++h_it) {
    hists[h_it].Draw(drawnfirst ? "SAMEE1" : "E1");
    leg.AddEntry(&hists[h_it], hists[h_it].GetName(), "l");
    drawnfirst = true;
  }

  TLine l(0, 0, 0, 0);
  l.SetLineColor(kBlack);
  l.SetLineStyle(2);
  l.SetLineWidth(1);
  if (LastOscVars && (LastOscVars != NPars)) {
    l.DrawLine(LastOscVars, -1.1, LastOscVars, 1.1);
  }
  if (LastFlux && (LastFlux != NPars)) {
    l.DrawLine(LastFlux, -1.1, LastFlux, 1.1);
  }
  if (LastXSec && (LastXSec != NPars)) {
    l.DrawLine(LastXSec, -1.1, LastXSec, 1.1);
  }

  leg.Draw();
  if (hdr.size()) {
    leg.SetHeader(hdr.c_str());
  }
  leg.SetBorderSize(0);

  gPad->SetTopMargin(0.16);
  gPad->SetLeftMargin(0.04);
  gPad->SetRightMargin(0.02);
  gPad->SetBottomMargin(0.4);
  gPad->RedrawAxis();

  c1.Print(output_file.c_str());
}
