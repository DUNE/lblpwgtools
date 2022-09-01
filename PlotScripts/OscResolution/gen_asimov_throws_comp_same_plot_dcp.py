import ROOT
ROOT.gROOT.SetBatch(1)
import array
import ctypes

def filldiff(up,down):
    n = up.GetN()
    diffgraph = ROOT.TGraph(2*n)
    i = 0
    xup = ctypes.c_double(-9.9)
    yup = ctypes.c_double(-9.9)
    xlo = ctypes.c_double(-9.9)
    ylo = ctypes.c_double(-9.9)
    while i<n:
        up.GetPoint(i,xup,yup)
        down.GetPoint(n-i-1,xlo,ylo)
        diffgraph.SetPoint(i,xup.value,yup.value)
        diffgraph.SetPoint(n+i,xlo.value,ylo.value)
        i += 1
    return diffgraph

if __name__=="__main__":
	tf = ROOT.TFile("asimov_throws_comp_hists/dcp_singlept_asimovscan_throwshist_ndfd100_hie1_th13=dyb_th23=0.58_dcp=-0.25.root")
	thr = tf.Get("throws_hist")
	asm = tf.Get("asimov_graph")
	
	thr.Scale(1. / thr.Integral() )
	
	c = ROOT.TCanvas()

	asmspl = ROOT.TSpline3("spline", asm)
	ymax = 0.
	prev = asmspl.Eval(-1.)
	sig1, sig90p, sig3 = [],[],[]
	sig1.append(-1.)
	sig90p.append(-1.)
	sig3.append(-1.)
	for i in range(2001):
		dcp = i*0.001 - 1.
		y = asmspl.Eval(dcp)
		if y > ymax: ymax = y
		if (prev < 1. and y > 1.) or (prev > 1. and y < 1.):
			sig1.append( dcp-0.001 + (1.-prev)*0.001/(y-prev) )
		if (prev < 2.7 and y > 2.7) or (prev > 2.7 and y < 2.7):
			sig90p.append( dcp-0.001 + (2.7-prev)*0.001/(y-prev) )
		if (prev < 9. and y > 9.) or (prev > 9. and y < 9.):
			sig3.append( dcp-0.001 + (9.-prev)*0.001/(y-prev) )
		prev = y
	sig1.append(1.)
	sig90p.append(1.)
	sig3.append(1.)	

	print sig1
	print sig90p
	print sig3
		
	thrcp = thr.Clone("throws_hist_cp")	
	totfrac = 0.
	sig1_throws, sig90p_throws, sig3_throws = [], [], []
	sig1_binedges, sig90p_binedges, sig3_binedges = [], [], []
	fill1s, fill90p, fill3s = True, True, True
	while totfrac < 0.999:
		maxb = thrcp.GetMaximumBin()
		maxcont = thrcp.GetBinContent(maxb)
		if totfrac + maxcont < 0.68:
			sig1_binedges.append(thrcp.GetBinLowEdge(maxb))
			sig1_binedges.append(thrcp.GetBinLowEdge(maxb+1))
		elif fill1s: 
			needfrac = (totfrac + maxcont - 0.68)/maxcont
			if thrcp.GetBinLowEdge(maxb) in sig1_binedges:
				sig1_binedges.append(thrcp.GetBinLowEdge(maxb))
				sig1_binedges.append(thrcp.GetBinLowEdge(maxb) + needfrac*thrcp.GetBinWidth(maxb))
				print "IN"
			else:
				sig1_binedges.append(thrcp.GetBinLowEdge(maxb+1))
				sig1_binedges.append(thrcp.GetBinLowEdge(maxb) + (1.-needfrac)*thrcp.GetBinWidth(maxb)) 
				print "OUT"
			fill1s = False	
		if totfrac + maxcont < 0.90:
			sig90p_binedges.append(thrcp.GetBinLowEdge(maxb))
			sig90p_binedges.append(thrcp.GetBinLowEdge(maxb + 1))
		elif fill90p: 
			needfrac = (totfrac + maxcont - 0.90)/maxcont
			if thrcp.GetBinLowEdge(maxb) in sig90p_binedges:
				sig90p_binedges.append(thrcp.GetBinLowEdge(maxb))
				sig90p_binedges.append(thrcp.GetBinLowEdge(maxb) + needfrac*thrcp.GetBinWidth(maxb))
				print "IN"
			else:
				sig90p_binedges.append(thrcp.GetBinLowEdge(maxb+1))
				sig90p_binedges.append(thrcp.GetBinLowEdge(maxb) + (1.-needfrac)*thrcp.GetBinWidth(maxb)) 
				print "OUT"
			fill90p = False		
		if totfrac + maxcont < 0.997:
			sig3_binedges.append(thrcp.GetBinLowEdge(maxb))
			sig3_binedges.append(thrcp.GetBinLowEdge(maxb+1))
		elif fill3s: 
			needfrac = (totfrac + maxcont - 0.997)/maxcont
			if thrcp.GetBinLowEdge(maxb) in sig3_binedges:
				sig3_binedges.append(thrcp.GetBinLowEdge(maxb))
				sig3_binedges.append(thrcp.GetBinLowEdge(maxb) + needfrac*thrcp.GetBinWidth(maxb))
				print "IN"
			elif thrcp.GetBinLowEdge(maxb) < sig3_binedges[0]:
				sig3_binedges.append(thrcp.GetBinLowEdge(maxb+1))
				sig3_binedges.append(thrcp.GetBinLowEdge(maxb) + (1.-needfrac)*thrcp.GetBinWidth(maxb)) 
				print "OUT"
			fill3s = False
		
		totfrac += maxcont
		thrcp.SetBinContent(maxb, 0.)
		sig1_binedges.sort()
		sig90p_binedges.sort()
		sig3_binedges.sort()
	
	#print sig1_binedges
	print sig90p_binedges
	#print sig3_binedges
	
	sig1_bins, sig90p_bins, sig3_bins = [array.array('d')], [array.array('d')], [array.array('d')]
	sig1_bins[-1].append(sig1_binedges[0])
	i = 1
	while i < len(sig1_binedges) - 1:
		sig1_bins[-1].append(sig1_binedges[i])
		if sig1_binedges[i+1] != sig1_binedges[i]:
			sig1_bins.append(array.array('d'))
			sig1_bins[-1].append(sig1_binedges[i+1])
		i+=2
	sig1_bins[-1].append(sig1_binedges[-1])
	sig90p_bins[-1].append(sig90p_binedges[0])
	i = 1
	while i < len(sig90p_binedges) - 1:
		sig90p_bins[-1].append(sig90p_binedges[i])
		if sig90p_binedges[i+1] != sig90p_binedges[i]:
			sig90p_bins.append(array.array('d'))
			sig90p_bins[-1].append(sig90p_binedges[i+1])
		i+=2
	sig90p_bins[-1].append(sig90p_binedges[-1])
	sig3_bins[-1].append(sig3_binedges[0])
	i = 1
	while i < len(sig3_binedges) - 1:
		sig3_bins[-1].append(sig3_binedges[i])
		if sig3_binedges[i+1] != sig3_binedges[i]:
			sig3_bins.append(array.array('d'))
			sig3_bins[-1].append(sig3_binedges[i+1])
		i+=2
	sig3_bins[-1].append(sig3_binedges[-1])
	
	#print sig1_bins
	print sig90p_bins
	#print sig3_bins

	leftmax = ymax
	rightmax = thr.GetMaximum()
	scale = leftmax/rightmax

	asm.Draw("AC")
	asm.SetTitle(";#delta_{CP}/#pi;Asimov #Delta#chi^{2}")
	asm.GetXaxis().CenterTitle()
	asm.GetXaxis().SetRangeUser(-1.,1.)
	asm.GetYaxis().SetRangeUser(0.,1.3*leftmax)
	asm.Draw("AC")
	

	thr.Scale(scale)
	thr.SetLineColor(ROOT.kRed)
	thr.Draw("hist same")
	#thr.SetTitle(";#delta_{CP}/#pi;Fraction of Throws Per 0.01")
	#thr.GetXaxis().SetRangeUser(-1.,1.)
	#thr.GetYaxis().SetRangeUser(0.,1.1*rightmax)	

	leg = ROOT.TLegend(0.2,0.72, 0.7,0.82)
	leg.SetNColumns(2)
	
	addentry = True
	sig90p_stack = ROOT.THStack()
	for binlist in sig90p_bins:
		h = ROOT.TH1D()
		h.SetBins(len(binlist) - 1, binlist)
		for i in range(1, h.GetNbinsX()+1):
			h.SetBinContent(i,thr.GetBinContent(thr.FindBin(h.GetBinLowEdge(i)+0.002)))
		h.SetFillColorAlpha(ROOT.kRed-9,1.)
		h.SetLineColor(ROOT.kRed-9)
		if addentry:
			leg.AddEntry(h, "90% of throws", "f")
			addentry = False
		sig90p_stack.Add(h)
	sig90p_stack.Draw("SAME F NOSTACK HIST")	

	addentry = True
	sig1_stack = ROOT.THStack()
	for binlist in sig1_bins:
		h = ROOT.TH1D()
		h.SetBins(len(binlist)-1, binlist)
		for i in range(1, h.GetNbinsX()+1):
			h.SetBinContent(i,thr.GetBinContent(thr.FindBin(h.GetBinLowEdge(i)+0.002)))
		h.SetFillColorAlpha(ROOT.kRed-7,1.)
		h.SetLineColor(ROOT.kRed-7)
		if addentry:
			leg.AddEntry(h, "68% of throws", "f")
			addentry = False
		sig1_stack.Add(h)
	sig1_stack.Draw("SAME F NOSTACK HIST")

	bands = []
	addentry = True
	for i in range(len(sig90p)-1):
		if asmspl.Eval((sig90p[i] + sig90p[i+1])/2) <= 2.7:
			print asmspl.Eval((sig90p[i] + sig90p[i+1])/2) 
			g = ROOT.TGraph()
			g.SetPoint(0,sig90p[i],asmspl.Eval(sig90p[i]))
			for j in range(asm.GetN()):
				xpt = ctypes.c_double(-9.9)
				ypt = ctypes.c_double(-9.9)
				asm.GetPoint(j,xpt,ypt)
				if xpt.value >= sig90p[i] and xpt.value <= sig90p[i+1]:
					print xpt.value
					g.SetPoint(g.GetN(),xpt.value, ypt.value)
			g.SetPoint(g.GetN(),sig90p[i+1], asmspl.Eval(sig90p[i+1]))
			#if sig90p[i] > -1.:
			#	asmcp.InsertPointBefore(1,sig90p[i], 2.7)
			#if sig90p[i+1] < 1.:
			#	asmcp.SetPoint(asmcp.GetN(), sig90p[i+1], 2.7)
			bot = ROOT.TGraph()
			for j in range(g.GetN()):
				xpt = ctypes.c_double(-9.9)
				ypt = ctypes.c_double(-9.9)
				g.GetPoint(j, xpt, ypt)
				bot.SetPoint(j, xpt.value, 0)
			sig90p_band = filldiff(g, bot)
			sig90p_band.SetFillColorAlpha(12, .7)
			if addentry:
				leg.AddEntry(sig90p_band, "Asimov #Delta#chi^{2} < 2.7", "f")
				addentry = False
			bands.append(sig90p_band)

	addentry = True
	for i in range(len(sig1)-1):
		if asmspl.Eval((sig1[i] + sig1[i+1])/2) <= 1.:
			print asmspl.Eval((sig1[i]+sig1[i+1])/2)
			print sig1[i]
			print sig1[i+1]
			g = ROOT.TGraph()
			g.SetPoint(0, sig1[i], asmspl.Eval(sig1[i]))
			for j in range(asm.GetN()):
				xpt = ctypes.c_double(-9.9)
				ypt = ctypes.c_double(-9.9)
				asm.GetPoint(j, xpt, ypt)
				if xpt.value >= sig1[i] and xpt.value <= sig1[i+1]:
					print xpt.value
					g.SetPoint(g.GetN(), xpt.value, ypt.value)
			g.SetPoint(g.GetN(),sig1[i+1], asmspl.Eval(sig1[i+1]))
			#if sig1[i] > -1.:
			#	asmcp.InsertPointBefore(1,sig1[i], 1.)
			#if sig1[i+1] < 1.:
			#	asmcp.SetPoint(asmcp.GetN(), sig1[i+1], 1.)
			bot = ROOT.TGraph()
			for j in range(g.GetN()):
				xpt = ctypes.c_double(-9.9)
				ypt = ctypes.c_double(-9.9)
				g.GetPoint(j, xpt, ypt)
				bot.SetPoint(j, xpt.value, 0)
			sig1_band = filldiff(g, bot)
			sig1_band.SetFillColorAlpha(1, .7)
			if addentry:
				leg.AddEntry(sig1_band, "Asimov #Delta#chi^{2} < 1", "f")
				addentry = False
			bands.append(sig1_band)
	
	for band in bands:
		band.Draw("F SAME")
	asm.Draw("C")
	thr.Draw("SAME HIST")
	
	axis = ROOT.TGaxis(1.,0.,1., 1.3*leftmax, 0., 1.3*rightmax, 510, "+L")
	axis.SetLineColor(ROOT.kRed)
	axis.SetLabelColor(ROOT.kRed)
	axis.SetTitle("Fraction of throws per 0.02")
	axis.SetTitleColor(ROOT.kRed)
	axis.SetTitleOffset(1.2)
	axis.Draw()
	
	leg.Draw()

	c.RedrawAxis()
	c.Print("asimov_throws_comp_plots/dcp_asimov_throws_comp_ndfd100_hie1_th13=dyb_th23=0.58_dcp=-0.25.png")
	c.Print("asimov_throws_comp_plots/dcp_asimov_throws_comp_ndfd100_hie1_th13=dyb_th23=0.58_dcp=-0.25.pdf")

