#pragma once

#include "CAFAna/Core/Spectrum.h"

#include <string>

class TDirectory;
class TH2;
class TH2D;

namespace ana
{
  /// %Spectrum with the value of a second variable, allowing for reweighting
  class ReweightableSpectrum
  {
  public:
    friend class SpectrumLoaderBase;
    friend class SpectrumLoader;
    friend class NullLoader;
    friend class MRCCLoader;

    ReweightableSpectrum(SpectrumLoaderBase& loader,
                         const HistAxis& recoAxis,
                         const HistAxis& trueAxis,
                         const Cut& cut,
                         const SystShifts& shift = kNoShift,
                         const Var& wei = kUnweighted);

    ReweightableSpectrum(const Eigen::MatrixXd&& mat,
                         const HistAxis& recoAxis,
                         const HistAxis& trueAxis,
                         double pot, double livetime);

    /// The only valid thing to do with such a spectrum is to assign something
    /// else into it.
    static ReweightableSpectrum Uninitialized(){return ReweightableSpectrum();}

    virtual ~ReweightableSpectrum();

    ReweightableSpectrum(const ReweightableSpectrum& rhs);
    ReweightableSpectrum& operator=(const ReweightableSpectrum& rhs);

    /// \brief The variable that will be used to fill the y-axis
    ///
    /// By convention, return zero if the information can't be obtained, and
    /// this event will be skipped.
    const Var& ReweightVar() const {return fRWVar;}

    void Fill(double x, double y, double w = 1);

    TH2D* ToTH2(double pot) const;

    double POT() const{return fPOT;}
    double Livetime() const{return fLivetime;}

    Spectrum UnWeighted() const;

    Spectrum WeightingVariable() const;

    /// Reco spectrum with truth weights applied
    Spectrum WeightedBy(const Ratio& weights) const;

    /// Rescale bins so that \ref WeightingVariable will return \a target
    void ReweightToTrueSpectrum(const Spectrum& target);
    /// Recale bins so that \ref Unweighted will return \a target
    void ReweightToRecoSpectrum(const Spectrum& target);

    // Arithmetic operators are as if these are unlike samples, each a
    // contribution to one total, not seperate sources of stats for the same
    // sample.
    ReweightableSpectrum& operator+=(const ReweightableSpectrum& rhs);
    ReweightableSpectrum operator+(const ReweightableSpectrum& rhs) const;

    ReweightableSpectrum& operator-=(const ReweightableSpectrum& rhs);
    ReweightableSpectrum operator-(const ReweightableSpectrum& rhs) const;

    void Clear();

    /// Function to save a ReweightableSpectrum to file
    /// the fRWVar member is not written to file, so when
    /// the spectrum is loaded back from file, ReweightVar
    /// should not be accessed, but reweighting still works
    void SaveTo(TDirectory* dir, const std::string& name) const;

    static std::unique_ptr<ReweightableSpectrum> LoadFrom(TDirectory* dir, const std::string& name);

    unsigned int NDimensions() const{return fAxisX.NDimensions();}
    const std::vector<std::string>& GetLabels() const {return fAxisX.GetLabels();}
    const std::vector<Binning>& GetBinnings() const {return fAxisX.GetBinnings();}
    Binning GetTrueBinning() const {return fBinsY;}

    /// DO NOT USE UNLESS YOU ARE 110% CERTAIN THERE ISN'T A BETTER WAY!
    void OverridePOT(double newpot) {fPOT = newpot;}

    /// DO NOT USE UNLESS YOU ARE 110% CERTAIN THERE ISN'T A BETTER WAY!
    void OverrideLivetime(double newlive) {fLivetime = newlive;}

    double POT() {return fPOT;}

    double Livetime() {return fLivetime;}

    Eigen::MatrixXd GetEigen(double pot) const {return fMat * pot/fPOT;}
  protected:
    // Derived classes can be trusted take care of their own construction
    ReweightableSpectrum(const HistAxis& axisX,
                         const Binning& ybins,
                         const Var& rwVar)
      : fRWVar(rwVar),
        fPOT(0), fLivetime(0),
        fAxisX(axisX), fBinsY(ybins)
    {
    }

    // Constructor for user by Uninitialized()
    ReweightableSpectrum()
      : fRWVar(kUnweighted), fPOT(0), fLivetime(0), fAxisX({}, {}, {}), fBinsY(Binning::Simple(1, 0, 1))
    {
    }

    ReweightableSpectrum& PlusEqualsHelper(const ReweightableSpectrum& rhs, int sign);

    void RemoveLoader(SpectrumLoaderBase*);
    void AddLoader(SpectrumLoaderBase*);

    void _SaveTo(TDirectory* dir,
                 const std::string& name,
                 const std::string& type) const;

    Var fRWVar; ///< What goes on the y axis?

    Eigen::MatrixXd fMat;
    double fPOT;
    double fLivetime;

    HistAxis fAxisX;

    Binning fBinsY;

    std::string fTrueLabel;

    /// This count is maintained by SpectrumLoader, as a sanity check
    std::set<SpectrumLoaderBase*> fLoaderCount;
  };
}
