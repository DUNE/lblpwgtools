#ifndef OSCCALCPMNSSERVICE_H
#define OSCCALCPMNSSERVICE_H

#include "OscLib/func/OscCalculatorPMNS.h"

// ART includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

namespace osc
{
  class OscCalcPMNSService: public OscCalculatorPMNS
  {
  public:
    OscCalcPMNSService(const fhicl::ParameterSet&, art::ActivityRegistry&)
    {
    }
  };

} // end namespace

DECLARE_ART_SERVICE(osc::OscCalcPMNSService, LEGACY)

#endif
