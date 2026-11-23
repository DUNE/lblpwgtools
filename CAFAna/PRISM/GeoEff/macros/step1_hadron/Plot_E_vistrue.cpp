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

void Plot_E_vistrue() // /pnfs/dune/persistent/users/flynnguo/myFDntuples/myntuple_61454381_*.root
{
  gStyle->SetOptStat(0);
  //
  // Read branch from input trees
  //
  TString FileIn = "/pnfs/dune/persistent/users/flynnguo/FDGeoEffinND/FDGeoEff_2811722/FDGeoEff_2811722_all.root";
  // TString FileIn = "/pnfs/dune/scratch/users/flynnguo/FDGeoEffinND/FDGeoEff_526487_998.root";
  TChain *effTreeND = new TChain("effTreeND");
  effTreeND->Add(FileIn.Data());

  double ND_E_vis_true;                 // True vis energy

  TChain *effValues = new TChain("effValues");
  effValues->Add(FileIn.Data());
  double ND_LAr_dtctr_pos;
  double ND_GeoEff;
  double ND_LAr_vtx_pos;

  effTreeND->SetBranchAddress("ND_E_vis_true",                      &ND_E_vis_true);
  effValues->SetBranchAddress("ND_LAr_dtctr_pos",                     &ND_LAr_dtctr_pos);
  effValues->SetBranchAddress("ND_GeoEff",                          &ND_GeoEff);
  effValues->SetBranchAddress("ND_LAr_vtx_pos",                         &ND_LAr_vtx_pos);

  // Create output files
  // TFile * outFile = new TFile("E_vis_true_d.pdf", "RECREATE");

  // Create Canvas
  TCanvas *c1 = new TCanvas("E_true","E_true",1600,1200);
  c1->Clear();
  c1->Divide(4,3);// All
  // c1->Divide(3,2);// Edge


  Int_t plot_total_num = 12; // 12: all, 6: edge
  // Create hist arrays
  TH1D** hist_ND_E_vis_true = new TH1D*[plot_total_num];
  TH1D** hist_ND_E_vis_true_eff_cut = new TH1D*[plot_total_num];
  TH1::AddDirectory(kFALSE);

  TPad** uppad = new TPad*[plot_total_num];
  TLegend** uppad_L = new TLegend*[plot_total_num];
  TPad** dnpad = new TPad*[plot_total_num];
  TH1F** ratio_Eff_cut = new TH1F*[plot_total_num];

  Int_t OffAxispos = 0; // OnAxis, units: cm
  // Int_t OffAxispos = -2800; // OffAxis, units: cm
  // Set range of plots
  Int_t n = 50; //7:-300 to -250 (edge); 50 : -300 to 300 (all)



  // Draw different plots with different ND_LAr_vtx_pos
  for (Int_t plot_num = 0; plot_num < plot_total_num; plot_num++)
  {
    // Set hist arrays
    hist_ND_E_vis_true[plot_num] = new TH1D("hist_ND_E_vis_true","hist_E_vis_true",20,0,5);
    hist_ND_E_vis_true[plot_num]->SetLineColor(2);
    hist_ND_E_vis_true[plot_num]->SetLineWidth(1);
    hist_ND_E_vis_true_eff_cut[plot_num] = new TH1D("hist_ND_E_vis_true_eff_cut","hist_ND_E_vis_true_eff_cut",20,0,5);
    hist_ND_E_vis_true_eff_cut[plot_num]->SetLineColor(4);
    hist_ND_E_vis_true_eff_cut[plot_num]->SetLineWidth(1);
  }

  // Output file
  // ofstream myfile;
  //  myfile.open ("E_vis_true_check.txt");
  // Loop over all events
  int nentries = 0; // Total input events
  // int ientry;
  nentries = effValues->GetEntries();
  // nentries = effTreeND->GetEntries();
  cout<< "nentries:" << nentries<<endl;
  int effTreeND_ientry;
  // for ( int i = 1; i <= (nentries/330); i++ )
  for ( int ientry = 0; ientry < nentries; ientry++ )
  {
      //15 off axis positions * 22 vtx positions,
      // ientry = i*(3*22)-1; //only choose On axis events 308-329, ND_LAr_dtctr_pos=0cm
      // ientry = (i-1)*330; //only choose Off axis events 0,1,...,  ND_LAr_dtctr_pos=-2800cm
    effTreeND_ientry = ientry/(3*22);
    // cout << "effTreeND_ientry: " << effTreeND_ientry << endl;
    effTreeND->GetEntry(effTreeND_ientry);
    effValues->GetEntry(ientry);

    if (ND_LAr_dtctr_pos != OffAxispos) continue;
    // cout << "ientry:" << ientry <<", ND_LAr_dtctr_pos: " << ND_LAr_dtctr_pos << ", ND_LAr_vtx_pos: " << ND_LAr_vtx_pos << ", ND_E_vis_true: " << ND_E_vis_true << endl;
    // myfile << "ientry:" << ientry <<", ND_LAr_dtctr_pos: " << ND_LAr_dtctr_pos << ", ND_LAr_vtx_pos: " << ND_LAr_vtx_pos << ", ND_E_vis_true: " << ND_E_vis_true << "\n";

    for (Int_t plot_num = 0; plot_num < plot_total_num; plot_num++)
    {
      // Set bin size edges
      Int_t Left_edge = -300 + plot_num*n;
      Int_t Right_edge = Left_edge + n;

      if (Left_edge < ND_LAr_vtx_pos && ND_LAr_vtx_pos < Right_edge)
      {
        cout << " Left_edge: "<< Left_edge << ", Right_edge: " << Right_edge << endl;
        // myfile << " Left_edge: "<< Left_edge << ", Right_edge: " << Right_edge << ",  ND_LAr_vtx_pos: " << ND_LAr_vtx_pos << "\n";

        hist_ND_E_vis_true[plot_num]->Fill(ND_E_vis_true);
        if (ND_GeoEff>0.1)
        {
          hist_ND_E_vis_true_eff_cut[plot_num]->Fill(ND_E_vis_true);
          // myfile << "eff>0.1, ND_E_vis_true: " << ND_E_vis_true << "\n";
        }
        if(ND_GeoEff<=0.1)
        {
          cout << "ientry: " <<ientry << ", ND_LAr_dtctr_pos:" << ND_LAr_dtctr_pos << ", ND_LAr_vtx_pos:" << ND_LAr_vtx_pos << ", ND_E_vis_true: " << ND_E_vis_true << ", ND_GeoEff: " << ND_GeoEff << endl;
          // myfile << "ientry: " <<ientry << ", ND_LAr_dtctr_pos:" << ND_LAr_dtctr_pos << ", ND_LAr_vtx_pos:" << ND_LAr_vtx_pos << ", ND_E_vis_true: " << ND_E_vis_true << ", ND_GeoEff: " << ND_GeoEff << "\n";
        }
      }
    }
  }// end ientry

  for (Int_t plot_num = 0; plot_num < plot_total_num; plot_num++)
  {
    // Set bin size edges
    Int_t Left_edge = -300 + plot_num*n;
    Int_t Right_edge = Left_edge + n;

    // Draw Plots
    c1->cd(plot_num+1);
    c1->GetPad(plot_num+1)->SetLeftMargin(0.05);
    c1->GetPad(plot_num+1)->SetRightMargin(0.05);
    c1->GetPad(plot_num+1)->SetTopMargin(0.05);
    c1->GetPad(plot_num+1)->SetBottomMargin(0.05);

    // Seperate into 2 pads
    uppad[plot_num] = new TPad("uppad", "", 0, 0.4, 1, 1.0); // xlow, ylow, xup, yup
    uppad[plot_num]->SetBottomMargin(0);
    uppad[plot_num]->SetGridx();
    uppad[plot_num]->SetLogy();
    uppad[plot_num]->Draw();
    uppad[plot_num]->cd();
    hist_ND_E_vis_true[plot_num]->SetStats(0);
    hist_ND_E_vis_true[plot_num]->Draw("HIST");
    TString hist_ND_E_vis_true_title = Form("On Axis = %d cm, %d cm < ND_LAr < %d cm", OffAxispos, Left_edge, Right_edge);
    hist_ND_E_vis_true[plot_num]->SetTitle(hist_ND_E_vis_true_title);
    // hist_ND_E_vis_true[plot_num]->SetTitle(" ");

    hist_ND_E_vis_true[plot_num]->SetTitleSize(15);
    hist_ND_E_vis_true[plot_num]->SetTitleFont(43);
    hist_ND_E_vis_true[plot_num]->GetYaxis()->SetTitle("# of events ");
    hist_ND_E_vis_true[plot_num]->GetYaxis()->SetTitleSize(10);
    hist_ND_E_vis_true[plot_num]->GetYaxis()->SetTitleFont(43);
    hist_ND_E_vis_true[plot_num]->GetYaxis()->SetTitleOffset(6);
    hist_ND_E_vis_true_eff_cut[plot_num]->Draw("SAME");


    uppad_L[plot_num] = new TLegend(0.5, 0.5, 0.9, 0.9);
    uppad_L[plot_num]->SetTextSize(0.045);
    uppad_L[plot_num]->AddEntry(hist_ND_E_vis_true[plot_num],TString::Format("raw hist_E_vis_true"),"l");
    uppad_L[plot_num]->AddEntry(hist_ND_E_vis_true_eff_cut[plot_num],TString::Format("hist_E_vis_true w/ eff>0.1"),"l");
    uppad_L[plot_num]->Draw();


    c1->cd(plot_num+1);

    dnpad[plot_num] = new TPad("dnpad", "", 0, 0.05, 1, 0.4);
    dnpad[plot_num]->SetTopMargin(0);
    dnpad[plot_num]->SetBottomMargin(0.2);
    dnpad[plot_num]->SetGridy();
    dnpad[plot_num]->SetGridx();
    dnpad[plot_num]->Draw();
    dnpad[plot_num]->cd();

    //hist_ND_E_vis_true_eff_cut / hist_ND_E_vis_true
    ratio_Eff_cut[plot_num] = (TH1F*)hist_ND_E_vis_true_eff_cut[plot_num]->Clone("hist_ND_E_vis_true_eff_cut"); // Clone to avoid changing the original hist
    ratio_Eff_cut[plot_num]->SetLineColor(kBlack);
    ratio_Eff_cut[plot_num]->SetMinimum(0.5);
    ratio_Eff_cut[plot_num]->SetMaximum(1.05);
    ratio_Eff_cut[plot_num]->Sumw2(); //Create structure to store sum of squares of weights.
    ratio_Eff_cut[plot_num]->SetStats(0);
    ratio_Eff_cut[plot_num]->SetLineWidth(0); // 0: No error bars; 1: error bars
    ratio_Eff_cut[plot_num]->Divide(hist_ND_E_vis_true[plot_num]);
    ratio_Eff_cut[plot_num]->SetTitle("");
    ratio_Eff_cut[plot_num]->GetXaxis()->SetTitle("ND_E_vis_true [GeV]");
    ratio_Eff_cut[plot_num]->GetXaxis()->SetTitleSize(10);
    ratio_Eff_cut[plot_num]->GetXaxis()->SetTitleFont(43);
    ratio_Eff_cut[plot_num]->GetXaxis()->SetTitleOffset(10);
    ratio_Eff_cut[plot_num]->GetXaxis()->SetLabelSize(10); // Labels will be 15 pixels
    ratio_Eff_cut[plot_num]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratio_Eff_cut[plot_num]->GetYaxis()->SetTitle("ratio cut/raw  ");
    ratio_Eff_cut[plot_num]->GetYaxis()->SetNdivisions(505);
    ratio_Eff_cut[plot_num]->GetYaxis()->SetTitleSize(10);
    ratio_Eff_cut[plot_num]->GetYaxis()->SetTitleFont(43);
    ratio_Eff_cut[plot_num]->GetYaxis()->SetTitleOffset(6);
    ratio_Eff_cut[plot_num]->GetYaxis()->SetLabelSize(10);
    ratio_Eff_cut[plot_num]->GetYaxis()->SetLabelFont(43);
    ratio_Eff_cut[plot_num]->SetMarkerStyle(21);
    ratio_Eff_cut[plot_num]->SetMarkerColor(6); // 1: black; 3: green; 6: pink
    ratio_Eff_cut[plot_num]->SetMarkerSize(0.5);// 0.4: w/ error bars; 0.5: w/o error bars
    ratio_Eff_cut[plot_num]->Draw("ep");// Draw error bars

    gPad->Update();
    gPad->Modified();
    gSystem->ProcessEvents();
  }

  c1->SaveAs("E_vis_true.pdf");


  // delete all hist variables
  delete[] hist_ND_E_vis_true;
  delete[] hist_ND_E_vis_true_eff_cut;
  delete[] uppad;
  delete[] uppad_L;
  delete[] dnpad;
  delete[] ratio_Eff_cut;

  // myfile.close();
  // outFile->Close();

} // end Plot_Evisture
