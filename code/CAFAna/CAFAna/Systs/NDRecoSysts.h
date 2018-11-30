// NDRecoSysts.h
// Systematics to simulate reconstruction systematics in the ND
#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/StandardRecord.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"

namespace ana {

  // Take ND events which pass the CC selection cuts but are NC and reweight by 20%
  class RecoNCSyst: public ISyst
  {
  public:
  RecoNCSyst() : ISyst("RecoNCSyst", "ND Neutral Current Reconstruction Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      // Is ND
      if(!sr->dune.isFD) {
	// CC event selection but is NC
	if((sr->dune.reco_numu || sr->dune.reco_nue) && (sr->dune.muon_contained || sr->dune.muon_tracker) && (sr->dune.reco_q == -1 || sr->dune.reco_q == 1) && sr->dune.Ehad_veto<30 && !sr->dune.isCC) {
	  weight *= 1 + .2*sigma;
	}
      }
    }
  };
  extern const RecoNCSyst kRecoNCSyst;
}
