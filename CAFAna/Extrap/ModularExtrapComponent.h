#pragma once

#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/OscillatableSpectrum.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Decomp/IDecomp.h"

class TDirectory;
class TH1;

namespace ana
{

  //  class IDecomp;

  /// Simple way to remember what to ask the decomposition for.
  enum class DecompResult {nue, numu, NCtot, NC, NCbar, nuebar, numubar};

  /// Base class for component extrapolation.
  class ModularExtrapComponent
  {
    public:

      ModularExtrapComponent() : fEvaluated(false), fCache(OscillatableSpectrum::Uninitialized()) {}
      virtual ~ModularExtrapComponent() {};
      virtual void SaveTo(TDirectory* dir, const std::string& name) const = 0;
      virtual void SavePlots(TDirectory* dir, double potFD) const=0;
      static void SetQuiet(bool quiet = true){ fQuiet = quiet; }

      /// Interface so that result of Eval() is called only once and cached.
      const OscillatableSpectrum& Return() const;

      virtual const IDecomp* GetDecomp() const = 0;

    protected:

      /// Core extrapolation math.
      virtual OscillatableSpectrum Eval() const = 0;

      /// Helper function to pick out single Spectrum from a decomposition.
      static Spectrum GetDecompResult(const IDecomp&, DecompResult);

      /// Helper function to turn a DecompResult into a string (for storage).
      static std::string DRToString(DecompResult);

      /// Helper function to turn a string into a DecompResult (for loading).
      static DecompResult StringToDR(std::string);

      /// Form Ratio, but be aware of zero division.
      /** Necessary because root thinks n/0==0, so we lose events when
          low stats cause empty bins. If zero division occurs, ratio is set to
          1 and warning is issued. If numerator is 0 and ratio will be
          multiplied by 0 later, the warning is suppressed. */
      static Ratio FormSmartRatio(
        const Spectrum& num,
        const Spectrum& denom,
        std::string component,
        std::string location,
        const Spectrum& mult
      );

      static void ComparisonPlot(
        Spectrum mc,
        Spectrum notMC,
        double pot,
        std::string notMCLabel,
        int notMCColor,
        std::string latex,
        std::string title,
        std::string saveAs,
        bool restrictRange = false
      );

    private:
      mutable bool fEvaluated;
      mutable OscillatableSpectrum fCache;
      static bool fQuiet;

      class DivByZeroCounter
      {
        public:
          DivByZeroCounter(bool quiet = false)
            : fQuiet(quiet)
          {}
          ~DivByZeroCounter();

          std::set<std::tuple<std::string, std::string, int>> fBins;

        private:
          bool fQuiet;

      };

  };

  /// "Extrapolates" component by returning FD Monte Carlo.
  class NoReweight: public ModularExtrapComponent
  {

    public:
      NoReweight(
        SpectrumLoader& loader,
        const HistAxis& axis,
        const Cut& fdcut,
        ana::RecoType recoIxnType,
        const SystShifts& shiftMC,
        const Weight& weight,
        const Cut& flavors,
        SpectrumLoader& extraloaderswap = kNullLoader, // for NC
        SpectrumLoader& extraloadertau  = kNullLoader  // for NC
      );
      OscillatableSpectrum Eval() const override;
      void SaveTo(TDirectory* dir, const std::string& name) const override;
      void SavePlots(TDirectory* dir, double potFD) const override;
      static std::unique_ptr<NoReweight> LoadFrom(TDirectory* dir, const std::string& name);

      const IDecomp* GetDecomp() const override {return 0;}
    private:
      NoReweight(const OscillatableSpectrum& recoFD) : fRecoFD(recoFD) {}

      OscillatableSpectrum fRecoFD;

  };

  /// Extrapolates component using truth-over-truth method.
  class TruthReweight: public ModularExtrapComponent
  {

    public:
      TruthReweight(
        SpectrumLoader& ndloader,
        const HistAxis& axisFD,
        const HistAxis& axisND,
        const Cut& fdcut,
        ana::RecoType recoFDIxnType,
        const SystShifts& shiftMC,
        const Weight& weight,
        std::string label,
        std::string latex,
        const Cut& ndcut,
        ana::RecoType recoNDIxnType,
        const IDecomp& decomposition,
        const DecompResult dr,
        const Cut& ndflavor,
        SpectrumLoader& fdloader,
        const Cut& fdflavors
      );
      ~TruthReweight();
      OscillatableSpectrum Eval() const override;
      void SaveTo(TDirectory* dir, const std::string& name) const override;
      void SavePlots(TDirectory* dir, double potFD) const override;
      static std::unique_ptr<TruthReweight> LoadFrom(TDirectory* dir, const std::string& name);

      const IDecomp* GetDecomp() const override {return &fDecomp;}
    private:
      TruthReweight(
        OscillatableSpectrum recoToTrueND,
        OscillatableSpectrum trueToRecoFD,
        const IDecomp& decomp,
        const DecompResult decompRes,
        std::string label,
        std::string latex
      ) : fRecoToTrueND(recoToTrueND), fTrueToRecoFD(trueToRecoFD),
          fDecomp(decomp), fDecompRes(decompRes),
          fLabel(label), fLatex(latex) {}

      OscillatableSpectrum fRecoToTrueND;
      OscillatableSpectrum fTrueToRecoFD;
      const IDecomp& fDecomp;
      bool fOwnDecomp;
      const DecompResult fDecompRes;
      std::string fLabel;
      std::string fLatex;

  };

  /// Extrapolates using reco-over-reco method.
  class RecoReweight: public ModularExtrapComponent
  {

    public:
      RecoReweight(
        SpectrumLoader& ndloader,
        const HistAxis& axis,
        const Cut& fdcut,
        const SystShifts& shiftMC,
        const Weight& weight,
        std::string label,
        std::string latex,
        const Cut& ndcut,
        const IDecomp& decomposition,
        const DecompResult dr,
        const Cut& ndflavor,
        ana::RecoType recoNDIxnType,
        SpectrumLoader& fdloader,
        const Cut& fdflavors,
        ana::RecoType recoFDIxnType,
        SpectrumLoader& extrafdloaderswap = kNullLoader, // for NC
        SpectrumLoader& extrafdloadertau  = kNullLoader  // for NC
      );
      ~RecoReweight();
      OscillatableSpectrum Eval() const override;
      void SaveTo(TDirectory* dir, const std::string& name) const override;
      void SavePlots(TDirectory* dir, double potFD) const override;
      static std::unique_ptr<RecoReweight> LoadFrom(TDirectory* dir, const std::string& name);

      /// Uses MC spectra and target MC stats to estimate optimal binning.
      TH1* OptimalBinning( double potMCFD, double potMCND ) const;
      /// Fits gaussians to FD and ND MC to estimate optimal binning
      TH1* OptimalBinningFit( double potMCFD, double potMCND ) const;
      static TH1* OptimalBinningHelper( TH1*, TH1* );

      const IDecomp* GetDecomp() const override {return fDecomp;}

      friend class ModularExtrap;
    private:
      RecoReweight(
        Spectrum recoND,
        OscillatableSpectrum trueToRecoFD,
        const IDecomp& decomp,
        const DecompResult decompRes,
        std::string label,
        std::string latex
      ) : fRecoND(recoND), fTrueToRecoFD(trueToRecoFD),
          fDecomp(&decomp), fOwnDecomp(false), fDecompRes(decompRes),
          fLabel(label), fLatex(latex) {}

      Spectrum fRecoND;
      OscillatableSpectrum fTrueToRecoFD;
      const IDecomp* fDecomp;
      bool fOwnDecomp;
      const DecompResult fDecompRes;
      std::string fLabel;
      std::string fLatex;

  };

}
