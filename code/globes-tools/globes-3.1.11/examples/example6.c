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
 * Example: Non-Standard-Interactions and user-defined priors
 * Compile with ``make example6''
 *
 * This example is similar to Chapter 4 of hep-ph/0502147
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <globes/globes.h>   /* GLoBES library */
#include "myio.h"            /* my input-output routines */

#define GLB_SIGMA_E 6        /* Index of non-standard parameter sigma_E */

/* If filename given, write to file; for empty filename write to screen */
char MYFILE1[]="test6a.dat";
char MYFILE2[]="test6b.dat";


/* Square of real number */
inline double square(double x)
{
    return x*x;
}


/***************************************************************************
 *     U S E R - D E F I N E D   P R O B A B I L I T Y   E N G I N E       *
 ***************************************************************************/

double th12;
double th13;
double th23;
double deltacp;
double sdm;
double ldm;
double sigma_E;


/***************************************************************************
 * Store oscillation parameters in internal data structures.               *
 * For more sophisticated probability engines, this would be the right     *
 * place to pre-compute the mixing matrix and parts of the Hamiltonian in  *
 * order to speed up the calls to the actual probability matrix function.  *
 ***************************************************************************/
int my_set_oscillation_parameters(glb_params p, void *user_data)
{
  th12    = glbGetOscParams(p, GLB_THETA_12);
  th13    = glbGetOscParams(p, GLB_THETA_13);
  th23    = glbGetOscParams(p, GLB_THETA_23);
  deltacp = glbGetOscParams(p, GLB_DELTA_CP);
  sdm     = glbGetOscParams(p, GLB_DM_21) * 1.0e-18;   /* Convert to GeV^2 */
  ldm     = glbGetOscParams(p, GLB_DM_31) * 1.0e-18;   /* Convert to GeV^2 */
  sigma_E = glbGetOscParams(p, GLB_SIGMA_E);

  return 0;
}


/***************************************************************************
 * Write oscillation parameters from internal data structures into p.      *
 ***************************************************************************/
int my_get_oscillation_parameters(glb_params p, void *user_data)
{
  glbSetOscParams(p, th12, GLB_THETA_12);
  glbSetOscParams(p, th13, GLB_THETA_13);
  glbSetOscParams(p, th23, GLB_THETA_23);
  glbSetOscParams(p, deltacp, GLB_DELTA_CP);
  glbSetOscParams(p, sdm*1.0e18, GLB_DM_21);  /* Convert to eV^2 */
  glbSetOscParams(p, ldm*1.0e18, GLB_DM_31);  /* Convert to eV^2 */
  glbSetOscParams(p, sigma_E, GLB_SIGMA_E);

  return 0;
}


/***************************************************************************
 * Calculate oscillation probabilities.                                    *
 * Since for our setup, only P_ee is required, all other entries of P are  *
 * set to zero for simplicity. Furthermore, we neglect matter effects and  *
 * the filter feature (parameter filter_sigma).                            *
 * The formula for P_ee is Eq. (36) from hep-ph/0502147.                   *
 ***************************************************************************
 * Parameters:                                                             *
 *   P:            The buffer where the probabilities are to be stored     *
 *   cp_sign:      +1 if probalities for neutrinos are requested, -1 for   *
 *                 anti-neutrinos.                                         *
 *   E:            The neutrino energy in GeV                              *
 *   psteps:       Number of constant density layers in the matter profile *
 *   length:       The lengths of these layers in km                       *
 *   density:      The individual densities of these layers in g/cm^3      *
 *   filter_sigma: Width of low-pass filter as given in the AEDL file      *
 ***************************************************************************/
int my_probability_matrix(double P[3][3], int cp_sign, double E, int psteps,
                          const double *length, const double *density,
                          double filter_sigma, void *user_data)
{
  int i, j;
  double L;
  double Delta21, Delta31, Delta32;
  double D21, D31, D32;
  double s13, c13, s12, c12;
  double t;
  
  /* Set all probabilities to zero initially */
  for (i=0; i < 3; i++)
    for (j=0; j < 3; j++)
      P[i][j] = 0.0;

  /* Calculate total baseline */
  L = 0.0;
  for (i=0; i < psteps; i++)
    L += length[i];
  L = GLB_KM_TO_EV(L) * 1.0e9;      /* Convert to GeV^{-1} */

  /* Compute P_ee */
  s12 = sin(th12);
  c12 = cos(th12);
  s13 = sin(th13);
  c13 = cos(th13);
  t = L / (4.0 * E);
  Delta21 = sdm * t;
  Delta31 = ldm * t;
  Delta32 = Delta31 - Delta21;
  t = M_SQRT2 * sigma_E / E;
  D21 = exp(-square( Delta21 * t ));
  D31 = exp(-square( Delta31 * t ));
  D32 = exp(-square( Delta32 * t ));
  P[0][0] = square(square(c13)) * ( 1 - 2.0*square(s12*c12)*(1 - D21*cos(2.0*Delta21)) )
               + 2.0*square(s13*c13) * ( D31*square(c12)*cos(2.0*Delta31)
                                       + D32*square(s12)*cos(2.0*Delta32) )
              + square(square(s13));

  return 0;
}


/***************************************************************************
 *                            M A I N   P R O G R A M                      *
 ***************************************************************************/

int main(int argc, char *argv[])
{ 
  double thetheta13, x, y, res;
  
  /* Define standard oscillation parameters (cf. hep-ph/0405172v5) */
  double true_theta12 = asin(sqrt(0.3));
  double true_theta13 = 0.0;
  double true_theta23 = M_PI/4;
  double true_deltacp = 0.0;
  double true_sdm = 7.9e-5;
  double true_ldm = 2.6e-3;

  /* Define one non-standard parameter sigma_E (wave packet energy spread
   * responsible for wave packet decoherence) */
  double true_sigma_E = 0.0;

  /* Initialize libglobes */
  glbInit(argv[0]);
 
  /* Register non-standard probability engine. This has to be done
   * before any calls to glbAllocParams() or glbAllocProjections() */
  glbRegisterProbabilityEngine(7,      /* Number of parameters */
                               &my_probability_matrix,
                               &my_set_oscillation_parameters,
                               &my_get_oscillation_parameters,
                               NULL);

  /* Initialize reactor experiment */
  glbInitExperiment("Reactor1.glb",&glb_experiment_list[0],&glb_num_of_exps); 
 
  /* Initialize parameter and projection vector(s) */
  glb_params true_values = glbAllocParams();
  glb_params test_values = glbAllocParams();
  glb_params input_errors = glbAllocParams();
  glb_projection th13_sigma_projection = glbAllocProjection();  

  glbDefineParams(true_values,true_theta12,true_theta13,true_theta23,
                              true_deltacp,true_sdm,true_ldm);
  glbSetOscParams(true_values,true_sigma_E, GLB_SIGMA_E);   /* Non-standard parameter */
  glbSetDensityParams(true_values,1.0,GLB_ALL);
  glbDefineParams(test_values,true_theta12,true_theta13,true_theta23,
                              true_deltacp,true_sdm,true_ldm);
  glbSetOscParams(test_values,true_sigma_E, GLB_SIGMA_E);   /* Non-standard parameter */
  glbSetDensityParams(test_values,1.0,GLB_ALL);

  /* The simulated data are computed */
  glbSetOscillationParameters(true_values);
  glbSetRates();

  /* Set starting values and input errors for all projections */  
  glbDefineParams(input_errors, 0.1*true_theta12, 0.0, 0.15*true_theta23,
                                0.0, 0.05*true_sdm, 0.05*true_ldm);  
  glbSetOscParams(input_errors, 0.0, GLB_SIGMA_E);      /* Non-standard parameter */
  glbSetDensityParams(input_errors, 0.05, GLB_ALL);
  glbSetCentralValues(true_values);
  glbSetInputErrors(input_errors);

  /* Compute chi^2 without correlations */
  InitOutput(MYFILE1,"Format: Log(10,s22th13)   sigma_E       chi^2 \n"); 
  for(x=0; x < 0.05+0.001; x+=0.005)        /* th13 loop */
    for(y=0.0; y < 0.010+0.001; y+=0.001)   /* sigame_E loop */
    {
      /* Set vector of test=fit values */
      thetheta13=asin(sqrt(x))/2.0;
      glbSetOscParams(test_values,thetheta13,GLB_THETA_13);
      glbSetOscParams(test_values,y,GLB_SIGMA_E);
     
      /* Compute Chi^2 with systematics only */
      res=glbChiSys(test_values,GLB_ALL,GLB_ALL);
  
      AddToOutput(x,y,res);
    }
  

  /* Set two-parameter projection onto s22th13 and sigma_E, marginalizing
   * over all other parameters except delta_CP and th23, which do not enter P_ee */
  glbDefineProjection(th13_sigma_projection, GLB_FREE, GLB_FIXED, GLB_FIXED,
    GLB_FIXED, GLB_FREE, GLB_FREE);
  glbSetDensityProjectionFlag(th13_sigma_projection, GLB_FIXED, GLB_ALL);
  glbSetProjectionFlag(th13_sigma_projection, GLB_FIXED, GLB_SIGMA_E); /* Non-standard parameter */
  glbSetProjection(th13_sigma_projection); 

  InitOutput(MYFILE2,"Format: Log(10,s22th13)   sigma_E       chi^2 \n"); 
  for(x=0; x < 0.05+0.001; x+=0.005)        /* th13 loop */
    for(y=0.0; y < 0.010+0.001; y+=0.001)   /* sigame_E loop */
    {
      /* Set vector of test=fit values */
      thetheta13=asin(sqrt(x))/2.0;
      glbSetOscParams(test_values,thetheta13,GLB_THETA_13);
      glbSetOscParams(test_values,y,GLB_SIGMA_E);
     
      /* Compute Chi^2 with correlations */
      res=glbChiNP(test_values,NULL,GLB_ALL);
  
      AddToOutput(x,y,res);
    }
  
  
  /* Destroy parameter and projection vector(s) */
  glbFreeParams(true_values);
  glbFreeParams(test_values); 
  glbFreeParams(input_errors); 
  glbFreeProjection(th13_sigma_projection);
    
  exit(0);
}

