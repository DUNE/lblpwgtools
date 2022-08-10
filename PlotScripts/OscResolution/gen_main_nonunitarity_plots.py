import ROOT
ROOT.gROOT.SetBatch(0)

if __name__=="__main__":
	ssth23_vals = [0.42,0.46,0.50,0.54,0.58]
	leg_names = ["0.42", "0.46","0.50", "0.54", "0.58"]
	dcp_vals = [-0.5,0.,0.5]
	dchisqGs = []
	bfGs = []
	for ssth23 in ssth23_vals:
		gf = ROOT.TFile("nonunitarity_graphs/main_dchisq_graphs.root")
		dchisqGs.append(gf.Get("dchisq_vs_trth13_th23_%lf" % ssth23))
		bfGs.append(gf.Get("bfth13_vs_trth13_th23_%lf" % ssth23))
	
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
		leg.AddEntry(g, "sin^{2}#theta_{23} = %s"%leg_names[i])
		g.SetLineColor(cols[i])
		g.SetLineWidth(2)
		g.Draw("c")
		i+=1
	leg.Draw()
	can.Print("nonunitarity_plots/main_dchisq_plot.png")
	can.Print("nonunitarity_plots/main_dchisq_plot.pdf")
	can.Clear()
	h = ROOT.TH2D()
	leg = ROOT.TLegend(0.1,0.9,0.9,1.0)
	leg.SetNColumns(3)
	h.SetTitle(";True sin^{2}2#theta_{13};Best Fit sin^{2}2#theta_{13}")
	h.GetXaxis().SetLimits(0.05,0.15)
	h.GetYaxis().SetLimits(0.07,0.12)
	h.Draw()
	i = 0
	for g in bfGs:
		leg.AddEntry(g,"sin^{2}#theta_{23} = %s"%leg_names[i])
		g.SetLineColor(cols[i])
		g.SetLineWidth(2)
		g.Draw("c")
		i+=1
	leg.Draw()
	can.Print("nonunitarity_plots/main_bfth13_vs_trth13_plot.png")
	can.Print("nonunitarity_plots/main_bfth13_vs_trth13_plot.pdf")
