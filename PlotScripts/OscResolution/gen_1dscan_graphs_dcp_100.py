import ROOT
ROOT.gROOT.SetBatch(1)
import math

if __name__=="__main__":
	ssth23_vals = [0.42, 0.46, 0.50, 0.54, 0.58]
	tr_dcp_vals = []
	dcp_names = []
	for i in range(41):
		curr_dcp = -1. + (i/40.)*(2)
		tr_dcp_vals.append(curr_dcp)
		dcp_names.append( int(100*curr_dcp))
	for i in range(len(ssth23_vals)):
		scanGs = []
		for j in range(len(tr_dcp_vals)):
			scanGs.append(ROOT.TGraph())
			scanGs[j].SetName("dchisq_vs_dcp_%d_trdcp" % dcp_names[j])
			scanGs[j].SetTitle(";#delta_{cp};#Delta#chi^{2}")
			tf = ROOT.TFile("/pnfs/dune/persistent/physicsgroups/dunelbl/picker24/contrib/jfleishh/dcp_1dscan_100/asimov_deltapi_ndfd100_allsyst_nopen_ssth23:%lf,deltapi:%lf_hie1_nominal.root" % (ssth23_vals[i], tr_dcp_vals[j]))
			global_tree = tf.Get("global_tree")
			global_tree.GetEntry(0)
			global_chisq = global_tree.chisq	
			tree = tf.Get("asimov_tree")
			k = 0
			#scan_left, scan_right, nbins = 0.10, 0.22, 50
			#binwidth = (scan_right - scan_left)/nbins
			for entry in tree:
				scan_dcp = tf.Get("deltapi").GetXaxis().GetBinCenter(k+1)
				#print scan_th13
				#print scan_ss2th13
				asimov_chisq = entry.chisq
				scanGs[j].SetPoint(k, scan_dcp, asimov_chisq-global_chisq)
				k+=1
			print("Num Points Added: %d" %(k))
			tf.Close()
		print "Writing Graphs"
		outf = ROOT.TFile("1dscan_graphs/dcp_100/1dscan_graphs_ssth23=%lf.root"%ssth23_vals[i], "RECREATE")
		for g in scanGs:
			g.Write()
		print "Graphs Written"
		outf.Close()
	print "-----END-----"
