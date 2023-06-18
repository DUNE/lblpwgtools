#pragma once

#include "CAFAna/Analysis/AnalysisBinnings.h"
#include "CAFAna/Analysis/AnalysisDialGroups.h"

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/SystShifts.h"

#include "CAFAna/Experiment/IExperiment.h"

#include "CAFAna/Fit/Fit.h"
#include "CAFAna/Fit/SeedList.h"

#include "CAFAna/Prediction/PredictionInterp.h"

#include "CAFAna/Systs/AnaSysts.h"

#include "OscLib/IOscCalc.h"

#include "TDirectory.h"

// Extra headers not required
#include "CAFAna/Analysis/ExtraHeaders.h"

#include <memory>
#include <string>
#include <vector>

extern unsigned gRNGSeed;

// Factor to account for simulated LAr mass/expected FV, pot_fd = pot_nd *
// pot_fd_FVMassFactor
extern double const pot_fd_FVMassFactor;
extern double const pot_fd2_FVMassFactor;
extern double const pot_fd3_FVMassFactor;
// POT for 3.5 years
extern double const pot_fd;
extern double const pot_nd;

// This is pretty annoying, but the above is for 7 years staged, which is 336 kT
// MW yr
extern double const nom_exposure;

// Exposure scaling for FD accounting for number of modules and beam upgrade
std::pair<double, double> ExposureScale(double &years, std::string staging_plan = "nostage");
// Exposure scaling for ND accounts for beam upgrade to 2.4 MW
std::pair<double, double> ExposureScaleND(double &years, std::string staging_plan = "nostage");

double GetBoundedGausThrow(double min, double max);

// For ease of penalty terms...
ana::IExperiment *GetPenalty(int hie, int oct, std::string penalty,
                             std::string asimov_set = "0",
                             bool modConstraint = false);

std::vector<const ana::IFitVar *>
GetOscVars(std::string oscVarString = "alloscvars", int hie = 0, int oct = 0);

ana::SystShifts GetFakeDataGeneratorSystShift(std::string input);

enum SampleType { kFDFHC, kFDRHC, kNDFHC, kNDRHC, kNDNue, kNDFHC_OA, kUnknown };
std::string GetSampleName(SampleType sample);
SampleType GetSampleType(std::string const &sample);

void MakePredictionInterp(
    TDirectory *saveDir, SampleType sample,
    std::vector<const ana::ISyst *> systlist, AxisBlob const &axes,
    std::vector<std::string> const &non_swap_file_list,
    std::vector<std::string> const &nue_swap_file_list = {},
    std::vector<std::string> const &tau_swap_file_list = {}, int max = 0);

std::vector<std::unique_ptr<ana::PredictionInterp>>
GetPredictionInterps(std::string fileName,
                     std::vector<const ana::ISyst *> systlist);

TH2D *make_corr_from_covar(TH2D *covar);

// Yet another string parser that does far too much. I can't be stopped!
void ParseDataSamples(std::string cmdLineInput, double &pot_nd_fhc,
                      double &pot_nd_rhc, double &pot_fd_fhc_nue,
                      double &pot_fd_rhc_nue, double &pot_fd_fhc_numu,
                      double &pot_fd_rhc_numu);

void ParseThrowInstructions(std::string throwString, bool &stats, bool &fakeOA,
                            bool &fakeNuis, bool &start, bool &central);

TMatrixD *GetNDCovMat(bool UseV3NDCovMat = false,
                      bool TwoBeams = true,
                      bool isFHC = true);

TMatrixD *MakeCovmat(ana::PredictionInterp const &prediction,
                     std::vector<ana::ISyst const *> const &systs,
                     osc::IOscCalcAdjustable *calc, size_t NToys = 1000,
                     TDirectory *outdir = nullptr);

void SaveTrueOAParams(TDirectory *outDir, osc::IOscCalcAdjustable *calc,
                      std::string tree_name = "true_OA");

void SaveParams(TDirectory *outDir, std::vector<const ana::ISyst *> systlist);

struct FitTreeBlob {
  FitTreeBlob(std::string tree_name = "", std::string meta_tree_name = "");
  static FitTreeBlob *MakeReader(TTree *t, TTree *m);
  ~FitTreeBlob();
  void CopyVals(FitTreeBlob const &fb);
  void Fill();
  void SetDirectory(TDirectory *d);
  void SetDirectoryClone(TDirectory *d);
  void Write();
  void WriteClone();
  bool fMeta_filled;
  TTree *throw_tree;
  TTree *meta_tree;
  std::vector<double> *fFakeDataVals;
  std::vector<std::string> *fParamNames;
  std::vector<std::string> *fEnvVarNames;
  std::vector<std::string> *fEnvVarValues;
  std::vector<double> *fPreFitValues;
  std::vector<double> *fPreFitErrors;
  std::vector<double> *fPostFitValues;
  std::vector<double> *fPostFitErrors;
  std::vector<double> *fCentralValues;
  double fChiSq;
  unsigned fNSeconds;
  unsigned fLoopRNGSeed;
  unsigned fJobRNGSeed;
  std::vector<unsigned> *fSpectraRNGSeeds;
  unsigned fNFills;
  unsigned fNOscSeeds;
  double fResMemUsage;
  double fVirtMemUsage;
  double fNFCN;
  double fEDM;
  bool fIsValid;
#ifdef USE_PREDINTERP_OMP
  unsigned fNMaxThreads;
#endif
};

std::string BuildLogInfoString();

struct seeded_spectra {
  seeded_spectra(unsigned ss, std::unique_ptr<ana::Spectrum> &&ospect)
      : stats_seed(ss), spect(std::move(ospect)) {}
  unsigned stats_seed;
  std::unique_ptr<ana::Spectrum> spect;
};

enum sample_idxs {
  kFDNueFHC = 0,
  kFDNumuFHC = 1,
  kFDNueRHC = 2,
  kFDNumuRHC = 3,
  kNDNumuFHC = 4,
  kNDNumuRHC = 5
};

// This is so so awful.
std::vector<seeded_spectra>
BuildSpectra(ana::PredictionInterp *predFDNumuFHC = nullptr,
             ana::PredictionInterp *predFDNueFHC = nullptr,
             ana::PredictionInterp *predFDNumuRHC = nullptr,
             ana::PredictionInterp *predFDNueRHC = nullptr,
             ana::PredictionInterp *predNDNumuFHC = nullptr,
             ana::PredictionInterp *predNDNumuRHC = nullptr,
             osc::IOscCalcAdjustable *fakeDataOsc = nullptr,
             ana::SystShifts fakeDataSyst = ana::kNoShift,
             bool fakeDataStats = false, double pot_fd_fhc_nue = 0,
             double pot_fd_fhc_numu = 0, double pot_fd_rhc_nue = 0,
             double pot_fd_rhc_numu = 0, double pot_nd_fhc = 0,
             double pot_nd_rhc = 0, std::vector<unsigned> const &Seeds = {});

double RunFitPoint(std::string stateFileName, std::string sampleString,
                   osc::IOscCalcAdjustable *fakeDataOsc,
                   ana::SystShifts fakeDataSyst, bool fakeDataStats,
                   std::vector<const ana::IFitVar *> oscVars,
                   std::vector<const ana::ISyst *> systlist,
                   osc::IOscCalcAdjustable *fitOsc,
                   ana::SystShifts fitSyst,
                   ana::SeedList oscSeeds = ana::SeedList(),
                   ana::IExperiment *penaltyTerm = nullptr,
                   ana::MinuitFitter::FitOpts fitStrategy = ana::MinuitFitter::kNormal,
                   TDirectory *outDir = nullptr,
                   FitTreeBlob *PostFitTreeBlob = nullptr,
                   std::vector<seeded_spectra> *spectra = nullptr,
                   ana::SystShifts &bf = ana::junkShifts);
