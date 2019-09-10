
#include "CAFAna/Systs/GENIEHad8nSysts.h"
#include "CAFAna/Core/ModeConversionUtilities.h"
#include "CAFAna/Systs/BDTReweighter.h"

#include "CAFAna/Systs/AGKY-ach-fix_reweight.h"
#include "CAFAna/Systs/KNO-nch-KuzminNaumov_reweight.h"

#include "StandardRecord/StandardRecord.h"
#include "GENIEHad8nSysts.h"


namespace ana
{
  const AGKYachFixSyst kAGKYachFixSyst;
  const KNOnchKuzminNaumovSyst kKNOnchKuzminNaumovSyst;

  void Had8nSyst::Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr, double &weight) const
  {
    // This proof-of-principle version was only trained for numus
    if (!sr->dune.isCC)
      return;
    if (sr->dune.nuPDG != 14)
      return;

    // the hadronization knobs only apply to DIS
    if (GENIEModeToString(GetGENIEModeFromSimbMode(sr->dune.mode)) != "DIS")
      return;

    // this is a straight one-way reweight
    if ( sigma < 0 || std::fabs(sigma - 1) > 0.01 )
      return;

    union BDTReweighter::BDTReweighterFeature features[10];
    features[0].fvalue = sr->dune.eN;
    features[1].fvalue = sr->dune.eP;
    features[2].fvalue = sr->dune.ePi0;
    features[3].fvalue = sr->dune.ePim;
    features[4].fvalue = sr->dune.ePip;

    features[5].fvalue = sr->dune.nN;
    features[6].fvalue = sr->dune.nP;
    features[7].fvalue = sr->dune.nipi0;
    features[8].fvalue = sr->dune.nipim;
    features[9].fvalue = sr->dune.nipip;

    weight *= GetReweighter()->predict(features, 1);

  }

  // -------------------------------------------

  // n.b. do NOT move this implementation to the header, or we'll have to read all 700K lines of AGKY-ach-fix_reweight.h
  // in every macro that #includes GENEIHad8nSysts.h
  AGKYachFixSyst::AGKYachFixSyst()
    : Had8nSyst("AGKY-ach-fix", "Fix to AGKY ach parameter"),
      fBDTReweighter(std::make_unique<AGKY_ach_fix_model>())
  {}

  // -------------------------------------------

  // n.b. do NOT move this implementation to the header, or we'll have to read all 700K lines of AGKY-ach-fix_reweight.h
  // in every macro that #includes GENEIHad8nSysts.h
  KNOnchKuzminNaumovSyst::KNOnchKuzminNaumovSyst()
    : Had8nSyst("KNO-nch-Kuzmin-Naumov", "Use Kuzmin-Naumov fit for nch in KNO parameterization"),
      fBDTReweighter(std::make_unique<KNO_nch_KuzminNaumov_model>())
  {}

}