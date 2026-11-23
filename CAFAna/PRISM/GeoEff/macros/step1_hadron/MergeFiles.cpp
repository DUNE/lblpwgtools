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
#include <TGraph2D.h>
#include <TRandom.h>
#include <TF2.h>

// C++ includes
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <vector> // Need this for generate dictionary for nested vectors
using namespace std;

// Select file w/ E_vis_true>70GeV
void MergeFiles() // /pnfs/dune/persistent/users/flynnguo/myFDntuples/myntuple_61454381_*.root
{
  TString FileIn = "/pnfs/dune/persistent/users/flynnguo/myFDntuples/myntuple_64401080.root";
  TChain *t = new TChain("MyEnergyAnalysis/MyTree");
  t->Add(FileIn.Data());

  // Define variables for FD event
  int FD_Run; // # of the run being processed
  int FD_SubRun; // # of the sub-run being processed
  int FD_Event; // # of the event being processed
  int FD_Sim_nNumu; // # of Sim muon neutrinos (numu and numubar)

  double FD_LepE;                      // Generator level neutrino lepton energy
  double FD_Gen_numu_E; // Energy of generator level neutrino [MeV]
  double FD_E_vis_true; // True visible energy [MeV],  E_vis, true = LepE + eP + ePip + ePim + ePi0 + eOther + nipi0 * pi0_mass,  where pi0_mass is a constant that's 135 MeV

  int FD_Sim_nMu; // # of Sim muons (mu+/mu-)
  int FD_CCNC_truth; // 0 =CC 1 =NC
  int FD_neuPDG; // Generator level neutrino PDG
  int FD_LepPDG;                       // Generator level outgoing lepton PDG code
  double FD_LepNuAngle;                // Angle b/w nu and lepton


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
  double FD_Sim_mu_Edep_a1;                // muon energy deposit [GeV]: total amount of electrons reaching the readout channel
  double FD_Sim_mu_Edep_a2;                // muon energy deposit [MeV]: total amount of energy released by ionizations in the event (from Geant4 simulation)
  double FD_Sim_mu_Edep_b1;                // [GeV]
  double FD_Sim_mu_Edep_b2;                // [MeV]

  double FD_Sim_hadronic_Edep_a2; // Total amount of energy released by ionizations in the event (from Geant4 simulation) [MeV]
  int FD_Sim_n_hadronic_Edep_a; // # of hadronic energy deposits
  vector<float> *FD_Sim_hadronic_hit_Edep_a2 = 0; // Need initialize 0 here to avoid error
  vector<float> *FD_Sim_hadronic_hit_x_a = 0; // Position of each energy deposit on the x-axis [cm]
  vector<float> *FD_Sim_hadronic_hit_y_a = 0; // Position of each energy deposit on the y-axis [cm]
  vector<float> *FD_Sim_hadronic_hit_z_a = 0; // Position of each energy deposit on the z-axis [cm]

  // Add true particle information
  std::vector<int> *FD_P_PDG = 0;                         // PDG code for each particle
  int FD_P_num;                         // Number of types of particle
  std::vector<int> *FD_P_StatusCode = 0;                  // Status code for each particle, https://internal.dunescience.org/doxygen/GENIEGen__module_8cc_source.html
  std::vector<float> *FD_P_vtx_x = 0;                    // Position: x component for each particle
  std::vector<float> *FD_P_vtx_y = 0;                    // Position: y component for each particle
  std::vector<float> *FD_P_vtx_z = 0;                    // Position: z component for each particle
  std::vector<float> *FD_P_ptot = 0;                     // Total momentum for each particle
  std::vector<float> *FD_P_px = 0;                       // Momentum: x component for each particle
  std::vector<float> *FD_P_py = 0;                       // Momentum: y component for each particle
  std::vector<float> *FD_P_pz = 0;                       // Momentum: z component for each particle
  std::vector<float> *FD_P_E = 0;                        // Energy for each particle [GeV]
  std::vector<float> *FD_P_mass = 0;                     // Mass for each particle [GeV/c^2]
  std::vector<float> *FD_P_Ek = 0;                       // Kinetic Energy for each particle [GeV]



  // Extract event info from ntuple
  t->SetBranchAddress("Run",                      &FD_Run);
  t->SetBranchAddress("SubRun",                   &FD_SubRun);
  t->SetBranchAddress("Event",                    &FD_Event);
  t->SetBranchAddress("Sim_nNumu",                &FD_Sim_nNumu);
  t->SetBranchAddress("Gen_numu_E",               &FD_Gen_numu_E);
  t->SetBranchAddress("E_vis_true",               &FD_E_vis_true);
  t->SetBranchAddress("Sim_nMu",                  &FD_Sim_nMu);
  t->SetBranchAddress("CCNC_truth",               &FD_CCNC_truth);
  t->SetBranchAddress("neuPDG",                   &FD_neuPDG);
  t->SetBranchAddress("LepPDG",                   &FD_LepPDG);
  t->SetBranchAddress("LepE",                     &FD_LepE);
  t->SetBranchAddress("LepNuAngle",               &FD_LepNuAngle);


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
  t->SetBranchAddress("Sim_mu_Edep_a1",     &FD_Sim_mu_Edep_a1);
  t->SetBranchAddress("Sim_mu_Edep_a2",     &FD_Sim_mu_Edep_a2);
  t->SetBranchAddress("Sim_mu_Edep_b1",     &FD_Sim_mu_Edep_b1);
  t->SetBranchAddress("Sim_mu_Edep_b2",     &FD_Sim_mu_Edep_b2);

  t->SetBranchAddress("Sim_hadronic_Edep_a2",     &FD_Sim_hadronic_Edep_a2);
  t->SetBranchAddress("Sim_n_hadronic_Edep_a",    &FD_Sim_n_hadronic_Edep_a);
  t->SetBranchAddress("Sim_hadronic_hit_Edep_a2", &FD_Sim_hadronic_hit_Edep_a2);
  t->SetBranchAddress("Sim_hadronic_hit_x_a",     &FD_Sim_hadronic_hit_x_a);
  t->SetBranchAddress("Sim_hadronic_hit_y_a",     &FD_Sim_hadronic_hit_y_a);
  t->SetBranchAddress("Sim_hadronic_hit_z_a",     &FD_Sim_hadronic_hit_z_a);

  t->SetBranchAddress("P_PDG",     &FD_P_PDG);
  t->SetBranchAddress("P_num",     &FD_P_num);
  t->SetBranchAddress("P_StatusCode",     &FD_P_StatusCode);
  t->SetBranchAddress("P_vtx_x",    &FD_P_vtx_x);
  t->SetBranchAddress("P_vtx_y",    &FD_P_vtx_y);
  t->SetBranchAddress("P_vtx_z",    &FD_P_vtx_z);
  t->SetBranchAddress("P_ptot",     &FD_P_ptot);
  t->SetBranchAddress("P_px",       &FD_P_px);
  t->SetBranchAddress("P_py",       &FD_P_py);
  t->SetBranchAddress("P_pz",       &FD_P_pz);
  t->SetBranchAddress("P_E",        &FD_P_E);
  t->SetBranchAddress("P_mass",     &FD_P_mass);
  t->SetBranchAddress("P_Ek",       &FD_P_Ek);



  // create a new ntuple file
  TFile * outFile = new TFile("myntuple_64401080_cut.root", "RECREATE");
  TDirectory *IP =(TDirectory*)outFile->mkdir("MyEnergyAnalysis");//create a new folder in the root file
  TTree *mytree = new TTree("MyTree", "MyTree");
  mytree->Branch("Run",                      &FD_Run,                    "Run/I");
  mytree->Branch("SubRun",                   &FD_SubRun,                 "SubRun/I");
  mytree->Branch("Event",                    &FD_Event,                  "Event/I");
  mytree->Branch("Sim_nNumu",                &FD_Sim_nNumu,              "Sim_nNumu/I");
  mytree->Branch("Gen_numu_E",               &FD_Gen_numu_E,             "Gen_numu_E/D");
  mytree->Branch("E_vis_true",               &FD_E_vis_true,             "E_vis_true/D");
  mytree->Branch("LepE",                     &FD_LepE,                   "LepE/D");

  mytree->Branch("Sim_nMu",                  &FD_Sim_nMu,                "Sim_nMu/I");
  mytree->Branch("CCNC_truth",               &FD_CCNC_truth,             "CCNC_truth/I");
  mytree->Branch("neuPDG",         	     	   &FD_neuPDG,        	       "neuPDG/I");
  mytree->Branch("LepPDG",        	         &FD_LepPDG,     	          "LepPDG/I");
  mytree->Branch("LepNuAngle",               &FD_LepNuAngle,             "LepNuAngle/D");


  mytree->Branch("Sim_mu_start_vx",          &FD_Sim_mu_start_vx,        "Sim_mu_start_vx/D");
  mytree->Branch("Sim_mu_start_vy",          &FD_Sim_mu_start_vy,        "Sim_mu_start_vy/D");
  mytree->Branch("Sim_mu_start_vz",          &FD_Sim_mu_start_vz,        "Sim_mu_start_vz/D");
  mytree->Branch("Sim_mu_end_vx",            &FD_Sim_mu_end_vx,          "Sim_mu_end_vx/D");
  mytree->Branch("Sim_mu_end_vy",            &FD_Sim_mu_end_vy,          "Sim_mu_end_vy/D");
  mytree->Branch("Sim_mu_end_vz",            &FD_Sim_mu_end_vz,          "Sim_mu_end_vz/D");
  mytree->Branch("Sim_mu_start_px",          &FD_Sim_mu_start_px,        "Sim_mu_start_px/D");
  mytree->Branch("Sim_mu_start_py",          &FD_Sim_mu_start_py,        "Sim_mu_start_py/D");
  mytree->Branch("Sim_mu_start_pz",          &FD_Sim_mu_start_pz,        "Sim_mu_start_pz/D");
  mytree->Branch("Sim_mu_start_E",           &FD_Sim_mu_start_E,         "Sim_mu_start_E/D");
  mytree->Branch("Sim_mu_end_px",            &FD_Sim_mu_end_px,          "Sim_mu_end_px/D");
  mytree->Branch("Sim_mu_end_py",            &FD_Sim_mu_end_py,          "Sim_mu_end_py/D");
  mytree->Branch("Sim_mu_end_pz",            &FD_Sim_mu_end_pz,          "Sim_mu_end_pz/D");
  mytree->Branch("Sim_mu_end_E",             &FD_Sim_mu_end_E,           "Sim_mu_end_E/D");
  mytree->Branch("Sim_mu_Edep_a1",              &FD_Sim_mu_Edep_a1,            "Sim_mu_Edep_a1/D");
  mytree->Branch("Sim_mu_Edep_a2",              &FD_Sim_mu_Edep_a2,            "Sim_mu_Edep_a2/D");
  mytree->Branch("Sim_mu_Edep_b1",              &FD_Sim_mu_Edep_b1,            "Sim_mu_Edep_b1/D");
  mytree->Branch("Sim_mu_Edep_b2",              &FD_Sim_mu_Edep_b2,            "Sim_mu_Edep_b2/D");


  mytree->Branch("Sim_hadronic_Edep_a2",     &FD_Sim_hadronic_Edep_a2,   "Sim_hadronic_Edep_a2/D");
  mytree->Branch("Sim_n_hadronic_Edep_a",    &FD_Sim_n_hadronic_Edep_a,  "Sim_n_hadronic_Edep_a/I");
  mytree->Branch("Sim_hadronic_hit_Edep_a2", &FD_Sim_hadronic_hit_Edep_a2);
  mytree->Branch("Sim_hadronic_hit_x_a",     &FD_Sim_hadronic_hit_x_a);
  mytree->Branch("Sim_hadronic_hit_y_a",     &FD_Sim_hadronic_hit_y_a);
  mytree->Branch("Sim_hadronic_hit_z_a",     &FD_Sim_hadronic_hit_z_a);


  // true info for each particle
  mytree->Branch("P_PDG",     &FD_P_PDG);
  mytree->Branch("P_num",     &FD_P_num);
  mytree->Branch("P_StatusCode",     &FD_P_StatusCode);
  mytree->Branch("P_vtx_x",    &FD_P_vtx_x);
  mytree->Branch("P_vtx_y",    &FD_P_vtx_y);
  mytree->Branch("P_vtx_z",    &FD_P_vtx_z);
  mytree->Branch("P_ptot",     &FD_P_ptot);
  mytree->Branch("P_px",       &FD_P_px);
  mytree->Branch("P_py",       &FD_P_py);
  mytree->Branch("P_pz",       &FD_P_pz);
  mytree->Branch("P_E",        &FD_P_E);
  mytree->Branch("P_mass",     &FD_P_mass);
  mytree->Branch("P_Ek",       &FD_P_Ek);


  // Loop over all events
  int nentries = 0; // Total input events
  nentries = t->GetEntries();
  cout<< " nentries:" << nentries<<endl;
  for ( int ientry = 0; ientry < nentries; ientry++ )
  {
    t->GetEntry(ientry);
    if ( FD_Sim_nMu == 0 || FD_Sim_n_hadronic_Edep_a == 0 ) continue;
    if ( FD_CCNC_truth == 1) continue;   // only use CC events
    if ( abs(FD_neuPDG) != 14 ) continue;       // only use muon neu
    if(FD_E_vis_true>=70)
    {
      cout << " ientry:" << ientry<<endl;
      mytree->Fill();
    }
  }


  outFile->cd("MyEnergyAnalysis");
  mytree->Write();
  outFile->Close();

} // end Plot_Evisture
