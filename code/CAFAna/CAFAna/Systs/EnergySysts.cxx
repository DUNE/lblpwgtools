#include "CAFAna/Systs/EnergySysts.h"

namespace ana {
  const eScaleMuLArSyst keScaleMuLArSyst;
  const EnergyScaleMuSystND kEnergyScaleMuSystND;
  const EnergyScaleESyst kEnergyScaleESyst;
  const EnergyScaleNSyst kEnergyScaleNSyst;
  const EnergyScalePSyst kEnergyScalePSyst;
  const EnergyScalePipSyst kEnergyScalePipSyst;
  const EnergyScalePimSyst kEnergyScalePimSyst;
  const EnergyScalePi0Syst kEnergyScalePi0Syst;

  void HadronCorr::Shift(double sigma,
			Restorer& restore,
			caf::StandardRecord* sr, double& weight) const
  {
    restore.Add(sr->dune.Ev_reco);
    restore.Add(sr->dune.Ev_reco_nue);
    restore.Add(sr->dune.Ev_reco_numu);
    restore.Add(sr->dune.RecoHadEnNumu);
    restore.Add(sr->dune.RecoHadEnNue);

    const double scaleN   = 1 + fNfrac * sigma;
    const double scaleP   = 1 + fPfrac * sigma;
    const double scalePim = 1 + fPimfrac * sigma;
    const double scalePip = 1 + fPipfrac * sigma;
    const double scalePi0 = 1 + fPi0frac * sigma;

    // Fraction of the energy contained in each particle specie
    // crude approximation for amount of visible neutron energy
    const double EfracN   = ((sr->dune.eN*0.25) / sr->dune.Ev);
    const double EfracP   = (sr->dune.eP / sr->dune.Ev);
    const double EfracPi0 = (sr->dune.ePi0 / sr->dune.Ev);
    const double EfracPim = (sr->dune.ePim / sr->dune.Ev);
    const double EfracPip = (sr->dune.ePip / sr->dune.Ev);
    // Fraction of the hadronic energy contained in each particle species
    const double EfracNY   = ((sr->dune.eN*0.25) / (sr->dune.Ev*sr->dune.Y));
    const double EfracPY   = (sr->dune.eP / (sr->dune.Ev*sr->dune.Y));
    const double EfracPi0Y = (sr->dune.ePi0 / (sr->dune.Ev*sr->dune.Y));
    const double EfracPimY = (sr->dune.ePim / (sr->dune.Ev*sr->dune.Y));
    const double EfracPipY = (sr->dune.ePip / (sr->dune.Ev*sr->dune.Y));

    // Shift the energies
    sr->dune.Ev_reco = sr->dune.Ev_reco*( scaleP*EfracP + scaleN*EfracN + scalePi0*EfracPi0 + scalePim*EfracPim + scalePip*EfracPip + (1 - (EfracP + EfracN + EfracPi0 + EfracPim + EfracPip)) );

    sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu*( scaleP*EfracP + scaleN*EfracN + scalePi0*EfracPi0 + scalePim*EfracPim + scalePip*EfracPip + (1 - (EfracP + EfracN + EfracPi0 + EfracPim + EfracPip)) );

    sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue*( scaleP*EfracP + scaleN*EfracN + scalePi0*EfracPi0 + scalePim*EfracPim + scalePip*EfracPip + (1 - (EfracP + EfracN + EfracPi0 + EfracPim + EfracPip)) );

    sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu*( scaleP*EfracPY + scaleN*EfracNY + scalePi0*EfracPi0Y + scalePim*EfracPimY + scalePip*EfracPipY + (1 - (EfracPY + EfracNY + EfracPi0Y + EfracPimY + EfracPipY)) );

    sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue*( scaleP*EfracPY + scaleN*EfracNY + scalePi0*EfracPi0Y + scalePim*EfracPimY + scalePip*EfracPipY + (1 - (EfracPY + EfracNY + EfracPi0Y + EfracPimY + EfracPipY)) );

  }

}
