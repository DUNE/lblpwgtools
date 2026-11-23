// Run this macro:
// root -l -b -q FDEffCalc.C
//
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
#include <vector>

// Include customized functions and constants
R__ADD_INCLUDE_PATH(include)
#include "step1_helpers.h"

void FDEffCalc() {

  bool debug             = false; // Print out for debug purpose
  // Inout file on FNAL dunegpvm machine
  TString FileIn         = "/home/fyguo/NDEff/DUNE_ND_GeoEff/bin/Output_FDGeoEff_ivy.root";

  double effthreshold    = 0.;    // b/t 0 and 1: only use evts with hadron contain eff above the threshold in some plots
  Double_t eff_binning   = 0.05;  // A positive number
  Double_t Vtx_x_binning = 1.;    // cm, a positive number
  Double_t Vtx_y_binning = 1.;
  Double_t Vtx_z_binning = 1.;
  Double_t PlotMinY      = 0.5;   // events
  Double_t PlotMaxY      = 2000.; // events
  double LepE_low        = 0.;
  double LepE_up         = 30.;   // GeV
  double LepE_binning    = 1;     // GeV, a positive number
  double HadE_low        = 0.;
  double HadE_up         = 2000.; // MeV
  double HadE_binning    = 100;   // MeV, a positive number
  int nentries           = 0;     // Total input events

  //
  // Read branch from input trees
  //

  // Initialize
  double Gen_numu_E;
  vector<double> *ND_off_axis_pos_vec = 0; // unit: m, need initialize 0 here to avoid error
  vector<double> *Sim_mu_start_vx     = 0; // unit: cm
  vector<vector<vector<vector<bool> > > > *Sim_hadron_contain_result_before_throw = 0; // Nested vector: ND off axis position x, evt vtx x, vetoSize, vetoEnergy
  vector<vector<vector<vector<vector<uint64_t> > > > > *Sim_hadron_throw_result   = 0; // ...... ....... .. ... .... ........ .. ... ... .. ......... .........., 64-throw-result chunks
  double Sim_mu_start_vy;
  double Sim_mu_start_vz;
  double Sim_mu_start_px;
  double Sim_mu_start_py;
  double Sim_mu_start_pz;
  double Sim_mu_start_E;
  double Sim_hadronic_Edep_a2;
  // Random throws
  vector<float> *throwVtxY = 0; // unit: cm
  vector<float> *throwVtxZ = 0;

  // Read hadron throw result
  TChain *myhadronchain = new TChain("effTreeFD");
  myhadronchain->Add( FileIn.Data() );
  myhadronchain->SetBranchAddress("Gen_numu_E",                             &Gen_numu_E);
  myhadronchain->SetBranchAddress("ND_off_axis_pos_vec",                    &ND_off_axis_pos_vec);     // vector<double>: entries = written evts * ND_off_axis_pos_steps
  myhadronchain->SetBranchAddress("Sim_mu_start_vx",                        &Sim_mu_start_vx);         // ............... entries = written evts * vtx_vx_steps, equivalent to b_vtx_vx_vec
  myhadronchain->SetBranchAddress("Sim_hadron_contain_result_before_throw", &Sim_hadron_contain_result_before_throw);
  myhadronchain->SetBranchAddress("Sim_hadron_throw_result",                &Sim_hadron_throw_result);
  myhadronchain->SetBranchAddress("Sim_mu_start_vy",                        &Sim_mu_start_vy);
  myhadronchain->SetBranchAddress("Sim_mu_start_vz",                        &Sim_mu_start_vz);
  myhadronchain->SetBranchAddress("Sim_mu_start_px",                        &Sim_mu_start_px);
  myhadronchain->SetBranchAddress("Sim_mu_start_py",                        &Sim_mu_start_py);
  myhadronchain->SetBranchAddress("Sim_mu_start_pz",                        &Sim_mu_start_pz);
  myhadronchain->SetBranchAddress("Sim_mu_start_E",                         &Sim_mu_start_E);
  myhadronchain->SetBranchAddress("Sim_hadronic_Edep_a2",                   &Sim_hadronic_Edep_a2);    // tot hadron deposited E in the evt

  // Read random throws each FD evt
  TChain *mythrowchain = new TChain("ThrowsFD");
  mythrowchain->Add( FileIn.Data() );
  mythrowchain->SetBranchAddress("throwVtxY", &throwVtxY); // vector<float>: entries = [ (int)(written evts / 100) + 1 ] * N_throws
  mythrowchain->SetBranchAddress("throwVtxZ", &throwVtxZ);

  //
  // Initialize histograms
  //

  // Set the number of histograms using variables defined in step1_helpers.h
  int nplot = ( OffAxisPoints[13] - OffAxisPoints[0] ) / ND_off_axis_pos_stepsize + 1; // Avoid the plot for random element
  int mplot = ( ND_local_x_max - ND_local_x_min ) / ND_local_x_stepsize + 1;

  // Create a dynamic array of pointers
  TH2F** Hadron_eff_vs_vtx_x              = new TH2F*[nplot];       // fill hadron eff and stepwise vtx x (exclude random x), each evt appears one time per evt vtx x
  TH2F** Hadron_eff_vs_random_vtx_x       = new TH2F*[nplot];       // fill hadron eff and random vtx x, each evt appears only one time
  TH1F** Vtx_x                            = new TH1F*[nplot];       // fill random evt vtx x
  TH1F** Vtx_x_HadronInND                 = new TH1F*[nplot];
  TH1F** Vtx_x_HadronEffWgted             = new TH1F*[nplot];
  TH1F** Vtx_y                            = new TH1F*[nplot];       // fill random evt vtx y
  TH1F** Vtx_y_HadronInND                 = new TH1F*[nplot];
  TH1F** Vtx_y_HadronEffWgted             = new TH1F*[nplot];
  TH1F** Vtx_z                            = new TH1F*[nplot];       // fill random evt vtx z
  TH1F** Vtx_z_HadronInND                 = new TH1F*[nplot];
  TH1F** Vtx_z_HadronEffWgted             = new TH1F*[nplot];
  TH1F** FDNeuE_random_vtx                = new TH1F*[nplot];       // fill neutrino energy with the random evt vx
  TH1F** FDNeuE_random_vtx_HadronInND     = new TH1F*[nplot];
  TH1F** FDNeuE_random_vtx_HadronEffWgted = new TH1F*[nplot];
  TH1F** FDLepE_random_vtx                = new TH1F*[nplot];       // fill lepton energy with the random evt vx
  TH1F** FDLepE_random_vtx_HadronInND     = new TH1F*[nplot];
  TH1F** FDLepE_random_vtx_HadronEffWgted = new TH1F*[nplot];
  TH1F** FDHadE_random_vtx                = new TH1F*[nplot];       // fill hadron deposited energy with the random evt vx
  TH1F** FDHadE_random_vtx_HadronInND     = new TH1F*[nplot];
  TH1F** FDHadE_random_vtx_HadronEffWgted = new TH1F*[nplot];
  // Similar plots as above but with stepwise increased evt vx
  TH1F** FDNeuE                          = new TH1F*[nplot*mplot];
  TH1F** FDNeuE_HadronInND               = new TH1F*[nplot*mplot];
  TH1F** FDNeuE_HadronEffWgted           = new TH1F*[nplot*mplot];
  TH1F** FDLepE                          = new TH1F*[nplot*mplot];
  TH1F** FDLepE_HadronInND               = new TH1F*[nplot*mplot];
  TH1F** FDLepE_HadronEffWgted           = new TH1F*[nplot*mplot];
  TH1F** FDHadE                          = new TH1F*[nplot*mplot];
  TH1F** FDHadE_HadronInND               = new TH1F*[nplot*mplot];
  TH1F** FDHadE_HadronEffWgted           = new TH1F*[nplot*mplot];

  for ( int ihist = 0; ihist < nplot; ihist++ ) {

    Hadron_eff_vs_vtx_x[ihist]              = new TH2F(TString::Format("Hadron_eff_vs_vtx_x_NdOffAxisPos_%.2f_m",              ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m",                                                     ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0]), int( (ND_local_x_max - ND_local_x_min)/ND_local_x_stepsize ) + 1, ND_local_x_min, ND_local_x_max + ND_local_x_stepsize, int( 1 / eff_binning ) + 1, 0., 1 + eff_binning);
    Hadron_eff_vs_random_vtx_x[ihist]       = new TH2F(TString::Format("Hadron_eff_vs_random_vtx_x_NdOffAxisPos_%.2f_m",       ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, random vtx",                                         ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0]), int( (ND_local_x_max - ND_local_x_min)/Vtx_x_binning ) + 1,       ND_local_x_min, ND_local_x_max + Vtx_x_binning,       int( 1 / eff_binning ) + 1, 0., 1 + eff_binning);
    Vtx_x[ihist]                            = new TH1F(TString::Format("Random_vtx_x_NdOffAxisPos_%.2f_m",                     ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx",                      ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (ND_local_x_max - ND_local_x_min)/Vtx_x_binning ) + 1, ND_local_x_min, ND_local_x_max + Vtx_x_binning);
    Vtx_x_HadronInND[ihist]                 = new TH1F(TString::Format("Random_vtx_x_NdOffAxisPos_%.2f_m_HadronInND",          ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx, pass ND hadron veto", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (ND_local_x_max - ND_local_x_min)/Vtx_x_binning ) + 1, ND_local_x_min, ND_local_x_max + Vtx_x_binning);
    Vtx_x_HadronEffWgted[ihist]             = new TH1F(TString::Format("Random_vtx_x_NdOffAxisPos_%.2f_m_HadronEffWgted",      ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx, hadron eff weighted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (ND_local_x_max - ND_local_x_min)/Vtx_x_binning ) + 1, ND_local_x_min, ND_local_x_max + Vtx_x_binning);
    Vtx_y[ihist]                            = new TH1F(TString::Format("Random_vtx_y_NdOffAxisPos_%.2f_m",                     ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx",                      ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (NDActiveVol_max[1] - NDActiveVol_min[1])/Vtx_y_binning ) + 1, NDActiveVol_min[1], NDActiveVol_max[1] + Vtx_y_binning);
    Vtx_y_HadronInND[ihist]                 = new TH1F(TString::Format("Random_vtx_y_NdOffAxisPos_%.2f_m_HadronInND",          ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx, pass ND hadron veto", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (NDActiveVol_max[1] - NDActiveVol_min[1])/Vtx_y_binning ) + 1, NDActiveVol_min[1], NDActiveVol_max[1] + Vtx_y_binning);
    Vtx_y_HadronEffWgted[ihist]             = new TH1F(TString::Format("Random_vtx_y_NdOffAxisPos_%.2f_m_HadronEffWgted",      ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx, hadron eff weighted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (NDActiveVol_max[1] - NDActiveVol_min[1])/Vtx_y_binning ) + 1, NDActiveVol_min[1], NDActiveVol_max[1] + Vtx_y_binning);
    Vtx_z[ihist]                            = new TH1F(TString::Format("Random_vtx_z_NdOffAxisPos_%.2f_m",                     ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx",                      ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (NDActiveVol_max[2] - NDActiveVol_min[2])/Vtx_z_binning ) + 1, NDActiveVol_min[2], NDActiveVol_max[2] + Vtx_z_binning);
    Vtx_z_HadronInND[ihist]                 = new TH1F(TString::Format("Random_vtx_z_NdOffAxisPos_%.2f_m_HadronInND",          ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx, pass ND hadron veto", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (NDActiveVol_max[2] - NDActiveVol_min[2])/Vtx_z_binning ) + 1, NDActiveVol_min[2], NDActiveVol_max[2] + Vtx_z_binning);
    Vtx_z_HadronEffWgted[ihist]             = new TH1F(TString::Format("Random_vtx_z_NdOffAxisPos_%.2f_m_HadronEffWgted",      ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx, hadron eff weighted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (NDActiveVol_max[2] - NDActiveVol_min[2])/Vtx_z_binning ) + 1, NDActiveVol_min[2], NDActiveVol_max[2] + Vtx_z_binning);
    FDNeuE_random_vtx[ihist]                = new TH1F(TString::Format("FDNeuE_NdOffAxisPos_%.2f_m_random_vtx",                ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx",                      ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (LepE_up - LepE_low)/LepE_binning ) + 1, LepE_low, LepE_up + LepE_binning);
    FDNeuE_random_vtx_HadronInND[ihist]     = new TH1F(TString::Format("FDNeuE_NdOffAxisPos_%.2f_m_random_vtx_HadronInND",     ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx, pass ND hadron veto", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (LepE_up - LepE_low)/LepE_binning ) + 1, LepE_low, LepE_up + LepE_binning);
    FDNeuE_random_vtx_HadronEffWgted[ihist] = new TH1F(TString::Format("FDNeuE_NdOffAxisPos_%.2f_m_random_vtx_HadronEffWgted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx, hadron eff weighted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (LepE_up - LepE_low)/LepE_binning ) + 1, LepE_low, LepE_up + LepE_binning);
    FDLepE_random_vtx[ihist]                = new TH1F(TString::Format("FDLepE_NdOffAxisPos_%.2f_m_random_vtx",                ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx",                      ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (LepE_up - LepE_low)/LepE_binning ) + 1, LepE_low, LepE_up + LepE_binning);
    FDLepE_random_vtx_HadronInND[ihist]     = new TH1F(TString::Format("FDLepE_NdOffAxisPos_%.2f_m_random_vtx_HadronInND",     ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx, pass ND hadron veto", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (LepE_up - LepE_low)/LepE_binning ) + 1, LepE_low, LepE_up + LepE_binning);
    FDLepE_random_vtx_HadronEffWgted[ihist] = new TH1F(TString::Format("FDLepE_NdOffAxisPos_%.2f_m_random_vtx_HadronEffWgted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx, hadron eff weighted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (LepE_up - LepE_low)/LepE_binning ) + 1, LepE_low, LepE_up + LepE_binning);
    FDHadE_random_vtx[ihist]                = new TH1F(TString::Format("FDHadE_NdOffAxisPos_%.2f_m_random_vtx",                ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx",                      ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (HadE_up - HadE_low)/HadE_binning ) + 1, HadE_low, HadE_up + HadE_binning);
    FDHadE_random_vtx_HadronInND[ihist]     = new TH1F(TString::Format("FDHadE_NdOffAxisPos_%.2f_m_random_vtx_HadronInND",     ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx, pass ND hadron veto", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (HadE_up - HadE_low)/HadE_binning ) + 1, HadE_low, HadE_up + HadE_binning);
    FDHadE_random_vtx_HadronEffWgted[ihist] = new TH1F(TString::Format("FDHadE_NdOffAxisPos_%.2f_m_random_vtx_HadronEffWgted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx, hadron eff weighted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), int( (HadE_up - HadE_low)/HadE_binning ) + 1, HadE_low, HadE_up + HadE_binning);

    for ( int jhist = 0; jhist < mplot; jhist++ ) {
      FDNeuE[ihist*mplot + jhist]                = new TH1F(TString::Format("FDNeuE_NdOffAxisPos_%.2f_m_Vtx_x_%.1f_cm",                ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min ), TString::Format("FD evt in ND: off-axis position = %.2f m, vtx x = %.1f cm, hadron eff > %.3f",                      ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min, effthreshold ), int( (LepE_up - LepE_low)/LepE_binning ) + 1, LepE_low, LepE_up + LepE_binning);
      FDNeuE_HadronInND[ihist*mplot + jhist]     = new TH1F(TString::Format("FDNeuE_NdOffAxisPos_%.2f_m_Vtx_x_%.1f_cm_HadronInND",     ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min ), TString::Format("FD evt in ND: off-axis position = %.2f m, vtx x = %.1f cm, hadron eff > %.3f, pass ND hadron veto", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min, effthreshold ), int( (LepE_up - LepE_low)/LepE_binning ) + 1, LepE_low, LepE_up + LepE_binning);
      FDNeuE_HadronEffWgted[ihist*mplot + jhist] = new TH1F(TString::Format("FDNeuE_NdOffAxisPos_%.2f_m_Vtx_x_%.1f_cm_HadronEffWgted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min ), TString::Format("FD evt in ND: off-axis position = %.2f m, vtx x = %.1f cm, hadron eff > %.3f, hadron eff weighted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min, effthreshold ), int( (LepE_up - LepE_low)/LepE_binning ) + 1, LepE_low, LepE_up + LepE_binning);
      FDLepE[ihist*mplot + jhist]                = new TH1F(TString::Format("FDLepE_NdOffAxisPos_%.2f_m_Vtx_x_%.1f_cm",                ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min ), TString::Format("FD evt in ND: off-axis position = %.2f m, vtx x = %.1f cm, hadron eff > %.3f",                      ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min, effthreshold ), int( (LepE_up - LepE_low)/LepE_binning ) + 1, LepE_low, LepE_up + LepE_binning);
      FDLepE_HadronInND[ihist*mplot + jhist]     = new TH1F(TString::Format("FDLepE_NdOffAxisPos_%.2f_m_Vtx_x_%.1f_cm_HadronInND",     ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min ), TString::Format("FD evt in ND: off-axis position = %.2f m, vtx x = %.1f cm, hadron eff > %.3f, pass ND hadron veto", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min, effthreshold ), int( (LepE_up - LepE_low)/LepE_binning ) + 1, LepE_low, LepE_up + LepE_binning);
      FDLepE_HadronEffWgted[ihist*mplot + jhist] = new TH1F(TString::Format("FDLepE_NdOffAxisPos_%.2f_m_Vtx_x_%.1f_cm_HadronEffWgted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min ), TString::Format("FD evt in ND: off-axis position = %.2f m, vtx x = %.1f cm, hadron eff > %.3f, hadron eff weighted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min, effthreshold ), int( (LepE_up - LepE_low)/LepE_binning ) + 1, LepE_low, LepE_up + LepE_binning);
      FDHadE[ihist*mplot + jhist]                = new TH1F(TString::Format("FDHadE_NdOffAxisPos_%.2f_m_Vtx_x_%.1f_cm",                ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min ), TString::Format("FD evt in ND: off-axis position = %.2f m, vtx x = %.1f cm, hadron eff > %.3f",                      ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min, effthreshold ), int( (HadE_up - HadE_low)/HadE_binning ) + 1, HadE_low, HadE_up + HadE_binning);
      FDHadE_HadronInND[ihist*mplot + jhist]     = new TH1F(TString::Format("FDHadE_NdOffAxisPos_%.2f_m_Vtx_x_%.1f_cm_HadronInND",     ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min ), TString::Format("FD evt in ND: off-axis position = %.2f m, vtx x = %.1f cm, hadron eff > %.3f, pass ND hadron veto", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min, effthreshold ), int( (HadE_up - HadE_low)/HadE_binning ) + 1, HadE_low, HadE_up + HadE_binning);
      FDHadE_HadronEffWgted[ihist*mplot + jhist] = new TH1F(TString::Format("FDHadE_NdOffAxisPos_%.2f_m_Vtx_x_%.1f_cm_HadronEffWgted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min ), TString::Format("FD evt in ND: off-axis position = %.2f m, vtx x = %.1f cm, hadron eff > %.3f, hadron eff weighted", ihist*ND_off_axis_pos_stepsize + OffAxisPoints[0], jhist*ND_local_x_stepsize + ND_local_x_min, effthreshold ), int( (HadE_up - HadE_low)/HadE_binning ) + 1, HadE_low, HadE_up + HadE_binning);
    } // end for jhist
  }   // end for ihist

  //
  // Loop over written events
  //
  nentries = myhadronchain->GetEntries();

  if ( debug ) std::cout << "Tot written evts: " << nentries << std::endl;
  for ( int iwritten = 0; iwritten < nentries; iwritten++ ) {

    myhadronchain->GetEntry(iwritten);

    if ( iwritten%10000 == 0 ) std::cout << "Looking at iwritten " << iwritten << std::endl;

    if ( debug ) std::cout << "N_throws set #" << iwritten/100 << std::endl;
    mythrowchain->GetEntry(iwritten/100);
    if ( debug ) std::cout << "throwVtxY size: " << throwVtxY->size() << std::endl; // this should be N_throws set by user

    // First loop over ND off_axis vector
    //   then loop over evt vtx x
    //     then [0][0][k-th chunk]
    //     or [i][j][k-th chunk], i for vetoSize, j for vetoEnergy

    if ( debug ) std::cout << "ND off axis size: " << Sim_hadron_throw_result->size() << std::endl;

    int counter1 = 0;

    for ( vector<vector<vector<vector<vector<uint64_t> > > > >::iterator it_nd_off_axis_pos = Sim_hadron_throw_result->begin(); it_nd_off_axis_pos != Sim_hadron_throw_result->end(); ++it_nd_off_axis_pos ) {

      counter1++;
      if ( debug ) std::cout << "  #" << counter1 << ", evt vtx x size: " << it_nd_off_axis_pos->size() << std::endl;

      int counter2 = 0;

      for ( vector<vector<vector<vector<uint64_t> > > >::iterator it_evt_vtx_x = it_nd_off_axis_pos->begin(); it_evt_vtx_x != it_nd_off_axis_pos->end(); ++it_evt_vtx_x ) {

        counter2++;
        if ( debug ) std::cout << "    #" << counter2 << ", vetoSize size: " << it_evt_vtx_x->size() << std::endl;

        int counter3 = 0;

        for ( vector<vector<vector<uint64_t> > >::iterator it_veto_size = it_evt_vtx_x->begin(); it_veto_size != it_evt_vtx_x->end(); ++it_veto_size ) {

          counter3++;
          if ( debug ) std::cout << "      #" << counter3 << ", vetoEnergy size: " << it_veto_size->size() << std::endl;

          int counter4 = 0;

          for ( vector<vector<uint64_t> >::iterator it_veto_energy = it_veto_size->begin(); it_veto_energy != it_veto_size->end(); ++it_veto_energy ) {

            counter4++;

            // Every 64 throw result is a chunk
            // current test case: each evt has 128 throws, so 2 chunks
            if ( debug ) std::cout << "        #" << counter4 << ", 64-throw size (chunks): " << it_veto_energy->size() << std::endl;

            bool contain_result_before_throw = false; // hadron ND-containment result of FD evt before throws
            int counter5    = 0;
            int validthrows = 0; // count no. of throws that meet ND FV cut for this evt
            int hadronpass  = 0; // count no. of throws that meet hadron containment cut
            double hadron_contain_eff = 0.; // initialize eff to zero

            for ( vector<uint64_t>::iterator it_chunk = it_veto_energy->begin(); it_chunk != it_veto_energy->end(); ++it_chunk ) {

              counter5++;
              if ( debug ) std::cout << "          chunk #" << counter5 << ": " << *it_chunk << std::endl;
              if ( debug ) std::cout << "                    throws passed ND FV cut" << std::endl;

              for ( unsigned int ithrow = 0; ithrow < 64; ithrow++ ) {

                // For the numerator, only consider throws where throwed FD evt vtx x/y/z is in ND FV, same as what is done for ND evts
                // For now, we use mu start pos as evt vtx pos, random throws for y/z are stored in the ThrowsFD tree
                if ( FDEffCalc_cfg::IsInNDFV( Sim_mu_start_vx->at( counter2 - 1 ), throwVtxY->at( (counter5-1)*64 + ithrow ) + Sim_mu_start_vy, throwVtxZ->at( (counter5-1)*64 + ithrow ) + Sim_mu_start_vz ) ) {

                  validthrows++;

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

                  if ( debug ) std::cout << "                    throw #" << ithrow+1 << ": " << throw_result << std::endl;

                  // Count no. of throws passed hadron containment requirement
                  if ( throw_result != 0 ) hadronpass++;

                } // end if FD event passed ND FV cut

              }   // end loop over 64 throws in a chunk

            }     // end loop over 64-throw chunks

            // Get the hadron contain result of this written event BEFORE throws
            contain_result_before_throw = (*Sim_hadron_contain_result_before_throw)[counter1 - 1][counter2 - 1][counter3 - 1][counter4 - 1];
            if ( debug ) std::cout << "        FD evt contained in ND before throws: " << contain_result_before_throw << std::endl;

            //
            // Calculate per-event hadron containment efficiency from throws
            //

            // If a throwed evt vtx is in ND dead region, it is not going to be detected by ND, but probably will be detected in FD (assume)
            // In principle, we should NOT exclude such throws in the denominator

            //hadron_contain_eff = hadronpass*1.0/N_throws; // N_throws also equals 64 * counter5
            //if ( debug ) std::cout << "        Passed throws: " << hadronpass << ", eff: " << hadron_contain_eff << ", evt vtx x [cm]: " << Sim_mu_start_vx->at( counter2 - 1 ) << ", nd off-axis pos [m]: " << ND_off_axis_pos_vec->at( counter1 -1 ) << std::endl;

            // But for the ND FV cut, we could probably factorize it analytically instead of using these random throws to evaluate
            // therefore we exclude such throws from the denominator as well
            if ( validthrows > 0 ) hadron_contain_eff = hadronpass*1.0/validthrows;
            if ( debug ) std::cout << "        Passed throws: " << hadronpass << ", tot. valid throws: " << validthrows << ", eff: " << hadron_contain_eff << ", evt vtx x [cm]: " << Sim_mu_start_vx->at( counter2 - 1 ) << ", nd off-axis pos [m]: " << ND_off_axis_pos_vec->at( counter1 -1 ) << std::endl;

            //
            // Fill histograms
            //

            for ( int ifill = 0; ifill < nplot; ifill++ ) {

              //
              // 2D histogram for each nd off-axis pos: efficiency - evt vtx x, for each ND-off-axis pos
              //

              // Skip first elements of ND-off-axis and evt vtx x as both are random generated
              if ( counter1 - 1 == ifill + 1 && counter2 - 1 > 0 ) Hadron_eff_vs_vtx_x[ifill]->Fill(Sim_mu_start_vx->at(counter2 - 1), hadron_contain_eff);
              // Only use the random evt vtx x but not the random ND off axis pos
              if ( counter1 - 1 == ifill + 1 && counter2 == 1 )    Hadron_eff_vs_random_vtx_x[ifill]->Fill(Sim_mu_start_vx->at(0), hadron_contain_eff);

              //
              // 1D histograms for each nd off-axis pos using random vtx x
              //

              // Only use the random evt vtx x but not the random ND off axis pos
              if ( counter1 - 1 == ifill + 1 && counter2 == 1 ) {

                // FD evts (before random throws, but already converted to nd coordinate sys) meet these requirement:
                //   1. hadron eff above a threshold,
                //   2. evt vtx in ND FV.
                // consitute the sample with good ND eff
                // FD evts failed these cuts constitute the ND-never-selected sample
                if ( hadron_contain_eff > effthreshold && FDEffCalc_cfg::IsInNDFV( Sim_mu_start_vx->at(0), Sim_mu_start_vy, Sim_mu_start_vz ) ) {

                  Vtx_x[ifill]->Fill(Sim_mu_start_vx->at(0));
                  Vtx_y[ifill]->Fill(Sim_mu_start_vy);
                  Vtx_z[ifill]->Fill(Sim_mu_start_vz);
                  FDNeuE_random_vtx[ifill]->Fill(Gen_numu_E);
                  FDLepE_random_vtx[ifill]->Fill(Sim_mu_start_E);
                  FDHadE_random_vtx[ifill]->Fill(Sim_hadronic_Edep_a2);

                  // Before random throws, FD evts pass ND hadron veto cut
                  if ( contain_result_before_throw ) {

                    Vtx_x_HadronInND[ifill]->Fill(Sim_mu_start_vx->at(0));
                    Vtx_y_HadronInND[ifill]->Fill(Sim_mu_start_vy);
                    Vtx_z_HadronInND[ifill]->Fill(Sim_mu_start_vz);
                    FDNeuE_random_vtx_HadronInND[ifill]->Fill(Gen_numu_E);
                    FDLepE_random_vtx_HadronInND[ifill]->Fill(Sim_mu_start_E);
                    FDHadE_random_vtx_HadronInND[ifill]->Fill(Sim_hadronic_Edep_a2);

                    // Weighted FD evts
                    Vtx_x_HadronEffWgted[ifill]->Fill(Sim_mu_start_vx->at(0), 1./hadron_contain_eff);
                    Vtx_y_HadronEffWgted[ifill]->Fill(Sim_mu_start_vy, 1./hadron_contain_eff);
                    Vtx_z_HadronEffWgted[ifill]->Fill(Sim_mu_start_vz, 1./hadron_contain_eff);
                    FDNeuE_random_vtx_HadronEffWgted[ifill]->Fill(Gen_numu_E, 1./hadron_contain_eff);
                    FDLepE_random_vtx_HadronEffWgted[ifill]->Fill(Sim_mu_start_E, 1./hadron_contain_eff);
                    FDHadE_random_vtx_HadronEffWgted[ifill]->Fill(Sim_hadronic_Edep_a2, 1./hadron_contain_eff);

                  } // end if passed ND hadron veto
                }   // end if pass eff threshold and ND FV cut
              }     // end if only use the random evt vtx x but not the random ND off axis pos

              //
              // 1D histograms for each nd off-axis pos and evt vtx_x pos
              //

              for ( int jfill = 0; jfill < mplot; jfill++ ) {

                // Again skip first random elements in both vectors
                if ( counter1 - 1 == ifill + 1 && counter2 - 1 == jfill + 1 ) {

                  // Same requirement as above just with stepwise increased vtx x
                  if ( hadron_contain_eff > effthreshold && FDEffCalc_cfg::IsInNDFV( Sim_mu_start_vx->at( counter2 - 1 ), Sim_mu_start_vy, Sim_mu_start_vz ) ) {

                    FDNeuE[ifill*mplot + jfill]->Fill(Gen_numu_E);
                    FDLepE[ifill*mplot + jfill]->Fill(Sim_mu_start_E);
                    FDHadE[ifill*mplot + jfill]->Fill(Sim_hadronic_Edep_a2);

                    // Before random throws, FD evts pass ND hadron veto cut
                    if ( contain_result_before_throw ) {

                      FDNeuE_HadronInND[ifill*mplot + jfill]->Fill(Gen_numu_E);
                      FDLepE_HadronInND[ifill*mplot + jfill]->Fill(Sim_mu_start_E);
                      FDHadE_HadronInND[ifill*mplot + jfill]->Fill(Sim_hadronic_Edep_a2);

                      // Weighted FD evts
                      FDNeuE_HadronEffWgted[ifill*mplot + jfill]->Fill(Gen_numu_E, 1./hadron_contain_eff);
                      FDLepE_HadronEffWgted[ifill*mplot + jfill]->Fill(Sim_mu_start_E, 1./hadron_contain_eff);
                      FDHadE_HadronEffWgted[ifill*mplot + jfill]->Fill(Sim_hadronic_Edep_a2, 1./hadron_contain_eff);

                    } // end if hadron contained in ND
                  }   // end if pass eff threshold and ND FV cut
                }     // end if skip random elements in vectors
              }       // end for jfill
            }         // end for ifill

          }     // end loop over veto energy
        }       // end loop over veto size
      }         // end loop over evt vtx x
    }           // end loop over ND off axis pos
  }             // end loop over events

  // Write trees
  TFile * outFile = new TFile("FDEffCalc.root", "RECREATE");

  // Create a dynamic array of pointers
  TCanvas** c_Hadron_eff_vs_vtx_x        = new TCanvas*[nplot]; // stepwise increased vtx x
  TCanvas** c_Hadron_eff_vs_random_vtx_x = new TCanvas*[nplot];
  TCanvas** c_Vtx_x                      = new TCanvas*[nplot]; // random vtx x
  TCanvas** c_Vtx_y                      = new TCanvas*[nplot]; //            y
  TCanvas** c_Vtx_z                      = new TCanvas*[nplot]; //            z
  TCanvas** c_FDNeuE_random_vtx           = new TCanvas*[nplot];
  TCanvas** c_FDLepE_random_vtx           = new TCanvas*[nplot];
  TCanvas** c_FDHadE_random_vtx           = new TCanvas*[nplot];
  TCanvas** c_FDNeuE                     = new TCanvas*[nplot*mplot];
  TCanvas** c_FDLepE                     = new TCanvas*[nplot*mplot];
  TCanvas** c_FDHadE                     = new TCanvas*[nplot*mplot];

  for ( int ic = 0; ic < nplot; ic++ ) {

    // Draw 2D histograms
    c_Hadron_eff_vs_vtx_x[ic] = new TCanvas(TString::Format("c_Hadron_eff_vs_vtx_x_NdOffAxisPos_%.2f_m", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0]), TString::Format( "FD evt in ND: off-axis position = %.2f m", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), 700, 500);
    c_Hadron_eff_vs_vtx_x[ic]->cd();
    Hadron_eff_vs_vtx_x[ic]->GetXaxis()->SetTitle("Vtx x [cm]");
    Hadron_eff_vs_vtx_x[ic]->GetYaxis()->SetTitle("Hadron contain efficiency");
    Hadron_eff_vs_vtx_x[ic]->GetZaxis()->SetTitle("Events");
    Hadron_eff_vs_vtx_x[ic]->GetZaxis()->SetTitleOffset(0.7);
    Hadron_eff_vs_vtx_x[ic]->SetStats(0);
    Hadron_eff_vs_vtx_x[ic]->Draw("COLZ TEXT");
    c_Hadron_eff_vs_vtx_x[ic]->Write();

    c_Hadron_eff_vs_random_vtx_x[ic] = new TCanvas(TString::Format("c_Hadron_eff_vs_random_vtx_x_NdOffAxisPos_%.2f_m", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0]), TString::Format( "FD evt in ND: off-axis position = %.2f m, random vtx", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0] ), 700, 500);
    c_Hadron_eff_vs_random_vtx_x[ic]->cd();
    Hadron_eff_vs_random_vtx_x[ic]->GetXaxis()->SetTitle("Random vtx x [cm]");
    Hadron_eff_vs_random_vtx_x[ic]->GetYaxis()->SetTitle("Hadron contain efficiency");
    Hadron_eff_vs_random_vtx_x[ic]->GetZaxis()->SetTitle("Events");
    Hadron_eff_vs_random_vtx_x[ic]->GetZaxis()->SetTitleOffset(0.7);
    Hadron_eff_vs_random_vtx_x[ic]->SetStats(0);
    Hadron_eff_vs_random_vtx_x[ic]->Draw("COLZ");
    c_Hadron_eff_vs_random_vtx_x[ic]->Write();

    //
    // Overlay 1D histograms on random evt vtx x
    //

    c_Vtx_x[ic] = new TCanvas(TString::Format("c_Random_vtx_x_NdOffAxisPos_%.2f_m", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0]), TString::Format( "FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold ), 700, 500);
    c_Vtx_x[ic]->Clear();
    // Create two pads for event and ratio plots
    TPad *uppadVtx_x = new TPad("uppadVtx_x", "", 0, 0.3, 1, 1.0); // xlow, ylow, xup, yup
    uppadVtx_x->SetBottomMargin(0); uppadVtx_x->Draw();
    TPad *dnpadVtx_x = new TPad("dnpadVtx_x", "", 0, 0.0, 1, 0.29);
    dnpadVtx_x->SetTopMargin(0); dnpadVtx_x->SetBottomMargin(0.3); dnpadVtx_x->SetGridy(); dnpadVtx_x->Draw();

    uppadVtx_x->cd();
    Vtx_x[ic]->GetYaxis()->SetTitle(TString::Format("Events/%.1f cm", Vtx_x_binning));
    Vtx_x[ic]->SetMinimum(PlotMinY);
    Vtx_x[ic]->SetMaximum(PlotMaxY);
    Vtx_x[ic]->SetLineColor(4);
    Vtx_x[ic]->SetStats(0);
    Vtx_x[ic]->Draw("HIST E1");

    Vtx_x_HadronInND[ic]->SetLineColor(2);
    Vtx_x_HadronInND[ic]->SetStats(0);
    Vtx_x_HadronInND[ic]->Draw("HIST E1 SAME");

    Vtx_x_HadronEffWgted[ic]->SetLineColor(3);
    Vtx_x_HadronEffWgted[ic]->SetStats(0);
    Vtx_x_HadronEffWgted[ic]->Draw("HIST E1 SAME");

    // Build Legend
    TLegend* uppadVtx_xL = new TLegend(0.6, 0.5, 0.9, 0.9);
    uppadVtx_xL->SetBorderSize(0); uppadVtx_xL->SetFillStyle(0); uppadVtx_xL->SetNColumns(1);
    uppadVtx_xL->AddEntry(Vtx_x[ic], TString::Format("HadronEff > %.3f + vtxInNDFV", effthreshold), "l");
    uppadVtx_xL->AddEntry(Vtx_x_HadronInND[ic], "ND hadron veto", "l");
    uppadVtx_xL->AddEntry(Vtx_x_HadronEffWgted[ic], "Weighted: 1/HadronEff", "l");
    uppadVtx_xL->Draw();

    uppadVtx_x->Update(); uppadVtx_x->Modified();
    gPad->RedrawAxis(); gPad->SetLogy();
    c_Vtx_x[ic]->cd(); c_Vtx_x[ic]->Update();

    dnpadVtx_x->cd();
    // Vtx_x_HadronEffWgted / Vtx_x
    TH1F *ratio_HadronEffWgted_vx = (TH1F*)Vtx_x_HadronEffWgted[ic]->Clone("ratio_HadronEffWgted_vx"); // Clone to avoid changing the original hist
    ratio_HadronEffWgted_vx->Divide(Vtx_x[ic]);
    ratio_HadronEffWgted_vx->SetTitle("");
    ratio_HadronEffWgted_vx->GetXaxis()->SetTitle("Random vtx x [cm]");
    ratio_HadronEffWgted_vx->GetXaxis()->SetTitleSize(15);
    ratio_HadronEffWgted_vx->GetXaxis()->SetTitleFont(43);
    ratio_HadronEffWgted_vx->GetXaxis()->SetTitleOffset(4.0);
    ratio_HadronEffWgted_vx->GetXaxis()->SetLabelSize(15); // Labels will be 15 pixels
    ratio_HadronEffWgted_vx->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratio_HadronEffWgted_vx->GetYaxis()->SetTitle("ratio");
    ratio_HadronEffWgted_vx->GetYaxis()->CenterTitle();
    ratio_HadronEffWgted_vx->GetYaxis()->SetTitleSize(15);
    ratio_HadronEffWgted_vx->GetYaxis()->SetTitleFont(43);
    ratio_HadronEffWgted_vx->GetYaxis()->SetTitleOffset(.9);
    ratio_HadronEffWgted_vx->GetYaxis()->SetLabelSize(15);
    ratio_HadronEffWgted_vx->GetYaxis()->SetLabelFont(43);
    ratio_HadronEffWgted_vx->Draw("E1");
    // Vtx_x_HadronInND / Vtx_x
    TH1F *ratio_HadronInND_vx = (TH1F*)Vtx_x_HadronInND[ic]->Clone("ratio_HadronInND_vx");
    ratio_HadronInND_vx->Divide(Vtx_x[ic]);
    ratio_HadronInND_vx->SetTitle("");
    ratio_HadronInND_vx->Draw("E1 SAME");

    c_Vtx_x[ic]->Write();

    //
    // Overlay 1D histograms on random evt vtx y
    //

    c_Vtx_y[ic] = new TCanvas(TString::Format("c_Random_vtx_y_NdOffAxisPos_%.2f_m", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0]), TString::Format( "FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold ), 700, 500);
    c_Vtx_y[ic]->Clear();
    TPad *uppadVtx_y = new TPad("uppadVtx_y", "", 0, 0.3, 1, 1.0);
    uppadVtx_y->SetBottomMargin(0); uppadVtx_y->Draw();
    TPad *dnpadVtx_y = new TPad("dnpadVtx_y", "", 0, 0.0, 1, 0.29);
    dnpadVtx_y->SetTopMargin(0); dnpadVtx_y->SetBottomMargin(0.3); dnpadVtx_y->SetGridy(); dnpadVtx_y->Draw();

    uppadVtx_y->cd();
    Vtx_y[ic]->GetYaxis()->SetTitle(TString::Format("Events/%.1f cm", Vtx_y_binning));
    Vtx_y[ic]->SetMinimum(PlotMinY);
    Vtx_y[ic]->SetMaximum(PlotMaxY);
    Vtx_y[ic]->SetLineColor(4);
    Vtx_y[ic]->SetStats(0);
    Vtx_y[ic]->Draw("HIST E1");

    Vtx_y_HadronInND[ic]->SetLineColor(2);
    Vtx_y_HadronInND[ic]->SetStats(0);
    Vtx_y_HadronInND[ic]->Draw("HIST E1 SAME");

    Vtx_y_HadronEffWgted[ic]->SetLineColor(3);
    Vtx_y_HadronEffWgted[ic]->SetStats(0);
    Vtx_y_HadronEffWgted[ic]->Draw("HIST E1 SAME");

    // Build Legend
    TLegend* uppadVtx_yL = new TLegend(0.6, 0.5, 0.9, 0.9);
    uppadVtx_yL->SetBorderSize(0); uppadVtx_yL->SetFillStyle(0); uppadVtx_yL->SetNColumns(1);
    uppadVtx_yL->AddEntry(Vtx_y[ic], TString::Format("HadronEff > %.3f + vtxInNDFV", effthreshold), "l");
    uppadVtx_yL->AddEntry(Vtx_y_HadronInND[ic], "ND hadron veto", "l");
    uppadVtx_yL->AddEntry(Vtx_y_HadronEffWgted[ic], "Weighted: 1/HadronEff", "l");
    uppadVtx_yL->Draw();

    uppadVtx_y->Update(); uppadVtx_y->Modified();
    gPad->RedrawAxis(); gPad->SetLogy();
    c_Vtx_y[ic]->cd(); c_Vtx_y[ic]->Update();

    // Vtx_y_HadronEffWgted / Vtx_y
    dnpadVtx_y->cd();
    TH1F *ratio_HadronEffWgted_vy = (TH1F*)Vtx_y_HadronEffWgted[ic]->Clone("ratio_HadronEffWgted_vy"); // Clone to avoid changing the original hist
    ratio_HadronEffWgted_vy->Divide(Vtx_y[ic]);
    ratio_HadronEffWgted_vy->SetTitle("");
    ratio_HadronEffWgted_vy->GetXaxis()->SetTitle("Random vtx y [cm]");
    ratio_HadronEffWgted_vy->GetXaxis()->SetTitleSize(15);
    ratio_HadronEffWgted_vy->GetXaxis()->SetTitleFont(43);
    ratio_HadronEffWgted_vy->GetXaxis()->SetTitleOffset(4.0);
    ratio_HadronEffWgted_vy->GetXaxis()->SetLabelSize(15); // Labels will be 15 pixels
    ratio_HadronEffWgted_vy->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratio_HadronEffWgted_vy->GetYaxis()->SetTitle("ratio");
    ratio_HadronEffWgted_vy->GetYaxis()->CenterTitle();
    ratio_HadronEffWgted_vy->GetYaxis()->SetTitleSize(15);
    ratio_HadronEffWgted_vy->GetYaxis()->SetTitleFont(43);
    ratio_HadronEffWgted_vy->GetYaxis()->SetTitleOffset(.9);
    ratio_HadronEffWgted_vy->GetYaxis()->SetLabelSize(15);
    ratio_HadronEffWgted_vy->GetYaxis()->SetLabelFont(43);
    ratio_HadronEffWgted_vy->Draw("E1");
    // Vtx_y_HadronInND / Vtx_y
    TH1F *ratio_HadronInND_vy = (TH1F*)Vtx_y_HadronInND[ic]->Clone("ratio_HadronInND_vy");
    ratio_HadronInND_vy->Divide(Vtx_y[ic]);
    ratio_HadronInND_vy->SetTitle("");
    ratio_HadronInND_vy->Draw("E1 SAME");

    c_Vtx_y[ic]->Write();

    //
    // Overlay 1D histograms on random evt vtx z
    //

    c_Vtx_z[ic] = new TCanvas(TString::Format("c_Random_vtx_z_NdOffAxisPos_%.2f_m", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0]), TString::Format( "FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold ), 700, 500);
    c_Vtx_z[ic]->Clear();
    TPad *uppadVtx_z = new TPad("uppadVtx_z", "", 0, 0.3, 1, 1.0);
    uppadVtx_z->SetBottomMargin(0); uppadVtx_z->Draw();
    TPad *dnpadVtx_z = new TPad("dnpadVtx_z", "", 0, 0.0, 1, 0.29);
    dnpadVtx_z->SetTopMargin(0); dnpadVtx_z->SetBottomMargin(0.3); dnpadVtx_z->SetGridy(); dnpadVtx_z->Draw();

    uppadVtx_z->cd();
    Vtx_z[ic]->GetYaxis()->SetTitle(TString::Format("Events/%.1f cm", Vtx_z_binning));
    Vtx_z[ic]->SetMinimum(PlotMinY);
    Vtx_z[ic]->SetMaximum(PlotMaxY);
    Vtx_z[ic]->SetLineColor(4);
    Vtx_z[ic]->SetStats(0);
    Vtx_z[ic]->Draw("HIST E1");

    Vtx_z_HadronInND[ic]->SetLineColor(2);
    Vtx_z_HadronInND[ic]->SetStats(0);
    Vtx_z_HadronInND[ic]->Draw("HIST E1 SAME");

    Vtx_z_HadronEffWgted[ic]->SetLineColor(3);
    Vtx_z_HadronEffWgted[ic]->SetStats(0);
    Vtx_z_HadronEffWgted[ic]->Draw("HIST E1 SAME");

    // Build Legend
    TLegend* uppadVtx_zL = new TLegend(0.6, 0.5, 0.9, 0.9);
    uppadVtx_zL->SetBorderSize(0); uppadVtx_zL->SetFillStyle(0); uppadVtx_zL->SetNColumns(1);
    uppadVtx_zL->AddEntry(Vtx_z[ic], TString::Format("HadronEff > %.3f + vtxInNDFV", effthreshold), "l");
    uppadVtx_zL->AddEntry(Vtx_z_HadronInND[ic], "ND hadron veto", "l");
    uppadVtx_zL->AddEntry(Vtx_z_HadronEffWgted[ic], "Weighted: 1/HadronEff", "l");
    uppadVtx_zL->Draw();

    uppadVtx_z->Update(); uppadVtx_z->Modified();
    gPad->RedrawAxis(); gPad->SetLogy();
    c_Vtx_z[ic]->cd(); c_Vtx_z[ic]->Update();

    // Vtx_z_HadronEffWgted / Vtx_z
    dnpadVtx_z->cd();
    TH1F *ratio_HadronEffWgted_vz = (TH1F*)Vtx_z_HadronEffWgted[ic]->Clone("ratio_HadronEffWgted_vz"); // Clone to avoid changing the original hist
    ratio_HadronEffWgted_vz->Divide(Vtx_z[ic]);
    ratio_HadronEffWgted_vz->SetTitle("");
    ratio_HadronEffWgted_vz->GetXaxis()->SetTitle("Random vtx z [cm]");
    ratio_HadronEffWgted_vz->GetXaxis()->SetTitleSize(15);
    ratio_HadronEffWgted_vz->GetXaxis()->SetTitleFont(43);
    ratio_HadronEffWgted_vz->GetXaxis()->SetTitleOffset(4.0);
    ratio_HadronEffWgted_vz->GetXaxis()->SetLabelSize(15); // Labels will be 15 pixels
    ratio_HadronEffWgted_vz->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratio_HadronEffWgted_vz->GetYaxis()->SetTitle("ratio");
    ratio_HadronEffWgted_vz->GetYaxis()->CenterTitle();
    ratio_HadronEffWgted_vz->GetYaxis()->SetTitleSize(15);
    ratio_HadronEffWgted_vz->GetYaxis()->SetTitleFont(43);
    ratio_HadronEffWgted_vz->GetYaxis()->SetTitleOffset(.9);
    ratio_HadronEffWgted_vz->GetYaxis()->SetLabelSize(15);
    ratio_HadronEffWgted_vz->GetYaxis()->SetLabelFont(43);
    ratio_HadronEffWgted_vz->Draw("E1");
    // Vtx_z_HadronInND / Vtx_z
    TH1F *ratio_HadronInND_vz = (TH1F*)Vtx_z_HadronInND[ic]->Clone("ratio_HadronInND_vz");
    ratio_HadronInND_vz->Divide(Vtx_z[ic]);
    ratio_HadronInND_vz->SetTitle("");
    ratio_HadronInND_vz->Draw("E1 SAME");

    c_Vtx_z[ic]->Write();

    //
    // Overlay 1D histograms on GENIE generated Numu E with random vtx
    //

    c_FDNeuE_random_vtx[ic] = new TCanvas(TString::Format("c_FDNeuE_NdOffAxisPos_%.2f_m_random_vtx", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0]), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), 700, 500);
    c_FDNeuE_random_vtx[ic]->Clear();
    TPad *uppadNeuE_random_vtx = new TPad("uppadNeuE_random_vtx", "", 0, 0.3, 1, 1.0);
    uppadNeuE_random_vtx->SetBottomMargin(0); uppadNeuE_random_vtx->Draw();
    TPad *dnpadNeuE_random_vtx = new TPad("dnpadNeuE_random_vtx", "", 0, 0.0, 1, 0.29);
    dnpadNeuE_random_vtx->SetTopMargin(0); dnpadNeuE_random_vtx->SetBottomMargin(0.3); dnpadNeuE_random_vtx->SetGridy(); dnpadNeuE_random_vtx->Draw();

    uppadNeuE_random_vtx->cd();
    FDNeuE_random_vtx[ic]->GetYaxis()->SetTitle(TString::Format("Events/%.1f GeV", LepE_binning));
    FDNeuE_random_vtx[ic]->SetMinimum(PlotMinY);
    FDNeuE_random_vtx[ic]->SetMaximum(PlotMaxY);
    FDNeuE_random_vtx[ic]->SetLineColor(4);
    FDNeuE_random_vtx[ic]->SetStats(0);
    FDNeuE_random_vtx[ic]->Draw("HIST E1");

    FDNeuE_random_vtx_HadronInND[ic]->SetLineColor(2);
    FDNeuE_random_vtx_HadronInND[ic]->SetStats(0);
    FDNeuE_random_vtx_HadronInND[ic]->Draw("HIST E1 SAME");

    FDNeuE_random_vtx_HadronEffWgted[ic]->SetLineColor(3);
    FDNeuE_random_vtx_HadronEffWgted[ic]->SetStats(0);
    FDNeuE_random_vtx_HadronEffWgted[ic]->Draw("HIST E1 SAME");

    TLegend* uppadNeuE_random_vtxL = new TLegend(0.6, 0.5, 0.9, 0.9);
    uppadNeuE_random_vtxL->SetBorderSize(0); uppadNeuE_random_vtxL->SetFillStyle(0); uppadNeuE_random_vtxL->SetNColumns(1);
    uppadNeuE_random_vtxL->AddEntry(FDNeuE_random_vtx[ic], TString::Format("HadronEff > %.3f + vtxInNDFV", effthreshold), "l");
    uppadNeuE_random_vtxL->AddEntry(FDNeuE_random_vtx_HadronInND[ic], "ND hadron veto", "l");
    uppadNeuE_random_vtxL->AddEntry(FDNeuE_random_vtx_HadronEffWgted[ic], "Weighted: 1/HadronEff", "l");
    uppadNeuE_random_vtxL->Draw();

    uppadNeuE_random_vtx->Update(); uppadNeuE_random_vtx->Modified();
    gPad->RedrawAxis(); gPad->SetLogy();
    c_FDNeuE_random_vtx[ic]->cd(); c_FDNeuE_random_vtx[ic]->Update();

    // FDNeuE_random_vtx_HadronEffWgted / FDNeuE_random_vtx
    dnpadNeuE_random_vtx->cd();
    TH1F *ratio_HadronEffWgted_FDNeuE_random_vtx = (TH1F*)FDNeuE_random_vtx_HadronEffWgted[ic]->Clone("ratio_HadronEffWgted_FDNeuE_random_vtx");
    ratio_HadronEffWgted_FDNeuE_random_vtx->Divide(FDNeuE_random_vtx[ic]);
    ratio_HadronEffWgted_FDNeuE_random_vtx->SetTitle("");
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetXaxis()->SetTitleSize(15);
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetXaxis()->SetTitleFont(43);
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetXaxis()->SetTitleOffset(4.0);
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetXaxis()->SetLabelSize(15);
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetXaxis()->SetLabelFont(43);
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetYaxis()->SetTitle("ratio");
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetYaxis()->CenterTitle();
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetYaxis()->SetTitleSize(15);
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetYaxis()->SetTitleFont(43);
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetYaxis()->SetTitleOffset(.9);
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetYaxis()->SetLabelSize(15);
    ratio_HadronEffWgted_FDNeuE_random_vtx->GetYaxis()->SetLabelFont(43);
    ratio_HadronEffWgted_FDNeuE_random_vtx->Draw("E1");
    // FDNeuE_random_vtx_HadronInND / FDNeuE_random_vtx
    TH1F *ratio_HadronInND_FDNeuE_random_vtx = (TH1F*)FDNeuE_random_vtx_HadronInND[ic]->Clone("ratio_HadronInND_FDNeuE_random_vtx");
    ratio_HadronInND_FDNeuE_random_vtx->Divide(FDNeuE_random_vtx[ic]);
    ratio_HadronInND_FDNeuE_random_vtx->SetTitle("");
    ratio_HadronInND_FDNeuE_random_vtx->Draw("E1 SAME");

    c_FDNeuE_random_vtx[ic]->Write();

    //
    // Overlay 1D histograms on lepton E with random vtx
    //

    c_FDLepE_random_vtx[ic] = new TCanvas(TString::Format("c_FDLepE_NdOffAxisPos_%.2f_m_random_vtx", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0]), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), 700, 500);
    c_FDLepE_random_vtx[ic]->Clear();
    TPad *uppadLepE_random_vtx = new TPad("uppadLepE_random_vtx", "", 0, 0.3, 1, 1.0);
    uppadLepE_random_vtx->SetBottomMargin(0); uppadLepE_random_vtx->Draw();
    TPad *dnpadLepE_random_vtx = new TPad("dnpadLepE_random_vtx", "", 0, 0.0, 1, 0.29);
    dnpadLepE_random_vtx->SetTopMargin(0); dnpadLepE_random_vtx->SetBottomMargin(0.3); dnpadLepE_random_vtx->SetGridy(); dnpadLepE_random_vtx->Draw();

    uppadLepE_random_vtx->cd();
    FDLepE_random_vtx[ic]->GetYaxis()->SetTitle(TString::Format("Events/%.1f GeV", LepE_binning));
    FDLepE_random_vtx[ic]->SetMinimum(PlotMinY);
    FDLepE_random_vtx[ic]->SetMaximum(PlotMaxY);
    FDLepE_random_vtx[ic]->SetLineColor(4);
    FDLepE_random_vtx[ic]->SetStats(0);
    FDLepE_random_vtx[ic]->Draw("HIST E1");

    FDLepE_random_vtx_HadronInND[ic]->SetLineColor(2);
    FDLepE_random_vtx_HadronInND[ic]->SetStats(0);
    FDLepE_random_vtx_HadronInND[ic]->Draw("HIST E1 SAME");

    FDLepE_random_vtx_HadronEffWgted[ic]->SetLineColor(3);
    FDLepE_random_vtx_HadronEffWgted[ic]->SetStats(0);
    FDLepE_random_vtx_HadronEffWgted[ic]->Draw("HIST E1 SAME");

    TLegend* uppadLepE_random_vtxL = new TLegend(0.6, 0.5, 0.9, 0.9);
    uppadLepE_random_vtxL->SetBorderSize(0); uppadLepE_random_vtxL->SetFillStyle(0); uppadLepE_random_vtxL->SetNColumns(1);
    uppadLepE_random_vtxL->AddEntry(FDLepE_random_vtx[ic], TString::Format("HadronEff > %.3f + vtxInNDFV", effthreshold), "l");
    uppadLepE_random_vtxL->AddEntry(FDLepE_random_vtx_HadronInND[ic], "ND hadron veto", "l");
    uppadLepE_random_vtxL->AddEntry(FDLepE_random_vtx_HadronEffWgted[ic], "Weighted: 1/HadronEff", "l");
    uppadLepE_random_vtxL->Draw();

    uppadLepE_random_vtx->Update(); uppadLepE_random_vtx->Modified();
    gPad->RedrawAxis(); gPad->SetLogy();
    c_FDLepE_random_vtx[ic]->cd(); c_FDLepE_random_vtx[ic]->Update();

    // FDLepE_random_vtx_HadronEffWgted / FDLepE_random_vtx
    dnpadLepE_random_vtx->cd();
    TH1F *ratio_HadronEffWgted_FDLepE_random_vtx = (TH1F*)FDLepE_random_vtx_HadronEffWgted[ic]->Clone("ratio_HadronEffWgted_FDLepE_random_vtx");
    ratio_HadronEffWgted_FDLepE_random_vtx->Divide(FDLepE_random_vtx[ic]);
    ratio_HadronEffWgted_FDLepE_random_vtx->SetTitle("");
    ratio_HadronEffWgted_FDLepE_random_vtx->GetXaxis()->SetTitle("E_{#mu} [GeV]");
    ratio_HadronEffWgted_FDLepE_random_vtx->GetXaxis()->SetTitleSize(15);
    ratio_HadronEffWgted_FDLepE_random_vtx->GetXaxis()->SetTitleFont(43);
    ratio_HadronEffWgted_FDLepE_random_vtx->GetXaxis()->SetTitleOffset(4.0);
    ratio_HadronEffWgted_FDLepE_random_vtx->GetXaxis()->SetLabelSize(15);
    ratio_HadronEffWgted_FDLepE_random_vtx->GetXaxis()->SetLabelFont(43);
    ratio_HadronEffWgted_FDLepE_random_vtx->GetYaxis()->SetTitle("ratio");
    ratio_HadronEffWgted_FDLepE_random_vtx->GetYaxis()->CenterTitle();
    ratio_HadronEffWgted_FDLepE_random_vtx->GetYaxis()->SetTitleSize(15);
    ratio_HadronEffWgted_FDLepE_random_vtx->GetYaxis()->SetTitleFont(43);
    ratio_HadronEffWgted_FDLepE_random_vtx->GetYaxis()->SetTitleOffset(.9);
    ratio_HadronEffWgted_FDLepE_random_vtx->GetYaxis()->SetLabelSize(15);
    ratio_HadronEffWgted_FDLepE_random_vtx->GetYaxis()->SetLabelFont(43);
    ratio_HadronEffWgted_FDLepE_random_vtx->Draw("E1");
    // FDLepE_random_vtx_HadronInND / FDLepE_random_vtx
    TH1F *ratio_HadronInND_FDLepE_random_vtx = (TH1F*)FDLepE_random_vtx_HadronInND[ic]->Clone("ratio_HadronInND_FDLepE_random_vtx");
    ratio_HadronInND_FDLepE_random_vtx->Divide(FDLepE_random_vtx[ic]);
    ratio_HadronInND_FDLepE_random_vtx->SetTitle("");
    ratio_HadronInND_FDLepE_random_vtx->Draw("E1 SAME");

    c_FDLepE_random_vtx[ic]->Write();

    //
    // Overlay 1D histograms on sim hadron deposits with random vtx
    //

    c_FDHadE_random_vtx[ic] = new TCanvas(TString::Format("c_FDHadE_NdOffAxisPos_%.2f_m_random_vtx", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0]), TString::Format("FD evt in ND: off-axis position = %.2f m, hadron eff > %.3f, random vtx", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0], effthreshold), 700, 500);
    c_FDHadE_random_vtx[ic]->Clear();
    TPad *uppadHadE_random_vtx = new TPad("uppadHadE_random_vtx", "", 0, 0.3, 1, 1.0);
    uppadHadE_random_vtx->SetBottomMargin(0); uppadHadE_random_vtx->Draw();
    TPad *dnpadHadE_random_vtx = new TPad("dnpadHadE_random_vtx", "", 0, 0.0, 1, 0.29);
    dnpadHadE_random_vtx->SetTopMargin(0); dnpadHadE_random_vtx->SetBottomMargin(0.3); dnpadHadE_random_vtx->SetGridy(); dnpadHadE_random_vtx->Draw();

    uppadHadE_random_vtx->cd();
    FDHadE_random_vtx[ic]->GetYaxis()->SetTitle(TString::Format("Events/%.0f MeV", HadE_binning));
    FDHadE_random_vtx[ic]->SetMinimum(PlotMinY);
    FDHadE_random_vtx[ic]->SetMaximum(PlotMaxY);
    FDHadE_random_vtx[ic]->SetLineColor(4);
    FDHadE_random_vtx[ic]->SetStats(0);
    FDHadE_random_vtx[ic]->Draw("HIST E1");

    FDHadE_random_vtx_HadronInND[ic]->SetLineColor(2);
    FDHadE_random_vtx_HadronInND[ic]->SetStats(0);
    FDHadE_random_vtx_HadronInND[ic]->Draw("HIST E1 SAME");

    FDHadE_random_vtx_HadronEffWgted[ic]->SetLineColor(3);
    FDHadE_random_vtx_HadronEffWgted[ic]->SetStats(0);
    FDHadE_random_vtx_HadronEffWgted[ic]->Draw("HIST E1 SAME");

    TLegend* uppadHadE_random_vtxL = new TLegend(0.6, 0.5, 0.9, 0.9);
    uppadHadE_random_vtxL->SetBorderSize(0); uppadHadE_random_vtxL->SetFillStyle(0); uppadHadE_random_vtxL->SetNColumns(1);
    uppadHadE_random_vtxL->AddEntry(FDHadE_random_vtx[ic], TString::Format("HadronEff > %.3f + vtxInNDFV", effthreshold), "l");
    uppadHadE_random_vtxL->AddEntry(FDHadE_random_vtx_HadronInND[ic], "ND hadron veto", "l");
    uppadHadE_random_vtxL->AddEntry(FDHadE_random_vtx_HadronEffWgted[ic], "Weighted: 1/HadronEff", "l");
    uppadHadE_random_vtxL->Draw();

    uppadHadE_random_vtx->Update(); uppadHadE_random_vtx->Modified();
    gPad->RedrawAxis(); gPad->SetLogy();
    c_FDHadE_random_vtx[ic]->cd(); c_FDHadE_random_vtx[ic]->Update();

    // FDHadE_random_vtx_HadronEffWgted / FDHadE_random_vtx
    dnpadHadE_random_vtx->cd();
    TH1F *ratio_HadronEffWgted_FDHadE_random_vtx = (TH1F*)FDHadE_random_vtx_HadronEffWgted[ic]->Clone("ratio_HadronEffWgted_FDHadE_random_vtx");
    ratio_HadronEffWgted_FDHadE_random_vtx->Divide(FDHadE_random_vtx[ic]);
    ratio_HadronEffWgted_FDHadE_random_vtx->SetTitle("");
    ratio_HadronEffWgted_FDHadE_random_vtx->GetXaxis()->SetTitle("E_{hadron} [MeV]");
    ratio_HadronEffWgted_FDHadE_random_vtx->GetXaxis()->SetTitleSize(15);
    ratio_HadronEffWgted_FDHadE_random_vtx->GetXaxis()->SetTitleFont(43);
    ratio_HadronEffWgted_FDHadE_random_vtx->GetXaxis()->SetTitleOffset(4.0);
    ratio_HadronEffWgted_FDHadE_random_vtx->GetXaxis()->SetLabelSize(15);
    ratio_HadronEffWgted_FDHadE_random_vtx->GetXaxis()->SetLabelFont(43);
    ratio_HadronEffWgted_FDHadE_random_vtx->GetYaxis()->SetTitle("ratio");
    ratio_HadronEffWgted_FDHadE_random_vtx->GetYaxis()->CenterTitle();
    ratio_HadronEffWgted_FDHadE_random_vtx->GetYaxis()->SetTitleSize(15);
    ratio_HadronEffWgted_FDHadE_random_vtx->GetYaxis()->SetTitleFont(43);
    ratio_HadronEffWgted_FDHadE_random_vtx->GetYaxis()->SetTitleOffset(.9);
    ratio_HadronEffWgted_FDHadE_random_vtx->GetYaxis()->SetLabelSize(15);
    ratio_HadronEffWgted_FDHadE_random_vtx->GetYaxis()->SetLabelFont(43);
    ratio_HadronEffWgted_FDHadE_random_vtx->Draw("E1");
    // FDHadE_random_vtx_HadronInND / FDHadE_random_vtx
    TH1F *ratio_HadronInND_FDHadE_random_vtx = (TH1F*)FDHadE_random_vtx_HadronInND[ic]->Clone("ratio_HadronInND_FDHadE_random_vtx");
    ratio_HadronInND_FDHadE_random_vtx->Divide(FDHadE_random_vtx[ic]);
    ratio_HadronInND_FDHadE_random_vtx->SetTitle("");
    ratio_HadronInND_FDHadE_random_vtx->Draw("E1 SAME");

    c_FDHadE_random_vtx[ic]->Write();

    // Similar overlay but with stepwise increased evt vx
    for ( int jc = 0; jc < mplot; jc++ ) {

      //
      // Overlay 1D histograms on GENIE generated Numu E
      //

      c_FDNeuE[ic*mplot + jc] = new TCanvas(TString::Format("c_FDNeuE_NdOffAxisPos_%.2f_m_Vtx_x_%.1f_cm", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0], jc*ND_local_x_stepsize + ND_local_x_min ), TString::Format( "FD evt in ND: off-axis position = %.2f m, vtx x = %.1f cm, hadron eff > %.3f", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0], jc*ND_local_x_stepsize + ND_local_x_min, effthreshold ), 700, 500);
      c_FDNeuE[ic*mplot + jc]->Clear();
      TPad *uppadNeuE = new TPad("uppadNeuE", "", 0, 0.3, 1, 1.0);
      uppadNeuE->SetBottomMargin(0); uppadNeuE->Draw();
      TPad *dnpadNeuE = new TPad("dnpadNeuE", "", 0, 0.0, 1, 0.29);
      dnpadNeuE->SetTopMargin(0); dnpadNeuE->SetBottomMargin(0.3); dnpadNeuE->SetGridy(); dnpadNeuE->Draw();

      uppadNeuE->cd();
      FDNeuE[ic*mplot + jc]->GetYaxis()->SetTitle(TString::Format("Events/%.1f GeV", LepE_binning));
      FDNeuE[ic*mplot + jc]->SetMinimum(PlotMinY);
      FDNeuE[ic*mplot + jc]->SetMaximum(PlotMaxY);
      FDNeuE[ic*mplot + jc]->SetLineColor(4);
      FDNeuE[ic*mplot + jc]->SetStats(0);
      FDNeuE[ic*mplot + jc]->Draw("HIST E1");

      FDNeuE_HadronInND[ic*mplot + jc]->SetLineColor(2);
      FDNeuE_HadronInND[ic*mplot + jc]->SetStats(0);
      FDNeuE_HadronInND[ic*mplot + jc]->Draw("HIST E1 SAME");

      FDNeuE_HadronEffWgted[ic*mplot + jc]->SetLineColor(3);
      FDNeuE_HadronEffWgted[ic*mplot + jc]->SetStats(0);
      FDNeuE_HadronEffWgted[ic*mplot + jc]->Draw("HIST E1 SAME");

      TLegend* uppadNeuEL = new TLegend(0.6, 0.5, 0.9, 0.9);
      uppadNeuEL->SetBorderSize(0); uppadNeuEL->SetFillStyle(0); uppadNeuEL->SetNColumns(1);
      uppadNeuEL->AddEntry(FDNeuE[ic*mplot + jc], TString::Format("HadronEff > %.3f + vtxInNDFV", effthreshold), "l");
      uppadNeuEL->AddEntry(FDNeuE_HadronInND[ic*mplot + jc], "ND hadron veto", "l");
      uppadNeuEL->AddEntry(FDNeuE_HadronEffWgted[ic*mplot + jc], "Weighted: 1/HadronEff", "l");
      uppadNeuEL->Draw();

      uppadNeuE->Update(); uppadNeuE->Modified();
      gPad->RedrawAxis(); gPad->SetLogy();
      c_FDNeuE[ic*mplot + jc]->cd(); c_FDNeuE[ic*mplot + jc]->Update();

      // Plot ratio = FDNeuE_HadronEffWgted / FDNeuE
      dnpadNeuE->cd();
      TH1F *ratioNeuE = (TH1F*)FDNeuE_HadronEffWgted[ic*mplot + jc]->Clone("ratioNeuE");
      ratioNeuE->Divide(FDNeuE[ic*mplot + jc]);
      ratioNeuE->SetTitle("");
      ratioNeuE->GetXaxis()->SetTitle("E_{#nu} [GeV]");
      ratioNeuE->GetXaxis()->SetTitleSize(15);
      ratioNeuE->GetXaxis()->SetTitleFont(43);
      ratioNeuE->GetXaxis()->SetTitleOffset(4.0);
      ratioNeuE->GetXaxis()->SetLabelSize(15);
      ratioNeuE->GetXaxis()->SetLabelFont(43);
      ratioNeuE->GetYaxis()->SetTitle("ratio");
      ratioNeuE->GetYaxis()->CenterTitle();
      ratioNeuE->GetYaxis()->SetTitleSize(15);
      ratioNeuE->GetYaxis()->SetTitleFont(43);
      ratioNeuE->GetYaxis()->SetTitleOffset(.9);
      ratioNeuE->GetYaxis()->SetLabelSize(15);
      ratioNeuE->GetYaxis()->SetLabelFont(43);
      ratioNeuE->Draw("E1");

      c_FDNeuE[ic*mplot + jc]->Write();

      //
      // Overlay 1D histograms on lepton E
      //

      c_FDLepE[ic*mplot + jc] = new TCanvas(TString::Format("c_FDLepE_NdOffAxisPos_%.2f_m_Vtx_x_%.1f_cm", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0], jc*ND_local_x_stepsize + ND_local_x_min ), TString::Format( "FD evt in ND: off-axis position = %.2f m, vtx x = %.1f cm, hadron eff > %.3f", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0], jc*ND_local_x_stepsize + ND_local_x_min, effthreshold ), 700, 500);
      c_FDLepE[ic*mplot + jc]->Clear();
      TPad *uppadLepE = new TPad("uppadLepE", "", 0, 0.3, 1, 1.0);
      uppadLepE->SetBottomMargin(0); uppadLepE->Draw();
      TPad *dnpadLepE = new TPad("dnpadLepE", "", 0, 0.0, 1, 0.29);
      dnpadLepE->SetTopMargin(0); dnpadLepE->SetBottomMargin(0.3); dnpadLepE->SetGridy(); dnpadLepE->Draw();

      uppadLepE->cd();
      FDLepE[ic*mplot + jc]->GetYaxis()->SetTitle(TString::Format("Events/%.1f GeV", LepE_binning));
      FDLepE[ic*mplot + jc]->SetMinimum(PlotMinY);
      FDLepE[ic*mplot + jc]->SetMaximum(PlotMaxY);
      FDLepE[ic*mplot + jc]->SetLineColor(4);
      FDLepE[ic*mplot + jc]->SetStats(0);
      FDLepE[ic*mplot + jc]->Draw("HIST E1");

      FDLepE_HadronInND[ic*mplot + jc]->SetLineColor(2);
      FDLepE_HadronInND[ic*mplot + jc]->SetStats(0);
      FDLepE_HadronInND[ic*mplot + jc]->Draw("HIST E1 SAME");

      FDLepE_HadronEffWgted[ic*mplot + jc]->SetLineColor(3);
      FDLepE_HadronEffWgted[ic*mplot + jc]->SetStats(0);
      FDLepE_HadronEffWgted[ic*mplot + jc]->Draw("HIST E1 SAME");

      TLegend* uppadLepEL = new TLegend(0.6, 0.5, 0.9, 0.9);
      uppadLepEL->SetBorderSize(0); uppadLepEL->SetFillStyle(0); uppadLepEL->SetNColumns(1);
      uppadLepEL->AddEntry(FDLepE[ic*mplot + jc], TString::Format("HadronEff > %.3f + vtxInNDFV", effthreshold), "l");
      uppadLepEL->AddEntry(FDLepE_HadronInND[ic*mplot + jc], "ND hadron veto", "l");
      uppadLepEL->AddEntry(FDLepE_HadronEffWgted[ic*mplot + jc], "Weighted: 1/HadronEff", "l");
      uppadLepEL->Draw();

      uppadLepE->Update(); uppadLepE->Modified();
      gPad->RedrawAxis(); gPad->SetLogy();
      c_FDLepE[ic*mplot + jc]->cd(); c_FDLepE[ic*mplot + jc]->Update();

      // Plot ratio = FDLepE_HadronEffWgted / FDLepE
      dnpadLepE->cd();
      TH1F *ratioLepE = (TH1F*)FDLepE_HadronEffWgted[ic*mplot + jc]->Clone("ratioLepE");
      ratioLepE->Divide(FDLepE[ic*mplot + jc]);
      ratioLepE->SetTitle("");
      ratioLepE->GetXaxis()->SetTitle("E_{#mu} [GeV]");
      ratioLepE->GetXaxis()->SetTitleSize(15);
      ratioLepE->GetXaxis()->SetTitleFont(43);
      ratioLepE->GetXaxis()->SetTitleOffset(4.0);
      ratioLepE->GetXaxis()->SetLabelSize(15);
      ratioLepE->GetXaxis()->SetLabelFont(43);
      ratioLepE->GetYaxis()->SetTitle("ratio");
      ratioLepE->GetYaxis()->CenterTitle();
      ratioLepE->GetYaxis()->SetTitleSize(15);
      ratioLepE->GetYaxis()->SetTitleFont(43);
      ratioLepE->GetYaxis()->SetTitleOffset(.9);
      ratioLepE->GetYaxis()->SetLabelSize(15);
      ratioLepE->GetYaxis()->SetLabelFont(43);
      ratioLepE->Draw("E1");

      c_FDLepE[ic*mplot + jc]->Write();

      //
      // Overlay 1D histograms on sim hadron deposits
      //

      c_FDHadE[ic*mplot + jc] = new TCanvas(TString::Format("c_FDHadE_NdOffAxisPos_%.2f_m_Vtx_x_%.1f_cm", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0], jc*ND_local_x_stepsize + ND_local_x_min ), TString::Format( "FD evt in ND: off-axis position = %.2f m, vtx x = %.1f cm, hadron eff > %.3f", ic*ND_off_axis_pos_stepsize + OffAxisPoints[0], jc*ND_local_x_stepsize + ND_local_x_min, effthreshold ), 700, 500);
      c_FDHadE[ic*mplot + jc]->Clear();
      TPad *uppadHadE = new TPad("uppadHadE", "", 0, 0.3, 1, 1.0);
      uppadHadE->SetBottomMargin(0); uppadHadE->Draw();
      TPad *dnpadHadE = new TPad("dnpadHadE", "", 0, 0.0, 1, 0.29);
      dnpadHadE->SetTopMargin(0); dnpadHadE->SetBottomMargin(0.3); dnpadHadE->SetGridy(); dnpadHadE->Draw();

      uppadHadE->cd();
      FDHadE[ic*mplot + jc]->GetYaxis()->SetTitle(TString::Format("Events/%.0f MeV", HadE_binning));
      FDHadE[ic*mplot + jc]->SetMinimum(PlotMinY);
      FDHadE[ic*mplot + jc]->SetMaximum(PlotMaxY);
      FDHadE[ic*mplot + jc]->SetLineColor(4);
      FDHadE[ic*mplot + jc]->SetStats(0);
      FDHadE[ic*mplot + jc]->Draw("HIST E1");

      FDHadE_HadronInND[ic*mplot + jc]->SetLineColor(2);
      FDHadE_HadronInND[ic*mplot + jc]->SetStats(0);
      FDHadE_HadronInND[ic*mplot + jc]->Draw("HIST E1 SAME");

      FDHadE_HadronEffWgted[ic*mplot + jc]->SetLineColor(3);
      FDHadE_HadronEffWgted[ic*mplot + jc]->SetStats(0);
      FDHadE_HadronEffWgted[ic*mplot + jc]->Draw("HIST E1 SAME");

      TLegend* uppadHadEL = new TLegend(0.6, 0.5, 0.9, 0.9);
      uppadHadEL->SetBorderSize(0); uppadHadEL->SetFillStyle(0); uppadHadEL->SetNColumns(1);
      uppadHadEL->AddEntry(FDHadE[ic*mplot + jc], TString::Format("HadronEff > %.3f + vtxInNDFV", effthreshold), "l");
      uppadHadEL->AddEntry(FDHadE_HadronInND[ic*mplot + jc], "ND hadron veto", "l");
      uppadHadEL->AddEntry(FDHadE_HadronEffWgted[ic*mplot + jc], "Weighted: 1/HadronEff", "l");
      uppadHadEL->Draw();

      uppadHadE->Update(); uppadHadE->Modified();
      gPad->RedrawAxis(); gPad->SetLogy();
      c_FDHadE[ic*mplot + jc]->cd(); c_FDHadE[ic*mplot + jc]->Update();

      // Plot ratio = FDHadE_HadronEffWgted / FDHadE
      dnpadHadE->cd();
      TH1F *ratioHadE = (TH1F*)FDHadE_HadronEffWgted[ic*mplot + jc]->Clone("ratioHadE");
      ratioHadE->Divide(FDHadE[ic*mplot + jc]);
      ratioHadE->SetTitle("");
      ratioHadE->GetXaxis()->SetTitle("E_{hadron} [MeV]");
      ratioHadE->GetXaxis()->SetTitleSize(15);
      ratioHadE->GetXaxis()->SetTitleFont(43);
      ratioHadE->GetXaxis()->SetTitleOffset(4.0);
      ratioHadE->GetXaxis()->SetLabelSize(15);
      ratioHadE->GetXaxis()->SetLabelFont(43);
      ratioHadE->GetYaxis()->SetTitle("ratio");
      ratioHadE->GetYaxis()->CenterTitle();
      ratioHadE->GetYaxis()->SetTitleSize(15);
      ratioHadE->GetYaxis()->SetTitleFont(43);
      ratioHadE->GetYaxis()->SetTitleOffset(.9);
      ratioHadE->GetYaxis()->SetLabelSize(15);
      ratioHadE->GetYaxis()->SetLabelFont(43);
      ratioHadE->Draw("E1");

      c_FDHadE[ic*mplot + jc]->Write();

      //
      // Delete objects to avoid potential memory leak
      //

      delete uppadNeuE;  delete dnpadNeuE;  delete uppadNeuEL;  delete ratioNeuE;
      delete uppadLepE;  delete dnpadLepE;  delete uppadLepEL;  delete ratioLepE;
      delete uppadHadE;  delete dnpadHadE;  delete uppadHadEL;  delete ratioHadE;
    } // end mplot

    delete uppadVtx_x;           delete dnpadVtx_x;           delete uppadVtx_xL;           delete ratio_HadronEffWgted_vx; delete ratio_HadronInND_vx;
    delete uppadVtx_y;           delete dnpadVtx_y;           delete uppadVtx_yL;           delete ratio_HadronEffWgted_vy; delete ratio_HadronInND_vy;
    delete uppadVtx_z;           delete dnpadVtx_z;           delete uppadVtx_zL;           delete ratio_HadronEffWgted_vz; delete ratio_HadronInND_vz;
    delete uppadNeuE_random_vtx; delete dnpadNeuE_random_vtx; delete uppadNeuE_random_vtxL; delete ratio_HadronEffWgted_FDNeuE_random_vtx; delete ratio_HadronInND_FDNeuE_random_vtx;
    delete uppadLepE_random_vtx; delete dnpadLepE_random_vtx; delete uppadLepE_random_vtxL; delete ratio_HadronEffWgted_FDLepE_random_vtx; delete ratio_HadronInND_FDLepE_random_vtx;
    delete uppadHadE_random_vtx; delete dnpadHadE_random_vtx; delete uppadHadE_random_vtxL; delete ratio_HadronEffWgted_FDHadE_random_vtx; delete ratio_HadronInND_FDHadE_random_vtx;

  }   // end nplot

  outFile->Close();

} // end macro
