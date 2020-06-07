  // Extra map-like logic from WgtMapExtra.h
  Proxy<WgtMapElem>& operator[](unsigned int systIdx)
  {
    if(fElems.size() <= systIdx) fElems.resize(systIdx+1);
    if(fElems[systIdx]) return *fElems[systIdx];

    const std::string name = ana::GetXSecSystName(systIdx);
    fElems[systIdx] = new Proxy<WgtMapElem>(fDir, fTree, "wgt_"+name, fBase, fOffset);
    // Replace the standard fSize using placement new
    fElems[systIdx]->fSize.~Proxy<int>();
    new (&fElems[systIdx]->fSize) Proxy<int>(fDir, fTree, name+"_nshifts", fBase, fOffset);
    return *fElems[systIdx];
  }
