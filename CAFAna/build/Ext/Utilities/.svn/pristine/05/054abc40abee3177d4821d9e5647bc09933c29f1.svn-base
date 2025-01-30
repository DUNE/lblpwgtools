#ifndef RANDHISTO_H
#define RANDHISTO_H

#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"

#include "TH1.h"
#include "TMath.h"

class RandHisto
{
 public:
  RandHisto(CLHEP::HepRandomEngine& engine);
  void GetRandom(TH1* histo, double& x, double& y, double& z);

 private:
  CLHEP::RandFlat fFlat;
};


#endif
