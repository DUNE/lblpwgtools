#include "CAFAna/Fit/GradientDescent.h"

#include <iostream>

/*
#include "TGraph.h"
#include "TH2.h"
#include "TPad.h"
*/

namespace ana
{
  //----------------------------------------------------------------------
  GradientDescent::
  GradientDescent(const ROOT::Math::IGradientFunctionMultiDim& func)
    : fFunc(&func)
  {
  }

  //----------------------------------------------------------------------
  GradientDescent::~GradientDescent()
  {
  }

  //----------------------------------------------------------------------
  bool GradientDescent::SetVariable(unsigned int ivar,
                                    const std::string&,
                                    double val,
                                    double step)
  {
    if(ivar >= fVals.size()) fVals.resize(ivar+1);
    fVals[ivar] = val;
    if(ivar >= fErrs.size()) fErrs.resize(ivar+1);
    fErrs[ivar] = step;
    return true;
  }

  //----------------------------------------------------------------------
  bool GradientDescent::Minimize()
  {
    // Initialize at the input parameters
    std::vector<double>& pt = fVals;
    const unsigned int N = pt.size();

    // Use the user errors as a crude way to set the initial step size
    double step = Magnitude(fErrs);

    // Usually we reuse the chisq from the last iteration, so need to
    // initialize it once out here.
    fChi = (*fFunc)(pt.data());
    unsigned int ncalls = 1;

    //    std::vector<TGraph*> gs(N);
    //    for(auto& g: gs) g = new TGraph;

    while(true){
      // Evaluate gradient to figure out a direction to step in
      std::vector<double> grad(pt.size());
      fFunc->Gradient(pt.data(), grad.data());
      ++ncalls;

      // We'll step in the downhill direction
      std::vector<double> stepdir = grad;
      MakeUnit(stepdir);
      for(unsigned int i = 0; i < N; ++i) stepdir[i] *= -1;

      // And the change in the chisq per unit step in this direction is this
      double gradmag = 0;
      for(unsigned int i = 0; i < N; ++i) gradmag += grad[i] * stepdir[i];

      // Take step in that direction
      std::vector<double> trialpt = pt;
      for(unsigned int i = 0; i < N; ++i) trialpt[i] += stepdir[i]*step;
      // And evaluate the chisq there
      const double trialchi = (*fFunc)(trialpt.data());
      ++ncalls;

      // Estimate the second derivative from the two points and one
      // gradient
      const double d2 = 2 * (trialchi - gradmag*step - fChi)/(step*step);
      // We expect the function to be convex
      if(d2 > 0){
        // If so, we can compute a better step size, one that would place us
        // right at the minimum if the function only had quadratic terms.
        step = -gradmag/d2;
        //        std::cout << " new step = " << step << std::endl;
      }
      else{
        // This direction is still downhill locally, so let the step-halving
        // logic below sort it out.

        // std::cout << "Non-convexity" << std::endl;
      }

      while(true){
        // Keep trying steps until we find one that reduces the chisq
        std::vector<double> newpt = pt;
        for(unsigned int i = 0; i < N; ++i) newpt[i] += stepdir[i]*step;

        const double newchi = (*fFunc)(newpt.data());
        ++ncalls;

        if(newchi > fChi){
          // If the chisq went up instead, try again with smaller step
          step /= 2;
          continue;
        }

        // In all other cases (ie we took a reasonable step and found a
        // reasonably better chisq) we want to update our state, take another
        // look at the gradient vector to figure out which direction to go
        // next, and preserve our step size, which is some kind of good
        // estimate of the scale of the function.
        pt = newpt;
        fChi = newchi;

        if(ncalls > MaxFunctionCalls() || step < Precision()){
          /*
          TH2* axes = new TH2F("", "", gs[0]->GetN(), 0, gs[0]->GetN(), 100, -1, +1);
          axes->Draw();
          for(unsigned int i = 0; i < N; ++i) gs[i]->Draw("l same");
          gPad->Print("history.pdf");
          */

          return true;
        }
        else{
          break;
        }
      } // end line search

      //      for(unsigned int i = 0; i < N; ++i) gs[i]->SetPoint(gs[i]->GetN(), gs[i]->GetN(), pt[i]);

    } // end while (searching for better pt)

    // Unreached
    return true;
  }

  //----------------------------------------------------------------------
  double GradientDescent::Magnitude(const std::vector<double>& xs) const
  {
    double ret = 0;
    for(double x: xs) ret += x*x;
    return sqrt(ret);
  }

  //----------------------------------------------------------------------
  void GradientDescent::MakeUnit(std::vector<double>& xs) const
  {
    const double mag = Magnitude(xs);
    for(double& x: xs) x /= mag;
  }
}
