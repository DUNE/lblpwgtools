#include "CAFAna/Fit/MinuitFitter.h"

#include "CAFAna/Fit/GradientDescent.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Experiment/IExperiment.h"

#include "OscLib/IOscCalc.h"
#include "CAFAna/Core/MathUtil.h"

#include "TError.h"
#include "TGraph.h"
#include "TH1.h"
#include "TMatrixDSym.h"

// ROOT fitter interface
#include "Fit/Fitter.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

#include "Minuit2/StackAllocator.h"

#include <cassert>
#include <iostream>

namespace ana
{
  // Minuit calls this at some point, and it creates a static. Which doesn't
  // like happening on multiple threads at once. So do it upfront while we're
  // still single-threaded.
  struct MinuitStaticInit
  {
    MinuitStaticInit(){ROOT::Minuit2::StackAllocatorHolder::Get();}
  } gMinuitStaticInit;

  //----------------------------------------------------------------------
  MinuitFitter::MinuitFitter(const IExperiment *expt,
                             std::vector<const IFitVar *> vars,
                             std::vector<const ISyst *> systs,
                             FitOpts opts)
    : IFitter(vars, systs),
      fExpt(expt),
      fFitOpts(opts),
      fSupportsDerivatives(SupportsDerivatives()),
      fCovar(0),
      fCovarStatus(-1)
  {
  }

  //----------------------------------------------------------------------
  MinuitFitter::~MinuitFitter()
  {
  }

  //----------------------------------------------------------------------
  bool MinuitFitter::SupportsDerivatives() const
  {
    // Most of this framework was torn out. But keep the hooks here in case we
    // want to replace the derivatives with stan.
    return false;
  }

  //----------------------------------------------------------------------
  std::unique_ptr<IFitter::IFitSummary>
  MinuitFitter::FitHelperSeeded(osc::IOscCalcAdjustable *seed,
                                SystShifts &systSeed,
                                Verbosity verb) const
  {
    fVerb = verb;
    if (fFitOpts & kIncludeSimplex)
      std::cout << "Simplex option specified but not implemented. Ignored."
                << std::endl;

    if((fFitOpts & kPrefitSysts) && !fVars.empty()){
      // Update systSeed to the best syst values, while holding the oscillation
      // parameters fixed
      MinuitFitter prefit(fExpt, {}, fSysts, fFitOpts);
      prefit.Fit(seed, systSeed, verb);
      // Then continue with the full fit as usual
    }

    // Why, when this is called for each seed?
    fCalc = seed;
    *fShifts = systSeed;

    std::unique_ptr<ROOT::Math::Minimizer> mnMin;

    if ((fFitOpts & kPrecisionMask) == kGradDesc)
      mnMin = std::make_unique<GradientDescent>(*this);
    else{
      mnMin = std::unique_ptr<ROOT::Math::Minimizer>(
        ROOT::Math::Factory::CreateMinimizer("Minuit2", "Combined"));
      // ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "BFGS2"));
      mnMin->SetStrategy(int(fFitOpts & kPrecisionMask));
    }
    mnMin->SetMaxFunctionCalls(1E9);
    mnMin->SetMaxIterations(1E9);

    static double tol =
      getenv("FIT_TOLERANCE") != 0 ? atof(getenv("FIT_TOLERANCE")) : 1;
    static double prec =
      getenv("FIT_PRECISION") != 0 ? atof(getenv("FIT_PRECISION")) : 1e-15;

    // Please give us all the decimal places
    mnMin->SetTolerance(tol);
    mnMin->SetPrecision(prec);

    fLastParamNames.clear();
    fLastPreFitValues.clear();
    fLastPreFitErrors.clear();
    fLastCentralValues.clear();

    for (const IFitVar *v: fVars)
    {
      const double val = v->GetValue(seed);
      // name, value, error
      mnMin->SetVariable(mnMin->NFree(), v->ShortName(), val,
                         val ? fabs(val / 2) : .1);
      fLastParamNames.push_back(v->ShortName());
      fLastPreFitValues.push_back(val);
      fLastPreFitErrors.push_back(val ? val / 2 : .1);
      fLastCentralValues.push_back(0);
    }
    // One way this can go wrong is if two variables have the same ShortName
    assert(mnMin->NFree() == fVars.size());
    for (const ISyst *s : fSysts)
    {
      const double val = systSeed.GetShift(s);
      // name, value, error
      mnMin->SetVariable(mnMin->NFree(), s->ShortName(), val, 1);
      fLastParamNames.push_back(s->ShortName());
      fLastPreFitValues.push_back(val);
      fLastPreFitErrors.push_back(1);
      fLastCentralValues.push_back(s->Central());
    }
    // One way this can go wrong is if two variables have the same ShortName
    assert(mnMin->NFree() == fVars.size() + fSysts.size());

    if (fSupportsDerivatives) {
      mnMin->SetFunction(*this);
    } else {
      mnMin->SetFunction((ROOT::Math::IBaseFunctionMultiDim &)*this);
    }

    if (fVerb <= Verbosity::kQuiet) {
      mnMin->SetPrintLevel(0);
    }

    if (!mnMin->Minimize()) {
      std::cout << "*** ERROR: minimum is not valid ***" << std::endl;
      std::cout << "*** Precision: " << mnMin->Precision() << std::endl;

      std::cout << "-- Stopped at: \n";
      for (uint i = 0; i < mnMin->NFree(); ++i) {
        std::cout << "\t" << mnMin->VariableName(i) << " = " << mnMin->X()[i]
                  << "\n";
      }
    }

    if (fFitOpts & kIncludeHesse) {
      std::cout << "[FIT]: It's Hesse o'clock" << std::endl;
      mnMin->Hesse();
    }

    if (fFitOpts & kIncludeMinos) {
      // std::cout << "It's minos time" << std::endl;
      fTempMinosErrors.clear();
      for (uint i = 0; i < mnMin->NDim(); ++i) {
        double errLow = 0, errHigh = 0;
        mnMin->GetMinosError(i, errLow, errHigh);
        std::cout << i << "/" << mnMin->NDim() << " " << fParamNames[i] << ": "
                  << errLow << ", +" << errHigh << " (" << mnMin->Errors()[i]
                  << ")" << std::endl;
        fTempMinosErrors.push_back(std::make_pair(errLow, errHigh));
      }
    }

    const double* minvec = mnMin->X();

    // Store results back to the "seed" variable
    for (unsigned int i = 0; i < fVars.size(); ++i){
      fVars[i]->SetValue(seed, minvec[i]);
    }
    // Store systematic results back into "systSeed"
    for (unsigned int j = 0; j < fSysts.size(); ++j){
      systSeed.SetShift(fSysts[j], minvec[fVars.size() + j]);
    }

    return std::make_unique<MinuitFitSummary>(std::move(mnMin));
  }

  //----------------------------------------------------------------------
  void MinuitFitter::SetFitOpts(FitOpts opts)
  {
    if ((opts & kPrecisionMask) == kGradDesc &&
        !fSysts.empty() &&
        !fSupportsDerivatives)
    {
      std::cout
        << "Warning - not setting precision to kGradDesc, since analytic gradients are not supported by this experiment"
        << std::endl;
      return;
    }

    fFitOpts = opts;
  }

  //----------------------------------------------------------------------
  double MinuitFitter::DoEval(const double *pars) const
  {
    ++fNEval;

    DecodePars(pars); // Updates fCalc and fShifts

    // Have to re-fetch the FitVar values because DecodePars() will have
    // truncated values to the physical range where necessary.
    double penalty = 0;
    for (unsigned int i = 0; i < fVars.size(); ++i)
    {
      penalty += fVars[i]->Penalty(pars[i], fCalc);
    }
    for (unsigned int j = 0; j < fSysts.size(); ++j) {
      penalty += fSysts[j]->Penalty(pars[fVars.size() + j]);
    }

    if ((fVerb >= kVerbose) && !(fNEval % (fVerb >= kTurbose ? 1 : 10000))) {

      std::chrono::time_point<std::chrono::system_clock> now =
          std::chrono::system_clock::now();

      std::time_t now_time = std::chrono::system_clock::to_time_t(now);

      std::cerr << "[FIT]: NEval: " << fNEval
                << ", LH: {samp: " << fExpt->ChiSq(fCalc, *fShifts)
                << ", pen: " << penalty << "}\n\tT += "
                << std::chrono::duration_cast<std::chrono::seconds>(now - fLastTP)
                       .count()
                << " s, = ";

      if (std::chrono::duration_cast<std::chrono::seconds>(now - fBeginTP)
              .count() > 60) {
        std::cout << std::chrono::duration_cast<std::chrono::minutes>(now -
                                                                      fBeginTP)
                         .count()
                  << " m. ";
      } else {
        std::cout << std::chrono::duration_cast<std::chrono::seconds>(now -
                                                                      fBeginTP)
                         .count()
                  << " s. ";
      }

      std::cout << BuildLogInfoString();
      fLastTP = now;
    }

    return fExpt->ChiSq(fCalc, *fShifts) + penalty;
  }

  //----------------------------------------------------------------------
  void MinuitFitter::Gradient(const double *pars, double *ret) const
  {
    ++fNEvalGrad;

    // All this logic was torn out, but this is where you would reimplement it
    // in terms of stan.
    abort();
  }

  //----------------------------------------------------------------------
  void MinuitFitter::DecodePars(double const *pars) const
  {
    if (fVars.size() > 0)
    {
      assert(fCalc);
      for(unsigned int i = 0; i < fVars.size(); ++i){
        auto val = pars[i];
        fVars[i]->SetValue(fCalc, val);
      }
    }

    for(unsigned int j = 0; j < fSysts.size(); ++j){
      auto val = pars[fVars.size()+j];
      fShifts->SetShift(fSysts[j], val);
    }
  }

   //----------------------------------------------------------------------
  void MinuitFitter::UpdatePostFit(const IFitter::IFitSummary *fitSummary) const
  {
    // Get them as set by the last seed fit
    fParamNames = fLastParamNames;
    fPreFitValues = fLastPreFitValues;
    fPreFitErrors = fLastPreFitErrors;
    fCentralValues = fLastCentralValues;

    fMinosErrors = fTempMinosErrors;

    const auto thisMin = dynamic_cast<const MinuitFitSummary*>(fitSummary)->GetMinimizer();

    // Now save postfit
    fPostFitValues =
        std::vector<double>(thisMin->X(), thisMin->X() + thisMin->NDim());
    fPostFitErrors = std::vector<double>(thisMin->Errors(),
                                         thisMin->Errors() + thisMin->NDim());

    fEdm = thisMin->Edm();
    fIsValid = !thisMin->Status();

    delete fCovar;
    fCovar = new TMatrixDSym(thisMin->NDim());

    for (unsigned int row = 0; row < thisMin->NDim(); ++row) {
      for (unsigned int col = 0; col < thisMin->NDim(); ++col) {
        (*fCovar)(row, col) = thisMin->CovMatrix(row, col);
      }
    }

  }
}
