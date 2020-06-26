#pragma once

#include "CAFAna/Core/StanTypedefs.h"

class TDirectory;
class TH1F;
class TH1D;
//class THnSparseD;
#include "THnSparse.h"

#include <Eigen/Dense> // TODO can we forward declare VectorXd?
namespace Eigen{using VectorXstan = Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>;}

namespace ana
{
  class Binning;

  class Hist
  {
  public:
    Hist(const Hist&);
    Hist(Hist&&);
    Hist& operator=(const Hist&);
    Hist& operator=(Hist&&);
    ~Hist();

    static Hist Uninitialized(){return Hist();}
    bool Initialized() const {return fHistD || fHistSparse || fEigenStan || fEigen;}

    // TODO try to convert all callers to Adopt()
    static Hist Copy(const TH1D* h);
    static Hist Copy(const TH1* h);
    static Hist Copy(const std::vector<stan::math::var>& v);

    static Hist Adopt(std::unique_ptr<TH1D> h);
    static Hist Adopt(std::unique_ptr<THnSparseD> h);
    static Hist Adopt(Eigen::VectorXstan&& v);
    static Hist Adopt(Eigen::VectorXd&& v);

    static Hist FromDirectory(TDirectory* dir);

    TH1D* ToTH1(const Binning& bins) const;

    int GetNbinsX() const;
    double GetBinError(int i) const;
    double Integral(int lo, int hi) const;
    double GetMean() const;

    void Fill(double x, double w);
    void Scale(double s);
    void ResetErrors();

    double GetBinContent(int i) const;
    void SetBinContent(int i, double x);
    void Reset();

    void Add(const Hist& rhs, double scale = 1);

    void Multiply(TH1D* rhs);
    void Multiply(const Hist& rhs);
    void Divide(TH1D* rhs);
    void Divide(const Hist& rhs);

    void Write() const;
  protected:
    Hist();

    // Helpers for the public Add() function
    void Add(const TH1D* rhs, double scale);
    void Add(const THnSparseD* rhs, double scale);
    void Add(const Eigen::VectorXstan* rhs, double scale);
    void Add(const Eigen::VectorXd* rhs, double scale);

    //    TH1F* fHistF;
    TH1D* fHistD;
    THnSparseD* fHistSparse;
    Eigen::VectorXstan* fEigenStan;
    Eigen::VectorXd* fEigen;
  };
}
