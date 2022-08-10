============================================================PlotScripts/OscResolution=========================================================
Compiled and edited by Jeremy Fleishhacker
This README contains information about the scripts used to produce the graph and histogram files for the "Prospects for Precision Measurements
..." paper. It subdivides the scripts by plot in the paper, and includes all of the information needed to run them.

BEFORE RUNNING ANY SCRIPTS:
run . build_directories.sh to build all subdirectories required for script input and output locally

------------------------------------------------------------Single Point Throws Plots---------------------------------------------------------
ORDER OF EXECUTION: python gen_single_pt_2d_hists.py
		    python gen_single_pt_2d_CLhists.py
		    python gen_single_pt_2d_plots.py

	gen_single_pt_2d_hists.py
		Generates raw TH2D histograms of 4D parameter space projected onto 6 2D histograms. The 4 parameters that are plotted
		against each other are ss2th13, dmsq32, ssth23, dcp.
		Histogram names: "PF[yParam]_vs_PF[xParam]"
		Inputs files from: /pnfs/dune/persistent/physicsgroups/dunelbl/callumw/fixed_osc_toy_throws/
			 	   /dune/data/users/jfleishh/fixed_osc_toy_throws/	
		Outputs files at:  ./single_pt_hists/
		NOTE: Requires numpy to run
		NOTE: Script can handle one input and output file at once. All input files will be in above directory, but input file name 
		      must be changed in script manually. Output file name must also be changed manually.

	gen_single_pt_2d_CLhists.py
		Generates 6 TH2d histograms as above with the 68%, 90%, and 99.7% confidence levels filled in. The 4 parameters that are 
		plotted against each other are ss2th13, dmsq32, ssth23, dcp.
		Histogram names: "PF[yParam]_vs_PF[xParam]_CLs"
		Inputs files from: ./single_pt_hists/	
		Outputs files at:  ./single_pt_hists/
		NOTE: Requires numpy to run
		NOTE: Script can handle one input and output file at once. All input files will be in above directory, but input file name 
		      must be changed in script manually. Output file name must also be changed manually.

	gen_single_pt_2d_plots.py
		Plotting script that produces a PNG plot file for each of the TH2Ds in a single file produced by either of the scripts above.
		File names: "[histogram_name].png"
		Inputs files from: ./single_pt_hists/	
		Outputs files at:  ./2ParamPFPlots_ndfd[exposure]_ss2th13=[ss2th13_val]_dmsq32=[dmsq32_val]_ssth23=[ssth23_val]_dcp=[dcp_val]/
		NOTE: Can handle one input file at once. Input file and output directory names must be manually changed.
----------------------------------------------------------------------------------------------------------------------------------------------
