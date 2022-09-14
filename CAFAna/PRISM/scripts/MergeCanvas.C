//------------------------------------------------------------------
// Overlay several histograms from a root file first use for sigma variation
// Run this macro:
// source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.24.02/x86_64-centos7-gcc48-opt/bin/thisroot.sh
// root -l -b -q MergeCanvas.C
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

// Define a struct to associate each syst with its impacts
struct syst{
    TString name;
    int signed_sigma;
    double im_tot_prism_pred;
    double im_flux_corr;
    double im_fd_nutauCC_bkg;
    double im_fd_intrinsic_bkg;
    double im_fd_ws_bkg;
    double im_fd_nc_bkg;
    double im_fd_wl_bkg;
    double im_nd_data_lc;
    double im_nd_ws_bkg_subtract;
    double im_nd_nc_bkg_subtract;
    double im_nd_wl_bkg_subtract;
    double im_nd_lc_weight_calc;
    double im_nd_unfold;
    double im_nd_eff_corr;
    double im_fd_eff_corr;
    double im_fd_smear;
    double im_xsec_corr; // only apply to nue/nueb appearance
};

// Define helpful functions
bool Descend(syst s1, syst s2) {
  //
  // Compare abs value of first property in struct syst
  //
  return ( fabs(s1.im_tot_prism_pred) > fabs(s2.im_tot_prism_pred) );
}

int CompareTH1F(TH1F** h0, TH1F** h1, int bin) {
  //
  // Compare first hist from a hist array, return the hist array w/ larger bin content: 0->h0, 1->h1
  //
  if ( fabs( h0[0]->GetBinContent(bin) ) > fabs( h1[0]->GetBinContent(bin) ) ) {
    return 0;
  } else {
    return 1;
  }
}

void MergeCanvas() // "Main func"
{
  bool debug = true; // Print out for debug purpose
  bool isApp = true; // true: FD_nu_nue, FD_nub_nue; false: FD_nu_numu, FD_nub_numu
  int InitEvisbins = 17; // for initialize the 2d struct syst
  TString Dir = "/dune/app/users/weishi/Overlay"; // files dir on FNAL dunegpvm machine

  std::vector<TString> suffix;
  suffix.clear();
  suffix.push_back("OnlyVaryNDWSBkg");
  suffix.push_back("OnlyVaryNDNCBkg");
  suffix.push_back("OnlyVaryNDWLBkg");
  suffix.push_back("OnlyVaryLCWeights");
  suffix.push_back("OnlyVaryNDUnfold");
  suffix.push_back("OnlyVaryNDEffCorr");
  suffix.push_back("OnlyVaryFDEffCorr");
  suffix.push_back("OnlyVaryFDSmear");

  std::vector<TString> texts;
  texts.clear();
  texts.push_back("Only shift ND WS Bkg");
  texts.push_back("Only shift ND NC Bkg");
  texts.push_back("Only shift ND WL Bkg");
  texts.push_back("Only shift LC weights");
  texts.push_back("Only shift ND unfold");
  texts.push_back("Only shift ND eff. corr.");
  texts.push_back("Only shift FD eff. corr.");
  texts.push_back("Only shift FD smear");

  if ( isApp ) { // appearance channel has one extra step: numu to nue xsec correction
    suffix.push_back("OnlyVaryXsecCorr");
    texts.push_back("Only shift Xsec corr.");
  }

  int ndprismpredsteps = suffix.size(); // for numu disp is 8. For nue app is 9: there is one more for nue app due to the numu to nue xsec correction
  int nplot = 8 + ndprismpredsteps; // for numu disp is 8+8. For nue app is 8+9: there is one more for nue app due to the numu to nue xsec correction

  // systematic name list: 117 syst = 56 xsec + 28 flux + 33 det
  //std::vector<TString> systnames {"MaCCQE", "VecFFCCQEshape", "CCQEPauliSupViaKF", "MaCCRES", "MvCCRES", "Theta_Delta2Npi", "AhtBY", "BhtBY", "CV1uBY", "CV2uBY", "MaNCRES", "MvNCRES", "FrCEx_N", "FrElas_N", "FrInel_N", "FrAbs_N", "FrPiProd_N", "FrCEx_pi", "FrElas_pi", "FrInel_pi", "FrAbs_pi", "FrPiProd_pi", "BeRPA_A", "BeRPA_B", "BeRPA_D", "C12ToAr40_2p2hScaling_nu", "C12ToAr40_2p2hScaling_nubar", "E2p2h_A_nu", "E2p2h_B_nu", "E2p2h_A_nubar", "E2p2h_B_nubar", "NR_nu_np_CC_1Pi", "NR_nu_n_CC_2Pi", "NR_nu_n_CC_3Pi", "NR_nu_p_CC_2Pi", "NR_nu_p_CC_3Pi", "NR_nu_n_NC_1Pi", "NR_nu_n_NC_2Pi", "NR_nu_n_NC_3Pi", "NR_nu_p_NC_1Pi", "NR_nu_p_NC_2Pi", "NR_nu_p_NC_3Pi", "NR_nubar_n_CC_1Pi", "NR_nubar_n_CC_2Pi", "NR_nubar_n_CC_3Pi", "NR_nubar_p_CC_1Pi", "NR_nubar_p_CC_2Pi", "NR_nubar_p_CC_3Pi", "NR_nubar_n_NC_1Pi", "NR_nubar_n_NC_2Pi", "NR_nubar_n_NC_3Pi", "NR_nubar_p_NC_1Pi", "NR_nubar_p_NC_2Pi", "NR_nubar_p_NC_3Pi", "nuenumu_xsec_ratio", "nuenuebar_xsec_ratio", "flux_Nov17_0", "flux_Nov17_1", "flux_Nov17_2", "flux_Nov17_3", "flux_Nov17_4", "flux_Nov17_5", "flux_Nov17_6", "flux_Nov17_7", "flux_Nov17_8", "flux_Nov17_9", "flux_Nov17_10", "flux_Nov17_11", "flux_Nov17_12", "flux_Nov17_13", "flux_Nov17_14", "flux_Nov17_15", "flux_Nov17_16", "flux_Nov17_17", "flux_Nov17_18", "flux_Nov17_19", "flux_Nov17_20", "flux_Nov17_21", "flux_Nov17_22", "flux_Nov17_23", "flux_Nov17_24", "flux_Nov17_25", "flux_Nov17_26", "flux_Nov17_27", "RecoEnergyScaleND", "RecoEnergySqrtND", "RecoEnergyInvSqrtND", "EMRecoUncorrND", "EMRecoUncorrSqrtND", "EMRecoUncorrInvSqrtND", "ChargedHadRecoUncorrND", "ChargedHadRecoUncorrSqrtND", "ChargedHadRecoUncorrInvSqrtND", "ERecoScaleMuLArND", "ERecoScaleMuLArSqrtND", "ERecoScaleMuLArInvSqrtND", "RecoEnergyScaleFD", "RecoEnergySqrtFD", "RecoEnergyInvSqrtFD", "EMRecoUncorrFD", "EMRecoUncorrSqrtFD", "EMRecoUncorrInvSqrtFD", "ChargedHadRecoUncorrFD", "ChargedHadRecoUncorrSqrtFD", "ChargedHadRecoUncorrInvSqrtFD", "ERecoScaleMuLArFD", "ERecoScaleMuLArSqrtFD", "ERecoScaleMuLArInvSqrtFD", "MuonRecoResND", "EMRecoResND", "ChargedHadRecoResND", "MuonRecoResFD", "EMRecoResFD", "ChargedHadRecoResFD", "NuOnECCBkgSyst", "NuOnENCBkgSyst", "NuOnERecoEffSyst"};
  // systematic name list: 56 xsec
  std::vector<TString> systnames {"MaCCQE", "VecFFCCQEshape", "CCQEPauliSupViaKF", "MaCCRES", "MvCCRES", "Theta_Delta2Npi", "AhtBY", "BhtBY", "CV1uBY", "CV2uBY", "MaNCRES", "MvNCRES", "FrCEx_N", "FrElas_N", "FrInel_N", "FrAbs_N", "FrPiProd_N", "FrCEx_pi", "FrElas_pi", "FrInel_pi", "FrAbs_pi", "FrPiProd_pi", "BeRPA_A", "BeRPA_B", "BeRPA_D", "C12ToAr40_2p2hScaling_nu", "C12ToAr40_2p2hScaling_nubar", "E2p2h_A_nu", "E2p2h_B_nu", "E2p2h_A_nubar", "E2p2h_B_nubar", "NR_nu_np_CC_1Pi", "NR_nu_n_CC_2Pi", "NR_nu_n_CC_3Pi", "NR_nu_p_CC_2Pi", "NR_nu_p_CC_3Pi", "NR_nu_n_NC_1Pi", "NR_nu_n_NC_2Pi", "NR_nu_n_NC_3Pi", "NR_nu_p_NC_1Pi", "NR_nu_p_NC_2Pi", "NR_nu_p_NC_3Pi", "NR_nubar_n_CC_1Pi", "NR_nubar_n_CC_2Pi", "NR_nubar_n_CC_3Pi", "NR_nubar_p_CC_1Pi", "NR_nubar_p_CC_2Pi", "NR_nubar_p_CC_3Pi", "NR_nubar_n_NC_1Pi", "NR_nubar_n_NC_2Pi", "NR_nubar_n_NC_3Pi", "NR_nubar_p_NC_1Pi", "NR_nubar_p_NC_2Pi", "NR_nubar_p_NC_3Pi", "nuenumu_xsec_ratio", "nuenuebar_xsec_ratio"};
  // systematic name list: 28 flux
  //std::vector<TString> systnames {"flux_Nov17_0", "flux_Nov17_1", "flux_Nov17_2", "flux_Nov17_3", "flux_Nov17_4", "flux_Nov17_5", "flux_Nov17_6", "flux_Nov17_7", "flux_Nov17_8", "flux_Nov17_9", "flux_Nov17_10", "flux_Nov17_11", "flux_Nov17_12", "flux_Nov17_13", "flux_Nov17_14", "flux_Nov17_15", "flux_Nov17_16", "flux_Nov17_17", "flux_Nov17_18", "flux_Nov17_19", "flux_Nov17_20", "flux_Nov17_21", "flux_Nov17_22", "flux_Nov17_23", "flux_Nov17_24", "flux_Nov17_25", "flux_Nov17_26", "flux_Nov17_27"};
  // systematic name list: 33 det (actual number is 39, but 6 don't work for now, so it's 33)
  //std::vector<TString> systnames {"RecoEnergyScaleND", "RecoEnergySqrtND", "RecoEnergyInvSqrtND", "EMRecoUncorrND", "EMRecoUncorrSqrtND", "EMRecoUncorrInvSqrtND", "ChargedHadRecoUncorrND", "ChargedHadRecoUncorrSqrtND", "ChargedHadRecoUncorrInvSqrtND", "ERecoScaleMuLArND", "ERecoScaleMuLArSqrtND", "ERecoScaleMuLArInvSqrtND", "RecoEnergyScaleFD", "RecoEnergySqrtFD", "RecoEnergyInvSqrtFD", "EMRecoUncorrFD", "EMRecoUncorrSqrtFD", "EMRecoUncorrInvSqrtFD", "ChargedHadRecoUncorrFD", "ChargedHadRecoUncorrSqrtFD", "ChargedHadRecoUncorrInvSqrtFD", "ERecoScaleMuLArFD", "ERecoScaleMuLArSqrtFD", "ERecoScaleMuLArInvSqrtFD", "MuonRecoResND", "EMRecoResND", "ChargedHadRecoResND", "MuonRecoResFD", "EMRecoResFD", "ChargedHadRecoResFD", "NuOnECCBkgSyst", "NuOnENCBkgSyst", "NuOnERecoEffSyst"};

  if ( systnames.size() == 0 ) {
    std::cout << "!!! Please check: no input systs !!! " << std::endl;
    exit(1);
  }

  if ( debug ) std::cout << "tot systs: " << systnames.size() << std::endl;

  // Initialize
  int nSysts = systnames.size();
  int nEvisbins = InitEvisbins;
  struct syst allSyst[nEvisbins][nSysts];
  double lowbinedge[nEvisbins];
  double upbinedge[nEvisbins];
  TString systname;

  //
  // Loop over systs
  //

  for ( int isyst = 0; isyst < systnames.size(); isyst++ ) {

    systname = systnames.at(isyst);

    if ( debug ) std::cout << "looking at syst #" << isyst + 1 << ": " << systname << std::endl;

    // For file0 draw the prism components
    TFile *file0 = TFile::Open( TString::Format("%s/Overlay_%s.root", Dir.Data(), systname.Data() ) );
    // For shifted nd related steps, draw first canvas in each file
    TFile** file = new TFile*[ndprismpredsteps];
    TCanvas** canvas = new TCanvas*[nplot]; // canvas[0], canvas[1], ... canvas[15], canvas[16]
    // Get histograms from canvas
    TH1F** n1_c = new TH1F*[nplot]; // -1 sigma variation
    TH1F** p1_c = new TH1F*[nplot]; // +1 sigma variation
    // Text box for plots relating to nd prism steps
    TPad** pad = new TPad*[ndprismpredsteps];
    TPaveLabel** label = new TPaveLabel*[ndprismpredsteps];

    //
    // Merge all canvas
    //

    // Big MergeCanvas for good resolution
    TCanvas* MergeCanvas = new TCanvas("MergeCanvas", "MergeCanvas", 7200, 6000);
    if ( ndprismpredsteps == 9 ) MergeCanvas->Divide(4, 5, 0.01, 0.01);
    else MergeCanvas->Divide(4, 4, 0.01, 0.01);
    // Loop over canvas
    for ( int icanv = 0; icanv < nplot; icanv++ ) {

      if ( icanv <= 7 ) {
        //
        // For all prism components: upto canvas[7]
        //
        // NDDataCorr_FDExtrap
        // FDFluxCorr
        // FDNuTauCCBkg
        // FDIntrinsicBkg
        // FDWSBkg
        // FDNCBkg
        // FDWrongLepBkg
        // NDData_FDExtrap
        //
        canvas[icanv] = (TCanvas *)file0->Get(TString::Format("canvas;%d", icanv+1 ) ); // canvas;1/2/.../8
        MergeCanvas->cd(icanv + 1); canvas[icanv]->DrawClonePad();
      } else {
        //
        // for all ND steps affecting NDData_FDExtrap: starting canvas[8]
        //
        file[icanv-8] = TFile::Open( TString::Format("%s/Overlay_%s_%s.root", Dir.Data(), systname.Data(), suffix.at(icanv-8).Data() ) ); // file[0], file[1], ...file[7], file[8]
        canvas[icanv] = (TCanvas *)file[icanv-8]->Get("canvas;8");
        MergeCanvas->cd(icanv + 1); canvas[icanv]->DrawClonePad();
        // Add label/text using a transparent pad
        pad[icanv-8] = new TPad(TString::Format("pad%d", icanv-8), TString::Format("pad%d", icanv-8), 0, 0, 1, 1); // pad0, pad1...., pad7, pad8
        pad[icanv-8]->SetFillStyle(4000); pad[icanv-8]->Draw(); pad[icanv-8]->cd();
        label[icanv-8] = new TPaveLabel(0.5, 0.8, 0.9, 0.9, texts.at(icanv-8).Data());
        label[icanv-8]->Draw();
      }

      n1_c[icanv] = (TH1F*)canvas[icanv]->GetPrimitive("n1");
      p1_c[icanv] = (TH1F*)canvas[icanv]->GetPrimitive("p1");

      nEvisbins = n1_c[icanv]->GetNbinsX(); // !!! Assume same bins in p1_c !!!

      if ( nEvisbins > InitEvisbins ) {
        std::cout << "!!! Please change InitEvisbins to "<< nEvisbins <<" or larger !!! " << std::endl;
        exit(2);
      }

    } // end loop over canvas

    MergeCanvas->SaveAs( TString::Format("%s/Plots/%s_MergeCanvas.png", Dir.Data(), systname.Data() ) );

    // Loop over Evis bin to fill the struct
    for ( int iEvisbin = 0; iEvisbin < nEvisbins; iEvisbin++ ) {

      allSyst[iEvisbin][isyst].name = systname; // Only need fill once

      TH1F** abs1_c = new TH1F*[nplot];
      int signedsigma = 0; // initialize
      // Compare, start from bin number 1, 0 is underflow
      if ( CompareTH1F(n1_c, p1_c, iEvisbin+1) == 0 ) { // use n1_c
        abs1_c = n1_c;
        signedsigma = -1; // the systematic is shifted -1 sigma
      } else { // use p1_c
        abs1_c = p1_c;
        signedsigma = 1; // the systematic is shifted +1 sigma
      }

      // Can't access struct property until higher c++ versions? ugly!
      allSyst[iEvisbin][isyst].signed_sigma          = signedsigma;
      allSyst[iEvisbin][isyst].im_tot_prism_pred     = abs1_c[0]->GetBinContent(iEvisbin+1); // do not use abs impact
      allSyst[iEvisbin][isyst].im_flux_corr          = abs1_c[1]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_fd_nutauCC_bkg     = abs1_c[2]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_fd_intrinsic_bkg   = abs1_c[3]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_fd_ws_bkg          = abs1_c[4]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_fd_nc_bkg          = abs1_c[5]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_fd_wl_bkg          = abs1_c[6]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_nd_data_lc         = abs1_c[7]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_nd_ws_bkg_subtract = abs1_c[8]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_nd_nc_bkg_subtract = abs1_c[9]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_nd_wl_bkg_subtract = abs1_c[10]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_nd_lc_weight_calc  = abs1_c[11]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_nd_unfold          = abs1_c[12]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_nd_eff_corr        = abs1_c[13]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_fd_eff_corr        = abs1_c[14]->GetBinContent(iEvisbin+1);
      allSyst[iEvisbin][isyst].im_fd_smear           = abs1_c[15]->GetBinContent(iEvisbin+1);
      if ( ndprismpredsteps == 9 ) {
        allSyst[iEvisbin][isyst].im_xsec_corr = abs1_c[16]->GetBinContent(iEvisbin+1); // only apply to nue/nueb appearance
      } else {
        allSyst[iEvisbin][isyst].im_xsec_corr = 0; // for numu disp
      }

      // Get bin edges at first syst
      if ( isyst == 0 ) {
        lowbinedge[iEvisbin] = abs1_c[0]->GetXaxis()->GetBinLowEdge(iEvisbin+1);
        upbinedge[iEvisbin]  = abs1_c[0]->GetXaxis()->GetBinLowEdge(iEvisbin+1) + abs1_c[0]->GetXaxis()->GetBinWidth(iEvisbin+1);
      }

    } // end loop over Evis bins

    delete MergeCanvas;
    for ( int ifile = 0; ifile < ndprismpredsteps; ifile++ ) file[ifile]->Close(); // file[0], file[1], ... file[7], file[8]

  } // end loop over syst

  if ( debug ) std::cout << "tot Evis bins: " << nEvisbins << std::endl;

  // Order based on tot prism pred
  for ( int iEvisbin = 0; iEvisbin < nEvisbins; iEvisbin++ ) {
    std::sort(allSyst[iEvisbin], allSyst[iEvisbin] + nSysts, Descend);
    if ( debug ) std::cout << "Ordering for Evis bin " << iEvisbin << std::endl;
  } // end sort

  // Print all ordered
  cout<< " " << endl;
  cout<< "Printing ordered tables..." << endl;
  cout<< " " << endl;
  for ( int iEvisbin = 0; iEvisbin < nEvisbins; iEvisbin++ ) {
    cout<< "= Evis bin " << iEvisbin+1 << ": " << lowbinedge[iEvisbin] <<" - " << upbinedge[iEvisbin] <<"GeV =" << endl;
    cout<<left<<setw(34)<<"Syst (sigma) \\ impact (\%)         "                                                         <<" | "<<left<<setw(12)<<"TotPRISMpred"                                                 <<" | "<<left<<setw(9)<<"FluxCorr"                                                <<" | "<<left<<setw(9)<<"FDnuTauCC"                                                    <<" | "<<left<<setw(11)<<"FDintrinsic"                                                    <<" | "<<left<<setw(9)<<"FD WS bkg"                                                                     <<" | "<<left<<setw(9)<<"FD NC bkg"                                               <<" | "<<left<<setw(9)<<"FD WL bkg"                                               <<" | "<<left<<setw(9)<<"NDdataLC"                                                 <<" | "<<left<<setw(9)<<"Sub ND WS"                                                        <<" | "<<left<<setw(9)<<"Sub ND NC"                                                                                  <<" | "<<left<<setw(9)<<"Sub ND WL"                                                        <<" | "<<left<<setw(9)<<"LCwgtCalc"                                                       <<" | "<<left<<setw(9)<<"ND Unfold"                                               <<" | "<<left<<setw(9)<<"NDEffCorr"                                                 <<" | "<<left<<setw(9)<<"FDEffCorr"                                                 <<" | "<<left<<setw(9)<<"FD Smear"                                               <<" | "<<left<<setw(9)<<"XsecCorr"                                               <<endl;
    for ( int isyst = 0; isyst < systnames.size(); isyst++ ) { // if set min precision = 3, then need setw(9) at least
      cout<<left<<setw(29)<<allSyst[iEvisbin][isyst].name<<" ("<<right<<setw(2)<<allSyst[iEvisbin][isyst].signed_sigma<<")"<<" | "<<left<<setw(12)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_tot_prism_pred*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_flux_corr*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_fd_nutauCC_bkg*100<<" | "<<left<<setw(11)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_fd_intrinsic_bkg*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_fd_ws_bkg*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_fd_nc_bkg*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_fd_wl_bkg*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_nd_data_lc*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_nd_ws_bkg_subtract*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_nd_nc_bkg_subtract*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_nd_wl_bkg_subtract*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_nd_lc_weight_calc*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_nd_unfold*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_nd_eff_corr*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_fd_eff_corr*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_fd_smear*100<<" | "<<left<<setw(9)<<setprecision(3)<<allSyst[iEvisbin][isyst].im_xsec_corr*100<<endl;
    } // end syst
    cout<< " " << endl;
  } // end Evis bin

} // end MergeCanvas
