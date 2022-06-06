//------------------------------------------------------------------
// Overlay several histograms from a root file first use for sigma variation
// Run this macro:
// source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.24.02/x86_64-centos7-gcc48-opt/bin/thisroot.sh
// root -l -b -q QuickHistPlotter.C
//------------------------------------------------------------------

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

void QuickHistPlotter()
{
  bool debug = true; // Print out for debug purpose
  if ( debug ) std::cout << "set input" << std::endl;
  Double_t MaxAbsY = 0.02;

  TString systname = "PlaceHolder";
  TFile *file0 = TFile::Open( TString::Format("/dune/app/users/weishi/PRISMPred/PRISMPred_EVisReco_%s.root", systname.Data() ) );

  // Oscillation channels: FD_nu_numu, FD_nu_nue, FD_nub_numu, FD_nub_nue
  TString channame = "FD_nub_nue";
  TString chanlabel;
  if ( channame == "FD_nu_numu" ) {
    chanlabel = "#nu_{#mu} #rightarrow #nu_{#mu}";
  } else if ( channame == "FD_nu_nue" ) {
    chanlabel = "#nu_{#mu} #rightarrow #nu_{e}";
  } else if ( channame == "FD_nub_numu" ) {
    chanlabel = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#mu}";
  } else if ( channame == "FD_nub_nue" ) {
    chanlabel = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e}";
  } else {
    std::cout << "!!! Please check input osc channel !!! " << std::endl;
    exit(1);
  }

  // 1D hists
  vector<TString> histnames;
  histnames.clear();
  histnames.push_back("NDDataCorr_FDExtrap"); // total PRISM prediction
  histnames.push_back("FDFluxCorr");
  histnames.push_back("FDNuTauCCBkg");
  histnames.push_back("FDIntrinsicBkg");
  histnames.push_back("FDWSBkg");
  histnames.push_back("FDNCBkg");
  histnames.push_back("FDWrongLepBkg");
  histnames.push_back("NDData_FDExtrap"); // ND linear combination

  // 2D hists
  vector<TString> hist2dndnames;
  hist2dndnames.clear();
  hist2dndnames.push_back("NDWSBkg_293kA"); // normalize to itself and nd data
  hist2dndnames.push_back("NDWSBkg_280kA");
  hist2dndnames.push_back("NDNCBkg_293kA");
  hist2dndnames.push_back("NDNCBkg_280kA");
  hist2dndnames.push_back("NDWrongLepBkg_293kA");
  hist2dndnames.push_back("NDWrongLepBkg_280kA");
  // Unfolding and smearing matrices
  hist2dndnames.push_back("Unfold_Matrices/ND_SmearingMatrix");
  hist2dndnames.push_back("Unfold_Matrices/FD_SmearingMatrix");

  // MC efficiency corr matrices
  vector<TString> histeffsel;
  histeffsel.clear();
  histeffsel.push_back("MCEfficiency/NDEff_293kA"); // 2d eff = sel/unsel
  histeffsel.push_back("MCEfficiency/FDEff");

  if ( debug ) std::cout << "clone plots" << std::endl;

  TH1F *FDUnOscPred_nom = (TH1F*)file0->Get( TString::Format("numu_EvMatch_nom/%s/FDUnOscPred", channame.Data() ) )->Clone("FDUnOscPred_nom"); // For normalization in disp channels
  TH1F *DataPred_Total_nom = (TH1F*)file0->Get( TString::Format("numu_EvMatch_nom/%s/DataPred_Total", channame.Data() ) )->Clone("DataPred_Total_nom"); // For normalization in app channels
  TH2F *NDData_293kA_nom = (TH2F*)file0->Get( TString::Format("numu_EvMatch_nom/%s/NDData_293kA", channame.Data() ) )->Clone("NDData_293kA_nom"); // ND data shouldn't change
  TH2F *NDData_280kA_nom = (TH2F*)file0->Get( TString::Format("numu_EvMatch_nom/%s/NDData_280kA", channame.Data() ) )->Clone("NDData_280kA_nom");

  TFile myPlot( TString::Format("/dune/app/users/weishi/Overlay/Overlay_%s.root", systname.Data() ), "RECREATE");
  if ( debug ) std::cout << "output" << std::endl;

  // Draw 1d hists component
  for ( size_t i = 0; i < histnames.size(); i++ ) {

    //
    // Clone histogram
    //

    if ( debug ) std::cout << "looking at histogram: " << histnames[i] << std::endl;

    //
    // Check whether dir exist before clone !!!
    // Some systematic can not be varied to +/3 or +/2 because of wierd behavior
    //
    TH1F *nom = (TH1F*)file0->Get( TString::Format("numu_EvMatch_nom/%s/%s",  channame.Data(), histnames[i].Data()) )->Clone("nom");
    TH1F *n1  = (TH1F*)file0->Get( TString::Format("numu_EvMatch_%s_-1/%s/%s", systname.Data(), channame.Data(), histnames[i].Data()) )->Clone("n1");
    TH1F *p1  = (TH1F*)file0->Get( TString::Format("numu_EvMatch_%s_1/%s/%s",  systname.Data(), channame.Data(), histnames[i].Data()) )->Clone("p1");

    //
    // Calculate (varied - nom)/normalization: FD UnOsc for numu, FD osc for nue (nom)
    //

    n1->Add(nom, -1);
    p1->Add(nom, -1);

    if ( channame == "FD_nu_numu" || channame == "FD_nub_numu" ) {
      n1->Divide(FDUnOscPred_nom);
      p1->Divide(FDUnOscPred_nom);
    } else if ( channame == "FD_nu_nue" || channame == "FD_nub_nue" ) {
      n1->Divide(DataPred_Total_nom);
      p1->Divide(DataPred_Total_nom);
    }

    //
    // Plot
    //

    TCanvas *canvas = new TCanvas("canvas", "canvas", 700, 500);
    canvas->cd(); canvas->SetGridx(); canvas->SetGridy();
    n1->SetTitle( TString::Format("%s: %s", chanlabel.Data(), histnames[i].Data()) );
    n1->SetMinimum(-1.*MaxAbsY);
    n1->SetMaximum(MaxAbsY);
    n1->SetStats(0);
    n1->GetXaxis()->SetTitle("Reco E_{vis} (GeV)");
    if ( channame == "FD_nu_numu" || channame == "FD_nub_numu" ) {
      n1->GetYaxis()->SetTitle("(N_{varied} - N_{nom})/N_{FD, unosc}");
    } else if ( channame == "FD_nu_nue" || channame == "FD_nub_nue" ) {
      n1->GetYaxis()->SetTitle("(N_{varied} - N_{nom})/N_{FD, osc}");
    }
    n1->GetYaxis()->SetTitleSize(0.05);
    n1->GetYaxis()->SetTitleOffset(0.9);

    n1->SetLineColor(kRed+1);  n1->SetLineStyle(1); n1->SetLineWidth(2); n1->Draw("HIST");
    p1->SetLineColor(kBlue+1); p1->SetLineStyle(1); p1->SetLineWidth(2); p1->Draw("HISTSAME");

    TLegend* legend = new TLegend(0.1, 0.7, 0.4, 0.9);
    legend->SetBorderSize(0); legend->SetFillStyle(0); legend->SetNColumns(1);
    legend->AddEntry(n1, TString::Format("%s: -1#sigma", systname.Data()), "l");
    legend->AddEntry(p1, TString::Format("%s: 1#sigma",  systname.Data()), "l");
    legend->Draw();
    canvas->Write();

    // delete created objects in current scope
    delete nom;
    delete n1;
    delete p1;
    delete canvas;
    delete legend;

  } // end loop over 1d hists

  //
  // Draw 2D ND bkg
  //

  for ( size_t i = 0; i < hist2dndnames.size(); i++ ) {

    if ( debug ) std::cout << "looking at 2D histogram: " << hist2dndnames[i] << std::endl;

    //
    // Clone histogram
    //

    // Check whether dir exist before clone !!!
    // Some systematic can not be varied to +/3 or +/2 because of wierd behavior
    TH2F *nom = (TH2F*)file0->Get( TString::Format("numu_EvMatch_nom/%s/%s",   channame.Data(), hist2dndnames[i].Data()) )->Clone("nom");
    TH2F *n1  = (TH2F*)file0->Get( TString::Format("numu_EvMatch_%s_-1/%s/%s", systname.Data(), channame.Data(), hist2dndnames[i].Data()) )->Clone("n1");
    TH2F *p1  = (TH2F*)file0->Get( TString::Format("numu_EvMatch_%s_1/%s/%s",  systname.Data(), channame.Data(), hist2dndnames[i].Data()) )->Clone("p1");

    //
    // Calculate (varied - nom)/nom, common for all 2d hists
    //

    // If a bin is empty, no division will be performed for the bins and a zero bin content is returned.
    n1->Add(nom, -1); n1->Divide(nom);
    p1->Add(nom, -1); p1->Divide(nom);

    //
    // Plot
    //

    TCanvas *canvas = new TCanvas("canvas", "canvas", 700, 500);
    canvas->cd(); canvas->SetGridx(); canvas->SetGridy();
    n1->SetTitle( TString::Format("%s: %s, %s -1#sigma, (N_{varied} - N_{nom})/N_{nom}", chanlabel.Data(), hist2dndnames[i].Data(), systname.Data()) );
    n1->SetStats(0);
    // Set axis titles
    if ( hist2dndnames[i].Contains("Bkg") ) {
      n1->GetXaxis()->SetTitle("Reco E_{vis} (GeV)");
      n1->GetYaxis()->SetTitle("Off axis position (m)");
    } else if ( hist2dndnames[i].Contains("SmearingMatrix") ) {
      n1->GetXaxis()->SetTitle("True E_{vis} (GeV)");
      n1->GetYaxis()->SetTitle("Reco E_{vis} (GeV)");
    }
    n1->GetYaxis()->SetTitleSize(0.05);
    n1->GetYaxis()->SetTitleOffset(0.9);
    n1->Draw("COLZ");
    canvas->Write();

    p1->SetTitle( TString::Format("%s: %s, %s 1#sigma, (N_{varied} - N_{nom})/N_{nom}", chanlabel.Data(), hist2dndnames[i].Data(), systname.Data()) );
    p1->SetStats(0);
    // Set axis titles
    if ( hist2dndnames[i].Contains("Bkg") ) {
      p1->GetXaxis()->SetTitle("Reco E_{vis} (GeV)");
      p1->GetYaxis()->SetTitle("Off axis position (m)");
    } else if ( hist2dndnames[i].Contains("SmearingMatrix") ) {
      p1->GetXaxis()->SetTitle("True E_{vis} (GeV)");
      p1->GetYaxis()->SetTitle("Reco E_{vis} (GeV)");
    }
    p1->GetYaxis()->SetTitleSize(0.05);
    p1->GetYaxis()->SetTitleOffset(0.9);
    p1->Draw("COLZ");
    canvas->Write();

    //
    // Calculate (varied - nom ND bkg)/ND data
    //

    // Only for 2d ND bkg
    if ( hist2dndnames[i].Contains("Bkg") ) {

      if ( debug ) std::cout << "found <Bkg> in name " << std::endl;
      TH2F *n1copy  = (TH2F*)file0->Get( TString::Format("numu_EvMatch_%s_-1/%s/%s", systname.Data(), channame.Data(), hist2dndnames[i].Data()) )->Clone("n1copy");
      TH2F *p1copy  = (TH2F*)file0->Get( TString::Format("numu_EvMatch_%s_1/%s/%s",  systname.Data(), channame.Data(), hist2dndnames[i].Data()) )->Clone("p1copy");

      if ( hist2dndnames[i].Contains("293kA") ) {
        if ( debug ) std::cout << "found <293kA> in name " << std::endl;
        n1copy->Add(nom, -1); n1copy->Divide(NDData_293kA_nom);
        p1copy->Add(nom, -1); p1copy->Divide(NDData_293kA_nom);
      }
      else if ( hist2dndnames[i].Contains("280kA") ) {
        if ( debug ) std::cout << "found <280kA> in name " << std::endl;
        n1copy->Add(nom, -1); n1copy->Divide(NDData_280kA_nom);
        p1copy->Add(nom, -1); p1copy->Divide(NDData_280kA_nom);
      } //end check currents

      // plot
      n1copy->SetTitle( TString::Format("%s: %s, %s -1#sigma, (N_{varied} - N_{nom})/N_{ND data}", chanlabel.Data(), hist2dndnames[i].Data(), systname.Data()) );
      n1copy->SetStats(0);
      n1copy->GetXaxis()->SetTitle("Reco E_{vis} (GeV)");
      n1copy->GetYaxis()->SetTitle("Off axis position (m)");
      n1copy->GetYaxis()->SetTitleSize(0.05);
      n1copy->GetYaxis()->SetTitleOffset(0.9);
      n1copy->Draw("COLZ");
      canvas->Write();

      p1copy->SetTitle( TString::Format("%s: %s, %s 1#sigma, (N_{varied} - N_{nom})/N_{ND data}", chanlabel.Data(), hist2dndnames[i].Data(), systname.Data()) );
      p1copy->SetStats(0);
      p1copy->GetXaxis()->SetTitle("Reco E_{vis} (GeV)");
      p1copy->GetYaxis()->SetTitle("Off axis position (m)");
      p1copy->GetYaxis()->SetTitleSize(0.05);
      p1copy->GetYaxis()->SetTitleOffset(0.9);
      p1copy->Draw("COLZ");
      canvas->Write();

      // Delete created objects in current scope
      delete n1copy;
      delete p1copy;

    } // end bkg

    // Delete created objects in current scope
    delete nom;
    delete n1;
    delete p1;
    delete canvas;

  } // end 2D hists

  //
  // ND FD Efficiency
  //

  // Make sure numerator and denominator vec has same size
  for ( size_t i = 0; i < histeffsel.size(); i++ ) {

    if ( debug ) std::cout << "looking at histogram: " << histeffsel[i] << " and " << histeffsel[i] << std::endl;

    //
    // Clone histogram
    //

    TH2F *nom = (TH2F*)file0->Get( TString::Format("numu_EvMatch_nom/%s/%s",   channame.Data(), histeffsel[i].Data()) )->Clone("nom");
    TH2F *n1  = (TH2F*)file0->Get( TString::Format("numu_EvMatch_%s_-1/%s/%s", systname.Data(), channame.Data(), histeffsel[i].Data()) )->Clone("n1");
    TH2F *p1  = (TH2F*)file0->Get( TString::Format("numu_EvMatch_%s_1/%s/%s",  systname.Data(), channame.Data(), histeffsel[i].Data()) )->Clone("p1");

    //
    // Calculate (varied - nom)/nom
    //

    n1->Add(nom, -1); n1->Divide(nom);
    p1->Add(nom, -1); p1->Divide(nom);

    // Plot
    TCanvas *canvas = new TCanvas("canvas", "canvas", 700, 500);
    canvas->cd(); canvas->SetGridx(); canvas->SetGridy();
    // Save nominal eff
    nom->SetTitle( TString::Format("%s: %s Efficiency nominal", chanlabel.Data(), histeffsel[i].Data()) );
    nom->SetStats(0);
    nom->GetXaxis()->SetTitle("True E_{vis} (GeV)");
    if ( histeffsel[i].Contains("FD") ) {
      nom->GetYaxis()->SetTitle("Efficiency");
    }
    else { // ND 2d hist: off-axis vs trueE
      nom->GetYaxis()->SetTitle("Off axis position (m)");
    }
    nom->GetYaxis()->SetTitleSize(0.05);
    nom->GetYaxis()->SetTitleOffset(0.9);
    nom->Draw("COLZ");
    canvas->Write();

    // Draw together if FD: 1d hist
    if ( histeffsel[i].Contains("FD") ) {
      n1->SetTitle( TString::Format("%s: %s", chanlabel.Data(), histeffsel[i].Data()) );
      n1->SetMinimum(-1.*MaxAbsY);
      n1->SetMaximum(MaxAbsY);
      n1->SetStats(0);
      n1->GetXaxis()->SetTitle("True E_{vis} (GeV)");
      n1->GetYaxis()->SetTitle("(Eff_{varied} - Eff_{nom})/Eff_{nom}");
      n1->GetYaxis()->SetTitleSize(0.05);
      n1->GetYaxis()->SetTitleOffset(0.9);

      n1->SetLineColor(kRed+1);  n1->SetLineStyle(1); n1->SetLineWidth(2); n1->Draw("HIST");
      p1->SetLineColor(kBlue+1); p1->SetLineStyle(1); p1->SetLineWidth(2); p1->Draw("HISTSAME");

      TLegend* legend = new TLegend(0.1, 0.7, 0.4, 0.9);
      legend->SetBorderSize(0); legend->SetFillStyle(0); legend->SetNColumns(1);
      legend->AddEntry(n1, TString::Format("%s: -1#sigma", systname.Data()), "l");
      legend->AddEntry(p1, TString::Format("%s: 1#sigma",  systname.Data()), "l");
      legend->Draw();
      canvas->Write();

      delete legend;
    } else {
      n1->SetTitle( TString::Format("%s: %s, %s -1#sigma, (Eff_{varied} - Eff_{nom})/Eff_{nom}", chanlabel.Data(), histeffsel[i].Data(), systname.Data()) );
      n1->SetStats(0);
      n1->GetXaxis()->SetTitle("True E_{vis} (GeV)");
      n1->GetYaxis()->SetTitle("Off axis position (m)");
      n1->GetYaxis()->SetTitleSize(0.05);
      n1->GetYaxis()->SetTitleOffset(0.9);
      n1->Draw("COLZ");
      canvas->Write();

      p1->SetTitle( TString::Format("%s: %s, %s 1#sigma, (Eff_{varied} - Eff_{nom})/Eff_{nom}", chanlabel.Data(), histeffsel[i].Data(), systname.Data()) );
      p1->SetStats(0);
      p1->GetXaxis()->SetTitle("True E_{vis} (GeV)");
      p1->GetYaxis()->SetTitle("Off axis position (m)");
      p1->GetYaxis()->SetTitleSize(0.05);
      p1->GetYaxis()->SetTitleOffset(0.9);
      p1->Draw("COLZ");
      canvas->Write();
    }

    // delte objects
    delete nom;
    delete n1;
    delete p1;
    delete canvas;
  } // end eff calc


  file0->Close();
  myPlot.Close();
}
