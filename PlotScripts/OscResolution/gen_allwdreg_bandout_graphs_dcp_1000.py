import ROOT
ROOT.gROOT.SetBatch(1)
import math

def LinInterp(x1, y1, x2, y2, targy):
	m = (y2 - y1)/(x2 - x1)
	b = y1 - m*x1
	return (targy - b)/m

def FindIntersections(spline, targ_dchisq, left, right, n):
	param_vals = []
	dchisq_vals = []
	for i in range(n + 1):
		multi = float(i)/float(n)
		curr_param = left + multi*(right - left)
		param_vals.append(curr_param)
		dchisq_vals.append(spline.Eval(curr_param))
	inter_pts = []
	inter_pts.append(left)
	for i in range(n):
		#print "dchisq at i: %lf, dchisq at i + 1: %lf" %(dchisq_vals[i],dchisq_vals[i+1])
		if dchisq_vals[i] <= targ_dchisq < dchisq_vals[i+1] or dchisq_vals[i+1] <= targ_dchisq < dchisq_vals[i]:
			inter_pts.append(LinInterp(param_vals[i], dchisq_vals[i], param_vals[i+1], dchisq_vals[i+1],targ_dchisq))
	inter_pts.append(right)
	
	bands = []

	for i in range(len(inter_pts) - 1):
		if spline.Eval((inter_pts[i] + inter_pts[i+1])/2) <= targ_dchisq:
			bands.append(inter_pts[i])
			bands.append(inter_pts[i+1])
		
	print "Number of Intersections: %d" % len(bands)
	print bands
	return bands

if __name__=="__main__":
	ssth23_vals = [0.42, 0.46, 0.50, 0.54, 0.58]
	#ssth23_vals = [0.58]
	dcp_names = []
	#ss2th13_names = [1130]
	tr_dcp_vals = []
	#tr_ss2th13_vals = [0.113]
	#graph_pos = ["a","b","c","d","e","f"]
	targ_dchisqs = [1., 2.7, 9.]
	level_names = ["1sigma", "90p","3sigma"]
	for i in range(41):
		curr_dcp = -1. + (i/40.)*(2)
		tr_dcp_vals.append(curr_dcp)
		dcp_names.append(int(100*curr_dcp))
	#print ss2th13_names
	#print tr_ss2th13_vals
	for n in range(len(targ_dchisqs)):
		for i in range(len(ssth23_vals)):
			upper_top_graphs = ROOT.TMultiGraph()
			upper_bot_graphs = ROOT.TMultiGraph()
			true_top_graph = ROOT.TGraph()
			true_bot_graph = ROOT.TGraph()
			lower_top_graphs = ROOT.TMultiGraph()
			lower_bot_graphs = ROOT.TMultiGraph()
			tf = ROOT.TFile("1dscan_graphs/dcp_1000/1dscan_graphs_ssth23=%lf.root" % ssth23_vals[i])
		#	for j in range(len(graph_pos)):
		#		graphs.append(ROOT.TGraph())
		#		graphs[j].SetName("allwd_vs_trdcp_%s_%s" % (level_names[n],graph_pos[j]))
		#		graphs[j].SetTitle(";True #delta_{cp};Allowed #delta_{cp}")	
			ptind = 0
			cont_upper_band=True
			cont_lower_band=True
			curr_upper_top_graph = ROOT.TGraph()
			curr_upper_bot_graph = ROOT.TGraph()
			curr_lower_top_graph = ROOT.TGraph()
			curr_lower_bot_graph = ROOT.TGraph()
			for k in range(len(dcp_names)):
				dchisq_graph = tf.Get("dchisq_vs_dcp_%s_trdcp" % dcp_names[k])
				dchisq_spline = ROOT.TSpline3("s",dchisq_graph)
				inters = FindIntersections(dchisq_spline,targ_dchisqs[n],-1.,1.,10000)
				if len(inters) != 2 and len(inters) != 4 and len(inters) != 6:
					print "ssth23 = %lf, trdcp = %lf, %s: INCORRECT NUMBER OF INTERSECTIONS, GRAPHS WILL BE EMPTY!!" % (ssth23_vals[i],tr_dcp_vals[k],level_names[n])
					continue
				if len(inters) == 2:
					true_bot_graph.SetPoint(true_bot_graph.GetN(), tr_dcp_vals[k], inters[0])
					true_top_graph.SetPoint(true_top_graph.GetN(), tr_dcp_vals[k], inters[1])
					cont_upper_band=False
					cont_lower_band=False
				elif len(inters) == 4:
					tr_dcp_placement = 0
					for pt in inters:
						if tr_dcp_vals[k] >= pt and tr_dcp_placement < 3:
							tr_dcp_placement += 1
					print "tr_dcp_placement: %d" % tr_dcp_placement
					if tr_dcp_placement == 1:
						true_bot_graph.SetPoint(true_bot_graph.GetN(), tr_dcp_vals[k], inters[0])
						true_top_graph.SetPoint(true_top_graph.GetN(), tr_dcp_vals[k], inters[1])
						if not cont_upper_band:
							upper_top_graphs.Add(curr_upper_top_graph)
							upper_bot_graphs.Add(curr_upper_bot_graph)
							curr_upper_top_graph = ROOT.TGraph()
							curr_upper_bot_graph = ROOT.TGraph()

						curr_upper_bot_graph.SetPoint(curr_upper_bot_graph.GetN(), tr_dcp_vals[k], inters[2])
						curr_upper_top_graph.SetPoint(curr_upper_top_graph.GetN(), tr_dcp_vals[k], inters[3])
						
						cont_upper_band = True
						if lower_top_graphs.GetListOfGraphs() != ROOT.nullptr: cont_lower_band = False
					
					elif tr_dcp_placement == 3:
						true_bot_graph.SetPoint(true_bot_graph.GetN(), tr_dcp_vals[k], inters[2])
						true_top_graph.SetPoint(true_top_graph.GetN(), tr_dcp_vals[k], inters[3])
						if not cont_lower_band:
							lower_top_graphs.Add(curr_lower_top_graph)
							lower_bot_graphs.Add(curr_lower_bot_graph)
							curr_lower_top_graph = ROOT.TGraph()
							curr_lower_bot_graph = ROOT.TGraph()

						curr_lower_bot_graph.SetPoint(curr_lower_bot_graph.GetN(), tr_dcp_vals[k], inters[0])
						curr_lower_top_graph.SetPoint(curr_lower_top_graph.GetN(), tr_dcp_vals[k], inters[1])
						
						cont_lower_band = True
						if upper_top_graphs.GetListOfGraphs() != ROOT.nullptr: cont_upper_band = False
					
					else: print "INCORRECT TRUE VALUE PLACEMENT OF %d" %tr_dcp_placement
				
				elif len(inters) == 6:	
					tr_dcp_placement = 0
					for pt in inters:
						if tr_dcp_vals[k] >= pt and tr_dcp_placement < 5:
							tr_dcp_placement += 1
					print "tr_dcp_placement: %d" % tr_dcp_placement
					if not cont_lower_band:
						lower_bot_graphs.Add(curr_lower_bot_graph)
						lower_top_graphs.Add(curr_lower_top_graph)
						curr_lower_bot_graph = ROOT.TGraph()
						curr_lower_top_graph = ROOT.TGraph()
					if not cont_upper_band:
						upper_bot_graphs.Add(curr_upper_bot_graph)
						upper_top_graphs.Add(curr_upper_top_graph)
						curr_upper_bot_graph = ROOT.TGraph()
						curr_upper_top_graph = ROOT.TGraph()
					if tr_dcp_placement == 1:
						true_bot_graph.SetPoint(true_bot_graph.GetN(), tr_dcp_vals[k], inters[0])
						true_top_graph.SetPoint(true_top_graph.GetN(), tr_dcp_vals[k], inters[1])

						curr_lower_bot_graph.SetPoint(curr_lower_bot_graph.GetN(), tr_dcp_vals[k], inters[2])
						curr_lower_top_graph.SetPoint(curr_lower_top_graph.GetN(), tr_dcp_vals[k], inters[3])
						curr_upper_bot_graph.SetPoint(curr_upper_bot_graph.GetN(), tr_dcp_vals[k], inters[4])
						curr_upper_top_graph.SetPoint(curr_upper_top_graph.GetN(), tr_dcp_vals[k], inters[5])
					elif tr_dcp_placement == 3:
						true_bot_graph.SetPoint(true_bot_graph.GetN(), tr_dcp_vals[k], inters[2])
						true_top_graph.SetPoint(true_top_graph.GetN(), tr_dcp_vals[k], inters[3])

						curr_lower_bot_graph.SetPoint(curr_lower_bot_graph.GetN(), tr_dcp_vals[k], inters[0])
						curr_lower_top_graph.SetPoint(curr_lower_top_graph.GetN(), tr_dcp_vals[k], inters[1])
						curr_upper_bot_graph.SetPoint(curr_upper_bot_graph.GetN(), tr_dcp_vals[k], inters[4])
						curr_upper_top_graph.SetPoint(curr_upper_top_graph.GetN(), tr_dcp_vals[k], inters[5])
					elif tr_dcp_placement == 5:
						true_bot_graph.SetPoint(true_bot_graph.GetN(), tr_dcp_vals[k], inters[4])
						true_top_graph.SetPoint(true_top_graph.GetN(), tr_dcp_vals[k], inters[5])

						curr_lower_bot_graph.SetPoint(curr_lower_bot_graph.GetN(), tr_dcp_vals[k], inters[0])
						curr_lower_top_graph.SetPoint(curr_lower_top_graph.GetN(), tr_dcp_vals[k], inters[1])
						curr_upper_bot_graph.SetPoint(curr_upper_bot_graph.GetN(), tr_dcp_vals[k], inters[2])
						curr_upper_top_graph.SetPoint(curr_upper_top_graph.GetN(), tr_dcp_vals[k], inters[3])
					else: print "INCORRECT TRUE VALUE PLACEMENT OF %d" %tr_dcp_placement
						
					cont_lower_band=True
					cont_upper_band=True
				
				if k == len(dcp_names) - 1:
					lower_bot_graphs.Add(curr_lower_bot_graph)
					lower_top_graphs.Add(curr_lower_top_graph)
					upper_bot_graphs.Add(curr_upper_bot_graph)
					upper_top_graphs.Add(curr_upper_top_graph)
				'''if len(inters) == 2:
					graphs[0].SetPoint(ptind, tr_dcp_vals[k], inters[0])
					graphs[1].SetPoint(ptind, tr_dcp_vals[k], inters[1])
					graphs[2].SetPoint(ptind, tr_dcp_vals[k], inters[0])	
					graphs[3].SetPoint(ptind, tr_dcp_vals[k], inters[1])
					graphs[4].SetPoint(ptind, tr_dcp_vals[k], inters[0])
					graphs[5].SetPoint(ptind, tr_dcp_vals[k], inters[1])
					ptind += 1
				elif len(inters) == 4:
					graphs[0].SetPoint(ptind, tr_dcp_vals[k], inters[0])
					graphs[1].SetPoint(ptind, tr_dcp_vals[k], inters[1])
					graphs[2].SetPoint(ptind, tr_dcp_vals[k], inters[2])	
					graphs[3].SetPoint(ptind, tr_dcp_vals[k], inters[3])
					graphs[4].SetPoint(ptind, tr_dcp_vals[k], inters[2])
					graphs[5].SetPoint(ptind, tr_dcp_vals[k], inters[3])	
					ptind += 1
				elif len(inters) == 6:	
					graphs[0].SetPoint(ptind, tr_dcp_vals[k], inters[0])
					graphs[1].SetPoint(ptind, tr_dcp_vals[k], inters[1])
					graphs[2].SetPoint(ptind, tr_dcp_vals[k], inters[2])	
					graphs[3].SetPoint(ptind, tr_dcp_vals[k], inters[3])
					graphs[4].SetPoint(ptind, tr_dcp_vals[k], inters[4])
					graphs[5].SetPoint(ptind, tr_dcp_vals[k], inters[5])'''	
			
			tf.Close()

			
			lower_bot_graphs.SetName("LowerBotMG")
			lower_top_graphs.SetName("LowerTopMG")
			upper_bot_graphs.SetName("UpperBotMG")
			upper_top_graphs.SetName("UpperTopMG")
			true_bot_graph.SetName("TrueBotG")
			true_top_graph.SetName("TrueTopG")
			outf = ROOT.TFile("1dscan_graphs/dcp_1000/allwdreg_bandout_graphs_ssth23=%lf_%s.root" % (ssth23_vals[i],level_names[n]), "RECREATE")
			lower_bot_graphs.Write()
			lower_top_graphs.Write()
			upper_bot_graphs.Write()
			upper_top_graphs.Write()
			true_bot_graph.Write()
			true_top_graph.Write()
			print "---------------------------------"
			print "%s, ssth23: %lf, Graphs Written" % (level_names[n],ssth23_vals[i])
			print "---------------------------------"
			outf.Close()
	print "----END----"
