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
    : fEigenSparse(0), fEigenStan(0), fEigen(0)
  {
  }

  //----------------------------------------------------------------------
  Hist::Hist(const Hist& rhs)
    : Hist()
  {
    assert(rhs.Initialized());

    if(rhs.fEigenSparse) fEigenSparse = new Eigen::SparseVector<double>(*rhs.fEigenSparse);

    if(rhs.fEigenStan) fEigenStan = new Eigen::ArrayXstan(*rhs.fEigenStan);

    if(rhs.fEigen) fEigen = new Eigen::ArrayXd(*rhs.fEigen);
  }

  //----------------------------------------------------------------------
  Hist::Hist(Hist&& rhs)
    : Hist()
  {
    assert(rhs.Initialized());

    std::swap(fEigenSparse, rhs.fEigenSparse);
    std::swap(fEigenStan, rhs.fEigenStan);
    std::swap(fEigen, rhs.fEigen);
  }

  //----------------------------------------------------------------------
  Hist& Hist::operator=(const Hist& rhs)
  {
    if(this == &rhs) return *this;

    DontAddDirectory guard;

    delete fEigenSparse;
    delete fEigenStan;
    delete fEigen;

    fEigenSparse = 0;
    fEigenStan = 0;
    fEigen = 0;

    assert(rhs.Initialized());

    if(rhs.fEigenSparse){
      fEigenSparse = new Eigen::SparseVector<double>(*rhs.fEigenSparse);
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

    std::swap(fEigenSparse, rhs.fEigenSparse);
    std::swap(fEigenStan, rhs.fEigenStan);
    std::swap(fEigen, rhs.fEigen);

    return *this;
  }

  //----------------------------------------------------------------------
  Hist::~Hist()
  {
    delete fEigenSparse;

    delete fEigenStan;

    delete fEigen;
  }

  //----------------------------------------------------------------------
  Hist Hist::Adopt(Eigen::SparseVector<double>&& v)
  {
    Hist ret;
    ret.fEigenSparse = new Eigen::SparseVector<double>(std::move(v));
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

    TH1D* h = (TH1D*)dir->Get("hist");
    THnSparseD* hSparse = (THnSparseD*)dir->Get("hist_sparse");
    assert(bool(h) != bool(hSparse));

    if(h){
      ret.fEigen = new Eigen::ArrayXd(h->GetNbinsX()+2);
      for(int i = 0; i < h->GetNbinsX()+2; ++i)
        (*ret.fEigen)[i] = h->GetBinContent(i);
    }
    if(hSparse){
      ret.fEigenSparse = new Eigen::SparseVector<double>(h->GetNbinsX()+2);
      for(int i = 0; i < hSparse->GetNbins()+2; ++i){
        int idx;
        const double y = hSparse->GetBinContent(&idx);
        ret.fEigen->coeffRef(idx) = y;
      }
    }

    delete h;
    delete hSparse;

    return ret;
  }

  //----------------------------------------------------------------------
  TH1D* Hist::ToTH1(const Binning& bins) const
  {
    assert(Initialized());

    TH1D* ret = HistCache::New("", bins);
    for(int i = 0; i < bins.NBins()+2; ++i){
      if(fEigen){
        ret->SetBinContent(i, (*fEigen)[i]);
      }
      else if(fEigenStan){
        ret->SetBinContent(i, (*fEigenStan)[i].val());
      }
      else if(fEigenSparse){
        // Interface requires returning literally a TH1D in any case
        ret->SetBinContent(i, fEigenSparse->coeff(i));
      }
      else{
        abort(); // unreachable
      }
    }
    return ret;
  }

  //----------------------------------------------------------------------
  int Hist::GetNbinsX() const
  {
    assert(Initialized());

    if(fEigenSparse) return fEigenSparse->size()-2;
    if(fEigenStan) return fEigenStan->size()-2;
    if(fEigen) return fEigen->size()-2;

    abort(); // unreachable
  }

  //----------------------------------------------------------------------
  double Hist::GetBinError(int i) const
  {
    assert(Initialized());

    // TODO implement optional bin error accumulation

    return 0;
  }

  //----------------------------------------------------------------------
  double Hist::Integral() const
  {
    assert(Initialized());

    if(fEigen) return fEigen->sum();

    if(fEigenStan) return fEigenStan->sum().val();

    if(fEigenSparse) return fEigenSparse->sum();

    abort(); // unreachable
  }

  //----------------------------------------------------------------------
  double Hist::GetMean() const
  {
    // TODO implement mean
    abort();
  }

  //----------------------------------------------------------------------
  void Hist::Fill(const Binning& bins, double x, double w)
  {
    assert(Initialized());

    if(fEigenSparse){
      fEigenSparse->coeffRef(bins.FindBin(x)) += w;
    }
    else if(fEigenStan){
      std::cout << "Hist::Fill() not supported for stan vars" << std::endl;
      abort();
    }
    else if(fEigen){
      (*fEigen)[bins.FindBin(x)] += w;
    }
    else{
      abort(); // unreachable
    }
  }

  //----------------------------------------------------------------------
  void Hist::Scale(double s)
  {
    assert(Initialized());

    if(fEigenSparse) *fEigenSparse *= s;
    if(fEigenStan) *fEigenStan *= s;
    if(fEigen) *fEigen *= s;
  }

  //----------------------------------------------------------------------
  void Hist::ResetErrors()
  {
    // TODO errors
  }

  //----------------------------------------------------------------------
  double Hist::GetBinContent(int i) const
  {
    assert(Initialized());

    if(fEigenSparse) return fEigenSparse->coeff(i);
    if(fEigenStan) return (*fEigenStan)[i].val();
    if(fEigen) return (*fEigen)[i];

    abort(); // unreachable
  }

  //----------------------------------------------------------------------
  void Hist::SetBinContent(int i, double x)
  {
    assert(Initialized());

    if(fEigenSparse){fEigenSparse->coeffRef(i) = x; return;}
    if(fEigenStan){
      std::cout << "Hist::SetBinContent() not implemented for stan vars" << std::endl;
      abort();
    }
    if(fEigen){(*fEigen)[i] = x; return;}

    abort(); // unreachable
  }

  //----------------------------------------------------------------------
  void Hist::Reset()
  {
    if(fEigenSparse) fEigenSparse->setZero();
    if(fEigenStan) fEigenStan->setZero();
    if(fEigen) fEigen->setZero();
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Eigen::SparseVector<double>* rhs, double scale)
  {
    if(fEigenSparse) *fEigenSparse += *rhs * scale;
    if(fEigenStan) *fEigenStan += *rhs * scale;
    if(fEigen) *fEigen += *rhs * scale;
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Eigen::ArrayXstan* rhs, double scale)
  {
    if(fEigenSparse){
      fEigenStan = new Eigen::ArrayXstan(*fEigen + *rhs * scale);
      delete fEigenSparse;
      fEigenSparse = 0;
    }

    if(fEigenStan) *fEigenStan += *rhs * scale;

    if(fEigen){
      fEigenStan = new Eigen::ArrayXstan(*fEigen + *rhs * scale);
      delete fEigen;
      fEigen = 0;
    }
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Eigen::ArrayXd* rhs, double scale)
  {
    if(fEigenSparse){
      fEigen = new Eigen::ArrayXd(Eigen::VectorXd(*fEigenSparse));
      *fEigen += *rhs * scale;
      delete fEigenSparse;
    }

    if(fEigenStan) *fEigenStan += *rhs * scale;

    if(fEigen) *fEigen += *rhs * scale;
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Hist& rhs, double scale)
  {
    assert(Initialized());
    assert(rhs.Initialized());

    if(rhs.fEigenSparse) Add(rhs.fEigenSparse, scale);
    if(rhs.fEigenStan)   Add(rhs.fEigenStan,   scale);
    if(rhs.fEigen)       Add(rhs.fEigen,       scale);
  }

  //----------------------------------------------------------------------
  void Hist::Multiply(const Hist& rhs)
  {
    assert(Initialized());
    assert(rhs.Initialized());

    if(fEigenSparse || rhs.fEigenSparse){
      std::cout << "Hist::Multiply() not implemented for sparse vectors" << std::endl;
      abort();
    }

    if(fEigenStan){
      if(rhs.fEigenStan){
        *fEigenStan *= *rhs.fEigenStan;
      }
      else{
        *fEigenStan *= *rhs.fEigen;
      }
    }
    else{
      if(rhs.fEigenStan){
        fEigenStan = new Eigen::ArrayXstan(*rhs.fEigenStan);
        *fEigenStan *= *fEigen;
        delete fEigen;
      }
      else{
        *fEigen *= *rhs.fEigen;
      }
    }
  }

  //----------------------------------------------------------------------
  void Hist::Divide(const Hist& rhs)
  {
    assert(Initialized());
    assert(rhs.Initialized());

    if(fEigenSparse || rhs.fEigenSparse){
      std::cout << "Hist::Divide() not implemented for sparse vectors" << std::endl;
      abort();
    }

    if(fEigenStan){
      if(rhs.fEigenStan){
        *fEigenStan /= *rhs.fEigenStan;
      }
      else{
        *fEigenStan /= *rhs.fEigen;
      }
    }
    else{
      if(rhs.fEigenStan){
        fEigenStan = new Eigen::ArrayXstan(*rhs.fEigenStan);
        *fEigenStan /= *fEigen;
        delete fEigen;
      }
      else{
        *fEigen /= *rhs.fEigen;
      }
    }
  }

  //----------------------------------------------------------------------
  void Hist::Write(const Binning& bins) const
  {
    assert(Initialized());

    if(fEigenStan){
      std::cout << "Hist::Write() not implemented (impossible?) for stan vars" << std::endl;
      abort();
    }
    if(fEigen){
      TH1D* h = ToTH1(bins);
      h->Write("hist");
      delete h;
    }
    if(fEigenSparse){
      const int n = bins.NBins();
      const double x0 = bins.IsSimple() ? bins.Min() : 0;
      const double x1 = bins.IsSimple() ? bins.Max() : bins.NBins();
      THnSparseD* h = new THnSparseD("", "", 1, &n, &x0, &x1);

      for(Eigen::SparseVector<double>::InnerIterator it(*fEigenSparse); it; ++it){
        h->SetBinContent(it.index(), it.value());
      }

      h->Write("hist_sparse");
      delete h;
    }
  }
}
