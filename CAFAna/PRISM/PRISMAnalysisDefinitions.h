#pragma once

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Var.h"

#include "fhiclcpp/ParameterSet.h"

#include <array>
#include <iostream>
#include <string>
#include <utility>

namespace PRISM {
enum class NuChan;
enum class BeamMode;
struct BeamChan;
struct MatchChan;

bool operator&(NuChan const &l, NuChan const &r);
std::ostream &operator<<(std::ostream &os, NuChan const &nc);
std::ostream &operator<<(std::ostream &os, BeamMode const &bm);
std::ostream &operator<<(std::ostream &os, BeamChan const &bm);
bool operator==(BeamChan const &l, BeamChan const &r);
bool operator<(BeamChan const &l, BeamChan const &r);
bool operator<(MatchChan const &l, MatchChan const &r);
std::ostream &operator<<(std::ostream &os, MatchChan const &mc);

enum class NuChan {
  kNumuApp = (1 << 0),
  kNumuBarApp = (1 << 1),
  kNueApp = (1 << 2),
  kNueBarApp = (1 << 3),

  kNumuIntrinsic = (1 << 4),
  kNumuBarIntrinsic = (1 << 5),
  kNueIntrinsic = (1 << 6),
  kNueBarIntrinsic = (1 << 7),

  kNumu = kNumuApp | kNumuIntrinsic,
  kNumuBar = kNumuBarApp | kNumuBarIntrinsic,
  kNue = kNueApp | kNueIntrinsic,
  kNueBar = kNueBarApp | kNueBarIntrinsic,

  kNumuNumuBar = kNumu | kNumuBar,
  kNueNueBar = kNue | kNueBar
};

enum class BeamMode {
  kNuMode_293kA = (1 << 0),
  kNuMode_280kA = (1 << 1),
  kNuMode = kNuMode_293kA | kNuMode_280kA,
  kNuBarMode_293kA = (1 << 2),
  kNuBarMode_280kA = (1 << 3),
  kNuBarMode = kNuBarMode_293kA | kNuBarMode_280kA
};

struct BeamChan {
  BeamMode mode;
  NuChan chan;
};

static BeamChan const kNumu_Numode = {BeamMode::kNuMode,
                                      NuChan::kNumuIntrinsic};
static BeamChan const kNumuBar_NuBarmode = {BeamMode::kNuBarMode,
                                            NuChan::kNumuBarIntrinsic};
static BeamChan const kNue_Numode = {BeamMode::kNuMode, NuChan::kNueApp};
static BeamChan const kNue_I_Numode = {BeamMode::kNuMode, NuChan::kNueIntrinsic};
static BeamChan const kNue_I_NuBarmode = {BeamMode::kNuBarMode, NuChan::kNueBarIntrinsic};
static BeamChan const kNueBar_NuBarmode = {BeamMode::kNuBarMode,
                                           NuChan::kNueBarApp};

inline BeamChan GetBeamChan(std::string const &descript, bool IsND) {

  if (IsND) {
    if (descript == "numu_numode") {
      return kNumu_Numode;
    } else if (descript == "numubar_nubarmode") {
      return kNumuBar_NuBarmode;
    } else if (descript == "nue_numode") {
      return kNue_I_Numode;
    } else if (descript == "nuebar_nubarmode") {
      return kNueBar_NuBarmode;
    } else {
      std::cout << "[ERROR]: Invalid ND beam-chan, currently support "
                   "\"numu_numode\" or \"numubar_nubarmode\"."
                << std::endl;
      abort();
    }
  } else {
    if (descript == "numu_numode") {
      return kNumu_Numode;
    } else if (descript == "numubar_nubarmode") {
      return kNumuBar_NuBarmode;
    } else if (descript == "nue_numode") {
      return kNue_Numode;
    } else if (descript == "nuebar_nubarmode") {
      return kNueBar_NuBarmode;
    } else {
      std::cout << "[ERROR]: Invalid ND beam-chan, currently support "
                   "\"numu_numode\", \"numubar_nubarmode\", \"nue_numode\", "
                   "\"nuebar_nubarmode\"."
                << std::endl;
      abort();
    }
  }
}

struct MatchChan {
  BeamChan from;
  BeamChan to;
};

static MatchChan const kNumuDisappearance_Numode = {kNumu_Numode, kNumu_Numode};
static MatchChan const kNumuBarDisappearance_NuBarmode = {kNumuBar_NuBarmode,
                                                          kNumuBar_NuBarmode};
static MatchChan const kNDNumutoNDNue_Numode = {kNumu_Numode, kNue_I_Numode};
static MatchChan const kNDNumutoNDNue_NuBarmode = {kNumuBar_NuBarmode, kNue_I_NuBarmode};
static MatchChan const kNueAppearance_Numode = {kNumu_Numode, kNue_Numode};
static MatchChan const kNueBarAppearance_NuBarmode = {kNumuBar_NuBarmode,
                                                      kNueBar_NuBarmode};

inline std::string GetMatchChanShortName(MatchChan ch) {
  if (ch.to.mode == BeamMode::kNuMode) {
    return (ch.to.chan & NuChan::kNumu) ? "NumuDisp" : "NueApp";// "NueI";
  } else {
    return (ch.to.chan & NuChan::kNumuBar) ? "NumuBarDisp" : "NueBarApp";// "NueBarI";
  }
  return "UnknownChannel";
}

inline MatchChan GetMatchChan(fhicl::ParameterSet const &ps) {
  return {GetBeamChan(ps.get<std::string>("ND"), true),
          GetBeamChan(ps.get<std::string>("ND"), true)}; //eran fix temp
}

// Enum-like list of Ids for use in lists of PRISM objects //eran change whole list!
size_t const kND_nu_numu = 0; //was kND_nu = 0 //then was kND_nu_numu 
size_t const kND_293kA_nu_numu = 0; // was kND_293kA_nu = 0 // then was kND_293kA_numu . The specific des of 293kA was shorthand for numu, avoid for nue
size_t const kND_280kA_nu_numu = 1; // was kND_280kA_nu = 1 // then was kND_280kA_numu
size_t const kND_nu_nue = 2; // new //293kA // then was kND_nu_nue
// The pairs of nonswap/numu and nueswap/nue are equivalent in terms of index
// position but not semantically identical. e.g. To make a full nue selection
// prediction you need the non swap (intrinsic) and nueswap (appeared files).
// However, both semantic meanings will not be used in the same list of objects
// and so can be safetly mapped onto the same indices.
size_t const kFD_nu_nonswap = 3; //was 2
size_t const kFD_nu_numu = 3; //was 2
size_t const kFD_nu_nueswap = 4; //was 3
size_t const kFD_nu_nue = 4; //was 3
size_t const kND_nub_numu = 5;// was kND_nub = 4; //then was kND_nub_numu
size_t const kND_293kA_nub_numu = 5; // was kND_293kA_numub = 4; //then was kND_293kA_numub = 5;
size_t const kND_280kA_nub_numu = 6; // was kND_280kA_numub = 5; //changed back to kND_280kA_numub = 7;//was kND_280kA_numub = 7;
size_t const kND_nub_nue = 7; //new eran // then was kND_nub_nue //was 6 first eran
size_t const kFD_nub_nonswap = 8; //was 6
size_t const kFD_nub_numu = 8; //was 6
size_t const kFD_nub_nueswap = 9; //was 7
size_t const kFD_nub_nue = 9; //was 7

size_t const kNPRISMConfigs_nu = 5; //was 4, was same as kND_numub//then first eran change was still 4
size_t const kNPRISMConfigs = 10; //was 8
size_t const kNPRISMNDConfigs_nu = 3;
size_t const kNPRISMNDConfigs_numu = 2; //was 2 //eran changed //but isn't it also 1? 
size_t const kNPRISMNDConfigs_nue = 1; //eran added
size_t const kNPRISMFDConfigs_nu = 2; //was 2 
size_t const kNPRISMFDConfigs = 4; //was 4

inline bool IsNuConfig(size_t conf) { return conf < kNPRISMConfigs_nu; }

inline bool IsNDConfig(size_t conf) {
  return ((conf % kNPRISMConfigs_nu) < kNPRISMNDConfigs_nu);//eran
}

inline bool IsNDNueConfig(size_t conf){  //eran
  return ((conf % kNPRISMConfigs_nu) == 2); //was different, e
}

inline bool IsND293kA_numu_Config(size_t conf) { //was IsND293kA_numu_Config
  return ((conf % kNPRISMConfigs_nu) == 0);
}

inline bool IsND280kA_numu_Config(size_t conf) { //was IsND280kA_numu_Config
  return ((conf % kNPRISMConfigs_nu) == 1);
}

inline size_t GetFDConfig(size_t conf) {
  if (IsNDConfig(conf)) {
    std::cout << "[ERROR]: Tried to get FD sub-config from an ND config("
              << conf << ")." << std::endl;
    abort();
  }
  return (((conf % kNPRISMConfigs_nu) - kNPRISMNDConfigs_nu) + //eran config name
          (!IsNuConfig(conf) * kNPRISMFDConfigs_nu));
}

inline bool IsNuFDConfig(size_t conf) { return conf < kNPRISMFDConfigs_nu; }

inline size_t GetConfigFromFD(size_t conf) {
  if (conf >= kNPRISMFDConfigs) {
    std::cout << "[ERROR]: Tried to get config from an FD config(" << conf
              << ")." << std::endl;
    abort();
  }
  return (((conf % kNPRISMFDConfigs_nu) + kNPRISMNDConfigs_nu) + //eran config name
          (!IsNuFDConfig(conf) * kNPRISMConfigs_nu));
}

inline bool IsNumuConfig(size_t conf) {
  return (IsNDConfig(conf) && !IsNDNueConfig(conf)) || ((conf % kNPRISMConfigs_nu) == kFD_nu_numu);
}

inline bool IsFDNueConfig(size_t conf) {//WARNING: IsFDNueConfig (formerly IsNueConfig) is ONLY FD!! If want ND Nue, then use IsNDNueConfig!!
  return !IsNDConfig(conf) && ((conf % kNPRISMConfigs_nu) == kFD_nu_nue);
}

inline size_t GetConfigNueSwap(size_t conf) {
  if (IsNDConfig(conf)) {
    std::cout << "[ERROR]: Tried to get FD sub-config from an ND config("
              << conf << ")." << std::endl;
    abort();
  }
  if (IsFDNueConfig(conf)) {
    return conf;
  }
  // Is a numu config
  return conf + 1;
}

inline size_t GetConfigNonSwap(size_t conf) {
  if (IsNDConfig(conf)) {
    std::cout << "[ERROR]: Tried to get FD sub-config from an ND config("
              << conf << ")." << std::endl;
    abort();
  }
  if (IsFDNueConfig(conf)) {
    return conf - 1;
  }
  // Is a numu config
  return conf;
}

inline std::string DescribeConfig(size_t conf) {
  switch (conf) {
  case kND_293kA_nu_numu: {
    return "ND_293kA_nu";
  }
  case kND_280kA_nu_numu: {
    return "ND_280kA_nu";
  }
  case kFD_nu_nonswap: {
    return "FD_nu_numu";
  }
  case kFD_nu_nueswap: {
    return "FD_nu_nue";
  }
  case kND_293kA_nub_numu: {//eran
    return "ND_293kA_nub";
  }
  case kND_280kA_nub_numu: {//eran
    return "ND_280kA_nub";
  }
  case kFD_nub_nonswap: {
    return "FD_nub_numu";
  }
  case kFD_nub_nueswap: {
    return "FD_nub_nue";
  }
  }
  std::cout << "Invalid conf: " << conf << std::endl;
  abort();
}

inline std::string DescribeFDConfig(size_t conf) {
  switch (GetConfigFromFD(conf)) {
  case kFD_nu_nonswap: {
    return "FD_nu_numu";
  }
  case kFD_nu_nueswap: {
    return "FD_nu_nue";
  }
  case kFD_nub_nonswap: {
    return "FD_nub_numu";
  }
  case kFD_nub_nueswap: {
    return "FD_nub_nue";
  }
  }
  std::cout << "Invalid FD conf: " << conf
            << " = (Determined FD conf: " << GetConfigFromFD(conf) << ")"
            << std::endl;
  abort();
}

inline size_t GetConfigFromNuChan(BeamChan nc, bool IsND) {
  if (IsND) {

    switch (nc.mode) {
    case BeamMode::kNuMode: { //eran implemented nested switch
         switch (nc.chan) {
         case NuChan::kNumu: {
           return kND_nu_numu;
         }
         case NuChan::kNue: {
           return kND_nu_nue;
         }
         default: {
         std::cout << "[ERROR]: Invalid beam mode and channel: eran error " << nc.mode << ", "
         << nc.chan << std::endl;
         abort();
         }
         }
    }
    case BeamMode::kNuBarMode: { //eran implemented nested switch
         switch (nc.chan) {
         case NuChan::kNumuBar: {
           return kND_nub_numu;
         }
         case NuChan::kNueBar: {
           return kND_nub_nue;
         }
         default: {
         std::cout << "[ERROR]: Invalid beam mode and channel: eran error " << nc.mode << ", "
         << nc.chan << std::endl;
         abort();
         }
         }
    }
    case BeamMode::kNuMode_293kA: {
      return kND_293kA_nu_numu;
    }
    case BeamMode::kNuMode_280kA: {
      return kND_280kA_nu_numu;
    }
    case BeamMode::kNuBarMode_293kA: {
      return kND_293kA_nub_numu;
    }
    case BeamMode::kNuBarMode_280kA: {
      return kND_280kA_nub_numu;
    }
    default: {
      std::cout << "[ERROR]: Explicit Horn Current Beam Mode required when "
                   "determining ND config."
                << std::endl;
      abort();
    }
    }
  } else {
    if ((nc.mode == BeamMode::kNuMode) && (nc.chan & NuChan::kNumu)) {
      return kFD_nu_numu;
    } else if ((nc.mode == BeamMode::kNuBarMode) &&
               (nc.chan & NuChan::kNumuBar)) {
      return kFD_nub_numu;
    } else if ((nc.mode == BeamMode::kNuMode) && (nc.chan & NuChan::kNue)) {
      return kFD_nu_nue;
    } else if ((nc.mode == BeamMode::kNuBarMode) &&
               (nc.chan & NuChan::kNueBar)) {
      return kFD_nub_nue;
    } else {
      std::cout << "[ERROR]: Invalid beam mode and channel: " << nc.mode << ", "
                << nc.chan << std::endl;
      abort();
    }
  }
}

inline size_t GetFDConfigFromNuChan(BeamChan nc) {
  return GetFDConfig(GetConfigFromNuChan(nc, false));
}

inline int FluxSpeciesPDG(NuChan fps) {
  if (fps & NuChan::kNumu) {
    return 14;
  } else if (fps & NuChan::kNue) {
    return 12;
  } else if (fps & NuChan::kNumuBar) {
    return -14;
  } else if (fps & NuChan::kNueBar) {
    return -12;
  }
  std::cout << "[ERROR]: Invalid NuChan: " << fps << std::endl;
  abort();
}

#define LOUDASSERT(a)                                                          \
  {                                                                            \
    if (!(a)) {                                                                \
      std::cout << "ASSERT FAILED: " << #a << std::endl;                       \
    }                                                                          \
    assert(a);                                                                 \
  }

inline void TestConfigDefinitions() {
  LOUDASSERT(IsNuConfig(kND_293kA_nu_numu));
  LOUDASSERT(IsNDConfig(kND_293kA_nu_numu));
  LOUDASSERT(!IsNDNueConfig(kND_293kA_nu_numu));
  LOUDASSERT(IsND293kA_numu_Config(kND_293kA_nu_numu));
  LOUDASSERT(!IsND280kA_numu_Config(kND_293kA_nu_numu));
  LOUDASSERT(IsNumuConfig(kND_293kA_nu_numu));
  LOUDASSERT(!IsFDNueConfig(kND_293kA_nu_numu));

  LOUDASSERT(IsNuConfig(kND_280kA_nu_numu));
  LOUDASSERT(IsNDConfig(kND_280kA_nu_numu));
  LOUDASSERT(!IsNDNueConfig(kND_280kA_nu_numu));
  LOUDASSERT(!IsND293kA_numu_Config(kND_280kA_nu_numu));
  LOUDASSERT(IsND280kA_numu_Config(kND_280kA_nu_numu));
  LOUDASSERT(IsNumuConfig(kND_280kA_nu_numu));
  LOUDASSERT(!IsFDNueConfig(kND_280kA_nu_numu));

  LOUDASSERT(!IsNuConfig(kND_293kA_nub_numu));
  LOUDASSERT(IsNDConfig(kND_293kA_nub_numu));
  LOUDASSERT(!IsNDNueConfig(kND_293kA_nub_numu));
  LOUDASSERT(IsND293kA_numu_Config(kND_293kA_nub_numu));
  LOUDASSERT(!IsND280kA_numu_Config(kND_293kA_nub_numu));
  LOUDASSERT(IsNumuConfig(kND_293kA_nub_numu));
  LOUDASSERT(!IsFDNueConfig(kND_293kA_nub_numu));

  LOUDASSERT(!IsNuConfig(kND_280kA_nub_numu));
  LOUDASSERT(IsNDConfig(kND_280kA_nub_numu));
  LOUDASSERT(!IsNDNueConfig(kND_280kA_nub_numu));
  LOUDASSERT(!IsND293kA_numu_Config(kND_280kA_nub_numu));
  LOUDASSERT(IsND280kA_numu_Config(kND_280kA_nub_numu));
  LOUDASSERT(IsNumuConfig(kND_280kA_nub_numu));
  LOUDASSERT(!IsFDNueConfig(kND_280kA_nub_numu));

//eran
  LOUDASSERT(IsNuConfig(kND_nu_nue));
  LOUDASSERT(IsNDConfig(kND_nu_nue));
  LOUDASSERT(IsNDNueConfig(kND_nu_nue));
  LOUDASSERT(!IsND293kA_numu_Config(kND_nu_nue));
  LOUDASSERT(!IsND280kA_numu_Config(kND_nu_nue));
  LOUDASSERT(!IsNumuConfig(kND_nu_nue));
  LOUDASSERT(!IsFDNueConfig(kND_nu_nue));

  LOUDASSERT(!IsNuConfig(kND_nub_nue));
  LOUDASSERT(IsNDConfig(kND_nub_nue));
  LOUDASSERT(IsNDNueConfig(kND_nub_nue));
  LOUDASSERT(!IsND293kA_numu_Config(kND_nub_nue));
  LOUDASSERT(!IsND280kA_numu_Config(kND_nub_nue));
  LOUDASSERT(!IsNumuConfig(kND_nub_nue));
  LOUDASSERT(!IsFDNueConfig(kND_nub_nue));
//eran

  LOUDASSERT(IsNuConfig(kFD_nu_numu));
  LOUDASSERT(!IsNDConfig(kFD_nu_numu));
  LOUDASSERT(!IsNDNueConfig(kFD_nu_numu));
  LOUDASSERT(GetFDConfig(kFD_nu_numu) == 0);
  LOUDASSERT(IsNuFDConfig(GetFDConfig(kFD_nu_numu)));
  LOUDASSERT(GetConfigFromFD(GetFDConfig(kFD_nu_numu)) == kFD_nu_numu);
  LOUDASSERT(IsNumuConfig(kFD_nu_numu));
  LOUDASSERT(!IsFDNueConfig(kFD_nu_numu));
  LOUDASSERT(GetConfigNueSwap(kFD_nu_numu) == kFD_nu_nue);
  LOUDASSERT(GetConfigNonSwap(kFD_nu_numu) == kFD_nu_numu);

  LOUDASSERT(!IsNuConfig(kFD_nub_numu));
  LOUDASSERT(!IsNDConfig(kFD_nub_numu));
  LOUDASSERT(!IsNDNueConfig(kFD_nub_numu));
  LOUDASSERT(GetFDConfig(kFD_nub_numu) == 2);
  LOUDASSERT(!IsNuFDConfig(GetFDConfig(kFD_nub_numu)));
  LOUDASSERT(GetConfigFromFD(GetFDConfig(kFD_nub_numu)) == kFD_nub_numu);
  LOUDASSERT(IsNumuConfig(kFD_nub_numu));
  LOUDASSERT(!IsFDNueConfig(kFD_nub_numu));
  LOUDASSERT(GetConfigNueSwap(kFD_nub_numu) == kFD_nub_nue);
  LOUDASSERT(GetConfigNonSwap(kFD_nub_numu) == kFD_nub_numu);

  LOUDASSERT(IsNuConfig(kFD_nu_nue));
  LOUDASSERT(!IsNDConfig(kFD_nu_nue));
  LOUDASSERT(!IsNDNueConfig(kFD_nu_nue));
  LOUDASSERT(GetFDConfig(kFD_nu_nue) == 1);
  LOUDASSERT(IsNuFDConfig(GetFDConfig(kFD_nu_nue)));
  LOUDASSERT(GetConfigFromFD(GetFDConfig(kFD_nu_nue)) == kFD_nu_nue);
  LOUDASSERT(!IsNumuConfig(kFD_nu_nue));
  LOUDASSERT(IsFDNueConfig(kFD_nu_nue));
  LOUDASSERT(GetConfigNueSwap(kFD_nu_nue) == kFD_nu_nue);
  LOUDASSERT(GetConfigNonSwap(kFD_nu_nue) == kFD_nu_numu);

  LOUDASSERT(!IsNuConfig(kFD_nub_nue));
  LOUDASSERT(!IsNDConfig(kFD_nub_nue));
  LOUDASSERT(!IsNDNueConfig(kFD_nub_nue));
  LOUDASSERT(GetFDConfig(kFD_nub_nue) == 3);
  LOUDASSERT(!IsNuFDConfig(GetFDConfig(kFD_nub_nue)));
  LOUDASSERT(GetConfigFromFD(GetFDConfig(kFD_nub_nue)) == kFD_nub_nue);
  LOUDASSERT(!IsNumuConfig(kFD_nub_nue));
  LOUDASSERT(IsFDNueConfig(kFD_nub_nue));
  LOUDASSERT(GetConfigNueSwap(kFD_nub_nue) == kFD_nub_nue);
  LOUDASSERT(GetConfigNonSwap(kFD_nub_nue) == kFD_nub_numu);
}

struct PRISMAxisBlob {
  ana::HistAxis XProjection;
  ana::HistAxis OffAxisPosition;
  ana::HistAxis OffAxis280kAPosition;
};

ana::HistAxis GetEventRateMatchAxes(std::string const &binning = "uniform");
PRISMAxisBlob GetPRISMAxes(std::string const &varname,
                           std::string const &xbinning = "default",
                           std::string const &oabinning = "default");

extern const ana::Cut kETrueLT10GeV;

extern const ana::Cut kIsOutOfTheDesert;

// Use to weight by Exposure
extern const ana::Var kRunPlanWeight;

extern const ana::Cut kCut280kARun;
extern const ana::Cut kSel280kARun;
extern const ana::Var kSpecHCRunWeight;

ana::Cut GetNDSignalCut(bool UseOnAxisSelection = true, bool isNuMode = true, bool isNuMu = true); //eran combing, isNuMu prob? 
ana::Cut GetFDSignalCut(bool UseOnAxisSelection = false, bool isNuMode = true,
                        bool isNuMu = true);

ana::Var GetNDWeight(std::string const &eweight = "", bool isNuMode = true);
ana::Var GetFDWeight(std::string const &eweight = "", bool isNuMode = true);

double Get280kAWeight_numu(double enu, bool isNu);
ana::Var GetNDSpecialRun(std::string const &SRDescriptor = "");

} // namespace PRISM
