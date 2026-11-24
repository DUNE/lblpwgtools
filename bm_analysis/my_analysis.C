#include <iostream>

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/Navigate.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TRatioPlot.h"
#include "TStyle.h"

#include <iomanip>
#include <cmath>
#include <fstream>

#include "utils.h"
#include "selections.h"


using namespace ana;
using namespace utils;

const std::string kOutNom = "workspace/bm_results_duneanasel/";                            // nominal plots
const std::string kOutVar = "workspace/bm_results_duneanasel/horn_current_neg/plots/";     // ratio varied/nominal
const std::string kOutTests = "workspace/bm_results_duneanasel/horn_current_neg/tests/";   // statistics tests
const std::string kVarLabel = "Horn current neg";                                          // label for TLegend

const bool kPrintIntegrals = false; // print integrals raw/rescaled + ratio
const bool kRunStatTests   = false; // writes KS/AD tests onto a file

// reco vars
const Var kRecoNuEnergy = SIMPLEVAR(Enu.calo);
const Var kRecoXVertex = SIMPLEVAR(vtx.x);
const Var kRecoYVertex = SIMPLEVAR(vtx.y);
const Var kRecoZVertex = SIMPLEVAR(vtx.z);

const Var kEnergyRes([](const caf::SRInteractionProxy* recoint)
                  {
                    const caf::SRProxy *top = recoint->Ancestor<caf::SRProxy>();
                    const caf::SRTrueInteractionProxy* matched_int = caf::FindInteraction(top->mc, recoint->truth[0]);
                    if (matched_int == nullptr) return -999.;
                    // std::cout << "Reco Enu: " << recoint->Enu.calo << ", True Enu: " << matched_int->E << std::endl;
                    // const auto en_res = (recoint->Enu.calo - static_cast<double>(matched_int->E)) / static_cast<double>(matched_int->E);
                    // std::cout << "Energy resolution: " << en_res << std::endl;
                    return (recoint->Enu.calo - static_cast<double>(matched_int->E)) / static_cast<double>(matched_int->E);
                  });

const RecoPartVar kRecoParticleEnergy = SIMPLEPARTVAR(E);
const RecoPartVar kRecoParticleMomentum([](const caf::SRRecoParticleProxy* recopart)
                  {
                    double p = 0.;
                    p = std::sqrt( (recopart->p.x * recopart->p.x) + (recopart->p.y * recopart->p.y) + (recopart->p.z * recopart->p.z));
                    return p;
                  });

// cuts
const Cut kNuMuCC([](const caf::SRInteractionProxy* recoint) { return HasSandParticles(recoint) && NPrimaryMuons(recoint) == 1;} );
const Cut kNuMuCC_SAND_FV([](const caf::SRInteractionProxy* recoint) { return HasSandParticles(recoint) && NPrimaryMuons(recoint) == 1 && inSANDFV(recoint);} );
const RecoPartCut kIsRecoMuon([](const caf::SRRecoParticleProxy* recopart) { return recopart->primary == 1 && abs(recopart->pdg) == 13; });

void my_analysis()
{

  const std::string fname_n = "/pnfs/dune/persistent/users/gsantoni/beam_monitoring/data/fast_reco/nominal/CAF.flat/*.root";
  const std::string fname_v = "/pnfs/dune/persistent/users/gsantoni/beam_monitoring/data/fast_reco/varied/horn_current_neg/CAF.flat/*.root";

  SpectrumLoader loader_n(fname_n);

  SpectrumLoader loader_v(fname_v);

  // true histo
  // const TruthHistAxis axTrueNuEnergy("True Nu Energy (GeV)", Binning::Simple(50,0,20), kTrueNuEnergy);
  // const TruthPartHistAxis axTrueMuonMomentum("True Muon Momentum (GeV)", Binning::Simple(50,0,20), kTrueMuMomentum);

  // reco histo
  const HistAxis axRecoNuEnergy("Reco Nu Energy (GeV)", Binning::Simple(200,0,20), kRecoNuEnergy);
  const HistAxis axRecoVtxPositionYZ( "z(m)", Binning::Simple(100, 2000, 2900), kRecoZVertex, 
                                      "y(m)", Binning::Simple(100,-600,100), kRecoYVertex);
  const HistAxis axRecoVtxPositionXY( "x(m)", Binning::Simple(100, -500, 500), kRecoXVertex, 
                                      "y(m)", Binning::Simple(100,-600,100), kRecoYVertex);
  const HistAxis axRecoVtxPositionZX( "z(m)", Binning::Simple(100, 2000, 2800), kRecoZVertex, 
                                      "x(m)", Binning::Simple(100,-500,500), kRecoXVertex);
  const HistAxis axEnergyRes("Energy Resolution", Binning::Simple(50,-1,1), kEnergyRes);
  const RecoPartHistAxis axRecoMuonEnergy("Reco Muons Energy (GeV)", Binning::Simple(50,0,20), kRecoParticleEnergy);
  const RecoPartHistAxis axRecoMuonsMomentum("Reco Muons Momentum (GeV)", Binning::Simple(200,0,20), kRecoParticleMomentum);
  const HistAxis axFine("Reco Nu Energy (GeV)", Binning::Simple(1000,0,20), kRecoNuEnergy);

  // nominal
  Spectrum sRecoNuEnergy(loader_n.Interactions(RecoType::kSandreco), axRecoNuEnergy);
  Spectrum sRecoNuEnergyNuMuCC(loader_n.Interactions(RecoType::kSandreco)[kNuMuCC], axRecoNuEnergy);
  // Spectrum sRecoEnergyNuMuCCFine(loader_n.Interactions(RecoType::kSandreco)[kMatchedNuMuCC], axFine);
  // Spectrum sTruthNuEnergy(loader_n.NuTruths(), axTrueNuEnergy);
  // Spectrum sTruthNuEnergyCC(loader_n.NuTruths()[kIsNuMuCC], axTrueNuEnergy);
  // Spectrum sTrueMuonTrueMomentum(loader_n.NuTruths().TruthParticles(TruePType::kPrim)[kIsTrueMuon], axTrueMuonMomentum);
  Spectrum sRecoVtxPositionYZ(loader_n.Interactions(RecoType::kSandreco), axRecoVtxPositionYZ);
  Spectrum sRecoVtxPositionYZNuMuCC(loader_n.Interactions(RecoType::kSandreco)[kNuMuCC], axRecoVtxPositionYZ);
  Spectrum sRecoVtxPositionXY(loader_n.Interactions(RecoType::kSandreco), axRecoVtxPositionXY);
  Spectrum sRecoVtxPositionZX(loader_n.Interactions(RecoType::kSandreco), axRecoVtxPositionZX);
  Spectrum sEnergyRes(loader_n.Interactions(RecoType::kSandreco), axEnergyRes);
  // Spectrum sRecoMuonEnergy(loader_n.Interactions(RecoType::kSandreco)[kMatchedNuMuCC].RecoParticles(RecoType::kSandreco)[kIsRecoMuon], axRecoMuonEnergy);
  // Spectrum sRecoMuonRecoMomentum(loader_n.Interactions(RecoType::kSandreco)[kMatchedNuMuCC].RecoParticles(RecoType::kSandreco)[kIsRecoMuon], axRecoMuonsMomentum);
  
  //varied
  Spectrum sRecoNuEnergyVaried(loader_v.Interactions(RecoType::kSandreco), axRecoNuEnergy);
  Spectrum sRecoNuEnergyNuMuCCVaried(loader_v.Interactions(RecoType::kSandreco)[kNuMuCC], axRecoNuEnergy);
  // Spectrum sRecoEnergyNuMuCCVariedFine(loader_v.Interactions(RecoType::kSandreco)[kMatchedNuMuCC], axFine);
  // Spectrum sTruthNuEnergyVaried(loader_v.NuTruths(), axTrueNuEnergy);
  // Spectrum sTruthNuEnergyVariedCC(loader_v.NuTruths()[kIsNuMuCC], axTrueNuEnergy);
  // Spectrum sTrueMuonTrueMomentumVaried(loader_v.NuTruths().TruthParticles(TruePType::kPrim)[kIsTrueMuon], axTrueMuonMomentum);
  Spectrum sRecoVtxPositionYZVaried(loader_v.Interactions(RecoType::kSandreco), axRecoVtxPositionYZ);
  Spectrum sRecoVtxPositionXYVaried(loader_v.Interactions(RecoType::kSandreco), axRecoVtxPositionXY);
  Spectrum sRecoVtxPositionZXVaried(loader_v.Interactions(RecoType::kSandreco), axRecoVtxPositionZX);
  // Spectrum sRecoMuonEnergyVaried(loader_v.Interactions(RecoType::kSandreco)[kMatchedNuMuCC].RecoParticles(RecoType::kSandreco)[kIsRecoMuon], axRecoMuonEnergy);
  // Spectrum sRecoMuonRecoMomentumVaried(loader_v.Interactions(RecoType::kSandreco)[kMatchedNuMuCC].RecoParticles(RecoType::kSandreco)[kIsRecoMuon], axRecoMuonsMomentum);


  loader_n.Go();
  loader_v.Go();


  const double pot_n = sRecoNuEnergy.POT();
  const double pot_v = sRecoNuEnergyVaried.POT();

  RatioOpts optDefault;
  optDefault.varLabel = kVarLabel;


  // ************ NU RECO ENERGY
  const auto r_reco = DrawRatio(sRecoNuEnergy, sRecoNuEnergyVaried, pot_n,
                                kOutVar + "reco_nu_energy.png", optDefault);
  if (kPrintIntegrals) PrintIntegrals(r_reco, "Reco nu energy NO FILTERED", pot_n);

  // ************ RECO ENERGY TOT vs SELECTED NUMU CC
  DrawOverlay(sRecoNuEnergy, sRecoNuEnergyNuMuCC,
              "Nominal", "Selected SAND Inner Volume",
              pot_n, kOutNom + "reco_energy_sel_SANDinnerVol.png");

  // ************ NUMU CC RECO ENERGY
  RatioOpts optCC = optDefault;
  if (kRunStatTests) optCC.statFile = kOutTests + "test-statistics-results-reco-energy-numuCC.txt";
  const auto r_cc = DrawRatio(sRecoNuEnergyNuMuCC, sRecoNuEnergyNuMuCCVaried, pot_n,
                              kOutVar + "reco_energy_numu_cc.png", optCC);
  if (kPrintIntegrals) PrintIntegrals(r_cc, "Reco nu energy numuCC", pot_n);

  // ************ ENERGY RESOLUTION
  Draw1D(sEnergyRes, pot_n, kOutNom + "energy_res.png", kBlue);

  // ************ VERTICI RECO (nominale, numuCC)
  Draw2D(sRecoVtxPositionYZNuMuCC, pot_n, kOutNom + "reco_nu_vtx_posYZ_nominal_SANDinnerVol.png");

  // =========================================================================
  // Blocchi che erano commentati: ora sono UNA riga ciascuno.
  // Decommenta (insieme alle righe "Spectrum ..." sopra) quando servono.
  // =========================================================================

  // // ---- NU TRUE ENERGY / NUMU CC   (prima ~60 righe ciascuno)
  // const auto r_true   = DrawRatio(sTruthNuEnergy,   sTruthNuEnergyVaried,   pot_n, kOutVar + "truth_nu_energy.png", optDefault);
  // const auto r_trueCC = DrawRatio(sTruthNuEnergyCC, sTruthNuEnergyVariedCC, pot_n, kOutVar + "truth_nu_energy_numuCC.png", optDefault);
  // if (kPrintIntegrals) { PrintIntegrals(r_true, "True nu energy", pot_n); PrintIntegrals(r_trueCC, "True nu energy numuCC", pot_n); }

  // // ---- MUON RECO ENERGY
  // // [FIX] nell'originale la ratio era nominal/varied (h2_n, h2_v), senza "E" e con range 0-2,
  // // mentre tutte le altre sono varied/nominal. Ora e' coerente; se vuoi il range 0-2:
  // RatioOpts optMuE = optDefault; optMuE.rmin = 0; optMuE.rmax = 2;
  // DrawRatio(sRecoMuonEnergy, sRecoMuonEnergyVaried, pot_n, kOutVar + "reco_muons_energy_numuCC.png", optMuE);
  // // [FIX] nell'originale "auto leg" era dichiarato due volte nello stesso scope (errore di
  // // compilazione se decommentavi tutto): ora la legenda vive dentro DrawRatio.

  // // ---- MUON RECO MOMENTUM (+ test statistici)
  // RatioOpts optMuP = optDefault;
  // if (kRunStatTests) optMuP.statFile = kOutTests + "test-statistics-results-reco-mu-momentum-numuCC.txt";
  // DrawRatio(sRecoMuonRecoMomentum, sRecoMuonRecoMomentumVaried, pot_n, kOutVar + "reco_muons_momentum.png", optMuP);

  // // ---- KS/AD al variare del binning
  // KSRebinScan(sRecoEnergyNuMuCCFine, sRecoEnergyNuMuCCVariedFine, pot_n);

  // // ---- VERTICI: ratio 2D e altre proiezioni
  // // [FIX] nell'originale h5_n/h6_n usavano "pot" (non definito): ora pot_n.
  // DrawRatio2D(sRecoVtxPositionYZ, sRecoVtxPositionYZVaried, pot_n, kOutVar + "reco_nu_vtx_posYZ_ratio.png");
  // Draw2D(sRecoVtxPositionXY,       pot_n, kOutNom + "reco_nu_vtx_posXY_nominal.png");
  // Draw2D(sRecoVtxPositionXYVaried, pot_n, kOutNom + "reco_nu_vtx_posXY_variated.png");
  // Draw2D(sRecoVtxPositionZX,       pot_n, kOutNom + "reco_nu_vtx_posZX_nominal.png");
  // Draw2D(sRecoVtxPositionZXVaried, pot_n, kOutNom + "reco_nu_vtx_posZX_variated.png");

}
