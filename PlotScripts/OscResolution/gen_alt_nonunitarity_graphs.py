import ROOT
ROOT.gROOT.SetBatch(1)
import math

if __name__=="__main__":
	dcp_vals = [-0.5,0.,0.5]
	ssth23_vals = [0.42, 0.46, 0.50, 0.54, 0.58]
	th13_vals = []
	for i in range(21): 
		curr_ss2th13 = 0.05 + (i/20.)*(0.1)
		th13_vals.append(math.asin(math.sqrt(curr_ss2th13))/2)
	
	for n in range(len(dcp_vals)):		
		dchisqGs = []
		for i in range(len(ssth23_vals)):
			dchisqGs.append(ROOT.TGraph())
			dchisqGs[i].SetName("dchisq_vs_trth13_th23_%lf" % ssth23_vals[i])
			dchisqGs[i].SetTitle(";True sin^{2}2#theta_{13};#Delta#chi^{2}")
			for j in range(len(th13_vals)): 
				tfname = "/pnfs/dune/persistent/users/jfleishh/th13_sense_grid2/fit_covar_ndfd1000ktMWyr_allsyst_th13_hie1_th13:%lf,ssth23:%lf,deltapi:%lf_nominal.root" % (th13_vals[j],ssth23_vals[i],dcp_vals[n])
				tf = ROOT.TFile(tfname)
				tree = tf.Get("fit_info")
				tree.GetEntry(0)
				chisq = tree.chisq
				th13_val = tree.fFakeDataVals[0]
				ss2th13_val = (math.sin(2*th13_val))**2
				dchisqGs[i].SetPoint(j,ss2th13_val,chisq)
				tf.Close()
		
		outf = ROOT.TFile("nonunitarity_graphs/alt_dchisq_graphs_dcp=%lf.root" % (dcp_vals[n]), "RECREATE")
		for g in dchisqGs:
			g.Write()
		outf.Close()
		print "File Made"

	dchisq_invGs = []
	for i in range(len(ssth23_vals)):
		dchisq_invGs.append(ROOT.TGraph())
		dchisq_invGs[i].SetName("dchisq_vs_trth13_th23_%lf" % ssth23_vals[i])
		dchisq_invGs[i].SetTitle(";True sin^{2}2#theta_{13};#Delta#chi^{2}")
		for j in range(len(th13_vals)):	
			tfname = "/pnfs/dune/persistent/users/jfleishh/th13_sense_grid2/fit_covar_ndfd1000ktMWyr_allsyst_th13_hie-1_th13:%lf,ssth23:%lf,deltapi:%lf_nominal.root" % (th13_vals[j],ssth23_vals[i],dcp_vals[1])
			tf = ROOT.TFile(tfname)
			tree = tf.Get("fit_info")
			tree.GetEntry(0)
			chisq = tree.chisq
			th13_val = tree.fFakeDataVals[0]
			ss2th13_val = (math.sin(2*th13_val))**2
			dchisq_invGs[i].SetPoint(j,ss2th13_val,chisq)
			tf.Close()
	outf = ROOT.TFile("nonunitarity_graphs/alt_dchisq_graphs_dcp=%lf_hie-1.root" % (dcp_vals[1]), "RECREATE")
	for g in dchisq_invGs:
		g.Write()
	outf.Close()
	print "File Made"
	print "-----END-----"
