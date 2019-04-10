#pragma once

#include <string>
#include <vector>

// Could move this to Systs/AnaSysts.h but it might end up in a loop of dependencies
// Leave this here for now. For now.

// Define a few groups of systematics:
// nofd_det, nofd_escale, nofd_muon_escale, noxsec_qe, noxsec_res, noxsec_dis,
// noxsec_fsi, noxsec_ratios

// All detector nuisance parameters
std::vector<std::string> const nd_det_list = {"RecoNCSyst", "FVNumuND"};
std::vector<std::string> const fd_det_list = {
    "EnergyScaleFD",      "UncorrFDTotSqrt",
    "UncorrFDTotInvSqrt",

    "ChargedHadUncorrFD", "UncorrFDHadSqrt",
    "UncorrFDHadInvSqrt", "ChargedHadResFD",

    "EScaleMuLArFD",      "UncorrFDMuSqrt",
    "UncorrFDMuInvSqrt",  "MuonResFD",

    "EMUncorrFD",         "UncorrFDEMSqrt",
    "UncorrFDEMInvSqrt",  "EMResFD",

    "NUncorrFD",          "UncorrFDNSqrt",
    "UncorrFDNInvSqrt",   "NResFD",

    "FDRecoNumuSyst",     "FDRecoNueSyst",
    "FVNumuFD",           "FVNueFD"};

// FD detector subsets
std::vector<std::string> const fd_escale_list = {
    "EnergyScaleFD",      "UncorrFDTotSqrt", "UncorrFDTotInvSqrt",
    "ChargedHadUncorrFD", "UncorrFDHadSqrt", "UncorrFDHadInvSqrt",
    "EScaleMuLArFD",      "UncorrFDMuSqrt",  "UncorrFDMuInvSqrt",
    "EMUncorrFD",         "UncorrFDEMSqrt",  "UncorrFDEMInvSqrt",
    "NUncorrFD",          "UncorrFDNSqrt",   "UncorrFDNInvSqrt",
};
std::vector<std::string> const fd_muon_escale_list = {"EScaleMuLArFD"};
std::vector<std::string> const fd_eres_list = {"ChargedHadResFD", "MuonResFD",
                                               "EMResFD", "NResFD"};
std::vector<std::string> const fd_muon_eres_list = {"MuonResFD"};
std::vector<std::string> const fd_other_det_list = {
    "FDRecoNumuSyst", "FDRecoNueSyst", "FVNumuFD", "FVNueFD"};
// ~New dials for V4
std::vector<std::string> const fd_nonlin_det_list = {
    "UncorrFDTotSqrt",    "UncorrFDTotInvSqrt", "UncorrFDHadSqrt",
    "UncorrFDHadInvSqrt", "UncorrFDMuSqrt",     "UncorrFDMuInvSqrt",
    "UncorrFDEMSqrt",     "UncorrFDEMInvSqrt",  "UncorrFDNSqrt",
    "UncorrFDNInvSqrt"};
