import ROOT
ROOT.gROOT.SetBatch(1)

def dcpLoop(tree):
	pf_dcp_vals = []
	i = 0
	for entry in tree:
		if i % 5000 == 0: print "Throws Tree: Getting Entry %d" % i
		curr_pf_dcp = entry.fPostFitValues[3]
		while curr_pf_dcp > 1:
			curr_pf_dcp -= 2
		while curr_pf_dcp < -1:
			curr_pf_dcp += 2
		pf_dcp_vals.append(curr_pf_dcp)
		i += 1
	return pf_dcp_vals

if __name__=="__main__":
	throws_tf =  ROOT.TFile("/pnfs/dune/persistent/physicsgroups/dunelbl/callumw/fixed_osc_toy_throws/toy_throws_ndfd100_hie1_nopen_deltapi-0.25.root")
	throws_tree = throws_tf.Get("fit_info")
	pf_dcp_vals = dcpLoop(throws_tree)
	throws_hist = ROOT.TH1D("throws_hist",";Post Fit #delta_{CP};Bin Frequency Probability", 100,-1.,1.)
	i = 0
	for val in pf_dcp_vals:
		if i % 5000 == 0: print "Throws Hist: Filling Entry %d" % i
		throws_hist.Fill(val)
		i+=1
	print "Throws Hist: %d Entries Filled" % i
	throws_hist.Scale(1./throws_hist.Integral())
	
	asimov_gf = ROOT.TFile("1dscan_graphs/dcp_100/1dscan_graphs_ssth23=0.580000.root")
	asimov_graph = asimov_gf.Get("dchisq_vs_dcp_-25_trdcp")
	asimov_graph.SetName("asimov_graph")
	asimov_graph.SetTitle(";Scan #delta_{CP};#Delta#chi^{2}")
	
	outf = ROOT.TFile("asimov_throws_comp_hists/dcp_singlept_asimovscan_throwshist_ndfd100_hie1_th13=dyb_th23=0.58_dcp=-0.25.root", "RECREATE")
	throws_hist.Write()
	asimov_graph.Write()
	outf.Close()
	throws_tf.Close()
	asimov_gf.Close()
	print "----END----"
	
