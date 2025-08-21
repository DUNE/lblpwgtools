#pragma once

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Decomp/IDecomp.h"
#include "CAFAna/Core/Spectrum.h"
#include <memory>
#include <string>

class TDirectory;

namespace ana
{

  class Binning;
  class Loaders;
  class SpectrumLoaderBase;

  /// Splits Data proportionally according to MC
  class ProportionalDecomp: public IDecomp
  {

  public:
    ProportionalDecomp(IInteractionSource& ndmcSrc,
                       IInteractionSource& nddataSrc,
                       const HistAxis& axis,
                       const Cut& cut,
                       const SystShifts& shiftMC = kNoShift,
                       const SystShifts& shiftData = kNoShift,
                       const Weight& wei = kUnweighted);

    ProportionalDecomp(Loaders& loaders,
                       const HistAxis& axis,
                       const Cut& cut,
                       ana::RecoType recoIxnType,
                       const SystShifts& shiftMC = kNoShift,
                       const SystShifts& shiftData = kNoShift,
                       const Weight& wei = kUnweighted);

    Spectrum NumuComponent()     const override;
    Spectrum AntiNumuComponent() const override;
    Spectrum NCTotalComponent()  const override;
    Spectrum NCComponent()       const override;
    Spectrum NCAntiComponent()   const override;
    Spectrum NueComponent()      const override;
    Spectrum AntiNueComponent()  const override;

    Spectrum MC_NumuComponent() const override{return fNumu;}
    Spectrum MC_AntiNumuComponent() const override{return fAntiNumu;}
    Spectrum MC_NueComponent() const override{return fNue;}
    Spectrum MC_AntiNueComponent() const override{return fAntiNue;}
    Spectrum MC_NCTotalComponent() const override{return fNCTot;}
    Spectrum MC_NCComponent()       const override{return fNC;}
    Spectrum MC_NCAntiComponent()   const override{return fNCAnti;}

    Spectrum Data_Component()    const override{return fData;}


    void SaveTo(TDirectory* dir, const std::string& name) const override;
    static std::unique_ptr<ProportionalDecomp> LoadFrom(TDirectory* dir, const std::string& name);

  protected:
    ProportionalDecomp()
      : fData(Spectrum::Uninitialized()),
    //nc 
        fNCTot  (Spectrum::Uninitialized()),
        fNC  (Spectrum::Uninitialized()),
        fNCAnti  (Spectrum::Uninitialized()),
    //end nc
        fNue (Spectrum::Uninitialized()),
        fAntiNue(Spectrum::Uninitialized()),
        fNumu(Spectrum::Uninitialized()),
        fAntiNumu(Spectrum::Uninitialized()),
        fTotal(Spectrum::Uninitialized())
    {};

    Spectrum fData;
    //nc
    Spectrum fNCTot;
    Spectrum fNC;
    Spectrum fNCAnti;
    //end nc
    Spectrum fNue;
    Spectrum fAntiNue;
    Spectrum fNumu;
    Spectrum fAntiNumu;
    Spectrum fTotal;

  };

}
