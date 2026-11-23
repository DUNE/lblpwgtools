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

void ReadEffNtuple()
{
  gROOT->Reset();

  // Input FDroot file
  TString FileIn = "/pnfs/dune/scratch/users/flynnguo/FDGeoEffinND/FDGeoEff_2811722_879.root";
  // TString FileIn = "/dune/app/users/flynnguo/NDEff/DUNE_ND_GeoEff/bin/Output_FDGeoEff.root";
  // TString FileIn = "/pnfs/dune/scratch/users/flynnguo/FDGeoEffinND/FDGeoEff_2293930_80.root";
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
  Double_t ND_OffAxis_MeanEff;
  t_effValues->SetBranchAddress("iwritten",         &iwritten);
  t_effValues->SetBranchAddress("ND_LAr_dtctr_pos",   &ND_LAr_dtctr_pos);
  t_effValues->SetBranchAddress("ND_LAr_vtx_pos",   &ND_LAr_vtx_pos);
  t_effValues->SetBranchAddress("ND_GeoEff",        &ND_GeoEff);
  t_effValues->SetBranchAddress("ND_OffAxis_MeanEff",        &ND_OffAxis_MeanEff);


  // Read PosVec
  TChain *t_PosVec = new TChain("effPosND");
  t_PosVec->Add(FileIn.Data());

  gROOT->Reset();
  vector<Double_t> *ND_LAr_dtctr_pos_vec = 0; // unit: cm, ND off-axis choices for each FD evt
  vector<Double_t> *ND_vtx_vx_vec = 0;       // unit: cm, vtx x choices for each FD evt in ND volume
  vector<Int_t> *iwritten_vec = 0;
  TBranch *b_ND_LAr_dtctr_pos_vec = 0;
  TBranch *b_ND_vtx_vx_vec = 0;
  TBranch *b_iwritten_vec = 0;
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
  Int_t hadronhit_n_plots = tot_size * N_throws;

  Int_t nentries = t_effValues->GetEntries();

  // Output
  TFile * outFile = new TFile("EffPlots_ivy.root", "RECREATE");
  TDirectory *IP2d =(TDirectory*)outFile->mkdir("2dGeoEff");//create a new folder in the root file
  TDirectory *IP1d =(TDirectory*)outFile->mkdir("1dGeoEff");//create a new folder in the root file

  // Canvas
  TCanvas** c_2dGeoEff = new TCanvas*[iwritten_vec_size];
  TProfile2D** h_2dGeoEff = new TProfile2D*[iwritten_vec_size];

  TCanvas** c_1dGeoEff = new TCanvas*[iwritten_vec_size];
  TGraph** h_1dGeoEff = new TGraph*[ND_LAr_dtctr_pos_vec_size];

  TH1F *h_eff = new TH1F("h_eff", "h_eff", 12, 0, 1.2);

  // Set Palette
  gStyle->SetPalette(1);
  Int_t iwritten_effcounter = 1;
  Int_t iwrittennum = 0;

  Double_t TotMeanEff = 0.;

  // Loop all events
  for (Int_t i_iwritten : *iwritten_vec)
  {
    cout << "i_iwritten: " << i_iwritten << "\n";
    Int_t i_entry = tot_size * i_iwritten;
    Int_t OnAxisEff_counter = 0;
    Double_t OnAxisEff = 0.;
    Double_t MeanOnAxisEff = 0.;
    Double_t Leff = 0.;
    Double_t Reff = 0.;
    Int_t Leff_counter = 0;
    Int_t Reff_counter = 0;


    for (i_entry ; i_entry < tot_size * (i_iwritten+1); i_entry++ )
    {
      t_effValues->GetEntry(i_entry);
      if (ND_LAr_dtctr_pos == -50)
      {
        if(verbose) cout << "ND_LAr_vtx_pos: " << ND_LAr_vtx_pos << endl;
        if(ND_LAr_vtx_pos<-250)
        {Leff += ND_GeoEff;Leff_counter++;}
        else if(ND_LAr_vtx_pos>250)
        {Reff += ND_GeoEff;Reff_counter++;}
        else
        {OnAxisEff += ND_GeoEff;OnAxisEff_counter++;}
      }
    }
    if(verbose) cout << "OnAxisEff: " << OnAxisEff << endl;
    if(verbose) cout << "OnAxisEff_counter: " << OnAxisEff_counter << endl;
    if(verbose) cout << "Leff: " << Leff*1.0/Leff_counter << endl;
    if(verbose) cout << "Reff: " << Reff*1.0/Reff_counter << endl;


    MeanOnAxisEff = (Leff*1.0/Leff_counter+OnAxisEff*1.0+Reff*1.0/Reff_counter)/(OnAxisEff_counter+2);
    h_eff->Fill(MeanOnAxisEff);
    cout << "MeanOnAxisEff: " << ND_OffAxis_MeanEff << endl;
    TotMeanEff += MeanOnAxisEff;
    if(verbose) cout << "TotMeanEff: " << TotMeanEff << endl;

    //
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    // Create canvas for 2d GeoEff
    TString c_2dGeoEff_name = Form("c_2dGeoEff_event_%d", i_iwritten);
    TString c_2dGeoEff_title = Form("2D GeoEff_event_%d", i_iwritten);
    c_2dGeoEff[i_iwritten] = new TCanvas(c_2dGeoEff_name, c_2dGeoEff_title, 600, 400);
    c_2dGeoEff[i_iwritten]->Clear();
    c_2dGeoEff[i_iwritten]->SetLeftMargin(0.15);
    c_2dGeoEff[i_iwritten]->SetRightMargin(0.15);
    // Create TProfile2D
    TString h_2dGeoEff_name = Form("h_2dGeoEff_event_%d", i_iwritten);
    h_2dGeoEff[i_iwritten] = new TProfile2D(h_2dGeoEff_name,c_2dGeoEff_title,50,-350,350,50,-3500,500,0,1);
    h_2dGeoEff[i_iwritten]->SetStats(0);
    h_2dGeoEff[i_iwritten]->SetMinimum(0);
    h_2dGeoEff[i_iwritten]->SetMaximum(1);
    h_2dGeoEff[i_iwritten]->GetYaxis()->SetTitle("ND_LAr_dtctr_pos [cm]");
    h_2dGeoEff[i_iwritten]->GetXaxis()->SetTitle("ND_LAr_vtx_pos [cm]");
    h_2dGeoEff[i_iwritten]->GetZaxis()->SetTitle("ND_GeoEff");
    //------------------------------------------------------------------------------
    // Loop entries matching current iwritten

    for (i_entry ; i_entry < tot_size * (i_iwritten+1); i_entry++ )
    {
      t_effValues->GetEntry(i_entry);
      // Fill 2D
      h_2dGeoEff[i_iwritten]->Fill(ND_LAr_vtx_pos,ND_LAr_dtctr_pos,ND_GeoEff);
    } // end i_entry
    //------------------------------------------------------------------------------
    // Save canvas for 2d GeoEff
    c_2dGeoEff[i_iwritten]->cd();
    // TExec *ex1 = new TExec("ex1","set_plot_style();");
    h_2dGeoEff[i_iwritten]->Draw("COLZ");
    // ex1->Draw();
    outFile->cd("2dGeoEff");
    c_2dGeoEff[i_iwritten]->Write();
    // c_2dGeoEff[i_iwritten]->SaveAs( TString::Format("Plots/2D_GeoEff_event_%d.pdf",iwritten ) );

    gPad->Update();
    gPad->Modified();
    gSystem->ProcessEvents();
    c_2dGeoEff[i_iwritten]->Close();
    //
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //
    // Create canvas for 1d GeoEff
    TString c_1dGeoEff_name = Form("c_1dGeoEff_event_%d", i_iwritten);
    TString c_1dGeoEff_title = Form("1D GeoEff_event_%d", i_iwritten);
    c_1dGeoEff[i_iwritten] = new TCanvas(c_1dGeoEff_name, c_1dGeoEff_title, 1120,1692,921,585);
    c_1dGeoEff[i_iwritten]->Clear();
    c_1dGeoEff[i_iwritten]->SetLeftMargin(0.12);
    c_1dGeoEff[i_iwritten]->SetRightMargin(0.15);
    c_1dGeoEff[i_iwritten]->SetGrid();

    TMultiGraph *mg = new TMultiGraph("mg",c_1dGeoEff_title);
    TLegend *leg = new TLegend(0.8626653,0.3074398,0.9857579,0.6947484,NULL,"brNDC");

    // Fill 1D
    Int_t ND_LAr_dtctr_pos_counter = 0;
    Int_t ND_OffAxis_effcounter = 0;
    Int_t ND_Lar_effcounter = 0;



    for (Int_t i_ND_LAr_dtctr_pos: *ND_LAr_dtctr_pos_vec)
    {
      Int_t m=0;
      Double_t x_ND_LAr_vtx_pos[ND_vtx_vx_vec_size];
      Double_t y_geoeff[ND_vtx_vx_vec_size];
      Int_t i_entry = tot_size * i_iwritten;
      for (i_entry ; i_entry < tot_size * (i_iwritten+1); i_entry++ )
      {
        t_effValues->GetEntry(i_entry);
        if (ND_LAr_dtctr_pos == i_ND_LAr_dtctr_pos)
        {
          x_ND_LAr_vtx_pos[m] = ND_LAr_vtx_pos;
          y_geoeff[m] = ND_GeoEff;
          if(ND_GeoEff == 1 ) ND_Lar_effcounter++;
          m++;
        }
      }
      if(ND_Lar_effcounter>=8) ND_OffAxis_effcounter++;

      // Add a function to zoom in the Plots


      TString h_1dGeoEff_name = Form("ND_LAr_dtctr_pos_%d [cm]", i_ND_LAr_dtctr_pos);

      h_1dGeoEff[ND_LAr_dtctr_pos_counter] = new TGraph(ND_vtx_vx_vec_size, x_ND_LAr_vtx_pos, y_geoeff);
      // h_1dGeoEff[ND_LAr_dtctr_pos_counter]->SetMinimum(0);
      // h_1dGeoEff[ND_LAr_dtctr_pos_counter]->SetMaximum(1.1);
      h_1dGeoEff[ND_LAr_dtctr_pos_counter]->SetMarkerStyle(ND_LAr_dtctr_pos_counter+20);
      // h_1dGeoEff[ND_LAr_dtctr_pos_counter]->SetMarkerColor((ND_LAr_dtctr_pos_counter+2)/2); // Use when ND_LAr pos > 10
      h_1dGeoEff[ND_LAr_dtctr_pos_counter]->SetMarkerColor(ND_LAr_dtctr_pos_counter+1);
      mg->Add(h_1dGeoEff[ND_LAr_dtctr_pos_counter]);

      // only chose even number
      // if( ND_LAr_dtctr_pos_counter % 2 == 0)
      // {
        mg->Add(h_1dGeoEff[ND_LAr_dtctr_pos_counter]);
        leg->AddEntry(h_1dGeoEff[ND_LAr_dtctr_pos_counter], h_1dGeoEff_name, "p");
      // }
      ND_LAr_dtctr_pos_counter++;
      cout << "ND_LAr_dtctr_pos_counter: " << ND_LAr_dtctr_pos_counter << endl;
    }

    if(ND_OffAxis_effcounter>=10) iwritten_effcounter++;
    cout << "iwritten_effcounter: " << iwritten_effcounter << endl;

    //------------------------------------------------------------------------------
    // Save canvas for 1d GeoEff
    c_1dGeoEff[i_iwritten]->cd();
    mg->Draw("apl");
    mg->GetXaxis()->SetTitle("ND_LAr_vtx_pos [cm]");
    mg->GetYaxis()->SetTitle("GeoEff");
    // Comment out SetRangeUser if we need zoom in
    mg->GetYaxis()->SetRangeUser(0,1.05);
    leg->Draw();
    outFile->cd("1dGeoEff");
    c_1dGeoEff[i_iwritten]->Write();
    c_1dGeoEff[i_iwritten]->SaveAs( TString::Format("Plots/1D_GeoEff_event_%d.pdf",iwritten ) );
    gPad->Update();
    gPad->Modified();
    gSystem->ProcessEvents();
    c_1dGeoEff[i_iwritten]->Close();
    iwrittennum++;

  } // end iwritten_vec

  double percent = iwritten_effcounter*1.0/iwrittennum;
  cout << "eff == 1: " << percent << endl;

  Double_t meanEff = TotMeanEff*1.0/iwrittennum;
  cout << "ND_LAr_dtctr_pos == -50 (~ On Axis) meanEff: " << meanEff << endl;
  h_eff->GetXaxis()->SetTitle("GeoEff");
  h_eff->Write();


  delete[] h_2dGeoEff;
  delete[] c_2dGeoEff;
  delete[] h_1dGeoEff;
  delete[] c_1dGeoEff;

  outFile->Close();
} // end ReadNtuple

// Find out throws which caused dip in the geoeff
void ThrowPass()
{
  gROOT->Reset();

  // Input FDroot file
  TString FileIn = "/dune/app/users/flynnguo/NDEff/DUNE_ND_GeoEff/bin/Output_FDGeoEff_2293930_80.root";


  TChain *t_Throws = new TChain("ThrowsFD");
  t_Throws->Add(FileIn.Data());
  vector<float> *throwVtxY=0;
  vector<float> *throwVtxZ=0;
  vector<float> *throwRot=0;
  t_Throws->SetBranchAddress("throwVtxY",                     &throwVtxY);
  t_Throws->SetBranchAddress("throwVtxZ",                     &throwVtxZ);
  t_Throws->SetBranchAddress("throwRot",                     &throwRot);

  TChain *t_effTree = new TChain("effTreeND");
  t_effTree->Add(FileIn.Data());
  vector<vector<vector<uint64_t> > > *hadron_throw_result=0;
  t_effTree->SetBranchAddress("hadron_throw_result",                     &hadron_throw_result);

  TChain *t_effValues = new TChain("effValues");
  t_effValues->Add(FileIn.Data());
  Int_t iwritten;
  Double_t ND_LAr_dtctr_pos;
  Double_t ND_LAr_vtx_pos;
  Double_t ND_GeoEff;
  t_effValues->SetBranchAddress("iwritten",         &iwritten);
  t_effValues->SetBranchAddress("ND_LAr_dtctr_pos",   &ND_LAr_dtctr_pos);
  t_effValues->SetBranchAddress("ND_LAr_vtx_pos",   &ND_LAr_vtx_pos);
  t_effValues->SetBranchAddress("ND_GeoEff",        &ND_GeoEff);

  Int_t nentries = t_effValues->GetEntries();
  cout<< "t_effValues entry: " << nentries << endl;
  // Int_t nentries_a = t_effTree->GetEntries();
  // cout<< "t_effTree entry: " << nentries_a << endl;  same result as nentries
  int i_entry = 0;
  vector<int> ithrow1;
  vector<int> ithrow2;
  for(i_entry; i_entry<nentries; i_entry++)
  {
    t_effTree->GetEntry(i_entry);
    t_effValues->GetEntry(i_entry);
    if (iwritten!=0) continue;
    if (ND_LAr_dtctr_pos!=0) continue;
    if (ND_LAr_vtx_pos!=-216 && ND_LAr_vtx_pos!=-264) continue; // only pick vtx at 168cm and 216cm
    cout << "i_entry: " << i_entry << ", iwritten: " << iwritten << ", ND_LAr_dtctr_pos: " << ND_LAr_dtctr_pos << ", ND_LAr_vtx_pos: " << ND_LAr_vtx_pos << ", ND_GeoEff: " << ND_GeoEff << endl;
    //

    for ( vector<vector<vector<uint64_t> > >::iterator it_veto_size = hadron_throw_result->begin(); it_veto_size != hadron_throw_result->end(); ++it_veto_size )
    {
      for ( vector<vector<uint64_t> >::iterator it_veto_energy = it_veto_size->begin(); it_veto_energy != it_veto_size->end(); ++it_veto_energy )
      {

        // Every 64 throw result is a chunk
        // current test case: each evt has 64*64 throws, so 64 chunks

        int counter    = 0;

        for ( vector<uint64_t>::iterator it_chunk = it_veto_energy->begin(); it_chunk != it_veto_energy->end(); ++it_chunk )
        {
          counter++;
          // cout << "chunk #" << counter << ": " << *it_chunk << endl;

          for ( unsigned int ithrow = 0; ithrow < 64; ithrow++ )
          {
            t_Throws->GetEntry((counter-1)*64 + ithrow);
            // For the numerator, only consider throws where throwed FD evt vtx x/y/z is in ND FV, same as what is done for ND evts
            // For now, we use mu start pos as evt vtx pos, random throws for y/z are stored in the ThrowsFD tree
            if ( FDEffCalc_cfg::IsInNDFV(ND_LAr_vtx_pos, throwVtxY->at( (counter-1)*64 + ithrow ) - NDLAr_OnAxis_offset[1], throwVtxZ->at( (counter-1)*64 + ithrow ) - NDLAr_OnAxis_offset[2]))
            {
              uint64_t throw_result = (*it_chunk) & ( ((uint64_t)1)<<(ithrow%64) );
              if (verbose) std::cout << "                    throw #" << ithrow+1 << ": " << throw_result << std::endl;
              // Count no. of throws passed hadron containment requirement
              if (throw_result == 0 && ND_LAr_vtx_pos == -216)
              {
                ithrow1.emplace_back((counter-1)*64 + ithrow);
                cout << "-24cm, ithrow: " << (counter-1)*64 + ithrow << endl;
              }
              if (throw_result != 0 && ND_LAr_vtx_pos == -264)
              {
                ithrow2.emplace_back((counter-1)*64 + ithrow);
                cout << "-72cm, ithrow: " << (counter-1)*64 + ithrow << endl;
              }
            } // end if FD event passed ND FV cut
          }   // end loop over 64 throws in a chunk
        }     // end loop over 64-throw chunks
      }     // end loop over veto energy
    }       // end loop over veto size
  }//end get entry

  // Initialise a vector
  // to store the common values
  // and an iterator
  // to traverse this vector
  vector<int> v(ithrow1.size() + ithrow2.size());
  vector<int>::iterator it, st;

  it = set_intersection(ithrow1.begin(),
                        ithrow1.end(),
                        ithrow2.begin(),
                        ithrow2.end(),
                        v.begin());
  vector<int> common_v;
  cout << "\nCommon elements:\n";
  int counter_size = 0;
  for (st = v.begin(); st != it; ++st)
  {
    cout << *st << ", ";
    common_v.emplace_back(*st);
    counter_size++;
  }
  cout << '\n';
  cout << "common_vsize: " << common_v.size() << '\n';
  cout << "size: " << counter_size << '\n';


}

//Find out the fraction of dip
void Fraction()
{
  gROOT->Reset();

  // Input FDroot file
  TString FileIn = "/dune/app/users/flynnguo/NDEff/DUNE_ND_GeoEff/bin/Output_FDGeoEff_2293930_988.root";


  TChain *t_effValues = new TChain("effValues");
  t_effValues->Add(FileIn.Data());
  Int_t iwritten;
  Double_t ND_LAr_dtctr_pos;
  Double_t ND_LAr_vtx_pos;
  Double_t ND_GeoEff;
  t_effValues->SetBranchAddress("iwritten",         &iwritten);
  t_effValues->SetBranchAddress("ND_LAr_dtctr_pos",   &ND_LAr_dtctr_pos);
  t_effValues->SetBranchAddress("ND_LAr_vtx_pos",   &ND_LAr_vtx_pos);
  t_effValues->SetBranchAddress("ND_GeoEff",        &ND_GeoEff);



}
