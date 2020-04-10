
#include "CAFAna/Experiment/IExperiment.h"

namespace ana
{
  void IExperiment::Validate(ana::IExperiment::Type typ, const std::string &methName) const
  {
    if ((typ == CHISQ && fChisqExpt) || (typ == LKHD && fLkhdExpt))
      return;

    std::cerr << "You tried to invoke method '" << methName << "()' on a "
              << (typ == CHISQ ? "IChiSqExperiment" : "ILkhdExperiment") << " object!" << std::endl;
    abort();
  }

}