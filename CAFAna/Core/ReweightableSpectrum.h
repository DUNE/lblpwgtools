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

    /// The only valid thing to do with such a spectrum is to assign something
    /// else into it.
    static ReweightableSpectrum Uninitialized(){return ReweightableSpectrum();}

    /*
    ReweightableSpectrum(const Var& rwVar,
                         const std::string& xlabel, const std::string& ylabel,
                         double pot,
                         int nbinsx, double xmin, double xmax,
                         int nbinsy, double ymin, double ymax);
    */
    /*
    ReweightableSpectrum(const Var& rwVar,
                         TH2* h,
                         const std::vector<std::string>& labels,
                         const std::vector<Binning>& bins,
                         double pot, double livetime);

    ReweightableSpectrum(const Var& rwVar,
                         std::unique_ptr<TH2D> h,
                         const std::vector<std::string>& labels,
                         const std::vector<Binning>& bins,
                         double pot, double livetime);
    */

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

    TAxis const *GetReweightTAxis() const;

    Spectrum UnWeighted() const;

    Spectrum WeightingVariable() const;

    /// Reco spectrum with truth weights applied
    Spectrum WeightedBy(const Ratio& weights) const;

    /*
    /// Rescale bins so that \ref WeightingVariable will return \a target
    void ReweightToTrueSpectrum(const Spectrum& target);
    /// Recale bins so that \ref Unweighted will return \a target
    void ReweightToRecoSpectrum(const Spectrum& target);
    */

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

    unsigned int NDimensions() const{return fLabels.size();}
    std::vector<std::string> GetLabels() const {return fLabels;}
    std::vector<Binning> GetBinnings() const {return fBins;}
    Binning GetTrueBinning() const {return fBinsY;}

    /// DO NOT USE UNLESS YOU ARE 110% CERTAIN THERE ISN'T A BETTER WAY!
    void OverridePOT(double newpot) {fPOT = newpot;}

    /// DO NOT USE UNLESS YOU ARE 110% CERTAIN THERE ISN'T A BETTER WAY!
    void OverrideLivetime(double newlive) {fLivetime = newlive;}

    double POT() {return fPOT;}

    double Livetime() {return fLivetime;}

  protected:
    // Derived classes can be trusted take care of their own construction
    ReweightableSpectrum(const std::vector<std::string>& labels,
                         const std::vector<Binning>& bins,
                         const Binning& ybins,
                         const Var& rwVar)
      : fRWVar(rwVar),
        fMat(0), fPOT(0), fLivetime(0),
        fLabels(labels), fBins(bins), fBinsY(ybins)
    {
    }

    ReweightableSpectrum(const std::string& label,
                         const Binning& bins,
                         const Binning& ybins,
                         const Var& rwVar)
      : fRWVar(rwVar),
        fMat(0), fPOT(0), fLivetime(0),
        fLabels(1, label), fBins(1, bins), fBinsY(ybins)
    {
    }

    // Constructor for user by Uninitialized()
    ReweightableSpectrum()
      : fRWVar(kUnweighted), fMat(0), fPOT(0), fLivetime(0), fBinsY(Binning::Simple(1, 0, 1))
    {
    }

    /// Constructor needed by LoadFrom. Since there's no good
    /// way to store a Var, ReweightVar will return nonsense
    /// for ReweightableSpectrum that are loaded from a file
    /*
    ReweightableSpectrum(TH2D* h,
                         const std::vector<std::string>& labels,
                         const std::vector<Binning>& bins,
                         const Binning& binsY,
                         double pot, double livetime)
      : ReweightableSpectrum(kUnweighted, h, labels, bins, binsY, pot, livetime)
    {
    }
    */

    ReweightableSpectrum& PlusEqualsHelper(const ReweightableSpectrum& rhs, int sign);

    void RemoveLoader(SpectrumLoaderBase*);
    void AddLoader(SpectrumLoaderBase*);

    Binning Bins1DX() const;

    Var fRWVar; ///< What goes on the y axis?

    Eigen::MatrixXd* fMat; // TODO no need for this to be a pointer
    double fPOT;
    double fLivetime;

    std::vector<std::string> fLabels;
    std::vector<Binning> fBins;
    Binning fBinsY;

    std::string fTrueLabel;

    /// This count is maintained by SpectrumLoader, as a sanity check
    std::set<SpectrumLoaderBase*> fLoaderCount;
  };
}
