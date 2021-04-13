locationpnfs="/pnfs/dune/persistent/users/dmendez/"
 indir="CAFAnaInputs" # output directory in pnfs
 gridScript="./FarmCAFENodeScript.sh"
 outdir="RunOptOutputs"
 walltimeCurve="--expected-walltime 24h"
 walltimeSurface="--expected-walltime 84h"

 infiles="root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/persistent/users/dmendez/CAFAnaInputs/State"

## Exposure combinations are in years, and are equivalent to the following %s
## FHC = (100, 75, 50, 25, 0)
## FHC = (  0, 25, 50, 75, 0)

 # years="05years"
 # fhc_years=(0.5 0.375 0.25 0.125 0.0)
 # rhc_years=(0.0 0.125 0.25 0.375 0.5)

 # years="1years"
 # fhc_years=(1. 0.75 0.5 0.25 0.)
 # rhc_years=(0. 0.25 0.5 0.75 1.)

 #years="2years"
 #fhc_years=(2. 1.5 1. 0.5 0.)
 #rhc_years=(0. 0.5 1. 1.5 2.)

 #years="3years"
 #fhc_years=(3. 2.25 1.5 0.75 0.)
 #rhc_years=(0. 0.75 1.5 2.25 3.)

years="7years"
years_fhc=(7. 5.25 3.5 1.75 0.)
years_rhc=(0. 1.75 3.5 5.25 7.)

 echo "This might save me some wrinkles"
 echo ""
 echo "*************************"
 echo "Sending jobs to FNAL grid"
 echo "*************************"
 echo ""


 for script in cpv mh; do
     for hierarchy in 1 -1; do
 		for det in ndfd; do
             for systs in allsyst; do
                 for pen in th13; do
                     for index in {0..5}; do

                         $gridScript -p ${outdir}/${script} $walltimeCurve --cafe-commands ${script}_joint.C ${infiles} optrun ${systs} ${det}:${years} ${pen} ${hierarchy} asimov0 ${fhc_years[$index]} ${rhc_years[$index]}
                         wait

                     done #years
                 done #pen
             done #systs
         done #det
     done #hierarchy
 done #script  