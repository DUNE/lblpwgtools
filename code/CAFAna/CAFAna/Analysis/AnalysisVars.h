#pragma once

#include "CAFAna/Core/Var.h"

// List of vars
// -->FD
extern ana::Var const kRecoE_nue;
extern ana::Var const kRecoE_numu;
extern ana::Var const kRecoE_FromDep;
extern ana::Var const kFDNumuPid;
extern ana::Var const kFDNuePid;
extern ana::Var const kMVANUMU;

// -->ND
extern ana::Var const kRecoEnergyND;
extern ana::Var const kRecoYND;
extern ana::Var const kRecoY_FromDep;

extern ana::Var const kTrueEnergy;

// CV weighting
extern ana::Var const kGENIEWeights; // kUnweighted
