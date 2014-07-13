#include "mctools.h"

void bestfitmc(double osc[]){
  //Use glbchiall to compute best fit for each throw
  double chi2;
  glb_params bf_values = glbAllocParams();
  
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbSetOscillationParameters(true_values);
  //set starting parameters to true values
  glbCopyParams(true_values,test_values);
  
  for(int pfluct=0;pfluct<=(arguments.pflucts>0?arguments.pflucts-1:0);pfluct++){
    glbSetRates();
    if(arguments.pflucts>0) pfluctTrueSpectra();
    chi2=ChiAllDeltaOctPrescan(test_values, bf_values, GLB_ALL);
    
    //output chi2, best-fit osc params
    double a[1+6];
    a[0]=chi2;
    for(int i=0;i<6;i++) a[1+i]=glbGetOscParams(bf_values,i);
    AddArrayToOutput(a,1+6);
  }
}

void nofitmc(double osc[],int priorson, int pf){
  //Bayesian approach
  //no fitting, just throwing
  double chi1=0,chi2=0,chipri=0;
  //number of nuisance parameters to to output
  int nuisances=0;
  if(arguments.chimode==19) nuisances=12;
  else if(arguments.chimode==30) nuisances=arguments.nuisances;
  double ranges[6][2]; //throw ranges for test parameters, set to zeros for no fitting
  double a[4+6+nuisances]; //output array
  double inputerror;
  
  ranges[0][0]=ranges[0][1]=0.0; //t12
  ranges[1][0]=0.1007;ranges[1][1]=0.2318; //t13
  ranges[2][0]=0.57;ranges[2][1]=1.0; //t23
  ranges[3][0]=-M_PI;ranges[3][1]=M_PI; //dcp
  ranges[4][0]=ranges[4][1]=0.0; //dm21
  ranges[5][0]=2e-3;ranges[5][1]=3e-3; //dm31
  
  //turn off systematics so glbChiSys doesn't fit anything
  glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
  
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbSetOscillationParameters(true_values);
  //set starting parameters to true values
  glbCopyParams(true_values,test_values);
  //initialize local rng
  //use env variables when running to randomize seeds and create independent MC
  const gsl_rng_type * lrngtype= gsl_rng_default;
  gsl_rng * lrng;
  gsl_rng_env_setup();
  lrng = gsl_rng_alloc(lrngtype);  
  
  for(int pfluct=0;pfluct<=(arguments.pflucts>0?arguments.pflucts-1:0);pfluct++){
    //set true rates and fluctuate bin by bin
    glbSetRates();
    if(pf>0 && arguments.pflucts>0) pfluctTrueSpectra();
    //randomly set values of test osc parameters within ranges
    for(int i=0;i<6;i++){  
        //flat priors
        if(ranges[i][0]!=0.0 || ranges[i][1]!=0.0){
          glbSetOscParams(test_values,ranges[i][0]+((ranges[i][1]-ranges[i][0])*gsl_rng_uniform(lrng)),i);
        }
    }
    chi1=glbChiSys(test_values, GLB_ALL, GLB_ALL);
    
    //do systematics part
    double narray[nuisances]; //to store nuisance parameters throw values
    int systindex=0; //index in nuisance outputs 
    switch(arguments.chimode){
      case 19: //response functions
        //throw 12 flat nuisance parameters between -1.5, and 1.5
        //for(int i=0;i<12;i++) narray[i]=-1.5+3*gsl_rng_uniform(lrng);
        for(int i=0;i<12;i++) narray[i]=-2*(i>3?1:rfCovMatrixInputs[i])+4*(i>3?1:rfCovMatrixInputs[i])*gsl_rng_uniform(lrng);
        //only vary stat terms for now
        //narray[0]=narray[1]=narray[2]=narray[3]=0;
        chi2=chi_ResponseFunctionCov(0,0,nuisances,narray,NULL,NULL);
        break;
      case 30: //norm systs on sig/bg
        chi2=0;
        systindex=0;
        for(int curexp=0;curexp<glb_num_of_exps;curexp++){
          for(int currule=0;currule<glbGetNumberOfRules(curexp);currule++){
            narray[0]=-3+6*gsl_rng_uniform(lrng);
            narray[1]=-3+6*gsl_rng_uniform(lrng);
            chi2+=chiSpectrumNormCustom(curexp,currule,2,narray,glbGetSysErrorsListPtr(curexp, currule, GLB_ON),NULL); //make this exist
            //store in output array
            a[4+6+systindex]=narray[0];
            a[4+6+systindex+1]=narray[1];
            systindex+=2;
          }
        }
        break;
    }
    if(priorson==1){
      chipri=0; //reset prior
      //compute prior with respect to input_errors and true oscillation parameters
      for(int i=0;i<6;i++){
          inputerror=glbGetOscParams(input_errors,i);
          if(inputerror>1e-12) chipri+=pow((glbGetOscParams(true_values,i)-glbGetOscParams(test_values,i))/inputerror,2);
      }
    }
    
    //output chi2, best-fit osc params, and nuisance parameters
    a[0]=chi2+chipri;  //total
    a[1]=chi1;  //nosysts
    a[2]=chipri; //prior term
    a[3]=fchi2; //penalty term
    //add oscillation parameters
    for(int i=0;i<6;i++) a[4+i]=glbGetOscParams(test_values,i);
    //add nuisance parameters
    if(arguments.chimode==19) for(int i=0;i<nuisances;i++) a[4+6+i]=narray[i];
    AddArrayToOutput(a,4+6+nuisances);
  }
}

void EventSpectraOutputTruePrePost(double osctrue[], double osctest[], int fittype){
  double chiprefit=0, chipostfit=0;
  //utility function to output true, pre-fit, and post-fit spectra
  glbDefineParams(true_values,osctrue[0],osctrue[1],osctrue[2],osctrue[3],osctrue[4],osctrue[5]);
  glbSetOscillationParameters(true_values);
  glbSetRates();  
  printf("Running with true parameters, osc(%f,%f,%f,%f,%f,%f)\n",osctrue[0],osctrue[1],osctrue[2],osctrue[3],osctrue[4],osctrue[5]);
  glbDefineParams(test_values,osctest[0],osctest[1],osctest[2],osctest[3],osctest[4],osctest[5]);
  glbSetCentralValues(test_values);
  printf("Running with fit start parameters, osc(%f,%f,%f,%f,%f,%f)\n",osctest[0],osctest[1],osctest[2],osctest[3],osctest[4],osctest[5]);
  
  //output true rates
  for(int curexp=0;curexp<glb_num_of_exps;curexp++){
    for(int currule=0;currule<glbGetNumberOfRules(curexp);currule++){
      PrintRatesToFile(0,curexp,currule);
      AddToOutputBlankline();
      AddToOutputBlankline();
    }
  }

  //do a fit with no systs and osc fixed to set pre-fit event rates
  glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_OFF);
  chiprefit=glbChiSys(test_values, GLB_ALL, GLB_ALL);
  if(arguments.chimode!=101) glbSwitchSystematics(GLB_ALL, GLB_ALL, GLB_ON);  

  //output pre-fit rates
  for(int curexp=0;curexp<glb_num_of_exps;curexp++){
    for(int currule=0;currule<glbGetNumberOfRules(curexp);currule++){
      PrintRatesToFile(1,curexp,currule);
      AddToOutputBlankline();
      AddToOutputBlankline();
    }
  }
  for(int exp=0;exp<glb_num_of_exps;exp++){
    for(int rule=0;rule<glbGetNumberOfRules(exp);rule++){
      char sys_id[255];
      glbGetChiFunction(exp,rule,GLB_ON,sys_id,255);
      printf("Experiment %d, rule %d chi function (turned %s) is set to %s with %d parameters\n"
        ,exp,rule,
        glbGetSysOnOffState(exp, rule)==1?"ON":"OFF",
        sys_id,glbGetSysDimInExperiment(exp, rule, GLB_ON)); 
    }
  }
  //enable nuisance parameter storage
  arguments.nuis_output=1;
  //fit
  if(arguments.test){
    chipostfit=glbChiSys(test_values, GLB_ALL,GLB_ALL);
  }else{
    switch(fittype){
      case 1:
        chipostfit=ChiAllDeltaOctPrescan(test_values,test_values, GLB_ALL);
        break;
      case 2:
        chipostfit=glbChiDelta(test_values,test_values, GLB_ALL);
        break;
      case 3:
        chipostfit=ChiNPDeltaPrescan(test_values,test_values, GLB_ALL);
        break;
    }
  }
  //disable nuisance parameter output (do it manually)
  arguments.nuis_output=0;
  printf("Found fit end parameters, osc(%f,%f,%f,%f,%f,%f)\n",glbGetOscParams(test_values,0),glbGetOscParams(test_values,1),glbGetOscParams(test_values,2),glbGetOscParams(test_values,3),glbGetOscParams(test_values,4),glbGetOscParams(test_values,5));
  //output post-fit rates
  for(int curexp=0;curexp<glb_num_of_exps;curexp++){
    for(int currule=0;currule<glbGetNumberOfRules(curexp);currule++){
      PrintRatesToFile(1,curexp,currule);
      AddToOutputBlankline();
      AddToOutputBlankline();
    }
  }
  printf("prefitchi2=%f, postfitchi2=%f\n",chiprefit,chipostfit);
  
  //for norm systs, recalculate fchi2 (penalty term)
  //because it gets overwritten with multiple calls to chisq
  if(arguments.chimode==30){
    fchi2=0;
    for(int curexp=0;curexp<glb_num_of_exps;curexp++){
      for(int currule=0;currule<glbGetNumberOfRules(curexp);currule++){
          double *sys_errors = glbGetSysErrorsListPtr(curexp, currule, GLB_ON);
          fchi2+=pow(xmin[curexp][currule][0]/sys_errors[0],2);
          fchi2+=pow(xmin[curexp][currule][1]/sys_errors[1],2);
      }
    }
  }
  
  //Add chi^2 values to end of file
  double a[]={chiprefit,chipostfit,fchi1,fchi2};
  AddArrayToOutput(a,4);
  
  //Add xmin values to end of file
  AddToOutputBlankline();
  AddToOutputBlankline();
  if(arguments.chimode==19){
    //output nuisance parameters for response function systematics
    int noutput=12*arguments.systs>32?32:12*arguments.systs;
    double b[noutput];
    for(int i=0;i<noutput;i++)b[i]=xmin[0][0][i];
    //double b[]={xmin[0][0][0],xmin[0][0][1],xmin[0][0][2],xmin[0][0][3],xmin[0][0][4],xmin[0][0][5],xmin[0][0][6],xmin[0][0][7],xmin[0][0][8],xmin[0][0][9],xmin[0][0][10],xmin[0][0][11],xmin[0][0][12]};
    AddArrayToOutput(b,noutput);
  }else if(arguments.chimode==30){
    //output nuisance parameters for chiSpectrumNormCustom
    for(int curexp=0;curexp<glb_num_of_exps;curexp++){
      for(int currule=0;currule<glbGetNumberOfRules(curexp);currule++){
          double b[]={xmin[curexp][currule][0],xmin[curexp][currule][1]};
          AddArrayToOutput(b,2);
      }
    }
  }else if(arguments.chimode==31 || arguments.chimode==31){
    //output nuisance parameters for chiCorrSplitBGs_LBNEFMC
    double b[]={xmin[0][0][0],xmin[0][0][1],xmin[0][0][2],xmin[0][0][3]};
    AddArrayToOutput(b,4);
  }
}
