#pragma once

#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Weight.h"

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

// FD + ND 2D Var for ELep EHad prediction
extern ana::Var const kLepHadETrue;

// FD + ND (affected by missing proton fake data)
extern ana::Var const kProxyERec;

// FD + ND (parameterised) reco lepton and hadronic  energy
extern ana::Var const kLepEReco;
extern ana::Var const kHadEReco;
// FD + ND separate p, pi+/- and pi0 energy
extern ana::Var const kPEReco; // proton
extern ana::Var const kPipmEReco; // charged pion
extern ana::Var const kPi0EReco; // neutral pion
// Evis = ELep + EHad
extern ana::Var const kEVisReco;
extern ana::Var const kEVisTrue;

// CV weighting
extern ana::Weight const kCVXSecWeights; // kUnweighted
