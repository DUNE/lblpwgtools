#include "CAFAna/Cuts/TruthCuts.h"
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include <cassert>
// Just make sure that the cuts get built on a regular basis. Could contain
// more complex cut logic in the future.
namespace ana 
{

// NOvA cuts
  const Cut kIsNC([](const caf::SRProxy* sr)
                  {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    return !sr->mc.nu[0].iscc;
                  });

  const Cut kIsNue([](const caf::SRProxy* sr){
         return (sr->mc.nnu == 1 && abs(sr->mc.nu[0].pdg) ==12);});

  const Cut kIsNumu([](const caf::SRProxy* sr){
         return (sr->mc.nnu == 1 && abs(sr->mc.nu[0].pdg) ==14);});


  bool CCFlavSel::operator()(const caf::SRProxy* sr) const
  {
    if(sr->mc.nnu == 0) return false;
    assert(sr->mc.nnu == 1);
    return (sr->mc.nu[0].iscc &&
            abs(sr->mc.nu[0].pdg) == fPdg &&
            abs(sr->mc.nu[0].pdgorig) == fPdgOrig);
  }


  const Cut kIsAntiNu([](const caf::SRProxy* sr)
                      {
                        if(sr->mc.nnu == 0) return false;
                        assert(sr->mc.nnu == 1);
                        return sr->mc.nu[0].pdg < 0;
                      });

  const Cut kIsNu([](const caf::SRProxy* sr)
                      {
                        if(sr->mc.nnu == 0) return false;
                        assert(sr->mc.nnu == 1);
                        return sr->mc.nu[0].pdg > 0;
                      });

  const Cut kHasNu([](const caf::SRProxy* sr)
                      {
                        if(sr->mc.nnu == 0) return false;
                        assert(sr->mc.nnu == 1);
                        return true;
                      });

  const Cut kHasNeutrino(
    [](const caf::SRProxy* sr)
      { return (sr->mc.nnu != 0); }
  );


}