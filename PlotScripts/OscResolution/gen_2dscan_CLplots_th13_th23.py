import ROOT
ROOT.gROOT.SetBatch(1)
import math
from array import array

if __name__=="__main__":
	names = ["th13_low_th23_up_troct","th13_low_th23_up_foct","th13_dyb_th23_low_troct","th13_dyb_th23_low_foct","th13_dyb_th23_up_troct","th13_dyb_th23_up_foct","th13_up_th23_low_troct", "th13_up_th23_low_foct", "th13_dyb_th23_max_troct"]
	zoomed_out_names = ["th13_low_th23_up", "th13_dyb_th23_low", "th13_dyb_th23_up","th13_up_th23_low", "th13_dyb_th23_max"]
	tr_pts = [(0.063,0.58), (0.088, 0.42), (0.088, 0.58), (0.113, 0.42), (0.088, 0.50)]
	gf = ROOT.TFile("2dscan_th13_th23_graphs/2dscan_graphs.root")
	cl_hists = []
	contours = array('d')
	contours.append(2.28)
	contours.append(4.61)
	contours.append(11.62)
	for i in range(len(names)):
		g = gf.Get("scan_2D_%s"%names[i])
		cl_hists.append(g.GetHistogram())
	i = 0
	troct_conts = []
	foct_conts = []
	for h in cl_hists:
		h.SetContour(3,contours)
		h.Draw("CONT LIST")
		ROOT.gPad.Update()
		if i % 2 == 0:
			troct_conts.append(ROOT.gROOT.GetListOfSpecials().FindObject("contours").Clone())
		else:
			foct_conts.append(ROOT.gROOT.GetListOfSpecials().FindObject("contours").Clone())
		i+=1
		#print troct_conts
		#print foct_conts
	emptyh = ROOT.TH2D()
	emptyh.SetTitle(";sin^{2}2#theta_{13};sin^{2}#theta_{23}")
	emptyh.GetXaxis().SetLimits(0.05,0.15)
	emptyh.GetYaxis().SetLimits(0.35,0.65)
	
	can = ROOT.TCanvas()	
	for i in range(len(troct_conts)):
		can.Clear()
		leg = ROOT.TLegend(0.5,0.6, 0.8, 0.8)
		emptyh.Draw()
		trcont_1s_list = troct_conts[i].At(0)
		#print trcont_1s_list.First()
		trcont_90p_list = troct_conts[i].At(1)
		trcont_3s_list = troct_conts[i].At(2)
		if i < 4:
			fcont_1_list = foct_conts[i].At(0)
			fcont_2_list = foct_conts[i].At(1)

		trcont_1s_graph = trcont_1s_list.First()
		trcont_1s_graph.SetLineColor(2)
		leg.AddEntry(trcont_1s_graph, "#Delta#chi^{2} < 2.3 (1#sigma)", "L")
		trcont_90p_graph = trcont_90p_list.First()
		trcont_90p_graph.SetLineColor(6)
		leg.AddEntry(trcont_90p_graph, "#Delta#chi^{2} < 4.6 (90%)", "L")
		trcont_3s_graph = trcont_3s_list.First()
		trcont_3s_graph.SetLineColor(4)
		leg.AddEntry(trcont_3s_graph, "#Delta#chi^{2} < 11.6 (3#sigma)", "L")
		
		trcont_1s_graph.Draw("C")
		trcont_90p_graph.Draw("C")
		trcont_3s_graph.Draw("C")
		
		if i < 4 and fcont_2_list.GetSize()>0:
			fcont_90p_graph = fcont_1_list.First()
			fcont_90p_graph.SetLineColor(6)
			fcont_90p_graph.Draw("C")
			fcont_3s_graph = fcont_2_list.First()
			fcont_3s_graph.SetLineColor(4)
			fcont_3s_graph.Draw("C")
		elif i < 4:
			fcont_3s_graph= fcont_1_list.First()
			fcont_3s_graph.SetLineColor(4)
			fcont_3s_graph.Draw("C")
		tr_g = ROOT.TGraph()
		tr_g.SetPoint(0,tr_pts[i][0], tr_pts[i][1])
		tr_g.SetMarkerStyle(43)
		tr_g.SetMarkerSize(1.5)
		tr_g.Draw("P")
		leg.Draw()
		can.Print("2dscan_th13_th23_CLplots/2dscan_CLplots_%s.png"% zoomed_out_names[i])
		can.Print("2dscan_th13_th23_CLplots/2dscan_CLplots_%s.pdf"% zoomed_out_names[i])
	print "---END---"
