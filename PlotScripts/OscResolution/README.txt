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
		Output File Names: 2ParamPFHists_ndfd[exp]_ss2th13=[ss2th13_val]_dmsq32=[dmsq32_val]_ssth23=[ssth23_val]_dcp=[dcp_val].root
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
		Output File Names: 2ParamPFCLs_ndfd[exp]_ss2th13=[ss2th13_val]_dmsq32=[dmsq32_val]_ssth23=[ssth23_val]_dcp=[dcp_val].root
		Inputs files from: ./single_pt_hists/	
		Outputs files at:  ./single_pt_hists/
		NOTE: Requires numpy to run
		NOTE: Script can handle one input and output file at once. All input files will be in above directory, but input file name 
		      must be changed in script manually. Output file name must also be changed manually.

	gen_single_pt_2d_plots.py
		Plotting script that produces a PNG plot file for each of the TH2Ds in a single file produced by either of the scripts above.
		Output File Names: [histogram_name].png
		Inputs files from: ./single_pt_hists/	
		Outputs files at:  ./2ParamPFPlots_ndfd[exp]_ss2th13=[ss2th13_val]_dmsq32=[dmsq32_val]_ssth23=[ssth23_val]_dcp=[dcp_val]/
		NOTE: Can handle one input file at once. Input file and output directory names must be manually changed.
--------------------------------------------------------------Nonunitarity Tension Plots------------------------------------------------------
ORDERS OF EXECUTION: ______________MAIN PLOTS______________
		     python gen_main_nonunitarity_graphs.py
		     python gen_main_nonunitarity_plots.py
		     ________________ALT PLOTS_______________
		     python gen_alt_nonunitarity_graphs.py
		     python gen_alt_dcp_nonunitarity_plots.py
		     python gen_alt_hie_nonunitarity_plots.py

	gen_main_nonunitarity_graphs.py
		Generates dchisq vs ss2th13 TGraphs for 5 ssth23 values with dmsq32 and dcp at their nu-fit 4.0 points. Dchisq is just the 
		chisq of the global fit with the reactor th13 penalty applied at the ss2th13 true abscissa point. 21 ss2th13 abscissa points
		used. Also generates global best fit vs true ss2th13 TGraphs for the same values of ssth23.
		Graph Names: "dchisq_vs_trth13_th23_[ssth23_val]"
			     "bfth13_vs_trth13_th23_[ssth23_val]"
		Output File Names: main_dchisq_graphs.root
		Inputs files from: /pnfs/dune/persistent/physicsgroups/dunelbl/picker24/contrib/jfleishh/main_nonunitarity_scan/
				   /pnfs/dune/persistent/users/jfleishh/th13_sense_grid/
		Outputs files at:  ./nonunitarity_graphs/
		
	gen_main_nonunitarity_plots.py
		Puts 5 dchisq vs ss2th13 TGraphs described above on one plot and prints as png and pdf. Puts 5 best fit vs true ss2th13
		TGraphs described above on one plot and prints as png and pdf.
		Output File Names: main_dchisq_plot.png
			    	   main_dchisq_plot.pdf
			    	   main_bfth13_vs_trth13_plot.png
			    	   main_bfth13_vs_trth13_plot.pdf
		Inputs files from: ./nonunitarity_graphs/
		Outputs files at:  ./nonunitarity_plots/

	gen_alt_nonunitarity_graphs.py
		Generates dchisq vs ss2th13 TGraphs for 5 ssth23 values with dmsq32 at nu-fit 4.0 point and dcp at -Pi/2, 0, and Pi/2. Also 
		makes 5 TGraphs for dcp at 0 in the inverted ordering (hie-1). Dchisq is just the chisq of the global fit with the reactor 
		th13 penalty applied at the ss2th13 true abscissa point. 21 ss2th13 abscissa points used.
		Graph Names: "dchisq_vs_trth13_th23_[ssth23_val]"
		Output File Names: alt_dchisq_graphs_dcp=[dcp_val].root
				   alt_dchisq_graphs_dcp=0.000000_hie-1.root
		Inputs files from: /pnfs/dune/persistent/physicsgroups/dunelbl/picker24/contrib/jfleishh/alt_nonunitarity_scan/
				   /pnfs/dune/persistent/users/jfleishh/th13_sense_grid2/
		Outputs files at:  ./nonunitarity_graphs/
		
	gen_alt_dcp_nonunitarity_plots.py
		Puts 5 dchisq vs ss2th13 TGraphs described above on one plot for each alternative value of dcp and prints as png and pdf.
		Output File Names: alt_dchisq_plot_dcp=[dcp_val].png
			    	   alt_dchisq_plot_dcp=[dcp_val].pdf	    	  
		Inputs files from: ./nonunitarity_graphs/
		Outputs files at:  ./nonunitarity_plots/

	gen_alt_hie_nonunitarity_plots.py
		Puts 5 dchisq vs ss2th13 TGraphs described above on one plot for dcp = 0 and inverted ordering and prints as png and pdf.
		Output File Names: alt_dchisq_plot_dcp=0.000000_hie-1.png
			    	   alt_dchisq_plot_dcp=0.000000_hie-1.pdf	    	  
		Inputs files from: ./nonunitarity_graphs/
		Outputs files at:  ./nonunitarity_plots/
