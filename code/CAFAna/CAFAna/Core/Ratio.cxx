#include "CAFAna/Core/Ratio.h"

#include "CAFAna/Core/Utilities.h"

#include "TH1.h"

#include <cassert>

namespace ana
{
  //----------------------------------------------------------------------
  Ratio::Ratio(const Spectrum& num, const Spectrum& denom,
	       bool purOrEffErrs)
  {
    // Scale to same arbitrary POT
    fHist = num.ToTH1(1e20);
    const TH1D* temp = denom.ToTH1(1e20);
    if(purOrEffErrs){
      fHist->Divide(fHist, temp, 1, 1, "B");
    }
    else{
      fHist->Divide(temp);
    }
    delete temp;

    fHist->GetYaxis()->SetTitle("Ratio");

    // TODO: set error bars smartly
  }

  //----------------------------------------------------------------------
  Ratio::Ratio(TH1* h, std::string varName)
  {
    if(!h){
      fHist = 0;
      return;
    }

    DontAddDirectory guard;

    const TString className = h->ClassName();

    if(className == "TH1D"){
      // Shortcut if types match
      fHist = new TH1D(*((TH1D*)h));
    }
    else{
      const TAxis* ax = h->GetXaxis();
      if(ax->GetXbins()->GetArray()){
        fHist = new TH1D(UniqueName().c_str(), "",
                         ax->GetNbins(), ax->GetXbins()->GetArray());
      }
      else{
        fHist = new TH1D(UniqueName().c_str(), "",
                         ax->GetNbins(), ax->GetXmin(), ax->GetXmax());
      }
      fHist->GetXaxis()->SetTitle(ax->GetTitle());
      fHist->Add(h);
    }

    if(!varName.empty()) fHist->GetXaxis()->SetTitle(varName.c_str());
  }

  //----------------------------------------------------------------------
  Ratio::~Ratio()
  {
    delete fHist;
  }

  //----------------------------------------------------------------------
  Ratio::Ratio(const Ratio& rhs)
  {
    DontAddDirectory guard;

    assert(rhs.fHist);
    fHist = new TH1D(*rhs.fHist);
  }

  //----------------------------------------------------------------------
  Ratio& Ratio::operator=(const Ratio& rhs)
  {
    if(this == &rhs) return *this;

    DontAddDirectory guard;

    delete fHist;
    assert(rhs.fHist);
    fHist = new TH1D(*rhs.fHist);
    return *this;
  }

  //----------------------------------------------------------------------
  Ratio& Ratio::operator*=(const Ratio& rhs)
  {
    fHist->Multiply(rhs.fHist);
    return *this;
  }

  //----------------------------------------------------------------------
  Ratio Ratio::operator*(const Ratio& rhs) const
  {
    Ratio ret = *this;
    ret *= rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  Ratio& Ratio::operator/=(const Ratio& rhs)
  {
    fHist->Divide(rhs.fHist);
    return *this;
  }

  //----------------------------------------------------------------------
  Ratio Ratio::operator/(const Ratio& rhs) const
  {
    Ratio ret = *this;
    ret /= rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  TH1D* Ratio::ToTH1(Color_t col, Style_t style) const
  {
    // Could have a file temporarily open
    DontAddDirectory guard;

    TH1D* ret = new TH1D(*fHist);
    ret->SetLineColor(col);
    ret->SetLineStyle(style);
    return ret;
  }
} // namespace
