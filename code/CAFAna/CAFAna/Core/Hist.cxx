#include "CAFAna/Core/Hist.h"

#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Core/HistCache.h"

#include "TH1.h"
#include "THnSparse.h"

#include "TDirectory.h"

namespace ana
{
  //----------------------------------------------------------------------
  Hist::Hist()
    : /*fHistF(0),*/ fHistD(0), fHistSparse(0), fBins(Binning::Simple(0, 0, 0))
  {
  }

  //----------------------------------------------------------------------
  Hist::Hist(const Hist& rhs)
    : Hist()
  {
    DontAddDirectory guard;

    fBins = rhs.fBins;

    assert(rhs.fHistD || rhs.fHistSparse);
    if(rhs.fHistD)
      fHistD = HistCache::Copy(rhs.fHistD, rhs.fBins);
    if(rhs.fHistSparse){
      // Doesn't exist?
      // fHistSparse = new THnSparseD(*rhs.fHistSparse);
      fHistSparse = (THnSparseD*)rhs.fHistSparse->Clone();
    }
  }

  //----------------------------------------------------------------------
  Hist::Hist(Hist&& rhs)
    : Hist()
  {
    assert(rhs.fHistD || rhs.fHistSparse);

    std::swap(fHistD, rhs.fHistD);
    std::swap(fHistSparse, rhs.fHistSparse);
    std::swap(fBins, rhs.fBins);
  }

  //----------------------------------------------------------------------
  Hist& Hist::operator=(const Hist& rhs)
  {
    if(this == &rhs) return *this;

    DontAddDirectory guard;

    if(fHistD) HistCache::Delete(fHistD, fBins.ID());
    delete fHistSparse;

    assert(rhs.fHistD || rhs.fHistSparse);

    fBins = rhs.fBins;

    if(rhs.fHistD){
      fHistD = HistCache::Copy(rhs.fHistD, rhs.fBins);
      fHistSparse = 0;
    }

    if(rhs.fHistSparse){
      fHistSparse = (THnSparseD*)rhs.fHistSparse->Clone();
      fHistD = 0;
    }

    return *this;
  }

  //----------------------------------------------------------------------
  Hist& Hist::operator=(Hist&& rhs)
  {
    if(this == &rhs) return *this;

    assert(rhs.fHistD || rhs.fHistSparse);

    std::swap(fHistD, rhs.fHistD);
    std::swap(fHistSparse, rhs.fHistSparse);
    std::swap(fBins, rhs.fBins);

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

    if(fHistD) HistCache::Delete(fHistD, fBins.ID());

    delete fHistSparse;
  }

  //----------------------------------------------------------------------
  Hist Hist::Copy(TH1D const* h, const Binning& bins)
  {
    Hist ret;
    ret.fHistD = HistCache::Copy(h);
    ret.fBins = bins;
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::Copy(TH1 const* h, const Binning& bins)
  {
    Hist ret;
    ret.fHistD = HistCache::New("", h->GetXaxis());
    ret.fHistD->Add(h);
    ret.fBins = bins;
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::Adopt(std::unique_ptr<TH1D> h, const Binning& bins)
  {
    Hist ret;
    ret.fHistD = h.release();
    ret.fBins = bins;
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::Adopt(std::unique_ptr<THnSparseD> h, const Binning& bins)
  {
    Hist ret;
    ret.fHistSparse = h.release();
    ret.fBins = bins;
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::FromDirectory(TDirectory* dir, const Binning& bins)
  {
    Hist ret;
    ret.fHistD = (TH1D*)dir->Get("hist");
    ret.fHistSparse = (THnSparseD*)dir->Get("hist_sparse");
    assert(ret.fHistD || ret.fHistSparse);
    ret.fBins = bins;
    return ret;
  }

  //----------------------------------------------------------------------
  TH1D* Hist::ToTH1() const
  {
    TH1D* ret = 0;
    if(fHistD){
      ret = HistCache::Copy(fHistD, fBins);
    }
    else{
      ret = fHistSparse->Projection(0);
    }
    return ret;
  }

  //----------------------------------------------------------------------
  int Hist::GetNbinsX() const
  {
    return fHistD ? fHistD->GetNbinsX() : fHistSparse->GetAxis(0)->GetNbins();
  }

  //----------------------------------------------------------------------
  double Hist::Integral(int lo, int hi) const
  {
    assert(fHistD); // TODO how to use fHistSparse?
    return fHistD->Integral(lo, hi);
  }

  //----------------------------------------------------------------------
  double Hist::GetMean() const
  {
    assert(fHistD); // TODO how to use fHistSparse?

    // Allow GetMean() to work even if this histogram never had any explicit
    // Fill() calls made.
    if(fHistD->GetEntries() == 0) fHistD->SetEntries(1);
    return fHistD->GetMean();
  }

  //----------------------------------------------------------------------
  void Hist::Fill(double x, double w)
  {
    assert( (fHistD || fHistSparse) && "Somehow both fHist Dand fHistSparse are null in Hist::Fill" );

    if(fHistD)
      fHistD->Fill(x, w);
    else if(fHistSparse)
      fHistSparse->Fill(&x, w);
  }

  //----------------------------------------------------------------------
  void Hist::Scale(double s)
  {
    if(fHistD) fHistD->Scale(s); else fHistSparse->Scale(s);
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
    if(fHistD) return fHistD->GetBinContent(i); else return fHistSparse->GetBinContent(i);
  }

  //----------------------------------------------------------------------
  void Hist::SetBinContent(int i, double x)
  {
    if(fHistD) fHistD->SetBinContent(i, x); else fHistSparse->SetBinContent(i, x);
  }

  //----------------------------------------------------------------------
  double Hist::GetBinError(int i) const
  {
    if(fHistD) return fHistD->GetBinError(i); else return fHistSparse->GetBinError(i);
  }

  //----------------------------------------------------------------------
  void Hist::SetBinError(int i, double x)
  {
    if(fHistD) fHistD->SetBinError(i, x); else fHistSparse->SetBinError(i, x);
  }

  //----------------------------------------------------------------------
  void Hist::Reset()
  {
    if(fHistD) fHistD->Reset();
    if(fHistSparse) fHistSparse->Reset();
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Hist& rhs, double scale)
  {
    if(fHistD){
      if(rhs.fHistD){
        fHistD->Add(rhs.fHistD, scale);
      }
      else{
        abort();
        //        fHistD->Add(rhs.fHistSparse, scale);
      }
    }
    else{
      if(rhs.fHistD){
        fHistSparse->Add(rhs.fHistD, scale);
      }
      else{
        fHistSparse->Add(rhs.fHistSparse, scale);
      }
    }
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
    if(fHistD) fHistD->Write("hist");
    if(fHistSparse) fHistSparse->Write("hist_sparse");
  }
}
