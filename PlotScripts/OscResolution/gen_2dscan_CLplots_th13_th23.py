import ROOT
ROOT.gROOT.SetBatch(1)
import math
from array import array

if __name__=="__main__":
	names = ["th13_low_th23_up_troct","th13_low_th23_up_foct","th13_dyb_th23_low_troct","th13_dyb_th23_low_foct","th13_dyb_th23_up_troct","th13_dyb_th23_up_foct","th13_up_th23_low_troct"]
	zoomed_out_names = ["th13_low_th23_up", "th13_dyb_th23_low", "th13_dyb_th23_up","th13_up_th23_low"]
	gf = ROOT.TFile("2dscan_th13_th23_graphs/2dscan_graphs.root")
	cl_hists = []
	contours = array('d')
	contours.append(1)
	contours.append(2.7)
	contours.append(9)
	combo_can = ROOT.TCanvas()
	emptyh = ROOT.TH2D()
	emptyh.SetTitle(";sin^{2}2#theta_{13};sin^{2}#theta_{23}")
	emptyh.GetXaxis().SetLimits(0.05,0.15)
	emptyh.GetYaxis().SetLimits(0.35,0.65)
	combo_can.cd(0)
	emptyh.Draw()
	for i in range(len(names)):
		g = gf.Get("scan_2D_%s"%names[i])
		cl_hists.append(g.GetHistogram())
	#gf.Close()
	i = 0
	j = 0
	zoomed_out_can = ROOT.TCanvas()
	for h in cl_hists:
		can = ROOT.TCanvas()
		if i % 2 == 0: 
			zoomed_out_can = ROOT.TCanvas()
			zoomed_out_can.cd(0)
			emptyh.Draw()
		can.cd(0)
		h.SetContour(3,contours)
		palette = array('i')
		palette.append(2)
		palette.append(6)
		palette.append(4)
		ROOT.gStyle.SetPalette(3,palette)
		h.Draw("cont1") 
		#can.Print("2D_th13_th23_hres_scan_plots/scan_2D_%s_CLs.png"%names[i])
		zoomed_out_can.cd(0)
		h.Draw("cont1 same")
		if i % 2 == 1 or i == 6:
			zoomed_out_can.Print("2dscan_th13_th23_CLplots/2dscan_CLplots_%s.png"% zoomed_out_names[j])
			zoomed_out_can.Print("2dscan_th13_th23_CLplots/2dscan_CLplots_%s.pdf"% zoomed_out_names[j])
			j+=1
		combo_can.cd(0)
		h.Draw("cont1 same")
		i+=1
	#combo_can.Print("2D_th13_th23_hres_scan_plots/scan_2D_combo_CLs.png")
	print "---END---"
