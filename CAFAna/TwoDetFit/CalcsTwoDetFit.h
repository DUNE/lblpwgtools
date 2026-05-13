// This can probably evolve to live in a more general and shared place

#include "CAFAna/Analysis/Calcs.h"

#include "OscLib/OscCalcPMNS_NSI.h"
#include "OscLib/IOscCalcSterile.h"
#include "OscLib/OscCalcSterile.h"

#include "CAFAna/Core/MathUtil.h"

using namespace osc;
using namespace ana;

  void PrintOscCalc(osc::IOscCalcAdjustable* calc) {
    std::cout << "\n---------- OSCILLATION PARAMETERS ----------"          << std::endl
              << "DmSq21:        " << calc->GetDmsq21()                    << std::endl
              << "DmSq32:        " << calc->GetDmsq32()                    << std::endl
              << "SinSq(Th12):   " << util::sqr(std::sin(calc->GetTh12())) << std::endl
              << "SinSq(Th13):   " << util::sqr(std::sin(calc->GetTh13())) << std::endl
              << "SinSq(Th23):   " << util::sqr(std::sin(calc->GetTh23())) << std::endl
              << "Delta CP (pi): " << calc->GetdCP()/M_PI                  << std::endl
              << "Rho:           " << calc->GetRho()                       << std::endl
              << "L (km):        " << calc->GetL()                         << std::endl
              << "--------------------------------------------\n"          << std::endl;
  }


// Specific asimov points of interest
void ResetOscCalcToAsimovPoint(std::string const &s_asimov_point, osc::IOscCalcAdjustable* calc) {

ResetOscCalcToDefault(calc);

 if(s_asimov_point == "pdg2025"){ // https://pdg.lbl.gov/2025/web/viewer.html?file=../tables/rpp2025-sum-leptons.pdf
      calc->SetDmsq21(7.50e-5);
      calc->SetDmsq32(2.451e-3);        // NO
      calc->SetTh12(asin(sqrt(0.307)));
      calc->SetTh13(asin(sqrt(0.0216)));
      calc->SetTh23(asin(sqrt(0.534))); // NO
      calc->SetdCP(1.21*M_PI);
 }
  else {
      std::cout << "\n\n Oooops! Asimov point << " << s_asimov_point << " not supported, exiting..." << std::endl;
      std::exit(0);
    }
}


void ResetOscCalcSterileToAsimovPoint(std::string const &s_asimov_point_nsi, osc::IOscCalcAdjustable* osc_calc) {

    auto* calc = dynamic_cast<osc::OscCalcSterile*>(osc_calc);
    if(!calc){
        throw std::runtime_error( "\nOops: osc_calc is not of OscCalcSterile type. Aborting...");
        exit(1);
    }

    // Set to default first
    ResetSterileCalcToDefault(calc);

    // Now pass any asimov point of interest
    if(s_asimov_point_nsi == "pdg2025"){ // https://pdg.lbl.gov/2025/web/viewer.html?file=../tables/rpp2025-sum-leptons.pdf
        calc->SetDm(2, 7.50e-5);
        calc->SetDm(3, 7.50e-5 + 2.451e-3);  // DmSq32 = 2.451e-3 for NO
        calc->SetAngle(1, 2, asin(sqrt(0.307)));
        calc->SetAngle(1, 3, asin(sqrt(0.0216)));
        calc->SetAngle(2, 3, asin(sqrt(0.534)));  // NO
        calc->SetDelta(1, 3, 1.21*M_PI);
    }
    else {
        std::cout << "\n\n Oooops! Asimov point '" << s_asimov_point_nsi << "' not supported for sterile, using default..." << std::endl;
    }

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
