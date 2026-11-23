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
#include <TMarker3DBox.h>
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
R__ADD_INCLUDE_PATH(include)
#include "step1_helpers.h"

void FDVtxPlot()
{
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // 0. FD: read event from FD MC ntuple: before earth curvature rotation
  //
  TChain *t = new TChain("MyEnergyAnalysis/MyTree");
  // Ntuple path on FNAL dunegpvm machine
  // For FNAL machine:
  t->Add("/pnfs/dune/persistent/users/flynnguo/myFDntuples/myntuple_68092381_99all.root");

  // Define variables for FD event
  int FD_Run; // # of the run being processed
  int FD_SubRun; // # of the sub-run being processed
  int FD_Event; // # of the event being processed
  int FD_Sim_nNumu; // # of Sim muon neutrinos (numu and numubar)
  double FD_Gen_numu_E; // Energy of generator level neutrino [GeV]
  int FD_Sim_nMu; // # of Sim muons (mu+/mu-)
  int FD_CCNC_truth; // 0 =CC 1 =NC
  int FD_neuPDG; // Generator level neutrino PDG
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
  t->SetBranchAddress("Sim_nMu",                  &FD_Sim_nMu);
  t->SetBranchAddress("CCNC_truth",               &FD_CCNC_truth);
  t->SetBranchAddress("neuPDG",                   &FD_neuPDG);
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

  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  //
  // Set Palette
  gStyle->SetPalette(55);
  gStyle->SetOptStat(1111);
  gStyle->SetStatX(0.85);		//Stat box x position (top right hand corner)
  gStyle->SetStatY(0.9); 		//Stat box y position
  gStyle->SetStatW(0.2);	 		//Stat box width as fraction of pad size
  gStyle->SetStatH(0.15);	 		//Size of each line in stat box



  Int_t Eff_FD_HV_nm = 0;
  Int_t Eff_FD_HV_dnm = 0;

  // Set X,Y d_transverse
  vector<double> Dtransverse;
  Int_t Dtrans_steps = 0;
  Double_t Dtrans_stepsize = 10;

  Dtransverse.clear();

  Dtrans_steps = ( FDActiveVol_max[0] - 30 -10) / Dtrans_stepsize; // 30 is the veto size
  for ( int i_Dtrans_step = 0; i_Dtrans_step < Dtrans_steps + 1; i_Dtrans_step++ ){
    Dtransverse.emplace_back( i_Dtrans_step*Dtrans_stepsize );
    cout<< "Dtransverse_xy: " << i_Dtrans_step*Dtrans_stepsize << endl;
  }

  Int_t Dtransverse_size = Dtransverse.size();
  std::cout << "Dtransverse size: "<< Dtransverse_size << "\n\n";

  // Set Z d_transverse
  vector<double> Dtransverse_z;
  Int_t Dtrans_steps_z = 0;
  Double_t Dtrans_stepsize_z = 10;

  Dtransverse_z.clear();

  Dtrans_steps_z = 200 / Dtrans_stepsize_z; // 30 is the veto size
  for ( int i_Dtrans_step = 0; i_Dtrans_step < Dtrans_steps_z + 1; i_Dtrans_step++ ){
    Dtransverse_z.emplace_back( i_Dtrans_step*Dtrans_stepsize_z );
    cout<< "Dtransverse_z: " << i_Dtrans_step*Dtrans_stepsize_z << endl;
  }

  Int_t Dtransverse_z_size = Dtransverse_z.size();
  std::cout << "Dtransverse z size: "<< Dtransverse_z_size << "\n\n";

  Int_t Dtransverse_tot = Dtransverse_size*Dtransverse_z_size;
  std::cout << "Dtransverse tot size: "<< Dtransverse_tot << "\n\n";


  Int_t z_counter = 0;

  TFile * outFile = new TFile("Output_FDVtxPlots.root", "RECREATE");

  Int_t Dtransverse_counter = 0;
  TCanvas** c_2dplot = new TCanvas*[Dtransverse_tot];
  TCanvas** c_3dplot = new TCanvas*[Dtransverse_tot];

  //  If you don’t need ROOT’s object lifetime management, call TH1::AddDirectory(false) before creating the first histogram, and you’ll get “regular C++ behavior”.
  TH1::AddDirectory(kFALSE); // Avoid potential memory leak

  TH2D** h_2dplot_xy = new TH2D*[Dtransverse_tot];
  TH2D** h_2dplot_zx = new TH2D*[Dtransverse_tot];
  TH2D** h_2dplot_zy = new TH2D*[Dtransverse_tot];

  TH3D** h_3dplot = new TH3D*[Dtransverse_tot];
  TGraph** g_dtransverse = new TGraph*[Dtransverse_z.size()];

  double y[Dtransverse_size];
  double x[Dtransverse_size];

  for (Double_t i_Dtransverse_z : Dtransverse_z )
  {
    for (Double_t i_Dtransverse : Dtransverse)
    {
      //
      // Declare variables used in this program
      //
      int nentries = 0; // Total input events
      float vetoEnergyFD; // Total hadron deposited energy in FD veto region
      int iwritten = 0; // Output event counter
      //
      // Calculate FD eff
      double FD_veto_eff = 0.;
      double FD_FV_eff = 0.;
      int FD_vetocut_counter = 0;
      int FD_FV_counter = 0;

      Double_t setFD_FV_min[] = { FDActiveVol_min[0]+30+i_Dtransverse, FDActiveVol_min[1]+30+i_Dtransverse, i_Dtransverse_z};
      Double_t setFD_FV_max[] = { FDActiveVol_max[0]-30-i_Dtransverse, FDActiveVol_max[1]-30-i_Dtransverse, 1244.};
      //
      //------------------------------------------------------------------------------
      //------------------------------------------------------------------------------
      //------------------------------------------------------------------------------
      //
      TString c_2dplot_name = Form("c_2dvtx_dtransverse_%f", i_Dtransverse);
      TString c_2dplot_title = Form("2d vtx plot dtransverse=%f", i_Dtransverse);
      c_2dplot[Dtransverse_counter] = new TCanvas(c_2dplot_name,c_2dplot_title,0,53,995,597);
      c_2dplot[Dtransverse_counter]->Clear();
      c_2dplot[Dtransverse_counter]->SetLeftMargin(0.10);
      c_2dplot[Dtransverse_counter]->SetRightMargin(0.30);
      c_2dplot[Dtransverse_counter]->Divide(2,2);

      h_2dplot_xy[Dtransverse_counter] = new TH2D("2dvtx_xy","2dvtx_xy",100,-420,420,100,-650,650);
      h_2dplot_xy[Dtransverse_counter]->GetXaxis()->SetTitle("X [cm]");
      h_2dplot_xy[Dtransverse_counter]->GetYaxis()->SetTitle("Y [cm]");
      h_2dplot_zx[Dtransverse_counter] = new TH2D("2dvtx_zx","2dvtx_zx",100,-50,1450,100,-420,420);
      h_2dplot_zx[Dtransverse_counter]->GetXaxis()->SetTitle("Z [cm]");
      h_2dplot_zx[Dtransverse_counter]->GetYaxis()->SetTitle("X [cm]");
      h_2dplot_zy[Dtransverse_counter] = new TH2D("2dvtx_zy","2dvtx_zy",100,-50,1450,100,-650,650);
      h_2dplot_zy[Dtransverse_counter]->GetXaxis()->SetTitle("Z [cm]");
      h_2dplot_zy[Dtransverse_counter]->GetYaxis()->SetTitle("Y [cm]");

      // 3d plots
      TString c_3dplot_name = Form("c_3dvtx_dtransverse_%f", i_Dtransverse);
      TString c_3dplot_title = Form("3d vtx plot dtransverse=%f", i_Dtransverse);
      c_3dplot[Dtransverse_counter] = new TCanvas(c_3dplot_name,c_3dplot_title,0,53,995,597);
      c_3dplot[Dtransverse_counter]->Clear();
      c_3dplot[Dtransverse_counter]->SetLeftMargin(0.10);
      c_3dplot[Dtransverse_counter]->SetRightMargin(0.10);

      h_3dplot[Dtransverse_counter] = new TH3D("3dvtx","3dvtx",100,-420,420,100,-650,650,100,-50,1450);
      h_3dplot[Dtransverse_counter]->GetXaxis()->SetTitle("X [cm]");
      h_3dplot[Dtransverse_counter]->GetYaxis()->SetTitle("Y [cm]");
      h_3dplot[Dtransverse_counter]->GetZaxis()->SetTitle("Z [cm]");

      Double_t Eff_FD_HV = 0.; // Hardonic veto eff in FD
      //------------------------------------------------------------------------------
      // Loop over FD events
      //
      nentries = t->GetEntries();
      std::cout << "Tot evts: " << nentries << std::endl;
      for ( int ientry = 0; ientry < nentries; ientry++ )
      {
        t->GetEntry(ientry);
        if ( ientry%10000 == 0 )
        {
          std::cout << "Looking at entry " << ientry << ", FD_run: " << FD_Run << ", FD_subrun: " << FD_SubRun << ", FD_event: " << FD_Event << std::endl;
        }

        //
        // Skip events without muon/hadronic deposits
        //
        if ( FD_Sim_nMu == 0 || FD_Sim_n_hadronic_Edep_b == 0 ) continue;
        if ( FD_CCNC_truth == 1) continue;   // only use CC events
        if ( abs(FD_neuPDG) != 14 ) continue;       // only use muon neu


        h_2dplot_xy[Dtransverse_counter]->Fill(FD_Sim_mu_start_vx,FD_Sim_mu_start_vy);
        h_2dplot_zx[Dtransverse_counter]->Fill(FD_Sim_mu_start_vz,FD_Sim_mu_start_vx);
        h_2dplot_zy[Dtransverse_counter]->Fill(FD_Sim_mu_start_vz,FD_Sim_mu_start_vy);
        h_3dplot[Dtransverse_counter]->Fill(FD_Sim_mu_start_vx,FD_Sim_mu_start_vy,FD_Sim_mu_start_vz);
        // Only pick the events' vertex inside the FD FV
        // if(FD_Sim_mu_start_vx > setFD_FV_max[0] || FD_Sim_mu_start_vx < setFD_FV_min[0] || FD_Sim_mu_start_vy > setFD_FV_max[1] || FD_Sim_mu_start_vy < setFD_FV_min[1] || FD_Sim_mu_start_vz > setFD_FV_max[2] || FD_Sim_mu_start_vz < setFD_FV_min[2]) continue;
        if(FD_Sim_mu_start_vx < setFD_FV_max[0] && FD_Sim_mu_start_vx > setFD_FV_min[0] && FD_Sim_mu_start_vy < setFD_FV_max[1] && FD_Sim_mu_start_vy > setFD_FV_min[1] && FD_Sim_mu_start_vz < setFD_FV_max[2] && FD_Sim_mu_start_vz > setFD_FV_min[2])
        {
          FD_FV_counter++;
          Eff_FD_HV_dnm++;
        }


        //
        // Calculate total hadron E in FD veto region
        //
        vetoEnergyFD = 0.;
        // Loop over hadron E deposits
        for ( int ihadronhit = 0; ihadronhit < FD_Sim_n_hadronic_Edep_b; ihadronhit++ )
        {

          // Veto region size: 30 cm from the active volume
          if ( ( FD_Sim_hadronic_hit_x_b->at(ihadronhit) > FDActiveVol_min[0]      && FD_Sim_hadronic_hit_x_b->at(ihadronhit) < FDActiveVol_min[0] + 30 ) ||
               ( FD_Sim_hadronic_hit_y_b->at(ihadronhit) > FDActiveVol_min[1]      && FD_Sim_hadronic_hit_y_b->at(ihadronhit) < FDActiveVol_min[1] + 30 ) ||
               ( FD_Sim_hadronic_hit_z_b->at(ihadronhit) > FDActiveVol_min[2]      && FD_Sim_hadronic_hit_z_b->at(ihadronhit) < FDActiveVol_min[2] + 30 ) ||
               ( FD_Sim_hadronic_hit_x_b->at(ihadronhit) > FDActiveVol_max[0] - 30 && FD_Sim_hadronic_hit_x_b->at(ihadronhit) < FDActiveVol_max[0] ) ||
               ( FD_Sim_hadronic_hit_y_b->at(ihadronhit) > FDActiveVol_max[1] - 30 && FD_Sim_hadronic_hit_y_b->at(ihadronhit) < FDActiveVol_max[1] ) ||
               ( FD_Sim_hadronic_hit_z_b->at(ihadronhit) > FDActiveVol_max[2] - 30 && FD_Sim_hadronic_hit_z_b->at(ihadronhit) < FDActiveVol_max[2] )
             ){
               vetoEnergyFD += FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit);
          } // end if hadron deposit in FD veto region

        } // end loop over hadron E deposits

        // Skip FD event if the total hadron E in veto region exceeds vetoEnergy [MeV]
        //
        if (verbose) cout << "\n ientry: " << ientry << ", iwritten: " << iwritten << ", VetoEnergyFD[MeV]: " << vetoEnergyFD <<"\n\n";
        if ( vetoEnergyFD > 30 ) continue; // 30 MeV
        FD_vetocut_counter++;
        if(FD_Sim_mu_start_vx < setFD_FV_max[0] && FD_Sim_mu_start_vx > setFD_FV_min[0] && FD_Sim_mu_start_vy < setFD_FV_max[1] && FD_Sim_mu_start_vy > setFD_FV_min[1] && FD_Sim_mu_start_vz < setFD_FV_max[2] && FD_Sim_mu_start_vz > setFD_FV_min[2])
        {
          Eff_FD_HV_nm++;
        }


        iwritten++;

      } // end loop over events entries

      std::cout << "Tot entries: " << nentries << std::endl;
      std::cout << "Written evts: " << iwritten << std::endl;
      std::cout << "Dtransverse: " << i_Dtransverse << std::endl;
      FD_veto_eff = FD_vetocut_counter*1.0/nentries;
      FD_FV_eff = FD_FV_counter*1.0/nentries;
      std::cout << "FD_vetocut_counter: " << FD_vetocut_counter << std::endl;
      std::cout << "FD_FV_counter: " << FD_FV_counter << std::endl;
      std::cout << "FD veto eff: " << FD_veto_eff << std::endl;
      std::cout << "FD FV eff: " << FD_FV_eff << std::endl;


      Eff_FD_HV = Eff_FD_HV_nm*1.0/Eff_FD_HV_dnm;
      std::cout << "Eff_FD_HV: " << Eff_FD_HV << std::endl;
      if(Eff_FD_HV>1) cout<< "Eff_FD_HV OVER THAN 1, i_Dtransverse is: " << i_Dtransverse << endl;
      x[Dtransverse_counter]=i_Dtransverse;
      y[Dtransverse_counter]=Eff_FD_HV;

      // Draw plots
      c_2dplot[Dtransverse_counter]->cd(1);
      c_2dplot[Dtransverse_counter]->GetPad(1)->SetRightMargin(.15);
      h_2dplot_xy[Dtransverse_counter]->Draw();
      auto *xy_box = new TBox(FDActiveVol_min[0],FDActiveVol_min[1],FDActiveVol_max[0],FDActiveVol_max[1]);
      xy_box->SetLineColor(kBlack);
      xy_box->SetLineWidth(2);
      xy_box->SetFillStyle(0);
      xy_box->Draw();
      auto *xy_box1 = new TBox(FDActiveVol_min[0]+30,FDActiveVol_min[1]+30,FDActiveVol_max[0]-30,FDActiveVol_max[1]-30);
      xy_box1->SetLineColor(kBlue);
      xy_box1->SetLineWidth(2);
      xy_box1->SetFillStyle(0);
      xy_box1->Draw();
      auto *xy_box2 = new TBox(setFD_FV_min[0],setFD_FV_min[1],setFD_FV_max[0],setFD_FV_max[1]);
      xy_box2->SetLineColor(kRed);
      xy_box2->SetLineWidth(2);
      xy_box2->SetFillStyle(0);
      xy_box2->Draw();

      c_2dplot[Dtransverse_counter]->cd(2);
      c_2dplot[Dtransverse_counter]->GetPad(2)->SetRightMargin(.15);
      h_2dplot_zx[Dtransverse_counter]->Draw();
      // ND active volume
      auto *zx_box = new TBox(FDActiveVol_min[2],FDActiveVol_min[0],FDActiveVol_max[2],FDActiveVol_max[0]);
      zx_box->SetLineColor(kBlack);
      zx_box->SetLineWidth(2);
      zx_box->SetFillStyle(0);
      zx_box->Draw();
      // ND veto volume
      auto *zx_box1 = new TBox(FDActiveVol_min[2]+30,FDActiveVol_min[0]+30,FDActiveVol_max[2]-30,FDActiveVol_max[0]-30);
      zx_box1->SetLineColor(kBlue);
      zx_box1->SetLineWidth(2);
      zx_box1->SetFillStyle(0);
      zx_box1->Draw();
      // ND fiducial volume
      auto *zx_box2 = new TBox(setFD_FV_min[2],setFD_FV_min[0],setFD_FV_max[2],setFD_FV_max[0]);
      zx_box2->SetLineColor(kRed);
      zx_box2->SetLineWidth(2);
      zx_box2->SetFillStyle(0);
      zx_box2->Draw();

      c_2dplot[Dtransverse_counter]->cd(3);
      c_2dplot[Dtransverse_counter]->GetPad(3)->SetRightMargin(.15);
      h_2dplot_zy[Dtransverse_counter]->Draw();
      auto *yz_box = new TBox(FDActiveVol_min[2],FDActiveVol_min[1],FDActiveVol_max[2],FDActiveVol_max[1]);
      yz_box->SetLineColor(kBlack);
      yz_box->SetLineWidth(2);
      yz_box->SetFillStyle(0);
      yz_box->Draw();
      auto *yz_box1 = new TBox(FDActiveVol_min[2]+30,FDActiveVol_min[1]+30,FDActiveVol_max[2]-30,FDActiveVol_max[1]-30);
      yz_box1->SetLineColor(kBlue);
      yz_box1->SetLineWidth(2);
      yz_box1->SetFillStyle(0);
      yz_box1->Draw();
      auto *yz_box2 = new TBox(setFD_FV_min[2],setFD_FV_min[1],setFD_FV_max[2],setFD_FV_max[1]);
      yz_box2->SetLineColor(kRed);
      yz_box2->SetLineWidth(2);
      yz_box2->SetFillStyle(0);
      yz_box2->Draw();

      c_3dplot[Dtransverse_counter]->cd();
      h_3dplot[Dtransverse_counter]->Draw();
      auto *box_1 = new	TMarker3DBox((FDActiveVol_min[0]+FDActiveVol_max[0])/2, (FDActiveVol_min[1]+FDActiveVol_max[1])/2, (FDActiveVol_min[2]+FDActiveVol_max[2])/2,(FDActiveVol_max[0]-FDActiveVol_min[0])/2, (FDActiveVol_max[1]-FDActiveVol_min[1])/2, (FDActiveVol_max[2]-FDActiveVol_min[2])/2,0,0);
      box_1->SetLineColor(kBlack);
      box_1->SetLineWidth(2);
      box_1->SetFillStyle(0);
      box_1->Draw();
      auto *box_2 = new	TMarker3DBox((FDActiveVol_min[0]+FDActiveVol_max[0])/2, (FDActiveVol_min[1]+FDActiveVol_max[1])/2, (FDActiveVol_min[2]+FDActiveVol_max[2])/2,(FDActiveVol_max[0]-FDActiveVol_min[0])/2-30, (FDActiveVol_max[1]-FDActiveVol_min[1])/2-30, (FDActiveVol_max[2]-FDActiveVol_min[2])/2-30,0,0);
      box_2->SetLineColor(kBlue);
      box_2->SetLineWidth(2);
      box_2->SetFillStyle(0);
      box_2->Draw();
      auto *box_3 = new	TMarker3DBox((setFD_FV_min[0]+setFD_FV_max[0])/2, (setFD_FV_min[1]+setFD_FV_max[1])/2, (setFD_FV_min[2]+setFD_FV_max[2])/2,(setFD_FV_max[0]-setFD_FV_min[0])/2, (setFD_FV_max[1]-setFD_FV_min[1])/2, (setFD_FV_max[2]-setFD_FV_min[2])/2,0,0);
      box_3->SetLineColor(kRed);
      box_3->SetLineWidth(2);
      box_3->SetFillStyle(0);
      box_3->Draw();

      //
      //------------------------------------------------------------------------------
      //------------------------------------------------------------------------------
      //------------------------------------------------------------------------------
      //
      // Write trees

      c_2dplot[Dtransverse_counter]->Write();
      c_3dplot[Dtransverse_counter]->Write();
      gPad->Update();
      gPad->Modified();
      gSystem->ProcessEvents();
      c_2dplot[Dtransverse_counter]->Close();
      c_3dplot[Dtransverse_counter]->Close();

      Dtransverse_counter++;
      cout << "Dtransverse_counter: " << Dtransverse_counter << endl;

    } // end i_Dtransverse

    g_dtransverse[z_counter] = new TGraph(Dtransverse_size,x,y);
    g_dtransverse[z_counter]->SetMarkerStyle(20);
    g_dtransverse[z_counter]->SetMarkerColor(2);
    g_dtransverse[z_counter]->SetMarkerSize(3);
    g_dtransverse[z_counter]->Draw("AC*");
    g_dtransverse[z_counter]->SetTitle("Eff vs Dtransverse");
    g_dtransverse[z_counter]->GetXaxis()->SetTitle("d_transverse [cm]");
    g_dtransverse[z_counter]->GetYaxis()->SetTitle("Eff_{FD}^{FV}");
    TString d_transverse_z_name = Form("d_transverse_z_%.2f_cm", i_Dtransverse_z);
    TLatex xy_text(350,0.98,d_transverse_z_name);
    xy_text.DrawClone();
    g_dtransverse[z_counter]->Write();
    z_counter++;

  } // end i_Dtransverse_z


  delete[] h_2dplot_xy;
  delete[] h_2dplot_zx;
  delete[] h_2dplot_zy;
  delete[] h_3dplot;
  delete[] c_2dplot;
  delete[] c_3dplot;
  delete[] g_dtransverse;




  outFile->Close();
} // end main
