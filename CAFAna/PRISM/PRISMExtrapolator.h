#pragma once

#include "CAFAna/Core/SystShifts.h"

#include "Eigen/Dense"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace osc {
class IOscCalculator;
}
namespace ana {
class Binning;
class PredictionInterp;
} // namespace ana

class TH1;
class TH2;
class TDirectory;

namespace ana {

// Caching flux matcher.
class PRISMExtrapolator {
public:
  enum class FluxPredSpecies {
    kNumu_numode = 0,
    kNue_numode = 1,
    kNumubar_numode = 2,
    kNuebar_numode = 3,

    kNumu_nubarmode = 4,
    kNue_nubarmode = 5,
    kNumubar_nubarmode = 6,
    kNuebar_nubarmode = 7,

    kUnhandled = 8
  };

  PRISMExtrapolator();

  void InitializeFluxMatcher(std::string const &FluxFilePath,
                             int OffAxisBinMerge = 1, int NDEnergyBinMerge = 1,
                             int FDEnergyBinMerge = 1);

  void InitializeEventRateMatcher(PredictionInterp const *NDEventRateInterp,
                                  PredictionInterp const *FDEventRateInterp);

  /// Use to check whether the ND flux histograms are binned consistently with
  /// the proposed ND analysis off axis binning. Will not merge bins, but will
  /// ignore extra bins in the flux prediction
  bool CheckOffAxisBinningConsistency(Binning const &) const;

  void SetStoreDebugMatches() { fStoreDebugMatches = true; }

  TH1 const *GetMatchCoefficientsEventRate(osc::IOscCalculator *osc,
                                           double max_OffAxis_m,
                                           SystShifts shift = kNoShift) const;
  TH1 const *GetMatchCoefficientsFlux(
      osc::IOscCalculator *osc, double max_OffAxis_m,
      FluxPredSpecies NDMode = FluxPredSpecies::kNumu_numode,
      FluxPredSpecies FDMode = FluxPredSpecies::kNumu_numode) const;
  TH1 const *
  GetMatchCoefficients(osc::IOscCalculator *osc, double max_OffAxis_m,
                       FluxPredSpecies NDMode = FluxPredSpecies::kNumu_numode,
                       FluxPredSpecies FDMode = FluxPredSpecies::kNumu_numode,
                       SystShifts const &shift = kNoShift) const;

  TH1 const *GetLastResidual() const { return fLastResidual.get(); }

  void Write(TDirectory *);

  void
  SetTargetConditioning(double RegFactor = 1E-8, double LowECutoff = 0,
                        double HighECutoff = std::numeric_limits<double>::max(),
                        bool LowEGaussTail = false) {
    fRegFactor = RegFactor;
    fENuMin = LowECutoff;
    fENuMax = HighECutoff;
    fLowEGaussFallOff = LowEGaussTail;
  }

protected:
  std::vector<std::unique_ptr<TH2>> NDOffAxisPrediction;
  std::vector<std::unique_ptr<TH1>> FDUnOscPrediction;

  mutable std::unique_ptr<TH1> fLastResidual;

  PredictionInterp const *fFDEventRateInterp;
  PredictionInterp const *fNDEventRateInterp;

  bool fMatchEventRates;

  double fRegFactor;
  double fENuMin;
  double fENuMax;
  bool fLowEGaussFallOff;

  mutable std::map<std::string, std::unique_ptr<TH1>> fMatchCache;

  bool fStoreDebugMatches;
  mutable std::map<std::string, std::unique_ptr<TH1>> fDebugTarget;
  mutable std::map<std::string, std::unique_ptr<TH1>> fDebugBF;
  mutable std::map<std::string, std::unique_ptr<TH2>> fDebugND;
};

} // namespace ana

size_t
FillHistFromEigenVector(TH2 *, Eigen::VectorXd const &, size_t vect_offset = 0,
                        size_t histx_offset = 0, size_t histy_offset = 0,
                        Eigen::VectorXd const &error = Eigen::VectorXd());
size_t
FillHistFromEigenVector(TH1 *, Eigen::VectorXd const &, size_t vect_offset = 0,
                        size_t histx_offset = 0, size_t histy_offset = 0,
                        Eigen::VectorXd const &error = Eigen::VectorXd());
template <typename THN>
inline size_t
FillHistFromEigenVector(std::vector<std::unique_ptr<THN>> &rhv,
                        Eigen::VectorXd const &vals, size_t vect_offset = 0,
                        size_t histx_offset = 0, size_t histy_offset = 0,
                        Eigen::VectorXd const &error = Eigen::VectorXd()) {

  for (std::unique_ptr<THN> const &rh : rhv) {
    vect_offset = FillHistFromEigenVector(rh.get(), vals, vect_offset,
                                          histx_offset, histy_offset, error);
  }
  return vect_offset;
}

std::vector<double> Getstdvector(TH2 const *);
std::vector<double> Getstdvector(TH1 const *);

template <typename T>
inline void Mergestdvector(std::vector<T> &a, std::vector<T> const &b) {
  for (T const &b_i : b) {
    a.push_back(b_i);
  }
}

template <typename T>
inline void Mergestdvector(std::vector<T> &a, std::vector<T> &&b) {
  for (T &b_i : b) {
    a.push_back(std::move(b_i));
  }
}

template <typename THN>
inline std::vector<double>
Getstdvector(std::vector<std::unique_ptr<THN>> const &rhv) {
  std::vector<double> ev;

  for (std::unique_ptr<THN> const &rh : rhv) {
    Mergestdvector(ev, Getstdvector(rh.get()));
  }

  return ev;
}

Eigen::MatrixXd
GetEigenMatrix(TH2 const *,
               size_t max_rows = std::numeric_limits<size_t>::max(),
               size_t max_cols = std::numeric_limits<size_t>::max());
Eigen::VectorXd GetEigenFlatVector(std::vector<double> const &);
Eigen::VectorXd GetEigenFlatVector(TH1 const *);

Eigen::VectorXd SolveLinComb(TH2 const *NDPred, TH1 const *FDOsc,
                             double max_OffAxis_m, double EMin = 0,
                             double EMax = std::numeric_limits<double>::max(),
                             double reg_factor = 0);
