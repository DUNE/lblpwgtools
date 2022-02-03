// Script to add together state files for PRISM.
// Replicates state file format when adding together state files.
// Takes care to add the syst_names histogram, this is important
// for loading PredictionInterp objects when doing systematic
// studies.
// C. Hasnip 6/5/2021
//

#include "CAFAna/Core/Utilities.h"

#include <string>
#include <iostream>

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "TClass.h"
#include "TObject.h"
#include "TObjString.h"
#include "TROOT.h"

TFile *Target;
//gROOT->SetMustClean(false);

// TODO: do this with just one function. Two is not necessary
void MergeFilesLayer2(TDirectory *target, TFile *source);
void MergeFiles( TDirectory *target, TFile *source );

void MergeFiles( TDirectory *target, TFile *source ) {
  TString path( (char*)strstr( target->GetPath(), ":" ) );
  path.Remove( 0, 2 );
  source->cd( path );
  TDirectory *current_sourcedir = gDirectory;

  //gain time, do not add the objects in the list in memory
  Bool_t status = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);

  TIter nextkey( current_sourcedir->GetListOfKeys() );
  TKey *key;

  while ((key = (TKey*)nextkey())) {

    // read object from first source file
    source->cd( path );
    TObject *obj = key->ReadObj();

    if (obj->IsA()->InheritsFrom(TDirectory::Class())) {
      // subdirectory
      // everything in the first layer of the file should be a subdirectory
      std::cout << "Found subdirectory " << obj->GetName() << std::endl;
      
      // create a new subdir of same name and title in the target file
      target->cd();
      if (!target->FindObject(key->GetName())) {
        TDirectory *newdir = target->mkdir(obj->GetName(), obj->GetTitle());
        MergeFilesLayer2(newdir, source);
      } else {
        TDirectory *existdir = target->GetDirectory(obj->GetName());
        MergeFilesLayer2(existdir, source);
      }
    }

  }
}

void MergeFilesLayer2(TDirectory *target, TFile *source) {
  TString path( (char*)strstr( target->GetPath(), ":" ) ); 
  path.Remove(0, 2);
  source->cd( path );
  TDirectory *current_sourcedir = gDirectory;

  //gain time, do not add the objects in the list in memory
  Bool_t status = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);

  TIter nextkey(current_sourcedir->GetListOfKeys()); 
  TKey *key;

  std::string systname = "syst_names";

  while ((key = (TKey*)nextkey())) {
    source->cd( path );
    TObject *obj = key->ReadObj();
    std::string keyname(key->GetName());

    target->cd();
    if (obj->IsA()->InheritsFrom(TDirectory::Class())) {
      if (!target->FindObject(key->GetName())) {
        //target->cd();
        TDirectory *newdir = target->mkdir(obj->GetName(), obj->GetTitle());
        MergeFilesLayer2(newdir, source);
      } else {
        //std::cout << "Subdirectory "<< obj->GetName() << " already exists." << std::endl;
        TDirectory *existdir = target->GetDirectory(obj->GetName());
        MergeFilesLayer2(existdir, source);
      }
    } else if (keyname == "syst_names") {
      TH1 *h_newsysts = (TH1*)obj;
      h_newsysts->SetDirectory(nullptr);
      target->cd();
      TKey *key2 = (TKey*)gDirectory->GetListOfKeys()->FindObject("syst_names");

      if (!key2) {
        h_newsysts->Write("syst_names");
      } else {  
        std::cout << "new syst_names hist!" << std::endl;
        TH1 *h_oldsysts = (TH1*)target->Get("syst_names");
        h_oldsysts->SetDirectory(nullptr);
        int Nsysts = h_oldsysts->GetNbinsX() + h_newsysts->GetNbinsX();
        TH1F hSystNames("syst_names", ";Syst names", Nsysts, 0, Nsysts);
        for (int bin = 1; bin <= h_oldsysts->GetNbinsX(); bin++) {
          hSystNames.GetXaxis()->SetBinLabel(bin, h_oldsysts->GetXaxis()->GetBinLabel(bin));
        }
        for (int bin = h_oldsysts->GetNbinsX()+1; bin <= Nsysts; bin++) {
          hSystNames.GetXaxis()->SetBinLabel(bin, 
               h_newsysts->GetXaxis()->GetBinLabel(bin - h_oldsysts->GetNbinsX()));
        }
        target->cd();
        gDirectory->Delete("syst_names;*");
        delete h_oldsysts;
        hSystNames.Write("syst_names");
      }
      delete h_newsysts; 
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

  gROOT->SetMustClean(false);

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

  Target = TFile::Open(outfilename.c_str(), "RECREATE");

  for(std::string& fname : infilenames) {
    fname = ana::pnfs2xrootd(fname);
    TFile *fin = TFile::Open(fname.c_str());
    if (!fin || fin->IsZombie()) {
      std::cout << "[ERROR] failed to open " << fname << std::endl;
      exit(1);
    }
    std::cout << "Opened " << fname << std::endl;

    MergeFiles(Target, fin);
    fin->Close();
  }
 
  return 0; 
}                                                                                              
