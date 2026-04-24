// This can probably evolve to live in a more general and shared place

#include "CAFAna/Analysis/Calcs.h"

#include "OscLib/OscCalcPMNS_NSI.h"
#include "OscLib/IOscCalcSterile.h"
#include "OscLib/OscCalcSterile.h"

using namespace osc;
using namespace ana;

  void PrintOscCalc(osc::IOscCalcAdjustable* calc) {

    std::cout << "\n---------- OSCILLATION PARAMETERS ----------" << std::endl
              << "Dm21:          " << calc->GetDmsq21()          << std::endl
              << "Dm32:          " << calc->GetDmsq32()          << std::endl
              << "Theta 12       " << calc->GetTh12()            << std::endl
              << "Theta 13:      " << calc->GetTh13()            << std::endl
              << "Theta 23:      " << calc->GetTh23()            << std::endl
              << "Delta CP (pi): " << calc->GetdCP()/M_PI << std::endl
              << "Rho:           " << calc->GetRho()             << std::endl
              << "--------------------------------------------\n" << std::endl;
  }


// NSI

void ResetOscCalcNSIToDefault(osc::OscCalcPMNS_NSI* calc){
    // share the PMNS parameters from the std osc calc
    ResetOscCalcToDefault(calc);

    // defaults all nsi to zero
    calc->SetEps_ee(0.0);
    calc->SetEps_mumu(0.0);
    calc->SetEps_tautau(0.0);
    calc->SetEps_emu(0.0);
    calc->SetEps_etau(0.0);
    calc->SetEps_mutau(0.0);
    calc->SetDelta_emu(0.0*M_PI);
    calc->SetDelta_etau(0.0*M_PI);
    calc->SetDelta_mutau(0.0*M_PI);
  }

  osc::OscCalcPMNS_NSI* DefaultOscCalcNSI()
  {
    auto* c = new osc::OscCalcPMNS_NSI;
    ResetOscCalcNSIToDefault(c);
    return c;
  }


// Steriles

  void ResetSterileCalcToDefault(osc::OscCalcSterile* calc)
  {
    osc::IOscCalcSterile* tmp = new osc::OscCalcSterile();
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
