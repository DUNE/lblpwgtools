#pragma once

#include "CAFAna/Core/ISyst.h"

#include <memory>
#include "CAFAna/Systs/BDTReweighter.h"


namespace ana
{
  /// Base class for hadronization systematics
  class Had8nSyst : public ISyst
  {
    public:
      Had8nSyst(const std::string & shortName, const std::string latexName)
        : ISyst(shortName, latexName)
      {};

      void Shift(double sigma,
                 ana::Restorer &restore,
                 caf::StandardRecord *sr,
                 double &weight) const override;

    protected:
      // normally I'd just make this a BDTReweighter member,
      // but I want this class to remain pure virtual
      virtual BDTReweighter * GetReweighter() const = 0;
  };

  // -------------------------------------

  class AGKYachFixSyst : public Had8nSyst
  {
    public:
      AGKYachFixSyst();

    private:
      mutable std::unique_ptr<BDTReweighter> fBDTReweighter;
      BDTReweighter * GetReweighter() const override { return fBDTReweighter.get(); }
  };

  extern const AGKYachFixSyst kAGKYachFixSyst;

  // -------------------------------------

  class KNOnchKuzminNaumovSyst : public Had8nSyst
  {
    public:
      KNOnchKuzminNaumovSyst();

    private:
      mutable std::unique_ptr<BDTReweighter> fBDTReweighter;
      BDTReweighter * GetReweighter() const override { return fBDTReweighter.get(); }

  };

  extern const KNOnchKuzminNaumovSyst kKNOnchKuzminNaumovSyst;
}