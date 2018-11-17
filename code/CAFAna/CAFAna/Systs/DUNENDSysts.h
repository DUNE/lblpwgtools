#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/StandardRecord.h"
#include "CAFAna/Core/Utilities.h"
#include "math.h"

namespace ana
{
  class DUNENDEvSyst: public ISyst
  {
  public:
    DUNENDEvSyst() : ISyst("NDEv", "Near Detector reco Ev") {}

    void Shift(double sigma,
                       Restorer& restore,
                       caf::StandardRecord* sr,
                       double& weight) const override
    {
      restore.Add( sr->dune.Ev_reco, sr->dune.Ev_reco_numu, sr->dune.Ev_reco_nue );
      double scale = pow( 1. + sr->dune.sigma_Ev_reco, sigma );
      sr->dune.Ev_reco *= scale;
      sr->dune.Ev_reco_numu *= scale;
      sr->dune.Ev_reco_nue *= scale;
    }
  };

  extern const DUNENDEvSyst kNDEvSyst;

  class DUNENDPIDSyst: public ISyst
  {
  public:
    DUNENDPIDSyst() : ISyst("NDPID", "Near Detector lepton PID") {}

    void Shift(double sigma,
                       Restorer& restore,
                       caf::StandardRecord* sr,
                       double& weight) const override
    {
      restore.Add( sr->dune.numu_pid, sr->dune.nue_pid );

      bool numu = ( sigma > sr->dune.sigma_numu_pid );
      bool nue = ( sigma > sr->dune.sigma_nue_pid );

      // theoretically it is possible for both to be true in the same event, though this is highly unlikely.
      // in these rare cases, use the one with the smaller sigma
      if( numu && nue ) { // very rare
        if( sr->dune.sigma_numu_pid < sr->dune.sigma_nue_pid ) nue = false;
        else numu = false;
      }

      if( numu ) {
        sr->dune.numu_pid = 1.;
        sr->dune.nue_pid = -1.;
      } else if( nue ) {
        sr->dune.numu_pid = -1.;
        sr->dune.nue_pid = 1.;
      } else { // NC event
        sr->dune.numu_pid = -1.;
        sr->dune.nue_pid = -1.;
      }

    }
  };

  extern const DUNENDPIDSyst kNDPIDSyst;
}
