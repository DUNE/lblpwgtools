import ROOT
ROOT.gROOT.SetBatch(1)
import math

if __name__=="__main__":
	ssth23_vals = [0.42, 0.46, 0.50, 0.54, 0.58]
	th13_vals = []
	for i in range(21): 
		curr_ss2th13 = 0.05 + (i/20.)*(0.1)
		th13_vals.append(math.asin(math.sqrt(curr_ss2th13))/2)
	
	dchisqGs = []
	bfGs = []
	for i in range(len(ssth23_vals)):
		dchisqGs.append(ROOT.TGraph())
		dchisqGs[i].SetName("dchisq_vs_trth13_th23_%lf" % ssth23_vals[i])
		dchisqGs[i].SetTitle(";True sin^{2}2#theta_{13};#Delta#chi^{2}")
		bfGs.append(ROOT.TGraph())
		bfGs[i].SetName("bfth13_vs_trth13_th23_%lf" % ssth23_vals[i])
		bfGs[i].SetTitle(";True sin^{2}2#theta_{13};Best Fit sin^{2}2#theta_{13}")
		for j in range(len(th13_vals)): 
			tfname = "/pnfs/dune/persistent/physicsgroups/dunelbl/picker24/contrib/jfleishh/main_nonunitarity_scan/fit_covar_ndfd1000ktMWyr_allsyst_th13_hie1_th13:%lf,ssth23:%lf_nominal.root" % (th13_vals[j],ssth23_vals[i])
			tf = ROOT.TFile(tfname)
			tree = tf.Get("fit_info")
			tree.GetEntry(0)
			chisq = tree.chisq
			th13_val = tree.fFakeDataVals[0]
			ss2th13_val = (math.sin(2*th13_val))**2
			dchisqGs[i].SetPoint(j,ss2th13_val,chisq)
			bf_th13_val = tree.fPostFitValues[0]
			bf_ss2th13_val = (math.sin(2*bf_th13_val))**2
			bfGs[i].SetPoint(j,ss2th13_val, bf_ss2th13_val)
			tf.Close()
	
	outf = ROOT.TFile("nonunitarity_graphs/main_dchisq_graphs.root", "RECREATE")
	for g in dchisqGs:
		g.Write()
	for g in bfGs:
		g.Write()
	outf.Close()
	print "-----DONE----"
