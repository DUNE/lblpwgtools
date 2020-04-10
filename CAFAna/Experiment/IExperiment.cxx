
#include "CAFAna/Experiment/IExperiment.h"

namespace ana
{
  IExperiment::IExperiment(const ana::IChiSqExperiment *expt)
    : fTag(CHISQ), fChisqExpt(expt)
  {}

  IExperiment::IExperiment(const ana::ILkhdExperiment *expt)
    : fTag(LKHD), fLkhdExpt(expt)
  {}

  void IExperiment::Validate(ana::IExperiment::Tag tag, const std::string &methName) const
  {
    if (tag == fTag)
      return;

    std::cerr << "You tried to invoke method '" << methName << "()' on a "
              << (fTag == CHISQ ? "IChiSqExperiment" : "ILkhdExperiment") << " object!" << std::endl;
    abort();
  }

}