#pragma once

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/SpectrumLoaderBase.h"
#include "CAFAna/Core/Spectrum.h"

class TDirectory;

namespace ana
{

  /// Standard interface to all decomposition techniques
  class IDecomp
  {
    public:
      virtual ~IDecomp() = default;

    virtual Spectrum NCTotalComponent()   const  { return NCComponent()+NCAntiComponent(); }
    virtual Spectrum NCComponent()        const = 0;
    virtual Spectrum NCAntiComponent()    const = 0;
    virtual Spectrum NumuComponent()     const = 0;
    virtual Spectrum AntiNumuComponent() const = 0;
    virtual Spectrum NueComponent()      const = 0;
    virtual Spectrum AntiNueComponent()  const = 0;
    virtual Spectrum Data_Component()     const;

    virtual Spectrum MC_NCTotalComponent()  const;
    virtual Spectrum MC_NCComponent()       const;
    virtual Spectrum MC_NCAntiComponent()   const;
    virtual Spectrum MC_NumuComponent()     const;
    virtual Spectrum MC_AntiNumuComponent() const;
    virtual Spectrum MC_NueComponent()      const;
    virtual Spectrum MC_AntiNueComponent()  const;

    virtual void SaveTo(TDirectory* dir, const std::string& name) const = 0;
  };
}
