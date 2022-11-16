#include "TFile.h"
#include "TGraph.h"
#include "TH2D.h"

void gen_fixed_point_prob_plots() {
	gStyle->SetPadTopMargin(0.25);
	gROOT->ForceStyle();
	TFile gf = TFile("prob_graphs/fixed_point_prob_graphs.root");
	int cols[] = {1,2,4,6,8,46};
	TCanvas * can = new TCanvas();
	Double_t w, h;
	w = can->GetWw();
	h = can->GetWh();
	char leg_names[6][90] = {"IO, sin^{2}2#theta_{13} = 0.088, sin^{2}#theta_{23} = 0.50, #delta_{CP} = 0",
			       "NO, sin^{2}2#theta_{13} = 0.088, sin^{2}#theta_{23} = 0.50, #delta_{CP} = -#pi/2",
			       "NO, sin^{2}2#theta_{13} = 0.088, sin^{2}#theta_{23} = 0.50, #delta_{CP} = -#pi/4",
			       "NO, sin^{2}2#theta_{13} = 0.088, sin^{2}#theta_{23} = 0.50, #delta_{CP} = -3#pi/4",
			       "NO, sin^{2}2#theta_{13} = 0.088, sin^{2}#theta_{23} = 0.58, #delta_{CP} = -#pi/4",
			       "NO, sin^{2}2#theta_{13} = 0.113, sin^{2}#theta_{23} = 0.44, #delta_{CP} = -#pi/4"};
	TLegend * nueleg = new TLegend(0.1,0.78,0.9,0.98);
	nueleg->SetNColumns(1);
	TH2D * nueh = new TH2D();
	nueh->SetTitle(";E (GeV);#DeltaP/P(#nu_{#mu}#rightarrow#nu_{e})");
	nueh->GetXaxis()->SetLimits(1.,6.0);
	nueh->GetYaxis()->SetLimits(-1, 1);
	nueh->Draw();
	for (int i = 0; i < 6; ++i) {
		TGraph * nueg = (TGraph*)gf.Get(("fixedpt" + to_string(i) + "_nue_graph").c_str());
		nueg->SetLineColor(cols[i]);
		nueg->SetLineWidth(2);
		nueleg->AddEntry(nueg, leg_names[i], "L");
		nueg->Draw("C");
	}
	nueleg->Draw();
	can->Print("prob_plots/fixed_point_prob_nue_plot.png");
	can->Print("prob_plots/fixed_point_prob_nue_plot.pdf");
	can->SetLogx(true);
	can->Print("prob_plots/fixed_point_prob_nue_logplot.png");
	can->Print("prob_plots/fixed_point_prob_nue_logplot.pdf");

	can->Clear();
	can->SetLogx(false);
	TLegend * nuebleg = new TLegend(0.1,0.78,0.9,0.98);
	nuebleg->SetNColumns(1);
	TH2D * nuebh = new TH2D();
	nuebh->SetTitle(";E (GeV);#DeltaP/P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e})");
	nuebh->GetXaxis()->SetLimits(1.,6.0);
	nuebh->GetYaxis()->SetLimits(-1., 1);
	nuebh->Draw();
	for (int i = 0; i < 6; ++i) {
		TGraph * nuebg = (TGraph*)gf.Get(("fixedpt" + to_string(i) + "_nueb_graph").c_str());
		nuebg->SetLineColor(cols[i]);
		nuebg->SetLineWidth(2);
		nuebleg->AddEntry(nuebg, leg_names[i], "L");
		nuebg->Draw("C");
	}
	nuebleg->Draw();
	can->Print("prob_plots/fixed_point_prob_nueb_plot.png");
	can->Print("prob_plots/fixed_point_prob_nueb_plot.pdf");
	can->SetLogx(true);
	can->Print("prob_plots/fixed_point_prob_nueb_logplot.png");
	can->Print("prob_plots/fixed_point_prob_nueb_logplot.pdf");
}
