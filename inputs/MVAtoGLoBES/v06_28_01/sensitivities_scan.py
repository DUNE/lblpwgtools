#!/usr/bin/env python

import os
import math

#Set up exposure scan

#myexps = [1, 5, 10, 28, 50, 100, 200, 300, 400, 556, 600, 800, 984, 1000, 1250, 1500]
#myexps = [300, 556]
myexps = [300]

#Scale the power
power60 = 1.03
power80 = 1.07
power120 = 1.2

mass = 40

params_no = "-p\'0.5857,0.148,0.726,0.0,0.000075,0.002524\' "
paramse_no = "-E\'0.023,0.018,0.058,0.0,0.024,0.016\' "

params_io = "-p\'0.5857,0.148,0.873,0.0,0.000075,-0.002439\' "
paramse_io = "-E\'0.023,0.018,0.048,0.0,0.024,0.016\' "

paramse_noconstr = "-E\'0.023,0.0,0.0,0.0,0.024,0.0\' "

params_no_q23lo = "-p\'0.5857,0.148,0.693,0.0,0.000075,0.002524\' "
params_no_q23hi = "-p\'0.5857,0.148,0.785,0.0,0.000075,0.002524\' "

params_io_q23lo = "-p\'0.5857,0.148,0.838,0.0,0.000075,-0.002439\' "
params_io_q23hi = "-p\'0.5857,0.148,0.900,0.0,0.000075,-0.002439\' "

params_no_oscpar_dmsqlo = "-p\'0.5857,0.148,0.726,0.0,0.000075,0.002407\' "
params_no_oscpar_dmsqhi = "-p\'0.5857,0.148,0.726,0.0,0.000075,0.002643\' "

params_no_oscpar_q23lo = "-p\'0.5857,0.148,0.670,0.0,0.000075,0.002524\' "
params_no_oscpar_q23hi = "-p\'0.5857,0.148,0.922,0.0,0.000075,0.002524\' "

params_io_oscpar_q23lo = "-p\'0.5857,0.148,0.677,0.0,0.000075,-0.002439\' "
params_io_oscpar_q23hi = "-p\'0.5857,0.148,0.927,0.0,0.000075,-0.002439\' "

params_no_oscpar_q13lo = "-p\'0.5857,0.139,0.726,0.0,0.000075,0.002524\' "
params_no_oscpar_q13hi = "-p\'0.5857,0.155,0.726,0.0,0.000075,0.002524\' "

params_io_oscpar_q13lo = "-p\'0.5857,0.140,0.873,0.0,0.000075,-0.002439\' "
params_io_oscpar_q13hi = "-p\'0.5857,0.156,0.873,0.0,0.000075,-0.002439\' "

logq13lo = "-1.12"
logq13hi = "-1.03"

#Set for each config because it depends on power:
power = power80
for myexp in myexps:
    years = myexp/(power*2*mass)


#Nominal sensitivity
    outfile = "data/mva_no_exp"+str(myexp)
    call = "/lbne/u/etw2/globes/LBNE_trunk/LAr_syst/cdr_testsetup/mydis -X-1.07,-1.07 -R1,37 -Y-180,180 -x-1.5,-0.7 -y-180,180 -r40,40 "+params_no+paramse_no+" -F DUNE_GLoBES.glb -d 0.02 -DNUTIME="+str(years)+" -DNUBARTIME="+str(years)+" "+outfile+" &"
    print call
    os.system(call) 

    outfile = "data/mva_scale5_no_exp"+str(myexp)
    call = "/lbne/u/etw2/globes/LBNE_trunk/LAr_syst/cdr_testsetup/mydis -X-1.07,-1.07 -R1,100 -Y-180,180 -x-1.5,-0.7 -y-180,180 -r40,40 "+params_no+paramse_no+" -F DUNE_GLoBES_scale5.glb -d 0.02 -DNUTIME="+str(years)+" -DNUBARTIME="+str(years)+" "+outfile+" &"
    #print call
    #os.system(call) 

    outfile = "data/mva_scale10_no_exp"+str(myexp)
    call = "/lbne/u/etw2/globes/LBNE_trunk/LAr_syst/cdr_testsetup/mydis -X-1.07,-1.07 -R1,100 -Y-180,180 -x-1.5,-0.7 -y-180,180 -r40,40 "+params_no+paramse_no+" -F DUNE_GLoBES_scale10.glb -d 0.02 -DNUTIME="+str(years)+" -DNUBARTIME="+str(years)+" "+outfile+" &"
    #print call
    #os.system(call) 

    outfile = "data/mva_fast_no_exp"+str(myexp)
    call = "/lbne/u/etw2/globes/LBNE_trunk/LAr_syst/cdr_testsetup/mydis -X-1.07,-1.07 -R1,10 -Y-180,180 -x-1.5,-0.7 -y-180,180 -r40,40 "+params_no+paramse_no+" -F DUNE_GLoBES.glb -d 0.02 -DNUTIME="+str(years)+" -DNUBARTIME="+str(years)+" "+outfile+" &"
    #print call
    #os.system(call) 

    outfile = "data/mva_io_exp"+str(myexp)
    call = "/lbne/u/etw2/globes/LBNE_trunk/LAr_syst/cdr_testsetup/mydis -X-1.07,-1.07 -R1,100 -Y-180,180 -x-1.5,-0.7 -y-180,180 -r40,40 "+params_io+paramse_io+" -F DUNE_GLoBES.glb -d 0.02 -DNUTIME="+str(years)+" -DNUBARTIME="+str(years)+" "+outfile+" &"
    #print call
    #os.system(call) 

