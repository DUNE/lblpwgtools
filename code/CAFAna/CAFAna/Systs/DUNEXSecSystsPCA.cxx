#include "CAFAna/Systs/DUNEXSecSystsPCA.h"

#include "TFile.h"
#include "TH1.h"
#include "TObjString.h"

namespace ana
{
  //----------------------------------------------------------------------
  DUNEXSecSystPCA::DUNEXSecSystPCA(int idx) :
    ISyst(TString::Format("xsecpca%d", fIdx).Data(),
          TString::Format("Cross section principal component %d", fIdx).Data()),
    fIdx(idx)
  {
    TFile f((FindCAFAnaDir()+"Systs/xsec_shifts.root").c_str());
    assert(!f.IsZombie());

    TH1* h = (TH1*)f.Get(TString::Format("syst%d", idx).Data());
    assert(h);
    for(int i = 0; i < nuenumu_dummy; ++i) fCoeffs[i] = h->GetBinContent(i+1);
  }

  //----------------------------------------------------------------------
  void DUNEXSecSystPCA::Shift(double sigma,
                              Restorer& restore,
                              caf::StandardRecord* sr,
                              double& weight) const
  {
    weight *= 1+fCoeffs[GetVALORCategory(sr)]*sigma;

    weight = std::max(0., weight);
  }

  //----------------------------------------------------------------------
  const DUNEXSecSystPCA* GetDUNEXSecSystPCA(unsigned int i)
  {
    // Make sure we always give the same one back
    static std::vector<const DUNEXSecSystPCA*> cache;
    if(i >= cache.size()) cache.resize(i+1);
    if(!cache[i]) cache[i] = new DUNEXSecSystPCA(i);
    return cache[i];
  }

  //----------------------------------------------------------------------
  SystVector<DUNEXSecSystPCA> GetDUNEXSecSystsPCA(int N)
  {
    SystVector<DUNEXSecSystPCA> ret;
    for(int i = 0; i < N; ++i) ret.push_back(GetDUNEXSecSystPCA(i));
    return ret;
  }
}
