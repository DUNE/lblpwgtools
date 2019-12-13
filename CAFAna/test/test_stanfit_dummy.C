/*
 * test_stanfit_dummy.C:
 *    Test the use of the MCMC tool Stan for fitting in CAFAna.
 *    This version doesn't use any neutrinos, just the fitting mechanics.
 *
 *    Original author:  J. Wolcott <jwolcott@fnal.gov>
 *               Date:  November 2018
 */

#include <memory>

#include "TCanvas.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TMath.h"
#include "TROOT.h"

#include "CAFAna/Fit/StanConfig.h"
#include "CAFAna/Fit/StanFitter.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Experiment/ILkhdExperiment.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "Utilities/func/MathUtil.h"
#include "Utilities/func/Stan.h"

using namespace ana;

namespace test
{
  std::size_t N_POINTS = 5;
  double TRUE_A = 1.8;

  // try a starting point pretty far away from the truth
  double SEED_VAL = 25000;

  // a 'systematic' for fitting.
  // will actually be the only free parameter in our model...
  class QuadraticParameter : public ISyst
  {
    public:
      QuadraticParameter()
        : ISyst("QuadraticParameter", "quadratic scale factor")
      {}

      // req'd by the ISyst interface, but we aren't doing anything with it
      void Shift(double sigma,
                 Restorer& restore,
                 caf::StandardRecord* sr,
                 double& weight) const
      {}
  };
  const QuadraticParameter kQuadParam;

  double LogGauss(double x, double mu, double sigma)
  {
    const double offset = log(1./sqrt(2*TMath::Pi()));
    return offset - util::sqr(x - mu) / (2 * util::sqr(sigma));
  }

  // a very simple experiment that uses a log-Gaussian likelihood
  // to fit a single parameter in a quadratic function:
  // y = a * x^2
  class GaussQuadExperiment : public ILkhdExperiment
  {
    public:
      stan::math::var LogLikelihood(osc::IOscCalculatorAdjustableStan*,
                                    const SystShifts& syst = SystShifts::Nominal()) const override
      {
        stan::math::var ll = 0;
        for (unsigned int i = 1; i <= N_POINTS; i++)
        {
          auto ll_contrib = stan::math::normal_lpdf(TRUE_A * util::sqr(i),
                                                    syst.GetShift<stan::math::var>(&kQuadParam) * util::sqr(i), 1.);
          ll += ll_contrib;
//          std::cout << "For a = " << syst.GetShift(&kQuadParam) << ", LL = " << ll_contrib << std::endl;
        }
        return ll;
      }

  };
}

void test_stanfit_dummy()
{
  test::GaussQuadExperiment expt;



  StanFitter fitter(&expt, {}, {&test::kQuadParam});
  StanConfig config;
  config.num_warmup = 1000;
  config.num_samples = 10000;
//  config.max_depth = 17;
//  config.verbosity =  StanConfig::Verbosity::kVerbose;
  config.verbosity =  StanConfig::Verbosity::kQuiet;
  fitter.SetStanConfig(config);


  SystShifts shifts(&test::kQuadParam, 1);

  TCanvas c;
  TGraph ll;
  for (int x = -25; x <= 25; x++)
  {
    shifts.ResetToNominal();
    shifts.SetShift(&test::kQuadParam, x);
//    std::cout << "x = " << x << "; chi2 = " << expt.ChiSq(nullptr, shifts)/-2 << std::endl;
    ll.SetPoint(ll.GetN(), x, expt.LogLikelihood(nullptr, shifts).val());
  }
  ll.Draw("alp");
  c.SaveAs("test_stanfit_LL.png");

  std::cout << "Seeding fit parameter at value: " << test::SEED_VAL << std::endl;
  shifts.ResetToNominal();
  shifts.SetShift(&test::kQuadParam, test::SEED_VAL);

  fitter.Fit(shifts);
  //fitter.TestGradients(nullptr, shifts);

  std::cout << "Best fit: a = " << shifts.GetShift(&test::kQuadParam) << std::endl;
  std::cout << "  (true value was a = " << test::TRUE_A << ")" << std::endl;

  c.Clear();
  const_cast<TTree*>(fitter.GetSamples().ToTTree())->Draw("QuadraticParameter");
  c.SaveAs("test_stanfit_samples.png");
}

#ifndef __CINT__
int main(int argc, char const *argv[]) {
 gROOT->SetMustClean(false);
 test_stanfit_dummy();
}
#endif