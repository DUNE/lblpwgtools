#include "priors.h"

void registerprior(){
    switch(arguments.regprior){
      case 0:
        break;
      case 1:
        //Capozzi 2013, no dcp constraint
        printf("Using Capozzi 2013 constraints (no dcp)\n");
        initCapozzi13(0,0);
        glbRegisterPriorFunction(my_prior_Capozzi13,NULL,NULL,NULL);
        break;
      case 2:
        //Capozzi 2013, with dcp constraint
        printf("Using Capozzi 2013 constraints (with dcp)\n");
        initCapozzi13(1,0);
        glbRegisterPriorFunction(my_prior_Capozzi13,NULL,NULL,NULL);
        break;
      case 3:
        //Capozzi 2013, no dcp constraint, projected t13 constraint
        printf("Using Capozzi 2013 constraints (no dcp, projected)\n");
        initCapozzi13(0,1);
        glbRegisterPriorFunction(my_prior_Capozzi13,NULL,NULL,NULL);
        break;
    }
}

double my_prior_t23_opoct(const glb_params in, void* user_data)
{
	/* This prior only contrains t23 to the opposite octant
	 * all other parameters are treated normally.
	 * It does this by keeping t23 fit in the same octant as the central
	 * value. The inputerror on t23 has no effect for this prior.
	*/
	glb_params central_values = glbAllocParams();
	glb_params input_errors = glbAllocParams();
	glb_projection p = glbAllocProjection();
	glbGetCentralValues(central_values);
	glbGetInputErrors(input_errors);
	glbGetProjection(p);
	int i;
	double pv = 0.0;
	double fitvalue,centralvalue,inputerror;
	/* Add oscillation parameter priors */
	for(i=0;i<6;i++)
		if(glbGetProjectionFlag(p,i)==GLB_FREE){
			fitvalue=glbGetOscParams(in,i);
			centralvalue=glbGetOscParams(central_values,i);
			inputerror=glbGetOscParams(input_errors,i);
			if(i==GLB_THETA_23){
				if(centralvalue>M_PI/4){
					if(fitvalue<M_PI/4) pv+=1e6;
				}else{
					if(fitvalue>M_PI/4) pv+=1e6;
				}
			}else{
				if(inputerror>1e-12) pv+=pow((centralvalue-fitvalue)/inputerror,2);
			}
		}
	/* Add matter parameter priors */
	for(i=0;i<glb_num_of_exps;i++)
		if(glbGetDensityProjectionFlag(p,i)==GLB_FREE){
			fitvalue=glbGetDensityParams(in,i);
			centralvalue=1.0;
			inputerror=glbGetDensityParams(input_errors,i);
			if(inputerror>1e-12)
				pv+=pow((centralvalue-fitvalue)/inputerror,2);
		}
		
	glbFreeParams(central_values);
	glbFreeParams(input_errors);
	glbFreeProjection(p);
	return pv;
}

void initCapozzi13(int which_dcp, int which_t13){
  //init splines for Capozzi 13 global fit
  //init accelerators
  //parameter dcp=1 means use dcp constraint, otherwise no constraint
  for(int i=0;i<12;i++){
    priors_acc[i]=gsl_interp_accel_alloc ();
  }

  //dm^2 (nh & ih)
  double x4[] = {5.73E-05,6.99E-05,7.15E-05,7.32E-05,7.54E-05,7.8E-05,8E-05,8.18E-05,9.44E-05};
  double y4[] = {10,3,2,1,0,1,2,3,10};
  priors_spline[4] = gsl_spline_alloc(gsl_interp_linear,9);
  gsl_spline_init(priors_spline[4],x4,y4,9);
  priors_spline[10] = gsl_spline_alloc(gsl_interp_linear,9);
  gsl_spline_init(priors_spline[10],x4,y4,9);
  priors_splinerange[4][0]=priors_splinerange[10][0]=5.73E-05;
  priors_splinerange[4][1]=priors_splinerange[10][1]=9.44E-05;
  
  //Dm^2 (nh)
  double x5[] = {0.00166,0.00222,0.0023,0.00238,0.00244,0.00252,0.00259,0.00266,0.00315};
  double y5[] = {10,3,2,1,0,1,2,3,10};
  priors_spline[5] = gsl_spline_alloc(gsl_interp_linear,9);
  gsl_spline_init(priors_spline[5],x5,y5,9);
  priors_splinerange[5][0]=0.00166;
  priors_splinerange[5][1]=0.00315;
  
  //Dm^2 (ih)
  double x11[] = {0.00161,0.00217,0.00225,0.00233,0.0024,0.00247,0.00254,0.00261,0.0031};
  double y11[] = {10,3,2,1,0,1,2,3,10};
  priors_spline[11] = gsl_spline_alloc(gsl_interp_linear,9);
  gsl_spline_init(priors_spline[11],x11,y11,9);
  priors_splinerange[11][0]=0.00161;
  priors_splinerange[11][1]=0.0031;
  
  if(which_dcp==1){
    //dcp/pi (nh)
    double x3[] = {0,0.11,0.191617,0.299401,0.383234,0.479042,0.610778,0.694611,0.754491,0.88,1.12,1.23353,1.39,1.49701,1.64072,1.72,1.80838,2};
    double y3[] = {1.74803,2,2.29921,2.53543,2.69291,2.77165,2.69291,2.56693,2.3622,2,1,0.472441,0,0.488189,0.834646,1,1.25984,1.74803};
    priors_spline[3] = gsl_spline_alloc(gsl_interp_linear,18);
    gsl_spline_init(priors_spline[3],x3,y3,18);
    priors_splinerange[3][0]=0;
    priors_splinerange[3][1]=2;
    
    //dcp/pi (ih)
    double x9[] = {0,0.04,0.191617,0.335329,0.431138,0.526946,0.586826,0.65,0.742515,0.96,1.05389,1.1018,1.17365,1.24551,1.25749,1.28144,1.31737,1.35,1.37725,1.43713,1.59,1.68862,1.73653,1.79641,1.85629,2};
    double y9[] = {1.9685,2,2.25197,2.33071,2.31496,2.23622,2.12598,2,1.73228,1,0.692913,0.551181,0.488189,0.409449,0.283465,0.188976,0.0314961,0,0.173228,0.472441,1,1.37008,1.49606,1.62205,1.74803,1.9685};
    priors_spline[9] = gsl_spline_alloc(gsl_interp_linear,26);
    gsl_spline_init(priors_spline[9],x9,y9,26);
    priors_splinerange[9][0]=0;
    priors_splinerange[9][1]=2;
  }else{
    //dcp/pi
    double x3[] = {0,2};
    double y3[] = {0,0};
    priors_spline[3] = gsl_spline_alloc(gsl_interp_linear,2);
    gsl_spline_init(priors_spline[3],x3,y3,2);
    priors_splinerange[3][0]=0;
    priors_splinerange[3][1]=2;
    priors_spline[9] = gsl_spline_alloc(gsl_interp_linear,2);
    gsl_spline_init(priors_spline[9],x3,y3,2);
    priors_splinerange[9][0]=0;
    priors_splinerange[9][1]=2;   
  }
  //sin^2(q12) (nh & ih)
  double x0[] = {0.147,0.259,0.275,0.291,0.308,0.325,0.342,0.359,0.478};
  double y0[] = {10,3,2,1,0,1,2,3,10};
  priors_spline[0] = gsl_spline_alloc(gsl_interp_linear,9);
  gsl_spline_init(priors_spline[0],x0,y0,9);
  priors_spline[6] = gsl_spline_alloc(gsl_interp_linear,9);
  gsl_spline_init(priors_spline[6],x0,y0,9);
  priors_splinerange[0][0]=0.147;priors_splinerange[6][0]=0.147;
  priors_splinerange[0][1]=0.478;priors_splinerange[6][1]=0.478;
  
  //sin^2(q23) (nh)
  double x2[] = {0.340719,0.347904,0.357,0.376,0.398,0.425,0.454,0.47006,0.489222,0.506,0.522754,0.553892,0.582635,0.606587,0.620958,0.641,0.654491,0.668862};
  double y2[] = {4,3.51373,3,2,1,0,1,1.50588,1.81961,2,2.21176,2.25882,2.2902,2.41569,2.65098,3,3.52941,4};
  priors_spline[2] = gsl_spline_alloc(gsl_interp_linear,18);
  gsl_spline_init(priors_spline[2],x2,y2,18);
  priors_splinerange[2][0]=0.340719;
  priors_splinerange[2][1]=0.668862;
  
  //sin^2(q23) (ih)
  double x8[] = {0.343114,0.363,0.384,0.408,0.437,0.455689,0.47485,0.496,0.510778,0.531,0.556287,0.58024,0.597006,0.61,0.623353,0.637,0.659,0.668862,0.676048};
  double y8[] = {4,3,2,1,0,0.517647,0.847059,1,1.06667,1,0.752941,0.470588,0.784314,1,1.58431,2,3,3.5451,4};
  priors_spline[8] = gsl_spline_alloc(gsl_interp_linear,19);
  gsl_spline_init(priors_spline[8],x8,y8,19);
  priors_splinerange[8][0]=0.343114;
  priors_splinerange[8][1]=0.676048;
  
  if(which_t13==0){
    //Capozzi 2013
    //sin^2(q13) (nh)
    //double x1[] = {0.0154762,0.0164087,0.0177,0.0197,0.0216,0.0234,0.0256,0.0276,0.0297,0.0302363,0.0311043};
    //double y1[] = {4,3.49955,3,2,1,0,1,2,3,3.56981,4};
    //Smoothed and extrapolated
    double x1[] = {0.0034,0.02340,0.0434};
    double y1[] = {10,0,10};    
    priors_spline[1] = gsl_spline_alloc(gsl_interp_linear,3);
    gsl_spline_init(priors_spline[1],x1,y1,3);
    priors_splinerange[1][0]=0.0034;
    priors_splinerange[1][1]=0.0434;
    
    //sin^2(q13) (ih)
    //double x7[] = {0.0154762,0.0167745,0.0178,0.0198,0.0218,0.0239,0.026,0.028,0.03,0.030773,0.0314647};
    //double y7[] = {4,3.40525,3,2,1,0,1,2,3,3.60158,4};
    double x7[] = {0.0039,0.0239,0.0439};
    double y7[] = {10,0,10};     
    priors_spline[7] = gsl_spline_alloc(gsl_interp_linear,3);
    gsl_spline_init(priors_spline[7],x7,y7,3);
    priors_splinerange[7][0]=0.0039;
    priors_splinerange[7][1]=0.0439;
  }else{
    //Capozzi13, projected to no stat errors
    //sin^2(q13) (nh)
    double x1[] = {0.0103,0.02340,0.0381};
    double y1[] = {10,0,10};
    priors_spline[1] = gsl_spline_alloc(gsl_interp_linear,3);
    gsl_spline_init(priors_spline[1],x1,y1,3);
    priors_splinerange[1][0]=0.011512;
    priors_splinerange[1][1]=0.039332;
    
    //sin^2(q13) (ih)
    double x7[] = {0.0106,0.02390,0.0389};
    double y7[] = {10,0,10};
    priors_spline[7] = gsl_spline_alloc(gsl_interp_linear,3);
    gsl_spline_init(priors_spline[7],x7,y7,3);
    priors_splinerange[7][0]=0.011759;
    priors_splinerange[7][1]=0.040167;    
  }
  
}

double my_prior_defglobes(const glb_params in, void* user_data){
	glb_params central_values = glbAllocParams();
	glb_params input_errors = glbAllocParams();
	glb_projection p = glbAllocProjection();
	glbGetCentralValues(central_values);
	glbGetInputErrors(input_errors);
	glbGetProjection(p);
	int i;
	double pv = 0.0;
	double fitvalue,centralvalue,inputerror;
	/* Add oscillation parameter priors */
	for(i=0;i<6;i++)
		if(glbGetProjectionFlag(p,i)==GLB_FREE){
			fitvalue=glbGetOscParams(in,i);
			centralvalue=glbGetOscParams(central_values,i);
			inputerror=glbGetOscParams(input_errors,i);
			if(inputerror>1e-12) pv+=pow((centralvalue-fitvalue)/inputerror,2);
		}
	/* Add matter parameter priors */
	for(i=0;i<glb_num_of_exps;i++){
		if(glbGetDensityProjectionFlag(p,i)==GLB_FREE){
			fitvalue=glbGetDensityParams(in,i);
			centralvalue=1.0;
			inputerror=glbGetDensityParams(input_errors,i);
			if(inputerror>1e-12)
				pv+=pow((centralvalue-fitvalue)/inputerror,2);
		}
  }
		
	glbFreeParams(central_values);
	glbFreeParams(input_errors);
	glbFreeProjection(p);
	return pv;
}

double my_prior_Capozzi13(const glb_params in, void* user_data)
{
	/* This prior implements constraints on the oscillation parameters
   * via splines. The splines are digitized forms of 
   * Capozzi et al, 2013, arXiv:1312.2878v1
	*/
	glb_projection p = glbAllocProjection();
  glb_params linput_errors = glbAllocParams();
  glbGetInputErrors(linput_errors);
	glbGetProjection(p);
  //glbPrintProjection(stdout,p);
	int i,spline_i;
	double pv = 0.0;
	double fitvalue,xvalue=0,inputerror;
  
	/* Add oscillation parameter priors */
	for(i=0;i<6;i++){
		if(glbGetProjectionFlag(p,i)==GLB_FREE){
			//chose whether to use nh or ih spline based on sign of dm31
      if(glbGetOscParams(in,5)<0){
        spline_i=i+6;
      }else{
        spline_i=i;
      }
      //printf("%d,%d,%f\n",spline_i,i,glbGetOscParams(in,5));
      fitvalue=glbGetOscParams(in,i);
      //printf("%d\n",i);
      switch (i){
        case 0: case 1: case 2: //t12, t13, t23
          xvalue=pow(sin(fitvalue),2);
          break;
        case 3: //dcp, restricted to 0, 2
          xvalue=fitvalue;
          while(xvalue<0) xvalue+=2*M_PI;
          while(xvalue>2*M_PI) xvalue-=2*M_PI;
          xvalue=xvalue/(M_PI);
          //printf("%d,%d,%f,%f,%f\n",i,spline_i,xvalue, fitvalue,gsl_spline_eval(priors_spline[spline_i], xvalue, priors_acc[spline_i]));
          break;
        case 4: //dm^2=dm21
          xvalue=fitvalue;
          break;
        case 5: //Dm^2=dm31-0.5*dm^2
          xvalue=(spline_i>5?-1:1)*(fitvalue-0.5*glbGetOscParams(in,4));
          break;
      }
      //printf("%d,%d,%f,%f,%f\n",i,spline_i,xvalue, fitvalue,gsl_spline_eval(priors_spline[spline_i], xvalue, priors_acc[spline_i]));
      //check that xvalue is in the spline range
      //printf("\t%f,%f,%f,%d\n",xvalue,priors_splinerange[0][0],priors_splinerange[spline_i][1],spline_i);
      if(xvalue>priors_splinerange[spline_i][1] || xvalue<priors_splinerange[spline_i][0]){
        //printf("\t%i,%i,%f,%f,%f\n",i,spline_i,xvalue,priors_splinerange[0][0],priors_splinerange[spline_i][1]);
        pv=1e6; //just short circuit with a huge penalty if we're out of range, to speed things up
        break;
      }
      //splines y values are in terms of Nsigma so square the return value
      pv+=pow(gsl_spline_eval(priors_spline[spline_i], xvalue, priors_acc[spline_i]),2);
      //printf("%d,%f\n",i,pv);
		}
  }
	/* Add matter parameter priors */
  //Make sure this is constrained properly, if the matter density goes negative then the universe may be destroyed
	for(i=0;i<glb_num_of_exps;i++){
		if(glbGetDensityProjectionFlag(p,i)==GLB_FREE){
			fitvalue=glbGetDensityParams(in,i);
			inputerror=glbGetDensityParams(linput_errors,i);
			if(inputerror>1e-12)
				pv+=pow((1.0-fitvalue)/inputerror,2);
		}
  }
  
  glbFreeParams(linput_errors); //this has to be free before the projection!
  glbFreeProjection(p);
	return pv;
}


void test_priors(double osc[], int proj, double xrange[]){
  //Compute penalty term for a scan over osc parameter proj
	int steps=1000;
	double testvals[7];
  double penalty=0;	
  
	glb_projection projnp = glbAllocProjection();
	glbDefineProjection(projnp, GLB_FREE, GLB_FREE, GLB_FREE, GLB_FREE, GLB_FREE, GLB_FREE);
	glbSetDensityProjectionFlag(projnp, GLB_FREE, GLB_ALL);
	glbSetProjection(projnp);

	glbDefineParams(test_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
	glbSetDensityParams(test_values,1.0,GLB_ALL);
 
  
	for(double x=xrange[0];x<=xrange[1];x+=(xrange[1]-xrange[0])/steps){
    for(int i=0;i<6;i++) testvals[i]=(proj==i?x:osc[i]);
    //printf("%d,%f\n",proj,x);
    glbDefineParams(test_values,testvals[0],testvals[1],testvals[2],testvals[3],testvals[4],testvals[5]);
    glbSetDensityParams(test_values,1.0,GLB_ALL);
    //glbSetCentralValues(test_values);
    switch(arguments.regprior){
      case 0:
        penalty=my_prior_defglobes(test_values,NULL);
        break;
      case 1: case 2: case 3:
        penalty=my_prior_Capozzi13(test_values,NULL);
        break;
    }
    double a[]={x,penalty};
		AddArrayToOutput(a,2);
	}
}

/*void twod_priors(double osc[], int osc1, int osc2){
  //2-dimensional exploration of priors
  //no systs, no corrs

}*/
