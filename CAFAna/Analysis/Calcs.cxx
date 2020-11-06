#include "CAFAna/Analysis/Calcs.h"
#include "CAFAna/Analysis/CalcsVars.h"

#include "OscLib/OscCalcPMNSOpt.h"
#include "OscLib/OscCalcSterile.h"
#include "OscLib/OscCalcGeneral.h"

#include <cmath>
#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  void ResetOscCalcToDefault(osc::IOscCalcAdjustable* calc)
  {
    calc->SetL(kBaseline);
    calc->SetRho(kEarthDensity);

    calc->SetDmsq21(kPDGDmsq21);
    calc->SetTh12(kPDGTh12);
    calc->SetTh13(kPDGTh13);

    calc->SetTh23(kPDGTh23NH); // Picking anything away from maximal mixing doesn't behave
    calc->SetDmsq32(kPDGDmsq32NH);

    calc->SetdCP(0); // Going to have to plot for nue analysis anyway 
  }

  //----------------------------------------------------------------------
  osc::IOscCalcAdjustable* DefaultOscCalc()
  {
    osc::IOscCalcAdjustable* ret = new osc::OscCalcGeneral;
    ResetOscCalcToDefault(ret);
    return ret;
  }

  //----------------------------------------------------------------------
  void ResetOscCalcToDefaultIH(osc::IOscCalcAdjustable* calc)
  {
    //Share most defaults 
    ResetOscCalcToDefault(calc);
    calc->SetTh23(kPDGTh23IH);
    calc->SetDmsq32(kPDGDmsq32IH); 

  }

  //----------------------------------------------------------------------
  osc::IOscCalcAdjustable* DefaultOscCalcIH()
  {
    osc::IOscCalcAdjustable* ret = new osc::OscCalcGeneral;
    ResetOscCalcToDefaultIH(ret);
    return ret;
  }

  //----------------------------------------------------------------------
  void ResetSterileCalcToDefault(osc::OscCalcSterile* calc)
  {
    osc::OscCalcPMNSOpt* tmp = new osc::OscCalcPMNSOpt();
    ResetOscCalcToDefault(tmp);

    calc->SetL(tmp->GetL());
    calc->SetRho(tmp->GetRho());

    calc->SetDm(2, tmp->GetDmsq21());
    calc->SetDm(3, tmp->GetDmsq21() + tmp->GetDmsq32());

    calc->SetAngle(1, 2, tmp->GetTh12());
    calc->SetAngle(1, 3, tmp->GetTh13());
    calc->SetAngle(2, 3, tmp->GetTh23());

    calc->SetDelta(1, 3, tmp->GetdCP());

    delete tmp;
  }

  //----------------------------------------------------------------------
  osc::OscCalcSterile* DefaultSterileCalc(int nflavors)
  {
    osc::OscCalcSterile* ret = new osc::OscCalcSterile;

    if(nflavors < 3) {
      std::cout << "The default calculator requires at least 3 flavors." << std::endl;
      std::cout << "Using 3 flavors." << std::endl;
      ret->SetNFlavors(3);
    }
    else {
      ret->SetNFlavors(nflavors);
    }

    ResetSterileCalcToDefault(ret);
    return ret;
  }
}
