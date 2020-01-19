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

#include "../customchis/customChis.h"

#define FAILURE -1
#define SUCCESS 0

#define THETA 1
#define DELTA 2
#define NEITHER 0
#define THETA23 3
#define ALL 4

const char *argp_program_version = "";
static char doc[] ="";
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
  {"parameterse",'E',"PARAMETERS",0,
   "Set of relative oscillation parameter errors\n"
   "   Format 'th12e,th13e,th23e,deltae,dm21e,dm31e'",0},
  {"xrange",'x',"RANGE",0,"Range in log(sin^2 2theta13) for locating the inital guess, Format 'a,b'",4},
  {"yrange",'y',"RANGE",0,"Range in delta in degrees for locating the inital guess, Format 'a,b'",4},
  {"Xrange",'X',"RANGE",0,"Range in log(sin^2 2theta13), Format 'a,b'",3},
  {"Yrange",'Y',"RANGE",0,"Range in delta in degrees, Format 'a,b'",3},
  {"Oktant", 'O', 0, 0,"Compute chi^2 with th23->pi/2-th23",1},
  {"Hierarchy",'H', 0, 0,"Compute chi^2 with dm31->-dm31",1}, 
  {"density",'d',"NUMBER",0,"Relative error on the matter density,\ndefault is 0.05",2},
  {"quiet",'q',0,0,"silent operation"},
  {"Plane",'P',"PAIR",0,"Chose the plane of true values"},
  {"Maxtheta",'M',0,0,"Compute chi^2 at theta23=pi/4",5},
  {"CPV",'C',0,0,"Compute chi^2 at delta=pi,0",5},
  {"Sin22th13",'S',0,0,"Compute chi^2 at th13=0",5},
  {"All",'A',0,0,"same as -NMCS, default",5},
  {"None",'N',0,0,"do none of the above, i.e locate only the degenerate solutions",5},
  {"Fast",'F',0,0,"Fast minimizer (experimental feature)",6},
  {"test",'t',0,0,"test, will produce output using only glbChiSys"},
  {"chi",'I',"NUMBER",0,"dim of systematics function to use, otherwise defaults are used"},
  {"EnergySpectrum",'e',0,0,"Enable output of various event spectra. Only does one point so choose starting values carefully."},
  {"Jellybeans",'j',0,0,"Enable confididence regions fit for osc[1], dcp=-90,0,90."},
  { 0 } 
};

/* Used by `main' to communicate with `parse_opt'. */
struct arguments
{
  int exp,oct,hier,silent,force,test;    
  int cpv,sth13,mth23,fast;
  char* args[32];                /* up to 32 experiments */
  char *resolution,*Resolution,*Plane;
  char *params,*xrange,*yrange,*Xrange,*Yrange,*paramse;
  double density;
  int chimode, enablespectra, jellybeans;
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
    case 'd':
      arguments->density = atof(arg);
      break;
    case 'p':
      arguments->params = arg;
      break;
    case 'E':
      arguments->paramse = arg;
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
    case 'e':
      arguments->enablespectra = YES;
      break;
    case 'j':
      arguments->jellybeans = YES;
      break;
	case 'I':
	  arguments->chimode = atof(arg);
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
struct arguments arguments;

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


void my_print_params2(FILE *fp,const glb_params iv)
{
  int i;
  for(i=0;i<GLB_OSCP;i++) {
    if(i==GLB_DELTA_CP) fprintf(fp,"%6.6e\t",fmod(glbGetOscParams(iv,i),2*M_PI));
    else fprintf(fp,"%6.6e\t",glbGetOscParams(iv,i));
      }
  for(i=0;i<glb_num_of_exps;i++) fprintf(fp,"%6.6f\t",glbGetDensityParams(iv,i));
  //add on contents of xmin
  int sys_dim = glbGetSysDimInExperiment(0, 0, GLB_ON);
	int sysi=0;
	for(sysi=0;sysi<sys_dim;sysi++) fprintf(fp,"%6.6e\t",xmin[0][sysi]);
  for(sysi=0;sysi<sys_dim;sysi++) fprintf(fp,"%6.6e\t",xmin[1][sysi]);
}

void save_spectra(char * type, int hier, int number){
  //output (to file) simulated and fit spectra for first experiment
  //also output systematics adjusted signals and backgrounds
  FILE *fout[2];
  double *signal_fit_rates[2];
  signal_fit_rates[0] = glbGetSignalFitRatePtr(0, 0);
  signal_fit_rates[1] = glbGetSignalFitRatePtr(1, 0);
  double *bg_fit_rates[2];
  bg_fit_rates[0] = glbGetBGFitRatePtr(0, 0);
  bg_fit_rates[1] = glbGetBGFitRatePtr(1, 0);
  double *bin_centers[2];
  bin_centers[0] = glbGetBinCentersListPtr(0);
  bin_centers[1] = glbGetBinCentersListPtr(1);
  char filename[32];
  int i,ebin,exp;
  double emin[2], emax[2],ecur;
  
  //get channel counts per experiment
  int n_chans[2];
  int n_chansmax;
  n_chans[0] = glbGetNumberOfChannels(0);
  n_chans[1] = glbGetNumberOfChannels(1);
  if(n_chans[0]>n_chans[1])
    n_chansmax=n_chans[0];
  else
    n_chansmax=n_chans[1];
  
  //get bin information per experiment
  int n_bins[2];
  int n_binsmax;
  n_bins[0] = glbGetNumberOfBins(0);
  n_bins[1] = glbGetNumberOfBins(0);
  if(n_bins[0]>n_bins[1])
    n_binsmax=n_bins[0];
  else
    n_binsmax=n_bins[1];
  glbGetEminEmax(0, &emin[0], &emax[0]);
  glbGetEminEmax(1, &emin[1], &emax[1]);
  double *binwidths[2];
  binwidths[0]=glbGetBinSizeListPtr(0);
  binwidths[1]=glbGetBinSizeListPtr(1);
  
  //populate channel based fit and sim rates per experiment
  double fit_rates_per_channel[2][n_chansmax][n_binsmax];
  double sim_rates_per_channel[2][n_chansmax][n_binsmax];
  for(exp=0;exp<=1;exp++){
    for(i=0;i<n_chans[exp];i++){
      double * srates=glbGetChannelRatePtr(exp,i,GLB_POST);
      double * frates=glbGetChannelFitRatePtr(exp,i,GLB_POST);
      for(ebin=0;ebin<n_bins[exp];ebin++){
        sim_rates_per_channel[exp][i][ebin] = srates[ebin];
        fit_rates_per_channel[exp][i][ebin] = frates[ebin];
      }
    }
  }
    
  //create a file for combined, nu, and anu spectrums
  sprintf(filename,"spec_%s_hier%d_deg%d_nu.dat",type,hier,number);
  fout[0] = fopen(filename, "w");  
  sprintf(filename,"spec_%s_hier%d_deg%d_anu.dat",type,hier,number);
  fout[1] = fopen(filename, "w");
  
  //get systematics info for first experiment only
  int bg_channels[2];
  bg_channels[0] = glbGetLengthOfRule(0,0,GLB_BG);
  bg_channels[1] = glbGetLengthOfRule(1,0,GLB_BG);

  char sys_id[2][255];
  glbGetChiFunction(0,0,GLB_ON,sys_id[0],255);
  glbGetChiFunction(1,0,GLB_ON,sys_id[1],255);
 
  for(exp=0;exp<=1;exp++){
    ecur=emin[exp];
    for(ebin=0;ebin<n_bins[exp];ebin++){
      fprintf(fout[exp],"%6.4e\t",ecur);
      for(i=0;i<n_chans[exp];i++){
        fprintf(fout[exp],"%6.6e\t",sim_rates_per_channel[exp][i][ebin]);
      }
      fprintf(fout[exp],"\t");
      for(i=0;i<n_chans[exp];i++){
        fprintf(fout[exp],"%6.6e\t",fit_rates_per_channel[exp][i][ebin]);
      }
      fprintf(fout[exp],"\t");
      //print adjusted spectra depending on systematics function in use
      /*if(strncmp(sys_id,"chiSpectrumTiltSplitBG",22)==0){
        double ecenter = 0.5 * (emax + emin);
        double signal_tilt = xmin[1] / (emax - emin);
        double bg_tilt = xmin[2];
        double bg_norm_center, bg_tilt_center;
        glbGetBGCenters(0,0, &bg_norm_center, &bg_tilt_center);
        
        //signal
        fprintf(fout,"%6.6e\t",(1.0+xmin[0])*signal_fit_rates[ebin]+ signal_tilt*(bin_centers[ebin]-ecenter)*signal_fit_rates[ebin]);
        //backgrounds (note this relies on the background channels coming first in order in the glb)
        double bg_total=0;
        for(i=0;i<bg_channels;i++){
          bg_total += (1.0+xmin[i+3])*fit_rates_per_channel[i][ebin];
        }
        fprintf(fout,"%6.6e\t",bg_total);
      }*/
      if(strncmp(sys_id[exp],"chiSpectrumSplitBG",18)==0){
        //signal
        fprintf(fout[exp],"%6.6e\t",(1.0+xmin[exp][0])*signal_fit_rates[exp][ebin]);
        //backgrounds (note this relies on the background channels coming first in order in the glb)
        double bg_total=0;
        for(i=0;i<bg_channels[exp];i++){
          bg_total += (1.0+xmin[exp][i+1])*fit_rates_per_channel[exp][i][ebin];
        }
        fprintf(fout[exp],"%6.6e\t",bg_total);
      }
      if(strncmp(sys_id[exp],"chiSpectrumTiltCustom",21)==0){
        double ecenter = 0.5 * (emax[exp] + emin[exp]);
        double signal_tilt = xmin[exp][1] / (emax[exp] - emin[exp]);
        double bg_tilt = xmin[exp][3];
        double bg_norm_center, bg_tilt_center;
        glbGetBGCenters(0,0, &bg_norm_center, &bg_tilt_center);
        double bg_norm = bg_norm_center * (1.0 + xmin[exp][2]);
        
        //signal
        double sig = (1.0+xmin[exp][0])*signal_fit_rates[exp][ebin]
                    + signal_tilt*(bin_centers[exp][ebin]-ecenter)*signal_fit_rates[exp][ebin];
        fprintf(fout[exp],"%6.6e\t",sig);
        //backgrounds
        double bg = bg_norm*bg_fit_rates[exp][ebin]
                    + bg_tilt*(bin_centers[exp][ebin]-ecenter)*bg_fit_rates[exp][ebin];
        fprintf(fout[exp],"%6.6e\t",bg);
        
        //if(type=="T"){
        //  printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\n",ebin,sig+bg,ts[0][ebin],ts[1][ebin],ts[2][ebin],ts[3][ebin],ts[4][ebin]);
        //}
        
      }
      fprintf(fout[exp],"\n");
      ecur=ecur+binwidths[exp][ebin];
    }
  }

  fclose(fout[0]);
  fclose(fout[1]);
  
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
  double osc[]={0.57964,0.100679,M_PI/4,0,0.000079,0.0024};
  double osce[]={0.1,0.0,0.1,0,0.04,0.1};
  double xrange[]={-4,0};
  double yrange[]={0,360};
  double Yrange[]={0,360};
  double Xrange[]={-3,-1};
  double posx,posy;
  int resolution[]={25,25};
  int Resolution[]={2,2};
  int Plane[]={GLB_THETA_13,GLB_DELTA_CP};
 
  char* central_values=NULL;
  
  /* defaults */
  arguments.args[0]="-";
  arguments.resolution=NULL;
  arguments.Resolution=NULL;
  arguments.exp=0;
  arguments.params=NULL;
  arguments.paramse=NULL;
  arguments.xrange=NULL;
  arguments.yrange=NULL;
  arguments.Xrange=NULL;
  arguments.Yrange=NULL;
  arguments.Plane=NULL;
  arguments.force=FORCE;
  arguments.oct=NO;
  arguments.hier=NO;
  arguments.density=0.05;
  arguments.silent=NO;
  arguments.sth13=YES; 
  arguments.cpv=YES;
  arguments.mth23=YES;
  arguments.fast=NO;
  arguments.chimode=-1;
  arguments.enablespectra=NO;
  arguments.jellybeans=NO;

  /* parsing the comand line */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);  

  /* Lexing the oscillation parameters as given by the environment
   * variable GLB_CENTRAL_VALUES
   */
  central_values=getenv("GLB_CENTRAL_VALUES");
 
  vector_read_double(central_values,osc,6,parse_error);
  vector_read_double(arguments.params,osc,6,parse_error);
  vector_read_double(arguments.paramse,osce,6,parse_error);
  vector_read_double(arguments.Xrange,Xrange,2,parse_error);
  vector_read_double(arguments.Yrange,Yrange,2,parse_error);
  vector_read_double(arguments.xrange,xrange,2,parse_error);
  vector_read_double(arguments.yrange,yrange,2,parse_error);
 
  vector_read_int(arguments.resolution,resolution,2,parse_error);
  vector_read_int(arguments.Resolution,Resolution,2,parse_error);
  vector_read_int(arguments.Plane,Plane,2,parse_error);

  /* Initialize libglobes */
  glbInit(argv[0]);
  glbDefineChiFunction(&chiSpectrumTiltSplitBG,7,"chiSpectrumTiltSplitBG7",NULL);
  glbDefineChiFunction(&chiSpectrumTiltSplitBG,11,"chiSpectrumTiltSplitBG11",NULL);
  glbDefineChiFunction(&chiSpectrumSplitBG,5,"chiSpectrumSplitBG5",NULL);
  glbDefineChiFunction(&chiSpectrumSplitBG,9,"chiSpectrumSplitBG9",NULL);
  glbDefineChiFunction(&chiSpectrumSplitBG,6,"chiSpectrumSplitBG6",NULL);
  glbDefineChiFunction(&chiSpectrumTiltCustom,4,"chiSpectrumTiltCustom",NULL);
  if(arguments.fast==YES) glbSelectMinimizer(GLB_MIN_POWELL);

  /* Initialize my prior */
  tmyprior_init();
  glbRegisterPriorFunction(tmyprior_prior,tmyprior_starting_values,tmyprior_input_errors,NULL);


  for(i=0;i<arguments.exp-1;i++)
    {
      s=glbInitExperiment(arguments.args[i],&glb_experiment_list[0],&glb_num_of_exps);

      /* Testing for failure */
      if(s<-1) {fprintf(stderr,"%s: FATAL: Unrecoverable parse error\n",
		    argv[0]);exit(1);}
  
    }

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
      printf("Systematics are switched off\n");
      glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
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

  //verify setting of chi function
  old_sys_errors = glbGetSysErrorsListPtr(0, 0, GLB_ON); 
  sys_dim = glbGetSysDimInExperiment(0, 0, GLB_ON);
  char sys_id[255];
  glbGetChiFunction(0,0,GLB_ON,sys_id,255);
  double snorm=0.,stilt=0.,bgnorm=0.,bgtilt=0.;
 
  if(sys_dim>0) snorm=old_sys_errors[0];
  if(sys_dim>1) stilt=old_sys_errors[1];
  if(sys_dim>2) bgnorm=old_sys_errors[2];
  if(sys_dim>3) bgtilt=old_sys_errors[3];
  printf("Chi function is set to: %s(%f,%f,%f,%f (sys_dim is %d))\n",sys_id,snorm,stilt,bgnorm,bgtilt,sys_dim);

  

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
      print_header(output,"globes-discovery","0",argc,argv);
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
	
	glbDefineParams(input_errors,osce[0]*osc[0],osce[1]*osc[1],osce[2]*osc[2],osce[3]*osc[3],osce[4]*osc[4],osce[5]*osc[5]);
	glbSetDensityParams(input_errors,arguments.density,GLB_ALL);

	ct=eightfold(true_values,input_errors,arguments.hier,arguments.oct,xrange,yrange,resolution,&listb,&fchi);
  glbPrintParams(stdout,input_errors);
	double rdeg=DBL_MAX;
	for(i=0;i<ct;i++){
	 
	  fprintf(output,"G%c%c %6.6e\t",hie,oct,fchi[i]);
	  my_print_params2(output,listb[i]);
	  fprintf(output,"\n");
    if(arguments.enablespectra)
      save_spectra("G",arguments.hier,i);
	  if(fchi[i]<=rdeg) rdeg=fchi[i];
	  
	}
	fflush(output);
	glbCopyParams(true_values,test_values);
	
	if(arguments.hier==YES)
	  {
	    double nv;
	    /* following hep-ph/0509359 eq. 3 */
	    nv=-osc[5]+2*osc[4]*cos(osc[0])*cos(osc[0]);
      //changed to be as in the globes user manual
      //nv=-osc[5]+osc[4];
      //printf("dm_atm becomes: %f\n",nv);
	    glbSetOscParams(test_values,nv,GLB_DM_ATM);
	    
	  }
	if(arguments.oct==YES) {
	  glbSetOscParams(test_values,M_PI/2-osc[2],GLB_THETA_23);
	}
	glbCopyParams(test_values,starting_values);
	glbDefineParams(input_errors,osce[0]*osc[0],osce[1]*osc[1],osce[2]*osc[2],osce[3]*osc[3],osce[4]*osc[4],osce[5]*osc[5]);
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


	if(arguments.jellybeans==YES){
		for(i=0;i<ct;i++){
			glbCopyParams(listb[i],buffer);
			glbSetOscParams(buffer,osc[1],GLB_THETA_13);
			glbSetOscParams(buffer,0,GLB_DELTA_CP);
			if(arguments.test==YES){
				rest=glbChiSys(buffer,GLB_ALL,GLB_ALL);
			}else{
				if(osce[1]==0.0)
					rest=glbChiTheta13Delta(buffer,minimum,GLB_ALL);
				else
					rest=glbChiDelta(buffer,minimum,GLB_ALL);
			}
			fprintf(output,"Z%c%c %6.6e\t",hie,oct,rest);
			my_print_params2(output,minimum);
			fprintf(output,"\n");

			glbCopyParams(listb[i],buffer);
			glbSetOscParams(buffer,osc[1],GLB_THETA_13);
			glbSetOscParams(buffer,-M_PI/2,GLB_DELTA_CP);
			if(arguments.test==YES){
				rest=glbChiSys(buffer,GLB_ALL,GLB_ALL);
			}else{
				if(osce[1]==0.0)
					rest=glbChiTheta13Delta(buffer,minimum,GLB_ALL);
				else
					rest=glbChiDelta(buffer,minimum,GLB_ALL);
			}
			fprintf(output,"M%c%c %6.6e\t",hie,oct,rest);
			my_print_params2(output,minimum);
			fprintf(output,"\n");

			glbCopyParams(listb[i],buffer);
			glbSetOscParams(buffer,osc[1],GLB_THETA_13);
			glbSetOscParams(buffer,M_PI/2,GLB_DELTA_CP);
			if(arguments.test==YES){
				rest=glbChiSys(buffer,GLB_ALL,GLB_ALL);
			}else{
				if(osce[1]==0.0)
					rest=glbChiTheta13Delta(buffer,minimum,GLB_ALL);
				else
					rest=glbChiDelta(buffer,minimum,GLB_ALL);
			}
			fprintf(output,"P%c%c %6.6e\t",hie,oct,rest);
			my_print_params2(output,minimum);
			fprintf(output,"\n");

			fflush(output);
		}
	}
	   
	    if(arguments.sth13==YES)
	      {
		for(i=0;i<ct;i++)
		  {
		    glbCopyParams(listb[i],buffer);
		
		    glbSetOscParams(buffer,0,GLB_THETA_13);
		     if(arguments.test==YES){
				rest=glbChiSys(buffer,GLB_ALL,GLB_ALL);
			  }else{
				rest=glbChiTheta(buffer,minimum,GLB_ALL);
			  }
		    
		    fprintf(output,"T%c%c %6.6e\t",hie,oct,rest);
		    my_print_params2(output,minimum);
		    fprintf(output,"\n");
        if(arguments.enablespectra)
          save_spectra("T",arguments.hier,i);

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
			if(arguments.test==YES){
				resc=glbChiSys(buffer,GLB_ALL,GLB_ALL); 
			}else{
				resc=glbChiDelta(buffer,minimum,GLB_ALL); 
			}
		    fprintf(output,"D%c%c %6.6e\t",hie,oct,resc);
		    my_print_params2(output,minimum);
		    fprintf(output,"\n");
        if(arguments.enablespectra)
          save_spectra("D",arguments.hier,0+i*4);
        
          		    
		    glbCopyParams(true_values,buffer);
		    glbSetOscParams(buffer,M_PI,GLB_DELTA_CP);
		    if(arguments.test==YES){
				resd=glbChiSys(buffer,GLB_ALL,GLB_ALL); 
			}else{
				resd=glbChiDelta(buffer,minimum,GLB_ALL); 
			} 
		    fprintf(output,"D%c%c %6.6e\t",hie,oct,resd);
		    my_print_params2(output,minimum);
		    fprintf(output,"\n");
        if(arguments.enablespectra)
          save_spectra("D",arguments.hier,1+i*4);

		    glbCopyParams(listb[i],buffer);
		    /*testing the other cases found previously as global
		      minima */ 	
		    glbSetOscParams(buffer,0,GLB_DELTA_CP);
		    if(arguments.test==YES){
				resa=glbChiSys(buffer,GLB_ALL,GLB_ALL); 
			}else{
				resa=glbChiDelta(buffer,minimum,GLB_ALL); 
			}
		    fprintf(output,"D%c%c %6.6e\t",hie,oct,resa);
		    my_print_params2(output,minimum);
		    fprintf(output,"\n");
        if(arguments.enablespectra)
          save_spectra("D",arguments.hier,2+i*4);
        //printf("FINAL CHI1:%f\n",fchi1);
        //printf("FINAL CHI2:%f\n",fchi2);
        //int p=0;
        //for(p=0;p<4;p++)
        //  printf("\t prior: %f\n",priors[p]);
          
		    glbSetOscParams(buffer,M_PI,GLB_DELTA_CP);
		    if(arguments.test==YES){
				resb=glbChiSys(buffer,GLB_ALL,GLB_ALL); 
			}else{
				resb=glbChiDelta(buffer,minimum,GLB_ALL); 
			}
		    fprintf(output,"D%c%c %6.6e\t",hie,oct,resb);
		    my_print_params2(output,minimum);
		    fprintf(output,"\n");
        if(arguments.enablespectra)
          save_spectra("D",arguments.hier,3+i*4);
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
		    my_print_params2(output,minimum);
		    fprintf(output,"\n");

		    if(resat<=rat) rat=resat;
		    fflush(output);
		  }
	      }
	    
	  
	free_list(listb,ct);
	free(fchi);
  if(arguments.enablespectra==YES){
    ybreak=xbreak=YES;
  }
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



