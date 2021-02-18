// Run after each change to CAFAna
// Either manually or in unit test
#pragma once

#include "helper_validation_lbl.h"

#include "CAFAna/Systs/EnergySysts.h"
#include "CAFAna/Systs/FDRecoSysts.h"

using namespace ana;

void reduce_predfile()
{
  std::vector<std::string> stdobjects  = {"osc_origin","pred_nom"};
  std::vector<std::string> systobjects = {"pred_EnergyScale","pred_FDRecoNumuSyst"};
  std::vector<std::string> systshifts  = {"_+0","_+1","_+2","_+3","_-1","_-2","_-3"};

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
        std::string objname = dname+"/pred_nom";
        /*
        // std::string objname = "pred_nom";
        std::cout << "dname: " << dname << ", objname: " << objname << std::endl;
        TDirectoryFile* prediction = (TDirectoryFile*)file->Get(objname.c_str());
        if(prediction){
          std::cout << "found prediction" << std::endl;
        }
        if(!prediction){
          std::cout << "prediction not found" << std::endl;
        }
        // file->Close();
        
        std::cout << "target file cd" << std::endl;
        ftarget->cd();
        ftarget->mkdir(dname.c_str(),"osc_origin");
        ftarget->cd(dname.c_str());
        gDirectory->WriteObject(prediction,"pred_nom");
        //file->Close();
        */
        ftarget->cd();
        ftarget->WriteObject(file->GetDirectory(objname.c_str()), objname.c_str());

        file->Close();

      } // nuId
    } // hornId
  } // detId

  ftarget->Close();

}
