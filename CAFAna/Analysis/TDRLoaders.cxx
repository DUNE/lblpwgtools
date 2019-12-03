#include "CAFAna/Analysis/TDRLoaders.h"

#include "unistd.h"

namespace ana
{
  //----------------------------------------------------------------------
  TDRLoaders::TDRLoaders(FluxType flux)
  {
    // FNAL location
    std::string base = "/pnfs/dune/persistent/users/LBL_TDR/CAFs/";

    // UCL location
    char host[1024];
    gethostname(host, 1023);
    if(std::string(host).find(".ucl.") != std::string::npos)
      base = "/unix/dune/CAFs/";

    base += "/v4/";

    const std::string hc = (flux == kFHC) ? "FHC" : "RHC";

    SetLoaderPath(base+"ND_"+hc+"_CAF.root",    caf::kNEARDET, Loaders::kMC, ana::kBeam, Loaders::kNonSwap);

    SetLoaderPath(base+"FD_"+hc+"_nonswap.root", caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNonSwap);
    SetLoaderPath(base+"FD_"+hc+"_nueswap.root", caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNueSwap);

    SetLoaderPath(base+"FD_"+hc+"_tauswap.root", caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNuTauSwap);
  }
}
