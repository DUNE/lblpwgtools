#include "customChis.h"



/***********************************************************************
//custom chi functions*/
double sigma_binbin = 0.05;
inline double myprior(double x, double center, double sigma)
{
  double tmp = (x - center)/sigma;
  return tmp*tmp;
}

inline double mylikelihood(double true_rate, double fit_rate)
{
  double res;
  res = fit_rate - true_rate;
  if (true_rate > 0)
  {
    if (fit_rate <= 0.0)
      res = 1e100;
    else
      res += true_rate * log(true_rate/fit_rate);
  }
  else
    res = fabs(res);

  return 2.0 * res;
 }

/* Gauss likelihood (this is sufficient here due to the large event numbers
 * in a reactor experiment; for other setups, one should use Poisson statistics) */
inline double glikelihood(double true_rate, double fit_rate, double sqr_sigma)
{
  if (sqr_sigma > 0)
    return (true_rate - fit_rate)*(true_rate - fit_rate) / sqr_sigma;
  else
    return 0.0;
} 
 
double chiSpectrumTiltSplitBG(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double *bin_centers      = glbGetBinCentersListPtr(exp);
  double signal_norm, signal_tilt, bg_tilt;
  int ew_low, ew_high;
  double emin, emax, ecenter;
  double fit_rate;
  double chi2 = 0.0;
  int i,j,ebin;
  
  glbGetEminEmax(exp, &emin, &emax);
  ecenter = 0.5 * (emax + emin);
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  
  //signal energy tilt systematics still treated in sum
  signal_norm = 1.0 + x[0];
  signal_tilt = x[1] / (emax - emin);
  //bg tilt treated in sum as well
  bg_tilt=x[2]/(emax-emin);
  
  //find out how many background channels there are
  //init bg_norm array
  int bg_channels = glbGetLengthOfRule(exp,rule,GLB_BG);
  double bg_norm[bg_channels];

  //count energy bins and init/populate bg_fit_rates_per_channel
  int ebins=0;
  for (i=ew_low; i <= ew_high; i++) ebins++;
  double bg_fit_rates_per_channel[bg_channels][ebins];
  //printf("%d ebins; %d bg_channels\n",ebins,bg_channels);
  for(i=0;i<bg_channels;i++){
	  //printf("channel %d is %s\n",i,glbValueToName(exp,"channel",i));
	  double * trates=glbGetChannelRatePtr(exp,i,GLB_POST);
		for(ebin=0;ebin<ebins;ebin++){
			bg_fit_rates_per_channel[i][ebin] = trates[ebin];
			//printf("%f : %f\n",bin_centers[ebin],bg_fit_rates_per_channel[i][ebin]);
	  }
  }
  double bg_norm_center, bg_tilt_center;
  glbGetBGCenters(exp, rule, &bg_norm_center, &bg_tilt_center);
  //printf("bg_norm_center:%f   bg_tilt_center:%f\n",bg_norm_center,bg_tilt_center);
  //exit(0);
  //bg energy norm is split by background channel
  //there should be an norm for each BG type
  //otherwise, kaboom (TODO: add a check)
  j=3;
  for(i=0; i<bg_channels; i++){
	  bg_norm[i]=bg_norm_center * (1.0 + x[j]);
	  j=j+1;
  }

  for (i=ew_low; i <= ew_high; i++){
    fit_rate = signal_norm*signal_fit_rates[i]
                + signal_tilt*(bin_centers[i]-ecenter)*signal_fit_rates[i];
                
		//addon to fit_rate for each bg channel
		for(j=0; j<bg_channels; j++){
			fit_rate += 	bg_norm[j]*bg_fit_rates_per_channel[j][i]
						 +bg_tilt*(bin_centers[i]-ecenter)*bg_fit_rates_per_channel[j][i];
		 }
    
    chi2 += mylikelihood(true_rates[i], fit_rate);
  }

  chi2 += myprior(x[0], 0.0, errors[0])
            + myprior(x[1], 0.0, errors[1])
            + myprior(bg_tilt, bg_tilt_center, errors[2]);
  //add chi2 penalties for bg priors
  j=3;
  for(i=0; i<bg_channels; i++){
	  chi2 += myprior(bg_norm[i], bg_norm_center, errors[j]);
		j=j+1;
  }
  
  //copy parameter array into globally accessible variable
  for(i=0; i<n_params; i++){
		xmin[exp][i]=x[i];
	}
  
  //printf("%f\n",chi2);
  //exit(0);
  return chi2;
}

//same as above, but tilt is removed
double chiSpectrumSplitBG(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double signal_norm;
  int ew_low, ew_high;
  double emin, emax, ecenter;
  double fit_rate;
  double chi2 = 0.0;
  int i,j,ebin;
  
  glbGetEminEmax(exp, &emin, &emax);
  ecenter = 0.5 * (emax + emin);
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  
  //signal norm systematics still treated in sum
  signal_norm = 1.0 + x[0];
  
  //find out how many background channels there are
  //init bg_norm array
  int bg_channels = glbGetLengthOfRule(exp,rule,GLB_BG);
  double bg_norm[bg_channels];

  //count energy bins and init/populate bg_fit_rates_per_channel
  int ebins=0;
  for (i=ew_low; i <= ew_high; i++) ebins++;
  double bg_fit_rates_per_channel[bg_channels][ebins];
  //printf("%d ebins; %d bg_channels\n",ebins,bg_channels);
  for(i=0;i<bg_channels;i++){
	  //printf("channel %d is %s\n",i,glbValueToName(exp,"channel",i));
	  double * trates=glbGetChannelRatePtr(exp,i,GLB_POST);
		for(ebin=0;ebin<ebins;ebin++){
			bg_fit_rates_per_channel[i][ebin] = trates[ebin];
	  }
  }
  double bg_norm_center=1.0;
  //printf("bg_norm_center:%f\n",bg_norm_center);
  //exit(0);
  //bg energy norm is split by background channel
  //there should be an norm for each BG type
  //otherwise, kaboom (TODO: add a check)
  j=1;
  for(i=0; i<bg_channels; i++){
	  bg_norm[i]=bg_norm_center * (1.0 + x[j]);
	  j=j+1;
  }

  for (i=ew_low; i <= ew_high; i++){
    fit_rate = signal_norm*signal_fit_rates[i];
                
		//addon to fit_rate for each bg channel
		for(j=0; j<bg_channels; j++){
			fit_rate += 	bg_norm[j]*bg_fit_rates_per_channel[j][i];
		 }
    
    chi2 += mylikelihood(true_rates[i], fit_rate);
  }
  //printf("after likelihood, chi2 is %f\n",chi2);
  //add penalty for signal norm
  chi2 += myprior(x[0], 0.0, errors[0]);
  //printf("after first priors, chi2 is %f\n",chi2);
  //add chi2 penalties for bg norms
  j=1;
  for(i=0; i<bg_channels; i++){
	  chi2 += myprior(bg_norm[i], bg_norm_center, errors[j]);
    //printf("after prior %d for channel %s\t, chi2 is %f\n",i,glbValueToName(exp,"channel",i),chi2);
		j=j+1;
  }
  //printf("after all priors, chi2 is %f\n",chi2);
  //copy parameter array into globally accessible variable
  for(i=0; i<n_params; i++){
		xmin[exp][i]=x[i];
	}
  
  //printf("%f\n",chi2);
  //exit(0);
  return chi2;
}

//This adds in a bin to bin uncorrelated error to reflect
//imperfect knowledge of the shape in the spectrum.
double chiSpectrumSplitBG_BtoB(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  //printf("chiSpectrumSplitBG_BtoB\n");
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double signal_norm;
  int ew_low, ew_high;
  double emin, emax, ecenter;
  double fit_rate;
  double chi2 = 0.0;
  int i,j,ebin;
  
  glbGetEminEmax(exp, &emin, &emax);
  ecenter = 0.5 * (emax + emin);
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  
  //signal norm systematics still treated in sum
  signal_norm = 1.0 + x[0];
  
  //find out how many background channels there are
  //init bg_norm array
  int bg_channels = glbGetLengthOfRule(exp,rule,GLB_BG);
  double bg_norm[bg_channels];

  //count energy bins and init/populate bg_fit_rates_per_channel
  int ebins=0;
  for (i=ew_low; i <= ew_high; i++) ebins++;
  double bg_fit_rates_per_channel[bg_channels][ebins];
  //printf("%d ebins; %d bg_channels\n",ebins,bg_channels);
  for(i=0;i<bg_channels;i++){
	  //printf("channel %d is %s\n",i,glbValueToName(exp,"channel",i));
	  double * trates=glbGetChannelRatePtr(exp,i,GLB_POST);
		for(ebin=0;ebin<ebins;ebin++){
			bg_fit_rates_per_channel[i][ebin] = trates[ebin];
	  }
  }
  double bg_norm_center=1.0;
  //printf("bg_norm_center:%f\n",bg_norm_center);
  //exit(0);
  //bg energy norm is split by background channel
  //there should be an norm for each BG type
  //otherwise, kaboom (TODO: add a check)
  j=1;
  for(i=0; i<bg_channels; i++){
	  bg_norm[i]=bg_norm_center * (1.0 + x[j]);
	  j=j+1;
  }

  for (i=ew_low; i <= ew_high; i++){
    fit_rate = signal_norm*signal_fit_rates[i];
                
		//addon to fit_rate for each bg channel
		for(j=0; j<bg_channels; j++){
			fit_rate += 	bg_norm[j]*bg_fit_rates_per_channel[j][i];
		 }
    chi2 += glikelihood(true_rates[i], fit_rate,true_rates[i] * (1.0 + true_rates[i]*(sigma_binbin*sigma_binbin)));
  }
  //printf("after likelihood, chi2 is %f\n",chi2);
  //add penalty for signal norm
  chi2 += myprior(x[0], 0.0, errors[0]);
  //printf("after first priors, chi2 is %f\n",chi2);
  //add chi2 penalties for bg norms
  j=1;
  for(i=0; i<bg_channels; i++){
	  chi2 += myprior(bg_norm[i], bg_norm_center, errors[j]);
    //printf("after prior %d for channel %s\t, chi2 is %f\n",i,glbValueToName(exp,"channel",i),chi2);
		j=j+1;
  }
  //printf("after all priors, chi2 is %f\n",chi2);
  //copy parameter array into globally accessible variable
  for(i=0; i<n_params; i++){
		xmin[exp][i]=x[i];
	}
  
  //printf("%f\n",chi2);
  //exit(0);
  return chi2;
}

//same as globes, but customized to copy values to xmin for later use
double chiSpectrumTiltCustom(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  //int n_bins = glbGetNumberOfBins(exp);
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double *bg_fit_rates     = glbGetBGFitRatePtr(exp, rule);
  double *bin_centers      = glbGetBinCentersListPtr(exp);
  double signal_norm, signal_tilt;
  double bg_norm_center, bg_tilt_center;
  double bg_norm, bg_tilt;
  int ew_low, ew_high;
  double emin, emax, ecenter;
  double fit_rate;//, true_rate
  double chi2 = 0.0;
  int i;

  glbGetEminEmax(exp, &emin, &emax);
  ecenter = 0.5 * (emax + emin);
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  glbGetBGCenters(exp, rule, &bg_norm_center, &bg_tilt_center);
  signal_norm = 1.0 + x[0];
  signal_tilt = x[1] / (emax - emin);
  bg_norm = bg_norm_center * (1.0 + x[2]);
  bg_tilt = x[3] / (emax - emin);
  for (i=ew_low; i <= ew_high; i++)
  {
    fit_rate = signal_norm*signal_fit_rates[i]
                + signal_tilt*(bin_centers[i]-ecenter)*signal_fit_rates[i]
                + bg_norm*bg_fit_rates[i]
                + bg_tilt*(bin_centers[i]-ecenter)*bg_fit_rates[i];
    chi2 += mylikelihood(true_rates[i], fit_rate);

  }
  fchi1=chi2;
  
  chi2 += myprior(x[0], 0.0, errors[0])
            + myprior(x[1], 0.0, errors[1])
            + myprior(bg_norm, bg_norm_center, errors[2])
            + myprior(bg_tilt, bg_tilt_center, errors[3]);

  fchi2=chi2;
  priors[0]=myprior(x[0], 0.0, errors[0]);
  priors[1]=myprior(x[1], 0.0, errors[1]);
  priors[2]=myprior(bg_norm, bg_norm_center, errors[2]);
  priors[3]=myprior(bg_tilt, bg_tilt_center, errors[3]);
  //for(i=0; i<n_params; i++){
	//	xmin[i+15*exp]=x[i+15*exp];
	//}
  xmin[exp][0]=x[0];
  xmin[exp][1]=x[1];
  xmin[exp][2]=x[2];
  xmin[exp][3]=bg_tilt;
	
  
  
  /*for (i=ew_low; i <= ew_high; i++){
    double tfit_rate=signal_norm*signal_fit_rates[i]
                  + signal_tilt*(bin_centers[i]-ecenter)*signal_fit_rates[i]
                  + bg_norm*bg_fit_rates[i]
                  + bg_tilt*(bin_centers[i]-ecenter)*bg_fit_rates[i];
    ts[0][i]=true_rates[i];
    ts[1][i]=signal_fit_rates[i];
    ts[2][i]=tfit_rate;
    ts[3][i]=mylikelihood(true_rates[i],tfit_rate);
    ts[4][i]=2*(tfit_rate - true_rates[i]+ true_rates[i] * log(true_rates[i]/tfit_rate));
   

  }*/
           
  return chi2;
}

//same as globes, but customized to copy values to xmin for later use
//and to fix the pivot point elsewhere
double chiSpectrumTiltCustomPivot(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  //int n_bins = glbGetNumberOfBins(exp);
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double *bg_fit_rates     = glbGetBGFitRatePtr(exp, rule);
  double *bin_centers      = glbGetBinCentersListPtr(exp);
  double signal_norm, signal_tilt;
  double bg_norm_center, bg_tilt_center;
  double bg_norm, bg_tilt;
  int ew_low, ew_high;
  double emin, emax, ecenter;
  double fit_rate;//, true_rate
  double chi2 = 0.0;
  int i;

  glbGetEminEmax(exp, &emin, &emax);
  //ecenter = 0.5 * (emax + emin);
  ecenter = 1.5;
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  glbGetBGCenters(exp, rule, &bg_norm_center, &bg_tilt_center);
  signal_norm = 1.0 + x[0];
  signal_tilt = x[1] / (emax - emin);
  bg_norm = bg_norm_center * (1.0 + x[2]);
  bg_tilt = x[3] / (emax - emin);
  for (i=ew_low; i <= ew_high; i++)
  {
    fit_rate = signal_norm*signal_fit_rates[i]
                + signal_tilt*(bin_centers[i]-ecenter)*signal_fit_rates[i]
                + bg_norm*bg_fit_rates[i]
                + bg_tilt*(bin_centers[i]-ecenter)*bg_fit_rates[i];
    chi2 += mylikelihood(true_rates[i], fit_rate);

  }
  fchi1=chi2;
  
  chi2 += myprior(x[0], 0.0, errors[0])
            + myprior(x[1], 0.0, errors[1])
            + myprior(bg_norm, bg_norm_center, errors[2])
            + myprior(bg_tilt, bg_tilt_center, errors[3]);

  fchi2=chi2;
  priors[0]=myprior(x[0], 0.0, errors[0]);
  priors[1]=myprior(x[1], 0.0, errors[1]);
  priors[2]=myprior(bg_norm, bg_norm_center, errors[2]);
  priors[3]=myprior(bg_tilt, bg_tilt_center, errors[3]);
  //for(i=0; i<n_params; i++){
	//	xmin[i+15*exp]=x[i+15*exp];
	//}
  xmin[exp][0]=x[0];
  xmin[exp][1]=x[1];
  xmin[exp][2]=x[2];
  xmin[exp][3]=bg_tilt;
	
  
  
  /*for (i=ew_low; i <= ew_high; i++){
    double tfit_rate=signal_norm*signal_fit_rates[i]
                  + signal_tilt*(bin_centers[i]-ecenter)*signal_fit_rates[i]
                  + bg_norm*bg_fit_rates[i]
                  + bg_tilt*(bin_centers[i]-ecenter)*bg_fit_rates[i];
    ts[0][i]=true_rates[i];
    ts[1][i]=signal_fit_rates[i];
    ts[2][i]=tfit_rate;
    ts[3][i]=mylikelihood(true_rates[i],tfit_rate);
    ts[4][i]=2*(tfit_rate - true_rates[i]+ true_rates[i] * log(true_rates[i]/tfit_rate));
   

  }*/
           
  return chi2;
}


//same as globes, but customized to copy values to xmin for later use and to use B2B errors
double chiSpectrumTiltCustom_BtoB(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  //int n_bins = glbGetNumberOfBins(exp);
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double *bg_fit_rates     = glbGetBGFitRatePtr(exp, rule);
  double *bin_centers      = glbGetBinCentersListPtr(exp);
  double signal_norm, signal_tilt;
  double bg_norm_center, bg_tilt_center;
  double bg_norm, bg_tilt;
  int ew_low, ew_high;
  double emin, emax, ecenter;
  double fit_rate;//, true_rate
  double chi2 = 0.0;
  int i;

  glbGetEminEmax(exp, &emin, &emax);
  ecenter = 0.5 * (emax + emin);
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  glbGetBGCenters(exp, rule, &bg_norm_center, &bg_tilt_center);
  signal_norm = 1.0 + x[0];
  signal_tilt = x[1] / (emax - emin);
  bg_norm = bg_norm_center * (1.0 + x[2]);
  bg_tilt = x[3] / (emax - emin);
  for (i=ew_low; i <= ew_high; i++)
  {
    fit_rate = signal_norm*signal_fit_rates[i]
                + signal_tilt*(bin_centers[i]-ecenter)*signal_fit_rates[i]
                + bg_norm*bg_fit_rates[i]
                + bg_tilt*(bin_centers[i]-ecenter)*bg_fit_rates[i];
    //chi2 += glikelihood(true_rates[i], fit_rate);
    chi2 += glikelihood(true_rates[i], fit_rate,true_rates[i] * (1.0 + true_rates[i]*(sigma_binbin*sigma_binbin)));

  }
  fchi1=chi2;
  
  chi2 += myprior(x[0], 0.0, errors[0])
            + myprior(x[1], 0.0, errors[1])
            + myprior(bg_norm, bg_norm_center, errors[2])
            + myprior(bg_tilt, bg_tilt_center, errors[3]);

  fchi2=chi2;
  priors[0]=myprior(x[0], 0.0, errors[0]);
  priors[1]=myprior(x[1], 0.0, errors[1]);
  priors[2]=myprior(bg_norm, bg_norm_center, errors[2]);
  priors[3]=myprior(bg_tilt, bg_tilt_center, errors[3]);
  //for(i=0; i<n_params; i++){
	//	xmin[i+15*exp]=x[i+15*exp];
	//}
  xmin[exp][0]=x[0];
  xmin[exp][1]=x[1];
  xmin[exp][2]=x[2];
  xmin[exp][3]=bg_tilt;
	
  
  
  /*for (i=ew_low; i <= ew_high; i++){
    double tfit_rate=signal_norm*signal_fit_rates[i]
                  + signal_tilt*(bin_centers[i]-ecenter)*signal_fit_rates[i]
                  + bg_norm*bg_fit_rates[i]
                  + bg_tilt*(bin_centers[i]-ecenter)*bg_fit_rates[i];
    ts[0][i]=true_rates[i];
    ts[1][i]=signal_fit_rates[i];
    ts[2][i]=tfit_rate;
    ts[3][i]=mylikelihood(true_rates[i],tfit_rate);
    ts[4][i]=2*(tfit_rate - true_rates[i]+ true_rates[i] * log(true_rates[i]/tfit_rate));
   

  }*/
           
  return chi2;
}

/* File-scope static variables to store starting values and input errors */
//static glb_params tsv;
//static glb_params ter;
glb_params tsv;
glb_params ter;

int tmyprior_init()
{
  tsv=glbAllocParams();
  ter=glbAllocParams();
  glbGetStartingValues(tsv);
  glbGetInputErrors(ter);


  return 0;
}

/* myprior_clean is optional. It should free all memory reserved by
 * myprior_init and also reverse all side effects of myprior_init.
 *
 * Here we just free the memory for sv and er.
 */
void tmyprior_clean()
{
  glbFreeParams(ter);
  glbFreeParams(tsv);
}

/* This is a local function which implements the prior calculation
 * and ensure that the builtin prior behaves as described in the
 * documentation, i.e. it returns 0 when the error is below 1E-12
 */
static double tsprior(double x, double center, double sigma)
{
  if(fabs(sigma-0)<1E-12) return 0;
  return (x-center)*(x-center)/sigma/sigma;
}


/* myprior_prior is mandatory (for prior modules).
 *
 * This is the default prior, i.e. priors are only added for free
 * parameters, and the prior accepts starting values as stored in sv
 * and input errors as stored in er.
 */
double tmyprior_prior(const glb_params in, void *user_data)
{
  int i;
  double res=0;
  glb_projection pro;
  pro=glbAllocProjection();
  glbGetProjection(pro);
  for(i=0;i<GLB_OSCP;i++)
    {
      if(glbGetProjectionFlag(pro,i)==GLB_FREE) 
	{
      res +=
        tsprior(glbGetOscParams(in,i),
         glbGetOscParams(tsv,i),
         glbGetOscParams(ter,i)
         );                                              
      gpriors[i][0]=glbGetOscParams(in,i);
      gpriors[i][1]=glbGetOscParams(tsv,i);
      gpriors[i][2]=glbGetOscParams(ter,i);
      gpriors[i][3]=tsprior(glbGetOscParams(in,i),
         glbGetOscParams(tsv,i),
         glbGetOscParams(ter,i)
         );
	}
    }

  for(i=0;i<glb_num_of_exps;i++)
    {

      if(glbGetDensityProjectionFlag(pro,i)==GLB_FREE)
        res +=
          tsprior(glbGetDensityParams(in,i),
                 glbGetDensityParams(tsv,i),
                 glbGetDensityParams(ter,i)
                 );
        gpriors[6][0]=glbGetDensityParams(in,i);
        gpriors[6][1]=glbGetOscParams(tsv,i);
        gpriors[6][2]=glbGetDensityParams(tsv,i);
        gpriors[6][3]=tsprior(glbGetDensityParams(in,i),
                 glbGetDensityParams(tsv,i),
                 glbGetDensityParams(ter,i)
                 );
    }

  glbFreeProjection(pro);

  return res;
}

/* myprior_starting_values is optional. It is used to synchronize
 * the values set by glbSetStartingValues with the values used in here.
 * This function (if it exists) is called by glbSetStartingValues
 * with the same argument.
 *
 * Here we just copy the argument to our local buffer.
 */
int tmyprior_starting_values(const glb_params in, void *user_data)
{
  if(glbCopyParams(in,tsv)!=NULL) return 0;
  return -1;
}

/* myprior_input_errors is optional. It is used to synchronize
 * the values set by glbSetInputErrors with the values used in here.
 * This function (if it exists) is called by glbSetInputErrors
 * with the same argument.
 *
 * Here we just copy the argument to our local buffer.
 */
int tmyprior_input_errors(const glb_params in, void *user_data)
{
  if(glbCopyParams(in,ter)!=NULL) return 0;
  return -1;
}
