#include "TLatex.h"

TH1* cloneAndAddBinsToCoeffs(TH1* hist, int nExtraBins, double extraBinContent1, double extraBinContent2) {
    // Get the original histogram properties
    int nBinsOriginal = hist->GetNbinsX();
    double xMin = hist->GetXaxis()->GetXmin();
    double xMax = hist->GetXaxis()->GetXmax();
    // Calculate new x-axis range to include additional bins
    double binWidth = (xMax - xMin) / nBinsOriginal;
    double newXMax = xMax + nExtraBins * binWidth;
    // Create a new histogram with additional bins
    TH1* newHist = new TH1F("newHist", hist->GetTitle(), nBinsOriginal + nExtraBins, xMin, newXMax);
    // Copy contents from original histogram to the new histogram
    for (int i = 1; i <= nBinsOriginal; ++i) {
        newHist->SetBinContent(i, hist->GetBinContent(i));
    }
    // Set contents of the additional bins
    newHist->SetBinContent(nBinsOriginal + 1, extraBinContent1);
    newHist->SetBinContent(nBinsOriginal + 2, extraBinContent2);
    // Print to confirm
    for (int i = 1; i <= newHist->GetNbinsX(); ++i) {
        std::cout << "Bin " << i << ": Content = " << newHist->GetBinContent(i) << std::endl;
    }

    return newHist;
}


void PlotCoefficients(){


  //gStyle->SetOptStat(0);

  //file with ALL possible data drivend background: RS intrinsic = true and WSbkg (WSB+ WS intrinsic) = true

  //TFile* FilePRISMPredNDFDExtrapNewPairedData = new TFile("/localscratch/icaracas/GeomEfficiency/PRISMPred_OnlyNuMuToNuMu_NoSysts_EnuRecoFDExtrapPred_visEtrue_NewMuonPairedData_NoSysts.root", "READ");
  TFile* FilePRISMPredNDFDExtrapNewPairedData = new TFile("/localscratch/icaracas/GeomEfficiency/PRISMPred_OnlyNuMuToNuMu_NoSysts_EnuRecoFDExtrapPred_visEtrue_OldPairedData_NoSysts_NoOscPred.root", "READ");


  FilePRISMPredNDFDExtrapNewPairedData->cd();
  TDirectory* MainEventsDir_PRISMPredNDFDExtrap = (TDirectoryFile*) FilePRISMPredNDFDExtrapNewPairedData->Get("numu_EvMatch");
  MainEventsDir_PRISMPredNDFDExtrap->cd();



  //NuMu->NuMuNuMu diectory
  TDirectory* NuMuNuMuDir_PRISMPredNDFDExtrap = (TDirectoryFile*) MainEventsDir_PRISMPredNDFDExtrap->Get("FD_nu_numu");
  NuMuNuMuDir_PRISMPredNDFDExtrap->cd();

  TH1D* CoefficientsNDPRISM = (TH1D*) NuMuNuMuDir_PRISMPredNDFDExtrap->Get("NDFDWeightings_293kA");
  CoefficientsNDPRISM->SetDirectory(0);


  //don't need the adding bins up to 3m OA : using same NDFV as NDCAFs: -2, 2 m
  // TH1* CoeffPRISMUpTo3mOA = cloneAndAddBinsToCoeffs(CoefficientsNDPRISM, 2, CoefficientsNDPRISM->GetBinContent(65), CoefficientsNDPRISM->GetBinContent(65));
  TH1D* CoeffPRISMUpTo3mOA =  (TH1D*) CoefficientsNDPRISM->Clone();

  cout<<" mean average of coefficients before scaling = "<<CoeffPRISMUpTo3mOA->GetMean(2)<<" integral / nbins: "<< CoeffPRISMUpTo3mOA->Integral()/CoeffPRISMUpTo3mOA->GetNbinsX()<<endl;
  //scale so that average coefficients (on y-axis) = 1
  // int nBins = CoeffPRISMUpTo3mOA->GetNbinsX();
  // int entries = CoefficientsNDPRISM->GetEntries();
  // double avgHeight = CoeffPRISMUpTo3mOA->Integral()/nBins;

  CoeffPRISMUpTo3mOA->Scale(1.0/CoeffPRISMUpTo3mOA->Integral());

  cout<<" integral: "<<CoeffPRISMUpTo3mOA->Integral()<<" mean average coeff after scaling: "<< CoeffPRISMUpTo3mOA->Integral()/CoeffPRISMUpTo3mOA->GetNbinsX()<<endl;
  cout<<" Nbins: "<<CoeffPRISMUpTo3mOA->GetNbinsX()<<" from "<<CoeffPRISMUpTo3mOA->GetBinLowEdge(1)<<" to "<<CoeffPRISMUpTo3mOA->GetBinLowEdge(68)<<endl;


  CoeffPRISMUpTo3mOA->Draw("hist");

  TFile* FileWithCoeffsNuMu = new TFile("FileWithCoeffsNuMuNoOsc.root", "RECREATE");
  CoeffPRISMUpTo3mOA->Write("CoeffPRISMUpTo3mOA");
  FileWithCoeffsNuMu->Write();
  FileWithCoeffsNuMu->Close();
}
