#!/usr/bin/env python

import sys
import math
import ROOT
from array import array

ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit()
ROOT.gStyle.SetCanvasColor(0)
ROOT.gStyle.SetTitleFillColor(0)
ROOT.gStyle.SetTitleBorderSize(0)
ROOT.gStyle.SetFrameBorderMode(0)
ROOT.gStyle.SetMarkerStyle(20)

datafile = sys.argv[1]

f = open(datafile,'r')
lines = f.readlines()

mind = 9999
ming = 9999
mhresult = {}
cpvresult = {}
first = 1
for line in lines:
    if line[0] == 'C':
        if not first:
            mhresult[dcp] = ming
            cpvresult[dcp] = mind
            ming = 9999
            mind = 9999
        first = 0
        cols = line.split()
        dcp = float(cols[2])
    if ((line[0] == 'G') and (line[1] == '-')):
        cols = line.split()
        chi2 = float(cols[1])
        ming = min(ming,chi2)
    if (line[0] == 'D'):
        cols = line.split()
        chi2 = float(cols[1])
        mind = min(mind,chi2)

mhresult[dcp] = ming
cpvresult[dcp] = mind

dcpval = []
cpvsig = []
mhsig = []
for dcp in sorted(mhresult.keys()):
    dcpval.append(float(dcp)/180)
    result = cpvresult[dcp]
    if result > 0:
        cpvsig.append(math.sqrt(result))
        #cpvsig.append(result)
    else:
        cpvsig.append(0.0)
    result = mhresult[dcp]
    if result > 0:
        mhsig.append(math.sqrt(result))
        #mhsig.append(result)
    else:
        mhsig.append(0.0)


dcpval = array('d',dcpval)
mhsig = array('d',mhsig)
cpvsig = array('d',cpvsig)
n = len(dcpval)
cpvout = ROOT.TGraph(n,dcpval,cpvsig)
cpvout.SetName("CPVSig")
mhout = ROOT.TGraph(n,dcpval,mhsig)
mhout.SetName("MHSig")

label = datafile[5:]
label = label.strip(".dat")
outfile = "root/sens_"+label+".root"
rootout = ROOT.TFile(outfile,"recreate")
cpvout.Write()
mhout.Write()
rootout.Close()

