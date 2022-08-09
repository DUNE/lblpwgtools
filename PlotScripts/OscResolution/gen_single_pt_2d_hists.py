import ROOT
ROOT.gROOT.SetBatch(1)
import numpy as np
import sys
import math

#-------------Post Fit Two Parameter Plots, Plotting PF xParam on x-axis and PF yParam on y-axis---------
# Parameter Indices:
# 0 - th13
# 1 - dmsq32
# 2 - ssth23
# 3 dcp
def twoParamLoop(xParam, yParam, tree):
	pf_xVals, pf_yVals = [],[]
	n = 0
	for entry in tree:
		if n % 5000 == 0:
			print "Getting entry %d" % n
		n += 1
		pf_xVal = entry.fPostFitValues[xParam]
		pf_yVal = entry.fPostFitValues[yParam]
		if xParam == 0: #Take ss2th13
			pf_xNew = (math.sin(2*pf_xVal))**2
			pf_xVal = pf_xNew
		if yParam == 0:
			pf_yNew = (math.sin(2*pf_yVal))**2
			pf_yVal = pf_yNew
		if xParam == 3: #Modulo Pi
			while pf_xVal > 1.: pf_xVal -= 2.
			while pf_xVal < -1.: pf_xVal += 2.
		if yParam == 3:
			while pf_yVal > 1.: pf_yVal -= 2.
			while pf_yVal < -1.: pf_yVal += 2.
		pf_xVals.append(pf_xVal)
		pf_yVals.append(pf_yVal)
	return pf_xVals, pf_yVals
if __name__ == "__main__":
	hists = []
	paramNames = ["ss2th13","dmsq32","ssth23","dcp"]
	mean_x, std_x, mean_y, std_y = 0,0,0,0
	rangesx = [[0.05, 0.15], [2.36, 2.56],[0.35, 0.65],[-1.,1.]]
	rangesy = [[0.05, 0.15], [2.36, 2.56],[0.35, 0.65],[-1.,1.]]
	axisLabels = ["sin^{2}2#theta_{13}","#Delta m^{2}_{32}","sin^{2}#theta_{23}","#delta_{cp}"]
	for i in range(4): #Plotting relations between four parameters
		for j in range(i+1,4): #Create 6 plots like T2K
			#input file tf:
			tf = ROOT.TFile("/pnfs/dune/persistent/physicsgroups/dunelbl/callumw/fixed_osc_toy_throws/toy_throws_ndfd1000_hie1_nopen_deltapi-0.5.root")
			tree = tf.Get("fit_info")
			pf_xVals, pf_yVals = twoParamLoop(i,j,tree)
			tf.Close()
			mean_x, std_x = np.mean(pf_xVals), np.std(pf_xVals) #Calculate mean/stdev to set histogram range
			mean_y, std_y = np.mean(pf_yVals), np.std(pf_yVals)
			#Make histogram
			h = ROOT.TH2D("PF%s_vs_PF%s"%(paramNames[j],paramNames[i]),";Post Fit %s;Post Fit %s" % (axisLabels[i],axisLabels[j]), 100,rangesx[i][0],rangesx[i][1],100, rangesy[j][0], rangesy[j][1])
			for n in range(len(pf_xVals)): #Fill Plot
				if n % 1000 == 0:
					print "Filling entry %d" % (n)
				h.Fill(pf_xVals[n],pf_yVals[n])
			hists.append(h.Clone()) #Put histogram in list
			print hists
	

	#Write histograms to file
	pltfile = ROOT.TFile("single_pt_hists/2ParamPFHists_ndfd1000_ss2th13=0.088_dmsq32=2.45_ssth23=0.58_dcp=-0.5.root","RECREATE")
	print "Writing Histograms"
	print hists
	print "Num hists: %d" % (len(hists))
	for h in hists:
		h.Write()
	pltfile.Close()
print "------END------"
sys.exit()
