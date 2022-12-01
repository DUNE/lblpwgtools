#pragma once

// framework
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

// root
#include "TDirectory.h"
#include "TH1.h"
#include "TObjString.h"

namespace ana {

  enum class SystChannel {
    kSig,
    kBkg,
    kUnknown
  };

  // --------------------------------------------------------------------------
  class NuISyst : public ISyst {

  public:

    NuISyst(std::string name, std::string label, std::string sample_name = "",
	    std::map<int, std::pair<TH1*, TH1*> > shifts = {});
    ~NuISyst();

    std::string SampleName() const { return fSampleName; };
    std::string BaseName() const;
    virtual void SaveTo(TDirectory* dir, std::string type) const;

  protected:

    virtual double WeightFor(SystChannel channel, double sigma, double calE) const;
    std::string fSampleName;
    std::map<int, std::pair<TH1*, TH1*> > fShifts;

  };

  // --------------------------------------------------------------------------
  class KeySyst : public ISyst {

  public:

    KeySyst(std::string name, std::string label);
    void SetOneSided(bool onesided=true) { fOneSided=onesided; };
    bool IsOneSided() const { return fOneSided; };

  private:

    bool fOneSided;

  };

  // --------------------------------------------------------------------------
  namespace covmx {
    class NormSyst : public ISyst {
    public:
      NormSyst(std::string shortName, std::string latexName, double scale)
        : ISyst("Norm"+shortName, "Norm. "+latexName), fScale(scale) {
      };
      virtual void Shift(double sigma, caf::SRProxy* sr, double& weight) const override;
    protected:
      double fScale;
    };
  }

} // namespace ana
