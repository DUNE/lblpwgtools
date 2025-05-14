#include "OscLib/OscCalcPMNS.h"
#include "OscLib/OscCalcPMNS_NSI.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"

void plot_probabilities_compare_nsi()
{
  // Energy range
  const int nPts = 200;
  const double Emin = 0.1, Emax = 10.0;
  double baseline = 810; // km
  double rho = 2.8;       // g/cm^3

  TGraph* gStd = new TGraph();
  TGraph* gNSI = new TGraph();

  // --- Standard Oscillation Calculator ---
  osc::OscCalcPMNS* calc_std = new osc::OscCalcPMNS();
  calc_std->SetL(baseline);
  calc_std->SetRho(rho);
  calc_std->SetDmsq21(7.53e-5);
  calc_std->SetDmsq32(2.44e-3);
  calc_std->SetTh12(asin(sqrt(0.846)));
  calc_std->SetTh13(asin(sqrt(0.085)));
  calc_std->SetTh23(M_PI/4);
  calc_std->SetdCP(0);

  // --- NSI Calculator ---
  osc::OscCalcPMNS_NSI* calc_nsi = new osc::OscCalcPMNS_NSI();
  calc_nsi->SetL(baseline);
  calc_nsi->SetRho(rho);
  calc_nsi->SetDmsq21(7.53e-5);
  calc_nsi->SetDmsq32(2.44e-3);
  calc_nsi->SetTh12(asin(sqrt(0.846)));
  calc_nsi->SetTh13(asin(sqrt(0.085)));
  calc_nsi->SetTh23(M_PI/4);
  calc_nsi->SetdCP(0);

  // --- NSI Parameters ---
  calc_nsi->SetEps_emu(0.05);          // NSI coupling
  calc_nsi->SetDelta_emu(0.0);         // NSI phase
  // You can also set other epsilons: etau, mutau, etc.

  // --- Loop over energy points ---
  for(int i = 0; i < nPts; ++i){
    double E = Emin + i*(Emax - Emin)/nPts;

    double p_std = calc_std->P(14, 12, E); // P(ν_μ → ν_e)
    double p_nsi = calc_nsi->P(14, 12, E);

    gStd->SetPoint(i, E, p_std);
    gNSI->SetPoint(i, E, p_nsi);
  }

  // --- Plot ---
  TCanvas* c = new TCanvas("c", "Oscillation Comparison", 800, 600);
  gStd->SetLineColor(kBlue);
  gStd->SetLineWidth(2);
  gStd->SetTitle("Oscillation Probability P(#nu_{#mu} #rightarrow #nu_{e});Energy (GeV);P");

  gNSI->SetLineColor(kRed);
  gNSI->SetLineWidth(2);
  gNSI->Draw("AL");
  gStd->Draw("L SAME");

  TLegend* leg = new TLegend(0.6, 0.7, 0.88, 0.88);
  leg->AddEntry(gStd, "Standard PMNS", "l");
  leg->AddEntry(gNSI, "NSI: #epsilon_{#mu e} = 0.05", "l");
  leg->Draw();

  
  c->Draw();
  c->Update();
  c->SaveAs("probability_nsi_vs_standard.pdf"); // <--- Save to file
}


