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
ROOT.gStyle.SetTitleX(0.5)
ROOT.gStyle.SetTitleAlign(23)
ROOT.gStyle.SetLineWidth(3)
ROOT.gStyle.SetLineColor(1)
ROOT.gStyle.SetTitleSize(0.03,"t")

h = ROOT.TH1F("h","numuspec",30, 0.5, 8.0)
h_ws = ROOT.TH1F("h_ws","numuspec_ws",30, 0.5, 8.0)
h_nutau = ROOT.TH1F("h_nutau","Nutau BG",30, 0.5, 8.0)
h_NC = ROOT.TH1F("h_NC","NC BG",30, 0.5, 8.0)

datfile = open("data/spec_nu_no_3.5years.dat")

lines = datfile.readlines()

first = 1
next = 0
#Fill histograms
for line in lines:
    if "numubar_dis" in line:
        break
    if "numu_dis" in line:
        first = 0
        continue
    if (first==0):
        if ((not line.strip()) or line[0]=='-'):
            continue
        if "Total" in line:
            next = 1
            continue
        cols = line.split()
        if (next == 0):
            h.Fill(float(cols[0]),float(cols[1]))
            h_ws.Fill(float(cols[0]),float(cols[2]))
        if (next == 1):
            h_nutau.Fill(float(cols[0]),float(cols[1]))
            h_nutau.Fill(float(cols[0]),float(cols[2]))
            h_NC.Fill(float(cols[0]),float(cols[3]))
            h_NC.Fill(float(cols[0]),float(cols[4]))

h.SetLineColor(ROOT.kBlack)
h_ws.SetLineColor(ROOT.kGreen)
h_nutau.SetLineColor(ROOT.kCyan)
h_NC.SetLineColor(ROOT.kRed)

h.SetLineWidth(3)
h_ws.SetLineWidth(3)
h_nutau.SetLineWidth(3)
h_NC.SetLineWidth(3)

hs = ROOT.THStack("hs","Stacked spectrum")
hs.Add(h_NC)
hs.Add(h_nutau)
hs.Add(h_ws)
hs.Add(h)

t1 = ROOT.TPaveText(0.5,0.78,0.89,0.89,"NDC")
t1.AddText("DUNE #nu_{#mu} Disappearance")
t1.AddText("sin^{2}2#theta_{13} = 0.085")
t1.AddText("sin^{2}#theta_{23} = 0.441")
t1.SetFillColor(0)
t1.SetBorderSize(0)
t1.SetTextAlign(12)

l1 = ROOT.TLegend(0.5,0.65,0.89,0.78)
l1.AddEntry(h,"Signal #nu_{#mu} CC", "L")
l1.AddEntry(h_ws,"#bar{#nu}_{#mu} CC", "L")
l1.AddEntry(h_nutau, "(#nu_{#tau} + #bar{#nu}_{#tau}) CC", "L")
l1.AddEntry(h_NC, "NC", "L")
l1.SetFillColor(0)
l1.SetBorderSize(0)


c1 = ROOT.TCanvas("c1","c1",800,800)
c1.SetLeftMargin(0.15)
h1 = c1.DrawFrame(0.5,0.,8.,550.)
h1.GetXaxis().SetTitle("Reconstructed Energy (GeV)")
h1.GetYaxis().SetTitle("Events per 0.25 GeV")
h1.GetYaxis().SetTitleOffset(1.5)
hs.SetTitle("")
hs.Draw("same")
t1.Draw("same")
l1.Draw("same")
c1.SaveAs("plots/spec_dis_nu.png")


