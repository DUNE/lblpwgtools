import ROOT
ROOT.gROOT.SetBatch(1)
import math

def GetJCP(ss2th12, th13, ssth23, dcp):
	th12 = math.asin(math.sqrt(ss2th12))/2
	th23 = math.asin(math.sqrt(ssth23))
	s13 = math.sin(th13)
	cs13 = math.cos(th13)**2
	s12 = math.sin(th12)
	c12 = math.cos(th12)
	s23 = math.sin(th23)
	c23 = math.cos(th23)
	return s13*cs13*s12*c12*s23*c23*math.sin(math.pi*dcp)

def JCPLoop(tree):
	pf_jcpVals = []
	i = 0
	for entry in tree:
		if i % 5000 == 0:
			print "Getting  Entry %d" % i
		entry_ss2th12 = entry.fPostFitValues[5]
		entry_th13 = entry.fPostFitValues[0]
		entry_ssth23 = entry.fPostFitValues[2]
		entry_dcp = entry.fPostFitValues[3]
		entry_jcp = GetJCP(entry_ss2th12, entry_th13, entry_ssth23, entry_dcp)
		pf_jcpVals.append(entry_jcp)
		i += 1

	return pf_jcpVals
if __name__=="__main__":
	h100 = ROOT.TH1D("ndfd100_jcp_freq_hist",";J #equiv s_{13}c^{2}_{12}s_{12}c_{12}s_{23}c_{23}sin#delta_{CP};Bin Frequency Probability", 100, -0.045, 0.045)
	tf100 = ROOT.TFile("/pnfs/dune/persistent/physicsgroups/dunelbl/callumw/fixed_osc_toy_throws/toy_throws_ndfd100_hie1_nopen_deltapi-0.25.root")
	tree100 = tf100.Get("fit_info")
	pf_jcpVals100 = JCPLoop(tree100)
	i = 0
	for val in pf_jcpVals100:
		if i % 5000 == 0:
			print "Filling Entry %d" % i
		h100.Fill(val)
		i+=1
	print "%d entries filled" % (i)
	tf100.Close()
	print "h100 prenormalization: %f total" % h100.Integral()
	h100.Scale(1./h100.Integral())
	print "h100 postnormalization: %f total" % h100.Integral()
	
	h1000 = ROOT.TH1D("ndfd1000_jcp_freq_hist",";J #equiv s_{13}c^{2}_{12}s_{12}c_{12}s_{23}c_{23}sin#delta_{CP};Bin Frequency Probability", 100, -0.045, 0.045)
	tf1000 = ROOT.TFile("/pnfs/dune/persistent/physicsgroups/dunelbl/callumw/fixed_osc_toy_throws/toy_throws_ndfd1000_hie1_nopen_deltapi-0.25.root")
	tree1000 = tf1000.Get("fit_info")
	pf_jcpVals1000 = JCPLoop(tree1000)
	i = 0
	for val in pf_jcpVals1000:
		if i % 1000 == 0:
			print "Filling Entry %d" % i
		h1000.Fill(val)
		i+=1
	print "%d entries filled" % (i) 
	tf1000.Close()
	print "h1000 prenormalization: %f total" % h1000.Integral()
	h1000.Scale(1./h1000.Integral())
	print "h1000 postnormalization: %f total" % h1000.Integral()
	
	outf = ROOT.TFile("jcp_hists/jcp_1d_hists_dcp=-0.25.root", "RECREATE")
	h100.Write()
	h1000.Write()
	outf.Close()
	
	print "----END----"
