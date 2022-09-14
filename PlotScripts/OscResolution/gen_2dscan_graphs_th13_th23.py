import ROOT
ROOT.gROOT.SetBatch(1)
import math

if __name__=="__main__":
	tr_ssth23_vals = [0.58,0.58,0.42, 0.42, 0.58,0.58, 0.42,0.42,0.50]
	tr_th13_vals = [0.126855,0.126855,0.150590,0.150590,0.150590,0.150590,0.171415,0.171415,0.150590]
	names = ["th13_low_th23_up_troct","th13_low_th23_up_foct","th13_dyb_th23_low_troct","th13_dyb_th23_low_foct","th13_dyb_th23_up_troct","th13_dyb_th23_up_foct","th13_up_th23_low_troct", "th13_up_th23_low_foct", "th13_dyb_th23_max_troct"]
	graphs = []
	for i in range(len(tr_ssth23_vals)):
		graphs.append(ROOT.TGraph2D())
		graphs[i].SetName("scan_2D_%s" % names[i])
		graphs[i].SetTitle(";sin^{2}2#theta_{13};sin^{2}#theta_{23}")
		for j in range(100):
			tf = ROOT.TFile("/pnfs/dune/persistent/users/jfleishh/2D_th13_th23_scan_hres_grid2/%s/asimov_th13-ssth23_ndfd1000_allsyst_nopen_th13:%lf,ssth23:%lf_hie1_nominal_%03d.root" % (names[i], tr_th13_vals[i], tr_ssth23_vals[i], j))
			if j % 10 == 0: print "Getting Fit Information from asimov_th13-ssth23_ndfd1000_allsyst_nopen_th13:%lf,ssth23:%lf_hie1_nominal_%03d.root" % (tr_th13_vals[i], tr_ssth23_vals[i], j)
			global_tree = tf.Get("global_tree")
			global_tree.GetEntry(0)
			global_chisq = global_tree.chisq
			asimov_tree = tf.Get("asimov_tree")
			k = 0
			for entry in asimov_tree:
				asimov_chisq = entry.chisq
				curr_th13 = tf.Get("th13_ssth23").GetXaxis().GetBinCenter(j+1)
				curr_ss2th13 = (math.sin(2*curr_th13))**2
				curr_ssth23 = tf.Get("th13_ssth23").GetYaxis().GetBinCenter(k+1)
				graphs[i].SetPoint(j*100 + k, curr_ss2th13, curr_ssth23, asimov_chisq-global_chisq)
				k+=1
			tf.Close()
	outf = ROOT.TFile("2dscan_th13_th23_graphs/2dscan_graphs.root", "RECREATE")
	for g in graphs:
		g.Write()
	print "Graphs Written"
	outf.Close()
	print "----END----"
