#include "CAFAna/Vars/FitVarsSterile.h"

#include "OscLib/IOscCalcSterile.h"

#include "CAFAna/Core/MathUtil.h"

namespace ana
{
  // declared as 'extern' in FitVarsSterile.cxx
  const FitDmSq32Sterile kFitDmSq32Sterile;
  const FitDmSq32NHSterile kFitDmSq32NHSterile;
  const FitDmSq32IHSterile kFitDmSq32IHSterile;
  const FitDmSq41Sterile kFitDmSq41Sterile;
  const FitLog10DmSq41 kFitLog10DmSq41;
  const FitDmSq43Sterile kFitDmSq43Sterile;
  const FitDelta13InPiUnitsSterile kFitDelta13InPiUnitsSterile;
  const FitDelta14InPiUnitsSterile kFitDelta14InPiUnitsSterile;
  const FitDelta24InPiUnitsSterile kFitDelta24InPiUnitsSterile;
  const FitTheta13Sterile kFitTheta13Sterile;
  const FitSinSqTheta13Sterile kFitSinSqTheta13Sterile;
  const FitSinSq2Theta13Sterile kFitSinSq2Theta13Sterile;
  const FitTheta23Sterile kFitTheta23Sterile;
  const FitTheta23LOSterile kFitTheta23LOSterile;
  const FitTheta23UOSterile kFitTheta23UOSterile;
  const FitSinSqTheta23Sterile kFitSinSqTheta23Sterile;
  const FitTheta14Sterile kFitTheta14Sterile;
  const FitSinSqTheta14Sterile kFitSinSqTheta14Sterile;
  const FitTheta24Sterile kFitTheta24Sterile;
  const FitSinSqTheta24Sterile kFitSinSqTheta24Sterile;
  const FitSinSq2Theta24Sterile kFitSinSq2Theta24Sterile;
  const FitTheta34Sterile kFitTheta34Sterile;
  const FitSinSqTheta34Sterile kFitSinSqTheta34Sterile;
  const FitTheta13InDegreesSterile kFitTheta13InDegreesSterile;
  const FitTheta23InDegreesSterile kFitTheta23InDegreesSterile;
  const FitTheta14InDegreesSterile kFitTheta14InDegreesSterile;
  const FitTheta24InDegreesSterile kFitTheta24InDegreesSterile;
  const FitTheta34InDegreesSterile kFitTheta34InDegreesSterile;
  const FitVarsProduct kFitSinSq2ThetaMuTauSterileAlpha(
    kFitSinSq2Theta24Sterile, kFitSinSqTheta34Sterile);
  const FitVarsProductMarg kFitSinSq2ThetaMuTauSterileBeta(
    kFitSinSq2Theta24Sterile, kFitSinSqTheta34Sterile);

  //---------------------------------------------------------------------------
  double FitDmSq32Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    const osc::IOscCalcSterile* sterile = osc::DowncastToSterile(osc);
    double dm221 = sterile->GetDm(2);
    double dm231 = sterile->GetDm(3);
    return dm231 - dm221;
  }

  //---------------------------------------------------------------------------
  void FitDmSq32Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::IOscCalcSterile* sterile = osc::DowncastToSterile(osc);
    double dm221 = sterile->GetDm(2);
    double dm231 = val + dm221;
    sterile->SetDm(3, dm231);
  }

  //---------------------------------------------------------------------------
  double FitDmSq32NHSterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    const osc::IOscCalcSterile* sterile = osc::DowncastToSterile(osc);
    double dm221 = sterile->GetDm(2);
    double dm231 = sterile->GetDm(3);
    return dm231 - dm221;
  }

  //---------------------------------------------------------------------------
  void FitDmSq32NHSterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::IOscCalcSterile* sterile = osc::DowncastToSterile(osc);
    double dm221 = sterile->GetDm(2);
    double dm231 = val + dm221;
    sterile->SetDm(3, Clamp(dm231));
  }

  //---------------------------------------------------------------------------
  double FitDmSq32IHSterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    const osc::IOscCalcSterile* sterile = osc::DowncastToSterile(osc);
    double dm221 = sterile->GetDm(2);
    double dm231 = sterile->GetDm(3);
    return dm231 - dm221;
  }

  //---------------------------------------------------------------------------
  void FitDmSq32IHSterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::IOscCalcSterile* sterile = osc::DowncastToSterile(osc);
    double dm221 = sterile->GetDm(2);
    double dm231 = val + dm221;
    sterile->SetDm(3, Clamp(dm231));
  }

  //---------------------------------------------------------------------------
  double FitDmSq41Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToSterile(osc)->GetDm(4);
  }

  //---------------------------------------------------------------------------
  void FitDmSq41Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetDm(4, Clamp(val));
  }

  //---------------------------------------------------------------------------
  double FitLog10DmSq41::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    double val = std::log10(osc::DowncastToSterile(osc)->GetDm(4));
    if (val == -std::numeric_limits<double>::infinity())
      val = LowLimit();
    return val;
  }

  //---------------------------------------------------------------------------
  void FitLog10DmSq41::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetDm(4, pow(10, Clamp(val)));
  }

  //---------------------------------------------------------------------------
  double FitDmSq43Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    const osc::IOscCalcSterile* sterile = osc::DowncastToSterile(osc);
    double dm241 = sterile->GetDm(4);
    double dm231 = sterile->GetDm(3);
    return dm241 - dm231;
  }

  //---------------------------------------------------------------------------
  void FitDmSq43Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::IOscCalcSterile* sterile = osc::DowncastToSterile(osc);
    double dm231 = sterile->GetDm(3);
    double dm241 = val + dm231;
    sterile->SetDm(4, dm241);
  }

  //----------------------------------------------------------------------
  double FitDelta13InPiUnitsSterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    const osc::IOscCalcSterile* sterile = osc::DowncastToSterile(osc);
    double ret = sterile->GetDelta(1, 3)/M_PI;
    while(ret < 0) ret += 2;
    while(ret > 2) ret -= 2;
    return ret;
  }

  //----------------------------------------------------------------------
  void FitDelta13InPiUnitsSterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetDelta(1, 3, M_PI*val);
  }

  //----------------------------------------------------------------------
  double FitDelta14InPiUnitsSterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    const osc::IOscCalcSterile* sterile = osc::DowncastToSterile(osc);
    double ret = sterile->GetDelta(1, 4)/M_PI;
    while(ret < 0) ret += 2;
    while(ret > 2) ret -= 2;
    return ret;
  }

  //----------------------------------------------------------------------
  void FitDelta14InPiUnitsSterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetDelta(1, 4, M_PI*val);
  }

  //----------------------------------------------------------------------
  double FitDelta24InPiUnitsSterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToSterile(osc)->GetDelta(2, 4) / M_PI;
  }

  //----------------------------------------------------------------------
  void FitDelta24InPiUnitsSterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetDelta(2, 4, M_PI*Clamp(val));
  }

  //---------------------------------------------------------------------------
  double FitTheta13Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToSterile(osc)->GetAngle(1,3);
  }

  //----------------------------------------------------------------------
  void FitTheta13Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(1, 3, Clamp(val));
  }

  //----------------------------------------------------------------------
  double FitSinSqTheta13Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return util::sqr(sin(osc::DowncastToSterile(osc)->GetAngle(1,3)));
  }

  //----------------------------------------------------------------------
  void FitSinSqTheta13Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(1, 3, asin(sqrt(Clamp(val))));
  }

  //----------------------------------------------------------------------
  double FitSinSq2Theta13Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return util::sqr(sin(2*osc::DowncastToSterile(osc)->GetAngle(1,3)));
  }

  //----------------------------------------------------------------------
  void FitSinSq2Theta13Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(1, 3, 0.5*asin(sqrt(Clamp(val))));
  }

  //---------------------------------------------------------------------------
  double FitTheta23Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToSterile(osc)->GetAngle(2,3);
  }

  //----------------------------------------------------------------------
  void FitTheta23Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(2, 3, Clamp(val));
  }

  //---------------------------------------------------------------------------
  double FitTheta23LOSterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToSterile(osc)->GetAngle(2,3);
  }

  //----------------------------------------------------------------------
  void FitTheta23LOSterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(2, 3, Clamp(val));
  }

  //---------------------------------------------------------------------------
  double FitTheta23UOSterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToSterile(osc)->GetAngle(2,3);
  }

  //----------------------------------------------------------------------
  void FitTheta23UOSterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(2, 3, Clamp(val));
  }

  //----------------------------------------------------------------------
  double FitSinSqTheta23Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return util::sqr(sin(osc::DowncastToSterile(osc)->GetAngle(2,3)));
  }

  //----------------------------------------------------------------------
  void FitSinSqTheta23Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(2, 3, asin(sqrt(Clamp(val))));
  }

  //---------------------------------------------------------------------------
  double FitTheta14Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToSterile(osc)->GetAngle(1,4);
  }

  //----------------------------------------------------------------------
  void FitTheta14Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(1, 4, Clamp(val));
  }

  //----------------------------------------------------------------------
  double FitSinSqTheta14Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return util::sqr(sin(osc::DowncastToSterile(osc)->GetAngle(1,4)));
  }

  //----------------------------------------------------------------------
  void FitSinSqTheta14Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(1, 4, asin(sqrt(Clamp(val))));
  }

  //---------------------------------------------------------------------------
  double FitTheta24Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToSterile(osc)->GetAngle(2,4);
  }

  //----------------------------------------------------------------------
  void FitTheta24Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(2, 4, Clamp(val));
  }

  //----------------------------------------------------------------------
  double FitSinSqTheta24Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return util::sqr(sin(osc::DowncastToSterile(osc)->GetAngle(2,4)));
  }

  //----------------------------------------------------------------------
  void FitSinSqTheta24Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(2, 4, asin(sqrt(Clamp(val))));
  }

  //----------------------------------------------------------------------
  double FitSinSq2Theta24Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return util::sqr(sin(2*osc::DowncastToSterile(osc)->GetAngle(2,4)));
  }

  //----------------------------------------------------------------------
  void FitSinSq2Theta24Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(2, 4, asin(sqrt(Clamp(val)))/2);
  }

  //---------------------------------------------------------------------------
  double FitTheta34Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToSterile(osc)->GetAngle(3,4);
  }

  //----------------------------------------------------------------------
  void FitTheta34Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(3, 4, Clamp(val));
  }

  //----------------------------------------------------------------------
  double FitSinSqTheta34Sterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return util::sqr(sin(osc::DowncastToSterile(osc)->GetAngle(3,4)));
  }

  //----------------------------------------------------------------------
  void FitSinSqTheta34Sterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(3, 4, asin(sqrt(Clamp(val))));
  }

  //---------------------------------------------------------------------------
  double FitTheta13InDegreesSterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return TMath::RadToDeg()*osc::DowncastToSterile(osc)->GetAngle(1,3);
  }

  //----------------------------------------------------------------------
  void FitTheta13InDegreesSterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(1, 3, TMath::DegToRad()*Clamp(val));
  }

  //---------------------------------------------------------------------------
  double FitTheta23InDegreesSterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return TMath::RadToDeg()*osc::DowncastToSterile(osc)->GetAngle(2,3);
  }

  //----------------------------------------------------------------------
  void FitTheta23InDegreesSterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(2, 3, TMath::DegToRad()*Clamp(val));
  }

  //---------------------------------------------------------------------------
  double FitTheta14InDegreesSterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return TMath::RadToDeg()*osc::DowncastToSterile(osc)->GetAngle(1,4);
  }

  //----------------------------------------------------------------------
  void FitTheta14InDegreesSterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(1, 4, TMath::DegToRad()*Clamp(val));
  }

  //---------------------------------------------------------------------------
  double FitTheta24InDegreesSterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return TMath::RadToDeg()*osc::DowncastToSterile(osc)->GetAngle(2,4);
  }

  //----------------------------------------------------------------------
  void FitTheta24InDegreesSterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(2, 4, TMath::DegToRad()*Clamp(val));
  }

  //---------------------------------------------------------------------------
  double FitTheta34InDegreesSterile::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return TMath::RadToDeg()*osc::DowncastToSterile(osc)->GetAngle(3,4);
  }

  //----------------------------------------------------------------------
  void FitTheta34InDegreesSterile::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToSterile(osc)->SetAngle(3, 4, TMath::DegToRad()*Clamp(val));
  }

} // namespace