#include "CAFAna/Fit/Fit.h"
#include "CAFAna/Fit/MinuitFitter.h"

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

#include <cassert>
#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  double SimpleFOM(const Spectrum& obs, const Spectrum& unosc, double pot)
  {
    if(pot == 0)
      pot = obs.POT();

    Eigen::ArrayXd oh(obs.GetEigen(pot));
    Eigen::ArrayXd bh(unosc.GetEigen(pot));
    assert(oh.size() == bh.size());

    double fomSq = 0;

    // Combine s/sqrt(s+b) in quadrature between bins
    for(int i = 0; i < oh.size(); ++i){
      const double o = oh[i];
      const double b = bh[i];
      const double s = o-b;

      if (s <= 0)
        continue;

      fomSq += util::sqr(s)/(s+b);
    }

    return sqrt(fomSq);
  }

  //----------------------------------------------------------------------
  TH1* Profile(const IExperiment* expt,
               osc::IOscCalcAdjustable* calc, const IFitVar* v,
               int nbinsx, double minx, double maxx,
               double input_minchi,
               const std::vector<const IFitVar*>& profVars,
               const std::vector<const ISyst*>& profSysts,
               const SeedList& seedPts,
               const std::vector<SystShifts>& systSeedPts,
               std::map<const IFitVar*, TGraph*>& profVarsMap,
               std::map<const ISyst*, TGraph*>& profSystsMap,
               MinuitFitter::FitOpts opts)
  {
    Progress prog ("Filling profile");
    // If we're called with the default arguments they could already have stuff
    // in from before.
    for (auto it : profVarsMap)
      delete it.second;
    for (auto it : profSystsMap)
      delete it.second;
    profVarsMap.clear();
    profSystsMap.clear();

    // And then create the plots we'll be filling
    for(const IFitVar* v: profVars)
      profVarsMap[v] = new TGraph;
    for(const ISyst* s: profSysts)
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

    MinuitFitter fit(expt, profVars, profSysts, opts);

    for(int n = 0; n < nbinsx; ++n){
      prog.SetProgress((double)n / nbinsx);

      const double x = ret->GetXaxis()->GetBinCenter(n + 1);
      v->SetValue(calc, x);

      // Put oscillation values back to their seed position each iteration
      for(unsigned int i = 0; i < seedValues.size(); ++i)
        profVars[i]->SetValue( calc, seedValues[i] );

      SystShifts systshift = SystShifts::Nominal();
      const double chi = fit.Fit(calc, systshift, seedPts, systSeedPts, MinuitFitter::kQuiet)->EvalMetricVal();

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
    if(input_minchi == -1)
    {
      std::vector<const IFitVar*> allVars = {v};
      for(unsigned int i = 0; i < seedValues.size(); ++i) {
        profVars[i]->SetValue(calc, seedValues[i]);
        allVars.push_back(profVars[i]);
      }
      MinuitFitter fit(expt, allVars, profSysts, opts);
      // Seed from best grid point
      v->SetValue(calc, minpos);
      minchi = fit.Fit(calc)->EvalMetricVal(); // get a better value

    }
    else
      minchi = input_minchi;

    // Zero-subtract the result histogram
    for (int n = 0; n < nbinsx; ++n)
      ret->AddBinContent(n + 1, -minchi);

    return ret;
  }

  //----------------------------------------------------------------------
  TH1* SqrtProfile(const IExperiment* expt,
                   osc::IOscCalcAdjustable* calc, const IFitVar* v,
                   int nbinsx, double minx, double maxx, double minchi,
                   std::vector<const IFitVar*> profVars,
                   std::vector<const ISyst*> profSysts,
                   const SeedList& seedPts,
                   const std::vector<SystShifts>& systSeedPts,
                   std::map<const IFitVar*, TGraph*>& profVarsMap,
                   std::map<const ISyst*, TGraph*>& systsMap,
                   MinuitFitter::FitOpts opts)
  {
    TH1* ret = Profile(expt, calc,
                          v, nbinsx, minx, maxx,
                          minchi, profVars, profSysts, seedPts, systSeedPts,
                          profVarsMap, systsMap, opts);

    for(int n = 0; n < ret->GetNbinsX() + 2; ++n){
      const double v = ret->GetBinContent(n);
      ret->SetBinContent(n, v > 0 ? sqrt(v) : 0);
    }

  ret->GetYaxis()->SetTitle("#sigma");
    return ret;
  }

  //----------------------------------------------------------------------
  TH1* Slice(const IExperiment* expt,
             osc::IOscCalcAdjustable* calc, const IFitVar* v,
             int nbinsx, double minx, double maxx,
             double minchi,
             MinuitFitter::FitOpts opts)
  {
    return Profile(expt, calc, v, nbinsx, minx, maxx, minchi,
                   {}, {}, {}, {}, empty_vars_map, empty_syst_map, opts);
  }

  //----------------------------------------------------------------------
  TH1* SqrtSlice(const IExperiment* expt,
                    osc::IOscCalcAdjustable* calc, const IFitVar* v,
                    int nbinsx, double minx, double maxx, double minchi,
                    MinuitFitter::FitOpts opts)
  {
    TH1 *ret = Slice(expt, calc, v, nbinsx, minx, maxx, minchi, opts);

    for (int n = 0; n < ret->GetNbinsX() + 2; ++n) {
      const double v = ret->GetBinContent(n);
      ret->SetBinContent(n, v > 0 ? sqrt(v) : 0);
    }

    ret->GetYaxis()->SetTitle("#sigma");
    return ret;
  }

  //----------------------------------------------------------------------
  TGraph* FindValley(const IExperiment* expt,
                     osc::IOscCalcAdjustable* calc,
                     const IFitVar& scanVar,
                     const IFitVar& fitVar,
                     int nbinsx, double xmin, double xmax,
                     const std::vector<const IFitVar*>& profVars,
                     const std::vector<const ISyst*>& profSysts,
                     const SeedList& seedPts,
                     const std::vector<SystShifts>& systSeedPts,
                     bool transpose,
                     MinuitFitter::FitOpts opts)
  {
    TGraph* g = new TGraph;
    g->SetLineWidth(2);

    std::vector<const IFitVar*> vars = profVars;
    vars.push_back(&fitVar);

    for(int i = 0; i <= nbinsx; ++i){
      const double x = xmin + i*(xmax-xmin)/nbinsx;
      scanVar.SetValue(calc, x);
      MinuitFitter fit(expt, vars, profSysts, opts);
      SystShifts shiftSeed = SystShifts::Nominal();
      fit.Fit(calc, shiftSeed, seedPts, systSeedPts, MinuitFitter::kQuiet);
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
} // namespace ana
