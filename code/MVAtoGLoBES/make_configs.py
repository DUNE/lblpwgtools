#!/bin/env python

import os
from make_effs import *

def MakeConfigs(nuefile,label,seltype):
 """Function to create GLoBES configuration given MVA selection tree input

 Args:
   datadir: path to root files with default names
   label: name of directory in lblpwgtools/inputs/MVAtoGLoBES where configs will be written
   seltype: mva, cvn """

 directory = "../../inputs/MVAtoGLoBES/"+label
 plotdir = directory+"/plots"

 ok = MakeEfficiencies(nuefile,seltype)
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
 else:
        print "Something has gone wrong...not making your configs!"

def main():
    a1 = sys.argv[1]
    a2 = sys.argv[2]
    a3 = sys.argv[3]
    MakeConfigs(a1,a2,a3)

if __name__ == "__main__":
    main()

