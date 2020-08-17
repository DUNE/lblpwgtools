#pragma once

#include "Utilities/func/Stan.h" // need stan proper for fDataStan

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
    bool Initialized() const {return fType != kUninitialized;}

    static Hist Zero(int nbins);
    static Hist ZeroSparse(int nbins);

    static Hist Adopt(Eigen::SparseVector<double>&& v);
    static Hist Adopt(Eigen::ArrayXstan&& v);
    static Hist Adopt(Eigen::ArrayXd&& v);

    static Hist FromDirectory(TDirectory* dir);

    TH1D* ToTH1(const Binning& bins) const;

    bool HasStan() const {return fType == kDenseStan;}
    const Eigen::ArrayXd& GetEigen() const {assert(fType == kDense); return fData;}
    const Eigen::ArrayXstan& GetEigenStan() const {assert(fType == kDenseStan); return fDataStan;}

    int GetNbinsX() const;
    double GetBinError(int i) const;
    double Integral() const;

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
    void Add(const Eigen::SparseVector<double>& rhs, double scale);
    void Add(const Eigen::ArrayXstan& rhs, double scale);
    void Add(const Eigen::ArrayXd& rhs, double scale);

    enum EType{kUninitialized, kDense, kDenseStan, kSparse};
    EType fType;

    Eigen::SparseVector<double> fDataSparse;
    Eigen::ArrayXstan fDataStan;
    Eigen::ArrayXd fData;
    Eigen::ArrayXd fSumSq; ///< Accumulate errors, if enabled
    bool fSqrtErrs; ///< Special case when filled with unweighted data
  };
}
