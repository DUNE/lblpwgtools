###################################################################################
## very simple script to look for files within a directory                       ##
## sends a job to the grid with the appropiate arguments if a file doesn't exist ##
##                                                                               ##
## assumes that all the files in the directory are complete or uncorrupted       ##
## thus remove_bad_files.sh is supposed to be ran beforehand                     ##
###################################################################################

location="/my_long_path/" # input and output path in pnfs
directory="output_directory" # output directory in pnfs
gridScript="./FarmCAFENodeScript.sh"
walltimeCurve="--expected-walltime 24h"
walltimeSurface="--expected-walltime 84h"

bin="OneBin"
lobin="onebin"
infiles="root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/StandardState/State"$bin # full path to the prediction files including their preffix
totalyears_tag="05years"
fhc_years=(0.0 0.05 0.1 0.2 0.25 0.3 0.4 0.45 0.5) # number of years
rhc_years=(0.5 0.45 0.4 0.3 0.25 0.2 0.1 0.05 0.0) # number of years
fhc_tag=(0 5 10 20 25 30 40 45 50) # name tag for number of fhc years
rhc_tag=(50 45 40 30 25 20 10 5 0) # name tag number of rhc years


echo "This will save you some wrinkles!"
echo ""
echo "*************************"
echo "Sending jobs to FNAL grid"
echo "*************************"
echo ""

for script in cpv mh octant; do
    for hierarchy in 1 -1; do
    	hier="nh"
    	if [ hierarchy == -1 ]; then
    		hier="ih"
    	fi

		for penalty in nopen th13; do
		    for index in {0..8}; do
				for det in fd ndfd; do

					fout_long=$locationpnfs${directory}/${script}_curves_${lobin}/files/$script$years${lobin}_allsyst_${det}_${penalty}_asimov0_${hier}__fhc${fhc[$index]}_rhc${rhc[$index]}.root
					fout_short=$script$years${lobin}_allsyst_${det}_${penalty}_asimov0_${hier}__fhc${fhc[$index]}_rhc${rhc[$index]}.root
				    if [ ! -f "$fout_long" ]; then
				    	echo "$fout_short doesnt exist. sending job to the grid."
						echo "${script} $walltimeCurve ${script}.C $infiles $script${totyears_tag} allsyst ${det}:7year ${penalty} ${hierarchy} asimov0 ${fhc_years[$index]} ${rhc_years[$index]}"
						$gridScript -p ${directory}/${script}_curves_$lobin $walltimeCurve --cafe-commands ${script}.C $infiles $script$totyears_tag$lobin allsyst ${det}:7year $penalty $hierarchy asimov0 ${fhc_years[$index]} ${rhc_years[$index]}
						wait
					fi
				done #detector
		    done #index
		done #penalty
    done #hierarchy
done #script



for script in cpv mh octant; do
    for hierarchy in 1 -1; do
    	hier="nh"
    	if [ hierarchy == -1 ]; then
    		hier="ih"
    	fi

		for index in {0..8}; do
		    for det in fd ndfd; do

				fout_long=$locationpnfs${directory}/${script}_curves_${lobin}/files/$script$years${lobin}_nosyst_${det}_nopen_asimov0_${hier}__fhc${fhc[$index]}_rhc${rhc[$index]}.root
				fout_short=$script$years${lobin}_nosyst_${det}_nopen_asimov0_${hier}__fhc${fhc[$index]}_rhc${rhc[$index]}.root
			    if [ ! -f "$fout_long" ]; then
			    	echo "$fout_long doesnt exist. sending job to the grid."
					echo "${script} $walltimeCurve ${script}.C $infiles ${script}${totyears_tag} nosyst ${det}:7year nopen $hierarchy asimov0 ${fhc_years[$index]} ${rhc_years[$index]}"
					$gridScript -p ${directory}/${script}_curves_$lobin $walltimeCurve --cafe-commands ${script}.C $infiles $script$years$lobin nosyst ${det}:7year nopen $hierarchy asimov0 ${fhc_years[$index]} ${rhc_years[$index]}
					wait
				fi
		    done #detector
        done #index
    done #hierarchy
done #script  
