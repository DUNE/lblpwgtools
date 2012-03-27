$version="3.0.0"
/* Cross sections */
include "./LAINPUT/cross_sections.inc"

/* ----------------------------------------------------------------- */
/*             Detector properties : Liquid Argon                    */
/* ----------------------------------------------------------------- */

/* Binning */
/* $emin =                 0.5 */
/*$emax =                20.0 */
/*$bins =               150 */

/*$sampling_min =        0.5 */
/*$sampling_max =       20.0 */

$emin =                 0.5
$emax =                 60.0
$binsize= {0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 
0.125, 0.125,0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 
0.125, 0.125,0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 
0.125, 0.125,0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 
0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 
0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4}


$sampling_min =                 0.5
$sampling_max =                 60.0
$sampling_stepsize= {0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 
0.125, 0.125, 0.125,0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 
0.125, 0.125, 0.125,0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 
0.125, 0.125, 0.125,0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 
0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 
0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 2, 2, 2, 2, 2, 2, 2, 
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}



/* Technical information */
$filter_state = 0
$filter_value = 0.125


/* Energy Resolution */

energy(#mucc)<
        @type = 1
        @sigma_e = {0.0,0.15,0.0}
>

energy(#muqe)<
        @type = 1
        @sigma_e = {0.0,0.05,0.0}
>

energy(#elec)<
        @type = 1
        @sigma_e = {0.0,0.15,0.0}
>

energy(#hadr)<
        @type = 1
        @sigma_e = {0.0,0.40,0.0}
>

energy(#nc)<>
include "./LAINPUT/nc_smear_nu_ereco.dat"

/* ----------------------------------------------------------------- */
/*                                Channels                           */
/* ----------------------------------------------------------------- */

channel(#anu_mu_cc)<
        @channel =      #fnal_minus:    -:      m:      m:      #CC:    #mucc
>

channel(#anu_mu_qe)<
        @channel =      #fnal_minus:     -:      m:      m:      #QE:    #muqe
>

channel(#anu_mu_nc)<
        @channel =      #fnal_minus:    -:      m: NOSC_m:      #NC:    #nc
		@pre_smearing_efficiencies ={ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	     1, 1, 1, 1, 1, 1}

        @post_smearing_efficiencies ={ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	     1, 1, 1, 1, 1}
>

channel(#anu_e_beam)<
        @channel =      #fnal_minus:    -:      e:      e:      #CC:    #elec
>

channel(#anu_e_bar_beam)<
        @channel =      #fnal_minus:    +:      e:      e:      #CC:    #elec
>

channel(#anu_e_sig)<
        @channel =      #fnal_minus:    -:      m:      e:      #CC:    #elec
>

channel(#anu_e_bar_sig)<
        @channel =      #fnal_minus:    +:      m:      e:      #CC:    #elec
>

channel(#anu_mu_plus_cc)<
        @channel =      #fnal_minus:    +:      m:      m:      #CC:    #mucc
>



/* ----------------------------------------------------------------- */
/*                    Rules for a Liquid Argon Detector              */
/* ----------------------------------------------------------------- */

/* CC */
rule(#anu_e_appearance)<
        @signal = 0.8@#anu_e_sig
        @signalerror = 0.01 : 0.0001
        @background = 0.01@#anu_mu_cc  :  0.8@#anu_e_bar_beam   :  0.8@#anu_e_beam : 0.01@#anu_mu_nc
		@backgrounderror = 0.05 : 0.0001
        @sys_on_function = "chiSpectrumTilt"
        @sys_off_function = "chiNoSysSpectrum"
>

rule(#anu_mu_disappearance)<
        @signal = 1.0@#anu_mu_cc
        @signalerror = 0.01 : 0.0001
        @background = 0.005@#anu_mu_nc
        @backgrounderror = 0.01 : 0.0001
        @sys_on_function = "chiSpectrumTilt"
        @sys_off_function = "chiNoSysSpectrum"
>




