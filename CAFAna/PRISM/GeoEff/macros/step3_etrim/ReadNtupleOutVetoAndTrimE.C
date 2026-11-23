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
void ReadNtupleOutVetoAndTrimE()
{
  gROOT->Reset();
  //gStyle->SetOptStat(0); // Remove Stat Box

  // Input FDroot file
  TString FileIn = "Output_FDGeoEff_vetoEEventsPass_TrimEEventsPass_EventsUpToEntry33_NoTrimX.root";
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
  int NPassedThrows;

  vector<uint64_t> *ThrowResults = nullptr; //all throw results
  vector<float> *VetoEnergyEventsPass = nullptr; // veto energy of events that pass the throw
  vector<float> *TrimEnergyEventsPass = nullptr;
  vector<float> *TotalEnergyEventsPass = nullptr;
  vector<float> *VtxYEventsPass = nullptr;
  vector<float> *VtxZEventsPass = nullptr;
  t_effValues->SetBranchAddress("iwritten",         &iwritten);
  t_effValues->SetBranchAddress("ND_LAr_dtctr_pos",   &ND_LAr_dtctr_pos);
  t_effValues->SetBranchAddress("ND_LAr_vtx_pos",       &ND_LAr_vtx_pos);
  t_effValues->SetBranchAddress("ND_GeoEff",        &ND_GeoEff);
  t_effValues->SetBranchAddress("ThrowResults",      &ThrowResults);
  t_effValues->SetBranchAddress("validThrows",  &validThrows);
  t_effValues->SetBranchAddress("NPassedThrows", &NPassedThrows);
  t_effValues->SetBranchAddress("VetoEnergyEventsPass", &VetoEnergyEventsPass);
  t_effValues->SetBranchAddress("TrimEnergyEventsPass",  &TrimEnergyEventsPass);
  t_effValues->SetBranchAddress("TotalEnergyEventsPass",  &TotalEnergyEventsPass);
  t_effValues->SetBranchAddress("VtxYEventsPass",  &VtxYEventsPass);
  t_effValues->SetBranchAddress("VtxZEventsPass",  &VtxZEventsPass);
  //hardon info
  TChain *t_effTree = new TChain("effTreeND");
  Float_t totEnergyFDatND_f;
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
  t_effValues->SetBranchAddress("totEnergyFDatND_f",   &totEnergyFDatND_f);
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
        //cout<<" veto E: "<<valueVeto<<endl;
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

  //mockup Coefficients (based on 1 histo..)
  vector<Double_t> Coefficients;

  // Define the non-dead regions
  // if (true)
  for (auto x : a_ND_vtx_vx_vec){
      std::cout <<" vtx: "<< x << ",  ";
      if (x<-250)
        Coefficients.emplace_back(69*1e-6);
      if(x>=-250 && x<-200)
        Coefficients.emplace_back(100*1e-6);
      if(x>=-200 && x<-150)
        Coefficients.emplace_back(20*1e-6);
      if(x>=-150 && x<-100)
        Coefficients.emplace_back(25*1e-6);
      if(x>=-100 && x<-50)
        Coefficients.emplace_back(28*1e-6);
      if(x>=-50 && x<0)
        Coefficients.emplace_back(29*1e-6);
      if(x>=0 && x< 50)
        Coefficients.emplace_back(32*1e-6);
      if(x>=50 && x<100)
        Coefficients.emplace_back(27*1e-6);
      if(x>=100 && x<150)
        Coefficients.emplace_back(19*1e-6);
      if(x>=150 && x<=300)
        Coefficients.emplace_back(10*1e-6);



  }

  TGraph* GraphCoeffVsVtxX = new TGraph(a_ND_vtx_vx_vec.size(), a_ND_vtx_vx_vec.data(),Coefficients.data());

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
  // TH1D* HistEout[nFDEvents][nvtxXpositions];
  TH1D* HistVetoE[nFDEvents][nvtxXpositions];
  TH1D* HistEtrim[nFDEvents][nvtxXpositions];
  TH1D* HistEtrimAllVtxX[nFDEvents];
  TH1D* HistEtrimAllVtxXTimesCoeff[nFDEvents];
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

  TFile* FileWithHistoInfo = new TFile("FileWithHist_EventsUpToEntry33_NoTrimX.root", "RECREATE");

  //TFile* FileWithHistoInfo = new TFile("FileWithHistoInfo.root", "UPDATE");

  // for (Int_t i_iwritten : *iwritten_vec)
  for (Int_t i_iwritten = 0; i_iwritten<4; i_iwritten++)
  {
    cout<<" i_iwritten: "<<i_iwritten<<endl;
    Double_t x_ND_LAr_vtx_pos[ND_vtx_vx_vec_size];
    Double_t y_geoeff[ND_vtx_vx_vec_size];

    TString HistEtrimAllVtxX_name = Form("HistEtrimAllVtxX_FDEvt_%d", i_iwritten);
    HistEtrimAllVtxX[i_iwritten] = new TH1D(HistEtrimAllVtxX_name, HistEtrimAllVtxX_name, 25000, 0, 25000);
    TString HistEtrimAllVtxXTimesCoeff_name = Form("HistEtrimAllVtxXTimesCoeff_FDEvt_%d", i_iwritten);
    HistEtrimAllVtxXTimesCoeff[i_iwritten] = new TH1D(HistEtrimAllVtxXTimesCoeff_name, HistEtrimAllVtxXTimesCoeff_name, 25000, 0, 25000);

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
        // TString HistEout_name = Form("HistEout_FDEvt_%d_vtxXpos_%f", i_iwritten, i_ND_LAr_vtx_pos);
        // HistEout[i_iwritten][i_vtxX_plot-1] = new TH1D(HistEout_name, HistEout_name, 100, 0, 10000);
        TString HistVetoE_name = Form("HistVetoE_FDEvt_%d_vtxXpost_%f", i_iwritten, i_ND_LAr_vtx_pos);
        HistVetoE[i_iwritten][i_vtxX_plot-1] = new TH1D(HistVetoE_name, HistVetoE_name, 60, 0, 60);
        TString HistEtrim_name = Form("HistEtrim_FDEvt_%d_vtxXpost_%f", i_iwritten, i_ND_LAr_vtx_pos);
        HistEtrim[i_iwritten][i_vtxX_plot-1] = new TH1D(HistEtrim_name, HistEtrim_name, 25000, 0, 25000);

        cout<<" coeff: "<<Coefficients[i_vtxX_plot-1]<<endl;

        Int_t i_entry = tot_size * i_iwritten;
        //cout<<" i entry: "<<i_entry<<endl;
        for (i_entry ; i_entry < tot_size * (i_iwritten+1); i_entry++ )
        {
          t_effTree->GetEntry(i_entry);
          t_effValues->GetEntry(i_entry);

          cout<<" i_iwritten "<<i_iwritten<<" totEnergyFDatND_f " <<totEnergyFDatND_f<<endl;

          // for(uint64_t value : *ThrowResults)
          //   cout<<" throw value : "<<value<<endl;
          //cout<<" i_ND_LAr_dtctr_pos "<<i_ND_LAr_dtctr_pos<<" ND_LAr_dtctr_pos "<<ND_LAr_dtctr_pos<<endl;
          //cout<<" i_nd_lar_vrx_pos: "<<i_ND_LAr_vtx_pos<<" ND_LAr_vtx_pos " << ND_LAr_vtx_pos<<endl;
          // if (ND_LAr_dtctr_pos == i_ND_LAr_dtctr_pos && ND_LAr_vtx_pos == i_ND_LAr_vtx_pos){
        //  cout<<" i_ND_LAr_vtx_pos "<< i_ND_LAr_vtx_pos<< " ND_LAr_vtx_pos: "<<ND_LAr_vtx_pos <<endl;
      if (ND_LAr_dtctr_pos == i_ND_LAr_dtctr_pos && ND_LAr_vtx_pos == i_ND_LAr_vtx_pos ){
        //cout<<" i_nd_lar_vrx_pos: "<<i_ND_LAr_vtx_pos<<" ND_LAr_vtx_pos " << ND_LAr_vtx_pos<<" i_entry "<< i_entry<<endl;
        // for(int i = 0; i<validThrows; i++)
        //   cout<<" throw: "<<ThrowResults->at(i)<<" valid throws: "<<validThrows<<endl;
        x_ND_LAr_vtx_pos[i_vtxX_plot-1] = ND_LAr_vtx_pos;
        y_geoeff[i_vtxX_plot-1] = ND_GeoEff;
        PlotEfficiencyVsVtxX[i_iwritten] = new TGraph(a_ND_vtx_vx_vec.size(), x_ND_LAr_vtx_pos, y_geoeff);

        //PlotEfficiencyVsVtxX[0] = new TGraph(1, x_ND_LAr_vtx_pos, y_geoeff);
        // cout<<" size: "<<a_ND_vtx_vx_vec.size()<<" x_ND_LAr_vtx_pos "<<x_ND_LAr_vtx_pos[i_vtxX_plot-1]<<" ef: "<<y_geoeff[i_vtxX_plot-1]<<endl;
        // cout<<" ND vtx x pos: "<<ND_LAr_vtx_pos<< " ND geo eff: "<<ND_GeoEff<<" pss throws: "<< NPassedThrows<<" valid throws: "<<validThrows<<endl;

        //cout<<"here??"<<endl;
        Double_t offset_X = i_ND_LAr_dtctr_pos;
        // Store hadron hit info into vector
        vector<float> ThrowDepsX_hit;
        vector<vector<float>> ThrowDepsX;
        ThrowDepsX.clear();
        int it_throw_x_counter =0;
        double averageTotalE ; //totalE at vtx_x = 7.75

        int throw_couter = 0;

        int nthrowsToLoop = NPassedThrows; //this is going to be the validThrows


        for (Int_t ithrow = 0; ithrow < nthrowsToLoop; ithrow++ ){
            // cout << "ithrow: " << ithrow <<endl;
            // for all events
            n_plot = i_n_plot*N_throws + ithrow;

            // calculate energy outside the ND active volume
            Double_t CurrentThrowoutEnergyND = 0.;
            Double_t CurrentThrowvetoEnergyND = 0.;
            // cout<<" total E :"<<TotalEnergyEventsPass->at(0)<<endl;


            // if(i_iwritten == 0 )
            //   cout<<" ** Etrim = "<<TrimEnergyEventsPass->at(ithrow)<<" tot E: "<<TotalEnergyEventsPass->at(ithrow)<<endl;
                  //<<" vtx y = "<<VtxYEventsPass->at(ithrow)- NDLAr_OnAxis_offset[1]<< " vtx z = "<<VtxZEventsPass->at(ithrow)-NDLAr_OnAxis_offset[2]<<endl;

            //cout<<"i_written: "<<i_iwritten<<" veto e: "<<VetoEnergyEventsPass->at(ithrow)<<" trim E: "<<TrimEnergyEventsPass->at(ithrow)<<" totE "<<TotalEnergyEventsPass->at(ithrow)<<endl;
            CurrentThrowvetoEnergyND = VetoEnergyEventsPass->at(ithrow);

              // HistEout[i_iwritten][i_vtxX_plot-1]->Fill(CurrentThrowoutEnergyND);
            HistVetoE[i_iwritten][i_vtxX_plot-1]->Fill(CurrentThrowvetoEnergyND);
              // cout<<" tot energy: "<<CurrentThrowTotE->at(ithrow)<<endl;
            HistEtrim[i_iwritten][i_vtxX_plot-1]->Fill(TrimEnergyEventsPass->at(ithrow));
            HistEtrimAllVtxX[i_iwritten]->Fill(TrimEnergyEventsPass->at(ithrow), ND_GeoEff/(nthrowsToLoop*a_ND_vtx_vx_vec.size()));

            HistEtrimAllVtxXTimesCoeff[i_iwritten]->Fill(TrimEnergyEventsPass->at(ithrow), Coefficients[i_vtxX_plot-1]*ND_GeoEff/(nthrowsToLoop*a_ND_vtx_vx_vec.size()));
            // HistEtrimAllVtxXTimesCoeff[i_iwritten]->Scale(1.0 / (ND_GeoEff/(nthrowsToLoop*a_ND_vtx_vx_vec.size()) ) );

          } //end throw


          PlotEfficiencyVsVtxX[i_iwritten]->SetTitle(Form("TotalFD Energy = %.2f MeV", totEnergyFDatND_f));

          // TString CanvasOutEout_name = Form("CanvasEout_FDEvt_%d_vtxXpos_%d", i_iwritten, i_ND_LAr_vtx_pos);
          // CanvasOutE[i_vtxX_plot-1] = new TCanvas(CanvasOutEout_name, CanvasOutEout_name, 600, 400);
          // CanvasOutE[i_vtxX_plot-1]->Clear();
          // CanvasOutE[i_vtxX_plot-1]->SetLeftMargin(0.15);
          // CanvasOutE[i_vtxX_plot-1]->SetRightMargin(0.15);
          // CanvasOutE[i_vtxX_plot-1]->cd();
          // HistEout[i_iwritten][i_vtxX_plot-1]->Scale(ND_GeoEff/nthrowsToLoop);
          // HistEout[i_iwritten][i_vtxX_plot-1]->Draw("hist");
          //HistEout[i_iwritten-1][i_vtxX_plot-1]->SetDirectory(0);

          // TString CanvasVetoE_name = Form("CanvasVetoE_FDEvt_%d_vtxXpos_%d", i_iwritten, i_ND_LAr_vtx_pos);
          // CanvasVetoE[i_vtxX_plot-1] = new TCanvas(CanvasVetoE_name, CanvasVetoE_name, 600, 400);
          // CanvasVetoE[i_vtxX_plot-1]->Clear();
          // CanvasVetoE[i_vtxX_plot-1]->SetLeftMargin(0.15);
          // CanvasVetoE[i_vtxX_plot-1]->SetRightMargin(0.15);
          // CanvasVetoE[i_vtxX_plot-1]->cd();
          // HistVetoE[i_iwritten][i_vtxX_plot-1]->Scale(ND_GeoEff/nthrowsToLoop);
          // HistVetoE[i_iwritten][i_vtxX_plot-1]->Draw("hist");
          //
          TString CanvasTrimE_name = Form("CanvasTrimE_FDEvt_%d_vtxXpos_%d", i_iwritten, i_ND_LAr_vtx_pos);
          CanvasTrimE[i_vtxX_plot-1] = new TCanvas(CanvasTrimE_name, CanvasTrimE_name, 600, 400);
          CanvasTrimE[i_vtxX_plot-1]->Clear();
          CanvasTrimE[i_vtxX_plot-1]->SetLeftMargin(0.15);
          CanvasTrimE[i_vtxX_plot-1]->SetRightMargin(0.15);

          CanvasTrimE[i_vtxX_plot-1]->cd();
          HistEtrim[i_iwritten][i_vtxX_plot-1]->Scale(ND_GeoEff/nthrowsToLoop);
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
    PlotEfficiencyVsVtxX[i_iwritten]->SetMarkerStyle(20);
    PlotEfficiencyVsVtxX[i_iwritten]->Draw("AP");
    // PlotEfficiencyVsVtxX[i_iwritten]->SetTitle(Form("GraphEffficiency_FDEventNr_%d",i_iwritten));
    PlotEfficiencyVsVtxX[i_iwritten]->Write(Form("GraphEffficiency_FDEventNr_%d",i_iwritten));

  }//end iwritten

  GraphCoeffVsVtxX->Write("CoefficientsVsVtxX");

  FileWithHistoInfo->Write();
  FileWithHistoInfo->Close();

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
