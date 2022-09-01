import ROOT
ROOT.gROOT.SetBatch(1)
import ctypes

def filldiff(up,down, diffgraph):
    n = up.GetN()
    i = 0
    xup = ctypes.c_double(-9.9)
    yup = ctypes.c_double(-9.9)
    xlo = ctypes.c_double(-9.9)
    ylo = ctypes.c_double(-9.9)
    while i<n:
        up.GetPoint(i,xup,yup)
        down.GetPoint(n-i-1,xlo,ylo)
        diffgraph.SetPoint(i,xup.value,yup.value)
        diffgraph.SetPoint(n+i,xlo.value,ylo.value)
        i += 1
    return True

if __name__=="__main__":
	#ssth23_vals = [0.42, 0.46, 0.50, 0.54, 0.58]
	level_names = ["1sigma", "90p", "3sigma"]
	level_names.reverse()
	legend_names = ["1#sigma", "90%", "3#sigma"]
	legend_names.reverse()
	cols = [ROOT.kRed+4,ROOT.kRed-3, ROOT.kRed-9]
	ops = [1,1,1]
	cols.reverse()
	ops.reverse()
	graph_pos = ["lowband","upband"]
	for ind in range(1):
		can = ROOT.TCanvas()
		h = ROOT.TH2D()
		h.SetTitle(";True sin^{2}#theta_{23};Allowed sin^{2}#theta_{23}")
		h.GetXaxis().SetLimits(0.4, 0.6)
		h.GetYaxis().SetLimits(0.35, 0.65)
		h.Draw()
		leg = ROOT.TLegend(0.1,0.9,0.9,1.0)
		leg.SetNColumns(3)
		for i in range(len(level_names)):
			gf = ROOT.TFile("1dscan_graphs/ssth23_1000/allwdreg_bandfull_graphs_%s.root" % (level_names[i]))
			for pos in graph_pos:
				g = gf.Get("%s" % (pos))
				g.SetFillColorAlpha(cols[i],ops[i])
				g.SetLineColor(0)
				if pos == "lowband": leg.AddEntry(g, legend_names[i])
				g.Draw("F SAME")
		leg.Draw()
		can.Print("allwdreg_plots/ssth23_1000/allwdreg_bandfull_plot.png")
		can.Print("allwdreg_plots/ssth23_1000/allwdreg_bandfull_plot.pdf")
