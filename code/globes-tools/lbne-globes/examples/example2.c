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
 * Example: Projection of two- and n-dimensional manifold onto stheta-axis
 * Compile with ``make example2''
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <globes/globes.h>   /* GLoBES library */
#include "myio.h"             /* my input-output routines */

/* If filename given, write to file; for empty filename write to screen */
char MYFILE[]="test2.dat";

int main(int argc, char *argv[])
{ 
  /* Initialize libglobes */
  glbInit(argv[0]); 

  /* Initialize experiment NFstandard.glb */
  glbInitExperiment("NFstandard.glb",&glb_experiment_list[0],&glb_num_of_exps); 

  /* Intitialize output */
  InitOutput(MYFILE,"Format: Log(10,s22th13)   chi^2 one param   chi^2 all params \n"); 

  /* Define standard oscillation parameters */
  double theta12 = asin(sqrt(0.8))/2;
  double theta13 = asin(sqrt(0.001))/2;
  double theta23 = M_PI/4;
  double deltacp = M_PI/2;
  double sdm = 7e-5;
  double ldm = 2e-3;
  
  /* Initialize parameter and projection vector(s) */
  glb_params true_values = glbAllocParams();
  glb_params test_values = glbAllocParams();
  glb_params input_errors = glbAllocParams();
  glb_projection theta13_projection = glbAllocProjection();  

  glbDefineParams(true_values,theta12,theta13,theta23,deltacp,sdm,ldm);
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbDefineParams(test_values,theta12,theta13,theta23,deltacp,sdm,ldm);  
  glbSetDensityParams(test_values,1.0,GLB_ALL);

  /* The simulated data are computed */
  glbSetOscillationParameters(true_values);
  glbSetRates();

  /* Set starting values and input errors for all projections */  
  glbDefineParams(input_errors,theta12*0.1,0,0,0,sdm*0.1,0);  
  glbSetDensityParams(input_errors,0.05,GLB_ALL);
  glbSetCentralValues(true_values);
  glbSetInputErrors(input_errors);

  /* Set two-parameter projection onto s22th13-axis: only deltacp free! */
  glbDefineProjection(theta13_projection,GLB_FIXED,GLB_FIXED,GLB_FIXED,
    GLB_FREE,GLB_FIXED,GLB_FIXED);
  glbSetDensityProjectionFlag(theta13_projection, GLB_FIXED, GLB_ALL);
  glbSetProjection(theta13_projection); 

  /* Iteration over all values to be computed */
  double thetheta13,x,res1,res2;    
  for(x=-4;x<-2.0+0.001;x=x+2.0/50)
  {
      /* Set vector of test=fit values */
      thetheta13=asin(sqrt(pow(10,x)))/2;
      glbSetOscParams(test_values,thetheta13,GLB_THETA_13);
     
      /* Guess fit value for deltacp in order to safely find minimum */
      glbSetOscParams(test_values,200.0/2*(x+4)*M_PI/180,GLB_DELTA_CP);
 
      /* Compute Chi^2 for two-parameter correlation: minimize over deltacp only */
      res1=glbChiNP(test_values,NULL,GLB_ALL);
      
      /* Compute Chi^2 for full correlation: minimize over all but theta13 */
      res2=glbChiTheta13(test_values,NULL,GLB_ALL);
      
      AddToOutput(x,res1,res2);
  }
  
  /* Destroy parameter and projection vector(s) */
  glbFreeParams(true_values);
  glbFreeParams(test_values); 
  glbFreeParams(input_errors); 
  glbFreeProjection(theta13_projection);
    
  exit(0);
}
