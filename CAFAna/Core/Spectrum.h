#pragma once

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Hist.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/Utilities.h"

#include <Eigen/Dense>

#include "TAttLine.h"

#include <string>
#include <vector>

class TDirectory;
class TH1;
class TH2;
class TH3;
class TH1D;

/// Oscillation analysis framework, runs over CAF files outside of ART
namespace ana
{
  class MultiVar;
  class Ratio;
  class SpectrumLoaderBase;

  /// Representation of a spectrum in any variable, with associated POT
  class Spectrum
  {
  public:
    friend class SpectrumLoaderBase;
    friend class SpectrumLoader;
    friend class NullLoader;
    friend class Ratio;

    enum ESparse{kDense, kSparse};

    // One constructor to rule them all
    Spectrum(SpectrumLoaderBase& loader,
             const HistAxis& axis,
             const Cut& cut,
             const SystShifts& shift = kNoShift,
             const Var& wei = kUnweighted,
             ESparse sparse = kDense);

    Spectrum(const std::string& label,
             const Binning& bins,
             SpectrumLoaderBase& loader,
             const Var& var,
             const Cut& cut,
             const SystShifts& shift = kNoShift,
             const Var& wei = kUnweighted,
             ESparse sparse = kDense)
      : Spectrum(loader, HistAxis(label, bins, var), cut, shift, wei, sparse)
    {
    }

    /// The only \ref MultiVar variant available
    Spectrum(const std::string& label, const Binning& bins,
             SpectrumLoaderBase& loader,
             const MultiVar& var,
             const Cut& cut,
             const SystShifts& shift = kNoShift,
             const Var& wei = kUnweighted);

    /// Makes a spectrum from an eigen array
    Spectrum(Eigen::ArrayXd&& h,
             const HistAxis& axis,
             double pot, double livetime);

    /// Makes a spectrum from an eigen array of stan vars
    Spectrum(Eigen::ArrayXstan&& h,
             const HistAxis& axis,
             double pot, double livetime);

    /// 2D Spectrum taking 2 HistAxis
    Spectrum(SpectrumLoaderBase& loader,
             const HistAxis& xAxis,
             const HistAxis& yAxis,
             const Cut& cut,
             const SystShifts& shift = kNoShift,
             const Var& wei = kUnweighted,
             ESparse sparse = kDense)
      : Spectrum(loader, HistAxis(xAxis, yAxis), cut, shift, wei, sparse)
    {
    }

    /// 2D Spectrum of two Vars
    Spectrum(const std::string& xLabel,
	     const std::string& yLabel,
	     SpectrumLoaderBase& loader,
	     const Binning& binsx, const Var& varx,
	     const Binning& binsy, const Var& vary,
	     const Cut& cut,
	     const SystShifts& shift = kNoShift,
	     const Var& wei = kUnweighted,
             ESparse sparse = kDense)
      : Spectrum(loader, HistAxis(xLabel, binsx, varx,
                                  yLabel, binsy, vary),
                 cut, shift, wei, sparse)
    {
    }

    /// 3D Spectrum of three Vars
    Spectrum(const std::string& xLabel,
	     const std::string& yLabel,
	     const std::string& zLabel,
	     SpectrumLoaderBase& loader,
             const Binning& binsx, const Var& varx,
             const Binning& binsy, const Var& vary,
             const Binning& binsz, const Var& varz,
             const Cut& cut,
             const SystShifts& shift = kNoShift,
             const Var& wei = kUnweighted,
	     ESparse sparse = kDense)
      : Spectrum(loader, HistAxis(HistAxis(xLabel, binsx, varx),
                                  HistAxis(yLabel, binsy, vary),
                                  HistAxis(zLabel, binsz, varz)),
                 cut, shift, wei, sparse)
    {
    }

    /// 3D Spectrum taking 3 HistAxis
    Spectrum(SpectrumLoaderBase& loader,
             const HistAxis& xAxis,
             const HistAxis& yAxis,
	     const HistAxis& zAxis,
             const Cut& cut,
             const SystShifts& shift = kNoShift,
             const Var& wei = kUnweighted,
	     ESparse sparse = kDense)
      : Spectrum(loader, HistAxis(xAxis, yAxis, zAxis), cut, shift, wei, sparse)
    {
    }

    /// Expert constructor for ReweightableSpectrum et al
    Spectrum(Hist&& hist,
             const HistAxis& axis,
             double pot,
             double livetime)
      : fHist(std::move(hist)), fPOT(pot), fLivetime(livetime), fAxis(axis)
    {
    }

    /// The only valid thing to do with such a spectrum is to assign something
    /// else into it.
    static Spectrum Uninitialized(){return Spectrum();}

    virtual ~Spectrum();

    Spectrum(const Spectrum& rhs);
    Spectrum(Spectrum&& rhs);
    Spectrum& operator=(const Spectrum& rhs);
    Spectrum& operator=(Spectrum&& rhs);

    void Fill(double x, double w = 1);

    /// \brief Histogram made from this Spectrum, scaled to some exposure
    ///
    /// \param exposure POT or livetime (seconds)
    /// \param col Histogram color (default black)
    /// \param style Histogram line style (default solid)
    /// \param expotype How to interpret exposure (kPOT (default) or kLivetime)
    TH1D* ToTH1(double exposure,
                Color_t col = kBlack,
                Style_t style = kSolid,
                EExposureType expotype = kPOT,
                EBinType bintype = kBinContent) const;

    /// \brief Histogram made from this Spectrum, scaled to some exposure
    ///
    /// \param exposure POT or livetime (seconds)
    /// \param expotype How to interpret exposure (kPOT (default) or kLivetime)
    TH1D* ToTH1(double exposure,
                EExposureType expotype,
                EBinType bintype = kBinContent) const;

    /// Spectrum must be 2D to obtain TH2
    TH2*  ToTH2     (double exposure, EExposureType expotype = kPOT,
		     EBinType bintype = kBinContent) const;
    /// Spectrum must be 2D to obtain TH2. Normalized to X axis.
    TH2*  ToTH2NormX(double exposure, EExposureType expotype = kPOT) const;

    /// Spectrum must be 3D to obtain TH3
    TH3*  ToTH3     (double exposure, EExposureType expotype = kPOT,
		     EBinType bintype = kBinContent) const;

    /// Function decides what is the appropriate projection based on fBins, and does that
    TH1*  ToTHX     (double exposure, bool force1D = false, EExposureType expotype = kPOT) const;

    TH1* ToTH1ProjectX(double exposure, EExposureType expotype = kPOT) const;

    bool HasStan() const {return fHist.HasStan();}
    /// NB these don't have POT scaling. For expert high performance ops only!
    const Eigen::ArrayXd& GetEigen() const {return fHist.GetEigen();}
    const Eigen::ArrayXstan& GetEigenStan() const {return fHist.GetEigenStan();}

    Eigen::ArrayXd GetEigen(double pot) const;
    Eigen::ArrayXstan GetEigenStan(double pot) const;

    /// \brief Return total number of events scaled to \a pot
    ///
    /// \param exposure POT/livetime to scale to
    /// \param err      The statistical error on this total (optional)
    /// \param expotype What the first parameter represents
    double Integral(double exposure, double* err = 0,
		    EExposureType expotype = kPOT) const;

    /// \brief Return mean of 1D histogram
    double Mean() const;

    /// \brief Mock data is \ref FakeData with Poisson fluctuations applied
    ///
    /// Use for low-budget MDCs, or just getting a sense of the expected scale
    /// of statistical variation
    Spectrum MockData(double pot, bool makethrow=true, int seed=0) const;
    /// \brief Fake data is a MC spectrum scaled to the POT expected in the data
    ///
    /// Use for sensitivity plots and testing fit convergence
    Spectrum FakeData(double pot) const;

    double POT() const {return fPOT;}

    /// Seconds. For informational purposes only. No calculations use this.
    double Livetime() const {return fLivetime;}

    /// DO NOT USE UNLESS YOU ARE 110% CERTAIN THERE ISN'T A BETTER WAY!
    void OverridePOT(double newpot) {fPOT = newpot;}

    /// DO NOT USE UNLESS YOU ARE 110% CERTAIN THERE ISN'T A BETTER WAY!
    void OverrideLivetime(double newlive) {fLivetime = newlive;}

    void Clear();

    /// Multiply this spectrum by a constant c
    void Scale(double c);

    // Arithmetic operators are as if these are unlike samples, each a
    // contribution to one total, not seperate sources of stats for the same
    // sample.
    Spectrum& operator+=(const Spectrum& rhs);
    Spectrum operator+(const Spectrum& rhs) const;

    Spectrum& operator-=(const Spectrum& rhs);
    Spectrum operator-(const Spectrum& rhs) const;

    Spectrum& operator*=(const Ratio& rhs);
    Spectrum operator*(const Ratio& rhs) const;

    Spectrum& operator/=(const Ratio& rhs);
    Spectrum operator/(const Ratio& rhs) const;

    void SaveTo(TDirectory* dir, const std::string& name) const;
    static std::unique_ptr<Spectrum> LoadFrom(TDirectory* dir, const std::string& name);

    unsigned int NDimensions() const{return fAxis.NDimensions();}
    const std::vector<std::string>& GetLabels() const {return fAxis.GetLabels();}
    const std::vector<Binning>& GetBinnings() const {return fAxis.GetBinnings();}

  protected:
    // Constructor for Uninitialized()
    Spectrum()
      : fHist(Hist::Uninitialized()),
        fPOT(0), fLivetime(0),
        fAxis({}, {}, {})
    {
    }

    void RemoveLoader(SpectrumLoaderBase*);
    void AddLoader(SpectrumLoaderBase*);

    /// Helper for operator+= and operator-=
    Spectrum& PlusEqualsHelper(const Spectrum& rhs, int sign);

    Hist fHist;
    double fPOT;
    double fLivetime;

    /// This count is maintained by SpectrumLoader, as a sanity check
    std::set<SpectrumLoaderBase*> fLoaderCount;

    HistAxis fAxis;
  };

  // Commutative
  inline Spectrum operator*(const Ratio& lhs, const Spectrum& rhs){return rhs*lhs;}
}
