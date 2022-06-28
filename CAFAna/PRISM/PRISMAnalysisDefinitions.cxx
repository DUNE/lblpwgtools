#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"
#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/Cuts/AnaCuts.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include "TGraph.h"

#include <cmath>
#include <iostream>

namespace PRISM {

bool operator&(NuChan const &l, NuChan const &r) {
  return bool(static_cast<int>(l) & static_cast<int>(r));
}

std::ostream &operator<<(std::ostream &os, NuChan const &nc) {
  switch (nc) {
  case NuChan::kNumuApp: {
    return os << "kNumuApp";
  }
  case NuChan::kNumuBarApp: {
    return os << "kNumuBarApp";
  }
  case NuChan::kNueApp: {
    return os << "kNueApp";
  }
  case NuChan::kNueBarApp: {
    return os << "kNueBarApp";
  }
  case NuChan::kNumuIntrinsic: {
    return os << "kNumuIntrinsic";
  }
  case NuChan::kNumuBarIntrinsic: {
    return os << "kNumuBarIntrinsic";
  }
  case NuChan::kNueIntrinsic: {
    return os << "kNueIntrinsic";
  }
  case NuChan::kNueBarIntrinsic: {
    return os << "kNueBarIntrinsic";
  }
  case NuChan::kNumu: {
    return os << "kNumu";
  }
  case NuChan::kNumuBar: {
    return os << "kNumuBar";
  }
  case NuChan::kNue: {
    return os << "kNue";
  }
  case NuChan::kNueBar: {
    return os << "kNueBar";
  }
  case NuChan::kNumuNumuBar: {
    return os << "kNumuNumuBar";
  }
  case NuChan::kNueNueBar: {
    return os << "kNueNueBar";
  }
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, BeamMode const &bm) {
  switch (bm) {
  case BeamMode::kNuMode: {
    return os << "kNuMode";
  }
  case BeamMode::kNuBarMode: {
    return os << "kNuBarMode";
  }
  case BeamMode::kNuMode_293kA: {
    return os << "kNuMode_293kA";
  }
  case BeamMode::kNuMode_280kA: {
    return os << "kNuMode_280kA";
  }
  case BeamMode::kNuBarMode_293kA: {
    return os << "kNuBarMode_293kA";
  }
  case BeamMode::kNuBarMode_280kA: {
    return os << "kNuBarMode_280kA";
  }
  }
  return os;
}
std::ostream &operator<<(std::ostream &os, BeamChan const &bm) {
  return os << "{ " << bm.mode << ", " << bm.chan << " }";
}
bool operator==(BeamChan const &l, BeamChan const &r) {
  return (l.mode == r.mode) && (l.chan == r.chan);
}

bool operator<(BeamChan const &l, BeamChan const &r) {
  if (l.mode == r.mode) {
    return l.chan < r.chan;
  }
  return l.mode < r.mode;
}

bool operator<(MatchChan const &l, MatchChan const &r) {
  if (l.from == r.from) {
    return l.to < r.to;
  }
  return l.from < r.from;
}
std::ostream &operator<<(std::ostream &os, MatchChan const &mc) {
  return os << mc.from << " -> " << mc.to;
}

} // namespace PRISM
