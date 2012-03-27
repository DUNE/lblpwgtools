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



#ifdef GLB_STATIC
#include   <ltdl.h>
#endif /* GLB_STATIC */


#include "globes/globes.h"   /* GLoBES library */
#include "glb_tools_header.h"


#define FAILURE -1
#define SUCCESS 0


const char *argp_program_version =
"globes-simple "VERSION"\n(C) 2005, 2006 Patrick Huber\n"
"This is NOT free software see the source for copying conditions. There is NO\n"
"warranty; not even for MERCHANTABILITY or"
" FITNESS FOR A PARTICULAR PURPOSE.";
const char *argp_program_bug_address =
PACKAGE_BUGREPORT;

/* Program documentation. */
static char doc[] ="Reads values from a file and performs a chi^2 calculation in the theta13-delta plane with GLoBES";

/* A description of the arguments we accept. */
static char args_doc[] = "glb-file(s)";

/* The options we understand. */
static struct argp_option options[] ={
  {"Define",'D',"DEFINITION",0,"Define AEDL variable",0},
  {"output",   'o', "FILE", 0,
   "Output to FILE instead of standard output",3},
  {"parameters",'p',"PARAMETERS",0,
   "Set of oscillation parameters\n   for which the data are computed\n"
   "   Format 'th12,th13,th23,delta,dm21,dm31'",0},
  {"Oktant", 'O', 0, 0,"Compute chi^2 with th23->pi/2-th23",1},
  {"Hierarchy",'H', 0, 0,"Compute chi^2 with dm31->-dm31",1}, 
  {"correlations",'c',0,0,"Inlcude correlations (slow)",2},
  {"atmospheric",'a',"NUMBER",0,"Relative error on theta23 and dm31 each,\ndefault is 0.05",2},
  {"solar",'s',"NUMBER",0,"Relative error on theta12 and dm21 each,\ndefault is 0.1",2},
  {"density",'d',"NUMBER",0,"Relative error on the matter density,\ndefault is 0.05",2},
  {"quiet",'q',0,0,"silent operation"},
  { 0 } 
};

/* Used by `main' to communicate with `parse_opt'. */
struct arguments
{
  int exp,oct,hier,cor,silent;           /* number of experiments */
  char* args[32];                /* up to 32 experiments */
  char *output_file,*min_file;
  char *params;
  double solar,atm,density;
};


static void parse_definition(const char *in)
{
  const char *delim="=";
  double val;
  char *token=NULL;
  char *lhs=NULL;
  char *rhs=NULL;
  char *inc=NULL;
  size_t length=0;

  inc=strdup(in);
  if(inc==NULL) return;
  token=strtok(inc,delim);
  if(token!=NULL) 
    {
     lhs=strdup(token);
     length++;
    }
  token=strtok(NULL,delim); 
  if(token!=NULL) 
    {
     rhs=strdup(token);
     length++;
    }
  
  if(length!=2) {
    free(lhs);
    free(rhs);
    fprintf(stderr,"ERROR: Definition is not of form 'DEFINITION=VALUE'\n");
    return;}
  val=atof(rhs);
  glbDefineAEDLVariable(lhs,val);
  free(lhs);
  free(rhs);
  return;
}


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
    case 'D':
      parse_definition(arg);
      break;
    case 'q':
      arguments->silent = YES;
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
    case 'O':
      arguments->oct = YES;
      break;
    case 'H':
      arguments->hier = YES;
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

int get_values(FILE *fp,FILE *out,double ***list)
{
  int s,ts=0;
  float b1,b2;
  char tok;
  double **tmp=NULL;
  if(fp!=NULL)
    { 
      


      while (1)
	{
	  /* get rid of comments inside */
	  tok=fgetc(fp);
	  
	  if(tok=='#')
	    {
	      b1=1;b2=1;ts++;
	      while(fgetc(fp)!='\n');
	    
	    }
	  else if (tok==EOF)
	    {
	      return ts;
	    }
	  else
	    {
	      ungetc(tok,fp);
	      /* read the data */
	      s=fscanf(fp,"%f %f\n",
		       &b1,
		       &b2);
	    
	      
	      if(s!=2)
		{ 
                    fprintf(stderr,"Error: Wrong format in file\n");
		    return -1;
		}
	      else
		{
		  ts++;
		}
	      
	    }
	  
	  //fprintf(stderr,"%d %d %f %f\n",ts,s,b1,b2);
	      tmp=(double **) realloc(tmp,sizeof(double *)*ts);
	      tmp[ts-1]=(double *) malloc(sizeof(double)*2);
		      tmp[ts-1][0]=(double) b1;
		      tmp[ts-1][1]=(double) b2;
	      *list=tmp;
				       
	} 
     
    }
  else
    {
      return -2;
    }
  return ts;
}


/************************************************************/
int main(int argc, char *argv[])
{

#ifdef GLB_STATIC
  LTDL_SET_PRELOADED_SYMBOLS();
#endif /* GLB_STATIC */


  double progress;
  int count=0;
  int s,i,rv;
  FILE *output=NULL,*inputfile=NULL;
  
  //double osc[]={0.553574,0.160875,M_PI/4,0.0,0.00007,0.003};
  double osc[]={0.553574,0.0160875,M_PI/4,M_PI/4,0.000082,0.0025};
  char* central_values=NULL;
  struct arguments arguments;
  /* defaults */
  arguments.args[0]="-";
  int resolution=0;
  arguments.output_file=NULL;
  arguments.exp=0;
  arguments.params=NULL;
   arguments.oct=NO;
  arguments.hier=NO;
  arguments.cor=NO;
  
  arguments.solar=0.1;
  arguments.atm=0.05;
  arguments.density=0.05;
  
  arguments.silent=NO;
  
  /* parsing the comand line */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);  

  /* Lexing the oscillation parameters as given by the environment variable
   * GLB_CENTRAL_VALUES
   */
  central_values=getenv("GLB_CENTRAL_VALUES");
  if(central_values!=NULL)
    {
      rv=sscanf(central_values,"%lf , %lf , %lf , %lf , %lf , %lf",&osc[0],
	     &osc[1],&osc[2],&osc[3],&osc[4],&osc[5]);
      if(rv!=GLB_OSCP) 
	{
	  fprintf(stderr,"%s: FATAL: Wrong format for oscillation"
		  " parameters from environment variable GLB_CENTRAL_VALUES\n"
		  ,argv[0]);
	  exit(1);
	}
    }



  
    inputfile=fopen(arguments.args[0],"r");
    if(inputfile==NULL) {
      fprintf(stderr,"FATAL: %s: Could not open output file '%s'\n",argv[0],
	      arguments.args[0]);
      exit(1);
    }
 


  /* Lexing the oscillation parameters as given by -p='1,2,3...' */
  if(arguments.params!=NULL)
    {
      rv=sscanf(arguments.params,"%lf , %lf , %lf , %lf , %lf , %lf",&osc[0],
	     &osc[1],&osc[2],&osc[3],&osc[4],&osc[5]);
      if(rv!=GLB_OSCP) 
	{
	  fprintf(stderr,"%s: FATAL: Wrong format for oscillation"
		  " parameters\n ",argv[0]);
	  exit(1);
	}
    }
 

  if(arguments.output_file==NULL)  output=stdout;
  else {
    output=fopen(arguments.output_file,"w");
    if(output==NULL) {
      fprintf(stderr,"FATAL: %s: Could not open output file '%s'\n",argv[0],
	      arguments.output_file);
      exit(1);
    }
  }



  print_header(output,"globes-deltatheta",VERSION,argc,argv);

  /* Initialize libglobes */
  glbInit(argv[0]);

  for(i=1;i<arguments.exp;i++)
    {
      s=glbInitExperiment(arguments.args[i],&glb_experiment_list[0],&glb_num_of_exps);

      /* Testing for failure */
      if(s<-1) {fprintf(stderr,"%s: FATAL: Unrecoverable parse error\n",
		    argv[0]);exit(1);}
  
    }
  
  /* Initialize parameter vector(s) */
  glb_params true_values = glbAllocParams();
  glb_params test_values = glbAllocParams();
  glb_params starting_values = glbAllocParams();
  glb_params input_errors = glbAllocParams();
  glb_params minimum = glbAllocParams();


  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbCopyParams(true_values,test_values);  
  
  if(arguments.hier==YES) glbSetOscParams(test_values,-osc[5],GLB_DM_ATM);
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

  
  double x,y,thetheta13,res,**list=NULL;
  resolution=get_values(inputfile,output,&list);
  /*  for(i=0;i<resolution;i++)
      {
      printf("%f %f\n",list[i][0],list[i][1]);
      }
      printf("resolution %d\n",resolution);
      exit(1);
  */
  int block=0;
  for(i=0;i<resolution;i++)
      {
	x=list[i][1];
	y=list[i][0];
	if(x==1&&y==1) {
	  /* write a marker to the file */
	  block++;
	  fprintf(output,"# -- sub-region %d --\n",block);
	  continue;}
       
	/* Set vector of test values */
	thetheta13=asin(sqrt(pow(10,x)))/2;
	glbSetOscParams(test_values,thetheta13,GLB_THETA_13);
	glbSetOscParams(test_values,y*M_PI/180.0,GLB_DELTA_CP);
	count++;
	progress=count/(resolution*1.0)*100.0;
	if(arguments.silent==NO) fprintf(stderr,"progress %3.1f%%\r",progress);
	if(arguments.cor==NO)
	  {  
	    /* Compute Chi^2 for all loaded experiments and all rules */
	    res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
	    fprintf(output,"%f %f %f\n",x,y,res);
	    
	  }
	else
	  {    
	    /* Compute Chi^2 for all loaded experiments and all rules */
	    res=glbChiThetaDelta(test_values,minimum,GLB_ALL);
	    fprintf(output,"%f %f %f\n",x,y,res);
	    fflush(output);
	  }
      }
  if(arguments.silent==NO) fprintf(stderr,"\n");  


  /* Destroy parameter vector(s) */
  glbFreeParams(true_values);
  glbFreeParams(test_values); 
  glbFreeParams(starting_values);
  glbFreeParams(input_errors); 
  glbFreeParams(minimum); 
  fclose(output);
  exit(0);
}
