#include "CAFAna/Core/Registry.txx"

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/ISyst.h"

namespace ana
{
  // Instantiate the registries we need
  template class Registry<IFitVar>;
  template class Registry<ISyst>;
  template class Registry<FitMultiverse>;
}
