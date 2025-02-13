#include "CAFAna/Core/ISyst.h"

#include "cafanacore/Registry.h"
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Core/MathUtil.h"

namespace ana
{

  //----------------------------------------------------------------------
  //template<class RecT> ISyst<RecT>::ISyst(const std::string& shortName,
  ISyst::ISyst(const std::string& shortName,
               const std::string& latexName,
         bool applyPenalty,
         double min,
         double max,
         double cv)
    //: _ISyst<RecT>(shortName, latexName), fApplyPenalty(applyPenalty), fMin(min), fMax(max)
    : _ISyst<caf::SRInteractionProxy>(shortName, latexName), fApplyPenalty(applyPenalty), fMin(min), fMax(max), fCentral(cv)
  {
    //Registry<ISyst<RecT>>::Register(this);
    Registry<ISyst>::Register(this);
  }

  //----------------------------------------------------------------------
  //template<class RecT> ISyst<RecT>::~ISyst<RecT>()
  ISyst::~ISyst()
  {
    // Normally ISysts should last for the life of the process, but in case one
    // is deleted it's best not to leave a dangling pointer in Registry.
    //Registry<ISyst<RecT>>::UnRegister(this);
    Registry<ISyst>::UnRegister(this);
  }

  //----------------------------------------------------------------------
  //template<class RecT> double ISyst<RecT>::Penalty(double x ) const
  double ISyst::Penalty(double x) const
  {
    if(fApplyPenalty){
      // Regular quadratic penalty term
      // Error is always 1, so can ignore that
      return (x-fCentral)*(x-fCentral);
    }
    else{
      // Otherwise, no penalty within range, but still apply one outside
      if(x >= Min() && x <= Max()) return 0;

      // Try to direct fit back towards centre of the space. Engineer penalty
      // to be zero at the limits.
      const double mean = (Min()+Max())/2;
      const double rad = (Max()-Min())/2;
      return util::sqr((x-mean)/rad)-1;
    }
  }
}
