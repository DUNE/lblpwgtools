#include "Prob3ppWrapper.hxx"
#include "TGraph.h"
#include "TFile.h"
#include "TSystem.h"

bool FillProbGraphs(TGraph * nue_graph, TGraph * nueb_graph, double osc_params[6],double nom_osc_params[6], double L_km, int n) {
	OscillationHelper oh_nue, oh_nueb;
	oh_nue.SetOscillationChannel(14,12);
	oh_nueb.SetOscillationChannel(-14,-12);
	oh_nue.Setup_baseline(osc_params, L_km);
	oh_nueb.Setup_baseline(osc_params, L_km);
	
	OscillationHelper oh_nom_nue, oh_nom_nueb;
	oh_nom_nue.SetOscillationChannel(14,12);
	oh_nom_nueb.SetOscillationChannel(-14,-12);
	oh_nom_nue.Setup_baseline(nom_osc_params, L_km);
	oh_nom_nueb.Setup_baseline(nom_osc_params, L_km);
	
	for (int i = 0; i < n + 1; ++i){
		double curr_E_GeV = 0.5 + (double(i)/double(n)) * (6.0 - 0.5);
		nue_graph->SetPoint((int) i, curr_E_GeV, (oh_nue.GetWeight(curr_E_GeV) - oh_nom_nue.GetWeight(curr_E_GeV))/oh_nom_nue.GetWeight(curr_E_GeV));
		nueb_graph->SetPoint((int) i, curr_E_GeV, (oh_nueb.GetWeight(curr_E_GeV) - oh_nom_nueb.GetWeight(curr_E_GeV))/oh_nom_nueb.GetWeight(curr_E_GeV));
	}
	return true;
}

void gen_fixed_point_graphs() {
	double nom_osc_params[] = {0.310, pow(sin(asin(sqrt(0.088))/2),2), 0.50, 7.39E-5, 2.4511E-3, 0.};
	double L_km = 1285;
	double osc_params_array[6][6] = {{0.310, pow(sin(asin(sqrt(0.088))/2),2), 0.50, 7.39E-5, -2.4511E-3, 0.},
					 {0.310, pow(sin(asin(sqrt(0.088))/2),2), 0.50, 7.39E-5, 2.4511E-3, 1.5*M_PI},
					 {0.310, pow(sin(asin(sqrt(0.088))/2),2), 0.50, 7.39E-5, 2.4511E-3, 1.75*M_PI},
					 {0.310, pow(sin(asin(sqrt(0.088))/2),2), 0.50, 7.39E-5, 2.4511E-3, 1.25*M_PI},
					 {0.310, pow(sin(asin(sqrt(0.088))/2),2), 0.58, 7.39E-5, 2.4511E-3, 1.75*M_PI},
					 {0.310, pow(sin(asin(sqrt(0.113))/2),2), 0.44, 7.39E-5, 2.4511E-3, 1.75*M_PI},
					 };
	TGraph * nue_graphs[6];
	TGraph * nueb_graphs[6];
	for (int i = 0; i < 6; ++i) {
		nue_graphs[i] = new TGraph();
		nueb_graphs[i] = new TGraph();
		nue_graphs[i]->SetName(("fixedpt" + to_string(i) + "_nue_graph").c_str());
		nue_graphs[i]->SetTitle(";E (GeV);#DeltaP/P(#nu_{#mu}#rightarrow#nu_{e})");
		nueb_graphs[i]->SetName(("fixedpt" + to_string(i) + "_nueb_graph").c_str());
		nueb_graphs[i]->SetTitle(";E (GeV);#DeltaP/P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e})");
		
		if (FillProbGraphs(nue_graphs[i], nueb_graphs[i], osc_params_array[i], nom_osc_params, L_km, 1000)){
			printf("Loop %d: Graphs Filled\n", i);
		} else {
			printf("Loop %d: GRAPHS FAILED TO FILL", i);
		}
	}
	
	TFile outf = TFile("prob_graphs/fixed_point_prob_graphs.root", "RECREATE");
	for (int i = 0; i < 6; ++i) {
		nue_graphs[i]->Write();
		nueb_graphs[i]->Write();
	}
}
