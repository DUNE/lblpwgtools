/* ----------------------------------------------------------------- */
/* Input file containing flux definitions for 120 GeV WBB            */
/* see README file for copyright information and version history     */
/* ----------------------------------------------------------------- */

/* Neutrinos */

nuflux(#fnal_plus)<
        @flux_file="./LAINPUT/Fluxes/dusel120e250i002dr280dz_tgtz30_1300km_0kmoa_flux.txt"
        @time = NUTIME /* years */
		@power = (BEAMPOWER*(1.98e7)/(BEAMENERGY*1.6022e-10))/10e16  /* 10^20 POT per year (708kW,1.98e7s per yr,120GeV) */
        @norm = 1.0544e17 
>

/* Anti-neutrinos */
nuflux(#fnal_minus)<
       @flux_file="./LAINPUT/Fluxes/dusel120e250ni002dr280dz_tgtz30_1300km_0kmoa_flux.txt"
       @time = 0   /* years */
       @power = (BEAMPOWER*(1.98e7)/(BEAMENERGY*1.6022e-10))/10e16 /* x 1e20 pot */
       @norm = 5.19*7.504*10^12*1300*1300/120.0 / 5.202
>

