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
-------------------------------------------------------------1D Allowed Region Plots----------------------------------------------------------
ORDER OF EXECUTION: __________________DCP 1000 EXP_________________
		    python gen_1dscan_graphs_dcp_1000.py
		    python gen_allwdreg_bandout_graphs_dcp_1000.py
		    python gen_allwdreg_bandfull_graphs_dcp_1000.py
		    python gen_allwdreg_bandfull_plots_dcp_1000.py

	gen_1dscan_graphs_dcp_1000.py
		Generates dchisq vs dcp 1d asimov scan TGraphs at 41 true dcp points for 5 true ssth23 values at 1000 kt-MW-yrs exposure. 
		Dchisq is the difference between the asimov and global chisqs. These graphs will be used to produce the allowed dcp regions.
		Graph Names: dchisq_vs_dcp_[100*tr_dcp_val]_trdcp
		Output File Names: 1dscan_graphs_ssth23=[ssth23_val].root
		Inputs files from: /pnfs/dune/persistent/physicsgroups/dunelbl/picker24/contrib/jfleishh/dcp_1dscan_1000/
				   /pnfs/dune/persistent/users/jfleishh/nopen_dcp_scan_grid/
		Outputs files at: ./1dscan_graphs/dcp_1000/
	
	gen_allwdreg_bandout_graphs_dcp_1000.py
		Generates outline TGraphs of the 1 sigma, 90%, and 3 sigma allowed region bands as a function of true dcp for 5 true ssth23
		values at 1000 kt-MW-yrs exposure. Three bands are outlined by the script. The top and bottom outlines of the band containing
		the true point are TGraphs and called "TrueTopG" and "TrueBotG", respectively. There are two other bands outlined by this 
		script: "Lower" and "Upper". These are implemented using TMultiGraphs and the top and bottom outlines of these bands are 
		called "Lower(Upper)TopMG" and "Lower(Upper)BotMG", respectively.
		Graph Names: "TrueBotG" (TGraph)
			     "TrueTopG" (TGraph)
			     "LowerBotMG" (TMultiGraph)
			     "LowerTopMG" (TMultiGraph)
			     "UpperBotMG" (TMultiGraph)
			     "UpperTopMG" (TMultiGraph)
		Output File Names: allwdreg_bandout_graphs_ssth23=[ssth23_val]_[confidence_level].root
		Inputs files from: ./1dscan_graphs/dcp_1000/
		Outputs files at:  ./1dscan_graphs/dcp_1000/
		NOTE: Some graphs or multigraphs may be empty, which can lead to warning messages when plotting. It is generally best to get
		      TGraphs from the list in the TMultiGraph and draw them individually instead of using the TMultiGraph draw function.

	gen_allwdreg_bandfull_graphs_dcp_1000.py
		Generates filled band TGraphs using the outlines generated by the above script. The 1 sigma, 90%, and 3 sigma bands are filled
		with decreasing intensities of red. Outputs these TGraphs as a single TMultiGraph.
		Graph Names: "BandMG" (TMultiGraph)
		Output File Names: allwdreg_bandfull_graphs_ssth23=[ssth23_val]_[confidence_level].root
		Inputs files from: ./1dscan_graphs/dcp_1000/
		Outputs files at:  ./1dscan_graphs/dcp_1000/
		NOTE: Some graphs or multigraphs may be empty, which can lead to warning messages when plotting. It is generally best to get
		      TGraphs from the list in the TMultiGraph and draw them individually instead of using the TMultiGraph draw function.

	gen_allwdreg_bandfull_plots_dcp_1000.py
		Generates plots with the 1 sigma, 90%, and 3 sigma filled allowed region bands drawn for each value of ssth23 at 1000
		kt-MW-yrs exposure.
		Output File Names: allwdreg_bandfull_plot_ssth23=[ssth23_val].png
				   allwdreg_bandfull_plot_ssth23=[ssth23_val].pdf
		Inputs files from: ./1dscan_graphs/dcp_1000/
		Outputs files at:  ./allwdreg_plots/dcp_1000/
		NOTE: Some graphs or multigraphs may be empty, which can lead to warning messages when plotting.

ORDER OF EXECUTION: __________________DCP 250 EXP_________________
		    python gen_1dscan_graphs_dcp_250.py
		    python gen_allwdreg_bandout_graphs_dcp_250.py
		    python gen_allwdreg_bandfull_graphs_dcp_250.py
		    python gen_allwdreg_bandfull_plots_dcp_250.py

	gen_1dscan_graphs_dcp_250.py
		Generates dchisq vs dcp 1d asimov scan TGraphs at 41 true dcp points for 5 true ssth23 values at 250 kt-MW-yrs exposure. 
		Dchisq is the difference between the asimov and global chisqs. These graphs will be used to produce the allowed dcp regions.
		Graph Names: dchisq_vs_dcp_[100*tr_dcp_val]_trdcp
		Output File Names: 1dscan_graphs_ssth23=[ssth23_val].root
		Inputs files from: /pnfs/dune/persistent/physicsgroups/dunelbl/picker24/contrib/jfleishh/dcp_1dscan_250/
				   /pnfs/dune/persistent/users/jfleishh/nopen_dcp_scan_ndfd250_grid/
		Outputs files at: ./1dscan_graphs/dcp_250/
	
	gen_allwdreg_bandout_graphs_dcp_250.py
		Generates outline TGraphs of the 1 sigma, 90%, and 3 sigma allowed region bands as a function of true dcp for 5 true ssth23
		values at 250 kt-MW-yrs exposure. Three bands are outlined by the script. The top and bottom outlines of the band containing
		the true point are TGraphs and called "TrueTopG" and "TrueBotG", respectively. There are two other bands outlined by this 
		script: "Lower" and "Upper". These are implemented using TMultiGraphs and the top and bottom outlines of these bands are 
		called "Lower(Upper)TopMG" and "Lower(Upper)BotMG", respectively.
		Graph Names: "TrueBotG" (TGraph)
			     "TrueTopG" (TGraph)
			     "LowerBotMG" (TMultiGraph)
			     "LowerTopMG" (TMultiGraph)
			     "UpperBotMG" (TMultiGraph)
			     "UpperTopMG" (TMultiGraph)
		Output File Names: allwdreg_bandout_graphs_ssth23=[ssth23_val]_[confidence_level].root
		Inputs files from: ./1dscan_graphs/dcp_250/
		Outputs files at:  ./1dscan_graphs/dcp_250/
		NOTE: Some graphs or multigraphs may be empty, which can lead to warning messages when plotting. It is generally best to get
		      TGraphs from the list in the TMultiGraph and draw them individually instead of using the TMultiGraph draw function.

	gen_allwdreg_bandfull_graphs_dcp_250.py
		Generates filled band TGraphs using the outlines generated by the above script. The 1 sigma, 90%, and 3 sigma bands are filled
		with decreasing intensities of red. Outputs these TGraphs as a single TMultiGraph.
		Graph Names: "BandMG" (TMultiGraph)
		Output File Names: allwdreg_bandfull_graphs_ssth23=[ssth23_val]_[confidence_level].root
		Inputs files from: ./1dscan_graphs/dcp_250/
		Outputs files at:  ./1dscan_graphs/dcp_250/
		NOTE: Some graphs or multigraphs may be empty, which can lead to warning messages when plotting. It is generally best to get
		      TGraphs from the list in the TMultiGraph and draw them individually instead of using the TMultiGraph draw function.

	gen_allwdreg_bandfull_plots_dcp_250.py
		Generates plots with the 1 sigma, 90%, and 3 sigma filled allowed region bands drawn for each value of ssth23 at 250
		kt-MW-yrs exposure.
		Output File Names: allwdreg_bandfull_plot_ssth23=[ssth23_val].png
				   allwdreg_bandfull_plot_ssth23=[ssth23_val].pdf
		Inputs files from: ./1dscan_graphs/dcp_250/
		Outputs files at:  ./allwdreg_plots/dcp_250/
		NOTE: Some graphs or multigraphs may be empty, which can lead to warning messages when plotting.

ORDER OF EXECUTION: __________________DCP 100 EXP_________________
		    python gen_1dscan_graphs_dcp_100.py
		    python gen_allwdreg_bandout_graphs_dcp_100.py
		    python gen_allwdreg_bandfull_graphs_dcp_100.py
		    python gen_allwdreg_bandfull_plots_dcp_100.py

	gen_1dscan_graphs_dcp_100.py
		Generates dchisq vs dcp 1d asimov scan TGraphs at 41 true dcp points for 5 true ssth23 values at 100 kt-MW-yrs exposure. 
		Dchisq is the difference between the asimov and global chisqs. These graphs will be used to produce the allowed dcp regions.
		Graph Names: dchisq_vs_dcp_[100*tr_dcp_val]_trdcp
		Output File Names: 1dscan_graphs_ssth23=[ssth23_val].root
		Inputs files from: /pnfs/dune/persistent/physicsgroups/dunelbl/picker24/contrib/jfleishh/dcp_1dscan_100/
				   /pnfs/dune/persistent/users/jfleishh/nopen_dcp_scan_ndfd100_grid/
		Outputs files at: ./1dscan_graphs/dcp_100/
	
	gen_allwdreg_bandout_graphs_dcp_100.py
		Generates outline TGraphs of the 1 sigma, 90%, and 3 sigma allowed region bands as a function of true dcp for 5 true ssth23
		values at 100 kt-MW-yrs exposure. Three bands are outlined by the script. The top and bottom outlines of the band containing
		the true point are TGraphs and called "TrueTopG" and "TrueBotG", respectively. There are two other bands outlined by this 
		script: "Lower" and "Upper". These are implemented using TMultiGraphs and the top and bottom outlines of these bands are 
		called "Lower(Upper)TopMG" and "Lower(Upper)BotMG", respectively.
		Graph Names: "TrueBotG" (TGraph)
			     "TrueTopG" (TGraph)
			     "LowerBotMG" (TMultiGraph)
			     "LowerTopMG" (TMultiGraph)
			     "UpperBotMG" (TMultiGraph)
			     "UpperTopMG" (TMultiGraph)
		Output File Names: allwdreg_bandout_graphs_ssth23=[ssth23_val]_[confidence_level].root
		Inputs files from: ./1dscan_graphs/dcp_100/
		Outputs files at:  ./1dscan_graphs/dcp_100/
		NOTE: Some graphs or multigraphs may be empty, which can lead to warning messages when plotting. It is generally best to get
		      TGraphs from the list in the TMultiGraph and draw them individually instead of using the TMultiGraph draw function.

	gen_allwdreg_bandfull_graphs_dcp_100.py
		Generates filled band TGraphs using the outlines generated by the above script. The 1 sigma, 90%, and 3 sigma bands are filled
		with decreasing intensities of red. Outputs these TGraphs as a single TMultiGraph.
		Graph Names: "BandMG" (TMultiGraph)
		Output File Names: allwdreg_bandfull_graphs_ssth23=[ssth23_val]_[confidence_level].root
		Inputs files from: ./1dscan_graphs/dcp_100/
		Outputs files at:  ./1dscan_graphs/dcp_100/
		NOTE: Some graphs or multigraphs may be empty, which can lead to warning messages when plotting. It is generally best to get
		      TGraphs from the list in the TMultiGraph and draw them individually instead of using the TMultiGraph draw function.

	gen_allwdreg_bandfull_plots_dcp_100.py
		Generates plots with the 1 sigma, 90%, and 3 sigma filled allowed region bands drawn for each value of ssth23 at 100
		kt-MW-yrs exposure.
		Output File Names: allwdreg_bandfull_plot_ssth23=[ssth23_val].png
				   allwdreg_bandfull_plot_ssth23=[ssth23_val].pdf
		Inputs files from: ./1dscan_graphs/dcp_100/
		Outputs files at:  ./allwdreg_plots/dcp_100/
		NOTE: Some graphs or multigraphs may be empty, which can lead to warning messages when plotting.

ORDER OF EXECUTION: ________________SS2TH13 1000 EXP_________________
		    python gen_1dscan_graphs_ss2th13_1000.py
		    python gen_allwdreg_bandout_graphs_ss2th13_1000.py
		    python gen_allwdreg_bandfull_graphs_ss2th13_1000.py
		    python gen_allwdreg_bandfull_plots_ss2th13_1000.py

	gen_1dscan_graphs_ss2th13_1000.py
		Generates dchisq vs ss2th13 1d asimov scan TGraphs at 21 true ss2th13 points for 5 true ssth23 values at 1000 kt-MW-yrs 
		exposure. Dchisq is the difference between the asimov and global chisqs. These graphs will be used to produce the allowed 
		ss2th13 regions.
		Graph Names: "dchisq_vs_ss2th13_[10000*tr_ss2th13_val]_trss2th13"
		Output File Names: 1dscan_graphs_ssth23=[ssth23_val].root
		Inputs files from: /pnfs/dune/persistent/physicsgroups/dunelbl/picker24/contrib/jfleishh/th13_1dscan_1000
				   /pnfs/dune/persistent/users/jfleishh/nopen_scan_grid/
		Outputs files at: ./1dscans/ss2th13_1000/
	
	gen_allwdreg_bandout_graphs_ss2th13_1000.py
		Generates outline TGraphs of the 1 sigma, 90%, and 3 sigma allowed region bands as a function of true ss2th13 for 5 true 
		ssth23 values at 1000 kt-MW-yrs exposure. Two bands are outlined by the script: a lower and an upper. The bottom and top of 
		the lower band are TGraphs with names "pos_B" and "pos_MB", respectively. The bottom and top of the upper band are TGraphs 
		with names "pos_MT" and "pos_T", respectively. When there is only one band to draw the lower and upper bands coincide.
		Graph Names: "pos_B" (TGraph)
			     "pos_MB" (TGraph)
			     "pos_MT" (TGraph)
			     "pos_T" (TGraph)
		Output File Names: allwdreg_bandout_graphs_ssth23=[ssth23_val]_[confidence_level].root
		Inputs files from: ./1dscan_graphs/ss2th13_1000/
		Outputs files at:  ./1dscan_graphs/ss2th13_1000/
		NOTE: Because the bands can coincide, there are plotting flaws in which bands can appear to merge when they are meant to
		      remain separate.

	gen_allwdreg_bandfull_graphs_ss2th13_1000.py
		Generates filled band TGraphs using the outlines generated by the above script. The 1 sigma, 90%, and 3 sigma bands are filled
		with decreasing intensities of red. Outputs these TGraphs with names "lowband" for the lower band and "upband" for the upper
		band.
		Graph Names: "lowband" (TGraph)
			     "upband" (TGraph)
		Output File Names: allwdreg_bandfull_graphs_ssth23=[ssth23_val]_[confidence_level].root
		Inputs files from: ./1dscan_graphs/ss2th13_1000/
		Outputs files at:  ./1dscan_graphs/ss2th13_1000/
		NOTE: Because the bands can coincide, there are plotting flaws in which bands can appear to merge when they are meant to
		      remain separate.

	gen_allwdreg_bandfull_plots_ss2th13_1000.py
		Generates plots with the 1 sigma, 90%, and 3 sigma filled allowed region bands drawn for each value of ssth23 at 1000
		kt-MW-yrs exposure.
		Output File Names: allwdreg_bandfull_plot_ssth23=[ssth23_val].png
				   allwdreg_bandfull_plot_ssth23=[ssth23_val].pdf
		Inputs files from: ./1dscan_graphs/ss2th13_1000/
		Outputs files at:  ./allwdreg_plots/ss2th13_1000/
		NOTE: Because the bands can coincide, there are plotting flaws in which bands can appear to merge when they are meant to
		      remain separate.

ORDER OF EXECUTION: ________________SSTH23 1000 EXP_________________
		    python gen_1dscan_graphs_ssth23_1000.py
		    python gen_allwdreg_bandout_graphs_ssth23_1000.py
		    python gen_allwdreg_bandfull_graphs_ssth23_1000.py
		    python gen_allwdreg_bandfull_plots_ssth23_1000.py

	gen_1dscan_graphs_ssth23_1000.py
		Generates dchisq vs ssth23 1d asimov scan TGraphs at 21 true ssth23 points at 1000 kt-MW-yrs exposure. Dchisq is the 
		difference between the asimov and global chisqs. These graphs will be used to produce the allowed ssth23 regions.
		Graph Names: "dchisq_vs_ssth23_[100*tr_ssth23_val]_trssth23"
		Output File Names: 1dscan_graphs.root
		Inputs files from: /pnfs/dune/persistent/physicsgroups/dunelbl/picker24/contrib/jfleishh/ssth23_1dscan_1000/
				   /pnfs/dune/persistent/users/jfleishh/nopen_ssth23_scan_grid/
		Outputs files at: ./1dscans/ssth23_1000/
	
	gen_allwdreg_bandout_graphs_ssth23_1000.py
		Generates outline TGraphs of the 1 sigma, 90%, and 3 sigma allowed region bands as a function of true ssth23 at 1000 kt-MW-yrs
		exposure. Two bands are outlined by the script: a lower and an upper. The bottom and top of the lower band are TGraphs with 
		names "pos_B" and "pos_MB", respectively. The bottom and top of the upper band are TGraphs with names "pos_MT" and "pos_T", 
		respectively. When there is only one band to draw the lower and upper bands coincide.
		Graph Names: "pos_B" (TGraph)
			     "pos_MB" (TGraph)
			     "pos_MT" (TGraph)
			     "pos_T" (TGraph)
		Output File Names: allwdreg_bandout_graphs_[confidence_level].root
		Inputs files from: ./1dscan_graphs/ssth23_1000/
		Outputs files at:  ./1dscan_graphs/ssth23_1000/
		NOTE: Because the bands can coincide, there are plotting flaws in which bands can appear to merge when they are meant to
		      remain separate.

	gen_allwdreg_bandfull_graphs_ssth23_1000.py
		Generates filled band TGraphs using the outlines generated by the above script. The 1 sigma, 90%, and 3 sigma bands are filled
		with decreasing intensities of red. Outputs these TGraphs with names "lowband" for the lower band and "upband" for the upper
		band.
		Graph Names: "lowband" (TGraph)
			     "upband" (TGraph)
		Output File Names: allwdreg_bandfull_graphs_[confidence_level].root
		Inputs files from: ./1dscan_graphs/ssth23_1000/
		Outputs files at:  ./1dscan_graphs/ssth23_1000/
		NOTE: Because the bands can coincide, there are plotting flaws in which bands can appear to merge when they are meant to
		      remain separate.

	gen_allwdreg_bandfull_plots_ssth23_1000.py
		Generates plots with the 1 sigma, 90%, and 3 sigma filled allowed region bands drawn for each value of ssth23 at 1000
		kt-MW-yrs exposure.
		Output File Names: allwdreg_bandfull_plot.png
				   allwdreg_bandfull_plot.pdf
		Inputs files from: ./1dscan_graphs/ssth23_1000/
		Outputs files at:  ./allwdreg_plots/ssth23_1000/
		NOTE: Because the bands can coincide, there are plotting flaws in which bands can appear to merge when they are meant to
		      remain separate.
---------------------------------------------------------2D Asimov th13-th23 Scan Plots-------------------------------------------------------
ORDER OF EXECUTION: python gen_2dscan_graphs_th13_th23.py
		    python gen_2dscan_CLplots_th13_th23.py
	
	gen_2dscan_graphs_th13_th23.py
		Generates a TGraph2D for each scanned region in ss2th13-ssth23 space. The TGraph2D gives the dchisq as a function of ssth23 
		and ss2th13. Dchisq is the difference between the asimov and global fit chisqs. The th13-th23 space is scanned for 4 true 
		points which produce a total of 7 scanned regions as described below:
		"th13_low_th23_up_troct": ss2th13=0.063, ssth23=0.58, true octant (True Point 1)
		"th13_low_th23_up_foct": ss2th13=0.063, ssth23=0.58, flipped octant (True Point 1)
		"th13_dyb_th23_low_troct": ss2th13=0.088, ssth23=0.42, true octant (True Point 2)
		"th13_dyb_th23_low_foct": ss2th13=0.088, ssth23=0.42, flipped octant (True Point 2)
		"th13_dyb_th23_up_troct": ss2th13=0.088, ssth23=0.58, true octant (True Point 3)
		"th13_dyb_th23_up_foct": ss2th13=0.088, ssth23=0.58, flipped octant (True Point 3)
		"th13_up_th23_low_troct": ss2th13=0.113, ssth23=0.42, true octant (True Point 4)
		Graph Names: "scan_2D_[region_name]"
		Output File Names: 2dscan_graphs.root
		Inputs files from: /pnfs/dune/persistent/physicsgroups/dunelbl/picker24/contrib/jfleishh/th13_ssth23_2dscan_hres/[region_name]
				   /pnfs/dune/persistent/users/jfleishh/2D_th13_th23_scan_hres_grid/[region_name]/
		Outputs files at:  ./2dscan_th13_th23_graphs/
	
	gen_2dscan_CLplots_th13_th23.py
		Generates plots for the 4 true th13-th23 points with the 1 sigma, 90%, and 3 sigma contour lines drawn.
		True points:
		1: "th13_low_th23_up": ss2th13=0.063, ssth23=0.58
		2: "th13_dyb_th23_up": ss2th13=0.088, ssth23=0.42
		3: "th13_dyb_th23_up": ss2th13=0.088, ssth23=0.58
		4: "th13_up_th23_low": ss2th13=0.113, ssth23=0.42
		Output File Names: 2dscan_CLplots_[true_point].png
				   2dscan_CLplots_[true_point].pdf
		Inputs files from: ./2dscan_th13_th23_graphs/
		Outputs files at:  ./2dscan_th13_th23_CLplots/
--------------------------------------------------------Fixed Point Probability Plots---------------------------------------------------------
ORDER OF EXECUTION: root -l gen_fixed_point_prob_graphs.C
		    root -l gen_fixed_point_prob_plots.C

	gen_fixed_point_prob_graphs.C
		Generates DeltaP/P vs E probability TGraphs in both nue and nueb appearance channels for 6 fixed points. DeltaP is the 
		difference between the probability at the fixed point and the probability at the normal point. The P in the denominator is the
		probability at the normal point.
		Normal Point: ss2th13=0.088, ssth23=0.50, dcp=0, NO, all others at nu-fit 4.0
		Fixed Points: 1. ss2th13=0.088, ssth23=0.50, dcp=0, IO, all others at nu-fit 4.0
			      2. ss2th13=0.088, ssth23=0.50, dcp=-Pi/2, NO, all others at nu-fit 4.0
			      3. ss2th13=0.088, ssth23=0.50, dcp=-Pi/4, NO, all others at nu-fit 4.0
			      4. ss2th13=0.088, ssth23=0.50, dcp=-3Pi/4, NO, all others at nu-fit 4.0  
			      5. ss2th13=0.088, ssth23=0.58, dcp=-Pi/4, NO, all others at nu-fit 4.0
			      6. ss2th13=0.113, ssth23=0.44, dcp=-Pi/4, NO, all others at nu-fit 4.0
		Output File Names: fixed_point_prob_graphs.root
		Inputs files from: NONE
		Outputs files at:  ./prob_graphs/
		NOTE: Requires prob3++ to run. Likely you will need to copy this script and its directory dependencies into a local prob3++
		      installation to run.

	gen_fixed_point_prob_plots.C
		Generates plots with 6 nue and 6 nueb appearance DeltaP/P vs E curves created by script above plotted together.
		Output File Names: fixed_point_prob_nue_plot.png
				   fixed_point_prob_nue_plot.pdf
				   fixed_point_prob_nue_logplot.png
				   fixed_point_prob_nue_logplot.pdf
				   fixed_point_prob_nueb_plot.png
				   fixed_point_prob_nueb_plot.pdf
				   fixed_point_prob_nueb_logplot.png
				   fixed_point_prob_nueb_logplot.pdf
		Inputs files from: ./prob_graphs/
		Outputs files at:  ./prob_plots/
-------------------------------------------------------dcp Asimov Throws Comparison Plots-----------------------------------------------------
ORDER OF EXECUTION: python gen_asimov_throws_comp_hists_dcp.py
		    python gen_asimov_throws_comp_plots_dcp.py

	gen_asimov_throws_comp_hists_dcp.py
		Generates dchisq vs dcp 1d Asimov scan TGraph and bin frequency probability vs dcp throws TH1D for comparison at the fixed 
		point. Dchisq is the difference between the asimov and global fit chisqs. The bin frequency probability is the fraction of
		throws that falls in a bin. The bins are of width 0.02.
		Graph/hist Names: "throws_hist" (TH1D)
				  "asimov_graph" (TGraph)
		Output File Names: dcp_singlept_asimovscan_throwshist_ndfd[exp]_hie1_th13=dyb_th23=0.58_dcp=-0.25.root
		Inputs files from: /pnfs/dune/persistent/physicsgroups/dunelbl/callumw/fixed_osc_toy_throws/
				   /dune/data/users/jfleishh/fixed_osc_toy_throws/
				   ./1dscan_graphs/dcp_[exp]/
		Outputs files at:  ./asimov_throws_comp_hists/
		NOTE: Requires running gen_1dscan_graphs_dcp_[exp].py first.
		NOTE: Inputs one fixed point, which must be manually changed to switch to a different exposure. Different points may also be
		      used, as long as the throws and asimov fits are there. Read script for current point being used.
	
	gen_asimov_throws_comp_plots_dcp.py
		Generates plots for each of the objects produced by the above script.
		Output File Names: dcp_singlept_throwshist_ndfd[exp]_hie1_th13=dyb_th23=0.58_dcp=-0.25.png
				   dcp_singlept_throwshist_ndfd[exp]_hie1_th13=dyb_th23=0.58_dcp=-0.25.pdf
				   dcp_singlept_asimovgraph_ndfd[exp]_hie1_th13=dyb_th23=0.58_dcp=-0.25.png
				   dcp_singlept_asimovgraph_ndfd[exp]_hie1_th13=dyb_th23=0.58_dcp=-0.25.pdf
		Inputs files from: ./asimov_throws_comp_hists/
		Outputs files at:  ./asimov_throws_comp_plots/
