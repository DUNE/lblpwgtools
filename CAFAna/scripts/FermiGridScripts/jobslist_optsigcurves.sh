locationpnfs="/pnfs/dune/persistent/users/dmendez/"
indir="CAFAnaInputs" # output directory in pnfs
gridScript="./FarmCAFENodeScript.sh"
outdir="CAFAnaTests"
walltimeCurve="--expected-walltime 24h"
walltimeSurface="--expected-walltime 84h"

infiles="root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/persistent/users/dmendez/CAFAnaInputs/State"

# years="05years"
fhc_years=(0.0 0.1 0.25 0.4 0.5)
rhc_years=(0.5 0.4 0.25 0.1 0.0)

# years="1years"
# fhc_years=(0. 0.2 0.5 0.8 1.)
# rhc_years=(1. 0.8 0.5 0.2 0.)

#years="1years"
#fhc_years=(0. 0.1 0.2 0.4 0.5 0.6 0.8 0.9 1.)
#rhc_years=(1. 0.9 0.8 0.6 0.5 0.4 0.2 0.1 0.)

#years="2years"
#fhc_years=(0. 0.2 1.8 2.)
#rhc_years=(2. 1.8 0.2 0.)

#years="3years"
#fhc_years=(0. 0.3 2.7 3.)
#rhc_years=(3. 2.7 0.3 0.)

echo "This will save you some wrinkles!"
echo ""
echo "*************************"
echo "Sending jobs to FNAL grid"
echo "*************************"
echo ""


for script in cpv mh; do
    for hierarchy in 1 -1; do
		for det in fd ndfd; do
            for systs in nosyst allsyst; do
                for pen in nopen th13; do
                    for index in {0..4}; do

                        $gridScript -p ${outdir}/${script} $walltimeCurve --cafe-commands ${script}_joint.C dummy ${systs} ${det}:7year ${pen} ${hierarchy} ${fhc_years[$index]} ${rhc_years[$index]}
                        wait

                    done #years
                done #pen
            done #systs
        done #det
    done #hierarchy
done #script  
