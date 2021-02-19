// Run after each change to CAFAna
// Either manually or in unit test
#pragma once

#include "helper_validation_lbl.h"

#include "CAFAna/Systs/EnergySysts.h"
#include "CAFAna/Systs/FDRecoSysts.h"

using namespace ana;

void reduce_predfile()
{

  std::vector<const ISyst*> systlist = {&kFDRecoNumuSyst};

  std::vector<std::string> neutrino = {"nue","numu"};
  std::vector<std::string> detector = {"nd","fd"};
  std::vector<std::string> horn     = {"fhc","rhc"};

  const int nhorn = horn.size();
  const int ndetector = detector.size();
  const int nneutrino = neutrino.size();

  ///// Load Predictions once
  std::string dloc = "/pnfs/dune/persistent/users/dmendez/lbl_fit_validation/";

  TString starget = "reduced_lbl_predictions.root";
  TFile* ftarget  = new TFile(starget, "RECREATE");

  for(int detId=0; detId<ndetector; detId++){
    for(int hornId=0; hornId<nhorn; hornId++){
      for(int nuId=0; nuId<nneutrino; nuId++){

        if(detector[detId]=="nd" && neutrino[nuId]=="nue") continue; // no nd nue prediction

        std::string fname = dloc+Form("state_%s_%s.root", detector[detId].c_str(), horn[hornId].c_str());
        std::string dname = Form("%s_interp_%s_%s", detector[detId].c_str(), neutrino[nuId].c_str(), horn[hornId].c_str());
        TFile* file       = TFile::Open(fname.c_str(), "READ");
        PredictionInterp* prediction = LoadFrom<PredictionInterp>(file, dname).release();
        if(prediction){
          std::cout << "found prediction" << std::endl;
          prediction->ReducedSaveTo(ftarget, dname.c_str(), systlist);
          std::cout << "saved!" << std::endl;
        }
        if(!prediction){
          std::cout << "prediction not found. skipping." << std::endl;
          continue;
        }
        
        file->Close();

      } // nuId
    } // hornId
  } // detId

  ftarget->Close();

}
