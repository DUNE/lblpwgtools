#include "CAFAna/Analysis/Calcs.h"
#include "CAFAna/Analysis/CalcsVars.h"

#include "OscLib/func/OscCalculatorPMNSOpt.h"
#include "OscLib/func/OscCalculatorSterile.h"
#include "OscLib/func/OscCalculatorGeneral.h"

#include <cmath>
#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  void ResetOscCalcToDefault(osc::IOscCalculatorAdjustable* calc)
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
  osc::IOscCalculatorAdjustable* DefaultOscCalc()
  {
    osc::IOscCalculatorAdjustable* ret = new osc::OscCalculatorGeneral;
    ResetOscCalcToDefault(ret);
    return ret;
  }

  //----------------------------------------------------------------------
  void ResetOscCalcToDefaultIH(osc::IOscCalculatorAdjustable* calc)
  {
    //Share most defaults 
    ResetOscCalcToDefault(calc);
    calc->SetTh23(kPDGTh23IH);
    calc->SetDmsq32(kPDGDmsq32IH); 

  }

  //----------------------------------------------------------------------
  osc::IOscCalculatorAdjustable* DefaultOscCalcIH()
  {
    osc::IOscCalculatorAdjustable* ret = new osc::OscCalculatorGeneral;
    ResetOscCalcToDefaultIH(ret);
    return ret;
  }

  //----------------------------------------------------------------------
  void ResetSterileCalcToDefault(osc::OscCalculatorSterile* calc)
  {
    osc::OscCalculatorPMNSOpt* tmp = new osc::OscCalculatorPMNSOpt();
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
  osc::OscCalculatorSterile* DefaultSterileCalc(int nflavors)
  {
    osc::OscCalculatorSterile* ret = new osc::OscCalculatorSterile;

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
