#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/HistAxis.h"
#include "mesonless_cuts.h"


namespace ana{


    //2D histaxis with interaction level variables
  const HistAxis vtxPosition( "x(cm)", Binning::Simple(60,-60,60), SIMPLEVAR(vtx.x),
                                "z(cm)", Binning::Simple(60,-60,60), SIMPLEVAR(vtx.z));

  //  Multiplicity of all particles reconstructed from selected vertices
    // tracks
    // showers
  const HistAxis trackMult("Number of particles", Binning::Simple(20,0,20), kNumberOfTracks);
  const HistAxis shwMult("Number of particles", Binning::Simple(20,0,20), kNumberOfShowers);
  const HistAxis trkshwMult("Number of particles", Binning::Simple(20,0,20), kNumberOfTrkShw);
  // const HistAxis primMuonMult("Number of particles", Binning::Simple(20,0,20), kPrimPart(13));
  // const HistAxis primProtonMult("Number of particles", Binning::Simple(20,0,20), kPrimPart(2212));
  // const HistAxis primPionMult("Number of particles", Binning::Simple(20,0,20), kPrimPart(211));
  // const HistAxis primPhotonMult("Number of particles", Binning::Simple(20,0,20), kPrimPart(22));
  // const HistAxis primElectronMult("Number of particles", Binning::Simple(20,0,20), kPrimPart(11));
  // Define axes for the spectra we'll make
  const RecoPartHistAxis axEnergy("Muon energy (GeV)", Binning::Simple(50, 0, 1), kRecoParticleEnergy);
  const RecoPartHistAxis axPDG("PDG of reco particle", Binning::Simple(14, 0, 14), kRecoParticlePDG);


  // - Track length of all reconstructed tracks binned in 1 cm bins.
  const RecoPartHistAxis trkLen("Track length (cm)", Binning::Simple(100,0,100),kPartLen);

  //  /// Mesonless analysis axes
  const RecoPartHistAxis axPartMomentum("Momentum (GeV)", Binning::Simple(50, 0.0, 1.0), kPartMomentum);
  const RecoPartHistAxis axPartAngle("angle(deg)",Binning::Simple(60, 0.0, 180),kPartAngle);

  const TruthPartHistAxis axTruthPartMomentum("Momentum (GeV)", Binning::Simple(500., 0.0, 10.), kTruthPartMomentum);
  const TruthPartHistAxis axTruthPartAngle("angle(deg)",Binning::Simple(60, 0.0, 180),kTruthPartAngle);
  const TruthHistAxis TruevtxPosition( "x(cm)", Binning::Simple(60,-60,60), SIMPLETRUTHVAR(vtx.x),
                                       "z(cm)", Binning::Simple(60,-60,60), SIMPLETRUTHVAR(vtx.z));


}
