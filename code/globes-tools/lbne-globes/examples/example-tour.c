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
 * Example: GLoBES tour
 * Compile with ``make example-tour''
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <globes/globes.h>   /* GLoBES library */


int main(int argc, char *argv[])
{
  /* char* MYFILE=""; */ 
  char* MYFILE="gl-tour.dat"; /* if empty, write to screen, otherwise to file name given here */
  FILE* stream;
  if(strlen(MYFILE)>0) stream=fopen(MYFILE, "w");
   else stream = stdout;

  /* Initialize libglobes */
  glbInit(argv[0]);
  

  /* Define my standard oscillation parameters */
  double theta12 = asin(sqrt(0.8))/2;
  double theta13 = asin(sqrt(0.001))/2;
  double theta23 = M_PI/4;
  double deltacp = M_PI/2;
  double sdm = 7e-5;
  double ldm = 2e-3;
 
  /* Initialize one experiment NFstandard.glb */
  glbInitExperiment("NFstandard.glb",&glb_experiment_list[0],&glb_num_of_exps); 
   
  /* Initialize a number of parameter vector(s) */
  glb_params true_values = glbAllocParams();
  glb_params fit_values = glbAllocParams();
  glb_params central_values = glbAllocParams();
  glb_params input_errors = glbAllocParams();
  glb_params minimum = glbAllocParams();
    
  /* Assign standard oscillation parameters */
  glbDefineParams(true_values,theta12,theta13,theta23,deltacp,sdm,ldm);
  glbSetDensityParams(true_values,1.0,GLB_ALL);

  /* The simulated data are computed with "true_values" */
  glbSetOscillationParameters(true_values);
  glbSetRates();
  
  /* Return some low level information */
  int i;
  fprintf(stream,"\nOscillation probabilities in vacuum: ");
  for(i=1;i<4;i++) fprintf(stream,"1->%i: %g ",i,glbVacuumProbability(1,i,+1,50,3000)); 
  fprintf(stream,"\nOscillation probabilities in matter: ");
  for(i=1;i<4;i++) fprintf(stream,"1->%i: %g ",i,glbProfileProbability(0,1,i,+1,50));
  fprintf(stream,"\n\n");    
  
  /* Set test/fit values slightly off true values at s22th13=0.0015 */
  glbCopyParams(true_values,fit_values);
  glbSetOscParams(fit_values,asin(sqrt(0.0015))/2,GLB_THETA_13); 
  
  /* Compute chi2 with systematics only for all experiments and rules */
  double chi2,chi2b,chi2sum;
  chi2 = glbChiSys(fit_values,GLB_ALL,GLB_ALL);
  fprintf(stream,"chi2 with systematics only: %g\n\n",chi2);
    
  /* What would be obtain from the appearance channels only? */
  chi2 = glbChiSys(fit_values,0,0);
  fprintf(stream,"This we would have from the CP-even appearance channel only: %g\n\n",chi2);

  /* What would be obtain from the appearance channels only? */
  chi2 = glbChiSys(fit_values,GLB_ALL,0)+
         glbChiSys(fit_values,GLB_ALL,1)+
         glbChiSys(fit_values,GLB_ALL,2)+
         glbChiSys(fit_values,GLB_ALL,3);         ;
  fprintf(stream,"The sum over all rules gives again: %g\n\n",chi2);

  /* Prepare minimizors: Set errors for external parameters: */
  /* 10% for each of the solar parameters, 5% for the matter density */
  glbDefineParams(input_errors,theta12*0.1,0,0,0,sdm*0.1,0);
  glbSetDensityParams(input_errors,0.05,GLB_ALL);
  glbSetCentralValues(true_values);
  glbSetInputErrors(input_errors);
  
  /* Let's do the some with correlations included: */
  chi2 = glbChiTheta13(fit_values,minimum,GLB_ALL); 
  fprintf(stream,"chi2 with correlations: %g \n",chi2);
  fprintf(stream,"Position of minimum: theta12,theta13,theta23,delta,sdm,ldm,rho\n");
  glbPrintParams(stream,minimum);  
  fprintf(stream,"Note that s22theta13 is unchanged/kept fixed: %g! \n\n",
   pow(sin(2*glbGetOscParams(minimum,GLB_THETA_13)),2));

  /* Check what the difference is if we keep in addition deltacp fixed. */
  /* This corresponds to projection onto theta13-deltacp-plane */
  chi2 = glbChiTheta13Delta(fit_values,minimum,GLB_ALL);
  fprintf(stream,"chi2 with correlations other than with deltacp: %g \n\n",chi2);
  
  /* Similarly: what if we only take into account correlation with deltacp? */
  /* For this, we need an arbitrary user-defined projection: */
  glb_projection myprojection = glbAllocProjection();
  glbDefineProjection(myprojection,GLB_FIXED,GLB_FIXED,GLB_FIXED,GLB_FREE,
   GLB_FIXED,GLB_FIXED);
  glbSetDensityProjectionFlag(myprojection,GLB_FREE,GLB_ALL);
  glbSetProjection(myprojection);
  chi2 = glbChiNP(fit_values,minimum,GLB_ALL);
  fprintf(stream,"chi2 with correlation only with deltacp: %g \n\n",chi2);
  glbFreeProjection(myprojection);   
 
 

  /* Now: compare with statistics chi2 only */
  glbSwitchSystematics(GLB_ALL,GLB_ALL,GLB_OFF);   
  chi2 = glbChiSys(fit_values,GLB_ALL,GLB_ALL);
  glbSwitchSystematics(GLB_ALL,GLB_ALL,GLB_ON);   
  fprintf(stream,"chi2 with statistics only: %g\n\n",chi2);
     
  /* Find position of sgn-degeneracy;  */
  glbDefineParams(input_errors,theta12*0.1,0,0,0,sdm*0.1,ldm/3);
  glbSetDensityParams(input_errors,0.05,GLB_ALL);
  glbDefineParams(central_values,theta12,theta13,theta23,deltacp,sdm,-ldm);
  glbSetDensityParams(central_values,1.0,GLB_ALL);
  glbSetCentralValues(central_values);
  glbSetInputErrors(input_errors);
  chi2=glbChiAll(central_values,minimum,GLB_ALL); 
  fprintf(stream,"chi2 at minimum: %g \n",chi2);
  fprintf(stream,"Position of minimum: theta12,theta13,theta23,delta,sdm,ldm,rho\n");
  glbPrintParams(stream,minimum);  

  /* Now: multi-experiment setup */
  /* Destroy parameter vectors, because they depend on no of exps */
  glbFreeParams(true_values);
  glbFreeParams(fit_values); 
  glbFreeParams(central_values);
  glbFreeParams(input_errors);
  glbFreeParams(minimum);

  fprintf(stream,"\nNOW: TWO-EXPERIMENT SETUP NuFact@3000km+NuFact@7500km\n\n");
  
  /* Initialize two experiments NFstandard.glb */
  glbClearExperimentList();
  glbInitExperiment("NFstandard.glb",&glb_experiment_list[0],&glb_num_of_exps); 
  glbInitExperiment("NFstandard.glb",&glb_experiment_list[0],&glb_num_of_exps); 
   
  /* Change baseline of second experiment to magic baseline */  
  double* lengths; 
  double* densities;
  glbAverageDensityProfile(7500,&lengths,&densities);
  fprintf(stream,"Magic baseline length: %g, Density: %g\n\n",lengths[0],densities[0]); 
  glbSetProfileDataInExperiment(1,1,lengths,densities);
  free(lengths);
  free(densities);
 
	/* Initialize a number of parameter vector(s) again */
  true_values = glbAllocParams();
  fit_values = glbAllocParams();
  central_values = glbAllocParams();
  input_errors = glbAllocParams();
  minimum = glbAllocParams();

  /* Assign standard oscillation parameters */
  glbDefineParams(true_values,theta12,theta13,theta23,deltacp,sdm,ldm);
  glbSetDensityParams(true_values,1.0,GLB_ALL);

  /* The simulated data are computed with "true_values" */
  glbSetOscillationParameters(true_values);
  glbSetRates();
  
  /* Set test/fit values slightly off true values at s22th13=0.0015 */
  glbCopyParams(true_values,fit_values);
  glbSetOscParams(fit_values,asin(sqrt(0.0015))/2,GLB_THETA_13); 
    
  /* Compute chi2 with systematics only for all experiments and rules */
  chi2 = glbChiSys(fit_values,GLB_ALL,GLB_ALL);
  fprintf(stream,"chi2 with systematics for all exps: %g\n",chi2);

  /* Compute chi2 with systematics only for each experiment */
  chi2 = glbChiSys(fit_values,0,GLB_ALL);
  fprintf(stream,"chi2 with systematics for 3000km: %g\n",chi2);
  chi2b = glbChiSys(fit_values,1,GLB_ALL);
  fprintf(stream,"chi2 with systematics for 7500km: %g\n",chi2b);
  fprintf(stream,"The two add again to: %g\n\n",chi2+chi2b);
   
  /* Compute chi2 with correlations for each exp. and combination */
  /* The sum of the two chi2 is not equal to the chi2 of the combination anymore, *
   * since the minimum may have a different position and the priors are only added once */
  /* Note that there are now two densities in the vectors! */
  /* Note that a minimum at a negative value of theta13 is unphysical. However, if there
     can be none at a positive value found at small enough chi2, there is none (Magic Baseline) */  
  glbDefineParams(input_errors,theta12*0.1,0,0,0,sdm*0.1,0);
  glbSetDensityParams(input_errors,0.05,GLB_ALL);
  glbSetCentralValues(true_values);
  glbSetInputErrors(input_errors);
  chi2 = glbChiTheta13(fit_values,minimum,0); 
  fprintf(stream,"chi2 with correlations for 3000km: %g \n",chi2);
  glbPrintParams(stream,minimum);  
  chi2b = glbChiTheta13(fit_values,minimum,1); 
  fprintf(stream,"\nchi2 with correlations for 7500km: %g \n",chi2b);
  glbPrintParams(stream,minimum);  
  chi2sum = glbChiTheta13(fit_values,minimum,GLB_ALL); 
  fprintf(stream,"\nchi2 with correlations for combination: %g \n",chi2sum);
  glbPrintParams(stream,minimum);  
  fprintf(stream,"\nThe sum of the two chi2s is %g, whereas the total chi2 is %g !\n\n",chi2+chi2b,chi2sum);
  
  /* Find sgn-degeneracy for 3000km experiments and test if it is still there for comb. */
  glbDefineParams(input_errors,theta12*0.1,theta13,theta23,deltacp,sdm*0.1,ldm/3);
  glbSetDensityParams(input_errors,0.05,GLB_ALL);
  glbDefineParams(central_values,theta12,theta13,theta23,deltacp,sdm,-ldm);
  glbSetDensityParams(central_values,1.0,GLB_ALL);
  glbSetInputErrors(input_errors);
  glbSetCentralValues(central_values);
  chi2=glbChiAll(central_values,minimum,0); 
  fprintf(stream,"chi2 at minimum, L=3000km: %g \n",chi2);
  glbPrintParams(stream,minimum);  
  chi2=glbChiAll(minimum,minimum,GLB_ALL); 
  fprintf(stream,"\nchi2 for combination at minimum of L=3000km: %g \n",chi2);
  glbPrintParams(stream,minimum);  
 
  /* Destroy parameter vector(s) */
  glbFreeParams(true_values);
  glbFreeParams(fit_values); 
  glbFreeParams(central_values);
  glbFreeParams(input_errors);
  glbFreeParams(minimum);

  if(strlen(MYFILE)>0) fclose(stream);

  exit(0);
}
