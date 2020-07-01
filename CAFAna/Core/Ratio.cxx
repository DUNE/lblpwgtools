#include "CAFAna/Core/Ratio.h"

#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/Utilities.h"

#include "TH1.h"

#include <cassert>

namespace ana
{
  //----------------------------------------------------------------------
  Ratio::Ratio(const Spectrum& num, const Spectrum& denom,
	       bool purOrEffErrs)
    : fHist(num.fHist), fBins(num.GetBinnings()), fLabels(num.GetLabels())
  {
    fHist.Divide(denom.fHist);
    fHist.Scale(denom.POT()/num.POT());

    // This is clumsy, but the old histogram operation considered 0/0 = 0,
    // which is actually pretty useful (at least PredictionInterp relies on
    // this).
    for(int i = 0; i < fHist.GetNbinsX()+2; ++i){
      if(num.fHist.GetBinContent(i) == 0 &&
         denom.fHist.GetBinContent(i) == 0){
        fHist.SetBinContent(i, 0);
      }
    }

    // TODO do something with purOrEffErrs
  }

  //----------------------------------------------------------------------
  Ratio::~Ratio()
  {
  }

  //----------------------------------------------------------------------
  Ratio& Ratio::operator*=(const Ratio& rhs)
  {
    fHist.Multiply(rhs.fHist);
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
    fHist.Divide(rhs.fHist);
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
    TH1D* ret = fHist.ToTH1(fBins[0]); // TODO
    ret->SetLineColor(col);
    ret->SetLineStyle(style);
    ret->GetXaxis()->SetTitle(fLabels[0].c_str()); // TODO
    ret->GetYaxis()->SetTitle("Ratio");
    return ret;
  }
} // namespace
