#include "CAFAna/Core/FixupRecord.h"

#include "CAFAna/Core/ModeConversionUtilities.h"
#include "CAFAna/Systs/XSecSystList.h"
#include "CAFAna/Analysis/AnalysisVersion.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "TTree.h"

#include <cmath>
#include <iostream>

namespace ana
{
  void FixupRecord(caf::SRProxy* sr, TTree* tr)
  {
    // Set GENIE_ScatteringMode and eRec_FromDep
    if(sr->isFD){
      sr->eRec_FromDep = sr->eDepP + sr->eDepN + sr->eDepPip +
                         sr->eDepPim + sr->eDepPi0 +
                         sr->eDepOther + sr->LepE;

      sr->GENIE_ScatteringMode =
          ana::GetGENIEModeFromSimbMode(sr->mode);
    }
    else {
      sr->eRec_FromDep = sr->eRecoP + sr->eRecoN +
                         sr->eRecoPip + sr->eRecoPim +
                         sr->eRecoPi0 + sr->eRecoOther +
                         sr->LepE;
      sr->GENIE_ScatteringMode = int(sr->mode); // TODO make this work without cast?
    }

    // Patch up isFHC which isn't set properly in FD CAFs
    if(sr->isFD){
      if(sr->isFHC != 0 && sr->isFHC != 1){
        if(sr->run == 20000001 || sr->run == 20000002 ||
           sr->run == 20000003) {
          sr->isFHC = true;
          static bool once = true;
          if(once) {
            std::cout << "\nPatching up FD file to be considered FHC"
                      << std::endl;
            once = false;
          }
        }
        else if(sr->run == 20000004 || sr->run == 20000005 ||
                sr->run == 20000006) {
          sr->isFHC = false;
          static bool once = true;
          if(once){
            std::cout << "\nPatching up FD file to be considered RHC"
                      << std::endl;
            once = false;
          }
        }
        else {
          std::cout
            << "When patching FD CAF with unknown isFHC, saw unknown run "
            << sr->run << std::endl;
          abort();
        }
      }
    }
    else{
      // ND
      if(sr->isFHC == -1){
        // nu-on-e files
        sr->isFHC = true;
        static bool once = true;
        if (once) {
          std::cout << "\nPatching up nu-on-e file to be considered FHC"
                    << std::endl;
          once = false;
        }
      }
      else if (sr->isFHC != 0 && sr->isFHC != 1) {
        std::cout << "isFHC not set properly in ND file: " << sr->isFHC
                  << std::endl;
        abort();
      }
    }

    if(tr->GetNbranches() == 302 || tr->GetNbranches() == 280 /*ndgas*/){
      static bool once = true;
      if(once){
        once = false;
        std::cout << "Detected TDR-era file. Skipping CV weights, which aren't going to work" << std::endl;
      }
      return;
    }

    // These aren't going to change during the job, so do it once up-front.
    static const std::vector<std::string>& XSSyst_names = GetAllXSecSystNames();

    // HACK HACK HACK for knobs that aren't in file
    static bool hackOnce = true;
    static std::vector<bool> veto(XSSyst_names.size());
    if(hackOnce){
      hackOnce = false;
      for(unsigned int syst_it = 0; syst_it < XSSyst_names.size(); ++syst_it){
        if(ana::GetXSecSystName(syst_it) == "Mnv2p2hGaussEnhancement_NN" ||
           ana::GetXSecSystName(syst_it) == "Mnv2p2hGaussEnhancement_2p2h" ||
           ana::GetXSecSystName(syst_it) == "Mnv2p2hGaussEnhancement_1p1h" ||
           ana::GetXSecSystName(syst_it) == "MissingProtonFakeData" ||
           ana::GetXSecSystName(syst_it) == "NuWroReweightFakeData"
           ) veto[syst_it] = true;
      }
    }

    // HACK to survive the absence of crazyFlux values in the file
    sr->wgt_CrazyFlux = std::vector<double>(7, 1);

    // Reformat the genie systs
    sr->total_xsSyst_cv_wgt = 1;

    std::vector<float> XSSyst_cv_tmp(XSSyst_names.size());
    for(unsigned int syst_it = 0; syst_it < XSSyst_names.size(); ++syst_it){
      XSSyst_cv_tmp[syst_it] = 1;
    }

    static auto AnaV = GetAnaVersion();
    if(AnaV == kV3){
      for(unsigned int syst_it = 0; syst_it < XSSyst_names.size(); ++syst_it){
        // Do some error checking here
        if(std::isnan(sr->cvwgt[syst_it]) ||
           std::isinf(sr->cvwgt[syst_it]) ||
           sr->cvwgt[syst_it] == 0){
          std::cout << "Warning: " << XSSyst_names[syst_it]
                    << " has a bad CV of " << sr->cvwgt[syst_it]
                    << std::endl;
        }
        else {
          sr->total_xsSyst_cv_wgt *= sr->cvwgt[syst_it];
        }
      }
    }
    else{
      static std::vector<bool> alreadyWarned(XSSyst_names.size(), false);

      for(unsigned int syst_it = 0; syst_it < XSSyst_names.size(); ++syst_it){
        // Continuation of the hack from further up
        if(veto[syst_it]) continue;

        if(IsDoNotIncludeSyst(syst_it)){ // Multiply CV weight back into
                                         // response splines.
          if(std::isnan(sr->cvwgt[syst_it]) ||
             std::isinf(sr->cvwgt[syst_it]) ||
             XSSyst_cv_tmp[syst_it] == 0) {
            if(!alreadyWarned[syst_it]){
              alreadyWarned[syst_it] = true;
              std::cout << "Warning: " << XSSyst_names[syst_it]
                        << " has a bad CV of " << sr->cvwgt[syst_it]
                        << " - will only warn once"
                        << std::endl;
            }
          }
          else{
            const int Nuniv = 7; // HACK HACK HACK sr->xsSyst_wgt[syst_it].size();
            for(int univ_it = 0; univ_it < Nuniv; ++univ_it){
              sr->xsSyst_wgt[syst_it][univ_it] *= sr->cvwgt[syst_it];
            }
          }
        }
        else{ // Include CV weight in the total
          // Do some error checking here
          if(std::isnan(sr->cvwgt[syst_it]) ||
             std::isinf(sr->cvwgt[syst_it]) ||
             sr->cvwgt[syst_it] == 0) {
            if(!alreadyWarned[syst_it]){
              alreadyWarned[syst_it] = true;
              std::cout << "Warning: " << XSSyst_names[syst_it]
                        << " has a bad CV of " << sr->cvwgt[syst_it]
                        << " - will only warn once"
                        << std::endl;
            }
          }
          else{
            sr->total_xsSyst_cv_wgt *= sr->cvwgt[syst_it];
          }
        }
      }
    } // end version switch
  }
}
