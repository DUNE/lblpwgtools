

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

std::map<int, TH1D *> SumFileExposures(std::vector<std::string> infilenames) {

  std::vector<int> SpecRunIds_all = {-293, -280, 280, 293};

  std::map<int, TH1D *> FileExposures;

  int fileIdx = -1;

  for(std::string& fname : infilenames) {
    ++fileIdx;
    fname = ana::pnfs2xrootd(fname);
    TFile *fin = TFile::Open(fname.c_str());
    if (!fin || fin->IsZombie()) {
      std::cout << "[ERROR] failed to open " << fname << std::endl;
      exit(1);
    }
    std::cout << "Opened " << fname << std::endl;
                                                                      
    for (int SpecRunID_local : SpecRunIds_all) {
      std::stringstream ss("");
      ss << ((SpecRunID_local < 0) ? "m" : "") << SpecRunID_local;
  
      TH1D *f_FileExposure;
      fin->GetObject(("FileExposure_" + ss.str()).c_str(), f_FileExposure);
      if (f_FileExposure) {
        if (fileIdx > 0) {
          assert(FileExposures[SpecRunID_local]);
          FileExposures[SpecRunID_local]->Add(f_FileExposure);
        } else {
          FileExposures[SpecRunID_local] = new TH1D(*f_FileExposure);
          FileExposures[SpecRunID_local]->SetDirectory(nullptr);
        }
      }
    }
    fin->Close();
  }

  return FileExposures;
}

void usage() {
  std::cout << "Usage: OffAxisPOTCalc input1.root input2.root ..." << std::endl;
  exit(1);
}

int main(int argc, char** argv) {

  gROOT->SetMustClean(false);

  if(argc < 3 ||
     argv[1] == std::string("-h") ||
     argv[1] == std::string("--help")) usage();

  std::vector<std::string> infilenames;
  for (int i = 1; i < argc; i++) {
    std::string str(argv[i]);
    infilenames.push_back(str);
  }

  for (const auto &inf : infilenames) {
     std::cout << "Input file is " << inf << std::endl;
  }

  std::map<int, TH1D *> SummedFileExposures = SumFileExposures(infilenames);

  std::string outfilename = "TotalOffAxisFileExposure.root";
  std::cout << "Output file is " << outfilename << std::endl;

  TFile *Target = TFile::Open(outfilename.c_str(), "RECREATE");

  std::vector<int> SpecRunIds_all = {-293, -280, 280, 293};

  for (int SpecRunID_local : SpecRunIds_all) {
    Target->cd(); 
    std::stringstream ss("");
    ss << ((SpecRunID_local < 0) ? "m" : "") << SpecRunID_local;

    SummedFileExposures[SpecRunID_local]->Write(("FileExposure_" + ss.str()).c_str()); 
  } 
 
  return 0; 
}                                                                                           
