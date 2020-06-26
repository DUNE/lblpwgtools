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
    : /*fHistF(0),*/ fHistD(0), fHistSparse(0), fEigenStan(0), fEigen(0)
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

    if(rhs.fEigenStan) fEigenStan = new Eigen::ArrayXstan(*rhs.fEigenStan);

    if(rhs.fEigen) fEigen = new Eigen::ArrayXd(*rhs.fEigen);
  }

  //----------------------------------------------------------------------
  Hist::Hist(Hist&& rhs)
    : Hist()
  {
    assert(rhs.Initialized);

    std::swap(fHistD, rhs.fHistD);
    std::swap(fHistSparse, rhs.fHistSparse);
    std::swap(fEigenStan, rhs.fEigenStan);
    std::swap(fEigen, rhs.fEigen);
  }

  //----------------------------------------------------------------------
  Hist& Hist::operator=(const Hist& rhs)
  {
    if(this == &rhs) return *this;

    DontAddDirectory guard;

    if(fHistD) HistCache::Delete(fHistD);
    delete fHistSparse;
    delete fEigenStan;
    fHistD = 0;
    fHistSparse = 0;
    fEigenStan = 0;

    assert(rhs.Initialized());

    if(rhs.fHistD){
      fHistD = HistCache::Copy(rhs.fHistD);
    }

    if(rhs.fHistSparse){
      fHistSparse = (THnSparseD*)rhs.fHistSparse->Clone();
    }

    if(rhs.fEigenStan){
      fEigenStan = new Eigen::ArrayXstan(*rhs.fEigenStan);
    }

    if(rhs.fEigen){
      fEigen = new Eigen::ArrayXd(*rhs.fEigen);
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
    std::swap(fEigenStan, rhs.fEigenStan);
    std::swap(fEigen, rhs.fEigen);

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

    delete fEigenStan;

    delete fEigen;
  }

  //----------------------------------------------------------------------
  Hist Hist::Copy(const TH1D* h)
  {
    Hist ret;
    ret.fHistD = HistCache::Copy(h);
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::Copy(const TH1* h)
  {
    Hist ret;
    ret.fHistD = HistCache::New("", h->GetXaxis());
    ret.fHistD->Add(h);
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::Copy(const std::vector<stan::math::var>& v)
  {
    Hist ret;
    ret.fEigenStan = new Eigen::ArrayXstan(v.size());
    for(unsigned int i = 0; i < v.size(); ++i) (*ret.fEigenStan)[i] = v[i];
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
  Hist Hist::Adopt(Eigen::ArrayXstan&& v)
  {
    Hist ret;
    ret.fEigenStan = new Eigen::ArrayXstan(std::move(v));
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::Adopt(Eigen::ArrayXd&& v)
  {
    Hist ret;
    ret.fEigen = new Eigen::ArrayXd(std::move(v));
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

    if(fEigenStan){
      TH1D* ret = HistCache::New("", bins);
      for(int i = 0; i < bins.NBins()+2; ++i) ret->SetBinContent(i, (*fEigenStan)[i].val());
      return ret;
    }

    if(fEigen){
      TH1D* ret = HistCache::New("", bins);
      for(int i = 0; i < bins.NBins()+2; ++i) ret->SetBinContent(i, (*fEigen)[i]);
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
    if(fEigenStan) return fEigenStan->size()-2;
    if(fEigen) return fEigen->size()-2;

    abort(); // unreachable
  }

  //----------------------------------------------------------------------
  double Hist::GetBinError(int i) const
  {
    assert(Initialized());

    if(fHistD) return fHistD->GetBinError(i);
    if(fHistSparse) return fHistSparse->GetBinError(i);

    return 0; // TODO is this the right thing in stan/eigen case or should we abort?
  }

  //----------------------------------------------------------------------
  double Hist::Integral(int lo, int hi) const
  {
    assert(fHistD); // TODO how to use fHistSparse or stan or eigen?
    return fHistD->Integral(lo, hi);
  }

  //----------------------------------------------------------------------
  double Hist::GetMean() const
  {
    assert(fHistD); // TODO how to use fHistSparse or stan or eigen?

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
    else if(fEigenStan){
      std::cout << "Hist::Fill() not supported for stan vars" << std::endl;
      abort();
    }
    else if(fEigen){
      std::cout << "Hist::Fill() not supported for eigen" << std::endl;
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
    if(fEigenStan) *fEigenStan *= s;
    if(fEigen) *fEigen *= s;
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
    if(fEigenStan) return (*fEigenStan)[i].val();
    if(fEigen) return (*fEigen)[i];

    abort(); // unreachable
  }

  //----------------------------------------------------------------------
  void Hist::SetBinContent(int i, double x)
  {
    assert(Initialized());

    if(fHistD) fHistD->SetBinContent(i, x);
    if(fHistSparse) fHistSparse->SetBinContent(i, x);
    if(fEigenStan){
      std::cout << "Hist::SetBinContent() not implemented for stan vars" << std::endl;
      abort();
    }
    if(fEigen) (*fEigen)[i] = x;

    abort(); // unreachable
  }

  //----------------------------------------------------------------------
  void Hist::Reset()
  {
    if(fHistD) fHistD->Reset();
    if(fHistSparse) fHistSparse->Reset();
    if(fEigenStan) fEigenStan->setZero();
    if(fEigen) fEigen->setZero();
  }

  //----------------------------------------------------------------------
  void Hist::Add(const TH1D* rhs, double scale)
  {
    if(fHistD) fHistD->Add(rhs, scale);
    if(fHistSparse) fHistSparse->Add(rhs, scale);
    if(fEigenStan){
      const double* arr = rhs->GetArray();
      for(unsigned int i = 0; i < fEigenStan->size(); ++i){
        (*fEigenStan)[i] += arr[i];
      }
    }
    if(fEigen){
      const double* arr = rhs->GetArray();
      for(unsigned int i = 0; i < fEigen->size(); ++i){
        (*fEigen)[i] += arr[i];
      }
    }
  }

  //----------------------------------------------------------------------
  void Hist::Add(const THnSparseD* rhs, double scale)
  {
    if(fHistD){
      double* arr = fHistD->GetArray();
      for(int i = 0; i < fHistD->GetNbinsX()+2; ++i){
        arr[i] += rhs->GetBinContent(i) * scale;
      }
    }
    if(fHistSparse) fHistSparse->Add(rhs, scale);
    if(fEigenStan){
      for(unsigned int i = 0; i < fEigenStan->size(); ++i){
        (*fEigenStan)[i] += rhs->GetBinContent(i);
      }
    }
    if(fEigen){
      for(unsigned int i = 0; i < fEigen->size(); ++i){
        (*fEigen)[i] += rhs->GetBinContent(i);
      }
    }
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Eigen::ArrayXstan* rhs, double scale)
  {
    if(fHistD){
      fEigenStan = new Eigen::ArrayXstan(*rhs * scale);
      const double* arr = fHistD->GetArray();
      for(unsigned int i = 0; i < fEigenStan->size(); ++i){
        (*fEigenStan)[i] += arr[i];
      }
      HistCache::Delete(fHistD);
    }

    if(fHistSparse){
      fEigenStan = new Eigen::ArrayXstan(*rhs * scale);
      for(unsigned int i = 0; i < fEigenStan->size(); ++i){
        (*fEigenStan)[i] += fHistSparse->GetBinContent(i);
      }
      delete fHistSparse;
      fHistSparse = 0;
    }

    if(fEigenStan){
      *fEigenStan += *rhs * scale;
    }

    if(fEigen){
      fEigenStan = new Eigen::ArrayXstan(*fEigen + *rhs * scale);
      delete fEigen;
      fEigen = 0;
    }
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Eigen::ArrayXd* rhs, double scale)
  {
    if(fHistD){
      double* arr = fHistD->GetArray();
      for(unsigned int i = 0; i < rhs->size(); ++i) arr[i] += (*rhs)[i] * scale;
    }

    if(fHistSparse){
      for(unsigned int i = 0; i < rhs->size(); ++i) fHistSparse->AddBinContent(i, (*rhs)[i] * scale);
    }

    if(fEigenStan) *fEigenStan += *rhs * scale;

    if(fEigen) *fEigen += *rhs * scale;
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Hist& rhs, double scale)
  {
    assert(Initialized());
    assert(rhs.Initialized());

    if(rhs.fHistD)      Add(rhs.fHistD,      scale);
    if(rhs.fHistSparse) Add(rhs.fHistSparse, scale);
    if(rhs.fEigenStan)  Add(rhs.fEigenStan,  scale);
    if(rhs.fEigen)      Add(rhs.fEigen,      scale);
  }

  //----------------------------------------------------------------------
  void Hist::Multiply(TH1D* rhs)
  {
    if(fHistD) fHistD->Multiply(rhs); else abort();//fHistSparse->Multiply(rhs);
  }

  //----------------------------------------------------------------------
  void Hist::Multiply(const Hist& rhs)
  {
    if(fEigen && rhs.fEigen) *fEigen *= *rhs.fEigen;
    else if(fEigenStan && rhs.fEigenStan) *fEigenStan *= *rhs.fEigenStan;
    else abort();
  }

  //----------------------------------------------------------------------
  void Hist::Divide(TH1D* rhs)
  {
    if(fHistD) fHistD->Divide(rhs); else abort();//fHistSparse->Divide(rhs);
  }

  //----------------------------------------------------------------------
  void Hist::Divide(const Hist& rhs)
  {
    if(fEigen && rhs.fEigen) *fEigen /= *rhs.fEigen;
    else if(fEigenStan && rhs.fEigenStan) *fEigenStan /= *fEigenStan;
    else abort(); // TODO mixed modes
  }

  //----------------------------------------------------------------------
  void Hist::Write() const
  {
    assert(Initialized());

    if(fHistD) fHistD->Write("hist");
    if(fHistSparse) fHistSparse->Write("hist_sparse");
    if(fEigenStan){
      std::cout << "Hist::Write() not implemented (impossible?) for stan vars" << std::endl;
      abort();
    }
    if(fEigen){
      std::cout << "Hist::Write() not implemented for eigen" << std::endl;
      abort();
    }
  }
}
