#include "CAFAna/Analysis/Fit.h"

#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Experiment/IExperiment.h"
#include "CAFAna/Analysis/GradientDescent.h"

#include "OscLib/func/IOscCalculator.h"
#include "Utilities/func/MathUtil.h"

#include "TError.h"
#include "TGraph.h"
#include "TH1.h"
#include "TMatrixDSym.h"

#include <cassert>
#include <memory>
#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  double SimpleFOM(const Spectrum& obs, const Spectrum& unosc, double pot)
  {
    if(pot == 0) pot = obs.POT();

    std::unique_ptr<TH1> oh(obs.ToTH1(pot));
    std::unique_ptr<TH1> bh(unosc.ToTH1(pot));
    assert(oh->GetNbinsX() == bh->GetNbinsX());

    double fomSq = 0;

    // Combine s/sqrt(s+b) in quadrature between bins
    for(int i = 0; i < oh->GetNbinsX(); ++i){
      const double o = oh->GetBinContent(i);
      const double b = bh->GetBinContent(i);
      const double s = o-b;

      if(s <= 0) continue;

      fomSq += util::sqr(s)/(s+b);
    }

    return sqrt(fomSq);
  }

  //----------------------------------------------------------------------
  Fitter::Fitter(const IExperiment* expt,
                 std::vector<const IFitVar*> vars,
                 std::vector<const ISyst*> systs,
                 Precision prec)
    : fExpt(expt), fVars(vars), fSysts(systs), fPrec(prec), fCalc(0),
      fSupportsDerivatives(SupportsDerivatives()), fCovar(0)
  {
  }

  //----------------------------------------------------------------------
  std::vector<Fitter::SeedPt> Fitter::
  ExpandSeeds(const std::map<const IFitVar*,
                             std::vector<double>>& seedPts,
              std::vector<SystShifts> systSeedPts) const
  {
    std::vector<SeedPt> ret;
    ret.push_back(SeedPt());

    for(auto it: seedPts){
      // For every variable, duplicate every entry in ret with the value set to
      // each possibility.
      const IFitVar* fv = it.first;
      std::vector<SeedPt> newret;
      for(double val: it.second){
        for(SeedPt pt: ret){
          pt.fitvars[fv] = val;
          newret.push_back(pt);
        }
      } // end for val
      ret = newret;
    } // end for it

    // Now duplicate as many times as required for the syst seeds
    if(!systSeedPts.empty()){
      std::vector<SeedPt> newret;
      for(const SystShifts& s: systSeedPts){
        for(SeedPt pt: ret){
          pt.shift = s;
          newret.push_back(pt);
        }
      }
      ret = newret;
    }

    return ret;
  }

  //----------------------------------------------------------------------
  bool Fitter::SupportsDerivatives() const
  {
    // Make completely opt-in again (some issues for stats. throws)
    if(getenv("CAFANA_ANALYTIC_DERIVATIVES") == 0) return false;

    // No point using derivatives for FitVars only, we do finite differences,
    // probably worse than MINUIT would.
    if(fSysts.empty()) return false;

    // Otherwise, do the minimal trial to see if the experiment will return a
    // gradient.
    std::unordered_map<const ISyst*, double> dchi = {{fSysts[0], 0}};
    osc::NoOscillations calc;
    fExpt->Derivative(&calc, SystShifts::Nominal(), dchi);

    return !dchi.empty();
  }

  //----------------------------------------------------------------------
  double Fitter::FitHelperSeeded(osc::IOscCalculatorAdjustable* seed,
                                 SystShifts& systSeed,
                                 Verbosity verb) const
  {

    fCalc = seed;
    fShifts = systSeed;

    ROOT::Minuit2::MnUserParameters mnPars;

    for(const IFitVar* v: fVars){
      const double val = v->GetValue(seed);
      // name, value, error
      mnPars.Add(v->ShortName(), val, val ? val/2 : .1);
      fParamNames  .push_back(v->ShortName());
      fPreFitValues.push_back(val);
      fPreFitErrors.push_back(val ? val/2 : .1);
    }
    // One way this can go wrong is if two variables have the same ShortName
    assert(mnPars.Params().size() == fVars.size());
    for(const ISyst* s: fSysts){
      const double val = systSeed.GetShift(s);
      // name, value, error
      mnPars.Add(s->ShortName(), val, 1);
      // Add a limit on each
      mnPars.SetLimits(mnPars.Params().size()-1, s->Min(), s->Max());
      fParamNames  .push_back(s->ShortName());
      fPreFitValues.push_back(val);
      fPreFitErrors.push_back(1);
    }
    // One way this can go wrong is if two variables have the same ShortName
    assert(mnPars.Params().size() == fVars.size()+fSysts.size());

    ROOT::Minuit2::MnApplication* mnApp = 0;

    if((fPrec & kAlgoMask) == kGradDesc){
      mnApp = new GradientDescent(*this, mnPars);
    }
    else{
      // TODO - could this be handled with templates?
      if(fSupportsDerivatives){
        if(fPrec & kIncludeSimplex){
          // MnMinimize will try Simplex if the migrad minimum is invalid
          mnApp = new ROOT::Minuit2::MnMinimize(*this, mnPars, int(fPrec & kAlgoMask));
        }
        else{
          mnApp = new ROOT::Minuit2::MnMigrad(*this, mnPars, int(fPrec & kAlgoMask));
        }
      }
      else{
        if(fPrec & kIncludeSimplex){
          mnApp = new ROOT::Minuit2::MnMinimize(*((ROOT::Minuit2::FCNBase*)this), mnPars, int(fPrec & kAlgoMask));
        }
        else{
          mnApp = new ROOT::Minuit2::MnMigrad(*((ROOT::Minuit2::FCNBase*)this), mnPars, int(fPrec & kAlgoMask));
        }
      }
    }

    // Minuit2 doesn't give a good way to control verbosity...
    const int olderr = gErrorIgnoreLevel;
    if(verb == kQuiet) gErrorIgnoreLevel = 1001; // Ignore warnings

    ROOT::Minuit2::FunctionMinimum minpt = (*mnApp)();
    gErrorIgnoreLevel = olderr;

    // Hesse can find a better minimum, so let's do this before panicking
    if(fPrec & kIncludeHesse){
      std::cout << "Notice: attempting to build the Hessian matrix" << std::endl;
      ROOT::Minuit2::MnHesse hesse(2);
      hesse(*this, minpt, 1e5);
    }
    
    // Okay, time to panic! If this isn't valid, we probably should worry more...
    if(!minpt.IsValid()){
      std::cout << "*** ERROR: minimum is not valid ***" << std::endl;
    }

    // Store some basic fit information
    this->fEdm = minpt.Edm();
    this->fIsValid = minpt.IsValid();
      
    fPostFitValues = minpt.UserParameters().Params();
    fPostFitErrors = minpt.UserParameters().Errors();

    gErrorIgnoreLevel = olderr;
    // Store results back to the "seed" variable
    for(unsigned int i = 0; i < fVars.size(); ++i){
      fVars[i]->SetValue(seed, fPostFitValues[i]);
    }
    // Store systematic results back into "systSeed"
    for(unsigned int j = 0; j < fSysts.size(); ++j){
      systSeed.SetShift(fSysts[j], fPostFitValues[fVars.size()+j]);
    }

    // Store covariances
    const ROOT::Minuit2::MnUserCovariance mncov = minpt.UserCovariance();
    // Slightly tedious
    std::vector<double> covdata = mncov.Data();
    if (this->fCovar) delete this->fCovar;
    this->fCovar = new TMatrixDSym(mncov.Nrow());

    for (uint row = 0; row < mncov.Nrow(); ++row){
      for (uint col = 0; col < mncov.Nrow(); ++col){
	if (row > col)
	  (*this->fCovar)(row, col) = covdata[col+row*(row+1)/2];
	else (*this->fCovar)(row, col) = covdata[row+col*(col+1)/2];
      }
    }

    delete mnApp;

    return minpt.Fval();
  }

  //----------------------------------------------------------------------
  double Fitter::FitHelper(osc::IOscCalculatorAdjustable* seed,
                           SystShifts& bestSysts,
                           const std::map<const IFitVar*, std::vector<double>>& seedPts,
                           std::vector<SystShifts> systSeedPts,
                           Verbosity verb) const
  {
    const std::vector<SeedPt> pts = ExpandSeeds(seedPts, systSeedPts);

    double minchi = 1e10;
    std::vector<double> bestFitPars, bestSystPars;

    for(const SeedPt& pt: pts){
      for(auto it: pt.fitvars) it.first->SetValue(seed, it.second);

      SystShifts shift = pt.shift;
      const double chi = FitHelperSeeded(seed, shift, verb);
      if(chi < minchi){
        minchi = chi;
        // Store the best fit values of all the parameters we know are being
        // varied.
        bestFitPars.clear();
        bestSystPars.clear();
        for(const IFitVar* v: fVars)
          bestFitPars.push_back(v->GetValue(seed));
        for(const ISyst* s: fSysts)
          bestSystPars.push_back(shift.GetShift(s));
      }
    }

    // Stuff the results of the actual best fit back into the seeds
    for(unsigned int i = 0; i < fVars.size(); ++i)
      fVars[i]->SetValue(seed, bestFitPars[i]);
    for(unsigned int i = 0; i < fSysts.size(); ++i)
      bestSysts.SetShift(fSysts[i], bestSystPars[i]);

    return minchi;
  }

  //----------------------------------------------------------------------
  void Fitter::SetPrecision(Precision prec)
  {
    if((prec & kAlgoMask) == kGradDesc && !fSysts.empty() && !fSupportsDerivatives){
      std::cout << "Warning - not setting precision to kGradDesc, since analytic gradients are not supported by this experiment" << std::endl;
      return;
    }

    fPrec = prec;
  }

  //----------------------------------------------------------------------
  double Fitter::Fit(osc::IOscCalculatorAdjustable* seed,
                     SystShifts& bestSysts,
                     const std::map<const IFitVar*, std::vector<double>>& seedPts,
                     const std::vector<SystShifts>& systSeedPts,
                     Verbosity verb) const
  {
    // Fits with no osc calculator shouldn't be trying to optimize any
    // oscilation parameters...
    assert(seed || fVars.empty());

    for(const auto& it: seedPts){
      if(std::find(fVars.begin(), fVars.end(), it.first) == fVars.end()){
        std::cout << "ERROR Fitter::Fit() trying to seed '"
                  << it.first->ShortName()
                  << "' which is not part of the fit." << std::endl;
        abort();
      }
    }
    for(const auto& it: systSeedPts){
      for(const ISyst* s: it.ActiveSysts()){
        if(std::find(fSysts.begin(), fSysts.end(), s) == fSysts.end()){
          std::cout << "ERROR Fitter::Fit() trying to seed '"
                    << s->ShortName()
                    << "' which is not part of the fit." << std::endl;
          abort();
        }
      }
    }

    if(verb == kVerbose){
      std::cout << "Finding best fit for";
      for(const IFitVar* v: fVars) std::cout << " " << v->ShortName();
      for(const ISyst* s: fSysts) std::cout << " " << s->ShortName();
      if(fSupportsDerivatives) std::cout << " using analytic derivatives";
      std::cout << "..." << std::endl;
    }

    // Do all the actual work. This wrapper function is just so we can have
    // better control of printing.
    const double chi = FitHelper(seed, bestSysts, seedPts, systSeedPts, verb);

    if(verb == kVerbose){
      std::cout << "Best fit";
      for(const IFitVar* v: fVars){
        std::cout << ", " << v->ShortName() << " = " << v->GetValue(seed);
      }
      for(const ISyst* s: fSysts){
        std::cout << ", " << s->ShortName() << " = " << bestSysts.GetShift(s);
      }
      std::cout << ", LL = " << chi << std::endl;

      std::cout << "  found with " << fNEval << " evaluations of the likelihood";
      if(fNEvalFiniteDiff > 0)
        std::cout << " (" << fNEvalFiniteDiff << " to evaluate gradients numerically)";
      if(fNEvalGrad > 0)
        std::cout << " and " << fNEvalGrad << " evaluations of the gradient";
      std::cout << std::endl;
    }

    return chi;
  }

  //----------------------------------------------------------------------
  void Fitter::DecodePars(const std::vector<double>& pars) const
  {
    assert(pars.size() == fVars.size()+fSysts.size());

    for(unsigned int i = 0; i < fVars.size(); ++i){
      const double val = pars[i];
      fVars[i]->SetValue(fCalc, val);
    }
    for(unsigned int j = 0; j < fSysts.size(); ++j){
      const double val = pars[fVars.size()+j];
      fShifts.SetShift(fSysts[j], val);
    }
  }

  //----------------------------------------------------------------------
  double Fitter::operator()(const std::vector<double>& pars) const
  {
    ++fNEval;

    assert(pars.size() == fVars.size()+fSysts.size());

    DecodePars(pars); // Updates fCalc and fShifts

    // Have to re-fetch the FitVar values because DecodePars() will have
    // truncated values to the physical range where necessary.
    double penalty = 0;
    for(unsigned int i = 0; i < fVars.size(); ++i){
      penalty += fVars[i]->Penalty(pars[i], fCalc);
    }

    return fExpt->ChiSq(fCalc, fShifts) + penalty + fShifts.Penalty();
  }

  //----------------------------------------------------------------------
  std::vector<double> Fitter::Gradient(const std::vector<double>& pars) const
  {
    ++fNEvalGrad;

    std::vector<double> ret(pars.size());

    // TODO handling of FitVars including penalty terms

    if(!fVars.empty()){
      // Have to use finite differences to calculate these derivatives
      const double dx = 1e-9;
      const double nom = (*this)(pars);
      ++fNEvalFiniteDiff;
      std::vector<double> parsCopy = pars;
      for(unsigned int i = 0; i < fVars.size(); ++i){
        parsCopy[i] += dx;
        ret[i] = ((*this)(parsCopy)-nom)/dx;
        ++fNEvalFiniteDiff;
        parsCopy[i] = pars[i];
      }
    }

    // Get systematic parts analytically

    DecodePars(pars); // Updates fCalc and fShifts

    std::unordered_map<const ISyst*, double> dchi;
    for(const ISyst* s: fSysts) dchi[s] = 0;
    fExpt->Derivative(fCalc, fShifts, dchi);

    for(unsigned int i = 0; i < fSysts.size(); ++i){
      // Include the derivative of the penalty terms too. TODO this is only
      // right for quadratic penalties (ie all the currently existing ones)
      ret[fVars.size()+i] = dchi[fSysts[i]] + 2*fShifts.GetShift(fSysts[i]);
    }

    return ret;
  }

  //----------------------------------------------------------------------
  TH1* Profile(const IExperiment* expt,
	       osc::IOscCalculatorAdjustable* calc, const IFitVar* v,
	       int nbinsx, double minx, double maxx,
	       double input_minchi,
               const std::vector<const IFitVar*>& profVars,
               const std::vector<const ISyst*>& profSysts,
               const std::map<const IFitVar*, std::vector<double>>& seedPts,
               const std::vector<SystShifts>& systSeedPts,
               std::map<const IFitVar*, TGraph*>& profVarsMap,
               std::map<const ISyst*, TGraph*>& profSystsMap)
  {
    Progress prog ("Filling profile");
    // If we're called with the default arguments they could already have stuff
    // in from before.
    for(auto it: profVarsMap) delete it.second;
    for(auto it: profSystsMap) delete it.second;
    profVarsMap.clear();
    profSystsMap.clear();

    // And then create the plots we'll be filling
    for(const IFitVar* v: profVars) profVarsMap[v] = new TGraph;
    for(const ISyst* s: profSysts) profSystsMap[s] = new TGraph;

    TH1* ret = new TH1F(UniqueName().c_str(),
			(";"+v->LatexName()+ (input_minchi == 0? ";#chi^{2}" : ";#Delta#chi^{2}") ).c_str(),
			nbinsx, minx, maxx);

    // Save the values of the fit vars as they were in the seed so we can put
    // them back to that value every iteration.
    std::vector<double> seedValues;
    for(const IFitVar* v: profVars) seedValues.push_back(v->GetValue(calc));

    double minpos = 0;
    double minchi = 1e10;

    Fitter fit(expt, profVars, profSysts);

    for(int n = 0; n < nbinsx; ++n){
      prog.SetProgress((double) n/nbinsx);

      const double x = ret->GetXaxis()->GetBinCenter(n+1);
      v->SetValue(calc, x);

      // Put oscillation values back to their seed position each iteration
      for(unsigned int i = 0; i < seedValues.size(); ++i)
        profVars[i]->SetValue( calc, seedValues[i] );

      SystShifts systshift = SystShifts::Nominal();
      const double chi = fit.Fit(calc, systshift, seedPts, systSeedPts, Fitter::kQuiet);

      ret->Fill(x, chi);

      if(chi < minchi){
	minchi = chi;
	minpos = x;
      }
      for(const IFitVar* var: profVars){
	profVarsMap[var]->SetPoint(n, x, var->GetValue(calc));
      }
      for(const ISyst* s: profSysts){
	profSystsMap[s]->SetPoint(n, x, systshift.GetShift(s));
      }
    }
    prog.Done();
    // If we weren't given an explicit minimum chisq, go find one
    if(input_minchi == -1){
      std::vector<const IFitVar*> allVars = {v};
      for(unsigned int i = 0; i < seedValues.size(); ++i) {
	 profVars[i]->SetValue(calc, seedValues[i]);
	 allVars.push_back(profVars[i]);
      }
      Fitter fit(expt, allVars, profSysts);
      // Seed from best grid point
      v->SetValue(calc, minpos);
      minchi = fit.Fit(calc); // get a better value
    }
    else{
      minchi = input_minchi;
    }

    // Zero-subtract the result histogram
    for(int n = 0; n < nbinsx; ++n) ret->AddBinContent(n+1, -minchi);

    return ret;
  }

  //----------------------------------------------------------------------
  TH1* SqrtProfile(const IExperiment* expt,
		   osc::IOscCalculatorAdjustable* calc, const IFitVar* v,
		   int nbinsx, double minx, double maxx, double minchi,
		   std::vector<const IFitVar*> profVars,
		   std::vector<const ISyst*> profSysts,
                   const std::map<const IFitVar*, std::vector<double>>& seedPts,
                   const std::vector<SystShifts>& systSeedPts,
                   std::map<const IFitVar*, TGraph*>& profVarsMap,
                   std::map<const ISyst*, TGraph*>& systsMap)
  {
    TH1* ret = Profile(expt, calc,
                       v, nbinsx, minx, maxx,
                       minchi, profVars, profSysts, seedPts, systSeedPts,
                       profVarsMap, systsMap);
    for(int n = 0; n < ret->GetNbinsX()+2; ++n){
      const double v = ret->GetBinContent(n);
      ret->SetBinContent(n, v > 0 ? sqrt(v) : 0);
    }
    ret->GetYaxis()->SetTitle("#sigma");
    return ret;
  }

  //----------------------------------------------------------------------
  TH1* Slice(const IExperiment* expt,
             osc::IOscCalculatorAdjustable* calc, const IFitVar* v,
             int nbinsx, double minx, double maxx,
             double minchi)
  {
    return Profile(expt, calc, v, nbinsx, minx, maxx, minchi);
  }

  //----------------------------------------------------------------------
  TH1* SqrtSlice(const IExperiment* expt,
                 osc::IOscCalculatorAdjustable* calc, const IFitVar* v,
                 int nbinsx, double minx, double maxx, double minchi)
  {
    TH1* ret = Slice(expt, calc, v, nbinsx, minx, maxx, minchi);
    for(int n = 0; n < ret->GetNbinsX()+2; ++n){
      const double v = ret->GetBinContent(n);
      ret->SetBinContent(n, v > 0 ? sqrt(v) : 0);
    }
    ret->GetYaxis()->SetTitle("#sigma");
    return ret;
  }

  //----------------------------------------------------------------------
  TGraph* FindValley(const IExperiment* expt,
		     osc::IOscCalculatorAdjustable* calc,
		     const IFitVar& scanVar,
		     const IFitVar& fitVar,
		     int nbinsx, double xmin, double xmax,
		     const std::vector<const IFitVar*>& profVars,
		     const std::vector<const ISyst*>& profSysts,
                     const std::map<const IFitVar*, std::vector<double>>& seedPts,
                     const std::vector<SystShifts>& systSeedPts,
		     bool transpose)
  {
    TGraph* g = new TGraph;
    g->SetLineWidth(2);

    std::vector<const IFitVar*> vars = profVars;
    vars.push_back(&fitVar);

    for(int i = 0; i <= nbinsx; ++i){
      const double x = xmin + i*(xmax-xmin)/nbinsx;
      scanVar.SetValue(calc, x);
      Fitter fit(expt, vars, profSysts);
      SystShifts shiftSeed = SystShifts::Nominal();
      fit.Fit(calc, shiftSeed, seedPts, systSeedPts, Fitter::kQuiet);
      const double y = fitVar.GetValue(calc);
      if(transpose)
	g->SetPoint(i, y, x);
      else
	g->SetPoint(i, x, y);
    }

    return g;
  }

  //----------------------------------------------------------------------
  std::vector<double> FindCurveCrossings(TH1* h, double critVal)
  {
    std::vector<double> ret;

    // Don't look at the under/overflow bins because they don't have properly
    // defined centre positions in any case. Stop one short because we compare
    // i and i+1 inside the loop.
    for(int i = 1; i < h->GetNbinsX(); ++i){
      const double x0 = h->GetXaxis()->GetBinCenter(i);
      const double x1 = h->GetXaxis()->GetBinCenter(i+1);

      const double y0 = h->GetBinContent(i);
      const double y1 = h->GetBinContent(i+1);

      // Passed the critical value between the two points
      if((y0 < critVal) != (y1 < critVal)){
        // Interpolate the actual crossing point
        const double x = x0 + (x1-x0)*(critVal-y0)/(y1-y0);
        ret.push_back(x);
      }
    } // end for i

    return ret;
  }
}
