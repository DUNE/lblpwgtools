#include "CAFAna/GAr/GArTruthCuts.h"

namespace ana
{

    const SRCut kIsNoCut([](const caf::SRProxy* sr) {
        if(sr->mc.nnu == 0) return false;
        assert(sr->mc.nnu == 1);
        return true;
    });

    const SRCut kIsTrueContained(double delta_l, double delta_r){
        const SRCut kIsThisTrueContained([delta_l, delta_r](const caf::SRProxy* sr) {
            if(sr->mc.nnu == 0) return false;
            assert(sr->mc.nnu == 1);
            const std::vector<double> vtx = fGetVectorStd(&sr->mc.nu[0].vtx);
            bool contained = fContainedInFiducial(vtx, delta_l, delta_r);
            return contained;
        });

        return kIsThisTrueContained;
    }

    const SRCut kIsTrueNuMuCC([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    int pdg = sr->mc.nu[0].pdg;
                    bool cc = sr->mc.nu[0].iscc;
                    return (pdg==14)&&cc;
    });

    const SRCut kIsTrueNuECC([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    int pdg = sr->mc.nu[0].pdg;
                    bool cc = sr->mc.nu[0].iscc;
                    return (pdg==12)&&cc;
    });

    const SRCut kIsTrueANuMuCC([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    int pdg = sr->mc.nu[0].pdg;
                    bool cc = sr->mc.nu[0].iscc;
                    return (pdg==-14)&&cc;
    });

    const SRCut kIsTrueANuECC([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    int pdg = sr->mc.nu[0].pdg;
                    bool cc = sr->mc.nu[0].iscc;
                    return (pdg==-12)&&cc;
    });

    const SRCut kIsTrueNC([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    bool cc = sr->mc.nu[0].iscc;
                    return !cc;
    });

    const SRCut kIsTrueQE([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    return sr->mc.nu[0].mode == caf::kQE;
    });

    const SRCut kIsTrueDIS([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    return sr->mc.nu[0].mode == caf::kDIS;
    });

    const SRCut kIsTrueRES([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    return sr->mc.nu[0].mode == caf::kRes;
    });

    const SRCut kIsTrueMEC([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    return sr->mc.nu[0].mode == caf::kMEC;
    });

    const SRCut kIsTrueOther([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    caf::ScatteringMode mode = sr->mc.nu[0].mode;
                    return (mode != caf::kQE)&&(mode != caf::kDIS)&&(mode != caf::kRes)&&(mode != caf::kMEC);
    });

}