#include "common_fit_definitions.C"

void cpv_joint_twopoint(
    std::string stateFname = "common_state_mcc11v3.root",
    std::string outputFname = "cpv_twopoint_ndfd_nosyst.root",
    std::string systSet = "nosyst", std::string sampleString = "ndfd",
    std::string penaltyString = "nopen", std::string SetsToInclude = "",
    std::string asimov_set = "0", bool RunGlobalMin = false, bool Onlydcp0 = false) {

  gROOT->SetBatch(1);

  double dcp_test = 0;

  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to use
  std::vector<const IFitVar *> oscVars = {&kFitDmSq32Scaled, &kFitSinSqTheta23,
                                          &kFitTheta13,      &kFitSinSq2Theta12,
                                          &kFitDmSq21,       &kFitRho};

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");

  assert(fout && !fout->IsZombie());

  std::vector<double> test_hies = {1 /*, -1*/};
  std::vector<double> test_octs = {1 /*, -1*/};
  std::vector<double> test_dcps = Onlydcp0 ? std::vector<double>{0,} :std::vector<double>{0, TMath::Pi()};

  // Sense hierarchies
  for (int hie : {1 /*, -1*/}) {

    TTree *syst_chi2 = new TTree(
        (std::string("syst_chi2_") + (hie > 0 ? "NH" : "IH")).c_str(), "");

    std::string *syst_name = nullptr;
    double chi2;
    double deltachi2;

    syst_chi2->Branch("syst_name", &syst_name);
    syst_chi2->Branch("chi2", &chi2);
    syst_chi2->Branch("deltachi2", &deltachi2);

    double hmin = RunGlobalMin ? std::numeric_limits<double>::max() : 0;

    osc::IOscCalculatorAdjustable *trueOsc = NuFitOscCalc(hie, 1, asimov_set);

    if (RunGlobalMin) {
      for (int ihie : test_hies) {
        for (int ioct : test_octs) {

          std::cout << "--FIT Global min for hierarchy: " << hie
                    << " with test: {hie: " << ihie << ", oct: " << ioct << "}."
                    << std::endl;

          osc::IOscCalculatorAdjustable *testOsc =
              NuFitOscCalc(ihie, ioct, asimov_set);

          IExperiment *penalty =
              GetPenalty(ihie, ioct, penaltyString, asimov_set);
          SystShifts trueSyst = kNoShift;
          SystShifts testSyst = kNoShift;

          // Dump expected global best fit ((ihie == hie) && (ioct == 1))
          // spectra to file
          double thischisq = RunFitPoint(
              stateFname, sampleString, trueOsc, trueSyst, false, oscVars,
              systlist, testOsc, testSyst, {}, penalty, Fitter::kNormal,
              ((ihie == hie) && (ioct == 1)) ? fout : nullptr);

          if (thischisq < hmin) {
            std::cout << "--New min LL " << thischisq << " < " << hmin
                      << std::endl;
          }

          hmin = std::min(hmin, thischisq);
        }
      }
    }

    std::map<double, SystShifts> AllSystBestFits;

    double allsyst_min = std::numeric_limits<double>::max();
    for (double idcp : test_dcps) {
      for (int ihie : test_hies) {
        for (int ioct : test_octs) {

          std::cout << "--FIT all syst min for hierarchy: " << hie
                    << " with test: {hie: " << ihie << ", oct: " << ioct
                    << ", dcp = " << idcp << "}." << std::endl;

          osc::IOscCalculatorAdjustable *testOsc =
              NuFitOscCalc(ihie, ioct, asimov_set);
          testOsc->SetdCP(idcp);

          IExperiment *penalty =
              GetPenalty(ihie, ioct, penaltyString, asimov_set);
          SystShifts trueSyst = kNoShift;
          SystShifts testSyst = kNoShift;

          SystShifts best_fit_syst;

          double thischisq =
              RunFitPoint(stateFname, sampleString, trueOsc, trueSyst, false,
                          oscVars, systlist, testOsc, testSyst, {}, penalty,
                          Fitter::kNormal, nullptr, best_fit_syst);

          AllSystBestFits[ceil(idcp / TMath::Pi()) + 10 * ihie + 100 * ioct] =
              best_fit_syst;

          if (thischisq < allsyst_min) {
            std::cout << "--New min LL " << thischisq << " < " << allsyst_min
                      << ", allsyst sense = " << sqrt(allsyst_min - hmin)
                      << std::endl;
          }

          allsyst_min = std::min(allsyst_min, thischisq);
        }
      }
    }

    (*syst_name) = "allsyst";
    chi2 = allsyst_min;
    deltachi2 = allsyst_min - hmin;
    syst_chi2->Fill();

    std::map<std::string, std::vector<std::string>> syst_blocks = {
        {"Flux_systs",
         {"flux0", "flux1", "flux2", "flux3", "flux4", "flux5", "flux6",
          "flux7", "flux8", "flux9"}},

        {"RecoNCSystND", {"RecoNCSyst"}},
        {"FVNumuND", {"FVNumuND"}},

        {"FD_escale_systs",
         {
             "eScaleFD",
             "eScaleMuLArFD",
             "eScaleN_FD",
         }},
        {"EMUncorrFD", {"EMUncorrFD"}},
        {"FDPID_syst", {"FDRecoNumuSyst", "FDRecoNueSyst"}},
        {"FDFV_syst", {"FVNumuFD", "FVNueFD"}},
        {"FD_Res_systs",
         {
             "MuonResFD",
             "EMResFD",
             "ChargedHadResFD",
         }},

        {"QEFF_systs",
         {
             "MaCCQE",
             "VecFFCCQEshape",
             "CCQEPauliSupViaKF",
         }},
        {"QERPA_systs", {"BeRPA_A", "BeRPA_B", "BeRPA_D", "BeRPA_E"}},

        {"NCQEFF_systs", {"MaNCEL", "EtaNCEL"}},
        {"NCRESFF_systs", {"MaNCRES", "MvNCRES"}},

        {"SPPLowQ2Suppression",
         {
             "SPPLowQ2Suppression",
         }},

        {"MKSPP_ReWeight",
         {
             "MKSPP_ReWeight",
         }},

        {"CCRES_systs",
         {"MaCCRES", "MvCCRES", "RDecBR1gamma", "RDecBR1eta",
          "Theta_Delta2Npi"}},

        {"NR_CCPiProd_nu",
         {"NR_nu_n_CC_2Pi", "NR_nu_n_CC_3Pi", "NR_nu_p_CC_2Pi",
          "NR_nu_p_CC_3Pi", "NR_nu_np_CC_1Pi"}},

        {"NR_CCPiProd_nubar",
         {"NR_nubar_n_CC_1Pi", "NR_nubar_n_CC_2Pi", "NR_nubar_n_CC_3Pi",
          "NR_nubar_p_CC_1Pi", "NR_nubar_p_CC_2Pi", "NR_nubar_p_CC_3Pi"}},

        {"NR_NCPiProd_nu",
         {"NR_nu_n_NC_1Pi", "NR_nu_n_NC_2Pi", "NR_nu_n_NC_3Pi",
          "NR_nu_p_NC_1Pi", "NR_nu_p_NC_2Pi", "NR_nubar_p_NC_3Pi"}},

        {"NR_NCPiProd_nubar",
         {"NR_nubar_n_NC_1Pi", "NR_nubar_n_NC_2Pi", "NR_nubar_n_NC_3Pi",
          "NR_nubar_p_NC_1Pi", "NR_nubar_p_NC_2Pi", "NR_nubar_p_NC_3Pi"}},

        {"DISXSec_systs", {"AhtBY", "BhtBY", "CV1uBY", "CV2uBY"}},

        {"NucFSI_systs",
         {
             "FrCEx_N",
             "FrElas_N",
             "FrInel_N",
             "FrAbs_N",
             "FrPiProd_N",
         }},

        {"PionFSI_systs",
         {"FrCEx_pi", "FrElas_pi", "FrInel_pi", "FrAbs_pi", "FrPiProd_pi"}},

        {"FSILikeEAvailSmearing",
         {
             "FSILikeEAvailSmearing",
         }},

        {"nuenuebar_xsec_ratio", {"nuenuebar_xsec_ratio"}},
        {"nuenumu_xsec_ratio", {"nuenumu_xsec_ratio"}},

        {"2p2h_systs",
         {
             "Mnv2p2hGaussEnhancement",
             "E2p2h_A_nu",
             "E2p2h_B_nu",
             "E2p2h_A_nubar",
             "E2p2h_B_nubar",
             "C12ToAr40_2p2hScaling_nu",
             "C12ToAr40_2p2hScaling_nubar",
         }},

    };

    if (SetsToInclude.size()) {
      for (auto iter = syst_blocks.begin(); iter != syst_blocks.end();) {
        if (SetsToInclude.find(iter->first) == std::string::npos) {
          std::cout << "Removing block: " << iter->first << std::endl;
          syst_blocks.erase(iter++);
        } else {
          ++iter;
        }
      }
    }

    for (auto const &systs_to_remove : syst_blocks) {

      double test_min = std::numeric_limits<double>::max();

      std::vector<const ISyst *> red_systlist = systlist;
      RemoveSysts(red_systlist, systs_to_remove.second);
      if (red_systlist.size() == systlist.size()) {
        std::cout << "[WARN]: Failed to remove " << systs_to_remove.first
                  << std::endl;
        continue;
      }

      for (double idcp : test_dcps) {
        for (int ihie : test_hies) {
          for (int ioct : test_octs) {

            std::cout << "--FIT without " << systs_to_remove.first
                      << " min for hierarchy: " << hie
                      << " with test: {hie: " << ihie << ", oct: " << ioct
                      << ", dcp = " << idcp << "}." << std::endl;

            osc::IOscCalculatorAdjustable *testOsc =
                NuFitOscCalc(ihie, ioct, asimov_set);
            testOsc->SetdCP(idcp);

            IExperiment *penalty =
                GetPenalty(ihie, ioct, penaltyString, asimov_set);
            SystShifts trueSyst = kNoShift;
            SystShifts testSyst = kNoShift;

            // If we have it, use the best fit from the allsyst fit to seed
            // other param values, should significantly speed up fits for
            // removed blocks that make little difference
            if (AllSystBestFits.find(ceil(idcp / TMath::Pi()) + 10 * ihie +
                                     100 * ioct) != AllSystBestFits.end()) {
              testSyst = AllSystBestFits[ceil(idcp / TMath::Pi()) + 10 * ihie +
                                         100 * ioct];
            }

            double thischisq = RunFitPoint(
                stateFname, sampleString, trueOsc, trueSyst, false, oscVars,
                red_systlist, testOsc, testSyst, {}, penalty);

            if (thischisq < test_min) {
              std::cout << "--New min LL " << thischisq << " < " << test_min
                        << ", sense sigma = " << sqrt(test_min - hmin)
                        << ", allsyst sense = " << sqrt(allsyst_min - hmin)
                        << std::endl;
            }

            test_min = std::min(test_min, thischisq);
          }
        }
      }

      (*syst_name) = systs_to_remove.first;
      chi2 = test_min;
      deltachi2 = test_min - hmin;

      syst_chi2->Fill();
    }

    syst_chi2->ResetBranchAddresses();
    fout->cd();
    syst_chi2->SetDirectory(fout);
    syst_chi2->Write();
  }
	fout->Write();
  fout->Close();
}
