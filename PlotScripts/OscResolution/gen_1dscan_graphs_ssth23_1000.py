import ROOT
ROOT.gROOT.SetBatch(1)
import math

if __name__=="__main__":
	#ssth23_vals = [0.42, 0.46, 0.50, 0.54, 0.58]
	tr_ssth23_vals = []
	ssth23_names = []
	for i in range(21):
		curr_ssth23 = 0.4 + (i/20.)*(0.2)
		tr_ssth23_vals.append(curr_ssth23)
		ssth23_names.append( int(100*curr_ssth23))
	for i in range(1):
		scanGs = []
		for j in range(len(tr_ssth23_vals)):
			scanGs.append(ROOT.TGraph())
			scanGs[j].SetName("dchisq_vs_ssth23_%d_trssth23" % ssth23_names[j])
			scanGs[j].SetTitle(";sin^{2}#theta_{23};#Delta#chi^{2}")
			tf = ROOT.TFile("/pnfs/dune/persistent/physicsgroups/dunelbl/picker24/contrib/jfleishh/ssth23_1dscan_1000/asimov_ssth23_ndfd1000_allsyst_nopen_ssth23:%lf_hie1_nominal.root" % (tr_ssth23_vals[j]))
			global_tree = tf.Get("global_tree")
			global_tree.GetEntry(0)
			global_chisq = global_tree.chisq	
			tree = tf.Get("asimov_tree")
			k = 0
			#scan_left, scan_right, nbins = 0.10, 0.22, 50
			#binwidth = (scan_right - scan_left)/nbins
			for entry in tree:
				scan_ssth23 = tf.Get("ssth23").GetXaxis().GetBinCenter(k+1)
				#print scan_th13
				#print scan_ss2th13
				asimov_chisq = entry.chisq
				scanGs[j].SetPoint(k, scan_ssth23, asimov_chisq-global_chisq)
				k+=1
			print("Num Points Added: %d" %(k))
			tf.Close()
		print "Writing Graphs"
		outf = ROOT.TFile("1dscan_graphs/ssth23_1000/1dscan_graphs.root", "RECREATE")
		for g in scanGs:
			g.Write()
		print "Graphs Written"
		outf.Close()
	print "-----END-----"
