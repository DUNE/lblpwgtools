// Script to add together fits
// C. Hasnip 16/5/2021
//


#include <string>
#include <iostream>
#include <algorithm>

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "TClass.h"
#include "TObject.h"
#include "TObjString.h"

TFile *Target;
TFile *UpdateTarget;

void MergeFits(TDirectory *target, TFile *source);

void MergeFits(TDirectory *target, TFile *source) {
  TString path( (char*)strstr( target->GetPath(), ":" ) ); 
  path.Remove(0, 2);
  source->cd( path );
  TDirectory *current_sourcedir = gDirectory;
  TIter nextkey(current_sourcedir->GetListOfKeys()); 
  TKey *key;

  std::string systname = "syst_names";

  while ((key = (TKey*)nextkey())) {
    source->cd( path );
    TObject *obj = key->ReadObj();
    std::string keyname(key->GetName());

    if (obj->IsA()->InheritsFrom(TDirectory::Class())) {
      std::cout << "dir name will be: " << keyname << std::endl;

      if (!target->FindObject(keyname.c_str())) {
        std::cout << "New dir " << keyname << std::endl;
        target->cd();
        TDirectory *newdir = target->mkdir(keyname.c_str(), keyname.c_str());
        MergeFits(newdir, source);
      } else {
        std::cout << "Update existing dir " << keyname << std::endl;
        TDirectory *existdir = target->GetDirectory(keyname.c_str());
        MergeFits(existdir, source);
      }
    } else if (keyname == "dChi2Scan" && obj->IsA()->InheritsFrom(TH1::Class())) {
      TH1 *h_fit = (TH1*)obj;
      h_fit->SetDirectory(nullptr);
      target->cd();
      TKey *key_g = (TKey*)gDirectory->GetListOfKeys()->FindObject("g_dchi2_1D");

      if (!key_g) {
        std::unique_ptr<TGraph> g_chi2surf = std::make_unique<TGraph>(
                                               h_fit->GetXaxis()->GetNbins());
        
        for (int bin = 1; bin <= h_fit->GetNbinsX(); bin++) {
          g_chi2surf->SetPoint(0, h_fit->GetBinCenter(bin),
                               h_fit->GetBinContent(bin));
        }
        g_chi2surf.release()->Write("g_dchi2_1D");
      } else {  

        std::cout << "Updating previous Chi2 surface." << std::endl;
        TGraph *g_update = (TGraph*)target->Get("g_dchi2_1D");        

        for (int bin = 1; bin <= h_fit->GetNbinsX(); bin++) {
          g_update->SetPoint(g_update->GetN(), h_fit->GetBinCenter(bin),
                          h_fit->GetBinContent(bin)); 
        }

        target->cd();
        gDirectory->Delete("g_dchi2_1D;*");
        g_update->Write("g_dchi2_1D");
        delete g_update;
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

void ConvertFitToHist(TDirectory *target, TFile *source, double h_lowlim, double h_highlim) {
  TString path( (char*)strstr( target->GetPath(), ":" ) );
  path.Remove(0, 2);
  std::cout << "path = " << path << std::endl;
  source->cd( path );
  TDirectory *current_sourcedir = gDirectory;
  TIter nextkey(current_sourcedir->GetListOfKeys());
  TKey *key;

  while ((key = (TKey*)nextkey())) {
    source->cd(path);
    TObject *obj = key->ReadObj();
    std::string keyname(key->GetName());

    if (obj->IsA()->InheritsFrom(TDirectory::Class())) {
      if (!target->FindObject(keyname.c_str())) {
        target->cd();
        TDirectory *newdir = target->mkdir(keyname.c_str(), keyname.c_str());
        ConvertFitToHist(newdir, source, h_lowlim, h_highlim);    
      } else {
        std::cout << "Update existing dir " << keyname << std::endl;
        TDirectory *existdir = target->GetDirectory(keyname.c_str());
        ConvertFitToHist(existdir, source, h_lowlim, h_highlim);
      } 
    } else if (keyname == "g_dchi2_1D" && obj->IsA()->InheritsFrom(TGraph::Class())) {
      
      source->cd(path);
      TKey *key_g = (TKey*)gDirectory->GetListOfKeys()->FindObject("g_dchi2_1D");
      
      if (!key_g) {
        std::cout << "[ERROR] Contour TGraph not found." << std::endl;
      } else {  
      
        std::cout << "Updating previous Chi2 surface." << std::endl;
        TGraph *g_update = (TGraph*)gDirectory->Get("g_dchi2_1D"); 

        std::unique_ptr<TH1D> h_totalfit = std::make_unique<TH1D>(
                               "Chi2_1DFit", "Chi2_1DFit", g_update->GetN(),
                               h_lowlim, h_highlim);        
        h_totalfit->SetDirectory(nullptr);
        std::cout << "N points = " << g_update->GetN() << std::endl;
        for (int point = 0; point < g_update->GetN(); point++) {
          double bincentre = g_update->GetX()[point];
          double binheight = g_update->GetY()[point];
          std::cout << "centre = " << bincentre << " ; height = " << binheight << std::endl;
          int bin = h_totalfit->FindBin(bincentre);
          if (bin != 0) h_totalfit->SetBinContent(bin, binheight);
          std::cout << "Filling " << bin << " ; " << binheight << std::endl;
        } 
        target->cd();
        h_totalfit.release()->Write("Chi2_Total1DFit");
        delete g_update;
        //delete h_totalfit;
      }
      
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
  std::cout << "Usage: hadd_state output.root input1.root input2.root ..." << std::endl;
  exit(1);
}

int main(int argc, char** argv) {

  if(argc < 3 ||
     argv[1] == std::string("-h") ||
     argv[1] == std::string("--help")) usage();

  std::string outfilename = argv[1]; 
  std::cout << "Output file is " << outfilename << std::endl;

  std::vector<std::string> infilenames;
  for (int i = 2; i < argc; i++) {
    std::string str(argv[i]);
    infilenames.push_back(str);
  }

  for (const auto &inf : infilenames) {
     std::cout << "Input file is " << inf << std::endl;
  }

  //Target = TFile::Open(outfilename.c_str(), "RECREATE");
  Target = TFile::Open("intermediateF", "RECREATE"); 

  for(const std::string& fname : infilenames) {
    TFile *fin = TFile::Open(fname.c_str());
    if (!fin || fin->IsZombie()) {
      std::cout << "[ERROR] failed to open " << fname << std::endl;
      exit(1);
    }
    std::cout << "Opened " << fname << std::endl;

    MergeFits(Target, fin);
  }
  //double h_lowlim = 0.4;
  double h_lowlim = 2.30;
  //double h_highlim = 0.65;
  double h_highlim = 2.55;

  UpdateTarget = TFile::Open(outfilename.c_str(), "RECREATE");

  ConvertFitToHist(UpdateTarget, Target, h_lowlim, h_highlim);
 
  return 0; 
}
