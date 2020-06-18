#pragma once

#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Extrap/IExtrap.h"
#include <Eigen/Dense>
namespace ana
{
  //  class IExtrap;

  /// Take the output of an extrapolation and oscillate it as required
  class PredictionExtrap: public IPrediction
  {
    public:
      /// Takes ownership of \a extrap
      PredictionExtrap(IExtrap* extrap);
      virtual ~PredictionExtrap();

      // un-hide inherited method stubs so we don't get warnings from the compiler
      using IPrediction::Predict;
      using IPrediction::PredictComponent;
      using IPrediction::PredictSyst;

      Spectrum     Predict(osc::IOscCalculator* calc) const override;
      SpectrumStan Predict(osc::IOscCalculatorStan* calc) const override;

      Spectrum PredictComponent(osc::IOscCalculator* calc,
                                Flavors::Flavors_t flav,
                                Current::Current_t curr,
                                Sign::Sign_t sign) const override;
      SpectrumStan PredictComponent(osc::IOscCalculatorStan* calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

      OscillatableSpectrum ComponentCC(int from, int to) const override;
      Spectrum ComponentNC() const override;

    virtual void SaveTo(TDirectory* dir, const std::string& name) const override;
    static std::unique_ptr<PredictionExtrap> LoadFrom(TDirectory* dir, const std::string& name);

      PredictionExtrap() = delete;

      IExtrap* GetExtrap() const {return fExtrap;}
    protected:
      /// Templated helper function called by the non-templated versions
      template <typename U, typename T>
      U _PredictComponent(osc::_IOscCalculator<T>* calc,
                          Flavors::Flavors_t flav,
                          Current::Current_t curr,
                          Sign::Sign_t sign) const;

      IExtrap* fExtrap;

    private:
      template <typename T>
      Eigen::Matrix<T,Eigen::Dynamic,1> OscillatedEigen(osc::_IOscCalculator<T> * calc,
                                                        const Eigen::MatrixXd & m,
                                                        int from,
                                                        int to,
                                                        double POT,
                                                        double fPOT) const;
      Eigen::MatrixXd mNueSurv      ;
      Eigen::MatrixXd mAntiNueSurv  ;
      Eigen::MatrixXd mNumuApp      ;
      Eigen::MatrixXd mAntiNumuApp  ;
      Eigen::MatrixXd mTauFromE     ;
      Eigen::MatrixXd mAntiTauFromE ;

      Eigen::MatrixXd mNueApp       ;
      Eigen::MatrixXd mAntiNueApp   ;
      Eigen::MatrixXd mNumuSurv     ;
      Eigen::MatrixXd mAntiNumuSurv ;
      Eigen::MatrixXd mTauFromMu    ;
      Eigen::MatrixXd mAntiTauFromMu;

      Eigen::VectorXd vNC;

      int nbinsx;

      double potNueSurv      ;
      double potAntiNueSurv  ;
      double potNumuApp      ;
      double potAntiNumuApp  ;
      double potTauFromE     ;
      double potAntiTauFromE ;

      double potNueApp       ;
      double potAntiNueApp   ;
      double potNumuSurv     ;
      double potAntiNumuSurv ;
      double potTauFromMu    ;
      double potAntiTauFromMu;

  };
}
