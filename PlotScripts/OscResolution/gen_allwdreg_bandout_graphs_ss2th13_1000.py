import ROOT
ROOT.gROOT.SetBatch(1)
import math

def LinInterp(x1, y1, x2, y2, targy):
	m = (y2 - y1)/(x2 - x1)
	b = y1 - m*x1
	return (targy - b)/m

def FindIntersections(spline, targ_dchisq, th13left, th13right, n):
	ss2th13_vals = []
	dchisq_vals = []
	for i in range(n + 1):
		multi = float(i)/float(n)
		curr_ss2th13 = th13left + multi*(th13right - th13left)
		ss2th13_vals.append(curr_ss2th13)
		dchisq_vals.append(spline.Eval(curr_ss2th13))
	inters = []
	i = 0
	for i in range(n):
		#print "dchisq at i: %lf, dchisq at i + 1: %lf" %(dchisq_vals[i],dchisq_vals[i+1])
		if dchisq_vals[i] <= targ_dchisq < dchisq_vals[i+1] or dchisq_vals[i+1] <= targ_dchisq < dchisq_vals[i]:
			inters.append(LinInterp(ss2th13_vals[i], dchisq_vals[i], ss2th13_vals[i+1], dchisq_vals[i+1],targ_dchisq))
	print "Number of Intersections: %d" % len(inters)
	print inters
	return inters

if __name__=="__main__":
	ssth23_vals = [0.42, 0.46, 0.50, 0.54, 0.58]
	#ssth23_vals = [0.58]
	ss2th13_names = []
	#ss2th13_names = [1130]
	tr_ss2th13_vals = []
	#tr_ss2th13_vals = [0.113]
	graph_pos = ["B", "MB", "MT", "T"]
	targ_dchisqs = [1., 2.7, 9.]
	level_names = ["1sigma", "90p","3sigma"]
	for i in range(21):
		curr_ss2th13 = 0.063 + (i/20.)*(0.05)
		tr_ss2th13_vals.append(curr_ss2th13)
		ss2th13_names.append(int(10000*curr_ss2th13))
	#print ss2th13_names
	#print tr_ss2th13_vals
	for n in range(len(targ_dchisqs)):
		for i in range(len(ssth23_vals)):
			graphs = []
			tf = ROOT.TFile("1dscan_graphs/ss2th13_1000/1dscan_graphs_ssth23=%lf.root" % ssth23_vals[i])
			for j in range(len(graph_pos)):
				graphs.append(ROOT.TGraph())
				graphs[j].SetName("pos_%s" % (graph_pos[j]))
				graphs[j].SetTitle(";True sin^{2}2#theta_{13};Allowed sin^{2}2#theta_{13}")
			for j in range(len(graph_pos)):	
				ptind = 0
				for k in range(len(ss2th13_names)):
					dchisq_graph = tf.Get("dchisq_vs_ss2th13_%s_trss2th13"%ss2th13_names[k])
					dchisq_spline = ROOT.TSpline3("s",dchisq_graph)
					inters = FindIntersections(dchisq_spline,targ_dchisqs[n],(math.sin(2*0.10))**2,(math.sin(2*0.22))**2,10000)
					if len(inters) == 2:
						graphs[0].SetPoint(ptind, tr_ss2th13_vals[k], inters[0])
						graphs[1].SetPoint(ptind, tr_ss2th13_vals[k], inters[1])
						graphs[2].SetPoint(ptind, tr_ss2th13_vals[k], inters[0])	
						graphs[3].SetPoint(ptind, tr_ss2th13_vals[k], inters[1])
						ptind += 1
					elif len(inters) == 4:
						graphs[0].SetPoint(ptind, tr_ss2th13_vals[k], inters[0])
						graphs[1].SetPoint(ptind, tr_ss2th13_vals[k], inters[1])
						graphs[2].SetPoint(ptind, tr_ss2th13_vals[k], inters[2])	
						graphs[3].SetPoint(ptind, tr_ss2th13_vals[k], inters[3])
						ptind += 1
					else:
						print "ssth23 = %lf, trss2th13 = %lf, INCORRECT NUMBER OF INTERSECTIONS, GRAPHS WILL BE EMPTY!!" % (ssth23_vals[i],tr_ss2th13_vals[k])
			
			tf.Close()
			outf = ROOT.TFile("1dscan_graphs/ss2th13_1000/allwdreg_bandout_graphs_ssth23=%lf_%s.root" % (ssth23_vals[i],level_names[n]), "RECREATE")
			for g in graphs:
				g.Write()
			print "%s, ssth23: %lf, Graphs Written" % (level_names[n],ssth23_vals[i])
			outf.Close()
	print "----END----"
