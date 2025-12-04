#pragma once

#include "CAFAna/Decomp/IDecomp.h"

#include "CAFAna/Core/Loaders.h"

namespace ana
{
  // Uses MC for NC and \f$ \nu_e \f$ CC components, assigns remainder of data to \f$ \nu_\mu \f$ CC
  class NumuDecomp: public IDecomp
  {
  public:
    NumuDecomp(IInteractionSource& nearDetMCsrc,
               IInteractionSource& nearDetDatasrc,
               const HistAxis& axis,
               const Cut& cut,
               const SystShifts& shiftMC = kNoShift,
               const SystShifts& shiftData = kNoShift,
               const Weight& wei = kUnweighted);

    NumuDecomp(Loaders& loaders,
               const HistAxis& axis,
               const Cut& cut,
               const ana::RecoType& ixnRecoType,
               const SystShifts& shiftMC = kNoShift,
               const SystShifts& shiftData = kNoShift,
               const Weight& wei = kUnweighted);

    Spectrum Data_Component () const override {return fData;}
    Spectrum NueComponent() const override {return fNue;}
    Spectrum AntiNueComponent() const override {return fAntiNue;}

    Spectrum NumuComponent() const override;
    Spectrum AntiNumuComponent() const override;
    Spectrum NCTotalComponent() const override {return fNCTot;}
    Spectrum NCComponent() const override {std::cout << "NumuDecomp::NCComponent() not implemented" <<std::endl; abort();}
    Spectrum NCAntiComponent() const override {std::cout << "NumuDecomp::NCAntiComponent() not implemented" << std::endl; abort();}

    Spectrum MC_NueComponent() const override      {return fNue;}
    Spectrum MC_AntiNueComponent() const override  {return fAntiNue;}
    Spectrum MC_NumuComponent() const override     {return fNumu;}
    Spectrum MC_AntiNumuComponent() const override {return fAntiNumu;}
    Spectrum MC_NCTotalComponent() const override  {return fNCTot;}

    void SaveTo(TDirectory* dir, const std::string& name) const override;

    static std::unique_ptr<NumuDecomp> LoadFrom(TDirectory* dir, const std::string& name);

  protected:
    NumuDecomp()
      : fData(Spectrum::Uninitialized()),
        fNue(Spectrum::Uninitialized()),
        fAntiNue(Spectrum::Uninitialized()),
        fNumu(Spectrum::Uninitialized()),
        fAntiNumu(Spectrum::Uninitialized()),
        fNCTot(Spectrum::Uninitialized()),
        fNotNumu(Spectrum::Uninitialized())
    {};

    Spectrum fData;
    Spectrum fNue;
    Spectrum fAntiNue;
    Spectrum fNumu;
    Spectrum fAntiNumu;
    Spectrum fNCTot;
    Spectrum fNotNumu;
  };
}
