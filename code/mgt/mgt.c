/*based on example5.c from GLoBES examples*/

#ifndef MGT_HELPER_H
#include "mgt_helper.h"
#endif
#include "customChis.h"
#include "sigdcp.h"
#include "senseVlum.h"
#include "dcpVsTheta.h"
#include "priors.h"
#include "mctools.h"
#include "globes-REdiscovery.h"

int main(int argc, char *argv[])
{ 
  //set default arguments
  parse_setdefaults();
  /* parsing the comand line */
  argp_parse (&_argp, argc, argv, 0, 0, &arguments);  

  //set default oscillation parameters 
  double osc[]={0.593,0.154,0.705,0,7.58e-5,2.35e-3};
  double osce[]={0.03,0.03,0.11,0,0.03,0.05};
  vector_read_double(arguments.params,osc,6,parse_error);
  vector_read_double(arguments.paramse,osce,6,parse_error);
  printf("Running with osc parameters, osc(%f,%f,%f,%f,%f,%f)\n",osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  printf("Running with relative errors, osce(%f,%f,%f,%f,%f,%f)\n",osce[0],osce[1],osce[2],osce[3],osce[4],osce[5]);
  if(arguments.test)
    printf("Running in test mode (glbChisys used everywhere).");
  printf("Opposite hierarchy %s be minimized over.\n",(arguments.hier==YES ? "will" : "will not"));
	
  /*parse range and part from arguments*/
  double xrange[2]={1e-2,10};
  vector_read_double(arguments.xrange,xrange,2,parse_error);
  arguments.min_runtime=xrange[0];
  arguments.max_runtime=xrange[1];
  printf("Using range %f,%f with %d steps.\n",arguments.min_runtime,arguments.max_runtime,arguments.tSteps);
  double parts[2]={0,0};
  vector_read_double(arguments.inpart,parts,2,parse_error);
  arguments.part[0]=(int)parts[0];
  arguments.part[1]=(int)parts[1];
  if(arguments.part[1]>0)
    printf("Runing part %d of %d.\n",arguments.part[0],arguments.part[1]);

  
  /* Initialize GLoBES and define chi^2 functions and prior*/
  glbInit(argv[0]);
  definechifunctions();
  registerprior();
  
  //load experiments from command line arguments
  for(int i=0;i<arguments.exp-1;i++){
    printf("Loading glb file:%s\n",arguments.args[i]);
    int s=glbInitExperiment(arguments.args[i],&glb_experiment_list[0],&glb_num_of_exps);

    /* Testing for failure */
    if(s<-1) {fprintf(stderr,"%s: FATAL: Unrecoverable parse error\n",
		      argv[0]);exit(1);}
  }

  //ETW 4/10/2015 Uncomment the following to correlate experiments for version 3.2.13
  /* for (int i=0;i<glb_num_of_exps;i++)
    {
      for (int j=i+1;j<glb_num_of_exps;j++){
        int t = glbCorrelateSys(glb_experiment_list[i],glb_experiment_list[j]);
      }
      } */

  /* Initialize parameter vectors */
  true_values  = glbAllocParams();
  test_values  = glbAllocParams();
  input_errors = glbAllocParams();
  central_values = glbAllocParams();
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbDefineParams(central_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetDensityParams(central_values,1.0,GLB_ALL);
  glbDefineParams(test_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);  
  glbSetDensityParams(test_values,1.0,GLB_ALL);
  glbDefineParams(input_errors,osce[0]*osc[0],osce[1]*osc[1],osce[2]*osc[2],osce[3]*osc[3],osce[4]*osc[4],fabs(osce[5]*osc[5]));
  //glbDefineParams(input_errors,osce[0]*osc[0],osce[1]*osc[1],osce[2]*osc[2],osce[3]*osc[3],osce[4]*osc[4],osce[5]*osc[5]);
  glbSetDensityParams(input_errors, 0.02, GLB_ALL);
  //glbSetDensityParams(input_errors, 1e-5, GLB_ALL);
  glbSetOscillationParameters(true_values);
  glbSetCentralValues(central_values);
  glbSetInputErrors(input_errors);
  glbSetRates();

  //setup systematics
  FILE *emFile;
  float buf; int dbuf;
  double *old_sys_errors = glbGetSysErrorsListPtr(0, 0, GLB_ON); 
  int sys_dim = glbGetSysDimInExperiment(0, 0, GLB_ON);
  double new_errors[sys_dim];
  for(int idim=0;idim<sys_dim;idim++) new_errors[idim]=old_sys_errors[idim];
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
  case 10: case 13: case 20:
    //chiSpectrum_ErrorMatrix
    //10 is for regular matrices, 20 regular matrices with siman
    //13 is for combined analysis matrices
    //14 is for regular matrices, but only first four experiments have matrices
    //15 is for combined analysis matrices, but only first four experiments have matrices
			
    printf("Using chiSpectrum_ErrorMatrix...\n");
			
    if(!arguments.ematrixfile){
      printf("Error matrix file location (ematrixfile) not set! Exiting...\n");
      exit(1);
    }
			
    printf("Opening ematrix file: %s\n",arguments.ematrixfile);
    emFile = fopen(arguments.ematrixfile, "r");
    if(emFile==NULL){
      printf("Ematrix file does not exist, exiting!\n");
      exit(1);
    }
		
    for(int j=0;j<(arguments.chimode==14||arguments.chimode==15?4:glb_num_of_exps);j+=(arguments.chimode==13||arguments.chimode==15?2:1)){
      int analysis_bins=0;
      int channels;
      for(int k=0; k<(arguments.chimode==10||arguments.chimode==14||arguments.chimode==20?1:2); k++){
	int ew_low, ew_high;
	glbGetEnergyWindowBins(j+k, 0, &ew_low, &ew_high);
	analysis_bins+=ew_high-ew_low+1;
	channels=glbGetNumberOfChannels(j); //assume square covariance matrix for combined analysis
      }
      errorMatrix[j] = (double*)malloc(channels*channels*analysis_bins*analysis_bins*sizeof(double));		
				
      printf("Setting ematrix (channels=%d,analysis_bins=%d,elements=%d) for exp %d to:\n[",analysis_bins,channels,channels*channels*analysis_bins*analysis_bins,j);
      for(int i=0; i<channels*channels*analysis_bins*analysis_bins; i++){
	if(fscanf(emFile, "%f", &buf)==EOF){
	  printf("Ematrix has improper size. Exiting.\n");
	  exit(1);
	}
	errorMatrix[j][i]=buf;
	if(arguments.debug==1) printf(((i+1)%(channels*analysis_bins)==0 ? "%f" : "%f,"),errorMatrix[j][i]);
	if((i+1)%(channels*analysis_bins)==0 && i>0 && arguments.debug==1) printf("],\n[");
      }
      if(arguments.debug==1) printf("\n");
      char chiname[64];
				

	  sprintf(chiname,"chiSpectrum_ErrorMatrix_exp%d",j);
	  glbDefineChiFunction(&chiSpectrum_ErrorMatrix,analysis_bins,chiname,NULL);

        
      glbSetChiFunction(j,0,GLB_ON,chiname,new_errors);
    }
    fclose(emFile);

    break;
  case 11:
    //chiSpectrumTiltCustom_BtoB
    glbSetChiFunction(GLB_ALL,GLB_ALL,GLB_ON,"chiSpectrumTiltCustom_BtoB",new_errors);
    printf("sigma_binbin set to %f\n",sigma_binbin);
    break;
  case 16:
    //chiNormPerBin, uncorrelated normalization per bin
    //1s ranges from -e file
    printf("Using chiNormPerBin...\n");
			
    if(!arguments.ematrixfile){
      printf("1s errors file location not set! Exiting...\n");
      exit(1);
    }
    printf("Opening 1s errors file: %s\n",arguments.ematrixfile);
    emFile = fopen(arguments.ematrixfile, "r");
    if(emFile==NULL){
      printf("1s errors file does not exist, exiting!\n");
      exit(1);
    }
		
    int iRule=0; //counter for number of rules encountered
    for(int j=0;j<glb_num_of_exps;j++){
      for(int k=0;k<glbGetNumberOfRules(j);k++){
	int analysis_bins=0;
	int ew_low, ew_high;
	glbGetEnergyWindowBins(j, k, &ew_low, &ew_high);
	analysis_bins=ew_high-ew_low+1;
				
	errorMatrix[iRule] = (double*)malloc(analysis_bins*sizeof(double));	
	printf("Setting 1s errors (analysis_bins=%d) for exp %d, rule %d (rule %d) to:\n[",analysis_bins,j,k,iRule);
	for(int i=0; i<analysis_bins; i++){
	  if(fscanf(emFile, "%f", &buf)==EOF){
	    printf("1s errors file has improper size. Exiting.\n");
	    exit(1);
	  }
	  errorMatrix[iRule][i]=buf;
	  debugprintf("%f,",errorMatrix[iRule][i]); 
	}
	debugprintf("\n");
	char chiname[64];
	sprintf(chiname,"chiNormPerBin%d",iRule);
	glbDefineChiFunction(&chiNormPerBin,analysis_bins,chiname,NULL);
	glbSetChiFunction(j,k,GLB_ON,chiname,errorMatrix[iRule]);
	iRule++;
      }
    }
    fclose(emFile);
    break;
  case 19:
    //chiSpectrum_ResponseFunctions
    printf("Using chiSpectrum_ResponseFunctions...\n");
           
    if(!arguments.ematrixfile){
      printf("Response functions file location (ematrixfile) not set! Exiting...\n");
      exit(1);
    }


    printf("Opening response function file: %s\n",arguments.ematrixfile);

    emFile = fopen(arguments.ematrixfile, "r");
    if(emFile==NULL){
      printf("Response function file does not exist, exiting!\n");
      exit(1);
    }

    for(int exp=0;exp<glb_num_of_exps;exp++){
      int sampbins = glbGetNumberOfSamplingPoints(exp); //pre-smearing bins, note: for postsmearing systematics the pre/post smearing bins must be equal
      int channels=glbGetNumberOfChannels(exp);
      
      //initialize pre/post smearing effs
      mult_presmear_effs[exp] = (double**) malloc(channels*sizeof(double*));
      mult_postsmear_effs[exp] = (double**) malloc(channels*sizeof(double*));
      for(int ch=0;ch<channels;ch++){
        mult_presmear_effs[exp][ch]= (double*) malloc(sampbins*sizeof(double));
        mult_postsmear_effs[exp][ch]= (double*) malloc(sampbins*sizeof(double));
        for(int b=0;b<sampbins;b++){
          mult_presmear_effs[exp][ch][b]=1;
          mult_postsmear_effs[exp][ch][b]=1;
        }
      }
      
      //setup splines array for this experiment	
      rf_acc[exp] = (gsl_interp_accel**) malloc(arguments.systs*sampbins*channels*sizeof(gsl_interp_accel));
      rf_spline[exp] = (gsl_spline**) malloc(arguments.systs*sampbins*channels*sizeof(gsl_spline));
        
      for(int syst=0; syst<arguments.systs; syst++){
	      //read in header information
	      //first four lines are number of sigmas, sigma low, sigma high, penalty
	      for(int i=0;i<5;i++){
	        if((i==0||i==3?fscanf(emFile, "%d", &dbuf):fscanf(emFile, "%f", &buf))==EOF){
	          printf("Response function has improper size. Exiting.\n");
	          exit(1);
	        }
	        if(i==0) rfCols[exp][syst]=(int)dbuf;
	        if(i==3) penalty[exp][syst]=(int)dbuf;
	        if(i==4) prepost[exp][syst]=(int)dbuf; //0=pre- 1=post-smearing systematic
	      }
	      printf("rfCols[%d][%d]=%d, penalty[%d][%d]=%d, prepost[%d][%d]=%d\n",exp,syst,rfCols[exp][syst],exp,syst,penalty[exp][syst],exp,syst,prepost[exp][syst]);
	      //read in sigma values
	      double sigmas[rfCols[exp][syst]];
	      for(int i=0;i<rfCols[exp][syst];i++){
	        if(fscanf(emFile, "%f", &buf)==EOF){
	          printf("Response function has improper size. Exiting.\n");
	          exit(1);
	        }
	        sigmas[i]=buf;
	        //printf("%f, ",sigmas[i]);
	      }
	      double resp[rfCols[exp][syst]];
	      for(int ch=0; ch<channels; ch++){
	        printf("\n\nCHANNEL %d\n",ch);
	        for(int ebin=0; ebin<sampbins; ebin++){
	          for(int i=0; i<rfCols[exp][syst]; i++){
	            if(fscanf(emFile, "%f", &buf)==EOF){
		      printf("Response function has improper size. Exiting.\n");
		      exit(1);
	            }
	            resp[i]=buf;
	            //printf("%f, ",resp[i]);
	          }
	          //printf("\n");
	          //initialize spline
	          int index=sampbins*arguments.systs*ch+sampbins*syst+ebin;
	          rf_acc[exp][index] = gsl_interp_accel_alloc ();
	          //rf_spline[exp][index] = gsl_spline_alloc(gsl_interp_cspline, rfCols[exp][syst]);
	          rf_spline[exp][index] = gsl_spline_alloc(gsl_interp_linear, rfCols[exp][syst]);
	          gsl_spline_init (rf_spline[exp][index], sigmas, resp, rfCols[exp][syst]);
	        }
	      }
      }
      //only enable systematics for first experiment
      //other experiments get their systs from first exp
      if(exp==0){
	      char chiname[64];
	      sprintf(chiname,"chi_ResponseFunction_exp%d",exp);
	      double errors[2*glb_num_of_exps];
	      int nuisances=0;
	      switch(arguments.chimode){
	        case 19:
	          nuisances=12*arguments.systs;
	          break;
        }
	      glbDefineChiFunction(&chi_ResponseFunctionCov,nuisances,chiname,NULL);
        
	      glbSetChiFunction(exp,GLB_ALL,GLB_ON,chiname,errors);
      }else{
	      glbSetChiFunction(exp,GLB_ALL,GLB_ON,"chiZero",NULL);
      }
    }
    fclose(emFile);
    //load sigmas for channel2channel nuisance parameters
    LoadRFCovMatrixInputs();
    break;

  case 29:
    //ETW June 2015, copying case 19 with changes to accomodate 3d energy response functions
    //chiSpectrum_ResponseFunctions with energy response treatment
    printf("Using chiSpectrum_ResponseFunctions with Energy Response...\n");

    if(!arguments.ematrixfile){
      printf("Response functions file location (ematrixfile) not set! Exiting...\n");
      exit(1);
    }

    printf("Opening response function file: %s\n",arguments.ematrixfile);
    emFile = fopen(arguments.ematrixfile, "r");


    if(emFile==NULL){
      printf("Response function file does not exist, exiting!\n");
      exit(1);
    }

    for(int exp=0;exp<glb_num_of_exps;exp++){
      int sampbins = glbGetNumberOfSamplingPoints(exp); //pre-smearing bins
      int recbins = glbGetNumberOfBins(exp); //post-smearing bins
      int channels=glbGetNumberOfChannels(exp);

      //initialize pre-smearing effs and smearing adjustment matrices
      mult_presmear_effs[exp] = (double**) malloc(channels*sizeof(double*));
      mult_postsmear_matrix[exp] = (double***)malloc(channels*sizeof(double**));
      mult_postsmear_matrix_in[exp] = (double*****)malloc(arguments.systs*sizeof(double****));
      syst_sigmas[exp] = (double**)malloc(arguments.systs*sizeof(double*));
      size_sigmas[exp] = (int*)malloc(arguments.systs*sizeof(int));
      for(int ch=0;ch<channels;ch++){
        mult_presmear_effs[exp][ch]= (double*) malloc(sampbins*sizeof(double));
	mult_postsmear_matrix[exp][ch] = (double**)malloc(recbins*sizeof(double*));
        for(int b=0;b<sampbins;b++){
          mult_presmear_effs[exp][ch][b]=1;
        }
	for(int b=0;b<recbins;b++){
	  mult_postsmear_matrix[exp][ch][b] = (double*)malloc(recbins*sizeof(double));
	  for (int bb=0;bb<recbins;bb++){
	    if (b==bb) {
	      mult_postsmear_matrix[exp][ch][b][bb] = 1;
	    }
	    else {
	      mult_postsmear_matrix[exp][ch][b][bb] = 0;
	    }
	  }		    
	}
      }

      //setup splines array for this experiment	
      //ETW Splines will only be filled with the presmearing rfs, will do manual linear interpolation for
      //smearing adjustment matrices, so just set up regular arrays
      rf_acc[exp] = (gsl_interp_accel**) malloc(arguments.systs*sampbins*channels*sizeof(gsl_interp_accel));
      rf_spline[exp] = (gsl_spline**) malloc(arguments.systs*sampbins*channels*sizeof(gsl_spline));
	

      for(int syst=0; syst<arguments.systs; syst++){
	      //read in header information
	      //first four lines are number of sigmas, sigma low, sigma high, penalty
	      for(int i=0;i<5;i++){
	        if((i==0||i==3?fscanf(emFile, "%d", &dbuf):fscanf(emFile, "%f", &buf))==EOF){
	          printf("Response function has improper size. Exiting.\n");
	          exit(1);
	        }
	        if(i==0) rfCols[exp][syst]=(int)dbuf;
	        if(i==3) penalty[exp][syst]=(int)dbuf;
	        if(i==4) prepost[exp][syst]=(int)dbuf; //0=pre- 1=post-smearing systematic
	      }
	      mult_postsmear_matrix_in[exp][syst] = (double****)malloc(channels*sizeof(double***));
	      syst_sigmas[exp][syst] = (double*)malloc(rfCols[exp][syst]*sizeof(double));
	      size_sigmas[exp][syst] = rfCols[exp][syst];
	      printf("rfCols[%d][%d]=%d, penalty[%d][%d]=%d, prepost[%d][%d]=%d\n",exp,syst,rfCols[exp][syst],exp,syst,penalty[exp][syst],exp,syst,prepost[exp][syst]);
	      //read in sigma values
	      double sigmas[rfCols[exp][syst]];
	      for(int i=0;i<rfCols[exp][syst];i++){
	        if(fscanf(emFile, "%f", &buf)==EOF){
	          printf("Response function has improper size. Exiting.\n");
	          exit(1);
	        }
	        sigmas[i]=buf;
		syst_sigmas[exp][syst][i] = sigmas[i];
	        //printf("%f, ",sigmas[i]);
	      }
	      double resp[rfCols[exp][syst]];
	      for(int ch=0; ch<channels; ch++){
		mult_postsmear_matrix_in[exp][syst][ch] = (double***)malloc(rfCols[exp][syst]*sizeof(double**));
	        //printf("\n\nCHANNEL %d\n",ch);
		//ETW seperate treatment for reading in pre and post, pre unchanged
		if (prepost[exp][syst]==0){
		  for(int ebin=0; ebin<sampbins; ebin++){
		    for(int i=0; i<rfCols[exp][syst]; i++){
		      if(fscanf(emFile, "%f", &buf)==EOF){
			printf("Response function has improper size. Exiting.\n");
			exit(1);
		      }
		      resp[i]=buf;
		      //printf("%f, ",resp[i]);
		    }
		    //printf("\n");
		    //initialize spline
		    int index=sampbins*arguments.systs*ch+sampbins*syst+ebin;
		    rf_acc[exp][index] = gsl_interp_accel_alloc ();
		    rf_spline[exp][index] = gsl_spline_alloc(gsl_interp_linear, rfCols[exp][syst]);
		    gsl_spline_init (rf_spline[exp][index], sigmas, resp, rfCols[exp][syst]);
		  }
		}
		else if (prepost[exp][syst]==1){
		  for(int sbin=0; sbin<rfCols[exp][syst]; sbin++){
		    mult_postsmear_matrix_in[exp][syst][ch][sbin] = (double**)malloc(recbins*sizeof(double*));
		    for(int rbin=0; rbin<recbins; rbin++){
		      mult_postsmear_matrix_in[exp][syst][ch][sbin][rbin] = (double*)malloc(recbins*sizeof(double));
		      for (int rrbin=0; rrbin<recbins; rrbin++){
			if(fscanf(emFile, "%f", &buf)==EOF){
			  printf("Response function has improper size. Exiting.\n");
			  exit(1);
			}
			mult_postsmear_matrix_in[exp][syst][ch][sbin][rbin][rrbin] = buf;
		      }
		    }
		  }
		}
	      }
      }

    //only enable systematics for first experiment
      //other experiments get their systs from first exp
      if(exp==0){
	      char chiname[64];
	      sprintf(chiname,"chi_ResponseFunction_exp%d",exp);
	      double errors[2*glb_num_of_exps];
	      int nuisances=0;
	      switch(arguments.chimode){
	        case 29:
	          nuisances=12*arguments.systs;
	          break;
	      }
	      glbDefineChiFunction(&chi_ResponseFunctionCovE,nuisances,chiname,NULL);
	      glbSetChiFunction(exp,GLB_ALL,GLB_ON,chiname,errors);
      }
      else{
	glbSetChiFunction(exp,GLB_ALL,GLB_ON,"chiZero",NULL);
      }
    }
    fclose(emFile);
    //load sigmas for channel2channel nuisance parameters
    LoadRFCovMatrixInputs();
    break;

  case 30:
    //chiSpectrumNormCustom
    //use norm/bg from each exp/rule
    for(int exp=0;exp<glb_num_of_exps;exp++){
      for(int rule=0;rule<glbGetNumberOfRules(exp);rule++){
	double *old_sys_errors = glbGetSysErrorsListPtr(exp, rule, GLB_ON); 
	int sys_dim = glbGetSysDimInExperiment(exp, rule, GLB_ON);
	if(sys_dim<4){
	  printf("Error: sys_dim(=%d) is less than 4, can not use -I30\n",sys_dim);
	  exit(1);
	}
	double normerrors[2];
	normerrors[0]=old_sys_errors[0];
	normerrors[1]=old_sys_errors[2];
	printf("Setting errors on sig:%f and bg:%f in exp %d, rule%d\n", normerrors[0],normerrors[1],exp,rule);
	glbSetChiFunction(exp,rule,GLB_ON,"chiSpectrumNormCustom",normerrors);
      }
    }
    break;
  case 31: case 32:
    //chiCorrSplitBGs_LBNEFMC
    printf("Using chiCorrSplitBGs LBNEFMC...\n");
      
    for(int exp=0;exp<glb_num_of_exps;exp++){
      //only enable systematics for first experiment
      //other experiments get their systs from first exp
      if(exp==0){
	char chiname[64];
	sprintf(chiname,"chiCorrSplitBGs_LBNEFMC_exp%d",exp);
	glbDefineChiFunction(&chiCorrSplitBGs_LBNEFMC,4,chiname,NULL);
	double terrors[4];
	if(arguments.chimode==31){
	  terrors[0]=0.05;terrors[1]=0.05;terrors[2]=0.5;terrors[3]=0.1;
	}else{
	  terrors[0]=0.05;terrors[1]=0.05;terrors[2]=0.2;terrors[3]=0.1;
	}
	glbSetChiFunction(exp,GLB_ALL,GLB_ON,chiname,terrors);
      }else{
	glbSetChiFunction(exp,GLB_ALL,GLB_ON,"chiZero",NULL);
      }
    }

    break;
  }
  //output chi sys setting for each experiment, rule and computer number of nuisance paramters
  arguments.nuisances=0;
  for(int exp=0;exp<glb_num_of_exps;exp++){
    for(int rule=0;rule<glbGetNumberOfRules(exp);rule++){
      char sys_id[255];
      glbGetChiFunction(exp,rule,GLB_ON,sys_id,255);
      printf("Experiment %d, rule %d chi function (turned %s) is set to %s with %d parameters\n"
	     ,exp,rule,
	     glbGetSysOnOffState(exp, rule)==1?"ON":"OFF",
	     sys_id,glbGetSysDimInExperiment(exp, rule, GLB_ON));
      arguments.nuisances += glbGetSysDimInExperiment(exp, rule, GLB_ON);
    }
  }
  printf("Total number of nuisance parameters: %d\n",arguments.nuisances);
  
  
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


  //initialize output file if it ends in *.dat, otherwise exit
  //this prevents accidentally overwriting files if the output file is omitted
  if(strstr(arguments.args[arguments.exp-1],".dat")>0){
    InitOutput(arguments.args[arguments.exp-1],"");
  }else{
    printf("Output filename must end in *.dat. Exiting...\n");
    exit(0);  
  }
   
  //turn off systematics if needed
  //Note: this must be done after experiments are initialized
  if(arguments.chimode==101){
    glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
    printf("Systematics are switched off.\n");
  }

  /*compute exposure using mass of each experiment and time*/
  float exposure=0;
  for(int i=0;i<glb_num_of_exps;i++){
    exposure=exposure+glbGetTargetMass(i)*glbGetRunningTime(i, 0);
  }
  printf("Exposure is set to:%f\n",exposure);
  
  glb_projection projtNP; //projection for senseVlum runtypes
  double osctest[]={osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]}; //test oscs for functions that need it
  //handle runType settings
  printf("runCat, runtype is %d, %d\n",arguments.runCat,arguments.runType);
  switch (arguments.runCat){
  case 1: //sigdcp runtypes (sigdcp.c)
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
    case 5:
      OscProbs(osc);
      break;
    case 6:
      EventRates(osc);
      break;
    case 7:
      ComputeMHSigCurve_v2(osc,0.0,0);
      break;
    case 8:
      ComputeMHSigCurve_v2(osc,-M_PI/2, 0);
      break;
    case 9:
      ComputeCPResolutionCurve2(osc);
      break;
    case 10:
      ComputeCPVCurve_minOct(osc,osce);
      break; 
    case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27:
      //1d chisquareds
      //20=t12, 21=t13, 22=t23, 23=dcp, 24=DM21, 25=DM31
      dchisquared(osc,arguments.runType-20,xrange,(arguments.runType==22 || arguments.runType==25?1:0));
      break;
    case 28: case 29:
      //1d chisquareds for q23 and dm31 without dcp minimization
      dchisquared(osc,arguments.runType==28?2:5,xrange,0);
      break;
    case 30:
      //utility function for response function testing
      ComputeRFCurve(osc,0);
      break;
    case 31:
      //utility function for response function testing
      TrueFitSpectraMinOutput(osc);
      break;
    case 32:
      //utility function for response function testing
      ComputeRFCurve(osc,1);
      break;    
    case 33:
      //utility function for response function testing
      if(arguments.systs==2){
	ComputeRFCurve2d(osc);
      }else{
	printf("Not enough systs defined for this type. Exiting...\n");
	return(0);
	break; 
      }
      break;
    case 34:
      //utility function for response function testing
      ComputeNuisanceReponse(osc,arguments.nuis_output);
      break;   
    case 35:
      ComputeMHSigCurve_v2(osc,0.0,1);
      break;
    case 40: case 41: case 42: case 43: case 44: case 45:
      //1d FC Maps
      //40=t12, 41=t13, 42=t23, 43=dcp, 44=DM21, 45=DM31
      FCMap_1D(osc,arguments.runType-40,xrange);
      break;
    case 50: case 51: case 52: case 53: case 54: case 55:
      //Prior test scans
      //50=t12, 51=t13, 52=t23, 53=dcp, 54=DM21, 55=DM31
      test_priors(osc,arguments.runType-50,xrange);
      break;     
    default:
      printf("No runType set... exiting...\n");
      return(0);
      break; 
    }
    break;
  case 2:
    switch (arguments.runType){
    case 1:
      gsl_func.function = &DoChiSquareT13;
      ComputeT13SensitivityCurve(osc);
      break; 
    case 2:
      //deprecated
      //osc[1]=0.10067896; set this via cmd line parms
      //osc[5]=-osc[5]+osc[4];
      //osc[3]=0.0;
      //gsl_func.function = &DoChiSquareMH_deg;
      //ComputeMHSensitivityCurve(osc);
      break;
    case 3:
      gsl_func.function = &DoChiSquareCPRSVL;
      ComputeCPResolutionCurveSVL(osc);
      break;
    case 4:
      ComputeCPVSigFracCurve(osc);
      break;
    case 5:
      ComputeMHSigCurveSVL(osc);
      break;
    case 6:
      ComputeCPBubblesCurve(osc);
      break;
    case 7:
      ComputeMHSigCurve_bruteForce(osc);
      break;
    case 8:
      ComputeCPResolutionCurve2SVL(osc);
      break;
    case 9:
      //theta_23
      if(!arguments.test) arguments.preScan=1; //enable dcp prescan
      //printf("test\n");
      projtNP = glbAllocProjection();
      glbDefineProjection(projtNP, GLB_FREE, GLB_FREE, GLB_FIXED, GLB_FREE, GLB_FREE, GLB_FREE);
      glbSetDensityProjectionFlag(projtNP, GLB_FREE, GLB_ALL);
      glbSetProjection(projtNP);
      gsl_func.function = &DoChiSquareNP;
      arguments.scanVar = GLB_THETA_23;
      ComputeNPResCurve(osc, M_PI,0.0005, 0.0);
      break;
    case 10:
      //dm^2_31
      if(!arguments.test) arguments.preScan=1; //enable dcp prescan
      projtNP = glbAllocProjection();
      glbDefineProjection(projtNP, GLB_FREE, GLB_FREE, GLB_FREE, GLB_FREE, GLB_FREE, GLB_FIXED);
      glbSetDensityProjectionFlag(projtNP, GLB_FREE, GLB_ALL);
      glbSetProjection(projtNP);
      gsl_func.function = &DoChiSquareNP;
      arguments.scanVar = GLB_DM_31;
      gsl_set_error_handler_off();
      arguments.logs22th13_precision = 0.0000001;
      ComputeNPResCurve(osc, osc[5]+0.1*osc[5],0.001, 0.0);
      break;
    case 11:
      //theta_13
      if(!arguments.test) arguments.preScan=1; //enable dcp prescan
      projtNP = glbAllocProjection();
      glbDefineProjection(projtNP, GLB_FREE, GLB_FIXED, GLB_FREE, GLB_FREE, GLB_FREE, GLB_FREE);
      glbSetDensityProjectionFlag(projtNP, GLB_FREE, GLB_ALL);
      glbSetProjection(projtNP);
      gsl_func.function = &DoChiSquareNP;
      arguments.scanVar = GLB_THETA_13;
      ComputeNPResCurve(osc, M_PI,0.005, 0.0);
      break;
    case 12:
      ComputeNonZeroT13(osc);
      break;
    case 13:
      //delta_CP
      projtNP = glbAllocProjection();
      glbDefineProjection(projtNP, GLB_FREE, GLB_FREE, GLB_FREE, GLB_FIXED, GLB_FREE, GLB_FREE);
      glbSetDensityProjectionFlag(projtNP, GLB_FREE, GLB_ALL);
      glbSetProjection(projtNP);
      gsl_func.function = &DoChiSquareNP;
      arguments.scanVar = GLB_DELTA_CP;
      if(osc[3]==0){
	ComputeNPResCurve(osc,M_PI,0.01,-M_PI);
      }else{
	ComputeNPResCurve(osc,2*M_PI,0.01, 0.0);
      }
      break;
    case 14:
      ComputeMHSigCurve_glbchiAll(osc);
      break;
    case 15:
      ComputeOctCurve(osc);
      break;
    case 16:
      ComputeNMQCurve(osc);
      break;
    case 17:
      //this one uses glbChiAll AND computes dcp coverage at each exposure
      ComputeMHSigCurveFrac_glbchiAll(osc);
      break;
    case 18:
      ComputeCPVSVLCurve(osc);
      break;
    default:
      printf("No runType set... exiting...\n");
      return(0);
      break; 
    }
    break;
  case 3: //dcpVsTheta runtypes
    switch (arguments.runType){
    case 1:
      if(xrange[0]==1e-2 && xrange[1]==10.0){
	xrange[0]=0.57; 
	xrange[1]=1.0;
      }
      dcpvst23(osc,xrange);
      break;
    case 2:
      t13vst23(osc);
      break;
    case 3:
      if(xrange[0]==1e-2 && xrange[1]==10.0){
	xrange[0]=0.1007;
	xrange[1]=0.2318;
      }
      dcpvst13(osc,xrange);
      break;
    case 4:
      if(xrange[0]==1e-2 && xrange[1]==10.0){
	xrange[0]=0.55; 
	xrange[1]=1.0;
      }
      dm31vst23(osc,xrange);
      break;
    case 5:
      if(xrange[0]==1e-2 && xrange[1]==10.0){
	xrange[0]=0.57; 
	xrange[1]=1.0;
      }
      dcpvst23_oct(osc,xrange);
      break;
    case 6:
      if(xrange[0]==1e-2 && xrange[1]==10.0){
	xrange[0]=0.57; 
	xrange[1]=1.0;
      }
      dcpvst23_cpv(osc,xrange);
      break;
    case 7:
      if(xrange[0]==1e-2 && xrange[1]==10.0){
	xrange[0]=0.57; 
	xrange[1]=1.0;
      }
      dcpvst23_mh(osc,xrange);
      break;
    case 8:
      if(xrange[0]==1e-2 && xrange[1]==10.0){
	xrange[0]=0.57; 
	xrange[1]=1.0;
      }
      dcpvst23_nmq(osc,xrange);
      break;
    case 9:
      if(xrange[0]==1e-2 && xrange[1]==10.0){
	xrange[0]=0.57; 
	xrange[1]=1.0;
      }
      dcpvst13_nzt(osc,xrange);
      break;
    case 10:
      t13vst23_AR(osc);
      break;
    default:
      printf("No runType set... exiting...\n");
      return(0);
      break; 
    }
    break;
  case 4: //mc runs
    switch (arguments.runType){
    case 1:
      bestfitmc(osc);
      break;
    case 2: //MC with not fitting, no priors
      nofitmc(osc,0,0);
      break;
    case 3: //MC with not fitting, with priors
      nofitmc(osc,1,0);
      break; 
    case 4: //MC with not fitting, with priors, no bin-to-bin fluctuations
      nofitmc(osc,1,1);
      break;     
    case 10:
      osctest[5]=-osc[5]+osc[4];
      EventSpectraOutputTruePrePost(osc, osctest, 1);
      break;
    case 11:
      osctest[3]=-M_PI/2; //pi/2
      EventSpectraOutputTruePrePost(osc, osctest, 2);
      break;
    case 12:
      //compare to maximal
      projtNP = glbAllocProjection();
      glbDefineProjection(projtNP, GLB_FREE, GLB_FREE, GLB_FIXED, GLB_FREE, GLB_FREE, GLB_FREE);
      glbSetDensityProjectionFlag(projtNP, GLB_FREE, GLB_ALL);
      glbSetProjection(projtNP);
      osc[2]=M_PI/4; 
      EventSpectraOutputTruePrePost(osc, osctest, 3);
      break;
    case 13:
      //compare to opposite octant
      projtNP = glbAllocProjection();
      glbDefineProjection(projtNP, GLB_FREE, GLB_FREE, GLB_FIXED, GLB_FREE, GLB_FREE, GLB_FREE);
      glbSetDensityProjectionFlag(projtNP, GLB_FREE, GLB_ALL);
      glbSetProjection(projtNP);
      osc[2]=M_PI/2 - osc[2]; 
      EventSpectraOutputTruePrePost(osc, osctest, 3);
      break;
    case 14:
      osctest[3]=0;
      osc[3]=-M_PI/2;
      EventSpectraOutputTruePrePost(osc, osctest, 2);
      break;
    default:
      printf("No runType set... exiting...\n");
      return(0);
      break; 
    }
    break;
  case 5: //mc runs
    switch (arguments.runType){
    case 1:
      globesREdiscovery(osc, xrange);
      break;
    default:
      printf("No runType set... exiting...\n");
      return(0);
      break; 
    }
    break;
  }
  /* Clean up */
  glbFreeParams(true_values);
  glbFreeParams(test_values); 
  glbFreeParams(input_errors);

  return 0;  
}


