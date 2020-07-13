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
  Hist::Hist() : fType(kUninitialized)
  {
  }

  //----------------------------------------------------------------------
  Hist::Hist(const Hist& rhs)
    : Hist()
  {
    assert(rhs.Initialized());

    fType = rhs.fType;
    // Only one of these will actually have contents
    fDataSparse = rhs.fDataSparse;
    fDataStan   = rhs.fDataStan;
    fData       = rhs.fData;
  }

  //----------------------------------------------------------------------
  Hist::Hist(Hist&& rhs)
    : Hist()
  {
    assert(rhs.Initialized());

    fType = rhs.fType;
    std::swap(fDataSparse, rhs.fDataSparse);
    std::swap(fDataStan,   rhs.fDataStan);
    std::swap(fData,       rhs.fData);
  }

  //----------------------------------------------------------------------
  Hist& Hist::operator=(const Hist& rhs)
  {
    if(this == &rhs) return *this;

    DontAddDirectory guard;

    assert(rhs.Initialized());

    fType = rhs.fType;
    fDataSparse = rhs.fDataSparse;
    fDataStan   = rhs.fDataStan;
    fData       = rhs.fData;

    return *this;
  }

  //----------------------------------------------------------------------
  Hist& Hist::operator=(Hist&& rhs)
  {
    if(this == &rhs) return *this;

    assert(rhs.Initialized());

    fType = rhs.fType;
    std::swap(fDataSparse, rhs.fDataSparse);
    std::swap(fDataStan,   rhs.fDataStan);
    std::swap(fData,       rhs.fData);

    return *this;
  }

  //----------------------------------------------------------------------
  Hist::~Hist()
  {
  }

  //----------------------------------------------------------------------
  Hist Hist::Adopt(Eigen::SparseVector<double>&& v)
  {
    Hist ret;
    ret.fType = kSparse;
    ret.fDataSparse = std::move(v);
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::Adopt(Eigen::ArrayXstan&& v)
  {
    Hist ret;
    ret.fType = kDenseStan;
    ret.fDataStan = std::move(v);
    return ret;
  }

  //----------------------------------------------------------------------
  Hist Hist::Adopt(Eigen::ArrayXd&& v)
  {
    Hist ret;
    ret.fType = kDense;
    ret.fData = std::move(v);
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
      ret.fType = kDense;
      ret.fData.resize(h->GetNbinsX()+2);
      ret.fData.setZero();
      for(int i = 0; i < h->GetNbinsX()+2; ++i)
        ret.fData[i] = h->GetBinContent(i);
    }
    if(hSparse){
      ret.fType = kSparse;
      ret.fDataSparse.resize(h->GetNbinsX()+2);
      ret.fDataSparse.setZero();
      for(int i = 0; i < hSparse->GetNbins()+2; ++i){
        int idx;
        const double y = hSparse->GetBinContent(&idx);
        ret.fData.coeffRef(idx) = y;
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
      switch(fType){
      case kDense:     ret->SetBinContent(i, fData[i]); break;
      case kDenseStan: ret->SetBinContent(i, fDataStan[i].val()); break;
        // Interface requires returning literally a TH1D in any case
      case kSparse:    ret->SetBinContent(i, fDataSparse.coeff(i)); break;
      default:
        abort(); // unreachable
      }
    }
    return ret;
  }

  //----------------------------------------------------------------------
  int Hist::GetNbinsX() const
  {
    assert(Initialized());

    switch(fType){
    case kSparse:    return fDataSparse.size()-2;
    case kDenseStan: return fDataStan  .size()-2;
    case kDense:     return fData      .size()-2;
    default: abort(); // unreachable
    }
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

    switch(fType){
    case kSparse:    return fDataSparse.sum();
    case kDenseStan: return fDataStan  .sum().val();
    case kDense:     return fData      .sum();
    default: abort(); // unreachable
    }
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

    switch(fType){
    case kSparse:
      fDataSparse.coeffRef(bins.FindBin(x)) += w;
      break;
    case kDenseStan:
      std::cout << "Hist::Fill() not supported for stan vars" << std::endl;
      abort();
    case kDense:
      fData[bins.FindBin(x)] += w;
      break;
    default:
      abort(); // unreachable
    }
  }

  //----------------------------------------------------------------------
  void Hist::Scale(double s)
  {
    assert(Initialized());

    switch(fType){
    case kSparse:    fDataSparse *= s; break;
    case kDenseStan: fDataStan   *= s; break;
    case kDense:     fData       *= s; break;
    default:
      abort(); // unreachable
    }
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

    switch(fType){
    case kSparse: return fDataSparse.coeff(i);
    case kDenseStan: return fDataStan[i].val();
    case kDense: return fData[i];
    default:
      abort(); // unreachable
    }
  }

  //----------------------------------------------------------------------
  void Hist::SetBinContent(int i, double x)
  {
    assert(Initialized());

    switch(fType){
    case kSparse: fDataSparse.coeffRef(i) = x; break;
    case kDenseStan:
      std::cout << "Hist::SetBinContent() not implemented for stan vars" << std::endl;
      abort();
    case kDense: fData[i] = x; break;
    default:
      abort(); // unreachable
    }
  }

  //----------------------------------------------------------------------
  void Hist::Reset()
  {
    switch(fType){
    case kSparse:    fDataSparse.setZero(); break;
    case kDenseStan: fDataStan  .setZero(); break;
    case kDense:     fData      .setZero(); break;
    default: ; // OK?
    }
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Eigen::SparseVector<double>& rhs, double scale)
  {
    switch(fType){
    case kSparse:    fDataSparse += rhs * scale; break;
    case kDenseStan: fDataStan   += rhs * scale; break;
    case kDense:     fData       += rhs * scale; break;
    default: abort(); // unreachable
    }
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Eigen::ArrayXstan& rhs, double scale)
  {
    switch(fType){
    case kSparse:
      fType = kDenseStan;
      fDataStan = rhs * scale;
      fDataStan += fDataSparse;
      fDataSparse.resize(0);
      break;

    case kDenseStan:
      fDataStan += rhs * scale;
      break;

    case kDense:
      fType = kDenseStan;
      fDataStan = fData + rhs * scale;
      fData.resize(0);
      break;

    default: abort(); // unreachable
    }
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Eigen::ArrayXd& rhs, double scale)
  {
    switch(fType){
    case kSparse:
      fType = kDense;
      fData = rhs * scale;
      fData += fDataSparse;
      fDataSparse.resize(0);
      break;

    case kDenseStan:
      fDataStan += rhs * scale;
      break;

    case kDense:
      fData += rhs * scale;
      break;

    default: abort(); // unreachable
    }
  }

  //----------------------------------------------------------------------
  void Hist::Add(const Hist& rhs, double scale)
  {
    assert(Initialized());
    assert(rhs.Initialized());

    switch(rhs.fType){
    case kSparse:    Add(rhs.fDataSparse, scale); break;
    case kDenseStan: Add(rhs.fDataStan,   scale); break;
    case kDense:     Add(rhs.fData,       scale); break;
    default: abort(); // unreachable
    }
  }

  //----------------------------------------------------------------------
  void Hist::Multiply(const Hist& rhs)
  {
    assert(Initialized());
    assert(rhs.Initialized());

    if(fType == kSparse || rhs.fType == kSparse){
      std::cout << "Hist::Multiply() not implemented for sparse vectors" << std::endl;
      abort();
    }

    if(fType == kDenseStan){
      if(rhs.fType == kDenseStan){
        fDataStan *= rhs.fDataStan;
      }
      else{
        fDataStan *= rhs.fData;
      }
    }
    else{
      if(rhs.fType == kDenseStan){
        fType = kDenseStan;
        fDataStan = fData * rhs.fDataStan;
        fData.resize(0);
      }
      else{
        fData *= rhs.fData;
      }
    }
  }

  //----------------------------------------------------------------------
  void Hist::Divide(const Hist& rhs)
  {
    assert(Initialized());
    assert(rhs.Initialized());

    if(fType == kSparse || rhs.fType == kSparse){
      std::cout << "Hist::Divide() not implemented for sparse vectors" << std::endl;
      abort();
    }

    if(fType == kDenseStan){
      if(rhs.fType == kDenseStan){
        fDataStan /= rhs.fDataStan;
      }
      else{
        fDataStan /= rhs.fData;
      }
    }
    else{
      if(rhs.fType == kDenseStan){
        fType = kDenseStan;
        fDataStan = fData / rhs.fDataStan;
        fData.resize(0);
      }
      else{
        fData /= rhs.fData;
      }
    }
  }

  //----------------------------------------------------------------------
  void Hist::Write(const Binning& bins) const
  {
    assert(Initialized());

    if(fType == kDenseStan){
      std::cout << "Hist::Write() not implemented (impossible?) for stan vars" << std::endl;
      abort();
    }
    if(fType == kDense){
      TH1D* h = ToTH1(bins);
      h->Write("hist");
      delete h;
    }
    if(fType == kSparse){
      const int n = bins.NBins();
      const double x0 = bins.IsSimple() ? bins.Min() : 0;
      const double x1 = bins.IsSimple() ? bins.Max() : bins.NBins();
      THnSparseD* h = new THnSparseD("", "", 1, &n, &x0, &x1);

      for(Eigen::SparseVector<double>::InnerIterator it(fDataSparse); it; ++it){
        h->SetBinContent(it.index(), it.value());
      }

      h->Write("hist_sparse");
      delete h;
    }
  }
}
