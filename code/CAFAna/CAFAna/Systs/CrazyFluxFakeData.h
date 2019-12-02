#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include "TH2.h"

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <cassert>
#include <cmath>
#include <iostream>

class CrazyFluxFakeDataGenerator : public ana::ISyst {
public:

  virtual ~CrazyFluxFakeDataGenerator(){};

  std::vector<std::unique_ptr<TH2>> nu_Histos;
  std::vector<std::unique_ptr<TH2>> nubar_Histos;

  double EpFrac;
  bool fDoWeight;

  TH2 const *GetWeightingHisto(int gmode, bool is_nu) const {
    if ((gmode < 1) || (gmode > 14)) {
      return nullptr;
    }

    return (is_nu ? nu_Histos : nubar_Histos)[gmode].get();
  }

  void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
             double &weight) const override {

    if (!sr->dune.isCC) {
    }

    if (sigma != 1) {
    }

    if (!fDoWeight) {
      return;
    }

    if (!sr->dune.isFHC) {
      //return;
    }

    if(sr->dune.wgt_CrazyFlux < 0.01 || sr->dune.wgt_CrazyFlux > 100 || sr->dune.nuPDG>0) weight *= 1;
    else weight *= sr->dune.wgt_CrazyFlux;
  }

public:

  CrazyFluxFakeDataGenerator(bool DoWeight = true, double epfrac = 0.2)
      : ana::ISyst(DoWeight ? "CrazyFluxFakeDataGenerator"
                            : "CrazyFluxEnergyGenerator",
                   DoWeight ? "CrazyFluxFakeDataGenerator"
                            : "CrazyFluxEnergyGenerator"),
        EpFrac(epfrac), fDoWeight(DoWeight) {
  }

  //std::vector<const ana::ISyst *> GetCrazyFluxFakeDataSyst() ;

};

std::vector<const ana::ISyst *> GetCrazyFluxFakeDataSyst() 
{
  static CrazyFluxFakeDataGenerator mpfd;
  return {&mpfd};
}

