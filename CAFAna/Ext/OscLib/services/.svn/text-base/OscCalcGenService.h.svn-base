#ifndef OSCCALCGENSERVICE_H
#define OSCCALCGENSERVICE_H

#include "OscLib/func/OscCalculatorGeneral.h"

// ART includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

namespace osc
{
  class OscCalcGenService: public OscCalculatorGeneral
  {
  public:
    OscCalcGenService(const fhicl::ParameterSet&, art::ActivityRegistry&)
    {
    }
  };

} // end namespace

DECLARE_ART_SERVICE(osc::OscCalcGenService, LEGACY)

#endif

