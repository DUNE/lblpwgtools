import ROOT
ROOT.gROOT.SetBatch(1)
import math

if __name__=="__main__":
	ssth23_vals = [0.42, 0.46, 0.50, 0.54, 0.58]
	tr_ss2th13_vals = []
	tr_th13_vals = []
	ss2th13_names = []
	for i in range(21):
		curr_ss2th13 = 0.063 + (i/20.)*(0.05)
		tr_ss2th13_vals.append(curr_ss2th13)
		ss2th13_names.append( int(10000*curr_ss2th13))
		tr_th13_vals.append(math.asin(math.sqrt(curr_ss2th13))/2)
	for i in range(len(ssth23_vals)):
		scanGs = []
		for j in range(len(tr_th13_vals)):
			scanGs.append(ROOT.TGraph())
			scanGs[j].SetName("dchisq_vs_ss2th13_%d_trss2th13" % ss2th13_names[j])
			scanGs[j].SetTitle(";sin^{2}2#theta_{13};#chi^{2}")
			tf = ROOT.TFile("/pnfs/dune/persistent/users/jfleishh/nopen_scan_grid/asimov_th13_ndfd1000_allsyst_nopen_th13:%lf,ssth23:%lf_hie1_nominal.root" % (tr_th13_vals[j], ssth23_vals[i]))
			global_tree = tf.Get("global_tree")
			global_tree.GetEntry(0)
			global_chisq = global_tree.chisq	
			tree = tf.Get("asimov_tree")
			k = 0
			#scan_left, scan_right, nbins = 0.10, 0.22, 50
			#binwidth = (scan_right - scan_left)/nbins
			for entry in tree:
				scan_th13 = tf.Get("th13").GetXaxis().GetBinCenter(k+1)
				scan_ss2th13 = (math.sin(2*scan_th13))**2
				#print scan_th13
				#print scan_ss2th13
				asimov_chisq = entry.chisq
				scanGs[j].SetPoint(k, scan_ss2th13, asimov_chisq-global_chisq)
				k+=1
			print("Num Points Added: %d" %(k))
			tf.Close()
		print "Writing Graphs"
		outf = ROOT.TFile("1dscan_graphs/ss2th13_1000/1dscan_graphs_ssth23=%lf.root"%ssth23_vals[i], "RECREATE")
		for g in scanGs:
			g.Write()
		print "Graphs Written"
		outf.Close()
	print "-----END-----"
