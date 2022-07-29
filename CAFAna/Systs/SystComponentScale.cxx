#include "CAFAna/Systs/SystComponentScale.h"

#include "CAFAna/Systs/DUNEXSecSysts.h"

#include "TDirectory.h"
#include "TObjString.h"

#include <cmath>

namespace ana
{
  //----------------------------------------------------------------------
  SystComponentScale::~SystComponentScale()
  {
  }

  //----------------------------------------------------------------------
  void SystComponentScale::Shift(double sigma,
                                 caf::SRProxy* sr,
                                 double& weight) const
  {
    if(!fCut(sr)) return;

    if(fType == kExponential){
      weight *= pow(1+fOneSigma, sigma);
    }
    else{
      weight *= 1+sigma*fOneSigma;
      weight = std::max(0., weight);
    }
  }

  //----------------------------------------------------------------------
  std::unique_ptr<SystComponentScale> SystComponentScale::LoadFrom(TDirectory* dir, const std::string& name)
  {
    TObjString* ptag = (TObjString*)dir->Get((name+"/type").c_str());
    assert(ptag);

    const TString tag = ptag->GetString();
    delete ptag;

    if(tag == "DUNEXSecSyst") return DUNEXSecSyst::LoadFrom(dir, name);

    std::cerr << "Unknown SystComponentScale type '" << tag << "'" << std::endl;
    abort();
  }
}
