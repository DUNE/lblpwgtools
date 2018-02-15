This module converts the outputs from the MVA selection to GLoBES formatted inputs.

Usage for running full analysis including cut optimization:
opt_scan.py [NUFILE = path to neutrino caf file] [LABEL = directory name for output GLoBES configs] [SELTYPE = 'mva' or 'cvn'] [SCALE = scale factor for reconstructed nue energy]
Requires working installation of mgt

Usage for making full GLoBES configuration:
make_configs.py [NUFILE = path to neutrino caf file] [LABEL = directory name for output GLoBES configs] [SELTYPE = 'mva' or 'cvn'] [NUECUT = desired PID cut value for nue selection] [SCALE = scale factor for reconstructed nue energy] [FINAL = True/False is final cut value]

Places full GLoBES configurations based on trees in PATH in lblpwgtools/inputs/MVAtoGLoBES/LABEL/

TODO:

Add GLoBES smearing & efficiency plotting scripts.
