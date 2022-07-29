#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/Proxy/SRProxy.h"
#include "CAFAna/Core/Utilities.h"
#include "math.h"

namespace ana
{
  class DUNENDEvSyst: public ISyst
  {
  public:
    DUNENDEvSyst() : ISyst("NDEv", "Near Detector reco Ev") {}

    void Shift(double sigma,
               caf::SRProxy* sr,
               double& weight) const override
    {
      double scale = pow( 1. + sr->sigma_Ev_reco, sigma );
      sr->Ev_reco *= scale;
      sr->Ev_reco_numu *= scale;
      sr->Ev_reco_nue *= scale;
    }
  };

  extern const DUNENDEvSyst kNDEvSyst;

  class DUNENDPIDSyst: public ISyst
  {
  public:
    DUNENDPIDSyst() : ISyst("NDPID", "Near Detector lepton PID") {}

    void Shift(double sigma,
               caf::SRProxy* sr,
               double& weight) const override
    {
      bool numu = ( sigma > sr->sigma_numu_pid );
      bool nue = ( sigma > sr->sigma_nue_pid );

      // theoretically it is possible for both to be true in the same event, though this is highly unlikely.
      // in these rare cases, use the one with the smaller sigma
      if( numu && nue ) { // very rare
        if( sr->sigma_numu_pid < sr->sigma_nue_pid ) nue = false;
        else numu = false;
      }

      if( numu ) {
        sr->numu_pid = 1.;
        sr->nue_pid = -1.;
      } else if( nue ) {
        sr->numu_pid = -1.;
        sr->nue_pid = 1.;
      } else { // NC event
        sr->numu_pid = -1.;
        sr->nue_pid = -1.;
      }

    }
  };

  extern const DUNENDPIDSyst kNDPIDSyst;
}
