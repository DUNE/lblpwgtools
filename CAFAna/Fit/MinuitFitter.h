#pragma once

#include "CAFAna/Fit/IFitter.h"

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Experiment/IChiSqExperiment.h"

#include "Minuit2/FCNGradientBase.h"

namespace ana
{

/// Perform MINUIT fits in one or two dimensions
  class MinuitFitter
    : public IFitter, public ROOT::Math::IGradientFunctionMultiDim
  {
    public:
      enum Verbosity { kQuiet = 0, kVerbose = 1, kTurbose = 2 };

      enum FitOpts {
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

      MinuitFitter(const IChiSqExperiment *expt,
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

      Fitter *Clone() const override
      {
        std::cout << "Fitter::Clone() not implemented" << std::endl;
        abort();
      }

      // TODO unused
      bool CheckGradient() const override { return (fFitOpts & kPrecisionMask) != kFast; }

    protected:
      /// Stuff the parameters into the calculator and/or syst shifts object
      void DecodePars(const std::vector<double>& pars, osc::IOscCalculatorAdjustable * calc) const;

      /// Helper for \ref FitHelper
      double FitHelperSeeded(osc::IOscCalculatorAdjustable *seed,
                             SystShifts &systSeed,
                             Verbosity verb) const override;

      /// Intended to be called only once (from constructor) to initialize
      /// fSupportsDerivatives
      bool SupportsDerivatives() const;

      mutable osc::IOscCalculatorAdjustable * fCalc;
      const IChiSqExperiment* fExpt;

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
  };

// Modern C++ thinks that enum | enum == int. Make things work like we expect
// for this bitmask.
inline Fitter::Precision operator|(Fitter::Precision a, Fitter::Precision b) {
  return Fitter::Precision(int(a) | int(b));
}

