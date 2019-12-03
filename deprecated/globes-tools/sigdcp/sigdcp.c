/*Tool to compute signifiance vs delta_cp for mass hierarchy and cpv*/
/*based on example5.c from GLoBES examples*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <gsl/gsl_math.h>    /* GNU Scientific library (required for root finding) */
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>
#include <argp.h>


void parse_definition(const char *in);
int vector_read_double(const char *str, double *result, 
		       size_t n, void (*parse_error)(void));
void parse_error();
#include "globes/globes.h"   /* GLoBES library */
#include "../customchis/customChis.h"
#include "glb_tools_eightfold.h"
static char* THEFILE;
void InitOutput(char* filename, char* headline)
{
 THEFILE=filename;
 if(strlen(THEFILE)==0) printf(headline);
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

void AddToOutput2(double n1,double n2)
{
 if(strlen(THEFILE)==0) printf("%g %g \n",n1,n2);
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
void AddArrayToOutput(double a[],int n)
{
 int i=0;
 if(strlen(THEFILE)==0){
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
    fprintf(f,"\n");
    fclose(f);
   }
 }
}
void AddToOutputBlankline()
{
 if(strlen(THEFILE)==0) printf("\n");
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

//defaults for oscillation parameters
struct arguments arguments;
  
/* A description of the arguments we accept. */
static char args_doc[] = "glb-file(s) output-file";

/* The options we understand. */
static struct argp_option options[] ={
  {"Define",'D',"DEFINITION",0,"Define AEDL variable",0},
  {"xrange",'x',"RANGE",0,"Range in exposure , Format 'a,b'",4},
  {"parameters",'p',"PARAMETERS",0,
   "Set of oscillation parameters\n   for which the data are computed\n"
   "   Format 'th12,th13,th23,delta,dm21,dm31'",0},
  {"parameterse",'E',"PARAMETERS",0,
   "Set of relative oscillation parameter errors\n"
   "   Format 'th12e,th13e,th23e,deltae,dm21e,dm31e'",0},
  {"chi",'I',"NUMBER",0,"dim of systematics function to use, otherwise defaults are used"},
  {"bintobin",'b',"NUMBER",0,"uncorellated bin to bin error to use (if enabled in chi function)"},
  {"runType",'T',"NUMBER",0,"1=cpv sig vs dcp, 2=CP Resolution vs dcp, 3=MH Significance vs dcp"},
  {"test",'t',0,0,"will produce output using only glbChiSys (good for fast testing)"},
  {"hierarchy",'h',0,0,"Minimize over the opposite mass hierarchy"},
  { 0 } 
};
/* Program documentation. */
static char doc[] ="Producing senseVlum plots";
/* Used by `main' to communicate with `parse_opt'. */
struct arguments
{
  char* args[32];                /* up to 32 experiments */
  char *resolution;
  char *xrange;
  char *params, *paramse;
  int exp, test, hier;
  int chimode;
  double bintobin;
  int runType; //t, m, or c
};
/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
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
    case 'I':
      arguments->chimode = atof(arg);
      break;
    case 'b':
      arguments->bintobin = atof(arg);
      break;
    case 'T':
      arguments->runType = atoi(arg);
      break;
    case 't':
      arguments->test = YES;
      break;
    case 'h':
      arguments->hier = YES;
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

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

/* GLoBES parameter structures */
glb_params true_values;
glb_params test_values;
glb_params input_errors;

/* GSL stuff */
const gsl_root_fsolver_type *T;   /* GSL algorithm for root finding */
gsl_root_fsolver *s;              /* GSL Solver object */
gsl_function gsl_func;            /* Function to minimize */

void gslError(const char *reason, const char *file, int line, int gsl_errno)
{
  static int n_errors=0;
  
  printf("GSL Error(%d) in file %s:%d : %s\n",gsl_errno, file, line, gsl_strerror(gsl_errno));
  if (++n_errors > 50000)
  {
    printf("Too many GSL errors. Program aborted.\n");
    abort();
  }
}

double DoChiSquareCPR(double x, void *dummy)
{
  double chi2;
  
  /* Set vector of test values */
  glbSetOscParams(test_values, M_PI*x/180.0, GLB_DELTA_CP);

  /* Compute Chi^2 for all loaded experiments and all rules*/
  if(arguments.test){
		chi2 = glbChiSys(test_values, GLB_ALL, 0);
	}else{
		chi2 = glbChiDelta(test_values, test_values, GLB_ALL);
	}
  printf("\tchi2 is %f at %f\n",chi2,glbGetOscParams(test_values,GLB_DELTA_CP));
  return chi2 - 1.0;
}

void ComputeCPVCurve(double osc[])
{
  int i,k;
  double this_dcp;
  double chi2[4],chi2min;
  int opconsidered = (arguments.hier==YES ? 4 : 2);
 
	for(this_dcp=-M_PI;this_dcp<M_PI;this_dcp=this_dcp+(M_PI/18)){
		glbDefineParams(test_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc[5]);  
		glbSetDensityParams(test_values,1.0,GLB_ALL);


		double osc5[]={osc[5],osc[5],-osc[5]+osc[4],-osc[5]+osc[4]};
		double osc3[]={0,M_PI,0,M_PI};
		
		for(k=0;k<opconsidered;k++){
			glbDefineParams(true_values,osc[0],osc[1],osc[2],osc3[k],osc[4],osc5[k]);
			glbSetDensityParams(true_values,1.0,GLB_ALL);
			glbSetOscillationParameters(true_values);
			glbSetRates();
			if(arguments.test){
				chi2[k]=glbChiSys(test_values, GLB_ALL,0);
			}else{
				chi2[k]=glbChiDelta(test_values,test_values, GLB_ALL);
			}
			//glbPrintParams(stdout,test_values);
		}
	
		chi2min=chi2[0];
		for(k=0;k<opconsidered;k++){
			if(chi2[k]<chi2min) chi2min=chi2[k];
			//printf("%d,%f\n",i,chi2[k]);
		}
		AddToOutput2(this_dcp, chi2min);

	}
 
}

void ComputeMHSigCurve(double osc[])
{
  int i,k;
  double this_dcp;
  double chi2s[3];
 
	for(this_dcp=-M_PI;this_dcp<M_PI;this_dcp=this_dcp+(M_PI/18)){
		//glbDefineParams(test_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);  
		//glbSetDensityParams(test_values,1.0,GLB_ALL);

		//double osc3[]={-M_PI/2,0,M_PI/2};
		
		//for(k=0;k<3;k++){
			glbDefineParams(true_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc[5]);
			glbSetDensityParams(true_values,1.0,GLB_ALL);
			glbSetOscillationParameters(true_values);
			glbSetRates();
			
			//find degeneracies
			double xrange[]={-4,-0.1};
			double yrange[]={0,360};
			int resolution[]={25,25};
			glb_params *listb, Fit = NULL;
			double *fchi;
			int ct,i;
			double chi2  = 1.0e10;
			ct=eightfold(true_values,input_errors,YES,NO,xrange,yrange,resolution,&listb,&fchi);
			
			for(i=0; i < ct; i++){
				//printf("degen %d has chi2 %f\n",i,fchi[i]);
				//glbPrintParams(stdout,listb[i]);
				if (fchi[i] < chi2){
					chi2 = fchi[i];
					Fit  = listb[i];
				}
			}
			//chi2s[k]=chi2;
			//t13s[k]=glbGetOscParams(Fit,GLB_THETA_13);
			//printf("found %d degs with min chi2 of %f\n",ct,chi2);
		//}
		
		double a[]={this_dcp,sqrt(chi2),glbGetOscParams(Fit,GLB_DELTA_CP),glbGetOscParams(Fit,GLB_THETA_13)};
    AddArrayToOutput(a,4);
	}
 
}
void ComputeAllowedCP(double osc[])
{
  int i,j,k; 
  double chi2[2],chi2min;
  double this_dcptest,this_dcptrue;
  int divisions=18;
  int opconsidered = (arguments.hier==YES ? 2 : 1);
  double osc5[]={osc[5],-osc[5]+osc[4]};
  
  for(this_dcptest=-M_PI;this_dcptest<M_PI;this_dcptest=this_dcptest+(M_PI/divisions)){
		glbDefineParams(test_values,osc[0],osc[1],osc[2],this_dcptest,osc[4],osc[5]);  
		glbSetDensityParams(test_values,1.0,GLB_ALL);
	
		for(this_dcptrue=-M_PI;this_dcptrue<M_PI;this_dcptrue=this_dcptrue+(M_PI/divisions)){
			for(k=0;k<opconsidered;k++){
				glbDefineParams(true_values,osc[0],osc[1],osc[2],this_dcptrue,osc[4],osc5[k]);
				glbSetDensityParams(true_values,1.0,GLB_ALL);
				glbSetOscillationParameters(true_values);
				glbSetRates();
				if(arguments.test){
					chi2[k]=glbChiSys(test_values, GLB_ALL,0);
				}else{
					chi2[k]=glbChiDelta(test_values,test_values, GLB_ALL);
				}
			}
			chi2min=chi2[0];
			for(k=0;k<opconsidered;k++){
				if(chi2[k]<chi2min) chi2min=chi2[k];
			}
			double a[]={this_dcptest,this_dcptrue, chi2min};
			AddArrayToOutput(a,3);
		}
		/*add blank line between sets*/
		AddToOutputBlankline();
	}
}

void ComputeCPResolutionCurve(double osc[])
{
  double x,this_dcp;               /* Current value of dcp */
  double x_lo, x_hi;      /* Bracketing interval for root finder */
  int gsl_status;         /* GSL error code */
  int iter;               /* Iteration counter for root finder */
  const int max_iter=100; /* Maximum number of iterations allowed in root finder */
  double xup,xdown;
  
  /* Loop over all data points in this dataset, using log stepping */
  for(this_dcp=-M_PI;this_dcp<M_PI;this_dcp=this_dcp+(M_PI/36)){
    /* Calculate "true" event rates */
    glbDefineParams(true_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc[5]);
    glbSetDensityParams(true_values,1.0,GLB_ALL);
    glbDefineParams(test_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc[5]);  
    glbSetDensityParams(test_values,1.0,GLB_ALL);
    glbSetOscillationParameters(true_values);
    glbSetInputErrors(input_errors);
    glbSetRates();
    x=this_dcp;
	
    /* Determine resolution on dcp=0(-90) by  using the GSL Brent-Dekker
     * algorithm to find the dcp, for which chi^2 = 1 (1 sigma) */
    x_lo = 180*this_dcp/M_PI;
    x_hi = x_lo+0.01;
    iter = 0;
   printf("starting root finder for dcp=%f\n",this_dcp);
   gsl_status = gsl_root_fsolver_set(s, &gsl_func, x_lo, x_hi);
    /* Start root finder. The initial search interval is guessed based on the
     * results from a neighboring grid point */
    double deviation=1;
    do {
      //if (j > 0) {
        //x_lo = x_res[j-1] - deviation;
        //x_hi = min(x_res[j-1] + deviation, -0.001);
        x_lo = x_lo;
        x_hi = x_hi + deviation;
        //x_hi = min(x_sens[j-1], -0.001);
      //}
      printf("\tsetting initial search interval x_lo:%f, x_hi:%f\n",x_lo,x_hi);
      gsl_status = gsl_root_fsolver_set(s, &gsl_func, x_lo, x_hi);
      //deviation *= 1.1;
    } while (gsl_status != GSL_SUCCESS && x_hi<180 && x_hi>-180);
    
    /* Iterate root finder */
    do {
      iter++;
      gsl_status = gsl_root_fsolver_iterate(s);
      x          = gsl_root_fsolver_root(s);
      x_lo       = gsl_root_fsolver_x_lower(s);
      x_hi       = gsl_root_fsolver_x_upper(s);
      printf("\tx_lo:%f, x_hi:%f, x:%f\n",x_lo,x_hi,x);
      gsl_status = gsl_root_test_interval (x_lo, x_hi, 0.01, 0);
    } while (gsl_status == GSL_CONTINUE && iter < max_iter);
    printf("found minimum on iter %d at %f\n",iter,x);
    //store this upward going value
    xup=x;
    
    //do again but going down in dcp
    x_hi = 180*this_dcp/M_PI;
    x_lo = x_hi-0.01;
    
    iter = 0;
   printf("starting root finder for dcp=%f\n",this_dcp);
   gsl_status = gsl_root_fsolver_set(s, &gsl_func, x_lo, x_hi);
    /* Start root finder. The initial search interval is guessed based on the
     * results from a neighboring grid point */
    do{
      x_hi = x_hi;
      x_lo = x_lo-deviation;
      printf("\tsetting initial search interval x_lo:%f, x_hi:%f\n",x_lo,x_hi);
      gsl_status = gsl_root_fsolver_set(s, &gsl_func, x_lo, x_hi);
      //deviation *= 1.1;
    } while (gsl_status != GSL_SUCCESS && x_lo>-360);
    
    /* Iterate root finder */
    do {
      iter++;
      gsl_status = gsl_root_fsolver_iterate(s);
      x          = gsl_root_fsolver_root(s);
      x_lo       = gsl_root_fsolver_x_lower(s);
      x_hi       = gsl_root_fsolver_x_upper(s);
      printf("\tx_lo:%f, x_hi:%f, x:%f\n",x_lo,x_hi,x);
      gsl_status = gsl_root_test_interval (x_lo, x_hi, 0.01, 0);
    } while (gsl_status == GSL_CONTINUE && iter < max_iter);
    printf("found minimum on iter %d at %f\n",iter,x);
    //store this upward going value
    xdown=x;  
    
    /* Save results */
    //AddToOutput2(this_dcp, 0.5*(xup-180*osc[3]/M_PI - (xdown-180*osc[3]/M_PI)));
    double a[]={this_dcp,xup, xdown};
		AddArrayToOutput(a,3);
    //exit(0);
  }
}



int main(int argc, char *argv[])
{ 
  /* defaults */
  arguments.args[0]="-";
  arguments.params=NULL;
  arguments.xrange=NULL;
  arguments.chimode=-1;
  arguments.bintobin=0.0;
  arguments.runType=1;
  arguments.hier=NO;
  /* parsing the comand line */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);  

  int i;
  
  //set standard oscillation parameters 
  double osc[]={0.593,0.154,0.705,0,7.58e-5,2.35e-3};
  double osce[]={0.03,0.03,0.11,0,0.03,0.05};
  vector_read_double(arguments.params,osc,6,parse_error);
  vector_read_double(arguments.paramse,osce,6,parse_error);
  printf("Running with osc parameters, osc(%f,%f,%f,%f,%f,%f)\n",osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  printf("Running with relative errors, osce(%f,%f,%f,%f,%f,%f)\n",osce[0],osce[1],osce[2],osce[3],osce[4],osce[5]);
  if(arguments.test)
		printf("Running in test mode (glbChisys used everywhere).");
	printf("Opposite hierardchy %s be minimized over.\n",(arguments.hier==YES ? "will" : "will not"));
	
  /* Initialize GLoBES and define chi^2 functions */
  glbInit(argv[0]);                    
  glbDefineChiFunction(&chiSpectrumTiltSplitBG,7,"chiSpectrumTiltSplitBG7",NULL);
  glbDefineChiFunction(&chiSpectrumTiltSplitBG,11,"chiSpectrumTiltSplitBG11",NULL);
  glbDefineChiFunction(&chiSpectrumSplitBG_BtoB,4,"chiSpectrumSplitBG4",NULL);
  glbDefineChiFunction(&chiSpectrumSplitBG_BtoB,5,"chiSpectrumSplitBG5",NULL);
  glbDefineChiFunction(&chiSpectrumSplitBG_BtoB,7,"chiSpectrumSplitBG7",NULL);
  glbDefineChiFunction(&chiSpectrumSplitBG_BtoB,9,"chiSpectrumSplitBG9",NULL);
  glbDefineChiFunction(&chiSpectrumTiltCustom,4,"chiSpectrumTiltCustom",NULL);
  //glbDefineChiFunction(&chiSpectrumTiltCustom_BtoB,4,"chiSpectrumTiltCustom",NULL);

  //load experiments from command line arguments
  for(i=0;i<arguments.exp-1;i++)
  {
		printf("Loading glb file:%s\n",arguments.args[i]);
    int s=glbInitExperiment(arguments.args[i],&glb_experiment_list[0],&glb_num_of_exps);
    /* Testing for failure */
    if(s<-1) {fprintf(stderr,"%s: FATAL: Unrecoverable parse error\n",
          argv[0]);exit(1);}
  }
  
  //setup systematics dim variables
  double *old_sys_errors = glbGetSysErrorsListPtr(0, 0, GLB_ON); 
  int sys_dim = glbGetSysDimInExperiment(0, 0, GLB_ON);
  double new_errors[sys_dim];
  int idim=0;
  for(idim=0;idim<sys_dim;idim++) new_errors[idim]=old_sys_errors[idim];
  //set systematics functions based on commandline chimode
  switch(arguments.chimode){
    case 100:
      //chiZero -- is this equivalent to systematics off?
      glbSetChiFunction(GLB_ALL,GLB_ALL,GLB_ON,"chiZero",new_errors);
      break;
    case 101:
      //turn off systematics
      //glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
      //do this after init of experiments
      break;
    case 0:
      //chiSpectrumTilt
      glbSetChiFunction(GLB_ALL,GLB_ALL,GLB_ON,"chiSpectrumTilt",new_errors);
      break;
    case 2:
      //chiNoSysSpectrum
      glbSetChiFunction(GLB_ALL,GLB_ALL,GLB_ON,"chiNoSysSpectrum",new_errors);
      break;
    case 4:
      //chiTotalRatesTilt
      glbSetChiFunction(GLB_ALL,GLB_ALL,GLB_ON,"chiTotalRatesTilt",new_errors);
      break;
    case 7:
      //chiSpectrumOnly
      glbSetChiFunction(GLB_ALL,GLB_ALL,GLB_ON,"chiSpectrumOnly",new_errors);
      break;
    case 8:
      //chiNoSysTotalRates
      glbSetChiFunction(GLB_ALL,GLB_ALL,GLB_ON,"chiNoSysTotalRates",new_errors);
      break;
    case 9:
      //chiSpectrumCalib
      glbSetChiFunction(GLB_ALL,GLB_ALL,GLB_ON,"chiSpectrumCalib",new_errors);
      break;
  }
  //output chi sys setting
  char sys_id[255];
  glbGetChiFunction(0,0,GLB_ON,sys_id,255);
  printf("Chi function is set to: %s(sys_dim is %d))\n",sys_id,sys_dim);
  
  if(arguments.bintobin>0){
    sigma_binbin=arguments.bintobin;
    printf("sigma_binbin set to %f\n",sigma_binbin);
  }
  
  gsl_set_error_handler(&gslError);    /* Initialize GSL root finder */
  gsl_func.params   = NULL;
  T = gsl_root_fsolver_brent;
  //T = gsl_root_fsolver_falsepos;
  //T = gsl_root_fsolver_bisection;
  s = gsl_root_fsolver_alloc(T);

  /* Initialize parameter vectors */
  true_values  = glbAllocParams();
  test_values  = glbAllocParams();
  input_errors = glbAllocParams();
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbDefineParams(test_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);  
  glbSetDensityParams(test_values,1.0,GLB_ALL);
  glbDefineParams(input_errors,osce[0]*osc[0],osce[1]*osc[1],osce[2]*osc[2],osce[3]*osc[3],osce[4]*osc[4],osce[5]*osc[5]);
  glbSetDensityParams(input_errors, 0.02, GLB_ALL);
  glbSetOscillationParameters(true_values);
  glbSetInputErrors(input_errors);
  glbSetRates();


  InitOutput(arguments.args[arguments.exp-1],""); 
  if(arguments.chimode==101){
    glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
  }
  
  //handle runType settings
  printf("runtype is %d\n",arguments.runType);
  switch (arguments.runType){
    case 1:
      ComputeCPVCurve(osc);
      break; 
		case 2:
		  gsl_func.function = &DoChiSquareCPR;
      ComputeCPResolutionCurve(osc);
      break;
    case 3:
      ComputeMHSigCurve(osc);
      break; 
    case 4:
      ComputeAllowedCP(osc);
      break;
    default:
      printf("No runType set... exiting...\n");
      return(0);
      break; 
  } 

   /* Clean up */
  glbFreeParams(true_values);
  glbFreeParams(test_values); 
  glbFreeParams(input_errors);
  
  return 0;  
}


