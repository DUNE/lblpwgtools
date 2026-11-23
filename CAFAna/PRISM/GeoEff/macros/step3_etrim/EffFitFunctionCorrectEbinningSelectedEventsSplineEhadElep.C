#include <TFile.h>
#include <TVectorD.h>
#include <vector>

void EffFitFunctionCorrectEbinningSelectedEventsSplineEhadElep() {

  //  TFile* FileWithEfficiency = new TFile("OutFileWithNDEfficiencyAndSelAndUnselEv_ALLEv_ElepTrueRecoEnergyTransfer_HadCutAndMuCut_AllHadronbins_14Elepbin_perFileWeight.root", "READ");
   //TFile* FileWithEfficiency = new TFile("OutFileWithNDEfficiencyAndSelAndUnselEv_10Ev_ElepTrueRecoEnergyTransfer_HadCutAndMuCut_AllHadronbins_14Elepbin_perFileWeightNO280kA_no0nAxisOutliers.root", "READ");

  TFile* FileWithEfficiency = new TFile("OutFileWithNDEfficiencyAndSelAndUnselEv_ALLEv_ElepTrueRecoEnergyTransfer_HadCutAndMuCut_WithPRISMWeights_AllHadronbins_13Elepbin.root", "READ");
  //TFile* FileWithEfficiency = new TFile("OutFileWithNDEfficiencyAndSelAndUnselEv_ALLEv_ElepTrueRecoEnergyTransfer_HadCutONLY_noRecoQnoRecoNuMu_WithPRISMWeights_AllHadronbins_67Elepbin.root", "READ");
   //TFile* FileWithEfficiency = new TFile("OutFileWithNDEfficiencyAndSelAndUnselEv_ALLEv_ElepTrueRecoEnergyTransfer_HadCutAndMuCut_AllHadronbins_14Elepbin.root", "READ");
  FileWithEfficiency->cd();
  TDirectory* numu_EvMatch = (TDirectoryFile*) FileWithEfficiency->Get("numu_EvMatch");
  numu_EvMatch->cd();
  TDirectory* FD_nu_numu = (TDirectoryFile*) numu_EvMatch->Get("FD_nu_numu");
  FD_nu_numu->cd();
  TDirectory* EffFolder = (TDirectoryFile*) FD_nu_numu->Get("MCEfficiency");
  EffFolder->cd();
  TH2D* Eff293kA = (TH2D*) EffFolder->Get("NDEff_293kA");
  Eff293kA->Draw("COLZ");
  TH2D* Eff293kANonans = (TH2D*) Eff293kA->Clone();
  TH2D* SelectedEv = (TH2D*) EffFolder->Get("SelectedEv_293kA");
  SelectedEv->SetDirectory(0);
  TH2D* UnselectedEv = (TH2D*)  EffFolder->Get("UnselectedEv_293kA");
  UnselectedEv->SetDirectory(0);




  // Loop over all binsOutFileWithNDEfficiencyAndSelAndUnselEv_1000Ev_ElepTrueTrueEnergyTransfer_HadCutONLY_noRecoQnoRecoNuMu_WithPRISMWeights_noSlicewidtth_Only0m_AllHadronbins_only1Elepbin.root
  for (int i = 1; i <= Eff293kA->GetNbinsX(); ++i) {
     for (int j = 1; j <= Eff293kA->GetNbinsY(); ++j) {
         double value = Eff293kA->GetBinContent(i, j);
         if (std::isnan(value)) {
             Eff293kANonans->SetBinContent(i, j, -1);
         }
     }
  }

  Eff293kANonans->SetMinimum(0);
  TCanvas* canvasef = new TCanvas("canvasef", "canvasef");
  Eff293kANonans->Draw("COLZ");

  // TCanvas* CanvasUnSelectedOrig = new TCanvas("CanvasUnSelectedOrig", "CanvasUnSelectedOrig");
  // UnselectedEv->Draw("COLZ");

  int nBinsX = Eff293kANonans->GetNbinsX(); // Total bins in X-axis
  int nBinsY = Eff293kANonans->GetNbinsY(); // Total bins in Y-axis

  int nBinsOA = 130;
  int nBinsDet = 130;
  // gStyle->SetOptStat(0);

  vector<double> EdgesBinsOA = {-30.25, -30.0, -29.75, -29.5, -29.25, -29.0, -28.75, -28.5, -28.25, -28.0, -27.75, -27.5, -27.25, -27.0, -26.75, -26.5, -26.25, -26.0, -25.75, -25.5,
      -25.25, -25.0, -24.75, -24.5, -24.25, -24.0, -23.75, -23.5, -23.25, -23.0, -22.75, -22.5, -22.25, -22.0, -21.75, -21.5, -21.25, -21.0, -20.75, -20.5, -20.25, -20.0,
      -19.75, -19.5, -19.25, -19.0, -18.75, -18.5, -18.25, -18.0, -17.75, -17.5, -17.25, -17.0, -16.75, -16.5, -16.25, -16.0, -15.75, -15.5, -15.25, -15.0, -14.75, -14.5,
      -14.25, -14.0, -13.75, -13.5, -13.25, -13.0, -12.75, -12.5, -12.25, -12.0, -11.75, -11.5, -11.25, -11.0, -10.75, -10.5, -10.25, -10.0, -9.75, -9.5, -9.25, -9.0, -8.75,
      -8.5, -8.25, -8.0, -7.75, -7.5, -7.25, -7.0, -6.75, -6.5, -6.25, -6.0, -5.75, -5.5, -5.25, -5.0, -4.75, -4.5, -4.25, -4.0, -3.75, -3.5, -3.25, -3.0, -2.75, -2.5, -2.25,
       -2.0, -1.75, -1.5, -1.25, -1.0, -0.75, -0.5, -0.25, 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25};
  vector<double> EdgesBinsDetPos = {-30.25, -30.0, -29.75, -29.5, -29.25, -29.0, -28.75, -28.5, -28.25, -28.0, -27.75, -27.5, -27.25, -27.0, -26.75, -26.5, -26.25, -26.0, -25.75, -25.5,
          -25.25, -25.0, -24.75, -24.5, -24.25, -24.0, -23.75, -23.5, -23.25, -23.0, -22.75, -22.5, -22.25, -22.0, -21.75, -21.5, -21.25, -21.0, -20.75, -20.5, -20.25, -20.0,
          -19.75, -19.5, -19.25, -19.0, -18.75, -18.5, -18.25, -18.0, -17.75, -17.5, -17.25, -17.0, -16.75, -16.5, -16.25, -16.0, -15.75, -15.5, -15.25, -15.0, -14.75, -14.5,
          -14.25, -14.0, -13.75, -13.5, -13.25, -13.0, -12.75, -12.5, -12.25, -12.0, -11.75, -11.5, -11.25, -11.0, -10.75, -10.5, -10.25, -10.0, -9.75, -9.5, -9.25, -9.0, -8.75,
          -8.5, -8.25, -8.0, -7.75, -7.5, -7.25, -7.0, -6.75, -6.5, -6.25, -6.0, -5.75, -5.5, -5.25, -5.0, -4.75, -4.5, -4.25, -4.0, -3.75, -3.5, -3.25, -3.0, -2.75, -2.5, -2.25,
           -2.0, -1.75, -1.5, -1.25, -1.0, -0.75, -0.5, -0.25, 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25};


  vector<double> kHadBinEdges = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 1., 2., 20.};
  vector<double> kLepBinEdges = {0., 0.5, 0.75, 1., 1.25, 1.5, 1.75, 2., 2.25, 2.5, 2.75, 3., 4., 20};
  // convert Elep and Ehad bins to TVectors to be able to write them in a tfile
  TVectorD hadEdges(kHadBinEdges.size());
  for (size_t i = 0; i < kHadBinEdges.size(); ++i)
      hadEdges[i] = kHadBinEdges[i];

  TVectorD lepEdges(kLepBinEdges.size());
  for (size_t i = 0; i < kLepBinEdges.size(); ++i)
      lepEdges[i] = kLepBinEdges[i];

  // array with number of CAF files/ det pos -> first element is very far off axis, last element is for on axis
  //fromOffaxisPOTCalc
  double NDCAFsPerDetPos[19] = {29647, 29785, 28436, 29607, 29916, 28397, 29755, 29892, 29793, 21448, 29888, 29900, 29110, 28858, 27960, 28384, 28914, 26045, 103281};



  // Define the range of X bins corresponding to Ehad_bin = 4

  const int nElepBins = 13;
  const int nEhadBins = 8;

  //    // Create new 2D histogram for projection
  std::vector<std::vector<TH2D*>> SelectedEvAllEnergiesHist(nElepBins, std::vector<TH2D*>(nEhadBins));
  std::vector<std::vector<TH2D*>> UnselectedEvAllEnergiesHist(nElepBins, std::vector<TH2D*>(nEhadBins));
  std::vector<std::vector<TH2D*>> SelOverGenTwoDHist(nElepBins, std::vector<TH2D*>(nEhadBins));

  for (int Elep_bin = 1; Elep_bin <= nElepBins; ++Elep_bin) {
      for (int Ehad_bin = 1; Ehad_bin <= nEhadBins; ++Ehad_bin) {

          // Create histogram for this bin combination
          TString nameSel = Form("SelectedEv_Elep%d_Ehad%d", Elep_bin, Ehad_bin);
          TString nameUnsel = Form("UnselectedEv_Elep%d_Ehad%d", Elep_bin, Ehad_bin);
          TString nameRatio = Form("SelOverGen_Elep%d_Ehad%d", Elep_bin, Ehad_bin);

          SelectedEvAllEnergiesHist[Elep_bin-1][Ehad_bin-1] = new TH2D(nameSel, nameSel, nBinsOA, EdgesBinsOA.data(), nBinsDet,EdgesBinsDetPos.data());
          UnselectedEvAllEnergiesHist[Elep_bin-1][Ehad_bin-1] = new TH2D(nameUnsel, nameUnsel, nBinsOA, EdgesBinsOA.data(), nBinsDet, EdgesBinsDetPos.data());

          int elep_k = Elep_bin - 1;
          int ehad_k = Ehad_bin - 1;
          int k = ehad_k + elep_k * nEhadBins;

          for (int i = 1; i <= nBinsDet; ++i) {
              for (int j = 1; j <= nBinsOA; ++j) {
                  int binY = (j - 1) * nBinsOA + i;

                  double valSel = SelectedEv->GetBinContent(k + 1, binY);
                  double valUnsel = UnselectedEv->GetBinContent(k + 1, binY);

                  SelectedEvAllEnergiesHist[elep_k][ehad_k]->SetBinContent(j, i, valSel);
                  UnselectedEvAllEnergiesHist[elep_k][ehad_k]->SetBinContent(j, i, valUnsel);
              }
          }

          SelOverGenTwoDHist[Elep_bin-1][Ehad_bin-1] = (TH2D*)SelectedEvAllEnergiesHist[Elep_bin-1][Ehad_bin-1]->Clone(nameRatio);
          SelOverGenTwoDHist[Elep_bin-1][Ehad_bin-1]->Divide(UnselectedEvAllEnergiesHist[Elep_bin-1][Ehad_bin-1]);
      }
  }

  TCanvas* CanvasSelOverGenTwoDhist = new TCanvas("CanvasSelOverGenTwoDhist", "CanvasSelOverGenTwoDhist");
  CanvasSelOverGenTwoDhist->cd();
  SelOverGenTwoDHist[2][1]->Draw("COLZ");
  // UnselectedEvAllEnergiesHist[2][1]->Draw("hist");
  cout<<" bin cont: "<<UnselectedEvAllEnergiesHist[2][1]->GetBinContent(1,1)<<endl;

  //plot Events vs OApos for each det pos (for now all E bins)
  //1. ===Selected Events
  std::vector<std::vector<std::vector<TH1D*>>> SelectedEventsVsOAPosForFixedDetPos;
  SelectedEventsVsOAPosForFixedDetPos.resize(nBinsDet + 1);  // DetPos is outermost

  for (int i = 0; i <= nBinsDet; ++i) {
      SelectedEventsVsOAPosForFixedDetPos[i].resize(nElepBins + 1);
      for (int j = 0; j <= nElepBins; ++j) {
          SelectedEventsVsOAPosForFixedDetPos[i][j].resize(nEhadBins + 1, nullptr);
      }
  }

  vector<double> detPosUsed;
  //for spline function flat after 28.25 (which is the detector center of the last det Pos )
  detPosUsed.push_back(-30.25);
  detPosUsed.push_back(-30);
  detPosUsed.push_back(-29.5);
  detPosUsed.push_back(-29);

  //===calculate selected events vs OA pos for each det position used in the analysis
  for (int elep = 1; elep <= nElepBins; ++elep) {
    for (int ehad = 1; ehad <= nEhadBins; ++ehad) {
        for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
            bool isEmpty = true;
            //cout<<" elep ="<<elep<<" ehad = "<<ehad<<endl;
            for (int oaBin = 1; oaBin <= nBinsOA; ++oaBin) {
                if (UnselectedEvAllEnergiesHist[elep-1][ehad-1]->GetBinContent(oaBin, detBin) > 0) {
                    isEmpty = false;
                    break;
                }
            }

            if (isEmpty) continue;
            // Check if this detPos is already in the detPosUsed vector
            if(!isEmpty){
                double detPos = EdgesBinsDetPos[detBin - 1];
                if (std::find(detPosUsed.begin(), detPosUsed.end(), detPos) == detPosUsed.end()) {
                    detPosUsed.push_back(detPos);
                  //  std::cout << "det pos used: " << detPos << std::endl;
                }
            }

            TString histName = Form("SelEventsVsOAPos_DetPos%d_Elep%d_Ehad%d", detBin, elep, ehad);
            TH1D* h1D = SelectedEvAllEnergiesHist[elep-1][ehad-1]->ProjectionX(histName, detBin, detBin);
            h1D->SetTitle(Form("DetPos bin %d; OApos; Events", detBin));

            SelectedEventsVsOAPosForFixedDetPos[detBin][elep][ehad] = h1D;
        }
    }
  }


  for (int elep = 1; elep <= nElepBins; ++elep) {
      for (int ehad = 1; ehad <= nEhadBins; ++ehad) {
        int sumdetBins = 0;
        //cout<<" elp = "<<elep<<" ehad = "<<ehad<<endl;
          for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
              // Check if histogram exists for this detBin, elep, ehad
              if (SelectedEventsVsOAPosForFixedDetPos[detBin][elep][ehad]) {
                  // Access the histogram
                  TH1D* h1D = SelectedEventsVsOAPosForFixedDetPos[detBin][elep][ehad];
                  sumdetBins +=1;
                  // Perform operations on the histogram
                  //std::cout << "Processing histogram for detBin = " << detBin << ", elep = " << elep << ", ehad = " << ehad << std::endl;
                  h1D->Scale(1.0 / NDCAFsPerDetPos[sumdetBins - 1]);  // Scaling if necessary

                  // Example: Draw the histogram
                  // h1D->Draw("HIST");
                  // std::cout << "Integral: " << h1D->Integral() << std::endl;
                  // cout<<" sum det bins: "<<sumdetBins<<endl;
              }

          }

      }
  }

  //==draw selected evebts vs OA pos fo each detPos
  TCanvas* CanvasAllSelectedEventsVsOAPos = new TCanvas("CanvasAllSelectedEventsVsOAPos", "CanvasAllSelectedEventsVsOAPos");
  CanvasAllSelectedEventsVsOAPos->cd();
  //===note use SelectedEventsVsOAPosForFixedDetPos[detBin][3][2] <-> SelectedEventsVsOAPosForFixedDetPos[detBin][elep][ehad] for reproducing Ehad_bin = 2; Elep_bin = 3;in the previous easier code
  // === Elep = 3, means the 3rd bin in Elep, Ehad =2 means 2nd bin in Elep etc
  // SelectedEventsVsOAPosForFixedDetPos[123][3][2]->Draw("hist");
  for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
    // Check if histogram exists for this detBin, elep, ehad

    if (SelectedEventsVsOAPosForFixedDetPos[detBin][3][2]) {
      cout<<" det bin: "<<detBin<<endl;
      SelectedEventsVsOAPosForFixedDetPos[detBin][3][2]->SetLineWidth(2);
      SelectedEventsVsOAPosForFixedDetPos[detBin][3][2]->SetLineColor(2);
      SelectedEventsVsOAPosForFixedDetPos[detBin][3][2]->GetYaxis()->SetRangeUser(1e-18, 1e-15);
      SelectedEventsVsOAPosForFixedDetPos[detBin][3][2]->Draw("histsames");
    }
  }



  //2.=== Unselected Events
  //1. ===Selected Events
  std::vector<std::vector<std::vector<TH1D*>>> UnSelectedEventsVsOAPosForFixedDetPos;
  UnSelectedEventsVsOAPosForFixedDetPos.resize(nBinsDet + 1);  // DetPos is outermost

  for (int i = 0; i <= nBinsDet; ++i) {
      UnSelectedEventsVsOAPosForFixedDetPos[i].resize(nElepBins + 1);
      for (int j = 0; j <= nElepBins; ++j) {
          UnSelectedEventsVsOAPosForFixedDetPos[i][j].resize(nEhadBins + 1, nullptr);
      }
  }

  //===calculate unselected events vs OA pos for each det position used in the analysis
  for (int elep = 1; elep <= nElepBins; ++elep) {
    for (int ehad = 1; ehad <= nEhadBins; ++ehad) {
        for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
            bool isEmpty = true;
            //cout<<" elep ="<<elep<<" ehad = "<<ehad<<endl;
            for (int oaBin = 1; oaBin <= nBinsOA; ++oaBin) {
                if (UnselectedEvAllEnergiesHist[elep-1][ehad-1]->GetBinContent(oaBin, detBin) > 0) {
                    isEmpty = false;
                    break;
                }
            }

            if (isEmpty) continue;

            TString histName = Form("UnSelEventsVsOAPos_DetPos%d_Elep%d_Ehad%d", detBin, elep, ehad);
            TH1D* h1D = UnselectedEvAllEnergiesHist[elep-1][ehad-1]->ProjectionX(histName, detBin, detBin);
            h1D->SetTitle(Form("DetPos bin %d; OApos; Events", detBin));

            UnSelectedEventsVsOAPosForFixedDetPos[detBin][elep][ehad] = h1D;
        }
    }
  }


  for (int elep = 1; elep <= nElepBins; ++elep) {
      for (int ehad = 1; ehad <= nEhadBins; ++ehad) {
        int sumdetBins = 0;
        // cout<<" elp = "<<elep<<" ehad = "<<ehad<<endl;
          for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
              // Check if histogram exists for this detBin, elep, ehad
              if (UnSelectedEventsVsOAPosForFixedDetPos[detBin][elep][ehad]) {
                  // Access the histogram
                  TH1D* h1D = UnSelectedEventsVsOAPosForFixedDetPos[detBin][elep][ehad];
                  sumdetBins +=1;
                  // Perform operations on the histogram
                  //std::cout << "Processing histogram for detBin = " << detBin << ", elep = " << elep << ", ehad = " << ehad << std::endl;
                  h1D->Scale(1.0 / NDCAFsPerDetPos[sumdetBins - 1]);  // Scaling if necessary

                  // Example: Draw the histogram
                  // h1D->Draw("HIST");
                  // std::cout << "Integral: " << h1D->Integral() << std::endl;
                  // cout<<" sum det bins: "<<sumdetBins<<endl;
              }

          }

      }
  }

  //Calculate efficiency = selected/Unselected vs OApos for each detPos
  std::vector<std::vector<std::vector<TH1D*>>> Efficiency;  // [detBin][elep][ehad]

  // Resize to match dimensions
  Efficiency.resize(nBinsDet + 1);
  for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
      Efficiency[detBin].resize(nElepBins + 1);
      for (int elep = 1; elep <= nElepBins; ++elep) {
          Efficiency[detBin][elep].resize(nEhadBins + 1);
          for (int ehad = 1; ehad <= nEhadBins; ++ehad) {
              // Check if both selected and unselected histograms exist
              TH1D* selected = SelectedEventsVsOAPosForFixedDetPos[detBin][elep][ehad];
              TH1D* unselected = UnSelectedEventsVsOAPosForFixedDetPos[detBin][elep][ehad];

              if (selected && unselected) {
                  // Clone selected to create the efficiency histogram
                  TString effName = Form("Efficiency_Det%d_Elep%d_Ehad%d", detBin, elep, ehad);
                  TH1D* efficiencyHist = (TH1D*)selected->Clone(effName);
                  efficiencyHist->SetTitle(Form("Efficiency DetBin %d; OApos; Efficiency", detBin));

                  // Divide selected by unselected (bin-by-bin)
                  efficiencyHist->Divide(unselected);

                  // Store result
                  Efficiency[detBin][elep][ehad] = efficiencyHist;
              } else {
                  Efficiency[detBin][elep][ehad] = nullptr;  // Mark as missing
              }
          }
      }
  }

  //==draw selected evebts vs OA pos fo each detPos
  TCanvas* CanvasAllEfficienciesVsOAPos = new TCanvas("CanvasAllEfficienciesVsOAPos", "CanvasAllEfficienciesVsOAPos");
  CanvasAllEfficienciesVsOAPos->cd();
  for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
    // Check if histogram exists for this detBin, elep, ehad

    if (Efficiency[detBin][3][2]) {
      cout<<" det bin: "<<detBin<<endl;
      Efficiency[detBin][3][2]->SetLineWidth(2);
      Efficiency[detBin][3][2]->SetLineColor(4);
      Efficiency[detBin][3][2]->GetYaxis()->SetRangeUser(0, 1);
      Efficiency[detBin][3][2]->Draw("histsames");
    }
  }

  // // Declare 2D arrays (use vector of vectors for flexibility)

  std::vector<std::vector<std::vector<double>>> EventsAtDetCenter(nElepBins, std::vector<std::vector<double>>(nEhadBins));
  // Resize to match dimensions: [nElepBins + 1][nEhadBins + 1][nBinsDet + 1]
  EventsAtDetCenter.resize(nElepBins + 1);
  // Loop over elep, ehad, and detBin
  //1. ===for spline function flat after 28.25 (which is the detector center of the last det Pos ) set same values as at det center of last detPos
  for (int elep = 0; elep <= nElepBins; ++elep) {
    EventsAtDetCenter[elep].resize(nEhadBins + 1);
      for (int ehad = 0; ehad <= nEhadBins; ++ehad) {
        // EventsAtDetCenter[elep][ehad].resize(nBinsDet + 1, 0.0);
          // for (int detBin = 0; detBin <= nBinsDet; ++detBin) {
              TH1D* hist = SelectedEventsVsOAPosForFixedDetPos[9][elep][ehad];
              if (hist) {
                  int binIdx = hist->FindBin(-28.0);  // Bin for OA = -28
                  double content = hist->GetBinContent(binIdx);
                  if(elep == 2 && ehad ==3)
                  cout<<" content bin -28 "<<content<<endl;
                  EventsAtDetCenter[elep][ehad].push_back(content);
                  EventsAtDetCenter[elep][ehad].push_back(content);
                  EventsAtDetCenter[elep][ehad].push_back(content);
                  EventsAtDetCenter[elep][ehad].push_back(content);
              }

      }
  }
    //===save selected events at detector center for each detPos used in the analysis (19 detPos used so far)

  for (int elep = 0; elep <= nElepBins; ++elep) {
      for (int ehad = 0; ehad <= nEhadBins; ++ehad) {
        int sumdetBins = 0;
          for (int detBin = 0; detBin <= nBinsDet; ++detBin) {
              TH1D* hist = SelectedEventsVsOAPosForFixedDetPos[detBin][elep][ehad];
              if (hist) {
                  sumdetBins +=1;
                  //cout<<" det Pos used: "<<detPosUsed[sumdetBins+3]<<endl;
                  int binIdx = hist->FindBin(detPosUsed[sumdetBins+3]);  // Bin for OA = -28
                  double content = hist->GetBinContent(binIdx);
                  EventsAtDetCenter[elep][ehad].push_back(content);

              }
              // else {
              //     std::cout << "Missing hist at detBin = " << detBin << ", elep = " << elep << ", ehad = " << ehad << std::endl;
              //     // EventsAtDetCenter[elep][ehad].push_back(0.0);
              //     // binOA30p25[elep][ehad].push_back(-1);
              // }
          }
      }
  }
  //
  //===3. for spline function flat before 0m (detector center for on-axis case)
  detPosUsed.push_back(0.5);
  detPosUsed.push_back(1.0);
  detPosUsed.push_back(1.5);
  detPosUsed.push_back(2.25);

  //make all bins up to first bin which is detector center = 0 (bins from 2.25 to 0) have the same number of events as at detCenter = 0
  for (int elep = 0; elep <= nElepBins; ++elep) {
      for (int ehad = 0; ehad <= nEhadBins; ++ehad) {
          // for (int detBin = 0; detBin <= nBinsDet; ++detBin) {
              TH1D* hist = SelectedEventsVsOAPosForFixedDetPos[123][elep][ehad];
              if (hist) {
                  int binIdx = hist->FindBin(0.25);  // Bin for OA = -28
                  double content = hist->GetBinContent(binIdx);

                  EventsAtDetCenter[elep][ehad].push_back(content);
                  EventsAtDetCenter[elep][ehad].push_back(content);
                  EventsAtDetCenter[elep][ehad].push_back(content);
                  EventsAtDetCenter[elep][ehad].push_back(content);

              }

      }
  }
  //
  //===4. make graph with selected Events vs OAPos and create the spline

  std::vector<std::vector<TGraph*>> graphs;
  std::vector<std::vector<TSpline3*>> splines;

  // Resize for 1-based indexing: [nElepBins + 1][nEhadBins + 1]
  graphs.resize(nElepBins + 1);
  splines.resize(nElepBins + 1);
  for (int elep = 1; elep <= nElepBins; ++elep) {
      graphs[elep].resize(nEhadBins + 1, nullptr);
      splines[elep].resize(nEhadBins + 1, nullptr);
  }
  //
  for (int elep = 1; elep <= nElepBins; ++elep) {
      for (int ehad = 1; ehad <= nEhadBins; ++ehad) {

         // Skip if no data
          if (EventsAtDetCenter[elep][ehad].size() != detPosUsed.size()) {
             std::cerr << "Size mismatch at elep=" << elep << ", ehad=" << ehad <<" size ev: "<<EventsAtDetCenter[elep][ehad].size()<<" size det: "<<detPosUsed.size()<< std::endl;
              continue;
          }


          TString graphName = Form("Graph_Elep%d_Ehad%d", elep, ehad);
          graphs[elep][ehad] = new TGraph(detPosUsed.size(), &detPosUsed[0], &EventsAtDetCenter[elep][ehad][0]);
          graphs[elep][ehad]->SetTitle(Form("Events at Center - Elep %d, Ehad %d", elep, ehad));
          graphs[elep][ehad]->SetMarkerStyle(20);
          graphs[elep][ehad]->SetName(graphName);
          cout<<" elep = "<<elep<<" ehad = "<<ehad<<endl;

          // Create spline from graph
          TString splineName = Form("Spline_Elep%d_Ehad%d", elep, ehad);
          splines[elep][ehad] = new TSpline3(splineName, graphs[elep][ehad]);
          splines[elep][ehad]->SetLineColor(kBlue);
      }
  }
  //
  //draw spline and points used for spline
  TCanvas* CanvasGraphFit = new TCanvas("CanvasGraphFit", "CanvasGraphFit");
  CanvasGraphFit->cd();
  graphs[3][2]->Draw("AP");
  splines[3][2]->Draw("LSame");
  CanvasAllSelectedEventsVsOAPos->cd();
  //draw the spline on the same canvas as Selected event
  splines[3][2]->Draw("LSame");

  TFile* outFile = new TFile("Splines_FDEventRateAtND.root", "RECREATE");
  hadEdges.Write("kHadBinEdges");
  lepEdges.Write("kLepBinEdges");

  std::vector<std::vector<TSpline3*>> normalizedSplines(nElepBins, std::vector<TSpline3*>(nEhadBins, nullptr));
  std::vector<std::vector<TF1*>> f_normalized(nElepBins, std::vector<TF1*>(nEhadBins, nullptr));

  normalizedSplines.resize(nElepBins + 1);
  f_normalized.resize(nElepBins + 1);

  for (int elep = 1; elep <= nElepBins; ++elep) {
    normalizedSplines[elep].resize(nEhadBins + 1, nullptr);
    f_normalized[elep].resize(nEhadBins + 1, nullptr);
      for (int ehad = 1; ehad <= nEhadBins; ++ehad) {
          TSpline3* spline = splines[elep][ehad];
          if (!spline) continue;


          double xmin = spline->GetXmin() - 0.25;
          double xmax = spline->GetXmax() - 0.25;

          TF1* f_spline = new TF1(Form("f_spline_e%d_h%d", elep, ehad),
              [spline](double* x, double*) { return spline->Eval(x[0]); },
              xmin, xmax, 0);

          double integral = f_spline->Integral(xmin, xmax);
          if (integral <= 0) {
              std::cerr << "Skipping elep=" << elep << ", ehad=" << ehad << " due to non-positive integral\n";
              continue;
          }
          //
          int np = spline->GetNp();
          std::vector<double> x(np), y(np);
          for (int i = 0; i < np; ++i) {
              double xi, yi;
              spline->GetKnot(i, xi, yi);
              x[i] = xi;
              y[i] = yi / integral;
          }

          TSpline3* normSpline = new TSpline3(Form("normalizedSpline_e%d_h%d", elep, ehad), &x[0], &y[0], np);
          normSpline->SetLineColor(kRed);
          normalizedSplines[elep][ehad] = normSpline;

          TF1* f_norm = new TF1(Form("f_norm_lep%d_had%d", elep, ehad),
              [normSpline](double* x, double*) { return normSpline->Eval(x[0]); },
              xmin, xmax, 0);
          f_normalized[elep][ehad] = f_norm;
          f_normalized[elep][ehad]->Write();

          double checkIntegral = f_norm->Integral(xmin, xmax);
          std::cout << "elep=" << elep << ", ehad=" << ehad
                    << " → normalized integral = " << checkIntegral << std::endl;
      }
  }

  TCanvas* CanvasNormalizedSpline = new TCanvas("CanvasNormalizedSpline", "CanvasNormalizedSpline");
  CanvasNormalizedSpline->cd();
  f_normalized[3][2]->SetLineColor(2);
  f_normalized[3][2]->Draw();

  //===create Tfile to save the splines and energy bins used

  outFile->Close();

/*


  //===5. normalize splines to 1 (probability density function)
  TCanvas* CanvasNormalizedSpline = new TCanvas("CanvasNormalizedSpline", "CanvasNormalizedSpline");
  CanvasNormalizedSpline->cd();

  // ==5.1 get min and max of function
  double xmin = spline->GetXmin() - 0.25;
  double xmax = spline->GetXmax() - 0.25;

  //5.2 Integrate the spline numerically
  TF1* f_spline = new TF1("f_spline", [spline](double* x, double*) { return spline->Eval(x[0]); }, xmin, xmax, 0);
  double integral = f_spline->Integral(xmin, xmax);

  // 5.3: Normalize — create a new set of scaled points
  int np = spline->GetNp();
  std::vector<double> x(np), y(np);

  for (int i = 0; i < np; ++i) {
      double xi, yi;
      spline->GetKnot(i, xi, yi);
      x[i] = xi;
      y[i] = yi / integral;  // normalize y
  }

  // 5.4: Create normalized spline
  TSpline3* normalizedSpline = new TSpline3("normalizedSpline", &x[0], &y[0], np);
  normalizedSpline->SetLineColor(kRed);
  normalizedSpline->Draw("AL");
  TF1* f_splinenormalized = new TF1("f_spline", [normalizedSpline](double* x, double*) { return normalizedSpline->Eval(x[0]); }, xmin, xmax, 0);
  f_splinenormalized->Draw();
  cout<<" integral normalized spline = "<<f_splinenormalized->Integral(xmin, xmax)<<" x min = "<<xmin<<" xmax = "<<xmax<<endl;

*/




}
