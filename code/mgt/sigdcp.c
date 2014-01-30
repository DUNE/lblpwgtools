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
  int nuisances=arguments.systs*arguments.chimode==19?5:1;
  double narray[nuisances];
  for(double n=pstart(-4,4,inc);n<=pend(-4,4,inc);n=n+pinc(-4,4,inc)){
    for(int i=0; i<nuisances; i++){
      if(arguments.chimode==19)
        narray[i]=(i%5==0?n:0); //only modify the numu nuisance param.
      else  
        narray[i]=n;
    }
    for(int k=0;k<(truefitspectra==1?1:3);k++){
      glbDefineParams(test_values,osc[0],osc[1],osc[2],osc3[k],osc[4],osc[5]);
      
      //switch systematics off and call glbchisys so that event rates are set
      glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
      glbChiSys(test_values, GLB_ALL, GLB_ALL);
      glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_ON);
      if(arguments.chimode==19)
        chi2[k]=chi_ResponseFunctionCov(0, 0, nuisances, narray, NULL, NULL);
      else
        chi2[k]=chi_ResponseFunction(0, 0, nuisances, narray, NULL, NULL);
    }
    double a[]={n,chi2[0],chi2[1],chi2[2]};
    if(truefitspectra==0) AddArrayToOutput(a,4);
  }
}

void TrueFitSpectraMinOutput(double osc[]){
  //utility function for output true and fit energy spectra from a minization
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
          chi2[k]=glbChiDelta(test_values,NULL, GLB_ALL);
        }
      }
      chi2min=chi2[0];
      for(int k=0;k<opconsidered;k++){
        if(chi2[k]<chi2min){
          chi2min=chi2[k];
          mindcp=osc3[k];
          mindm=osc5[k];
        }
      }
      double a[4+5*glb_num_of_exps];
      a[0]=this_dcp;
      a[1]=(chi2min);
      a[2]=mindcp;
      a[3]=mindm;
      
      //get event rates for each experiment at these values
      double dcpvals[]={this_dcp,0,0,M_PI,M_PI};
      double dmvals[]={osc[5],osc[5],-osc[5]+osc[4],osc[5],-osc[5]+osc[4]};
      for(int exp=0;exp<glb_num_of_exps;exp++){
        for(int k=0;k<5;k++){
          glbDefineParams(test_values,osc[0],osc[1],osc[2],dcpvals[k],osc[4],dmvals[k]); 
          glbSetDensityParams(test_values,1.0,GLB_ALL);
          glbSetOscillationParameters(test_values);
          glbSetRates();
          a[4+5*exp+k]=glbTotalRuleRate(exp,0,GLB_ALL,GLB_W_EFF,GLB_W_BG,GLB_W_COEFF,GLB_SIG);
        }
      }
      AddArrayToOutput(a,4+5*glb_num_of_exps);
    }
  }

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

void ComputeMHSigCurve_v2(double osc[], double dcpin)
{
  //Compute MH sensitivity as a function of deltaCP.
	//This version computes mh sig without eightfold by prescanning
  //over deltaCP and using glbChiAll
  double this_dcp,this_tdcp;
  double tchi2,chi2min;
  double mindcp=0;
 	double osc5[]={osc[5],-osc[5]+osc[4]};	
  double minchi2s[2];
  
  for(this_dcp=pstart(-M_PI,M_PI,M_PI/18);this_dcp<=pend(-M_PI,M_PI,M_PI/18);this_dcp=this_dcp+pinc(-M_PI,M_PI,M_PI/18)){
    for(int pfluct=0;pfluct<=(arguments.pflucts>0?arguments.pflucts-1:0);pfluct++){ //loop for statistical fluctuations
      glbDefineParams(true_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc[5]);
      glbSetDensityParams(true_values,1.0,GLB_ALL);
      glbSetOscillationParameters(true_values);
      glbSetRates();
      if(arguments.pflucts>0) pfluctTrueSpectra();
      
      for(int i=0;i<2;i++){
        chi2min=1e8;
        if(dcpin!=0){
          glbDefineParams(test_values,osc[0],osc[1],osc[2],dcpin,osc[4],osc5[i]);  
          glbSetDensityParams(test_values,1.0,GLB_ALL);
          chi2min=glbChiSys(test_values,GLB_ALL,GLB_ALL);
          mindcp=dcpin;
        }else{
          for(this_tdcp=-M_PI;this_tdcp<M_PI;this_tdcp=this_tdcp+(M_PI/72)){
            glbDefineParams(test_values,osc[0],osc[1],osc[2],this_tdcp,osc[4],osc5[i]);  
            glbSetDensityParams(test_values,1.0,GLB_ALL);
            tchi2=glbChiSys(test_values,GLB_ALL,GLB_ALL);
            if(tchi2<chi2min){
              chi2min=tchi2;
              mindcp=this_tdcp;
            }
          }
        }
        
        if(arguments.test==NO){
          glbDefineParams(test_values,osc[0],osc[1],osc[2],mindcp,osc[4],osc5[i]);  
          glbSetDensityParams(test_values,1.0,GLB_ALL);
          glbSetCentralValues(test_values);
          chi2min=glbChiAll(test_values, test_values, GLB_ALL);
          mindcp=glbGetOscParams(test_values,GLB_DELTA_CP);
        }
        minchi2s[i]=chi2min;
      }
      
      //output false test first since that's what was plotted historically
      double a[]={this_dcp,minchi2s[1], minchi2s[0]};
      AddArrayToOutput(a,3);
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
			chi2min=chi2[0];
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
			
			chi2min=chi2[0];
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
	int dcpsteps=18;
	
	for(double this_dcp=-M_PI;this_dcp<M_PI;this_dcp=this_dcp+(M_PI/dcpsteps)){
		for(double thisE=erange[0]+estep;thisE<=erange[1];thisE+=estep){
			glbDefineParams(true_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc[5]);
			glbSetDensityParams(true_values,1.0,GLB_ALL);
			glbSetOscillationParameters(true_values);
			glbSetRates();
			double a[]={this_dcp,thisE,glbProfileProbability(0, 2, 1, +1, thisE),glbProfileProbability(0, 2, 1, -1, thisE),0.0,0.0,glbProfileProbability(0, 2, 2, +1, thisE),glbProfileProbability(0, 2, 2, -1, thisE),0.0,0.0,glbProfileProbability(0, 2, 3, +1, thisE),glbProfileProbability(0, 2, 3, -1, thisE),0.0,0.0};
			glbDefineParams(true_values,osc[0],osc[1],osc[2],this_dcp,osc[4],-osc[5]+osc[4]);
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
		AddToOutputBlankline();
	}

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
	double testvals[7];
	double chi2[2],mindcp[2],fitdcp[2];
	
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
		for(int i=0;i<6;i++) testvals[i]=(proj==i?x:osc[i]);
    double osc5[]={testvals[5],-testvals[5]+testvals[4]};
		for(int k=0;k<2;k++){
			if(dcpPreScan==1 && !arguments.test){
				//do prescan over delta cp
				double thischi=0.0;
				double minchi=1e6;
				for(double thisdcp=-M_PI;thisdcp<=M_PI;thisdcp+=M_PI/32){
					glbDefineParams(test_values,testvals[0],testvals[1],testvals[2],thisdcp,testvals[4],osc5[k]);
					glbSetDensityParams(test_values,1.0,GLB_ALL);
					thischi=glbChiSys(test_values, GLB_ALL,GLB_ALL);
					if(thischi<minchi){
						minchi=thischi;
						mindcp[k]=thisdcp;
					}
				}
			}else{
				mindcp[k]=testvals[3];
			}
			glbDefineParams(test_values,testvals[0],testvals[1],testvals[2],mindcp[k],testvals[4],osc5[k]);
			glbSetDensityParams(test_values,1.0,GLB_ALL);
			glbSetCentralValues(test_values);
			if(arguments.test){
				chi2[k]=glbChiSys(test_values, GLB_ALL,GLB_ALL);
			}else{
				chi2[k]=glbChiNP(test_values,test_values, GLB_ALL);
			}
			fitdcp[k]=glbGetOscParams(test_values,GLB_DELTA_CP);
		}

		double a[]={x,chi2[0], chi2[1], mindcp[0],fitdcp[0],mindcp[1],fitdcp[1],glbGetOscParams(test_values,0),glbGetOscParams(test_values,1),glbGetOscParams(test_values,2),glbGetOscParams(test_values,3),glbGetOscParams(test_values,4),glbGetOscParams(test_values,5)};
		AddArrayToOutput(a,13);
	}
}



