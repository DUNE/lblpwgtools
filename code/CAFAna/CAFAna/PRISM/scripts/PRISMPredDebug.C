void PRISMPredDebug() {
  gStyle->SetOptStat(0);
  TFile *fPRISM = new TFile("prism_pred.root");

  TH2 *ETrueND;
  fPRISM->GetObject("nufit/ETrue/NearDet", ETrueND);
  if (!ETrueND) {
    std::cout << "Error failed to read nufit/ETrue/NearDet" << std::endl;
    return;
  }
  ETrueND->GetZaxis()->SetTitle("ND NumuCC Event rate");

  TCanvas *c1 = new TCanvas("c1", "");

  ETrueND->Draw("COLZ");

  TFile *fFlux =
      new TFile("DUNE_Flux_OffAxis_Nov2017Review_syst_shifts_fine.root");

  TH2 *ETrueNDFlux;
  fFlux->GetObject("ND_nu_ppfx/LBNF_numu_flux_Nom", ETrueNDFlux);

  if (!ETrueNDFlux) {
    std::cout << "Error failed to read ND_nu_ppfx/LBNF_numu_flux_Nom"
              << std::endl;
    return;
  }

  ETrueNDFlux->RebinX(4);
  ETrueNDFlux->Scale(1.0 / 4.0);

  TFile *fFluxpane = new TFile("DUNEFluxFineOptEngNov2017.2019_03_06.root");

  TH2 *ETrueNDFluxPane;
  fFluxpane->GetObject("ND_numode/LBNF_numu_flux", ETrueNDFluxPane);

  if (!ETrueNDFluxPane) {
    std::cout << "Error failed to read ND_numode/LBNF_numu_flux" << std::endl;
    return;
  }

  ETrueNDFluxPane->RebinX(10);
  ETrueNDFluxPane->Scale(1.0 / 10.0);

  TCanvas *c2 = new TCanvas("c2", "");

  ETrueNDFlux->Draw("COLZ");

  TH2 *EXSecND = (TH2 *)ETrueND->Clone("EXSecND");

  for (int i = 0; i < EXSecND->GetXaxis()->GetNbins(); ++i) {
    for (int j = 0; j < EXSecND->GetYaxis()->GetNbins(); ++j) {
      EXSecND->SetBinContent(i + 1, j + 1,
                             EXSecND->GetBinContent(i + 1, j + 1) /
                                 ETrueNDFlux->GetBinContent(i + 1, j + 1));
      EXSecND->SetBinError(i + 1, j + 1,
                           EXSecND->GetBinError(i + 1, j + 1) /
                               ETrueNDFlux->GetBinContent(i + 1, j + 1));
    }
  }

  EXSecND->GetXaxis()->SetRangeUser(0, 5);

  TCanvas *c3 = new TCanvas("c3", "");
  Int_t *Colors_bwr;
  {

    double Stops[] = {0, 0.5, 1};
    double Reds[] = {0, 1, 1};
    double Greens[] = {0, 1, 0};
    double Blues[] = {1, 1, 0};

    int First_bwr =
        TColor::CreateGradientColorTable(3, Stops, Reds, Greens, Blues, 255, 1);

    Colors_bwr = new Int_t[255];

    for (Int_t i = 0; i < 255; ++i) {
      Colors_bwr[i] = First_bwr + i;
    }
  }
  EXSecND->Draw("COLZ");

  double Stops[] = {0, 0.5, 1};
  double Reds[] = {1, 0, 0};
  double Greens[] = {0, 1, 0};
  double Blues[] = {0, 0, 1};

  Int_t First =
      TColor::CreateGradientColorTable(3, Stops, Reds, Greens, Blues, 60, 1);

  TCanvas *c4 = new TCanvas("c4", "");
  for (int i = 0; i < 60; i += 5) {
    std::stringstream ss("");
    ss << "pi_" << i;
    TH1 *pi = EXSecND->ProjectionX(ss.str().c_str(), i + 1, i + 1);
    pi->GetXaxis()->SetRangeUser(0.5, 2.5);
    pi->SetLineColor(First + i);
    pi->SetLineWidth(4);
    ss.str("");
    ss << EXSecND->GetYaxis()->GetBinCenter(i + 1) << " m";
    pi->SetTitle(ss.str().c_str());
    pi->Draw(i ? "EHISTSAME" : "EHIST");
  }
  c4->BuildLegend();

  TCanvas *c4_y = new TCanvas("c4_y", "");
  for (int i = 5; i < 50; i += 5) {
    std::stringstream ss("");
    ss << "ei_" << i;
    TH1 *ei = EXSecND->ProjectionY(ss.str().c_str(), i + 1, i + 1);
    ei->GetYaxis()->SetRangeUser(0, 2E15);
    ei->SetLineColor(First + i);
    ei->SetLineWidth(4);
    ss.str("");
    ss << EXSecND->GetXaxis()->GetBinCenter(i + 1) << " GeV";
    ei->SetTitle(ss.str().c_str());
    ei->Draw(i ? "EHISTSAME" : "EHIST");
  }
  c4_y->BuildLegend();

  gStyle->SetPalette(255, Colors_bwr);
  gStyle->SetNumberContours(255);

  // gStyle->SetPalette(108);

  TCanvas *c5 = new TCanvas("c5", "");

  TH2 *ETrueNDFlux_rat = (TH2 *)ETrueNDFlux->Clone("ETrueNDFlux_rat");

  for (int i = 0; i < ETrueNDFlux_rat->GetXaxis()->GetNbins(); ++i) {
    for (int j = 0; j < ETrueNDFlux_rat->GetYaxis()->GetNbins(); ++j) {
      ETrueNDFlux_rat->SetBinContent(
          i + 1, j + 1,
          ETrueNDFlux_rat->GetBinContent(i + 1, j + 1) /
              ETrueNDFluxPane->GetBinContent(i + 1, j + 1));
    }
  }

  ETrueNDFlux_rat->GetZaxis()->SetRangeUser(0.85,1.15);
  ETrueNDFlux_rat->Draw("COLZ");
}
