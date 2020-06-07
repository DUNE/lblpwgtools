#include "SRProxy/BasicTypesProxy.h"

#include "CAFAna/Systs/XSecSystList.h"

#include <string>
#include <iostream>
class TDirectory;
class TTree;

namespace caf
{
  class SRWgtMap
  {
  public:
    SRWgtMap(TDirectory* d, TTree* tr, const std::string& /*name*/, const long& base, int offset) : fDir(d), fTree(tr), fBase(base), fOffset(offset)
    {
    }

    ~SRWgtMap(){for(auto it: fElems) delete it;}

    Proxy<float[100]>& operator[](unsigned int systIdx)
    {
      if(fElems.size() <= systIdx) fElems.resize(systIdx+1);
      if(fElems[systIdx]) return *fElems[systIdx];

      const std::string name = ana::GetXSecSystName(systIdx);
      fElems[systIdx] = new Proxy<float[100]>(fDir, fTree, "wgt_"+name, fBase, fOffset);
      return *fElems[systIdx];
    }

    // Unimplemented for now
    template<class T> SRWgtMap& operator=(const T&){abort();}
    template<class T> void CheckEquals(const T&) const {abort();}

  protected:
    TDirectory* fDir;
    TTree* fTree;
    const long& fBase;
    int fOffset;

    std::vector<Proxy<float[100]>*> fElems;
  };
}

#define xsSyst_wgt UNUSED(){abort();} SRWgtMap xsSyst_wgt;
