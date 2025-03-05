#pragma once

#include "CAFAna/Core/Cut.h"
#include "CAFAna/GAr/GArParams.h"
#include "CAFAna/GAr/GArUtils.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include <vector>

namespace ana
{

    extern const SRCut kIsNoCut; // just a little trick

    const SRCut kIsTrueContained(double delta_l = kHPgTPCFidCutX, double delta_r = kHPgTPCFidCutR);

    extern const SRCut kIsTrueNuMuCC;
    extern const SRCut kIsTrueNuECC;
    extern const SRCut kIsTrueANuMuCC;
    extern const SRCut kIsTrueANuECC;

    extern const SRCut kIsTrueNC;

    extern const SRCut kIsTrueQE;
    extern const SRCut kIsTrueDIS;
    extern const SRCut kIsTrueRES;
    extern const SRCut kIsTrueMEC;
    extern const SRCut kIsTrueOther;

}