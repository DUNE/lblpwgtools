#pragma once
#include <string>

namespace ana {

int GetGENIEModeFromSimbMode(int simbmode) {
  int gmode = 0;

  // simb defintion from:
  // https://nusoft.fnal.gov/larsoft/doxsvn/html/namespacesimb.html
  // kUnknownInteraction        =   -1,
  // kQE                        =    0,
  // kRes                       =    1,
  // kDIS                       =    2,
  // kCoh                       =    3,
  // kCohElastic                =    4,
  // kElectronScattering        =    5,
  // kIMDAnnihilation           =    6,
  // kInverseBetaDecay          =    7,
  // kGlashowResonance          =    8,
  // kAMNuGamma                 =    9,
  // kMEC                       =   10,
  // kDiffractive               =   11,
  // kEM                        =   12,
  // kWeakMix                   =   13

  // GENIE definition from:
  // https://github.com/GENIE-MC/Generator/blob/R-2_12_10/src/Interaction/ScatteringType.h
  // Modes list:
  // * QE: 1
  // * Single Kaon: 2
  // * DIS: 3
  // * RES: 4
  // * COH: 5
  // * Diffractive: 6
  // * Nu-e El: 7
  // * IMD: 8
  // * AMnuGamma: 9
  // * MEC: 10
  // * COHEl: 11
  // * IBD: 12
  // * GlashowRES: 13
  // * IMDAnnihalation: 14

  switch (simbmode) {
  case -1: { // Unknwon
    gmode = 0;
    break;
  }
  case 0: { // QE
    gmode = 1;
    break;
  }
  case 1: { // RES
    gmode = 4;
    break;
  }
  case 2: { // DIS
    gmode = 3;
    break;
  }
  case 3: { // COH
    gmode = 5;
    break;
  }
  case 4: { // COHEl
    gmode = 11;
    break;
  }
  case 5: { // Nu-e el.
    gmode = 7;
    break;
  }
  case 6: { // IMDAnnihalation
    gmode = 14;
    break;
  }
  case 7: { // IBD
    gmode = 12;
    break;
  }
  case 8: { // GlashowRES
    gmode = 13;
    break;
  }
  case 9: { // AMnuGamma
    gmode = 9;
    break;
  }
  case 10: { // MEC
    gmode = 10;
    break;
  }
  case 11: { // Diffractive
    gmode = 6;
    break;
  }
  default: { gmode = 0; }
  }
  return gmode;
}

std::string GENIEModeToString(int gmode) {
  switch (gmode) {
  // * QE: 1
  case 1: {
    return "QE";
  }
  // * Single Kaon: 2
  case 2: {
    return "Single Kaon";
  }
  // * DIS: 3
  case 3: {
    return "DIS";
  }
  // * RES: 4
  case 4: {
    return "RES";
  }
  // * COH: 5
  case 5: {
    return "COH";
  }
  // * Diffractive: 6
  case 6: {
    return "Diffractive";
  }
  // * Nu-e El: 7
  case 7: {
    return "Nu-e El";
  }
  // * IMD: 8
  case 8: {
    return "IMD";
  }
  // * AMnuGamma: 9
  case 9: {
    return "AMnuGamma";
  }
  // * MEC: 10
  case 10: {
    return "MEC";
  } // * COHEl: 11
  case 11: {
    return "COHEl";
  } // * IBD: 12
  case 12: {
    return "IBD";
  } // * GlashowRES: 13
  case 13: {
    return "GlashowRES";
  } // * IMDAnnihalation: 14
  case 14: {
    return "IMDAnnihalation";
  }
  default :{
    return "Unknown";
  }
  }
}

} // namespace ana
