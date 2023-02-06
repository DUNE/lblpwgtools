#include "CAFAna/Analysis/AnalysisDialGroups.h"
#include "CAFAna/Analysis/AnalysisVersion.h"

#include "CAFAna/Systs/AnaSysts.h"
#include "CAFAna/Systs/CrazyFluxFakeData.h"
#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/EnergySysts.h"
#include "CAFAna/Systs/FDRecoSysts.h"
#include "CAFAna/Systs/RecoEnergyNDSysts.h"
#include "CAFAna/Systs/NDRecoSysts.h"
#include "CAFAna/Systs/NuOnESysts.h"
#include "CAFAna/Systs/XSecSysts.h"

#include "TFile.h"
#include "TH1.h"

#include <sstream>
#include <string>
#include <vector>

size_t NFluxParametersToAddToStatefile = 25;

namespace ana
{
  // Because most particle physics is stuck in C++
  // TO DO: Define this elsewhere, in some kind of general Utilities
  std::vector<std::string> SplitString(std::string input, char delim) {
    std::vector<std::string> output;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, delim))
      output.push_back(token);
    return output;
  }

  // Take a list of all the systs known about, and retain the named systs
  void KeepSysts(std::vector<const ISyst *> &systlist,
                 std::vector<std::string> const &systsToInclude){
    systlist.erase(std::remove_if(systlist.begin(), systlist.end(),
                                  [&](const ISyst *s) {
                                    return (std::find(systsToInclude.begin(),
                                                      systsToInclude.end(),
                                                      s->ShortName()) ==
                                            systsToInclude.end());
                                  }), systlist.end());
  }

  void KeepSysts(std::vector<const ISyst *> &systlist,
                 std::vector<const ISyst *> const &systsToInclude){

    systlist.erase(std::remove_if(systlist.begin(), systlist.end(),
                                  [&](const ISyst *s) {
                                    return (std::find(systsToInclude.begin(),
                                                      systsToInclude.end(),
                                                      s) == systsToInclude.end());
                                  }), systlist.end());
  }

  void RemoveSysts(std::vector<const ISyst *> &systlist,
                   std::vector<std::string> const &namesToRemove){

    systlist.erase(std::remove_if(systlist.begin(), systlist.end(),
                                  [&](const ISyst *s) {
                                    return (std::find(namesToRemove.begin(),
                                            namesToRemove.end(),
                                            s->ShortName()) != namesToRemove.end());
                                  }), systlist.end());
  }

  std::vector<const ISyst*> getDetectorSysts(bool useFD, bool useND, bool useNueOnE)
  {

    std::vector<const ISyst*> systs = {};

    std::vector<const ISyst *> fdEScalelist = GetEnergySysts();
    std::vector<const ISyst *> fdlist = GetFDRecoSysts();
    std::vector<const ISyst *> ndEScalelist = GetRecoENDSysts();
    std::vector<const ISyst *> ndlist = GetNDRecoSysts();
    std::vector<const ISyst *> nuelist = GetNuOnESysts();

    if (useFD) {
      systs.insert(systs.end(), fdEScalelist.begin(), fdEScalelist.end());
      systs.insert(systs.end(), fdlist.begin(), fdlist.end());
    }
    if (useND) {
      systs.insert(systs.end(), ndEScalelist.begin(), ndEScalelist.end());
      systs.insert(systs.end(), ndlist.begin(), ndlist.end());
    }
    if (useND && useNueOnE) {
      systs.insert(systs.end(), nuelist.begin(), nuelist.end());
    }

    return systs;
  }

  std::vector<const ISyst*> getReducedXSecSysts(bool fluxXsecPenalties){

    std::vector<const ISyst*> systs = GetXSecSysts(GetAllXSecSystNames(), fluxXsecPenalties);
    // Always remove these dials
    // TO DO: Only add the dials to actually use and avoid the need of RemoveSysts
    // Then we won't have to worry about systs addition ordering
    RemoveSysts(systs, GetDoNotIncludeSystNames());

    return systs;
  }

  std::vector<const ISyst*> getFakeDataSysts(bool fluxXsecPenalties){

    std::vector<const ISyst*> systs = GetCrazyFluxSysts();

    std::vector<const ISyst *> xseclist = GetXSecSysts(GetAllXSecSystNames(), fluxXsecPenalties);
    KeepSysts(xseclist, GetFakeDataGenerationSystNames());
    systs.insert(systs.end(), xseclist.begin(), xseclist.end());

    return systs;
  }

  std::vector<const ISyst *> GetListOfSysts(bool fluxsyst_Nov17, bool xsecsyst,
                                            bool detsyst, bool useND, bool useFD,
                                            bool useNueOnE, bool useFakeDataDials,
                                            bool fluxsyst_CDR, bool fluxsyst_Sept21,
                                            int NFluxSysts, bool removeFDNonFitDials) {

    // This doesn't need to be an argument because I basically never change it:
    bool fluxXsecPenalties = true;

    std::vector<const ISyst *> systlist;
    if (fluxsyst_Nov17) {
      std::vector<const ISyst *> fluxsyst_Nov17 =
          GetDUNEFluxSysts(NFluxSysts, fluxXsecPenalties, false);
      systlist.insert(systlist.end(), fluxsyst_Nov17.begin(), fluxsyst_Nov17.end());
    }

    if (fluxsyst_CDR) { // CHECK: GetDUNEFluxSysts loading nothing when set to true
      std::vector<const ISyst *> fluxsyst_CDR =
          GetDUNEFluxSysts(NFluxSysts, fluxXsecPenalties, true);
      systlist.insert(systlist.end(), fluxsyst_CDR.begin(), fluxsyst_CDR.end());
    }

    if (fluxsyst_Sept21) {
      std::vector<const ISyst *> fluxsyst_Sept21 =
          GetDUNEFluxSysts(NFluxSysts, fluxXsecPenalties, false, true);
      systlist.insert(systlist.end(), fluxsyst_Sept21.begin(), fluxsyst_Sept21.end());
    }

    if (detsyst) {
      std::vector<const ISyst *> detsyst = getDetectorSysts(useFD,useND,useNueOnE);
      systlist.insert(systlist.end(), detsyst.begin(), detsyst.end());
    }

    if (xsecsyst) {
      // This function removes some dials. Don't know why it has to be so
      // Those will need to be added again if using fake data. So always add fake data last
      std::vector<const ISyst *> xsecsyst = getReducedXSecSysts(fluxXsecPenalties);
      systlist.insert(systlist.end(), xsecsyst.begin(), xsecsyst.end());
    }

    // If using fake data dials (for state generation) add the previously removed systs back in
    if (useFakeDataDials) {
      std::vector<const ISyst *> fakedatasyst = getFakeDataSysts(fluxXsecPenalties);
      systlist.insert(systlist.end(), fakedatasyst.begin(), fakedatasyst.end());
    }

    return systlist;
  }

  std::vector<const ISyst *> GetListOfSysts(std::string systString, bool useND,
                                            bool useFD, bool useNueOnE) {

    if (systString == "fakedata") {
      return GetListOfSysts(false, false, false, false, false, false,
                            true /*add fake data*/, false, false);
    }

    // Now defaults to true!
    bool detsyst = true;
    bool fluxsyst_Nov17 = (GetAnaVersion() == kV3) ? false : true;
    bool fluxsyst_CDR = (GetAnaVersion() == kV3) ? true : false;
    bool fluxsyst_Sept21 = true;
    bool xsecsyst = true;
    bool useFakeData = false;
    int NFluxSysts =
        (GetAnaVersion() == kV3) ? 10 : NFluxParametersToAddToStatefile;

    // If you find an argument in the form list:name1:name2:name3 etc etc, keep
    // only those systematics This is pretty much a magic option to allow single
    // parameters... there must be a better way, but for now I'm just going to
    // continue to support it
    if (systString.find("list") != std::string::npos) {

      // 1) Get a default list with everything
      std::vector<const ISyst *> namedList =
          GetListOfSysts(true, true, true, useND, useFD, useNueOnE,
                         false /*no fake data*/, true /*Get CDR flux systs*/,
                         false /* Default to Nov17 flux systs for now*/);

      // 2) Interpret the list of short names
      std::vector<std::string> systs = SplitString(systString, ':');

      // 3) Don't include "list"
      systs.erase(systs.begin());

      // 4) Regret nothing
      KeepSysts(namedList, systs);

      // 5) $$$ Profit
      return namedList;
    }

    // Can't transform anymore, so... BEHAVE YOURSELF
    // std::transform(systString.begin(), systString.end(), systString.begin(),
    // ::tolower);

    // Do even more horrific things...
    std::vector<std::string> systs = SplitString(systString, ':');

    // Start off by checking for certain keywords
    for (auto syst : systs) {
      if (syst == "allsyst") {
        xsecsyst = true;
        fluxsyst_Nov17 = (GetAnaVersion() == kV3) ? false : true;
        fluxsyst_CDR = (GetAnaVersion() == kV3) ? true : false;
        fluxsyst_Sept21 = false;
        detsyst = true;
      }

      if (syst == "nosyst") {
        xsecsyst = false;
        fluxsyst_Nov17 = false;
        fluxsyst_CDR = false;
        fluxsyst_Sept21 = false;
        detsyst = false;
      }

      // Now we're getting a bit funky as these options now conflict.
      // But, if you do something stupid, YOU ONLY HAVE YOURSELF TO BLAME
      if (syst == "nodet") {
        detsyst = false;
        fluxsyst_Sept21 = false; // Default to getting the Nov17 flux systs for now
      }
      if (syst == "noflux") {
        fluxsyst_CDR = false;
        fluxsyst_Nov17 = false;
        fluxsyst_Sept21 = false;
      }
      if (syst == "cdrflux") {
        fluxsyst_CDR = true;
        fluxsyst_Nov17 = false;
        fluxsyst_Sept21 = false;
      }
      if (syst == "nov17flux") {
        fluxsyst_CDR = false;
        fluxsyst_Nov17 = true;
        fluxsyst_Sept21 = false;
      }
      if (syst == "sept21flux") {
        fluxsyst_CDR = false;
        fluxsyst_Nov17 = false;
        fluxsyst_Sept21 = true;
      }
      if (syst == "fakedata") {
        useFakeData = true;
      } // LOOK MA, I GOT BRACES!

      if (syst.find("nflux=") == 0) {
        auto NFluxSplit = SplitString(syst, '=');
        if (NFluxSplit.size() != 2) {
          std::cout << "[ERROR]: Found NFlux option, but couldn't parse how many "
                       "to include, expected to find nflux=<0--"
                    << NFluxParametersToAddToStatefile << "> but found: \""
                    << syst << "\".";
          abort();
        }

        NFluxSysts = atoi(NFluxSplit[1].c_str());
      }
      if (syst == "noxsec") {
        xsecsyst = false;
        fluxsyst_Sept21 = false; // Default to getting the Nov17 flux systs for now
      }
    }

    // Okay, now get the list, and start from there...
    std::vector<const ISyst *> namedList =
        GetListOfSysts(fluxsyst_Nov17, xsecsyst, detsyst, useND, useFD, useNueOnE,
                       useFakeData, fluxsyst_CDR, fluxsyst_Sept21, NFluxSysts);

    // Now do something REALLY FUNKY. Remove specific dials from the list we
    // already have Need to allow single dials, and a few specific groups...
    for (auto syst : systs) {
      // ignore anything we previously dealt with
      if ((syst == "noxsec") || (syst == "nodet") || (syst == "noflux") ||
          (syst.find("nflux=") == 0)) {
        continue;
      }
      // Now remove some specific groups
      // nofd_det, nofd_escale, nofd_muon_escale, noxsec_qe, noxsec_res,
      // noxsec_dis, noxsec_fsi, noxsec_ratios
      else if (syst == "nofd_det") {
        RemoveSysts(namedList, fd_det_list);
      } else if (syst == "nofd_escale") {
        RemoveSysts(namedList, fd_escale_list);
      } else if (syst == "nofd_muon_escale") {
        RemoveSysts(namedList, fd_muon_escale_list);
      } else if (syst == "nofd_eres") {
        RemoveSysts(namedList, fd_eres_list);
      } else if (syst == "nofd_muon_eres") {
        RemoveSysts(namedList, fd_muon_eres_list);
      } else if (syst == "nofd_other_det") {
        RemoveSysts(namedList, fd_other_det_list);
      } else if (syst == "nofd_nonlin_det") {
        RemoveSysts(namedList, fd_nonlin_det_list);
      } else if (syst == "noxsec_qe") {
        RemoveSysts(namedList, GetXSecSystNames("QELike"));
      } else if (syst == "noxsec_res") {
        RemoveSysts(namedList, GetXSecSystNames("RES"));
      } else if (syst == "noxsec_dis") {
        RemoveSysts(namedList, GetXSecSystNames("DIS"));
      } else if (syst == "noxsec_fsi") {
        RemoveSysts(namedList, GetXSecSystNames("FSI"));
      } else if (syst == "noxsec_ratios") {
        RemoveSysts(namedList, GetXSecSystNames("Ratios"));
      }
      // If not, remove as if it's a single parameter instruction
      else {
        RemoveSysts(namedList, {syst.erase(0, 2)});
      }
    }

    if (GetAnaVersion() == kV3) {
      RemoveSysts(namedList,
                  {"UncorrFDTotSqrt", "UncorrFDTotInvSqrt", "UncorrFDHadSqrt",
                   "UncorrFDHadInvSqrt", "UncorrFDMuSqrt", "UncorrFDMuInvSqrt",
                   "UncorrFDNSqrt", "UncorrFDNInvSqrt", "UncorrFDEMSqrt",
                   "UncorrFDEMInvSqrt", "ChargedHadUncorrFD"});
    }

    // Now return the list
    return namedList;
  }

  std::vector<const ISyst *> GetListOfSysts(char const *systCString, bool useND,
                                            bool useFD, bool useNueOnE) {
    return GetListOfSysts(std::string(systCString), useND, useFD, useNueOnE);
  }

  std::vector<const ISyst *>
  OrderListOfSysts(std::vector<const ISyst *> const &systlist) {
    std::vector<const ISyst *> superorder;
    for (auto &s : GetListOfSysts("flux:noxsec:nodet")) {
      superorder.emplace_back(s);
    }
    for (auto &s : GetListOfSysts("cdrflux:noxsec:nodet")) {
      superorder.emplace_back(s);
    }
    for (auto &s : GetListOfSysts("noflux:xsec:nodet")) {
      superorder.emplace_back(s);
    }
    for (auto &s : GetListOfSysts("noflux:noxsec:det")) {
      superorder.emplace_back(s);
    }
    for (auto &s : GetListOfSysts("fakedata")) {
      superorder.emplace_back(s);
    }
    std::vector<const ISyst *> retlist;

    for (auto s : superorder) {
      if (std::find(systlist.begin(), systlist.end(), s) == systlist.end()) {
        continue;
      }
      retlist.emplace_back(s);
    }
    return retlist;
  }

}
