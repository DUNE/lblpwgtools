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
	#level_names.reverse()
	legend_names = ["1#sigma", "90%", "3#sigma"]
	#legend_names.reverse()
	cols = [ROOT.kRed-9,ROOT.kBlue-9, ROOT.kYellow-9]
	#cols.reverse()
	graph_pos = ["B", "MB", "MT", "T"]
	for ind in range(1):
		for i in range(len(level_names)):
			gf = ROOT.TFile("1dscan_graphs/ssth23_1000/allwdreg_bandout_graphs_%s.root" % (level_names[i]))
			lband = ROOT.TGraph()
			lband.SetName("lowband")
			if filldiff(gf.Get("pos_%s" % ( graph_pos[0])),gf.Get("pos_%s" % (graph_pos[1])),lband): print "lband filled"
			uband = ROOT.TGraph()
			uband.SetName("upband")
			if filldiff(gf.Get("pos_%s" % (graph_pos[2])),gf.Get("pos_%s" % (graph_pos[3])), uband): print "uband filled"
			
			outf = ROOT.TFile("1dscan_graphs/ssth23_1000/allwdreg_bandfull_graphs_%s.root" % (level_names[i]), "RECREATE")
			lband.Write()
			uband.Write()
	print "---END---"
