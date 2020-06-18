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
  print "Functions to write out GLoBES files from CAFAna input"
  print "Called from make_effs.py"
  

if __name__ == "__main__":
    main()




