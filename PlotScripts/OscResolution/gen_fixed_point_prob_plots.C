#include "TFile.h"
#include "TGraph.h"
#include "TH2D.h"

void gen_fixedpt_prob_plts() {
	TFile gf = TFile("prob_graphs/fixed_point_prob_graphs.root");
	int cols[] = {1,2,4,6,8,46};
	TCanvas * can = new TCanvas();
	TLegend * nueleg = new TLegend(0.1,0.9,0.9,1.0);
	nueleg->SetNColumns(3);
	TH2D * nueh = new TH2D();
	nueh->SetTitle(";E (GeV);#DeltaP/P(#nu_{#mu}#rightarrow#nu_{e})");
	nueh->GetXaxis()->SetLimits(0.5,6.0);
	nueh->GetYaxis()->SetLimits(-1, 1);
	nueh->Draw();
	for (int i = 0; i < 6; ++i) {
		TGraph * nueg = (TGraph*)gf.Get(("fixedpt" + to_string(i) + "_nue_graph").c_str());
		nueg->SetLineColor(cols[i]);
		nueg->SetLineWidth(2);
		nueleg->AddEntry(nueg, ("Fixed Point " + to_string(i+1)).c_str());
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
	TLegend * nuebleg = new TLegend(0.1,0.9,0.9,1.0);
	nuebleg->SetNColumns(3);
	TH2D * nuebh = new TH2D();
	nuebh->SetTitle(";E (GeV);#DeltaP/P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e})");
	nuebh->GetXaxis()->SetLimits(0.5,6.0);
	nuebh->GetYaxis()->SetLimits(-1., 1);
	nuebh->Draw();
	for (int i = 0; i < 6; ++i) {
		TGraph * nuebg = (TGraph*)gf.Get(("fixedpt" + to_string(i) + "_nueb_graph").c_str());
		nuebg->SetLineColor(cols[i]);
		nuebg->SetLineWidth(2);
		nuebleg->AddEntry(nuebg, ("Fixed Point " + to_string(i+1)).c_str());
		nuebg->Draw("C");
	}
	nuebleg->Draw();
	can->Print("prob_plots/fixed_point_prob_nueb_plot.png");
	can->Print("prob_plots/fixed_point_prob_nueb_plot.pdf");
	can->SetLogx(true);
	can->Print("prob_plots/fixed_point_prob_nueb_logplot.png");
	can->Print("prob_plots/fixed_point_prob_nueb_logplot.pdf");
}
