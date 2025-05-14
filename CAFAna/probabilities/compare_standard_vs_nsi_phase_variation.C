#include "OscLib/OscCalcPMNS.h"
#include "OscLib/OscCalcPMNS_NSI.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TArrow.h"
#include "TColor.h"
#include "TAxis.h"


void compare_standard_vs_nsi_phase_variation()
{
    const int nPts = 200;
    const double Emin = 0.1, Emax = 10.0;   // GeV
    const double baseline = 1300;         // km
    const double rho = 2.8;               // g/cm^3
    
    std::vector<double> delta_vals = {0, M_PI/2, M_PI, 3*M_PI/2};
    std::vector<int> colors = {kRed+1, kOrange+1, kGreen+2, kBlue+1, kViolet};
    
    // --- Standard Oscillation Calculator ---
    osc::OscCalcPMNS* calc_std = new osc::OscCalcPMNS();
    calc_std->SetL(baseline);
    calc_std->SetRho(rho);
    calc_std->SetDmsq21(7.53e-5);
    calc_std->SetDmsq32(2.44e-3);
    calc_std->SetTh12(asin(sqrt(0.846)));
    calc_std->SetTh13(asin(sqrt(0.085)));
    calc_std->SetTh23(M_PI / 4);
    calc_std->SetdCP(0);
    
    // --- Standard Graph ---
    TGraph* gStd = new TGraph();
    for (int i = 0; i < nPts; ++i) {
        double E = Emin + i * (Emax - Emin) / nPts;
        gStd->SetPoint(i, E, calc_std->P(14, 12, E));
    }
    gStd->SetLineStyle(2);
    gStd->SetLineWidth(3);
    gStd->SetLineColor(kBlack);
    
    // --- Canvas Setup ---
    TCanvas* c = new TCanvas("c", "NSI Phase Variation", 900, 600);
    c->Clear();
    
    gStd->SetTitle("Transition probability  #nu_{#mu} #rightarrow #nu_{e};E (GeV);P(#nu_{#mu} #rightarrow #nu_{e})");
    gStd->GetYaxis()->SetRangeUser(0.0, 0.4);  // Set Y-axis range from 0 to 1
    
    gStd->Draw("AL");
    
    TLegend* leg = new TLegend(0.62, 0.6, 0.88, 0.88);
    leg->AddEntry(gStd, "Standard oscillation", "l");
    
    // --- Loop over NSI phases ---
    for (size_t d = 0; d < delta_vals.size(); ++d) {
        double delta = delta_vals[d];
        
        // Create a new NSI calculator and manually copy parameters
        osc::OscCalcPMNS_NSI* calc_nsi = new osc::OscCalcPMNS_NSI();
        calc_nsi->SetL(baseline);
        calc_nsi->SetRho(rho);
        calc_nsi->SetDmsq21(7.53e-5);
        calc_nsi->SetDmsq32(2.44e-3);
        calc_nsi->SetTh12(asin(sqrt(0.846)));
        calc_nsi->SetTh13(asin(sqrt(0.085)));
        calc_nsi->SetTh23(M_PI / 4);
        calc_nsi->SetdCP(0);
        
        // Set NSI parameters
        calc_nsi->SetEps_etau(0.5);
        calc_nsi->SetDelta_etau(delta);
        // calc_nsi->UpdateOscCalc(); // Ensure changes are applied
        
        // Fill graph for current delta_etau
        TGraph* gNSI = new TGraph();
        for (int i = 0; i < nPts; ++i) {
            double E = Emin + i * (Emax - Emin) / nPts;
            double prob = calc_nsi->P(14, 12, E); // νμ → νe
            gNSI->SetPoint(i, E, prob);
        }
        
        gNSI->SetLineColor(colors[d % colors.size()]);
        gNSI->SetLineWidth(2);
        gNSI->Draw("L SAME");
        
        TString label;
        int numerator = static_cast<int>(round(delta / (M_PI / 2)));
        
        if (numerator == 0)
            label = "#delta_{e#tau} = 0";
        else if (numerator == 2)
            label = "#delta_{e#tau} = #pi";
        else if (numerator == 4)
            label = "#delta_{e#tau} = 2#pi";
        else
            label.Form("#delta_{e#tau} = %d#pi/2", numerator);
        
        leg->AddEntry(gNSI, label, "l");
        
        
        leg->Draw();
        c->SaveAs("nsi_phase_variation.pdf");
    }
}

























