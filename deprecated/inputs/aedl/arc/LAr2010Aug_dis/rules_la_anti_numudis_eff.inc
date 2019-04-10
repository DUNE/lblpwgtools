/* ----------------------------------------------------------------- */
/* Include file containing LAr detector specific stuff for far-only  */
/* simulation                                                        */
/* see README file for copyright information and version history     */
/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------- */
/*                                Rules                              */
/* ----------------------------------------------------------------- */


rule(#rule7)<
        @signal = 0.85@#ANTI_nu_mu_bar_disappearance_CC
        @signalerror = 0.05 : 0.02
        @background = 0.005@#ANTI_nu_mu_bar_disappearance_NC : 0.85@#ANTI_nu_mu_beam
        @backgrounderror = 0.1 : 0.02
        @sys_on_function = "chiSpectrumTilt"
        @sys_off_function = "chiNoSysSpectrum"
>
