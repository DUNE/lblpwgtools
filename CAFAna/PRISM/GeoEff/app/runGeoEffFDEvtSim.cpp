// Library methods
#include "geoEff.h"

// ROOT includes
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TH3.h>
#include <TCut.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <THStack.h>
#include <TFitResultPtr.h>
#include <TChain.h>
#include <TChainElement.h>
#include <TEfficiency.h>
#include <TMath.h>
#include "TLorentzVector.h"
#include <TRandom3.h>
#include "TSystem.h"
#include "TROOT.h"

// C++ includes
#include <iostream>
#include <iomanip>
using namespace std;
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <vector> // Need this for generate dictionary for nested vectors

// Include customized functions and constants
#include "step1_helpers.h"

// Pick six evenly spaced points in each non-dead region
vector<double> generatePoints(double start)
{
  vector<double> points;
  double step = 7.0; // Interval between points
  double current;

  if (start == -300) {
      current = start + 1.45; // Adjusted start for the edge case
  } else if (start == 256.55) {
      current = start + 7.0; // Start for the symmetric edge case
  } else {
      current = start + step; // Regular start
  }

  for (int i = 0; i < 6; ++i) {
      points.push_back(current + i * step);
  }
  return points;
}

// The program which do all translations and rotations when moving event from FD to ND:
// 0. FD: read event from FD MC ntuple: before earth curvature rotation
// 1. FD to ND: after earth curvature rotation
// 2. ND: move back to the beam center
// 3. ND to ND: translate from OnAxis to OffAxis
// 4. ND: get after eigen rotated vectors for step 3
// 5. ND: generate random throws
// 6. Calculate Geo Eff
int main(int argc, char** argv)
{

  string inFname;
  int seed;

  if ( (argc > 3) or (argc == 1) ){
    cout << "Usage: ./runGeoEffFDEvtSim inputFDntuple (seed)" << endl;
    exit(-1);
  } else if(argc == 2){
    inFname = string(argv[1]);
    random_device rd; // generate random seed number
    seed = rd();
  } else {
    inFname = string(argv[1]);
    seed = atoi(argv[2]); //turn char into int, use the input number as seed
  }

  cout << "seed: " << seed << endl;

  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // 0. FD: read event from FD MC ntuple: before earth curvature rotation
  //
  TChain *t = new TChain("MyEnergyAnalysis/MyTree");
  // Ntuple path on FNAL dunegpvm machine
  t->Add(inFname.c_str());

  // Define variables for FD event
  int FD_Run; // # of the run being processed
  int FD_SubRun; // # of the sub-run being processed
  int FD_Event; // # of the event being processed
  int FD_Sim_nNumu; // # of Sim muon neutrinos (numu and numubar)
  double FD_Gen_numu_E; // Energy of generator level neutrino [MeV]
  double FD_Sim_numu_E; //Energt og neutrino at GEANT level
  double FD_E_vis_true; // True visible energy [MeV],  E_vis, true = LepE + eP + ePip + ePim + ePi0 + eOther + nipi0 * pi0_mass,  where pi0_mass is a constant that's 135 MeV
  double FD_LepNuAngle;                // Angle b/w nu and lepton [radians]
  double FD_W;     //Invariant mass of hadronic system [GeV]
  double FD_True_HadE; // true HadE (not the one from deposits)
  int FD_Sim_nMu; // # of Sim muons (mu+/mu-)
  int FD_Sim_nPionCharged; // # of sim charged pions (pi+/pi-)
  int FD_Sim_nPionNeutral; // #of sim neutral pions pi0
  int FD_Sim_nP; //#of sim protons


  int FD_CCNC_truth; // 0 =CC 1 =NC
  int FD_neuPDG; // Generator level neutrino PDG
  int FD_LepPDG; // PDG code of the final-state lepton
  double FD_Sim_mu_start_vx; // Position of the muon trajectory at start point on the x-axis [cm]
  double FD_Sim_mu_start_vy; // Position of the muon trajectory at start point on the y-axis [cm]
  double FD_Sim_mu_start_vz; // Position of the muon trajectory at start point on the z-axis [cm]
  double FD_Sim_mu_end_vx; // Position of the muon trajectory at end point on the x-axis [cm]
  double FD_Sim_mu_end_vy; // Position of the muon trajectory at end point on the y-axis [cm]
  double FD_Sim_mu_end_vz; // Position of the muon trajectory at end point on the z-axis [cm]
  double FD_Sim_mu_start_px; // Momentum of the muon trajectory at start point on the x-axis [GeV]
  double FD_Sim_mu_start_py; // Momentum of the muon trajectory at start point on the y-axis [GeV]
  double FD_Sim_mu_start_pz; // Momentum of the muon trajectory at start point on the z-axis [GeV]
  double FD_Sim_mu_start_E; // Energy of leading mu at start point [GeV]
  double FD_Sim_mu_end_px; // Momentum of the muon trajectory at end point on the x-axis [GeV]
  double FD_Sim_mu_end_py; // Momentum of the muon trajectory at end point on the y-axis [GeV]
  double FD_Sim_mu_end_pz; // Momentum of the muon trajectory at end point on the z-axis [GeV]
  double FD_Sim_mu_end_E; // Energy of leading mu at end point [GeV]
  double FD_Sim_hadronic_Edep_b2; // Total amount of energy released by ionizations in the event (from Geant4 simulation) [MeV]
  double FD_Sim_mu_track_length; // leading muon track length
  double FD_Sim_mu_Edep_b2; // Muon energy deposit: total amount of energy released by ionizations in the event (from Geant 4 sim) [MeV]
  int FD_Sim_n_hadronic_Edep_b; // # of hadronic energy deposits
  vector<float> *FD_Sim_hadronic_hit_Edep_b2 = 0; // Need initialize 0 here to avoid error
  vector<float> *FD_Sim_hadronic_hit_x_b = 0; // Position of each energy deposit on the x-axis [cm]
  vector<float> *FD_Sim_hadronic_hit_y_b = 0; // Position of each energy deposit on the y-axis [cm]
  vector<float> *FD_Sim_hadronic_hit_z_b = 0; // Position of each energy deposit on the z-axis [cm]

  // Extract event info from ntuple
  t->SetBranchAddress("Run",                      &FD_Run);
  t->SetBranchAddress("SubRun",                   &FD_SubRun);
  t->SetBranchAddress("Event",                    &FD_Event);
  t->SetBranchAddress("Sim_nNumu",                &FD_Sim_nNumu);
  t->SetBranchAddress("Gen_numu_E",               &FD_Gen_numu_E);
  t->SetBranchAddress("Sim_numu_E",               &FD_Sim_numu_E);
  t->SetBranchAddress("True_HadE" ,               &FD_True_HadE);
  t->SetBranchAddress("LepNuAngle",               &FD_LepNuAngle);
  t->SetBranchAddress("W",                        &FD_W);
  t->SetBranchAddress("E_vis_true",               &FD_E_vis_true);
  t->SetBranchAddress("Sim_nMu",                  &FD_Sim_nMu);
  t->SetBranchAddress("Sim_nProton",              &FD_Sim_nP);
  t->SetBranchAddress("Sim_nPionCharged",         &FD_Sim_nPionCharged);
  t->SetBranchAddress("Sim_nPionNeutral",         &FD_Sim_nPionNeutral);
  t->SetBranchAddress("CCNC_truth",               &FD_CCNC_truth);
  t->SetBranchAddress("neuPDG",                   &FD_neuPDG);
  t->SetBranchAddress("LepPDG",                   &FD_LepPDG);
  t->SetBranchAddress("Sim_mu_start_vx",          &FD_Sim_mu_start_vx);
  t->SetBranchAddress("Sim_mu_start_vy",          &FD_Sim_mu_start_vy);
  t->SetBranchAddress("Sim_mu_start_vz",          &FD_Sim_mu_start_vz);
  t->SetBranchAddress("Sim_mu_end_vx",            &FD_Sim_mu_end_vx);
  t->SetBranchAddress("Sim_mu_end_vy",            &FD_Sim_mu_end_vy);
  t->SetBranchAddress("Sim_mu_end_vz",            &FD_Sim_mu_end_vz);
  t->SetBranchAddress("Sim_mu_start_px",          &FD_Sim_mu_start_px);
  t->SetBranchAddress("Sim_mu_start_py",          &FD_Sim_mu_start_py);
  t->SetBranchAddress("Sim_mu_start_pz",          &FD_Sim_mu_start_pz);
  t->SetBranchAddress("Sim_mu_start_E",           &FD_Sim_mu_start_E);
  t->SetBranchAddress("Sim_mu_end_px",            &FD_Sim_mu_end_px);
  t->SetBranchAddress("Sim_mu_end_py",            &FD_Sim_mu_end_py);
  t->SetBranchAddress("Sim_mu_end_pz",            &FD_Sim_mu_end_pz);
  t->SetBranchAddress("Sim_mu_end_E",             &FD_Sim_mu_end_E);
  t->SetBranchAddress("Sim_hadronic_Edep_b2",     &FD_Sim_hadronic_Edep_b2);
  t->SetBranchAddress("Sim_n_hadronic_Edep_b",    &FD_Sim_n_hadronic_Edep_b);
  t->SetBranchAddress("Sim_hadronic_hit_Edep_b2", &FD_Sim_hadronic_hit_Edep_b2);
  t->SetBranchAddress("Sim_hadronic_hit_x_b",     &FD_Sim_hadronic_hit_x_b);
  t->SetBranchAddress("Sim_hadronic_hit_y_b",     &FD_Sim_hadronic_hit_y_b);
  t->SetBranchAddress("Sim_hadronic_hit_z_b",     &FD_Sim_hadronic_hit_z_b);
  t->SetBranchAddress("Sim_mu_Edep_b2",           &FD_Sim_mu_Edep_b2);
  t->SetBranchAddress("Sim_mu_track_length",      &FD_Sim_mu_track_length);
  //
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // Declare variables used in this program
  //
  int nentries = 0; // Total input events
  double vetoEnergyFD; // Total hadron deposited energy in FD veto region
  int iwritten = 0; // Output event counter
  float decayZbeamCoord; // Decay point (neutrino production point) in beam coordinate [cm]
  float decayXdetCoord; // Decay point (neutrino production point) in detector coordinate on the x-axis [cm]
  float decayYdetCoord; // Decay point (neutrino production point) in detector coordinate on the y-axis [cm]
  float decayZdetCoord; // Decay point (neutrino production point) in detector coordinate on the z-axis [cm]
  vector<float> HadronHitEdeps; // Hadron hit segment energy deposits [MeV]
  vector<float> HadronHitPoss;  // Hadron hit segment energy deposits position [cm]

  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // Choose ND_Lar detector and vtx within ND_LAr positions
  vector<double> ND_LAr_dtctr_pos_vec = {0}; // {-175, -200, -400, -575, -800, -975, -1200, -1375, -1600, -1775, -2000, -2175, -2400, -2575, -2625, -2800, -2825, -2850};      // unit: cm, ND LAr detector off-axis choices for each FD evt
  vector<double> ND_vtx_vx_vec;             // unit: cm, vtx x choices for each FD evt in ND_Lar volume
  vector<double> ND_OffAxis_pos_vec;        // unit: cm, Off-Axis pos = ND_LAr_dtctr_pos_vec + ND_vtx_vx_vec;

  // int ND_off_axis_pos_steps = 0;
  // int vtx_vx_steps = 0;

  // Initialize first element as -999, to be replaced by a random off-axis nd pos in each evt below
  //ND_LAr_dtctr_pos_vec.clear();
  //ND_LAr_dtctr_pos_vec.emplace_back(0);

  // Old algorithm chooseing ND off axis pos
  // if ( ND_Lar_dtctr_pos_new_stepsize > 0 && ND_Lar_dtctr_pos_new_stepsize <= OffAxisPoints[13] ) {
  //   ND_off_axis_pos_steps = ( OffAxisPoints[13] - OffAxisPoints[0] ) / ND_Lar_dtctr_pos_new_stepsize;
  // }
  // else std::cout << "Error: please set the ND_Lar_dtctr_pos_new_stepsize above 0 and below max element of OffAxisPoints." << std::endl;
  //
  // // if (verbose) std::cout << "ND_off_axis_pos_steps: " << ND_off_axis_pos_steps << std::endl;
  //
  // // The rest elements follow fixed increments from min ND local x
  // for ( int i_ND_off_axis_pos_step = 0; i_ND_off_axis_pos_step < ND_off_axis_pos_steps + 1; i_ND_off_axis_pos_step++ ){
  //   ND_LAr_dtctr_pos_vec.emplace_back( -(i_ND_off_axis_pos_step*ND_Lar_dtctr_pos_new_stepsize + OffAxisPoints[0])*100. );
  // }
  //
  // if (verbose) std::cout << "ND_LAr_dtctr_pos_vec size: "<< ND_LAr_dtctr_pos_vec.size() << std::endl;

  // //------------------------------------------------------------------------------
  // // New algorithm chooseing ND off axis pos
  // int LArPos_new1_step = 0;
  // int LArPos_new2_step = 0;
  //
  // // Calculate steps
  // if ( ND_Lar_dtctr_pos_new_stepsize > 0 && ND_Lar_dtctr_pos_new_stepsize <= abs(NDLarPos_new1[1]) ) {
  //   LArPos_new1_step = - ( NDLarPos_new1[1] - NDLarPos_new1[0] ) / ND_Lar_dtctr_pos_new_stepsize;
  // }
  // else std::cout << "Error: please set the ND_Lar_dtctr_pos_new_stepsize above 0 and below max element of OffAxisPoints." << std::endl;
  // if ( ND_Lar_dtctr_pos_new_stepsize > 0 && ND_Lar_dtctr_pos_new_stepsize <= abs(NDLarPos_new2[1]) ) {
  //   LArPos_new2_step = - ( NDLarPos_new2[1] - NDLarPos_new2[0] ) / ND_Lar_dtctr_pos_new_stepsize;
  // }
  // else std::cout << "Error: please set the ND_Lar_dtctr_pos_new_stepsize above 0 and below max element of OffAxisPoints." << std::endl;
  //
  // if (verbose) cout << "LArPos_new1_step: " << LArPos_new1_step <<endl;
  // if (verbose) cout << "LArPos_new2_step: " << LArPos_new2_step <<endl;
  //
  // for ( int i_ND_off_axis_pos_step = 0; i_ND_off_axis_pos_step < LArPos_new1_step + 1; i_ND_off_axis_pos_step++ )
  // {
  //   ND_LAr_dtctr_pos_vec.emplace_back( (i_ND_off_axis_pos_step*ND_Lar_dtctr_pos_new_stepsize + NDLarPos_new1[1])*100. );
  // }
  //
  // // use below only when we need more ND_LAr positions w/ different distance
  // // for ( int i_ND_off_axis_pos_step = 0; i_ND_off_axis_pos_step < LArPos_new2_step + 1; i_ND_off_axis_pos_step++ )
  // // {
  // //   ND_LAr_dtctr_pos_vec.emplace_back( (i_ND_off_axis_pos_step*ND_Lar_dtctr_pos_new_stepsize + NDLarPos_new2[1])*100. );
  // // }
  //
  // // Sort the vector
  // sort(ND_LAr_dtctr_pos_vec.begin(), ND_LAr_dtctr_pos_vec.end());
  // if (true)
  // {for (auto x : ND_LAr_dtctr_pos_vec)
  //       std::cout << x << ",  ";}
  // if (true) std::cout << "ND_LAr_dtctr_pos_vec size: "<< ND_LAr_dtctr_pos_vec.size() << std::endl;
  //
  //

  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  // // Initialize first element as -999, to be replaced by a random vtx x in each evt below
  // ND_vtx_vx_vec.clear();
  //
  // if ( ND_local_x_stepsize > 0 && ND_local_x_stepsize <= ND_local_x_max ) {
  //   vtx_vx_steps = ( ND_local_x_max - ND_local_x_min ) / ND_local_x_stepsize;
  // }
  // else std::cout << "Error: please set the ND_local_x_stepsize above 0 and below ND_local_x_max." << std::endl;
  //
  // if (verbose) std::cout << "vtx_vx_steps: " << vtx_vx_steps << std::endl;
  //
  // for (int i =0; i<5; i++)
  // {
  //   ND_vtx_vx_vec.emplace_back(-299+7*i);
  // }
  // // The rest elements follow fixed increments from min ND local x
  // for ( int i_vtx_vx_step = 0; i_vtx_vx_step < vtx_vx_steps + 1; i_vtx_vx_step++ ){
  //   if((i_vtx_vx_step*ND_local_x_stepsize + ND_local_x_min)<300 && (i_vtx_vx_step*ND_local_x_stepsize + ND_local_x_min)>-300)
  //   {
  //     ND_vtx_vx_vec.emplace_back( i_vtx_vx_step*ND_local_x_stepsize + ND_local_x_min );
  //   }
  // }
  // for (int i =0; i<5; i++)
  // {
  //   ND_vtx_vx_vec.emplace_back(271+7*i);
  // }

  // Generate six evenly spaced point in each non-dead region
  // vector<pair<double, double>> non_dead_regions = {
  //     {-300, -256.55}, {-253.95, -204.95}, {-203.45, -154.45}, {-151.85, -102.85},
  //     {-101.35, -52.35}, {-49.75, -0.75}, {0.75, 49.75}, {52.35, 101.35},
  //     {102.85, 151.85}, {154.45, 203.45}, {204.95, 253.95}, {256.55, 300}
  // };
  //for NDFV -200, 200
  vector<pair<double, double>> non_dead_regions = {
      {-203.45, -154.45}, {-151.85, -102.85},
      {-101.35, -52.35}, {-49.75, -0.75}, {0.75, 49.75}, {52.35, 101.35},
      {102.85, 151.85}, {154.45, 203.45}
  };

  // Iterate over each non-dead region and generate points
  for (const auto& region : non_dead_regions) {
      auto points = generatePoints(region.first);
      // std::cout << "Points between " << region.first << " and " << region.second << ": ";
      for (auto point : points) {
          std::cout << point << " ";
          ND_vtx_vx_vec.emplace_back(point);
      }
      std::cout << std::endl;
  }

  // Define the non-dead regions
  if (true)
  {for (auto x : ND_vtx_vx_vec)
        std::cout << x << ",  ";}

  if (true) std::cout << "ND_vtx_vx_vec size: "<< ND_vtx_vx_vec.size() << std::endl;

  // Generate ND_off_axis_pos_vec
  for ( double i_ND_off_axis_pos : ND_LAr_dtctr_pos_vec )
  {
    for ( double i_vtx_vx : ND_vtx_vx_vec )
    {
        ND_OffAxis_pos_vec.emplace_back(i_ND_off_axis_pos+i_vtx_vx);
    }
  }
  //
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //

  // Result of hadron containment is stored in nested vectors, need to generate dictionary
  gSystem->Exec("rm -f AutoDict*vector*vector*float*"); // Remove old dictionary if exists
  gSystem->Exec("rm -f AutoDict*vector*vector*vector*float*"); // Remove old dictionary if exists
  gSystem->Exec("rm -f AutoDict*vector*vector*vector*vector*float*"); // Remove old dictionary if exists
  gSystem->Exec("rm -f AutoDict*vector*vector*double*"); // Remove old dictionary if exists
  gSystem->Exec("rm -f AutoDict*vector*vector*vector*double*"); // Remove old dictionary if exists
  gSystem->Exec("rm -f AutoDict*vector*vector*vector*vector*double*"); // Remove old dictionary if exists
  gSystem->Exec("rm -f AutoDict*vector*vector*bool*"); // Remove old dictionary if exists
  gSystem->Exec("rm -f AutoDict*vector*vector*vector*bool*");
  gSystem->Exec("rm -f AutoDict*vector*vector*vector*vector*bool*");
  gSystem->Exec("rm -f AutoDict*vector*vector*vector*uint64_t*");
  gSystem->Exec("rm -f AutoDict*vector*vector*vector*vector*uint64_t*");
  gSystem->Exec("rm -f AutoDict*vector*vector*vector*vector*vector*uint64_t*");
  // Generate new dictionary
  gInterpreter->GenerateDictionary("vector<vector<float> >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<vector<float> > >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<vector<vector<float> > > >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<double> >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<vector<double> > >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<vector<vector<double> > > >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<bool> >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<vector<bool> > >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<vector<vector<bool> > > >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<vector<uint64_t> > >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<vector<vector<uint64_t> > > >", "vector");
  gInterpreter->GenerateDictionary("vector<vector<vector<vector<vector<uint64_t> > > > >", "vector");
  // Nested vector (vetoSize > vetoEnergy > 64_bit_throw_result) returned from function getHadronContainmentThrows
  vector<vector<vector<uint64_t>>> hadron_throw_result;
  vector<vector<vector<vector<uint64_t>>>> hadron_throw_result_vtx; // vtx_pos > vetoSize > vetoEnergy > 64_bit_throw_result
  vector<vector<vector<vector<vector<uint64_t>>>>> hadron_throw_result_LAr; // LAr_pos > vtx_pos > vetoSize > vetoEnergy > 64_bit_throw_result

  //nested vectore returning vetoE of throw
  vector<vector<vector<float>>> vetoEThrow;
  //nested vector reurning trimE of throws
  vector<vector<vector<float>>> trimEThrow;
  //total hadronic energy at FD
  float totEnergyFDatFD_float;
  //IOANA: save total nr of protons and charged and neutral pions within the simulation
  int TotalSimP;
  int TotalSimNeutralPions;
  int TotalSimChargedPions;

  //
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // 1. FD to ND: after earth curvature rotation
  //
  // Lepton info: expressed in ND coordinate sys, do not confuse with branches read above in FD coordinate sys
  double ND_Gen_numu_E; // Energy of generator level neutrino [GeV]
  double ND_E_vis_true; // True visible energy of neutrino [GeV]
  double ND_LepNuAngle; // Angle b/w nu and lepton [radians]
  double ND_W;          // Invariant mass of hadronic system [GeV]
  // Muon info
  // array: (x,y,z)<->dim=(0,1,2)
  double ND_RandomVtx_Sim_mu_start_v[3]; // Position of the muon trajectory at start point [cm]
  double ND_RandomVtx_Sim_mu_end_v[3]; // Position of the muon trajectory at end point [cm]
  double ND_RandomVtx_Sim_mu_start_p[3]; // Momentum of the muon trajectory at start point on the x-axis [GeV]
  // double ND_RandomVtx_Sim_mu_start_E; // Energy of leading mu at start point [GeV]
  // Hadron info
  double ND_Sim_hadronic_Edep_b2; // Total amount of energy released by ionizations in the event (from Geant4 simulation) [MeV]
  vector<vector<float>> ND_RandomVtx_Sim_hadronic_hit; // Position of each energy deposit [cm]
  vector<float> ND_RandomVtx_Sim_hadronic_hit_xyz; // Position of each energy deposit [cm]
  // Momentum conservation check
  float ND_RandomVtx_Sim_mu_start_p_total; // Total momentum of ND_OnAxis_Sim_mu_start_p_total

  //
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // 2. ND: move back to the beam center
  //
  // Muon info
  // array: (x,y,z)<->dim=(0,1,2)
  double ND_OnAxis_Sim_mu_start_v[3]; // Position of the muon trajectory at start point [cm]
  double ND_OnAxis_Sim_mu_end_v[3]; // Position of the muon trajectory at end point [cm]
  double ND_OnAxis_Sim_mu_start_p[3]; // Momentum of the muon trajectory at start point on the x-axis [GeV]
  // Hadron info
  vector<vector<float>> ND_OnAxis_Sim_hadronic_hit; // Position of each energy deposit [cm]
  vector<float> ND_OnAxis_Sim_hadronic_hit_xyz;

  // Momentum conservation check
  float ND_OnAxis_Sim_mu_start_p_total; // Total momentum of ND_OnAxis_Sim_mu_start_p_total
  //
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // 3. ND to ND: translate from OnAxis to OffAxis
  //
  // Muon info
  // array: (x,y,z)<->dim=(0,1,2)
  double ND_OffAxis_Unrotated_Sim_mu_start_v[3]; // Position of the muon trajectory at start point [cm]
  vector<double> ND_OffAxis_Unrotated_Sim_mu_start_v_xyz; // Vector of ND_OffAxis_Unrotated_Sim_mu_start_v in (x1,y1,z1,x2,y2,z2,......) order
  vector<vector<double>> ND_OffAxis_Unrotated_Sim_mu_start_v_xyz_vtx; // nested vector: <vtx_pos<ND_OffAxis_Unrotated_Sim_mu_start_v_xyz>>
  vector<vector<vector<double>>> ND_OffAxis_Unrotated_Sim_mu_start_v_xyz_LAr; // nested vector: <ND_LAr_pos<vtx_pos<ND_OffAxis_Unrotated_Sim_mu_start_v_xyz>>>

  double ND_OffAxis_Unrotated_Sim_mu_end_v[3]; // Position of the muon trajectory at end point [cm]
  vector<double> ND_OffAxis_Unrotated_Sim_mu_end_v_xyz; // Vector of ND_OffAxis_Unrotated_Sim_mu_end_v in (x1,y1,z1,x2,y2,z2,......) order
  vector<vector<double>> ND_OffAxis_Unrotated_Sim_mu_end_v_xyz_vtx; // nested vector: <vtx_pos<ND_OffAxis_Unrotated_Sim_mu_end_v_xyz>>
  vector<vector<vector<double>>> ND_OffAxis_Unrotated_Sim_mu_end_v_xyz_LAr; // nested vector: <ND_LAr_pos<vtx_pos<ND_OffAxis_Unrotated_Sim_mu_end_v_xyz>>>

  double ND_OffAxis_Unrotated_Sim_mu_start_p[3]; // Momentum of the muon trajectory at start point on the x-axis [GeV]
  vector<double> ND_OffAxis_Unrotated_Sim_mu_start_p_xyz; // Vector of ND_OffAxis_Unrotated_Sim_mu_start_p_xyz in (x1,y1,z1,x2,y2,z2,......) order
  vector<vector<double>> ND_OffAxis_Unrotated_Sim_mu_start_p_xyz_vtx; // nested vector: <vtx_pos<ND_OffAxis_Unrotated_Sim_mu_start_p_xyz>>
  vector<vector<vector<double>>> ND_OffAxis_Unrotated_Sim_mu_start_p_xyz_LAr; // nested vector: <ND_LAr_pos<vtx_pos<ND_OffAxis_Unrotated_Sim_mu_start_p_xyz>>>
  // Hadron info
  vector<vector<float>> ND_OffAxis_Unrotated_Sim_hadronic_hit; // Position of each energy deposit [cm] : <ihadronhit < hadronic hits xyz>>
  vector<float> ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz; // Position of each energy deposit [cm]
  vector<vector<vector<float>>> ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz_vtx; // < vtx_pos <ihadronhit < hadronic hits xyz>>>
  vector<vector<vector<vector<float>>>> ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz_LAr; // <ND_LAr_pos < vtx_pos <ihadronhit < hadronic hits xyz>>>>

  // Momentum conservation check
  float ND_OffAxis_Unrotated_Sim_mu_start_p_total; // Total momentum of ND_OffAxis_Unrotated_Sim_mu_start_p
  //
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // 4. ND: get after eigen rotated vectors for step 4
  //
  // Muon info
  // array: (x,y,z)<->dim=(0,1,2)
  double ND_OffAxis_Sim_mu_start_v[3]; // Position of the muon trajectory at start point [cm]
  vector<double> ND_OffAxis_Sim_mu_start_v_xyz; // Vector of ND_OffAxis_Sim_mu_start_v in (x1,y1,z1,x2,y2,z2,......) order
  vector<vector<double>> ND_OffAxis_Sim_mu_start_v_xyz_vtx; // nested vector: <vtx_pos<ND_OffAxis_Sim_mu_start_v_xyz>>
  vector<vector<vector<double>>> ND_OffAxis_Sim_mu_start_v_xyz_LAr; // nested vector: <ND_LAr_pos<vtx_pos<ND_OffAxis_Sim_mu_start_v_xyz>>>

  double ND_OffAxis_Sim_mu_end_v[3]; // Position of the muon trajectory at end point [cm]
  vector<double> ND_OffAxis_Sim_mu_end_v_xyz; // Vector of ND_OffAxis_Sim_mu_end_v_xyz in (x1,y1,z1,x2,y2,z2,......) order
  vector<vector<double>> ND_OffAxis_Sim_mu_end_v_xyz_vtx; // nested vector: <vtx_pos<ND_OffAxis_Sim_mu_end_v_xyz>>
  vector<vector<vector<double>>> ND_OffAxis_Sim_mu_end_v_xyz_LAr; // nested vector: <ND_LAr_pos<vtx_pos<ND_OffAxis_Sim_mu_end_v_xyz>>>

  double ND_OffAxis_Sim_mu_start_p[3]; // Momentum of the muon trajectory at start point on the x-axis [GeV]
  vector<double> ND_OffAxis_Sim_mu_start_p_xyz; // Vector of ND_OffAxis_Sim_mu_start_p_xyz in (x1,y1,z1,x2,y2,z2,......) order
  vector<vector<double>> ND_OffAxis_Sim_mu_start_p_xyz_vtx; // nested vector: <vtx_pos<ND_OffAxis_Sim_mu_start_p_xyz>>
  vector<vector<vector<double>>> ND_OffAxis_Sim_mu_start_p_xyz_LAr; // nested vector: <ND_LAr_pos<vtx_pos<ND_OffAxis_Sim_mu_start_p_xyz>>>

  double ND_OffAxis_Sim_mu_start_E; // Energy of the muon trajectory [GeV]
  vector<double> ND_OffAxis_Sim_mu_start_E_xyz; // Vector of ND_OffAxis_Sim_mu_start_E_xyz in (E1,E2,......) order
  vector<vector<double>> ND_OffAxis_Sim_mu_start_E_xyz_vtx; // nested vector: <vtx_pos<ND_OffAxis_Sim_mu_start_E_xyz>>
  vector<vector<vector<double>>> ND_OffAxis_Sim_mu_start_E_xyz_LAr; // nested vector: <ND_LAr_pos<vtx_pos<ND_OffAxis_Sim_mu_start_E_xyz>>>


  // Hadron info
  vector<vector<float>> vetoEnergyFDatND; // Veto E of FD events at ND <ND_LAr_pos < vtx_pos <vetoEnergyFDatND> > >
  vector<float> vetoEnergyFDatND_vtx; // Veto E of FD events at ND  < vtx_pos <vetoEnergyFDatND> >
  vector<vector<float>> outEnergyFDatND; // out E of FD events at ND <ND_LAr_pos < vtx_pos <vetoEnergyFDatND> > >
  vector<float> outEnergyFDatND_vtx; // out E of FD events at ND  < vtx_pos <vetoEnergyFDatND> >
  vector<vector<float>> totEnergyFDatND; // tot E of FD events at ND <ND_LAr_pos < vtx_pos <vetoEnergyFDatND> > >
  vector<float> totEnergyFDatND_vtx; // tot E of FD events at ND  < vtx_pos <vetoEnergyFDatND> >
  vector<vector<float>> ND_OffAxis_Sim_hadronic_hit; // Position of each energy deposit [cm]: <ihadronhit < hadronic hits xyz>>
  vector<float> ND_OffAxis_Sim_hadronic_hit_xyz; //order is differert from previous
  vector<vector<vector<float>>> ND_OffAxis_Sim_hadronic_hit_xyz_vtx; // < vtx_pos <ihadronhit < hadronic hits xyz>>>
  vector<vector<vector<vector<float>>>> ND_OffAxis_Sim_hadronic_hit_xyz_LAr; // <ND_LAr_pos < vtx_pos <ihadronhit < hadronic hits xyz>>>>

  // Momentum conservation check
  float ND_OffAxis_Sim_mu_start_p_total; // Total momentum of ND_OffAxis_Sim_mu_start_p
  //
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  // 5. ND: generate random throws
    vector<vector<float>> CurrentThrowDepsX; // Coordinates of hadron hits X after random throws
    vector<vector<float>> CurrentThrowDepsY; // Coordinates of hadron hits Y after random throws
    vector<vector<float>> CurrentThrowDepsZ; // Coordinates of hadron hits Z after random throws
    vector<float> CurrentThrowTotE;


  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // Store variables into a tree

  // Store ND_LAr_dtctr_pos_vec and ND_vtx_vx_vec
  vector<Int_t> iwritten_vec; // vector of iwritten
  TTree *effPosND = new TTree("effPosND", "ND offaxis pos vector");
  effPosND->Branch("ND_LAr_dtctr_pos_vec",                       &ND_LAr_dtctr_pos_vec);                             // vector<double>: entries = written evts * ND_off_axis_pos_steps
  effPosND->Branch("ND_vtx_vx_vec",                              &ND_vtx_vx_vec);
  effPosND->Branch("ND_OffAxis_pos_vec",                         &ND_OffAxis_pos_vec);
  effPosND->Branch("iwritten_vec",                               &iwritten_vec);

  //IOANA: save the results of throws in the effTreeND
  vector<uint64_t> ThrowResults;
  //IOANA: save nr of valid throws for each event (at each vtx_x and OA pos)
  int validThrows ;
  //IOANA: save nr of total passed throws for each event (at each vtx_x and OA pos)
  int NPassedThrows;
  //IOANA: save veto Energy of events that pass the throws
  vector<float> VetoEnergyEventsPass;
  //IOANA: save trim Energy of events that pass the throws
  vector<float> TrimEnergyEventsPass;
  //IOANA: save total energy of events that pass throws
  vector<float> TotalEnergyEventsPass;
  //IOANA: save vtx y and vtx X of throws corresponding to passing events
  vector<float> VtxYEventsPass;
  vector<float> VtxZEventsPass;

  TTree *effTreeFD = new TTree("effTreeND", "FD eff Tree");
  effTreeFD->Branch("ND_Gen_numu_E",                             &ND_Gen_numu_E,            "ND_Gen_numu_E/D");
  effTreeFD->Branch("ND_E_vis_true",                             &ND_E_vis_true,            "ND_E_vis_true/D");
  effTreeFD->Branch("vetoEnergyFD",                              &vetoEnergyFD,             "vetoEnergyFD/D");
  effTreeFD->Branch("ND_LepNuAngle",                             &ND_LepNuAngle,            "ND_LepNuAngle/D");
  effTreeFD->Branch("ND_W",                                      &ND_W,                     "ND_W/D");
  effTreeFD->Branch("ND_Sim_n_hadronic_Edep_b",                  &FD_Sim_n_hadronic_Edep_b,            "FD_Sim_n_hadronic_Edep_b/I");
  // 1. FD to ND: after earth curvature rotation
  effTreeFD->Branch("ND_RandomVtx_Sim_mu_start_v",               ND_RandomVtx_Sim_mu_start_v,       "ND_RandomVtx_Sim_mu_start_v[3]/D");   // entries = written evts*3
  effTreeFD->Branch("ND_RandomVtx_Sim_mu_end_v",                 ND_RandomVtx_Sim_mu_end_v,         "ND_RandomVtx_Sim_mu_end_v[3]/D");   // entries = written evts*3
  effTreeFD->Branch("ND_RandomVtx_Sim_mu_start_p",               ND_RandomVtx_Sim_mu_start_p,       "ND_RandomVtx_Sim_mu_start_p[3]/D");   // entries = written evts*3
  effTreeFD->Branch("ND_Sim_hadronic_Edep_b2",                   &ND_Sim_hadronic_Edep_b2,          "ND_Sim_hadronic_Edep_b2/D"); // entries = written evts
  if (ntupleVerbose) effTreeFD->Branch("ND_RandomVtx_Sim_hadronic_hit_xyz",         &ND_RandomVtx_Sim_hadronic_hit);

  // 2. ND: move back to the beam center
  effTreeFD->Branch("ND_OnAxis_Sim_mu_start_v",               ND_OnAxis_Sim_mu_start_v,       "ND_OnAxis_Sim_mu_start_v[3]/D");   // entries = written evts*3
  effTreeFD->Branch("ND_OnAxis_Sim_mu_end_v",                 ND_OnAxis_Sim_mu_end_v,         "ND_OnAxis_Sim_mu_end_v[3]/D");   // entries = written evts*3
  effTreeFD->Branch("ND_OnAxis_Sim_mu_start_p",               ND_OnAxis_Sim_mu_start_p,       "ND_OnAxis_Sim_mu_start_p[3]/D");   // entries = written evts*3
  if (ntupleVerbose) effTreeFD->Branch("ND_OnAxis_Sim_hadronic_hit_xyz",             &ND_OnAxis_Sim_hadronic_hit);

  // 3. ND to ND: translate from OnAxis to OffAxis
  // effTreeFD->Branch("ND_OffAxis_Unrotated_Sim_mu_start_v",               ND_OffAxis_Unrotated_Sim_mu_start_v,       "ND_OffAxis_Unrotated_Sim_mu_start_v[3]/D");   // entries = written evts*3
  // effTreeFD->Branch("ND_OffAxis_Unrotated_Sim_mu_end_v",                 ND_OffAxis_Unrotated_Sim_mu_end_v,         "ND_OffAxis_Unrotated_Sim_mu_end_v[3]/D");   // entries = written evts*3
  // effTreeFD->Branch("ND_OffAxis_Unrotated_Sim_mu_start_p",               ND_OffAxis_Unrotated_Sim_mu_start_p,       "ND_OffAxis_Unrotated_Sim_mu_start_p[3]/D");   // entries = written evts*3
  // if (ntupleVerbose) effTreeFD->Branch("ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz",         &ND_OffAxis_Unrotated_Sim_hadronic_hit);
  effTreeFD->Branch("ND_OffAxis_Unrotated_Sim_mu_start_v_xyz_LAr",       &ND_OffAxis_Unrotated_Sim_mu_start_v_xyz_LAr);
  effTreeFD->Branch("ND_OffAxis_Unrotated_Sim_mu_end_v_xyz_LAr",         &ND_OffAxis_Unrotated_Sim_mu_end_v_xyz_LAr);
  effTreeFD->Branch("ND_OffAxis_Unrotated_Sim_mu_start_p_xyz_LAr",       &ND_OffAxis_Unrotated_Sim_mu_start_p_xyz_LAr);
  if (ntupleVerbose) effTreeFD->Branch("ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz_LAr",       &ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz_LAr);

 // 4. ND: get after eigen rotated vectors for step 4
  // effTreeFD->Branch("ND_OffAxis_Sim_mu_start_v",               ND_OffAxis_Sim_mu_start_v,       "ND_OffAxis_Sim_mu_start_v[3]/D");   // entries = written evts*3
  // effTreeFD->Branch("ND_OffAxis_Sim_mu_end_v",                 ND_OffAxis_Sim_mu_end_v,         "ND_OffAxis_Sim_mu_end_v[3]/D");   // entries = written evts*3
  // effTreeFD->Branch("ND_OffAxis_Sim_mu_start_p",               ND_OffAxis_Sim_mu_start_p,       "ND_OffAxis_Sim_mu_start_p[3]/D");   // entries = written evts*3
  // effTreeFD->Branch("ND_OffAxis_Sim_mu_start_E",               &ND_OffAxis_Sim_mu_start_E,      "ND_OffAxis_Sim_mu_start_E/D");
  // effTreeFD->Branch("ND_OffAxis_Sim_hadronic_hit_xyz",         &ND_OffAxis_Sim_hadronic_hit);

  effTreeFD->Branch("ND_OffAxis_Sim_mu_start_v_xyz_LAr",       &ND_OffAxis_Sim_mu_start_v_xyz_LAr);
  effTreeFD->Branch("ND_OffAxis_Sim_mu_end_v_xyz_LAr",         &ND_OffAxis_Sim_mu_end_v_xyz_LAr);
  effTreeFD->Branch("ND_OffAxis_Sim_mu_start_p_xyz_LAr",       &ND_OffAxis_Sim_mu_start_p_xyz_LAr);
  effTreeFD->Branch("ND_OffAxis_Sim_mu_start_E_xyz_LAr",       &ND_OffAxis_Sim_mu_start_E_xyz_LAr);
  if (ntupleVerbose) effTreeFD->Branch("ND_OffAxis_Sim_hadronic_hit_xyz_LAr",         &ND_OffAxis_Sim_hadronic_hit_xyz_LAr);
  effTreeFD->Branch("vetoEnergyFDatND",         &vetoEnergyFDatND);
  effTreeFD->Branch("outEnergyFDatND",          &outEnergyFDatND);
  effTreeFD->Branch("totEnergyFDatND",          &totEnergyFDatND);
  effTreeFD->Branch("TotalSimP",                &TotalSimP);
  effTreeFD->Branch("TotalSimNeutralPions",     &TotalSimNeutralPions);
  effTreeFD->Branch("TotalSimChargedPions",     &TotalSimChargedPions);

  // 5. ND: generate random throws
  // effTreeFD->Branch("hadron_throw_result",                     &hadron_throw_result);
  effTreeFD->Branch("hadron_throw_result_LAr",                     &hadron_throw_result_LAr);
  effTreeFD->Branch("HadronHitEdeps",                       &HadronHitEdeps);
  effTreeFD->Branch("CurrentThrowDepsX",                       &CurrentThrowDepsX);
  effTreeFD->Branch("CurrentThrowDepsY",                       &CurrentThrowDepsY);
  effTreeFD->Branch("CurrentThrowDepsZ",                       &CurrentThrowDepsZ);
  effTreeFD->Branch("CurrentThrowTotE",                        &CurrentThrowTotE);
  // 6. Calculate Geo Eff
  double ND_LAr_dtctr_pos;
  double ND_LAr_vtx_pos;
  double ND_GeoEff;
  double ND_OffAxis_MeanEff = 0.;

  float vetoEnergyFDatND_float;
  float outEnergyFDatND_float;
  float totEnergyFDatND_float;
  double muonEdep;
  double muonTrackLength;

  TTree *effValues = new TTree("effValues", "ND eff Tree");
  effValues->Branch("iwritten",                     &iwritten,             "iwritten/I");
  effValues->Branch("ND_LAr_dtctr_pos",             &ND_LAr_dtctr_pos,     "ND_LAr_dtctr_pos/D");
  effValues->Branch("ND_LAr_vtx_pos",               &ND_LAr_vtx_pos,       "ND_LAr_vtx_pos/D");
  effValues->Branch("ND_GeoEff",                    &ND_GeoEff,            "ND_GeoEff/D");
  effValues->Branch("vetoEnergyFDatND_f",           &vetoEnergyFDatND_float,           "vetoEnergyFDatND_float/F");
  effValues->Branch("outEnergyFDatND_f",            &outEnergyFDatND_float,            "outEnergyFDatND_float/F");
  effValues->Branch("totEnergyFDatND_f",            &totEnergyFDatND_float,            "totEnergyFDatND_float/F");
  effValues->Branch("muonEdep_f",                   &muonEdep,             "muondEDep/D");
  effValues->Branch("muonTrackLength_f",            &muonTrackLength,      "muonTrackLength/D");
  effValues->Branch("ND_OffAxis_MeanEff",           &ND_OffAxis_MeanEff,   "ND_OffAxis_MeanEff/D");
  effValues->Branch("ThrowResults",                         &ThrowResults);
  effValues->Branch("VetoEnergyEventsPass",         &VetoEnergyEventsPass);
  effValues->Branch("TrimEnergyEventsPass",         &TrimEnergyEventsPass);
  effValues->Branch("TotalEnergyEventsPass",        &TotalEnergyEventsPass);
  effValues->Branch("VtxYEventsPass",               &VtxYEventsPass);
  effValues->Branch("VtxZEventsPass",               &VtxZEventsPass);
  effValues->Branch("validThrows",         &validThrows);
  effValues->Branch("NPassedThrows",        &NPassedThrows);



  //
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //

  //
  // A separate tree to store translations and rotations of throws
  // which will be applied to leptons before NN training
  //
  vector<float> throwVtxY;
  vector<float> throwVtxZ;
  vector<float> throwRot;

  TTree * ThrowsFD = new TTree("ThrowsFD", "FD Throws");
  ThrowsFD->Branch("seed", &seed);
  ThrowsFD->Branch("throwVtxY", &throwVtxY); // vector<float>: entries = [ (int)(written evts / 100) + 1 ] * N_throws
  ThrowsFD->Branch("throwVtxZ", &throwVtxZ);
  ThrowsFD->Branch("throwRot",  &throwRot);

  // Mean neutrino production point (beam coordinate) on z axis as a function of ND off-axis position
  TGraph* gDecayZ = new TGraph(27, OffAxisPoints, meanPDPZ);
  //
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // Get beam parameters: eventually need to read from XML file: which XML for FD? same as ND?
  //
  // Clockwise rotate beamline around ND local x axis
  double beamLineRotation = -0.101;           // unit: rad
  vector<double> BeamCenterAtND={0.0*100., 0.05387*100., 6.6*100.}; // beam center(0.0, 0.05387, 6.66) [cm]

  // Put back into beam center(0.0, 0.05387, 6.66) tanslation 1 [cm]
  ND_OnAxis_Sim_mu_start_v[0] = 0.0*100.;
  ND_OnAxis_Sim_mu_start_v[1] = 0.05387*100.;
  ND_OnAxis_Sim_mu_start_v[2] = 6.6*100.;
  // Coordinate transformation, units: meters
  double beamRefDetCoord[3] = {BeamCenterAtND[0]/100., BeamCenterAtND[1]/100., BeamCenterAtND[2]/100.}; // (0, 0, 0) is ND detector origin
  double detRefBeamCoord[3] = {0., 0., 574.}; // (0, 0, 0) is beam origin

  // Calculate neutrino production x in detector coordinate, y/z later as they depend on ND off-axis position
  decayXdetCoord = beamRefDetCoord[0] - detRefBeamCoord[0];
  //
  // Initialize geometric efficiency module
  //
  geoEff * eff = new geoEff(seed, false); // set verbose to true for debug , seed range: between 0 and 624
  eff->setNthrows(N_throws);
  // Rotate w.r.t. neutrino direction, rather than fixed beam direction
  eff->setUseFixedBeamDir(false);

  // 30 cm veto
  eff->setVetoSizes(vector<float>(1, 30.));
  // 30 MeV
  eff->setVetoEnergyThresholds(vector<float>(1, 30.));

  // Active detector dimensions for ND
  eff->setActiveX(NDActiveVol_min[0], NDActiveVol_max[0]);
  eff->setActiveY(NDActiveVol_min[1], NDActiveVol_max[1]);
  eff->setActiveZ(NDActiveVol_min[2], NDActiveVol_max[2]);

  // Range for translation throws. Use full ND FV volume but fix X.
  eff->setRangeX(-1, -1);
  eff->setRandomizeX(false);
  eff->setRangeY(ND_FV_min[1], ND_FV_max[1]);
  eff->setRangeZ(ND_FV_min[2], ND_FV_max[2]);
  // to be consistent w/ ND CAF
  // eff->setRangeY(NDActiveVol_min[1], NDActiveVol_max[1]);
  // eff->setRangeZ(NDActiveVol_min[2], NDActiveVol_max[2]);

  // Set offset between MC coordinate system and det volumes
  eff->setOffsetX(NDLAr_OnAxis_offset[0]);
  eff->setOffsetY(NDLAr_OnAxis_offset[1]);
  eff->setOffsetZ(NDLAr_OnAxis_offset[2]);


  //
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // Add hist of veto E
  TH1F *hist_vetoEnergyFD = new TH1F("hist_vetoEnergyFD", "hist_vetoEnergyFD", 150, 0, 1500);

  //
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // Add output txt file
  ofstream myfile;
   myfile.open ("Output_FDGeoEff_DataCheck.txt");

  //
  // Calculate FD eff
  double FD_eff = 0.;
  int FD_vetocut_counter = 0;
  int FD_FV_counter = 0;

  //------------------------------------------------------------------------------
  // Loop over FD events
  //
  nentries = t->GetEntries();
  std::cout << "Tot evts: " << nentries << std::endl;
  if (myfileVerbose) myfile << "Tot evts: " << nentries << "\n";
  if (throwfileVerbose) myfile << "Tot evts: " << nentries << "\n";
  if (hadronhitVerbose) myfile << "Tot evts: " << nentries << "\n";
  for ( int ientry = 0; ientry < nentries; ientry++ )
  //for ( int ientry = 55; ientry < 56; ientry++ ) // Use for drwaing one hardronic hits plots
  {

    t->GetEntry(ientry);
    //if(FD_Gen_numu_E > 6 && FD_Gen_numu_E<15)
    {

    if ( ientry%10000 == 0 )
    {
      std::cout << "Looking at entry " << ientry << ", FD_run: " << FD_Run << ", FD_subrun: " << FD_SubRun << ", FD_event: " << FD_Event << std::endl;
      if (myfileVerbose) myfile << "Looking at entry " << ientry << ", FD_run: " << FD_Run << ", FD_subrun: " << FD_SubRun << ", FD_event: " << FD_Event << "\n\n\n";
      if (throwfileVerbose) myfile << "Looking at entry " << ientry << ", FD_run: " << FD_Run << ", FD_subrun: " << FD_SubRun << ", FD_event: " << FD_Event << "\n\n\n";
    }
    if (myfileVerbose) myfile << "\n ientry: " << ientry <<"\n\n";
    if (throwfileVerbose) myfile << "\n ientry: " << ientry <<"\n\n";
    if (hadronhitVerbose) myfile << "\n ientry: " << ientry <<"\n\n";


    //
    // Skip events without muon/hadronic deposits
    //
    if ( FD_Sim_nMu == 0 || FD_Sim_n_hadronic_Edep_b == 0 ) continue;
    if (myfileVerbose) myfile << "FD_Sim_n_hadronic_Edep_b: " << FD_Sim_n_hadronic_Edep_b <<"\n";
    if (throwfileVerbose) myfile << "FD_Sim_n_hadronic_Edep_b: " << FD_Sim_n_hadronic_Edep_b <<"\n";
    if ( FD_CCNC_truth == 1) continue;   // only use CC events
    // if ( FD_neuPDG != 14 ) continue;       // only use muon nu as interacting nu for FHC mode
    if ( FD_LepPDG != 13 ) continue;       // only use muon as final state lep
    // Only pick the events' vertex inside the FD FV
    if(FD_Sim_mu_start_vx > FD_FV_max[0] || FD_Sim_mu_start_vx < FD_FV_min[0] || FD_Sim_mu_start_vy > FD_FV_max[1] || FD_Sim_mu_start_vy < FD_FV_min[1] || FD_Sim_mu_start_vz > FD_FV_max[2] || FD_Sim_mu_start_vz < FD_FV_min[2]) continue;
    FD_FV_counter++;
    //
    // Calculate total hadron E in FD veto region
    //
    vetoEnergyFD = 0.;
    // Loop over hadron E deposits
    for ( int ihadronhit = 0; ihadronhit < FD_Sim_n_hadronic_Edep_b; ihadronhit++ ){

      // Veto region size: 30 cm from the active volume
      if ( ( FD_Sim_hadronic_hit_x_b->at(ihadronhit) > FDActiveVol_min[0] && FD_Sim_hadronic_hit_x_b->at(ihadronhit) < FDActiveVol_min[0] + 30 ) ||
           ( FD_Sim_hadronic_hit_y_b->at(ihadronhit) > FDActiveVol_min[1] && FD_Sim_hadronic_hit_y_b->at(ihadronhit) < FDActiveVol_min[1] + 30 ) ||
           ( FD_Sim_hadronic_hit_z_b->at(ihadronhit) > FDActiveVol_min[2] && FD_Sim_hadronic_hit_z_b->at(ihadronhit) < FDActiveVol_min[2] + 30 ) ||
           ( FD_Sim_hadronic_hit_x_b->at(ihadronhit) > FDActiveVol_max[0] - 30 && FD_Sim_hadronic_hit_x_b->at(ihadronhit) < FDActiveVol_max[0] ) ||
           ( FD_Sim_hadronic_hit_y_b->at(ihadronhit) > FDActiveVol_max[1] - 30 && FD_Sim_hadronic_hit_y_b->at(ihadronhit) < FDActiveVol_max[1] ) ||
           ( FD_Sim_hadronic_hit_z_b->at(ihadronhit) > FDActiveVol_max[2] - 30 && FD_Sim_hadronic_hit_z_b->at(ihadronhit) < FDActiveVol_max[2] )
         ){
           vetoEnergyFD += FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit);
      } // end if hadron deposit in FD veto region

    } // end loop over hadron E deposits
    //add a vetoEnergyFD histogram in the root file

    hist_vetoEnergyFD->Fill(vetoEnergyFD);

    //
    // Skip FD event if the total hadron E in veto region exceeds vetoEnergy [MeV]
    //
    if (throwfileVerbose) myfile << "vetoEnergyFD[MeV]: " << vetoEnergyFD <<"\n\n";
    // if ( vetoEnergyFD > 30 ) continue; // 30 MeV
    FD_vetocut_counter++;
    //
    // Renew throws every 100th (iwritten % 100 == 0)written event to save file size, i.e., if N = 128,
    // for written evt 0-99:    same 128 transformations for each event,
    // for written evt 100-199: same but renewed 128 transformations for each evt
    // so on so forth...
    // These transformations will be applied to leptons in the event, so need to keep track of iwritten
    //
    // Now change to renew throws every one written event
    if ( iwritten % 100 == 0 ) {

      // Produce N throws defined at setNthrows(N)
      // Same throws applied for hadron below
      eff->throwTransforms(); // Does not depend on evt vtx

      throwVtxY.clear();
      throwVtxZ.clear();
      throwRot.clear();

      throwVtxY = eff->getCurrentThrowTranslationsY();
      throwVtxZ = eff->getCurrentThrowTranslationsZ();
      throwRot  = eff->getCurrentThrowRotations();
      ThrowsFD->Fill();
    }

    //
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //
    // 1. FD to ND: after earth curvature rotation
    // Rotation affects mu start/end position and hadron positions below
    // Mu_start_v
    double FD_Sim_mu_start_v[3] = {FD_Sim_mu_start_vx, FD_Sim_mu_start_vy, FD_Sim_mu_start_vz};
    for(int i=0; i<3; i++)
    {
      ND_RandomVtx_Sim_mu_start_v[i] = eff->getEarthCurvature(FD_Sim_mu_start_v, beamLineRotation, i);
    }
    if (myfileVerbose) {
      myfile << "FD_Sim_mu_start_vx[cm]: " << FD_Sim_mu_start_vx << "\n";
      myfile << "FD_Sim_mu_start_vy[cm]: " << FD_Sim_mu_start_vy << "\n";
      myfile << "FD_Sim_mu_start_vz[cm]: " << FD_Sim_mu_start_vz << "\n\n";
      myfile << "ND_RandomVtx_Sim_mu_start_vx[cm]: " << ND_RandomVtx_Sim_mu_start_v[0] << "\n";
      myfile << "ND_RandomVtx_Sim_mu_start_vy[cm]: " << ND_RandomVtx_Sim_mu_start_v[1] << "\n";
      myfile << "ND_RandomVtx_Sim_mu_start_vz[cm]: " << ND_RandomVtx_Sim_mu_start_v[2] << "\n\n";

    }

    // Mu_end_v
    double FD_Sim_mu_end_v[3] = {FD_Sim_mu_end_vx, FD_Sim_mu_end_vy, FD_Sim_mu_end_vz};
    for(int i=0; i<3; i++)
    {
      ND_RandomVtx_Sim_mu_end_v[i] = eff->getEarthCurvature(FD_Sim_mu_end_v, beamLineRotation, i);
    }
    double FD_Sim_mu_v_end_start = eff->getDistance(FD_Sim_mu_start_v,FD_Sim_mu_end_v);
    double ND_RandomVtx_Sim_mu_v_end_start = eff->getDistance(ND_RandomVtx_Sim_mu_start_v,ND_RandomVtx_Sim_mu_end_v);
    if (myfileVerbose)
    {
      myfile << "FD_Sim_mu_end_vx[cm]: " << FD_Sim_mu_end_vx << "\n";
      myfile << "FD_Sim_mu_end_vy[cm]: " << FD_Sim_mu_end_vy << "\n";
      myfile << "FD_Sim_mu_end_vz[cm]: " << FD_Sim_mu_end_vz << "\n";
      myfile << "Distance between FD_Sim_mu_v_end_start[cm]: " << FD_Sim_mu_v_end_start << "\n\n";
      myfile << "ND_RandomVtx_Sim_mu_end_vx[cm]: " << ND_RandomVtx_Sim_mu_end_v[0] << "\n";
      myfile << "ND_RandomVtx_Sim_mu_end_vy[cm]: " << ND_RandomVtx_Sim_mu_end_v[1] << "\n";
      myfile << "ND_RandomVtx_Sim_mu_end_vz[cm]: " << ND_RandomVtx_Sim_mu_end_v[2] << "\n";
      myfile << "Distance between ND_RandomVtx_Sim_mu_v_end_start[cm]: " << ND_RandomVtx_Sim_mu_v_end_start << "\n\n";
    }


    // Mu_start_p
    double FD_Sim_mu_start_p[3] = {FD_Sim_mu_start_px, FD_Sim_mu_start_py, FD_Sim_mu_start_pz};
    for(int i=0; i<3; i++)
    {
      ND_RandomVtx_Sim_mu_start_p[i] = eff->getEarthCurvature(FD_Sim_mu_start_p, beamLineRotation, i);
    }
    ND_RandomVtx_Sim_mu_start_p_total = eff->getTotalMomentum(ND_RandomVtx_Sim_mu_start_p);
    double FD_Sim_mu_start_p_total = eff->getTotalMomentum(FD_Sim_mu_start_p);
    if (myfileVerbose) {
      myfile << "FD_Sim_mu_start_px[cm]: " << FD_Sim_mu_start_px << "\n";
      myfile << "FD_Sim_mu_start_py[cm]: " << FD_Sim_mu_start_py << "\n";
      myfile << "FD_Sim_mu_start_pz[cm]: " << FD_Sim_mu_start_pz << "\n";
      myfile << "FD_Sim_mu_start_p_total[GeV]: " << FD_Sim_mu_start_p_total << "\n\n";
      myfile << "ND_RandomVtx_Sim_mu_start_px[GeV]: " << ND_RandomVtx_Sim_mu_start_p[0] << "\n";
      myfile << "ND_RandomVtx_Sim_mu_start_py[GeV]: " << ND_RandomVtx_Sim_mu_start_p[1] << "\n";
      myfile << "ND_RandomVtx_Sim_mu_start_pz[GeV]: " << ND_RandomVtx_Sim_mu_start_p[2] << "\n";
      myfile << "ND_RandomVtx_Sim_mu_start_p_total[GeV]: " << ND_RandomVtx_Sim_mu_start_p_total << "\n\n";
    }


    // Branches that are not affected by ND off axis position and vtx x (loops below)
    ND_Gen_numu_E = FD_Gen_numu_E;
    TotalSimP = FD_Sim_nP;
    TotalSimChargedPions = FD_Sim_nPionCharged;
    TotalSimNeutralPions = FD_Sim_nPionNeutral;
    ND_E_vis_true = FD_E_vis_true;
    ND_LepNuAngle = FD_LepNuAngle;
    ND_W = FD_W;
    ND_Sim_hadronic_Edep_b2 = FD_Sim_hadronic_Edep_b2;
    totEnergyFDatFD_float = 0.;
    //IOANA: get the total hadron energy at FD without any trim
    for ( int ihadronhit = 0; ihadronhit < FD_Sim_n_hadronic_Edep_b; ihadronhit++ )
      totEnergyFDatFD_float +=FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit);


    for ( int ihadronhit = 0; ihadronhit < FD_Sim_n_hadronic_Edep_b; ihadronhit++ ){
      double FD_Sim_hadronic_hit[3] = {FD_Sim_hadronic_hit_x_b->at(ihadronhit),FD_Sim_hadronic_hit_y_b->at(ihadronhit), FD_Sim_hadronic_hit_z_b->at(ihadronhit)};
      for (int i =0; i<3;i++)
      {
        ND_RandomVtx_Sim_hadronic_hit_xyz.emplace_back((float)eff->getEarthCurvature(FD_Sim_hadronic_hit, beamLineRotation, i));
      }
      ND_RandomVtx_Sim_hadronic_hit.emplace_back(ND_RandomVtx_Sim_hadronic_hit_xyz);
      ND_RandomVtx_Sim_hadronic_hit_xyz.clear();
    }
    //
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //
    // 2. ND: move back to the beam center
    // ND_OnAxis_Sim_mu_end_v
    // double geoEff::getTranslations(double v_bf[3], double vtx_bf[3], double vtx_af[3], int dim)
    for(int i=0; i<3; i++)
    {
      ND_OnAxis_Sim_mu_end_v[i] = eff->getTranslations(ND_RandomVtx_Sim_mu_end_v, ND_RandomVtx_Sim_mu_start_v, ND_OnAxis_Sim_mu_start_v, i);
    }
    double ND_OnAxis_Sim_mu_v_end_start = eff->getDistance(ND_OnAxis_Sim_mu_start_v,ND_OnAxis_Sim_mu_end_v);

    if (myfileVerbose)
    {
      myfile << "ND_OnAxis_Sim_mu_start_vx[cm]: " << ND_OnAxis_Sim_mu_start_v[0] << "\n";
      myfile << "ND_OnAxis_Sim_mu_start_vy[cm]: " << ND_OnAxis_Sim_mu_start_v[1] << "\n";
      myfile << "ND_OnAxis_Sim_mu_start_vz[cm]: " << ND_OnAxis_Sim_mu_start_v[2] << "\n\n";
      myfile << "ND_OnAxis_Sim_mu_end_vx[cm]: " << ND_OnAxis_Sim_mu_end_v[0] << "\n";
      myfile << "ND_OnAxis_Sim_mu_end_vy[cm]: " << ND_OnAxis_Sim_mu_end_v[1] << "\n";
      myfile << "ND_OnAxis_Sim_mu_end_vz[cm]: " << ND_OnAxis_Sim_mu_end_v[2] << "\n";
      myfile << "Distance between ND_OnAxis_Sim_mu_v_end_start[cm]: " << ND_OnAxis_Sim_mu_v_end_start << "\n\n";

    }
    // ND_OnAxis_Sim_mu_start_p
    for(int i=0; i<3; i++)
    {
      ND_OnAxis_Sim_mu_start_p[i] = eff->RemainUnchanged(ND_RandomVtx_Sim_mu_start_p[i]);
    }
    ND_OnAxis_Sim_mu_start_p_total = eff->getTotalMomentum(ND_OnAxis_Sim_mu_start_p);
    if (myfileVerbose)
    {
      myfile << "ND_OnAxis_Sim_mu_start_px[cm]: " << ND_OnAxis_Sim_mu_start_p[0] << "\n";
      myfile << "ND_OnAxis_Sim_mu_start_py[cm]: " << ND_OnAxis_Sim_mu_start_p[1] << "\n";
      myfile << "ND_OnAxis_Sim_mu_start_pz[cm]: " << ND_OnAxis_Sim_mu_start_p[2] << "\n";
      myfile << "ND_OnAxis_Sim_mu_start_p_total[GeV]: " << ND_OnAxis_Sim_mu_start_p_total << "\n\n";
    }

    // ND_OnAxis_Sim_hadronic_hit
    for ( int ihadronhit = 0; ihadronhit < FD_Sim_n_hadronic_Edep_b; ihadronhit++ ){
      double ND_RandomVtx_Sim_hadronic_hit_array[3] = {ND_RandomVtx_Sim_hadronic_hit[ihadronhit][0],ND_RandomVtx_Sim_hadronic_hit[ihadronhit][1],ND_RandomVtx_Sim_hadronic_hit[ihadronhit][2]};
      for (int i =0; i<3;i++)
      {
        ND_OnAxis_Sim_hadronic_hit_xyz.emplace_back((float)eff->getTranslations(ND_RandomVtx_Sim_hadronic_hit_array, ND_RandomVtx_Sim_mu_start_v, ND_OnAxis_Sim_mu_start_v, i));
      }
      ND_OnAxis_Sim_hadronic_hit.emplace_back(ND_OnAxis_Sim_hadronic_hit_xyz);
      ND_OnAxis_Sim_hadronic_hit_xyz.clear();
    }

    // Set On-Axis vertex where center beam cross
    eff->setOnAxisVertex(ND_OnAxis_Sim_mu_start_v[0],ND_OnAxis_Sim_mu_start_v[1],ND_OnAxis_Sim_mu_start_v[2]);
    //
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //
    // Loop over ND_LAr_dtctr_pos_vec: random off_axis_pos or every ND_Lar_dtctr_pos_new_stepsize
    // Don't put it outside event loop to avoid looping over all events multiple times
    //


    int ND_off_axis_pos_counter = 0;


    for ( double i_ND_off_axis_pos : ND_LAr_dtctr_pos_vec )
    {
      // Calculate meanEff
      Int_t MiddleEff_counter = 0;
      Double_t MiddleEff = 0.;
      Double_t Leff = 0.;
      Double_t Reff = 0.;
      Int_t Leff_counter = 0;
      Int_t Reff_counter = 0;


      // eff->setOffsetX(NDLAr_OnAxis_offset[0]-i_ND_off_axis_pos);
      ND_off_axis_pos_counter++;
      //
      // Loop over vtx x: random x or stepwise increased x
      // Don't put it outside event loop to avoid looping over all events multiple times
      //
      //------------------------------------------------------------------------------
      //------------------------------------------------------------------------------
      //------------------------------------------------------------------------------
      //
      int vtx_vx_counter = 0;

      for ( double i_vtx_vx : ND_vtx_vx_vec )
      {
        // Interpolate event neutrino production point (beam coordinate, units in meter)
        decayZbeamCoord = gDecayZ->Eval( (i_ND_off_axis_pos + i_vtx_vx)*0.01 - detRefBeamCoord[0] );

        // Calculate neutrino production point in detector coordinate
        decayYdetCoord = beamRefDetCoord[1] - detRefBeamCoord[1]*cos(beamLineRotation) + ( decayZbeamCoord - detRefBeamCoord[2] )*sin(beamLineRotation);
        decayZdetCoord = beamRefDetCoord[2] + detRefBeamCoord[1]*sin(beamLineRotation) + ( decayZbeamCoord - detRefBeamCoord[2] )*cos(beamLineRotation);
        // Set production point in unit: cm
        eff->setDecayPos(decayXdetCoord*100., decayYdetCoord*100., decayZdetCoord*100.);
        if (myfileVerbose)
        {
          myfile << "ND Off-Axis pos #" << ND_off_axis_pos_counter << "[cm]: " << i_ND_off_axis_pos << "\n";
          myfile << "ND LAr pos #" << vtx_vx_counter << "[cm]: " << i_vtx_vx << "\n";
          myfile << "Event position #" << vtx_vx_counter << "[cm]: " << i_ND_off_axis_pos + i_vtx_vx << "\n";
          myfile << "Decay Pos [cm]: " << "{" << eff->getDecayPos(0) << ", " << eff->getDecayPos(1) << ", " << eff->getDecayPos(2) << "}" << "\n\n";
        }
        if (hadronhitVerbose)
        {
          myfile << "ND Off-Axis pos #" << ND_off_axis_pos_counter << "[cm]: " << i_ND_off_axis_pos << "\n";
          myfile << "ND LAr pos #" << vtx_vx_counter << "[cm]: " << i_vtx_vx << "\n";
          myfile << "Event position #" << vtx_vx_counter << "[cm]: " << i_ND_off_axis_pos + i_vtx_vx << "\n";
          myfile << "Decay Pos [cm]: " << "{" << eff->getDecayPos(0) << ", " << eff->getDecayPos(1) << ", " << eff->getDecayPos(2) << "}" << "\n\n";
        }

        //
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        //
        // 3. ND to ND: translate from OnAxis to OffAxis
        //
        // ND_OffAxis_Unrotated_Sim_mu_start_v
        ND_OffAxis_Unrotated_Sim_mu_start_v[0] = ND_OnAxis_Sim_mu_start_v[0] + i_ND_off_axis_pos + i_vtx_vx;
        ND_OffAxis_Unrotated_Sim_mu_start_v[1] = eff->RemainUnchanged(ND_OnAxis_Sim_mu_start_v[1]);
        ND_OffAxis_Unrotated_Sim_mu_start_v[2] = eff->RemainUnchanged(ND_OnAxis_Sim_mu_start_v[2]);
        if (myfileVerbose)
        {
          myfile << "ND_OffAxis_Unrotated_Sim_mu_start_vx[cm]: " << ND_OffAxis_Unrotated_Sim_mu_start_v[0] << "\n";
          myfile << "ND_OffAxis_Unrotated_Sim_mu_start_vy[cm]: " << ND_OffAxis_Unrotated_Sim_mu_start_v[1] << "\n";
          myfile << "ND_OffAxis_Unrotated_Sim_mu_start_vz[cm]: " << ND_OffAxis_Unrotated_Sim_mu_start_v[2] << "\n\n";
        }
        ND_OffAxis_Unrotated_Sim_mu_start_v_xyz.emplace_back(ND_OffAxis_Unrotated_Sim_mu_start_v[0]);
        ND_OffAxis_Unrotated_Sim_mu_start_v_xyz.emplace_back(ND_OffAxis_Unrotated_Sim_mu_start_v[1]);
        ND_OffAxis_Unrotated_Sim_mu_start_v_xyz.emplace_back(ND_OffAxis_Unrotated_Sim_mu_start_v[2]);
        ND_OffAxis_Unrotated_Sim_mu_start_v_xyz_vtx.emplace_back(ND_OffAxis_Unrotated_Sim_mu_start_v_xyz);
        ND_OffAxis_Unrotated_Sim_mu_start_v_xyz.clear();

        // ND_OffAxis_Unrotated_Sim_mu_end_v
        // double geoEff::getTranslations(double v_bf[3], double vtx_bf[3], double vtx_af[3], int dim)
        for(int i=0; i<3; i++)
        {
          ND_OffAxis_Unrotated_Sim_mu_end_v[i] = eff->getTranslations(ND_OnAxis_Sim_mu_end_v, ND_OnAxis_Sim_mu_start_v, ND_OffAxis_Unrotated_Sim_mu_start_v, i);
        }
        double ND_OffAxis_Unrotated_Sim_mu_v_end_start = eff->getDistance(ND_OffAxis_Unrotated_Sim_mu_end_v,ND_OffAxis_Unrotated_Sim_mu_start_v);
        if (myfileVerbose)
        {
          myfile << "ND_OffAxis_Unrotated_Sim_mu_end_vx[cm]: " << ND_OffAxis_Unrotated_Sim_mu_end_v[0] << "\n";
          myfile << "ND_OffAxis_Unrotated_Sim_mu_end_vy[cm]: " << ND_OffAxis_Unrotated_Sim_mu_end_v[1] << "\n";
          myfile << "ND_OffAxis_Unrotated_Sim_mu_end_vz[cm]: " << ND_OffAxis_Unrotated_Sim_mu_end_v[2] << "\n";
          myfile << "Distance between ND_OffAxis_Unrotated_Sim_mu_v_end_start[cm]: " << ND_OffAxis_Unrotated_Sim_mu_v_end_start << "\n\n";
        }
        ND_OffAxis_Unrotated_Sim_mu_end_v_xyz.emplace_back(ND_OffAxis_Unrotated_Sim_mu_end_v[0]);
        ND_OffAxis_Unrotated_Sim_mu_end_v_xyz.emplace_back(ND_OffAxis_Unrotated_Sim_mu_end_v[1]);
        ND_OffAxis_Unrotated_Sim_mu_end_v_xyz.emplace_back(ND_OffAxis_Unrotated_Sim_mu_end_v[2]);
        ND_OffAxis_Unrotated_Sim_mu_end_v_xyz_vtx.emplace_back(ND_OffAxis_Unrotated_Sim_mu_end_v_xyz);
        ND_OffAxis_Unrotated_Sim_mu_end_v_xyz.clear();

        // ND_OffAxis_Unrotated_Sim_mu_start_p
        for(int i=0; i<3; i++)
        {
          ND_OffAxis_Unrotated_Sim_mu_start_p[i] = eff->RemainUnchanged(ND_OnAxis_Sim_mu_start_p[i]);
        }
        ND_OffAxis_Unrotated_Sim_mu_start_p_total = eff->getTotalMomentum(ND_OffAxis_Unrotated_Sim_mu_start_p);
        if (myfileVerbose)
        {
          myfile << "ND_OffAxis_Unrotated_Sim_mu_start_px[cm]: " << ND_OffAxis_Unrotated_Sim_mu_start_p[0] << "\n";
          myfile << "ND_OffAxis_Unrotated_Sim_mu_start_py[cm]: " << ND_OffAxis_Unrotated_Sim_mu_start_p[1] << "\n";
          myfile << "ND_OffAxis_Unrotated_Sim_mu_start_pz[cm]: " << ND_OffAxis_Unrotated_Sim_mu_start_p[2] << "\n";
          myfile << "ND_OffAxis_Unrotated_Sim_mu_start_p_total[GeV]: " << ND_OffAxis_Unrotated_Sim_mu_start_p_total << "\n\n";
        }
        ND_OffAxis_Unrotated_Sim_mu_start_p_xyz.emplace_back(ND_OffAxis_Unrotated_Sim_mu_start_p[0]);
        ND_OffAxis_Unrotated_Sim_mu_start_p_xyz.emplace_back(ND_OffAxis_Unrotated_Sim_mu_start_p[1]);
        ND_OffAxis_Unrotated_Sim_mu_start_p_xyz.emplace_back(ND_OffAxis_Unrotated_Sim_mu_start_p[2]);


        // ND_OffAxis_Unrotated_Sim_hadronic_hit
        for ( int ihadronhit = 0; ihadronhit < FD_Sim_n_hadronic_Edep_b; ihadronhit++ ){
          double ND_OnAxis_Sim_hadronic_hit_array[3] = {ND_OnAxis_Sim_hadronic_hit[ihadronhit][0],ND_OnAxis_Sim_hadronic_hit[ihadronhit][1],ND_OnAxis_Sim_hadronic_hit[ihadronhit][2]};
          for (int i =0; i<3;i++)
          {
            ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz.emplace_back((float)eff->getTranslations(ND_OnAxis_Sim_hadronic_hit_array, ND_OnAxis_Sim_mu_start_v, ND_OffAxis_Unrotated_Sim_mu_start_v, i));
          }
          ND_OffAxis_Unrotated_Sim_hadronic_hit.emplace_back(ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz);
          ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz.clear();
        }
        ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz_vtx.emplace_back(ND_OffAxis_Unrotated_Sim_hadronic_hit);
        //
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        //
        // 4. ND: get after eigen rotated vectors for step 3
        // ND_OffAxis_Sim_mu_start_v
        ND_OffAxis_Sim_mu_start_v[0] = eff->RemainUnchanged(ND_OffAxis_Unrotated_Sim_mu_start_v[0]);
        ND_OffAxis_Sim_mu_start_v_xyz.emplace_back(ND_OffAxis_Sim_mu_start_v[0]);
        ND_OffAxis_Sim_mu_start_v[1] = eff->RemainUnchanged(ND_OffAxis_Unrotated_Sim_mu_start_v[1]);
        ND_OffAxis_Sim_mu_start_v_xyz.emplace_back(ND_OffAxis_Sim_mu_start_v[1]);
        ND_OffAxis_Sim_mu_start_v[2] = eff->RemainUnchanged(ND_OffAxis_Unrotated_Sim_mu_start_v[2]);
        ND_OffAxis_Sim_mu_start_v_xyz.emplace_back(ND_OffAxis_Sim_mu_start_v[2]);

        ND_OffAxis_Sim_mu_start_v_xyz_vtx.emplace_back(ND_OffAxis_Sim_mu_start_v_xyz); //emplace_back for each vtx pos within the ND_LAr
        ND_OffAxis_Sim_mu_start_v_xyz.clear(); // empty the vector

        if (myfileVerbose)
        {
          myfile << "ND_OffAxis_Sim_mu_start_v[cm]: " << ND_OffAxis_Sim_mu_start_v[0] << "\n";
          myfile << "ND_OffAxis_Sim_mu_start_v[cm]: " << ND_OffAxis_Sim_mu_start_v[1] << "\n";
          myfile << "ND_OffAxis_Sim_mu_start_v[cm]: " << ND_OffAxis_Sim_mu_start_v[2] << "\n\n";
        }

        // setVertex
        eff->setOffAxisVertex(ND_OffAxis_Sim_mu_start_v[0], ND_OffAxis_Sim_mu_start_v[1], ND_OffAxis_Sim_mu_start_v[2]);
        // ND_OffAxis_Sim_mu_end_v
        eff->setMuEndV(ND_OffAxis_Unrotated_Sim_mu_end_v[0],ND_OffAxis_Unrotated_Sim_mu_end_v[1],ND_OffAxis_Unrotated_Sim_mu_end_v[2]);
        for(int i=0; i<3; i++)
        {
          ND_OffAxis_Sim_mu_end_v[i] = eff->getOffAxisMuEndV(i);
        }
        double ND_OffAxis_Sim_mu_v_end_start = eff->getDistance(ND_OffAxis_Sim_mu_end_v,ND_OffAxis_Sim_mu_start_v);
        if (myfileVerbose)
        {
          myfile << "ND_OffAxis_Sim_mu_end_v[cm]: " << ND_OffAxis_Sim_mu_end_v[0] << "\n";
          myfile << "ND_OffAxis_Sim_mu_end_v[cm]: " << ND_OffAxis_Sim_mu_end_v[1] << "\n";
          myfile << "ND_OffAxis_Sim_mu_end_v[cm]: " << ND_OffAxis_Sim_mu_end_v[2] << "\n";
          myfile << "Distance between ND_OffAxis_Sim_mu_v_end_start[cm]: " << ND_OffAxis_Sim_mu_v_end_start << "\n\n";
        }
        ND_OffAxis_Sim_mu_end_v_xyz.emplace_back(ND_OffAxis_Sim_mu_end_v[0]);
        ND_OffAxis_Sim_mu_end_v_xyz.emplace_back(ND_OffAxis_Sim_mu_end_v[1]);
        ND_OffAxis_Sim_mu_end_v_xyz.emplace_back(ND_OffAxis_Sim_mu_end_v[2]);
        ND_OffAxis_Sim_mu_end_v_xyz_vtx.emplace_back(ND_OffAxis_Sim_mu_end_v_xyz);
        ND_OffAxis_Sim_mu_end_v_xyz.clear();

        // ND_OffAxis_Sim_mu_start_p
        eff->setMuStartP(ND_OffAxis_Unrotated_Sim_mu_start_p[0],ND_OffAxis_Unrotated_Sim_mu_start_p[1],ND_OffAxis_Unrotated_Sim_mu_start_p[2]);
        for(int i=0; i<3; i++)
        {
          ND_OffAxis_Sim_mu_start_p[i] = eff->getOffAxisMuStartP(i);
        }
        ND_OffAxis_Sim_mu_start_p_total = eff->getTotalMomentum(ND_OffAxis_Sim_mu_start_p);
        if (myfileVerbose)
        {
          myfile << "ND_OffAxis_Sim_mu_start_px[cm]: " << ND_OffAxis_Sim_mu_start_p[0] << "\n";
          myfile << "ND_OffAxis_Sim_mu_start_py[cm]: " << ND_OffAxis_Sim_mu_start_p[1] << "\n";
          myfile << "ND_OffAxis_Sim_mu_start_pz[cm]: " << ND_OffAxis_Sim_mu_start_p[2] << "\n";
          myfile << "ND_OffAxis_Sim_mu_start_p_total[GeV]: " << ND_OffAxis_Sim_mu_start_p_total << "\n\n";
        }
        ND_OffAxis_Sim_mu_start_p_xyz.emplace_back(ND_OffAxis_Sim_mu_start_p[0]);
        ND_OffAxis_Sim_mu_start_p_xyz.emplace_back(ND_OffAxis_Sim_mu_start_p[1]);
        ND_OffAxis_Sim_mu_start_p_xyz.emplace_back(ND_OffAxis_Sim_mu_start_p[2]);
        ND_OffAxis_Sim_mu_start_p_xyz_vtx.emplace_back(ND_OffAxis_Sim_mu_start_p_xyz);
        ND_OffAxis_Sim_mu_start_p_xyz.clear();

        // ND_OffAxis_Sim_mu_start_E
        ND_OffAxis_Sim_mu_start_E = FD_Sim_mu_start_E; // Energy of leading mu at start point [GeV]
        ND_OffAxis_Sim_mu_start_E_xyz.emplace_back(ND_OffAxis_Sim_mu_start_E);
        ND_OffAxis_Sim_mu_start_E_xyz_vtx.emplace_back(ND_OffAxis_Sim_mu_start_E_xyz);
        ND_OffAxis_Sim_mu_start_E_xyz.clear();

        vetoEnergyFDatND_float = 0.;
        totEnergyFDatND_float = 0.;
        outEnergyFDatND_float = 0.;
        muonEdep = FD_Sim_mu_Edep_b2; //Deposited Energy of muon [MeV]
        muonTrackLength = FD_Sim_mu_track_length;
   
        /*if (FD_Sim_mu_Edep_b2/FD_Sim_mu_track_length > 3. || FD_Sim_mu_track_length < 100.){
           std::cout<<" **** this event is going to be cut..!! "<<" dE/dX = "<< FD_Sim_mu_Edep_b2/FD_Sim_mu_track_length << " track length = "<<FD_Sim_mu_track_length<<std:: endl;
        }*/
        // ND_OffAxis_Sim_hadronic_hit
        for ( int ihadronhit = 0; ihadronhit < FD_Sim_n_hadronic_Edep_b; ihadronhit++ )
        {
          eff->setHadronHitV(ND_OffAxis_Unrotated_Sim_hadronic_hit[ihadronhit][0],ND_OffAxis_Unrotated_Sim_hadronic_hit[ihadronhit][1],ND_OffAxis_Unrotated_Sim_hadronic_hit[ihadronhit][2]);
          for (int i =0; i<3;i++)
          {
            ND_OffAxis_Sim_hadronic_hit_xyz.emplace_back(eff->getOffAxisHadronHitV(i));
          }
          ND_OffAxis_Sim_hadronic_hit.emplace_back(ND_OffAxis_Sim_hadronic_hit_xyz);

          totEnergyFDatND_float += FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit);
          // cout << "pos: " << ND_OffAxis_Sim_hadronic_hit[ihadronhit][0] - i_ND_off_axis_pos << ", " << ND_OffAxis_Sim_hadronic_hit[ihadronhit][1] - NDLAr_OnAxis_offset[1] << ", " << ND_OffAxis_Sim_hadronic_hit[ihadronhit][2] - NDLAr_OnAxis_offset[2] << endl;
          // Calculate vetoE of FD event at ND
          // Veto region size: 30 cm from the active volume

          if ( ( ND_OffAxis_Sim_hadronic_hit[ihadronhit][0] - i_ND_off_axis_pos > NDActiveVol_min[0] && ND_OffAxis_Sim_hadronic_hit[ihadronhit][0] - i_ND_off_axis_pos < NDActiveVol_min[0] + 30 ) ||
               ( ND_OffAxis_Sim_hadronic_hit[ihadronhit][1] - NDLAr_OnAxis_offset[1] > NDActiveVol_min[1] && ND_OffAxis_Sim_hadronic_hit[ihadronhit][1] - NDLAr_OnAxis_offset[1] < NDActiveVol_min[1] + 30 ) ||
               ( ND_OffAxis_Sim_hadronic_hit[ihadronhit][2] - NDLAr_OnAxis_offset[2] > NDActiveVol_min[2] && ND_OffAxis_Sim_hadronic_hit[ihadronhit][2] - NDLAr_OnAxis_offset[2] < NDActiveVol_min[2] + 30 ) ||
               ( ND_OffAxis_Sim_hadronic_hit[ihadronhit][0] - i_ND_off_axis_pos > NDActiveVol_max[0] - 30 && ND_OffAxis_Sim_hadronic_hit[ihadronhit][0] - i_ND_off_axis_pos < NDActiveVol_max[0] ) ||
               ( ND_OffAxis_Sim_hadronic_hit[ihadronhit][1] - NDLAr_OnAxis_offset[1] > NDActiveVol_max[1] - 30 && ND_OffAxis_Sim_hadronic_hit[ihadronhit][1] - NDLAr_OnAxis_offset[1] < NDActiveVol_max[1] ) ||
               ( ND_OffAxis_Sim_hadronic_hit[ihadronhit][2] - NDLAr_OnAxis_offset[2] > NDActiveVol_max[2] - 30 && ND_OffAxis_Sim_hadronic_hit[ihadronhit][2] - NDLAr_OnAxis_offset[2] < NDActiveVol_max[2] )
             ){
               vetoEnergyFDatND_float += FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit);
          } // end if hadron deposit in FD veto region
          else if
          (
           ( ND_OffAxis_Sim_hadronic_hit[ihadronhit][0]-i_ND_off_axis_pos             < NDActiveVol_min[0] ) ||
           ( ND_OffAxis_Sim_hadronic_hit[ihadronhit][1]-NDLAr_OnAxis_offset[1]        < NDActiveVol_min[1] ) ||
           ( ND_OffAxis_Sim_hadronic_hit[ihadronhit][2]-NDLAr_OnAxis_offset[2]        < NDActiveVol_min[2] ) ||
           ( ND_OffAxis_Sim_hadronic_hit[ihadronhit][0]-i_ND_off_axis_pos             > NDActiveVol_max[0] ) ||
           ( ND_OffAxis_Sim_hadronic_hit[ihadronhit][1]-NDLAr_OnAxis_offset[1]        > NDActiveVol_max[1] ) ||
           ( ND_OffAxis_Sim_hadronic_hit[ihadronhit][2]-NDLAr_OnAxis_offset[2]        > NDActiveVol_max[2] )
          )
          {
            outEnergyFDatND_float += FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit);
            // Trim all out energy
            // IOANA: temporary comment the =0
            //FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit) = 0;
          } // end if hadron deposit outside FD active region
          ND_OffAxis_Sim_hadronic_hit_xyz.clear();
        } // end hadron hit loop
        ND_OffAxis_Sim_hadronic_hit_xyz_vtx.emplace_back(ND_OffAxis_Sim_hadronic_hit);

        // if(vetoEnergyFDatND_float > 30)
        //cout << "i_ND_off_axis_pos: " << i_ND_off_axis_pos << ", i_vtx_vx: " << i_vtx_vx << ", Ev: " << ND_Gen_numu_E << ", W: "<< ND_W << endl;
        //cout << ",------------------------ totEnergyFDatND: " << totEnergyFDatND_float<<" FD_True_hadronic_E = "<< FD_True_HadE<<" mu energy: "<<FD_Sim_mu_start_E<<" vis E = "<<ND_E_vis_true<<" Enu = "<<ND_Gen_numu_E << ", vetoEnergyFDatND: " << vetoEnergyFDatND_float << ", outEnergyFDatND: " << outEnergyFDatND_float << endl;
        vetoEnergyFDatND_vtx.emplace_back(vetoEnergyFDatND_float);
        totEnergyFDatND_vtx.emplace_back(totEnergyFDatND_float);
        outEnergyFDatND_vtx.emplace_back(outEnergyFDatND_float);


        // Add hadron hits output
        if (myfileVerbose)
        {
          for (int ihadronhit = FD_Sim_n_hadronic_Edep_b-2; ihadronhit < FD_Sim_n_hadronic_Edep_b; ihadronhit++) {
            myfile << "Hit #" << ihadronhit <<"/" << FD_Sim_n_hadronic_Edep_b << "\n";
            myfile<<"FD_Sim_hadronic_hit_x[cm]: "<<FD_Sim_hadronic_hit_x_b->at(ihadronhit)<<"\n";
            myfile<<"FD_Sim_hadronic_hit_y[cm]: "<<FD_Sim_hadronic_hit_y_b->at(ihadronhit)<<"\n";
            myfile<<"FD_Sim_hadronic_hit_z[cm]: "<<FD_Sim_hadronic_hit_z_b->at(ihadronhit)<<"\n";
            double FD_Sim_hadronic_hit[3] = {FD_Sim_hadronic_hit_x_b->at(ihadronhit),FD_Sim_hadronic_hit_y_b->at(ihadronhit), FD_Sim_hadronic_hit_z_b->at(ihadronhit)};
            double FD_Sim_hadronic_hit_end_start = eff->getDistance(FD_Sim_hadronic_hit,FD_Sim_mu_start_v);
            myfile << "Distance between FD_Sim_hadronic_hit_end_start[cm]:" << FD_Sim_hadronic_hit_end_start <<"\n\n";

            for (int j = 0; j < 3; j++)
            {
              myfile << "ND_RandomVtx_Sim_hadronic_hit_["<<j<<"][cm]: "<< ND_RandomVtx_Sim_hadronic_hit[ihadronhit][j] << "\n";
            }
            double ND_RandomVtx_Sim_hadronic_hit_array[3] = {ND_RandomVtx_Sim_hadronic_hit[ihadronhit][0],ND_RandomVtx_Sim_hadronic_hit[ihadronhit][1],ND_RandomVtx_Sim_hadronic_hit[ihadronhit][2]};
            double ND_RandomVtx_Sim_hadronic_hit_end_start = eff->getDistance(ND_RandomVtx_Sim_hadronic_hit_array,ND_RandomVtx_Sim_mu_start_v);
            myfile << "Distance between ND_RandomVtx_Sim_hadronic_hit_end_start[cm]:" << ND_RandomVtx_Sim_hadronic_hit_end_start <<"\n\n";

            for (int j = 0; j < 3; j++)
            {
              myfile << "ND_OnAxis_Sim_hadronic_hit_["<<j<<"][cm]: "<< ND_OnAxis_Sim_hadronic_hit[ihadronhit][j] << "\n";
            }
            double ND_OnAxis_Sim_hadronic_hit_array[3] = {ND_OnAxis_Sim_hadronic_hit[ihadronhit][0],ND_OnAxis_Sim_hadronic_hit[ihadronhit][1],ND_OnAxis_Sim_hadronic_hit[ihadronhit][2]};
            double ND_OnAxis_Sim_hadronic_hit_end_start = eff->getDistance(ND_OnAxis_Sim_hadronic_hit_array,ND_OnAxis_Sim_mu_start_v);
            myfile << "Distance between ND_OnAxis_Sim_hadronic_hit_end_start[cm]:" << ND_OnAxis_Sim_hadronic_hit_end_start <<"\n\n";

            for (int j = 0; j < 3; j++)
            {
              myfile << "ND_OffAxis_Unrotated_Sim_hadronic_hit_["<<j<<"][cm]: "<< ND_OffAxis_Unrotated_Sim_hadronic_hit[ihadronhit][j] << "\n";
            }
            double ND_OffAxis_Unrotated_Sim_hadronic_hit_array[3] = {ND_OffAxis_Unrotated_Sim_hadronic_hit[ihadronhit][0],ND_OffAxis_Unrotated_Sim_hadronic_hit[ihadronhit][1],ND_OffAxis_Unrotated_Sim_hadronic_hit[ihadronhit][2]};
            double ND_OffAxis_Unrotated_Sim_hadronic_hit_end_start = eff->getDistance(ND_OffAxis_Unrotated_Sim_hadronic_hit_array,ND_OffAxis_Unrotated_Sim_mu_start_v);
            myfile << "Distance between ND_OffAxis_Unrotated_Sim_hadronic_hit_end_start[cm]:" << ND_OffAxis_Unrotated_Sim_hadronic_hit_end_start <<"\n\n";

            for (int j = 0; j < 3; j++)
            {
              myfile << "ND_OffAxis_Sim_hadronic_hit_["<<j<<"][cm]: "<< ND_OffAxis_Sim_hadronic_hit[ihadronhit][j] << "\n";
            }
            double ND_OffAxis_Sim_hadronic_hit_array[3] = {ND_OffAxis_Sim_hadronic_hit[ihadronhit][0],ND_OffAxis_Sim_hadronic_hit[ihadronhit][1],ND_OffAxis_Sim_hadronic_hit[ihadronhit][2]};
            double ND_OffAxis_Sim_hadronic_hit_end_start = eff->getDistance(ND_OffAxis_Sim_hadronic_hit_array,ND_OffAxis_Sim_mu_start_v);
            myfile << "Distance between ND_OffAxis_Sim_hadronic_hit_end_start[cm]:" << ND_OffAxis_Sim_hadronic_hit_end_start <<"\n\n";
          }
        }
        //
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        //
        // 5. ND: generate random throws
        // Hardon hit positions
        HadronHitEdeps.clear();
        HadronHitPoss.clear();
        HadronHitEdeps.reserve(FD_Sim_n_hadronic_Edep_b);
        HadronHitPoss.reserve(FD_Sim_n_hadronic_Edep_b*3);
        // Set HadronHitPoss
        for ( int ihadronhit = 0; ihadronhit < FD_Sim_n_hadronic_Edep_b; ihadronhit++ ){
          for (int i =0; i<3;i++)
          {
            HadronHitPoss.emplace_back(ND_OffAxis_Sim_hadronic_hit[ihadronhit][i]);
          }
          HadronHitEdeps.emplace_back( FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit) );
          if (throwfileVerbose) myfile << "HadronHitEdeps: " << FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit) << "\n";
        }

        eff->setVertex(ND_OffAxis_Sim_mu_start_v[0], ND_OffAxis_Sim_mu_start_v[1], ND_OffAxis_Sim_mu_start_v[2]);
        eff->setHitSegEdeps(HadronHitEdeps);
        eff->setHitSegPoss(HadronHitPoss); // this is converted hadrom deposit pos in ND coordinate sys.


        // Set offset between MC coordinate system and det volumes
        eff->setOffAxisOffsetX(i_ND_off_axis_pos);
        eff->setOffAxisOffsetY(NDLAr_OnAxis_offset[1]);
        eff->setOffAxisOffsetZ(NDLAr_OnAxis_offset[2]);
        // Get hadron containment result after everything is set to ND coordinate sys
        // Do random throws regardless whether FD evt is contained in ND volume by setting a false flag
        hadron_throw_result = eff->getHadronContainmentThrows_FD_GEC(false); // Every 64 throw results stored into a 64 bit unsigned int: 0101101...
        //cout<<" hadron_thrw_result size: "<<hadron_throw_result.size()<<endl;
        //std::cout<<" vtx x: "<<i_vtx_vx<<endl;
        vetoEThrow = eff->getVetoEPerThrow_FD_GEC();//(hadron_throw_result);
        trimEThrow = eff->getTrimEPerThrow_FD_GEC();


        if (throwfileVerbose) myfile << "i_ND_off_axis_pos: " << i_ND_off_axis_pos << " cm, vtx x #" << vtx_vx_counter << ": " << i_vtx_vx << " cm, throw result[0][0][0]: " << hadron_throw_result[0][0][0] << "\n";
        //if (verbose)
        //cout << "iwritten: " << iwritten << ", i_ND_off_axis_pos: " << i_ND_off_axis_pos << " cm, vtx x #" << vtx_vx_counter << ": " << i_vtx_vx << " cm, throw result[0][0][0]: " << hadron_throw_result[0][0][0] << "hadron_trhow result[0][0][4096]: "<<hadron_throw_result[0][0][4095]<<" veto E_0: "<<vetoEThrow[0][0][0]<<endl;//<<" veto E _4095 "<<vetoEThrow[0][0][4095]<<"\n";
        // Get the coordinates of hadron hits after eigen transformation, i is the # of throw


        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        //
        // 6. Calculate Geo Eff
        ThrowResults.clear();
        ThrowResults.reserve(64*64);
        VetoEnergyEventsPass.clear();
        VetoEnergyEventsPass.reserve(64*64);
        TrimEnergyEventsPass.clear();
        TrimEnergyEventsPass.reserve(64*64);
        TotalEnergyEventsPass.clear();
        TotalEnergyEventsPass.reserve(64*64);
        VtxYEventsPass.clear();
        VtxYEventsPass.reserve(64*64);
        VtxZEventsPass.clear();
        VtxZEventsPass.reserve(64*64);

        for ( vector<vector<vector<uint64_t> > >::iterator it_veto_size = hadron_throw_result.begin(); it_veto_size != hadron_throw_result.end(); ++it_veto_size )
        {
          for ( vector<vector<uint64_t> >::iterator it_veto_energy = it_veto_size->begin(); it_veto_energy != it_veto_size->end(); ++it_veto_energy )
          {

            // Every 64 throw result is a chunk
            // current test case: each evt has 128 throws, so 2 chunks

            int counter5    = 0;
            int validthrows = 0; // count no. of throws that meet ND FV cut for this evt
            int hadronpass  = 0; // count no. of throws that meet hadron containment cut
            double hadron_contain_eff = 0.; // initialize eff to zero

            for ( vector<uint64_t>::iterator it_chunk = it_veto_energy->begin(); it_chunk != it_veto_energy->end(); ++it_chunk )
            {
              counter5++;

              //if (verbose)
              //cout << "i_ND_off_axis_pos: " << i_ND_off_axis_pos << " cm, vtx x #" << vtx_vx_counter << ": " << i_vtx_vx << " cm, it_chunk " << counter5<< endl;
              //if (verbose)
              //std::cout << "          chunk #" << counter5 << ": " << *it_chunk << std::endl;

              for ( unsigned int ithrow = 0; ithrow < 64; ithrow++ )
              {
                // For the numerator, only consider throws where throwed FD evt vtx x/y/z is in ND FV, same as what is done for ND evts
                // For now, we use mu start pos as evt vtx pos, random throws for y/z are stored in the ThrowsFD tree
                if ( FDEffCalc_cfg::IsInNDFV(i_vtx_vx, throwVtxY.at( (counter5-1)*64 + ithrow ) - NDLAr_OnAxis_offset[1], throwVtxZ.at( (counter5-1)*64 + ithrow ) - NDLAr_OnAxis_offset[2]))
                {
                    validthrows++;
                    //cout << "validthrows: " << validthrows<<endl;// <<" veto E throws: "<<vetoEThrow<<endl;
                    //cout<<" counter 5: "<<counter5<< " (counter5-1)*64 "<<(counter5-1)*64<<" it chunk: "<<(*it_chunk)<<endl;
                    //cout<<" hadronthroresults[0][0].size "<<hadron_throw_result[0][0].size()<<" vetoE[0][0][.size() "<< vetoEThrow[0][0].size()<<endl;
                    //cout <<" veto E: "<<eff->Ge
                      // Access per throw result for the evt
                      // Example (in ROOT):
                      //   In:  uint64_t number = 18446744073709551615 (this is 2^64-1, the max value for uint64_t)
                      //   In:  number & ((uint64_t)1) << 0
                      //   Out: (unsigned long long) 1
                      //   In:  number & ((uint64_t)1) << 1
                      //   Out: (unsigned long long) 2
                      //   ...
                      //   In:  number & ((uint64_t)1) << 63
                      //   Out: (unsigned long long) 9223372036854775808
                      // A non-zero result indicates the throw result is true, zero indicates the throw result is false

                    uint64_t throw_result = (*it_chunk) & ( ((uint64_t)1)<<(ithrow%64) );
                    //std::cout<<" throw result: "<<throw_result<<" hadron throw: "<<hadron_throw_result[0][0][(counter5-1) ]<<" veto Energy all events: "<< vetoEThrow[0][0][(counter5-1)*64 + ithrow]<<endl;
                    /*if(i_vtx_vx == -298.55){
                    //if (verbose) std::cout<<"(counter5-1)*64 + ithrow"<< (counter5-1)*64 + ithrow << "iwritten: " << iwritten<< "entry: "<<ientry<<"    throw #" << ithrow+1 << ": " << throw_result << std::endl;
		    ThrowResults.emplace_back(throw_result);
                    //save throws energy deposit
                    CurrentThrowDepsX.emplace_back(eff->getCurrentThrowDepsX((counter5-1)*64 + ithrow));
                    CurrentThrowDepsY.emplace_back(eff->getCurrentThrowDepsY((counter5-1)*64 + ithrow));
                    CurrentThrowDepsZ.emplace_back(eff->getCurrentThrowDepsZ((counter5-1)*64 + ithrow));
                    CurrentThrowTotE.emplace_back(eff->getCurrentThrowsTotE());
                    }*/
                    //cout<<" tot energy: "<<eff->getCurrentThrowsTotE()<<endl;
                    //CurrentThrowDepsZ.emplace_back(eff->getCurrentThrowDepsZ((counter5-1)*64 + ithrow));
                   // CurrentThrowTotE.emplace_back(eff->getCurrentThrowsTotE());
                    // Count no. of throws passed hadron containment requirement
                    if ( throw_result != 0 ) {
		       hadronpass++;
                       //std::cout<<" test: outE "<<trimEThrow[0][0][(counter5-1)*64 + ithrow]<<"hadronic energy at FD: "<<totEnergyFDatFD_float<<" tot E: "<<totEnergyFDatND_vtx[vtx_vx_counter]<<std::endl;
                       //cout<<"****PASS** ithrow: "<<ithrow<<" throws results hadron_throws when hadron pass: "<<hadron_throw_result[0][0][(counter5-1) ]
			   //<<" veto e: "<<vetoEThrow[0][0][(counter5-1)*64 + ithrow]<<" it chuk: "<<(*it_chunk)<<" trim E: "<<trimEThrow[0][0][(counter5-1)*64 + ithrow]
                           //<<" throw Tot E : "<<eff->getCurrentThrowsTotE()  <<endl;
                       VetoEnergyEventsPass.emplace_back(vetoEThrow[0][0][(counter5-1)*64 + ithrow]);
                       TrimEnergyEventsPass.emplace_back(trimEThrow[0][0][(counter5-1)*64 + ithrow]);
                       TotalEnergyEventsPass.emplace_back(eff->getCurrentThrowsTotE());
                       /*if(i_vtx_vx == 270.55){
                         CurrentThrowDepsX.emplace_back(eff->getCurrentThrowDepsX((counter5-1)*64 + ithrow));
                         CurrentThrowDepsY.emplace_back(eff->getCurrentThrowDepsY((counter5-1)*64 + ithrow));
                         CurrentThrowDepsZ.emplace_back(eff->getCurrentThrowDepsZ((counter5-1)*64 + ithrow));
                         CurrentThrowTotE.emplace_back(eff->getCurrentThrowsTotE());
                         std::cout<<" iwritten: "<<iwritten<<std::endl;
                       } */
                       //VtxYEventsPass = eff->getCurrentThrowTranslationsY();
                       //VtxZEventsPass = eff->getCurrentThrowTranslationsZ();
                    }
		    // save hadron signature for event not passing
		    /*if ( throw_result == 0 ) {
		      if(i_vtx_vx == 246.95){
                         CurrentThrowDepsX.emplace_back(eff->getCurrentThrowDepsX((counter5-1)*64 + ithrow));
                         CurrentThrowDepsY.emplace_back(eff->getCurrentThrowDepsY((counter5-1)*64 + ithrow));
                         CurrentThrowDepsZ.emplace_back(eff->getCurrentThrowDepsZ((counter5-1)*64 + ithrow));
                         CurrentThrowTotE.emplace_back(eff->getCurrentThrowsTotE());
                         std::cout<<" iwritten: "<<iwritten<<std::endl;
                       }

		    } //end if for events failing*/

                } // end if FD event passed ND FV cut

              }   // end loop over 64 throws in a chunk

            }     // end loop over 64-throw chunks

                //
                // Calculate per-event hadron containment efficiency from throws
                //

                // If a throwed evt vtx is in ND dead region, it is not going to be detected by ND, but probably will be detected in FD (assume)
                // In principle, we should NOT exclude such throws in the denominator
                // hadron_contain_eff = hadronpass*1.0/N_throws; // N_throws also equals 64 * counter5
                //if ( debug ) std::cout << "        Passed throws: " << hadronpass << ", eff: " << hadron_contain_eff << ", evt vtx x [cm]: " << ND_OffAxis_Sim_mu_start_v[0]->at( counter2 - 1 ) << ", nd off-axis pos [m]: " << ND_LAr_dtctr_pos_vec->at( counter1 -1 ) << std::endl;

                // But for the ND FV cut, we could probably factorize it analytically instead of using these random throws to evaluate
                // therefore we exclude such throws from the denominator as well
                if ( validthrows > 0 ) hadron_contain_eff = hadronpass*1.0/validthrows;
                ND_GeoEff = hadron_contain_eff;
                ND_LAr_vtx_pos = i_vtx_vx;
                ND_LAr_dtctr_pos = i_ND_off_axis_pos;
                NPassedThrows = hadronpass;
                validThrows = validthrows;

                if (throwfileVerbose) myfile << "        ND_LAr_dtctr_pos: " << ND_LAr_dtctr_pos << "cm,  ND_LAr_vtx_pos: " << ND_LAr_vtx_pos << ", Passed throws: " << hadronpass << ", tot. valid throws: " << validthrows << ", eff: " << ND_GeoEff << "\n\n";
                //if (verbose)
                //cout << "        ND_LAr_dtctr_pos: " << ND_LAr_dtctr_pos << " cm,  ND_LAr_vtx_pos: " << ND_LAr_vtx_pos << " cm, Passed throws: " << hadronpass << ", tot. valid throws: " << validthrows <<"cross check validThrows: "<<validThrows << ", eff: " << ND_GeoEff << "\n\n";
                effValues->Fill();

                // Calculate the meanEff
                if(ND_LAr_vtx_pos<-250)
                {Leff += ND_GeoEff;Leff_counter++;}
                else if(ND_LAr_vtx_pos>250)
                {Reff += ND_GeoEff;Reff_counter++;}
                else
                {MiddleEff += ND_GeoEff;MiddleEff_counter++;}


          }     // end loop over veto energy
        }       // end loop over veto size
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        //
        ND_OffAxis_Unrotated_Sim_hadronic_hit.clear();
        ND_OffAxis_Sim_hadronic_hit.clear();

        hadron_throw_result_vtx.emplace_back(hadron_throw_result);
        hadron_throw_result.clear();
        vetoEThrow.clear();
        trimEThrow.clear();



      } // end Loop over ND_vtx_vx_vec

      // Calculate the average geo eff for different ND off axis positions
      ND_OffAxis_MeanEff = (Leff*1.0/Leff_counter+MiddleEff*1.0+Reff*1.0/Reff_counter)/(MiddleEff_counter+2);
      if (verbose) cout << "        ND_LAr_dtctr_pos: " << ND_LAr_dtctr_pos << " cm, mean eff: " << ND_OffAxis_MeanEff << "\n\n";

      ND_OffAxis_Unrotated_Sim_mu_start_v_xyz_LAr.emplace_back(ND_OffAxis_Unrotated_Sim_mu_start_v_xyz_vtx);
      ND_OffAxis_Unrotated_Sim_mu_start_v_xyz_vtx.clear();
      ND_OffAxis_Unrotated_Sim_mu_end_v_xyz_LAr.emplace_back(ND_OffAxis_Unrotated_Sim_mu_end_v_xyz_vtx);
      ND_OffAxis_Unrotated_Sim_mu_end_v_xyz_vtx.clear();
      ND_OffAxis_Unrotated_Sim_mu_start_p_xyz_LAr.emplace_back(ND_OffAxis_Unrotated_Sim_mu_start_p_xyz_vtx);
      ND_OffAxis_Unrotated_Sim_mu_start_p_xyz_vtx.clear();
      ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz_LAr.emplace_back(ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz_vtx);
      ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz_vtx.clear();

      ND_OffAxis_Sim_mu_start_v_xyz_LAr.emplace_back(ND_OffAxis_Sim_mu_start_v_xyz_vtx);
      ND_OffAxis_Sim_mu_start_v_xyz_vtx.clear();
      ND_OffAxis_Sim_mu_end_v_xyz_LAr.emplace_back(ND_OffAxis_Sim_mu_end_v_xyz_vtx);
      ND_OffAxis_Sim_mu_end_v_xyz_vtx.clear();
      ND_OffAxis_Sim_mu_start_p_xyz_LAr.emplace_back(ND_OffAxis_Sim_mu_start_p_xyz_vtx);
      ND_OffAxis_Sim_mu_start_p_xyz_vtx.clear();
      ND_OffAxis_Sim_mu_start_E_xyz_LAr.emplace_back(ND_OffAxis_Sim_mu_start_E_xyz_vtx);
      ND_OffAxis_Sim_mu_start_E_xyz_vtx.clear();
      ND_OffAxis_Sim_hadronic_hit_xyz_LAr.emplace_back(ND_OffAxis_Sim_hadronic_hit_xyz_vtx);
      ND_OffAxis_Sim_hadronic_hit_xyz_vtx.clear();

      hadron_throw_result_LAr.emplace_back(hadron_throw_result_vtx);
      hadron_throw_result_vtx.clear();

      vetoEnergyFDatND.emplace_back(vetoEnergyFDatND_vtx);
      vetoEnergyFDatND_vtx.clear();
      totEnergyFDatND.emplace_back(totEnergyFDatND_vtx);
      totEnergyFDatND_vtx.clear();
      outEnergyFDatND.emplace_back(outEnergyFDatND_vtx);
      outEnergyFDatND_vtx.clear();


    }   // end Loop over ND_LAr_dtctr_pos_vec




    effTreeFD->Fill();
    ND_OffAxis_Unrotated_Sim_mu_start_v_xyz_LAr.clear();
    ND_OffAxis_Unrotated_Sim_mu_end_v_xyz_LAr.clear();
    ND_OffAxis_Unrotated_Sim_mu_start_p_xyz_LAr.clear();
    ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz_LAr.clear();
    ND_OffAxis_Sim_mu_start_v_xyz_LAr.clear();
    ND_OffAxis_Sim_mu_end_v_xyz_LAr.clear();
    ND_OffAxis_Sim_mu_start_p_xyz_LAr.clear();
    ND_OffAxis_Sim_mu_start_E_xyz_LAr.clear();
    ND_OffAxis_Sim_hadronic_hit_xyz_LAr.clear();
    hadron_throw_result_LAr.clear();

    ND_RandomVtx_Sim_hadronic_hit.clear();
    ND_OnAxis_Sim_hadronic_hit.clear();

    vetoEnergyFDatND.clear();
    totEnergyFDatND.clear();
    outEnergyFDatND.clear();

    CurrentThrowDepsX.clear();
    CurrentThrowDepsY.clear();
    CurrentThrowDepsZ.clear();
    CurrentThrowTotE.clear();


    iwritten_vec.emplace_back(iwritten);
    cout<< "ientry: " << ientry << ", iwritten: " << iwritten << endl;
    if (verbose) myfile << "ientry: " << ientry << ", iwritten: " << iwritten << endl;

    iwritten++;
    }
  } // end loop over events entries
  std::cout << "Written evts: " << iwritten << std::endl;
  FD_eff = FD_vetocut_counter*1.0/FD_FV_counter;
  std::cout << "FD_vetocut_counter: " << FD_vetocut_counter << std::endl;
  std::cout << "FD_FV_counter: " << FD_FV_counter << std::endl;
  std::cout << "FD eff: " << FD_eff << std::endl;

  if (myfileVerbose) myfile << "Written evts: " << iwritten << "\n";
  if (throwfileVerbose) myfile << "Written evts: " << iwritten << "\n";

  //
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // Write trees
  TFile * outFile = new TFile("Output_FDGeoEff.root", "RECREATE");
  ThrowsFD->Write();
  effTreeFD->Write();
  effValues->Write();
  effPosND->Fill();
  effPosND->Write();
  hist_vetoEnergyFD->Write();

  myfile.close();
  outFile->Close();
} // end main
