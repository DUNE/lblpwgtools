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
// Include customized functions and constants
R__ADD_INCLUDE_PATH(include)
#include "step3_helpers.h"

void PlotAllFDEventsInNDvsEtrimInFDVsEhadFD()
{
  gROOT->Reset();

  // Input FDroot file
  TString FileIn = "RootFiles/FDGeoEff_72040717_0.root";
  // Read effValues
  TChain *t_effValues = new TChain("effValues");
  t_effValues->Add(FileIn.Data());
  Int_t iwritten;
  Float_t totEnergyFDatND_f;
  t_effValues->SetBranchAddress("iwritten",         &iwritten);
  t_effValues->SetBranchAddress("totEnergyFDatND_f",   &totEnergyFDatND_f);


  TChain *t_PosVec = new TChain("effPosND");
  //("PosVec");
  //("effPosND");
  t_PosVec->Add(FileIn.Data());

  gROOT->Reset();
  vector<Double_t> *ND_LAr_dtctr_pos_vec = 0; // unit: cm, ND off-axis choices for each FD evt
  vector<Double_t> *ND_vtx_vx_vec = 0;       // unit: cm, vtx x choices for each FD evt in ND volume
  vector<Int_t> *iwritten_vec = 0;
  TBranch *b_ND_LAr_dtctr_pos_vec = 0;
  TBranch *b_iwritten_vec = 0;
  TBranch *b_ND_vtx_vx_vec = 0;

  t_PosVec->SetBranchAddress("ND_LAr_dtctr_pos_vec",         &ND_LAr_dtctr_pos_vec , &b_ND_LAr_dtctr_pos_vec);
  t_PosVec->SetBranchAddress("ND_vtx_vx_vec",             &ND_vtx_vx_vec,      &b_ND_vtx_vx_vec);
  t_PosVec->SetBranchAddress("iwritten_vec",               &iwritten_vec,        &b_iwritten_vec);

  Long64_t tentry = t_PosVec->LoadTree(0);
  b_ND_LAr_dtctr_pos_vec->GetEntry(tentry);
  b_ND_vtx_vx_vec->GetEntry(tentry);
  b_iwritten_vec->GetEntry(tentry);

  Int_t ND_LAr_dtctr_pos_vec_size = ND_LAr_dtctr_pos_vec->size();
  Int_t ND_vtx_vx_vec_size = ND_vtx_vx_vec->size();
  Int_t iwritten_vec_size = iwritten_vec->size();
  Int_t tot_size = ND_LAr_dtctr_pos_vec_size*ND_vtx_vx_vec_size;

  TH1D* hist_FDTotEnergy = new TH1D("hist_FDTotEnergy", "hist_FDTotEnergy", 25000, 0, 25000);

  // Loop all events
  for (Int_t i_iwritten : *iwritten_vec)
  {
    cout << "i_iwritten: " << i_iwritten << "\n";
    Int_t i_entry = tot_size * i_iwritten;

    for (i_entry ; i_entry < tot_size * (i_iwritten+1); i_entry++ )
    {
      t_effValues->GetEntry(i_entry);
      t_PosVec->GetEntry(i_entry);
      hist_FDTotEnergy->Fill(totEnergyFDatND_f);
    }


  }
  hist_FDTotEnergy->Scale(1.0/72); // we have 72 vtxX positions therefore instead of 1 event in the histo we have 72 entries

  hist_FDTotEnergy->Draw("hist");


  TFile* fileWithEtrimHistos = new TFile("OutPutEtrimFiles/FileWithHist_10EventsFromJobSub_NoTrimX_CoeffsAndOAPoswithSameEff_sampleOADetPosAsCAFs.root", "READ");
  fileWithEtrimHistos->cd();

  int nFDEvents = iwritten_vec->size();
  cout<<" we have "<<nFDEvents<<" events at FD"<<endl;

  //read in the coeff linearly combined histos using the OA coeffs
  TH1D* HistEtrimAllVtxXTimesCoeff[nFDEvents];
  // histos all vtx X with no coeff applied (coeff=1 case)
  TH1D* HistEtrimAllVtxX[nFDEvents];
  //efficiency graph
  TGraph* EfficiencyGraph[nFDEvents];


  TCanvas* CanvasPlotEtrimTimesCoeff = new TCanvas("CanvasPlotEtrimTimesCoeff", "CanvasPlotEtrimTimesCoeff");
  CanvasPlotEtrimTimesCoeff->Divide(2,5);
  TCanvas* CanvasEfficiencyVsVtxX = new TCanvas("CanvasEfficiencyVsVtxX", "CanvasEfficiencyVsVtxX");
  CanvasEfficiencyVsVtxX->Divide(2,5);


  for (Int_t i_iwritten = 0; i_iwritten<nFDEvents; i_iwritten++){
    HistEtrimAllVtxXTimesCoeff[i_iwritten] = (TH1D*) fileWithEtrimHistos->Get(Form("HistEtrimAllVtxXTimesCoeff_FDEvt_%d", i_iwritten));
    HistEtrimAllVtxXTimesCoeff[i_iwritten]->SetDirectory(0);
    HistEtrimAllVtxX[i_iwritten] = (TH1D*) fileWithEtrimHistos->Get(Form("HistEtrimAllVtxX_FDEvt_%d", i_iwritten));

    EfficiencyGraph[i_iwritten] = (TGraph*) fileWithEtrimHistos->Get(Form("GraphEffficiency_FDEventNr_%d",i_iwritten));
    EfficiencyGraph[i_iwritten]->GetXaxis()->SetTitle("vtx_x (m)");
    EfficiencyGraph[i_iwritten]->GetYaxis()->SetTitle("ND Efficiency");

    CanvasPlotEtrimTimesCoeff->cd(i_iwritten+1);
    HistEtrimAllVtxXTimesCoeff[i_iwritten]->SetLineColor(2);
    HistEtrimAllVtxXTimesCoeff[i_iwritten]->SetLineWidth(2);
    HistEtrimAllVtxXTimesCoeff[i_iwritten]->Draw("hist");
    HistEtrimAllVtxX[i_iwritten]->SetLineColor(4);
    HistEtrimAllVtxX[i_iwritten]->SetLineWidth(2);
    HistEtrimAllVtxX[i_iwritten]->Draw("histsames");

    CanvasEfficiencyVsVtxX->cd(i_iwritten+1);
    EfficiencyGraph[i_iwritten]->Draw("AP");


  }

  TH1D* AllNDEventsGeomCorrectedVsEtrimCoeffApplied = (TH1D*) HistEtrimAllVtxXTimesCoeff[0]->Clone();
  AllNDEventsGeomCorrectedVsEtrimCoeffApplied->Reset();

  TH1D* AllNDEventsGeomCorrectedVsEtrimNoCoeffApplied = (TH1D*) HistEtrimAllVtxX[0]->Clone();
  AllNDEventsGeomCorrectedVsEtrimNoCoeffApplied->Reset();

  for (Int_t i_iwritten = 0; i_iwritten<nFDEvents; i_iwritten++){
    AllNDEventsGeomCorrectedVsEtrimCoeffApplied->Add(HistEtrimAllVtxXTimesCoeff[i_iwritten]);
    AllNDEventsGeomCorrectedVsEtrimNoCoeffApplied->Add(HistEtrimAllVtxX[i_iwritten]);
  }

  TCanvas* CanvasAllEventsInND = new TCanvas("CanvasAllEventsInND", "CanvasAllEventsInND");
  CanvasAllEventsInND->cd();


  hist_FDTotEnergy->SetLineWidth(2);
  hist_FDTotEnergy->SetLineColor(1);
  hist_FDTotEnergy->GetYaxis()->SetRangeUser(1e-9, 1.2);
  hist_FDTotEnergy->Draw("hist");
  AllNDEventsGeomCorrectedVsEtrimNoCoeffApplied->Draw("histsames");
  AllNDEventsGeomCorrectedVsEtrimCoeffApplied->Draw("histsames");

  TLegend* leg = new TLegend();
  leg->AddEntry(hist_FDTotEnergy, "FD Events");
  leg->AddEntry(AllNDEventsGeomCorrectedVsEtrimCoeffApplied, "ND Events, OA Coefficients applied");
  leg->AddEntry(AllNDEventsGeomCorrectedVsEtrimNoCoeffApplied, "ND Events, Coefficients = 1");
  leg->Draw("same");
  //===finished reading the hadron energy at FD

}
