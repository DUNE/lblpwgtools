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
	#ssth23_vals = [0.42, 0.46, 0.50, 0.54, 0.58]
	#ssth23_vals = [0.58]
	ssth23_names = []
	#ss2th13_names = [1130]
	tr_ssth23_vals = []
	#tr_ss2th13_vals = [0.113]
	graph_pos = ["B", "MB", "MT", "T"]
	targ_dchisqs = [1., 2.7, 9.]
	level_names = ["1sigma", "90p","3sigma"]
	for i in range(21):
		curr_ssth23 = 0.4 + (i/20.)*(0.2)
		tr_ssth23_vals.append(curr_ssth23)
		ssth23_names.append(int(100*curr_ssth23))
	#print ss2th13_names
	#print tr_ss2th13_vals
	for n in range(len(targ_dchisqs)):
		for i in range(1):
			graphs = []
			tf = ROOT.TFile("1dscan_graphs/ssth23_1000/1dscan_graphs.root")
			for j in range(len(graph_pos)):
				graphs.append(ROOT.TGraph())
				graphs[j].SetName("pos_%s" % (graph_pos[j]))
				graphs[j].SetTitle(";True sin^{2}#theta_{23};Allowed sin^{2}#theta_{23}")
			for j in range(len(graph_pos)):	
				ptind = 0
				for k in range(len(ssth23_names)):
					dchisq_graph = tf.Get("dchisq_vs_ssth23_%s_trssth23" % ssth23_names[k])
					dchisq_spline = ROOT.TSpline3("s",dchisq_graph)
					inters = FindIntersections(dchisq_spline,targ_dchisqs[n],0.35,0.65,10000)
					if len(inters) == 2:
						graphs[0].SetPoint(ptind, tr_ssth23_vals[k], inters[0])
						graphs[1].SetPoint(ptind, tr_ssth23_vals[k], inters[1])
						graphs[2].SetPoint(ptind, tr_ssth23_vals[k], inters[0])	
						graphs[3].SetPoint(ptind, tr_ssth23_vals[k], inters[1])
						ptind += 1
					elif len(inters) == 4:
						graphs[0].SetPoint(ptind, tr_ssth23_vals[k], inters[0])
						graphs[1].SetPoint(ptind, tr_ssth23_vals[k], inters[1])
						graphs[2].SetPoint(ptind, tr_ssth23_vals[k], inters[2])	
						graphs[3].SetPoint(ptind, tr_ssth23_vals[k], inters[3])
						ptind += 1
					else:
						print "trssth23 = %lf, INCORRECT NUMBER OF INTERSECTIONS, GRAPHS WILL BE EMPTY!!" % (tr_ssth23_vals[k])
			
			tf.Close()
			outf = ROOT.TFile("1dscan_graphs/ssth23_1000/allwdreg_bandout_graphs_%s.root" % (level_names[n]), "RECREATE")
			for g in graphs:
				g.Write()
			print "%s, Graphs Written" % (level_names[n])
			outf.Close()
	print "----END----"
