#!/bin/env python

import os
from make_effs import *

def MakeConfigs(nufile,label,seltype,nuecut,scale=1.0,final=True):
 """Function to create GLoBES configuration given MVA selection tree input

 Args:
   nufile: path to nu file (assumes anu file same with nu->anu)
   label: name of directory in lblpwgtools/inputs/MVAtoGLoBES where configs will be written
   seltype: mva, cvn
   nuecut: PID cut (accept nue events with PID variable greater than this value)
   scale: Correct the nue reco energy by this factor
   final: If final, put in top directory, if not, put in subdirectory (used for optimization scan) """

 directory = "../../inputs/MVAtoGLoBES/"+label
 if (not final=="True"):
  directory += "/nuecut"+str(nuecut)
 plotdir = directory+"/plots"
 rootdir = directory+"/root"
 datadir = directory+"/data"

 if not os.path.exists(directory):
  ok = MakeEfficiencies(nufile,seltype,nuecut,scale)
  if (ok==0): 
   print("New configs made!")
   os.makedirs(directory)
   os.system("cp ../../inputs/MVAtoGLoBES/template/DUNE_GLoBES.glb "+directory+"/.")
   os.system("cp ../../inputs/MVAtoGLoBES/template/definitions.inc "+directory+"/.")
   os.system("cp ../../inputs/MVAtoGLoBES/template/syst_list.inc "+directory+"/.")
   os.system("cp -r ../../inputs/MVAtoGLoBES/template/flux "+directory+"/.")
   os.system("cp -r ../../inputs/MVAtoGLoBES/template/xsec "+directory+"/.")
   os.system("cp -r ../../inputs/MVAtoGLoBES/template/eff "+directory+"/.")
   os.system("cp -r ../../inputs/MVAtoGLoBES/template/smr "+directory+"/.")
   os.system("mv post*.txt "+directory+"/eff/.")
   os.system("mv *.txt "+directory+"/smr/.")
   if not os.path.exists(plotdir):
    os.makedirs(plotdir)
    os.system("mv *.png "+plotdir+"/.")
   if not os.path.exists(rootdir):
    os.makedirs(rootdir)
    os.system("mv *.root "+rootdir+"/.")
   if not os.path.exists(datadir):
    os.makedirs(datadir)

  else:
   print "Something has gone wrong...not making your configs!"
 else:
  print ("Not making new configs...directory exists!")
 return directory

        

def main():
    a1 = sys.argv[1]
    a2 = sys.argv[2]
    a3 = sys.argv[3]
    a4 = sys.argv[4]
    if len(sys.argv) > 5:
     a5 = sys.argv[5]
    else:
     a5 = 1.0
    if len(sys.argv) > 6:
     a6 = sys.argv[6]
    else:
     a6 = True
    MakeConfigs(a1,a2,a3,a4,a5,a6)

if __name__ == "__main__":
    main()

