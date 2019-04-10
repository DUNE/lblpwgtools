#pragma once

#include "CAFAna/Fit/IFitter.h"

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Experiment/IExperiment.h"

#include "TMatrixDSymfwd.h"
#include "Math/Minimizer.h"

#include <chrono>
#include <memory>

namespace ana
{

/// Perform MINUIT fits in one or two dimensions
  class MinuitFitter
      : public IFitter, public ROOT::Math::IGradientFunctionMultiDim
  {
    public:
      enum FitOpts
      {
        // You must select one of these. The first three codes match the settings
        // used by migrad. The fourth is a custom minimizer.
        kFast = 0,
        kNormal = 1,
        kCareful = 2,
        kGradDesc = 3,

        // Allow bitmask operations to extract these first four options
        kPrecisionMask = 3,

        // The remaining options are independent, you may xor them in with one of the above

        // improve the chances of escaping from invalid minima
        kIncludeSimplex = 4,

        // You may optionally specify these to improve the final error estimates
        kIncludeHesse = 8,
        kIncludeMinos = 16,

        // try fitting the systs before the oscillation parameters.  might speed up your fit
        kPrefitSysts = 32
      };

      void SetFitOpts(FitOpts opts);

      MinuitFitter(const IExperiment *expt,
                   std::vector<const IFitVar *> vars,
                   std::vector<const ISyst *> systs = {},
                   FitOpts opts = kNormal);

      ~MinuitFitter();

      /// Return the fit covariance
      TMatrixDSym *GetCovariance() { return this->fCovar; }

      /// covariance matrix status (0 = not valid, 1 approximate, 2, full but made
      /// pos def, 3 accurate and not pos def)
      int GetCovarianceStatus() { return this->fCovarStatus; }

      /// Return the fit names
      std::vector<std::string> GetParamNames() { return this->fParamNames; }

      /// Return the prefit values
      std::vector<double> GetPreFitValues() { return this->fPreFitValues; }

      /// Return the prefit errors
      std::vector<double> GetPreFitErrors() { return this->fPreFitErrors; }

      /// Return the postfit values
      std::vector<double> GetPostFitValues() { return this->fPostFitValues; }

      /// Return the postfit errors
      std::vector<double> GetPostFitErrors() { return this->fPostFitErrors; }

      /// Return the postfit errors
      std::vector<double> GetCentralValues() { return this->fCentralValues; }

      /// Return the minos errors
      std::vector<std::pair<double, double>> GetMinosErrors() { return this->fMinosErrors; }

      /// Return number of function calls
      int GetNFCN() { return this->fNEval; }

      /// Return edm form the fit
      double GetEDM() { return this->fEdm; }

      /// Say whether the fit was good
      bool GetIsValid() { return this->fIsValid; }

      /// Evaluate the log-likelihood, as required by MINUT interface
      virtual double DoEval(const double *pars) const override;

      // Part of the fitter interface
      virtual unsigned int NDim() const override { return fVars.size() + fSysts.size(); }

      void Gradient(const double *x, double *grad) const override;

      virtual double DoDerivative(const double *x,
                                  unsigned int icoord) const override
      {
        std::cout << "Fitter::DoDerivative() not implemented" << std::endl;
        abort();
      }

      MinuitFitter *Clone() const override
      {
        std::cout << "MinuitFitter::Clone() not implemented" << std::endl;
        abort();
      }

      // TODO unused
      bool CheckGradient() const { return (fFitOpts & kPrecisionMask) != kFast; }

    protected:
      /// Stuff the parameters into the calculator and/or syst shifts object
      void DecodePars(double const *pars) const;

      /// Concrete instance of IFitSummary for use in MinuitFitter
      class MinuitFitSummary : public IFitSummary
      {
        public:
          MinuitFitSummary(std::unique_ptr<ROOT::Math::Minimizer> && minimizer)
            : fMinimizer(std::move(minimizer))
          {
            assert (fMinimizer.get() && "Can't make a MinuitFitSummary from a null ROOT::Math::Minimizer");
          }

          bool IsBetterThan(const IFitSummary* other) const override
          {
            if (!other)
              return true;

            if (const auto mnFitSummary = dynamic_cast<const MinuitFitSummary*>(other))
              return EvalMetricVal() < mnFitSummary->EvalMetricVal(); 
            else
            {
              assert(false && "Can't compare a MinuitFitSummary to another kind of IFitSummary");
              return false;  // prevent compiler warnings
            }
          }

          double EvalMetricVal() const override
          {
            return fMinimizer->MinValue();
          }

          const ROOT::Math::Minimizer * GetMinimizer() const { return fMinimizer.get(); }

        private:
          /// the minimizer instance
          std::unique_ptr<ROOT::Math::Minimizer> fMinimizer;
      };
      /// Helper for \ref FitHelper
      std::unique_ptr<IFitSummary> FitHelperSeeded(osc::IOscCalcAdjustable *seed,
                                                   SystShifts &systSeed,
                                                   Verbosity verb) const override;

      void UpdatePostFit(const IFitSummary * fitSummary) const override;

      /// Intended to be called only once (from constructor) to initialize
      /// fSupportsDerivatives
      bool SupportsDerivatives() const;

      mutable osc::IOscCalcAdjustable *fCalc;
      const IExperiment *fExpt;

      FitOpts fFitOpts;

      bool fSupportsDerivatives;

      mutable int fNEval = 0;
      mutable int fNEvalGrad = 0;
      mutable int fNEvalFiniteDiff = 0;

      // Some information for post-fit evaluation if necessary
      mutable double fEdm = -1;
      mutable bool fIsValid = false;
      mutable TMatrixDSym *fCovar;
      mutable bool fCovarStatus;
      mutable std::vector<std::string> fParamNames;
      mutable std::vector<double> fPreFitValues;
      mutable std::vector<double> fPreFitErrors;
      mutable std::vector<std::string> fLastParamNames;
      mutable std::vector<double> fLastCentralValues;
      mutable std::vector<double> fLastPreFitValues;
      mutable std::vector<double> fLastPreFitErrors;
      mutable std::vector<double> fPostFitValues;
      mutable std::vector<double> fPostFitErrors;
      mutable std::vector<double> fCentralValues;
      mutable std::vector<std::pair<double, double>> fMinosErrors;
      mutable std::vector<std::pair<double, double>>
          fTempMinosErrors; // Bit of a hack

      mutable std::chrono::time_point<std::chrono::system_clock> fLastTP;
      mutable std::chrono::time_point<std::chrono::system_clock> fBeginTP;

      mutable Verbosity fVerb;
  };

  // Modern C++ thinks that enum | enum == int. Make things work like we expect
  // for this bitmask.
  inline MinuitFitter::FitOpts operator|(MinuitFitter::FitOpts a, MinuitFitter::FitOpts b)
  {
    return MinuitFitter::FitOpts(int(a) | int(b));
  }

}
