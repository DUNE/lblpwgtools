

#ifndef TOPOLOGYENUMS
#define TOPOLOGYENUMS

#include <string>
#include <iostream>

// Note: kCosmic, kMixed, and kOutFV will never be set by GetTopology.cxx
// They will be kUnknown and you must check for them yourself
enum NuIntTopology{
  kCC0pi0p,
  kCC0pi1p,
  kCC0piNp,
  kCC1piplus0p,
  kCC1piplus1p,
  kCC1piplusNp,
  kCC1piminus0p,
  kCC1piminus1p,
  kCC1piminusNp,
  kCC1pizero0p,
  kCC1pizero1p,
  kCC1pizeroNp,
  kCCmultipi0p,
  kCCmultipi1p,
  kCCmultipiNp,
  kCCother,
  kCCNue,
  kNC,
  kCosmic,
  kMixed,
  kOutFV,
  kUnknown,
};

inline std::string topologyenum2str(NuIntTopology topology)
{
  std::string returnString = "";

  switch(topology){
  case kCC0pi0p:
    returnString = "#nu_{#mu} CC 0#pi 0p";
    break;
  case kCC0pi1p:
    returnString = "#nu_{#mu} CC 0#pi 1p";
    break;
  case kCC0piNp:
    returnString = "#nu_{#mu} CC 0#pi Np (N>1)";
    break;
  case kCC1piplus0p:
    returnString = "#nu_{#mu} CC 1#pi^{+} 0p";
    break;
  case kCC1piplus1p:
    returnString = "#nu_{#mu} CC 1#pi^{+} 1p";
    break;
  case kCC1piplusNp:
    returnString = "#nu_{#mu} CC 1#pi^{+} Np (N>1)";
    break;
  case kCC1piminus0p:
    returnString = "#nu_{#mu} CC 1#pi^{-} 0p";
    break;
  case kCC1piminus1p:
    returnString = "#nu_{#mu} CC 1#pi^{-} 1p";
    break;
  case kCC1piminusNp:
    returnString = "#nu_{#mu} CC 1#pi^{-} Np (N>1)";
    break;
  case kCC1pizero0p:
    returnString = "#nu_{#mu} CC 1pi^{0} 0p";
    break;
  case kCC1pizero1p:
    returnString = "#nu_{#mu} CC 1pi^{0} 1p";
    break;
  case kCC1pizeroNp:
    returnString = "#nu_{#mu} CC 1pi^{0} Np (N>1)";
    break;
  case kCCmultipi0p:
    returnString = "#nu_{#mu} CC multi#pi 0p";
    break;
  case kCCmultipi1p:
    returnString = "#nu_{#mu} CC multi#pi 1p";
    break;
  case kCCmultipiNp:
    returnString = "#nu_{#mu} CC multi#pi Np (N>1)";
    break;
  case kCCother:
    returnString = "#nu_{#mu} CC other";
    break;
  case kCCNue:
    returnString = "#nu_{e} CC";
    break;
  case kNC:
    returnString = "NC";
    break;
  case kCosmic:
    returnString = "Cosmic";
    break;
  case kMixed:
    returnString = "Mixed";
    break;
  case kOutFV:
    returnString = "Out of FV";
    break;
  case kUnknown:
    returnString = "Unknown";
    break;
  default:
    std::cout << "[ERROR: TopologyEnums.h] Could not find string conversion for " << topology << std::endl;
    returnString = "Unknown";
    break;
  }

  return returnString;
}


inline std::string topologyenum2str_coarse(NuIntTopology topology)
{
  std::string returnString = "";

  switch(topology){
  case kCC0pi0p:
  case kCC0pi1p:
  case kCC0piNp:
    returnString = "#nu_{#mu} CC 0#pi";
    break;
  case kCC1piplus0p:
  case kCC1piplus1p:
  case kCC1piplusNp:
  case kCC1piminus0p:
  case kCC1piminus1p:
  case kCC1piminusNp:
    returnString = "#nu_{#mu} CC 1#pi^{#pm}";
    break;
  case kCC1pizero0p:
  case kCC1pizero1p:
  case kCC1pizeroNp:
    returnString = "#nu_{#mu} CC 1#pi^{0}";
    break;
  case kCCmultipi0p:
  case kCCmultipi1p:
  case kCCmultipiNp:
    returnString = "#nu_{#mu} CC >1#pi";
    break;
  case kCCother:
    returnString = "#nu_{#mu} CC other";
    break;
  case kCCNue:
    returnString = "#nu_{e} CC";
    break;
  case kNC:
    returnString = "NC";
    break;
  case kCosmic:
    returnString = "Cosmic";
    break;
  case kMixed:
    returnString = "Mixed";
    break;
  case kOutFV:
    returnString = "Out of FV";
    break;
  case kUnknown:
    returnString = "Unknown";
    break;
  default:
    std::cout << "[ERROR: TopologyEnums.h] Could not find coarse string conversion for " << topology << std::endl;
    returnString = "Unknown";
    break;
  }

  return returnString;
}

#endif
