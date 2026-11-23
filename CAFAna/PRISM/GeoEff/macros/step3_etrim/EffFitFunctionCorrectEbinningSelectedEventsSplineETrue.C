#include <TFile.h>
#include <TVectorD.h>
#include <vector>
#include "TGraphSmooth.h"

void EffFitFunctionCorrectEbinningSelectedEventsSplineETrue() {

  TFile* FileWithEfficiency = new TFile("SelNonSelEfficiencye_EtrueVsOaPosVsDePots_AllCuts_ALLEv_perFileWeight_FlatRunPlan.root", "READ");
  FileWithEfficiency->cd();
  TDirectory* numu_EvMatch = (TDirectoryFile*) FileWithEfficiency->Get("numu_EvMatch_nom");
  numu_EvMatch->cd();
  TDirectory* FD_nu_numu = (TDirectoryFile*) numu_EvMatch->Get("FD_nu_numu");
  FD_nu_numu->cd();
  TDirectory* EffFolder = (TDirectoryFile*) FD_nu_numu->Get("MCEfficiency");
  EffFolder->cd();
  TH2D* Eff293kA = (TH2D*) EffFolder->Get("NDEff_293kA");
  Eff293kA->Draw("COLZ");
  TH2D* SelectedEv = (TH2D*) EffFolder->Get("SelectedEv_293kA");
  SelectedEv->SetDirectory(0);
  TH2D* UnselectedEv = (TH2D*)  EffFolder->Get("UnselectedEv_293kA");
  UnselectedEv->SetDirectory(0);

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


  //true energy bins in the file
  vector<double> kTrueEEdges = {0, 0.5, 0.54, 0.58, 0.62, 0.66, 0.7, 0.74, 0.78, 0.82, 0.86, 0.9, 0.94, 0.98, 1.02, 1.06,
                                1.1, 1.14, 1.18, 1.22, 1.26, 1.3, 1.34, 1.38, 1.42, 1.46, 1.5, 1.54, 1.58,
                                1.62, 1.66, 1.7, 1.74, 1.78, 1.82, 1.86, 1.9, 1.94, 1.98, 2.02, 2.1, 2.18,
                                2.26, 2.34, 2.42, 2.5, 2.58, 2.66, 2.74, 2.82, 2.9, 2.98, 3.06, 3.16, 3.26, 3.36, 3.46,
                                3.56, 3.66, 3.76, 3.86, 3.96, 4.06, 4.5, 5, 6, 10};
  const int nTrueEbins = 67;

  // convert ETrue bins to TVectors to be able to write them in a tfile
  TVectorD TrueEEdges(kTrueEEdges.size()+1);
  for (size_t i = 0; i < kTrueEEdges.size(); ++i)
      TrueEEdges[i] = kTrueEEdges[i];
  TrueEEdges[67] = 120;

  // array with number of CAF files/ det pos -> first element is very far off axis, last element is for on axis
  //fromOffaxisPOTCalc for each CAF position -> nr of entries in FileExposure_293 histogram
  double NDCAFsPerDetPos[19] = {474352.00,476560.00,454976.00, 473712.00,478656.00,454352.00,476080.00,
                                476688.00,476688.00,343168.00,478208.00, 478400.00, 465760.00, 461728.00, 447360.00, 454144.00, 462624.00, 416720.00 ,1652496.0};
  //det pos CAFS = 28.5 ,28.25, 28,26.25,25.75,24,21.75, 20 ,17.75,16,13.75,12,9.75,8,5.75,4,2,1.75,0};

  std::vector<TH2D*> SelectedEvAllEnergiesHist(nTrueEbins);
  std::vector<TH2D*> UnselectedEvAllEnergiesHist(nTrueEbins);
  std::vector<TH2D*> SelOverGenTwoDHist(nTrueEbins);

  for (int Etrue_bin = 1; Etrue_bin <= nTrueEbins; ++Etrue_bin) {
    // Create histogram for this bin combination
    TString nameSel = Form("SelectedEv_ETrue%d", Etrue_bin);
    TString nameUnsel = Form("UnselectedEv_ETrue%d", Etrue_bin);
    TString nameRatio = Form("SelOverGen_ETrue%d", Etrue_bin);

    SelectedEvAllEnergiesHist[Etrue_bin-1] = new TH2D(nameSel, nameSel, nBinsOA, EdgesBinsOA.data(), nBinsDet,EdgesBinsDetPos.data());
    UnselectedEvAllEnergiesHist[Etrue_bin-1] = new TH2D(nameUnsel, nameUnsel, nBinsOA, EdgesBinsOA.data(), nBinsDet, EdgesBinsDetPos.data());
    for (int i = 1; i <= nBinsDet; ++i) {
        for (int j = 1; j <= nBinsOA; ++j) {
            int binY = (j - 1) * nBinsOA + i;

            double valSel = SelectedEv->GetBinContent(Etrue_bin, binY);
            double valUnsel = UnselectedEv->GetBinContent(Etrue_bin, binY);

            SelectedEvAllEnergiesHist[Etrue_bin-1]->SetBinContent(j, i, valSel);
            UnselectedEvAllEnergiesHist[Etrue_bin-1]->SetBinContent(j, i, valUnsel);
        }
    }
    SelOverGenTwoDHist[Etrue_bin-1] = (TH2D*)SelectedEvAllEnergiesHist[Etrue_bin-1]->Clone(nameRatio);
    SelOverGenTwoDHist[Etrue_bin-1]->Divide(UnselectedEvAllEnergiesHist[Etrue_bin-1]);

  }

  TCanvas* CanvasSelOverGenTwoDhist = new TCanvas("CanvasSelOverGenTwoDhist", "CanvasSelOverGenTwoDhist");
  CanvasSelOverGenTwoDhist->cd();
  SelOverGenTwoDHist[3]->Draw("COLZ");

  //plot Events vs OApos for each det pos (for now all E bins)
  //1. ===Selected Events
  std::vector<std::vector<TH1D*>> SelectedEventsVsOAPosForFixedDetPos;
  SelectedEventsVsOAPosForFixedDetPos.resize(nBinsDet + 1);  // DetPos is outermost
  //
  for (int i = 0; i <= nBinsDet; ++i) {
      SelectedEventsVsOAPosForFixedDetPos[i].resize(nTrueEbins + 1);
  }

  vector<double> detPosUsed;
  //for spline function after 28.25 (which is the detector center of the last det Pos ) but we want to have extrapolation until 30.5 off-axis
  detPosUsed.push_back(-30.25);
  detPosUsed.push_back(-30);
  detPosUsed.push_back(-29.5);
  detPosUsed.push_back(-29);
  //
  // //===calculate selected events vs OA pos for each det position used in the analysis
  ///=== spline function tries to fit every point and slope between points. using all of the detector positions for the splines results in a very wobbly oscillating behavior
  // this is why I am only using detector positions every 4m off axis (i,e at 0, 4, 8, 12, 16 20, 24, 28, 28.5 ) for the fit -> more smooth non wobbly result
  // ---- if you want to use all detector positions for the spline simply comment out the
  //detBin!=115 && detBin!= 116 && detBin!= 100 && detBin!=84 && detBin!=68 && detBin!=52 && detBin!=36 && detBin!=20 && detBin!=18 && detBin!=11
  // inf the if condition that is saving the detPosUsed
  for (int Etrue_bin = 1; Etrue_bin <= nTrueEbins; ++Etrue_bin) {
      for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
          bool isEmpty = true;

          for (int oaBin = 1; oaBin <= nBinsOA; ++oaBin) {
              if (UnselectedEvAllEnergiesHist[Etrue_bin-1]->GetBinContent(oaBin, detBin) > 0) {
                  isEmpty = false;
                  break;
              }
          }
          if (isEmpty) continue;
          // Check if this detPos is already in the detPosUsed vector
          if(!isEmpty && (detBin!=115 && detBin!= 116 && detBin!= 100 && detBin!=84 && detBin!=68 && detBin!=52 && detBin!=36 && detBin!=20 && detBin!=18 && detBin!=11)){
              double detPos = EdgesBinsDetPos[detBin - 1];
              if (std::find(detPosUsed.begin(), detPosUsed.end(), detPos) == detPosUsed.end()) {
                  detPosUsed.push_back(detPos);
                 //std::cout << "det pos used: " << detPos << " det bin = "<< detBin<<std::endl;
              }
          }
          TString histName = Form("SelEventsVsOAPos_DetPos%d_ETrue%d", detBin, Etrue_bin);
          TH1D* h1D = SelectedEvAllEnergiesHist[Etrue_bin-1]->ProjectionX(histName, detBin, detBin);
          h1D->SetTitle(Form("DetPos bin %d; OApos; Events", detBin));
          //h1D->Rebin(2);

          SelectedEventsVsOAPosForFixedDetPos[detBin][Etrue_bin] = h1D;
      }
  }

  //===scale to 1.0/CafsPerDetPos
  for (int Etrue_bin = 1; Etrue_bin <= nTrueEbins; ++Etrue_bin) {

      int sumdetBins = 0;

      for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
          // Check if histogram exists for this detBin, elep, ehad
          if (SelectedEventsVsOAPosForFixedDetPos[detBin][Etrue_bin]) {
              // Access the histogram
              TH1D* h1D = SelectedEventsVsOAPosForFixedDetPos[detBin][Etrue_bin];
              sumdetBins +=1;
              // Perform operations on the histogram (i.e scale it to 1/ nr of events in the CAFs)
              h1D->Scale(1.0 / NDCAFsPerDetPos[sumdetBins - 1]);  // Scaling if necessary

              // Example: Draw the histogram
              // h1D->Draw("HIST");
              // std::cout << "Integral: " << h1D->Integral() << std::endl;
              // cout<<" sum det bins: "<<sumdetBins<<endl;
          }
      }
  }

  //==draw selected evebts vs OA pos fo each detPos
  TCanvas* CanvasAllSelectedEventsVsOAPos = new TCanvas("CanvasAllSelectedEventsVsOAPos", "CanvasAllSelectedEventsVsOAPos");
  CanvasAllSelectedEventsVsOAPos->cd();
  //===note use SelectedEventsVsOAPosForFixedDetPos[detBin][3] <-> SelectedEventsVsOAPosForFixedDetPos[detBin][ETrue_bin]
  //===for reproducing the slected events at all det positions vs off-axis position at a fixed ETrue_bin =3

  for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
    // Check if histogram exists for this detBin, etrue
    if (SelectedEventsVsOAPosForFixedDetPos[detBin][3]) {
      // cout<<" det bin: "<<detBin<<endl;
      SelectedEventsVsOAPosForFixedDetPos[detBin][3]->SetLineWidth(2);
      SelectedEventsVsOAPosForFixedDetPos[detBin][3]->SetLineColor(2);
      SelectedEventsVsOAPosForFixedDetPos[detBin][3]->GetYaxis()->SetRangeUser(0.01*1E-21, 0.25*1E-21);
      SelectedEventsVsOAPosForFixedDetPos[detBin][3]->Draw("histsames");
    }
  }
  // SelectedEventsVsOAPosForFixedDetPos[59][3]->SetLineColor(kMagenta);
  // SelectedEventsVsOAPosForFixedDetPos[59][3]->Draw("histsames");

  //2.=== Unselected Events
  std::vector<std::vector<TH1D*>> UnSelectedEventsVsOAPosForFixedDetPos;
  UnSelectedEventsVsOAPosForFixedDetPos.resize(nBinsDet + 1);  // DetPos is outermost

  for (int i = 0; i <= nBinsDet; ++i) {
      UnSelectedEventsVsOAPosForFixedDetPos[i].resize(nTrueEbins + 1);
  }
  //===calculate unselected events vs OA pos for each det position used in the analysis
  for (int Etrue_bin = 1; Etrue_bin <= nTrueEbins; ++Etrue_bin) {
    for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
        bool isEmpty = true;

        for (int oaBin = 1; oaBin <= nBinsOA; ++oaBin) {
            if (UnselectedEvAllEnergiesHist[Etrue_bin-1]->GetBinContent(oaBin, detBin) > 0) {
                isEmpty = false;
                break;
            }
        }
        if (isEmpty) continue;

        TString histName = Form("UnSelEventsVsOAPos_DetPos%d_ETrue%d", detBin, Etrue_bin);
        TH1D* h1D = UnselectedEvAllEnergiesHist[Etrue_bin-1]->ProjectionX(histName, detBin, detBin);
        h1D->SetTitle(Form("DetPos bin %d; OApos; Events", detBin));

        UnSelectedEventsVsOAPosForFixedDetPos[detBin][Etrue_bin] = h1D;
    }
  }
  //scale unselected events to CafsPerDetPos

  for (int Etrue_bin = 1; Etrue_bin <= nTrueEbins; ++Etrue_bin) {
    int sumdetBins = 0;

    for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
        // Check if histogram exists for this detBin, elep, ehad
        if (UnSelectedEventsVsOAPosForFixedDetPos[detBin][Etrue_bin]) {
            // Access the histogram
            TH1D* h1D = UnSelectedEventsVsOAPosForFixedDetPos[detBin][Etrue_bin];
            sumdetBins +=1;
            // Perform operations on the histogram i.e scale to 1 / events in CAFs
            h1D->Scale(1.0 / NDCAFsPerDetPos[sumdetBins - 1]);  // Scaling if necessary

            // Example: Draw the histogram
            // h1D->Draw("HIST");
        }
    }
  }

  TCanvas* CanvasAllUnSelectedEventsVsOAPos = new TCanvas("CanvasAllUnSelectedEventsVsOAPos", "CanvasAllUnSelectedEventsVsOAPos");
  CanvasAllUnSelectedEventsVsOAPos->cd();
  //===note use UnSelectedEventsVsOAPosForFixedDetPos[detBin][3] <-> UnSelectedEventsVsOAPosForFixedDetPos[detBin][ETrue_bin]
  //===for reproducing the unselected events (unselected events = all generated events, no selection applied) at all det positions vs off-axis position at a fixed ETrue_bin =3
  for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
    // Check if histogram exists for this detBin, elep, ehad
    if (UnSelectedEventsVsOAPosForFixedDetPos[detBin][3]) {

      UnSelectedEventsVsOAPosForFixedDetPos[detBin][3]->SetLineWidth(2);
      UnSelectedEventsVsOAPosForFixedDetPos[detBin][3]->SetLineColor(4);
      UnSelectedEventsVsOAPosForFixedDetPos[detBin][3]->GetYaxis()->SetRangeUser(0.01*1E-21, 0.6*1E-21);
      UnSelectedEventsVsOAPosForFixedDetPos[detBin][3]->Draw("histsames");
    }
  }

  //Calculate efficiency = selected/Unselected vs OApos for each detPos
  std::vector<std::vector<TH1D*>> Efficiency;  // [detBin][etrue]
  // Resize to match dimensions
  Efficiency.resize(nBinsDet + 1);

  for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
      Efficiency[detBin].resize(nTrueEbins + 1);
      for (int Etrue_bin = 1; Etrue_bin <= nTrueEbins; ++Etrue_bin) {
          // Check if both selected and unselected histograms exist
          TH1D* selected = SelectedEventsVsOAPosForFixedDetPos[detBin][Etrue_bin];
          TH1D* unselected = UnSelectedEventsVsOAPosForFixedDetPos[detBin][Etrue_bin];

          if (selected && unselected) {
              // Clone selected to create the efficiency histogram
              TString effName = Form("Efficiency_Det%d_ETrue%d", detBin, Etrue_bin);
              TH1D* efficiencyHist = (TH1D*)selected->Clone(effName);
              efficiencyHist->SetTitle(Form("Efficiency DetBin %d; OApos; Efficiency", detBin));

              // Divide selected by unselected (bin-by-bin)
              efficiencyHist->Divide(unselected);
              // Store result
              Efficiency[detBin][Etrue_bin]= efficiencyHist;
          } else {
              Efficiency[detBin][Etrue_bin] = nullptr;  // Mark as missing
          }
      }
  }

  //==draw efficiency vs OA pos for each detPos
  TCanvas* CanvasAllEfficienciesVsOAPos = new TCanvas("CanvasAllEfficienciesVsOAPos", "CanvasAllEfficienciesVsOAPos");
  CanvasAllEfficienciesVsOAPos->cd();
  for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
    // Check if histogram exists for this detBin, elep, ehad
    if (Efficiency[detBin][63]) {
      cout<<" det bin: "<<detBin<<endl;
      Efficiency[detBin][63]->SetLineWidth(2);
      Efficiency[detBin][63]->SetLineColor(4);
      Efficiency[detBin][63]->GetYaxis()->SetRangeUser(0, 1);
      Efficiency[detBin][63]->Draw("histsames");
    }
  }

  // Declare 2D arrays (use vector of vectors for flexibility)
  std::vector<std::vector<double>> EventsAtDetCenter(nTrueEbins + 1);

  // Loop over Etrue
  //1. ===for spline function flat after 28.25 (which is the detector center of the last det Pos ) set same values as at det center of last detPos
  for (int Etrue_bin = 0; Etrue_bin <= nTrueEbins; ++Etrue_bin) {
      TH1D* hist = SelectedEventsVsOAPosForFixedDetPos[9][Etrue_bin];
      if (hist) {
        // there is no data at the last detector position desired -> fill it with the last data available, i.e the content at -30
        int binIdxLastDetBinLastOAPos = hist->FindBin(-30.25);
        double contentLastDetBinLastOAPos = hist->GetBinContent(binIdxLastDetBinLastOAPos);
        EventsAtDetCenter[Etrue_bin].push_back(contentLastDetBinLastOAPos);
        for(int i = 2; i >= 0; --i){
          int binIdx = hist->FindBin(-28.5-0.5*(i+1));  //for the last bins after -28.25 (detector center at last detector position used) set the events = nr of events at those OA positions
          double content = hist->GetBinContent(binIdx);
          EventsAtDetCenter[Etrue_bin].push_back(content);
        }
      }
  }


  //===save selected events at detector center for each detPos used in the analysis (19 detPos used so far)
  ///=== spline function tries to fit every point and slope between points. using all of the detector positions for the splines results in a very wobbly oscillating behavior
  // this is why I am only using detector positions every 4m off axis (i,e at 0, 4, 8, 12, 16 20, 24, 28, 28.5 ) for the fit -> more smooth non wobbly result
  // ---- if you want to use all detector positions for the spline simply comment out the
  //detBin!=115 && detBin!= 116 && detBin!= 100 && detBin!=84 && detBin!=68 && detBin!=52 && detBin!=36 && detBin!=20 && detBin!=18 && detBin!=11

  for (int Etrue_bin = 0; Etrue_bin <= nTrueEbins; ++Etrue_bin) {
    int sumdetBins = 0;
    for (int detBin = 0; detBin <= nBinsDet; ++detBin) {
      TH1D* hist = SelectedEventsVsOAPosForFixedDetPos[detBin][Etrue_bin];
      if (hist && (detBin!=115 && detBin!= 116 && detBin!= 100 && detBin!=84 && detBin!=68 && detBin!=52 && detBin!=36 && detBin!=20 && detBin!=18 && detBin!=11)) {
          sumdetBins +=1;
          //cout<<" det Pos used: "<<detPosUsed[sumdetBins+3]<<endl;
          int binIdx = hist->FindBin(detPosUsed[sumdetBins+3]);  // Bin for OA = -28
          double content = hist->GetBinContent(binIdx);
          EventsAtDetCenter[Etrue_bin].push_back(content);

      }
    }
  }

  //===3. for spline function flat before 0m (detector center for on-axis case)
  detPosUsed.push_back(0.5);
  detPosUsed.push_back(1.0);
  detPosUsed.push_back(1.5);
  detPosUsed.push_back(2.25);

  //make all bins up to first bin which is detector center = 0 (bins from 2.25 to 0) have the same number of events as at detCenter = 0
  for (int Etrue_bin = 0; Etrue_bin <= nTrueEbins; ++Etrue_bin) {
    TH1D* hist = SelectedEventsVsOAPosForFixedDetPos[123][Etrue_bin];
    if (hist) {
        int binIdx = hist->FindBin(0.25);  // Bin for OA = -28
        double content = hist->GetBinContent(binIdx);

        EventsAtDetCenter[Etrue_bin].push_back(content);
        EventsAtDetCenter[Etrue_bin].push_back(content);
        EventsAtDetCenter[Etrue_bin].push_back(content);
        EventsAtDetCenter[Etrue_bin].push_back(content);

    }
  }

  //===4. make graph with selected Events vs OAPos and create the spline
  std::vector<TGraph*> graphs;
  std::vector<TSpline3*> splines;
  // Resize
  graphs.resize(nTrueEbins + 1);
  splines.resize(nTrueEbins + 1);

  for (int Etrue_bin = 1; Etrue_bin <= nTrueEbins; ++Etrue_bin) {
     // Skip if no data
      if (EventsAtDetCenter[Etrue_bin].size() != detPosUsed.size()) {
         std::cerr << "Size mismatch at etru = " << Etrue_bin << " size ev: "<<EventsAtDetCenter[Etrue_bin].size()<<" size det: "<<detPosUsed.size()<< std::endl;
          continue;
      }
      TString graphName = Form("Graph_ETrue%d", Etrue_bin);
      graphs[Etrue_bin] = new TGraph(detPosUsed.size(), &detPosUsed[0], &EventsAtDetCenter[Etrue_bin][0]);
      graphs[Etrue_bin]->SetTitle(Form("Events at Center - ETrue %d", Etrue_bin));
      graphs[Etrue_bin]->SetMarkerStyle(20);
      graphs[Etrue_bin]->SetName(graphName);
      // Create spline from graph
      TString splineName = Form("Spline_Etrue_bin%d", Etrue_bin);
      splines[Etrue_bin] = new TSpline3(splineName,graphs[Etrue_bin], "b1e1");//graphs[Etrue_bin]);
      splines[Etrue_bin]->SetLineColor(kBlue);
      splines[Etrue_bin]->SetLineWidth(2);

  }

  //draw spline and points used for spline
  TCanvas* CanvasGraphFit = new TCanvas("CanvasGraphFit", "CanvasGraphFit");
  CanvasGraphFit->cd();
  graphs[3]->Draw("AP");
  splines[3]->Draw("LSame");
  CanvasAllSelectedEventsVsOAPos->cd();
  //draw the spline on the same canvas as Selected event
  splines[3]->Draw("LSame");
  graphs[3]->Draw("PSame");

  //===create Tfile to save the splines and energy bins used
  TFile* outFile = new TFile("Splines_FDEventRateAtND_ETrue.root", "RECREATE");
  TrueEEdges.Write("kTrueEBinEdges");

  //==== normalize the spline to 1 (it's a probability function) and save it as a TF1 in the ouptut file
  //-- the TF1 f_normalized(Etrue) will represent the probability an event with Etrue would appear in the ND at differ OA Positions
  std::vector<TSpline3*> normalizedSplines(nTrueEbins+1);
  std::vector<TF1*> f_normalized(nTrueEbins+1);

  for (int Etrue_bin = 1; Etrue_bin <= nTrueEbins; ++Etrue_bin) {

          TSpline3* spline = splines[Etrue_bin];
          if (!spline) continue;

          double xmin = spline->GetXmin() - 0.25;
          double xmax = spline->GetXmax() - 0.25;

          TF1* f_spline = new TF1(Form("f_spline_etrue%d", Etrue_bin),
              [spline](double* x, double*) { return spline->Eval(x[0]); },
              xmin, xmax, 0);

          double integral = f_spline->Integral(xmin, xmax);
          if (integral <= 0) {
              std::cerr << "Skipping etrue =" << Etrue_bin << " due to non-positive integral\n";
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

          TSpline3* normSpline = new TSpline3(Form("normalizedSpline_etrue%d", Etrue_bin), &x[0], &y[0], np);
          normSpline->SetLineColor(kRed);
          normalizedSplines[Etrue_bin] = normSpline;

          // TF1* f_norm = new TF1(Form("f_norm_etrue%d", Etrue_bin),
          //     [normSpline](double* x, double*) { return normSpline->Eval(x[0]); },
          //     xmin, xmax, 0);
          TF1* f_norm = new TF1(Form("f_norm_etrue%d", Etrue_bin),
                [normSpline](double* x, double*) {
                  double val = normSpline->Eval(x[0]);
                  return (val > 0.0) ? val : 0.0; },
                xmin, xmax, 0);
          f_normalized[Etrue_bin] = f_norm;
          f_normalized[Etrue_bin]->Write();

          double checkIntegral = f_norm->Integral(xmin, xmax);
          std::cout << "etrue= " << Etrue_bin
                    << " → normalized integral = " << checkIntegral << std::endl;

  }

  TCanvas* CanvasNormalizedSpline = new TCanvas("CanvasNormalizedSpline", "CanvasNormalizedSpline");
  CanvasNormalizedSpline->cd();
  f_normalized[3]->SetLineColor(2);
  f_normalized[3]->Draw();

  outFile->Close();

}
