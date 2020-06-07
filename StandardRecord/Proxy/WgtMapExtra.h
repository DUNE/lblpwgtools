  // Extra map-like logic from WgtMapExtra.h
  Proxy<double[100]>& operator[](unsigned int systIdx)
  {
    if(fElems.size() <= systIdx) fElems.resize(systIdx+1);
    if(fElems[systIdx]) return *fElems[systIdx];

    const std::string name = ana::GetXSecSystName(systIdx);
    fElems[systIdx] = new Proxy<double[100]>(fDir, fTree, "wgt_"+name, fBase, fOffset);
    return *fElems[systIdx];
  }
