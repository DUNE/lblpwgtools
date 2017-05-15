#!/bin/env python

import os
from make_effs import *

def MakeConfigs(datadir,label,seltype):
 """Function to create GLoBES configuration given MVA selection tree input

 Args:
   datadir: path to root files with default names
   label: name of directory in lblpwgtools/inputs/MVAtoGLoBES where configs will be written
   seltype: mva, cnn """

 directory = "../../inputs/MVAtoGLoBES/"+label

 ok = MakeEfficiencies(datadir,seltype)
 if (ok==0): 
        if not os.path.exists(directory):
               os.makedirs(directory)
        os.system("cp ../../inputs/MVAtoGLoBES/v2.1/DUNE_GLoBES.glb "+directory+"/.")
        os.system("cp ../../inputs/MVAtoGLoBES/v2.1/definitions.inc "+directory+"/.")
        os.system("cp ../../inputs/MVAtoGLoBES/v2.1/syst_list.inc "+directory+"/.")
        os.system("cp -r ../../inputs/MVAtoGLoBES/v2.1/flux "+directory+"/.")
        os.system("cp -r ../../inputs/MVAtoGLoBES/v2.1/xsec "+directory+"/.")
        os.system("cp -r ../../inputs/MVAtoGLoBES/v2.1/eff "+directory+"/.")
        os.system("cp -r ../../inputs/MVAtoGLoBES/v2.1/smr "+directory+"/.")
        os.system ("mv post*.txt "+directory+"/eff/.")
        os.system ("mv *.txt "+directory+"/smr/.")
 else:
        print "Something has gone wrong...not making your configs!"

def main():
    a1 = sys.argv[1]
    a2 = sys.argv[2]
    a3 = sys.argv[3]
    MakeConfigs(a1,a2,a3)

if __name__ == "__main__":
    main()

