#pragma once

#include <string>
#include <vector>

// Define a few groups of systematics:
// nofd_det, nofd_escale, nofd_muon_escale, noxsec_qe, noxsec_res, noxsec_dis,
// noxsec_fsi, noxsec_ratios

// All detector nuisance parameters
std::vector<std::string> const fd_det_list = {"eScaleFD",
                                              "UncorrFDTotSqrtSyst",
                                              "UncorrFDTotInvSqrtSyst",
                                              "ChargedHadUncorrFD",
                                              "UncorrFDHadSqrtSyst",
                                              "UncorrFDHadInvSqrtSyst",
                                              "eScaleMuLArFD",
                                              "UncorrFDMuSqrtSyst",
                                              "UncorrFDMuInvSqrtSyst",
                                              "EMUncorrFD",
                                              "UncorrFDEMSqrtSyst",
                                              "UncorrFDEMInvSqrtSyst",
                                              "eScaleN_FD",
                                              "UncorrFDNSqrtSyst",
                                              "UncorrFDNInvSqrtSyst",
                                              "EMResFD",
                                              "MuonResFD",
                                              "ChargedHadResFD",
                                              "NResFD",
                                              "FDRecoNumuSyst",
                                              "FDRecoNueSyst",
                                              "FVNumuFD",
                                              "FVNueFD"};

// FD detector subsets
std::vector<std::string> const fd_escale_list = {
    "eScaleFD",           "UncorrFDTotSqrtSyst", "UncorrFDTotInvSqrtSyst",
    "ChargedHadUncorrFD", "UncorrFDHadSqrtSyst", "UncorrFDHadInvSqrtSyst",
    "eScaleMuLArFD",      "UncorrFDMuSqrtSyst",  "UncorrFDMuInvSqrtSyst",
    "EMUncorrFD",         "UncorrFDEMSqrtSyst",  "UncorrFDEMInvSqrtSyst",
    "eScaleN_FD",         "UncorrFDNSqrtSyst",   "UncorrFDNInvSqrtSyst"};
std::vector<std::string> const fd_muon_escale_list = {"eScaleMuLArFD"};
std::vector<std::string> const fd_eres_list = {"MuonResFD", "EMResFD",
                                               "ChargedHadResFD"};
std::vector<std::string> const fd_muon_eres_list = {"MuonResFD"};
std::vector<std::string> const fd_other_det_list = {
    "FDRecoNumuSyst", "FDRecoNueSyst", "FVNumuFD", "FVNueFD"};
// ~New dials for V4
std::vector<std::string> const fd_nonlin_det_list = {
    "UncorrFDTotSqrtSyst",    "UncorrFDTotInvSqrtSyst", "UncorrFDHadSqrtSyst",
    "UncorrFDHadInvSqrtSyst", "UncorrFDMuSqrtSyst",     "UncorrFDMuInvSqrtSyst",
    "UncorrFDEMSqrtSyst",     "UncorrFDEMInvSqrtSyst",  "UncorrFDNSqrtSyst",
    "UncorrFDNInvSqrtSyst"};

// All QE XSEC parameters
std::vector<std::string> const xsec_qe_list = {"MaCCQE",
                                               "VecFFCCQEshape",
                                               "CCQEPauliSupViaKF",
                                               "Mnv2p2hGaussEnhancement",
                                               "E2p2h_A_nu",
                                               "E2p2h_B_nu",
                                               "E2p2h_A_nubar",
                                               "E2p2h_B_nubar",
                                               "BeRPA_A",
                                               "BeRPA_B",
                                               "BeRPA_D",
                                               "C12ToAr40_2p2hScaling_nu",
                                               "C12ToAr40_2p2hScaling_nubar"};

// All RES XSEC parameters
std::vector<std::string> const xsec_res_list = {
    "MaCCRES",           "MvCCRES",           "MaNCRES",
    "MvNCRES",           "NR_nu_np_CC_1Pi",   "NR_nu_n_NC_1Pi",
    "NR_nu_p_NC_1Pi",    "NR_nubar_n_CC_1Pi", "NR_nubar_p_CC_1Pi",
    "NR_nubar_n_NC_1Pi", "NR_nubar_p_NC_1Pi", "SPPLowQ2Suppression"};

// All NR_* and AKGY
std::vector<std::string> const xsec_dis_list = {"AhtBY",
                                                "BhtBY",
                                                "CV1uBY",
                                                "CV2uBY",
                                                "NR_nu_n_CC_2Pi",
                                                "NR_nu_n_CC_3Pi",
                                                "NR_nu_p_CC_2Pi",
                                                "NR_nu_p_CC_3Pi",
                                                "NR_nu_n_NC_2Pi",
                                                "NR_nu_n_NC_3Pi",
                                                "NR_nu_p_NC_2Pi",
                                                "NR_nu_p_NC_3Pi",
                                                "NR_nubar_n_CC_2Pi",
                                                "NR_nubar_n_CC_3Pi",
                                                "NR_nubar_p_CC_2Pi",
                                                "NR_nubar_p_CC_3Pi",
                                                "NR_nubar_n_NC_2Pi",
                                                "NR_nubar_n_NC_3Pi",
                                                "NR_nubar_p_NC_2Pi",
                                                "NR_nubar_p_NC_3Pi",
                                                "Theta_Delta2Npi"};

// All nuclear effects/FSI
std::vector<std::string> const xsec_fsi_list = {
    "FrCEx_pi", "FrElas_pi", "FrInel_pi", "FrAbs_pi", "FrPiProd_pi",
    "FrCEx_N",  "FrElas_N",  "FrInel_N",  "FrAbs_N",  "FrPiProd_N"};

std::vector<std::string> const xsec_ratios_list = {"nuenuebar_xsec_ratio",
                                                   "nuenumu_xsec_ratio"};
