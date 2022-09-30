#include "CAFAna/Analysis/TDRLoaders.h"

#include "unistd.h"

namespace ana
{
  //----------------------------------------------------------------------
  TDRLoaders::TDRLoaders(FluxType flux)
  {
    // FNAL location
    std::string base = "/pnfs/dune/persistent/users/LBL_TDR/CAFs/v4/";

    const std::string hc = (flux == kFHC) ? "FHC" : "RHC";

    SetLoaderPath(base+"ND_"+hc+"_CAF.root",    caf::kNEARDET, Loaders::kMC, Loaders::kNonSwap);

    SetLoaderPath(base+"FD_"+hc+"_nonswap.root", caf::kFARDET, Loaders::kMC, Loaders::kNonSwap);
    SetLoaderPath(base+"FD_"+hc+"_nueswap.root", caf::kFARDET, Loaders::kMC, Loaders::kNueSwap);

    SetLoaderPath(base+"FD_"+hc+"_tauswap.root", caf::kFARDET, Loaders::kMC, Loaders::kNuTauSwap);
  }
}
