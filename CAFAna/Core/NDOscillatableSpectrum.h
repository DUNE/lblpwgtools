/////////////////////////////////////////////////////////////////////////////////
// NDOscillatableSpectrum.h
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, January 2021
//
// Object containing Spectrum with the ability to be oscillated (also contains
// true energy information).  Similar to OscillatableSpectrum but for ND.
// Adapted from NOvA sterile oscillation framework.
/////////////////////////////////////////////////////////////////////////////////

#pragma once

// framework
#include "CAFAna/Core/ReweightableSpectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/SpectrumLoaderBase.h"

// root
#include "TMD5.h"

// stl
#include <string>

class TH2;
class TH2D;

namespace ana {

  class Binning;

  /// Spectrum with true L/E information, allowing it to be oscillated
  class NDOscillatableSpectrum: public ReweightableSpectrum {
  public:

    friend class SpectrumLoaderBase;
    friend class SpectrumLoader;
    friend class NullLoader;

    NDOscillatableSpectrum(const std::string& label,
			   const Binning& bins,
			   SpectrumLoaderBase& loader,
			   const Var& var,
			   const Cut& cut,
			   const SystShifts& shift = kNoShift,
			   const Var& wei = kUnweighted);

    NDOscillatableSpectrum(SpectrumLoaderBase& loader,
			   const HistAxis& axis,
			   const Cut& cut,
			   const SystShifts& shift = kNoShift,
			   const Var& wei = kUnweighted);

    /// The only valid thing to do with such a spectrum is to assign something
    /// else into it.
    static NDOscillatableSpectrum Uninitialized() {return NDOscillatableSpectrum();}

    ~NDOscillatableSpectrum();

    /// Copy constructor
    NDOscillatableSpectrum(const NDOscillatableSpectrum& rhs);
    NDOscillatableSpectrum(NDOscillatableSpectrum&& rhs);

    /// Assignment operator
    NDOscillatableSpectrum& operator=(const NDOscillatableSpectrum& rhs);
    NDOscillatableSpectrum& operator=(NDOscillatableSpectrum&& rhs);

    Spectrum Oscillated(osc::IOscCalc* calc, int from, int to) const;
    void Scale(double x) {fMat *= x;}
    void SaveTo(TDirectory* dir, const std::string& name) const;
    static std::unique_ptr<NDOscillatableSpectrum> LoadFrom(TDirectory* dir, const std::string& name);

  protected:

    /// Constructor for Uninitialized()
    NDOscillatableSpectrum() { }

  };

}
