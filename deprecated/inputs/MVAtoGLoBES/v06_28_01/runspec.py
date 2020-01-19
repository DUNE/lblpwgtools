#!/usr/bin/env python

import os
import math

years = 3.5

mass = 40

params_no = "-p\'0.5857,0.148,0.726,0.0,0.000075,0.002524\' "

params_io = "-p\'0.5857,0.148,0.873,0.0,0.000075,-0.002439\' "

#Normal Ordering
outfile = "data/spec_nu_no_3.5years.dat"
call = "globes -s "+params_no+" DUNE_GLoBES.glb -DNUTIME="+str(years)+" -DNUBARTIME=0 > "+outfile
print call
os.system(call) 

outfile = "data/spec_anu_no_3.5years.dat"
call = "globes -s "+params_no+" DUNE_GLoBES.glb -DNUBARTIME="+str(years)+" -DNUTIME=0 > "+outfile
print call
os.system(call) 

