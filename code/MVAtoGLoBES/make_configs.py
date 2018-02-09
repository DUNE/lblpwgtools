#!/bin/env python

import os
from make_effs import *

def MakeConfigs(nufile,label,seltype,scale=1):
 """Function to create GLoBES configuration given MVA selection tree input

 Args:
   nufile: path to nu file (assumes anu file same with nu->anu)
   label: name of directory in lblpwgtools/inputs/MVAtoGLoBES where configs will be written
   seltype: mva, cvn
   scale: Correct the nue reco energy by this factor """

 directory = "../../inputs/MVAtoGLoBES/"+label
 plotdir = directory+"/plots"
 rootdir = directory+"/root"

 ok = MakeEfficiencies(nufile,seltype,scale)
 if (ok==0): 
        if not os.path.exists(directory):
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

 else:
        print "Something has gone wrong...not making your configs!"

def main():
    a1 = sys.argv[1]
    a2 = sys.argv[2]
    a3 = sys.argv[3]
    if len(sys.argv) > 4:
     a4 = sys.argv[4]
    else:
     a4 = 1.0
    MakeConfigs(a1,a2,a3,a4)

if __name__ == "__main__":
    main()

