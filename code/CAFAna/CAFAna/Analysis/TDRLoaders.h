#pragma once

#include "CAFAna/Core/Loaders.h"

namespace ana
{
  /// Standardized filenames for TDR analysis
  class TDRLoaders: public Loaders
  {
  public:
    TDRLoaders(FluxType flux);
  };
}
