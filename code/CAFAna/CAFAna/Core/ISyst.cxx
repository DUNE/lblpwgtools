#include "CAFAna/Core/ISyst.h"

#include "CAFAna/Core/SystRegistry.h"

namespace ana
{
  //----------------------------------------------------------------------
  ISyst::ISyst(const std::string& shortName,
               const std::string& latexName,
	       bool applyPenalty,
	       double min,
	       double max)
    : fShortName(shortName), fLatexName(latexName), fApplyPenalty(applyPenalty), fMin(min), fMax(max)
  {
    SystRegistry::Register(this);
  }

  //----------------------------------------------------------------------
  ISyst::~ISyst()
  {
    // Normally ISysts should last for the life of the process, but in case one
    // is deleted it's best not to leave a dangling pointer in SystRegistry.
    SystRegistry::UnRegister(this);
  }
}
