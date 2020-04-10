# pragma once

#include <type_traits>

#include "CAFAna/Experiment/IChiSqExperiment.h"
#include "CAFAna/Experiment/ILkhdExperiment.h"

namespace ana
{
  /// Semi-magic macro to wrap the method from the base class
  /// (so that a check that that method applies to the contained object can be done)
  /// without having to explicitly copy the function signature.
  /// N.B.: this variant assumes the function should be const...
#define VALIDATE(METHOD_NAME, CHISQ_OR_LKHD, BASE_CLASS_NAME, MEMBER_NAME ) \
  template <typename ... Args> \
  typename std::result_of<decltype(&BASE_CLASS_NAME::METHOD_NAME)(BASE_CLASS_NAME*, Args...)>::type METHOD_NAME(Args&& ... args) const \
  { \
    this->Validate(CHISQ_OR_LKHD, #METHOD_NAME); \
    return MEMBER_NAME->METHOD_NAME(std::forward<Args>(args)...); \
  }
#define VALIDATE_CHISQ(METHOD_NAME) VALIDATE(METHOD_NAME, CHISQ, IChiSqExperiment, fChisqExpt)
#define VALIDATE_LKHD(METHOD_NAME)  VALIDATE(METHOD_NAME, LKHD,  ILkhdExperiment, fLkhdExpt)

  /// Container that can hold both ChiSq and Lkhd type experiments
  class IExperiment
  {
    private:
      enum Tag {CHISQ, LKHD};

    public:
      IExperiment(const IChiSqExperiment * expt);
      IExperiment(const ILkhdExperiment * expt);

      ///@{ accessor methods for IChiSqExperiment
      VALIDATE_CHISQ(ChiSq);
      VALIDATE_CHISQ(Derivative);
      VALIDATE_CHISQ(SaveTo);
      VALIDATE_CHISQ(PredHist);
      VALIDATE_CHISQ(DataHist);
      VALIDATE_CHISQ(ApplyMask);
      ///@}

      ///@{ accessor methods for ILkhdExperiment
      VALIDATE_LKHD(LogLikelihood);
      ///@}



      /// used for pointer testing, mostly
      explicit operator bool() const
      {
        return (fTag == CHISQ) ? bool(fChisqExpt) : bool(fLkhdExpt);
      }

    private:
      void Validate(Tag tag, const std::string & methName) const;

      Tag fTag;
      union
      {
        const IChiSqExperiment * fChisqExpt;
        const ILkhdExperiment  * fLkhdExpt;
      };
  };

}