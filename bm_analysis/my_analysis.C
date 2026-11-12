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

using namespace ana;

caf::SRVector3D NDHallToSANDCoordinates(const caf::SRVector3DProxy& ndhall_coords){
  
  // Convert coordinates from ND Hall to SAND coordinates
  // z points along beam, y points up, x points to the left if you look at the beam direction
  caf::SRVector3D sand_coords;
  sand_coords.x = ndhall_coords.x;   // x remains the same
  sand_coords.y = ndhall_coords.y + 238.473;  // cm
  sand_coords.z = ndhall_coords.z - 2391.0;   // cm

  return sand_coords;
}

caf::SRVector3D SANDToNDHallCoordinates(const caf::SRVector3DProxy& sand_coords){
  
  // Convert coordinates from SAND to ND Hall coordinates
  // z points along beam, y points up, x points to the left if you look at the beam direction
  caf::SRVector3D ndhall_coords;
  ndhall_coords.x = sand_coords.x;   // x remains the same
  ndhall_coords.y = sand_coords.y - 238.473;  // cm
  ndhall_coords.z = sand_coords.z + 2391.0;   // cm
  
  return ndhall_coords;
}

namespace mysel {
  enum RecoSelection { kRejected, 
                       kNuMuCC, 
                       kNuMuCC_frontECAL, 
                       kNuMuCC_innerVol, 
                       kNuMuCC_SAND_FV,
                       kSAND_FV
                      };

  enum TrueSelection { kRejectedTrue, 
                       kNuMuCCTrue, 
                       kNuMuNCTrue 
                     };

  // true
  TrueSelection ApplyTrueSelection(const caf::SRTrueInteractionProxy* trueint)
  {
    if (abs(trueint->pdg) == 14 && trueint->iscc) {
      return kNuMuCCTrue;
    } else if (abs(trueint->pdg) == 14 && !trueint->iscc) {
      return kNuMuNCTrue;
    } else {
      return kRejectedTrue;
    }
  }

  // reco
  bool HasSandParticles(const caf::SRInteractionProxy* recoint)
  {
    return recoint->part.sandreco.size() > 0;
  }

  bool ExactlyOnePrimaryMuon(const caf::SRInteractionProxy* recoint)
  {
    int n_primary_muons = 0;
    for (const auto& recopart : recoint->part.sandreco) {
      if (recopart.primary == 1 && abs(recopart.pdg) == 13) {
        n_primary_muons++;
      }
    }
    // std::cout << "nr prim muons " << n_primary_muons << '\n';
    return n_primary_muons == 1;
  }

  // bool PrimaryMuons(const caf::SRRecoParticleProxy* recopart)
  // {
  //   return abs(recopart.pdg) == 13 && recopart.primary == 1;
  // }

  bool AtLeastOnePrimaryMuon(const caf::SRInteractionProxy* recoint)
  {
    for (const auto& recopart : recoint->part.sandreco) {
      if (recopart.primary == 1 && abs(recopart.pdg) == 13) {
        return true;
      }
    }
    return false;
  }

  bool inFrontMagnet(const caf::SRInteractionProxy* recoint){
    auto nu_vtx = NDHallToSANDCoordinates(recoint->vtx);

    // auto radius = sqrt(std::pow(nu_vtx.y, 2) + std::pow(nu_vtx.z, 2));

    if (std::abs(nu_vtx.x) < 169 && std::abs(nu_vtx.y) < 200) {
      return true;
    }

    return false;
  }

  // fiducial cut for the front ECAL:
  // considering the center of SAND, we get a region with 2.0 < r < 2.3 m
  // and z < 0.910 (which actually is z < 23 m in global NDHall coordinates)
  bool inFrontECAL(const caf::SRInteractionProxy* recoint){
    auto nu_vtx = NDHallToSANDCoordinates(recoint->vtx);
    auto radius = sqrt(std::pow(nu_vtx.y, 2) + std::pow(nu_vtx.z, 2));

    auto z_cut_glob = 2400.0; // cm
    auto z_cut_loc = z_cut_glob - 2391.0;

    if (radius < 230 && radius > 200 && nu_vtx.z < z_cut_loc && std::abs(nu_vtx.x) < 169) {
      // std::cout << "vtx in front ECAL: " << nu_vtx.x << ", " << nu_vtx.y << ", " << nu_vtx.z << std::endl;
      return true;
    }

    return false;
  }

  bool inSANDInnerVol(const caf::SRInteractionProxy* recoint){
    auto nu_vtx = NDHallToSANDCoordinates(recoint->vtx);
    auto radius = sqrt(std::pow(nu_vtx.y, 2) + std::pow(nu_vtx.z, 2));

    if (radius < 190 && std::abs(nu_vtx.x) < 159) {
      // std::cout << "vtx in SAND inner volume: " << nu_vtx.x << ", " << nu_vtx.y << ", " << nu_vtx.z << std::endl;
      return true;
    }

    return false;
  }

  RecoSelection ApplyRecoSelection(const caf::SRInteractionProxy* recoint)
  {

    // if (!HasSandParticles(recoint)) {
    //   return kRejected;
    // }

    // if (!ExactlyOnePrimaryMuon(recoint)) {
    //   return kRejected;
    // }

    // if (!inFrontECAL(recoint) && !inSANDInnerVol(recoint)) {
    //   return kRejected;
    // }

    // if (HasSandParticles(recoint) && ExactlyOnePrimaryMuon(recoint) && inFrontECAL(recoint)){
    //   return kNuMuCC_frontECAL;
    // }

    // if (HasSandParticles(recoint) && ExactlyOnePrimaryMuon(recoint) && inSANDInnerVol(recoint)){
    //   return kNuMuCC_innerVol;
    // }

    // if (HasSandParticles(recoint) && ExactlyOnePrimaryMuon(recoint) && (inSANDInnerVol(recoint) || inFrontECAL(recoint))) { // 
    //   // std::cout << "vtx in SAND FV: " << recoint->vtx.x << ", " << recoint->vtx.y << ", " << recoint->vtx.z << std::endl;
    //   return kNuMuCC_SAND_FV;
    // }

    if (HasSandParticles(recoint) && ExactlyOnePrimaryMuon(recoint) && inSANDInnerVol(recoint)) { // 
      // std::cout << "vtx in SAND FV: " << recoint->vtx.x << ", " << recoint->vtx.y << ", " << recoint->vtx.z << std::endl;
      return kNuMuCC_SAND_FV;
    }

    return kRejected;
      
  }

} // mysel


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

const Cut kNuMuCC([](const caf::SRInteractionProxy* recoint)
                  {
                    return mysel::ApplyRecoSelection(recoint) == mysel::kNuMuCC;
                  });

const Cut kNuMuCC_SAND_FV([](const caf::SRInteractionProxy* recoint)
                  {
                    return mysel::ApplyRecoSelection(recoint) == mysel::kNuMuCC_SAND_FV;
                  });

const RecoPartCut kIsRecoMuon([](const caf::SRRecoParticleProxy* recopart)
                  {
                    return recopart->primary == 1 && abs(recopart->pdg) == 13;
                  });

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
  Spectrum sRecoNuEnergyNuMuCC(loader_n.Interactions(RecoType::kSandreco)[kNuMuCC_SAND_FV], axRecoNuEnergy);
  // Spectrum sRecoEnergyNuMuCCFine(loader_n.Interactions(RecoType::kSandreco)[kMatchedNuMuCC], axFine);
  // Spectrum sTruthNuEnergy(loader_n.NuTruths(), axTrueNuEnergy);
  // Spectrum sTruthNuEnergyCC(loader_n.NuTruths()[kIsNuMuCC], axTrueNuEnergy);
  // Spectrum sTrueMuonTrueMomentum(loader_n.NuTruths().TruthParticles(TruePType::kPrim)[kIsTrueMuon], axTrueMuonMomentum);
  Spectrum sRecoVtxPositionYZ(loader_n.Interactions(RecoType::kSandreco), axRecoVtxPositionYZ);
  Spectrum sRecoVtxPositionYZNuMuCC(loader_n.Interactions(RecoType::kSandreco)[kNuMuCC_SAND_FV], axRecoVtxPositionYZ);
  Spectrum sRecoVtxPositionXY(loader_n.Interactions(RecoType::kSandreco), axRecoVtxPositionXY);
  Spectrum sRecoVtxPositionZX(loader_n.Interactions(RecoType::kSandreco), axRecoVtxPositionZX);
  Spectrum sEnergyRes(loader_n.Interactions(RecoType::kSandreco), axEnergyRes);
  // Spectrum sRecoMuonEnergy(loader_n.Interactions(RecoType::kSandreco)[kMatchedNuMuCC].RecoParticles(RecoType::kSandreco)[kIsRecoMuon], axRecoMuonEnergy);
  // Spectrum sRecoMuonRecoMomentum(loader_n.Interactions(RecoType::kSandreco)[kMatchedNuMuCC].RecoParticles(RecoType::kSandreco)[kIsRecoMuon], axRecoMuonsMomentum);
  
  //varied
  Spectrum sRecoNuEnergyVaried(loader_v.Interactions(RecoType::kSandreco), axRecoNuEnergy);
  Spectrum sRecoNuEnergyNuMuCCVaried(loader_v.Interactions(RecoType::kSandreco)[kNuMuCC_SAND_FV], axRecoNuEnergy);
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

  // // gStyle->SetOptStat(1111);

  // TH1D* fA = sRecoEnergyNuMuCCFine.ToTH1(pot_n, kPOT);
  // TH1D* fB = sRecoEnergyNuMuCCVariedFine.ToTH1(pot_n, kPOT);

  // for(int k : {1, 2, 5, 10, 20, 40}){            // 1000, 500, 200, 100, 50, 25 bin
  //   TH1D* a = (TH1D*)fA->Clone(); a->Rebin(k);
  //   TH1D* b = (TH1D*)fB->Clone(); b->Rebin(k);
  //   printf("nbin=%d  D=%.5f  p=%g\n  pAD=%g\n", 1000/k,
  //         a->KolmogorovTest(b, "M"), a->KolmogorovTest(b), a->AndersonDarlingTest(b));          // p-value
  //   delete a; delete b;
  // }

  // // ************ NU TRUE ENERGY
  // auto c1_true = std::make_unique<TCanvas>("c1_true", "c1_true");

  // // histograms rescaled to the same nominal POT (rescaling for nominal is 1, for varied is pot_n/pot_v)
  // // since I want to see the nr of events that would be expected in the varied case if the POT was the same as the nominal case
  // auto h1_true_n = sTruthNuEnergy.ToTH1(pot_n, kBlue);
  // auto h1_true_v = sTruthNuEnergyVaried.ToTH1(pot_n, kRed);
  // std::cout << "pot nominal true " << pot_n << std::endl;
  // std::cout << "pot varied true " << pot_v << std::endl;
  // std::cout << "ratio varied true " << pot_n / pot_v << std::endl;

  // // statistical errors: sqrt(sumw2), since it is after rescaling they are sqrt(N_raw)*rescaling_factor
  // // so for the nominal case they are sqrt(N_raw)*1 = sqrt(N_raw), for the varied case they are sqrt(N_raw)*pot_n/pot_v
  // if (h1_true_n->GetSumw2N() == 0) h1_true_n->Sumw2();
  // if (h1_true_v->GetSumw2N() == 0) h1_true_v->Sumw2();
  // h1_true_n->SetLineWidth(2);
  // h1_true_v->SetLineWidth(2);

  // // raw counts (exposure = own spectrum POT, rescaling == 1)
  // auto h1_true_n_raw = sTruthNuEnergy.ToTH1(pot_n);
  // auto h1_true_v_raw = sTruthNuEnergyVaried.ToTH1(pot_v);
  
  // // ratio plot
  // auto nu_true_en_ratio = new TRatioPlot(h1_true_v, h1_true_n);
  // nu_true_en_ratio->SetH1DrawOpt("hist E");
  // nu_true_en_ratio->SetH2DrawOpt("hist E");
  // nu_true_en_ratio->Draw();
  // c1_true->Update();
  // // Ratio axis range
  // nu_true_en_ratio->GetLowerRefGraph()->SetMinimum(0.5);
  // nu_true_en_ratio->GetLowerRefGraph()->SetMaximum(1.5);
  // nu_true_en_ratio->GetLowYaxis()->SetNdivisions(505);
  // // Legend on the upper pad
  // nu_true_en_ratio->GetUpperPad()->cd();
  // auto leg_true = new TLegend(0.65, 0.75, 0.88, 0.88);
  // leg_true->AddEntry((TObject*)0, Form("POT = %.3g", pot_n), "");
  // leg_true->AddEntry(h1_true_n, Form("Nominal (%.0f ev)", h1_true_n->Integral()), "le");
  // leg_true->AddEntry(h1_true_v, Form("Horn current neg (%.0f ev)", h1_true_v->Integral()), "le");
  // leg_true->SetTextFont(42);
  // leg_true->SetTextSize(0.03);      
  // leg_true->SetBorderSize(0);
  // leg_true->SetFillStyle(1);  
  // leg_true->Draw();
  // c1_true->SaveAs("workspace/bm_results/horn_current_neg/plots/truth_nu_energy.png");

  //   // ---- Integrals: raw and rescaled, with errors (include under/overflow)
  // const int nb_true = h1_true_n->GetNbinsX();
  // double err_n_raw_true = 0, err_v_raw_true = 0, err_n_sc_true = 0, err_v_sc_true = 0;
  // const double N_n_raw_true = h1_true_n_raw->IntegralAndError(0, nb_true + 1, err_n_raw_true);
  // const double N_v_raw_true = h1_true_v_raw->IntegralAndError(0, nb_true + 1, err_v_raw_true);
  // const double N_n_sc_true  = h1_true_n->IntegralAndError(0, nb_true + 1, err_n_sc_true);
  // const double N_v_sc_true  = h1_true_v->IntegralAndError(0, nb_true + 1, err_v_sc_true);
  
  // // ratio varied/nominal with propagation (independent samples)
  // const double R_true     = N_v_sc_true / N_n_sc_true;
  // const double err_R_true = R_true * std::sqrt(std::pow(err_v_sc_true / N_v_sc_true, 2) +
  //                                    std::pow(err_n_sc_true / N_n_sc_true, 2));
  
  // std::cout << std::fixed << std::setprecision(2)
  //           << "\n=== Reco nu energy: integral events (with under/overflow) ===\n"
  //           << "Raw counts:\n"
  //           << "  nominal: " << N_n_raw_true << " +- " << err_n_raw_true << "\n"
  //           << "  varied : " << N_v_raw_true << " +- " << err_v_raw_true << "\n"
  //           << "Rescaled to nominal POT (" << std::scientific << pot_n << std::fixed << "):\n"
  //           << "  nominal: " << N_n_sc_true << " +- " << err_n_sc_true << "\n"
  //           << "  varied : " << N_v_sc_true << " +- " << err_v_sc_true << "\n"
  //           << "Ratio varied/nominal (rescaled): " << std::setprecision(4)
  //           << R_true << " +- " << err_R_true << std::endl;
  // // *********************************************************************************

  //   // ************ NU TRUE ENERGY NUMU CC INTERACTIONS
  // auto c1_true_CC = std::make_unique<TCanvas>("c1_true_CC", "c1_true_CC");

  // // histograms rescaled to the same nominal POT (rescaling for nominal is 1, for varied is pot_n/pot_v)
  // // since I want to see the nr of events that would be expected in the varied case if the POT was the same as the nominal case
  // auto h1_trueCC_n = sTruthNuEnergyCC.ToTH1(pot_n, kBlue);
  // auto h1_trueCC_v = sTruthNuEnergyVariedCC.ToTH1(pot_n, kRed);
  // std::cout << "pot nominal true " << pot_n << std::endl;
  // std::cout << "pot varied true " << pot_v << std::endl;
  // std::cout << "ratio varied true " << pot_n / pot_v << std::endl;

  // // statistical errors: sqrt(sumw2), since it is after rescaling they are sqrt(N_raw)*rescaling_factor
  // // so for the nominal case they are sqrt(N_raw)*1 = sqrt(N_raw), for the varied case they are sqrt(N_raw)*pot_n/pot_v
  // if (h1_trueCC_n->GetSumw2N() == 0) h1_trueCC_n->Sumw2();
  // if (h1_trueCC_v->GetSumw2N() == 0) h1_trueCC_v->Sumw2();
  // h1_trueCC_n->SetLineWidth(2);
  // h1_trueCC_v->SetLineWidth(2);

  // // raw counts (exposure = own spectrum POT, rescaling == 1)
  // auto h1_trueCC_n_raw = sTruthNuEnergyCC.ToTH1(pot_n);
  // auto h1_trueCC_v_raw = sTruthNuEnergyVariedCC.ToTH1(pot_v);

  // // ratio plot
  // auto nu_trueCC_en_ratio = new TRatioPlot(h1_trueCC_v, h1_trueCC_n);
  // nu_trueCC_en_ratio->SetH1DrawOpt("hist E");
  // nu_trueCC_en_ratio->SetH2DrawOpt("hist E");
  // nu_trueCC_en_ratio->Draw();
  // c1_true_CC->Update();
  // // Ratio axis range
  // nu_trueCC_en_ratio->GetLowerRefGraph()->SetMinimum(0.5);
  // nu_trueCC_en_ratio->GetLowerRefGraph()->SetMaximum(1.5);
  // nu_trueCC_en_ratio->GetLowYaxis()->SetNdivisions(505);
  // // Legend on the upper pad
  // nu_trueCC_en_ratio->GetUpperPad()->cd();
  // auto leg_true_CC = new TLegend(0.65, 0.75, 0.88, 0.88);
  // leg_true_CC->AddEntry((TObject*)0, Form("POT = %.3g", pot_n), "");
  // leg_true_CC->AddEntry(h1_trueCC_n, Form("Nominal (%.0f ev)", h1_trueCC_n->Integral()), "le");
  // leg_true_CC->AddEntry(h1_trueCC_v, Form("Horn current neg (%.0f ev)", h1_trueCC_v->Integral()), "le");
  // leg_true_CC->SetTextFont(42);
  // leg_true_CC->SetTextSize(0.03);      
  // leg_true_CC->SetBorderSize(0);
  // leg_true_CC->SetFillStyle(1); 
  // leg_true_CC->Draw();
  // c1_true_CC->SaveAs("workspace/bm_results/horn_current_neg/plots/truth_nu_energy_numuCC.png");

  //   // ---- Integrals: raw and rescaled, with errors (include under/overflow)
  // const int nb_trueCC = h1_trueCC_n->GetNbinsX();
  // double err_n_raw_trueCC = 0, err_v_raw_trueCC = 0, err_n_sc_trueCC = 0, err_v_sc_trueCC = 0;
  // const double N_n_raw_trueCC = h1_trueCC_n_raw->IntegralAndError(0, nb_trueCC + 1, err_n_raw_trueCC);
  // const double N_v_raw_trueCC = h1_trueCC_v_raw->IntegralAndError(0, nb_trueCC + 1, err_v_raw_trueCC);
  // const double N_n_sc_trueCC  = h1_trueCC_n->IntegralAndError(0, nb_trueCC + 1, err_n_sc_trueCC);
  // const double N_v_sc_trueCC  = h1_trueCC_v->IntegralAndError(0, nb_trueCC + 1, err_v_sc_trueCC);
  
  // // ratio varied/nominal with propagation (independent samples)
  // const double R_trueCC     = N_v_sc_trueCC / N_n_sc_trueCC;
  // const double err_R_trueCC = R_trueCC * std::sqrt(std::pow(err_v_sc_trueCC / N_v_sc_trueCC, 2) +
  //                                    std::pow(err_n_sc_trueCC / N_n_sc_trueCC, 2));
  
  // std::cout << std::fixed << std::setprecision(2)
  //           << "\n=== Reco nu energy: integral events (with under/overflow) ===\n"
  //           << "Raw counts:\n"
  //           << "  nominal: " << N_n_raw_trueCC << " +- " << err_n_raw_trueCC << "\n"
  //           << "  varied : " << N_v_raw_trueCC << " +- " << err_v_raw_trueCC << "\n"
  //           << "Rescaled to nominal POT (" << std::scientific << pot_n << std::fixed << "):\n"
  //           << "  nominal: " << N_n_sc_trueCC << " +- " << err_n_sc_trueCC << "\n"
  //           << "  varied : " << N_v_sc_trueCC << " +- " << err_v_sc_trueCC << "\n"
  //           << "Ratio varied/nominal (rescaled): " << std::setprecision(4)
  //           << R_trueCC << " +- " << err_R_trueCC << std::endl;
  // // *********************************************************************************

  // ************ NU RECO ENERGY
  auto c1 = std::make_unique<TCanvas>("c1", "c1");

  // histograms rescaled to the same nominal POT (rescaling for nominal is 1, for varied is pot_n/pot_v)
  // since I want to see the nr of events that would be expected in the varied case if the POT was the same as the nominal case
  auto h1_n = sRecoNuEnergy.ToTH1(pot_n, kBlue);
  auto h1_v = sRecoNuEnergyVaried.ToTH1(pot_n, kRed);
  std::cout << "pot nominal " << pot_n << std::endl;
  std::cout << "pot varied " << pot_v << std::endl;
  std::cout << "ratio varied " << pot_n / pot_v << std::endl;

  // statistical errors: sqrt(sumw2), since it is after rescaling they are sqrt(N_raw)*rescaling_factor
  // so for the nominal case they are sqrt(N_raw)*1 = sqrt(N_raw), for the varied case they are sqrt(N_raw)*pot_n/pot_v
  if (h1_n->GetSumw2N() == 0) h1_n->Sumw2();
  if (h1_v->GetSumw2N() == 0) h1_v->Sumw2();
  h1_n->SetLineWidth(2);
  h1_v->SetLineWidth(2);

  // raw counts (exposure = own spectrum POT, rescaling == 1)
  auto h1_n_raw = sRecoNuEnergy.ToTH1(pot_n);
  auto h1_v_raw = sRecoNuEnergyVaried.ToTH1(pot_v);
  
  // ratio plot
  auto nu_en_ratio = new TRatioPlot(h1_v, h1_n);
  nu_en_ratio->SetH1DrawOpt("hist E");
  nu_en_ratio->SetH2DrawOpt("hist E");
  nu_en_ratio->Draw();
  c1->Update();
  // Ratio axis range
  nu_en_ratio->GetLowerRefGraph()->SetMinimum(0.5);
  nu_en_ratio->GetLowerRefGraph()->SetMaximum(1.5);
  nu_en_ratio->GetLowYaxis()->SetNdivisions(505);
  // Legend on the upper pad
  nu_en_ratio->GetUpperPad()->cd();
  auto leg = new TLegend(0.65, 0.75, 0.88, 0.88);
  leg->AddEntry((TObject*)0, Form("POT = %.3g", pot_n), "");
  leg->AddEntry(h1_n, Form("Nominal (%.0f ev)", h1_n->Integral()), "le");
  leg->AddEntry(h1_v, Form("Horn current neg (%.0f ev)", h1_v->Integral()), "le");
  leg->SetTextFont(42);
  leg->SetTextSize(0.03);      
  leg->SetBorderSize(0);
  leg->SetFillStyle(1); 
  leg->Draw();
  c1->SaveAs("workspace/bm_results/horn_current_neg/plots/reco_nu_energy.png");
  // *********************************************************************************

  // ************ RECO ENERGY TOT vs SELECTED NUMU CC
  auto c1_sel = std::make_unique<TCanvas>("c1_sel", "c1_sel");

  // histograms rescaled to the same nominal POT (rescaling for nominal is 1, for varied is pot_n/pot_v)
  // since I want to see the nr of events that would be expected in the varied case if the POT was the same as the nominal case
  auto h1_n_tot = sRecoNuEnergy.ToTH1(pot_n, kBlack);
  auto h1_n_sel = sRecoNuEnergyNuMuCC.ToTH1(pot_n, kOrange);

  // statistical errors: sqrt(sumw2), since it is after rescaling they are sqrt(N_raw)*rescaling_factor
  // so for the nominal case they are sqrt(N_raw)*1 = sqrt(N_raw), for the varied case they are sqrt(N_raw)*pot_n/pot_v
  if (h1_n_tot->GetSumw2N() == 0) h1_n_tot->Sumw2();
  if (h1_n_sel->GetSumw2N() == 0) h1_n_sel->Sumw2();
  h1_n_tot->SetLineWidth(2);
  h1_n_sel->SetLineWidth(2);

  // raw counts (exposure = own spectrum POT, rescaling == 1)
  auto h1_n_raw_tot = sRecoNuEnergy.ToTH1(pot_n);
  auto h1_n_raw_sel = sRecoNuEnergyNuMuCC.ToTH1(pot_v);
  
  h1_n_tot->Draw("hist");
  h1_n_sel->Draw("same hist");
  c1_sel->Update();

  // Legend on the upper pad
  auto leg_sel = new TLegend(0.65, 0.75, 0.88, 0.88);
  leg_sel->AddEntry((TObject*)0, Form("POT = %.3g", pot_n), "");
  leg_sel->AddEntry(h1_n_tot, Form("Nominal (%.0f ev)", h1_n_tot->Integral()), "le");
  leg_sel->AddEntry(h1_n_sel, Form("Selected SAND Inner Volume (%.0f ev)", h1_n_sel->Integral()), "le");
  leg_sel->SetTextFont(42);
  leg_sel->SetTextSize(0.03);      
  leg_sel->SetBorderSize(0);
  leg_sel->SetFillStyle(1); 
  leg_sel->Draw();
  c1_sel->SaveAs("workspace/bm_results/reco_energy_sel_SANDinnerVol.png");
  // *********************************************************************************

  // ************ NUMU CC RECO ENERGY
  auto c1_numu_cc = std::make_unique<TCanvas>("c1_numu_cc", "c1_numu_cc");

  // histograms rescaled to the same nominal POT (rescaling for nominal is 1, for varied is pot_n/pot_v)
  // since I want to see the nr of events that would be expected in the varied case if the POT was the same as the nominal case
  auto h1_n_numu_cc = sRecoNuEnergyNuMuCC.ToTH1(pot_n, kBlue);
  auto h1_v_numu_cc = sRecoNuEnergyNuMuCCVaried.ToTH1(pot_n, kRed);
  std::cout << "pot nominal " << pot_n << std::endl;
  std::cout << "pot varied " << pot_v << std::endl;
  std::cout << "ratio varied " << pot_n / pot_v << std::endl;

  // statistical errors: sqrt(sumw2), since it is after rescaling they are sqrt(N_raw)*rescaling_factor
  // so for the nominal case they are sqrt(N_raw)*1 = sqrt(N_raw), for the varied case they are sqrt(N_raw)*pot_n/pot_v
  if (h1_n_numu_cc->GetSumw2N() == 0) h1_n_numu_cc->Sumw2();
  if (h1_v_numu_cc->GetSumw2N() == 0) h1_v_numu_cc->Sumw2();
  h1_n_numu_cc->SetLineWidth(2);
  h1_v_numu_cc->SetLineWidth(2);

  // raw counts (exposure = own spectrum POT, rescaling == 1)
  auto h1_n_raw_numu_cc = sRecoNuEnergyNuMuCC.ToTH1(pot_n);
  auto h1_v_raw_numu_cc = sRecoNuEnergyNuMuCCVaried.ToTH1(pot_v);
  
  // ratio plot
  auto nu_en_ratio_numu_cc = new TRatioPlot(h1_v_numu_cc, h1_n_numu_cc);
  nu_en_ratio_numu_cc->SetH1DrawOpt("hist E");
  nu_en_ratio_numu_cc->SetH2DrawOpt("hist E");
  nu_en_ratio_numu_cc->Draw();
  c1_numu_cc->Update();
  // Ratio axis range
  nu_en_ratio_numu_cc->GetLowerRefGraph()->SetMinimum(0.5);
  nu_en_ratio_numu_cc->GetLowerRefGraph()->SetMaximum(1.5);
  nu_en_ratio_numu_cc->GetLowYaxis()->SetNdivisions(505);
  // Legend on the upper pad
  nu_en_ratio_numu_cc->GetUpperPad()->cd();
  auto leg_numu_cc = new TLegend(0.65, 0.75, 0.88, 0.88);
  leg_numu_cc->AddEntry((TObject*)0, Form("POT = %.3g", pot_n), "");
  leg_numu_cc->AddEntry(h1_n_numu_cc, Form("Nominal (%.0f ev)", h1_n_numu_cc->Integral()), "le");
  leg_numu_cc->AddEntry(h1_v_numu_cc, Form("Horn current neg (%.0f ev)", h1_v_numu_cc->Integral()), "le");
  leg_numu_cc->SetTextFont(42);
  leg_numu_cc->SetTextSize(0.03);      
  leg_numu_cc->SetBorderSize(0);
  leg_numu_cc->SetFillStyle(1); 
  leg_numu_cc->Draw();
  c1_numu_cc->SaveAs("workspace/bm_results/horn_current_neg/plots/reco_energy_numu_cc.png");
  // *********************************************************************************


  // ---- Integrals: raw and rescaled, with errors (include under/overflow)
  const int nb = h1_n->GetNbinsX();
  double err_n_raw = 0, err_v_raw = 0, err_n_sc = 0, err_v_sc = 0;
  const double N_n_raw = h1_n_raw->IntegralAndError(0, nb + 1, err_n_raw);
  const double N_v_raw = h1_v_raw->IntegralAndError(0, nb + 1, err_v_raw);
  const double N_n_sc  = h1_n->IntegralAndError(0, nb + 1, err_n_sc);
  const double N_v_sc  = h1_v->IntegralAndError(0, nb + 1, err_v_sc);
  
  // ratio varied/nominal with propagation (independent samples)
  const double R     = N_v_sc / N_n_sc;
  const double err_R = R * std::sqrt(std::pow(err_v_sc / N_v_sc, 2) +
                                     std::pow(err_n_sc / N_n_sc, 2));
  
  // std::cout << std::fixed << std::setprecision(2)
  //           << "\n=== Reco nu energy NO FILTERED: integral events (with under/overflow) ===\n"
  //           << "Raw counts:\n"
  //           << "  nominal: " << N_n_raw << " +- " << err_n_raw << "\n"
  //           << "  varied : " << N_v_raw << " +- " << err_v_raw << "\n"
  //           << "Rescaled to nominal POT (" << std::scientific << pot_n << std::fixed << "):\n"
  //           << "  nominal: " << N_n_sc << " +- " << err_n_sc << "\n"
  //           << "  varied : " << N_v_sc << " +- " << err_v_sc << "\n"
  //           << "Ratio varied/nominal (rescaled): " << std::setprecision(4)
  //           << R << " +- " << err_R << std::endl;

  // // ---- errors bin by bin rescaled to nominal POT (rescaling for nominal is 1, for varied is pot_n/pot_v)
  // std::cout << "\nbin  center[GeV]  N_n(sc)  err_n  N_v(sc)  err_v\n";
  // for (int i = 1; i <= nb; ++i)
  //   std::cout << std::setw(3) << i << "  "
  //             << std::setw(8) << h1_n->GetBinCenter(i) << "  "
  //             << std::setw(9) << h1_n->GetBinContent(i) << "  "
  //             << std::setw(7) << h1_n->GetBinError(i) << "  "
  //             << std::setw(9) << h1_v->GetBinContent(i) << "  "
  //             << std::setw(7) << h1_v->GetBinError(i) << "\n";
  // std::cout << std::endl;


  // ************ ENERGY RESOLUTION
  auto c1_res = std::make_unique<TCanvas>("c1_res", "c1_res");

  auto h1_n_res = sEnergyRes.ToTH1(pot_n, kBlue);
  h1_n_res->Draw("hist");
  c1_res->SaveAs("workspace/bm_results/energy_res.png");


  // // ************ MUON RECO ENERGY
  // auto c2 = std::make_unique<TCanvas>("c2", "c2");
  // auto h2_n = sRecoMuonEnergy.ToTH1(pot_n, kBlue);
  // auto h2_v = sRecoMuonEnergyVaried.ToTH1(pot_n, kRed);

  // auto muons_en_ratio = new TRatioPlot(h2_n, h2_v);
  // muons_en_ratio->SetH1DrawOpt("hist");
  // muons_en_ratio->SetH2DrawOpt("hist");
  // muons_en_ratio->Draw();
  // c2->Update();
  // // Ratio axis range
  // muons_en_ratio->GetLowerRefGraph()->SetMinimum(0);
  // muons_en_ratio->GetLowerRefGraph()->SetMaximum(2);
  // muons_en_ratio->GetLowYaxis()->SetNdivisions(505);
  // // Legend on the upper pad
  // muons_en_ratio->GetUpperPad()->cd();
  // auto leg = new TLegend(0.65, 0.75, 0.88, 0.88);
  // leg->AddEntry(h2_n, "Nominal", "l");
  // leg->AddEntry(h2_v, "Horn current neg", "l");
  // leg->Draw();
  // c2->SaveAs("workspace/bm_results/horn_current_neg/plots/reco_muons_energy_numuCC.png");

  // // ************************ MUON RECO MOMENTUM
  // auto c3 = std::make_unique<TCanvas>("c3", "c3");

  // // histograms rescaled to the same nominal POT (rescaling for nominal is 1, for varied is pot_n/pot_v)
  // // since I want to see the nr of events that would be expected in the varied case if the POT was the same as the nominal case
  // auto h3_n = sRecoMuonRecoMomentum.ToTH1(pot_n, kBlue);
  // auto h3_v = sRecoMuonRecoMomentumVaried.ToTH1(pot_n, kRed);
  // std::cout << "pot nominal reco muon mom " << pot_n << std::endl;
  // std::cout << "pot varied reco muon mom " << pot_v << std::endl;
  // std::cout << "ratio varied reco muon mom " << pot_n / pot_v << std::endl;

  // // statistical errors: sqrt(sumw2), since it is after rescaling they are sqrt(N_raw)*rescaling_factor
  // // so for the nominal case they are sqrt(N_raw)*1 = sqrt(N_raw), for the varied case they are sqrt(N_raw)*pot_n/pot_v
  // if (h3_n->GetSumw2N() == 0) h3_n->Sumw2();
  // if (h3_v->GetSumw2N() == 0) h3_v->Sumw2();
  // h3_n->SetLineWidth(2);
  // h3_v->SetLineWidth(2);

  // // raw counts (exposure = own spectrum POT, rescaling == 1)
  // auto h3_n_raw = sRecoMuonRecoMomentum.ToTH1(pot_n);
  // auto h3_v_raw = sRecoMuonRecoMomentumVaried.ToTH1(pot_v);

  // // ratio plot (varied / nominal)
  // auto muons_mom_ratio = new TRatioPlot(h3_v, h3_n);
  // muons_mom_ratio->SetH1DrawOpt("hist E");
  // muons_mom_ratio->SetH2DrawOpt("hist E");
  // muons_mom_ratio->Draw();
  // c3->Update();
  // // Ratio axis range
  // muons_mom_ratio->GetLowerRefGraph()->SetMinimum(0.5);
  // muons_mom_ratio->GetLowerRefGraph()->SetMaximum(1.5);
  // muons_mom_ratio->GetLowYaxis()->SetNdivisions(505);
  // // Legend on the upper pad
  // muons_mom_ratio->GetUpperPad()->cd();
  // auto leg3 = new TLegend(0.65, 0.75, 0.88, 0.88);
  // leg3->AddEntry((TObject*)0, Form("POT = %.3g", pot_n), "");
  // leg3->AddEntry(h3_n, Form("Nominal (%.0f ev)", h3_n->Integral()), "le");
  // leg3->AddEntry(h3_v, Form("Horn current neg (%.0f ev)", h3_v->Integral()), "le");
  // leg3->SetTextFont(42);
  // leg3->SetTextSize(0.03);
  // leg3->SetBorderSize(0);
  // leg3->SetFillStyle(1);
  // leg3->Draw();
  // c3->SaveAs("workspace/bm_results/horn_current_neg/plots/reco_muons_momentum.png");
  // // *********************************************************************************

  auto c4_n = std::make_unique<TCanvas>("c4n", "c4n");
  auto h4_n = sRecoVtxPositionYZNuMuCC.ToTH2(pot_n);
  h4_n->SetLineColor(kBlue);
  h4_n->Draw("COLZ");
  c4_n->SaveAs("workspace/bm_results/reco_nu_vtx_posYZ_nominal_SANDinnerVol.png");

  // auto c4_v = std::make_unique<TCanvas>("c4v", "c4v");
  // auto h4_v = sRecoVtxPositionYZVaried.ToTH2(pot_n);
  // h4_v->SetLineColor(kRed);
  // h4_v->Draw("COLZ");
  // c4_v->SaveAs("workspace/bm_results/reco_nu_vtx_posYZ_variated.png");

  // TH2D *ratio = (TH2D*)h4_n->Clone("ratio");
  // ratio->Divide(h4_v);   // opzionale: Divide(h1, h2, 1., 1., "B") per errori binomiali
  // ratio->SetTitle("h4_n / h4_v");
  // ratio->SetMinimum(0.5);   // scala z a piacere
  // ratio->SetMaximum(1.5);
  // auto c4_ratio = std::make_unique<TCanvas>("c4ratio", "c4ratio");
  // ratio->Draw("COLZ");
  // c4_ratio->SaveAs("reco_nu_vtx_posYZ_ratio.png");



  // auto c5_n = std::make_unique<TCanvas>("c5n", "c5n");

  // auto h5_n = sRecoVtxPositionXY.ToTH2(pot);
  // // h5_n->SetLineColor(kBlue);
  // h5_n->Draw("COLZ");
  // c5_n->SaveAs("reco_nu_vtx_posXY_nominal.png");

  // auto c5_v = std::make_unique<TCanvas>("c5v", "c5v");
  // auto h5_v = sRecoVtxPositionXYVaried.ToTH2(pot);
  // // h5_v->SetLineColor(kRed);
  // h5_v->Draw("COLZ");
  // c5_v->SaveAs("reco_nu_vtx_posXY_variated.png");


  // auto c6_n = std::make_unique<TCanvas>("c6n", "c6n");

  // auto h6_n = sRecoVtxPositionZX.ToTH2(pot);
  // // h6_n->SetLineColor(kBlue);
  // h6_n->Draw("COLZ");
  // c6_n->SaveAs("reco_nu_vtx_posZX_nominal.png");

  // auto c6_v = std::make_unique<TCanvas>("c6v", "c6v");
  // auto h6_v = sRecoVtxPositionZXVaried.ToTH2(pot);
  // // h6_v->SetLineColor(kRed);
  // h6_v->Draw("COLZ");
  // c6_v->SaveAs("reco_nu_vtx_posZX_variated.png");

  // std::ofstream outfile("workspace/bm_results/horn_current_neg/tests/test-statistics-results-reco-mu-momentum-numuCC.txt", std::ios::app);

  // // Kolmogorov-Smirnov
  // double pKS = h3_n->KolmogorovTest(h3_v);          // p-value
  // double dKS = h3_n->KolmogorovTest(h3_v, "M");     // distanza massima D
  // double pKSmc = h3_n->KolmogorovTest(h3_v, "X");   // p-value con pseudo-esperimenti (consigliato)
  // double pKSnorm = h3_n->KolmogorovTest(h3_v, "N");   // p-value normalized

  // // Anderson-Darling (2 campioni, binnato)
  // double pAD = h3_n->AndersonDarlingTest(h3_v);          // p-value
  // double tAD = h3_n->AndersonDarlingTest(h3_v, "T"); 
  
  // outfile << "=============== POT " << pot_n << " ===============" << std::endl;

  // outfile << "Nominal POT: "<< sRecoMuonRecoMomentum.POT() << '\n';
  // outfile << "Varied POT: " << sRecoMuonRecoMomentumVaried.POT() << '\n';
  
  // outfile << "Nominal POT used to rescale histograms: " << pot_n << '\n';
  // outfile << "Nominal POT / Varied POT: " << pot_n/sRecoMuonRecoMomentumVaried.POT() << std::endl;

  // outfile << "Nominal: int=" << h3_n->Integral() << " entries=" << h3_n->GetEntries() << " eff=" << h3_n->GetEffectiveEntries() << '\n';
  // outfile << "Varied: int=" << h3_v->Integral() << " entries=" << h3_v->GetEntries() << " eff=" << h3_v->GetEffectiveEntries() << '\n';

  // outfile << "Kolmogorov-Smirnov p-value: " << pKS << " Kolmogorov-Smirnov distance (D): " << dKS << " Kolmogorov-Smirnov pseudo-experiment: " << pKSmc << " Kolmogorov-Smirnov normalized: " << pKSnorm << std::endl;
  // outfile << "Anderson-Darling p-value: " << pAD << " Anderson-Darling test statistic (T): " << tAD << std::endl;
  // outfile << "=============================================" << std::endl;


  // outfile.close();

  // std::ofstream outfile2("workspace/bm_results/horn_current_neg/tests/test-statistics-results-reco-energy-numuCC.txt", std::ios::app);

  // // Kolmogorov-Smirnov
  // double pKS2 = h1_n_numu_cc->KolmogorovTest(h1_v_numu_cc);          // p-value
  // double dKS2 = h1_n_numu_cc->KolmogorovTest(h1_v_numu_cc, "M");     // distanza massima D
  // double pKSmc2 = h1_n_numu_cc->KolmogorovTest(h1_v_numu_cc, "X");   // p-value con pseudo-esperimenti (consigliato)
  // double pKSnorm2 = h1_n_numu_cc->KolmogorovTest(h1_v_numu_cc, "N");   // p-value normalized

  // // Anderson-Darling (2 campioni, binnato)
  // double pAD2 = h1_n_numu_cc->AndersonDarlingTest(h1_v_numu_cc);          // p-value
  // double tAD2 = h1_n_numu_cc->AndersonDarlingTest(h1_v_numu_cc, "T"); 
  
  // outfile2 << "=============== POT " << pot_n << " ===============" << std::endl;

  // outfile2 << "Nominal POT: "<< sRecoNuEnergyNuMuCC.POT() << '\n';
  // outfile2 << "Varied POT: " << sRecoNuEnergyNuMuCCVaried.POT() << '\n';
  
  // outfile2 << "Nominal POT used to rescale histograms: " << pot_n << '\n';
  // outfile2 << "Nominal POT / Varied POT: " << pot_n/sRecoNuEnergyNuMuCCVaried.POT() << std::endl;

  // outfile2 << "Nominal: int=" << h1_n_numu_cc->Integral() << " entries=" << h1_n_numu_cc->GetEntries() << " eff=" << h1_n_numu_cc->GetEffectiveEntries() << '\n';
  // outfile2 << "Varied: int=" << h1_v_numu_cc->Integral() << " entries=" << h1_v_numu_cc->GetEntries() << " eff=" << h1_v_numu_cc->GetEffectiveEntries() << '\n';

  // outfile2 << "Kolmogorov-Smirnov p-value: " << pKS2 << " Kolmogorov-Smirnov distance (D): " << dKS2 << " Kolmogorov-Smirnov pseudo-experiment: " << pKSmc2 << " Kolmogorov-Smirnov normalized: " << pKSnorm2 << std::endl;
  // outfile2 << "Anderson-Darling p-value: " << pAD2 << " Anderson-Darling test statistic (T): " << tAD2 << std::endl;
  // outfile2 << "=============================================" << std::endl;


  // outfile2.close();

}
