#pragma once

#include "CAFAna/Core/Binning.h"

class TDirectory;
class TH1F;
class TH1D;
//class THnSparseD;
#include "THnSparse.h"

namespace ana
{
  class Hist
  {
  public:
    Hist(const Hist&);
    Hist(Hist&&);
    Hist& operator=(const Hist&);
    Hist& operator=(Hist&&);
    ~Hist();

    static Hist Uninitialized(){return Hist();}
    bool Initialized() const {return fHistD || fHistSparse;}

    static Hist Copy(TH1D const* h, const Binning& bins);
    static Hist Copy(TH1 const* h, const Binning& bins);
    static Hist Adopt(std::unique_ptr<TH1D> h, const Binning& bins);
    static Hist Adopt(std::unique_ptr<THnSparseD> h, const Binning& bins);

    static Hist FromDirectory(TDirectory* dir, const Binning& bins);

    TH1D* ToTH1() const;

    int GetNbinsX() const;
    double Integral(int lo, int hi) const;
    double GetMean() const;

    void Fill(double x, double w);
    void Scale(double s);
    void ResetErrors();

    double GetBinContent(int i) const;
    void SetBinContent(int i, double x);
    double GetBinError(int i) const;
    void SetBinError(int i, double x);
    void Reset();

    void Add(const Hist& rhs, double scale = 1);

    // TODO better to do this in terms of Hist - need to update Ratio
    void Multiply(TH1D* rhs);
    void Divide(TH1D* rhs);

    void Write() const;
  protected:
    Hist();

    //    TH1F* fHistF;
    TH1D* fHistD;
    THnSparseD* fHistSparse;
    Binning fBins;
  };
}
