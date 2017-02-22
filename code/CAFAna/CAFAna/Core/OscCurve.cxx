#include "CAFAna/Core/OscCurve.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Utilities.h"

#include "OscLib/func/IOscCalculator.h"

#include <cassert>
#include <iostream>
#include <map>

#include "TH1.h"

namespace ana
{
  //----------------------------------------------------------------------
  OscCurve::OscCurve(osc::IOscCalculator* calc, int from, int to)
  {
    DontAddDirectory guard;

    fHist = new TH1D("", ";True Energy (GeV);Probability",
                     kTrueEnergyBins.NBins(), &kTrueEnergyBins.Edges()[0]);

    for(int i = 0; i < fHist->GetNbinsX()+2; ++i){
      const double E = fHist->GetBinCenter(i);
      if(E > 0){
        fHist->SetBinContent(i, calc->P(from, to, E));
      }
      else{
        fHist->SetBinContent(i, (from == to) ? 1 : 0);
      }
      fHist->SetBinError(i, 0);
    }

    std::map<int, std::string> nus;
    nus[12] = nus[-12] = "e";
    nus[14] = nus[-14] = "#mu";
    nus[16] = nus[-16] = "#tau";
    nus[0] = "active";
    const std::string nu = (from > 0) ? "#nu" : "#bar#nu";

    fHist->SetTitle((nu+"_{"+nus[from]+"}#rightarrow"+nu+"_{"+nus[to]+"}").c_str());
  }

  //----------------------------------------------------------------------
  OscCurve::OscCurve(TH1* h)
  {
    DontAddDirectory guard;

    const TString className = h->ClassName();

    if(className == "TH1D"){
      // Shortcut if types match
      fHist = new TH1D(*((TH1D*)h));
    }
    else{
      const TAxis* ax = h->GetXaxis();
      fHist = new TH1D("", "", ax->GetNbins(), ax->GetXbins()->GetArray());
      fHist->Add(h);
    }

    fHist->SetTitle(";True Energy (GeV);Probability");
  }

  //----------------------------------------------------------------------
  OscCurve::~OscCurve()
  {
    if(fHist && fHist->GetDirectory()){
      static bool once = true;
      if(once){
        once = false;
        std::cerr << "OscCurve's fHist is associated with a directory. How did that happen?" << std::endl;
      }
    }

    delete fHist;
  }

  //----------------------------------------------------------------------
  OscCurve::OscCurve(const OscCurve& rhs)
  {
    DontAddDirectory guard;

    assert(rhs.fHist);
    fHist = new TH1D(*rhs.fHist);
  }

  //----------------------------------------------------------------------
  OscCurve& OscCurve::operator=(const OscCurve& rhs)
  {
    if(&rhs == this) return *this;

    DontAddDirectory guard;

    delete fHist;
    assert(rhs.fHist);
    fHist = new TH1D(*rhs.fHist);
    return *this;
  }

  //----------------------------------------------------------------------
  TH1* OscCurve::ToTH1() const
  {
    // Could have a file temporarily open
    DontAddDirectory guard;

    return new TH1D(*fHist);
  }
}
