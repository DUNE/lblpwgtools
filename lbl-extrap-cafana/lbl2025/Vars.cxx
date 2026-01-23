#include "lbl2025/Vars.h"

namespace lbl2025
{
  const ana::Var kRecoEnu_lepCalo([](const caf::SRInteractionProxy* sri) {
      std::cout << "I AM HERE" << std::endl;
      std::cout << sri->Enu.calo << std::endl;
      return sri->Enu.calo;
      });
}
