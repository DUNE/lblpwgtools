#include "customChis.h"



/***********************************************************************
//custom chi functions*/
double sigma_binbin = 0.05;
inline double myprior(double x, double center, double sigma)
{
  double tmp = (x - center)/sigma;
  return tmp*tmp;
}

//log likelihood ratio
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

/* Gaussian likelihood*/
inline double glikelihood(double true_rate, double fit_rate, double sqr_sigma)
{
  if (sqr_sigma > 0)
    return (true_rate - fit_rate)*(true_rate - fit_rate) / sqr_sigma;
  else
    return 0.0;
} 

//chisq function with nuisance parameter for each bg channel
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
  for(i=0;i<bg_channels;i++){
	  double * trates=glbGetChannelRatePtr(exp,i,GLB_POST);
		for(ebin=0;ebin<ebins;ebin++){
			bg_fit_rates_per_channel[i][ebin] = trates[ebin];
	  }
  }
  double bg_norm_center, bg_tilt_center;
  glbGetBGCenters(exp, rule, &bg_norm_center, &bg_tilt_center);
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
  for(i=0;i<bg_channels;i++){
	  double * trates=glbGetChannelRatePtr(exp,i,GLB_POST);
		for(ebin=0;ebin<ebins;ebin++){
			bg_fit_rates_per_channel[i][ebin] = trates[ebin];
	  }
  }
  double bg_norm_center=1.0;
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
  //add penalty for signal norm
  chi2 += myprior(x[0], 0.0, errors[0]);
  //add chi2 penalties for bg norms
  j=1;
  for(i=0; i<bg_channels; i++){
	  chi2 += myprior(bg_norm[i], bg_norm_center, errors[j]);
		j=j+1;
  }
  //copy parameter array into globally accessible variable
  for(i=0; i<n_params; i++){
		xmin[exp][i]=x[i];
	}
  
  return chi2;
}

//This adds in a bin to bin uncorrelated error to reflect
//imperfect knowledge of the shape in the spectrum.
double chiSpectrumSplitBG_BtoB(int exp, int rule, int n_params, double *x, double *errors,
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
  for(i=0;i<bg_channels;i++){
	  double * trates=glbGetChannelRatePtr(exp,i,GLB_POST);
		for(ebin=0;ebin<ebins;ebin++){
			bg_fit_rates_per_channel[i][ebin] = trates[ebin];
	  }
  }
  double bg_norm_center=1.0;
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
  //add penalty for signal norm
  chi2 += myprior(x[0], 0.0, errors[0]);
  //add chi2 penalties for bg norms
  j=1;
  for(i=0; i<bg_channels; i++){
	  chi2 += myprior(bg_norm[i], bg_norm_center, errors[j]);
		j=j+1;
  }
  //copy parameter array into globally accessible variable
  for(i=0; i<n_params; i++){
		xmin[exp][i]=x[i];
	}
  
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
  xmin[exp][0]=x[0];
  xmin[exp][1]=x[1];
  xmin[exp][2]=x[2];
  xmin[exp][3]=bg_tilt;
	
           
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

  xmin[exp][0]=x[0];
  xmin[exp][1]=x[1];
  xmin[exp][2]=x[2];
  xmin[exp][3]=bg_tilt;
           
  return chi2;
}


//same as globes, but customized to copy values to xmin for later use and to use B2B errors
double chiSpectrumTiltCustom_BtoB(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
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

  xmin[exp][0]=x[0];
  xmin[exp][1]=x[1];
  xmin[exp][2]=x[2];
  xmin[exp][3]=bg_tilt;
           
  return chi2;
}

//one nuisance parameter per bin
double chiNormPerBin(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double *bg_fit_rates     = glbGetBGFitRatePtr(exp, rule);
  int ew_low, ew_high;
  double fit_rate;

  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  fchi1=fchi2=0;
  for (int i=ew_low; i <= ew_high; i++){
    fit_rate = (1+x[i-ew_low])*(signal_fit_rates[i]+bg_fit_rates[i]);
    fchi1 += mylikelihood(true_rates[i], fit_rate);
    fchi2 += myprior(x[i-ew_low], 0.0, errors[i-ew_low]);
  }

  return fchi1+fchi2;
}


//Uses supplied Fast MC response functions (one per channel) to compute
//changes in event rates channel by channel.
//This is done by modifying pre-smearing efficiencies so they must not
//be in use for this to work.
double chi_ResponseFunction(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  //This function applies the response functions as a function of x to the true rates.
  //Assumes that pre smearing effs aren't in use (i.e they are set to 1).
  //Assumes first exp has systs enabled, rest are disabled
  double tresp;
  int rfi;
  double sigmult=1, bgmult=1;
  fchi1=fchi2=0;
  //ensure point is within boundaries
  for(int syst=0;syst<arguments.systs;syst++){
    if(x[syst]>5.0 || x[syst]<-5.0) return 1e10;
  }

  for(int curexp=0;curexp<glb_num_of_exps;curexp++){
    double *true_rates       = glbGetRuleRatePtr(curexp, rule);
    double *signal_fit_rates = glbGetSignalFitRatePtr(curexp, rule);
    double *bg_fit_rates     = glbGetBGFitRatePtr(curexp, rule);
    int sampbins = glbGetNumberOfSamplingPoints(curexp);
    int bins = glbGetNumberOfBins(curexp);
    int channels=glbGetNumberOfChannels(curexp); 

    //set pre smearing effs to response function values
    for(int ch=0;ch<channels;ch++){
      for(int sbin=0; sbin<sampbins; sbin++){
        tresp=0;
        //add up contribution from each systematic for this bin and channel
        for(int syst=0; syst<arguments.systs; syst++){
          if(x[syst] != 0.0){
            rfi=sampbins*arguments.systs*ch+sampbins*syst+sbin;
            tresp+=gsl_spline_eval(rf_spline[curexp][rfi], x[syst], rf_acc[curexp][rfi])-1;
          }
        }
        mult_presmear_effs[curexp][ch][sbin]=tresp<=-1?0:1+tresp; 
      }
    }

    //recompute rates using these multiplicative smearing effs
    mgt_set_new_rates(curexp, mult_presmear_effs);

    if(arguments.chimode==18){
      //populate signal and background normalization multipliers
      sigmult=1+x[arguments.systs+2*curexp];
      bgmult=1+x[arguments.systs+2*curexp+1];
      //add penalty term
      fchi2+=pow(x[arguments.systs+2*curexp]/errors[2*curexp],2)+pow(x[arguments.systs+2*curexp+1]/errors[2*curexp+1],2);
    }
    for (int ebin=0; ebin < bins; ebin++){
      fchi1 += mylikelihood(true_rates[ebin], sigmult*signal_fit_rates[ebin]+bgmult*bg_fit_rates[ebin]);
      if(arguments.debug==1){
        double a[]={ebin,true_rates[ebin],(signal_fit_rates[ebin]+bg_fit_rates[ebin]),x[0], arguments.systs>1?x[1]:0};
        AddArrayToOutput(a,5);
      }
    }

    if(arguments.debug==1){
      AddToOutputBlankline();
      AddToOutputBlankline();
    }
  }
  
  //add penalty term
  for(int syst=0;syst<arguments.systs;syst++)
    fchi2 += x[syst]*x[syst];

      
  return fchi1+fchi2;
}

//Loads file containing covariance matrix and/or nuisance parameter uncertainties
void LoadRFCovMatrix()
{
  FILE *covFile;
  float buf;
  if(!arguments.covmatrixfile){
    printf("Covariance matrix file location (covmatrixfile) not set! Exiting...\n");
    exit(1);
  }
			
  printf("Opening covmatrixfile file: %s\n",arguments.covmatrixfile);
  covFile = fopen(arguments.covmatrixfile, "r");
  if(covFile==NULL){
    printf("Covmatrixfile file does not exist, exiting!\n");
    exit(1);
  }
  
  rfCovMatrix = (double*)malloc(5*arguments.systs*sizeof(double));	
  
  for(int i=0; i<5*arguments.systs; i++){
    if(fscanf(covFile, "%f", &buf)==EOF){
      printf("Covar. matrix file has improper size. Exiting.\n");
      exit(1);
    }
    rfCovMatrix[i]=buf;
  }
              
  fclose(covFile);
}


//Similar to chi_responseFunction, but adds multiple nuisance parameters
//so that channels are not strictly correlated in their systematics responses
//TODO: also allows covariance matrix between numu nuisance parameters
double chi_ResponseFunctionCov(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  //x will have dimensions 5*number of systematics
  //5 nuisance parameters are numu, numubar/numu, nue/numu, nutau/numu, stat
  //this function expects channel order:
  //  numu, numubar, nue_bkg, nuebar_bkg, NC, nue_sig, nuebar_sig, nutau, nutaubar
  double tresp;
  double xsigma=0;
  int rfi;
  fchi1=fchi2=0;

  for(int curexp=0;curexp<glb_num_of_exps;curexp++){
    double *true_rates       = glbGetRuleRatePtr(curexp, rule);
    double *signal_fit_rates = glbGetSignalFitRatePtr(curexp, rule);
    double *bg_fit_rates     = glbGetBGFitRatePtr(curexp, rule);
    int sampbins = glbGetNumberOfSamplingPoints(curexp);
    int bins = glbGetNumberOfBins(curexp);
    int channels=glbGetNumberOfChannels(curexp); 

    //set pre smearing effs to response function values
    for(int ch=0;ch<channels;ch++){
      for(int sbin=0; sbin<sampbins; sbin++){
        tresp=0;
        //add up contribution from each systematic for this bin and channel
        for(int syst=0; syst<arguments.systs; syst++){
          //compute sigma contribution for this channel based on nuisance parameters
          xsigma=0;
          switch(ch){
            case 0: case 4://numu, NC
              xsigma=x[5*syst];
              break;
            case 1: //numubar
              xsigma=x[5*syst]+x[5*syst+1] + x[5*syst+4];
              break;
            case 2: case 5: //nue_bkg, nue_sig
              xsigma=x[5*syst]+x[5*syst+2] + x[5*syst+4];
              break;
            case 3: case 6: //nuebar_bkg, nuebar_sig
              xsigma=x[5*syst]+x[5*syst+1]+x[5*syst+2] + x[5*syst+4];
              break;
            case 7: //nutau
              xsigma=x[5*syst]+x[5*syst+3] + x[5*syst+4];
              break;
            case 8: //nutaubar
              xsigma=x[5*syst]+x[5*syst+1]+x[5*syst+3] + x[5*syst+4];
              break;
          }
          if(xsigma>5.0 || xsigma<-5.0) return 1e10;
          if(x[syst] != 0.0){
            rfi=sampbins*arguments.systs*ch+sampbins*syst+sbin;
            tresp+=gsl_spline_eval(rf_spline[curexp][rfi], xsigma, rf_acc[curexp][rfi])-1;
          }
        }
        mult_presmear_effs[curexp][ch][sbin]=tresp<=-1?0:1+tresp; 
      }
    }

    //recompute rates using these multiplicative smearing effs
    mgt_set_new_rates(curexp, mult_presmear_effs);

    for (int ebin=0; ebin < bins; ebin++){
      fchi1 += mylikelihood(true_rates[ebin], signal_fit_rates[ebin]+bg_fit_rates[ebin]);
      if(arguments.debug==1){
        double a[]={ebin,true_rates[ebin],(signal_fit_rates[ebin]+bg_fit_rates[ebin]),x[0], x[1], x[2], x[3], x[4]};
        AddArrayToOutput(a,8);
      }
    }

    if(arguments.debug==1){
      AddToOutputBlankline();
      AddToOutputBlankline();
    }
  }
  
  //add penalty term
  for(int syst=0;syst<arguments.systs;syst++){
    for(int i=0;i<5;i++){
      fchi2 += x[5*syst+i]*x[5*syst+i]/pow(rfCovMatrix[5*syst+i],2);
    }
  }
      
  return fchi1+fchi2;
}


// matrix inversioon
// the result is put in Y
void MatrixInversion(float **A, int order, float **Y){
  // get the determinant of a
  double det = 1.0/CalcDeterminant(A,order);
 
  // memory allocation
  float** minor;  
  minor = (float**) malloc((order-1)*sizeof(float*));  
  for(int i = 0; i < (order-1); i++) minor[i] = (float*) malloc((order-1)*sizeof(float)); 

 
  for(int j=0;j<order;j++){
    for(int i=0;i<order;i++){
      // get the co-factor (matrix) of A(j,i)
      GetMinor(A,minor,j,i,order);
      Y[i][j] = det*CalcDeterminant(minor,order-1);
      if( (i+j)%2 == 1) Y[i][j] = -Y[i][j];
    }
  }
 
  // release memory
  for(int i = 0; i < (order-1); i++) free(minor[i]);   
  free(minor);
  
}
 
// calculate the cofactor of element (row,col)
int GetMinor(float **src, float **dest, int row, int col, int order){

  // indicate which col and row is being copied to dest
  int colCount=0,rowCount=0;

  for(int i = 0; i < order; i++ ){
    if( i != row ){
      colCount = 0;
      for(int j = 0; j < order; j++ ){
        // when j is not the element
        if( j != col ){
          dest[rowCount][colCount] = src[i][j];
          colCount++;
        }
      }
      rowCount++;
    }
  }
 
  return 1;

}
 
// Calculate the determinant recursively.
double CalcDeterminant( float **mat, int order){

  // order must be >= 0
  // stop the recursion when matrix is a single element
  if( order == 1 ) return mat[0][0];
 
  // the determinant value
  float det = 0;
 
  // allocate the cofactor matrix
  float** minor;
  minor = (float**) malloc((order-1)*sizeof(float*));
  for(int i = 0; i < (order-1); i++) minor[i] = (float*) malloc((order-1)*sizeof(float));
 
  for(int i = 0; i < order; i++ ){
    // get minor of element (0,i)
    GetMinor( mat, minor, 0, i , order);
    // the recusion is here!
 
    det += (i%2==1?-1.0:1.0) * mat[0][i] * CalcDeterminant(minor,order-1);
    //det += pow( -1.0, i ) * mat[0][i] * CalcDeterminant( minor,order-1 );
  }
 
  // release memory
  for(int i = 0; i < (order-1); i++) free(minor[i]);
  free(minor);
 
  return det;

}

void matrixPrint(const gsl_matrix* M, const int n){
	printf("[");
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			printf("%1.15e",gsl_matrix_get (M, i, j));
			if(j!=n-1) printf(",");
		}
		printf( "],\n[");
	}
}

void pinv(const gsl_matrix* in, const int n, gsl_matrix* Inv_Matrix){
  //pseudo inverse of n x n matrix in is stored in Inv_Matrix
  gsl_matrix* U = gsl_matrix_alloc(n, n);
  gsl_matrix_memcpy(U,in);

  gsl_matrix* V    = gsl_matrix_alloc(n, n);
  gsl_vector* S    = gsl_vector_alloc(n);
  gsl_vector* work = gsl_vector_alloc(n);

  gsl_linalg_SV_decomp_jacobi(U, V, S);
  gsl_matrix_transpose(V);
   
  gsl_matrix* S_inv = gsl_matrix_alloc(n,n);
  gsl_matrix_set_zero(S_inv);
  for(int i=0; i<n; i++){
    double s_i = gsl_vector_get(S, i);
    double inv_s_i = 0.0;
		if(s_i>=1e-6) inv_s_i = 1.0/s_i;
		gsl_matrix_set(S_inv,i,i,inv_s_i);
  }
  gsl_matrix* UxS_inv = gsl_matrix_alloc(n, n);
  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, U, S_inv, 0.0, UxS_inv);
  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, UxS_inv, V, 0.0, Inv_Matrix);

  gsl_vector_free(S);
  gsl_vector_free(work);
  gsl_matrix_free(V);
  gsl_matrix_free(U);
  gsl_matrix_free(S_inv);
  gsl_matrix_free(UxS_inv);
}

void LUinv(const gsl_matrix* in, const int n, gsl_matrix* Inv_Matrix){
	//LU inverse
	gsl_permutation * p = gsl_permutation_alloc (n);
	gsl_matrix* U = gsl_matrix_alloc(n, n);
	gsl_matrix_memcpy(U,in);
	int s;
	gsl_linalg_LU_decomp (U, p, &s);
	gsl_linalg_LU_invert (U, p, Inv_Matrix);
}

double getDet(const gsl_matrix* in,int ebins){
		int s;
		gsl_permutation* perm = gsl_permutation_alloc(ebins);
		gsl_matrix* LUD_Matrix = gsl_matrix_alloc(ebins, ebins);
		gsl_matrix_memcpy(LUD_Matrix, in);
		gsl_linalg_LU_decomp(LUD_Matrix, perm, &s);
		return gsl_linalg_LU_det(LUD_Matrix,s);
}

void testerrorMatrix(int exp,int channels, int ebins){
	//test pos defness of errorMatrix
	for(int k=0;k<channels;k++){
		//for(int kp=0;kp<channels;kp++){
		for(int kp=k;kp<=k;kp++){
			gsl_matrix* subMatrix = gsl_matrix_alloc(ebins,ebins);
			printf("printing submatrix: %d,%d\n[",k,kp);
			for(int i=0; i<ebins; i++){
				for(int j=0; j<ebins; j++){
					gsl_matrix_set(subMatrix,i,j,errorMatrix[exp][(channels*ebins)*(kp*ebins+j)+(k*ebins+i)]);
					printf(((j==ebins-1) ? "%f],\n[" : "%f,\t"),gsl_matrix_get(subMatrix,i,j));
				}
			}
			  //try cholesky decomp too
			  gsl_set_error_handler_off();
			  gsl_matrix* cinv_Matrix = gsl_matrix_alloc(ebins,ebins);
			  gsl_matrix_memcpy(cinv_Matrix,subMatrix);
			  int cholRet=gsl_linalg_cholesky_decomp(cinv_Matrix);
			  if(cholRet==1){
				printf("Cholesky decomposition failed!\n");
			  }else{
				printf("Cholesky decomp OK!!!\n");
			  }
			  //gsl_linalg_cholesky_invert(cinv_Matrix);
			  
			  gsl_matrix_free(subMatrix);
			  gsl_matrix_free(cinv_Matrix);
		}
	}
	
}


int firstLoop[]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; //tracks initialization of variables
int cholRet[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //stores return value of previous cholesky decomp.
double chiSpectrum_ErrorMatrix(int exp, int rule, int n_params, double *x, double *errors, void *user_data){
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double *bg_fit_rates     = glbGetBGFitRatePtr(exp, rule);

  int ew_low, ew_high;
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  double emin, emax;
  glbGetEminEmax(exp, &emin, &emax);
  int ebins = 0;
  for(int i=ew_low; i<=ew_high; i++) ebins++;
  int channels=glbGetNumberOfChannels(exp);
  double *channel_rates[channels];
	double sum_i = 0.0;
	double sum_j = 0.0;
	int checkDif=0; //flag for if true rates have changed
	//check to see if fit rates have changed since the last call
	if(firstLoop[exp]!=1){
		for(int i=0; i<ebins; i++){
			if(prior_fit_rates[exp][i]!=signal_fit_rates[i]){
				//if(checkDif==0) dprintf("True rates have changed...\n");
				checkDif=1;
        prior_fit_rates[exp][i]=signal_fit_rates[i];
			}

		}
	}else{
    //initialize dynamic variables if it's the first call for this exp
    Total_Input_Matrix[exp] = gsl_matrix_alloc(ebins, ebins);
		Inv_Matrix[exp] = gsl_matrix_alloc(ebins,ebins);
    prior_fit_rates[exp]=(double*)malloc(ebins*sizeof(double));
  }
  
  //if(checkTruevsFit==0) return 0;
  
	if(arguments.debug==1){
		printf("True/Test Signal Spectra: \n");
		for(int i=0; i<ebins; i++){
			printf("%f,",true_rates[i]);
		}
		printf("\n\n");
		for(int i=0; i<ebins; i++){
			printf("%f,",signal_fit_rates[i]+bg_fit_rates[i]);
		}
		printf("\n\n");
	}
	
	if(firstLoop[exp]==1 || checkDif==1){
		//create array of channel rates
		for(int curchan=0;curchan<channels;curchan++){
			channel_rates[curchan]=glbGetChannelFitRatePtr(exp,curchan,GLB_POST);
		}
		//gsl_matrix* test_total_matrix = gsl_matrix_alloc(ebins*channels, ebins*channels);
		//create collapsed input matrix
		for(int i=0; i<ebins; i++){
			for(int j=0; j<ebins; j++){
				sum_i=0.0;sum_j=0.0;
				for(int k=0;k<channels;k++){
					for(int kp=0;kp<channels;kp++){
							//dprintf("i=%d,j=%d,k=%d,kp=%d,channel_rates[k][i]=%f,channel_rates[kp][i]=%f,errorMatrix[%d][%d]=%f, mult=%f\n",i,j,k,kp,channel_rates[k][i],channel_rates[kp][j],(kp*ebins+j),(k*ebins+i),errorMatrix[exp][(channels*ebins)*(kp*ebins+j)+(k*ebins+i)],channel_rates[k][i]*channel_rates[kp][j]*errorMatrix[exp][(channels*ebins)*(kp*ebins+j)+(k*ebins+i)]);
							//gsl_matrix_set(test_total_matrix,(k*ebins+i),(kp*ebins+j),gsl_matrix_get(test_total_matrix,(k*ebins+i),(kp*ebins+j))+channel_rates[k][i]*channel_rates[kp][j]*errorMatrix[exp][(channels*ebins)*(kp*ebins+j)+(k*ebins+i)]);
							sum_i+=(channel_rates[k][i]*channel_rates[kp][j]*errorMatrix[exp][(channels*ebins)*(kp*ebins+j)+(k*ebins+i)]);
							
							//if(i==15 && j==15) printf("k=%d, kp=%d, er[k]=%f,er[kp]=%f,em[%d]=%f,prod=%f\n",k, kp,channel_rates[k][i],channel_rates[kp][j],(channels*ebins)*(kp*ebins+j)+(k*ebins+i),errorMatrix[exp][(channels*ebins)*(kp*ebins+j)+(k*ebins+i)],(channel_rates[k][i]*channel_rates[kp][j]*errorMatrix[exp][(channels*ebins)*(kp*ebins+j)+(k*ebins+i)]));
					}
					
				}
				gsl_matrix_set(Total_Input_Matrix[exp], i, j, (sum_i));
				//gsl_matrix_set(Total_Input_Matrix[exp], i, j, i==j?1+sum_i:sum_i);
			}
		}
		
		/*double tsum=0.0;
		for(int i=0;i<ebins*channels;i++)
			for(int j=0;j<ebins*channels;j++){
					printf((i==j?"\n%f,":"%f,"),gsl_matrix_get(test_total_matrix,i,j));
					tsum=tsum+gsl_matrix_get(test_total_matrix,i,j);
				}
					
		printf("tsum=%f\n\n",tsum);*/
		
		
		//try pseudo inversion too
		//this doesn't seem to work generally (tested with a 10x10 diagonal matrix)
		//pinv(Total_Input_Matrix[exp],ebins,Inv_Matrix[exp]);
		
		//set a minimum value on diagonal elements
		/*gsl_set_error_handler_off();
		double det=0;
		for(int j=0;j<100;j++){
			gsl_matrix_memcpy(Inv_Matrix[exp],Total_Input_Matrix[exp]);
			cholRet[exp]=gsl_linalg_cholesky_decomp(Inv_Matrix[exp]);
			det=getDet(Total_Input_Matrix[exp],ebins);
			if(cholRet[exp]==0){
				gsl_linalg_cholesky_invert(Inv_Matrix[exp]);
				break;
			}else{
				printf("Cholesky failed... determinant of total input matrix is %f, augmenting...\n",det);
				for(int i=0;i<ebins;i++){
					gsl_matrix_set(Total_Input_Matrix[exp],i,i,1.0000001*gsl_matrix_get(Total_Input_Matrix[exp],i,i));
					//gsl_matrix_set(Total_Input_Matrix[exp],i,i,1+gsl_matrix_get(Total_Input_Matrix[exp],i,i));
				}
				cholRet[exp]=0;
				pinv(Total_Input_Matrix[exp],ebins,Inv_Matrix[exp]);
				break;
			}
		}*/
		
		//try cholesky decomp
		gsl_matrix_memcpy(Inv_Matrix[exp],Total_Input_Matrix[exp]);
		cholRet[exp]=gsl_linalg_cholesky_decomp(Inv_Matrix[exp]);
    //cholRet[exp]=1; //uncomment to force everything to pinv
		if(cholRet[exp]==1){
				dprintf("Cholesky decomposition failed!\nComputing pseudo-inverse...\n");
				pinv(Total_Input_Matrix[exp],ebins,Inv_Matrix[exp]);
    }
		
		//optional debug output
		if(arguments.debug==1){
			//testerrorMatrix(exp,channels,ebins);
			sum_i=0.0;
			printf("Absolute ebins x ebins ematrix is:\n[");
			double var=0.0;
			for(int i=0; i<ebins; i++){
        sum_i+=gsl_matrix_get(Total_Input_Matrix[exp], i, i);
				for(int j=0; j<ebins; j++){
					printf((j==ebins-1 ? "%1.15e\t" : "%1.15e,\t"),gsl_matrix_get(Total_Input_Matrix[exp], i, j));
					var+=gsl_matrix_get(Total_Input_Matrix[exp], i, j);
				}
				printf("],\n[");
			}
			printf("Total Input Matrix Det,Tr: %e,%e\n",getDet(Total_Input_Matrix[exp],ebins),sum_i);
			printf("Inverted matrix:\n[");
      sum_i=0.0;
			for(int i=0; i<ebins; i++){
        sum_i+=gsl_matrix_get(Inv_Matrix[exp], i, i);
				for(int j=0; j<ebins; j++){
					printf((j==ebins-1 ? "%1.15e\t" : "%1.15e,\t"),gsl_matrix_get(Inv_Matrix[exp], i, j));
				}
				printf("],\n[");
			}
      printf("Inverse Matrix Det,Tr: %e,%e\n",getDet(Inv_Matrix[exp],ebins),sum_i);
			
		}
	}
	
	sum_i=0.0;
 
  //get number of bins and the bin to bin fluctuations
  gsl_vector* bg_bin_flucts = gsl_vector_alloc(ebins);
  gsl_vector* signal_bin_flucts = gsl_vector_alloc(ebins);
	gsl_vector* total_flucts = gsl_vector_alloc(ebins);
  for(int i=0; i<ebins; i++){
    gsl_vector_set(bg_bin_flucts, i, x[i]*bg_fit_rates[i+ew_low]);
    gsl_vector_set(signal_bin_flucts, i, x[i]*signal_fit_rates[i+ew_low]);
    gsl_vector_set(total_flucts, i, x[i]*(signal_fit_rates[i+ew_low]+bg_fit_rates[i+ew_low]));
    //check that fluctuations do not generate negative event rates
    if(gsl_vector_get(signal_bin_flucts,i)+signal_fit_rates[i]<0)
			gsl_vector_set(signal_bin_flucts, i, -signal_fit_rates[i]);
		if(gsl_vector_get(bg_bin_flucts,i)+bg_fit_rates[i]<0)
			gsl_vector_set(bg_bin_flucts, i, -bg_fit_rates[i]);
			
  }

  //compute the Poisson likelihood chi2
  double chi2 = 0.0;
  double chi2_1 = 0.0;
  for(int i=0; i<ebins; i++){
    chi2_1 = mylikelihood( true_rates[i],signal_fit_rates[i] + gsl_vector_get(signal_bin_flucts,i)+ bg_fit_rates[i] + gsl_vector_get(bg_bin_flucts,i));
    chi2_1=chi2_1<0.0?0:chi2_1;
    //printf("%f\t%f",sum_sig + gsl_vector_get(signal_bin_flucts,i) + gsl_vector_get(bg_bin_flucts,i) + sum_bkg,sum_data);
    dprintf("chi2_1 = %e    sig = %e   bkg = %e     Nobs = %e\n",chi2_1,gsl_vector_get(signal_bin_flucts, i)+signal_fit_rates[i+ew_low], gsl_vector_get(bg_bin_flucts, i)+bg_fit_rates[i+ew_low], true_rates[i+ew_low]);
    chi2 += chi2_1;
  }
  dprintf("chi2 1 = %e\n",chi2);


	//compute penalty term for the bin to bin fluctuations = [f][V^-1][f*]
	//multipy out f V^-1 f* and add it to the chi2
	double chi22=0.0;
	if(cholRet[exp]==0){
		//we have a cholesky decomposition
		//compute penalty: (x^T)(b=Lx)
		gsl_linalg_cholesky_svx(Inv_Matrix[exp],total_flucts);
		for(int i=0; i<ebins; i++){
			chi22+=gsl_vector_get(total_flucts, i)*(gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i));
		}
	}else{
		//we have a pseudo-inverse
    for(int i=0; i<ebins; i++){
      //don't allow variations in x_i's that have no constraints
      if(x[i]!=0.0 && gsl_matrix_get(Inv_Matrix[exp], i, i)==0.0){
        chi22=1e8;
        break;
      }
      for(int j=0; j<ebins; j++){
          chi22 += (gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)) * gsl_matrix_get(Inv_Matrix[exp], i, j)  * (gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j));
          //dprintf("inv_chi2=%e=(%e)*(%e)*(%e)\n",(gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)) * gsl_matrix_get(Inv_Matrix[exp], i, j)  * (gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j)),(gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)),gsl_matrix_get(Inv_Matrix[exp], i, j),(gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j)));
          //non-inverted version:
          //if( (gsl_matrix_get(Inv_Matrix[exp], i, j))>1e-4) chi22 += (i==j?1:-1)*(gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)) * (1/gsl_matrix_get(Inv_Matrix[exp], i, j))  * (gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j));
          //if((gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)) * (1/gsl_matrix_get(Inv_Matrix[exp], i, j))  * (gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j))>0)
          //	dprintf("inv_chi2=%e=(%e)*(%e)*(%e)\n",(i==j?1:-1)*(gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)) * (1/gsl_matrix_get(Inv_Matrix[exp], i, j))  * (gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j)),(gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)),(i==j?1:-1)*(1/gsl_matrix_get(Inv_Matrix[exp], i, j)),(gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j)));
      }
    }
  }

	/*if(chi22>0)*/ chi2+=chi22;
	dprintf("chi22:%e\n",chi22);
	//printf("chi22_2:%e\n",chi22_2);

  //copy parameter array into globally accessible variable
  for(int i=0; i<n_params; i++){
    xmin[exp][i]=x[i];
  }


  //if(chi2<0 || chi22<0 ||chi22==1e8){
  if(chi2<0){
		//printf("chi2 is negative, exiting!\n");
		//exit(0);
		chi2=1e10;
	} 
	
  if(arguments.debug==1){
    //printf("n_params = %d, ebins=%d\n",n_params, ebins);
    printf("chi2 = %e\n",chi2);
    for(int i=0; i<n_params; i++) printf("x[%d] = %g, ",i,x[i]);
    printf("\n");
	//exit(0);
  }

  
  //print fit and true vectors in animiateable format for testing
  //double *bc = glbGetBinCentersListPtr(exp);
  //for(int i=0;i<ebins;i++){
	//	printf("%d,%f,%f,%f\n",i,bc[i],true_rates[i],signal_fit_rates[i]+bg_fit_rates[i]+gsl_vector_get(signal_bin_flucts,i)+gsl_vector_get(bg_bin_flucts,i));
	//}
	//printf("\n\n");
  
  //copy true rates into prior_true_rates
  if(firstLoop[exp]==1) firstLoop[exp]=0;
  
  gsl_vector_free(signal_bin_flucts);
  gsl_vector_free(bg_bin_flucts);
  gsl_vector_free(total_flucts);

  return chi2;
}


double my_prior_t23_opoct(const glb_params in, void* user_data)
{
	/* This prior only contrains t23 to the opposite octant
	 * all other parameters are treated normally.
	 * It does this by keeping t23 fit in the same octant as the central
	 * value. The inputerror on t23 has no effect for this prior.
	*/
	glb_params central_values = glbAllocParams();
	glb_params input_errors = glbAllocParams();
	glb_projection p = glbAllocProjection();
	glbGetCentralValues(central_values);
	glbGetInputErrors(input_errors);
	glbGetProjection(p);
	int i;
	double pv = 0.0;
	double fitvalue,centralvalue,inputerror;
	/* Add oscillation parameter priors */
	for(i=0;i<6;i++)
		if(glbGetProjectionFlag(p,i)==GLB_FREE){
			fitvalue=glbGetOscParams(in,i);
			centralvalue=glbGetOscParams(central_values,i);
			inputerror=glbGetOscParams(input_errors,i);
			if(i==GLB_THETA_23){
				if(centralvalue>M_PI/4){
					if(fitvalue<M_PI/4) pv+=1e6;
				}else{
					if(fitvalue>M_PI/4) pv+=1e6;
				}
			}else{
				if(inputerror>1e-12) pv+=pow((centralvalue-fitvalue)/inputerror,2);
			}
		}
	/* Add matter parameter priors */
	for(i=0;i<glb_num_of_exps;i++)
		if(glbGetDensityProjectionFlag(p,i)==GLB_FREE){
			fitvalue=glbGetDensityParams(in,i);
			centralvalue=1.0;
			inputerror=glbGetDensityParams(input_errors,i);
			if(inputerror>1e-12)
				pv+=pow((centralvalue-fitvalue)/inputerror,2);
		}
		
	glbFreeParams(central_values);
	glbFreeParams(input_errors);
	glbFreeProjection(p);
	return pv;
}
