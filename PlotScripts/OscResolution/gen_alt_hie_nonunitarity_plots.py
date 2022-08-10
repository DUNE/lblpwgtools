import ROOT
ROOT.gROOT.SetBatch(0)

if __name__=="__main__":
	ssth23_vals = [0.42,0.46,0.50,0.54,0.58]
	ssth23_names = ["0.42", "0.46", "0.50", "0.54", "0.58"]
	dchisqGs = []
	for ssth23 in ssth23_vals:
		gf = ROOT.TFile("nonunitarity_graphs/alt_dchisq_graphs_dcp=%lf_hie-1.root" % 0.)
		dchisqGs.append(gf.Get("dchisq_vs_trth13_th23_%lf" % ssth23))

	can = ROOT.TCanvas()
	h = ROOT.TH2D()
	leg = ROOT.TLegend(0.1,0.9,0.9,1.0)
	leg.SetNColumns(3)
	h.SetTitle(";Effective True sin^{2}2#theta_{13};#Delta#chi^{2}")
	h.GetXaxis().SetLimits(0.05,0.15)
	h.GetYaxis().SetLimits(0.,50)
	h.Draw()
	cols = [1,2,4,6,8]
	i = 0
	for g in dchisqGs:
		leg.AddEntry(g, "sin^{2}#theta_{23} = %s"%ssth23_names[i])
		g.SetLineColor(cols[i])
		g.SetLineWidth(2)
		g.Draw("c")
		i+=1
	leg.Draw()
#	line = ROOT.TLine(0.05,25,0.15,25)
#	line.SetLineStyle(2)
#	line.SetLineColor(6)
#	line.SetLineWidth(2)
#	line.Draw()
	can.Print("alt_dchisq_plot_dcp=%lf_hie-1.png" % 0.)
	can.Print("alt_dchisq_plot_dcp=%lf_hie-1.pdf" % 0.)
