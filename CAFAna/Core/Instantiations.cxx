#include "CAFAna/Core/SpectrumLoaderBase.h"

#include "CAFAna/Core/SpectrumConstructors.txx"

namespace ana
{
  template Spectrum::Spectrum(SpectrumLoaderBase& loader,
                              const HistAxis& axis,
                              const Cut& cut,
                              const SystShifts& shift,
                              const Var& wei,
                              Spectrum::ESparse sparse);

  template Spectrum::Spectrum(const std::string& label,
                              const Binning& bins,
                              SpectrumLoaderBase& loader,       
                              const Var& var,
                              const Cut& cut,
                              const SystShifts& shift,
                              const Var& wei,
                              Spectrum::ESparse sparse);

  template Spectrum::Spectrum(SpectrumLoaderBase& loader,
                              const HistAxis& xAxis,
                              const HistAxis& yAxis,
                              const Cut& cut,
                              const SystShifts& shift,
                              const Var& wei,
                              ESparse sparse);

  template Spectrum::Spectrum(const std::string& xLabel,
                              const std::string& yLabel,
                              SpectrumLoaderBase& loader,
                              const Binning& binsx, const Var& varx,
                              const Binning& binsy, const Var& vary,
                              const Cut& cut,
                              const SystShifts& shift,
                              const Var& wei,
                              ESparse sparse);

  template Spectrum::Spectrum(SpectrumLoaderBase& loader,
                              const HistAxis& xAxis,
                              const HistAxis& yAxis,
                              const HistAxis& zAxis,
                              const Cut& cut,
                              const SystShifts& shift,
                              const Var& wei,
                              ESparse sparse);

  template Spectrum::Spectrum(const std::string& xLabel,
                              const std::string& yLabel,
                              const std::string& zLabel,
                              SpectrumLoaderBase& loader,
                              const Binning& binsx, const Var& varx,
                              const Binning& binsy, const Var& vary,
                              const Binning& binsz, const Var& varz,
                              const Cut& cut,
                              const SystShifts& shift,
                              const Var& wei,
                              ESparse sparse);

  template Spectrum::Spectrum(SpectrumLoaderBase& loader,
                              const _HistAxis<MultiVar>& axis,
                              const Cut& cut,
                              const SystShifts& shift,
                              const Var& wei);
}

#include "CAFAna/Core/ReweightableSpectrum.h"
#include "CAFAna/Core/ReweightableSpectrumConstructors.txx"

namespace ana
{  
  template ReweightableSpectrum::ReweightableSpectrum(SpectrumLoaderBase& loader,
                                                      const HistAxis& recoAxis,
                                                      const HistAxis& trueAxis,
                                                      const Cut& cut,
                                                      const SystShifts& shift,
                                                      const Var& wei);
}
