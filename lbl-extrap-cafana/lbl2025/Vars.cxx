#include "lbl2025/Vars.h"

#include <iostream>
#include <ostream>

namespace lbl2025
{
  const ana::Var kRecoEnu_lepCalo([](const caf::SRInteractionProxy* sri) {
      // FIXME: this needs to be fixed when Pandora CAFs have GeV rather than MeV
      const caf::SRProxy * sr = sri->Ancestor<caf::SRProxy>();
      auto val = sri->Enu.calo;
      if ( !(sr->meta.fd_hd.enabled || sr->meta.fd_vd.enabled)  )
        // std::cout << "    Enu.calo = " << sri->Enu.calo << std::endl;
        val *= 1e-3;
      return val;
      });
}
