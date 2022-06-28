#pragma once

#include "CAFAna/Core/Var.h"

#include "CAFAna/Prediction/PredictionInterp.h"

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
static BeamChan const kNue_I_Numode = {BeamMode::kNuMode,
                                       NuChan::kNueIntrinsic};
static BeamChan const kNueBar_I_NuBarmode = {BeamMode::kNuBarMode,
                                             NuChan::kNueBarIntrinsic};
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
static MatchChan const kNDNumuBartoNDNueBar_NuBarmode = {kNumuBar_NuBarmode,
                                                         kNueBar_I_NuBarmode};
static MatchChan const kNueAppearance_Numode = {kNumu_Numode, kNue_Numode};
static MatchChan const kNueBarAppearance_NuBarmode = {kNumuBar_NuBarmode,
                                                      kNueBar_NuBarmode};

inline std::string GetMatchChanShortName(MatchChan ch) {
  if (ch.to.mode == BeamMode::kNuMode) {
    return (ch.to.chan & NuChan::kNumu)
               ? "NumuDisp"
               : "NueApp"; // "NueI"; //eran, never decided if needed/how
  } else {
    return (ch.to.chan & NuChan::kNumuBar) ? "NumuBarDisp"
                                           : "NueBarApp"; // "NueBarI";
  }
  return "UnknownChannel";
}

inline MatchChan GetMatchChan(fhicl::ParameterSet const &ps) {
  return {GetBeamChan(ps.get<std::string>("ND"), true),
          GetBeamChan(ps.get<std::string>("FD"),
                      false)}; // was true now false//eran fix temp //back to FD
}

// Enum-like list of Ids for use in lists of PRISM objects
// The pairs of nonswap/numu and nueswap/nue are equivalent in terms of index
// position but not semantically identical. e.g. To make a full nue selection
// prediction you need the non swap (intrinsic) and nueswap (appeared files).
// However, both semantic meanings will not be used in the same list of objects
// and so can be safetly mapped onto the same indices. Indices for the tauswap
// files have been added.
size_t const kND_nu_numu = 0;
size_t const kND_293kA_nu_numu = 0;
size_t const kND_280kA_nu_numu = 1;
size_t const kND_293kA_nu_nue = 2;

size_t const kFD_nu_nonswap = 3;
size_t const kFD_nu_numu = 3;
size_t const kFD_nu_nueswap = 4;
size_t const kFD_nu_nue = 4;
size_t const kFD_nu_tauswap = 5;
size_t const kFD_nu_nutau = 5;

size_t const kND_nub_numub = 6;
size_t const kND_293kA_nub_numub = 6;
size_t const kND_280kA_nub_numub = 7;
size_t const kND_293kA_nub_nueb = 8;

size_t const kFD_nub_nonswap = 9;
size_t const kFD_nub_numub = 9;
size_t const kFD_nub_nueswap = 10;
size_t const kFD_nub_nueb = 10;
size_t const kFD_nub_tauswap = 11;
size_t const kFD_nub_nutaub = 11;

size_t const kNPRISMConfigs_nu = 6;
size_t const kNPRISMConfigs = 12;
size_t const kNPRISMNDConfigs_nu = 3;
size_t const kNPRISMFDConfigs_nu = 3;
size_t const kNPRISMFDConfigs = 6;

inline bool IsNuConfig(size_t conf) { return conf < kNPRISMConfigs_nu; }

inline bool IsNDConfig(size_t conf) {
  return ((conf % kNPRISMConfigs_nu) < kNPRISMNDConfigs_nu);
}

inline bool IsND293kA_numu_Config(size_t conf) {
  return ((conf % kNPRISMConfigs_nu) == 0);
}

inline bool IsND280kA_numu_Config(size_t conf) {
  return ((conf % kNPRISMConfigs_nu) == 1);
}

inline bool IsNDNueConfig(size_t conf) {
  return ((conf % kNPRISMConfigs_nu) == 2);
}

inline size_t GetFDConfig(size_t conf) { // conf = 2, 3, 6 or 7
  if (IsNDConfig(conf)) {
    std::cout << "[ERROR]: Tried to get FD sub-config from an ND config("
              << conf << ")." << std::endl;
    abort();
  }
  // 2->0 ; 3->1 ; 6->2 ; 7->3
  return (((conf % kNPRISMConfigs_nu) - kNPRISMNDConfigs_nu) +
          (!IsNuConfig(conf) * kNPRISMFDConfigs_nu));
}

inline bool IsNuFDConfig(size_t conf) { return conf < kNPRISMFDConfigs_nu; }

inline size_t GetConfigFromFD(size_t conf) {
  if (conf >= kNPRISMFDConfigs) {
    std::cout << "[ERROR]: Tried to get config from an FD config(" << conf
              << ")." << std::endl;
    abort();
  }
  return (
      ((conf % kNPRISMFDConfigs_nu) + kNPRISMNDConfigs_nu) + // eran config name
      (!IsNuFDConfig(conf) * kNPRISMConfigs_nu));
}

inline bool IsNumuConfig(size_t conf) {
  return (IsNDConfig(conf) && !IsNDNueConfig(conf)) ||
         ((conf % kNPRISMConfigs_nu) == kFD_nu_numu);
}

inline bool IsFDNueConfig(size_t conf) {
  return !IsNDConfig(conf) && ((conf % kNPRISMConfigs_nu) == kFD_nu_nue);
}

inline bool IsNutauConfig(size_t conf) {
  return !IsNDConfig(conf) && ((conf % kNPRISMConfigs_nu) == kFD_nu_nutau);
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
  if (IsNutauConfig(conf)) {
    return conf - 1;
  }
  // Is a numu config
  return conf + 1;
}

inline size_t GetND280kAConfig(size_t conf) {
  if (!IsNDConfig(conf)) {
    std::cout << "[ERROR]: Tried to get ND280kA config from an FD config("
              << conf << ")." << std::endl;
    abort();
  }
  if (IsND293kA_numu_Config(conf)) {
    return conf + 1;
  }
  return conf;
}

inline size_t GetND293kAConfig(size_t conf) {
  if (!IsNDConfig(conf)) {
    std::cout << "[ERROR]: Tried to get ND280kA config from an FD config("
              << conf << ")." << std::endl;
    abort();
  }
  if (IsND280kA_numu_Config(conf)) {
    return conf - 1;
  }
  return conf;
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
  if (IsNutauConfig(conf)) {
    return conf - 2;
  }
  // Is a numu config
  return conf;
}

inline std::string DescribeConfig(size_t conf) {
  switch (conf) {
  case kND_293kA_nu_numu: {
    return "ND_293kA_nu_numu";
  }
  case kND_280kA_nu_numu: {
    return "ND_280kA_nu_numu";
  }
  case kND_293kA_nu_nue: {
    return "ND_293kA_nu_nue";
  }
  case kFD_nu_numu: {
    return "FD_nu_numu";
  }
  case kFD_nu_nue: {
    return "FD_nu_nue";
  }
  case kFD_nu_nutau: {
    return "FD_nu_nutau";
  }
  case kND_293kA_nub_numub: {
    return "ND_293kA_nub_numub";
  }
  case kND_280kA_nub_numub: {
    return "ND_280kA_nub_numub";
  }
  case kND_293kA_nub_nueb: {
    return "ND_293kA_nub_nueb";
  }
  case kFD_nub_numub: {
    return "FD_nub_numub";
  }
  case kFD_nub_nueb: {
    return "FD_nub_nueb";
  }
  case kFD_nub_nutaub: {
    return "FD_nub_nutaub";
  }
  }
  std::cout << "Invalid conf: " << conf << std::endl;
  abort();
}

inline std::string DescribeFDConfig(size_t conf) {
  switch (GetConfigFromFD(conf)) {
  case kFD_nu_numu: {
    return "FD_nu_numu";
  }
  case kFD_nu_nue: {
    return "FD_nu_nue";
  }
  case kFD_nu_nutau: {
    return "FD_nu_nutau";
  }
  case kFD_nub_numub: {
    return "FD_nub_numub";
  }
  case kFD_nub_nueb: {
    return "FD_nub_nueb";
  }
  case kFD_nub_nutaub: {
    return "FD_nub_nutaub";
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
    case BeamMode::kNuMode: {
      switch (nc.chan) {
      case NuChan::kNumuIntrinsic: {
        return kND_nu_numu;
      }
      case NuChan::kNueIntrinsic: {
        return kND_293kA_nu_nue;
      }
      default: {
        std::cout << "[ERROR]: Invalid beam mode and channel: " << nc.mode
                  << ", " << nc.chan << std::endl;
        abort();
      }
      }
    }
    case BeamMode::kNuBarMode: {
      switch (nc.chan) {
      case NuChan::kNumuBarIntrinsic: {
        return kND_nub_numub;
      }
      case NuChan::kNueBarIntrinsic: {
        return kND_293kA_nub_nueb;
      }
      default: {
        std::cout << "[ERROR]: Invalid beam mode and channel: " << nc.mode
                  << ", " << nc.chan << std::endl;
        abort();
      }
      }
    }
    case BeamMode::kNuMode_293kA: {
      if (nc.chan == NuChan::kNumuIntrinsic) {
        return kND_293kA_nu_numu;
      } else {
        std::cout << "[ERROR]: Invalid beam mode and channel: " << nc.mode
                  << ", " << nc.chan << std::endl;
        abort();
      }
    }
    case BeamMode::kNuMode_280kA: {
      if (nc.chan == NuChan::kNumuIntrinsic) {
        return kND_280kA_nu_numu;
      } else {
        std::cout << "[ERROR]: Invalid beam mode and channel: " << nc.mode
                  << ", " << nc.chan << std::endl;
        abort();
      }
    }
    case BeamMode::kNuBarMode_293kA: {
      if (nc.chan == NuChan::kNumuBarIntrinsic) {
        return kND_293kA_nub_numub;
      } else {
        std::cout << "[ERROR]: Invalid beam mode and channel: " << nc.mode
                  << ", " << nc.chan << std::endl;
        abort();
      }
    }
    case BeamMode::kNuBarMode_280kA: {
      if (nc.chan == NuChan::kNumuBarIntrinsic) {
        return kND_280kA_nub_numub;
      } else {
        std::cout << "[ERROR]: Invalid beam mode and channel: " << nc.mode
                  << ", " << nc.chan << std::endl;
        abort();
      }
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
      return kFD_nub_numub;
    } else if ((nc.mode == BeamMode::kNuMode) && (nc.chan & NuChan::kNue)) {
      return kFD_nu_nue;
    } else if ((nc.mode == BeamMode::kNuBarMode) &&
               (nc.chan & NuChan::kNueBar)) {
      return kFD_nub_nueb;
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
  LOUDASSERT(IsND293kA_numu_Config(kND_293kA_nu_numu));
  LOUDASSERT(!IsND280kA_numu_Config(kND_293kA_nu_numu));
  LOUDASSERT(GetND293kAConfig(kND_293kA_nu_numu) == kND_293kA_nu_numu);
  LOUDASSERT(GetND280kAConfig(kND_293kA_nu_numu) == kND_280kA_nu_numu);
  LOUDASSERT(IsNumuConfig(kND_293kA_nu_numu));
  LOUDASSERT(!IsNDNueConfig(kND_293kA_nu_numu));

  LOUDASSERT(IsNuConfig(kND_280kA_nu_numu));
  LOUDASSERT(IsNDConfig(kND_280kA_nu_numu));
  LOUDASSERT(!IsND293kA_numu_Config(kND_280kA_nu_numu));
  LOUDASSERT(IsND280kA_numu_Config(kND_280kA_nu_numu));
  LOUDASSERT(GetND293kAConfig(kND_280kA_nu_numu) == kND_293kA_nu_numu);
  LOUDASSERT(GetND280kAConfig(kND_280kA_nu_numu) == kND_280kA_nu_numu);
  LOUDASSERT(IsNumuConfig(kND_280kA_nu_numu));
  LOUDASSERT(!IsNDNueConfig(kND_280kA_nu_numu));

  LOUDASSERT(!IsNuConfig(kND_293kA_nub_numub));
  LOUDASSERT(IsNDConfig(kND_293kA_nub_numub));
  LOUDASSERT(IsND293kA_numu_Config(kND_293kA_nub_numub));
  LOUDASSERT(!IsND280kA_numu_Config(kND_293kA_nub_numub));
  LOUDASSERT(GetND293kAConfig(kND_293kA_nub_numub) == kND_293kA_nub_numub);
  LOUDASSERT(GetND280kAConfig(kND_293kA_nub_numub) == kND_280kA_nub_numub);
  LOUDASSERT(IsNumuConfig(kND_293kA_nub_numub));
  LOUDASSERT(!IsNDNueConfig(kND_293kA_nub_numub));

  LOUDASSERT(!IsNuConfig(kND_280kA_nub_numub));
  LOUDASSERT(IsNDConfig(kND_280kA_nub_numub));
  LOUDASSERT(!IsND293kA_numu_Config(kND_280kA_nub_numub));
  LOUDASSERT(IsND280kA_numu_Config(kND_280kA_nub_numub));
  LOUDASSERT(GetND293kAConfig(kND_280kA_nub_numub) == kND_293kA_nub_numub);
  LOUDASSERT(GetND280kAConfig(kND_280kA_nub_numub) == kND_280kA_nub_numub);
  LOUDASSERT(IsNumuConfig(kND_280kA_nub_numub));
  LOUDASSERT(!IsNDNueConfig(kND_280kA_nub_numub));

  LOUDASSERT(IsNuConfig(kND_293kA_nu_nue));
  LOUDASSERT(IsNDConfig(kND_293kA_nu_nue));
  LOUDASSERT(IsNDNueConfig(kND_293kA_nu_nue));
  LOUDASSERT(!IsND293kA_numu_Config(kND_293kA_nu_nue));
  LOUDASSERT(!IsND280kA_numu_Config(kND_293kA_nu_nue));
  LOUDASSERT(!IsNumuConfig(kND_293kA_nu_nue));
  LOUDASSERT(!IsFDNueConfig(kND_293kA_nu_nue));

  LOUDASSERT(!IsNuConfig(kND_293kA_nub_nueb));
  LOUDASSERT(IsNDConfig(kND_293kA_nub_nueb));
  LOUDASSERT(IsNDNueConfig(kND_293kA_nub_nueb));
  LOUDASSERT(!IsND293kA_numu_Config(kND_293kA_nub_nueb));
  LOUDASSERT(!IsND280kA_numu_Config(kND_293kA_nub_nueb));
  LOUDASSERT(!IsNumuConfig(kND_293kA_nub_nueb));
  LOUDASSERT(!IsFDNueConfig(kND_293kA_nub_nueb));

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

  LOUDASSERT(!IsNuConfig(kFD_nub_numub));
  LOUDASSERT(!IsNDConfig(kFD_nub_numub));
  LOUDASSERT(GetFDConfig(kFD_nub_numub) == 3);
  LOUDASSERT(!IsNuFDConfig(GetFDConfig(kFD_nub_numub)));
  LOUDASSERT(GetConfigFromFD(GetFDConfig(kFD_nub_numub)) == kFD_nub_numub);
  LOUDASSERT(IsNumuConfig(kFD_nub_numub));
  LOUDASSERT(!IsFDNueConfig(kFD_nub_numub));
  LOUDASSERT(GetConfigNueSwap(kFD_nub_numub) == kFD_nub_nueb);
  LOUDASSERT(GetConfigNonSwap(kFD_nub_numub) == kFD_nub_numub);

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

  LOUDASSERT(!IsNuConfig(kFD_nub_nueb));
  LOUDASSERT(!IsNDConfig(kFD_nub_nueb));
  LOUDASSERT(GetFDConfig(kFD_nub_nueb) == 4);
  LOUDASSERT(!IsNuFDConfig(GetFDConfig(kFD_nub_nueb)));
  LOUDASSERT(GetConfigFromFD(GetFDConfig(kFD_nub_nueb)) == kFD_nub_nueb);
  LOUDASSERT(!IsNumuConfig(kFD_nub_nueb));
  LOUDASSERT(IsFDNueConfig(kFD_nub_nueb));
  LOUDASSERT(GetConfigNueSwap(kFD_nub_nueb) == kFD_nub_nueb);
  LOUDASSERT(GetConfigNonSwap(kFD_nub_nueb) == kFD_nub_numub);
}
} // namespace PRISM
