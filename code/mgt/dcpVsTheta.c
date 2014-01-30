#include "dcpVsTheta.h"

void dcpvst13(double osc[], double vrange[2])
{
  //Compute allowed regions in DeltaCP vs t13 for true values in osc over range vrange in t13
  double chi2[2];
  double mindeltas[2];
  double lowt13=vrange[0];
  double hight13=vrange[1];
  
  glb_projection projT13dcp = glbAllocProjection();
  glbDefineProjection(projT13dcp, GLB_FREE, GLB_FIXED, GLB_FREE, GLB_FIXED, GLB_FREE, GLB_FREE);
  glbSetDensityProjectionFlag(projT13dcp, GLB_FREE, GLB_ALL);
  glbSetProjection(projT13dcp);
  
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbSetOscillationParameters(true_values);
  glbSetRates();
  double osc5[]={osc[5],-osc[5]+osc[4]};
  for(double this_dcp=-M_PI;this_dcp<=M_PI;this_dcp=this_dcp+(M_PI/35)){
    for(double thist13=lowt13;thist13<=hight13;thist13+=(hight13-lowt13)/35){
			for(int k=0;k<2;k++){ //profile MH
				glbDefineParams(test_values,osc[0],thist13,osc[2],this_dcp,osc[4],osc5[k]);
				glbSetDensityParams(test_values,1.0,GLB_ALL);
				glbDefineParams(central_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc5[k]);
				glbSetCentralValues(central_values); 
				if(arguments.test){
					chi2[k]=glbChiSys(test_values,GLB_ALL,GLB_ALL);
				}else{
					chi2[k]=glbChiTheta13Delta(test_values,test_values,GLB_ALL);
				}
				mindeltas[k]=glbGetOscParams(test_values,GLB_THETA_23);
			}
			double a[]={this_dcp,thist13, chi2[0], chi2[1], mindeltas[0], mindeltas[1]};
			AddArrayToOutput(a,6);
    }
	AddToOutputBlankline();
  }
}

void dcpvst13_nzt(double osc[], double vrange[2])
{
  //Compute deltachisq for non-zero t13 as a function of deltaCP and t13
  //for input true values in osc and over vrange in t13.
  double chi2[2];
  double mindeltas[2];
  double lowt13=vrange[0];
  double hight13=vrange[1];
  
  glb_projection projT13dcp = glbAllocProjection();
  glbDefineProjection(projT13dcp, GLB_FREE, GLB_FIXED, GLB_FREE, GLB_FIXED, GLB_FREE, GLB_FREE);
  glbSetDensityProjectionFlag(projT13dcp, GLB_FREE, GLB_ALL);
  glbSetProjection(projT13dcp);
  

  double osc5[]={osc[5],-osc[5]+osc[4]};
  for(double this_dcp=-M_PI;this_dcp<=M_PI;this_dcp=this_dcp+(M_PI/35)){
    for(double thist13=lowt13;thist13<=hight13;thist13+=(hight13-lowt13)/35){
			glbDefineParams(true_values,osc[0],thist13,osc[2],this_dcp,osc[4],osc[5]);
			glbSetDensityParams(true_values,1.0,GLB_ALL);
			glbSetOscillationParameters(true_values);
			glbSetRates();
			
			for(int k=0;k<2;k++){ //profile MH
				glbDefineParams(test_values,osc[0],0,osc[2],this_dcp,osc[4],osc5[k]);
				glbSetDensityParams(test_values,1.0,GLB_ALL);
				glbDefineParams(central_values,osc[0],0,osc[2],osc[3],osc[4],osc5[k]);
				glbSetCentralValues(central_values);
				if(arguments.test){
					chi2[k]=glbChiSys(test_values,GLB_ALL,GLB_ALL);
				}else{
					chi2[k]=glbChiTheta13Delta(test_values,test_values,GLB_ALL);
				}
				mindeltas[k]=glbGetOscParams(test_values,GLB_THETA_23);
			}
			double a[]={this_dcp,thist13, chi2[0], chi2[1], mindeltas[0], mindeltas[1]};
			AddArrayToOutput(a,6);
    }
	AddToOutputBlankline();
  }
}

void dcpvst23(double osc[], double vrange[2])
{
  //Compute allowed regions in DeltaCP vs t23 for true values in osc over range vrange in t23
  double chi2[2];
  double lowt23=vrange[0];
  double hight23=vrange[1];
  
  glb_projection projT23dcp = glbAllocProjection();
  glbDefineProjection(projT23dcp, GLB_FREE, GLB_FREE, GLB_FIXED, GLB_FIXED, GLB_FREE, GLB_FREE);
  glbSetDensityProjectionFlag(projT23dcp, GLB_FREE, GLB_ALL);
  glbSetProjection(projT23dcp);
  
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbSetOscillationParameters(true_values);
  glbSetRates();
	double osc5[]={osc[5],-osc[5]+osc[4]};
	
  for(double this_dcp=-M_PI;this_dcp<=M_PI;this_dcp=this_dcp+(M_PI/40)){
    for(double thist23=lowt23;thist23<=hight23;thist23+=(hight23-lowt23)/40){
			for(int k=0;k<2;k++){ //profile MH
				glbDefineParams(test_values,osc[0],osc[1],thist23,this_dcp,osc[4],osc5[k]);  
				glbSetDensityParams(test_values,1.0,GLB_ALL);
				glbDefineParams(central_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc5[k]);
				glbSetCentralValues(central_values); 
				if(arguments.test)
					chi2[k]=glbChiSys(test_values,GLB_ALL,GLB_ALL);
				else
					chi2[k]=glbChiNP(test_values,test_values,GLB_ALL);
			}
			double a[]={this_dcp,thist23, chi2[0], chi2[1]};
			AddArrayToOutput(a,4);
		}
	AddToOutputBlankline();
  }
}

void dcpvst23_oct(double osc[], double vrange[2])
{
  //Compute deltachisq for octant sensitivity as a function of deltaCP and t23
  //for input true values in osc, and over vrange in t23.
  double chi2[2];
  double minvars[2][4];
  double lowt23=vrange[0];
  double hight23=vrange[1];
  double tminchi2, tchi2;
  double steps=35; //note that both axes use double this number of steps
  double mindcp=osc[3];
  double mint13=osc[1];
  
  glb_projection proj= glbAllocProjection();
  //keeping t12 and DM21 fixed to speed things up
  glbDefineProjection(proj, GLB_FIXED, GLB_FREE, GLB_FREE, GLB_FREE, GLB_FIXED, GLB_FREE);
  glbSetDensityProjectionFlag(proj, GLB_FIXED, GLB_ALL);
  glbSetProjection(proj);
  //use prior on t23 that constrains the fit to opposite octant
  glbRegisterPriorFunction(my_prior_t23_opoct,NULL,NULL,NULL);
  
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbSetOscillationParameters(true_values);
  glbSetRates();
	double osc5[]={osc[5],-osc[5]+osc[4]};
  for(double this_dcp=-M_PI;this_dcp<=M_PI;this_dcp=this_dcp+(M_PI/steps)){
    for(double thist23=lowt23;thist23<=hight23;thist23+=(hight23-lowt23)/(2*steps)){
			glbDefineParams(true_values,osc[0],osc[1],thist23,this_dcp,osc[4],osc[5]);
			glbSetDensityParams(true_values,1.0,GLB_ALL);
			glbSetOscillationParameters(true_values);
			glbSetRates();
			double osc2[]={(M_PI/2)-thist23,(M_PI/2)-thist23};
			for(int k=0;k<2;k++){ //profile over mass hierarchy
				//do prescan without systematics in deltacp, t23 octant
				tminchi2=1e6;
				for(double this_tdcp=-M_PI;this_tdcp<M_PI;this_tdcp=this_tdcp+(M_PI/80)){
					glbDefineParams(test_values,osc[0],osc[1],osc2[k],this_tdcp,osc[4],osc5[k]);  
					glbSetDensityParams(test_values,1.0,GLB_ALL);
					tchi2=glbChiSys(test_values,GLB_ALL,GLB_ALL);
					if(tchi2<tminchi2){
						tminchi2=tchi2;
						mindcp=this_tdcp;
					}
				}
				glbDefineParams(test_values,osc[0],mint13,osc2[k],mindcp,osc[4],osc5[k]);
				glbSetDensityParams(test_values,1.0,GLB_ALL); 
				glbDefineParams(central_values,osc[0],osc[1],osc2[k],mindcp,osc[4],osc5[k]);
				glbSetCentralValues(central_values); 
				if(arguments.test)
					chi2[k]=glbChiSys(test_values,GLB_ALL,GLB_ALL);
				else{
					chi2[k]=glbChiNP(test_values,test_values,GLB_ALL);
				}
				minvars[k][0]=glbGetOscParams(test_values,GLB_THETA_13);
				minvars[k][1]=glbGetOscParams(test_values,GLB_THETA_23);
				minvars[k][2]=glbGetOscParams(test_values,GLB_DELTA_CP);
				minvars[k][3]=glbGetOscParams(test_values,GLB_DM_31);
			}

			//get minimum chi2 value
			double minchi2=1e6; int mini=0;
			for(int i=0;i<2;i++) if(chi2[i]<minchi2) {minchi2=chi2[i]; mini=i;}
			double a[]={this_dcp,thist23, minchi2, minvars[mini][0], minvars[mini][1], minvars[mini][2], minvars[mini][3]};
			AddArrayToOutput(a,7);
		}
		AddToOutputBlankline();
  }
}

void dcpvst23_nmq(double osc[], double vrange[2])
{
  //Compute deltachisq for non-maximal t23 sensitivity as a function of deltaCP and t23
  //for input true values in osc, and over vrange in t23.
  double chi2[2];
  double minvars[2][4];
  double lowt23=vrange[0];
  double hight23=vrange[1];
	double tminchi2, tchi2;
  double steps=35; //note that both axes use double this number of steps
  double mindcp=osc[3];
  
  glb_projection proj= glbAllocProjection();
  //keeping t12 and DM21 fixed to speed things up
  glbDefineProjection(proj, GLB_FIXED, GLB_FREE, GLB_FIXED, GLB_FREE, GLB_FIXED, GLB_FREE);
  glbSetDensityProjectionFlag(proj, GLB_FIXED, GLB_ALL);
  glbSetProjection(proj);
  
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbSetOscillationParameters(true_values);
  glbSetRates();
	double osc5[]={osc[5],-osc[5]+osc[4]};
  for(double this_dcp=-M_PI;this_dcp<=M_PI;this_dcp=this_dcp+(M_PI/steps)){
    for(double thist23=lowt23;thist23<=hight23;thist23+=(hight23-lowt23)/(2*steps)){	
			for(int k=0;k<2;k++){ //profile MH
				glbDefineParams(true_values,osc[0],osc[1],thist23,this_dcp,osc[4],osc[5]);
				glbSetDensityParams(true_values,1.0,GLB_ALL);
				glbSetOscillationParameters(true_values);
				glbSetRates();
				
				//do prescan without systematics in deltacp, t23 octant
				tminchi2=1e6;
				for(double this_tdcp=-M_PI;this_tdcp<M_PI;this_tdcp=this_tdcp+(M_PI/36)){
					glbDefineParams(test_values,osc[0],osc[1],(M_PI/4),this_tdcp,osc[4],osc5[k]);  
					glbSetDensityParams(test_values,1.0,GLB_ALL);
					tchi2=glbChiSys(test_values,GLB_ALL,GLB_ALL);
					if(tchi2<tminchi2){
						tminchi2=tchi2;
						mindcp=this_tdcp;
					}
				}
				
				glbDefineParams(test_values,osc[0],osc[1],(M_PI/4),mindcp,osc[4],osc5[k]);
				glbSetDensityParams(test_values,1.0,GLB_ALL);
				glbDefineParams(central_values,osc[0],osc[1],(M_PI/4),this_dcp,osc[4],osc5[k]);
				glbSetCentralValues(central_values); 
				if(arguments.test)
					chi2[k]=glbChiSys(test_values,GLB_ALL,GLB_ALL);
				else
					chi2[k]=glbChiNP(test_values,test_values,GLB_ALL);
					
				minvars[k][0]=glbGetOscParams(test_values,GLB_THETA_13);
				minvars[k][1]=glbGetOscParams(test_values,GLB_THETA_23);
				minvars[k][2]=glbGetOscParams(test_values,GLB_DELTA_CP);
				minvars[k][3]=glbGetOscParams(test_values,GLB_DM_31);
			}
			double minchi2=1e6; int mini=0;
			for(int i=0;i<2;i++) if(chi2[i]<minchi2) {minchi2=chi2[i]; mini=i;}
			double a[]={this_dcp,thist23, minchi2, minvars[mini][0], minvars[mini][1], minvars[mini][2], minvars[mini][3]};
			AddArrayToOutput(a,7);
		}
	AddToOutputBlankline();
  }
}

void dcpvst23_cpv(double osc[], double vrange[2])
{
  //Compute CPV sensitivity as a function of deltaCP and t23
  //for input true values in osc, and over vrange in t23.
  double chi2[8];
  double minvars[4]={0,0,0,0};
  double lowt23=vrange[0];
  double hight23=vrange[1];
  double steps=35;
  
  glb_projection proj= glbAllocProjection();
  //keeping t12 and DM21 fixed to speed things up
  glbDefineProjection(proj, GLB_FIXED, GLB_FREE, GLB_FREE, GLB_FIXED, GLB_FIXED, GLB_FREE);
  glbSetDensityProjectionFlag(proj, GLB_FIXED, GLB_ALL);
  glbSetProjection(proj);
  
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbSetOscillationParameters(true_values);
  glbSetRates();
	double osc3[]={0,M_PI,0,M_PI,0,M_PI,0,M_PI};
	double osc5[]={osc[5],osc[5],-osc[5]+osc[4],-osc[5]+osc[4],osc[5],osc[5],-osc[5]+osc[4],-osc[5]+osc[4]};
	double minchi2=1e6;
  for(double this_dcp=-M_PI;this_dcp<=M_PI;this_dcp=this_dcp+(M_PI/steps)){
    for(double thist23=lowt23;thist23<=hight23;thist23+=(hight23-lowt23)/steps){	
			double osc2[] = {thist23,thist23,thist23,thist23,(M_PI/2)-thist23,(M_PI/2)-thist23,(M_PI/2)-thist23,(M_PI/2)-thist23};
			minchi2=1e6;
			for(int k=0;k<8;k++){ //profile octant, deltaCP=0,PI, and MH
				glbDefineParams(true_values,osc[0],osc[1],thist23,this_dcp,osc[4],osc[5]);
				glbSetDensityParams(true_values,1.0,GLB_ALL);
				glbSetOscillationParameters(true_values);
				glbSetRates();
			
				glbDefineParams(test_values,osc[0],osc[1],osc2[k],osc3[k],osc[4],osc5[k]);
				glbSetDensityParams(test_values,1.0,GLB_ALL);
				//set central values to match test_values
				glbDefineParams(central_values,osc[0],osc[1],osc2[k],osc3[k],osc[4],osc5[k]);
				glbSetCentralValues(central_values); 
			  
				if(arguments.test)
					chi2[k]=glbChiSys(test_values,GLB_ALL,GLB_ALL);
				else
					chi2[k]=glbChiNP(test_values,test_values,GLB_ALL);
					
				if(chi2[k]<minchi2){
					minchi2=chi2[k];
					minvars[0]=glbGetOscParams(test_values,GLB_THETA_13);
					minvars[1]=glbGetOscParams(test_values,GLB_THETA_23);
					minvars[2]=glbGetOscParams(test_values,GLB_DELTA_CP);
					minvars[3]=glbGetOscParams(test_values,GLB_DM_31);
				}
			}
						
			double a[]={this_dcp,thist23, minchi2, minvars[0], minvars[1], minvars[2], minvars[3]};
			AddArrayToOutput(a,7);
		}
	AddToOutputBlankline();
  }
}

void dcpvst23_mh(double osc[], double vrange[2])
{
  //Compute MH sensitivity as a function of deltaCP and t23
  //for input true values in osc, and over vrange in t23.
  double lowt23=vrange[0];
  double hight23=vrange[1];
  double steps=35;
  double minvars[4];
  double mindcp, mint23;

  glb_projection proj= glbAllocProjection();
  //keeping t12 and DM21 fixed to speed things up
  glbDefineProjection(proj, GLB_FIXED, GLB_FREE, GLB_FREE, GLB_FIXED, GLB_FIXED, GLB_FREE);
  glbSetDensityProjectionFlag(proj, GLB_FIXED, GLB_ALL);
  glbSetProjection(proj);
  
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbSetOscillationParameters(true_values);
  glbSetRates();

	double minchi2=1e6, tminchi2=1e6;
  for(double this_dcp=-M_PI;this_dcp<=M_PI;this_dcp=this_dcp+(M_PI/steps)){
    for(double thist23=lowt23;thist23<=hight23;thist23+=(hight23-lowt23)/steps){	
			double osc2[] = {thist23,(M_PI/2)-thist23};
			glbDefineParams(true_values,osc[0],osc[1],thist23,this_dcp,osc[4],osc[5]);
			glbSetDensityParams(true_values,1.0,GLB_ALL);
			glbSetOscillationParameters(true_values);
			glbSetRates();
		
			//profile without systematics in deltacp, t23 octant
			tminchi2=1e6;
			mindcp=this_dcp;
			mint23=thist23;
			for(double this_tdcp=-M_PI;this_tdcp<M_PI;this_tdcp=this_tdcp+(M_PI/36)){
				for(int tk=0;tk<2;tk++){
					glbDefineParams(test_values,osc[0],osc[1],osc2[tk],this_tdcp,osc[4],-osc[5]+osc[4]);  
					glbSetDensityParams(test_values,1.0,GLB_ALL);
					double tchi2=glbChiSys(test_values,GLB_ALL,GLB_ALL);
					if(tchi2<tminchi2){
						tminchi2=tchi2;
						mindcp=this_tdcp;
						mint23=osc2[tk];
					}
				}
			}

			glbDefineParams(test_values,osc[0],osc[1],mint23,mindcp,osc[4],-osc[5]+osc[4]);  
			glbSetDensityParams(test_values,1.0,GLB_ALL);
			glbDefineParams(central_values,osc[0],osc[1],mint23,this_dcp,osc[4],-osc[5]+osc[4]);
			glbSetCentralValues(central_values);   
			if(arguments.test)
				minchi2=glbChiSys(test_values,GLB_ALL,GLB_ALL);
			else{
				minchi2=glbChiDelta(test_values,test_values,GLB_ALL);
			}
			minvars[0]=glbGetOscParams(test_values,GLB_THETA_13);
			minvars[1]=glbGetOscParams(test_values,GLB_THETA_23);
			minvars[2]=glbGetOscParams(test_values,GLB_DELTA_CP);
			minvars[3]=glbGetOscParams(test_values,GLB_DM_31);

			double a[]={this_dcp,thist23, minchi2, minvars[0], minvars[1], minvars[2], minvars[3]};
			AddArrayToOutput(a,7);
		}
		AddToOutputBlankline();
	}
}

void dm31vst23(double osc[], double vrange[2])
{
  //Compute allowed regions in DM31 and t23
  //for input true values in osc, and over vrange in t23.
  double chi2[2];
  double lowt23=vrange[0];
  double hight23=vrange[1];
  double lowdm31=2.4e-3;
  double highdm31=2.55e-3;
  
  glb_projection projT23dm31 = glbAllocProjection();
  glbDefineProjection(projT23dm31, GLB_FREE, GLB_FREE, GLB_FIXED, GLB_FREE, GLB_FREE, GLB_FIXED);
  glbSetDensityProjectionFlag(projT23dm31, GLB_FREE, GLB_ALL);
  glbSetProjection(projT23dm31);
  
  glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbSetOscillationParameters(true_values);
  glbSetRates();
	
  for(double thisdm31=lowdm31;thisdm31<highdm31;thisdm31+=(highdm31-lowdm31)/40){
    for(double thist23=lowt23;thist23<=hight23;thist23+=(hight23-lowt23)/40){
			double osc5[]={thisdm31,-thisdm31+osc[4]};
			for(int k=0;k<2;k++){ //profile MH
				glbDefineParams(test_values,osc[0],osc[1],thist23,osc[3],osc[4],osc5[k]);
				glbSetDensityParams(test_values,1.0,GLB_ALL); 
				glbDefineParams(central_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc5[k]);
				glbSetCentralValues(central_values);  
				if(arguments.test)
					chi2[k]=glbChiSys(test_values,GLB_ALL,GLB_ALL);
				else
					chi2[k]=glbChiNP(test_values,test_values,GLB_ALL);
			}
			
			double a[]={thisdm31,thist23, chi2[0], chi2[1]};
			AddArrayToOutput(a,4);
		}
	AddToOutputBlankline();
  }
}

void t13vst23(double osc[])
{
  //Compute allowed regions in t13 and t23
  //for input true values in osc
  double chi2[2];
  double lowt23=osc[2];
  double hight23=M_PI/4;
  double lowt13=0;
  double hight13=osc[1];
  int divisions = 40;
  double tminchi2, tchi2;
  double mindcp[]={osc[3],osc[3]};
  
  glb_projection projT23T13 = glbAllocProjection();
  glbDefineProjection(projT23T13, GLB_FREE, GLB_FIXED, GLB_FIXED, GLB_FIXED, GLB_FREE, GLB_FREE);
  glbSetDensityProjectionFlag(projT23T13, GLB_FREE, GLB_ALL);
  glbSetProjection(projT23T13);
  double osc5[]={osc[5],-osc[5]+osc[4]};
  
  for(double thist13=lowt13;thist13<=hight13;thist13+=(hight13-lowt13)/divisions){
    for(double thist23=lowt23;thist23<=hight23;thist23+=(hight23-lowt23)/divisions){
			glbDefineParams(true_values,osc[0],thist13,thist23,osc[3],osc[4],osc[5]);
			glbSetDensityParams(true_values,1.0,GLB_ALL);
			glbSetOscillationParameters(true_values);
			glbSetRates();
			
			//prescan without systematics in deltacp, t23 octant
			for(int tk=0;tk<2;tk++){
				tminchi2=1e6;
				for(double this_tdcp=-M_PI;this_tdcp<M_PI;this_tdcp=this_tdcp+(M_PI/36)){
					glbDefineParams(test_values,osc[0],thist13,(M_PI/2)-thist23,this_tdcp,osc[4],osc5[tk]);  
					glbSetDensityParams(test_values,1.0,GLB_ALL);
					tchi2=glbChiSys(test_values,GLB_ALL,GLB_ALL);
					if(tchi2<tminchi2){
						tminchi2=tchi2;
						mindcp[tk]=this_tdcp;
					}
				}
			}
			for(int k=0;k<2;k++){ //profile over MH
				glbDefineParams(test_values,osc[0],thist13,(M_PI/2)-thist23,mindcp[k],osc[4],osc5[k]);  
				glbSetDensityParams(test_values,1.0,GLB_ALL);
				glbDefineParams(central_values,osc[0],thist13,thist23,osc[3],osc[4],osc5[k]);
				glbSetCentralValues(central_values);  
				if(arguments.test)
					chi2[k]=glbChiSys(test_values,GLB_ALL,GLB_ALL);
				else
					chi2[k]=glbChiNP(test_values,test_values,GLB_ALL);
			}
			double a[]={thist13,thist23, chi2[0], chi2[1]};
			AddArrayToOutput(a,4);
		}
	AddToOutputBlankline();
  }
}


