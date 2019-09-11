#include "CAFAna/Core/ModeConversionUtilities.h"

#include "CAFAna/Systs/GENIEHad8nFakeData.h"
#include "CAFAna/Systs/MissingProtonFakeData.h"
#include "CAFAna/Systs/NuWroReweightFakeData.h"
#include "CAFAna/Systs/XSecSysts.h"

#include "StandardRecord/StandardRecord.h"

namespace ana {

MissingProtonFakeDataGenerator mpfd;
NuWroReweightFakeDataGenerator nuwrofd;
AGKYachFixFakeDataGenerator agkyFixfd;
KNOnchKuzminNaumovFakeDataGenerator kuzminNaumovfd;

void XSecSyst::FakeDataDialShift(double sigma, Restorer &restore,
                                 caf::StandardRecord *sr,
                                 double &weight) const {

  // First time hook up known fake data dial IDs, logic then in switch
  // statement.
  static int Mnv2p2hGaussEnhancement_id = 0;
  static int Mnv2p2hGaussEnhancement_NN_id = 0;
  static int Mnv2p2hGaussEnhancement_2p2h_id = 0;
  static int Mnv2p2hGaussEnhancement_1p1h_id = 0;
  static int MKSPP_ReWeight_id = 0;
  static int SPPLowQ2Suppression_id = 0;
  static int FSILikeEAvailSmearing_id = 0;
  static int MissingProtonFakeData_id = 0;
  static int NuWroReweightFakeData_id = 0;
  static int AGKYachFixFakeData_id = 0;
  static int KNOnchKuzminNaumovFakeData_id = 0;

  static bool first = true;
  if (first) {
    first = false;
    Mnv2p2hGaussEnhancement_id = GetXSecSystIndex("Mnv2p2hGaussEnhancement");
    Mnv2p2hGaussEnhancement_NN_id =
        GetXSecSystIndex("Mnv2p2hGaussEnhancement_NN");
    Mnv2p2hGaussEnhancement_2p2h_id =
        GetXSecSystIndex("Mnv2p2hGaussEnhancement_2p2h");
    Mnv2p2hGaussEnhancement_1p1h_id =
        GetXSecSystIndex("Mnv2p2hGaussEnhancement_1p1h");
    MKSPP_ReWeight_id = GetXSecSystIndex("MKSPP_ReWeight");
    SPPLowQ2Suppression_id = GetXSecSystIndex("SPPLowQ2Suppression");
    FSILikeEAvailSmearing_id = GetXSecSystIndex("FSILikeEAvailSmearing");
    MissingProtonFakeData_id = GetXSecSystIndex("MissingProtonFakeData");
    NuWroReweightFakeData_id = GetXSecSystIndex("NuWroReweightFakeData");
    AGKYachFixFakeData_id = GetXSecSystIndex("AGKYachFixFakeData");
    KNOnchKuzminNaumovFakeData_id = GetXSecSystIndex("KNOnchKuzminNaumovFakeData");
  }

  if (fID == Mnv2p2hGaussEnhancement_NN_id) {
    weight *= sr->dune.xsSyst_wgt[Mnv2p2hGaussEnhancement_id][4];
    // if (sr->dune.xsSyst_wgt[Mnv2p2hGaussEnhancement_id][4] != 1) {
    //   std::cout << "[INFO]: Fake data Weight for Mnv2p2hGaussEnhancement_NN = "
    //             << sr->dune.xsSyst_wgt[Mnv2p2hGaussEnhancement_id][4]
    //             << ", mode = " << sr->dune.GENIE_ScatteringMode << std::endl;
    // }
  } else if (fID == Mnv2p2hGaussEnhancement_2p2h_id) {
    weight *= sr->dune.xsSyst_wgt[Mnv2p2hGaussEnhancement_id][3];
    // if (sr->dune.xsSyst_wgt[Mnv2p2hGaussEnhancement_id][3] != 1) {
    //   std::cout
    //       << "[INFO]: Fake data Weight for Mnv2p2hGaussEnhancement_2p2h = "
    //       << sr->dune.xsSyst_wgt[Mnv2p2hGaussEnhancement_id][3]
    //       << ", mode = " << sr->dune.GENIE_ScatteringMode << std::endl;
    // }
  } else if (fID == Mnv2p2hGaussEnhancement_1p1h_id) {
    weight *= sr->dune.xsSyst_wgt[Mnv2p2hGaussEnhancement_id][2];
    // if (sr->dune.xsSyst_wgt[Mnv2p2hGaussEnhancement_id][2] != 1) {
    //   std::cout
    //       << "[INFO]: Fake data Weight for Mnv2p2hGaussEnhancement_1p1h = "
    //       << sr->dune.xsSyst_wgt[Mnv2p2hGaussEnhancement_id][2]
    //       << ", mode = " << sr->dune.GENIE_ScatteringMode << std::endl;
    // }
  } else if (fID == MKSPP_ReWeight_id) {
    weight *= sr->dune.xsSyst_wgt[MKSPP_ReWeight_id][2];
    // if (sr->dune.xsSyst_wgt[MKSPP_ReWeight_id][2] != 1) {
    //   std::cout << "[INFO]: Fake data Weight for MKSPP_ReWeight = "
    //             << sr->dune.xsSyst_wgt[MKSPP_ReWeight_id][2]
    //             << ", mode = " << sr->dune.GENIE_ScatteringMode << std::endl;
    // }
  } else if (fID == SPPLowQ2Suppression_id) {
    weight *= sr->dune.xsSyst_wgt[SPPLowQ2Suppression_id][2];
    // if (sr->dune.xsSyst_wgt[SPPLowQ2Suppression_id][2] != 1) {
    //   std::cout << "[INFO]: Fake data Weight for SPPLowQ2Suppression = "
    //             << sr->dune.xsSyst_wgt[SPPLowQ2Suppression_id][2]
    //             << ", mode = " << sr->dune.GENIE_ScatteringMode << std::endl;
    // }
  } else if (fID == FSILikeEAvailSmearing_id) {
    weight *= sr->dune.xsSyst_wgt[FSILikeEAvailSmearing_id][2];
    // if (sr->dune.xsSyst_wgt[FSILikeEAvailSmearing_id][2] != 1) {
    //   std::cout << "[INFO]: Fake data Weight for FSILikeEAvailSmearing = "
    //             << sr->dune.xsSyst_wgt[FSILikeEAvailSmearing_id][2]
    //             << ", mode = " << sr->dune.GENIE_ScatteringMode << std::endl;
    // }
  } else if (fID == MissingProtonFakeData_id) {
    mpfd.Shift(sigma, restore, sr, weight);
  } else if (fID == NuWroReweightFakeData_id) {
    nuwrofd.Shift(sigma, restore, sr, weight);
  } else if (fID == AGKYachFixFakeData_id) {
    agkyFixfd.Shift(sigma, restore, sr, weight);
  } else if (fID == KNOnchKuzminNaumovFakeData_id) {
    kuzminNaumovfd.Shift(sigma, restore, sr, weight);
  }
}
void XSecSyst::Shift(double sigma, Restorer &restore, caf::StandardRecord *sr,
                     double &weight) const {
  // No xs weights in this event, skip reweighting it
  if (sr->dune.xsSyst_wgt.empty()) {
    return;
  }
  if (sr->dune.xsSyst_wgt[0].empty()) {
    return;
  }
  if (fabs(sigma) < 1E-5) {
    return;
  }

  if (IsFakeDataGenerationSyst(
          fID)) { // Separate out hard coded logic for fake data dials.
    assert(fabs(sigma - 1) < 1E-5);
    FakeDataDialShift(sigma, restore, sr, weight);
    return;
  }

  assert(std::abs(sigma) <= 3 &&
         "XSecs responses only valid up to +/-3 sigma!");

  bool IsExtrapolatedSyst = IsExtrapolateOffToOnSyst(fID);

  // How far apart are the points
  const double spacing = 1;

  // Get the top and bottom values in the array
  int low_index = std::floor(sigma / spacing) + 3;
  int high_index = std::ceil(sigma / spacing) + 3;

  double fact = 1;
  if (IsExtrapolatedSyst) {

    //    double diff = sigma / spacing;

    double weight_at_cv = 1;
    double weight_at_one_sig = sr->dune.xsSyst_wgt[fID][4];

    double dx = 1;
    double dy = weight_at_one_sig - weight_at_cv;

    // y = m*x + c
    fact =  (dy / dx) * sigma + weight_at_cv;

  } else {
    double diff = (sigma - double(low_index)) / spacing;

    double low_weight = sr->dune.xsSyst_wgt[fID][low_index];
    double high_weight = sr->dune.xsSyst_wgt[fID][high_index];

    fact = low_weight + (high_weight - low_weight) * diff;
  }

  fact = std::min(fact, 10.);
  weight *= fact;
}

XSecSyst::XSecSyst(int syst_id, bool applyPenalty)
    : ISyst(GetXSecSystName(syst_id), GetXSecSystName(syst_id), applyPenalty,
            GetXSecSystMin(syst_id), GetXSecSystMax(syst_id)),
      fID(syst_id) {}

std::vector<const ISyst *> GetXSecSysts(std::vector<std::string> names,
                                        bool applyPenalty) {
  static std::vector<const ISyst *> ret;

  if (names.empty()) {
    names = GetAllXSecSystNames();
  }

  if (ret.empty()) {
    for (auto &it : names) {
      ret.push_back(new XSecSyst(GetXSecSystIndex(it), applyPenalty));
    }
  }
  return ret;
}

} // namespace ana
