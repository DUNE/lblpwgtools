#include "CAFAna/Analysis/Fit.h"

#include "CAFAna/Experiment/IExperiment.h"
#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/Utilities.h"

#include "OscLib/func/IOscCalculator.h"
#include "Utilities/func/MathUtil.h"

#include "TError.h"
#include "TGraph.h"
#include "TH1.h"

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
                 std::vector<const ISyst*> systs)
    : fExpt(expt), fVars(vars), fSysts(systs)
  {
  }

  //----------------------------------------------------------------------
  double Fitter::FitHelper(osc::IOscCalculatorAdjustable* seed,
                            SystShifts& systSeed,
                            const std::map<const IFitVar*, std::vector<double>>& seedPts,
                            Verbosity verb) const
  {
    // If there are seed points to use, loop over all the combinations. We
    // achieve this recursively.
    if(!seedPts.empty()){
      const std::pair<const IFitVar*, std::vector<double>> front = *seedPts.begin();
      std::map<const IFitVar*, std::vector<double>> reducedPts = seedPts;
      reducedPts.erase(reducedPts.begin());

      if(front.second.empty())
        return FitHelper(seed, systSeed, reducedPts, verb);

      double minchi = 1e10;
      std::vector<double> bestFitPars, bestSystPars;
      for(double val: front.second){
        front.first->SetValue(seed, val);
        const double chi = FitHelper(seed, systSeed, reducedPts, verb);
        if(chi < minchi){
          minchi = chi;
          // Store the best fit values of all the parameters we know are being
          // varied.
          bestFitPars.clear();
          bestSystPars.clear();
          for(const IFitVar* v: fVars)
            bestFitPars.push_back(v->GetValue(seed));
          for(const ISyst* s: fSysts)
            bestSystPars.push_back(systSeed.GetShift(s));
        }
      }

      // Stuff the results of the actual best fit back into the seeds
      for(unsigned int i = 0; i < fVars.size(); ++i)
        fVars[i]->SetValue(seed, bestFitPars[i]);
      for(unsigned int i = 0; i < fSysts.size(); ++i)
        systSeed.SetShift(fSysts[i], bestSystPars[i]);

      return minchi;
    }

    // The actual fit, where there are no more seeds left to vary and we just
    // have to start at 'seed' and 'systSeed'.

    fCalc = seed;
    fShifts = systSeed;

    ROOT::Minuit2::MnUserParameters mnPars;

    for(const IFitVar* v: fVars){
      const double val = v->GetValue(seed);
      // name, value, error
      mnPars.Add(v->ShortName(), val, val ? val/2 : .1);
    }
    // One way this can go wrong is if two variables have the same ShortName
    assert(mnPars.Params().size() == fVars.size());
    for(const ISyst* s: fSysts){
      const double val = systSeed.GetShift(s);
      // name, value, error
      mnPars.Add(s->ShortName(), val, 1);
    }
    // One way this can go wrong is if two variables have the same ShortName
    assert(mnPars.Params().size() == fVars.size()+fSysts.size());

    ROOT::Minuit2::MnMigrad mnMigrad(*this, mnPars);

    // Minuit2 doesn't give a good way to control verbosity...
    const int olderr = gErrorIgnoreLevel;
    if(verb == kQuiet) gErrorIgnoreLevel = 1001; // Ignore warnings
    ROOT::Minuit2::FunctionMinimum minpt = mnMigrad();
    gErrorIgnoreLevel = olderr;

    if(!minpt.IsValid())
      std::cout << "*** ERROR: minimum is not valid ***" << std::endl;

    const std::vector<double> minvec = minpt.UserParameters().Params();

    // Store results back to the "seed" variable
    for(unsigned int i = 0; i < fVars.size(); ++i){
      fVars[i]->SetValue(seed, minvec[i]);
    }
    // Store systematic results back into "systSeed"
    for(unsigned int j = 0; j < fSysts.size(); ++j){
      systSeed.SetShift(fSysts[j], minvec[fVars.size()+j]);
    }

    return minpt.Fval();
  }

  //----------------------------------------------------------------------
  double Fitter::Fit(osc::IOscCalculatorAdjustable* seed,
                     SystShifts& systSeed,
                     const std::map<const IFitVar*, std::vector<double>>& seedPts,
                     Verbosity verb) const
  {
    // Fits with no osc calculator shouldn't be trying to optimize any
    // oscilation parameters...
    assert(seed || fVars.empty());

    if(verb == kVerbose){
      std::cout << "Finding best fit for";
      for(const IFitVar* v: fVars) std::cout << " " << v->ShortName();
      for(const ISyst* s: fSysts) std::cout << " " << s->ShortName();
      std::cout << "..." << std::endl;
    }

    // Do all the actual work. This wrapper function is just so we can have
    // better control of printing.
    const double chi = FitHelper(seed, systSeed, seedPts, verb);

    if(verb == kVerbose){
      std::cout << "Best fit";
      for(const IFitVar* v: fVars){
        std::cout << ", " << v->ShortName() << " = " << v->GetValue(seed);
      }
      for(const ISyst* s: fSysts){
        std::cout << ", " << s->ShortName() << " = " << systSeed.GetShift(s);
      }
      std::cout << ", LL = " << chi << std::endl;
    }

    return chi;
  }

  //----------------------------------------------------------------------
  double Fitter::operator()(const std::vector<double>& pars) const
  {
    assert(pars.size() == fVars.size()+fSysts.size());

    double penalty = 0;
    for(unsigned int i = 0; i < fVars.size(); ++i){
      const double val = pars[i];
      penalty += fVars[i]->Penalty(val, fCalc);
      fVars[i]->SetValue(fCalc, val);
    }
    for(unsigned int j = 0; j < fSysts.size(); ++j){
      const double val = pars[fVars.size()+j];
      fShifts.SetShift(fSysts[j], val);
    }

    return fExpt->ChiSq(fCalc, fShifts)+penalty+fShifts.Penalty();
  }

  //----------------------------------------------------------------------
  TH1* ProfileHelper(const IExperiment* expt,
                     osc::IOscCalculatorAdjustable* calc,
                     const IFitVar* v,
                     const ISyst* s,
                     int nbinsx, double minx, double maxx,
                     double input_minchi,
                     const std::vector<const IFitVar*>& profVars,
                     const std::vector<const ISyst*>& profSysts,
                     const std::map<const IFitVar*, std::vector<double>>& seedPts,
                     std::map<const IFitVar*, TGraph*>& profVarsMap,
                     std::map<const ISyst*, TGraph*>& profSystsMap)
  {
    assert(!v || !s);
    assert(v || s);

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
      const double x = ret->GetXaxis()->GetBinCenter(n+1);
      if(v) v->SetValue(calc, x);

      // Put oscillation values back to their seed position each iteration
      for(unsigned int i = 0; i < seedValues.size(); ++i)
        profVars[i]->SetValue( calc, seedValues[i] );

      SystShifts systshift = SystShifts::Nominal();
      if(s) systshift.SetShift(s, x);
      const double chi = fit.Fit(calc, systshift, seedPts, Fitter::kQuiet);

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
  TH1* Profile(const IExperiment* expt,
               osc::IOscCalculatorAdjustable* calc,
               const IFitVar* v,
               int nbinsx, double minx, double maxx,
               double input_minchi,
               const std::vector<const IFitVar*>& profVars,
               const std::vector<const ISyst*>& profSysts,
               const std::map<const IFitVar*, std::vector<double>>& seedPts,
               std::map<const IFitVar*, TGraph*>& profVarsMap,
               std::map<const ISyst*, TGraph*>& profSystsMap)
  {
    return ProfileHelper(expt, calc, v, 0, nbinsx, minx, maxx,
                         input_minchi, profVars, profSysts, seedPts,
                         profVarsMap, profSystsMap);
  }

  //----------------------------------------------------------------------
  TH1* Profile(const IExperiment* expt,
               osc::IOscCalculatorAdjustable* calc,
               const ISyst* s,
               int nbinsx, double minx, double maxx,
               double input_minchi,
               const std::vector<const IFitVar*>& profVars,
               const std::vector<const ISyst*>& profSysts,
               const std::map<const IFitVar*, std::vector<double>>& seedPts,
               std::map<const IFitVar*, TGraph*>& profVarsMap,
               std::map<const ISyst*, TGraph*>& profSystsMap)
  {
    return ProfileHelper(expt, calc, 0, s, nbinsx, minx, maxx,
                         input_minchi, profVars, profSysts, seedPts,
                         profVarsMap, profSystsMap);
  }

  //----------------------------------------------------------------------
  TH1* SqrtProfile(const IExperiment* expt,
		   osc::IOscCalculatorAdjustable* calc, const IFitVar* v,
		   int nbinsx, double minx, double maxx, double minchi,
		   std::vector<const IFitVar*> profVars,
		   std::vector<const ISyst*> profSysts,
                   const std::map<const IFitVar*, std::vector<double>>& seedPts)
  {
    TH1* ret = Profile(expt, calc,
                       v, nbinsx, minx, maxx,
                       minchi, profVars, profSysts, seedPts);
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
  TH1* Slice(const IExperiment* expt,
             osc::IOscCalculatorAdjustable* calc, const ISyst* s,
             int nbinsx, double minx, double maxx,
             double minchi)
  {
    return Profile(expt, calc, s, nbinsx, minx, maxx, minchi);
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
      fit.Fit(calc, shiftSeed, seedPts, Fitter::kQuiet);
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
