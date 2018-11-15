#include "CAFAna/Analysis/TDRLoaders.h"

namespace ana
{
  //----------------------------------------------------------------------
  TDRLoaders::TDRLoaders(FluxType flux)
  {
    const std::string base = "/dune/data/users/marshalc/CAFs/mcc11_test/";

    const std::string hc = (flux == kFHC) ? "FHC" : "RHC";

    SetLoaderPath(base+"ND_"+hc+"_CAF.root",    caf::kNEARDET, Loaders::kMC, ana::kBeam, Loaders::kNonSwap);

    SetLoaderPath(base+"FD_"+hc+"_nonswap.root", caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNonSwap);
    SetLoaderPath(base+"FD_"+hc+"_nueswap.root", caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNueSwap);

    // Don't exist yet
    //    SetLoaderPath(base+"FD_"+hc+"_tauswap.root", caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNuTauSwap);
  }
}
