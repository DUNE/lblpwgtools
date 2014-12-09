#define MGT_HELPER_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <argp.h>
#include <float.h>
#include "glb_multiex.h"
#include "glb_types.h"
#include "glb_error.h"
#include "glb_fluxes.h"
#include "glb_probability.h"
#include "glb_rate_engine.h" //for access to set_new_rates
#include "globes/globes.h"   /* GLoBES library */
#include "customChis.h"
#include "priors.h"
#include "glb_tools_eightfold.h"
#include <gsl/gsl_math.h>    /* GNU Scientific library (required for root finding) */
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <stdarg.h> //for debug version of printf (dprintf)



/*argument parsing*/
struct arguments arguments;
static char args_doc[] = "";
static struct argp_option options[] ={
  {"Define",'D',"DEFINITION",0,"Define AEDL variable",0},
  {"xrange",'x',"RANGE",0,"Range in exposure or variable , Format 'a,b'",4},
  {"parameters",'p',"PARAMETERS",0,
   "Set of oscillation parameters\n   for which the data are computed\n"
   "   Format 'th12,th13,th23,delta,dm21,dm31'",0},
  {"parameterse",'E',"PARAMETERS",0,
   "Set of relative oscillation parameter errors\n"
   "   Format 'th12e,th13e,th23e,deltae,dm21e,dm31e'",0},
  {"chi",'I',"NUMBER",0,"dim of systematics function to use, otherwise defaults are used"},
  {"bintobin",'b',"NUMBER",0,"uncorellated bin to bin error to use (if enabled in chi function)"},
  {"runCat",'C',"NUMBER",0,"category of run to do; see code"},
  {"runType",'T',"NUMBER",0,"type of run to do with run category; see code"},
  {"test",'t',0,0,"will produce output using only glbChiSys (good for fast testing)"},
  {"hierarchy",'h',0,0,"Minimize over the opposite mass hierarchy"},
  {"ematrixfile",'e',"RANGE",0,"Filename template for ematrix input files (with %d).",4},
  {"covmatrixfile",'i',"RANGE",0,"Filename template for covar matrix input file.",4},
  {"debug",'d',0,0,"Print debug output."},
	{"chigoal",'c',"NUMBER",0,"Chi^2 goal for runtypes that minimize (1sigma=1.0 is default, e.g. 3sigma=9.0)"},
	{"resolution",'r',"NUMBER",0,"number of points in exposure or variable to use"},
	{"varied",'v',"NUMBER",0,"number of configs that are varied in terms of runtime, rest are held fixed (0= all varied (default))"},
	{"zero",'z',0,0,"Include a zero point (regardless of xrange)"},
  {"systs",'s',"NUMBER",0,"number of systematics response functions to load"},
  {"part",'P',"RANGE",0,"Part x of y to run, Format 'x,y'",4},
  {"pflucts",'f',"NUMBER",0,"Number of pseudo experiments to run (if enabled)"},
  {"nuisoutput",'n',"NUMBER",0,"Add nuisance parameters to output"},
  {"regprior",'R',"NUMBER",0,"Register prior number x, default to GLoBES default prior"},
  { 0 }
};

static char doc[] ="";
struct arguments{
  char* args[32];                /* up to 32 experiments */
  char *resolution;
  char *xrange;
  char *inpart;
  char *params, *paramse;
  char *ematrixfile;
  char *covmatrixfile;
  int exp, test, hier, zero, varied, debug, systs;
  int chimode;
  double bintobin;
  int runType, runCat;
  /*from senseVlum*/
  double min_runtime;  /* Minimum running time to consider [years] */
  double max_runtime;   /* Maximum running time to consider [years] */
  double chi2_goal;  /* Desired chi^2 value in root finder (2.7 = 90% C.L.) */
  int tSteps;            /* Number of data points for each curve */
  double logs22th13_precision;/* Desired precision of log(sin[th13]^2) in root finder */
	int preScan, scanVar; //these are used by some runtypes to setup scanning and prescanning
  int part[2]; //stores part to run, and number of parts to run, respectively
  int pflucts; //determines the number of pseduo experiments to run
  int nuis_output; //if enabled, add nuisance parameters to output
  int regprior; //0 is default for GLoBES built-in prior
  
  //not input variables, just want global access to these:
  int nuisances; //total number of nuisance parameters
};
/* argp parser. */
error_t parse_opt (int key, char *arg, struct argp_state *state);
static struct argp argp = { options, parse_opt, "glb-file(s) output-file", doc };


/* GLoBES parameter structures */
glb_params true_values;
glb_params test_values;
glb_params input_errors;
glb_params central_values;

/* GSL stuff */
const gsl_root_fsolver_type *T;   /* GSL algorithm for root finding */
gsl_root_fsolver *s;              /* GSL Solver object */
gsl_function gsl_func;            /* Function to minimize */


void parse_definition(const char *in);
int vector_read_double(const char *str, double *result, 
		       size_t n, void (*parse_error)(void));
void parse_error();
void parse_setdefaults();
void definechifunctions();

int compare(const void *x, const void *y);
int compare_asc(const void *x, const void *y);
void InitOutput(char* filename, char* headline);
void AddToOutput2(double n1,double n2);
void AddArrayToOutput(double a[],int n);
void AddToOutputBlankline();
void gslError(const char *reason, const char *file, int line, int gsl_errno);
double min(double x, double y);
void dprintf( const char* format, ... );
void mgt_set_new_rates(int exp, double** mult_presmear_effs[], double** mult_postsmear_effs[]);

double pstart(double low, double high, double inc);
double pend(double low, double high, double inc);
double pinc(double low, double high, double inc);

void pfluctTrueSpectra();
void PrintRatesToFile(int truefit, int curexp, int rule);
