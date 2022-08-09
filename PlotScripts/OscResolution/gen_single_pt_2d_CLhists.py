import ROOT
ROOT.gROOT.SetBatch(1)
import numpy as np
import math
import ctypes

def GetBinDist(hist, b, targ_bin_list):
	if not targ_bin_list:
		return 0
	else: 
		distances = []
		bx, by, bz = ctypes.c_int(), ctypes.c_int(), ctypes.c_int()
		hist.GetBinXYZ(b[0], bx, by, bz)
		for targ_b in targ_bin_list:
			targx, targy, targz = ctypes.c_int(),ctypes.c_int(),ctypes.c_int()
			hist.GetBinXYZ(targ_b[0],targx, targy, targz)
			distances.append(math.sqrt((bx.value - targx.value)**2 + (by.value - targy.value)**2))	
		return min(distances)
		
 
def GetMinDistInd(hist, comp_bin_list, targ_bin_list):
	min_ind = 0
	distances = []
	for b in comp_bin_list:
		distances.append(GetBinDist(hist, b, targ_bin_list))
	min_ind = np.argmin(distances)
	return min_ind


if __name__=="__main__":
	f = ROOT.TFile("single_pt_hists/2ParamPFCLs_ndfd1000_ss2th13=0.088_dmsq32=2.45_ssth23=0.58_dcp=-0.5.root", "RECREATE")
	f.Close()
	hNames = ["PFdmsq32_vs_PFss2th13","PFssth23_vs_PFss2th13","PFdcp_vs_PFss2th13","PFssth23_vs_PFdmsq32","PFdcp_vs_PFdmsq32","PFdcp_vs_PFssth23"]
	#hNames = ["PFdcp_vs_PFss2th13"]
	for i in range(len(hNames)):
		print "Loop Started"
		hf = ROOT.TFile("single_pt_hists/2ParamPFHists_ndfd1000_ss2th13=0.088_dmsq32=2.45_ssth23=0.58_dcp=-0.5.root")
		print "Passed 1"
		
		h = hf.Get(hNames[i])
		print(h)
		numpts = h.Integral()
		print "Num Pts: %d" % numpts
		l1s, l90, l3s = 0.68*numpts, 0.9*numpts, 0.997*numpts
		print "Passed 2"
		h.Draw("colz")		

		nindx, nindy = h.GetNbinsX()+2, h.GetNbinsY()+2
		hbins = []
		for j in range(nindx):
			for k in range(nindy):
				hbins.append((h.GetBin(j,k),h.GetBinContent(h.GetBin(j,k))))

		print "Passed 3" 

		hbins.sort(reverse=True, key = lambda x:x[1])
		
		cont_sum = 0
		bin_ind = 0
		l1s_cont, l90_cont, l3s_cont = 0,0,0
		while cont_sum < l1s:
			l1s_cont = hbins[bin_ind][1]
			cont_sum += hbins[bin_ind][1]
			bin_ind += 1

		while cont_sum < l90:
			l90_cont = hbins[bin_ind][1]
			cont_sum += hbins[bin_ind][1]
			bin_ind += 1

		while cont_sum < l3s:
			l3s_cont = hbins[bin_ind][1]
			cont_sum += hbins[bin_ind][1]
			bin_ind += 1
		
		print "Content levels: %f, %f, %f" % (l1s_cont, l90_cont, l3s_cont)
		
		hbins_split = []
		j = 0
		curr_cont = hbins[0][1]
		while j < len(hbins):
			bins_slist = []
			while j < len(hbins) and hbins[j][1] == curr_cont:
				bins_slist.append(hbins[j])
				j += 1
			if j < len(hbins):
				curr_cont = hbins[j][1]
			hbins_split.append(bins_slist)
		#print hbins_split
		
		count = 0		
		cont_sum = 0
		l1sbins, l90bins, l3sbins = [], [], []
		slist_ind = 0
		while cont_sum < l1s:
			curr_slist = hbins_split[slist_ind]
			if count % 5 == 0: print "l1s loop, slist_ind: %d, count: %d, ties: % d" % (slist_ind, count, len(curr_slist))
			targ_ind = 0 
			if curr_slist[0][1] <= l1s_cont:
				targ_ind = GetMinDistInd(h, curr_slist, l1sbins)
			cont_sum += curr_slist[targ_ind][1]
			l1sbins.append(curr_slist.pop(targ_ind))
			if not curr_slist:
				slist_ind += 1
			count += 1

		while cont_sum < l90:
			curr_slist = hbins_split[slist_ind]
			if count % 5 == 0: print "l90 loop, slist_ind: %d, count: %d, ties: %d" % (slist_ind, count, len(curr_slist))
			targ_ind = 0
			if curr_slist[0][1] <= l90_cont:
				targ_ind = GetMinDistInd(h, curr_slist, l1sbins + l90bins)
			cont_sum += curr_slist[targ_ind][1]
			l90bins.append(curr_slist.pop(targ_ind))
			if not curr_slist:
				slist_ind += 1
			count+=1
	
		while cont_sum < l3s:
			curr_slist = hbins_split[slist_ind]
			if count % 5 == 0: print "l3s loop, slist_ind: %d, count: %d, ties: %d, excl in: %d" % (slist_ind, count, len(curr_slist), (l3s - cont_sum)/l3s_cont)
			targ_ind = 0
			if curr_slist[0][1] <= l3s_cont:
				targ_ind = GetMinDistInd(h, curr_slist, l1sbins + l90bins + l3sbins)
			cont_sum += curr_slist[targ_ind][1]
			l3sbins.append(curr_slist.pop(targ_ind))
			if not curr_slist:
				slist_ind += 1
			count += 1
		print "Num Excluded Bins at l3s: %d" % len(hbins_split[slist_ind])
		"""cont1sl, cont90l, cont3sl = 0,0,0
		cont_sum = 0
		ind = 0
		while cont_sum < l1s:
			cont_sum += bin_contents[ind]
			cont1sl = bin_contents[ind]
			ind+=1
		
		while cont_sum < l90:
			cont_sum += bin_contents[ind]
			cont90l = bin_contents[ind]
			ind+=1
		
		while cont_sum < l3s:
			cont_sum += bin_contents[ind]
			cont3sl = bin_contents[ind]
			ind+=1"""

		print "Passed 4"

		newName = "%s_CLs" % hNames[i]
		hCL = h.Clone(newName)
		print "Passed 5"

		
		for j in range(nindx):
			for k in range(nindy):
				if h.GetBinContent(h.GetBin(j,k)) > 0:
					hCL.SetBinContent(hCL.GetBin(j,k), 0.001)
		for b in l1sbins:
			hCL.SetBinContent(b[0],3)
		for b in l90bins:
			hCL.SetBinContent(b[0],2)
		for b in l3sbins:
			hCL.SetBinContent(b[0],1)
	
		for j in range(1,nindx - 1):
			for k in range(1,nindy - 1):
				if hCL.GetBinContent(hCL.GetBin(j+1, k)) == hCL.GetBinContent(hCL.GetBin(j, k+1)) == hCL.GetBinContent(hCL.GetBin(j-1, k)) == hCL.GetBinContent(hCL.GetBin(j, k-1)) and hCL.GetBinContent(hCL.GetBin(j+1, k)) > 0:
					hCL.SetBinContent(hCL.GetBin(j,k), hCL.GetBinContent(hCL.GetBin(j+1,k)))	
		print "Passed 6"

		outf = ROOT.TFile("single_pt_hists/2ParamPFCLs_ndfd1000_ss2th13=0.088_dmsq32=2.45_ssth23=0.58_dcp=-0.5.root", "UPDATE")
		hCL.Write()
		outf.Close()
		hf.Close()
		print "LOOP END"
