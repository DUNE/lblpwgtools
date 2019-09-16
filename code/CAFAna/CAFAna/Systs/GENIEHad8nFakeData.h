#pragma once

#include "CAFAna/Core/ISyst.h"

#include <memory>
#include "CAFAna/Systs/BDTReweighter.h"


namespace ana
{
  /// Base class for hadronization systematics
  class Had8nFakeDataGenerator : public ISyst
  {
    public:
      Had8nFakeDataGenerator(const std::string & shortName, const std::string latexName)
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

  class AGKYachFixFakeDataGenerator : public Had8nFakeDataGenerator
  {
    public:
      AGKYachFixFakeDataGenerator();

    private:
      mutable std::unique_ptr<BDTReweighter> fBDTReweighter;
      BDTReweighter * GetReweighter() const override { return fBDTReweighter.get(); }
  };

  // -------------------------------------

  class KNOnchKuzminNaumovFakeDataGenerator : public Had8nFakeDataGenerator
  {
    public:
      KNOnchKuzminNaumovFakeDataGenerator();

    private:
      mutable std::unique_ptr<BDTReweighter> fBDTReweighter;
      BDTReweighter * GetReweighter() const override { return fBDTReweighter.get(); }

  };
}