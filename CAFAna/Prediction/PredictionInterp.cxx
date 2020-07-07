#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Core/Registry.h"
#include "CAFAna/Core/Utilities.h"

#include "TDirectory.h"
#include "TH2.h"
#include "TMatrixD.h"
#include "TObjString.h"
#include "TVectorD.h"

// For debug plots
#include "TGraph.h"
#include "TCanvas.h"

#include "OscLib/func/IOscCalculator.h"
#include "Utilities/func/MathUtil.h"
#include "Utilities/func/Stan.h"
#include "Utilities/func/StanUtils.h"

#include "CAFAna/Core/Loaders.h"

#include <algorithm>
#include <malloc.h>

#ifdef USE_PREDINTERP_OMP
#include <omp.h>
#endif

namespace ana
{
  //----------------------------------------------------------------------
  PredictionInterp::PredictionInterp(std::vector<const ISyst*> systs,
                                     osc::IOscCalculator* osc,
                                     const IPredictionGenerator& predGen,
                                     Loaders& loaders,
                                     const SystShifts& shiftMC,
                                     EMode_t mode)
    : fOscOrigin(osc ? osc->Copy() : 0),
      fBinning(Spectrum::Uninitialized()),
      fSplitBySign(mode == kSplitBySign)
  {

    if(getenv("CAFANA_PRED_MINMCSTATS")){
      fMinMCStats = atoi(getenv("CAFANA_PRED_MINMCSTATS"));
    } else {
      fMinMCStats = 50;
    }


    for(const ISyst* syst: systs){
      ShiftedPreds sp;
      sp.systName = syst->ShortName();

      // Use whichever of these gives the most restrictive range
      const int x0 = std::max(-syst->PredInterpMaxNSigma(), int(trunc(syst->Min())));
      const int x1 = std::min(+syst->PredInterpMaxNSigma(), int(trunc(syst->Max())));

      for(int x = x0; x <= x1; ++x) sp.shifts.push_back(x);

      if(sp.shifts.size() < 2){
        std::cout << "PredictionInterp: " << syst->ShortName() << " with min = " << x0 << ", max = " << x1 << " only has " << sp.shifts.size() << " shifted values. Abort." << std::endl;
        abort();
      }

      for(int sigma: sp.shifts){
        SystShifts shiftHere = shiftMC;
        shiftHere.SetShift(syst, sigma);
        sp.preds.emplace_back(predGen.Generate(loaders, shiftHere));
      }

      fPreds.emplace_back(syst, std::move(sp));
    } // end for syst

    fPredNom = predGen.Generate(loaders, shiftMC);
  }

  //----------------------------------------------------------------------
  PredictionInterp::~PredictionInterp(){}

  //----------------------------------------------------------------------
  std::vector<std::vector<PredictionInterp::Coeffs>> PredictionInterp::
  FitRatios(const std::vector<double>& shifts,
            const std::vector<Eigen::ArrayXd>& ratios) const
  {
    if(ratios.size() < 2){
      std::cout << "PredictionInterp::FitRatios(): ratios.size() = " << ratios.size() << " - how did that happen?" << std::endl;

      abort();
    }

    assert(shifts.size() == ratios.size());

    std::vector<std::vector<Coeffs>> ret;

    const int binMax = ratios[0].size();

    for(int binIdx = 0; binIdx < binMax; ++binIdx){
      ret.push_back({});

      // This is cubic interpolation. For each adjacent set of four points we
      // determine coefficients for a cubic which will be the curve between the
      // center two. We constrain the function to match the two center points
      // and to have the right mean gradient at them. This causes this patch to
      // match smoothly with the next one along. The resulting function is
      // continuous and first and second differentiable. At the ends of the
      // range we fit a quadratic instead with only one constraint on the
      // slope. The coordinate conventions are that point y1 sits at x=0 and y2
      // at x=1. The matrices are simply the inverses of writing out the
      // constraints expressed above.

      // Special-case for linear interpolation
      if(ratios.size() == 2){
        const double y0 = ratios[0][binIdx];
        const double y1 = ratios[1][binIdx];

        ret.back().emplace_back(0, 0, y1-y0, y0);
        continue;
      }

      {
        const double y1 = ratios[0][binIdx];
        const double y2 = ratios[1][binIdx];
        const double y3 = ratios[2][binIdx];
        const double v[3] = {y1, y2, (y3-y1)/2};
        const double m[9] = { 1, -1,  1,
                             -2,  2, -1,
                              1,  0,  0};
        const TVectorD res = TMatrixD(3, 3, m) * TVectorD(3, v);
        ret.back().emplace_back(0, res(0), res(1), res(2));
      }

      // We're assuming here that the shifts are separated by exactly 1 sigma.
      for(unsigned int shiftIdx = 1; shiftIdx < ratios.size()-2; ++shiftIdx){
        const double y0 = ratios[shiftIdx-1][binIdx];
        const double y1 = ratios[shiftIdx  ][binIdx];
        const double y2 = ratios[shiftIdx+1][binIdx];
        const double y3 = ratios[shiftIdx+2][binIdx];

        const double v[4] = {y1, y2, (y2-y0)/2, (y3-y1)/2};
        const double m[16] = { 2, -2,  1,  1,
                              -3,  3, -2, -1,
                               0,  0,  1,  0,
                               1,  0,  0,  0};
        const TVectorD res = TMatrixD(4, 4, m) * TVectorD(4, v);
        ret.back().emplace_back(res(0), res(1), res(2), res(3));
      } // end for shiftIdx

      {
        const int N = ratios.size()-3;
        const double y0 = ratios[N  ][binIdx];
        const double y1 = ratios[N+1][binIdx];
        const double y2 = ratios[N+2][binIdx];
        const double v[3] = {y1, y2, (y2-y0)/2};
        const double m[9] = {-1,  1, -1,
                              0,  0,  1,
                              1,  0,  0};
        const TVectorD res = TMatrixD(3, 3, m) * TVectorD(3, v);
        ret.back().emplace_back(0, res(0), res(1), res(2));
      }
    } // end for binIdx

    double stride = -1;
    for(unsigned int i = 0; i < shifts.size()-1; ++i){
      const double newStride = shifts[i+1]-shifts[i];
      assert((stride < 0 || fabs(stride-newStride) < 1e-3) &&
             "Variably-spaced syst templates are unsupported");
      stride = newStride;
    }

    // If the stride is actually not 1, need to rescale all the coefficients
    for(std::vector<Coeffs>& cs: ret)
      for(Coeffs& c: cs){
        c = Coeffs(c.a/util::cube(stride),
                   c.b/util::sqr(stride),
                   c.c/stride,
                   c.d);}
    return ret;
  }

  //----------------------------------------------------------------------
  std::vector<std::vector<PredictionInterp::Coeffs>> PredictionInterp::
  FitComponent(const std::vector<double>& shifts,
               const std::vector<std::unique_ptr<IPrediction>>& preds,
               Flavors::Flavors_t flav,
               Current::Current_t curr,
               Sign::Sign_t sign,
               const std::string& systName) const
  {
    IPrediction const *pNom = nullptr;
    for(unsigned int i = 0; i < shifts.size(); ++i){
      if(shifts[i] == 0) {pNom = preds[i].get(); };
    }
    assert(pNom);

    // Do it this way rather than via fPredNom so that systematics evaluated
    // relative to some alternate nominal (eg Birks C where the appropriate
    // nominal is no-rock) can work.
    const Spectrum nom = pNom->PredictComponent(fOscOrigin,
						flav, curr, sign);

    std::vector<Eigen::ArrayXd> ratios;
    ratios.reserve(preds.size());
    for(auto& p: preds){
      ratios.emplace_back(Ratio(p->PredictComponent(fOscOrigin,
                                                    flav, curr, sign),
                                nom).GetEigen());

      // Check none of the ratio values is crazy
      Eigen::ArrayXd& r = ratios.back();
      for(int i = 0; i < r.size(); ++i){
        if (r[i] > 2){
          // std::cout << "PredictionInterp: WARNING, ratio in bin "
          // 	    << i << " for " << shifts[&p-&preds.front()]
          //           << " sigma shift of " << systName << " is " << y
          //           << " which exceeds limit of 2. Capping." << std::endl;
          r[i] = 2;
        }
      }
    }

    return FitRatios(shifts, ratios);
  }

  //----------------------------------------------------------------------
  void PredictionInterp::InitFitsHelper(ShiftedPreds& sp,
                                        std::vector<std::vector<std::vector<Coeffs>>>& fits,
                                        Sign::Sign_t sign) const
  {
    fits.resize(kNCoeffTypes);

    fits[kNueApp]   = FitComponent(sp.shifts, sp.preds, Flavors::kNuMuToNuE,  Current::kCC, sign, sp.systName);
    fits[kNueSurv]  = FitComponent(sp.shifts, sp.preds, Flavors::kNuEToNuE,   Current::kCC, sign, sp.systName);
    fits[kNumuSurv] = FitComponent(sp.shifts, sp.preds, Flavors::kNuMuToNuMu, Current::kCC, sign, sp.systName);

    fits[kNC]       = FitComponent(sp.shifts, sp.preds, Flavors::kAll, Current::kNC, sign, sp.systName);

    fits[kOther] = FitComponent(sp.shifts, sp.preds, Flavors::kNuEToNuMu | Flavors::kAllNuTau, Current::kCC, sign, sp.systName);
  }

  //----------------------------------------------------------------------
  void PredictionInterp::InitFits() const
  {
    // No systs
    if(fPreds.empty()){
      if(fBinning.POT() > 0 || fBinning.Livetime() > 0) return;
    }
    // Already initialized
    else if(!fPreds.empty() && !fPreds.begin()->second.fits.empty()) return;

    for(auto& it: fPreds){
      ShiftedPreds& sp = it.second;

      if(fSplitBySign){
        InitFitsHelper(sp, sp.fits, Sign::kNu);
        InitFitsHelper(sp, sp.fitsNubar, Sign::kAntiNu);
      }
      else{
        InitFitsHelper(sp, sp.fits, Sign::kBoth);
      }
      sp.nCoeffs = sp.fits[0][0].size();

      // Copy the outputs into the remapped indexing order. TODO this is very
      // ugly. Best would be to generate things in this order natively.

      sp.fitsRemap.resize(sp.fits.size());
      for(auto& it: sp.fitsRemap){
        it.resize(sp.fits[0][0].size());
        for(auto& it2: it) it2.resize(sp.fits[0].size(), Coeffs(0, 0, 0, 0));
      }

      for(unsigned int i = 0; i < sp.fitsRemap.size(); ++i){
        for(unsigned int j = 0; j < sp.fitsRemap[i].size(); ++j){
          for(unsigned int k = 0; k < sp.fitsRemap[i][j].size(); ++k){
            sp.fitsRemap[i][j][k] = sp.fits[i][k][j];
          }
        }
      }

      sp.fitsNubarRemap.resize(sp.fitsNubar.size());
      for(auto& it: sp.fitsNubarRemap){
        it.resize(sp.fitsNubar[0][0].size());
        for(auto& it2: it) it2.resize(sp.fitsNubar[0].size(), Coeffs(0, 0, 0, 0));
      }

      for(unsigned int i = 0; i < sp.fitsNubarRemap.size(); ++i){
        for(unsigned int j = 0; j < sp.fitsNubarRemap[i].size(); ++j){
          for(unsigned int k = 0; k < sp.fitsNubarRemap[i][j].size(); ++k){
            sp.fitsNubarRemap[i][j][k] = sp.fitsNubar[i][k][j];
          }
        }
      }

    }

    // Predict something, anything, so that we can know what binning to use
    fBinning = fPredNom->Predict(fOscOrigin);
    fBinning.Clear();
  }

  //----------------------------------------------------------------------
  void PredictionInterp::SetOscSeed(osc::IOscCalculator* oscSeed){
    fOscOrigin = oscSeed->Copy();
    for(auto& it: fPreds) it.second.fits.clear();
    InitFits();
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::Predict(osc::IOscCalculator* calc) const
  {
    return fPredNom->Predict(calc);
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::Predict(osc::IOscCalculatorStan* calc) const
  {
    return fPredNom->Predict(calc);
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::PredictComponent(osc::IOscCalculator* calc,
                                              Flavors::Flavors_t flav,
                                              Current::Current_t curr,
                                              Sign::Sign_t sign) const
  {
    return fPredNom->PredictComponent(calc, flav, curr, sign);
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::PredictComponent(osc::IOscCalculatorStan* calc,
                                              Flavors::Flavors_t flav,
                                              Current::Current_t curr,
                                              Sign::Sign_t sign) const
  {
    return fPredNom->PredictComponent(calc, flav, curr, sign);
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::PredictSyst(osc::IOscCalculator* calc,
                                         const SystShifts& shift) const
  {
    InitFits();

    return PredictComponentSyst(calc, shift,
                                Flavors::kAll,
                                Current::kBoth,
                                Sign::kBoth);
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::PredictSyst(osc::IOscCalculatorStan* calc,
                                         const SystShifts& shift) const
  {
    InitFits();

    return PredictComponentSyst(calc,
                                shift,
                                Flavors::kAll,
                                Current::kBoth,
                                Sign::kBoth);
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::ShiftSpectrum(const Spectrum &s, CoeffsType type,
                                           bool nubar,
                                           const SystShifts &shift) const
  {
    if(s.HasStan() && !shift.HasAnyStan()){
      std::cout << "PredictionInterp used with stan oscillations but non-stan syst shifts. Is that what you expected?" << std::endl;
    }

    if(s.HasStan() || shift.HasAnyStan()){
      // Need the second case for the NC component
      Eigen::ArrayXstan vec = s.HasStan() ? s.GetEigenStan(s.POT()) : Eigen::ArrayXstan(s.GetEigen(s.POT()));

      ShiftBins(vec.size(), vec.data(), type, nubar, shift);
      return Spectrum(std::move(vec), s.GetLabels(), s.GetBinnings(), s.POT(), s.Livetime());
    }
    else{
      Eigen::ArrayXd vec = s.GetEigen(s.POT());
      ShiftBins(vec.size(), vec.data(), type, nubar, shift);
      return Spectrum(std::move(vec), s.GetLabels(), s.GetBinnings(), s.POT(), s.Livetime());
    }
  }

  //----------------------------------------------------------------------
  template <typename T>
  void PredictionInterp::ShiftBins(unsigned int N,
                                   T* arr,
                                   CoeffsType type,
                                   bool nubar,
                                   const SystShifts& shift) const
  {
    static_assert(std::is_same_v<T, double> ||
                  std::is_same_v<T, stan::math::var>,
                  "PredictionInterp::ShiftBins() can only be called using doubles or stan::math::vars");
    if(nubar) assert(fSplitBySign);


    if(!std::is_same_v<T, stan::math::var> && shift.HasAnyStan()){
      std::cout << "PredictionInterp: stan shifts on non-stan spectrum, something is wrong" << std::endl;
      abort();
    }

#ifdef USE_PREDINTERP_OMP
    T corr[4][N];
    for (unsigned int i = 0; i < 4; ++i) {
      for (unsigned int j = 0; j < N; ++j) {
        corr[i][j] = 1;
      };
    }
#else
    T corr[N];
    for(unsigned int i = 0; i < N; ++i) corr[i] = 1;
#endif

    size_t NPreds = fPreds.size();

    #pragma omp parallel for
    for (size_t p_it = 0; p_it < NPreds; ++p_it) {
      const ISyst *syst = fPreds[p_it].first;
      const ShiftedPreds &sp = fPreds[p_it].second;

      T x = shift.GetShift<T>(syst);

      // need to actually do the calculation for the autodiff version
      // to make sure the gradient is computed correctly
      if(x == 0 && (!std::is_same_v<T, stan::math::var> || !shift.HasStan(syst))) continue;

      int shiftBin = (util::GetValAs<double>(x) - sp.shifts[0])/sp.Stride();
      shiftBin = std::max(0, shiftBin);
      shiftBin = std::min(shiftBin, sp.nCoeffs - 1);

      const Coeffs *fits = nubar ? &sp.fitsNubarRemap[type][shiftBin].front()
                                 : &sp.fitsRemap[type][shiftBin].front();

      x -= sp.shifts[shiftBin];

      const T x_cube = util::cube(x);
      const T x_sqr = util::sqr(x);

#ifdef USE_PREDINTERP_OMP
      ShiftSpectrumKernel(fits, N, x, x_sqr, x_cube,
                          corr[omp_get_thread_num()]);
#else
      ShiftSpectrumKernel(fits, N, x, x_sqr, x_cube, corr);
#endif
    } // end for syst

#ifdef USE_PREDINTERP_OMP
    for (unsigned int i = 1; i < 4; ++i) {
      for (unsigned int j = 0; j < N; ++j) {
        corr[0][j] *= corr[i][j];
      };
    }
#endif

    for (unsigned int n = 0; n < N; ++n) {
      if (arr[n] > fMinMCStats) {
#ifdef USE_PREDINTERP_OMP
        // std::max() doesn't work with stan::math::var
        arr[n] *= (corr[0][n] > 0.) ? corr[0][n] : 0.;
#else
        arr[n] *= (corr[n] > 0.) ? corr[n] : 0.;
#endif
      }
    }
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::ShiftedComponent(osc::IOscCalculator* calc,
                                              const TMD5* hash,
                                              const SystShifts& shift,
                                              Flavors::Flavors_t flav,
                                              Current::Current_t curr,
                                              Sign::Sign_t sign,
                                              CoeffsType type) const
  {
    return _ShiftedComponent(calc, hash, shift, flav, curr, sign, type);
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::ShiftedComponent(osc::_IOscCalculator<stan::math::var>* calc,
                                              const TMD5* hash,
                                              const SystShifts& shift,
                                              Flavors::Flavors_t flav,
                                              Current::Current_t curr,
                                              Sign::Sign_t sign,
                                              CoeffsType type) const
  {
    return _ShiftedComponent(calc, hash, shift, flav, curr, sign, type);
  }


  //----------------------------------------------------------------------
  template<typename T>
  Spectrum PredictionInterp::_ShiftedComponent(osc::_IOscCalculator<T>* calc,
                                               const TMD5* hash,
                                               const SystShifts& shift,
                                               Flavors::Flavors_t flav,
                                               Current::Current_t curr,
                                               Sign::Sign_t sign,
                                               CoeffsType type) const
  {
    static_assert(std::is_same<T, double>::value || std::is_same<T, stan::math::var>::value,
                  "PredictionInterp::ShiftedComponent() can only be called using doubles or stan::math::vars");
    if(fSplitBySign && sign == Sign::kBoth){
      return (ShiftedComponent(calc, hash, shift, flav, curr, Sign::kAntiNu, type)+
              ShiftedComponent(calc, hash, shift, flav, curr, Sign::kNu,     type));
    }

    // Must be the base case of the recursion to use the cache. Otherwise we
    // can cache systematically shifted versions of our children, which is
    // wrong. Also, some calculators won't hash themselves.
    // Moreover, caching is not going to work with stan::math::vars
    // since they get reset every time Stan's log_prob() is called.
    const bool canCache = (hash != 0) && !std::is_same<T, stan::math::var>::value;

    const Key_t key = {flav, curr, sign};
    auto it = fNomCache.find(key);

    // Should the interpolation use the nubar fits?
    const bool nubar = (fSplitBySign && sign == Sign::kAntiNu);

    // We have the nominal for this exact combination of flav, curr, sign, calc
    // stored.  Shift it and return.
    if(canCache && it != fNomCache.end() && it->second.hash == *hash){
      return ShiftSpectrum(it->second.nom, type, nubar, shift);
    }

    // We need to compute the nominal again for whatever reason
    const Spectrum nom = fPredNom->PredictComponent(calc, flav, curr, sign);

    if(canCache){
      // Insert into the cache if not already there, or update if there but
      // with old oscillation parameters.
      if(it == fNomCache.end())
        fNomCache.emplace(key, Val_t({*hash, nom}));
      else
        it->second = {*hash, nom};
    }

    return ShiftSpectrum(nom, type, nubar, shift);
  }

  void PredictionInterp::DiscardSysts(std::vector<ISyst const *> const &systs) {

    size_t NPreds = fPreds.size();
    for (auto s : systs) {
      auto it = find_pred(s);
      if(it != fPreds.end()){
        fPreds.erase(it);
      }
    }
    HistCache::ClearCache();
  }

  std::vector<ISyst const *> PredictionInterp::GetAllSysts() const {
    std::vector<ISyst const *> allsysts;
    for (auto const &p : fPreds) {
      allsysts.push_back(p.first);
    }
    return allsysts;
  }

  //----------------------------------------------------------------------
  template<typename T>
  Spectrum PredictionInterp::_PredictComponentSyst(osc::_IOscCalculator<T>* calc,
                                                   const SystShifts& shift,
                                                   Flavors::Flavors_t flav,
                                                   Current::Current_t curr,
                                                   Sign::Sign_t sign) const
  {
    InitFits();

    Spectrum ret = fBinning;
    ret.Clear();

    assert (ret.POT() > 0 && "Can't PredictComponentSyst() for 0 POT");

    // Check that we're able to handle all the systs we were passed
    for(const ISyst* syst: shift.ActiveSysts()){
      if(find_pred(syst) == fPreds.end()){
        std::cerr << "This PredictionInterp is not set up to handle the requested systematic: " << syst->ShortName() << std::endl;
        abort();
      }
    } // end for syst


    const TMD5* hash = calc ? calc->GetParamsHash() : 0;

    if(curr & Current::kCC){
      if(flav & Flavors::kNuEToNuE)    ret += ShiftedComponent(calc, hash, shift, Flavors::kNuEToNuE,    Current::kCC, sign, kNueSurv);
      if(flav & Flavors::kNuEToNuMu)   ret += ShiftedComponent(calc, hash, shift, Flavors::kNuEToNuMu,   Current::kCC, sign, kOther  );
      if(flav & Flavors::kNuEToNuTau)  ret += ShiftedComponent(calc, hash, shift, Flavors::kNuEToNuTau,  Current::kCC, sign, kOther  );

      if(flav & Flavors::kNuMuToNuE)   ret += ShiftedComponent(calc, hash, shift, Flavors::kNuMuToNuE,   Current::kCC, sign, kNueApp  );
      if(flav & Flavors::kNuMuToNuMu)  ret += ShiftedComponent(calc, hash, shift, Flavors::kNuMuToNuMu,  Current::kCC, sign, kNumuSurv);
      if(flav & Flavors::kNuMuToNuTau) ret += ShiftedComponent(calc, hash, shift, Flavors::kNuMuToNuTau, Current::kCC, sign, kOther   );
    }
    if(curr & Current::kNC){
      assert(flav == Flavors::kAll); // Don't know how to calculate anything else

      ret += ShiftedComponent(calc, hash, shift, Flavors::kAll, Current::kNC, sign, kNC);
    }

    delete hash;

    return ret;
  }

  //----------------------------------------------------------------------
  // can't template these directly since the interface isn't templated
  Spectrum PredictionInterp::PredictComponentSyst(osc::IOscCalculator* calc,
                                                  const SystShifts& shift,
                                                  Flavors::Flavors_t flav,
                                                  Current::Current_t curr,
                                                  Sign::Sign_t sign) const
  {
    return _PredictComponentSyst(calc, shift, flav, curr, sign);
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::PredictComponentSyst(osc::IOscCalculatorStan* calc,
                                                  const SystShifts& shift,
                                                  Flavors::Flavors_t flav,
                                                  Current::Current_t curr,
                                                  Sign::Sign_t sign) const
  {
    return _PredictComponentSyst(calc, shift, flav, curr, sign);
  }

  void PredictionInterp::SaveTo(TDirectory* dir, const std::string& name) const
  {
    InitFits();

    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("PredictionInterp").Write("type");

    fPredNom->SaveTo(dir, "pred_nom");

    for(auto &it: fPreds){
      const ShiftedPreds& sp = it.second;

      for(unsigned int i = 0; i < sp.shifts.size(); ++i){
        if(!sp.preds[i]){
          std::cout << "Can't save a PredictionInterp after MinimizeMemory()" << std::endl;
          abort();
        }

        sp.preds[i]->SaveTo(dir, TString::Format("pred_%s_%+d",
                                                 sp.systName.c_str(),
                                                 int(sp.shifts[i])).Data());
      } // end for i
    } // end for it

    ana::SaveTo(*fOscOrigin, dir, "osc_origin");

    if(!fPreds.empty()){
      TH1F hSystNames("syst_names", ";Syst names", fPreds.size(), 0, fPreds.size());
      int binIdx = 1;
      for(auto &it: fPreds){
        hSystNames.GetXaxis()->SetBinLabel(binIdx++, it.second.systName.c_str());
      }
      dir->cd();
      hSystNames.Write("syst_names");
    }

    TObjString split_sign = fSplitBySign ? "yes" : "no";
    dir->cd();
    split_sign.Write("split_sign");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionInterp> PredictionInterp::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    TObjString* tag = (TObjString*)dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "PredictionInterp");

    std::unique_ptr<PredictionInterp> ret(new PredictionInterp);

    LoadFromBody(dir, ret.get());

    TObjString* split_sign = (TObjString*)dir->Get("split_sign");
    // Can be missing from old files
    ret->fSplitBySign = (split_sign && split_sign->String() == "yes");

    delete dir;

    return ret;
  }

  //----------------------------------------------------------------------
  void PredictionInterp::LoadFromBody(TDirectory* dir, PredictionInterp* ret,
				      std::vector<const ISyst*> veto)
  {
    ret->fPredNom = ana::LoadFrom<IPrediction>(dir, "pred_nom");

    TH1* hSystNames = (TH1*)dir->Get("syst_names");
    if(hSystNames){
      for(int systIdx = 0; systIdx < hSystNames->GetNbinsX(); ++systIdx)
      {
        ShiftedPreds sp;
        sp.systName = hSystNames->GetXaxis()->GetBinLabel(systIdx + 1);

        const ISyst *syst = Registry<ISyst>::ShortNameToPtr(sp.systName, true);
        if (!syst)
        {
          std::cout << "PredictionInterp:  Couldn't match stored syst '" << sp.systName << "' to an ISyst*.  Ignoring!"
                    << std::endl;
          continue;
        }

        if (std::find(veto.begin(), veto.end(), syst) != veto.end()) continue;

        // Use whichever of these gives the most restrictive range
        const int x0 = std::max(-syst->PredInterpMaxNSigma(), int(trunc(syst->Min())));
        const int x1 = std::min(+syst->PredInterpMaxNSigma(), int(trunc(syst->Max())));

        if (std::abs(x0 - x1) < 1)
        {
          std::cout << "Warning: Syst '" << sp.systName
                    << "' has less than 1sigma of allowed range.  Won't interpolate it!" << std::endl;
          continue;
        }

        for (int shift = x0; shift <= x1; ++shift)
        {
          const std::string subname = TString::Format("pred_%s_%+d", sp.systName.c_str(), shift).Data();
          TDirectory *preddir = dir->GetDirectory(subname.c_str());
          if (!preddir)
          {
            std::cout << "PredictionInterp: " << syst->ShortName() << " " << shift << " sigma " << " not found in "
                      << dir->GetName() << std::endl;
            continue;
          }
          delete preddir;

          sp.shifts.push_back(shift);
          sp.preds.emplace_back(ana::LoadFrom<IPrediction>(dir, subname));
        } // end for shift

        ret->fPreds.emplace_back(syst, std::move(sp));
      } // end for systIdx
    } // end if hSystNames

    ret->fOscOrigin = ana::LoadFrom<osc::IOscCalculator>(dir, "osc_origin").release();
  }

  //----------------------------------------------------------------------
  void PredictionInterp::MinimizeMemory()
  {
    DiscardSysts(GetAllSysts());
  }

  //----------------------------------------------------------------------
  void PredictionInterp::DebugPlot(const ISyst* syst,
                                   osc::IOscCalculator* calc,
                                   Flavors::Flavors_t flav,
                                   Current::Current_t curr,
                                   Sign::Sign_t sign) const
  {
    InitFits();

    auto it = find_pred(syst);
    if(it == fPreds.end()){
      std::cout << "PredictionInterp::DebugPlots(): "
                << syst->ShortName() << " not found" << std::endl;
      return;
    }

    std::unique_ptr<TH1> nom(fPredNom->PredictComponent(calc, flav, curr, sign).ToTH1(18e20));
    const int nbins = nom->GetNbinsX();

    TGraph* curves[nbins];
    TGraph* points[nbins];

    for(int i = 0; i <= 80; ++i){
      const double x = .1*i-4;
      const SystShifts ss(it->first, x);
      std::unique_ptr<TH1> h(PredictComponentSyst(calc, ss, flav, curr, sign).ToTH1(18e20));

      for(int bin = 0; bin < nbins; ++bin){
        if(i == 0){
          curves[bin] = new TGraph;
          points[bin] = new TGraph;
        }

        const double ratio = h->GetBinContent(bin+1)/nom->GetBinContent(bin+1);

        if(!std::isnan(ratio)) curves[bin]->SetPoint(curves[bin]->GetN(), x, ratio);
        else curves[bin]->SetPoint(curves[bin]->GetN(), x, 1);
      } // end for bin
    } // end for i (x)

    // As elswhere, to allow BirksC etc that need a different nominal to plot
    // right.
    IPrediction const* pNom = 0;
    for(unsigned int shiftIdx = 0; shiftIdx < it->second.shifts.size(); ++shiftIdx){
      if(it->second.shifts[shiftIdx] == 0) {pNom = it->second.preds[shiftIdx].get();}
    }
    assert(pNom);
    std::unique_ptr<TH1> hnom(pNom->PredictComponent(calc, flav, curr, sign).ToTH1(18e20));

    for(unsigned int shiftIdx = 0; shiftIdx < it->second.shifts.size(); ++shiftIdx){
      if(!it->second.preds[shiftIdx]) continue; // Probably MinimizeMemory()
      std::unique_ptr<TH1> h;
        h = std::move(std::unique_ptr<TH1>(it->second.preds[shiftIdx]->PredictComponent(calc, flav, curr, sign).ToTH1(18e20)));

      for(int bin = 0; bin < nbins; ++bin){
        const double ratio = h->GetBinContent(bin+1)/hnom->GetBinContent(bin+1);
        if(!std::isnan(ratio)) points[bin]->SetPoint(points[bin]->GetN(), it->second.shifts[shiftIdx], ratio);
        else points[bin]->SetPoint(points[bin]->GetN(), it->second.shifts[shiftIdx], 1);
      }
    } // end for shiftIdx


    int nx = int(sqrt(nbins));
    int ny = int(sqrt(nbins));
    if(nx*ny < nbins) ++nx;
    if(nx*ny < nbins) ++ny;

    TCanvas* c = new TCanvas;
    c->Divide(nx, ny);

    for(int bin = 0; bin < nbins; ++bin){
      c->cd(bin+1);
      (new TH2F("",
                TString::Format("%s %g < %s < %g;Shift;Ratio",
                                it->second.systName.c_str(),
                                nom->GetXaxis()->GetBinLowEdge(bin+1),
                                nom->GetXaxis()->GetTitle(),
                                nom->GetXaxis()->GetBinUpEdge(bin+1)),
                100, -4, +4, 100, .5, 1.5))->Draw();
      curves[bin]->Draw("l same");
      points[bin]->SetMarkerStyle(kFullDotMedium);
      points[bin]->Draw("p same");
    } // end for bin

    c->cd(0);
  }

  //----------------------------------------------------------------------
  void PredictionInterp::DebugPlots(osc::IOscCalculator* calc,
				    const std::string& savePattern,
				    Flavors::Flavors_t flav,
				    Current::Current_t curr,
				    Sign::Sign_t sign) const
  {
    for(auto& it: fPreds){
      DebugPlot(it.first, calc, flav, curr, sign);

      if(!savePattern.empty()){
	assert(savePattern.find("%s") != std::string::npos);
	gPad->Print(TString::Format(savePattern.c_str(), it.second.systName.c_str()).Data());
      }
    } // end for it
  }

  //----------------------------------------------------------------------
  void PredictionInterp::DebugPlotColz(const ISyst* syst,
                                       osc::IOscCalculator* calc,
                                       Flavors::Flavors_t flav,
                                       Current::Current_t curr,
                                       Sign::Sign_t sign) const
  {
    InitFits();

    std::unique_ptr<TH1> nom(fPredNom->PredictComponent(calc, flav, curr, sign).ToTH1(18e20));
    const int nbins = nom->GetNbinsX();

    TH2* h2 = new TH2F("", (syst->LatexName()+";;#sigma").c_str(),
                       nbins, nom->GetXaxis()->GetXmin(), nom->GetXaxis()->GetXmax(),
                       80, -4, +4);
    h2->GetXaxis()->SetTitle(nom->GetXaxis()->GetTitle());

    for(int i = 1; i <= 80; ++i){
      const double y = h2->GetYaxis()->GetBinCenter(i);
      const SystShifts ss(syst, y);
      std::unique_ptr<TH1> h(PredictComponentSyst(calc, ss, flav, curr, sign).ToTH1(18e20));

      for(int bin = 0; bin < nbins; ++bin){
        const double ratio = h->GetBinContent(bin+1)/nom->GetBinContent(bin+1);

        if(!isnan(ratio) && !isinf(ratio))
          h2->Fill(h2->GetXaxis()->GetBinCenter(bin), y, ratio);
      } // end for bin
    } // end for i (x)

    h2->Draw("colz");
    h2->SetMinimum(0.5);
    h2->SetMaximum(1.5);
  }

  //----------------------------------------------------------------------
  void PredictionInterp::DebugPlotsColz(osc::IOscCalculator* calc,
                                        const std::string& savePattern,
                                        Flavors::Flavors_t flav,
                                        Current::Current_t curr,
                                        Sign::Sign_t sign) const
  {
    InitFits();

    for(auto &it: fPreds){
      new TCanvas;
      DebugPlotColz(it.first, calc, flav, curr, sign);

      if(!savePattern.empty()){
	assert(savePattern.find("%s") != std::string::npos);
	gPad->Print(TString::Format(savePattern.c_str(), it.second.systName.c_str()).Data());
      }
    } // end for it
  }

} // namespace
