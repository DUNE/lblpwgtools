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
	ssth23_vals = [0.42, 0.46, 0.50, 0.54, 0.58]
	level_names = ["1sigma", "90p", "3sigma"]
	#level_names.reverse()
	legend_names = ["1#sigma", "90%", "3#sigma"]
	#legend_names.reverse()
	cols = [ROOT.kRed-9,ROOT.kBlue-9, ROOT.kYellow-9]
	#cols.reverse()
	#graph_pos = ["a", "b", "c","d","e","f"]
	for ssth23 in ssth23_vals:
		for i in range(len(level_names)):
			bands = []
			gf = ROOT.TFile("1dscan_graphs/dcp_1000/allwdreg_bandout_graphs_ssth23=%lf_%s.root" % (ssth23,level_names[i]))
			lower_bot_graphs = gf.Get("LowerBotMG").GetListOfGraphs()
			lower_top_graphs = gf.Get("LowerTopMG").GetListOfGraphs()
			upper_bot_graphs = gf.Get("UpperBotMG").GetListOfGraphs()
			upper_top_graphs = gf.Get("UpperTopMG").GetListOfGraphs()
			true_bot_graph = gf.Get("TrueBotG")
			true_top_graph = gf.Get("TrueTopG")
			for j in range(lower_bot_graphs.GetSize()):
				band = ROOT.TGraph()
				if filldiff(lower_bot_graphs.At(j), lower_top_graphs.At(j), band):
					print "band filled"
					bands.append(band)
				else: print "BAND FAILED TO FILL"
			for j in range(upper_bot_graphs.GetSize()):
				band = ROOT.TGraph()
				if filldiff(upper_bot_graphs.At(j), upper_top_graphs.At(j), band):
					print "band filled"
					bands.append(band)
				else: print "BAND FAILED TO FILL"
			band = ROOT.TGraph()
			if filldiff(true_bot_graph, true_top_graph, band):
				print "band filled"
				bands.append(band)
			else: print "BAND FAILED TO FILL"
			#lband = ROOT.TGraph()
			#lband.SetName("allwd_vs_trdcp_%s_lowband" % level_names[i])
			#if filldiff(gf.Get("allwd_vs_trdcp_%s_%s" % (level_names[i], graph_pos[0])),gf.Get("allwd_vs_trdcp_%s_%s" % (level_names[i], graph_pos[1])),lband): print "lband filled"	
			#mband = ROOT.TGraph()
			#mband.SetName("allwd_vs_trdcp_%s_midband" % level_names[i])
			#if filldiff(gf.Get("allwd_vs_trdcp_%s_%s" % (level_names[i], graph_pos[2])),gf.Get("allwd_vs_trdcp_%s_%s" % (level_names[i], graph_pos[3])), mband): print "mband filled"
			#uband = ROOT.TGraph()
			#uband.SetName("allwd_vs_trdcp_%s_upband" % level_names[i])
			#if filldiff(gf.Get("allwd_vs_trdcp_%s_%s" % (level_names[i], graph_pos[4])),gf.Get("allwd_vs_trdcp_%s_%s" % (level_names[i], graph_pos[5])), uband): print "uband filled"
			
			band_mg = ROOT.TMultiGraph()
			band_mg.SetName("BandMG")
			for g in bands:
				band_mg.Add(g)
				print "Band added to MG"	
			outf = ROOT.TFile("1dscan_graphs/dcp_1000/allwdreg_bandfull_graphs_ssth23=%lf_%s.root" % (ssth23, level_names[i]), "RECREATE")
			band_mg.Write()
			#lband.Write()
			#mband.Write()
			#uband.Write()
	print "---END---"
