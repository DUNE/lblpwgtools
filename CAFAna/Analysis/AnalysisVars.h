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

// FD + ND (affected by missing proton fake data)
extern ana::Var const kProxyERec;

// CV weighting
extern ana::Var const kCVXSecWeights; // kUnweighted
