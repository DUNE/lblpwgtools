/* GLoBES -- General LOng Baseline Experiment Simulator
 * (C) 2002 - 2007,  The GLoBES Team
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

/* 
 * Example: Create bar charts
 * Compile with ``make example4''
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <globes/globes.h>   /* GLoBES library */
#include "myio.h"             /* my input-output routines */

/* If filenames given, write to file; for empty filenames, write to screen */
char MYFILE1[]="test4a.dat";
char MYFILE2[]="test4b.dat";
char MYFILE3[]="test4c.dat";
char MYFILE4[]="test4d.dat";

/* Global oscillation parameters */
double theta12,theta13,theta23,deltacp,sdm,ldm;

/* Simple sign-function */
double Sign(double n)
{
 if(n>=0) return +1.0;
 else return -1.0;   
}

/* Set simulated rates */
void SetOscParams(double theldm)
{
 glb_params true_values = glbAllocParams();
 glbDefineParams(true_values,theta12,theta13,theta23,deltacp,sdm,theldm);
 glbSetDensityParams(true_values,1.0,GLB_ALL);
 glbSetOscillationParameters(true_values);
 glbSetRates();
 glbFreeParams(true_values);  
}

/* Calculate chi^2 with systematics */
double CalcSystematics(double theldm,double thex)
{
  SetOscParams(theldm);
  double thetheta13=asin(sqrt(pow(10,thex)))/2;
  glb_params test_values = glbAllocParams();
  glbDefineParams(test_values,theta12,thetheta13,theta23,0.0,sdm,theldm);
  glbSetDensityParams(test_values,1.0,GLB_ALL);
  double res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
  glbFreeParams(test_values);  
  return res;
}

/* Calculate chi^2 with statistics only */
double CalcNoSystematics(double theldm,double thex)
{
 int rules=glbGetNumberOfRules(0);
 int j;
 glbSwitchSystematics(GLB_ALL,GLB_ALL,GLB_OFF);
 double res=CalcSystematics(theldm,thex);
 glbSwitchSystematics(GLB_ALL,GLB_ALL,GLB_ON);
 return res; 
}

/* Calculate chi^2 with correlations */
double CalcProjection(double theldm,double thex,glb_params start_vector)
{
  SetOscParams(theldm);

  double thetheta13=asin(sqrt(pow(10,thex)))/2;
  double thesign=Sign(glbGetOscParams(start_vector,GLB_DM_ATM));

  glb_params input_errors = glbAllocParams();
  glb_params central_values = glbAllocParams();
  glb_params minimum = glbAllocParams();

  glbDefineParams(input_errors,theta12*0.1,0,0,0,sdm*0.1,theldm/3);  
  glbSetDensityParams(input_errors,0.05,GLB_ALL);
  /* Set central values to respective +-dm31^2 to avoid falling into unwanted solution;
     Note that the error on dm31^2 should not be too small in order to avoid large
     contributions from the priors */  
  glbDefineParams(central_values,theta12,theta13,
                                   theta23,deltacp,sdm,theldm*thesign);
  glbSetDensityParams(central_values,1.0,GLB_ALL);
  glbSetCentralValues(central_values);
  glbSetInputErrors(input_errors);

  start_vector=glbSetOscParams(start_vector,thetheta13,GLB_THETA_13);
  double res=glbChiTheta13(start_vector,minimum,GLB_ALL);
  
  /* Trick for next run: Find position of minimum better by using deltacp of prior 
     calculation; note that this modifies the vectors in the main routine! */
  glbSetOscParams(start_vector,glbGetOscParams(minimum,GLB_DELTA_CP),GLB_DELTA_CP);
 
  glbFreeParams(input_errors);
  glbFreeParams(central_values);
  glbFreeParams(minimum);
  
  return res; 
}

/* Find degeneracy */
double FindDeg(glb_params deg_pos,double theldm)
{
  SetOscParams(theldm);

  glb_params input_errors = glbAllocParams();
  glb_params central_values = glbAllocParams();

  glbDefineParams(central_values,theta12,theta13,theta23,deltacp,sdm,-theldm);  
  glbSetDensityParams(central_values,1.0,GLB_ALL);
  glbDefineParams(input_errors,theta12*0.1,0,0,0,sdm*0.1,theldm/3);  
  glbSetDensityParams(input_errors,0.05,GLB_ALL);
  glbSetCentralValues(central_values);
  glbSetInputErrors(input_errors);
  double CL=glbChiAll(central_values,deg_pos,GLB_ALL); 

  glbFreeParams(input_errors);
  glbFreeParams(central_values);
  return CL;
}

int main(int argc, char *argv[])
{ 
  /* Initialize libglobes */
  glbInit(argv[0]); 

  /* Initialize experiment NFstandard.glb */
  glbInitExperiment("NFstandard.glb",&glb_experiment_list[0],&glb_num_of_exps); 

  /* Set standard oscillation parameters */
  theta12 = asin(sqrt(0.8))/2;
  theta13 = 0.0;
  theta23 = M_PI/4;
  deltacp = M_PI/2;
  sdm = 7e-5;
  ldm = 2e-3;

  glb_params original1 = glbAllocParams();
  glb_params original2 = glbAllocParams();
  glb_params degeneracy1 = glbAllocParams();
  glb_params degeneracy2 = glbAllocParams();
   
  double thetheta13,x,res1,res2;
  
  /* Compute 1st edges of bars: systematics off, fit value of deltacp=0 */
  InitOutput(MYFILE1,"Format: Log(10,s22th13)  chi^2:ldm=2e-3  chi^2:ldm=3.0e-3 \n"); 
  for(x=-7.0;x<-2.0+0.01;x=x+5.0/50)
  {
     res1=CalcNoSystematics(2.0e-3,x);
     res2=CalcNoSystematics(3.0e-3,x);
     AddToOutput(x,res1,res2);
  }
  
  /* Compute 2nd edges of bars: systematics on, fit value of deltacp=0 */
  InitOutput(MYFILE2,"Format: Log(10,s22th13)  chi^2:ldm=2e-3  chi^2:ldm=3.0e-3 \n"); 
  for(x=-7.0;x<-2.0+0.01;x=x+5.0/50)
  {
     res1=CalcSystematics(2.0e-3,x);
     res2=CalcSystematics(3.0e-3,x);
     AddToOutput(x,res1,res2);
  }
 
  /* Compute 3rd edges of bars: systematics on, projection onto s22th13 axis */
  InitOutput(MYFILE3,"Format: Log(10,s22th13)  chi^2:ldm=2e-3  chi^2:ldm=3.0e-3 \n"); 
  glbDefineParams(original1,theta12,theta13,theta23,deltacp,sdm,2.0e-3);
  glbSetDensityParams(original1,1.0,GLB_ALL);
  glbDefineParams(original2,theta12,theta13,theta23,deltacp,sdm,3.0e-3);
  glbSetDensityParams(original2,1.0,GLB_ALL);
  for(x=-7.0;x<-2.0+0.01;x=x+5.0/50)
  {
     res1=CalcProjection(2.0e-3,x,original1);
     res2=CalcProjection(3.0e-3,x,original2);
     AddToOutput(x,res1,res2);
  }

  /* Find sgn-degeneracies */
  FindDeg(degeneracy1,2.0e-3);
  FindDeg(degeneracy2,3.0e-3);
 
  /* Compute 4th edges of bars: systematics on, degeneracy, projection onto s22th13 axis */
  InitOutput(MYFILE4,"Format: Log(10,s22th13)  chi^2:ldm=2e-3  chi^2:ldm=3.0e-3 \n"); 
  for(x=-7.0;x<-2.0+0.01;x=x+5.0/30)
  {
     res1=CalcProjection(2.0e-3,x,degeneracy1);
     res2=CalcProjection(3.0e-3,x,degeneracy2);
     AddToOutput(x,res1,res2);
  }

  glbFreeParams(original1);
  glbFreeParams(original2);
  glbFreeParams(degeneracy1);
  glbFreeParams(degeneracy2);
  
  exit(0);
}
