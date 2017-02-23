#!/bin/env python

import os
from make_effs import *

def MakeConfigs(datadir,label):
 """Function to create GLoBES configuration given MVA selection tree input

 Args:
   datadir: path to root files with default names
   label: name of directory in lblpwgtools/inputs/MVAtoGLoBES where configs will be written"""

 directory = "../../inputs/MVAtoGLoBES/"+label
 if not os.path.exists(directory):
        os.makedirs(directory)
 os.system("cp -r ../../inputs/MVAtoGLoBES/v2.1/* "+directory+"/.")
 MakeEfficiencies(datadir)
 os.system ("mv post*.txt "+directory+"/eff/.")
 os.system ("mv *.txt "+directory+"/smr/.")

def main():
    a1 = sys.argv[1]
    a2 = sys.argv[2]
    MakeConfigs(a1,a2)

if __name__ == "__main__":
    main()

