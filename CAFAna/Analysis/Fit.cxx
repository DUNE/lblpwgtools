#include "CAFAna/Analysis/Fit.h"

#include "CAFAna/Analysis/GradientDescent.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Experiment/IExperiment.h"

#include "OscLib/func/IOscCalculator.h"
#include "Utilities/func/MathUtil.h"

#include "TError.h"
#include "TGraph.h"
#include "TH1.h"
#include "TMatrixDSym.h"

// ROOT fitter interface
#include "Fit/Fitter.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

#include <cassert>
#include <iostream>

namespace ana {
//----------------------------------------------------------------------
double SimpleFOM(const Spectrum &obs, const Spectrum &unosc, double pot) {
  if (pot == 0)
    pot = obs.POT();

  std::unique_ptr<TH1> oh(obs.ToTH1(pot));
  std::unique_ptr<TH1> bh(unosc.ToTH1(pot));
  assert(oh->GetNbinsX() == bh->GetNbinsX());

  double fomSq = 0;

  // Combine s/sqrt(s+b) in quadrature between bins
  for (int i = 0; i < oh->GetNbinsX(); ++i) {
    const double o = oh->GetBinContent(i);
    const double b = bh->GetBinContent(i);
    const double s = o - b;

    if (s <= 0)
      continue;

    fomSq += util::sqr(s) / (s + b);
  }

  return sqrt(fomSq);
}

//----------------------------------------------------------------------
Fitter::Fitter(const IExperiment *expt, std::vector<const IFitVar *> vars,
               std::vector<const ISyst *> systs, Precision prec)
    : fExpt(expt), fVars(vars), fSysts(systs), fPrec(prec), fCalc(0),
      fSupportsDerivatives(SupportsDerivatives()), fCovar(0), fCovarStatus(-1) {
}

//----------------------------------------------------------------------
std::vector<Fitter::SeedPt>
Fitter::ExpandSeeds(const SeedList &seedPts,
                    const std::vector<SystShifts> &systSeedPts) const {
  std::vector<SeedPt> ret;
  for (Seed seed : seedPts.GetSeeds())
    ret.push_back(SeedPt(seed, {}));

  // Now duplicate as many times as required for the syst seeds
  if (!systSeedPts.empty()) {
    std::vector<SeedPt> newret;
    for (const SystShifts &s : systSeedPts) {
      for (SeedPt pt : ret) {
        pt.shift = s;
        newret.push_back(pt);
      }
    }
    ret = newret;
  }

  return ret;
}

//----------------------------------------------------------------------
bool Fitter::SupportsDerivatives() const {
  // Provide an Opt-out in case something goes wrong
  if (getenv("CAFANA_DISABLE_DERIVATIVES") &&
      bool(atoi(getenv("CAFANA_DISABLE_DERIVATIVES")))) {
    return false;
  }

  // No point using derivatives for FitVars only, we do finite differences,
  // probably worse than MINUIT would.
  if (fSysts.empty())
    return false;

  // Otherwise, do the minimal trial to see if the experiment will return a
  // gradient.
  std::unordered_map<const ISyst *, double> dchi = {{fSysts[0], 0}};
  osc::NoOscillations calc;
  fExpt->Derivative(&calc, SystShifts::Nominal(), dchi);

  return !dchi.empty();
}

//----------------------------------------------------------------------
std::unique_ptr<ROOT::Math::Minimizer>
Fitter::FitHelperSeeded(osc::IOscCalculatorAdjustable *seed,
                        SystShifts &systSeed) const {
  // Why, when this is called for each seed?
  fCalc = seed;
  fShifts = systSeed;

  if (fPrec & kIncludeSimplex)
    std::cout << "Simplex option specified but not implemented. Ignored."
              << std::endl;

  std::unique_ptr<ROOT::Math::Minimizer> mnMin;

  if ((fPrec & kAlgoMask) == kGradDesc) {
    mnMin = std::make_unique<GradientDescent>(*this);
  } else {
    mnMin = std::unique_ptr<ROOT::Math::Minimizer>(
        ROOT::Math::Factory::CreateMinimizer("Minuit2", "Combined"));
    // ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "BFGS2"));
    mnMin->SetStrategy(int(fPrec & kAlgoMask));
  }
  mnMin->SetMaxFunctionCalls(1E8);
  mnMin->SetMaxIterations(1E8);

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

  for (const IFitVar *v : fVars) {
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
  for (const ISyst *s : fSysts) {
    const double val = systSeed.GetShift(s);
    // name, value, error
    mnMin->SetVariable(mnMin->NFree(), s->ShortName(), val, 0.1);
    fLastParamNames.push_back(s->ShortName());
    fLastPreFitValues.push_back(val);
    fLastPreFitErrors.push_back(0.1);
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

  if ((fPrec & kIncludeHesse) && mnMin->NFree() > 0) {
    std::cout << "[FIT]: It's Hesse o'clock" << std::endl;
    mnMin->Hesse();
  }

  if (fPrec & kIncludeMinos) {
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

  return mnMin;
}

//----------------------------------------------------------------------
double Fitter::FitHelper(osc::IOscCalculatorAdjustable *initseed,
                         SystShifts &bestSysts, const SeedList &seedPts,
                         const std::vector<SystShifts> &systSeedPts) const {
  const std::vector<SeedPt> &pts = ExpandSeeds(seedPts.GetSeeds(), systSeedPts);

  double minchi = 1e10;
  std::vector<double> bestFitPars, bestSystPars;

  // Used as a starting point for all seeds
  SystShifts inputShift = SystShifts(bestSysts);

  for (const SeedPt &pt : pts) {
    osc::IOscCalculatorAdjustable *seed = initseed->Copy();
    osc::IOscCalculatorAdjustable *seed_orig = initseed->Copy();

    pt.fitvars.ResetCalc(seed);

    // By default take values from the syst seed
    SystShifts shift = pt.shift;

    // If there were no syst seeds, start with the input values
    if (systSeedPts.empty()) shift = SystShifts(inputShift);

    std::unique_ptr<ROOT::Math::Minimizer> thisMin =
        FitHelperSeeded(seed, shift);

    // Check whether this is the best minimum we've found so far
    if (thisMin->MinValue() < minchi) {
      minchi = thisMin->MinValue();

      // Copy back best systs (this is a bit dodgy)
      bestSysts = shift;

      // Get them as set by the last seed fit
      fParamNames = fLastParamNames;
      fPreFitValues = fLastPreFitValues;
      fPreFitErrors = fLastPreFitErrors;
      fCentralValues = fLastCentralValues;

      fMinosErrors = fTempMinosErrors;

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

      // Store the best fit values of all the parameters we know are being
      // varied.
      bestFitPars.clear();
      bestSystPars.clear();
      for (unsigned int i = 0; i < fVars.size(); ++i)
        bestFitPars.push_back(fPostFitValues[i]);
      for (unsigned int j = 0; j < fSysts.size(); ++j)
        bestSystPars.push_back(fPostFitValues[fVars.size() + j]);
    }
    delete seed;
    delete seed_orig;
  }

  // Stuff the results of the actual best fit back into the seeds
  assert(bestFitPars.size() == fVars.size());
  for (unsigned int i = 0; i < fVars.size(); ++i)
    fVars[i]->SetValue(initseed, bestFitPars[i]);
  assert(bestSystPars.size() == fSysts.size());
  for (unsigned int i = 0; i < fSysts.size(); ++i)
    bestSysts.SetShift(fSysts[i], bestSystPars[i]);

  return minchi;
}

//----------------------------------------------------------------------
void Fitter::SetPrecision(Precision prec) {
  if ((prec & kAlgoMask) == kGradDesc && !fSysts.empty() &&
      !fSupportsDerivatives) {
    std::cout << "Warning - not setting precision to kGradDesc, since analytic "
                 "gradients are not supported by this experiment"
              << std::endl;
    return;
  }

  fPrec = prec;
}

//----------------------------------------------------------------------
double Fitter::Fit(osc::IOscCalculatorAdjustable *seed, SystShifts &bestSysts,
                   const SeedList &seedPts,
                   const std::vector<SystShifts> &systSeedPts,
                   Verbosity verb) const {

  fVerb = verb;

  if (getenv("CAFANA_FIT_TURBOSE") &&
      bool(atoi(getenv("CAFANA_FIT_TURBOSE")))) {
    fVerb = kTurbose;
  }

  // Fits with no osc calculator shouldn't be trying to optimize any
  // oscilation parameters...
  assert(seed || fVars.empty());

  /* TODO TODO TODO
  for(const std::vector<Seed>& seed: seedPts.GetSeeds()){
    if(std::find(fVars.begin(), fVars.end(), it.first) == fVars.end()){
      std::cout << "ERROR Fitter::Fit() trying to seed '"
                << it.first->ShortName()
                << "' which is not part of the fit." << std::endl;
       abort();
    }
  }
  */
  for (const auto &it : systSeedPts) {
    for (const ISyst *s : it.ActiveSysts()) {
      if (std::find(fSysts.begin(), fSysts.end(), s) == fSysts.end()) {
        std::cout << "ERROR Fitter::Fit() trying to seed '" << s->ShortName()
                  << "' which is not part of the fit." << std::endl;
        abort();
      }
    }
  }

  if (fVerb >= kVerbose) {
    fBeginTP = std::chrono::system_clock::now();
    fLastTP = fBeginTP;
    // std::cout << "[FIT]: Finding best fit for";
    // for (const IFitVar *v : fVars)
    //   std::cout << " " << v->ShortName();
    // for (const ISyst *s : fSysts)
    //   std::cout << " " << s->ShortName();
    // if (fSupportsDerivatives)
    //   std::cout << " using analytic derivatives";
    // std::cout << "\n" << std::endl;
  }

  // Do all the actual work. This wrapper function is just so we can have
  // better control of printing.
  const double chi = FitHelper(seed, bestSysts, seedPts, systSeedPts);

  if (fVerb >= kVerbose) {
    std::cout << "[FIT]: Best fit:\n";
    for (const IFitVar *v : fVars) {
      std::cout << "\t" << v->ShortName() << " = " << v->GetValue(seed)
                << std::endl;
    }
    for (const ISyst *s : fSysts) {
      std::cout << "\t" << s->ShortName() << " = " << bestSysts.GetShift(s)
                << std::endl;
    }
    std::cout << "[FIT]: What up in the LHood = " << chi << std::endl;

    std::cout << "\tfound with " << seedPts.size() << " osc seeds and "
              << fNEval << " evaluations of the likelihood";
    if (fNEvalFiniteDiff > 0)
      std::cout << " (" << fNEvalFiniteDiff
                << " to evaluate gradients numerically)";
    if (fNEvalGrad > 0)
      std::cout << " and " << fNEvalGrad << " evaluations of the gradient";
    std::cout << std::endl;
  }

  return chi;
}

//----------------------------------------------------------------------
void Fitter::DecodePars(const double *pars) const {
  for (unsigned int i = 0; i < fVars.size(); ++i) {
    const double val = pars[i];
    fVars[i]->SetValue(fCalc, val);
  }
  for (unsigned int j = 0; j < fSysts.size(); ++j) {
    const double val = pars[fVars.size() + j];
    fShifts.SetShift(fSysts[j], val);
  }
}

//----------------------------------------------------------------------
double Fitter::DoEval(const double *pars) const {
  ++fNEval;

  DecodePars(pars); // Updates fCalc and fShifts

  // Have to re-fetch the values because DecodePars() will have truncated
  // values to the physical range where necessary.
  double penalty = 0;
  for (unsigned int i = 0; i < fVars.size(); ++i) {
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
              << ", LH: {samp: " << fExpt->ChiSq(fCalc, fShifts)
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

  return fExpt->ChiSq(fCalc, fShifts) + penalty;
}

//----------------------------------------------------------------------
void Fitter::Gradient(const double *pars, double *ret) const {
  ++fNEvalGrad;

  if (!fVars.empty()) {
    // Have to use finite differences to calculate these derivatives
    const double dx = 1e-9;
    const double nom = DoEval(pars);
    ++fNEvalFiniteDiff;
    std::vector<double> parsCopy(pars, pars + NDim());
    for (unsigned int i = 0; i < fVars.size(); ++i) {
      parsCopy[i] += dx;
      ret[i] = (DoEval(parsCopy.data()) - nom) / dx;
      ++fNEvalFiniteDiff;
      parsCopy[i] = pars[i];
    }
  }

  // Get systematic parts analytically

  DecodePars(pars); // Updates fCalc and fShifts

  std::unordered_map<const ISyst *, double> dchi;
  for (const ISyst *s : fSysts)
    dchi[s] = 0;
  fExpt->Derivative(fCalc, fShifts, dchi);

  for (unsigned int j = 0; j < fSysts.size(); ++j) {
    // Get the un-truncated systematic shift
    const double x = pars[fVars.size() + j];

    ret[fVars.size() + j] = dchi[fSysts[j]] + fSysts[j]->PenaltyDerivative(x);
  }
}

//----------------------------------------------------------------------
TH1 *Profile(const IExperiment *expt, osc::IOscCalculatorAdjustable *calc,
             const IFitVar *v, int nbinsx, double minx, double maxx,
             double input_minchi, const std::vector<const IFitVar *> &profVars,
             const std::vector<const ISyst *> &profSysts,
             const SeedList &seedPts,
             const std::vector<SystShifts> &systSeedPts,
             std::map<const IFitVar *, TGraph *> &profVarsMap,
             std::map<const ISyst *, TGraph *> &profSystsMap) {
  Progress prog("Filling profile");
  // If we're called with the default arguments they could already have stuff
  // in from before.
  for (auto it : profVarsMap)
    delete it.second;
  for (auto it : profSystsMap)
    delete it.second;
  profVarsMap.clear();
  profSystsMap.clear();

  // And then create the plots we'll be filling
  for (const IFitVar *v : profVars)
    profVarsMap[v] = new TGraph;
  for (const ISyst *s : profSysts)
    profSystsMap[s] = new TGraph;

  TH1 *ret = new TH1F(UniqueName().c_str(),
                      (";" + v->LatexName() +
                       (input_minchi == 0 ? ";#chi^{2}" : ";#Delta#chi^{2}"))
                          .c_str(),
                      nbinsx, minx, maxx);

  // Save the values of the fit vars as they were in the seed so we can put
  // them back to that value every iteration.
  std::vector<double> seedValues;
  for (const IFitVar *v : profVars)
    seedValues.push_back(v->GetValue(calc));

  double minpos = 0;
  double minchi = 1e10;

  Fitter fit(expt, profVars, profSysts);

  for (int n = 0; n < nbinsx; ++n) {
    prog.SetProgress((double)n / nbinsx);

    const double x = ret->GetXaxis()->GetBinCenter(n + 1);
    v->SetValue(calc, x);

    // Put oscillation values back to their seed position each iteration
    for (unsigned int i = 0; i < seedValues.size(); ++i)
      profVars[i]->SetValue(calc, seedValues[i]);

    SystShifts systshift = SystShifts::Nominal();
    const double chi =
        fit.Fit(calc, systshift, seedPts, systSeedPts, Fitter::kQuiet);

    ret->Fill(x, chi);

    if (chi < minchi) {
      minchi = chi;
      minpos = x;
    }
    for (const IFitVar *var : profVars) {
      profVarsMap[var]->SetPoint(n, x, var->GetValue(calc));
    }
    for (const ISyst *s : profSysts) {
      profSystsMap[s]->SetPoint(n, x, systshift.GetShift(s));
    }
  }
  prog.Done();
  // If we weren't given an explicit minimum chisq, go find one
  if (input_minchi == -1) {
    std::vector<const IFitVar *> allVars = {v};
    for (unsigned int i = 0; i < seedValues.size(); ++i) {
      profVars[i]->SetValue(calc, seedValues[i]);
      allVars.push_back(profVars[i]);
    }
    Fitter fit(expt, allVars, profSysts);
    // Seed from best grid point
    v->SetValue(calc, minpos);
    minchi = fit.Fit(calc); // get a better value
  } else {
    minchi = input_minchi;
  }

  // Zero-subtract the result histogram
  for (int n = 0; n < nbinsx; ++n)
    ret->AddBinContent(n + 1, -minchi);

  return ret;
}

//----------------------------------------------------------------------
TH1 *SqrtProfile(const IExperiment *expt, osc::IOscCalculatorAdjustable *calc,
                 const IFitVar *v, int nbinsx, double minx, double maxx,
                 double minchi, std::vector<const IFitVar *> profVars,
                 std::vector<const ISyst *> profSysts, const SeedList &seedPts,
                 const std::vector<SystShifts> &systSeedPts,
                 std::map<const IFitVar *, TGraph *> &profVarsMap,
                 std::map<const ISyst *, TGraph *> &systsMap) {
  TH1 *ret = Profile(expt, calc, v, nbinsx, minx, maxx, minchi, profVars,
                     profSysts, seedPts, systSeedPts, profVarsMap, systsMap);
  for (int n = 0; n < ret->GetNbinsX() + 2; ++n) {
    const double v = ret->GetBinContent(n);
    ret->SetBinContent(n, v > 0 ? sqrt(v) : 0);
  }
  ret->GetYaxis()->SetTitle("#sigma");
  return ret;
}

//----------------------------------------------------------------------
TH1 *Slice(const IExperiment *expt, osc::IOscCalculatorAdjustable *calc,
           const IFitVar *v, int nbinsx, double minx, double maxx,
           double minchi) {
  return Profile(expt, calc, v, nbinsx, minx, maxx, minchi);
}

//----------------------------------------------------------------------
TH1 *SqrtSlice(const IExperiment *expt, osc::IOscCalculatorAdjustable *calc,
               const IFitVar *v, int nbinsx, double minx, double maxx,
               double minchi) {
  TH1 *ret = Slice(expt, calc, v, nbinsx, minx, maxx, minchi);
  for (int n = 0; n < ret->GetNbinsX() + 2; ++n) {
    const double v = ret->GetBinContent(n);
    ret->SetBinContent(n, v > 0 ? sqrt(v) : 0);
  }
  ret->GetYaxis()->SetTitle("#sigma");
  return ret;
}

//----------------------------------------------------------------------
TGraph *FindValley(const IExperiment *expt, osc::IOscCalculatorAdjustable *calc,
                   const IFitVar &scanVar, const IFitVar &fitVar, int nbinsx,
                   double xmin, double xmax,
                   const std::vector<const IFitVar *> &profVars,
                   const std::vector<const ISyst *> &profSysts,
                   const SeedList &seedPts,
                   const std::vector<SystShifts> &systSeedPts, bool transpose) {
  TGraph *g = new TGraph;
  g->SetLineWidth(2);

  std::vector<const IFitVar *> vars = profVars;
  vars.push_back(&fitVar);

  for (int i = 0; i <= nbinsx; ++i) {
    const double x = xmin + i * (xmax - xmin) / nbinsx;
    scanVar.SetValue(calc, x);
    Fitter fit(expt, vars, profSysts);
    SystShifts shiftSeed = SystShifts::Nominal();
    fit.Fit(calc, shiftSeed, seedPts, systSeedPts, Fitter::kQuiet);
    const double y = fitVar.GetValue(calc);
    if (transpose)
      g->SetPoint(i, y, x);
    else
      g->SetPoint(i, x, y);
  }

  return g;
}

//----------------------------------------------------------------------
std::vector<double> FindCurveCrossings(TH1 *h, double critVal) {
  std::vector<double> ret;

  // Don't look at the under/overflow bins because they don't have properly
  // defined centre positions in any case. Stop one short because we compare
  // i and i+1 inside the loop.
  for (int i = 1; i < h->GetNbinsX(); ++i) {
    const double x0 = h->GetXaxis()->GetBinCenter(i);
    const double x1 = h->GetXaxis()->GetBinCenter(i + 1);

    const double y0 = h->GetBinContent(i);
    const double y1 = h->GetBinContent(i + 1);

    // Passed the critical value between the two points
    if ((y0 < critVal) != (y1 < critVal)) {
      // Interpolate the actual crossing point
      const double x = x0 + (x1 - x0) * (critVal - y0) / (y1 - y0);
      ret.push_back(x);
    }
  } // end for i

  return ret;
}
} // namespace ana
