void PlotAllOAPosWithCAFLikeCutClean(){

	//this is the file obtained at the very end of geoEff analysis
	//i.e after having run FDHadronGeoEff, FDMuonGeoEff and the Etrim Analysis and after we summed together each individual file obtained after Etrim analysis
	// shortly this file contains ALL FDevents (obtained from all available ntuples) translated to ND with hadron and geo eff applied and all of the analysis cuts (like CAFLikeWeight, FDEventRateAtND etc)
	TFile* FileGeoEff = new TFile("AllOAPosWithFDEvRateForEtrue_EtrimSummed_histograms_NoCoeffsApplied_WithOscillatedSpectrum_CAFLikeWeight_2Dhistograms_withWeightPmuonChange.root", "READ");
	//replace the file with the "with oscillations" file if interested in the oscillated scenario
	FileGeoEff->cd();

	TH2D* SelectedEvGeoEff = (TH2D*) FileGeoEff->Get("SelectedEventsTwoDHisto_FDEvt_0");
	SelectedEvGeoEff->SetDirectory(0);
	// SelectedEvGeoEff->Scale(1, "width");
	TH2D* AllThrowsGeoEff = (TH2D*) FileGeoEff->Get("AllThrownEventsVsOAPosVsTotalETrim_FDEvt_0");
	AllThrowsGeoEff->SetDirectory(0);


	TH1D* AllGenEvGeoEffTrueE = (TH1D*) FileGeoEff->Get("Oschist_EnuFDEnergy");
	//===replace Get("hist_EnuFDEnergy" with Get("Oschist_EnuFDEnergy") if you are interested in the "oscillated scenario "
	AllGenEvGeoEffTrueE->SetDirectory(0);

	//these are the edges used for the energy binning : we have same binning in PRISM and in the file obtained from GeoEff for reco (VisEtrim) energy, and we would want to keep same binning for any energy plotting:
	//in this case the bin edges are used to bin the true energy from the GeoEff file same as the true energy in PRISM
	std::vector<double> edges;
  edges.emplace_back(0.);
  edges.emplace_back(0.5);
  while (edges.back() < 2.) {
    edges.emplace_back(edges.back() + 0.04);
  }
  while (edges.back() < 3.) {
    edges.emplace_back(edges.back() + 0.08);
  }
  while (edges.back() < 4.) {
    edges.emplace_back(edges.back() + 0.1);
  }
  edges.emplace_back(4.5);
  edges.emplace_back(5.);
  edges.emplace_back(6.);
  edges.emplace_back(10.);
  edges.emplace_back(120.);
	int nBins = edges.size() - 1; // Number of bins is size - 1

	//this is the histogram with the true neutrino energy at the FD as we have it from the ntuples -> useful to cross check you have the same true energy distribution in PRISM and GeoEff
	TH1D* TrueEnuGeoEffSameBins = new TH1D("TrueEnuGeoEffSameBins", "TrueEnuGeoEffSameBins", nBins, &edges[0]);

	for (int i = 1; i <= AllGenEvGeoEffTrueE->GetNbinsX(); ++i) {
			double content = AllGenEvGeoEffTrueE->GetBinContent(i);
			double error = AllGenEvGeoEffTrueE->GetBinError(i);
			double binCenter = AllGenEvGeoEffTrueE->GetBinCenter(i);

			int bin = TrueEnuGeoEffSameBins->FindBin(binCenter);

			if(bin>=1 && bin<= nBins){
				TrueEnuGeoEffSameBins->AddBinContent(bin, content);
				TrueEnuGeoEffSameBins->SetBinError(bin, std::sqrt(std::pow(TrueEnuGeoEffSameBins->GetBinError(bin), 2) + std::pow(error, 2)));
		}
	}

	TrueEnuGeoEffSameBins->Scale(1, "width"); // always scale to (1, "width") when having non-equal energy bins, otherwise spectrum looks weird; we always look at events / GeV
	TrueEnuGeoEffSameBins->SetLineWidth(2);
	TrueEnuGeoEffSameBins->SetLineColor(kOrange-1);
	TrueEnuGeoEffSameBins->Scale(1.0/TrueEnuGeoEffSameBins->Integral());


	//this is the file with the PRISM Prediction, i.e what we want to be able to match with. //NoOsc: "PRISMPred_ERecFromDepWithNeutronEnergyAndTrueEmu_NoSysts_NoOsc_FlatRunPlan_AllOAPos_correcPerFileWeight_NDEffVsEreco.root"
	TFile* PRISMFile = new TFile("PRISMPred_EdepWithNeutronEnergy_NoSysts_WithOsc_FlatRunPlan.root", "READ");
	PRISMFile->cd();
	// this is the ND Data at 293kA after Nd background has been subtracted
	TH2D* SelectedEvPRISM = (TH2D*) PRISMFile->Get("numu_EvMatch_nom/FD_nu_numu/NDDataCorr2D_293kA");
	SelectedEvPRISM->SetDirectory(0);
	// this is the ND Data at 280kA after Nd background has been subtracted
	TH2D* SelectedEvPRISM280 = (TH2D*) PRISMFile->Get("numu_EvMatch_nom/FD_nu_numu/NDDataCorr2D_280kA");
	SelectedEvPRISM280->SetDirectory(0);
	//thesse are events vs off-axis position vs true neutrino energy the ND (at 293 kA) (per POT)
	TH2D* AllGenEvPRISMVsETrueVsOA = (TH2D*) PRISMFile->Get("numu_EvMatch_nom/FD_nu_numu/NDFD_matcher/last_match_DebugND_293kA");
	AllGenEvPRISMVsETrueVsOA->SetDirectory(0);
	AllGenEvPRISMVsETrueVsOA->Scale(1, "width");
	AllGenEvPRISMVsETrueVsOA->Scale(1.0/AllGenEvPRISMVsETrueVsOA->Integral());
	//thesse are events vs off-axis position vs true neutrino energy the ND (at 280 kA) (per POT)
	TH2D* AllGenEvPRISMVsETrueVsOA280 = (TH2D*) PRISMFile->Get("numu_EvMatch_nom/FD_nu_numu/NDFD_matcher/last_match_DebugND_280kA");
	AllGenEvPRISMVsETrueVsOA280->SetDirectory(0);
	AllGenEvPRISMVsETrueVsOA280->Scale(1, "width");
	AllGenEvPRISMVsETrueVsOA280->Scale(1.0/AllGenEvPRISMVsETrueVsOA280->Integral());
	//this is the FD target spectrum that we want to reproduce with PRISM
	TH1D* FDTargetSpectrumETrue = (TH1D*) PRISMFile->Get("numu_EvMatch_nom/FD_nu_numu/NDFD_matcher/last_match_DebugTarget");
	FDTargetSpectrumETrue->SetDirectory(0);
	FDTargetSpectrumETrue->Scale(1, "width");
	FDTargetSpectrumETrue->Scale(1.0/FDTargetSpectrumETrue->Integral());
	//this is the linearly combined ND data vs true neutrino energy
	TH1D* LinCombMatchSpectrumETrue = (TH1D*) PRISMFile->Get("numu_EvMatch_nom/FD_nu_numu/NDFD_matcher/last_match_DebugMatch");
	LinCombMatchSpectrumETrue->SetDirectory(0);
	LinCombMatchSpectrumETrue->Scale(1, "width");
	LinCombMatchSpectrumETrue->Scale(1.0/LinCombMatchSpectrumETrue->Integral());
	// ==this is the linearly combined Selected ND Events (vs Ereco) after ND background subtraction i.e from NDData we subtract the background and then we apply the coefficients
	// == this will be as a function of reconstructed energy in the ND = deposited hadron energy + true muon energy for this file
	//==this is what we want to match with our FD events translated to the ND
	TH1D* PRISMDataTimesCoeff = (TH1D*) PRISMFile->Get("numu_EvMatch_nom/FD_nu_numu/NDLinearComb");
	PRISMDataTimesCoeff->SetDirectory(0);
	PRISMDataTimesCoeff->SetLineWidth(2);
	PRISMDataTimesCoeff->SetLineColor(4);
	//PRISMDataTimesCoeff->Scale(1.0/PRISMDataTimesCoeff->Integral());
	//===these are the coefficients vs off-axis position
	TH1D* Coeffs293kA = (TH1D*)PRISMFile->Get("numu_EvMatch_nom/FD_nu_numu/NDFDWeightings_293kA");
	Coeffs293kA->SetDirectory(0);
	TH1D* Coeffs280kA = (TH1D*)PRISMFile->Get("numu_EvMatch_nom/FD_nu_numu/NDFDWeightings_280kA");
	Coeffs280kA->SetDirectory(0);


	TCanvas* CanvasCoeff = new TCanvas("CanvasCoeff", "");
	Coeffs293kA->Draw("hist");

	//in case you want to apply the coefficients to the selected FD events -> we don't want to but might be a good way to show why we don't need to do this
	TH2D* SelectedEvGeoEffTimesCoeff = (TH2D*)SelectedEvGeoEff->Clone();
	SelectedEvGeoEffTimesCoeff->SetDirectory(0);

	int nY = SelectedEvGeoEffTimesCoeff->GetNbinsY();
  int nX = SelectedEvGeoEffTimesCoeff->GetNbinsX();


	for (int iy = 1; iy <= nY; ++iy) {
	    double c = Coeffs293kA->GetBinContent(iy);

	    // Loop over all X bins in this Y slice
	    for (int ix = 1; ix <= nX; ++ix) {
	        double oldv = SelectedEvGeoEffTimesCoeff->GetBinContent(ix, iy);
	        SelectedEvGeoEffTimesCoeff->SetBinContent(ix, iy, oldv * c);
	    }
	}

	//scale everything to bin widht, i.e events / GeV
	SelectedEvGeoEff->Scale(1, "width");
	SelectedEvGeoEffTimesCoeff->Scale(1,"width");
	AllThrowsGeoEff->Scale(1, "width");
	TCanvas* Canvas2DEventsGeoEff = new TCanvas("Canvas2DEventsGeoEff", "Canvas2DEventsGeoEff");
	Canvas2DEventsGeoEff->cd();
	Canvas2DEventsGeoEff->Divide(1,2);
	Canvas2DEventsGeoEff->cd(1);
	SelectedEvGeoEff->Draw("COLZ");
	Canvas2DEventsGeoEff->cd(2);
	AllThrowsGeoEff->Draw("COLZ");
	// SelectedEvGeoEffTimesCoeff->Draw("COLZ");

	// draw selected events from PRISM versus Ereco vs off-axis position
	TCanvas* Canvas2dPRISM = new TCanvas("Canvas2dPRISM", "Canvas2dPRISM");
	Canvas2dPRISM->cd();
	SelectedEvPRISM->Draw("COLZ");

	//scale so that integral = 1 -> for 1 to 1 comparison
	//AllThrowsGeoEff->Scale(1.0/AllThrowsGeoEff->Integral());
	// SelectedEvGeoEff->Scale(1.0/SelectedEvGeoEff->Integral());
	SelectedEvPRISM->Scale(1.0/SelectedEvPRISM->Integral());
	SelectedEvPRISM280->Scale(1.0/SelectedEvPRISM280->Integral());
	SelectedEvGeoEffTimesCoeff->Scale(1.0/SelectedEvGeoEffTimesCoeff->Integral());

	//project on the x-axis -> get selected events vs energy
	TH1D* SelectedEvGeoEffVsE = (TH1D*) SelectedEvGeoEff->ProjectionX();
	SelectedEvGeoEffVsE->SetDirectory(0);
	SelectedEvGeoEffVsE->SetLineWidth(2);
	SelectedEvGeoEffVsE->SetLineColor(kMagenta);

	TH1D* SelectedEvTimesCoeffGeoEffVsE = (TH1D*) SelectedEvGeoEffTimesCoeff->ProjectionX();
	SelectedEvTimesCoeffGeoEffVsE->SetDirectory(0);
	SelectedEvTimesCoeffGeoEffVsE->SetLineWidth(2);
	SelectedEvTimesCoeffGeoEffVsE->SetLineColor(1);

	// SelectedEvGeoEffVsE->Scale(1.0/SelectedEvGeoEffVsE->Integral());
	TH1D* SelectedEvPRISMVsE = (TH1D*) SelectedEvPRISM->ProjectionX();
	SelectedEvPRISMVsE->SetDirectory(0);
	SelectedEvPRISMVsE->SetLineWidth(2);
	SelectedEvPRISMVsE->SetLineColor(4);
	// SelectedEvPRISMVsE->Scale(1.0/SelectedEvPRISMVsE->Integral());
	TH1D* SelectedEvPRISMVsE280 = (TH1D*) SelectedEvPRISM280->ProjectionX();
	SelectedEvPRISMVsE280->SetDirectory(0);
	SelectedEvPRISMVsE280->SetLineWidth(2);
	SelectedEvPRISMVsE280->SetLineColor(1);
	// SelectedEvPRISMVsE280->Scale(1.0/SelectedEvPRISMVsE280->Integral());

	TH1D*AllGenEvPRISMVsETrue = (TH1D*)AllGenEvPRISMVsETrueVsOA->ProjectionX();
	AllGenEvPRISMVsETrue->SetDirectory(0);
	AllGenEvPRISMVsETrue->SetLineColor(kGreen-2);
	AllGenEvPRISMVsETrue->SetLineWidth(2);

	TH1D* AllGenEvPRISMVsETrue280 = (TH1D*)AllGenEvPRISMVsETrueVsOA280->ProjectionX();
	AllGenEvPRISMVsETrue280->SetDirectory(0);
	AllGenEvPRISMVsETrue280->SetLineColor(kGreen-4);
	AllGenEvPRISMVsETrue280->SetLineWidth(2);


	TCanvas* CanvasSelEvVsE = new TCanvas("CanvasSelEvVsE", "CanvasSelEvVsE");
	CanvasSelEvVsE->cd();
	//scale so that integral = 1 for 1 to 1 comparison
	SelectedEvGeoEffVsE->Scale(1.0/SelectedEvGeoEffVsE->Integral());
	PRISMDataTimesCoeff->Scale(1.0/PRISMDataTimesCoeff->Integral());

	cout<<" int sel ev: "<<SelectedEvGeoEff->Integral()<<" integral nd lin comb: "<<PRISMDataTimesCoeff->Integral()<<endl;

	// SelectedEvTimesCoeffGeoEffVsE->Draw("hist");
	SelectedEvGeoEffVsE->Draw("hist");
	PRISMDataTimesCoeff->Draw("histsames");

	FDTargetSpectrumETrue->SetLineColor(1);
	// FDTargetSpectrumETrue->Draw("histsames");
	TLegend* leg = new TLegend();
	leg->AddEntry(SelectedEvGeoEffVsE, "GeoEff");
	//leg->AddEntry(SelectedEvTimesCoeffGeoEffVsE, "GeoEff x coeffs");
	leg->AddEntry(PRISMDataTimesCoeff, "ND Lin Combined");
	leg->Draw("same");



	TCanvas* FDTrueEnergyCanas = new TCanvas("FDTrueEnergyCanas","FDTrueEnergyCanas");
	FDTargetSpectrumETrue->SetLineColor(kGreen -6);
	FDTargetSpectrumETrue->SetLineWidth(2);
	FDTargetSpectrumETrue->Draw("hist");
	TrueEnuGeoEffSameBins->SetLineColor(kOrange);
	TrueEnuGeoEffSameBins->Draw("histsames");
	LinCombMatchSpectrumETrue->SetLineWidth(2);
	LinCombMatchSpectrumETrue->SetLineColor(kBlue);
	LinCombMatchSpectrumETrue->Draw("histsames");


	TLegend* legEtruFD = new TLegend();
	legEtruFD->AddEntry(TrueEnuGeoEffSameBins, "Etrue Distribution @ FD for GeoEff");
	legEtruFD->AddEntry(FDTargetSpectrumETrue, "Target FD Spectrum, CAFs ");
	legEtruFD->AddEntry(LinCombMatchSpectrumETrue, "ND Lin Comb Match");
	legEtruFD->Draw("same");



	// TH2D* SelEvGeoEffOverSelEvPRISM = (TH2D*) SelectedEvGeoEff->Clone();
	// SelEvGeoEffOverSelEvPRISM->Divide(SelectedEvPRISM);
	// TCanvas* RatioSelGeoEffOverPRISMCanvas = new TCanvas("RatioSelGeoEffOverPRISMCanvas", "RatioSelGeoEffOverPRISMCanvas");
	// RatioSelGeoEffOverPRISMCanvas->cd();
	// SelEvGeoEffOverSelEvPRISM->Draw("COLZ");

	// SelectedEvGeoEff->Draw("COLZ");

	//===draw efficiencies and efficiency ratio

	TCanvas* CanvasEffs = new TCanvas("CanvasEffs", "CanvasEffs");
	CanvasEffs->Divide(1,2);
	CanvasEffs->cd(1);
	TH2D* effSelOverGenGeoEff = (TH2D*)SelectedEvGeoEff->Clone();
	effSelOverGenGeoEff->Divide(AllThrowsGeoEff);
	// effSelOverGenGeoEff->GetZaxis()->SetRangeUser(0,0.75);
	effSelOverGenGeoEff->Draw("COLZ");
	cout<<" x bin = "<<effSelOverGenGeoEff->GetXaxis()->GetNbins()<<" y bins "<<effSelOverGenGeoEff->GetYaxis()->GetNbins()<<endl;

	CanvasEffs->cd(2);
	TH2D* PRISMNDEff293 = (TH2D*) PRISMFile->Get("numu_EvMatch_nom/FD_nu_numu/MCEfficiency/NDEff_293kA");
	PRISMNDEff293->SetDirectory(0);
	// PRISMNDEff293->GetZaxis()->SetRangeUser(0,0.75);
	PRISMNDEff293->Draw("COLZ");

	TCanvas* CanvasEffsRatio = new TCanvas("CanvasEffsRatio", "CanvasEffsRatio");
	TH2D* GeoEffOVerPRISMEffRatio = (TH2D*) effSelOverGenGeoEff->Clone();
	// GeoEffOVerPRISMEffRatio->Add(PRISMNDEff293, -1);
	GeoEffOVerPRISMEffRatio->Divide(PRISMNDEff293);
	GeoEffOVerPRISMEffRatio->Draw("COLZ");



}
