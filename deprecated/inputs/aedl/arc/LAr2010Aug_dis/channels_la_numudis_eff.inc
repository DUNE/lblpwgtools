$version="3.0.0"


/* ----------------------------------------------------------------- */
/*                       Baseline/Detector properties                */
/* ----------------------------------------------------------------- */

/* Binning */
$emin =                 0.5
$emax =                20.0
$bins =               150

$sampling_min =        0.5
$sampling_max =       20.0


/* Technical information */
$filter_state = 0
$filter_value = 0.125


/* Energy Resolution */
energy(#ERES_CC_NC)<
        @type = 1
        @sigma_e = {0.0,0.20,0.0}
>



/* ----------------------------------------------------------------- */
/*                                Channels                           */
/* ----------------------------------------------------------------- */

channel(#nu_e_appearance_CC)<
        @channel =      #fnal_plus:     +:      m:      e:      #CC:    #ERES_CC_NC
>

channel(#nu_mu_disappearance_CC)<
        @channel =      #fnal_plus:     +:      m:      m:      #CC:    #ERES_CC_NC
>

channel(#nu_mu_disappearance_NC)<
        @channel =      #fnal_plus:     +:      m: NOSC_m:      #NC:    #ERES_CC_NC
>

channel(#nu_e_beam)<
        @channel =      #fnal_plus:     +:      e:      e:      #CC:    #ERES_CC_NC
>

channel(#nu_e_bar_beam)<
        @channel =      #fnal_plus:     -:      e:      e:      #CC:    #ERES_CC_NC
>

channel(#ANTI_nu_mu_bar_disappearance_CC)<
        @channel =      #fnal_minus:    -:      m:      m:      #CC:    #ERES_CC_NC
>

channel(#ANTI_nu_mu_bar_disappearance_NC)<
        @channel =      #fnal_minus:    -:      m: NOSC_m:      #NC:    #ERES_CC_NC
>

channel(#ANTI_nu_e_bar_beam)<
        @channel =      #fnal_minus:    -:      e:      e:      #CC:    #ERES_CC_NC
>

channel(#ANTI_nu_e_beam)<
        @channel =      #fnal_minus:    +:      e:      e:      #CC:    #ERES_CC_NC
>

channel(#ANTI_nu_e_bar_appearance_CC)<
        @channel =      #fnal_minus:    -:      m:      e:      #CC:    #ERES_CC_NC
>

channel(#ANTI_nu_mu_beam)<
        @channel =      #fnal_minus:    +:      m:      m:      #CC:    #ERES_CC_NC
>

