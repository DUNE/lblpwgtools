#!/bin/env python

import sys
import math
import ROOT
from array import array
from MVAtoGLoBES import *

def MakeEfficiencies(datadir,seltype):
 """Function to make efficiency and smearing files in GLoBES format from MVA selection tree

 Args:
   datadir: path to root files with default names
   seltype: selections available in MVASelection tree: mva, cvn """
   


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
 ROOT.gStyle.SetTitleSize(0.06,"t")

 nuefile = datadir+"nuetest.root"
 numufile = datadir+"numutest.root"
 anuefile = datadir+"anuetest.root"
 anumufile = datadir+"anumutest.root"

 nue = ROOT.TFile(nuefile)
 numu = ROOT.TFile(numufile)
 anue = ROOT.TFile(anuefile)
 anumu = ROOT.TFile(anumufile)

#Need these for any analysis
 efflist_nue = ["app_FHC_nue_sig", "app_FHC_nuebar_sig", "app_FHC_nue_bkg", "app_FHC_nuebar_bkg", "app_FHC_numu_bkg", "app_FHC_numubar_bkg", "app_FHC_nutau_bkg", "app_FHC_nutaubar_bkg", "app_FHC_NC_bkg", "app_FHC_aNC_bkg"]

 efflist_anue = ["app_RHC_nue_sig", "app_RHC_nuebar_sig", "app_RHC_nue_bkg", "app_RHC_nuebar_bkg", "app_RHC_numu_bkg", "app_RHC_numubar_bkg", "app_RHC_nutau_bkg", "app_RHC_nutaubar_bkg", "app_RHC_NC_bkg", "app_RHC_aNC_bkg"]

 efflist_numu = ["dis_FHC_numu_sig", "dis_FHC_numubar_sig", "dis_FHC_nutau_bkg", "dis_FHC_nutaubar_bkg", "dis_FHC_NC_bkg", "dis_FHC_aNC_bkg"]

 efflist_anumu = ["dis_RHC_numu_sig", "dis_RHC_numubar_sig", "dis_RHC_nutau_bkg", "dis_RHC_nutaubar_bkg", "dis_RHC_NC_bkg", "dis_RHC_aNC_bkg"]

 smearlist_nue = ["app_nue_sig", "app_nuebar_sig", "app_nue_bkg", "app_nuebar_bkg", "app_numu_bkg", "app_numubar_bkg", "app_nutau_bkg", "app_nutaubar_bkg", "app_NC_bkg", "app_aNC_bkg"]

 smearlist_numu = ["dis_numu_sig", "dis_numubar_sig", "dis_nutau_bkg", "dis_nutaubar_bkg", "dis_NC_bkg", "dis_aNC_bkg"]

 nue_MVA = nue.Get("mvaselect/MVASelection")
 numu_MVA = numu.Get("mvaselect/MVASelection")
 anue_MVA = anue.Get("mvaselect/MVASelection")
 anumu_MVA = anumu.Get("mvaselect/MVASelection")
 
 nueboth_MVA = ROOT.TChain("mvaselect/MVASelection")
 nueboth_MVA.Add(nuefile)
 nueboth_MVA.Add(anuefile)
    
 numuboth_MVA = ROOT.TChain("mvaselect/MVASelection")
 numuboth_MVA.Add(numufile)
 numuboth_MVA.Add(anumufile)

#Make common fiducial cut based on truth information
 fidcut = "abs(nuvtxx_truth)<310 && abs(nuvtxy_truth<550) && nuvtxz_truth>50 && nuvtxz_truth<1244 &&"

 if (seltype == "mva"):
     mva_select_cut = 0.8
     selcut_numu = fidcut+"mvaresult>"+str(mva_select_cut)+" && "
     selcut_nue = fidcut+"mvaresult>"+str(mva_select_cut)+ "&& "

 elif (seltype == "cvn"):
     selcut_numu = fidcut+"numuCVN>0.5 && "
     selcut_nue = fidcut+"nueCVN>0.7 && "

 else:
     print "Selection type must be mva or cvn"
     return -99

 cutlist_nue = ["ccnc==0 && neu==12 && beamPdg!=12",   # nueapp
                "ccnc==0 && neu==-12 && beamPdg!=-12",  # anueapp
                "ccnc==0 && neu==12 && beamPdg==12",   # nuebgapp
                "ccnc==0 && neu==-12 && beamPdg==-12",  # anuebgapp            
                "ccnc==0 && neu==14",   # numubgapp
                "ccnc==0 && neu==-14",  # anumubgapp
                "ccnc==0 && neu==16",   # nutaubgapp
                "ccnc==0 && neu==-16",  # anutaubgapp
                "ccnc==1 && neu>0",              # NC neutrino
                "ccnc==1 && neu<0"]              # NC antineutrino

 cutlist_numu = ["ccnc==0 && neu==14", #numudis
                 "ccnc==0 && neu==-14", #anumudis
                 "ccnc==0 && neu==16", #nutaubgdis
                 "ccnc==0 && neu==-16", #anutaubgdis
                 "ccnc==1 && neu>0",    #NC neutrino
                 "ccnc==1 && neu<0"]    #NC antineutrino

 happlist_nue = []
 happlist_anue = []
 hsmearlist_nue = []

 hdislist_numu = []
 hdislist_anumu = []
 hsmearlist_numu = []

 prebins=array('d',[0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1, 1.125, 1.25, 1.375, 1.5, 1.625, 1.75, 1.875, 2, 2.125, 2.25, 2.375, 2.5, 2.625, 2.75, 2.875, 3, 3.125, 3.25, 3.375, 3.5, 3.625, 3.75, 3.875, 4, 4.125, 4.25, 4.375, 4.5, 4.625, 4.75, 4.875, 5, 5.125, 5.25, 5.375, 5.5, 5.625, 5.75, 5.875, 6, 6.125, 6.25, 6.375, 6.5, 6.625, 6.75, 6.875, 7, 7.125, 7.25, 7.375, 7.5, 7.625, 7.75, 7.875, 8, 8.125, 8.25, 8.375, 8.5, 8.625, 8.75, 8.875, 9, 9.125, 9.25, 9.375, 9.5, 9.625, 9.75, 9.875, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110])
 nbins_pre = len(prebins)-1

 postbins=array('d',[0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1, 1.125, 1.25, 1.375, 1.5, 1.625, 1.75, 1.875, 2, 2.125, 2.25, 2.375, 2.5, 2.625, 2.75, 2.875, 3, 3.125, 3.25, 3.375, 3.5, 3.625, 3.75, 3.875, 4, 4.125, 4.25, 4.375, 4.5, 4.625, 4.75, 4.875, 5, 5.125, 5.25, 5.375, 5.5, 5.625, 5.75, 5.875, 6, 6.125, 6.25, 6.375, 6.5, 6.625, 6.75, 6.875, 7, 7.125, 7.25, 7.375, 7.5, 7.625, 7.75, 7.875, 8, 9, 10, 12, 14, 16, 18, 20])
 n = len(postbins)-1

#Efficiencies

 i = 0
 for eff in efflist_nue:
     thisfidcut = fidcut+cutlist_nue[i]
     thisselcut = selcut_nue+cutlist_nue[i]
     hall = ROOT.TH1D("hall","hall",n,postbins)
     hsel = ROOT.TH1D("hsel","hsel",n,postbins)
     nue_MVA.Draw("Ev_reco >> hall", thisfidcut)
     nue_MVA.Draw("Ev_reco >> hsel", thisselcut)
     hall = ROOT.gDirectory.FindObject("hall")
     hsel = ROOT.gDirectory.FindObject("hsel")
     ratio = hsel.Clone()
     ratio.Divide(hall)
     ratio.SetNameTitle(eff,eff)
     happlist_nue.append(ratio.Clone())
     hall.Delete()
     hsel.Delete()
     i += 1

 i = 0
 for eff in efflist_anue:
     thisfidcut = fidcut+cutlist_nue[i]
     thisselcut = selcut_nue+cutlist_nue[i]
     hall = ROOT.TH1D("hall","hall",n,postbins)
     hsel = ROOT.TH1D("hsel","hsel",n,postbins)
     anue_MVA.Draw("Ev_reco >> hall", thisfidcut)
     anue_MVA.Draw("Ev_reco >> hsel", thisselcut)
     hall = ROOT.gDirectory.FindObject("hall")
     hsel = ROOT.gDirectory.FindObject("hsel")
     ratio = hsel.Clone()
     ratio.Divide(hall)
     ratio.SetNameTitle(eff,eff)
     happlist_anue.append(ratio.Clone())
     hall.Delete()
     hsel.Delete()
     i += 1

 i = 0
 for eff in efflist_numu:
     thisfidcut = fidcut+cutlist_numu[i]
     thisselcut = selcut_numu+cutlist_numu[i]
     hall = ROOT.TH1D("hall","hall",n,postbins)
     hsel = ROOT.TH1D("hsel","hsel",n,postbins)
     numu_MVA.Draw("Ev_reco >> hall", thisfidcut)
     numu_MVA.Draw("Ev_reco >> hsel", thisselcut)
     hall = ROOT.gDirectory.FindObject("hall")
     hsel = ROOT.gDirectory.FindObject("hsel")
     ratio = hsel.Clone()
     ratio.Divide(hall)
     ratio.SetNameTitle(eff,eff)
     hdislist_numu.append(ratio.Clone())
     hall.Delete()
     hsel.Delete()
     i += 1

 i = 0
 for eff in efflist_anumu:
     thisfidcut = fidcut+cutlist_numu[i]
     thisselcut = selcut_numu+cutlist_numu[i]
     hall = ROOT.TH1D("hall","hall",n,postbins)
     hsel = ROOT.TH1D("hsel","hsel",n,postbins)
     anumu_MVA.Draw("Ev_reco >> hall", thisfidcut)
     anumu_MVA.Draw("Ev_reco >> hsel", thisselcut)
     hall = ROOT.gDirectory.FindObject("hall")
     hsel = ROOT.gDirectory.FindObject("hsel")
     ratio = hsel.Clone()
     ratio.Divide(hall)
     ratio.SetNameTitle(eff,eff)
     hdislist_anumu.append(ratio.Clone())
     hall.Delete()
     hsel.Delete()
     i += 1
    
#Smearings
 i=0
 for smear in smearlist_nue:
     thisfidcut = fidcut+cutlist_nue[i]
     hsmr = ROOT.TH2D("hsmr","hsmr",n,postbins,nbins_pre,prebins)    
     nueboth_MVA.Draw("Ev:Ev_reco >> hsmr", thisfidcut)
     hsmr = ROOT.gDirectory.FindObject("hsmr")
     hsmr.SetNameTitle(smear,smear)
     hsmr.GetXaxis().SetTitle("Ereco")
     hsmr.GetYaxis().SetTitle("Etrue")
     hsmearlist_nue.append(hsmr.Clone())
     hsmr.Delete()
     i += 1

 i=0
 for smear in smearlist_numu:
     thisfidcut = fidcut+cutlist_numu[i]
     hsmr = ROOT.TH2D("hsmr","hsmr",n,postbins,nbins_pre,prebins)    
     numuboth_MVA.Draw("Ev:Ev_reco >> hsmr", thisfidcut)
     hsmr = ROOT.gDirectory.FindObject("hsmr")
     hsmr.SetNameTitle(smear,smear)
     hsmr.GetXaxis().SetTitle("Ereco")
     hsmr.GetYaxis().SetTitle("Etrue")
     hsmearlist_numu.append(hsmr.Clone())
     hsmr.Delete()
     i += 1
    

#Make output files and plots    
 hout = ROOT.TFile("mvatoglobes_effs.root","RECREATE")
 d1 = ROOT.TCanvas("d1","d1",1600,1600)
 d1.Divide(5,2)
 i = 0
 for h in happlist_nue:
     d1.cd(i+1)
     h.GetXaxis().SetRangeUser(0.5,10.)
     h.Draw()
     h.Write()
     HistToFile1D(h,"post_"+efflist_nue[i]+".txt","post_"+efflist_nue[i], None)
     i += 1
 d1.SaveAs("nue_eff.png")

 d2 = ROOT.TCanvas("d2","d2",1600,1600)
 d2.Divide(5,2)
 i = 0
 for h in happlist_anue:
     d2.cd(i+1)
     h.GetXaxis().SetRangeUser(0.5,10.)
     h.Draw()
     h.Write()
     HistToFile1D(h,"post_"+efflist_anue[i]+".txt","post_"+efflist_anue[i], None)
     i += 1
 d2.SaveAs("anue_eff.png")

 d3 = ROOT.TCanvas("d3","d3",1600,1600)
 d3.Divide(2,3)
 i = 0
 for h in hdislist_numu:
     d3.cd(i+1)
     h.GetXaxis().SetRangeUser(0.5,10.)
     h.Draw()
     h.Write()
     HistToFile1D(h,"post_"+efflist_numu[i]+".txt","post_"+efflist_numu[i], None)
     i += 1
 d3.SaveAs("numu_eff.png")

 d4 = ROOT.TCanvas("d4","d4",1600,1600)
 d4.Divide(2,3)
 i = 0
 for h in hdislist_anumu:
     d4.cd(i+1)
     h.GetXaxis().SetRangeUser(0.5,10.)
     h.Draw()
     h.Write()
     HistToFile1D(h,"post_"+efflist_anumu[i]+".txt","post_"+efflist_anumu[i], None)
     i += 1
 d4.SaveAs("anumu_eff.png")

 hout.Close()

 d5 = ROOT.TCanvas("d5","d5",1600,1600)
 d5.Divide(5,2)
 i = 0
 for h in hsmearlist_nue:
     h.GetXaxis().SetRangeUser(0.5,10.)
     h.GetYaxis().SetRangeUser(0.5,10.)
     d5.cd(i+1)
     h.Draw("colz")
     HistToFile2D(h,smearlist_nue[i]+".txt",smearlist_nue[i], None)
     i += 1
 d5.SaveAs("nue_smear.png")

 d6 = ROOT.TCanvas("d6","d6",1600,1600)
 d6.Divide(2,3)
 i = 0
 for h in hsmearlist_numu:
     h.GetXaxis().SetRangeUser(0.5,10.)
     h.GetYaxis().SetRangeUser(0.5,10.)    
     d6.cd(i+1)
     h.Draw("colz")
     HistToFile2D(h,smearlist_numu[i]+".txt",smearlist_numu[i], None)
     i += 1
 d6.SaveAs("numu_smear.png")

 return 0

def main():
    MakeEfficiencies("/data0/lbne/etw/dunemva/","mva")

if __name__ == "__main__":
    main()




