// Script to add together fits
// C. Hasnip 16/5/2021
//

#include "CAFAna/Core/Utilities.h"

#include <string>
#include <iostream>
#include <algorithm>

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TTree.h"
#include "TKey.h"
//#include "Riostream.h"
#include "TClass.h"
#include "TObject.h"
#include "TObjString.h"

TFile *Target;

void MergeFits(TDirectory *target, TFile *source, std::string param, int Nbins);

void MergeFits(TDirectory *target, TFile *source, std::string param, int Nbins) {
  TString path( (char*)strstr( target->GetPath(), ":" ) ); 
  path.Remove(0, 2);
  source->cd( path );
  TDirectory *current_sourcedir = gDirectory;
  TIter nextkey(current_sourcedir->GetListOfKeys()); 
  TKey *key;

  double ssth23_lowlim = 0.4; // 0.35 0.4
  double dmsq32_lowlim = 2.30;
  double dcp_lowlim = -1;
  double ssth23_highlim = 0.65;
  double dmsq32_highlim = 2.55;
  double dcp_highlim = 1;

  double *lowlim, *highlim;
  if (param == "dmsq32") {
    lowlim = &dmsq32_lowlim;
    highlim = &dmsq32_highlim;
  } else if (param == "ssth23") {
    lowlim = &ssth23_lowlim;
    highlim = &ssth23_highlim;
  } else if (param == "dcp") { 
    lowlim = &dcp_lowlim;
    highlim = &dcp_highlim;
  } else {
    std::cout << "[ERROR] Unknown parameter." << std::endl;
    abort();
  }

  while ((key = (TKey*)nextkey())) {
    source->cd( path );
    TObject *obj = key->ReadObj();
    std::string keyname(key->GetName());

    if (obj->IsA()->InheritsFrom(TDirectory::Class())) {
      std::cout << "dir name will be: " << keyname << std::endl;
      target->cd();
      if (!target->FindObject(keyname.c_str())) {
        std::cout << "New dir " << keyname << std::endl;
        //target->cd();
        TDirectory *newdir = target->mkdir(keyname.c_str(), keyname.c_str());
        MergeFits(newdir, source, param, Nbins);
      } else {
        std::cout << "Update existing dir " << keyname << std::endl;
        TDirectory *existdir = target->GetDirectory(keyname.c_str());
        MergeFits(existdir, source, param, Nbins);
      }
    } else if (keyname == "dChi2Scan" && obj->IsA() == TH1D::Class()) {
      std::cout << "Found 1D scan." << std::endl;
      TH1 *h_fit = (TH1*)obj;
      h_fit->SetDirectory(nullptr);
      target->cd();
      TKey *key_g = (TKey*)gDirectory->GetListOfKeys()->FindObject("h_dChi2_Total");
      if (!key_g) {
        std::unique_ptr<TH1D> h_totalfit = std::make_unique<TH1D>(
                                             "Chi2_1DFit", "Chi2_1DFit",
                                             Nbins /*h_fit->GetXaxis()->GetNbins()*/,
                                             *lowlim, *highlim);

        for (int bin = 1; bin <= h_fit->GetNbinsX(); bin++) {
          double bincentre_x = h_fit->GetXaxis()->GetBinCenter(bin);
          double bincontent = h_fit->GetBinContent(bin);
          std::cout << "Chi2 = " << bincontent << std::endl;
          int binx_tot = h_totalfit->GetXaxis()->FindBin(bincentre_x);
          if (std::isnormal(bincontent)) {
            h_totalfit->SetBinContent(binx_tot, bincontent); 
          } else {
            h_totalfit->SetBinContent(binx_tot, 0);
          }
        }
        h_totalfit.release()->Write("h_dChi2_Total");
      } else {  

        std::cout << "Updating previous 1D Chi2 surface." << std::endl;
        TH1D *h_update = (TH1D*)target->Get("h_dChi2_Total");

        for (int bin = 1; bin <= h_fit->GetNbinsX(); bin++) {
          double bincentre_x = h_fit->GetXaxis()->GetBinCenter(bin);
          double bincontent = h_fit->GetBinContent(bin);
          std::cout << "Chi2 = " << bincontent << std::endl;
          int binx_tot = h_update->GetXaxis()->FindBin(bincentre_x);
          if (std::isnormal(bincontent)) {
            h_update->SetBinContent(binx_tot, bincontent);
          } else {
            h_update->SetBinContent(binx_tot, 0);
          }
        }

        target->cd();
        gDirectory->Delete("h_dChi2_Total;*");
        h_update->Write("h_dChi2_Total");
        delete h_update;
      }
      delete h_fit; 
    } else if (keyname == "dChi2Scan" && obj->IsA() == TH2D::Class()) {
      std::cout << "Found 2D scan." << std::endl;
      TH2 *h_fit = (TH2*)obj;
      h_fit->SetDirectory(nullptr);
      target->cd();

      TKey *key_g = (TKey*)gDirectory->GetListOfKeys()->FindObject("h_dChi2_Total");
      // 2D graph not yet made, so make a new one
      if (!key_g) {
        std::unique_ptr<TH2D> h_totalfit = std::make_unique<TH2D>(
                                             "Chi2_2DFit", "Chi2_2DFit",
                                             50, //h_fit->GetXaxis()->GetNbins(),
                                             ssth23_lowlim, ssth23_highlim,
                                             50,
                                             dmsq32_lowlim, dmsq32_highlim);
 
        for (int binx = 1; binx <= h_fit->GetNbinsX(); binx++) {
          for (int biny = 1; biny <= h_fit->GetNbinsY(); biny++) {
            double bincentre_x = h_fit->GetXaxis()->GetBinCenter(binx);
            double bincentre_y = h_fit->GetYaxis()->GetBinCenter(biny);
            double bincontent = h_fit->GetBinContent(binx, biny);

            int binx_tot = h_totalfit->GetXaxis()->FindBin(bincentre_x);       
            int biny_tot = h_totalfit->GetYaxis()->FindBin(bincentre_y);
            //std::cout << "Chi2 = " << bincontent << std::endl;
            h_totalfit->SetBinContent(binx_tot, biny_tot, bincontent);
          }
        }
        h_totalfit.release()->Write("h_dChi2_Total");
      } else {
        std::cout << "Updating previous 2D Chi2 surface." << std::endl;
        TH2D *h_update = (TH2D*)target->Get("h_dChi2_Total");
        for (int binx = 1; binx <= h_fit->GetNbinsX(); binx++) {
          for (int biny = 1; biny <= h_fit->GetNbinsY(); biny++) {
            //std::cout << h_fit->GetXaxis()->GetBinCenter(binx) 
            //  << " ; " << h_fit->GetYaxis()->GetBinCenter(biny) 
            //  << " ; " << h_fit->GetBinContent(binx, biny) << std::endl; 
            double bincentre_x = h_fit->GetXaxis()->GetBinCenter(binx);
            double bincentre_y = h_fit->GetYaxis()->GetBinCenter(biny);
            double bincontent = h_fit->GetBinContent(binx, biny);

            int binx_tot = h_update->GetXaxis()->FindBin(bincentre_x);
            int biny_tot = h_update->GetYaxis()->FindBin(bincentre_y);

            h_update->SetBinContent(binx_tot, biny_tot, bincontent);
          }
        }
        target->cd();
        gDirectory->Delete("h_dChi2_Total;*");
        std::cout << "Write TGraph2D" << std::endl;
        h_update->Write("h_dChi2_Total");
        delete h_update;
      }                                           
      delete h_fit;
    } else {
      target->cd();
      TKey *key3 = (TKey*)gDirectory->GetListOfKeys()->FindObject(key->GetName());
      if (!key3) {
        obj->Write(key->GetName());
      }
      delete obj;      
    }
  }
  target->SaveSelf(kTRUE);
}


void usage() {
  std::cout << "Usage: hadd_fits scan_param output.root input1.root input2.root ..." << std::endl
    << "Or can wildcard input files." << std::endl;
  exit(1);
}

int main(int argc, char** argv) {

  if(argc < 3 ||
     argv[1] == std::string("-h") ||
     argv[1] == std::string("--help")) usage();

  std::string scan_param = argv[1];
  if (scan_param == "dmsq32") {
    std::cout << "Scan parameter is " << scan_param << std::endl;
  } else if (scan_param == "ssth23") {
    std::cout << "Scan parameter is " << scan_param << std::endl;
  } else if (scan_param == "dcp") {
    std::cout << "Scan parameter is " << scan_param << std::endl;
  } else {
    std::cout << "[ERROR] Do not recognise this scan parameter." << std::endl;
    abort();
  }

  std::string outfilename = argv[2]; 
  std::cout << "Output file is " << outfilename << std::endl;

  std::vector<std::string> infilenames;
  for (int i = 3; i < argc; i++) {
    std::string str(argv[i]);
    infilenames.push_back(ana::pnfs2xrootd(str));
  }

  for (const auto &inf : infilenames) {
     std::cout << "Input file is " << inf << std::endl;
  }

  Target = TFile::Open(outfilename.c_str(), "RECREATE");

  int Nbins = infilenames.size();

  for(const std::string& fname : infilenames) {
    TFile *fin = TFile::Open(fname.c_str());
    if (!fin || fin->IsZombie()) {
      std::cout << "[ERROR] failed to open " << fname << std::endl;
      exit(1);
    }
    std::cout << "Opened " << fname << std::endl;

    MergeFits(Target, fin, scan_param, Nbins);
    fin->Close();
  }

  Target->Close();
 
  return 0; 
}
