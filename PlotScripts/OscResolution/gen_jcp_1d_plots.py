import ROOT
ROOT.gROOT.SetBatch(1)

if __name__=="__main__":
	gf = ROOT.TFile("jcp_hists/jcp_1d_hists_dcp=-0.25.root")
	can = ROOT.TCanvas()
	g100 = gf.Get("ndfd100_jcp_freq_hist")
	g100.Draw("HIST")
	can.Print("jcp_plots/ndfd100_jcp_1d_plot_dcp=-0.25.png")
	can.Print("jcp_plots/ndfd100_jcp_1d_plot_dcp=-0.25.pdf")
	can.Clear()
	g1000=gf.Get("ndfd1000_jcp_freq_hist")
	g1000.Draw("HIST")
	can.Print("jcp_plots/ndfd1000_jcp_1d_plot_dcp=-0.25.png")
	can.Print("jcp_plots/ndfd1000_jcp_1d_plot_dcp=-0.25.pdf")
