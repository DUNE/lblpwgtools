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
#include "step3_helpers.h"

float vSize = 30.;

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

// Draw ND hadronic hits plot
void ReadNtupleMakeHistos()
{
  gROOT->Reset();
  //gStyle->SetOptStat(0); // Remove Stat Box

  // Input FDroot file
  TString FileIn = "Output_FDGeoEff_vetoEEventsPass.root";
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
  int validThrows;
  vector<uint64_t> *ThrowResults = nullptr; //all throw results
  vector<float> *VetoEnergyEventsPass = nullptr; // veto energy of events that pass the throw
  t_effValues->SetBranchAddress("iwritten",         &iwritten);
  t_effValues->SetBranchAddress("ND_LAr_dtctr_pos",   &ND_LAr_dtctr_pos);
  t_effValues->SetBranchAddress("ND_LAr_vtx_pos",       &ND_LAr_vtx_pos);
  t_effValues->SetBranchAddress("ND_GeoEff",        &ND_GeoEff);
  t_effValues->SetBranchAddress("ThrowResults",      &ThrowResults);
  t_effValues->SetBranchAddress("validThrows",  &validThrows);
  t_effValues->SetBranchAddress("VetoEnergyEventsPass", &VetoEnergyEventsPass);
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
  // t_effTree->SetBranchAddress("ND_OffAxis_Sim_hadronic_hit_xyz",            &ND_OffAxis_Sim_hadronic_hit_xyz);


  // Read PosVec
  TChain *t_PosVec = new TChain("effPosND");
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

  cout<<" tot size: "<<tot_size<<endl;
  Int_t nentrieseffTree = t_effTree->GetEntries();


  for(Int_t i = 0; i< nentrieseffTree; i++){
    t_effValues->GetEntry(i);

    if (i==0){
      int count = 0;
      cout<<" Entry: "<<i<<endl;
      for(float valueVeto : *VetoEnergyEventsPass){
        cout<<" veto E: "<<valueVeto<<endl;
        if(valueVeto > 30.00)
          cout<<" more than 30 not correct!! "<<endl;
      }
      for(uint64_t value : *ThrowResults){
        count+=1;
        // if(value == 65536)
        // cout<<"count: "<<count << " results: "<<value<<endl;
      }
    }
  }
  // vector<Int_t> a_ND_off_axis_pos_vec = {-2800, -1600, 0};
  vector<Int_t> a_ND_off_axis_pos_vec = {0};
  vector<Double_t> a_ND_vtx_vx_vec;
  // Generate six evenly spaced point in each non-dead region
  vector<pair<double, double>> non_dead_regions = {
      {-300, -256.55}, {-253.95, -204.95}, {-203.45, -154.45}, {-151.85, -102.85},
      {-101.35, -52.35}, {-49.75, -0.75}, {0.75, 49.75}, {52.35, 101.35},
      {102.85, 151.85}, {154.45, 203.45}, {204.95, 253.95}, {256.55, 300}
  };

  // Iterate over each non-dead region and generate points
  for (const auto& region : non_dead_regions) {
      auto points = generatePoints(region.first);
      // std::cout << "Points between " << region.first << " and " << region.second << ": ";
      for (auto point : points) {
          std::cout << point << " ";
          a_ND_vtx_vx_vec.emplace_back(point);
      }
      std::cout << std::endl;
  }

  // Define the non-dead regions
  if (true)
  {for (auto x : a_ND_vtx_vx_vec)
        std::cout << x << ",  ";}
//  vector<Int_t> a_ND_vtx_vx_vec = {-299, -292, -285, -278, -271, -264, -216, -168, -120, -72, -24, 24, 72, 120, 168, 216, 264, 271, 278, 285, 292, 299};
  // vector<Int_t> a_ND_vtx_vx_vec = {-168, -216};

  // Set Palette
  gStyle->SetPalette(55);
//  gStyle->SetOptStat(0);

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

  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  // ND event display for throws w.r.t different vertex positions
  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  //
  TCanvas** c_hadronhit = new TCanvas*[hadronhit_n_plots];
  TH2F** h_hadronhit_xy = new TH2F*[hadronhit_n_plots];
  TH2F** h_hadronhit_zx = new TH2F*[hadronhit_n_plots];
  TH2F** h_hadronhit_zy = new TH2F*[hadronhit_n_plots];

  int nFDEvents = iwritten_vec->size();
  int nvtxXpositions = a_ND_vtx_vx_vec.size();
  cout<<" nFDEvents = "<<nFDEvents<<" nXpos = "<<nvtxXpositions<<endl;
  TH1D* HistEout[nFDEvents][nvtxXpositions];
  TH1D* HistVetoE[nFDEvents][nvtxXpositions];
  TH1D* HistEtrim[nFDEvents][nvtxXpositions];
  TH1D* HistEtrimAllVtxX[nFDEvents];
  // TH1D** HistVetoE = new TH1D*[nvtxXpositions];
  // TH1D** HistEtrim = new TH1D*[nvtxXpositions];
  // TCanvas* CanvasOutE[nFDEvents][nvtxXpositions];
  // TCanvas* CanvasVetoE[nFDEvents][nvtxXpositions];
  // TCanvas* CanvasTrimE[nFDEvents][nvtxXpositions];
  TCanvas** CanvasOutE = new TCanvas*[nvtxXpositions];
  TCanvas** CanvasVetoE = new TCanvas*[nvtxXpositions];
  TCanvas** CanvasTrimE = new TCanvas*[nvtxXpositions];

  TCanvas* CanvasEfficiencyVsVtxX[nFDEvents];

  TGraph* PlotEfficiencyVsVtxX[nFDEvents];

  //TFile* FileWithHistoInfo = new TFile("FileWithHistoInfo.root", "UPDATE");

  // for (Int_t i_iwritten : *iwritten_vec)
  for (Int_t i_iwritten = 0; i_iwritten<1; i_iwritten++)
  {
    Double_t x_ND_LAr_vtx_pos[ND_vtx_vx_vec_size];
    Double_t y_geoeff[ND_vtx_vx_vec_size];

    TString HistEtrimAllVtxX_name = Form("HistEtrimAllVtxX_FDEvt_%d", i_iwritten);
    HistEtrimAllVtxX[i_iwritten] = new TH1D(HistEtrimAllVtxX_name, HistEtrimAllVtxX_name, 10000000, 700, 800);

    //choose specific iwrittenR
    // if (i_iwritten != 2) continue;
    // cout << "i_iwritten: " << i_iwritten << "\n";

    Int_t n_plot = 0;
    Int_t i_n_plot = 0;
    Int_t i_vtxX_plot=0;
    for (Int_t i_ND_LAr_dtctr_pos: a_ND_off_axis_pos_vec)
    {
      for (Double_t i_ND_LAr_vtx_pos: a_ND_vtx_vx_vec)
      {
        // TString HistEout_name = Form("HistEout_FDEvt_%d_vtxXpos_%d", i_iwritten, i_ND_LAr_vtx_pos);
        // HistEout[i_ND_LAr_vtx_pos] = new TH1D(HistEout_name, HistEout_name, 100, 0, 10000);
        // TString HistVetoE_name = Form("HistVetoE_FDEvt_%d_vtxXpost_%d", i_iwritten, i_ND_LAr_vtx_pos);
        // HistVetoE[i_ND_LAr_vtx_pos] = new TH1D(HistVetoE_name, HistVetoE_name, 100, 0, 10000);
        // TString HistEtrim_name = Form("HistEtrim_FDEvt_%d_vtxXpost_%d", i_iwritten, i_ND_LAr_vtx_pos);
        // HistEtrim[i_ND_LAr_vtx_pos] = new TH1D(HistEtrim_name, HistEtrim_name, 100, 0, 10000);


        i_vtxX_plot +=1;
        TString HistEout_name = Form("HistEout_FDEvt_%d_vtxXpos_%f", i_iwritten, i_ND_LAr_vtx_pos);
        HistEout[i_iwritten][i_vtxX_plot-1] = new TH1D(HistEout_name, HistEout_name, 100, 0, 10000);
        TString HistVetoE_name = Form("HistVetoE_FDEvt_%d_vtxXpost_%f", i_iwritten, i_ND_LAr_vtx_pos);
        HistVetoE[i_iwritten][i_vtxX_plot-1] = new TH1D(HistVetoE_name, HistVetoE_name, 100, 0, 10000);
        TString HistEtrim_name = Form("HistEtrim_FDEvt_%d_vtxXpost_%f", i_iwritten, i_ND_LAr_vtx_pos);
        HistEtrim[i_iwritten][i_vtxX_plot-1] = new TH1D(HistEtrim_name, HistEtrim_name, 100, 0, 10000);



        Int_t i_entry = tot_size * i_iwritten;
        //cout<<" i entry: "<<i_entry<<endl;
        for (i_entry ; i_entry < tot_size * (i_iwritten+1); i_entry++ )
        {
          t_effTree->GetEntry(i_entry);
          t_effValues->GetEntry(i_entry);

          // for(uint64_t value : *ThrowResults)
          //   cout<<" throw value : "<<value<<endl;
          //cout<<" i_ND_LAr_dtctr_pos "<<i_ND_LAr_dtctr_pos<<" ND_LAr_dtctr_pos "<<ND_LAr_dtctr_pos<<endl;
          //cout<<" i_nd_lar_vrx_pos: "<<i_ND_LAr_vtx_pos<<" ND_LAr_vtx_pos " << ND_LAr_vtx_pos<<endl;
          // if (ND_LAr_dtctr_pos == i_ND_LAr_dtctr_pos && ND_LAr_vtx_pos == i_ND_LAr_vtx_pos){
          if (ND_LAr_dtctr_pos == i_ND_LAr_dtctr_pos && ND_LAr_vtx_pos == i_ND_LAr_vtx_pos && ND_LAr_vtx_pos == -298.55){
            cout<<" i_nd_lar_vrx_pos: "<<i_ND_LAr_vtx_pos<<" ND_LAr_vtx_pos " << ND_LAr_vtx_pos<<" i_entry "<< i_entry<<endl;
            // for(int i = 0; i<validThrows; i++)
            //   cout<<" throw: "<<ThrowResults->at(i)<<" valid throws: "<<validThrows<<endl;
            x_ND_LAr_vtx_pos[i_vtxX_plot-1] = ND_LAr_vtx_pos;
            y_geoeff[i_vtxX_plot-1] = ND_GeoEff;
            PlotEfficiencyVsVtxX[i_iwritten] = new TGraph(a_ND_vtx_vx_vec.size(), x_ND_LAr_vtx_pos, y_geoeff);
            cout<<" size: "<<a_ND_vtx_vx_vec.size()<<" x_ND_LAr_vtx_pos "<<x_ND_LAr_vtx_pos[i_vtxX_plot-1]<<" ef: "<<y_geoeff[i_vtxX_plot-1]<<endl;
            // cout<<" ND vtx x pos: "<<ND_LAr_vtx_pos<< " ND geo eff: "<<ND_GeoEff<<endl;

            //cout<<"here??"<<endl;
            Double_t offset_X = i_ND_LAr_dtctr_pos;
            // Store hadron hit info into vector
            vector<float> ThrowDepsX_hit;
            vector<vector<float>> ThrowDepsX;
            ThrowDepsX.clear();
            int it_throw_x_counter =0;

            for (vector<vector<float>>::iterator it_throw = CurrentThrowDepsX->begin(); it_throw!=CurrentThrowDepsX->end(); ++it_throw)
            {
              for (Int_t ihadronhit =0; ihadronhit < ND_Sim_n_hadronic_Edep_b; ihadronhit++)
              {
                 //cout << "it_throw_x_counter: " << it_throw_x_counter << ", ihadronhit: " << ihadronhit << ", hit_x: " << it_throw->at(ihadronhit) << endl;
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
                //cout << "it_throw_x_counter: " << it_throw_x_counter << ", ihadronhit: " << ihadronhit << ", hit_y: " << it_throw->at(ihadronhit) << endl;
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

            int nthrowsToLoop = 3954; //this is going to be the validThrows
            int sum=0;
          for (Int_t ithrow = 0; ithrow < nthrowsToLoop; ithrow++ )
          {
            // cout << "ithrow: " << ithrow <<endl;
            // for all events
            n_plot = i_n_plot*N_throws + ithrow;

            // calculate energy outside the ND active volume
            Double_t CurrentThrowoutEnergyND = 0.;
            Double_t CurrentThrowvetoEnergyND = 0.;

            //cout<<" total nr hadronit hits: "<<ND_Sim_n_hadronic_Edep_b<<endl;
            // if(ThrowResults->at(ithrow) !=0)
            // cout<<" throw result: "<<ThrowResults->at(ithrow)<<endl;

            if(ThrowResults->at(ithrow) !=0){
              sum+=1;
              for(Int_t ihadronhit = 0; ihadronhit < ND_Sim_n_hadronic_Edep_b; ihadronhit++)
              {
                 //cout<<"throwdeps: "<<ThrowDepsX[ithrow][ihadronhit]<<endl;

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
                    }// end if hadron deposit in FD veto region

              }
              //cout<<"sum : "<<sum<<endl;

              HistEout[i_iwritten][i_vtxX_plot-1]->Fill(CurrentThrowoutEnergyND);
              HistVetoE[i_iwritten][i_vtxX_plot-1]->Fill(CurrentThrowvetoEnergyND);
              // cout<<" tot energy: "<<CurrentThrowTotE->at(ithrow)<<endl;
              HistEtrim[i_iwritten][i_vtxX_plot-1]->Fill(CurrentThrowTotE->at(ithrow) - CurrentThrowoutEnergyND);

            }



            // cout<<" out energy: "<<CurrentThrowoutEnergyND<<" veto energy: "<< CurrentThrowvetoEnergyND
            //     <<" trim e: "<< CurrentThrowTotE->at(ithrow) - CurrentThrowoutEnergyND
            //     <<" throw result "<< ThrowResults->at(ithrow)<<endl;

          } //end throw
          TString CanvasOutEout_name = Form("CanvasEout_FDEvt_%d_vtxXpos_%d", i_iwritten, i_ND_LAr_vtx_pos);
          CanvasOutE[i_vtxX_plot-1] = new TCanvas(CanvasOutEout_name, CanvasOutEout_name, 600, 400);
          CanvasOutE[i_vtxX_plot-1]->Clear();
          CanvasOutE[i_vtxX_plot-1]->SetLeftMargin(0.15);
          CanvasOutE[i_vtxX_plot-1]->SetRightMargin(0.15);
          CanvasOutE[i_vtxX_plot-1]->cd();
          HistEout[i_iwritten][i_vtxX_plot-1]->Scale(ND_GeoEff/nthrowsToLoop);
          HistEout[i_iwritten][i_vtxX_plot-1]->Draw("hist");
          //HistEout[i_iwritten-1][i_vtxX_plot-1]->SetDirectory(0);

          TString CanvasVetoE_name = Form("CanvasVetoE_FDEvt_%d_vtxXpos_%d", i_iwritten, i_ND_LAr_vtx_pos);
          CanvasVetoE[i_vtxX_plot-1] = new TCanvas(CanvasVetoE_name, CanvasVetoE_name, 600, 400);
          CanvasVetoE[i_vtxX_plot-1]->Clear();
          CanvasVetoE[i_vtxX_plot-1]->SetLeftMargin(0.15);
          CanvasVetoE[i_vtxX_plot-1]->SetRightMargin(0.15);
          CanvasVetoE[i_vtxX_plot-1]->cd();
          HistVetoE[i_iwritten][i_vtxX_plot-1]->Scale(ND_GeoEff/nthrowsToLoop);
          HistVetoE[i_iwritten][i_vtxX_plot-1]->Draw("hist");
          //
          TString CanvasTrimE_name = Form("CanvasTrimE_FDEvt_%d_vtxXpos_%d", i_iwritten, i_ND_LAr_vtx_pos);
          CanvasTrimE[i_vtxX_plot-1] = new TCanvas(CanvasTrimE_name, CanvasTrimE_name, 600, 400);
          CanvasTrimE[i_vtxX_plot-1]->Clear();
          CanvasTrimE[i_vtxX_plot-1]->SetLeftMargin(0.15);
          CanvasTrimE[i_vtxX_plot-1]->SetRightMargin(0.15);

          CanvasTrimE[i_vtxX_plot-1]->cd();
          HistEtrim[i_iwritten][i_vtxX_plot-1]->Scale(ND_GeoEff/sum);
          HistEtrim[i_iwritten][i_vtxX_plot-1]->GetYaxis()->SetTitle("norm");
          HistEtrim[i_iwritten][i_vtxX_plot-1]->GetXaxis()->SetTitle("FD E_{trim} (MeV)");
          HistEtrim[i_iwritten][i_vtxX_plot-1]->Draw("hist");



        //  cout<<" integral: "<<HistEtrim[i_iwritten][i_vtxX_plot-1]->Integral()<< " nd geo: "<<ND_GeoEff;
          }// end vtx selection
        }//end ientry
        // CanvasTrimE[i_iwritten][i_ND_LAr_vtx_pos]->Delete();
        // CanvasOutE[i_iwritten][i_ND_LAr_vtx_pos]->Delete();
        // CanvasVetoE[i_iwritten][i_ND_LAr_vtx_pos]->Delete();
        // delete HistVetoE[i_iwritten-1][i_vtxX_plot-1];
        // delete HistEout[i_iwritten-1][i_vtxX_plot-1];
      }//end vtx pos inside LAr
    }//end LAr pos

    CanvasEfficiencyVsVtxX[i_iwritten] = new TCanvas(Form("EffficiencyCanvas_FDEventNr_%d",i_iwritten));
    CanvasEfficiencyVsVtxX[i_iwritten]->cd();
    PlotEfficiencyVsVtxX[0]->SetMarkerStyle(20);
    PlotEfficiencyVsVtxX[0]->Draw("AP");

  }//end iwritten

cout<<" before delete "<<endl;


  // delete all canvas

  delete[] h_hadronhit_xy;
  delete[] h_hadronhit_zx;
  delete[] h_hadronhit_zy;
  delete[] c_hadronhit;

  delete[] CanvasVetoE;
  delete[] CanvasOutE;
  delete[] CanvasTrimE;



}
