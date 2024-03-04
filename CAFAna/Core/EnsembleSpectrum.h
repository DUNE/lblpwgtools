#pragma once

#include "cafanacore/Spectrum.h"

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/IRecordSource.h"
#include "CAFAna/Core/Weight.h"

#include <cassert>

class TGraphAsymmErrors;

namespace ana
{
  class EnsembleRatio;
  class FitMultiverse;

  class EnsembleSpectrum : public IValueEnsembleSink
  {
  public:
    friend class EnsembleRatio;

    /// Construct an ensemble spectrum from a source of values and an axis
    /// definition
    EnsembleSpectrum(IValueEnsembleSource& src, const LabelsAndBins& axis);

    /// \brief Shorthand construction with a source of records and a HistAxis
    /// defining the Var to extract from those records.
    template<class RecT>
    EnsembleSpectrum(_IRecordEnsembleSource<RecT>& src,
                     const _HistAxis<_Var<RecT>>& axis)
      : EnsembleSpectrum(src[axis.GetVar1D()], axis)
    {
    }

    /// \brief Creates an ensemble spectrum for "data" from an input \ref Spectrum
    //         which is replicated nUniverse times from the multiverse which it adopts.
    //         Note that this is a temporary workaround for now
    static EnsembleSpectrum ReplicatedData(const Spectrum& spec, const FitMultiverse* multiverse);

    Spectrum Nominal() const {return Universe(0);}
    unsigned int NUniverses() const;
    Spectrum Universe(unsigned int i) const;

    // TODO consider naming confusion with Universe() above
    const FitMultiverse& GetMultiverse() const {return *fMultiverse;}

    double POT() const {return fPOT;}

    double Livetime() const {return fLivetime;}

    /// Result can be painted prettily with \ref DrawErrorBand
    TGraphAsymmErrors* ErrorBand(double exposure,
                                 EExposureType expotype = kPOT,
                                 EBinType bintype = kBinContent) const;

    /// Wrapper for \ref CalcCovMx
    Eigen::MatrixXd CovarianceMatrix(const double exposure, EExposureType expotype=kPOT);

    /// Wrapper for \ref CalcBiasMx
    Eigen::MatrixXd BiasMatrix(const double exposure, EExposureType expotype=kPOT);

    virtual void FillSingle(double x, double w, int universeId) override;

    virtual void FillEnsemble(double x, const std::vector<double>& ws) override;

    virtual void FillPOT(double pot) override;

    virtual void FillLivetime(double livetime) override;

    void Scale(double c);

    EnsembleSpectrum& operator+=(const EnsembleSpectrum& rhs);
    EnsembleSpectrum operator+(const EnsembleSpectrum& rhs) const;

    EnsembleSpectrum& operator-=(const EnsembleSpectrum& rhs);
    EnsembleSpectrum operator-(const EnsembleSpectrum& rhs) const;

    EnsembleSpectrum& operator*=(const EnsembleRatio& rhs);
    EnsembleSpectrum operator*(const EnsembleRatio& rhs) const;

    EnsembleSpectrum& operator/=(const EnsembleRatio& rhs);
    EnsembleSpectrum operator/(const EnsembleRatio& rhs) const;

    void SaveTo(TDirectory* dir, const std::string& name) const;
    static std::unique_ptr<EnsembleSpectrum> LoadFrom(TDirectory* dir,
                                                      const std::string& name);

    unsigned int NDimensions() const{return fAxis.NDimensions();}
    std::vector<std::string> GetLabels() const {return fAxis.GetLabels();}
    std::vector<Binning> GetBinnings() const {return fAxis.GetBinnings();}

  protected:
    friend class EnsembleReweightableSpectrum;

    /// Helper for LoadFrom()
    EnsembleSpectrum(const FitMultiverse* multiverse,
                     const Hist&& hist,
                     double pot,
                     double livetime,
                     const LabelsAndBins& axis);

    void CheckMultiverses(const FitMultiverse& rhs,
                          const std::string& func) const;

    /// Helper for operator+= and operator-=
    EnsembleSpectrum& PlusEqualsHelper(const EnsembleSpectrum& rhs, int sign,
                                       const std::string& func);

    const FitMultiverse* fMultiverse;

    Hist fHist;
    double fPOT;
    double fLivetime;
    LabelsAndBins fAxis;
  };

  // Commutative
  inline EnsembleSpectrum operator*(const EnsembleRatio& lhs, const EnsembleSpectrum& rhs){return rhs*lhs;}
  inline EnsembleSpectrum operator/(const EnsembleRatio& lhs, const EnsembleSpectrum& rhs){return rhs/lhs;}

  void DrawErrorBand(TH1* nom,
                     TGraphAsymmErrors* band,
                     int bandCol = -1,
                     double alpha = 1);
}