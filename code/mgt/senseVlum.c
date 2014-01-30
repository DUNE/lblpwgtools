/*based on example5.c from GLoBES examples*/
#include "senseVlum.h"

double DoChiSquareT13(double x, void *dummy)
{
  //Function for GSL to minimize for t13 resolution (fixed deltaCP)
  double thetheta13, chi2;
  
  /* Set vector of test values */
  thetheta13 = x;
  glbSetOscParams(test_values, thetheta13, GLB_THETA_13);

  if(arguments.test)
    chi2 = glbChiSys(test_values, GLB_ALL, GLB_ALL);
  else{
    chi2 = glbChiTheta13Delta(test_values,NULL , GLB_ALL);
  }

  return chi2 - arguments.chi2_goal;
}

double DoChiSquareCPRSVL(double x, void *dummy)
{
  //Function for GSL to minimize for CP resolution
  double chi2;
  
  /* Set vector of test values */
  glbSetOscParams(test_values, M_PI*x/180.0, GLB_DELTA_CP);

  if(arguments.test){
		chi2 = glbChiSys(test_values, GLB_ALL, GLB_ALL);
	}else{
		chi2 = glbChiDelta(test_values, NULL, GLB_ALL);
	}

  return chi2 - arguments.chi2_goal;
}

double DoChiSquareNP(double x, void *dummy)
{
  //Function for GSL to minimize for generic projection
  double chi2, tchi2;
  glbSetOscParams(test_values, x, arguments.scanVar);
  if(arguments.test){
		chi2 = glbChiSys(test_values, GLB_ALL, GLB_ALL);
	}else{
		//prescan over deltaCP if enabled
		if(arguments.preScan>0){
			double minchi2=1e8,mindcp=0;
			for(double this_dcp=-M_PI;this_dcp<M_PI;this_dcp=this_dcp+(M_PI/36)){
				glbSetOscParams(test_values, this_dcp, GLB_DELTA_CP);
				//systematics only
				tchi2=glbChiSys(test_values,GLB_ALL,GLB_ALL);
				if(tchi2<minchi2){
					minchi2=tchi2;
					mindcp=glbGetOscParams(test_values,GLB_DELTA_CP);
				}
			}
			//set to minimum dcp that was found
			glbSetOscParams(test_values, mindcp, GLB_DELTA_CP);
		}
		glbSetDensityParams(test_values,1.0,GLB_ALL);
		glbSetCentralValues(test_values);
		chi2 = glbChiNP(test_values,NULL,GLB_ALL);

		if(arguments.hier==YES){ //profile hierarchy if enabled
			double whchi2;
			//store old hierarchy
			double oldDM31=glbGetOscParams(test_values,GLB_DM_31);
			double DM21=glbGetOscParams(test_values,GLB_DM_21);
			glbSetOscParams(test_values, -oldDM31 + DM21, GLB_DM_31);
			//prescan over deltaCP if enabled
			if(arguments.preScan>0){
				double minchi2=1e8,mindcp=0;
				for(double this_dcp=-M_PI;this_dcp<M_PI;this_dcp=this_dcp+(M_PI/36)){
					glbSetOscParams(test_values, this_dcp, GLB_DELTA_CP);
					//systematics only
					tchi2=glbChiSys(test_values,GLB_ALL,GLB_ALL);
					if(tchi2<minchi2){
						minchi2=tchi2;
						mindcp=glbGetOscParams(test_values,GLB_DELTA_CP);
					}
				}
				//set to minimum dcp that was found
				glbSetOscParams(test_values, mindcp, GLB_DELTA_CP);
			}
			glbSetDensityParams(test_values,1.0,GLB_ALL);
			glbSetCentralValues(test_values);
			whchi2 = glbChiNP(test_values,NULL,GLB_ALL);
			if(whchi2<chi2) chi2=whchi2;
			//set the DM_31 back to other hierarchy
			glbSetOscParams(test_values, oldDM31, GLB_DM_31);
		}
	}
	
  return chi2 - arguments.chi2_goal;
}

void ComputeResolutionCurve(double osc[])
{
  //Compute deltachisq for all deltaCP values as a function exposure (fixed deltaCP)
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  double x_sens[arguments.tSteps+1];
  int this_dcp;
  int j,chi2count,exp;
  double chi2;
  
  for (j=0; j <= arguments.tSteps; j++)
    x_sens[j] = 1;

  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}

    chi2count=0;
    for(this_dcp=-180;this_dcp<180;this_dcp=this_dcp+10){
      /* Calculate "true" event rates */
      glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
      glbSetDensityParams(true_values,1.0,GLB_ALL);
      glbDefineParams(test_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc[5]);  
      glbSetDensityParams(test_values,1.0,GLB_ALL);
      glbSetOscillationParameters(true_values);
      glbSetRates();
      
      chi2=glbChiDelta(test_values,test_values, GLB_ALL);
      printf("%d\t%f\n",this_dcp,chi2);
      if(chi2>1.0){
          chi2count++;
      }
      
    }

    AddToOutput2(t, 36-chi2count);
  }
}

void ComputeCPResolutionCurveSVL(double osc[])
{
  //Compute resolution on deltaCP function of exposure
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  double x;               /* Current value of dcp */
  double x_lo, x_hi;      /* Bracketing interval for root finder */
  int gsl_status;         /* GSL error code */
  int iter;               /* Iteration counter for root finder */
  const int max_iter=100; /* Maximum number of iterations allowed in root finder */
  int i,j,exp;
  double xup,xdown;
  
  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				//printf("setting exp %d to t=%f\n",exp,t);
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}

    /* Calculate "true" event rates */
    glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
    glbSetDensityParams(true_values,1.0,GLB_ALL);
    glbDefineParams(test_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);  
    glbSetDensityParams(test_values,1.0,GLB_ALL);
    glbSetOscillationParameters(true_values);
    glbSetRates();

    /* Determine resolution on dcp by  using the GSL Brent-Dekker
     * algorithm to find the dcp, for which chi^2 = 1 (1 sigma) */
    x_lo = 180*osc[3]/M_PI;
    x_hi = x_lo+0.01;
    iter = 0;
   if(arguments.debug==YES) printf("starting root finder for t=%f\n",t);
   gsl_status = gsl_root_fsolver_set(s, &gsl_func, x_lo, x_hi);
    /* Start root finder. The initial search interval is guessed based on the
     * results from a neighboring grid point */
    double deviation=1;
    do {
        x_lo = x_lo;
        x_hi = x_hi + deviation;
      if(arguments.debug==YES) printf("\tsetting initial search interval x_lo:%f, x_hi:%f\n",x_lo,x_hi);
      gsl_status = gsl_root_fsolver_set(s, &gsl_func, x_lo, x_hi);
      deviation *= 2.0;
    } while (gsl_status != GSL_SUCCESS && x_hi<180 && x_hi>-180);
    
    /* Iterate root finder */
    do {
      iter++;
      gsl_status = gsl_root_fsolver_iterate(s);
      x          = gsl_root_fsolver_root(s);
      x_lo       = gsl_root_fsolver_x_lower(s);
      x_hi       = gsl_root_fsolver_x_upper(s);
      if(arguments.debug==YES) printf("\tx_lo:%f, x_hi:%f, x:%f\n",x_lo,x_hi,x);
      gsl_status = gsl_root_test_interval (x_lo, x_hi, 0.01, 0);
    } while (gsl_status == GSL_CONTINUE && iter < max_iter);
    if(arguments.debug==YES) printf("found minimum on iter %d at %f\n",iter,x);
    //store this upward going value
    xup=x;
    
    //do again but going down in dcp
    x_hi = 180*osc[3]/M_PI;
    x_lo = x_hi-0.01;
    
    iter = 0;
   if(arguments.debug==YES) printf("starting root finder for t=%f\n",t);
   gsl_status = gsl_root_fsolver_set(s, &gsl_func, x_lo, x_hi);
    /* Start root finder. The initial search interval is guessed based on the
     * results from a neighboring grid point */
    do{
      x_hi = x_hi;
      x_lo = x_lo-deviation;
      if(arguments.debug==YES) printf("\tsetting initial search interval x_lo:%f, x_hi:%f\n",x_lo,x_hi);
      gsl_status = gsl_root_fsolver_set(s, &gsl_func, x_lo, x_hi);
    } while (gsl_status != GSL_SUCCESS && x_lo>-360);
    
    /* Iterate root finder */
    do {
      iter++;
      gsl_status = gsl_root_fsolver_iterate(s);
      x          = gsl_root_fsolver_root(s);
      x_lo       = gsl_root_fsolver_x_lower(s);
      x_hi       = gsl_root_fsolver_x_upper(s);
      if(arguments.debug==YES) printf("\tx_lo:%f, x_hi:%f, x:%f\n",x_lo,x_hi,x);
      gsl_status = gsl_root_test_interval (x_lo, x_hi, 0.01, 0);
    } while (gsl_status == GSL_CONTINUE && iter < max_iter);
    if(arguments.debug==YES) printf("found minimum on iter %d at %f\n",iter,x);
    //store this upward going value
    xdown=x;  
    
    /* Save results */
    /*compute exposure using mass of each experiment and time*/
    float exposure=0;
    for(i=0;i<glb_num_of_exps;i++){
			exposure=exposure+glbGetTargetMass(i)*glbGetRunningTime(i, 0);
		}
    double a[]={exposure,0.5*(xup-180*osc[3]/M_PI - (xdown-180*osc[3]/M_PI)), xup, xdown};
    AddArrayToOutput(a,4);

  }
}

void ComputeNonZeroT13(double osc[])
{
  //Compute sensitivity to non-zero t13 as a function of exposure (fixed deltaCP)
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  int i,j,k,exp;
  double chi2[2];
  
  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}
		double osc5[]={osc[5],-osc[5]+osc[4]};
		
		glbDefineParams(test_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);  
		glbSetDensityParams(test_values,1.0,GLB_ALL);
		for(k=0;k<2;k++){
			glbDefineParams(true_values,osc[0],0,osc[2],osc[3],osc[4],osc5[k]);
			glbSetDensityParams(true_values,1.0,GLB_ALL);
			glbSetOscillationParameters(true_values);
			glbSetRates();
			if(arguments.test){
				chi2[k]=glbChiSys(test_values, GLB_ALL,GLB_ALL);
			}else{
				chi2[k]=glbChiTheta13Delta(test_values,test_values, GLB_ALL);
			}
		}
			
	/* Save results */
    /*compute exposure using mass of each experiment and time*/
    float exposure=0;
    for(i=0;i<glb_num_of_exps;i++){
			exposure=exposure+glbGetTargetMass(i)*glbGetRunningTime(i, 0);
		}
    double a[]={exposure,chi2[0],chi2[1]};
    AddArrayToOutput(a,3);
  }
}

void ComputeCPVSigFracCurve(double osc[])
{
  //Compute CPV sensitivity as a function of exposure
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  int i,j,k,exp;
	int dcpsteps=100;
  double chis[dcpsteps];
  double chi2[4];
  double chi2min, sig25, sig50, sig75, frac2sig, frac3sig, frac5sig;
  double this_dcp;
  int opconsidered = (arguments.hier==YES ? 4 : 2);
	double osc5[]={osc[5],osc[5],-osc[5]+osc[4],-osc[5]+osc[4]};
	double osc3[]={0,M_PI,0,M_PI};
			  
  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				//printf("setting exp %d to t=%f\n",exp,t);
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}
			
		for(i=0;i<dcpsteps;i++){
			this_dcp=-M_PI+(((float)i/(float)dcpsteps)*(2*M_PI));
			glbDefineParams(true_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc[5]);
			glbSetOscillationParameters(true_values);
			glbSetRates();
			 
			for(k=0;k<opconsidered;k++){
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
			for(k=0;k<opconsidered;k++){
				if(chi2[k]<chi2min) chi2min=chi2[k];
			}
			chis[i]=chi2min;
		}

		/*sort the chis array in descending order*/
		qsort(chis,dcpsteps,sizeof(double),compare);

		/*find the value for which 25% of the entries are less than*/
		sig25=sig50=sig75=0.0;
		for(i=0;i<dcpsteps;i++){
			if(((float)i/(float)dcpsteps)>0.25 && sig25==0.0) sig25=sqrt(chis[i]);
			if(((float)i/(float)dcpsteps)>0.50 && sig50==0.0) sig50=sqrt(chis[i]);
			if(((float)i/(float)dcpsteps)>0.75 && sig75==0.0) sig75=sqrt(chis[i]);
		}
		
		/*find the dcp fractions at which 2, 3, and 5 sigma occur (4,9,25)*/
		frac2sig=frac3sig=frac5sig=-0.01;
		for(i=0;i<dcpsteps;i++){
			if(chis[i]<4.0 && frac2sig==-0.01) frac2sig=(float)i/(float)dcpsteps;
			if(chis[i]<9.0 && frac3sig==-0.01) frac3sig=(float)i/(float)dcpsteps;
			if(chis[i]<25.0 && frac5sig==-0.01) frac5sig=(float)i/(float)dcpsteps;
		}	

    /* Save results */
    /*compute exposure using mass of each experiment and time*/
    float exposure=0;
    for(i=0;i<glb_num_of_exps;i++){
			exposure=exposure+glbGetTargetMass(i)*glbGetRunningTime(i, 0);
		}
    double a[]={exposure,sig25,sig50,sig75,frac2sig,frac3sig,frac5sig};
    AddArrayToOutput(a,7);
  }
}

void ComputeCPBubblesCurve(double osc[])
{
  //Compute deltachisq for all deltaCP values as a function of exposure
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  int i,j,k,exp;
	int dcpsteps=100;
  double chis[dcpsteps];
  double chi2[4];
  double chi2min;
  double this_dcp;
  int opconsidered = (arguments.hier==YES ? 2 : 1);
  
  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}
		double osc5[]={osc[5],-osc[5]+osc[4]};
		for(i=0;i<dcpsteps;i++){
			this_dcp=-M_PI+(((float)i/(float)dcpsteps)*(2*M_PI));
			glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
			glbSetOscillationParameters(true_values);
			glbSetRates();			 
		
			for(k=0;k<opconsidered;k++){
				glbDefineParams(test_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc5[k]); 
				glbSetDensityParams(test_values,1.0,GLB_ALL);
				if(arguments.test){
					chi2[k]=glbChiSys(test_values, GLB_ALL,GLB_ALL);
				}else{
					glbSetCentralValues(test_values);
					chi2[k]=glbChiDelta(test_values,NULL, GLB_ALL);
				}
			}
			
			chi2min=chi2[0];
			for(k=0;k<opconsidered;k++){
				if(chi2[k]<chi2min) chi2min=chi2[k];
			}
			chis[i]=chi2min;
		
			/* Save results */
			/*compute exposure using mass of each experiment and time*/
			float exposure=0;
			for(int k=0;k<glb_num_of_exps;k++){
				exposure=exposure+glbGetTargetMass(k)*glbGetRunningTime(k, 0);
			}
			double a[]={exposure,this_dcp, chi2min};
			AddArrayToOutput(a,3);
		}
		/*add blank line between sets*/
		AddToOutputBlankline();
  }
}

void ComputeOctCurve(double osc[])
{
  //Compute octant sensitivity as a function of exposure
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  int j,k,exp;
  int t23steps=40;
  double chi2[4];
  double chi2min;
  double this_t23;
  double t23min=0.580;
  double t23max=0.991;
  int opconsidered = (arguments.hier==YES ? 2 : 1);

  glb_projection proj= glbAllocProjection();
  glbDefineProjection(proj, GLB_FIXED, GLB_FREE, GLB_FREE, GLB_FREE, GLB_FIXED, GLB_FREE);
  glbSetDensityProjectionFlag(proj, GLB_FIXED, GLB_ALL);
  glbSetProjection(proj);
  //use prior on t23 that constrains the fit to opposite octant
  glbRegisterPriorFunction(my_prior_t23_opoct,NULL,NULL,NULL);
  
  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}
		for(this_t23=t23min;this_t23<=t23max;this_t23+=(t23max-t23min)/t23steps){
			glbDefineParams(true_values,osc[0],osc[1],this_t23,osc[3],osc[4],osc[5]);
			glbSetDensityParams(true_values,1.0,GLB_ALL);
			glbSetOscillationParameters(true_values);
			glbSetRates();

			double osc5[]={osc[5],-osc[5]+osc[4]};
			
			for(k=0;k<opconsidered;k++){
				glbDefineParams(test_values,osc[0],osc[1],(M_PI/2)-this_t23,osc[3],osc[4],osc5[k]);  
				glbSetDensityParams(test_values,1.0,GLB_ALL);
				if(arguments.test){
					chi2[k]=glbChiSys(test_values, GLB_ALL,GLB_ALL);
				}else{
					//prescan over delta cp
					double minchi2=1e8,mindcp=0;double tchi2=0;
					for(double this_dcp=-M_PI;this_dcp<M_PI;this_dcp=this_dcp+(M_PI/80)){
						glbSetOscParams(test_values, this_dcp, GLB_DELTA_CP);
						//systematics only
						tchi2=glbChiSys(test_values,GLB_ALL,GLB_ALL);
						if(tchi2<minchi2){
							minchi2=tchi2;
							mindcp=glbGetOscParams(test_values,GLB_DELTA_CP);
						}
					}
					//set to found delta CP minimum
					glbSetOscParams(test_values, mindcp, GLB_DELTA_CP);
					glbSetCentralValues(test_values); 
					chi2[k]=glbChiNP(test_values,NULL, GLB_ALL);
				}
			}
			
			chi2min=chi2[0];
			for(k=0;k<opconsidered;k++){
				if(chi2[k]<chi2min) chi2min=chi2[k];
			}

			/* Save results */
			/*compute exposure using mass of each experiment and time*/
			float exposure=0;
			for(int k=0;k<glb_num_of_exps;k++){
				exposure=exposure+glbGetTargetMass(k)*glbGetRunningTime(k, 0);
			}
			double a[]={exposure,this_t23, chi2min};
			AddArrayToOutput(a,3);
		}
		/*add blank line between sets*/
		AddToOutputBlankline();
  }
}

void ComputeNMQCurve(double osc[])
{
  //Compute non-maximal t23 sensitivity as a function of exposure
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  int j,k,exp;
  int t23steps=40;
  double chi2[4];
  double chi2min;
  double this_t23;
  double t23min=0.580;
  double t23max=0.991;
  int opconsidered = (arguments.hier==YES ? 2 : 1);

  glb_projection proj= glbAllocProjection();
  glbDefineProjection(proj, GLB_FIXED, GLB_FREE, GLB_FIXED, GLB_FREE, GLB_FIXED, GLB_FREE);
  glbSetDensityProjectionFlag(proj, GLB_FIXED, GLB_ALL);
  glbSetProjection(proj);
  //use prior on t23 that constrains the fit to opposite octant
  glbRegisterPriorFunction(my_prior_t23_opoct,NULL,NULL,NULL);
  
  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}
		for(this_t23=t23min;this_t23<=t23max;this_t23+=(t23max-t23min)/t23steps){
			glbDefineParams(true_values,osc[0],osc[1],this_t23,osc[3],osc[4],osc[5]);
			glbSetDensityParams(true_values,1.0,GLB_ALL);
			glbSetOscillationParameters(true_values);
			glbSetRates();

			double osc5[]={osc[5],-osc[5]+osc[4]};
			
			for(k=0;k<opconsidered;k++){
				glbDefineParams(test_values,osc[0],osc[1],(M_PI/4),osc[3],osc[4],osc5[k]);
				glbSetDensityParams(test_values,1.0,GLB_ALL);
				if(arguments.test){
					chi2[k]=glbChiSys(test_values, GLB_ALL,GLB_ALL);
				}else{
					//prescan over delta cp
					double minchi2=1e8,mindcp=0;double tchi2=0;
					for(double this_dcp=-M_PI;this_dcp<M_PI;this_dcp=this_dcp+(M_PI/36)){
						glbSetOscParams(test_values, this_dcp, GLB_DELTA_CP);
						//systematics only
						tchi2=glbChiSys(test_values,GLB_ALL,GLB_ALL);
						if(tchi2<minchi2){
							minchi2=tchi2;
							mindcp=glbGetOscParams(test_values,GLB_DELTA_CP);
						}
					}
					//set to found delta CP minimum
					glbSetOscParams(test_values, mindcp, GLB_DELTA_CP);
					glbSetCentralValues(test_values); 
					chi2[k]=glbChiNP(test_values,NULL, GLB_ALL);
				}
			}
			
			chi2min=chi2[0];
			for(k=0;k<opconsidered;k++){
				if(chi2[k]<chi2min) chi2min=chi2[k];
			}

			/* Save results */
			/*compute exposure using mass of each experiment and time*/
			float exposure=0;
			for(int k=0;k<glb_num_of_exps;k++){
				exposure=exposure+glbGetTargetMass(k)*glbGetRunningTime(k, 0);
			}
			double a[]={exposure,this_t23, chi2min};
			AddArrayToOutput(a,3);
		}
		/*add blank line between sets*/
		AddToOutputBlankline();
  }
}

void ComputeCPResolutionCurve2SVL(double osc[])
{
	//instead of fitting for 1 sigma, this version scans over all dcp
	//values, comparing to the input osc[3], and outputs the fraction
	//of dcp values greater than 1,2,3,5 sigma
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  int i,j,k,exp;
  int dcpsteps=360;
  double chis[dcpsteps];
  double chi2[4];
  double chi2min;
  double this_dcp;
  int opconsidered = (arguments.hier==YES ? 2 : 1);
  
  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}
		glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
		glbSetOscillationParameters(true_values);
		glbSetRates();			
		double osc5[]={osc[5],-osc[5]+osc[4]};  
		for(i=0;i<dcpsteps;i++){
			this_dcp=-M_PI+(((float)i/(float)dcpsteps)*(2*M_PI));
			for(k=0;k<opconsidered;k++){
				glbDefineParams(test_values,osc[0],osc[1],osc[2],this_dcp,osc[4],osc5[k]);
				glbSetDensityParams(test_values,1.0,GLB_ALL);
				if(arguments.test){
					chi2[k]=glbChiSys(test_values, GLB_ALL,GLB_ALL);
				}else{
					glbSetCentralValues(test_values);
					chi2[k]=glbChiDelta(test_values,NULL, GLB_ALL);
				}
			}
			
			chi2min=chi2[0];
			for(k=0;k<opconsidered;k++){
				if(chi2[k]<chi2min) chi2min=chi2[k];
			}
			chis[i]=chi2min;
		

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

		/* Save results */
		/*compute exposure using mass of each experiment and time*/
		float exposure=0;
		for(i=0;i<glb_num_of_exps;i++){
			exposure=exposure+glbGetTargetMass(i)*glbGetRunningTime(i, 0);
		}
		double a[]={exposure,frac1sig,frac2sig,frac3sig,frac5sig};
		AddArrayToOutput(a,5);
  }
}

void ComputeMHSigCurve_glbchiAll(double osc[])
{
  //Compute MH sensitivity as a function of exposure (using glbchiAll)
  //TESTING ONLY
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  int i,j,exp;
  
  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}

		glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
		glbSetDensityParams(true_values,1.0,GLB_ALL);
		glbSetOscillationParameters(true_values);
		glbSetCentralValues(true_values);
		glbSetRates();
		
		glbDefineParams(test_values,osc[0],osc[1],osc[2],osc[3],osc[4],-osc[5]+osc[4]);  
		glbSetCentralValues(test_values);
		glbSetDensityParams(test_values,1.0,GLB_ALL);

		double chi2  = 1.0e10;
		if(arguments.test){
			chi2=glbChiSys(test_values, GLB_ALL,GLB_ALL);
		}else{
			chi2=glbChiAll(test_values,test_values, GLB_ALL);
		}
		
   /* Save results */
    /*compute exposure using mass of each experiment and time*/
    float exposure=0;
    for(i=0;i<glb_num_of_exps;i++){
			exposure=exposure+glbGetTargetMass(i)*glbGetRunningTime(i, 0);
		}
    double a[]={exposure,chi2};
    AddArrayToOutput(a,2);
  }
}

void ComputeMHSigCurveSVL(double osc[])
{
  //Compute MH sensitivity as a function of exposure (using eightfold)
  //Compute for dcp=-90,0, and 90
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  int i,j,k,exp;
  double chi2s[3];
  double t13s[3],dcps[3];
  
  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}

		glbDefineParams(test_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);  
		glbSetDensityParams(test_values,1.0,GLB_ALL);

		double osc3[]={-M_PI/2,0,M_PI/2};
		
		for(k=0;k<3;k++){
			glbDefineParams(true_values,osc[0],osc[1],osc[2],osc3[k],osc[4],osc[5]);
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
				if (fchi[i] < chi2){
					chi2 = fchi[i];
					Fit  = listb[i];
				}
			}
			
			chi2s[k]=chi2;
			t13s[k]=glbGetOscParams(Fit,GLB_THETA_13);
			dcps[k]=glbGetOscParams(Fit,GLB_DELTA_CP);
		}
		
    /* Save results */
    /*compute exposure using mass of each experiment and time*/
    float exposure=0;
    for(i=0;i<glb_num_of_exps;i++){
			exposure=exposure+glbGetTargetMass(i)*glbGetRunningTime(i, 0);
		}
    double a[]={exposure,sqrt(chi2s[0]),sqrt(chi2s[1]),sqrt(chi2s[2]),t13s[0],t13s[1],t13s[2],dcps[0],dcps[1],dcps[2]};
    AddArrayToOutput(a,10);
  }
}

void ComputeMHSigCurve_bruteForce(double osc[])
{
  //Compute MH sensitivity as a function of exposure (using glbchiAll)
  //Compute for all dcp values
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  int i,j,k,exp;
  const int dcpsteps=32;
  double chi2s[dcpsteps],chi2smax[dcpsteps];
  double t13s[dcpsteps],dcps[dcpsteps],dm31s[dcpsteps];
  double t13smax[dcpsteps],dcpsmax[dcpsteps],dm31smax[dcpsteps];
  
  //populate osc3 with dcp values
  double osc3[dcpsteps];
	for(i=0;i<dcpsteps;i++){
		osc3[i]=-M_PI+(((float)i/(float)dcpsteps)*(2*M_PI));
	}

  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}

		
		for(k=0;k<dcpsteps;k++){
			glbDefineParams(true_values,osc[0],osc[1],osc[2],osc3[k],osc[4],osc[5]);
			glbSetDensityParams(true_values,1.0,GLB_ALL);
			glbSetOscillationParameters(true_values);
			glbSetRates();
			
			//find degeneracies
			double xrange[]={-4,-0.1};
			double yrange[]={0,360};
			int resolution[]={25,25};
			glb_params *listb, Fit = NULL,Fitmax = NULL;
			double *fchi;
			int ct,i;
			double chi2  = 1.0e10;
			double chi2max = 0;
			ct=eightfold(true_values,input_errors,YES,NO,xrange,yrange,resolution,&listb,&fchi);
			
			for(i=0; i < ct; i++){
				if (fchi[i] < chi2){
					chi2 = fchi[i];
					Fit  = listb[i];
				}
				if (fchi[i] > chi2max){
					chi2max = fchi[i];
					Fitmax  = listb[i];
				}
			}
			
			chi2s[k]=chi2;
			t13s[k]=glbGetOscParams(Fit,GLB_THETA_13);
			dcps[k]=glbGetOscParams(Fit,GLB_DELTA_CP);
			dm31s[k]=glbGetOscParams(Fit,GLB_DM_31);
			chi2smax[k]=chi2max;
			t13smax[k]=glbGetOscParams(Fitmax,GLB_THETA_13);
			dcpsmax[k]=glbGetOscParams(Fitmax,GLB_DELTA_CP);
			dm31smax[k]=glbGetOscParams(Fitmax,GLB_DM_31);
		}
		
    /* Save results */
    /*compute exposure using mass of each experiment and time*/
    float exposure=0;
    for(i=0;i<glb_num_of_exps;i++){
			exposure=exposure+glbGetTargetMass(i)*glbGetRunningTime(i, 0);
		}
		
		//find min,max in chi2s array
		double chi2min  = 1.0e10;
		double chi2max  = 0;
		double t13min=0,dcpmin=0,dm31min=0;
		double t13max=0,dcpmax=0,dm31max=0;
		for(i=0;i<dcpsteps;i++){
			if(chi2s[i]<chi2min){
				chi2min=chi2s[i];
				t13min=t13s[i];
				dcpmin=dcps[i];
				dm31min=dm31s[i];
			}
			if(chi2smax[i]>chi2max){
				chi2max=chi2smax[i];
				t13max=t13smax[i];
				dcpmax=dcpsmax[i];
				dm31max=dm31smax[i];
			}
		}
    //now sort the list of chi2s and find the cp fraction for which we can determine the MH to 2,3, and 5 sigma
    qsort(chi2s,dcpsteps,sizeof(double),compare);
    /*find the value for which 25% of the entries are less than*/
		double sig25=0.0,sig50=0.0,sig75=0.0;
    double frac2sig, frac3sig, frac5sig;
		frac2sig=frac3sig=frac5sig=1.00;
		for(i=0;i<dcpsteps;i++){
			if(((float)i/(float)dcpsteps)>0.25 && sig25==0.0) sig25=sqrt(chi2s[i]);
			if(((float)i/(float)dcpsteps)>0.50 && sig50==0.0) sig50=sqrt(chi2s[i]);
			if(((float)i/(float)dcpsteps)>0.75 && sig75==0.0) sig75=sqrt(chi2s[i]);
			
			if(chi2s[i]<4.0 && frac2sig==1.00) frac2sig=(float)i/(float)dcpsteps;
			if(chi2s[i]<9.0 && frac3sig==1.00) frac3sig=(float)i/(float)dcpsteps;
			if(chi2s[i]<25.0 && frac5sig==1.00) frac5sig=(float)i/(float)dcpsteps;
		}   
    double a[]={exposure,sqrt(chi2min),sqrt(chi2max),t13min,dcpmin,dm31min,t13max,dcpmax,dm31max,frac2sig,frac3sig,frac5sig,sig25,sig50,sig75};
    AddArrayToOutput(a,15);
  }
}


void ComputeMHSensitivityCurve(double osc[])
{
  //Compute smallest value of t13 for which MH can be determined as a function of exposure
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  double x;               /* Current value of log[sin^2(2*th13)] */
  double x_lo, x_hi;      /* Bracketing interval for root finder */
  double x_sens[arguments.tSteps+1];/* Calculated sensitivities to log[sin^2(2*th13)] */
  int gsl_status;         /* GSL error code */
  int iter;               /* Iteration counter for root finder */
  const int max_iter=100; /* Maximum number of iterations allowed in root finder */
  int j,exp;
  
  for (j=0; j <= arguments.tSteps; j++)      /* Initialize output vector */
    x_sens[j] = 1;                 /* As x is always <= 0 this signals "no value" */

  
  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}
  
    /* Determine sensitivity mh by  using the GSL Brent-Dekker
     * algorithm to find the th13, for which chi^2 = xxx */
    x_lo = -0.9;
    x_hi = -0.9;
    iter = 0;
   
   if(arguments.debug==YES) printf("starting root finder for t=%f\n",t);
    /* Start root finder. The initial search interval is guessed based on the
     * results from a neighboring grid point */
    double deviation=0.01;
    do {
      if (j > 0) {
        //grow range considered
        x_lo = x_sens[j-1] - deviation;
        x_hi = min(x_sens[j-1], -0.001);
      }else{
        //shrink range considered
        x_lo=x_lo-0.01;
        x_hi=x_hi+0.01;
      }
      if(arguments.debug==YES) printf("\tsetting initial search interval x_lo:%f, x_hi:%f\n",x_lo,x_hi);
      gsl_status = gsl_root_fsolver_set(s, &gsl_func, x_lo, x_hi);
      deviation += 0.02;
    } while (gsl_status != GSL_SUCCESS);
    
    /* Iterate root finder */
    do {
      iter++;
      gsl_status = gsl_root_fsolver_iterate(s);
      x          = gsl_root_fsolver_root(s);
      x_lo       = gsl_root_fsolver_x_lower(s);
      x_hi       = gsl_root_fsolver_x_upper(s);
      if(arguments.debug==YES) printf("\tx_lo:%f, x_hi:%f, x:%f\n",x_lo,x_hi,x);
      gsl_status = gsl_root_test_interval (x_lo, x_hi, arguments.logs22th13_precision, 0);
    } while (gsl_status == GSL_CONTINUE && iter < max_iter);
    if(arguments.debug==YES) printf("found minimum on iter %d at %f\n",iter,x);
    /* Save results */
    x_sens[j] = x;
    AddToOutput2(t, x);
  }
}

void ComputeNPResCurve(double osc[], double in_xup_hi, double deviation, double in_xdown_lo)
{
  //Compute resolution on arguments.scanVar using defined projection as a function of exposure
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  double xup,xdown;               /* Current value of scanVar */
  double xup_lo, xup_hi=osc[arguments.scanVar]+deviation*osc[arguments.scanVar], xdown_lo=osc[arguments.scanVar]-deviation*osc[arguments.scanVar], xdown_hi;      /* Bracketing interval for root finder */
  int gsl_status;         /* GSL error code */
  int iter;               /* Iteration counter for root finder */
  const int max_iter=100; /* Maximum number of iterations allowed in root finder */
  int j,exp;
  int up_foundaroot=0, down_foundaroot=0;
  gsl_set_error_handler_off();

  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0){
			//printf("setting all exps to t=%f\n",t);
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		}else{
			for(exp=0;exp<arguments.varied;exp++){
				//printf("setting exp %d to t=%f\n",exp,t);
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}

    /* Calculate "true" event rates */
    glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
    glbSetDensityParams(true_values,1.0,GLB_ALL);
    glbDefineParams(test_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);  
    glbSetDensityParams(test_values,1.0,GLB_ALL);
    glbSetOscillationParameters(true_values);
    glbSetRates();
    
			/* Determine sensitivity to scanVar by  using the GSL Brent-Dekker
			 * algorithm to find the th13, for which chi^2 = 1*/
		//double deviation=0.01;
		
		//search for upper bound
		xup_lo=osc[arguments.scanVar];
		if(j<=0 || xup_hi==in_xup_hi){
				if(osc[arguments.scanVar]>0.0){
					xup_hi = osc[arguments.scanVar]+deviation*osc[arguments.scanVar];
				}else{
					xup_hi=osc[arguments.scanVar]+deviation;
				}
		}
    iter = 0;
   
		if(arguments.debug==YES) printf("starting root finder for t=%f\n",t);
    do {
      if(arguments.debug==YES) printf("\tsetting initial search interval xup_lo:%f, xup_hi:%f\n",xup_lo,xup_hi);
			if(xup_hi<xup_lo){
				up_foundaroot=0;
			}
			if(xup_hi > in_xup_hi){
				break;
			}
      gsl_status = gsl_root_fsolver_set(s, &gsl_func, xup_lo, xup_hi);
			if(j>0 && up_foundaroot==1){
				xup_hi -= (osc[arguments.scanVar]>0.0?deviation*osc[arguments.scanVar]:deviation);
			}else{
				xup_hi += (osc[arguments.scanVar]>0.0?deviation*osc[arguments.scanVar]:deviation);
			}	
			//deviation *= 2;
		} while (gsl_status != GSL_SUCCESS);

    /* Iterate root finder */
	if(xup_hi<= in_xup_hi){
		do {
		  iter++;
		  gsl_status = gsl_root_fsolver_iterate(s);
		  xup          = gsl_root_fsolver_root(s);
		  xup_lo       = gsl_root_fsolver_x_lower(s);
		  xup_hi       = gsl_root_fsolver_x_upper(s);
		  if(arguments.debug==YES) printf("\txup_lo:%f, xup_hi:%f, xup:%f\n",xup_lo,xup_hi,xup);
		  gsl_status = gsl_root_test_interval (xup_lo, xup_hi, arguments.logs22th13_precision, 0);
		} while (gsl_status == GSL_CONTINUE && iter < max_iter);
		if(arguments.debug==YES) printf("found minimum on iter %d at %f\n",iter,xup);
		up_foundaroot=1;
	}else{
		xup=in_xup_hi;
		xup_hi=in_xup_hi;
	}
	
	
	//do again, but for the lower bound
	xdown_hi=osc[arguments.scanVar];
	if(down_foundaroot==0){
		if(osc[arguments.scanVar]==0){
			xdown_lo=osc[arguments.scanVar]-deviation;
		}else{
			xdown_lo = osc[arguments.scanVar]-deviation*osc[arguments.scanVar];
		}
	}
    iter = 0;
   
   if(arguments.debug==YES) printf("starting root finder for t=%f\n",t);
    do {
			if(arguments.debug==YES) printf("\tsetting initial search interval xdown_lo:%f, xdown_hi:%f\n",xdown_lo,xdown_hi);
			if(xdown_lo>xdown_hi){
				down_foundaroot=0;
			}
			if(xdown_lo <= in_xdown_lo){
				break;
			}
				gsl_status = gsl_root_fsolver_set(s, &gsl_func, xdown_lo, xdown_hi);
			if(j>0 && down_foundaroot==1){
				xdown_lo += (osc[arguments.scanVar]>0.0?deviation*osc[arguments.scanVar]:deviation);
			}else{
				xdown_lo -= (osc[arguments.scanVar]>0.0?deviation*osc[arguments.scanVar]:deviation);
			}	
			//deviation *=2;
    } while (gsl_status != GSL_SUCCESS);

    /* Iterate root finder */
	if(xdown_lo>in_xdown_lo && xdown_lo<xdown_hi){
		do {
		  iter++;
		  gsl_status = gsl_root_fsolver_iterate(s);
		  xdown          = gsl_root_fsolver_root(s);
		  xdown_lo       = gsl_root_fsolver_x_lower(s);
		  xdown_hi       = gsl_root_fsolver_x_upper(s);
		  if(arguments.debug==YES) printf("\txdown_lo:%f, xdown_hi:%f, xdown:%f\n",xdown_lo,xdown_hi,xdown);
		  gsl_status = gsl_root_test_interval (xdown_lo, xdown_hi, arguments.logs22th13_precision, 0);
		} while (gsl_status == GSL_CONTINUE && iter < max_iter);
		if(arguments.debug==YES) printf("found minimum on iter %d at %f\n",iter,xdown);
		down_foundaroot=1;
	}else{
	  xdown=in_xdown_lo;
	  xdown_lo=in_xdown_lo;
	}
	
	float exposure=0;
	for(int i=0;i<glb_num_of_exps;i++){
		exposure=exposure+glbGetTargetMass(i)*glbGetRunningTime(i, 0);
	}
	double a[]={exposure,xup,osc[arguments.scanVar],xdown};
    AddArrayToOutput(a,4);
  }
}

void ComputeT13SensitivityCurve(double osc[])
{
  //Compute resolution on t12 as a function of exposure
  double t_factor = pow(arguments.max_runtime/arguments.min_runtime, 1.0/arguments.tSteps);
  double t;
  double xup,xdown;               /* Current value of log[sin^2(2*th13)] */
  double xup_lo, xup_hi=osc[1]+0.01, xdown_lo=osc[1]-0.01, xdown_hi;      /* Bracketing interval for root finder */
  int gsl_status;         /* GSL error code */
  int iter;               /* Iteration counter for root finder */
  const int max_iter=100; /* Maximum number of iterations allowed in root finder */
  int j,exp;
  int up_foundaroot=0, down_foundaroot=0;
  

  /* Loop over all data points in this dataset, using log stepping */
  for (j=(arguments.zero==YES ? -1 : 0); j <= arguments.tSteps; j++){
    /* Set running times */
    t = (j==-1) ? 0.00001 : arguments.min_runtime * pow(t_factor,j);
    if(arguments.varied==0)
			glbSetRunningTime(GLB_ALL, GLB_ALL, t);
		else{
			for(exp=0;exp<arguments.varied;exp++){
				//printf("setting exp %d to t=%f\n",exp,t);
				glbSetRunningTime(exp, GLB_ALL, t);
			}
		}

    /* Calculate "true" event rates */
    glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
    glbSetDensityParams(true_values,1.0,GLB_ALL);
    glbDefineParams(test_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);  
    glbSetDensityParams(test_values,1.0,GLB_ALL);
    glbSetOscillationParameters(true_values);
    glbSetRates();
    
    /* Determine sensitivity to sin^2(2*th13) by  using the GSL Brent-Dekker
     * algorithm to find the th13, for which chi^2 = 1*/
	double deviation=0.01;
	
	//search for upper bound
	xup_lo=osc[1];
	if(j<=0 || xup_hi==M_PI/4){
      xup_hi = osc[1]+0.01;
	}
    iter = 0;
   
   if(arguments.debug==YES) printf("starting root finder for t=%f\n",t);
    do {
      if(arguments.debug==YES) printf("\tsetting initial search interval xup_lo:%f, xup_hi:%f\n",xup_lo,xup_hi);
	  if(xup_hi > M_PI/4) break;
      gsl_status = gsl_root_fsolver_set(s, &gsl_func, xup_lo, xup_hi);
	  if(j>0 && up_foundaroot==1){
	    xup_hi -= deviation;
	  }else{
	    xup_hi += deviation;
	  }	
	  //deviation *= 2;
    } while (gsl_status != GSL_SUCCESS);

    /* Iterate root finder */
	if(xup_hi<= M_PI/4){
		do {
		  iter++;
		  gsl_status = gsl_root_fsolver_iterate(s);
		  xup          = gsl_root_fsolver_root(s);
		  xup_lo       = gsl_root_fsolver_x_lower(s);
		  xup_hi       = gsl_root_fsolver_x_upper(s);
		  if(arguments.debug==YES) printf("\txup_lo:%f, xup_hi:%f, xup:%f\n",xup_lo,xup_hi,xup);
		  gsl_status = gsl_root_test_interval (xup_lo, xup_hi, arguments.logs22th13_precision, 0);
		} while (gsl_status == GSL_CONTINUE && iter < max_iter);
		if(arguments.debug==YES) printf("found minimum on iter %d at %f\n",iter,xup);
		up_foundaroot=1;
	}else{
		xup=M_PI/4;
		xup_hi=M_PI/4;
	}
	
	
	//do again, but for the lower bound
	deviation=0.01;
	xdown_hi=osc[1];
	if(down_foundaroot==0){
      xdown_lo = osc[1]-0.01;
	}
    iter = 0;
   
   if(arguments.debug==YES) printf("starting root finder for t=%f\n",t);
    do {
      if(arguments.debug==YES) printf("\tsetting initial search interval xdown_lo:%f, xdown_hi:%f\n",xdown_lo,xdown_hi);
	  if(xdown_lo <= 0.0 || xdown_lo>xdown_hi){
		down_foundaroot=0;
		break;
	  }
      gsl_status = gsl_root_fsolver_set(s, &gsl_func, xdown_lo, xdown_hi);
	  if(j>0 && down_foundaroot==1){
	    xdown_lo += deviation;
	  }else{
	    xdown_lo -= deviation;
	  }	
	  //deviation *=2;
    } while (gsl_status != GSL_SUCCESS);

    /* Iterate root finder */
	if(xdown_lo>0.0 && xdown_lo<xdown_hi){
		do {
		  iter++;
		  gsl_status = gsl_root_fsolver_iterate(s);
		  xdown          = gsl_root_fsolver_root(s);
		  xdown_lo       = gsl_root_fsolver_x_lower(s);
		  xdown_hi       = gsl_root_fsolver_x_upper(s);
		  if(arguments.debug==YES) printf("\txdown_lo:%f, xdown_hi:%f, xdown:%f\n",xdown_lo,xdown_hi,xdown);
		  gsl_status = gsl_root_test_interval (xdown_lo, xdown_hi, arguments.logs22th13_precision, 0);
		} while (gsl_status == GSL_CONTINUE && iter < max_iter);
		if(arguments.debug==YES) printf("found minimum on iter %d at %f\n",iter,xdown);
		down_foundaroot=1;
	}else{
	  xdown=0.0;
	  xdown_lo=0.0;
	}
	
	float exposure=0;
	for(int i=0;i<glb_num_of_exps;i++){
		exposure=exposure+glbGetTargetMass(i)*glbGetRunningTime(i, 0);
	}
	double a[]={exposure,xup,osc[1],xdown};
    AddArrayToOutput(a,4);
  }
}

