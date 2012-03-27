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
#include <time.h>




#include "globes/globes.h"   /* GLoBES library */

#include "glb_tools_eightfold.h"
#include "glb_tools_header.h"
#include "glb_tools_parser.h"


#define FAILURE -1
#define SUCCESS 0


const char *argp_program_version =
"globes-accuracy "VERSION"\n(C) 2005, 2006, 2007, 2008, 2009 Patrick Huber\n"
"This is NOT free software see the source for copying conditions. There is NO\n"
"warranty; not even for MERCHANTABILITY or"
" FITNESS FOR A PARTICULAR PURPOSE.";
const char *argp_program_bug_address =
PACKAGE_BUGREPORT;

/* Program documentation. */
static char doc[] ="Simple degeneracy finding with GLoBES";

/* A description of the arguments we accept. */
static char args_doc[] = "glb-file(s) output-file";

/* The options we understand. */
static struct argp_option options[] ={
  {"Define",'D',"DEFINITION",0,"Define AEDL variable",0},
  {"resolution",'r',  "RANGE", 0,  
   "Number of points to be used in each direction for locating the inital guess, Format 'a,b'",4},
  {"Resolution",'R',  "INTEGER", 0,  
  "Number of points to be used in in Xrange",3},
  {"parameters",'p',"PARAMETERS",0,
   "Set of oscillation parameters\n   for which the data are computed\n"
   "   Format 'th12,th13,th23,delta,dm21,dm31'",0},
  {"xrange",'x',"RANGE",0,"Range in log(sin^2 2theta13) for locating the inital guess, Format 'a,b'",4},
  {"yrange",'y',"RANGE",0,"Range in delta in degrees for locating the inital guess, Format 'a,b'",4},
  {"Xrange",'X',"RANGE",0,"Range in log(sin^2 2theta13), Format 'a,b'",3}, 
  {"Srange",'S',"RANGE",0,"Symmetric range central value +-RANGE, Format 'a'",3},
  {"Oktant", 'O', 0, 0,"Compute chi^2 with th23->pi/2-th23",1},
  {"Hierarchy",'H', 0, 0,"Compute chi^2 with dm31->-dm31",1}, 
  {"atmospheric",'a',"NUMBER",0,"Relative error on theta23 and dm31 each,\ndefault is 0.05",2},
  {"solar",'s',"NUMBER",0,"Relative error on theta12 and dm21 each,\ndefault is 0.1",2},
  {"density",'d',"NUMBER",0,"Relative error on the matter density,\ndefault is 0.05",2},
 {"Theta13",'c',"NUMBER",0,
   "prior on sin^22theta13, absolut",2},
  {"quiet",'q',0,0,"silent operation"},
  {"Axis",'A',"INTEGER",0,"direction on which is projected"},
  {"Fast",'F',0,0,"Fast minimizer (experimental feature)",6},
  { 0 } 
};

/* Used by `main' to communicate with `parse_opt'. */
struct arguments
{
  int exp,oct,hier,silent,which,fast;           /* number of experiments */
  char* args[32];                /* up to 32 experiments */
  char *resolution,*Resolution;
  char *params,*xrange,*yrange,*Xrange;
  double solar,atm,density,theta13error,srange;
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

    case 'c':
      arguments->theta13error = atof(arg);
      break;
   

    case 'F':
      arguments->fast = YES;
      break; 
      
    case 'D':
      parse_definition(arg);
      break;
    case 'q':
      arguments->silent = YES;
      break; 
     case 'r':
      arguments->resolution = arg;
      break; 
    case 'R':
      arguments->Resolution = arg;
      break; 
    case 'a':
      arguments->atm = atof(arg);
      break; 
    case 'S':
      arguments->srange = atof(arg);
      break; 
    case 'A':
      arguments->which = atoi(arg);
      break; 
    case 's':
      arguments->solar = atof(arg);
      break; 
    case 'd':
      arguments->density = atof(arg);
      break; 
    case 'p':
      arguments->params = arg;
      break;
     case 'x':
      arguments->xrange = arg;
      break;
    case 'y':
      arguments->yrange = arg;
      break;

    case 'X':
      arguments->Xrange = arg;
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
      if (state->arg_num < 2)
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




/****************************************************************************/
int main(int argc, char *argv[])
{



  projection axis;
  int s,i;
  FILE *message=stdout;
  FILE *output=NULL;
  double osc[]={0.553574,0.0160875,M_PI/4,M_PI/4,0.000082,0.0025};
  double xrange[]={-4,0};
  double yrange[]={0,360};
  double Xrange[]={-3,-1};

  int resolution[]={25,25};
  int Resolution[]={2,2};
 
  char* central_values=NULL;
  struct arguments arguments;
  /* defaults */
  arguments.args[0]="-";
  arguments.resolution=NULL;
  arguments.Resolution=NULL;

  arguments.exp=0;
  arguments.params=NULL;
  arguments.xrange=NULL;
  arguments.yrange=NULL;
  arguments.Xrange=NULL;
  arguments.oct=NO;
  arguments.hier=NO;
  arguments.solar=0.1;
  arguments.atm=0.05;
  arguments.density=0.05;
  arguments.srange=-1;
  arguments.silent=NO;
  arguments.which=GLB_THETA_13;
  /* parsing the comand line */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);  

  /* Lexing the oscillation parameters as given by the environment
   * variable GLB_CENTRAL_VALUES
   */
  central_values=getenv("GLB_CENTRAL_VALUES");
 
  vector_read_double(central_values,osc,6,parse_error);
  vector_read_double(arguments.params,osc,6,parse_error);
  if(arguments.srange<0) {vector_read_double(arguments.Xrange,Xrange,2,parse_error);}

  vector_read_double(arguments.xrange,xrange,2,parse_error);
  vector_read_double(arguments.yrange,yrange,2,parse_error);
 
  vector_read_int(arguments.resolution,resolution,2,parse_error);
  vector_read_int(arguments.Resolution,Resolution,1,parse_error);
 


  char *base=get_output_filename(arguments.args[arguments.exp-1]);
  

  output=fopen(base,"w");
  if(output==NULL) {
    fprintf(stderr,"FATAL: %s: Could not open output file '%s'\n",argv[0],
	    base);
    exit(1);
  }
 

  /* Try to provide a useful file header */

  print_header(output,"globes-accuracy",VERSION,argc,argv);

  /* ------------------------------------ */

  if(arguments.silent==YES) 
    {
      message=fopen("/dev/null","w");
      if(message==NULL) {
	fprintf(stderr,"ERROR: %s: Could not open '/dev/null'\n",argv[0]);
	      fprintf(stderr,"ERROR: %s: Will output messages to 'stdout' instead\n",argv[0]);
	      message=stdout;
      }
    }
  
  /* Initialize libglobes */
  glbInit(argv[0]);

  if(arguments.fast==YES) glbSelectMinimizer(GLB_MIN_POWELL);

  /* Initialize my prior */
  myprior_init();
  glbRegisterPriorFunction(myprior_prior,myprior_starting_values,myprior_input_errors,NULL);
  
  for(i=0;i<arguments.exp-1;i++)
    {
      
      s=glbInitExperiment(arguments.args[i],&glb_experiment_list[0],&glb_num_of_exps);

      /* Testing for failure */
      if(s<-1) {fprintf(stderr,"%s: FATAL: Unrecoverable parse error\n",
		    argv[0]);exit(1);}
  
    }
  
 
  setup_axis(&axis,arguments.which);

  /* Initialize parameter vector(s) */
  glb_params true_values = glbAllocParams();
  glb_params input_errors = glbAllocParams();
  glb_params test_values = glbAllocParams();
  glb_params starting_values = glbAllocParams();
  glb_params minimum = glbAllocParams();
  glb_params buffer = glbAllocParams();
   
 
  
  double X,Y;
  double thetheta13;    
  glb_params *listb;
  double *fchi;
  int ct;
  Resolution[0]=Resolution[0]-1;
  Resolution[1]=Resolution[1]-1;
 
  X=log10(pow(2,sin(2*osc[1])));
  thetheta13=asin(sqrt(pow(10,X)))/2;
  
  
  thetheta13=osc[1];
  Y=osc[3]/M_PI*180;
#ifdef DEBUG

  fprintf(stderr,"X:%lf Y:%lf\n",X,Y);

#endif /* !DEBUG */

  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetOscParams(true_values,thetheta13,GLB_THETA_13);
  glbSetOscParams(true_values,Y*M_PI/180.0,GLB_DELTA_CP);
  
  glbDefineParams(input_errors,osc[0]*arguments.solar,arguments.theta13error,osc[2]*arguments.atm,0,
		  osc[4]*arguments.solar,osc[5]*arguments.atm);
  glbSetDensityParams(input_errors,arguments.density,GLB_ALL);

  ct=eightfold(true_values,input_errors,arguments.hier,arguments.oct,xrange,yrange,resolution,&listb,&fchi);
  
  for(i=0;i<ct;i++){
    fprintf(message,"----------- This is final solution #%d -----------\n",i+1);
    fprintf(message,"chi^2 %f\n",fchi[i]);
    glbPrintParams(message,listb[i]);
  }
  
  glbCopyParams(true_values,test_values);
  
  if(arguments.hier==YES) glbSetOscParams(test_values,-osc[5],GLB_DM_ATM);
  if(arguments.oct==YES) glbSetOscParams(test_values,M_PI/2-osc[2],GLB_THETA_23);

  glbCopyParams(test_values,starting_values);
  glbDefineParams(input_errors,osc[0]*arguments.solar,arguments.theta13error,osc[2]*arguments.atm,0,
                  osc[4]*arguments.solar,osc[5]*arguments.atm);
  glbSetDensityParams(input_errors,arguments.density,GLB_ALL);
  glbSetInputErrors(input_errors);
  glbSetStartingValues(starting_values);

  /* The simulated data are computed */
  glbSetOscillationParameters(true_values);
  glbSetRates();



 
  double split,xsi;
  
  result bfp,back,safe;
  
  if(arguments.srange>0){
  double centerpoint=axis.inverse_f(osc[axis.which]);
  Xrange[0]=centerpoint-arguments.srange;
  Xrange[1]=centerpoint+arguments.srange;

  }

  for(i=0;i<ct;i++)
    {
      bfp=alloc_result();
      fprintf(output,"------ solution #%d out of %d ------\n",i+1,ct);     
      fflush(output);
      xsi=glbGetOscParams(listb[i],axis.which);
      split=axis.inverse_f(xsi);
      if(axis.which==GLB_THETA_13)
	{
	  if(xsi<1E-8) split=-250; 
	}
      
    
      result test=NULL,nn,tmp;
      test=setup_chain(Xrange,Resolution[0]);
      
      nn=find_nearest_neighbor(test,split);
      safe=nn;

      track_in_chain(nn,listb[i],starting_values,FORWARD,&axis,message);
      track_in_chain(get_backward(nn),listb[i],starting_values,BACKWARD,&axis,message);

      backpropagate_in_chain(nn,starting_values,FORWARD,&axis,message);
      backpropagate_in_chain(nn,starting_values,BACKWARD,&axis,message);
     
      bfp=set_xval(bfp,split);
      bfp=set_minimum(bfp,listb[i]);
      bfp=set_chi(bfp,fchi[i]);
      bfp=set_diagnosis(bfp,0);
      
      if(get_xval(bfp)<get_xval(safe))
	{
	  
	  back=get_backward(safe);
	  
	  safe=set_backward(safe,bfp);
	  bfp=set_forward(bfp,safe);

	  back=set_forward(back,bfp);
	  bfp=set_backward(bfp,back);
	}
      else
	{
	  back=get_forward(safe);

	  safe=set_forward(safe,bfp);
	  bfp=set_backward(bfp,safe);

	  back=set_backward(back,bfp);
	  bfp=set_forward(bfp,back);
	}

  
      tmp=rewind_chain(nn);
      int c=0;
      while(tmp){
	c++;
	print_result(output,tmp);
	tmp=get_forward(tmp);
	if(c>1E3) exit(1);
      }
      fflush(output);
      free_chain(test);
    
    }     
 
  
  /* Clean up */
  free_list(listb,ct);
  free(fchi);

  print_footer(output);

  fclose(output);
  glbFreeParams(test_values);
  glbFreeParams(starting_values);
  glbFreeParams(true_values);
  glbFreeParams(input_errors);
  glbFreeParams(minimum);
  glbFreeParams(buffer);
  free_axis(&axis);
  myprior_clean();
  free(base);
  exit(0);
}
