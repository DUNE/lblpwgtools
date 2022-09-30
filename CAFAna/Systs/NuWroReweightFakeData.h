#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Systs/BDTReweighter.h"

#include <vector>

class NuWroReweightFakeDataGenerator : public ana::ISyst {
public:
  NuWroReweightFakeDataGenerator();
  virtual ~NuWroReweightFakeDataGenerator();

  void Shift(double sigma, caf::SRProxy *sr,
             double &weight) const override;

protected:
  NuWroReweightFakeDataGenerator(const NuWroReweightFakeDataGenerator&) = delete;

  std::vector<BDTReweighter*> bdt_reweighters;
};
