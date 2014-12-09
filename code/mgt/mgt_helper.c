#ifndef MGT_HELPER_H
#include "mgt_helper.h"
#endif

//only prints if debug is turned on
void dprintf( const char* format, ... ) {
    if(arguments.debug==1){
			va_list args;
			va_start( args, format );
			vprintf( format, args );
			va_end( args );
		}
}

/*sort functions for qsort*/
int compare(const void *x, const void *y) {
	double dx, dy;

	dx = *(double *)x;
	dy = *(double *)y;

	if (dx > dy) {
			return -1;
	} else if (dx < dy) {
			return +1;
	}
	return 0;
}
int compare_asc(const void *x, const void *y) {
	double dx, dy;

	dx = *(double *)x;
	dy = *(double *)y;

	if (dx < dy) {
			return -1;
	} else if (dx > dy) {
			return +1;
	}
	return 0;
}


/*setup output files*/
static char* THEFILE;
void InitOutput(char* filename, char* headline){
 THEFILE=filename;
 if(THEFILE==NULL || strlen(THEFILE)==0) printf(headline);
 else 
 {
   FILE* f=fopen(THEFILE, "w");
   if (!f)
   {
     printf("File cannot be opened!\n");
     THEFILE[0]=0;
   }
   else {
    fprintf(f,headline);
    fclose(f);
   }
 }
}
void AddToOutput2(double n1,double n2){
 if(THEFILE==NULL || strlen(THEFILE)==0) printf("%g %g \n",n1,n2);
 else 
 {
   FILE* f=fopen(THEFILE, "a");
   if (!f)
   {
     printf("File cannot be opened!\n");
     THEFILE[0]=0;
   }
   else
   {
    fprintf(f,"%g %g \n",n1,n2);
    fclose(f);
   }
 }
}
void AddArrayToOutput(double a[],int n){
 int i=0;
 if(THEFILE==NULL || strlen(THEFILE)==0){
	 for(i=0;i<n;i++) printf("%g\t",a[i]);
	 printf("\n");
 }
 else 
 {
   FILE* f=fopen(THEFILE, "a");
   if (!f)
   {
     printf("File cannot be opened!\n");
     THEFILE[0]=0;
   }
   else
   {
		for(i=0;i<n;i++) fprintf(f,"%g\t",a[i]);
    //output nuisance parameters, if enabled
    if(arguments.nuis_output>0){
      if(arguments.chimode==19 || arguments.chimode==17 || arguments.chimode==31 || arguments.chimode==32){//only first exp
        for(int n=0;n<32;n++){
          fprintf(f,"%g\t",xmin[0][0][n]);
        }
        //also output stat/penalty chisq terms
        fprintf(f,"%g\t%g\t",fchi1,fchi2);
      }
    }
    //only print a newline if some array was passed
    if(n>0) fprintf(f,"\n");
    fclose(f);
   }
 }
}
void AddToOutputBlankline(){
 if(THEFILE==NULL || strlen(THEFILE)==0) printf("\n");
 else 
 {
   FILE* f=fopen(THEFILE, "a");
   if (!f)
   {
     printf("File cannot be opened!\n");
     THEFILE[0]=0;
   }
   else
   {
    fprintf(f,"\n");
    fclose(f);
   }
 }
}

/*argument parsing*/
error_t parse_opt (int key, char *arg, struct argp_state *state){
  /* Get the INPUT argument from `argp_parse', which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;
  
  switch (key)
    {
    case 'D':
      parse_definition(arg);
      break;
    case 'p':
      arguments->params = arg;
      break;
    case 'E':
      arguments->paramse = arg;
      break;
    case 'x':
      arguments->xrange = arg;
      break;
    case 'e':
      arguments->ematrixfile = arg;
      break;
    case 'i':
      arguments->covmatrixfile = arg;
      break;
    case 'I':
      arguments->chimode = atof(arg);
      break;
    case 'b':
      arguments->bintobin = atof(arg);
      break;
    case 'T':
      arguments->runType = atoi(arg);
      break;
    case 'C':
      arguments->runCat = atoi(arg);
      break;
    case 't':
      arguments->test = YES;
      break;
    case 'h':
      arguments->hier = YES;
      break;
    case 'd':
			arguments->debug = YES;
			break;
    case 'r':
      arguments->tSteps = atoi(arg);
      break;
    case 'v':
      arguments->varied = atoi(arg);
      break;
    case 'z':
      arguments->zero = YES;
      break;
    case 'c':
      arguments->chi2_goal = atof(arg);
      break;
    case 's':
      arguments->systs = atoi(arg);
      break;
    case 'n':
      arguments->nuis_output = atoi(arg);
      break;
    case 'P':
      arguments->inpart = arg;
      break;
    case 'f':
      arguments->pflucts = atoi(arg);
      break;
    case 'R':
      arguments->regprior = atoi(arg);
      break;
	            
    case ARGP_KEY_ARG:
      if (state->arg_num > 33)
	/* Too many arguments. */
	argp_usage (state);
     
      arguments->args[state->arg_num]=arg;
      
      break;
      
    case ARGP_KEY_END:
      if (state->arg_num < 1)
	/* Not enough arguments. */
	argp_usage (state);
      break;
      
    default:
      return ARGP_ERR_UNKNOWN;
    }
  arguments->exp=state->arg_num;
  return 0;
}

void parse_setdefaults(){
  //set default arguments
  arguments.args[0]="-";
  arguments.params=NULL;
  arguments.paramse=NULL;
  arguments.xrange=NULL;
  arguments.chimode=-1;
  arguments.bintobin=0.0;
  arguments.runType=1;
  arguments.runCat=1;
  arguments.hier=NO;
  arguments.debug=NO;
  arguments.min_runtime=1e-2;/* Minimum running time to consider [years] */
  arguments.max_runtime=10; /* Maximum running time to consider [years] */
  arguments.tSteps=10;/* Number of data points for each curve */
  arguments.chi2_goal=1.0;/* Desired chi^2 value 3 sigma */
  arguments.logs22th13_precision = 0.0001;/* Desired precision of log(sin[th13]^2) in root finder */
	arguments.varied=0;
  arguments.systs=1;
  arguments.zero=NO;
  arguments.preScan=0;
  arguments.scanVar=0;
  arguments.part[0]=0;
  arguments.part[1]=0;
  arguments.pflucts=0;
  arguments.nuis_output=0;
  arguments.regprior=0; //default to GLoBES built-in prior, i.e. do nothing
}

void definechifunctions(){
  glbDefineChiFunction(&chiSpectrumTiltSplitBG,7,"chiSpectrumTiltSplitBG7",NULL);
  glbDefineChiFunction(&chiSpectrumTiltSplitBG,11,"chiSpectrumTiltSplitBG11",NULL);
  glbDefineChiFunction(&chiSpectrumSplitBG_BtoB,4,"chiSpectrumSplitBG4",NULL);
  glbDefineChiFunction(&chiSpectrumSplitBG_BtoB,5,"chiSpectrumSplitBG5",NULL);
  glbDefineChiFunction(&chiSpectrumSplitBG_BtoB,7,"chiSpectrumSplitBG7",NULL);
  glbDefineChiFunction(&chiSpectrumSplitBG_BtoB,9,"chiSpectrumSplitBG9",NULL);
  glbDefineChiFunction(&chiSpectrumTiltCustom,4,"chiSpectrumTiltCustom",NULL);
  glbDefineChiFunction(&chiSpectrumNormCustom,2,"chiSpectrumNormCustom",NULL);
  glbDefineChiFunction(&chiSpectrumTiltCustom_BtoB,4,"chiSpectrumTiltCustom_BtoB",NULL);
}

 
/*gsl error handling*/
void gslError(const char *reason, const char *file, int line, int gsl_errno){
  static int n_errors=0;
  
  printf("GSL Error(%d) in file %s:%d : %s\n",gsl_errno, file, line, gsl_strerror(gsl_errno));
  if (++n_errors > 50000)
  {
    printf("Too many GSL errors. Program aborted.\n");
    abort();
  }
}


/* Minimum of two numbers */
double min(double x, double y)
{
  if (x < y)
    return x;
  else
    return y;
}

//reimplementation of set new rates to work with NOSC channels
static double Probs[3][3];
static double ProbsAnti[3][3];
static double RatesNOSC(struct glb_experiment *e, double en, double baseline,
		 int polarity, int anti, int l, int m,int ident)
{
  double ergebnis;

  ergebnis=glb_get_xsec(en,m,anti,e->xsecs[ident])
    *glb_get_flux(en,baseline,l,anti,e->fluxes[polarity])
    *e->targetmass;

  return ergebnis;
}

static double RatesXX(struct glb_experiment *e, double en, double baseline, int polarity, int anti, int l, int m,int ident)
{
  double ergebnis;
  if (anti == 1)
    {
      ergebnis=glb_get_xsec(en,m,anti,e->xsecs[ident])*
	glb_get_flux(en,baseline,l,anti,e->fluxes[polarity])
	*Probs[l-1][m-1]*e->targetmass;
    }
  else
    {
      ergebnis=glb_get_xsec(en,m,anti,e->xsecs[ident])
	*glb_get_flux(en,baseline,l,anti,e->fluxes[polarity])
	*ProbsAnti[l-1][m-1]*e->targetmass;

    }
  return ergebnis;
}

double mgt_calc_channel(struct glb_experiment *e, int i, double en, double baseline)
{
  double ergebnis;
  int final_flavour,initial_flavour;
  int nosc=0;
  if(e->listofchannels[3][i]>9) {final_flavour=e->listofchannels[3][i]-10;nosc=1;}
  else {final_flavour=e->listofchannels[3][i];}
  if(e->listofchannels[2][i]>9) {initial_flavour=e->listofchannels[2][i]-10;nosc=1;}
  else {initial_flavour=e->listofchannels[2][i];}


    if(nosc!=0)
      {
	if(final_flavour!=initial_flavour) {return 0;}
	ergebnis=RatesNOSC(e,en,baseline,e->listofchannels[0][i],
			   e->listofchannels[1][i],initial_flavour,
			   final_flavour,e->listofchannels[4][i]);
      }
    else
      {
	ergebnis=RatesXX(e,en,baseline,e->listofchannels[0][i],
			 e->listofchannels[1][i],initial_flavour,
			 final_flavour,e->listofchannels[4][i]);
      }
  return ergebnis;
}

static void mgt_CalcAllProbs(struct glb_experiment *e, double en, double baseline)
{
  int status;

  if ((status=glb_hook_probability_matrix(Probs, +1, en, e->psteps, e->lengthtab, e->densitybuffer,
          (e->filter_state == GLB_ON) ? e->filter_value : -1.0, glb_probability_user_data)) != GLB_SUCCESS)
    glb_error("Calculation of oscillation probabilities failed.");

  if ((status=glb_hook_probability_matrix(ProbsAnti, -1, en, e->psteps, e->lengthtab, e->densitybuffer,
          (e->filter_state == GLB_ON) ? e->filter_value : -1.0, glb_probability_user_data)) != GLB_SUCCESS)
    glb_error("Calculation of oscillation probabilities failed.");
}

void mgt_set_new_rates(int exp, double** mult_presmear_effs[32], double** mult_postsmear_effs[32])
{
  //glb_exp e = glb_experiment_list[exp];
  struct glb_experiment *e = glb_experiment_list[exp];
  int s=0;
  
  for (int j=0; j<e->simbins; j++){
    /* Calculate probability matrix */
    mgt_CalcAllProbs(e, e->smear_data[s]->simbincenter[j],e->baseline);
    for(int i=0; i < e->numofchannels; i++){
      s = e->listofchannels[5][i];
      e->chrb_1[i][j] = mgt_calc_channel(e, i, e->smear_data[s]->simbincenter[j],e->baseline)
              * e->user_pre_smearing_channel[i][j]
              * e->smear_data[s]->simbinsize[j]
              * mult_presmear_effs[exp][i][j]
              + e->user_pre_smearing_background[i][j];
    }
  }

  /* Calculate post-smearing rates for all channels */
  int k_low, k_high; 
  for(int i=0; i < e->numofchannels; i++)
  {
    s = e->listofchannels[5][i];
    for(int j=0; j < e->numofbins; j++){
      e->chra_1[i][j] = 0.0;
      k_low  = e->lowrange[s][j];
      k_high = e->uprange[s][j] + 1;
      for(int k=k_low; k < k_high; k++)
        e->chra_1[i][j] += e->smear[s][j][k-k_low] * e->chrb_1[i][k];
      e->chra_1[i][j] = e->chra_1[i][j] * e->user_post_smearing_channel[i][j] * mult_postsmear_effs[exp][i][j]
                                  + e->user_post_smearing_background[i][j];
    }
  }

  /* Merge channel rates into signal and background rates */
  for(int i=0; i < e->numofrules; i++){
    for(int j=0; j < e->numofbins; j++){
      e->rates1BG[i][j] = 0.0;
      for (int k=0; k < e->lengthofbgrules[i]; k++)
        e->rates1BG[i][j] += e->bgrulescoeff[i][k] * e->chra_1[e->bgrulechannellist[i][k]][j];

      e->rates1[i][j] = 0.0;
      for (int k=0; k < e->lengthofrules[i]; k++)
        e->rates1[i][j] += e->rulescoeff[i][k] * e->chra_1[e->rulechannellist[i][k]][j];
    }
  }
}


//Part handling
//Generate start argument for loop
double pstart(double low, double high, double inc){
    if(arguments.part[1]==0){
      return low;
    }else{
      return low+(arguments.part[0]-1)*((high-low)/(arguments.part[1]-1));
    }
}
//Generate end argument for loop
double pend(double low, double high, double inc){
    if(arguments.part[1]==0){
      return high;
    }else{
      return low+(arguments.part[0]-1)*((high-low)/(arguments.part[1]-1));
    }
}
//Generate increment argument for loop
double pinc(double low, double high, double inc){
    if(arguments.part[1]==0){
      return inc;
    }else{
      //doesn't really matter since we run only one point in parts mode
      //but return something anyway
      return ((high-low)/arguments.part[1]);
    }
}

//Poisonnian statistics for true event spectra
const gsl_rng_type * rng_T;
gsl_rng * rng_r;
int rng_init=0;
void pfluctTrueSpectra(){
  //Initialize RNG if not already
  if(rng_init==0){
    gsl_rng_env_setup();
    rng_T = gsl_rng_default;
    rng_r = gsl_rng_alloc (rng_T);
    rng_init=1;
  }
  
  //Osc. Fluctuations
  /*if(!arguments.test){
    glb_params cvs = glbAllocParams();
    glb_params errs = glbAllocParams();
    glbGetCentralValues(cvs);
    glbGetInputErrors(errs);
    //fluctuate oscillation parameters
    for(int i=0;i<6;i++){
        double cv=glbGetOscParams(cvs,i);
        double err=glbGetOscParams(errs,i);
        if(err>0){
          double f=cv+gsl_ran_gaussian(rng_r, err);
          printf("%d, %f, %f\n",i,cv,f);
          glbSetOscParams(cvs,f,i);
        }
    }
    glbSetOscillationParameters(cvs);
    glbSetRates();
  }*/
  
  
  
  double snorm, bgnorm,tilt;

  for(int exp=0;exp<glb_num_of_exps;exp++){
    for(int rule=0;rule<glbGetNumberOfRules(exp);rule++){
      double *true_rates = glbGetRuleRatePtr(exp, rule);
      double *sig=glbGetSignalRatePtr(exp, rule);
      double *bg=glbGetBGRatePtr(exp, rule);
      
      double smult=1.0;
      double bgmult=1.0;
      if(glbGetSysOnOffState(exp, rule)){
        //fluctuate systematics
        //assumes chispectrumtilt, do not use with other types of systmatics enabled
         glbGetSignalErrors(exp, rule, &snorm, &tilt);
         glbGetBGErrors(exp, rule, &bgnorm, &tilt);
         smult=1+gsl_ran_gaussian(rng_r, snorm);
         bgmult=1+gsl_ran_gaussian(rng_r, bgnorm);
      }
      
      //fluctuate statistics in each bin
      for(int i=0;i<glbGetNumberOfBins(exp);i++){
        true_rates[i]=gsl_ran_poisson(rng_r,smult*sig[i]+bgmult*bg[i]);
      }
    }
  }

}

void PrintRatesToFile(int truefit, int curexp, int rule){
    double *true_sig          = glbGetSignalRatePtr(curexp, rule);
    double *true_bg           = glbGetBGRatePtr(curexp, rule);
    double *fit_sig           = glbGetSignalFitRatePtr(curexp, rule);
    double *fit_bg            = glbGetBGFitRatePtr(curexp, rule);
    double *bincenters = glbGetBinCentersListPtr(curexp);
    int bins = glbGetNumberOfBins(curexp);    
    //get pointers to channels for this exp
    int channels=glbGetNumberOfChannels(curexp); 
    double *cr[channels];
    for(int curchan=0;curchan<channels;curchan++){
      cr[curchan]=glbGetChannelFitRatePtr(curexp,curchan,GLB_POST);
    }
    double x = (1+xmin[0][0][0])*(cr[5][0]+cr[6][0]);
          
    for (int ebin=0; ebin < bins; ebin++){
      //printf("ebin=%i, true=%f, fit=%f\n",ebin,true_rates[ebin],signal_fit_rates[ebin]+bg_fit_rates[ebin]);
      if(truefit==0){
        //true values
        double a[]={bincenters[ebin],true_sig[ebin],true_bg[ebin]};
        AddArrayToOutput(a,3);
      }else{
        //fit values
        if(arguments.chimode==30){
          //apply sig/bg normalization for this exp/rule
          double a[]={bincenters[ebin],(1+xmin[curexp][rule][0])*fit_sig[ebin],(1+xmin[curexp][rule][1])*fit_bg[ebin],(1+xmin[curexp][rule][0]),(1+xmin[curexp][rule][1])};
          AddArrayToOutput(a,5);          
        }else if(arguments.chimode==31 ||arguments.chimode==32){
          //apply normalizations for chiCorrSplitBGs_LBNEFMC custom chisq. function
          if(curexp==0 || curexp==1){ //appearance
            double a[]={bincenters[ebin],
                  (1+xmin[0][0][0])*(cr[5][ebin]+cr[6][ebin]), //sig
                  (1+xmin[0][0][1])*(cr[2][ebin]+cr[3][ebin]) + (1+xmin[0][0][2])*(cr[7][ebin]+cr[8][ebin]) + (1+xmin[0][0][3])*(cr[0][ebin]+cr[1][ebin]+cr[4][ebin]), //bg
                  (1+xmin[0][0][0]),
                  (1+xmin[0][0][1]),
                  (1+xmin[0][0][2]),
                  (1+xmin[0][0][3])};
            AddArrayToOutput(a,7); 
          }else{ //disappearance
            double a[]={bincenters[ebin],
                  (1+xmin[0][0][0])*(cr[0][ebin]+cr[1][ebin]), //sig
                  (1+xmin[0][0][1])*(cr[2][ebin]+cr[3][ebin]+cr[5][ebin]+cr[6][ebin]) + (1+xmin[0][0][2])*(cr[7][ebin]+cr[8][ebin]) + (1+xmin[0][0][3])*(cr[4][ebin]),
                  (1+xmin[0][0][0]),
                  (1+xmin[0][0][1]),
                  (1+xmin[0][0][2]),
                  (1+xmin[0][0][3])};
            AddArrayToOutput(a,7); 
          }
          
                   
        }else{
          double a[]={bincenters[ebin],fit_sig[ebin],fit_bg[ebin]};
          AddArrayToOutput(a,3);
        }
      }
      
    }
}
