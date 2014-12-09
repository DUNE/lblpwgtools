/*Tool to compute signifiance vs delta_cp for mass hierarchy, cpv, et al*/
/*based on example5.c from GLoBES examples*/

#include "sigdcp.h"



double DoChiSquareCPR(double x, void *dummy){
  //Function for GSL to minimize for CP resolution
  double chi2;
  
  /* Set vector of test values */
  glbSetOscParams(test_values, M_PI*x/180.0, GLB_DELTA_CP);
	glbSetDensityParams(test_values,1.0,GLB_ALL);
  /* Compute Chi^2 for all loaded experiments and all rules*/
  if(arguments.test){
		chi2 = glbChiSys(test_values, GLB_ALL, GLB_ALL);
	}else{
		chi2 = glbChiDelta(test_values, NULL, GLB_ALL);
	}
  return chi2 - 1.0;
}

void ComputeRFCurve(double osc[], int truefitspectra){
  //utility function for testing response curves
  //outputs chisq vs n (nuiscance parameter value)
  //if truefitspectra is 1 then only do dcp=0, and enable spectrum output
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetOscillationParameters(true_values);
  glbSetRates();  

  //enable truefit spectra output if truefitspectra==1
  if(truefitspectra==1) arguments.debug=1;
  
  double inc =(truefitspectra==1?0.5:0.1);
  double osc3[]={0,-M_PI/2,M_PI/2}; 
  double chi2[3];
  int nuisances=arguments.systs*(arguments.chimode==19?12:1);
  double narray[nuisances];
  for(double n=pstart(-4,4,inc);n<=pend(-4,4,inc);n=n+pinc(-4,4,inc)){
    for(int i=0; i<nuisances; i++){
      if(arguments.chimode==19)
        narray[i]=(i%12==0?n:0); //only modify the numu nuisance param.
      else  
        narray[i]=n;
    }
    for(int k=0;k<(truefitspectra==1?1:3);k++){
      glbDefineParams(test_values,osc[0],osc[1],osc[2],osc3[k],osc[4],osc[5]);
      
      //switch systematics off and call glbchisys so that event rates are set
      glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
      glbChiSys(test_values, GLB_ALL, GLB_ALL);
      glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_ON);
      chi2[k]=chi_ResponseFunctionCov(0, 0, nuisances, narray, NULL, NULL);
    }
    double a[]={n,chi2[0],chi2[1],chi2[2]};
    if(truefitspectra==0) AddArrayToOutput(a,4);
  }
}

void ComputeNuisanceReponse(double osc[], int nuis){
  //utility function for testing systematic responses of nuisance parameter nuis
  //outputs chisq vs n (nuisance parameter value)
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetOscillationParameters(true_values);
  glbSetRates();  

  double inc=((arguments.max_runtime-arguments.min_runtime)/arguments.tSteps);
  double chi2[3];
  double osc3[]={0,-M_PI/2,M_PI/2}; 
  
  for(double n=arguments.min_runtime;n<=arguments.max_runtime;n=n+inc){
    int nuisances=4;
    double narray[4];
    for(int i=0; i<nuisances; i++){
      narray[i]=0;
    }
    narray[nuis]=n;
    
    for(int k=0;k<3;k++){
      chi2[k]=0;
      //printf("%f,%f\n",n,inc);
      for(int curexp=0;curexp<glb_num_of_exps;curexp++){
        for(int currule=0;currule<glbGetNumberOfRules(curexp);currule++){
          glbDefineParams(test_values,osc[0],osc[1],osc[2],osc3[k],osc[4],osc[5]);
          //switch systematics off and call glbchisys so that event rates are set
          glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
          glbChiSys(test_values, GLB_ALL, GLB_ALL);
          glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_ON);
          
          glb_chi_function fpt=glbGetChiFunctionPtrInExperiment(curexp,currule, GLB_ON);
          double* errors=glbGetSysErrorsListPtr(curexp,currule, GLB_ON);
          chi2[k]+=fpt(curexp, currule, nuisances, narray, errors, NULL);
        }
      }
    }
    double a[]={n,chi2[0],chi2[1],chi2[2]};
    AddArrayToOutput(a,4);
  }
}

void ComputeRFCurve2d(double osc[]){
  //utility function for testing TWO response curves
  //outputs chisq vs n1,n2 (nuisance parameter values)
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetOscillationParameters(true_values);
  glbSetRates();  

 
  double inc=0.1;
  double chi2;
  int nuisances=arguments.systs*(arguments.chimode==19?12:1);
  double narray[nuisances];
  for(double n1=pstart(-4,4,inc);n1<=pend(-4,4,inc);n1=n1+pinc(-4,4,inc)){
    for(double n2=pstart(-4,4,inc);n2<=pend(-4,4,inc);n2=n2+pinc(-4,4,inc)){
      for(int i=0; i<nuisances; i++) narray[i]=0;
      narray[0]=n1; narray[12]=n2;
      //for(int i=0; i<nuisances; i++) printf("%f,",narray[i]);
      //printf("%f\n");
      glbDefineParams(test_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
      //switch systematics off and call glbchisys so that event rates are set
      glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
      glbChiSys(test_values, GLB_ALL, GLB_ALL);
      glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_ON);
      chi2=chi_ResponseFunctionCov(0, 0, nuisances, narray, NULL, NULL);
      double a[]={n1,n2,chi2};
      AddArrayToOutput(a,3);
    }
    AddToOutputBlankline();
  }
}

void TrueFitSpectraMinOutput(double osc[]){
  //utility function for output of true and fit energy spectra from a minization
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetOscillationParameters(true_values);
  glbSetRates();  
  
  //set debug flag to 1 so that rates will be added to output
  arguments.debug=1;
  double osc3[]={M_PI/2}; 
  for(int k=0;k<1;k++){
    glbDefineParams(test_values,osc[0],osc[1],osc[2],osc3[k],osc[4],osc[5]);
    glbChiSys(test_values, GLB_ALL,GLB_ALL);
    //glbChiSys(test_values, 0,0);
  }
}

void ComputeCPVCurve(double osc[]){
  //Compute CP violation sensitivity as a function of deltaCP
  double this_dcp;
  double chi2[4],chi2min;
  int opconsidered = (arguments.hier==YES ? 4 : 2);
  double mindcp=0;
  double mindm=0;
	double osc5[]={osc[5],osc[5],-osc[5]+osc[4],-osc[5]+osc[4]};		
	double osc3[]={0,M_PI,0,M_PI}; 
	double chi2_thisdcp[2];
	double osc5_thisdcp[2]={osc[5],-osc[5]+osc[4]};
	double bfs[6][2]; //store best fit values

  //define projection with only delta free, so that chisq (best_fit) can be calculated in the no osc systs case
	glb_projection projnp = glbAllocProjection();
	glbDefineProjection(projnp, GLB_FIXED, GLB_FIXED, GLB_FIXED, GLB_FREE, GLB_FIXED, GLB_FIXED);
	glbSetDensityProjectionFlag(projnp, GLB_FIXED, GLB_ALL);
	if(arguments.test) glbSetProjection(projnp);
	
  //define projection with delta, t12, dm21, and matt density fixed
	glb_projection projDeltaFixed = glbAllocProjection();
	glbDefineProjection(projDeltaFixed, GLB_FIXED, GLB_FREE, GLB_FREE, GLB_FIXED, GLB_FIXED, GLB_FREE);
	glbSetDensityProjectionFlag(projDeltaFixed, GLB_FIXED, GLB_ALL);
	//glbSetProjection(projDeltaFixed);

  //define projection with t12, dm21, and matt density fixed
	glb_projection projDeltaFree = glbAllocProjection();
	glbDefineProjection(projDeltaFree, GLB_FIXED, GLB_FREE, GLB_FREE, GLB_FREE, GLB_FIXED, GLB_FREE);
	glbSetDensityProjectionFlag(projDeltaFree, GLB_FIXED, GLB_ALL);
	//glbSetProjection(projDeltaFree);
	
  for(this_dcp=pstart(-M_PI,M_PI,M_PI/36);this_dcp<=pend(-M_PI,M_PI,M_PI/36);this_dcp=this_dcp+pinc(-M_PI,M_PI,M_PI/36)){
    for(int pfluct=0;pfluct<=(arguments.pflucts>0?arguments.pflucts-1:0);pfluct++){ //loop for statistical fluctuations
      glbDefineParams(true_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc[5]);
      glbSetOscillationParameters(true_values);
      glbSetRates();
      if(arguments.pflucts>0) pfluctTrueSpectra();
      for(int k=0;k<opconsidered;k++){
        glbDefineParams(test_values,osc[0],osc[1],osc[2],osc3[k],osc[4],osc5[k]);
        glbSetDensityParams(test_values,1.0,GLB_ALL);
        if(arguments.test){
          chi2[k]=glbChiSys(test_values, GLB_ALL,GLB_ALL);
        }else{
          glbSetCentralValues(test_values);
          glbSetProjection(projDeltaFixed);
          //chi2[k]=glbChiDelta(test_values,NULL, GLB_ALL);
          chi2[k]=glbChiNP(test_values,NULL, GLB_ALL);
        }
          //output xmin, fchi1, and fchi2 for testing
          if(arguments.nuis_output>0) AddArrayToOutput(NULL,0);
      }
      if(arguments.pflucts>0){
        //also compute a difference with this_dcp (for both MHs), so that a delta-chisq can be computed
        for(int k=0;k<2;k++){
          //set the fit start values at 0 and pi, though it shouldn't matter if the fitter works properly
          glbDefineParams(test_values,osc[0],osc[1],osc[2],osc3[k],osc[4],osc5_thisdcp[k]);
          glbSetDensityParams(test_values,1.0,GLB_ALL);
          if(arguments.test){
            //even with osc parms fixed, we still want the best fit to vary dcp
            chi2_thisdcp[k]=ChiNPDeltaPrescan(test_values,test_values,GLB_ALL);
          }else{
            glbSetCentralValues(test_values);
            //chi2_thisdcp[k]=glbChiDelta(test_values,NULL, GLB_ALL);
            //chi2_thisdcp[k]=ChiAllDeltaPrescan(test_values,NULL, GLB_ALL);
            //chi2_thisdcp[k]=glbChiAll(test_values,NULL, GLB_ALL);
            glbSetProjection(projDeltaFree);
            //chi2_thisdcp[k]=glbChiNP(test_values,test_values, GLB_ALL);
            chi2_thisdcp[k]=ChiNPDeltaPrescan(test_values,test_values, GLB_ALL);
          }
          bfs[3][k]=glbGetOscParams(test_values,GLB_DELTA_CP);
        }
      }
      chi2min=1e6;
      for(int k=0;k<opconsidered;k++){
        if(chi2[k]<chi2min){
          chi2min=chi2[k];
          mindcp=osc3[k];
          mindm=osc5[k];
        }
      }
      
      //print output
      double a[12];
      a[0]=this_dcp;
      a[1]=(chi2min);
      a[2]=mindcp;
      a[3]=mindm;
      a[4]=chi2[0];
      a[5]=chi2[1];
      a[6]=chi2[2];
      a[7]=chi2[3]; 
      a[8]=arguments.pflucts>0?chi2_thisdcp[0]:0;
      a[9]=arguments.pflucts>0?chi2_thisdcp[1]:0;
      a[10]=arguments.pflucts>0?bfs[3][0]:0;
      a[11]=arguments.pflucts>0?bfs[3][1]:0;
      AddArrayToOutput(a,12);
    }
  }
  glbFreeProjection(projnp);
  glbFreeProjection(projDeltaFixed);
  glbFreeProjection(projDeltaFree);
}

void ComputeMHSigCurve(double osc[]){
  //Compute MH sensitivity as a function of deltaCP
  //Uses GLoBES eightfold degeneracy finder
  double this_dcp;
 
  for(this_dcp=pstart(-M_PI,M_PI,M_PI/18);this_dcp<=pend(-M_PI,M_PI,M_PI/18);this_dcp=this_dcp+pinc(-M_PI,M_PI,M_PI/18)){
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
    int ct;
    double chi2  = 1.0e10;
    ct=eightfold(true_values,input_errors,YES,NO,xrange,yrange,resolution,&listb,&fchi);
    
    for(int i=0; i < ct; i++){
      if (fchi[i] < chi2){
        chi2 = fchi[i];
        Fit  = listb[i];
      }
    }
    
    double a[]={this_dcp,sqrt(chi2),glbGetOscParams(Fit,GLB_DELTA_CP),glbGetOscParams(Fit,GLB_THETA_13)};
    AddArrayToOutput(a,4);
	}
}

void ComputeMHSigCurve_v2(double osc[], double dcpin, int disableScanSysts)
{
  //Compute MH sensitivity as a function of deltaCP.
	//This version computes mh sig without eightfold by prescanning
  //over deltaCP and using glbChiAll
  double this_dcp,this_tdcp;
  double tchi2,chi2min;
  double oscmins[6][2];
 	double osc5[]={osc[5],-osc[5]+osc[4]};	
  double minchi2s[2];
  //set minimums to input values
  for(int i=0;i<5;i++) oscmins[i][0]=oscmins[i][1]=osc[i];
  
  for(this_dcp=pstart(-M_PI,M_PI,M_PI/18);this_dcp<=pend(-M_PI,M_PI,M_PI/18);this_dcp=this_dcp+pinc(-M_PI,M_PI,M_PI/18)){
    for(int pfluct=0;pfluct<=(arguments.pflucts>0?arguments.pflucts-1:0);pfluct++){ //loop for statistical fluctuations
      glbDefineParams(true_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc[5]);
      glbSetDensityParams(true_values,1.0,GLB_ALL);
      glbSetOscillationParameters(true_values);
      glbSetRates();
      if(arguments.pflucts>0) pfluctTrueSpectra();
      
      //if enabled, disable systs to speed up dcp prescan
      if(disableScanSysts==1) glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
      for(int i=0;i<2;i++){
        chi2min=1e8;
        if(dcpin!=0){
          glbDefineParams(test_values,osc[0],osc[1],osc[2],dcpin,osc[4],osc5[i]);  
          glbSetDensityParams(test_values,1.0,GLB_ALL);
          chi2min=glbChiSys(test_values,GLB_ALL,GLB_ALL);
          oscmins[3][i]=dcpin;
        }else{
          for(this_tdcp=-M_PI;this_tdcp<M_PI;this_tdcp=this_tdcp+(M_PI/72)){
            glbDefineParams(test_values,osc[0],osc[1],osc[2],this_tdcp,osc[4],osc5[i]);  
            glbSetDensityParams(test_values,1.0,GLB_ALL);
            tchi2=glbChiSys(test_values,GLB_ALL,GLB_ALL);
            if(tchi2<chi2min){
              chi2min=tchi2;
              oscmins[3][i]=this_tdcp;
            }
          }
        }
        if(disableScanSysts==1 && arguments.chimode!=101) glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_ON);
        if(arguments.test==NO){
          glbDefineParams(test_values,osc[0],osc[1],osc[2],oscmins[3][i],osc[4],osc5[i]);  
          glbSetDensityParams(test_values,1.0,GLB_ALL);
          glbSetCentralValues(test_values);
          chi2min=glbChiAll(test_values, test_values, GLB_ALL);
          for(int j=0;j<6;j++) oscmins[j][i]=glbGetOscParams(test_values,j);  
        }
        minchi2s[i]=chi2min;
      }
      
      //output false test first since that's what was plotted historically
      double a[12+3];
      a[0]=this_dcp;
      a[1]=minchi2s[1];
      a[2]=minchi2s[0];
      for(int i=0;i<6;i++){
        a[3+i]=oscmins[i][0];
        a[3+i+6]=oscmins[i][1];
      }
      //a[15]=glbGetDensityParams(test_values,0);
      //a[16]=glbGetDensityParams(test_values,1);
      //double a[]={this_dcp,minchi2s[1], minchi2s[0],mindcp[0], mindm31[0],mindcp[1], mindm31[1]};
      AddArrayToOutput(a,15);
    }
	}
 
}

void ComputeAllowedCP(double osc[])
{
  //Compute chisq as a function of true and test deltaCP
  double chi2[2],chi2min;
  double this_dcptest,this_dcptrue;
  int divisions=18;
  int opconsidered = (arguments.hier==YES ? 2 : 1);
  double osc5[]={osc[5],-osc[5]+osc[4]};
  
	for(this_dcptrue=-M_PI;this_dcptrue<M_PI;this_dcptrue=this_dcptrue+(M_PI/divisions)){
		glbDefineParams(true_values,osc[0],osc[1],osc[2],this_dcptrue,osc[4],osc[5]);
		glbSetOscillationParameters(true_values);
		glbSetRates();
		for(this_dcptest=-M_PI;this_dcptest<M_PI;this_dcptest=this_dcptest+(M_PI/divisions)){
			for(int k=0;k<opconsidered;k++){
				glbDefineParams(test_values,osc[0],osc[1],osc[2],this_dcptest,osc[4],osc5[k]);  
				glbSetDensityParams(test_values,1.0,GLB_ALL);
				if(arguments.test){
					chi2[k]=glbChiSys(test_values, GLB_ALL,GLB_ALL);
				}else{
					glbSetCentralValues(test_values);
					chi2[k]=glbChiDelta(test_values,test_values, GLB_ALL);
				}
			}
			chi2min=1e6;
			for(int k=0;k<opconsidered;k++){
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
  //Compute deltaCP resolution using GSL root finder.
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
    } while (gsl_status != GSL_SUCCESS && x_hi<360 && x_hi>-180);
    
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
    double a[]={this_dcp,xup, xdown};
		AddArrayToOutput(a,3);
  }
}
void ComputeCPResolutionCurve2(double osc[])
{
	//instead of fitting for 1 sigma, this version scans over all dcp
	//values, comparing to the input osc[3], and outputs the fraction
	//of dcp values greater than 1,2,3,5 sigma
  int i;
  int dcpsteps=360;
  double chis[dcpsteps];
  double chi2[4];
  double chi2min;
  double true_dcp, test_dcp;
  int opconsidered = (arguments.hier==YES ? 2 : 1);
  double osc5[]={osc[5],-osc[5]+osc[4]};
  
  for(true_dcp=-M_PI;true_dcp<M_PI;true_dcp=true_dcp+(M_PI/36)){
		i=0;
		glbDefineParams(true_values,osc[0],osc[1],osc[2],true_dcp,osc[4],osc[5]);
		glbSetOscillationParameters(true_values);
		glbSetRates();
		for(test_dcp=-M_PI;test_dcp<M_PI;test_dcp=test_dcp+(M_PI/(dcpsteps/2))){
			for(int k=0;k<opconsidered;k++){
				glbDefineParams(test_values,osc[0],osc[1],osc[2],test_dcp,osc[4],osc5[k]);  
				glbSetDensityParams(test_values,1.0,GLB_ALL);
				if(arguments.test){
					chi2[k]=glbChiSys(test_values, GLB_ALL,GLB_ALL);
				}else{
					glbSetCentralValues(test_values);
					chi2[k]=glbChiDelta(test_values,test_values, GLB_ALL);
				}
			}
			
			chi2min=1e6;
			for(int k=0;k<opconsidered;k++){
				if(chi2[k]<chi2min) chi2min=chi2[k];
			}
			chis[i]=chi2min;
			i+=1;

		}
		
		//now sort the list of chis (ascending) and find the cp fraction for which we can determine resolve dcp to 1,2,3,5 sigma
    qsort(chis,dcpsteps,sizeof(double),compare_asc);
    double frac1sig, frac2sig, frac3sig, frac5sig;
		frac1sig=frac2sig=frac3sig=frac5sig=1.00;
		for(i=0;i<dcpsteps;i++){
			if(chis[i]>1.0 && frac1sig==1.00) frac1sig=(float)i/(float)dcpsteps;	
			if(chis[i]>4.0 && frac2sig==1.00) frac2sig=(float)i/(float)dcpsteps;
			if(chis[i]>9.0 && frac3sig==1.00) frac3sig=(float)i/(float)dcpsteps;
			if(chis[i]>25.0 && frac5sig==1.00) frac5sig=(float)i/(float)dcpsteps;
		}

		double a[]={true_dcp,frac1sig,frac2sig,frac3sig,frac5sig};
		AddArrayToOutput(a,5);
  }
}

void OscProbs(double osc[]){
  //Compute oscillation probabilities as a function of E and deltaCP
	double erange[]={0,10};
	double estep=0.01;
  //double estep=1;
	//int dcpsteps=18;
	
	//for(double this_dcp=-M_PI;this_dcp<M_PI;this_dcp=this_dcp+(M_PI/dcpsteps)){
  //for(double this_dcp=0;this_dcp<M_PI;this_dcp=this_dcp+(M_PI/dcpsteps)){
  for(double thisE=erange[0]+estep;thisE<=erange[1];thisE+=estep){
    glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
    glbSetDensityParams(true_values,1.0,GLB_ALL);
    glbSetOscillationParameters(true_values);
    glbSetRates();
    double a[]={0,thisE,glbProfileProbability(0, 2, 1, +1, thisE),glbProfileProbability(0, 2, 1, -1, thisE),0.0,0.0,glbProfileProbability(0, 2, 2, +1, thisE),glbProfileProbability(0, 2, 2, -1, thisE),0.0,0.0,glbProfileProbability(0, 2, 3, +1, thisE),glbProfileProbability(0, 2, 3, -1, thisE),0.0,0.0};
    glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],-osc[5]+osc[4]);
    glbSetDensityParams(true_values,1.0,GLB_ALL);
    glbSetOscillationParameters(true_values);
    glbSetRates();
    a[4]=glbProfileProbability(0, 2, 1, +1, thisE);
    a[5]=glbProfileProbability(0, 2, 1, -1, thisE);
    a[8]=glbProfileProbability(0, 2, 2, +1, thisE);
    a[9]=glbProfileProbability(0, 2, 2, -1, thisE);
    a[12]=glbProfileProbability(0, 2, 3, +1, thisE);
    a[13]=glbProfileProbability(0, 2, 3, -1, thisE);
    AddArrayToOutput(a,14);			
  }
	//	AddToOutputBlankline();
	//}

}

void EventRates(double osc[]){
  //Compute event rates as a function of deltaCP
	int dcpsteps=18;
	int expnbr=0;
	int rulenbr=0;
	int n_bins=glbGetNumberOfBins(expnbr);
	double sig_rates_NH[glb_num_of_exps][n_bins];
	double sig_rates_IH[glb_num_of_exps][n_bins];
	/*note: assumes all bin centers match between the experiments*/
	double *binCenters;
	binCenters=glbGetBinCentersListPtr(0);
	
	for(double this_dcp=-M_PI;this_dcp<M_PI;this_dcp=this_dcp+(M_PI/dcpsteps)){
		glbDefineParams(true_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc[5]);
		glbSetDensityParams(true_values,1.0,GLB_ALL);
		glbSetOscillationParameters(true_values);
		glbSetRates();
		for(int i=0;i<glb_num_of_exps;i++){
			double *trates = glbGetSignalRatePtr(i, rulenbr);
			for(int thisEbin=0;thisEbin<=n_bins;thisEbin++) sig_rates_NH[i][thisEbin] = trates[thisEbin];
		}		
		glbDefineParams(true_values,osc[0],osc[1],osc[2],this_dcp,osc[4],-osc[5]+osc[4]);
		glbSetDensityParams(true_values,1.0,GLB_ALL);
		glbSetOscillationParameters(true_values);
		glbSetRates();
		for(int i=0;i<glb_num_of_exps;i++){
			double *trates = glbGetSignalRatePtr(i, rulenbr);
			for(int thisEbin=0;thisEbin<=n_bins;thisEbin++) sig_rates_IH[i][thisEbin] = trates[thisEbin];
		}		
		double a[2*glb_num_of_exps+2];
		for(int thisEbin=0;thisEbin<=n_bins;thisEbin++){
			a[0]=this_dcp; a[1]=binCenters[thisEbin];
			for(int i=0;i<glb_num_of_exps;i++){
				a[2+2*i]=sig_rates_NH[i][thisEbin];
				a[2+2*i+1]=sig_rates_IH[i][thisEbin];
			}
			AddArrayToOutput(a,2*glb_num_of_exps+2);
		}
		AddToOutputBlankline();
	}

}

void dchisquared(double osc[], int proj, double xrange[], int dcpPreScan){
  //Compute deltachisq for variations in osc, using projection proj, over xrange
  //dcpPrescan determines whether or not a minimization over deltaCP is done first for each step.
	int steps=100;
	int fixed[7];
	double testvals[2][7];
	double chi2[2];
	glb_params atest_values[2]; //store both best fit points in both MHs
	atest_values[0]  = glbAllocParams();
	atest_values[1]  = glbAllocParams();
  	
	for(int i=0;i<7;i++) fixed[i]=(proj==i?GLB_FIXED:GLB_FREE);
	glb_projection projnp = glbAllocProjection();
	glbDefineProjection(projnp, fixed[0], fixed[1], fixed[2], fixed[3], fixed[4], fixed[5]);
	glbSetDensityProjectionFlag(projnp, fixed[6], GLB_ALL);
	glbSetProjection(projnp);

	glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
	glbSetDensityParams(true_values,1.0,GLB_ALL);
	glbSetOscillationParameters(true_values);
	glbSetRates();

	for(double x=xrange[0];x<=xrange[1];x+=(xrange[1]-xrange[0])/steps){
		for(int k=0;k<2;k++){
			for(int i=0;i<6;i++) testvals[k][i]=(proj==i?x:osc[i]);
      double osc5[]={testvals[k][5],-testvals[k][5]+testvals[k][4]};
			glbDefineParams(atest_values[k],testvals[k][0],testvals[k][1],testvals[k][2],testvals[k][3],testvals[k][4],osc5[k]);
			glbSetDensityParams(atest_values[k],1.0,GLB_ALL);
			glbSetCentralValues(atest_values[k]);
			if(arguments.test){
				chi2[k]=glbChiSys(atest_values[k], GLB_ALL,GLB_ALL);
			}else{
			  if(dcpPreScan==1 && !arguments.test){
			    //include prescan over deltaCP
			    chi2[k]=ChiNPDeltaPrescan(atest_values[k],atest_values[k], GLB_ALL);
			  }else{
				  chi2[k]=glbChiNP(atest_values[k],atest_values[k], GLB_ALL);
			  }
			}
		}

		double a[]={x,chi2[0], chi2[1], glbGetOscParams(atest_values[0],1),glbGetOscParams(atest_values[0],2),glbGetOscParams(atest_values[0],3),glbGetOscParams(atest_values[0],3),glbGetOscParams(atest_values[0],5),glbGetOscParams(atest_values[1],1),glbGetOscParams(atest_values[1],2),glbGetOscParams(atest_values[1],3),glbGetOscParams(atest_values[1],3),glbGetOscParams(atest_values[1],5)};
		AddArrayToOutput(a,11);
	}
}

void FCMap_1D(double osc[], int proj, double xrange[]){
  //Compute FC map statistics for variations in osc, using projection proj, over xrange
  //Current only implemented for nocorr case

	int steps=100;
	int fixed[7];
	double testvals[7];
	double chi2[2];
	glb_params atest_values[2]; //store both best fit points for both fits
	atest_values[0]  = glbAllocParams();
	atest_values[1]  = glbAllocParams();

	//for(int i=0;i<7;i++) fixed[i]=(proj==i?GLB_FIXED:GLB_FREE);
	for(int i=0;i<7;i++) fixed[i]=(proj==i?GLB_FREE:GLB_FIXED);

	glb_projection projnp = glbAllocProjection();
	glbDefineProjection(projnp, fixed[0], fixed[1], fixed[2], fixed[3], fixed[4], fixed[5]);
	glbSetDensityProjectionFlag(projnp, fixed[6], GLB_ALL);
	glbSetProjection(projnp);


  //only need to set this once since it's not varied in the loop
	glbSetDensityParams(true_values,1.0,GLB_ALL);
  //setup parts arguments
  double pa1=xrange[0]; double pa2=xrange[1]; double pa3=(xrange[1]-xrange[0])/steps;
  for(double x=pstart(pa1,pa2,pa3);x<=pend(pa1,pa2,pa3);x=x+pinc(pa1,pa2,pa3)){

    for(int i=0;i<6;i++) testvals[i]=(proj==i?x:osc[i]);
    for(int pfluct=0;pfluct<=(arguments.pflucts>0?arguments.pflucts-1:0);pfluct++){ //loop for statistical fluctuations
        //set true values to this point in x and fluctuate
      	glbDefineParams(true_values,testvals[0],testvals[1],testvals[2],testvals[3],testvals[4],testvals[5]);
    		glbSetOscillationParameters(true_values);
	    	glbSetRates();
        if(arguments.pflucts>0) pfluctTrueSpectra();  

        //compute chisq fit (glbchinp)
			  glbDefineParams(atest_values[0],testvals[0],testvals[1],testvals[2],testvals[3],testvals[4],testvals[5]);
			  glbSetDensityParams(atest_values[0],1.0,GLB_ALL);
			  glbSetCentralValues(atest_values[0]);
			  chi2[0]=glbChiNP(atest_values[0],atest_values[0],GLB_ALL);

			  //compute chisq true (glbchisys)
			  glbDefineParams(atest_values[1],testvals[0],testvals[1],testvals[2],testvals[3],testvals[4],testvals[5]);
			  glbSetDensityParams(atest_values[1],1.0,GLB_ALL);
			  glbSetCentralValues(atest_values[1]);
			  chi2[1]=glbChiSys(atest_values[1],GLB_ALL,GLB_ALL);
			  
  		  double a[]={x,chi2[0], chi2[1], glbGetOscParams(atest_values[0],1),glbGetOscParams(atest_values[0],2),glbGetOscParams(atest_values[0],3),glbGetOscParams(atest_values[0],5),glbGetOscParams(atest_values[1],1),glbGetOscParams(atest_values[1],2),glbGetOscParams(atest_values[1],3),glbGetOscParams(atest_values[1],5)};
		  AddArrayToOutput(a,11);
	  }
  }
}

/*void FCMap_2D(double osc[], int projx, int projy, double xrange[], double vrange[]){
  //Compute FC map statistics for variations in osc, using projection proj, over xrange
  //Current only implemented for nocorr case

	int steps=100;
	int fixed[7];
	double testvals[7];
	double chi2[2];
	glb_params atest_values[2]; //store both best fit points for both fits
	atest_values[0]  = glbAllocParams();
	atest_values[1]  = glbAllocParams();

	for(int i=0;i<7;i++) fixed[i]=(projx==i||projy=i?GLB_FREE:GLB_FIXED);

	glb_projection projnp = glbAllocProjection();
	glbDefineProjection(projnp, fixed[0], fixed[1], fixed[2], fixed[3], fixed[4], fixed[5]);
	glbSetDensityProjectionFlag(projnp, fixed[6], GLB_ALL);
	glbSetProjection(projnp);


  //only need to set this once since it's not varied in the loop
	glbSetDensityParams(true_values,1.0,GLB_ALL);
  //setup parts arguments
  double pa1=xrange[0]; double pa2=xrange[1]; double pa3=(xrange[1]-xrange[0])/steps;
  for(double x=pstart(pa1,pa2,pa3);x<=pend(pa1,pa2,pa3);x=x+pinc(pa1,pa2,pa3)){

    for(int i=0;i<6;i++) testvals[i]=(proj==i?x:osc[i]);
    for(int pfluct=0;pfluct<=(arguments.pflucts>0?arguments.pflucts-1:0);pfluct++){ //loop for statistical fluctuations
        //set true values to this point in x and fluctuate
      	glbDefineParams(true_values,testvals[0],testvals[1],testvals[2],testvals[3],testvals[4],testvals[5]);
    		glbSetOscillationParameters(true_values);
	    	glbSetRates();
        if(arguments.pflucts>0) pfluctTrueSpectra();  

        //compute chisq fit (glbchinp)
			  glbDefineParams(atest_values[0],testvals[0],testvals[1],testvals[2],testvals[3],testvals[4],testvals[5]);
			  glbSetDensityParams(atest_values[0],1.0,GLB_ALL);
			  glbSetCentralValues(atest_values[0]);
			  chi2[0]=glbChiNP(atest_values[0],atest_values[0],GLB_ALL);

			  //compute chisq true (glbchisys)
			  glbDefineParams(atest_values[1],testvals[0],testvals[1],testvals[2],testvals[3],testvals[4],testvals[5]);
			  glbSetDensityParams(atest_values[1],1.0,GLB_ALL);
			  glbSetCentralValues(atest_values[1]);
			  chi2[1]=glbChiSys(atest_values[1],GLB_ALL,GLB_ALL);
			  
  		  double a[]={x,chi2[0], chi2[1], glbGetOscParams(atest_values[0],1),glbGetOscParams(atest_values[0],2),glbGetOscParams(atest_values[0],3),glbGetOscParams(atest_values[0],5),glbGetOscParams(atest_values[1],1),glbGetOscParams(atest_values[1],2),glbGetOscParams(atest_values[1],3),glbGetOscParams(atest_values[1],5)};
		  AddArrayToOutput(a,11);
	  }
  }
}*/
