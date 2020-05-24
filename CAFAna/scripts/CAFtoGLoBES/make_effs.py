#!/bin/env python

import sys
import math
import ROOT
from array import array
from CAFtoGLoBES import *

def MakeEfficiencies(specfile="spec_hist_forglobes.root"):
 """Function to make efficiency and smearing files in GLoBES format from CAFAna spectra

 Args:
   specfile: CAFAna spec file created by spec_for_globes.C
 History:
   ETW May 2020, based on MVAtoGLoBES but simplified to use the CAFAna output explicitly
 """

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

 nu = ROOT.TFile(specfile)

#List of variables
 efflist_nue = ["app_FHC_nue_sig", "app_FHC_nuebar_sig", "app_FHC_nue_bkg", "app_FHC_nuebar_bkg", "app_FHC_numu_bkg", "app_FHC_numubar_bkg", "app_FHC_nutau_bkg", "app_FHC_nutaubar_bkg", "app_FHC_NC_bkg", "app_FHC_aNC_bkg"]

 efflist_anue = ["app_RHC_nue_sig", "app_RHC_nuebar_sig", "app_RHC_nue_bkg", "app_RHC_nuebar_bkg", "app_RHC_numu_bkg", "app_RHC_numubar_bkg", "app_RHC_nutau_bkg", "app_RHC_nutaubar_bkg", "app_RHC_NC_bkg", "app_RHC_aNC_bkg"]

 efflist_numu = ["dis_FHC_numu_sig", "dis_FHC_numubar_sig", "dis_FHC_nutau_bkg", "dis_FHC_nutaubar_bkg", "dis_FHC_NC_bkg", "dis_FHC_aNC_bkg"]

 efflist_anumu = ["dis_RHC_numu_sig", "dis_RHC_numubar_sig", "dis_RHC_nutau_bkg", "dis_RHC_nutaubar_bkg", "dis_RHC_NC_bkg", "dis_RHC_aNC_bkg"]

 smearlist_nue = ["app_nue_sig", "app_nuebar_sig", "app_nue_bkg", "app_nuebar_bkg", "app_numu_bkg", "app_numubar_bkg", "app_nutau_bkg", "app_nutaubar_bkg", "app_NC_bkg", "app_aNC_bkg"]

 smearlist_nue_names = ["smr_app_FHC_nue_sig", "smr_app_FHC_nuebar_sig", "smr_app_FHC_nue_bkg", "smr_app_FHC_nuebar_bkg", "smr_app_FHC_numu_bkg", "smr_app_FHC_numubar_bkg", "smr_app_FHC_nutau_bkg", "smr_app_FHC_nutaubar_bkg", "smr_app_FHC_NC_bkg", "smr_app_FHC_aNC_bkg"]


 smearlist_numu = ["dis_numu_sig", "dis_numubar_sig", "dis_nutau_bkg", "dis_nutaubar_bkg", "dis_NC_bkg", "dis_aNC_bkg"]

 smearlist_numu_names = ["smr_dis_FHC_numu_sig", "smr_dis_FHC_numubar_sig", "smr_dis_FHC_nutau_bkg", "smr_dis_FHC_nutaubar_bkg", "smr_dis_FHC_NC_bkg", "smr_dis_FHC_aNC_bkg"]

 happlist_nue = []
 happlist_anue = []
 hsmearlist_nue = []

 hdislist_numu = []
 hdislist_anumu = []
 hsmearlist_numu = []

#Efficiencies

 i = 0
 for eff in efflist_nue:

  fidname = "fid_"+eff
  hall = nu.Get(fidname)
  hsel = nu.Get(eff)
  ratio = hsel.Clone()
  ratio.Divide(hall)
  ratio.SetNameTitle(eff,eff)
  happlist_nue.append(ratio.Clone())
  hall.Delete()
  hsel.Delete()
  ratio.Delete()
  i += 1

 i = 0
 for eff in efflist_anue:
  fidname = "fid_"+eff
  hall = nu.Get(fidname)
  hsel = nu.Get(eff)
  ratio = hsel.Clone()
  ratio.Divide(hall)
  ratio.SetNameTitle(eff,eff)
  happlist_anue.append(ratio.Clone())
  hall.Delete()
  hsel.Delete()
  ratio.Delete()
  i += 1

 i = 0
 for eff in efflist_numu:
  fidname = "fid_"+eff
  hall = nu.Get(fidname)
  hsel = nu.Get(eff)
  ratio = hsel.Clone()
  ratio.Divide(hall)
  ratio.SetNameTitle(eff,eff)
  hdislist_numu.append(ratio.Clone())
  hall.Delete()
  hsel.Delete()
  ratio.Delete()
  i += 1

 i = 0
 for eff in efflist_anumu:
  fidname = "fid_"+eff
  hall = nu.Get(fidname)
  hsel = nu.Get(eff)
  ratio = hsel.Clone()
  ratio.Divide(hall)
  ratio.SetNameTitle(eff,eff)
  hdislist_anumu.append(ratio.Clone())
  hall.Delete()
  hsel.Delete()
  ratio.Delete()
  i += 1
    
#Smearings
 i=0
 for smear in smearlist_nue_names:
  hsmr = nu.Get(smear)
  hsmr.SetNameTitle(smear,smear)
  hsmr.GetXaxis().SetTitle("Ereco")
  hsmr.GetYaxis().SetTitle("Etrue")
  hsmearlist_nue.append(hsmr.Clone())
  hsmr.Delete()
  i += 1

 i=0
 for smear in smearlist_numu_names:
  hsmr = nu.Get(smear)
  hsmr.SetNameTitle(smear,smear)
  hsmr.GetXaxis().SetTitle("Ereco")
  hsmr.GetYaxis().SetTitle("Etrue")
  hsmearlist_numu.append(hsmr.Clone())
  hsmr.Delete()
  i += 1
    

#Make output files and plots    
 i = 0
 for h in happlist_nue:
  HistToFile1D(h,"txtout/post_"+efflist_nue[i]+".txt","post_"+efflist_nue[i], None)
  i += 1

 i = 0
 for h in happlist_anue:
  HistToFile1D(h,"txtout/post_"+efflist_anue[i]+".txt","post_"+efflist_anue[i], None)
  i += 1

 i = 0
 for h in hdislist_numu:
  HistToFile1D(h,"txtout/post_"+efflist_numu[i]+".txt","post_"+efflist_numu[i], None)
  i += 1

 i = 0
 for h in hdislist_anumu:
  HistToFile1D(h,"txtout/post_"+efflist_anumu[i]+".txt","post_"+efflist_anumu[i], None)
  i += 1

 i = 0
 for h in hsmearlist_nue:
  HistToFile2D(h,"txtout/"+smearlist_nue[i]+".txt",smearlist_nue[i], None)
  i += 1

 i = 0
 for h in hsmearlist_numu:
  HistToFile2D(h,"txtout/"+smearlist_numu[i]+".txt",smearlist_numu[i], None)
  i += 1

 return 0

def main():
    MakeEfficiencies("spec_hist_forglobes.root")

if __name__ == "__main__":
    main()




