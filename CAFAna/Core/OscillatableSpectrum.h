#pragma once

#include "cafanacore/ReweightableSpectrum.h"
#include "cafanacore/FwdDeclare.h"
#include "cafanacore/Spectrum.h"
#include "cafanacore/ThreadLocal.h"

#include "CAFAna/Core/Binning.h"
//#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/OscCalcFwdDeclare.h"
//#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/SpectrumLoaderBase.h"
#include "CAFAna/Core/StanTypedefs.h"
//#include "CAFAna/Core/ThreadLocal.h"

#include "CAFAna/Core/IRecordSource.h"

#include <string>

#include "TMD5.h"

class TH2;
class TH2D;

//#include "CAFAna/Core/FwdDeclare.h"

namespace ana
{
  class Binning;

  struct OscCache
  {
    std::unique_ptr<TMD5> hash;
    Spectrum spect;

    OscCache()
      : spect(Spectrum::Uninitialized())
    {}
  };

  /// %Spectrum with true energy information, allowing it to be oscillated
  class OscillatableSpectrum: public ReweightableSpectrum
  {
  public:
    OscillatableSpectrum(ana::IInteractionSource& src, const HistAxis& axis);

    OscillatableSpectrum(const Eigen::MatrixXd&& mat,
                         const HistAxis& recoAxis,
                         double pot, double livetime);

    /// The only valid thing to do with such a spectrum is to assign something
    /// else into it.
    static OscillatableSpectrum Uninitialized(){return OscillatableSpectrum();}

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

    Spectrum Oscillated(osc::IOscCalc* calc, int from, int to) const;
    Spectrum Oscillated(osc::IOscCalcStan* calc, int from, int to) const;

    OscillatableSpectrum& operator+=(const OscillatableSpectrum& rhs);
    OscillatableSpectrum operator+(const OscillatableSpectrum& rhs) const;

    OscillatableSpectrum& operator-=(const OscillatableSpectrum& rhs);
    OscillatableSpectrum operator-(const OscillatableSpectrum& rhs) const;

    void SaveTo(TDirectory* dir, const std::string& name) const;
    static std::unique_ptr<OscillatableSpectrum> LoadFrom(TDirectory* dir, const std::string& name);

  protected:
    /// Constructor for Uninitialized()
    OscillatableSpectrum()
    {
    }

    template<class T> Spectrum _Oscillated(osc::_IOscCalc<T>* calc, int from, int to) const;

    mutable ThreadLocal<OscCache> fCache;
  };
}
