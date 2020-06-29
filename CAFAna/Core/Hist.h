#pragma once

#include "CAFAna/Core/StanTypedefs.h"

class TDirectory;

class TH1D;

#include <Eigen/Dense>
#include <Eigen/SparseCore>

namespace Eigen{
  using ArrayXstan = Eigen::Array<stan::math::var, Eigen::Dynamic, 1>;
  using VectorXstan = Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>;
}

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
    bool Initialized() const {return fEigenSparse || fEigenStan || fEigen;}

    static Hist Adopt(Eigen::SparseVector<double>&& v);
    static Hist Adopt(Eigen::ArrayXstan&& v);
    static Hist Adopt(Eigen::ArrayXd&& v);

    static Hist FromDirectory(TDirectory* dir);

    TH1D* ToTH1(const Binning& bins) const;

    bool HasStan() const {return fEigenStan != 0;}
    const Eigen::ArrayXd& GetEigen() const {assert(fEigen); return *fEigen;}
    const Eigen::ArrayXstan& GetEigenStan() const {assert(fEigenStan); return *fEigenStan;}

    int GetNbinsX() const;
    double GetBinError(int i) const;
    double Integral() const;
    double GetMean() const;

    void Fill(const Binning& bins, double x, double w);
    void Scale(double s);
    void ResetErrors();

    double GetBinContent(int i) const;
    void SetBinContent(int i, double x);
    void Reset();

    void Add(const Hist& rhs, double scale = 1);

    void Multiply(const Hist& rhs);
    void Divide(const Hist& rhs);

    void Write(const Binning& bins) const;
  protected:
    Hist();

    // Helpers for the public Add() function
    void Add(const Eigen::SparseVector<double>* rhs, double scale);
    void Add(const Eigen::ArrayXstan* rhs, double scale);
    void Add(const Eigen::ArrayXd* rhs, double scale);

    Eigen::SparseVector<double>* fEigenSparse;
    Eigen::ArrayXstan* fEigenStan;
    Eigen::ArrayXd* fEigen;
  };
}
