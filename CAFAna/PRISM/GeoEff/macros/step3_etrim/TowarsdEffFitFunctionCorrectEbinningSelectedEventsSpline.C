void TowarsdEffFitFunctionCorrectEbinningSelectedEventsSpline() {

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


   // Create new 2D histogram for projection
  TH2D* UnselectedEvAllEnergiesHist = new TH2D("UnselectedEvAllEnergiesHist", "Unsel EventRates All Ebins; Off-Axis Position; Detector Position",
                        //nBinsOA, 0, nBinsOA, nBinsDet, 0, nBinsDet);
                        nBinsOA, EdgesBinsOA.data(), nBinsDet, EdgesBinsDetPos.data());

  TH2D* SelectedEvAllEnergiesHist = new TH2D("SelectedEvAllEnergiesHist", "Sel EventRates All Ebins; Off-Axis Position; Detector Position",
                        //nBinsOA, 0, nBinsOA, nBinsDet, 0, nBinsDet);
                        nBinsOA, EdgesBinsOA.data(), nBinsDet, EdgesBinsDetPos.data());




  // Define the range of X bins corresponding to Ehad_bin = 4

      // User-defined selection (-1 means sum over all bins in that dimension)
    int Ehad_bin = 1;  // Choose an Ehad bin, or set to -1 to sum over all
    int Elep_bin = 1; // Choose an Elep bin, or set to -1 to sum over all

    int nElepBins = 13;
    int nEhadBins = 8;
    // Compute X bin ranges based on user selection

    // Compute X bin ranges based on user selection
  int Elep_bins_start = (Elep_bin >= 0) ? Elep_bin - 1 : 0;
  int Elep_bins_end   = (Elep_bin >= 0) ? Elep_bin - 1 : nElepBins - 1;

  int Ehad_bins_start = (Ehad_bin >= 0) ? Ehad_bin - 1 : 0;
  int Ehad_bins_end   = (Ehad_bin >= 0) ? Ehad_bin - 1 : nEhadBins - 1;


cout<<" elep start: "<<Elep_bins_start<<" elep end: "<<Elep_bins_end<<endl;

// array with number of CAF files/ det pos -> first element is very far off axis, last element is for on axis
//fromOffaxisPOTCalc
double NDCAFsPerDetPos[19] = {29647, 29785, 28436, 29607, 29916, 28397, 29755, 29892, 29793, 21448, 29888, 29900, 29110, 28858, 27960, 28384, 28914, 26045, 103281};

//===work with the 2D histogram (OAPos * detPos bins on y, Elep *Ehad bins on x) and get the selected and unselected events for each DetPos vs OAPos at fixed Ehad and fixed Elep
    for (int i = 1; i <= nBinsDet; ++i) {
        for (int j = 1; j <= nBinsOA; ++j) {
            double binContent = 0;
            double binContentSelEvents = 0;
            double binContentUnselEvents = 0;

            // Sum over selected X bins (for chosen Ehad_bin and/or Elep_bin)
            for (int elep_k = Elep_bins_start; elep_k <= Elep_bins_end; ++elep_k) {
              for (int ehad_k = Ehad_bins_start; ehad_k <= Ehad_bins_end; ++ehad_k) {

                    int k = ehad_k + elep_k * nEhadBins+1;  // Correct bin index for fastest-varying Ehad

                    int binY = (j - 1) * nBinsOA + i;  // Compute correct Y bin index

                    if (Eff293kANonans->GetBinContent(k, binY) != -1) {
                        binContent += Eff293kANonans->GetBinContent(k, binY);
                    }
                    // if (UnselectedEv->GetBinContent(k, binY)!=0)
                    //    cout<<" bin y = "<<binY<<" k = "<<k<<" sel Events = "<<SelectedEv->GetBinContent(k, binY)<<"   unsel ev "<<UnselectedEv->GetBinContent(k, binY)<<endl;
                    binContentSelEvents += SelectedEv->GetBinContent(k, binY);
                    binContentUnselEvents += UnselectedEv->GetBinContent(k, binY);

                }
            }

            // Fill the projected histograms
            UnselectedEvAllEnergiesHist->SetBinContent(j , i , binContentUnselEvents);
            SelectedEvAllEnergiesHist->SetBinContent(j , i, binContentSelEvents);
        }
    }


  TH2D* SelOverGenTwoDHist = (TH2D*)SelectedEvAllEnergiesHist->Clone();
  SelOverGenTwoDHist->Divide(UnselectedEvAllEnergiesHist);

  TCanvas* CanvasSelOverGenTwoDhist = new TCanvas("CanvasSelOverGenTwoDhist", "CanvasSelOverGenTwoDhist");
  CanvasSelOverGenTwoDhist->cd();
  SelOverGenTwoDHist->Draw("COLZ");

  //plot Events vs OApos for each det pos (for now all E bins)
  //1. ===Selected Events
  std::vector<TH1D*> SelectedEventsVsOAPosForFixedDetPos;  // Store non-empty 1D histograms
  // TCanvas* c1 = new TCanvas("c1", "Sel Events vs OApos for each DetPos", 1200, 800);

  int padIndex = 0;  // Track the number of valid histograms
  vector<double> detPosUsed;
  //for spline function flat after 28.25 (which is the detector center of the last det Pos )
  detPosUsed.push_back(-30.25);
  detPosUsed.push_back(-30);
  detPosUsed.push_back(-29.5);
  detPosUsed.push_back(-29);

  //===calculate selected events vs OA pos for each det position used in the analysis

  for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
      // Check if this detBin has any nonzero events
      bool isEmpty = true;
      for (int oaBin = 1; oaBin <= nBinsOA; ++oaBin) {
          if (UnselectedEvAllEnergiesHist->GetBinContent(oaBin, detBin) > 0) {
              isEmpty = false;
              break;  // Stop checking, this bin has events
          }
      }

      if (isEmpty) continue;  // Skip empty DetPos bins
      if (!isEmpty){
        detPosUsed.push_back(EdgesBinsDetPos[detBin-1]);
        cout<<" det pos used: "<<EdgesBinsDetPos[detBin-1]<<endl;
      }

      // Create a name for the new 1D histogram
      TString histName = Form("SelEventsVsOAPos_DetPos%f", EdgesBinsDetPos[detBin]);

      // Project X-axis for this non-empty detector position
      TH1D* h1D = SelectedEvAllEnergiesHist->ProjectionX(histName, detBin, detBin);

      h1D->SetTitle(Form("DetPos bin %f; OApos; Events", EdgesBinsDetPos[detBin-1]));
      //h1D->SetLineColor(padIndex % 10 + 1);  // Assign colors for visualization
      SelectedEventsVsOAPosForFixedDetPos.push_back(h1D);
      padIndex++;  // Increment only for valid histograms
  }

  // Adjust canvas layout
  // int nPlots = SelectedEventsVsOAPosForFixedDetPos.size();
  // int nRows = (nPlots > 3) ? 2 : 1;
  // c1->Divide((nPlots + 1) / nRows, nRows);

  for (size_t i = 0; i < SelectedEventsVsOAPosForFixedDetPos.size(); ++i) {
      std::cout<<"======= i = "<<i<<" NDCAFs = "<< NDCAFsPerDetPos[i]<< endl;
      // c1->cd(i + 1);
      //===scale Selected events to the total nr of CAFs
      SelectedEventsVsOAPosForFixedDetPos[i]->Scale(1.0/NDCAFsPerDetPos[i]);
      // SelectedEventsVsOAPosForFixedDetPos[i]->Draw("HIST");
      cout<<" integral: "<<SelectedEventsVsOAPosForFixedDetPos[i]->Integral()<<endl;
  }

  //2.=== Unselected Events
  std::vector<TH1D*> UnselectedEventsVsOAPosForFixedDetPos;  // Store non-empty 1D histograms
//  TCanvas* c2 = new TCanvas("c2", "Unsel Events vs OApos for each DetPos", 1200, 800);

  int padIndex2 = 0;  // Track the number of valid histograms
  //===calculate unselected events vs OA pos for each det position used in the analysis
  for (int detBin = 1; detBin <= nBinsDet; ++detBin) {
      // Check if this detBin has any nonzero events
      bool isEmpty = true;
      for (int oaBin = 1; oaBin <= nBinsOA; ++oaBin) {
          if (UnselectedEvAllEnergiesHist->GetBinContent(oaBin, detBin) > 0) {
              isEmpty = false;
              break;  // Stop checking, this bin has events
          }
      }

      if (isEmpty) continue;  // Skip empty DetPos bins

      // Create a name for the new 1D histogram
      TString histName = Form("UnselEventsVsOAPos_DetPos%f", EdgesBinsDetPos[detBin]);

      // Project X-axis for this non-empty detector position
      TH1D* h1D = UnselectedEvAllEnergiesHist->ProjectionX(histName, detBin, detBin);

      h1D->SetTitle(Form("Unselected Events vs OApos for DetPos bin %f; OApos; Events", EdgesBinsDetPos[detBin]));
      //h1D->SetLineColor(padIndex % 10 + 1);  // Assign colors for visualization
      UnselectedEventsVsOAPosForFixedDetPos.push_back(h1D);
      padIndex++;  // Increment only for valid histograms
  }


  // ===scale Unselected events to the total nr of CAFs
  for (size_t i = 0; i < UnselectedEventsVsOAPosForFixedDetPos.size(); ++i) {
      //c2->cd(i + 1);
      UnselectedEventsVsOAPosForFixedDetPos[i]->Scale(1.0/NDCAFsPerDetPos[i]);
      // UnselectedEventsVsOAPosForFixedDetPos[i]->Draw("HIST");
  }

  //Calculate efficiency = selected/Unselected vs OApos for each detPos
  std::vector<TH1D*> SelectedOverUnselectedEventsVsOAPosForFixedDetPos;

  for(int i = 0; i <UnselectedEventsVsOAPosForFixedDetPos.size(); ++i){

    TH1D* h1D = (TH1D*) SelectedEventsVsOAPosForFixedDetPos[i]->Clone();
    h1D->Divide(UnselectedEventsVsOAPosForFixedDetPos[i]);
    // h1D->SetName(Form("Efficiency %f; OApos; Events",  EdgesBinsDetPos[i]));
    SelectedOverUnselectedEventsVsOAPosForFixedDetPos.push_back(h1D);

  }


  //==draw selected evebts vs OA pos fo each detPos
  TCanvas* CanvasAllSelectedEventsVsOAPos = new TCanvas("CanvasAllSelectedEventsVsOAPos", "CanvasAllSelectedEventsVsOAPos");
  CanvasAllSelectedEventsVsOAPos->cd();
  SelectedEventsVsOAPosForFixedDetPos[0]->SetLineWidth(2);
  SelectedEventsVsOAPosForFixedDetPos[0]->SetLineColor(2);
  SelectedEventsVsOAPosForFixedDetPos[0]->GetYaxis()->SetRangeUser(1e-18, 1e-15);
  SelectedEventsVsOAPosForFixedDetPos[0]->Draw("hist");
  for(int i = 1; i<SelectedEventsVsOAPosForFixedDetPos.size();i++){
    SelectedEventsVsOAPosForFixedDetPos[i]->SetLineWidth(2);
    SelectedEventsVsOAPosForFixedDetPos[i]->SetLineColor(2);
    SelectedEventsVsOAPosForFixedDetPos[i]->Draw("histsames");
  }

  //==draw efficiency vs OA pos for each detPos
  TCanvas* CanvasAllEfficienciesVsOAPos = new TCanvas("CanvasAllEfficienciesVsOAPos", "CanvasAllEfficienciesVsOAPos");
  CanvasAllEfficienciesVsOAPos->cd();
  SelectedOverUnselectedEventsVsOAPosForFixedDetPos[0]->SetLineWidth(2);
  SelectedOverUnselectedEventsVsOAPosForFixedDetPos[0]->GetYaxis()->SetRangeUser(0,1);
  SelectedOverUnselectedEventsVsOAPosForFixedDetPos[0]->Draw("hist");
  for(int i = 1; i<SelectedOverUnselectedEventsVsOAPosForFixedDetPos.size();i++){
    SelectedOverUnselectedEventsVsOAPosForFixedDetPos[i]->SetLineWidth(2);
    SelectedOverUnselectedEventsVsOAPosForFixedDetPos[i]->Draw("histsames");
  }

  //===save selected events at detector center for each detPos used in the analysis (19 detPos used so far)
  int bin[19];
  vector<double> EventsAtDetCenter;
  double binOA30p25 = SelectedEventsVsOAPosForFixedDetPos[0]->FindBin(-28);
  //1. ===for spline function flat after 28.25 (which is the detector center of the last det Pos ) set same values as at det center of last detPos
  EventsAtDetCenter.push_back(SelectedEventsVsOAPosForFixedDetPos[0]->GetBinContent(binOA30p25));
  EventsAtDetCenter.push_back(SelectedEventsVsOAPosForFixedDetPos[0]->GetBinContent(binOA30p25));
  EventsAtDetCenter.push_back(SelectedEventsVsOAPosForFixedDetPos[0]->GetBinContent(binOA30p25));
  EventsAtDetCenter.push_back(SelectedEventsVsOAPosForFixedDetPos[0]->GetBinContent(binOA30p25));
  // 2. selected events (= value at the peak, i.e at detCenter) for each detPos used in the analysis
  for(int i = 0; i<SelectedEventsVsOAPosForFixedDetPos.size(); i++){
    bin[i] =SelectedEventsVsOAPosForFixedDetPos[i]->FindBin(detPosUsed[i+4]);

    EventsAtDetCenter.push_back(SelectedEventsVsOAPosForFixedDetPos[i]->GetBinContent(bin[i]));
  }
  //===3. for spline function flat before 0m (detector center for on-axis case)
  //make all bins up to first bin which is detector center = 0 (bins from 2.25 to 0) have the same number of events as at detCenter = 0
  double binOA2p25 = SelectedEventsVsOAPosForFixedDetPos[18]->FindBin(0.25);
  detPosUsed.push_back(0.5);
  EventsAtDetCenter.push_back(SelectedEventsVsOAPosForFixedDetPos[18]->GetBinContent(binOA2p25));
  detPosUsed.push_back(1.0);
  EventsAtDetCenter.push_back(SelectedEventsVsOAPosForFixedDetPos[18]->GetBinContent(binOA2p25));
  detPosUsed.push_back(1.5);
  EventsAtDetCenter.push_back(SelectedEventsVsOAPosForFixedDetPos[18]->GetBinContent(binOA2p25));
  detPosUsed.push_back(2.25);
  EventsAtDetCenter.push_back(SelectedEventsVsOAPosForFixedDetPos[18]->GetBinContent(binOA2p25));


  //===4. make graph with selected Events vs OAPos and create the spline
  TGraph* GraphAllEBinsEventsAtDetCenterVsDetPos = new TGraph(27, &detPosUsed[0], &EventsAtDetCenter[0]);
  TCanvas* CanvasGraphFit = new TCanvas("CanvasGraphFit", "CanvasGraphFit");
  CanvasGraphFit->cd();
  GraphAllEBinsEventsAtDetCenterVsDetPos->SetMarkerStyle(20);
  GraphAllEBinsEventsAtDetCenterVsDetPos->Draw("AP");
  TSpline3* spline = new TSpline3("spline", GraphAllEBinsEventsAtDetCenterVsDetPos);
  spline->SetLineColor(4);
  spline->Draw("LSame");

  //draw the spline on the same canvas as Selected events
  CanvasAllSelectedEventsVsOAPos->cd();
  spline->Draw("LSame");

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






}
