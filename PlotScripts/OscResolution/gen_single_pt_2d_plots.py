import ROOT
ROOT.gROOT.SetBatch(1)

if __name__=="__main__":
	hFile = ROOT.TFile("single_pt_hists/2ParamPFCLs_ndfd1000_ss2th13=0.088_dmsq32=2.45_ssth23=0.50_dcp=-0.25.root")
	#hNames = ["PFdmsq32_vs_PFss2th13", "PFssth23_vs_PFss2th13", "PFdcp_vs_PFss2th13", "PFssth23_vs_PFdmsq32", "PFdcp_vs_PFdmsq32", "PFdcp_vs_PFssth23"]
	hNames = ["PFdmsq32_vs_PFss2th13_CLs", "PFssth23_vs_PFss2th13_CLs", "PFdcp_vs_PFss2th13_CLs", "PFssth23_vs_PFdmsq32_CLs", "PFdcp_vs_PFdmsq32_CLs", "PFdcp_vs_PFssth23_CLs"]
	for name in hNames:
		can = ROOT.TCanvas()
		h = hFile.Get(name)
		h.Draw("colz")
		can.Print("2ParamPFPlots_ndfd1000_ss2th13=0.088_dmsq32=2.45_ssth23=0.50_dcp=-0.25/%s.png"%name)
	print "Plots Printed"
