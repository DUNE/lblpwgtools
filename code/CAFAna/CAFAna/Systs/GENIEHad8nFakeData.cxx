
#include "CAFAna/Systs/GENIEHad8nFakeData.h"
#include "CAFAna/Core/ModeConversionUtilities.h"
#include "CAFAna/Systs/BDTReweighter.h"

#include "CAFAna/Systs/AGKY-ach-fix_reweight.h"
#include "CAFAna/Systs/KNO-nch-KuzminNaumov_reweight.h"

#include "StandardRecord/StandardRecord.h"

namespace ana
{
  void Had8nFakeDataGenerator::Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr, double &weight) const
  {
    // This proof-of-principle version was only trained for numus
    if (!sr->dune.isCC)
      return;
    if (sr->dune.nuPDG != 14)
      return;

    // the hadronization knobs only apply to DIS
    std::string mode = GENIEModeToString(sr->dune.GENIE_ScatteringMode);
    if (mode != "DIS")
      return;

    // this is a straight one-way reweight
    if ( sigma < 0 || std::fabs(sigma - 1) > 0.01 )
      return;

    union BDTReweighter::BDTReweighterFeature features[10];
    // note that the eX fields are KINETIC energies, but the BDT was trained using TOTAL energy
    features[0].fvalue = sr->dune.eN + sr->dune.nN * 0.939565;
    features[1].fvalue = sr->dune.eP + sr->dune.nP * 0.938272;
    features[2].fvalue = sr->dune.ePi0 + sr->dune.nipi0 * 0.134977;
    features[3].fvalue = sr->dune.ePim + sr->dune.nipim * 0.139570;
    features[4].fvalue = sr->dune.ePip + sr->dune.nipip * 0.139570;

    features[5].fvalue = sr->dune.nN;
    features[6].fvalue = sr->dune.nP;
    features[7].fvalue = sr->dune.nipi0;
    features[8].fvalue = sr->dune.nipim;
    features[9].fvalue = sr->dune.nipip;

    // this could (should?) be done instead by asking the BDT for the 'margin'
    // (which can be exponentiated directly to get the probability)
    // instead of having to do the calculation below
    double bdtVal = GetReweighter()->GetWeight(features, 0);
    weight *= 1./(1. - bdtVal) - 1;

  }

  // -------------------------------------------

  // n.b. do NOT move this implementation to the header, or we'll have to read all 700K lines of AGKY-ach-fix_reweight.h
  // in every macro that #includes GENEIHad8nSysts.h
  AGKYachFixFakeDataGenerator::AGKYachFixFakeDataGenerator()
    : Had8nFakeDataGenerator("AGKY-ach-fix", "Fix to AGKY ach parameter"),
      fBDTReweighter(std::make_unique<AGKY_ach_fix_model>())
  {}

  // -------------------------------------------

  // n.b. do NOT move this implementation to the header, or we'll have to read all 700K lines of AGKY-ach-fix_reweight.h
  // in every macro that #includes GENEIHad8nSysts.h
  KNOnchKuzminNaumovFakeDataGenerator::KNOnchKuzminNaumovFakeDataGenerator()
    : Had8nFakeDataGenerator("KNO-nch-Kuzmin-Naumov", "Use Kuzmin-Naumov fit for nch in KNO parameterization"),
      fBDTReweighter(std::make_unique<KNO_nch_KuzminNaumov_model>())
  {}

}