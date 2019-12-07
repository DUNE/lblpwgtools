#!/bin/env python
import os,sys,string
import ROOT
from ROOT import TCanvas, TPad, TFormula, TF1, TPaveLabel, TH1F, TH2F, TFile, TChain, TGraph, TBox, TLegend
from array import array



def DrawToFile1D(tree, var, addFilter, txtfile, binsarray, name="test",tfilenew=None):
  """Function to export a 1d histogram to a text file in the efficiency array GLoBES format
  
  Args:
    tree: input ttree to query
    var: variable string to plot (should be 2d)
    addFilter: filter string to pass to draw command
    txtfile: text file to place output into
    binsarray: binning (bin edges in the X (post-smearing or reco) axis
    name: label for smearing matrix
    tfilenew: root file to write histrogram into"""
  
  h1=TH1F(name,name,len(binsarray)-1,binsarray)
  tree.Draw(var+">>"+name,addFilter,"goff")
  h1.SetTitle(name)

  HistToFile1D(h1,txtfile,name,tfilenew)


def HistToFile1D(hist, txtfile, name, tfilenew=None):
  """write 1d hist to output text file formatted as a GLoBES smearing matrix
  
  Args:
    hist: histogram to export
    txtfile: text file to place output into
    name: label for smearing matrix
    tfilenew: root file to write histrogram into  """
    
  with open(txtfile, 'w') as f:
    f.write("%%%s = {" %name)
    for i in range(1,hist.GetNbinsX()+1):
      #print i, hist.GetBinCenter(i)
      f.write("%s %.06f"%("," if i!=1 else "",hist.GetBinContent(i)))
    f.write("}\n")

  
def DrawToFile2D(tree, var, addFilter, txtfile, binsarrayX, binsarrayY, name="test", tfilenew=None):
  """Function to export a 2d matrix to a text file in the smearing matrix GLoBES format
  
  Args:
    tree: input ttree to query
    var: variable string to plot (should be 2d)
    addFilter: filter string to pass to draw command
    txtfile: text file to place output into
    binsarrayX: binning (bin edges in the X (post-smearing or reco) axis
    binsarrayY: binning (bin edges in the Y (pre-smearing or true) axis
    name: label for smearing matrix
    tfilenew: root file to write histrogram into"""
  
  h1=TH2F(name,name,len(binsarrayX)-1,binsarrayX,len(binsarrayY)-1,binsarrayY)
  tree.Draw(var+">>"+name,addFilter,"colz goff")
  h1.SetTitle(name)
  
  HistToFile2D(h1,txtfile,name,tfilenew)

def HistToFile2D(hist,txtfile,name, tfilenew=None):
  """write 2d hist to output text file formatted as a GLoBES smearing matrix
  
  Args:
    hist: histogram to export
    txtfile: text file to place output into
    name: label for smearing matrix
    tfilenew: root file to write histrogram into """ 
    
  with open(txtfile, 'w') as f:
    f.write("energy(#%s)<\n@energy =\n" % name)
    for i in range(1,hist.GetNbinsX()+1):
      f.write("{%d,%d," % (0,hist.GetNbinsY()-1))
      for j in range(1,hist.GetNbinsY()+1):
        rowsum=0
        #print i,j,hist.GetXaxis().GetBinCenter(i),hist.GetYaxis().GetBinCenter(j)
        #get sum of row to normalize row to unity
        for k in range(0,hist.GetNbinsX()+2):
          rowsum+=hist.GetBinContent(k,j)
        f.write("%s %.06f"%("," if j!=1 else "",0 if rowsum==0 else hist.GetBinContent(i,j)/rowsum))           
      f.write("}%s\n" % (":" if i<hist.GetNbinsX() else ";"))
    f.write(">")
 

def main():
  
  infile="/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v2.1/numutest.root"
  treename="mvaselect/MVASelection"
  outrootfile="test.root"
  
  
  postbins=array('d',[0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1, 1.125, 1.25, 1.375, 1.5, 1.625, 1.75, 1.875, 2, 2.125, 2.25, 2.375, 2.5, 2.625, 2.75, 2.875, 3, 3.125, 3.25, 3.375, 3.5, 3.625, 3.75, 3.875, 4, 4.125, 4.25, 4.375, 4.5, 4.625, 4.75, 4.875, 5, 5.125, 5.25, 5.375, 5.5, 5.625, 5.75, 5.875, 6, 6.125, 6.25, 6.375, 6.5, 6.625, 6.75, 6.875, 7, 7.125, 7.25, 7.375, 7.5, 7.625, 7.75, 7.875, 8, 9, 10, 12, 14, 16, 18, 20])
  prebins=array('d',[0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1, 1.125, 1.25, 1.375, 1.5, 1.625, 1.75, 1.875, 2, 2.125, 2.25, 2.375, 2.5, 2.625, 2.75, 2.875, 3, 3.125, 3.25, 3.375, 3.5, 3.625, 3.75, 3.875, 4, 4.125, 4.25, 4.375, 4.5, 4.625, 4.75, 4.875, 5, 5.125, 5.25, 5.375, 5.5, 5.625, 5.75, 5.875, 6, 6.125, 6.25, 6.375, 6.5, 6.625, 6.75, 6.875, 7, 7.125, 7.25, 7.375, 7.5, 7.625, 7.75, 7.875, 8, 8.125, 8.25, 8.375, 8.5, 8.625, 8.75, 8.875, 9, 9.125, 9.25, 9.375, 9.5, 9.625, 9.75, 9.875, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110])


  print "Opening file:", infile 
  f = ROOT.TFile.Open(infile)
  
  print "Opening tree:", treename 
  tree = f.Get(treename)
  
  print "Creating root file:", outrootfile
  tfilenew = ROOT.TFile.Open(outrootfile,"recreate")
  
  #DrawToFile1D(tree, "Ev/Ev_reco","mvaresult>0 && Ev<10 && Ev_reco<10 && neu==12", "test.txt" ,postbins, "test", tfilenew)
  DrawToFile2D(tree, "Ev_reco:Ev","mvaresult>0 && Ev<10 && Ev_reco<10 && neu==12", "test2d.txt", postbins, prebins, "test", tfilenew)

  tfilenew.Close()
  

if __name__ == "__main__":
    main()




