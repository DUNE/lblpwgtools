#include "CAFAna/Core/Hist.h"

#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Core/HistCache.h"

#include "Utilities/func/Stan.h"

#include "TH1.h"
#include "THnSparse.h"

#include "TDirectory.h"

namespace ana
{
  //----------------------------------------------------------------------
  Hist::Hist()
    : /*fHistF(0),*/ fHistD(0), fHistSparse(0), fStanVars(0)
  {
  }

  //----------------------------------------------------------------------
  Hist::Hist(const Hist& rhs)
    : Hist()
  {
    DontAddDirectory guard;

    assert(rhs.Initialized());

    if(rhs.fHistD)
      fHistD = HistCache::Copy(rhs.fHistD);

    if(rhs.fHistSparse){
      // Doesn't exist?
      // fHistSparse = new THnSparseD(*rhs.fHistSparse);
      fHistSparse = (THnSparseD*)rhs.fHistSparse->Clone();
    }

    if(rhs.fStanVars) fStanVars = new std::vector<stan::math::var>(*rhs.fStanVars);
  }

  //----------------------------------------------------------------------
  Hist::Hist(Hist&& rhs)
    : Hist()
  {
    assert(rhs.Initialized);

    std::swap(fHistD, rhs.fHistD);
    std::swap(fHistSparse, rhs.fHistSparse);
    std::swap(fStanVars, rhs.fStanVars);
  }

  //----------------------------------------------------------------------
  Hist& Hist::operator=(const Hist& rhs)
  {
    if(this == &rhs) return *this;

    DontAddDirectory guard;

    if(fHistD) HistCache::Delete(fHistD);
    delete fHistSparse;
    delete fStanVars;
    fHistD = 0;
    fHistSparse = 0;
    fStanVars = 0;

    assert(rhs.Initialized());

    if(rhs.fHistD){
      fHistD = HistCache::Copy(rhs.fHistD);
    }

    if(rhs.fHistSparse){
      fHistSparse = (THnSparseD*)rhs.fHistSparse->Clone();
    }

    if(rhs.fStanVars){
      fStanVars = new std::vector<stan::math::var>(*rhs.fStanVars);
    }

    return *this;
  }

  //----------------------------------------------------------------------
  Hist& Hist::operator=(Hist&& rhs)
  {
    if(this == &rhs) return *this;

    assert(rhs.Initialized());

    std::swap(fHistD, rhs.fHistD);
    std::swap(fHistSparse, rhs.fHistSparse);
    std::swap(fStanVars, rhs.fStanVars);

    return *this;
  }

  //----------------------------------------------------------------------
  Hist::~Hist()
  {
    if(fHistD && fHistD->GetDirectory()){
      static bool once = true;
      if(once){
        once = false;
        std::cerr << "Hist's fHistD (" << fHistD << ") is associated with a directory (" << fHistD->GetDirectory() << ". How did that happen?" << std::endl;
      }
    }

    if(fHistD) HistCache::Delete(fHistD);

    delete fHistSparse;

    delete fStanVars;
  }

  //----------------------------------------------------------------------
  Hist Hist::Copy(TH1D* h)
  {
    Hist ret;
    ret.fHistD = HistCache::Copy(h);
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::Copy(TH1* h)
  {
    Hist ret;
    ret.fHistD = HistCache::New("", h->GetXaxis());
    ret.fHistD->Add(h);
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::Adopt(std::unique_ptr<TH1D> h)
  {
    Hist ret;
    ret.fHistD = h.release();
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::Adopt(std::unique_ptr<THnSparseD> h)
  {
    Hist ret;
    ret.fHistSparse = h.release();
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::Adopt(std::vector<stan::math::var>&& v)
  {
    Hist ret;
    ret.fStanVars = new std::vector<stan::math::var>(std::move(v));
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::FromDirectory(TDirectory* dir)
  {
    Hist ret;
    ret.fHistD = (TH1D*)dir->Get("hist");
    ret.fHistSparse = (THnSparseD*)dir->Get("hist_sparse");
    assert(ret.fHistD || ret.fHistSparse);
    return ret;
  }

  //----------------------------------------------------------------------
  TH1D* Hist::ToTH1(const Binning& bins) const
  {
    assert(Initialized());

    if(fHistD) return HistCache::Copy(fHistD, bins);

    if(fHistSparse) return fHistSparse->Projection(0);

    if(fStanVars){
      TH1D* ret = HistCache::New("", bins);
      for(int i = 0; i < bins.NBins()+2; ++i) ret->SetBinContent(i, (*fStanVars)[i].val());
      return ret;
    }

    abort(); // unreachable
  }

  //----------------------------------------------------------------------
  int Hist::GetNbinsX() const
  {
    assert(Initialized());

    if(fHistD) return fHistD->GetNbinsX();
    if(fHistSparse) return fHistSparse->GetAxis(0)->GetNbins();
    if(fStanVars) return fStanVars->size();

    abort(); // unreachable
  }

  //----------------------------------------------------------------------
  double Hist::GetBinError(int i) const
  {
    assert(Initialized());

    if(fHistD) return fHistD->GetBinError(i);
    if(fHistSparse) return fHistSparse->GetBinError(i);

    return 0; // TODO is this the right thing in stan case or should we abort?
  }

  //----------------------------------------------------------------------
  double Hist::Integral(int lo, int hi) const
  {
    assert(fHistD); // TODO how to use fHistSparse or stan?
    return fHistD->Integral(lo, hi);
  }

  //----------------------------------------------------------------------
  double Hist::GetMean() const
  {
    assert(fHistD); // TODO how to use fHistSparse or stan?

    // Allow GetMean() to work even if this histogram never had any explicit
    // Fill() calls made.
    if(fHistD->GetEntries() == 0) fHistD->SetEntries(1);
    return fHistD->GetMean();
  }

  //----------------------------------------------------------------------
  void Hist::Fill(double x, double w)
  {
    assert(Initialized());

    if(fHistD){
      fHistD->Fill(x, w);
    }
    else if(fHistSparse){
      fHistSparse->Fill(&x, w);
    }
    else if(fStanVars){
      std::cout << "Hist::Fill() not supported for stan vars" << std::endl;
      abort();
    }
    else{
      abort(); // unreachable
    }
  }

  //----------------------------------------------------------------------
  void Hist::Scale(double s)
  {
    assert(Initialized());
    if(fHistD) fHistD->Scale(s);
    if(fHistSparse) fHistSparse->Scale(s);
    if(fStanVars) for(stan::math::var& v: *fStanVars) v *= s;
  }

  //----------------------------------------------------------------------
  void Hist::ResetErrors()
  {
    if(fHistD){
      fHistD->Sumw2(false);
      fHistD->Sumw2();
    }
  }

  //----------------------------------------------------------------------
  double Hist::GetBinContent(int i) const
  {
    assert(Initialized());

    if(fHistD) return fHistD->GetBinContent(i);
    if(fHistSparse) return fHistSparse->GetBinContent(i);
    if(fStanVars) return (*fStanVars)[i].val();

    abort(); // unreachable
  }

  //----------------------------------------------------------------------
  void Hist::SetBinContent(int i, double x)
  {
    assert(Initialized());

    if(fHistD) fHistD->SetBinContent(i, x);
    if(fHistSparse) fHistSparse->SetBinContent(i, x);
    if(fStanVars){
      std::cout << "Hist::SetBinContent() not implemented for stan vars" << std::endl;
      abort();
    }

    abort(); // unreachable
  }

  //----------------------------------------------------------------------
  void Hist::Reset()
  {
    if(fHistD) fHistD->Reset();
    if(fHistSparse) fHistSparse->Reset();
    if(fStanVars) for(stan::math::var& v: *fStanVars) v = 0;
  }

  //----------------------------------------------------------------------
  void Hist::Add(const TH1D* rhs, double scale)
  {
    if(fHistD) fHistD->Add(rhs, scale);
    if(fHistSparse) fHistSparse->Add(rhs, scale);
    if(fStanVars){
      for(unsigned int i = 0; i < fStanVars->size(); ++i){
        (*fStanVars)[i] += rhs->GetBinContent(i);
      }
    }
  }

  //----------------------------------------------------------------------
  void Hist::Add(const THnSparseD* rhs, double scale)
  {
    if(fHistD){
      for(int i = 0; i < fHistD->GetNbinsX()+2; ++i){
        fHistD->SetBinContent(i, fHistD->GetBinContent(i) + rhs->GetBinContent(i) * scale);
      }
    }
    if(fHistSparse) fHistSparse->Add(rhs, scale);
    if(fStanVars){
      for(unsigned int i = 0; i < fStanVars->size(); ++i){
        (*fStanVars)[i] += rhs->GetBinContent(i);
      }
    }
  }

  //----------------------------------------------------------------------
  void Hist::Add(const std::vector<stan::math::var>* rhs, double scale)
  {
    if(fHistD){
      fStanVars = new std::vector<stan::math::var>(rhs->size(), 0);
      for(unsigned int i = 0; i < fStanVars->size(); ++i){
        (*fStanVars)[i] = fHistD->GetBinContent(i) + (*rhs)[i] * scale;
      }
      HistCache::Delete(fHistD);
    }

    if(fHistSparse){
      fStanVars = new std::vector<stan::math::var>(rhs->size(), 0);
      for(unsigned int i = 0; i < fStanVars->size(); ++i){
        (*fStanVars)[i] = fHistSparse->GetBinContent(i) + (*rhs)[i] * scale;
      }
      delete fHistSparse;
      fHistSparse = 0;
    }

    if(fStanVars){
      for(unsigned int i = 0; i < fStanVars->size(); ++i){
        (*fStanVars)[i] += (*rhs)[i];
      }
    }
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Hist& rhs, double scale)
  {
    assert(Initialized());
    assert(rhs.Initialized());

    if(rhs.fHistD)      Add(rhs.fHistD,      scale);
    if(rhs.fHistSparse) Add(rhs.fHistSparse, scale);
    if(rhs.fStanVars)   Add(rhs.fStanVars,   scale);
  }

  //----------------------------------------------------------------------
  void Hist::Multiply(TH1D* rhs)
  {
    if(fHistD) fHistD->Multiply(rhs); else abort();//fHistSparse->Multiply(rhs);
  }

  //----------------------------------------------------------------------
  void Hist::Divide(TH1D* rhs)
  {
    if(fHistD) fHistD->Divide(rhs); else abort();//fHistSparse->Divide(rhs);
  }

  //----------------------------------------------------------------------
  void Hist::Write() const
  {
    assert(Initialized());

    if(fHistD) fHistD->Write("hist");
    if(fHistSparse) fHistSparse->Write("hist_sparse");
    if(fStanVars){
      std::cout << "Hist::Write() not implemented (impossible?) for stan vars" << std::endl;
      abort();
    }
  }
}
