#ifndef OSCCALCPMNSOPTSERVICE_H
#define OSCCALCPMNSOPTSERVICE_H

#include "../func/OscCalculatorPMNSOpt.h"

// ART includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

namespace osc
{
  class OscCalcPMNSOptService: public OscCalculatorPMNSOpt
  {
  public:
    OscCalcPMNSOptService(const fhicl::ParameterSet&, art::ActivityRegistry&)
    {
    }
  };

} // end namespace

DECLARE_ART_SERVICE(osc::OscCalcPMNSOptService, LEGACY)

#endif
