#ifndef OSCCALCSERVICE_H
#define OSCCALCSERVICE_H

#include "OscLib/func/OscCalculator.h"

// ART includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

namespace osc
{
  class OscCalcService: public OscCalculator
  {
  public:
    OscCalcService(const fhicl::ParameterSet&, art::ActivityRegistry&)
    {
    }
  };

} // end namespace

DECLARE_ART_SERVICE(osc::OscCalcService, LEGACY)

#endif
