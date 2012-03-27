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
#include <float.h>


#include "globes/globes.h"   /* GLoBES library */

#include "glb_tools_eightfold.h"
#include "glb_tools_header.h"
#include "glb_tools_parser.h"

#define FAILURE -1
#define SUCCESS 0

#define THETA 1
#define DELTA 2
#define NEITHER 0
#define THETA23 3
#define ALL 4

const char *argp_program_version =
"globes-discovery "VERSION"\n(C) 2005, 2006, 2007, 2008, 2009 Patrick Huber\n"
"This is NOT free software see the source for copying conditions. There is NO\n"
"warranty; not even for MERCHANTABILITY or"
" FITNESS FOR A PARTICULAR PURPOSE.";
const char *argp_program_bug_address =
PACKAGE_BUGREPORT;

/* Program documentation. */
static char doc[] ="Producing discovery plots (data only)";

/* A description of the arguments we accept. */
static char args_doc[] = "glb-file(s) output-file";

/* The options we understand. */
static struct argp_option options[] ={
  {"Define",'D',"DEFINITION",0,"Define AEDL variable",0},
  {"resolution",'r',  "RANGE", 0,  
   "Number of points to be used in each direction for locating the inital guess, Format 'a,b'",4},
  {"Resolution",'R',  "RANGE", 0,  
   "Number of points to be used in each direction, Format 'a,b'",3},
  {"parameters",'p',"PARAMETERS",0,
   "Set of oscillation parameters\n   for which the data are computed\n"
   "   Format 'th12,th13,th23,delta,dm21,dm31'",0},
  {"xrange",'x',"RANGE",0,"Range in log(sin^2 2theta13) for locating the inital guess, Format 'a,b'",4},
  {"yrange",'y',"RANGE",0,"Range in delta in degrees for locating the inital guess, Format 'a,b'",4},
  {"Xrange",'X',"RANGE",0,"Range in log(sin^2 2theta13), Format 'a,b'",3},
  {"Yrange",'Y',"RANGE",0,"Range in delta in degrees, Format 'a,b'",3},
  {"Oktant", 'O', 0, 0,"Compute chi^2 with th23->pi/2-th23",1},
  {"Hierarchy",'H', 0, 0,"Compute chi^2 with dm31->-dm31",1}, 
  {"atmospheric",'a',"NUMBER",0,"Relative error on theta23 and dm31 each,\ndefault is 0.05",2},
  {"solar",'s',"NUMBER",0,"Relative error on dm21 each,\ndefault is 0.1",2},
  {"Theta12",'T',"NUMBER",0,"Relative error on theta12 and dm21 each,\ndefault is 0.1",2},
  {"Theta13",'c',"NUMBER",0,
   "prior on sin^22theta13, absolut",2},
  {"density",'d',"NUMBER",0,"Relative error on the matter density,\ndefault is 0.05",2},
  {"quiet",'q',0,0,"silent operation"},
  {"Plane",'P',"PAIR",0,"Chose the plane of true values"},
  {"Maxtheta",'M',0,0,"Compute chi^2 at theta23=pi/4",5},
  {"CPV",'C',0,0,"Compute chi^2 at delta=pi,0",5},
  {"Sin22th13",'S',0,0,"Compute chi^2 at th13=0",5},
  {"All",'A',0,0,"same as -NMCS, default",5},
  {"None",'N',0,0,"do none of the above, i.e locate only the degenerate solutions",5},
  {"Fast",'F',0,0,"Fast minimizer (experimental feature)",6},
  {"Luminosity",'L',"NUMBER",0,
   "Rescale total luminosity by NUMBER (default=1)",6},
  {"test",'t',0,0,"test, will produce output but very fast for tests only"},
  
  { 0 } 
};

/* Used by `main' to communicate with `parse_opt'. */
struct arguments
{
  int exp,oct,hier,silent,force,test;    
  int cpv,sth13,mth23,fast;
  char* args[32];                /* up to 32 experiments */
  char *resolution,*Resolution,*Plane;
  char *params,*xrange,*yrange,*Xrange,*Yrange;
  double solar,solarangle,atm,density,theta13error;
  double lumi;
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
    
    case 'N':
      arguments->sth13 = NO;
      arguments->cpv = NO;
      arguments->mth23 = NO;
      break; 

    case 'F':
      arguments->fast = YES;
      break; 


    case 't':
      arguments->test = YES;
      break; 

    case 'S':
      arguments->sth13 = YES;
      break; 
    case 'C':
      arguments->cpv = YES;
      break; 
    case 'M':
      arguments->mth23 = YES;
      break; 

    case 'A':
      arguments->sth13 = YES;
      arguments->cpv = YES;
      arguments->mth23 = YES;
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

    case 'c':
      arguments->theta13error = atof(arg);
      break; 
      
    case 'L':
      arguments->lumi = atof(arg);
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
    case 'P':
      arguments->Plane = arg;
      break;
     case 'x':
      arguments->xrange = arg;
      break;
    case 'y':
      arguments->yrange = arg;
      break;
    case 'Y':
      arguments->Yrange = arg;
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
    case 'T':
      arguments->solarangle = atof(arg);
      break;
      
    case ARGP_KEY_ARG:
      if (state->arg_num > 33)
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


/* This function is need to ensure the proper result if either the
   upper and lower range are the same or the number of steps is 1.   
*/
double safe_range(double range[],int steps)
{
  double erg;
  /* trying to catch the case where upper and lower range are the same
     requires 4*DBL_EPSILON to make sure that when this value is
     divided by 2 is still is larger than DBL_EPSILON, and we take
     twice that, just to be sure.
  */
  if(steps==0||range[0]>=range[1]) erg=range[1]-range[0]+16*DBL_EPSILON;
  else erg=(range[1]-range[0])/steps;
  return erg;
}


int rescale_lumi(int exp, double factor)
{
  int s;
  double l0;
  l0=glbGetTargetMass(exp);
  s=glbSetTargetMass(exp,l0*factor);
  return s;
}


/****************************************************************************/
int main(int argc, char *argv[])
{



  projection Xaxis,Yaxis;
  int s,i,resume=0;
  FILE *output=NULL;
  char *base;
  char oct='+';
  char hie='+';
  double osc[]={0.553574,0.0160875,M_PI/4,M_PI/4,0.000082,0.0025};
  double xrange[]={-4,0};
  double yrange[]={0,360};
  double Yrange[]={0,360};
  double Xrange[]={-3,-1};
  double posx,posy;
  int resolution[]={25,25};
  int Resolution[]={2,2};
  int Plane[]={GLB_THETA_13,GLB_DELTA_CP};
 
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
  arguments.Yrange=NULL;
  arguments.Plane=NULL;
  arguments.force=FORCE;
  arguments.oct=NO;
  arguments.hier=NO;
  arguments.solar=0.1;
  arguments.solarangle=0.1;
  arguments.atm=0.05;
  arguments.density=0.05;
  arguments.silent=NO;
  arguments.sth13=YES; 
  arguments.cpv=YES;
  arguments.mth23=YES;
  arguments.fast=NO;
  arguments.lumi=1.0;
  arguments.theta13error=0;

  /* parsing the comand line */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);  

  /* Lexing the oscillation parameters as given by the environment
   * variable GLB_CENTRAL_VALUES
   */
  central_values=getenv("GLB_CENTRAL_VALUES");
 
  vector_read_double(central_values,osc,6,parse_error);
  vector_read_double(arguments.params,osc,6,parse_error);
  vector_read_double(arguments.Xrange,Xrange,2,parse_error);
  vector_read_double(arguments.Yrange,Yrange,2,parse_error);
  vector_read_double(arguments.xrange,xrange,2,parse_error);
  vector_read_double(arguments.yrange,yrange,2,parse_error);
 
  vector_read_int(arguments.resolution,resolution,2,parse_error);
  vector_read_int(arguments.Resolution,Resolution,2,parse_error);
  vector_read_int(arguments.Plane,Plane,2,parse_error);

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

  for(i=0;i<arguments.exp-1;i++) {rescale_lumi(i,arguments.lumi);}

  base=get_output_filename(arguments.args[arguments.exp-1]);

  resume=read_position(base, &posx, &posy,arguments.force);

  /* all done */
  if(posx+10*FLT_EPSILON>=Xrange[1]&&posy+10*FLT_EPSILON>=Yrange[1]&&resume==1) exit(0);

  /* The output files are:
     output0 - contains the main result
     degput  - contains the position of the degenerate solutions found
     (.deg)
     output1 - contains the the two cases for delta=0,\pi (.deg)
     output2 - contains the position of all minima found at th13=0 (.the)
     output3 - contains the position of all minima found at th23=0 (.tha)
  */

  output=goto_end(base,arguments.force);
  if(!output) {
    fprintf(stderr,"FATAL: %s: Could not open output file '%s'\n",argv[0],
	    base);
    exit(1);
    }

  if(resume==1)
    {
      fprintf(output,"\n# resumed at %s\n","not working yet");
    }
  else
    {
      print_header(output,"globes-discovery",VERSION,argc,argv);
    }
  base[strlen(base)-4]='\0';
  // base=strcat(base,".deg");
  
  /* Initialize parameter vector(s) */
  glb_params true_values = glbAllocParams();
  glb_params input_errors = glbAllocParams();
  glb_params test_values = glbAllocParams();
  glb_params starting_values = glbAllocParams();
  glb_params minimum = glbAllocParams();
  glb_params buffer = glbAllocParams();
   
 
  
  double X,Y;
  double thetheta13,delta;    
  glb_params *listb;
  double *fchi;
  int ct;

  setup_axis(&Xaxis,Plane[0]);
  setup_axis(&Yaxis,Plane[1]);

  Resolution[0]=Resolution[0]-1;
  Resolution[1]=Resolution[1]-1;

  /* just not to leave them undefined */
  if(resume!=1)
    {
      posx=Xrange[0];
      posy=Yrange[0];  
    }
#ifdef DEBUG
  fprintf(stderr,"X-start %f %f difference %g safe %g\n",Yrange[0],Yrange[1],Yrange[1]-Yrange[0],safe_range(Yrange,Resolution[1]));
  int count=0;
#endif
  
  int xbreak=NO;
  int ybreak=NO;

  int d1,d2;

  for(X=Xrange[0];X<=Xrange[1]+safe_range(Xrange,Resolution[0])/2&&xbreak==NO;
      X=X+safe_range(Xrange,Resolution[0]))
    {
      ybreak=NO;
      for(Y=Yrange[0];Y<=Yrange[1]+safe_range(Yrange,Resolution[1])/2&&ybreak==NO;
	  Y=Y+safe_range(Yrange,Resolution[1]))
	{

	  fprintf(output,"C   %6.6e\t%6.6e\n",X,Y);
	  for(d1=NO;d1<NO+2;d1++)
	    {
	      for(d2=NO;d2<NO+2;d2++)
		{
		  arguments.hier=d1;
		  arguments.oct=d2;
		  if(fabs(osc[2]-M_PI/4.0)<1E-3) d2++;
		  if(arguments.hier==NO )hie='+';
		  if(arguments.hier==YES )hie='-';
		  if(arguments.oct==NO )oct='+';
		  if(arguments.oct==YES )oct='-';
#ifdef DEBUG
	fprintf(stderr,"loop %f %f\n",X,Y);
	count++;
	if(count>50) exit(1);
#else /* !DEBUG */
	thetheta13=(Xaxis.f)(X);
	delta=(Yaxis.f)(Y);
	glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
	glbSetOscParams(true_values,thetheta13,Xaxis.which);
	glbSetOscParams(true_values,delta,Yaxis.which);
	
	glbDefineParams(input_errors,osc[0]*arguments.solarangle,arguments.theta13error,osc[2]*arguments.atm,0,
			osc[4]*arguments.solar,osc[5]*arguments.atm);
	glbSetDensityParams(input_errors,arguments.density,GLB_ALL);

	/* this allows us to produce output, test the parsing etc,
	   without needing much time at all.
	*/
	if(arguments.test==YES)
	  {
	    glbSetOscillationParameters(true_values);
	    glbSetRates();

	    fprintf(output,"T  %6.6e\n",
		    glbChiSys(true_values,GLB_ALL,GLB_ALL));
	    continue;
	  }
	
	ct=eightfold(true_values,input_errors,arguments.hier,arguments.oct,xrange,yrange,resolution,&listb,&fchi);
	double rdeg=DBL_MAX;
	for(i=0;i<ct;i++){
	 
	  fprintf(output,"G%c%c %6.6e\t",hie,oct,fchi[i]);
	  my_print_params(output,listb[i]);
	  fprintf(output,"\n");
	  if(fchi[i]<=rdeg) rdeg=fchi[i];
	  
	}
	fflush(output);
	glbCopyParams(true_values,test_values);
	
	if(arguments.hier==YES)
	  {
	    double nv;
	    /* following hep-ph/0509359 eq. 3 */
	    nv=-osc[5]+2*osc[4]*cos(osc[0])*cos(osc[0]);
	    glbSetOscParams(test_values,nv,GLB_DM_ATM);
	    
	  }
	if(arguments.oct==YES) {
	  glbSetOscParams(test_values,M_PI/2-osc[2],GLB_THETA_23);
	}
	glbCopyParams(test_values,starting_values);
	glbDefineParams(input_errors,osc[0]*arguments.solarangle,
			arguments.theta13error,osc[2]*arguments.atm,0,
			osc[4]*arguments.solar,osc[5]*arguments.atm);
	glbSetDensityParams(input_errors,arguments.density,GLB_ALL);
	glbSetInputErrors(input_errors);
	glbSetStartingValues(starting_values);
	
	/* The simulated data are computed */
	glbSetOscillationParameters(true_values);
	glbSetRates();



 
	double res=DBL_MAX,rest,resa,resb,rd,rt,rat,resat;
	
	rat=DBL_MAX;
	rd=DBL_MAX;
	rt=DBL_MAX;

	   
	    if(arguments.sth13==YES)
	      {
		for(i=0;i<ct;i++)
		  {
		    glbCopyParams(listb[i],buffer);
		
		    glbSetOscParams(buffer,0,GLB_THETA_13);
		    
		    rest=glbChiTheta(buffer,minimum,GLB_ALL);
		    fprintf(output,"T%c%c %6.6e\t",hie,oct,rest);
		    my_print_params(output,minimum);
		    fprintf(output,"\n");
		    if(rest<=rt) rt=rest;
		    fflush(output);
		  }
	      }
	    
	    if(arguments.cpv==YES)
	      {
		for(i=0;i<ct;i++)
		  {
		     glbCopyParams(listb[i],buffer);
		
		    
		    double resc=0;
		    double resd=0;
		    /* checking the case th13=th13^0 */
		    glbCopyParams(true_values,buffer);
		    glbSetOscParams(buffer,0,GLB_DELTA_CP);
		    resc=glbChiDelta(buffer,minimum,GLB_ALL); 
		    fprintf(output,"D%c%c %6.6e\t",hie,oct,resc);
		    my_print_params(output,minimum);
		    fprintf(output,"\n");
		    
		    glbCopyParams(true_values,buffer);
		    glbSetOscParams(buffer,M_PI,GLB_DELTA_CP);
		    resd=glbChiDelta(buffer,minimum,GLB_ALL); 
		    fprintf(output,"D%c%c %6.6e\t",hie,oct,resd);
		    my_print_params(output,minimum);
		    fprintf(output,"\n");

		    glbCopyParams(listb[i],buffer);
		    /*testing the other cases found previously as global
		      minima */ 	
		    glbSetOscParams(buffer,0,GLB_DELTA_CP);
		    resa=glbChiDelta(buffer,minimum,GLB_ALL); 
		    fprintf(output,"D%c%c %6.6e\t",hie,oct,resa);
		    my_print_params(output,minimum);
		    fprintf(output,"\n");
		    glbSetOscParams(buffer,M_PI,GLB_DELTA_CP);
		    resb=glbChiDelta(buffer,minimum,GLB_ALL);
		    fprintf(output,"D%c%c %6.6e\t",hie,oct,resb);
		    my_print_params(output,minimum);
		    fprintf(output,"\n");
		    if(resa<resb) res=resa;
		    else res =resb;
		    if(res<=rd) rd=res;
		    if(resc<=rd) rd=resc;	
		    if(resd<=rd) rd=resd;
		    fflush(output);
		  }
	      }

	    if(arguments.mth23==YES)
	      {
		for(i=0;i<ct;i++)
		  {
		    
		    glbCopyParams(listb[i],buffer);
		    glbSetOscParams(buffer,M_PI/4,GLB_THETA_23);
		    resat=glbChiTheta23(buffer,minimum,GLB_ALL);
		    fprintf(output,"A%c%c %6.6e\t",hie,oct,resat);
		    my_print_params(output,minimum);
		    fprintf(output,"\n");

		    if(resat<=rat) rat=resat;
		    fflush(output);
		  }
	      }
	    
	  
	free_list(listb,ct);
	free(fchi);
	if(Yrange[0]==Yrange[1]) {ybreak=YES;}
#endif /* !DEBUG */
      }
      if(Xrange[0]==Xrange[1]) {xbreak=YES;}
    }
	}
    }
  /* Clean up*/
  
  print_footer(output);
  fclose(output);


  glbFreeParams(test_values);
  glbFreeParams(starting_values);
  glbFreeParams(true_values);
  glbFreeParams(input_errors);
  glbFreeParams(minimum);
  glbFreeParams(buffer);
  free(base);
  myprior_clean();
  exit(0);
}
