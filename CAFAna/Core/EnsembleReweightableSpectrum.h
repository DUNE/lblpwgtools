#pragma once

#include "CAFAna/Core/EnsembleSpectrum.h"

#include "cafanacore/ReweightableSpectrum.h"

#include <string>

class TDirectory;

namespace ana
{
  class FitMultiverse;

  template<class RecT> class _Var;

  /// %Spectrum with the value of a second variable, allowing for reweighting
  class EnsembleReweightableSpectrum: public IValuePairEnsembleSink
  {
  public:
    EnsembleReweightableSpectrum(IValuePairEnsembleSource& src,
                                 const LabelsAndBins& recoAxis,
                                 const LabelsAndBins& trueAxis);

    template<class RecT>
    EnsembleReweightableSpectrum(_IRecordEnsembleSource<RecT>& src,
                                 const _HistAxis<_Var<RecT>>& recoAxis,
                                 const _HistAxis<_Var<RecT>>& trueAxis)
      : EnsembleReweightableSpectrum(src.GetVars(recoAxis.GetVar1D(), trueAxis.GetVar1D()), recoAxis, trueAxis)
    {
    }

    virtual ~EnsembleReweightableSpectrum();
    // why is copy constructor not working?
    //EnsembleReweightableSpectrum(const EnsembleReweightableSpectrum& rhs);
    EnsembleReweightableSpectrum& operator=(const EnsembleReweightableSpectrum& rhs);

    virtual void FillSingle(double x, double y, double w, int universeId) override;

    virtual void FillEnsemble(double x, double y, const std::vector<double>& ws) override;

    virtual void FillPOT(double pot) override;
    virtual void FillLivetime(double livetime) override;

    double POT() const{return fPOT;}
    double Livetime() const{return fLivetime;}

    EnsembleSpectrum UnWeighted() const;

    EnsembleSpectrum WeightingVariable() const;

    /// Reco spectrum with truth weights applied
    EnsembleSpectrum WeightedBy(const Ratio& weights) const;

// Not implemented for now
    /// Rescale bins so that \ref WeightingVariable will return \a target
//    void ReweightToTrueSpectrum(const EnsembleSpectrum& target);
    /// Recale bins so that \ref Unweighted will return \a target
//    void ReweightToRecoSpectrum(const EnsembleSpectrum& target);

    // Arithmetic operators are as if these are unlike samples, each a
    // contribution to one total, not seperate sources of stats for the same
    // sample.
    EnsembleReweightableSpectrum& operator+=(const EnsembleReweightableSpectrum& rhs);
    EnsembleReweightableSpectrum operator+(const EnsembleReweightableSpectrum& rhs) const;

    EnsembleReweightableSpectrum& operator-=(const EnsembleReweightableSpectrum& rhs);
    EnsembleReweightableSpectrum operator-(const EnsembleReweightableSpectrum& rhs) const;

    void SaveTo(TDirectory* dir, const std::string& name) const;

    static std::unique_ptr<EnsembleReweightableSpectrum> LoadFrom(TDirectory* dir, const std::string& name);

    unsigned int NDimensions() const{return fAxisX.NDimensions();}

    unsigned int NUniverses() const;
    ReweightableSpectrum Nominal() const {return Universe(0);}
    ReweightableSpectrum Universe(unsigned int univIdx) const;

    // TODO consider naming confusion with Universe() above
    const FitMultiverse& GetMultiverse() const {return *fMultiverse;}

    const std::vector<std::string>& GetLabels() const {return fAxisX.GetLabels();}
    const std::vector<Binning>& GetBinnings() const {return fAxisX.GetBinnings();}
    const std::vector<Binning>& GetTrueBinnings() const {return fAxisY.GetBinnings();}

  protected:
    // Derived classes can be trusted take care of their own construction
    EnsembleReweightableSpectrum(const FitMultiverse* multiverse,
                                 const LabelsAndBins& axisX,
                                 const LabelsAndBins& axisY)
      : fMultiverse(multiverse),
        fPOT(0), fLivetime(0),
        fAxisX(axisX), fAxisY(axisY)
    {
    }

    // Helper for LoadFrom
    EnsembleReweightableSpectrum(const FitMultiverse* multiverse,
                                 const Eigen::MatrixXd&& mat,
                                 const LabelsAndBins& recoAxis,
                                 const LabelsAndBins& trueAxis,
                                 double pot, double livetime);


    EnsembleReweightableSpectrum& PlusEqualsHelper(const EnsembleReweightableSpectrum& rhs, int sign, const std::string& func);

    void _SaveTo(TDirectory* dir,
                 const std::string& name,
                 const std::string& type) const;

    void CheckMultiverses(const FitMultiverse& rhs, const std::string& func) const;

    const FitMultiverse* fMultiverse;

    Eigen::MatrixXd fMat;
    double fPOT;
    double fLivetime;

    LabelsAndBins fAxisX, fAxisY;
  };
}