#include "OscLib/OscCalcPMNS.h"
#include "OscLib/OscCalcPMNS_NSI.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"

void plot_probabilities_compare_nsi()
{
  const int nPts = 100;
  const double Emin = 0.1, Emax = 10.0;
  double baseline = 810; // km
  double rho = 2.8;       // g/cm^3

  // --- Graphs ---
  TGraph* gStd = new TGraph();               // Standard PMNS
  TGraph* gNSI_0   = new TGraph();           // ε_μe = 0.0
  TGraph* gNSI_002 = new TGraph();           // ε_μe = 0.02
  TGraph* gNSI_005 = new TGraph();           // ε_μe = 0.05

  // --- Standard OscCalc ---
  osc::OscCalcPMNS* calc_std = new osc::OscCalcPMNS();
  calc_std->SetL(baseline);
  calc_std->SetRho(rho);
  calc_std->SetDmsq21(7.53e-5);
  calc_std->SetDmsq32(2.44e-3);
  calc_std->SetTh12(asin(sqrt(0.846)));
  calc_std->SetTh13(asin(sqrt(0.085)));
  calc_std->SetTh23(M_PI/4);
  calc_std->SetdCP(0);

  // --- NSI Calculators ---
  auto make_nsi_calc = [&](double eps){
    osc::OscCalcPMNS_NSI* calc = new osc::OscCalcPMNS_NSI();
    calc->SetL(baseline);
    calc->SetRho(rho);
    calc->SetDmsq21(7.53e-5);
    calc->SetDmsq32(2.44e-3);
    calc->SetTh12(asin(sqrt(0.846)));
    calc->SetTh13(asin(sqrt(0.085)));
    calc->SetTh23(M_PI/4);
    calc->SetdCP(0);
    calc->SetEps_emu(eps);
    calc->SetDelta_emu(0.0);
    return calc;
  };

  osc::OscCalcPMNS_NSI* calc_nsi_0   = make_nsi_calc(0.0);
  osc::OscCalcPMNS_NSI* calc_nsi_002 = make_nsi_calc(0.02);
  osc::OscCalcPMNS_NSI* calc_nsi_005 = make_nsi_calc(0.05);

  // --- Energy Loop ---
  for(int i = 0; i < nPts; ++i){
    double E = Emin + i * (Emax - Emin) / (nPts - 1);
    double p_std    = calc_std->P(14, 12, E);
    double p_nsi_0  = calc_nsi_0->P(14, 12, E);
    double p_nsi_02 = calc_nsi_002->P(14, 12, E);
    double p_nsi_05 = calc_nsi_005->P(14, 12, E);

    gStd->SetPoint(i, E, p_std);
    gNSI_0->SetPoint(i, E, p_nsi_0);
    gNSI_002->SetPoint(i, E, p_nsi_02);
    gNSI_005->SetPoint(i, E, p_nsi_05);
  }

  // --- Style ---
  gStd->SetLineColor(kBlack); gStd->SetLineWidth(2);
  gNSI_0->SetLineColor(kBlue); gNSI_0->SetLineStyle(2);
  gNSI_002->SetLineColor(kGreen+2); gNSI_002->SetLineStyle(2);
  gNSI_005->SetLineColor(kRed); gNSI_005->SetLineStyle(2);

  gNSI_005->SetTitle("NSI Comparison;E (GeV);Probability ( #nu_{#mu} #rightarrow #nu_{e} )");

  // --- Plot ---
  TCanvas* c = new TCanvas("c", "Oscillation Comparison", 900, 700);
  gNSI_005->Draw("AL");  // Draw frame
  gNSI_002->Draw("L SAME");
  gNSI_0->Draw("L SAME");
  gStd->Draw("L SAME");

  // --- Legend ---
  TLegend* leg = new TLegend(0.55, 0.6, 0.88, 0.88);
  leg->AddEntry(gStd,     "Standard PMNS", "l");
  leg->AddEntry(gNSI_0,   "#epsilon_{#mu e} = 0.00", "l");
  leg->AddEntry(gNSI_002, "#epsilon_{#mu e} = 0.02", "l");
  leg->AddEntry(gNSI_005, "#epsilon_{#mu e} = 0.05", "l");
  leg->Draw();

  c->SaveAs("probability_nsi_all.pdf");
}
