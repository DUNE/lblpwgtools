#include "CAFAna/Vars/FitVarsProduct.h"

#include "OscLib/IOscCalc.h"

#include <cassert>
#include <cmath>

namespace ana
{
  // see the .h for why these are functions and not just const FitVarsProduct(Marg) objects
  FitVarsProduct& FitVarSinSqTheta23SinSq2Theta13()
  {
    static FitVarsProduct fvp(kFitSinSqTheta23, kFitSinSq2Theta13);
    return fvp;
  }
  FitVarsProductMarg& FitSinSqTheta23SinSq2Theta13Marg()
  {
    static FitVarsProductMarg fvpm(kFitSinSqTheta23, kFitSinSq2Theta13);
    return fvpm;
  }
  Fit2SinSqTheta23SinSq2Theta13& FitVar2SinSqTheta23SinSq2Theta13()
  {
    static Fit2SinSqTheta23SinSq2Theta13 f;
    return f;
  }

  // Helper functions
  namespace
  {
    // This is the definition of the coordinates alpha and beta
    void XYToAlphaBeta(double x, double y, double& alpha, double& beta)
    {
      assert(x >= 0 && x <= 1);
      assert(y >= 0 && y <= 1);

      if(x == 1 && y == 1){
        alpha = 1;
        beta = 1;
        return;
      }

      alpha = x*y;
      beta = 2/M_PI*atan2(1-x, 1-y);

      assert(alpha >= 0 && alpha <= 1);
      assert(beta >= 0 && beta <= 1);
    }

    // And this is the solution back to x and y
    void AlphaBetaToXY(double alpha, double beta, double& x, double& y)
    {
      assert(alpha >= 0 && alpha <= 1);
      assert(beta >= 0 && beta <= 1);

      if(beta == 0){
        x = 1;
        y = alpha/x;
        return;
      }
      if(beta == 1){
        y = 1;
        x = alpha/y;
        return;
      }

      const double phi = tan(beta*M_PI/2)-1;
      const double det = sqrt(4*alpha*(phi+1)+phi*phi);

      // These are the full expressions. But for very extreme values of beta
      // (near zero or one) there can be rounding problems. So evaluate the
      // easy expression and then ensure that alpha truly remains fixed.

      //      x = (det-phi)/2;
      //      y = (det+phi)/(2+2*phi);

      if(alpha == 0){
        x = (det-phi)/2;
        y = (det+phi)/(2+2*phi);
      }
      else{
        if(beta < .5){
          x = (det-phi)/2;
          y = alpha/x;
        }
        else{
          y = (det+phi)/(2+2*phi);
          x = alpha/y;
        }
      }

      assert(x >= 0 && x <= 1+1e-6);
      assert(y >= 0 && y <= 1+1e-6);

      x = std::min(x, 1.);
      y = std::min(y, 1.);
    }
  }

  //----------------------------------------------------------------------
  FitVarsProduct::FitVarsProduct(const IConstrainedFitVar& x,
                                 const IConstrainedFitVar& y)
    : IConstrainedFitVar(x.ShortName()+"*"+y.ShortName(),
                         x.LatexName()+y.LatexName()),
      fVarX(x), fVarY(y)
  {
    assert(x.LowLimit() == 0 && x.HighLimit() == 1);
    assert(y.LowLimit() == 0 && y.HighLimit() == 1);
  }

  //----------------------------------------------------------------------
  double FitVarsProduct::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    // Easy, just the product of the two
    double alpha, beta;
    XYToAlphaBeta(fVarX.GetValue(osc), fVarY.GetValue(osc), alpha, beta);
    return alpha;
  }

  //----------------------------------------------------------------------
  void FitVarsProduct::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    // Conver to the other space, hold beta (the marg variable) fixed and
    // adjust alpha, see where that leaves us.
    double alpha, beta;
    XYToAlphaBeta(fVarX.GetValue(osc), fVarY.GetValue(osc), alpha, beta);
    alpha = Clamp(val);

    double x, y;
    AlphaBetaToXY(alpha, beta, x, y);
    fVarX.SetValue(osc, x);
    fVarY.SetValue(osc, y);
  }

  //----------------------------------------------------------------------
  FitVarsProductMarg::FitVarsProductMarg(const IConstrainedFitVar& x,
                                         const IConstrainedFitVar& y)
    : IConstrainedFitVar(x.ShortName()+"*"+y.ShortName()+"_marg",
                         "Marginalization parameter for "+x.LatexName()+y.LatexName()),
      fVarX(x), fVarY(y)
  {
    assert(x.LowLimit() == 0 && x.HighLimit() == 1);
    assert(y.LowLimit() == 0 && y.HighLimit() == 1);
  }

  //----------------------------------------------------------------------
  double FitVarsProductMarg::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    // Just evaluate the formula for beta.
    double alpha, beta;
    XYToAlphaBeta(fVarX.GetValue(osc), fVarY.GetValue(osc), alpha, beta);
    return beta;
  }

  //----------------------------------------------------------------------
  void FitVarsProductMarg::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    // Switch to the other space, hold alpha constant and adjust beta, see
    // where that leaves us.
    double alpha, beta;
    XYToAlphaBeta(fVarX.GetValue(osc), fVarY.GetValue(osc), alpha, beta);
    beta = Clamp(val);

    double x, y;
    AlphaBetaToXY(alpha, beta, x, y);
    fVarX.SetValue(osc, x);
    fVarY.SetValue(osc, y);
  }
} // namespace