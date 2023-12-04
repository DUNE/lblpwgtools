/* ----------------------------------------------------------------- */
/* Include file containing LAr detector specific stuff for far-only  */
/* simulation                                                        */
/* see README file for copyright information and version history     */
/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------- */
/*                                Rules                              */
/* ----------------------------------------------------------------- */


rule(#rule6)<
        @signal = 0.85@#nu_mu_disappearance_CC
        @signalerror = 0.05 : 0.02
        @background = 0.005@#nu_mu_disappearance_NC
        @backgrounderror = 0.1 : 0.02
	@sys_on_function = "chiSpectrumTilt"
        @sys_off_function = "chiNoSysSpectrum"
>

