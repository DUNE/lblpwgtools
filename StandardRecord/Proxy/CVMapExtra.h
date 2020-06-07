  // Extra map-like logic from CVMapExtra.h
  const Proxy<double>& operator[](unsigned int systIdx)
  {
    if(fElems.size() <= systIdx) fElems.resize(systIdx+1);
    if(fElems[systIdx]) return *fElems[systIdx];

    const std::string name = ana::GetXSecSystName(systIdx);
    fElems[systIdx] = new Proxy<double>(fDir, fTree, name+"_cvwgt", fBase, fOffset);
    return *fElems[systIdx];
  }
