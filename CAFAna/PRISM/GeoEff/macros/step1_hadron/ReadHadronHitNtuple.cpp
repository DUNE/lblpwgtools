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
using namespace std;
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <vector> // Need this for generate dictionary for nested vectors
using namespace std;

// Include customized functions and constants
R__ADD_INCLUDE_PATH(include)
#include "step1_helpers.h"

float vSize = 30.;

// Draw ND hadronic hits plot
void ReadHadronHitNtuple_ND()
{
  gROOT->Reset();
  gStyle->SetOptStat(0); // Remove Stat Box

  // Input FDroot file
  TString FileIn = "/dune/app/users/flynnguo/NDEff/DUNE_ND_GeoEff/bin/Output_FDGeoEff_2293930_80.root";
  //
  // Read branch from input trees
  //
  // Read effValues
  TChain *t_effValues = new TChain("effValues");
  t_effValues->Add(FileIn.Data());
  Int_t iwritten;
  Double_t ND_LAr_dtctr_pos;
  Double_t ND_LAr_vtx_pos;
  Double_t ND_GeoEff;
  t_effValues->SetBranchAddress("iwritten",         &iwritten);
  t_effValues->SetBranchAddress("ND_LAr_dtctr_pos",   &ND_LAr_dtctr_pos);
  t_effValues->SetBranchAddress("ND_LAr_vtx_pos",       &ND_LAr_vtx_pos);
  //hardon info
  TChain *t_effTree = new TChain("effTreeND");
  t_effTree->Add(FileIn.Data());
  int ND_Sim_n_hadronic_Edep_b;
  vector<float> *HadronHitEdeps =0; // Hadron hit segment energy deposits [MeV]
  vector<vector<float>> *CurrentThrowDepsX = 0; // Coordinates of hadron hits X after random throws
  vector<vector<float>> *CurrentThrowDepsY =0; // Coordinates of hadron hits Y after random throws
  vector<vector<float>> *CurrentThrowDepsZ = 0; // Coordinates of hadron hits Z after random throws
  vector<float> *CurrentThrowTotE = 0;
  vector<vector<float>> *ND_OffAxis_Sim_hadronic_hit_xyz=0; // coordinates of hadron hits before random throws

  t_effTree->SetBranchAddress("ND_Sim_n_hadronic_Edep_b",         &ND_Sim_n_hadronic_Edep_b);
  t_effTree->SetBranchAddress("CurrentThrowDepsX",         &CurrentThrowDepsX);
  t_effTree->SetBranchAddress("CurrentThrowDepsY",         &CurrentThrowDepsY);
  t_effTree->SetBranchAddress("CurrentThrowDepsZ",         &CurrentThrowDepsZ);
  t_effTree->SetBranchAddress("CurrentThrowTotE",          &CurrentThrowTotE);
  t_effTree->SetBranchAddress("HadronHitEdeps",            &HadronHitEdeps);
  t_effTree->SetBranchAddress("ND_OffAxis_Sim_hadronic_hit_xyz",            &ND_OffAxis_Sim_hadronic_hit_xyz);

  // Read PosVec
  TChain *t_PosVec = new TChain("PosVec");
  t_PosVec->Add(FileIn.Data());
  vector<Double_t> *ND_LAr_dtctr_pos_vec = 0; // unit: cm, ND off-axis choices for each FD evt
  vector<Double_t> *ND_vtx_vx_vec = 0;       // unit: cm, vtx x choices for each FD evt in ND volume
  vector<Int_t> *iwritten_vec = 0;
  TBranch *b_ND_LAr_dtctr_pos_vec = 0;
  TBranch *b_ND_vtx_vx_vec = 0;
  TBranch *b_iwritten_vec = 0;
  t_PosVec->SetBranchAddress("ND_LAr_dtctr_pos_vec",      &ND_LAr_dtctr_pos_vec , &b_ND_LAr_dtctr_pos_vec);
  t_PosVec->SetBranchAddress("ND_vtx_vx_vec",             &ND_vtx_vx_vec,         &b_ND_vtx_vx_vec);
  t_PosVec->SetBranchAddress("iwritten_vec",              &iwritten_vec,          &b_iwritten_vec);

  Long64_t tentry = t_PosVec->LoadTree(0);
  b_ND_LAr_dtctr_pos_vec->GetEntry(tentry);
  b_ND_vtx_vx_vec->GetEntry(tentry);
  b_iwritten_vec->GetEntry(tentry);

  Int_t ND_LAr_dtctr_pos_vec_size = ND_LAr_dtctr_pos_vec->size();
  Int_t ND_vtx_vx_vec_size = ND_vtx_vx_vec->size();
  Int_t iwritten_vec_size = iwritten_vec->size();
  Int_t tot_size = ND_LAr_dtctr_pos_vec_size*ND_vtx_vx_vec_size;
  Int_t hadronhit_n_plots = tot_size*N_throws;
  Int_t nentries = t_effValues->GetEntries();


  // vector<Int_t> a_ND_off_axis_pos_vec = {-2800, -1600, 0};
  vector<Int_t> a_ND_off_axis_pos_vec = {0};
  vector<Int_t> a_ND_vtx_vx_vec = {-299, -292, -285, -278, -271, -264, -216, -168, -120, -72, -24, 24, 72, 120, 168, 216, 264, 271, 278, 285, 292, 299};
  // vector<Int_t> a_ND_vtx_vx_vec = {-168, -216};

  // Set Palette
  gStyle->SetPalette(55);
  gStyle->SetOptStat(0);

  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  // ND event display w.r.t different vertex positions
  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------

  TCanvas** c_offaxis_hadronhit = new TCanvas*[tot_size];
  TH2F** h_offaxis_hadronhit_xy = new TH2F*[tot_size];
  TH2F** h_offaxis_hadronhit_zx = new TH2F*[tot_size];
  TH2F** h_offaxis_hadronhit_zy = new TH2F*[tot_size];

  for (Int_t i_iwritten : *iwritten_vec)
  {
    //choose specific iwrittenR
    // if (i_iwritten != 5) continue;
    cout << "i_iwritten: " << i_iwritten << "\n";

    Int_t canvas_counter = 0;
    for (Int_t i_ND_LAr_dtctr_pos: a_ND_off_axis_pos_vec)
    {
      for (Int_t i_ND_LAr_vtx_pos: a_ND_vtx_vx_vec)
      {
        Int_t i_entry = tot_size * i_iwritten;
        for (i_entry ; i_entry < tot_size * (i_iwritten+1); i_entry++ )
        {
          t_effTree->GetEntry(i_entry);
          t_effValues->GetEntry(i_entry);
          if (ND_LAr_dtctr_pos == i_ND_LAr_dtctr_pos && ND_LAr_vtx_pos == i_ND_LAr_vtx_pos)
          {
            // create hist
            TString h_hadronhit_xy_name = Form("hadronhitXY_event_%d_OffAxis_%d_cm_LAr_%d_cm", i_iwritten,i_ND_LAr_dtctr_pos,i_ND_LAr_vtx_pos);
            // TString h_hadronhit_xy_name = Form(" ");
            h_offaxis_hadronhit_xy[canvas_counter] = new TH2F(h_hadronhit_xy_name,h_hadronhit_xy_name,200,-500,500,200,-500,500);
            h_offaxis_hadronhit_xy[canvas_counter]->GetXaxis()->SetTitle("X [cm]");
            h_offaxis_hadronhit_xy[canvas_counter]->GetYaxis()->SetTitle("Y [cm]");
            h_offaxis_hadronhit_xy[canvas_counter]->GetZaxis()->SetTitle("HadronHitEdeps [MeV]");

            TString h_hadronhit_zx_name = Form("hadronhitZX_event_%d_OffAxis_%d_cm_LAr_%d_cm", i_iwritten,i_ND_LAr_dtctr_pos,i_ND_LAr_vtx_pos);
            // TString h_hadronhit_zx_name = Form(" ");
            h_offaxis_hadronhit_zx[canvas_counter] = new TH2F(h_hadronhit_zx_name,h_hadronhit_zx_name,200,-100,600,200,-500,500);
            h_offaxis_hadronhit_zx[canvas_counter]->GetXaxis()->SetTitle("Z [cm]");
            h_offaxis_hadronhit_zx[canvas_counter]->GetYaxis()->SetTitle("X [cm]");
            h_offaxis_hadronhit_zx[canvas_counter]->GetZaxis()->SetTitle("HadronHitEdeps [MeV]");

            TString h_hadronhit_zy_name = Form("hadronhitZY_event_%d_OffAxis_%d_cm_LAr_%d_cm", i_iwritten,i_ND_LAr_dtctr_pos,i_ND_LAr_vtx_pos);
            // TString h_hadronhit_zy_name = Form(" ");
            h_offaxis_hadronhit_zy[canvas_counter] = new TH2F(h_hadronhit_zy_name,h_hadronhit_zy_name,200,-100,600,200,-500,500);
            h_offaxis_hadronhit_zy[canvas_counter]->GetXaxis()->SetTitle("Z [cm]");
            h_offaxis_hadronhit_zy[canvas_counter]->GetYaxis()->SetTitle("Y [cm]");
            h_offaxis_hadronhit_zy[canvas_counter]->GetZaxis()->SetTitle("HadronHitEdeps [MeV]");
            // create vector to store hadronic hit
            Int_t offset_X = i_ND_LAr_dtctr_pos;
            vector<float> ND_OffAxis_hadronic_hit_xyz;
            vector<vector<float>> ND_OffAxis_hadronic_hit;

            ND_OffAxis_hadronic_hit.clear();
            int it_hadronhit_counter =0;
            for (vector<vector<float>>::iterator it_hadronhit = ND_OffAxis_Sim_hadronic_hit_xyz->begin(); it_hadronhit!=ND_OffAxis_Sim_hadronic_hit_xyz->end(); ++it_hadronhit)
            {
              for (Int_t i = 0; i < 3; i++)
              {
                if(verbose) cout << "it_hadronhit_counter: " << it_hadronhit_counter << ", it_xyz_counter: " << i << ", hit_x,y,z: " << it_hadronhit->at(i) << endl;
                ND_OffAxis_hadronic_hit_xyz.emplace_back(it_hadronhit->at(i));
              }
              ND_OffAxis_hadronic_hit.emplace_back(ND_OffAxis_hadronic_hit_xyz);
              ND_OffAxis_hadronic_hit_xyz.clear();
              it_hadronhit_counter++;
            }// end hadronhit emplace_back
            Double_t vetoEnergyND = 0.;
            Double_t totEnergyND = 0.;
            Double_t outEnergyND = 0.;


            for(Int_t ihadronhit = 0; ihadronhit < ND_OffAxis_hadronic_hit.size(); ihadronhit++)
            {
              h_offaxis_hadronhit_xy[canvas_counter]->Fill(ND_OffAxis_hadronic_hit[ihadronhit][0]-offset_X,ND_OffAxis_hadronic_hit[ihadronhit][1]-NDLAr_OnAxis_offset[1],HadronHitEdeps->at(ihadronhit));
              h_offaxis_hadronhit_zx[canvas_counter]->Fill(ND_OffAxis_hadronic_hit[ihadronhit][2]-NDLAr_OnAxis_offset[2],ND_OffAxis_hadronic_hit[ihadronhit][0]-offset_X,HadronHitEdeps->at(ihadronhit));
              h_offaxis_hadronhit_zy[canvas_counter]->Fill(ND_OffAxis_hadronic_hit[ihadronhit][2]-NDLAr_OnAxis_offset[2],ND_OffAxis_hadronic_hit[ihadronhit][1]-NDLAr_OnAxis_offset[1],HadronHitEdeps->at(ihadronhit));

              totEnergyND += HadronHitEdeps->at(ihadronhit);
              if ( ( ND_OffAxis_hadronic_hit[ihadronhit][0]-offset_X                      > NDActiveVol_min[0]         && ND_OffAxis_hadronic_hit[ihadronhit][0]-offset_X                      < NDActiveVol_min[0] + vSize ) ||
                   ( ND_OffAxis_hadronic_hit[ihadronhit][1]-NDLAr_OnAxis_offset[1]        > NDActiveVol_min[1]         && ND_OffAxis_hadronic_hit[ihadronhit][1]-NDLAr_OnAxis_offset[1]        < NDActiveVol_min[1] + vSize ) ||
                   ( ND_OffAxis_hadronic_hit[ihadronhit][2]-NDLAr_OnAxis_offset[2]        > NDActiveVol_min[2]         && ND_OffAxis_hadronic_hit[ihadronhit][2]-NDLAr_OnAxis_offset[2]        < NDActiveVol_min[2] + vSize ) ||
                   ( ND_OffAxis_hadronic_hit[ihadronhit][0]-offset_X                      > NDActiveVol_max[0] - vSize && ND_OffAxis_hadronic_hit[ihadronhit][0]-offset_X                      < NDActiveVol_max[0] ) ||
                   ( ND_OffAxis_hadronic_hit[ihadronhit][1]-NDLAr_OnAxis_offset[1]        > NDActiveVol_max[1] - vSize && ND_OffAxis_hadronic_hit[ihadronhit][1]-NDLAr_OnAxis_offset[1]        < NDActiveVol_max[1] ) ||
                   ( ND_OffAxis_hadronic_hit[ihadronhit][2]-NDLAr_OnAxis_offset[2]        > NDActiveVol_max[2] - vSize && ND_OffAxis_hadronic_hit[ihadronhit][2]-NDLAr_OnAxis_offset[2]        < NDActiveVol_max[2] )
                 ){
                   vetoEnergyND += HadronHitEdeps->at(ihadronhit);
              } // end if hadron deposit in FD veto region
              else if
              (
               ( ND_OffAxis_hadronic_hit[ihadronhit][0]-offset_X                      < NDActiveVol_min[0] ) ||
               ( ND_OffAxis_hadronic_hit[ihadronhit][1]-NDLAr_OnAxis_offset[1]        < NDActiveVol_min[1] ) ||
               ( ND_OffAxis_hadronic_hit[ihadronhit][2]-NDLAr_OnAxis_offset[2]        < NDActiveVol_min[2] ) ||
               ( ND_OffAxis_hadronic_hit[ihadronhit][0]-offset_X                      > NDActiveVol_max[0] ) ||
               ( ND_OffAxis_hadronic_hit[ihadronhit][1]-NDLAr_OnAxis_offset[1]        > NDActiveVol_max[1] ) ||
               ( ND_OffAxis_hadronic_hit[ihadronhit][2]-NDLAr_OnAxis_offset[2]        > NDActiveVol_max[2] )
              )
              {
                outEnergyND += HadronHitEdeps->at(ihadronhit);
              } // end if hadron deposit outside FD active region
            }// end hadronhit loop
            cout<<"i_ND_LAr_vtx_pos: " << i_ND_LAr_vtx_pos << ", totE: "<<totEnergyND<< ", outE: " << outEnergyND << ", vetoEnergyND:" << vetoEnergyND << endl;
            TString energy_name = Form("VetoE_%.2f_MeV, OutE_%.2f_MeV, TotE_%.2f_MeV", vetoEnergyND, outEnergyND, totEnergyND);

            // create canvas
            TString c_hadronhit_name = Form("c_hadronhit_event_%d_OffAxis_%d_cm_LAr_%d_cm", i_iwritten,i_ND_LAr_dtctr_pos,i_ND_LAr_vtx_pos);
            TString c_hadronhit_title = Form("hadronhit event_%d_OffAxis_%d_cm_LAr_%d_cm", i_iwritten,i_ND_LAr_dtctr_pos,i_ND_LAr_vtx_pos);
            // TString c_hadronhit_title = Form(" ");
            c_offaxis_hadronhit[canvas_counter] = new TCanvas(c_hadronhit_name, c_hadronhit_title, 0,53,995,597);
            c_offaxis_hadronhit[canvas_counter]->Clear();
            c_offaxis_hadronhit[canvas_counter]->SetLeftMargin(0.10);
            c_offaxis_hadronhit[canvas_counter]->SetRightMargin(0.10);
            c_offaxis_hadronhit[canvas_counter]->Divide(2,2);

            c_offaxis_hadronhit[canvas_counter]->cd(1);
            c_offaxis_hadronhit[canvas_counter]->GetPad(1)->SetRightMargin(.15);
            h_offaxis_hadronhit_xy[canvas_counter]->Draw("COLZ");
            auto *xy_box = new TBox(NDActiveVol_min[0],NDActiveVol_min[1],NDActiveVol_max[0],NDActiveVol_max[1]);
            xy_box->SetLineColor(kBlack);
            xy_box->SetLineWidth(2);
            xy_box->SetFillStyle(0);
            xy_box->Draw();
            auto *xy_box1 = new TBox(NDActiveVol_min[0]+30,NDActiveVol_min[1]+30,NDActiveVol_max[0]-30,NDActiveVol_max[1]-30);
            xy_box1->SetLineColor(kBlue);
            xy_box1->SetLineWidth(2);
            xy_box1->SetFillStyle(0);
            xy_box1->Draw();
            auto *xy_box2 = new TBox(ND_FV_min[0],ND_FV_min[1],ND_FV_max[0],ND_FV_max[1]);
            xy_box2->SetLineColor(kRed);
            xy_box2->SetLineWidth(2);
            xy_box2->SetFillStyle(0);
            xy_box2->Draw();
            // Draw Veto E and Tot E
            TLatex xy_text(-400,400,energy_name);
            xy_text.DrawClone();

            c_offaxis_hadronhit[canvas_counter]->cd(2);
            c_offaxis_hadronhit[canvas_counter]->GetPad(2)->SetRightMargin(.15);
            h_offaxis_hadronhit_zx[canvas_counter]->Draw("COLZ");
            // ND active volume
            auto *zx_box = new TBox(NDActiveVol_min[2],NDActiveVol_min[0],NDActiveVol_max[2],NDActiveVol_max[0]);
            zx_box->SetLineColor(kBlack);
            zx_box->SetLineWidth(2);
            zx_box->SetFillStyle(0);
            zx_box->Draw();
            // ND veto volume
            auto *zx_box1 = new TBox(NDActiveVol_min[2]+30,NDActiveVol_min[0]+30,NDActiveVol_max[2]-30,NDActiveVol_max[0]-30);
            zx_box1->SetLineColor(kBlue);
            zx_box1->SetLineWidth(2);
            zx_box1->SetFillStyle(0);
            zx_box1->Draw();
            // ND fiducial volume
            auto *zx_box2 = new TBox(ND_FV_min[2],ND_FV_min[0],ND_FV_max[2],ND_FV_max[0]);
            zx_box2->SetLineColor(kRed);
            zx_box2->SetLineWidth(2);
            zx_box2->SetFillStyle(0);
            zx_box2->Draw();
            // Draw Veto E and Tot E
            TLatex xz_text(-50,400,energy_name);
            xz_text.DrawClone();

            c_offaxis_hadronhit[canvas_counter]->cd(3);
            c_offaxis_hadronhit[canvas_counter]->GetPad(3)->SetRightMargin(.15);
            h_offaxis_hadronhit_zy[canvas_counter]->Draw("COLZ");
            auto *yz_box = new TBox(NDActiveVol_min[2],NDActiveVol_min[1],NDActiveVol_max[2],NDActiveVol_max[1]);
            yz_box->SetLineColor(kBlack);
            yz_box->SetLineWidth(2);
            yz_box->SetFillStyle(0);
            yz_box->Draw();
            auto *yz_box1 = new TBox(NDActiveVol_min[2]+30,NDActiveVol_min[1]+30,NDActiveVol_max[2]-30,NDActiveVol_max[1]-30);
            yz_box1->SetLineColor(kBlue);
            yz_box1->SetLineWidth(2);
            yz_box1->SetFillStyle(0);
            yz_box1->Draw();
            auto *yz_box2 = new TBox(ND_FV_min[2],ND_FV_min[1],ND_FV_max[2],ND_FV_max[1]);
            yz_box2->SetLineColor(kRed);
            yz_box2->SetLineWidth(2);
            yz_box2->SetFillStyle(0);
            yz_box2->Draw();
            // Draw Veto E and Tot E
            TLatex yz_text(-50,400,energy_name);
            yz_text.DrawClone();

            gPad->Update();
            gPad->Modified();
            gSystem->ProcessEvents();

            c_offaxis_hadronhit[canvas_counter]->SaveAs( TString::Format("HadronHitPlots_ND/c_event_%d_onaxis_hadronhit_%d.pdf",iwritten, canvas_counter ) );
            canvas_counter++;
          }
        }//end ientry
      }//end vtx pos inside LAr
    }//end LAr pos
  }//end iwritten

  // delete all canvas
  delete[] h_offaxis_hadronhit_zy;
  delete[] h_offaxis_hadronhit_zx;
  delete[] h_offaxis_hadronhit_xy;
  delete[] c_offaxis_hadronhit;

  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  // ND event display for throws w.r.t different vertex positions
  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------

  TCanvas** c_hadronhit = new TCanvas*[hadronhit_n_plots];
  TH2F** h_hadronhit_xy = new TH2F*[hadronhit_n_plots];
  TH2F** h_hadronhit_zx = new TH2F*[hadronhit_n_plots];
  TH2F** h_hadronhit_zy = new TH2F*[hadronhit_n_plots];


  for (Int_t i_iwritten : *iwritten_vec)
  {
    //choose specific iwrittenR
    if (i_iwritten != 2) continue;
    cout << "i_iwritten: " << i_iwritten << "\n";

    Int_t n_plot = 0;
    Int_t i_n_plot = 0;
    for (Int_t i_ND_LAr_dtctr_pos: a_ND_off_axis_pos_vec)
    {
      for (Int_t i_ND_LAr_vtx_pos: a_ND_vtx_vx_vec)
      {
        Int_t i_entry = tot_size * i_iwritten;
        for (i_entry ; i_entry < tot_size * (i_iwritten+1); i_entry++ )
        {
          t_effTree->GetEntry(i_entry);
          t_effValues->GetEntry(i_entry);
          if (ND_LAr_dtctr_pos == i_ND_LAr_dtctr_pos && ND_LAr_vtx_pos == i_ND_LAr_vtx_pos)
          {
            Int_t offset_X = i_ND_LAr_dtctr_pos;
            // Store hadron hit info into vector
            vector<float> ThrowDepsX_hit;
            vector<vector<float>> ThrowDepsX;
            ThrowDepsX.clear();
            int it_throw_x_counter =0;
            for (vector<vector<float>>::iterator it_throw = CurrentThrowDepsX->begin(); it_throw!=CurrentThrowDepsX->end(); ++it_throw)
            {
              for (Int_t ihadronhit =0; ihadronhit < ND_Sim_n_hadronic_Edep_b; ihadronhit++)
              {
                if(verbose) cout << "it_throw_x_counter: " << it_throw_x_counter << ", ihadronhit: " << ihadronhit << ", hit_x: " << it_throw->at(ihadronhit) << endl;
                ThrowDepsX_hit.emplace_back(it_throw->at(ihadronhit));
              }
              ThrowDepsX.emplace_back(ThrowDepsX_hit);
              ThrowDepsX_hit.clear();
              it_throw_x_counter++;
            }// end CurrentThrowDepsX
            vector<float> ThrowDepsY_hit;
            vector<vector<float>> ThrowDepsY;
            ThrowDepsY.clear();
            int it_throw_y_counter =0;
            for (vector<vector<float>>::iterator it_throw = CurrentThrowDepsY->begin(); it_throw!=CurrentThrowDepsY->end(); ++it_throw)
            {
              for (Int_t ihadronhit =0; ihadronhit < ND_Sim_n_hadronic_Edep_b; ihadronhit++)
              {
                if(verbose) cout << "it_throw_x_counter: " << it_throw_x_counter << ", ihadronhit: " << ihadronhit << ", hit_y: " << it_throw->at(ihadronhit) << endl;
                ThrowDepsY_hit.emplace_back(it_throw->at(ihadronhit));
              }
              ThrowDepsY.emplace_back(ThrowDepsY_hit);
              ThrowDepsY_hit.clear();
              it_throw_y_counter++;
            }// end CurrentThrowDepsY
            vector<float> ThrowDepsZ_hit;
            vector<vector<float>> ThrowDepsZ;
            ThrowDepsZ.clear();
            int it_throw_z_counter =0;
            for (vector<vector<float>>::iterator it_throw = CurrentThrowDepsZ->begin(); it_throw!=CurrentThrowDepsZ->end(); ++it_throw)
            {
              for (Int_t ihadronhit =0; ihadronhit < ND_Sim_n_hadronic_Edep_b; ihadronhit++)
              {
                if(verbose) cout<< "ND_LAr_dtctr_pos: " << ND_LAr_dtctr_pos << ", ND_LAr_vtx_pos: " << ND_LAr_vtx_pos << ", ithrow: " << it_throw_z_counter << ", ihadronhit: " << ihadronhit << ", hit_z: " << it_throw->at(ihadronhit) << endl;
                ThrowDepsZ_hit.emplace_back(it_throw->at(ihadronhit));
              }
              ThrowDepsZ.emplace_back(ThrowDepsZ_hit);
              ThrowDepsZ_hit.clear();
              it_throw_z_counter++;
            }// end CurrentThrowDepsZ

            int throw_couter = 0;
            // Draw plots
            // for(Int_t ithrow = 0; ithrow < N_throws; ithrow++) // check the throw number defined in the runGeoEffFDEvtSim_hadronplot.cpp
            // for(Int_t ithrow = 0; ithrow < 15; ithrow++)
            // vector<int> common_v={ 19, 30, 33, 38 }; // can only pick throw smaller than 50
            // for (Int_t ithrow: common_v)
            for (Int_t ithrow = 0; ithrow < 8; ithrow++ )
            {
              // cout << "ithrow: " << ithrow <<endl;
              // for all events
              n_plot = i_n_plot*N_throws + ithrow;

              TString h_hadronhit_xy_name = Form("hadronhitXY_event_%d_OffAxis_%d_cm_LAr_%d_cm_throw_%d", i_iwritten,i_ND_LAr_dtctr_pos,i_ND_LAr_vtx_pos,ithrow);
              // TString h_hadronhit_xy_name = Form(" ");
              h_hadronhit_xy[n_plot] = new TH2F(h_hadronhit_xy_name,h_hadronhit_xy_name,200,-500,500,200,-500,500);
              h_hadronhit_xy[n_plot]->GetXaxis()->SetTitle("X [cm]");
              h_hadronhit_xy[n_plot]->GetYaxis()->SetTitle("Y [cm]");
              h_hadronhit_xy[n_plot]->GetZaxis()->SetTitle("HadronHitEdeps [MeV]");

              TString h_hadronhit_zx_name = Form("hadronhitZX_event_%d_OffAxis_%d_cm_LAr_%d_cm_throw_%d", i_iwritten,i_ND_LAr_dtctr_pos,i_ND_LAr_vtx_pos,ithrow);
              // TString h_hadronhit_zx_name = Form(" ");
              h_hadronhit_zx[n_plot] = new TH2F(h_hadronhit_zx_name,h_hadronhit_zx_name,200,-100,600,200,-500,500);
              h_hadronhit_zx[n_plot]->GetXaxis()->SetTitle("Z [cm]");
              h_hadronhit_zx[n_plot]->GetYaxis()->SetTitle("X [cm]");
              h_hadronhit_zx[n_plot]->GetZaxis()->SetTitle("HadronHitEdeps [MeV]");

              TString h_hadronhit_zy_name = Form("hadronhitZY_event_%d_OffAxis_%d_cm_LAr_%d_cm_throw_%d", i_iwritten,i_ND_LAr_dtctr_pos,i_ND_LAr_vtx_pos,ithrow);
              // TString h_hadronhit_zy_name = Form(" ");
              h_hadronhit_zy[n_plot] = new TH2F(h_hadronhit_zy_name,h_hadronhit_zy_name,200,-100,600,200,-500,500);
              h_hadronhit_zy[n_plot]->GetXaxis()->SetTitle("Z [cm]");
              h_hadronhit_zy[n_plot]->GetYaxis()->SetTitle("Y [cm]");
              h_hadronhit_zy[n_plot]->GetZaxis()->SetTitle("HadronHitEdeps [MeV]");


              // calculate energy outside the ND active volume
              Double_t CurrentThrowoutEnergyND = 0.;
              Double_t CurrentThrowvetoEnergyND = 0.;

              for(Int_t ihadronhit = 0; ihadronhit < ND_Sim_n_hadronic_Edep_b; ihadronhit++)
              {
                // cout << "ihadronhit: " << ihadronhit << ", HadronHitEdeps: " << HadronHitEdeps->at(ihadronhit) << endl;
                h_hadronhit_xy[n_plot]->Fill(ThrowDepsX[ithrow][ihadronhit] - offset_X,ThrowDepsY[ithrow][ihadronhit]-NDLAr_OnAxis_offset[1],HadronHitEdeps->at(ihadronhit));
                h_hadronhit_zx[n_plot]->Fill(ThrowDepsZ[ithrow][ihadronhit] - NDLAr_OnAxis_offset[2],ThrowDepsX[ithrow][ihadronhit] - offset_X,HadronHitEdeps->at(ihadronhit));
                h_hadronhit_zy[n_plot]->Fill(ThrowDepsZ[ithrow][ihadronhit] - NDLAr_OnAxis_offset[2],ThrowDepsY[ithrow][ihadronhit] - NDLAr_OnAxis_offset[1],HadronHitEdeps->at(ihadronhit));

                if(
                  ( ThrowDepsX[ithrow][ihadronhit] - offset_X                      < NDActiveVol_min[0] ) ||
                  ( ThrowDepsY[ithrow][ihadronhit] - NDLAr_OnAxis_offset[1]        < NDActiveVol_min[1] ) ||
                  ( ThrowDepsZ[ithrow][ihadronhit] - NDLAr_OnAxis_offset[2]        < NDActiveVol_min[2] ) ||
                  ( ThrowDepsX[ithrow][ihadronhit] - offset_X                      > NDActiveVol_max[0] ) ||
                  ( ThrowDepsY[ithrow][ihadronhit] - NDLAr_OnAxis_offset[1]        > NDActiveVol_max[1] ) ||
                  ( ThrowDepsZ[ithrow][ihadronhit] - NDLAr_OnAxis_offset[2]        > NDActiveVol_max[2] )
                ){
                  CurrentThrowoutEnergyND += HadronHitEdeps->at(ihadronhit);
                }
                else if ( ( ThrowDepsX[ithrow][ihadronhit]-offset_X                      > NDActiveVol_min[0]         && ThrowDepsX[ithrow][ihadronhit]-offset_X                      < NDActiveVol_min[0] + vSize ) ||
                     ( ThrowDepsY[ithrow][ihadronhit]-NDLAr_OnAxis_offset[1]        > NDActiveVol_min[1]         && ThrowDepsY[ithrow][ihadronhit]-NDLAr_OnAxis_offset[1]        < NDActiveVol_min[1] + vSize ) ||
                     ( ThrowDepsZ[ithrow][ihadronhit]-NDLAr_OnAxis_offset[2]        > NDActiveVol_min[2]         && ThrowDepsZ[ithrow][ihadronhit]-NDLAr_OnAxis_offset[2]        < NDActiveVol_min[2] + vSize ) ||
                     ( ThrowDepsX[ithrow][ihadronhit]-offset_X                      > NDActiveVol_max[0] - vSize && ThrowDepsX[ithrow][ihadronhit]-offset_X                      < NDActiveVol_max[0] ) ||
                     ( ThrowDepsY[ithrow][ihadronhit]-NDLAr_OnAxis_offset[1]        > NDActiveVol_max[1] - vSize && ThrowDepsY[ithrow][ihadronhit]-NDLAr_OnAxis_offset[1]        < NDActiveVol_max[1] ) ||
                     ( ThrowDepsZ[ithrow][ihadronhit]-NDLAr_OnAxis_offset[2]        > NDActiveVol_max[2] - vSize && ThrowDepsZ[ithrow][ihadronhit]-NDLAr_OnAxis_offset[2]        < NDActiveVol_max[2] )
                   ){
                     CurrentThrowvetoEnergyND += HadronHitEdeps->at(ihadronhit);
                } // end if hadron deposit in FD veto region

              }
              // find out the fraction
              if(CurrentThrowvetoEnergyND<30&&CurrentThrowoutEnergyND>30)
              {
                throw_couter++;
                // cout << " ithrow: " << ithrow << ", throw_couter: " << throw_couter << endl;
              }

              TString energy_name = Form("VetoE_%.2f_MeV, OutE_%.2f_MeV, TotE_%.2f_MeV", CurrentThrowvetoEnergyND, CurrentThrowoutEnergyND, CurrentThrowTotE->at(ithrow));
              TString c_hadronhit_name = Form("c_hadronhit_event_%d_OffAxis_%d_cm_LAr_%d_cm_throw_%d", i_iwritten,i_ND_LAr_dtctr_pos,i_ND_LAr_vtx_pos,ithrow);
              TString c_hadronhit_title = Form("hadronhit event_%d_OffAxis_%d_cm_LAr_%d_cm_throw_%d", i_iwritten,i_ND_LAr_dtctr_pos,i_ND_LAr_vtx_pos,ithrow);
              c_hadronhit[n_plot] = new TCanvas(c_hadronhit_name, c_hadronhit_title, 0,53,995,597);
              c_hadronhit[n_plot]->Clear();
              c_hadronhit[n_plot]->Range(-666.6667,-625,1000,625);
              c_hadronhit[n_plot]->SetLeftMargin(0.10);
              c_hadronhit[n_plot]->SetRightMargin(0.10);
              c_hadronhit[n_plot]->Divide(2,2);

              c_hadronhit[n_plot]->cd(1);
              c_hadronhit[n_plot]->GetPad(1)->SetRightMargin(.15);
              h_hadronhit_xy[n_plot]->Draw("COLZ");
              auto *xy_box = new TBox(NDActiveVol_min[0],NDActiveVol_min[1],NDActiveVol_max[0],NDActiveVol_max[1]);
              xy_box->SetLineColor(kBlack);
              xy_box->SetLineWidth(2);
              xy_box->SetFillStyle(0);
              xy_box->Draw();
              auto *xy_box1 = new TBox(NDActiveVol_min[0]+30,NDActiveVol_min[1]+30,NDActiveVol_max[0]-30,NDActiveVol_max[1]-30);
              xy_box1->SetLineColor(kBlue);
              xy_box1->SetLineWidth(2);
              xy_box1->SetFillStyle(0);
              xy_box1->Draw();
              auto *xy_box2 = new TBox(ND_FV_min[0],ND_FV_min[1],ND_FV_max[0],ND_FV_max[1]);
              xy_box2->SetLineColor(kRed);
              xy_box2->SetLineWidth(2);
              xy_box2->SetFillStyle(0);
              xy_box2->Draw();
              TLatex xy_text(-400,400,energy_name);
              xy_text.DrawClone();

              c_hadronhit[n_plot]->cd(2);
              c_hadronhit[n_plot]->GetPad(2)->SetRightMargin(.15);
              h_hadronhit_zx[n_plot]->Draw("COLZ");
              // ND active volume
              auto *zx_box = new TBox(NDActiveVol_min[2],NDActiveVol_min[0],NDActiveVol_max[2],NDActiveVol_max[0]);
              zx_box->SetLineColor(kBlack);
              zx_box->SetLineWidth(2);
              zx_box->SetFillStyle(0);
              zx_box->Draw();
              // ND veto volume
              auto *zx_box1 = new TBox(NDActiveVol_min[2]+30,NDActiveVol_min[0]+30,NDActiveVol_max[2]-30,NDActiveVol_max[0]-30);
              zx_box1->SetLineColor(kBlue);
              zx_box1->SetLineWidth(2);
              zx_box1->SetFillStyle(0);
              zx_box1->Draw();
              // ND fiducial volume
              auto *zx_box2 = new TBox(ND_FV_min[2],ND_FV_min[0],ND_FV_max[2],ND_FV_max[0]);
              zx_box2->SetLineColor(kRed);
              zx_box2->SetLineWidth(2);
              zx_box2->SetFillStyle(0);
              zx_box2->Draw();
              // Draw Veto E and Tot E
              TLatex xz_text(-50,400,energy_name);
              xz_text.DrawClone();

              c_hadronhit[n_plot]->cd(3);
              c_hadronhit[n_plot]->GetPad(3)->SetRightMargin(.15);
              h_hadronhit_zy[n_plot]->Draw("COLZ");
              auto *yz_box = new TBox(NDActiveVol_min[2],NDActiveVol_min[1],NDActiveVol_max[2],NDActiveVol_max[1]);
              yz_box->SetLineColor(kBlack);
              yz_box->SetLineWidth(2);
              yz_box->SetFillStyle(0);
              yz_box->Draw();
              auto *yz_box1 = new TBox(NDActiveVol_min[2]+30,NDActiveVol_min[1]+30,NDActiveVol_max[2]-30,NDActiveVol_max[1]-30);
              yz_box1->SetLineColor(kBlue);
              yz_box1->SetLineWidth(2);
              yz_box1->SetFillStyle(0);
              yz_box1->Draw();
              auto *yz_box2 = new TBox(ND_FV_min[2],ND_FV_min[1],ND_FV_max[2],ND_FV_max[1]);
              yz_box2->SetLineColor(kRed);
              yz_box2->SetLineWidth(2);
              yz_box2->SetFillStyle(0);
              yz_box2->Draw();
              // Draw Veto E and Tot E
              TLatex yz_text(-50,400,energy_name);
              yz_text.DrawClone();

              gPad->Update();
              gPad->Modified();
              gSystem->ProcessEvents();

              c_hadronhit[n_plot]->SaveAs( TString::Format("HadronHitPlots_ND/c_event_%d_hadronhit_%d_throw_%d.pdf",iwritten, n_plot,ithrow ) );

            }// end throw
            i_n_plot++;
            cout << "i_iwritten: " << i_iwritten << ", i_ND_LAr_vtx_pos: " << i_ND_LAr_vtx_pos << ", fraction: "<< throw_couter*1.0/50 << endl;
          }// end vtx selection
        }//end ientry
      }//end vtx pos inside LAr
    }//end LAr pos
  }//end iwritten

  // delete all canvas
  delete[] h_hadronhit_xy;
  delete[] h_hadronhit_zx;
  delete[] h_hadronhit_zy;
  delete[] c_hadronhit;
}
// end ND ReadHadronHitNtuple
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Draw FD hadronic hits plot
void ReadHadronHitNtuple_FD()
{
  gROOT->Reset();

  // Input FDroot file
  // TString FileIn = "/dune/app/users/weishi/DebugFDEdep/srcs/myntuples/myntuples/MyEnergyAnalysis/myntuple.root"; // one file test
  TString FileIn = "/pnfs/dune/persistent/users/flynnguo/myFDntuples/myntuple_62775806/myntuple_62775806_1104.root";

  TChain *t = new TChain("MyEnergyAnalysis/MyTree");
  t->Add(FileIn.Data());

  double FD_Vis_LepE;                      // Generator level neutrino lepton energy
  double FD_E_vis_true;                 // True vis energy [GeV]
  t->SetBranchAddress("Vis_LepE",       &FD_Vis_LepE);
  t->SetBranchAddress("E_vis_true",     &FD_E_vis_true);
  // Muon
  double FD_Gen_numu_E;                // Energy of generator level neutrino [GeV]
  double FD_Sim_mu_start_vx;           // x position of the muon trajectory start
  double FD_Sim_mu_start_vy;           // y .....................................
  double FD_Sim_mu_start_vz;           // z .....................................
  double FD_Sim_mu_start_E;            // Energy of leading mu
  double FD_Sim_mu_end_vx;             // x position of the muon trajectory end
  double FD_Sim_mu_end_vy;             // y ...................................
  double FD_Sim_mu_end_vz;             // z ...................................
  double FD_Sim_mu_Edep_b2;            // [MeV]
  /*
  double FD_Sim_mu_Edep_a1;                // muon energy deposit [GeV]: total amount of electrons reaching the readout channel
  double FD_Sim_mu_Edep_a2;                // muon energy deposit [MeV]: total amount of energy released by ionizations in the event (from Geant4 simulation)
  double FD_Sim_mu_Edep_b1;                // [GeV]
  double FD_Sim_mu_Edep_NonCollectionPlane_b2;                // [MeV]
  double FD_Sim_mu_Edep_b2_debug;          // [MeV]
  */

  t->SetBranchAddress("Gen_numu_E",               &FD_Gen_numu_E);
  t->SetBranchAddress("Sim_mu_start_vx",          &FD_Sim_mu_start_vx);
  t->SetBranchAddress("Sim_mu_start_vy",          &FD_Sim_mu_start_vy);
  t->SetBranchAddress("Sim_mu_start_vz",          &FD_Sim_mu_start_vz);
  t->SetBranchAddress("Sim_mu_start_E",           &FD_Sim_mu_start_E);
  t->SetBranchAddress("Sim_mu_end_vx",            &FD_Sim_mu_end_vx);
  t->SetBranchAddress("Sim_mu_end_vy",            &FD_Sim_mu_end_vy);
  t->SetBranchAddress("Sim_mu_end_vz",            &FD_Sim_mu_end_vz);
  t->SetBranchAddress("Sim_mu_Edep_b2",           &FD_Sim_mu_Edep_b2);
  /*
  t->SetBranchAddress("Sim_mu_Edep_a1",           &FD_Sim_mu_Edep_a1);
  t->SetBranchAddress("Sim_mu_Edep_a2",     &FD_Sim_mu_Edep_a2);
  t->SetBranchAddress("Sim_mu_Edep_b1",     &FD_Sim_mu_Edep_b1);
  t->SetBranchAddress("Sim_mu_Edep_NonCollectionPlane_b2",     &FD_Sim_mu_Edep_NonCollectionPlane_b2);
  t->SetBranchAddress("Sim_mu_Edep_b2_debug",     &FD_Sim_mu_Edep_b2_debug);
  */

  double FD_Sim_numu_E, FD_Sim_LepE, FD_Sim_HadE;   //GEANT4 Sim energy
  t->SetBranchAddress("Sim_numu_E",     &FD_Sim_numu_E);
  t->SetBranchAddress("Sim_LepE",       &FD_Sim_LepE);
  t->SetBranchAddress("Sim_HadE",       &FD_Sim_HadE);

  std::vector<int> *FD_SimP_TrackID_vec=0;
  std::vector<int> *FD_SimP_PDG_vec=0;
  std::vector<int> *FD_SimP_Mom_vec=0;
  std::vector<int> *FD_SimP_SC_vec=0;
  std::vector<float> *FD_SimP_vtx_x_vec=0;
  std::vector<float> *FD_SimP_vtx_y_vec=0;
  std::vector<float> *FD_SimP_vtx_z_vec=0;
  std::vector<float> *FD_SimP_ptot_vec=0;
  std::vector<float> *FD_SimP_px_vec=0;
  std::vector<float> *FD_SimP_py_vec=0;
  std::vector<float> *FD_SimP_pz_vec=0;
  std::vector<float> *FD_SimP_E_vec=0;
  std::vector<float> *FD_SimP_M_vec=0;
  std::vector<float> *FD_SimP_Ek_vec=0;
  t->SetBranchAddress("SimP_TrackID_vec", &FD_SimP_TrackID_vec);
  t->SetBranchAddress("SimP_PDG_vec",     &FD_SimP_PDG_vec);
  t->SetBranchAddress("SimP_Mom_vec",     &FD_SimP_Mom_vec);
  t->SetBranchAddress("SimP_SC_vec",      &FD_SimP_SC_vec);
  t->SetBranchAddress("SimP_vtx_x_vec",   &FD_SimP_vtx_x_vec);
  t->SetBranchAddress("SimP_vtx_y_vec",   &FD_SimP_vtx_y_vec);
  t->SetBranchAddress("SimP_vtx_z_vec",   &FD_SimP_vtx_z_vec);
  t->SetBranchAddress("SimP_ptot_vec",    &FD_SimP_ptot_vec);
  t->SetBranchAddress("SimP_px_vec",      &FD_SimP_px_vec);
  t->SetBranchAddress("SimP_py_vec",      &FD_SimP_py_vec);
  t->SetBranchAddress("SimP_pz_vec",      &FD_SimP_pz_vec);
  t->SetBranchAddress("SimP_E_vec",       &FD_SimP_E_vec);
  t->SetBranchAddress("SimP_M_vec",       &FD_SimP_M_vec);
  t->SetBranchAddress("SimP_Ek_vec",      &FD_SimP_Ek_vec);

  int FD_Sim_nMu; // # of Sim muons (mu+/mu-)
  int FD_CCNC_truth; // 0 =CC 1 =NC
  int FD_neuPDG; // Generator level neutrino PDG
  t->SetBranchAddress("Sim_nMu",                  &FD_Sim_nMu);
  t->SetBranchAddress("CCNC_truth",               &FD_CCNC_truth);
  t->SetBranchAddress("neuPDG",                   &FD_neuPDG);

  // Hardon hits
  int    FD_Sim_n_hadronic_Edep_b;   // # of hadronic energy deposits
  double FD_Sim_hadronic_Edep_b2;    // Total amount of energy released by ionizations in the event (from Geant4 simulation) [MeV]
  //double FD_Sim_hadronic_Edep_NonCollectionPlane_b2; // [MeV]
  //double FD_Sim_hadronic_Edep_b2_debug;          // [MeV]
  vector<float> *FD_Sim_hadronic_hit_Edep_b2 = 0; // Need initialize 0 here to avoid error
  vector<float> *FD_Sim_hadronic_hit_x_b = 0; // Position of each energy deposit on the x-axis [cm]
  vector<float> *FD_Sim_hadronic_hit_y_b = 0; // Position of each energy deposit on the y-axis [cm]
  vector<float> *FD_Sim_hadronic_hit_z_b = 0; // Position of each energy deposit on the z-axis [cm]

  //t->SetBranchAddress("Sim_hadronic_Edep_NonCollectionPlane_b2",     &FD_Sim_hadronic_Edep_NonCollectionPlane_b2);
  //t->SetBranchAddress("Sim_hadronic_Edep_b2_debug",     &FD_Sim_hadronic_Edep_b2_debug);
  t->SetBranchAddress("Sim_hadronic_Edep_b2",     &FD_Sim_hadronic_Edep_b2);
  t->SetBranchAddress("Sim_n_hadronic_Edep_b",    &FD_Sim_n_hadronic_Edep_b);
  t->SetBranchAddress("Sim_hadronic_hit_Edep_b2", &FD_Sim_hadronic_hit_Edep_b2);
  t->SetBranchAddress("Sim_hadronic_hit_x_b",     &FD_Sim_hadronic_hit_x_b);
  t->SetBranchAddress("Sim_hadronic_hit_y_b",     &FD_Sim_hadronic_hit_y_b);
  t->SetBranchAddress("Sim_hadronic_hit_z_b",     &FD_Sim_hadronic_hit_z_b);


  // add true info for each particle
  double FD_LepNuAngle;                // Angle b/w nu and lepton
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
  std::vector<int>   *FD_P_mother = 0;                   // Find the mother particle
  std::vector<int>   *FD_P_TrackID = 0;
  t->SetBranchAddress("LepNuAngle",               &FD_LepNuAngle);
  t->SetBranchAddress("P_PDG",     &FD_P_PDG);
  t->SetBranchAddress("P_num",     &FD_P_num);
  t->SetBranchAddress("P_TrackID",     &FD_P_TrackID);
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
  t->SetBranchAddress("P_mother",       &FD_P_mother);

  // True info for energy
  double FD_True_HadE;                              // True had E by adding all fP_E (!=lepton)
  double FD_True_LepE;                              // True Lep E by adding all fP_E (==lepton)
  double FD_Vis_HadE;                               // Visible had E
  t->SetBranchAddress("True_HadE",       &FD_True_HadE);
  t->SetBranchAddress("True_LepE",       &FD_True_LepE);
  t->SetBranchAddress("Vis_HadE",        &FD_Vis_HadE);

  // neutron info
  int FD_nN;         // # of neutron
  double FD_eN;      // Energy of neutron
  t->SetBranchAddress("nN",       &FD_nN);
  t->SetBranchAddress("eN",       &FD_eN);


  Int_t nentries = t->GetEntries();
  cout << "nentries: " << nentries << endl;

  TCanvas** c_hadronhit = new TCanvas*[nentries];
  TH2F** h_hadronhit_xy = new TH2F*[nentries];
  TH2F** h_hadronhit_zx = new TH2F*[nentries];
  TH2F** h_hadronhit_zy = new TH2F*[nentries];

  // Set Palette
  gStyle->SetPalette(55);
  gStyle->SetOptStat(0); // Remove Stat Box
  // gStyle->SetOptStat("nemou");
  // gStyle->SetStatX(0.85);		//Stat box x position (top right hand corner)
  // gStyle->SetStatY(0.9); 		//Stat box y position
  // gStyle->SetStatW(0.2);	 		//Stat box width as fraction of pad size
  // gStyle->SetStatH(0.15);	 		//Size of each line in stat box

  // Print out the data
  ofstream myfile;
   myfile.open ("Output_FD_HadronhitCheck_FDGeoEff_2293930_80.txt");


  // create histograms
  for ( int ientry = 0; ientry < nentries; ientry++ )
  // for ( int ientry = 0; ientry < 300; ientry++ )
  {
    t->GetEntry(ientry);
    if ( FD_Sim_nMu == 0 || FD_Sim_n_hadronic_Edep_b == 0 ) continue;
    if ( FD_CCNC_truth == 1) continue;   // only use CC events
    if ( abs(FD_neuPDG) != 14 ) continue;       // only use muon neu
    cout << "hist: " << ientry << endl;

    TString h_hadronhit_xy_name = Form("hadronhitXY_event_%d", ientry);
    // TString h_hadronhit_xy_name = Form(" ");
    h_hadronhit_xy[ientry] = new TH2F(h_hadronhit_xy_name,h_hadronhit_xy_name,200,-500,500,200,-800,800);
    h_hadronhit_xy[ientry]->GetXaxis()->SetTitle("X [cm]");
    h_hadronhit_xy[ientry]->GetYaxis()->SetTitle("Y [cm]");
    h_hadronhit_xy[ientry]->GetZaxis()->SetTitle("HadronHitEdeps [MeV]");

    TString h_hadronhit_zx_name = Form("hadronhitZX_event_%d", ientry);
    // TString h_hadronhit_zx_name = Form(" ");
    h_hadronhit_zx[ientry] = new TH2F(h_hadronhit_zx_name,h_hadronhit_zx_name,200,-100,1500,200,-500,500);
    h_hadronhit_zx[ientry]->GetXaxis()->SetTitle("Z [cm]");
    h_hadronhit_zx[ientry]->GetYaxis()->SetTitle("X [cm]");
    h_hadronhit_zx[ientry]->GetZaxis()->SetTitle("HadronHitEdeps [MeV]");

    TString h_hadronhit_zy_name = Form("hadronhitZY_event_%d", ientry);
    // TString h_hadronhit_zy_name = Form(" ");
    h_hadronhit_zy[ientry] = new TH2F(h_hadronhit_zy_name,h_hadronhit_zy_name,200,-100,1500,200,-800,800);
    h_hadronhit_zy[ientry]->GetXaxis()->SetTitle("Z [cm]");
    h_hadronhit_zy[ientry]->GetYaxis()->SetTitle("Y [cm]");
    h_hadronhit_zy[ientry]->GetZaxis()->SetTitle("HadronHitEdeps [MeV]");

  }

  // fill events

  for ( int ientry = 0; ientry < nentries; ientry++ )
  // for ( int ientry = 0; ientry < 300; ientry++ )
  {
    t->GetEntry(ientry);
    if ( FD_Sim_nMu == 0 || FD_Sim_n_hadronic_Edep_b == 0 ) continue;
    if ( FD_CCNC_truth == 1) continue;   // only use CC events
    if ( abs(FD_neuPDG) != 14 ) continue;       // only use muon neu
    cout << "fill: " << ientry << endl;

    if(myfileVerbose)
    {
      myfile<< "ientry: " << ientry << "\n\n";
    }

    double vetoEnergyFD = 0.;
    for(Int_t ihadronhit =0; ihadronhit < FD_Sim_n_hadronic_Edep_b; ihadronhit++)
    {
      h_hadronhit_xy[ientry]->Fill(FD_Sim_hadronic_hit_x_b->at(ihadronhit), FD_Sim_hadronic_hit_y_b->at(ihadronhit),FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit));
      h_hadronhit_zx[ientry]->Fill(FD_Sim_hadronic_hit_z_b->at(ihadronhit), FD_Sim_hadronic_hit_x_b->at(ihadronhit),FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit));
      h_hadronhit_zy[ientry]->Fill(FD_Sim_hadronic_hit_z_b->at(ihadronhit), FD_Sim_hadronic_hit_y_b->at(ihadronhit),FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit));
      if(myfileVerbose)
      {
        myfile << "ientry: " << ientry << ", ihadronhit: " << ihadronhit << "\n";
        myfile << "FD_Sim_hadronic_hit_x_b: " << FD_Sim_hadronic_hit_x_b->at(ihadronhit) << "\n";
        myfile << "FD_Sim_hadronic_hit_y_b: " << FD_Sim_hadronic_hit_y_b->at(ihadronhit) << "\n";
        myfile << "FD_Sim_hadronic_hit_z_b: " << FD_Sim_hadronic_hit_z_b->at(ihadronhit) << "\n\n";

      }
    } // end ihadronhit
    cout << "ientry: " << ientry << endl;
  } // end ientry

  // draw histograms
  for ( int ientry = 0; ientry < nentries; ientry++ )
  // for ( int ientry = 0; ientry < 300; ientry++ )
  {
    // if ( ientry!=7&& ientry!=16 && ientry!=36 && ientry!=44 && ientry!=45 && ientry!=47 && ientry!=49 && ientry!=55 ) continue;


    t->GetEntry(ientry);
    if ( FD_Sim_nMu == 0 || FD_Sim_n_hadronic_Edep_b == 0 ) continue;
    if ( FD_CCNC_truth == 1) continue;   // only use CC events
    if ( abs(FD_neuPDG) != 14 ) continue;       // only use muon neu

    double vetoEnergyFD = 0.;
    double totEnergyFD = 0.;
    for(Int_t ihadronhit =0; ihadronhit < FD_Sim_n_hadronic_Edep_b; ihadronhit++)
    {
      totEnergyFD += FD_Sim_hadronic_hit_Edep_b2->at(ihadronhit);
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
    } // end ihadronhit
    cout << "ientry: " << ientry << ", E total: " << totEnergyFD << "\n\n";
    myfile << "ientry: " << ientry<< ", E total: " << totEnergyFD  << ", veto E total: " << vetoEnergyFD << "\n\n";
    TString energy_name = Form("VetoE_%.2f_MeV, TotE_%.2f_MeV", vetoEnergyFD, totEnergyFD);
    TString c_hadronhit_name = Form("c_hadronhit_event_%d", ientry);
    TString c_hadronhit_title = Form("hadronhit event_%d", ientry);
    c_hadronhit[ientry] = new TCanvas(c_hadronhit_name, c_hadronhit_title, 0,53,995,597);
    c_hadronhit[ientry]->Clear();
    c_hadronhit[ientry]->Range(-666.6667,-625,1000,625);
    c_hadronhit[ientry]->SetLeftMargin(0.10);
    c_hadronhit[ientry]->SetRightMargin(0.10);
    c_hadronhit[ientry]->Divide(2,2);

    // xy plot
    c_hadronhit[ientry]->cd(1);
    c_hadronhit[ientry]->GetPad(1)->SetRightMargin(.15);
    h_hadronhit_xy[ientry]->Draw("COLZ");
    auto *xy_box = new TBox(FDActiveVol_min[0],FDActiveVol_min[1],FDActiveVol_max[0],FDActiveVol_max[1]);
    xy_box->SetLineColor(kBlack);
    xy_box->SetLineWidth(1);
    xy_box->SetFillStyle(0);
    xy_box->Draw();
    auto *xy_box1 = new TBox(FDActiveVol_min[0]+vSize,FDActiveVol_min[1]+vSize,FDActiveVol_max[0]-vSize,FDActiveVol_max[1]-vSize);
    xy_box1->SetLineColor(kBlue);
    xy_box1->SetLineWidth(1);
    xy_box1->SetFillStyle(0);
    xy_box1->Draw();
    auto *xy_box2 = new TBox(FD_FV_min[0],FD_FV_min[1],FD_FV_max[0],FD_FV_max[1]);
    xy_box2->SetLineColor(kRed);
    xy_box2->SetLineWidth(1);
    xy_box2->SetFillStyle(0);
    xy_box2->Draw();
    TLatex xy_text(-400,640,energy_name);
    xy_text.DrawClone();

    // zx plot
    c_hadronhit[ientry]->cd(2);
    c_hadronhit[ientry]->GetPad(2)->SetRightMargin(.15);
    h_hadronhit_zx[ientry]->Draw("COLZ");
    // ND active volume
    auto *zx_box = new TBox(FDActiveVol_min[2],FDActiveVol_min[0],FDActiveVol_max[2],FDActiveVol_max[0]);
    zx_box->SetLineColor(kBlack);
    zx_box->SetLineWidth(1);
    zx_box->SetFillStyle(0);
    zx_box->Draw();
    // ND veto volume
    auto *zx_box1 = new TBox(FDActiveVol_min[2]+vSize,FDActiveVol_min[0]+vSize,FDActiveVol_max[2]-vSize,FDActiveVol_max[0]-vSize);
    zx_box1->SetLineColor(kBlue);
    zx_box1->SetLineWidth(1);
    zx_box1->SetFillStyle(0);
    zx_box1->Draw();
    // ND fiducial volume
    auto *zx_box2 = new TBox(FD_FV_min[2],FD_FV_min[0],FD_FV_max[2],FD_FV_max[0]);
    zx_box2->SetLineColor(kRed);
    zx_box2->SetLineWidth(1);
    zx_box2->SetFillStyle(0);
    zx_box2->Draw();
    TLatex zx_text(0,400,energy_name);
    zx_text.DrawClone();

    // yz plot
    c_hadronhit[ientry]->cd(3);
    c_hadronhit[ientry]->GetPad(3)->SetRightMargin(.15);
    h_hadronhit_zy[ientry]->Draw("COLZ");
    auto *yz_box = new TBox(FDActiveVol_min[2],FDActiveVol_min[1],FDActiveVol_max[2],FDActiveVol_max[1]);
    yz_box->SetLineColor(kBlack);
    yz_box->SetLineWidth(1);
    yz_box->SetFillStyle(0);
    yz_box->Draw();
    auto *yz_box1 = new TBox(FDActiveVol_min[2]+vSize,FDActiveVol_min[1]+vSize,FDActiveVol_max[2]-vSize,FDActiveVol_max[1]-vSize);
    yz_box1->SetLineColor(kBlue);
    yz_box1->SetLineWidth(1);
    yz_box1->SetFillStyle(0);
    yz_box1->Draw();
    auto *yz_box2 = new TBox(FD_FV_min[2],FD_FV_min[1],FD_FV_max[2],FD_FV_max[1]);
    yz_box2->SetLineColor(kRed);
    yz_box2->SetLineWidth(1);
    yz_box2->SetFillStyle(0);
    yz_box2->Draw();
    TLatex yz_text(0,640,energy_name);
    yz_text.DrawClone();



    // text plot
    if (false)
    {
      c_hadronhit[ientry]->cd(4);
      gPad->DrawFrame(0.,0.,60.,10.);
      TString line1 = Form("VetoE_%.4f_MeV, Angleb/wLep&Nu_%.4f_Radians", vetoEnergyFD, FD_LepNuAngle);
      TString line2 = Form("GenNumuE_%.4f_GeV", FD_Gen_numu_E );
      TLatex text1(1,9.5,line1);
      TLatex text2(1,8.9,line2);
      text1.SetTextSize(0.04);
      text2.SetTextSize(0.04);
      text1.DrawClone();
      text2.DrawClone();

      TString line3 = Form("FD_True_LepE_%.4f_GeV, FD_True_HadE_%.4f_GeV, FD_True_E_%.4f_GeV", FD_True_LepE, FD_True_HadE, FD_True_LepE+FD_True_HadE);
      TLatex text3(1,8.3,line3);
      text3.SetTextSize(0.04);
      text3.DrawClone();

      TString line4 = Form("FD_BindingE_%.4f_GeV", FD_Gen_numu_E - (FD_True_LepE+FD_True_HadE) );
      TLatex text4(1,7.7,line4);
      text4.SetTextSize(0.04);
      text4.DrawClone();

      // TString line4 = Form("FD_Vis_LepE_%.2f_GeV, FD_Vis_HadE_%.2f_GeV, FD_Vis_E_%.2f_GeV", FD_Vis_LepE, FD_Vis_HadE, FD_Vis_HadE+FD_Vis_LepE );
      // TLatex text4(1,7.7,line4);
      // text4.SetTextSize(0.04);
      // text4.DrawClone();

      TString line5 = Form("FD_#ofNeutron_%.2d, FD_KEofNeutron_%.4f_GeV", FD_nN, FD_eN );
      TLatex text5(1,7.1,line5);
      text5.SetTextSize(0.04);
      text5.DrawClone();

      TString line6 = Form("FD_DepMuE_%.4f_MeV, FD_DepHadE_%.4f_MeV, FD_TotDepE_%.4f_MeV", FD_Sim_mu_Edep_b2, FD_Sim_hadronic_Edep_b2, FD_Sim_hadronic_Edep_b2+FD_Sim_mu_Edep_b2);
      TLatex text6(1,6.5,line6);
      text6.SetTextSize(0.04);
      text6.DrawClone();

      /*
      TString line61 = Form("FD_NCP_DepMuE_%.4f_MeV, FD_NCP_DepHadE_%.4f_MeV", FD_Sim_mu_Edep_NonCollectionPlane_b2, FD_Sim_hadronic_Edep_NonCollectionPlane_b2);
      TLatex text61(1,5.9,line61);
      text61.SetTextSize(0.04);
      text61.DrawClone();


      TString line62 = Form("FD_NCP_TotDepE_%.4f_MeV", FD_Sim_mu_Edep_NonCollectionPlane_b2+FD_Sim_hadronic_Edep_NonCollectionPlane_b2);
      TLatex text62(1,5.3,line62);
      text62.SetTextSize(0.04);
      text62.DrawClone();


      TString line63 = Form("FD_DepMuE_debug_%.4f_MeV, FD_DepHadE_debug_%.4f_MeV", FD_Sim_mu_Edep_b2_debug, FD_Sim_hadronic_Edep_b2_debug);
      TLatex text63(1,4.7,line63);
      text63.SetTextSize(0.04);
      text63.DrawClone();

      TString line64 = Form("FD_DepTotE_debug_%.4f_MeV", FD_Sim_mu_Edep_b2_debug+FD_Sim_hadronic_Edep_b2_debug);
      TLatex text64(1,4.1,line64);
      text64.SetTextSize(0.04);
      text64.DrawClone();
      */

      // TString line7 = Form("FD_MuonMass_105.66_MeV, FD_DepMuE+FD_MuonMass: %.2f_MeV", FD_Sim_mu_Edep_b2+105.66);
      // TLatex text7(1,5.9,line7);
      // text7.SetTextSize(0.04);
      // text7.DrawClone();

      TString line71 = Form("FD_True_LepE - (FD_DepMuE+FD_MuonMass): %.2f_MeV", FD_True_LepE*1000-(FD_Sim_mu_Edep_b2+105.66));
      TLatex text71(1,3.5,line71);
      text71.SetTextSize(0.04);
      text71.DrawClone();

      TString line8 = Form("FD_SimMuE_%.2f_GeV, FD_SimHadE_%.2f_GeV, FD_SimE_sum_%.2f_GeV", FD_Sim_LepE, FD_Sim_HadE, FD_Sim_HadE+FD_Sim_LepE);
      TLatex text8(1,2.9,line8);
      text8.SetTextSize(0.04);
      text8.DrawClone();


      TString line9 = Form("FD_SimMu_Start: Vx_%.2f_cm, Vy_%.2f_cm, Vz_%.2f_cm", FD_Sim_mu_start_vx, FD_Sim_mu_start_vy, FD_Sim_mu_start_vz);
      TLatex text9(1,1.3,line9);
      text9.SetTextSize(0.04);
      text9.DrawClone();

      TString line10 = Form("FD_SimMu_End: Vx_%.2f_cm, Vy_%.2f_cm, Vz_%.2f_cm", FD_Sim_mu_end_vx, FD_Sim_mu_end_vy, FD_Sim_mu_end_vz);
      TLatex text10(1,0.7,line10);
      text10.SetTextSize(0.04);
      text10.DrawClone();
    }

    // Save true information
    // if(ientry>-1)
    // {
      for (int p = 0; p < FD_P_num; p++)
      {
        myfile << "ientry: " << ientry << ", FD_P_TrackID: " << FD_P_TrackID->at(p) << "\n";
        myfile << "FD_P_PDG: " << FD_P_PDG->at(p) << "\n";
        myfile << "FD_P_mother: " << FD_P_mother->at(p) << "\n";
        myfile << "FD_P_StatusCode: " << FD_P_StatusCode->at(p) << "\n";
        myfile << "FD_P_vtx_x: " << FD_P_vtx_x->at(p) << "\n";
        myfile << "FD_P_vtx_y: " << FD_P_vtx_y->at(p) << "\n";
        myfile << "FD_P_vtx_z: " << FD_P_vtx_z->at(p) << "\n";
        myfile << "FD_P_ptot: " << FD_P_ptot->at(p) << "\n";
        myfile << "FD_P_px: " << FD_P_px->at(p) << "\n";
        myfile << "FD_P_py: " << FD_P_py->at(p) << "\n";
        myfile << "FD_P_pz: " << FD_P_pz->at(p) << "\n";
        myfile << "FD_P_E: " << FD_P_E->at(p) << "\n";
        myfile << "FD_P_mass: " << FD_P_mass->at(p) << "\n";
        myfile << "FD_P_Ek: " << FD_P_Ek->at(p) << "\n\n";
      }
      for(int i=0; i<FD_SimP_PDG_vec->size();i++)
      {
        myfile << "ientry: " << ientry << ", SimP_TrackID: " << FD_SimP_TrackID_vec->at(i) << "\n";
        myfile << "SimP_PDG: " << FD_SimP_PDG_vec->at(i) << "\n";
        myfile << "SimP_Mother: " << FD_SimP_Mom_vec->at(i) << "\n";
        myfile << "SimP_StatusCode: " << FD_SimP_SC_vec->at(i) << "\n";
        myfile << "SimP_vtx_x_vec: " << FD_SimP_vtx_x_vec->at(i) << "\n";
        myfile << "SimP_vtx_y_vec: " << FD_SimP_vtx_y_vec->at(i) << "\n";
        myfile << "SimP_vtx_z_vec: " << FD_SimP_vtx_z_vec->at(i) << "\n";
        myfile << "SimP_ptot_vec: " << FD_SimP_ptot_vec->at(i) << "\n";
        myfile << "SimP_px_vec: " << FD_SimP_px_vec->at(i) << "\n";
        myfile << "SimP_py_vec: " << FD_SimP_py_vec->at(i) << "\n";
        myfile << "SimP_pz_vec: " << FD_SimP_pz_vec->at(i) << "\n";
        myfile << "SimP_E: " << FD_SimP_E_vec->at(i) << "\n";
        myfile << "SimP_Mass: " << FD_SimP_M_vec->at(i) << "\n";
        myfile << "SimP_Ek: " << FD_SimP_Ek_vec->at(i) << "\n\n";
      }
    // }

    //
    gPad->Update();
    gPad->Modified();
    gSystem->ProcessEvents();

    c_hadronhit[ientry]->SaveAs( TString::Format("HadronHitPlots_FD/c_event_%d_hadronhit.pdf",ientry) );
  }

  delete[] h_hadronhit_xy;
  delete[] h_hadronhit_zx;
  delete[] h_hadronhit_zy;
  delete[] c_hadronhit;

  myfile.close();
} // end FD ReadHadronHitNtuple
