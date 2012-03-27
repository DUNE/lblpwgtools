/* GLoBES -- General LOng Baseline Experiment Simulator
 * (C) 2002 - 2011,  The GLoBES Team
 *
 * GLoBES is mainly intended for academic purposes. Proper
 * credit must be given if you use GLoBES or parts of it. Please
 * read the section 'Credit' in the README file.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <argp.h>

#include "globes/globes.h"   /* GLoBES library */
#include "glb_tools_header.h"
#include "glb_tools_parser.h"


#define FAILURE -1
#define SUCCESS 0


const char *argp_program_version =
"globes-deltatheta "VERSION"\n(C) 2005, 2006, 2007 Patrick Huber\n"
"This is NOT free software see the source for copying conditions. There is NO\n"
"warranty; not even for MERCHANTABILITY or"
" FITNESS FOR A PARTICULAR PURPOSE.";
const char *argp_program_bug_address =
PACKAGE_BUGREPORT;

/* Program documentation. */
static char doc[] ="Chi^2 calculation in the theta13-delta plane with GLoBES";

/* A description of the arguments we accept. */
static char args_doc[] = "glb-file(s) output-file";

/* The options we understand. */
static struct argp_option options[] ={
  {"Define",'D',"DEFINITION",0,"Define AEDL variable",0},
  {"Resolution",'R',  "PAIR", 0,  
   "Number of points to be used in each direction. Format a,b",3},
  {"output",   'o', "FILE", 0,
   "Output to FILE instead of standard output",3},
  {"parameters",'p',"PARAMETERS",0,
   "Set of oscillation parameters\n   for which the data are computed\n"
   "   Format 'th12,th13,th23,delta,dm21,dm31'",0},
  {"Xrange",'X',"RANGE",0,"Range in log(sin^2 2theta13), Format 'a,b'",3},
  {"Yrange",'Y',"RANGE",0,"Range in delta in degrees, Format 'a,b'",3},
  {"Oktant", 'O', 0, 0,"Compute chi^2 with th23->pi/2-th23",1},
  {"Hierarchy",'H', 0, 0,"Compute chi^2 with dm31->-dm31 + 2 dm_21 cos^2th12",1}, 
  {"correlations",'c',0,0,"Inlcude correlations (slow)",2},
  {"both",'b',0,0,"Output w/o correlations and correlations included",2},
  {"minimum",'m',"FILE",0,"Put the position of the minimum into FILE",2},
  {"atmospheric",'a',"NUMBER",0,"Relative error on theta23 and dm31 each,\ndefault is 0.05",2},
  {"solar",'s',"NUMBER",0,"Relative error on theta12 and dm21 each,\ndefault is 0.1",2},
  {"density",'d',"NUMBER",0,"Relative error on the matter density,\ndefault is 0.05",2},
  {"Cp",'C',0,0,"CP sensitivity"},
  {"Atmospheric",'A',0,0,"compute chi^2 in the theta23, dm31 plane"},
  {"Atmospheric",'S',0,0,"compute chi^2 in the theta12, dm21 plane"},
  {"rho",'r',0,0,"compute chi^2 in the density (1st experiment), delta plane"},
  {"Dm31",'M',0,0,"compute chi^2 projected on the dm31 axis"},
  {"Theta23",'T',0,0,"compute chi^2 projected on the theta23 axis"},
  {"Fast",'F',0,0,"Fast minimizer (experimental feature)",6},
  {"Luminosity",'L',"NUMBER",0,
   "Rescale total luminosity by NUMBER (default=1)",6},
  {"quiet",'q',0,0,"silent operation"},
  { 0 } 
};

/* Used by `main' to communicate with `parse_opt'. */
struct arguments
{
  int exp,oct,hier,cor,bot,cps,silent,Atm,DM,TH,SOL,rho;           /* number of experiments */
  char* args[32];                /* up to 32 experiments */
  char *resolution;
  char *output_file,*min_file;
  char *params,*xrange,*yrange;
  double solar,atm,density;
  int fast;
  double lumi;
};


#define YES 1
#define NO 0

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the INPUT argument from `argp_parse', which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;
  
  switch (key)
    {
      
    case 'L':
      arguments->lumi = atof(arg);
      break; 

    case 'D':
      parse_definition(arg);
      break;
    case 'q':
      arguments->silent = YES;
      break; 
    case 'C':
      arguments->cps = YES;
      break;
    case 'A':
      arguments->Atm = YES;
      break; 
    case 'T':
      arguments->TH = YES;
      break;
    case 'M':
      arguments->DM = YES;
      break; 
    case 'R':
      arguments->resolution = arg;
      break; 

    case 'r':
      arguments->rho = YES;
      break; 

    case 'a':
      arguments->atm = atof(arg);
      break; 
    case 's':
      arguments->solar = atof(arg);
      break; 
    case 'd':
      arguments->density = atof(arg);
      break; 
    case 'o':
      arguments->output_file = arg;
      break;
    case 'p':
      arguments->params = arg;
      break;
    case 'c':
      arguments->cor = YES;
      break;
    case 'b':
      arguments->bot = YES;
      arguments->cor = -100; 
      break;
    case 'X':
      arguments->xrange = arg;
      break;
    case 'Y':
      arguments->yrange = arg;
      break;
    case 'O':
      arguments->oct = YES;
      break;
    case 'S':
      arguments->SOL = YES;
      break;
    case 'H':
      arguments->hier = YES;
      break;
    case 'm':
      arguments->min_file = arg;
      break;

    case 'F':
      arguments->fast = YES;
      break; 
      
    case ARGP_KEY_ARG:
      if (state->arg_num > 32)
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




int rescale_lumi(int exp, double factor)
{
  int s;
  double l0;
  l0=glbGetTargetMass(exp);
  s=glbSetTargetMass(exp,l0*factor);
  return s;
}



/**********************************************************************/
int main(int argc, char *argv[])
{
  double progress;
  int count=0;
  int s,i,rv;
  FILE *output=NULL,*minfile=NULL;
  double osc[]={0.553574,0.160875,M_PI/4,0.0,0.00007,0.003};
  double xrange[]={-3,-1};
  double yrange[]={0,360};
  int resolution[]={10,10};
  char* central_values=NULL;
  char* base=NULL;
  struct arguments arguments;
  /* defaults */
  arguments.args[0]="-";
  arguments.resolution=NULL;
  arguments.output_file=NULL;
  arguments.exp=0;
  arguments.params=NULL;
  arguments.xrange=NULL;
  arguments.yrange=NULL;
  arguments.oct=NO;
  arguments.hier=NO;
  arguments.cor=NO;
  arguments.min_file=NULL;
  arguments.solar=0.1;
  arguments.atm=0.05;
  arguments.density=0.05;
  arguments.bot=NO;
  arguments.cps=NO;
  arguments.silent=NO;
  arguments.Atm=NO;
  arguments.TH=NO;
  arguments.DM=NO;
  arguments.SOL=NO;
  arguments.rho=NO;
  arguments.fast=NO;
  arguments.lumi=1.0;
  /* parsing the comand line */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);  

  /* Lexing the oscillation parameters as given by the environment
   * variable GLB_CENTRAL_VALUES
   */
  central_values=getenv("GLB_CENTRAL_VALUES");
 
  vector_read_double(central_values,osc,6,parse_error);
  vector_read_double(arguments.params,osc,6,parse_error);
  vector_read_double(arguments.xrange,xrange,2,parse_error);
  vector_read_double(arguments.yrange,yrange,2,parse_error);
 
  vector_read_int(arguments.resolution,resolution,2,parse_error);
 
  /* getting the outputfile right */
  base=strdup(arguments.args[arguments.exp-1]);
  if(!base) {fprintf(stderr,"No mem\n");exit(1);}
  /* possible source of the strange segfault in malloc_consolidate */
  base=(char *) realloc(base,(strlen(base)+5)*sizeof(char));
  if(!base) {fprintf(stderr,"No mem\n");exit(1);}
  base=strcat(base,".dat");
  output=fopen(base,"w");
  if(output==NULL) {
    fprintf(stderr,"FATAL: %s: Could not open output file '%s'\n",argv[0],
	    base);
    exit(1);
  }
  

  print_header(output,"globes-deltatheta",VERSION,argc,argv);

  if(arguments.min_file!=NULL) 
    {    
      minfile=fopen(arguments.min_file,"w");
      if(minfile==NULL) {
	fprintf(stderr,"FATAL: %s: Could not open minimum file '%s'\n",argv[0],
		arguments.min_file);
	exit(1);
      }
    }


  /* Initialize libglobes */
  glbInit(argv[0]);
  if(arguments.fast==YES) glbSelectMinimizer(GLB_MIN_POWELL);
 
  for(i=0;i<arguments.exp-1;i++)
    {
      s=glbInitExperiment(arguments.args[i],
			  &glb_experiment_list[0],&glb_num_of_exps);

      /* Testing for failure */
      if(s<-1) {fprintf(stderr,"%s: FATAL: Unrecoverable parse error\n",
			argv[0]);exit(1);}
    }
  
  for(i=0;i<arguments.exp-1;i++) {rescale_lumi(i,arguments.lumi);}


  /* reasonable default range for -A */
  /*if(arguments.Atm==YES)
    {
      xrange[0]=30;
      xrange[1]=60;
      yrange[0]=0.001;
      yrange[1]=0.003;
    }
  */
  /* Initialize parameter vector(s) */
  glb_params true_values = glbAllocParams();
  glb_params test_values = glbAllocParams();
  glb_params starting_values = glbAllocParams();
  glb_params input_errors = glbAllocParams();
  glb_params minimum = glbAllocParams();
  glb_projection proj=glbAllocProjection();

  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbCopyParams(true_values,test_values);  
  
  if(arguments.hier==YES) {
    osc[5]=-osc[5]+2*cos(osc[0])*cos(osc[0])*osc[4];
    
    glbSetOscParams(test_values,osc[5],GLB_DM_ATM);}
  if(arguments.oct==YES) glbSetOscParams(test_values,M_PI/2-osc[2],GLB_THETA_23);
  
  glbCopyParams(test_values,starting_values);
  glbDefineParams(input_errors,osc[0]*arguments.solar,0,osc[2]*arguments.atm,0,
		  osc[4]*arguments.solar,osc[5]*arguments.atm);
  glbSetDensityParams(input_errors,arguments.density,GLB_ALL);
  glbSetInputErrors(input_errors);
  glbSetStartingValues(starting_values);

  /* The simulated data are computed */
  glbSetOscillationParameters(true_values);
  glbSetRates();

   if(arguments.Atm==YES)
    {
      /*switching off the atmospheric prior */ 
      glbDefineParams(input_errors,osc[0]*arguments.solar,0,0,0,
		      osc[4]*arguments.solar,0);
      glbSetDensityParams(input_errors,arguments.density,GLB_ALL);
      glbSetInputErrors(input_errors);

     /* Iteration over all values to be computed */
      double x,y,res,res2;    
      resolution[0]=resolution[0]-1;
      resolution[1]=resolution[1]-1;
      for(x=xrange[0];x<=xrange[1]+(xrange[1]-xrange[0])/resolution[0]/2;
	  x=x+(xrange[1]-xrange[0])/resolution[0])
	for(y=yrange[0];y<=yrange[1]+(yrange[1]-yrange[0])/resolution[1]/2;
	    y=y+(yrange[1]-yrange[0])/resolution[1])
	  {
	    /* Defining the right projection: we want th12 and dm21 to
	       be varied, but keep th13=0 and thus get also rid of the
	       CP phase 
	    */
	    glbDefineProjection(proj,GLB_FREE,GLB_FIXED,  
				GLB_FIXED,GLB_FIXED,GLB_FREE,GLB_FIXED); 
	    glbSetProjection(proj);
	    

	    /* Set vector of test values */
	    glbSetOscParams(test_values,x*M_PI/180.0,GLB_THETA_23);
	    glbSetOscParams(test_values,y,GLB_DM_ATM);
	    count++;
	    progress=count/(resolution[0]+1)/(resolution[1]+1)*100;
	    if(arguments.silent==NO) fprintf(stderr,
					     "progress %3.1f%%\r",progress);
	    if(arguments.cor==NO)
	      {
		/* Compute Chi^2 for all loaded experiments and all rules */
		res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
		fprintf(output,"%f %f %f\n",x,y,res);
	      }  
	    if(arguments.cor==YES)
	      {    
		/* Compute Chi^2 for all loaded experiments and all rules */
		res=glbChiNP(test_values,minimum,GLB_ALL);
		fprintf(output,"%f %f %f\n",x,y,res);
		if(minfile!=NULL) {
		  glbPrintParams(minfile,minimum);fflush(minfile);}
		fflush(output);
	      }
	    if(arguments.bot==YES)
	      {
		res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
		res2=glbChiNP(test_values,minimum,GLB_ALL);
		fprintf(output,"%f %f %f %f\n",x,y,res,res2);
		if(minfile!=NULL) {
		  glbPrintParams(minfile,minimum);fflush(minfile);}
		fflush(output);
	      }
	  }
      if(arguments.silent==NO) fprintf(stderr,"\n");
    }




   if(arguments.rho==YES)
    {
      arguments.Atm=YES;
      /*switching off the atmospheric prior */ 
      // glbDefineParams(input_errors,osc[0]*arguments.solar,0,0,0,
      //		      osc[4]*arguments.solar,0);
      glbSetDensityParams(input_errors,0,GLB_ALL);
      glbSetInputErrors(input_errors);

     /* Iteration over all values to be computed */
      double x,y,res,res2;    
      resolution[0]=resolution[0]-1;
      resolution[1]=resolution[1]-1;
      for(x=xrange[0];x<=xrange[1]+(xrange[1]-xrange[0])/resolution[0]/2;
	  x=x+(xrange[1]-xrange[0])/resolution[0])
	for(y=yrange[0];y<=yrange[1]+(yrange[1]-yrange[0])/resolution[1]/2;
	    y=y+(yrange[1]-yrange[0])/resolution[1])
	  {
	    /* Defining the right projection */
	    
	    glbDefineProjection(proj,GLB_FREE,GLB_FREE,  
				GLB_FREE,GLB_FIXED,GLB_FREE,GLB_FREE);
	    glbSetDensityProjectionFlag(proj,GLB_FIXED,GLB_ALL);
	    glbSetProjection(proj);
	    

	    /* Set vector of test values */
	    glbSetOscParams(test_values,y*M_PI/180.0,GLB_DELTA_CP);
	    glbSetDensityParams(test_values,x,0);
	    count++;
	    progress=count/(resolution[0]+1)/(resolution[1]+1)*100;
	    if(arguments.silent==NO) fprintf(stderr,
					     "progress %3.1f%%\r",progress);
	    if(arguments.cor==NO)
	      {
		/* Compute Chi^2 for all loaded experiments and all rules */
		res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
		fprintf(output,"%f %f %f\n",x,y,res);
	      }  
	    if(arguments.cor==YES)
	      {    
		/* Compute Chi^2 for all loaded experiments and all rules */
		res=glbChiNP(test_values,minimum,GLB_ALL);
		fprintf(output,"%f %f %f\n",x,y,res);
		if(minfile!=NULL) {
		  glbPrintParams(minfile,minimum);fflush(minfile);}
		fflush(output);
	      }
	    if(arguments.bot==YES)
	      {
		res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
		res2=glbChiNP(test_values,minimum,GLB_ALL);
		fprintf(output,"%f %f %f %f\n",x,y,res,res2);
		if(minfile!=NULL) {
		  glbPrintParams(minfile,minimum);fflush(minfile);}
		fflush(output);
	      }
	  }
      if(arguments.silent==NO) fprintf(stderr,"\n");
    }


   double thetheta;
   if(arguments.SOL==YES)
     {
       /*switching off the atmospheric prior */ 
       glbDefineParams(input_errors,0,0,0,0,0,0);
       glbSetDensityParams(input_errors,arguments.density,GLB_ALL);
       glbSetInputErrors(input_errors);
       
       /* Iteration over all values to be computed */
       double x,y,res,res2;    
       resolution[0]=resolution[0]-1;
       resolution[1]=resolution[1]-1;
       
       for(x=xrange[0];x<=xrange[1]+(xrange[1]-xrange[0])/resolution[0]/2;
	   x=x+(xrange[1]-xrange[0])/resolution[0])
	 for(y=yrange[0];y<=yrange[1]+(yrange[1]-yrange[0])/resolution[1]/2;
	     y=y+(yrange[1]-yrange[0])/resolution[1])
	   {
	     /* Defining the right projection: we want th12 and dm21 to
		be varied, but keep th13=0 and thus get also rid of the
		CP phase 
	     */
	     glbDefineProjection(proj,GLB_FIXED,GLB_FREE,GLB_FREE,
				 GLB_FREE,GLB_FIXED,GLB_FREE); 
	     glbSetDensityProjectionFlag(proj,GLB_FREE,GLB_ALL);
	     glbSetProjection(proj);
	    

	     /* Set vector of test values */
	     thetheta=asin(sqrt(pow(10,x)))/2;
	     glbSetOscParams(test_values,thetheta,GLB_THETA_12);
	     glbSetOscParams(test_values,pow(10,y),GLB_DM_SOL);
	     count++;
	     progress=count/(resolution[0]+1)/(resolution[1]+1)*100;
	     if(arguments.silent==NO) fprintf(stderr,
					      "progress %3.1f%%\r",progress);
	     if(arguments.cor==NO)
	       {
		 /* Compute Chi^2 for all loaded experiments and all rules */
		 res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
		 fprintf(output,"%f %f %f\n",x,y,res);
	       }  
	     if(arguments.cor==YES)
	       {    
		 /* Compute Chi^2 for all loaded experiments and all rules */
		 res=glbChiNP(test_values,minimum,GLB_ALL);
		 fprintf(output,"%f %f %f\n",x,y,res);
		 if(minfile!=NULL) {
		   glbPrintParams(minfile,minimum);fflush(minfile);}
		 fflush(output);
	       }
	     if(arguments.bot==YES)
	       {
		 res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
		 res2=glbChiNP(test_values,minimum,GLB_ALL);
		 fprintf(output,"%f %f %f %f\n",x,y,res,res2);
		 if(minfile!=NULL) {
		   glbPrintParams(minfile,minimum);fflush(minfile);}
		 fflush(output);
	       }
	   }
       if(arguments.silent==NO) fprintf(stderr,"\n");
    }


   if(arguments.TH==YES)
     {
      /*switching off the atmospheric prior */ 
       glbDefineParams(input_errors,osc[0]*arguments.solar,0,0,0,
		       osc[4]*arguments.solar,0);
       glbSetDensityParams(input_errors,arguments.density,GLB_ALL);
       glbSetInputErrors(input_errors);
       
       /* Iteration over all values to be computed */
       double x,res,res2;    
       resolution[0]=resolution[0]-1;
       resolution[1]=resolution[1]-1;
      for(x=xrange[0];x<=xrange[1]+(xrange[1]-xrange[0])/resolution[0]/2;
	  x=x+(xrange[1]-xrange[0])/resolution[0])
	{
	  /* Defining the right projection: we want th12 and dm21 to
	     be varied, but keep th13=0 and thus get also rid of the
	     CP phase 
	  */
	  glbDefineProjection(proj,GLB_FREE,GLB_FIXED,  
				GLB_FIXED,GLB_FIXED,GLB_FREE,GLB_FREE); 
	  glbSetProjection(proj);
	    

	  /* Set vector of test values */
	  glbSetOscParams(test_values,x*M_PI/180.0,GLB_THETA_23);
	  count++;
	  progress=count/pow(resolution[0]+1,1)*100;
	  if(arguments.silent==NO) fprintf(stderr,"progress %3.1f%%\r",progress);
	  if(arguments.cor==NO)
	    {
	      /* Compute Chi^2 for all loaded experiments and all rules */
	      res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
	      fprintf(output,"%f %f\n",x,res);
	    }  
	  if(arguments.cor==YES)
	    {    
	      /* Compute Chi^2 for all loaded experiments and all rules */
	      res=glbChiNP(test_values,minimum,GLB_ALL);
	      fprintf(output,"%f %f\n",x,res);
	      if(minfile!=NULL) {glbPrintParams(minfile,minimum);fflush(minfile);}
	      fflush(output);
	    }
	  if(arguments.bot==YES)
	    {
	      res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
	      res2=glbChiNP(test_values,minimum,GLB_ALL);
	      fprintf(output,"%f %f %f\n",x,res,res2);
	      if(minfile!=NULL) {glbPrintParams(minfile,minimum);fflush(minfile);}
	      fflush(output);
	    }
	}
      if(arguments.silent==NO) fprintf(stderr,"\n");
      arguments.Atm=YES;
    }


 if(arguments.DM==YES)
    {
      /*switching off the atmospheric prior */ 
      glbDefineParams(input_errors,osc[0]*arguments.solar,0,0,0,
		      osc[4]*arguments.solar,0);
      glbSetDensityParams(input_errors,arguments.density,GLB_ALL);
      glbSetInputErrors(input_errors);

     /* Iteration over all values to be computed */
      double y,res,res2;    
      resolution[0]=resolution[0]-1;
      for(y=yrange[0];y<=yrange[1]+(yrange[1]-yrange[0])/resolution[0]/2;
	  y=y+(yrange[1]-yrange[0])/resolution[0])
	{
	  /* Defining the right projection: we want th12 and dm21 to
	     be varied, but keep th13=0 and thus get also rid of the
	     CP phase 
	  */
	  glbDefineProjection(proj,GLB_FREE,GLB_FIXED,  
				GLB_FREE,GLB_FIXED,GLB_FREE,GLB_FIXED); 
	  glbSetProjection(proj);
	    

	  /* Set vector of test values */
	  glbSetOscParams(test_values,y,GLB_DM_ATM);
	  count++;
	  progress=count/pow(resolution[0]+1,1)*100;
	  if(arguments.silent==NO) fprintf(stderr,"progress %3.1f%%\r",progress);
	  if(arguments.cor==NO)
	    {
	      /* Compute Chi^2 for all loaded experiments and all rules */
	      res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
	      fprintf(output,"%f %f\n",y,res);
	    }  
	  if(arguments.cor==YES)
	    {    
	      /* Compute Chi^2 for all loaded experiments and all rules */
	      res=glbChiNP(test_values,minimum,GLB_ALL);
	      fprintf(output,"%f %f\n",y,res);
	      if(minfile!=NULL) {glbPrintParams(minfile,minimum);fflush(minfile);}
	      fflush(output);
	    }
	  if(arguments.bot==YES)
	    {
	      res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
	      res2=glbChiNP(test_values,minimum,GLB_ALL);
	      fprintf(output,"%f %f %f\n",y,res,res2);
	      if(minfile!=NULL) {glbPrintParams(minfile,minimum);fflush(minfile);}
	      fflush(output);
	    }
	}
      if(arguments.silent==NO) fprintf(stderr,"\n");
      arguments.Atm=YES;
    }


  if(arguments.Atm==NO)
    {
      /* Iteration over all values to be computed */
      double thetheta13,x,y,res,res2;    
      resolution[0]=resolution[0]-1;
      resolution[1]=resolution[1]-1;
      for(x=xrange[0];x<=xrange[1]+(xrange[1]-xrange[0])/resolution[0]/2;
	  x=x+(xrange[1]-xrange[0])/resolution[0])
	for(y=yrange[0];y<=yrange[1]+(yrange[1]-yrange[0])/resolution[1]/2;
	    y=y+(yrange[1]-yrange[0])/resolution[1])
	  {
	    /* Set vector of test values */
	    thetheta13=asin(sqrt(pow(10,x)))/2;
	    glbSetOscParams(test_values,thetheta13,GLB_THETA_13);
	    glbSetOscParams(test_values,y*M_PI/180.0,GLB_DELTA_CP);
	    count++;
	    progress=count/(resolution[0]+1)/(resolution[1]+1)*100;
	    if(arguments.silent==NO) fprintf(stderr,"progress %3.1f%%\r",progress);
	    if(arguments.cor==NO)
	      {
		if(arguments.cps==NO)
		  {
		    /* Compute Chi^2 for all loaded experiments and all rules */
		    res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
		    fprintf(output,"%f %f %f\n",x,y,res);
		  }
		else
		  {
		    glbDefineProjection(proj,GLB_FIXED,GLB_FREE,
					GLB_FIXED,GLB_FIXED,GLB_FIXED,GLB_FIXED);
		    glbSetProjection(proj);
		    glbSetOscParams(true_values,thetheta13,GLB_THETA_13);
		    glbSetOscParams(true_values,y*M_PI/180.0,GLB_DELTA_CP);
		    glbSetOscillationParameters(true_values);
		    glbSetRates();
		    glbSetOscParams(test_values,0,GLB_DELTA_CP);
		    res=glbChiNP(test_values,NULL,GLB_ALL);
		    glbSetOscParams(test_values,M_PI,GLB_DELTA_CP);
		    res2=glbChiNP(test_values,NULL,GLB_ALL);
		
		    if(res<=res2) fprintf(output,"%f %f %f\n",x,y, res);
		    else fprintf(output,"%f %f %f\n",x,y, res2);
		  }
	      }
	    if(arguments.cor==YES)
	      {    
		/* Compute Chi^2 for all loaded experiments and all rules */
		res=glbChiThetaDelta(test_values,minimum,GLB_ALL);
		fprintf(output,"%f %f %f\n",x,y,res);
		if(minfile!=NULL) {glbPrintParams(minfile,minimum);fflush(minfile);}
		fflush(output);
	      }
	    if(arguments.bot==YES)
	      {
		res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
		res2=glbChiThetaDelta(test_values,minimum,GLB_ALL);
		fprintf(output,"%f %f %f %f\n",x,y,res,res2);
		if(minfile!=NULL) {glbPrintParams(minfile,minimum);fflush(minfile);}
		fflush(output);
	      }
	  }
      if(arguments.silent==NO) fprintf(stderr,"\n");
    }


  /* Destroy parameter vector(s) */
  glbFreeParams(true_values);
  glbFreeParams(test_values); 
  glbFreeParams(starting_values);
  glbFreeParams(input_errors); 
  glbFreeParams(minimum); 

  print_footer(output);
  fclose(output);
  exit(0);
}
