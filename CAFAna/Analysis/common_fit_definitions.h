#include "CAFAna/Analysis/AnalysisBinnings.h"
#include "CAFAna/Analysis/AnalysisDialGroups.h"

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/SystShifts.h"

#include "CAFAna/Experiment/IChiSqExperiment.h"

#include "CAFAna/Fit/Fit.h"
#include "CAFAna/Fit/SeedList.h"

#include "CAFAna/Prediction/PredictionInterp.h"

#include "OscLib/func/IOscCalculator.h"

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
// POT for 3.5 years
extern double const pot_fd;
extern double const pot_nd;

// This is pretty annoying, but the above is for 7 years staged, which is 336 kT
// MW yr
extern double const nom_exposure;

extern size_t NFluxParametersToAddToStatefile;

double GetBoundedGausThrow(double min, double max);

// I miss python...
std::vector<std::string> SplitString(std::string input, char delim = ' ');

// For ease of penalty terms...
ana::IChiSqExperiment *GetPenalty(int hie, int oct, std::string penalty,
                                  std::string asimov_set = "0",
                                  bool modConstraint = false);

std::vector<const ana::IFitVar *>
GetOscVars(std::string oscVarString = "alloscvars", int hie = 0, int oct = 0);

// Take a list of all the systs known about, and retain the named systs...
void KeepSysts(std::vector<const ana::ISyst *> &systlist,
               std::vector<std::string> const &systsToInclude);

void KeepSysts(std::vector<const ana::ISyst *> &systlist,
               std::vector<const ana::ISyst *> const &systsToInclude);

void RemoveSysts(std::vector<const ana::ISyst *> &systlist,
                 std::vector<std::string> const &namesToRemove);

std::vector<const ana::ISyst *>
GetListOfSysts(bool fluxsyst_Nov17 = true, bool xsecsyst = true,
               bool detsyst = true, bool useND = true, bool useFD = true,
               bool useNueOnE = false, bool useFakeDataDials = true,
               bool fluxsyst_CDR = true,
               int NFluxSysts = NFluxParametersToAddToStatefile,
               bool removeFDNonFitDials = false);

std::vector<const ana::ISyst *> GetListOfSysts(std::string systString,
                                               bool useND = true,
                                               bool useFD = true,
                                               bool useNueOnE = false);

std::vector<const ana::ISyst *> GetListOfSysts(char const *systCString,
                                               bool useND = true,
                                               bool useFD = true,
                                               bool useNueOnE = false);

/// Put a list of systematics in the 'standard' order
std::vector<const ana::ISyst *>
OrderListOfSysts(std::vector<const ana::ISyst *> const &systlist);

///
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

TMatrixD *MakeCovmat(ana::PredictionInterp const &prediction,
                     std::vector<ana::ISyst const *> const &systs,
                     osc::IOscCalculatorAdjustable *calc, size_t NToys = 1000,
                     TDirectory *outdir = nullptr);

void SaveTrueOAParams(TDirectory *outDir, osc::IOscCalculatorAdjustable *calc,
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
             osc::IOscCalculatorAdjustable *fakeDataOsc = nullptr,
             ana::SystShifts fakeDataSyst = ana::kNoShift,
             bool fakeDataStats = false, double pot_fd_fhc_nue = 0,
             double pot_fd_fhc_numu = 0, double pot_fd_rhc_nue = 0,
             double pot_fd_rhc_numu = 0, double pot_nd_fhc = 0,
             double pot_nd_rhc = 0, std::vector<unsigned> const &Seeds = {});

double RunFitPoint(
    std::string stateFileName, std::string sampleString,
    osc::IOscCalculatorAdjustable *fakeDataOsc, ana::SystShifts fakeDataSyst,
    bool fakeDataStats, std::vector<const ana::IFitVar *> oscVars,
    std::vector<const ana::ISyst *> systlist,
    osc::IOscCalculatorAdjustable *fitOsc, ana::SystShifts fitSyst,
    ana::SeedList oscSeeds = ana::SeedList(),
    ana::IChiSqExperiment *penaltyTerm = nullptr,
    ana::MinuitFitter::FitOpts fitStrategy = ana::MinuitFitter::kNormal,
    TDirectory *outDir = nullptr, FitTreeBlob *PostFitTreeBlob = nullptr,
    std::vector<seeded_spectra> *spectra = nullptr,
    ana::SystShifts &bf = ana::junkShifts);
