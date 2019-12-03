
$version="3.2.13"

/* Beam data */

/*======================================================================================================*/
/*                                     Beam Power Configurations                                        */
/*                            (Use this chart to detemine @power in units of pot/yr)                    */
/*  MI prot/pulse    Energy (GeV)    Cycle time    Beam power (MW)    Uptime&efficiency    pot/year     */
/*     7.50E+13          120            1.2           1.20E+00              0.56           1.10E+21     */
/*     7.50E+13           80            0.9           1.07E+00              0.56           1.47E+21     */
/*     7.50E+13           60            0.7           1.03E+00              0.56           1.89E+21     */
/*                                                                                                      */
/*======================================================================================================*/


nuflux(#flux_FHC)<
  @flux_file="./flux/flux_optimized_FHC.txt"
  @time = NUTIME /* years */
  @power = 14.7  /*e20 POT per year ****MATCH FLUX POWER TO TABLE ABOVE**** */
  @norm = 1.017718e17 
>

nuflux(#flux_RHC)<
  @flux_file="./flux/flux_optimized_RHC.txt"
  @time = NUBARTIME /* years */
  @power = 14.7  /*e20 POT per year ****MATCH FLUX POWER TO TABLE ABOVE**** */
  @norm = 1.017718e17 
>

