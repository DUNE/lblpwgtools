#pragma once

#include "CAFAna/Core/ReweightableSpectrum.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/SpectrumStan.h"
#include "CAFAna/Core/SpectrumLoaderBase.h"
#include "CAFAna/Core/StanTypedefs.h"

#include <string>

#include "TMD5.h"

class TH2;
class TH2D;

#include "CAFAna/Core/FwdDeclare.h"

namespace ana
{
  class Binning;

  template <typename T>
  struct OscCache
  {
    mutable std::unique_ptr<TMD5> hash;
    mutable T spect;

    OscCache()
      : spect(0, {}, {}, 0, 0)
    {}
  };
  /// %Spectrum with true energy information, allowing it to be oscillated
  class OscillatableSpectrum: public ReweightableSpectrum, protected OscCache<Spectrum>, protected OscCache<SpectrumStan>
  {
  public:
    friend class SpectrumLoaderBase;
    friend class SpectrumLoader;
    friend class NullLoader;

    OscillatableSpectrum(const std::string& label,
                         const Binning& bins,
                         SpectrumLoaderBase& loader,
                         const Var& var,
                         const Cut& cut,
                         const SystShifts& shift = kNoShift,
                         const Var& wei = kUnweighted);

    OscillatableSpectrum(SpectrumLoaderBase& loader,
                         const HistAxis& axis,
                         const Cut& cut,
                         const SystShifts& shift = kNoShift,
                         const Var& wei = kUnweighted);

    OscillatableSpectrum(const std::string& label, const Binning& bins);
    OscillatableSpectrum(const std::string& label, double pot, double livetime,
                         const Binning& bins);
    OscillatableSpectrum(TH2* h,
                         const std::vector<std::string>& labels,
                         const std::vector<Binning>& bins,
                         double pot, double livetime);

    OscillatableSpectrum(std::unique_ptr<TH2D> h,
                         const std::vector<std::string>& labels,
                         const std::vector<Binning>& bins,
                         double pot, double livetime);

    ~OscillatableSpectrum();

    /// Copy constructor
    OscillatableSpectrum(const OscillatableSpectrum& rhs);
    OscillatableSpectrum(OscillatableSpectrum&& rhs);
    /// Assignment operator
    OscillatableSpectrum& operator=(const OscillatableSpectrum& rhs);
    OscillatableSpectrum& operator=(OscillatableSpectrum&& rhs);

    // Expose these ones directly
    using ReweightableSpectrum::Fill;
    using ReweightableSpectrum::ToTH2;
    using ReweightableSpectrum::Clear;

    /// Rescale bins so that \ref TrueEnergy will return \a target
    using ReweightableSpectrum::ReweightToTrueSpectrum;
    /// Rescale bins so that \ref Unoscillated will return \a target
    using ReweightableSpectrum::ReweightToRecoSpectrum;

    // These under a different name
    Spectrum Unoscillated() const {return UnWeighted();}
    Spectrum TrueEnergy() const {return WeightingVariable();}

    Spectrum Oscillated(osc::IOscCalculator* calc, int from, int to) const;
    SpectrumStan Oscillated(osc::IOscCalculatorStan* calc, int from, int to) const;

    Eigen::MatrixXd getCoreMatrix() const;

    OscillatableSpectrum& operator+=(const OscillatableSpectrum& rhs);
    OscillatableSpectrum operator+(const OscillatableSpectrum& rhs) const;

    OscillatableSpectrum& operator-=(const OscillatableSpectrum& rhs);
    OscillatableSpectrum operator-(const OscillatableSpectrum& rhs) const;

    void SaveTo(TDirectory* dir) const;
    static std::unique_ptr<OscillatableSpectrum> LoadFrom(TDirectory* dir);

  protected:
    // Derived classes can be trusted take care of their own construction
    OscillatableSpectrum(const std::vector<std::string>& labels,
                         const std::vector<Binning>& bins,
                         const Var& rwVar)
      : ReweightableSpectrum(labels, bins, rwVar)
    {
    }

    OscillatableSpectrum(const std::string& label,
                         const Binning& bins,
                         const Var& rwVar)
      : ReweightableSpectrum(label, bins, rwVar)
    {
    }

  };
}
