#include "customChis.h"



/***********************************************************************
//custom chi functions*/
double sigma_binbin = 0.05;
double myprior(double x, double center, double sigma)
{
  double tmp = (x - center)/sigma;
  return tmp*tmp;
}

//log likelihood ratio
double mylikelihood(double true_rate, double fit_rate)
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
double glikelihood(double true_rate, double fit_rate, double sqr_sigma)
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
		xmin[exp][rule][i]=x[i];
	}
  
  return chi2;
}

//same as above, but tilt is removed
double chiSpectrumSplitBG(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  //Note that anything done pre 2014 with this function was using glbGetChannelRatePtr instead of glbGetChannelFitRatePtr
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
	  double * trates=glbGetChannelFitRatePtr(exp,i,GLB_POST);
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
		xmin[exp][rule][i]=x[i];
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
		xmin[exp][rule][i]=x[i];
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
  
  fchi2=0;
  fchi2 += myprior(x[0], 0.0, errors[0])
            + myprior(x[1], 0.0, errors[1])
            + myprior(bg_norm, bg_norm_center, errors[2])
            + myprior(bg_tilt, bg_tilt_center, errors[3]);

  priors[0]=myprior(x[0], 0.0, errors[0]);
  priors[1]=myprior(x[1], 0.0, errors[1]);
  priors[2]=myprior(bg_norm, bg_norm_center, errors[2]);
  priors[3]=myprior(bg_tilt, bg_tilt_center, errors[3]);
  xmin[exp][rule][0]=x[0];
  xmin[exp][rule][1]=x[1];
  xmin[exp][rule][2]=x[2];
  xmin[exp][rule][3]=bg_tilt;
	
  //printf("%f, %f, %f\n", fchi1, fchi2, chi2);
  return fchi1+fchi2;
}

double chiSpectrumNormCustom(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data){
  //only sig/bg norm systematics (no tilt)
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double *bg_fit_rates     = glbGetBGFitRatePtr(exp, rule);
  int ew_low, ew_high;
  double fit_rate;//, true_rate
  double chi2 = 0.0;
  
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  for (int i=ew_low; i <= ew_high; i++){
    fit_rate = (1+x[0])*signal_fit_rates[i]
                + (1+x[1])*bg_fit_rates[i];
    chi2 += mylikelihood(true_rates[i], fit_rate);

  }
  fchi1=chi2;
  
  fchi2 = myprior(x[0], 0.0, errors[0]) + myprior(x[1], 0.0, errors[1]);
  xmin[exp][rule][0]=x[0];
  xmin[exp][rule][1]=x[1];

  return fchi1+fchi2;
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

  xmin[exp][rule][0]=x[0];
  xmin[exp][rule][1]=x[1];
  xmin[exp][rule][2]=x[2];
  xmin[exp][rule][3]=bg_tilt;
           
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

  xmin[exp][rule][0]=x[0];
  xmin[exp][rule][1]=x[1];
  xmin[exp][rule][2]=x[2];
  xmin[exp][rule][3]=bg_tilt;
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

double chiCorrSplitBGs_LBNEFMC(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  //4 nuisance parameters: f_sig, f_nue, f_nutau, f_nc+numu
  //correlated between
  //Assumes LBNE FMC ordering of channels: 
  //    0      1          2           3     4      5       6           7      8
  //    numu, numubar, nue_bkg, nuebar_bkg, NC, nue_sig, nuebar_sig, nutau, nutaubar
  //4 experiments in order: nue,nuebar,numu,numubar  (1 rule/exp)
  
  fchi1=fchi2=0;

  for(int curexp=0;curexp<glb_num_of_exps;curexp++){
    double *true_rates       = glbGetRuleRatePtr(curexp, rule);
    //double *signal_fit_rates = glbGetSignalFitRatePtr(curexp, rule);
    //double *bg_fit_rates     = glbGetBGFitRatePtr(curexp, rule);
    int bins = glbGetNumberOfBins(curexp);
    int channels=glbGetNumberOfChannels(curexp); 

    //get pointers to channels for this exp
    double *cr[channels];
		for(int curchan=0;curchan<channels;curchan++){
			cr[curchan]=glbGetChannelFitRatePtr(curexp,curchan,GLB_POST);
		}
    
    //stat term of chisq
    if(curexp==0 || curexp==1){ //appearance
      for (int e=0; e < bins; e++){
        fchi1 += mylikelihood(true_rates[e], (1+x[0])*(cr[5][e]+cr[6][e])  //f_sig (nue)
                              + (1+x[1])*(cr[2][e]+cr[3][e]) //f_nue
                              + (1+x[2])*(cr[7][e]+cr[8][e]) //f_nutau
                              + (1+x[3])*(cr[0][e]+cr[1][e]+cr[4][e]) //f_nc+numu
                              );
      }
    }else{ //disappearance
      for (int e=0; e < bins; e++){
        fchi1 += mylikelihood(true_rates[e], 
                              (1+x[0])*(cr[0][e]+cr[1][e])  //f_sig (numu)
                              + (1+x[1])*(cr[2][e]+cr[3][e]+cr[5][e]+cr[6][e]) //f_nue (beam + osc)
                              + (1+x[2])*(cr[7][e]+cr[8][e]) //f_nutau
                              + (1+x[3])*(cr[4][e]) //f_nc+numu (only nc for disapp)
                              );
      }   
    }
    //printf("%d,%d, %f, %f, %f,,,%f,%f,%f,%f\n", curexp,glb_num_of_exps, fchi1, fchi2, fchi1+fchi2,x[0],x[1],x[2],x[3]);
  }
  
  //penalty term + store nuisances
  for(int i=0;i<4;i++){
        fchi2 += pow(x[i]/errors[i],2);
        xmin[0][0][i]=x[i];
  }
  //printf("%f, %f, %f,,,%f,%f,%f,%f\n", fchi1, fchi2, fchi1+fchi2,x[0],x[1],x[2],x[3]);
  return fchi1+fchi2;
}

void LoadRFCovMatrixInputs(){
  //Loads file containing covariance matrix inputs
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

  //get extracted response value for each systematic
  double responses[arguments.systs];
  for(int syst=0;syst<arguments.systs;syst++){
    responses[syst]=extract1sResponse(syst);
    printf("Found response for syst %d: %f\n",syst, responses[syst]);
  }
  
  //load array of (4*syst) values
  //the array has 12 values per systematic,
  //the first 4 of every syst are populated from the file
  //the rest are populated from the 1s response extraction
  rfCovMatrixInputs = (double*)malloc(12*arguments.systs*sizeof(double));
  printf("Setting sigmas to:\n");
  for(int i=0; i<12*arguments.systs; i++){
    if(fscanf(covFile, "%f", &buf)==EOF){
      printf("Covar. matrix file has improper size. Exiting.\n");
      exit(1);
    }
    rfCovMatrixInputs[i]=buf;
    if(i%12>=4){
      //add extracted response value
      rfCovMatrixInputs[i]+=responses[i/12];
    }
    //if anything is zero, set to be small so that it is tightly constrained
    //and won't cause a divide by zero error
    //printf("rvCovMatrixInputs: %f\n",rfCovMatrixInputs[i]);
    if(fabs(rfCovMatrixInputs[i])<1e-8){
      rfCovMatrixInputs[i]=1e-8;
    }
    printf("%d, %f\n", i, rfCovMatrixInputs[i]);
  }
  fclose(covFile);
}

void SetupRFCovMatrix(){
  //sets up gsl matrix object containing V^-1 covariance matrix
  //V has systs*9 by systs*9 elements
  
  LoadRFCovMatrixInputs();
  printf("inp=%f,%f\n",rfCovMatrixInputs[0],rfCovMatrixInputs[1]);
  //initialize the globally defined inverse covariance matrix
  rfCovMatrix = gsl_matrix_alloc(arguments.systs*9, arguments.systs*9);
  //initialize matrix to identity so penalty term is just sigma^2
  gsl_matrix_set_identity(rfCovMatrix);
  //compute 1s reponse that makes up sigma_stat
  double s_stat=extract1sResponse();
  printf("s_stat=%f\n",s_stat);

  //create nine rotation matrices
  //gsl_matrix* m_rot[11]; //extra elements are to store product
  //for(int i=0;i<11;i++){
  //  m_rot[i]=gsl_matrix_alloc(arguments.systs*9, arguments.systs*9);
  //  gsl_matrix_set_zero(m_rot[i]);
  //}
  
  gsl_matrix* cov=gsl_matrix_alloc(arguments.systs*9, arguments.systs*9); // covar matrix
  gsl_matrix_set_zero(cov);
  matrixPrint(cov, 9*arguments.systs);
    
  //populate them
  //double sig1=0,sig2=0;
  //sig1=pow(1+rfCovMatrixInputs[0]+s_stat,2);
  /*for(int i=0;i<9;i++){
    sig2=pow(rfCovMatrixInputs[i]+s_stat,2);
    gsl_matrix_set(m_rot[i],0,0,(sig1+sig2)/2);
    gsl_matrix_set(m_rot[i],0,i,(sig1-sig2)/2);
    gsl_matrix_set(m_rot[i],i,0,(sig1-sig2)/2);
    gsl_matrix_set(m_rot[i],i,i,(sig1+sig2)/2);
    matrixPrint(m_rot[i], 9*arguments.systs);
  }*/
  
  double s[9];
  s[0]=1;
  printf("s:%f\n",s[0]);
  for(int i=1;i<9;i++){
    s[i]=pow(rfCovMatrixInputs[i]+s_stat,2);
    printf("s:%f\n",s[i]);
  }

  

  for (int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      //if(i==1&&j==1) gsl_matrix_set(cov,i,j,(2*s[0]+s[1]+s[2]+s[3]+s[4]+s[5]+s[6]+s[7]+s[8])/2);
      if(j==i) gsl_matrix_set(cov,i,j,(s[0]+s[1]+s[2]+s[3]+s[4]+s[5]+s[6]+s[7]+s[8])/2);
      else if(i==0) gsl_matrix_set(cov,0,j,9*(s[0]-s[j])/4);
      else if(j==0) gsl_matrix_set(cov,i,0,9*(s[0]-s[i])/4);
      //else gsl_matrix_set(cov,i,j,63*(s[0])/16);
      else gsl_matrix_set(cov,i,j,(i%2==1?-1:1)*(j%2==1?-1:1)*63*(s[0])/16); //yellow
      //else gsl_matrix_set(cov,i,j,(i%2==1?-1:1)*(j%2==1?-1:1)*(s[0]));
    }
  }
  
  //multiply or add them
  //for(int i=1;i<9;i++){
    //alternate storage matrix, evens store in 9, odds store in 10
    //gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, m_rot[9+(i%2==1?0:1)], m_rot[i], 0.0, m_rot[9+(i%2==1?1:0)]);
  //  gsl_matrix_add(m_rot[0],m_rot[i]);
  //}
  
  matrixPrint(cov, 9*arguments.systs);
  
  //invert and store for penalty term calculation in rfCovMatrix
  pinv(cov, arguments.systs*9, rfCovMatrix);
  
  matrixPrint(rfCovMatrix, 9*arguments.systs);
  //exit(0);
}

double extract1sResponse(int syst){
  //find 1 sigma range (=0.5(up-down)) for syst
  //Note, if it never crosses zero, this returns zero
  int nuisances=arguments.systs*((arguments.chimode==19||arguments.chimode==29)?12:1);
  //printf("syst = %d, nuisances = %d\n",syst,nuisances);
  double narray[nuisances];
  double s_up=0, s_down=0, chi2=0;
  int chiuserdata=1;
  
  //set event rates to input values
  glbSetOscillationParameters(true_values);
  glbSetRates();
  //switch systematics off and call glbchisys so that event rates are set
  glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
  glbChiSys(test_values, GLB_ALL, GLB_ALL);
  glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_ON);
 
  //scan up from zero
  for(int ud =-1;ud<=1;ud+=2){ //scan down then scan up
    for(double s=0;s<5;s=s+ud*0.01){
      for(int i=0; i<nuisances; i++){
        narray[i]=0;
      }
      if(arguments.chimode==19 || arguments.chimode==29)
        narray[12*syst]=s;//only modify the numu nuisance parameter for the current systs
      else  
        narray[0]=s;
      for(int i=0; i<nuisances; i++){
        //printf("%f, ", narray[i]);
      }
      //printf("\ns=%f, nuisances=%d\n",s,nuisances);
      //printf("chimode = %d\n",arguments.chimode);
      if (arguments.chimode==29){
	//printf("narray0 = %f\n",narray[0]);
	chi2=chi_ResponseFunctionCovE(0,0,nuisances,narray,NULL,&chiuserdata);
      }
      else {
	chi2=chi_ResponseFunctionCov(0,0,nuisances,narray,NULL,&chiuserdata);
      }
      //printf("s=%f, chi2(%d)=%f\n",s,ud,chi2);
      if(chi2>1){
        if(ud==-1)s_down=s;
        if(ud==1)s_up=s;
        break;
      }
    }
  }

  return 0.5*(s_up-s_down);
}

//Similar to chi_responseFunction, but adds multiple nuisance parameters
//so that channels are not strictly correlated in their systematics responses
//TODO: also allows covariance matrix between numu nuisance parameters
double chi_ResponseFunctionCov(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  //x will have dimensions 12*number of systematics
  //12 nuisance parameters are numu, numubar/numu, nue/numu, nutau/numu
  //stat parameters for numubar, nue_bkg, nuebar_bkg, NC, nue_sig, nuebar_sig, nutau, nutaubar
  //if user_data is not null, then use sigma^2 for penalty term
  double tresppre;
  double tresppost;
  double xsigma=0;
  int rfi;
  fchi1=fchi2=0;

           
  for(int curexp=0;curexp<glb_num_of_exps;curexp++){
    double *true_rates       = glbGetRuleRatePtr(curexp, rule);
    double *signal_fit_rates = glbGetSignalFitRatePtr(curexp, rule);
    double *bg_fit_rates     = glbGetBGFitRatePtr(curexp, rule);
    //note: only sampbins is used so postsmearing will only work if sampbins (pre)=bins (post)
    int sampbins = glbGetNumberOfSamplingPoints(curexp); 
    int bins = glbGetNumberOfBins(curexp);
    int channels=glbGetNumberOfChannels(curexp); 

    //set pre smearing effs to response function values
    for(int ch=0;ch<channels;ch++){
      for(int sbin=0; sbin<sampbins; sbin++){
        tresppre=0;tresppost=0;
        //add up contribution from each systematic for this bin and channel
        for(int syst=0; syst<arguments.systs; syst++){
        //for(int syst=1; syst<arguments.systs; syst++){
          xsigma=0.0;
          switch(ch){
            case 0: case 4://numu, NC
              xsigma=x[12*syst];
              break;
            case 1: //numubar
              xsigma=x[12*syst]+x[12*syst+1] + x[12*syst+4];
              break;
            case 2: case 5: //nue_bkg, nue_sig
              xsigma=x[12*syst]+x[12*syst+2] + x[12*syst+3+ch];
              break;
            case 3: case 6: //nuebar_bkg, nuebar_sig
              xsigma=x[12*syst]+x[12*syst+1]+x[12*syst+2] + x[12*syst+3+ch];
              break;
            case 7: //nutau
              xsigma=x[12*syst]+x[12*syst+3] + x[12*syst+10];
              break;
            case 8: //nutaubar
              xsigma=x[12*syst]+x[12*syst+1]+x[12*syst+3] + x[12*syst+11];
              break;
          }
          if(xsigma>5.0 || xsigma<-5.0) {fchi2=1e10; return fchi2;}
          if(xsigma != 0.0){
            rfi=sampbins*arguments.systs*ch+sampbins*syst+sbin;
            if(prepost[curexp][syst]==0){
              tresppre+=gsl_spline_eval(rf_spline[curexp][rfi], xsigma, rf_acc[curexp][rfi])-1;
            }else{
              tresppost+=gsl_spline_eval(rf_spline[curexp][rfi], xsigma, rf_acc[curexp][rfi])-1;
            }
          }
          //if(ch==0 && sbin==10) printf("xsigma=%f\ttresp=%f, rfi=%d\n",xsigma, tresp,rfi);
        }
        mult_presmear_effs[curexp][ch][sbin]=tresppre<=-1?1e-8:1+tresppre;
        mult_postsmear_effs[curexp][ch][sbin]=tresppost<=-1?1e-8:1+tresppost;  
        /*if(fabs(x[0])<0.001 && mult_postsmear_effs[curexp][ch][sbin]>1.0 ){
            for(int i=0; i<n_params; i++){
              printf("x[%d]=%f",i, x[i]*1e10);
            }
          rfi=sampbins*arguments.systs*ch+sampbins*1+sbin;
          printf("\ncurexp=%d, ch=%d, sbin=%d, x0=%f, total resp pre=%f,total resp post=%f, systs=%d, spline=%f\n",curexp,ch,sbin, x[0], mult_presmear_effs[curexp][ch][sbin]*1e10, 1e10*mult_postsmear_effs[curexp][ch][sbin],arguments.systs,1e10*(gsl_spline_eval(rf_spline[curexp][rfi], xsigma, rf_acc[curexp][rfi])-1));
          exit(0);
        }*/
      }
    }

    //recompute rates using these multiplicative pre/post smearing effs
    mgt_set_new_rates(curexp, mult_presmear_effs, mult_postsmear_effs);

    for (int ebin=0; ebin < bins; ebin++){
      fchi1 += mylikelihood(true_rates[ebin], signal_fit_rates[ebin]+bg_fit_rates[ebin]);
      /*if(fabs(x[0])<0.001){
        printf("x[0]=%f, ebin=%i, true=%f, fit=%f, like=%f\n",x[0], ebin,true_rates[ebin],signal_fit_rates[ebin]+bg_fit_rates[ebin],mylikelihood(true_rates[ebin], signal_fit_rates[ebin]+bg_fit_rates[ebin]));
      }*/
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
    for(int i=0;i<12;i++){
      if(user_data!=NULL){
        fchi2 += pow(x[12*syst+i],2);
      }else{
        fchi2 += pow(x[12*syst+i]/rfCovMatrixInputs[12*syst+i],2);
      }
    }
  }
  /*if(arguments.chimode==19){
    for(int i=0;i<arguments.systs*9;i++){
      for(int j=0;j<arguments.systs*9;j++){
        fchi2 += x[i] * gsl_matrix_get(rfCovMatrix, i, j) * x[j];
      }
    }
  }else{
    for(int syst=0;syst<arguments.systs;syst++){
      for(int i=0;i<9;i++){
        fchi2 += pow(x[9*syst+i],2);
      }
    }
  }*/

  
  //store nuisance parameters for output, if enabled
  if(arguments.nuis_output>0){
    for(int syst=0;syst<arguments.systs;syst++){
      for(int i=0;i<12;i++){
        //only store first 32 nuisance parameters
        if(12*syst+i<32) xmin[0][0][12*syst+i]=x[12*syst+i];
      }
    }
  }
  //printf("fchi1=%f, fchi2=%f\n",fchi1, fchi2);
  return fchi1+fchi2;
}

//ETW June 2015
//Similar to chi_responseFunctionCov, but handles energy systematics
double chi_ResponseFunctionCovE(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  //x will have dimensions 12*number of systematics
  //12 nuisance parameters are numu, numubar/numu, nue/numu, nutau/numu
  //stat parameters for numubar, nue_bkg, nuebar_bkg, NC, nue_sig, nuebar_sig, nutau, nutaubar
  //if user_data is not null, then use sigma^2 for penalty term
  double tresppre;
  double xsigma=0;
  int rfi;
  fchi1=fchi2=0;

  for(int curexp=0;curexp<glb_num_of_exps;curexp++){
    double *true_rates       = glbGetRuleRatePtr(curexp, rule);
    double *signal_fit_rates = glbGetSignalFitRatePtr(curexp, rule);
    double *bg_fit_rates     = glbGetBGFitRatePtr(curexp, rule);
    int sampbins = glbGetNumberOfSamplingPoints(curexp); 
    int recbins = glbGetNumberOfBins(curexp); //post-smearing bins
    int reclobin, rechibin;
    int temp = glbGetEnergyWindowBins(curexp,rule,&reclobin,&rechibin); //post-smearing bins to include in chi2
    //printf("sampbins = %d, recbins = %d, lo = %d, hi = %d\n",sampbins,recbins,reclobin,rechibin);
    

    //initialize smearing matrix
    double **tresppost = (double**)malloc(recbins*sizeof(double*));
    for (int i=0;i<recbins;i++){
      tresppost[i] = (double*)malloc(recbins*sizeof(double));
      for (int j=0;j<recbins;j++){
	if (i==j) {
	  tresppost[i][j]=1;
	}
	else
	  tresppost[i][j]=0;
      }
    }

    int channels=glbGetNumberOfChannels(curexp); 

    //set pre smearing effs to response function values
    for(int ch=0;ch<channels;ch++){
      for(int sbin=0; sbin<sampbins; sbin++){
        tresppre=0;
        //add up contribution from each systematic for this bin and channel
        for(int syst=0; syst<arguments.systs; syst++){
          xsigma=0.0;
          switch(ch){
            case 0: case 4://numu, NC
              xsigma=x[12*syst];
              break;
            case 1: //numubar
              xsigma=x[12*syst]+x[12*syst+1] + x[12*syst+4];
              break;
            case 2: case 5: //nue_bkg, nue_sig
              xsigma=x[12*syst]+x[12*syst+2] + x[12*syst+3+ch];
              break;
            case 3: case 6: //nuebar_bkg, nuebar_sig
              xsigma=x[12*syst]+x[12*syst+1]+x[12*syst+2] + x[12*syst+3+ch];
              break;
            case 7: //nutau
              xsigma=x[12*syst]+x[12*syst+3] + x[12*syst+10];
              break;
            case 8: //nutaubar
              xsigma=x[12*syst]+x[12*syst+1]+x[12*syst+3] + x[12*syst+11];
              break;
          }
	  //printf("channel %d sbin %d syst %d sigma %f\n",ch,sbin,syst,xsigma);

          if(xsigma>5.0 || xsigma<-5.0) {fchi2=1e10; return fchi2;}
          if(xsigma != 0.0){
            rfi=sampbins*arguments.systs*ch+sampbins*syst+sbin;
            if(prepost[curexp][syst]==0){
              tresppre+=gsl_spline_eval(rf_spline[curexp][rfi], xsigma, rf_acc[curexp][rfi])-1;
	    }

          //if(ch==0 && sbin==10) printf("xsigma=%f\ttresp=%f, rfi=%d\n",xsigma, tresp,rfi);
	  }
	  mult_presmear_effs[curexp][ch][sbin]=tresppre<=-1?1e-8:1+tresppre;
	}
      }


      //Fill smearing adjustment matrices

      //add up contribution from each systematic for this bin and channel
      for(int syst=0; syst<arguments.systs; syst++){
	xsigma=0.0;
	switch(ch){
	  case 0: case 4://numu, NC
	    xsigma=x[12*syst];
	    break;
	  case 1: //numubar
	    xsigma=x[12*syst]+x[12*syst+1] + x[12*syst+4];
	    break;
          case 2: case 5: //nue_bkg, nue_sig
	    xsigma=x[12*syst]+x[12*syst+2] + x[12*syst+3+ch];
	    break;
          case 3: case 6: //nuebar_bkg, nuebar_sig
	    xsigma=x[12*syst]+x[12*syst+1]+x[12*syst+2] + x[12*syst+3+ch];
	    break;
          case 7: //nutau
	    xsigma=x[12*syst]+x[12*syst+3] + x[12*syst+10];
	    break;
          case 8: //nutaubar
	    xsigma=x[12*syst]+x[12*syst+1]+x[12*syst+3] + x[12*syst+11];
	    break;
	}
	if(xsigma>5.0 || xsigma<-5.0) {fchi2=1e10; return fchi2;}
	//printf("channel %d syst %d sigma %f\n",ch,syst,xsigma);
	if (prepost[curexp][syst]==1){
	  mgt_get_smear_interp(mult_postsmear_matrix_in[curexp][syst][ch], recbins, size_sigmas[curexp][syst], syst_sigmas[curexp][syst], xsigma, tresppost);

	  //tmp for debugging
	    for (int b=0;b<recbins;b++){
	      float mysum = 0;
	      for (int bb=0;bb<recbins;bb++){
		mysum += mult_postsmear_matrix_in[curexp][syst][ch][50][bb][b];
	      }
	      //printf("For recbin %d, sum = %f\n",b,mysum);
	    }

	}

      }

      //Have to figure out what to do. For single syst it's ok to just copy tresppost in, but ultimately have to combine - not clear how to do that since
      //matrix multiplication does not commute

      //Fill smearing matrix
      for(int b=0;b<recbins;b++){
	for (int bb=0;bb<recbins;bb++){
	  mult_postsmear_matrix[curexp][ch][b][bb]=tresppost[b][bb];
	  //printf("Matrix %d %d = %f\n",b,bb,tresppost[b][bb]);
	}
      }

      //Tmp for debugging
      for (int b = 0; b<recbins; b++){
	float tresppostsum = 0;
	for (int bb=0; bb<recbins; bb++){
	  tresppostsum += tresppost[bb][b];
	}
	//printf("For ch %d, recbin %d, sum = %f\n",ch,b,tresppostsum);
      }
    }

    //Free tresppost
    for (int i=0; i<recbins; i++){
      free(tresppost[i]);
    }
    free(tresppost);

    //Temporary, check chi2 with old rates
    float oldchi = 0;
    float newrates;
    float oldrates[recbins];
    float oldchi2cont[recbins];
    newrates = 0;
    for (int ebin=reclobin; ebin < rechibin; ebin++){
      oldchi += mylikelihood(true_rates[ebin], signal_fit_rates[ebin]+bg_fit_rates[ebin]);
      oldchi2cont[ebin] = mylikelihood(true_rates[ebin], signal_fit_rates[ebin]+bg_fit_rates[ebin]);
      oldrates[ebin] = signal_fit_rates[ebin]+bg_fit_rates[ebin];
    }

    //recompute rates using these multiplicative pre/post smearing effs
    mgt_set_new_rates_e(curexp, mult_presmear_effs, mult_postsmear_matrix);

    //only add up chi2 over analysis bins
    for (int ebin=reclobin; ebin < rechibin+1; ebin++){
      fchi1 += mylikelihood(true_rates[ebin], signal_fit_rates[ebin]+bg_fit_rates[ebin]);
      newrates = signal_fit_rates[ebin]+bg_fit_rates[ebin];
      //printf("xsigma: %f: ebin %d: truerate = %f, oldrate = %f, newrate = %f\n",xsigma,ebin,true_rates[ebin],oldrates[ebin],newrates);
      //printf("xsigma: %f: ebin %d: truerate = %f, oldrate = %f, oldchi2cont = %f, newrate = %f, chi2cont = %f\n",xsigma,ebin,true_rates[ebin],oldrates[ebin],oldchi2cont[ebin],newrates,mylikelihood(true_rates[ebin],signal_fit_rates[ebin]+bg_fit_rates[ebin]));      
      if(arguments.debug==1){
        double a[]={ebin,true_rates[ebin],(signal_fit_rates[ebin]+bg_fit_rates[ebin]),x[0], x[1], x[2], x[3], x[4]};
        AddArrayToOutput(a,8);
      }
    }

    //printf("xsigma %f: oldchi = %f, newchi = %f\n",xsigma,oldchi,fchi1);

    if(arguments.debug==1){
      AddToOutputBlankline();
      AddToOutputBlankline();
    }
  }
  
  //add penalty term
  for(int syst=0;syst<arguments.systs;syst++){
    for(int i=0;i<12;i++){
      if(user_data!=NULL){
        fchi2 += pow(x[12*syst+i],2);
      }else{
        fchi2 += pow(x[12*syst+i]/rfCovMatrixInputs[12*syst+i],2);
      }
    }
  }

  //store nuisance parameters for output, if enabled
  if(arguments.nuis_output>0){
    for(int syst=0;syst<arguments.systs;syst++){
      for(int i=0;i<12;i++){
        //only store first 32 nuisance parameters
        if(12*syst+i<32) xmin[0][0][12*syst+i]=x[12*syst+i];
      }
    }
  }
  //printf("fchi1=%f, fchi2=%f\n",fchi1, fchi2);
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
	
	for(int i=0;i<n;i++){
    printf("[");
		for(int j=0;j<n;j++){
			printf("%g",gsl_matrix_get (M, i, j));
			if(j!=n-1) printf(",\t");
		}
		printf( "],\n");
	}
  printf("\n");
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
				//if(checkDif==0) debugprintf("True rates have changed...\n");
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
							//debugprintf("i=%d,j=%d,k=%d,kp=%d,channel_rates[k][i]=%f,channel_rates[kp][i]=%f,errorMatrix[%d][%d]=%f, mult=%f\n",i,j,k,kp,channel_rates[k][i],channel_rates[kp][j],(kp*ebins+j),(k*ebins+i),errorMatrix[exp][(channels*ebins)*(kp*ebins+j)+(k*ebins+i)],channel_rates[k][i]*channel_rates[kp][j]*errorMatrix[exp][(channels*ebins)*(kp*ebins+j)+(k*ebins+i)]);
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
				debugprintf("Cholesky decomposition failed!\nComputing pseudo-inverse...\n");
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
    debugprintf("chi2_1 = %e    sig = %e   bkg = %e     Nobs = %e\n",chi2_1,gsl_vector_get(signal_bin_flucts, i)+signal_fit_rates[i+ew_low], gsl_vector_get(bg_bin_flucts, i)+bg_fit_rates[i+ew_low], true_rates[i+ew_low]);
    chi2 += chi2_1;
  }
  debugprintf("chi2 1 = %e\n",chi2);


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
          //debugprintf("inv_chi2=%e=(%e)*(%e)*(%e)\n",(gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)) * gsl_matrix_get(Inv_Matrix[exp], i, j)  * (gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j)),(gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)),gsl_matrix_get(Inv_Matrix[exp], i, j),(gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j)));
          //non-inverted version:
          //if( (gsl_matrix_get(Inv_Matrix[exp], i, j))>1e-4) chi22 += (i==j?1:-1)*(gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)) * (1/gsl_matrix_get(Inv_Matrix[exp], i, j))  * (gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j));
          //if((gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)) * (1/gsl_matrix_get(Inv_Matrix[exp], i, j))  * (gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j))>0)
          //	debugprintf("inv_chi2=%e=(%e)*(%e)*(%e)\n",(i==j?1:-1)*(gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)) * (1/gsl_matrix_get(Inv_Matrix[exp], i, j))  * (gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j)),(gsl_vector_get(signal_bin_flucts, i)+gsl_vector_get(bg_bin_flucts, i)),(i==j?1:-1)*(1/gsl_matrix_get(Inv_Matrix[exp], i, j)),(gsl_vector_get(signal_bin_flucts, j)+gsl_vector_get(bg_bin_flucts, j)));
      }
    }
  }

	/*if(chi22>0)*/ chi2+=chi22;
	debugprintf("chi22:%e\n",chi22);
	//printf("chi22_2:%e\n",chi22_2);

  //copy parameter array into globally accessible variable
  for(int i=0; i<n_params; i++){
    xmin[exp][rule][i]=x[i];
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

double ChiAllDeltaPrescan(glb_params in_values,glb_params out_values, int exp){
  //wrapper for glbChiAll that performs a prescan over dcp
  glb_params test_values = glbAllocParams();
  glbCopyParams(in_values,test_values);
  
  //find minimum in dcp with glbchisys
  glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
  double tchi2,chi2min=1e6,mindcp=0;
  for(double this_tdcp=-M_PI;this_tdcp<M_PI;this_tdcp=this_tdcp+(M_PI/18)){
    glbSetOscParams(test_values, this_tdcp, GLB_DELTA_CP);
    tchi2=glbChiSys(test_values,exp,GLB_ALL);
    if(tchi2<chi2min){
      chi2min=tchi2;
      mindcp=this_tdcp;
    }
  }
 //only switch systs back on if they were enabled to begin with
  if(arguments.chimode!=101){
    glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_ON);
  } 
  
  //set test_values dcp back to mindcp found
  glbSetOscParams(test_values, mindcp, GLB_DELTA_CP);
  glbSetCentralValues(test_values);
  tchi2=glbChiAll(test_values, out_values, exp);  
  
  glbFreeParams(test_values);
  return tchi2;
}

double ChiNPDeltaPrescan(glb_params in_values,glb_params out_values, int exp){
  //wrapper for glbChiNP that performs a prescan over dcp
  glb_params test_values = glbAllocParams();
  glbCopyParams(in_values,test_values);
  
  //find minimum in dcp with glbchisys and no systs
  glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
  double tchi2,chi2min=1e6,mindcp=0;
  for(double this_tdcp=-M_PI;this_tdcp<M_PI;this_tdcp=this_tdcp+(M_PI/18)){
    glbSetOscParams(test_values, this_tdcp, GLB_DELTA_CP);
    tchi2=glbChiSys(test_values,exp,GLB_ALL);
    if(tchi2<chi2min){
      chi2min=tchi2;
      mindcp=this_tdcp;
    }
    //printf("%f,%f,%f,%f\n",this_tdcp+(M_PI/18),chi2min,tchi2,mindcp);
  }
 //only switch systs back on if they were enabled to begin with
  if(arguments.chimode!=101){
    glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_ON);
  } 
  //set test_values dcp back to mindcp found
  //mindcp=0;
  glbSetOscParams(test_values, mindcp, GLB_DELTA_CP);
  glbSetCentralValues(test_values);
  //printf("%f,%f\n",glbGetOscParams(test_values,GLB_DELTA_CP),glbGetOscParams(test_values,2));
  tchi2=glbChiNP(test_values, out_values, exp);  
  //printf("%f,%f\n",glbGetOscParams(out_values,GLB_DELTA_CP),glbGetOscParams(out_values,2)); exit(0);
  glbFreeParams(test_values);
  return tchi2;
}

double ChiAllDeltaOctPrescan(glb_params in_values,glb_params out_values, int exp){
  //wrapper for glbChiAll that performs a prescan over dcp and the octant
  glb_params test_values = glbAllocParams();
  glbCopyParams(in_values,test_values);
  
  //values of t23 to prescan
  double thist23=glbGetOscParams(in_values,2);
  double osc2[] = {thist23, M_PI/2, (M_PI/2) - thist23};
  
  //find minimum in dcp with glbchisys and NO systematics
  glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
  double tchi2,chi2min=1e6,mindcp=0, mint23=0;
  for(int i_t23=0; i_t23<3; i_t23++){
    glbSetOscParams(test_values, osc2[i_t23], 2);
    for(double this_tdcp=-M_PI;this_tdcp<M_PI;this_tdcp=this_tdcp+(M_PI/18)){
      glbSetOscParams(test_values, this_tdcp, 3);
      tchi2=glbChiSys(test_values,exp,GLB_ALL);
      if(tchi2<chi2min){
        chi2min=tchi2;
        mindcp=this_tdcp;
        mint23=osc2[i_t23];
      }
    }
  }
  //only switch systs back on if they were enabled to begin with
  if(arguments.chimode!=101){
    glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_ON);
  }
  //set test_values dcp back to mindcp found
  glbSetOscParams(test_values, mindcp, 3);
  glbSetOscParams(test_values, mint23, 2);
  glbSetCentralValues(test_values);
  tchi2=glbChiAll(test_values, out_values, exp);  
  
  glbFreeParams(test_values);
  return tchi2;
}

