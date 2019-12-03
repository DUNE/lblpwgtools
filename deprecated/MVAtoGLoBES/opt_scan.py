#!/bin/env python

import os
from make_configs import *

def OptScan(nufile,label,seltype,scale=1.0):
 """Function to perform a scan through PID cuts to perform optimization
   and create GLoBES configs for the optimized cut value

   Currently scans cut values from 0.0 to 0.95
   Requires working installation of mgt (also in lblpwgtools repository)

 Args:
   nufile: path to nu file (assumes anu file same with nu->anu)
   label: name of directory in lblpwgtools/inputs/MVAtoGLoBES where configs will be written
   seltype: mva, cvn
   scale: Correct the nue reco energy by this factor """

 homedir = os.getcwd()
 thiscut = 0.0
 stepsize = 0.05
 bestchi2 = 0.0
 bestcut = thiscut
 bestdir = ""
 keepgoing = True
 while (thiscut<0.95 and keepgoing):
     dir = MakeConfigs(nufile,label,seltype,thiscut,scale,False)
     os.chdir(dir)
     mycmd = "mgt -h -DNUTIME=3.5046728972 -DNUBARTIME=3.5046728972 -p\'0.5857,0.148,0.726,-1.57,0.000075,0.002524\' -E\'0.023,0.018,0.058,0.0,0.024,0.016\' -C1 -T1 -P10,37 -h DUNE_GLoBES.glb data/mgt_cpv.dat"
     os.system(mycmd)
     f = open("data/mgt_cpv.dat")
     thischi2 = float(f.readlines()[0].split()[1])
     f.close()
     if thischi2>bestchi2:
         bestchi2 = thischi2
         bestcut = thiscut
         bestdir = dir
     else:
         keepgoing = False
     os.chdir(homedir)

     thiscut += stepsize

 print "With stepsize of {}, optimum cut value is {}, with sigma={:03.2f}".format(stepsize, bestcut, math.sqrt(bestchi2))
 
 os.chdir(bestdir)
 os.system("cp -r * ../.")
 os.chdir("..")
 mycmd = "mgt -h -DNUTIME=3.5046728972 -DNUBARTIME=3.5046728972 -p\'0.5857,0.148,0.726,0.0,0.000075,0.002524\' -E\'0.023,0.018,0.058,0.0,0.024,0.016\' -C1 -T1 -h DUNE_GLoBES.glb data/mgt_cpv_scan.dat"
 os.system(mycmd)
 return 1

def main():
    a1 = sys.argv[1]
    a2 = sys.argv[2]
    a3 = sys.argv[3]
    a4 = sys.argv[4]
    OptScan(a1,a2,a3,a4)

if __name__ == "__main__":
    main()


